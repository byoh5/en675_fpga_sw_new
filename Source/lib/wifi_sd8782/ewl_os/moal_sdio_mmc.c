
#include <ewl_os.h>
#include "sdio_func.h"
#include "moal_main.h"
#include "sdio.h"
#include "moal_sdio.h"
#include "moal_util.h"
#include "moal_sdio.h"
#include <lwip/pbuf.h>


/** Device ID for SD8782 */
#define SD_DEVICE_ID_8782   (0x9121)

/** WLAN IDs */
static const struct sdio_device_id wlan_ids[] = {
    {SDIO_DEVICE(MARVELL_VENDOR_ID, SD_DEVICE_ID_8782)},
    {},
};

static int
woal_sdio_probe(struct sdio_func *func, const struct sdio_device_id *id);

static struct sdio_driver wlan_sdio = {
  .name = "wlan_sdio",
  .id_table = wlan_ids,
  .probe = woal_sdio_probe,
};


/********************************************************
		Local Functions
********************************************************/

/** 
 *  @brief This function handles the interrupt.
 *  
 *  @param func	   A pointer to the sdio_func structure
 *  @return 	   None
 */
static void
woal_sdio_interrupt(struct sdio_func *func)
{
    moal_handle *handle;
    struct sdio_mmc_card *card;

    ENTER();

    card = sdio_get_drvdata(func);
    if (!card || !card->handle) {
    PRINTM(MINFO,
           "sdio_mmc_interrupt(func = %p) card or handle is NULL, card=%p\n",
           func, card);
    LEAVE();
    return;
    }
    handle = card->handle;

    PRINTM(MINFO, "*** IN SDIO IRQ ***\n");
    woal_interrupt(handle);
    LEAVE();
}

/** 
 *  @brief This function registers the device
 *  
 *  @param handle  A pointer to moal_handle structure
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_register_dev(moal_handle * handle)
{
	int ret = MLAN_STATUS_SUCCESS;
	struct sdio_mmc_card *card = handle->card;
	struct sdio_func *func;
	
	ENTER();
	
	func = card->func;

	sdio_claim_host(func);
	/* Request the SDIO IRQ */
	ret = sdio_claim_irq(func, woal_sdio_interrupt);
	if (ret) {
		PRINTM(MFATAL, "sdio_claim_irq failed: ret=%d\n", ret);
		goto release_host;
	}
	
	/* Set block size */
	ret = sdio_set_block_size(card->func, MLAN_SDIO_BLOCK_SIZE);
	if (ret) {
		PRINTM(MERROR, "sdio_set_block_seize(): cannot set SDIO block size\n");
		ret = MLAN_STATUS_FAILURE;
		goto release_irq;
	}
	
	sdio_release_host(func);
	sdio_set_drvdata(func, card); //func->priv_data point to card
	
	LEAVE();
	return MLAN_STATUS_SUCCESS;
	
release_irq:
	sdio_release_irq(func);

release_host:
	sdio_release_host(func);
	handle->card = NULL;
	
	LEAVE();
	
	return MLAN_STATUS_FAILURE;
}

/**  @brief This function handles client driver probe.
 *
 * This function probes an mlan device and registers it. It allocates
 * the card structure, enables SDIO function number and initiates the
 * device registration and initialization procedure by adding a logical
 * interface.
 *
 *  @param func	   A pointer to sdio_func structure.
 *  @param id	   A pointer to sdio_device_id structure.
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE/error code
 */
static int
woal_sdio_probe(struct sdio_func *func, const struct sdio_device_id *id)
{
	int ret = MLAN_STATUS_SUCCESS;
	struct sdio_mmc_card *card = NULL;
	u8 cs;

    MLAN_UNUSED_ARG(id);			
	
	ENTER();
	
	PRINTM(MINFO, "vendor=0x%4.04X device=0x%4.04X class=%d function=%d\n",
	       func->vendor, func->device, func->class, func->num);

	card = (struct sdio_mmc_card*)ewl_os_malloc(sizeof(struct sdio_mmc_card));
	if (!card) {
		PRINTM(MERROR,"Failed to allocate memory in probe function!\n");
		LEAVE();
		return -1;
	}
	
	card->func = func;

	sdio_claim_host(func);
	ret = sdio_enable_func(func);
	sdio_release_host(func);
	
	cs=sdio_readb(func,0x30, &ret);
    
	if (ret) {
		ewl_os_free(card);
		PRINTM(MFATAL, "sdio_enable_func() failed: ret=%d\n", ret);
		LEAVE();
		return -1;
	}
		
	woal_add_card(card);
	
	LEAVE();
	return ret;
}



/********************************************************
		Global Functions
********************************************************/

/** 
 *  @brief This function writes data into card register
 *
 *  @param handle   A Pointer to the moal_handle structure
 *  @param reg      Register offset
 *  @param data     Value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_write_reg(moal_handle * handle, t_u32 reg, t_u32 data)
{
    mlan_status ret = MLAN_STATUS_FAILURE;
    sdio_writeb(((struct sdio_mmc_card *) handle->card)->func, (t_u8) data, reg,
                (int *) &ret);
    return ret;
}



/** 
 *  @brief This function reads data from card register
 *
 *  @param handle   A Pointer to the moal_handle structure
 *  @param reg      Register offset
 *  @param data     Value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_read_reg(moal_handle * handle, t_u32 reg, t_u32 * data)
{
	mlan_status ret = MLAN_STATUS_FAILURE;
	t_u8 val;
	val =
	sdio_readb(((struct sdio_mmc_card *) handle->card)->func, reg,
	             (int *) &ret);
	*data = val;
	
	return ret;
}


/**
 *  @brief This function writes multiple bytes into card memory
 *
 *  @param handle   A Pointer to the moal_handle structure
 *  @param pmbuf	Pointer to mlan_buffer structure
 *  @param port		Port
 *  @param timeout 	Time out value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */  

mlan_status
woal_write_data_sync(moal_handle * handle, mlan_buffer * pmbuf, t_u32 port,
                     t_u32 timeout)
{
    struct pbuf *q_buf=NULL;
    mlan_status ret = MLAN_STATUS_FAILURE;
    t_u8 *buffer = (t_u8 *) (pmbuf->pbuf + pmbuf->data_offset);
    t_u8 blkmode = (port & MLAN_SDIO_BYTE_MODE_MASK) ? BYTE_MODE : BLOCK_MODE;
    t_u32 blksz = (blkmode == BLOCK_MODE) ? MLAN_SDIO_BLOCK_SIZE : 1;
    t_u32 blkcnt =
        (blkmode ==
         BLOCK_MODE) ? (pmbuf->data_len /
                        MLAN_SDIO_BLOCK_SIZE) : pmbuf->data_len;
                        	
    t_u32 ioport = (port & MLAN_SDIO_IO_PORT_MASK);

    MLAN_UNUSED_ARG(timeout);
    
    if(pmbuf->buf_type == MLAN_BUF_TYPE_CMD)
    {
        if (!sdio_writesb
            (((struct sdio_mmc_card *) handle->card)->func, ioport, buffer,
            blkcnt * blksz))         
            ret = MLAN_STATUS_SUCCESS;

    }
    else
    {
        q_buf = pmbuf->pdesc;

        if(q_buf == NULL)
        {
            if (!sdio_writesb
            (((struct sdio_mmc_card *) handle->card)->func, ioport, buffer,
            blkcnt * blksz))         
            ret = MLAN_STATUS_SUCCESS;
        }
        else
        {
            pbuf_header(q_buf,-pmbuf->data_offset);
            if (!sdio_write_pbuf
                (((struct sdio_mmc_card *) handle->card)->func, ioport, q_buf,
                blkcnt * blksz))         
                ret = MLAN_STATUS_SUCCESS;  
        }
    }
//fail:    
    return ret;
}


/**
 *  @brief This function reads multiple bytes from card memory
 *
 *  @param handle   A Pointer to the moal_handle structure
 *  @param pmbuf	Pointer to mlan_buffer structure
 *  @param port		Port
 *  @param timeout 	Time out value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_read_data_sync(moal_handle * handle, mlan_buffer * pmbuf, t_u32 port,
                    t_u32 timeout)
{
    mlan_status ret = MLAN_STATUS_FAILURE;
    t_u8 *buffer = (t_u8 *) (pmbuf->pbuf + pmbuf->data_offset);
    t_u8 blkmode = (port & MLAN_SDIO_BYTE_MODE_MASK) ? BYTE_MODE : BLOCK_MODE;
    t_u32 blksz = (blkmode == BLOCK_MODE) ? MLAN_SDIO_BLOCK_SIZE : 1;
    t_u32 blkcnt =
        (blkmode ==
         BLOCK_MODE) ? (pmbuf->data_len /
                        MLAN_SDIO_BLOCK_SIZE) : pmbuf->data_len;
    t_u32 ioport = (port & MLAN_SDIO_IO_PORT_MASK);

    MLAN_UNUSED_ARG(timeout);
//	ENTER();

//	PRINTM(MMSG,"pmbuf->buf_type %d\n",pmbuf->buf_type);

	if(pmbuf->buf_type == MLAN_BUF_TYPE_CMD)
	{
        if (!sdio_readsb
            (((struct sdio_mmc_card *) handle->card)->func, buffer, ioport,
            blkcnt * blksz))
        ret = MLAN_STATUS_SUCCESS;
    }
    else
    {
        struct pbuf *p = pmbuf->pdesc;
        pbuf_header(p, -pmbuf->data_offset); /*payload += pmbuf->data_offset, pbuf->tot_len -= data_offset */    
        if (!sdio_read_pbuf
            (((struct sdio_mmc_card *) handle->card)->func, p, ioport,
            blkcnt * blksz))
        {
            pbuf_header(p, pmbuf->data_offset);
            ret = MLAN_STATUS_SUCCESS;
        }
    }

//	LEAVE();
    return ret;
}


void woal_sdio_register(void)
{    
	ENTER();	
	sdio_register_driver(&wlan_sdio);
	LEAVE();
}


