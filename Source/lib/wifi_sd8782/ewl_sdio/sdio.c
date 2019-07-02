#include "dev.h"
#include "msg.h"
#include "sdio_func.h"
#include "sdio.h"
#include <netif/etharp.h>
#include <ewl_os.h>

#define ENX_SDIO_CMD_DEBUG
//#define ENX_SDIO_DAT_DEBUG
#define ENX_SDIO_ERR_DEBUG

#ifdef ENX_SDIO_CMD_DEBUG
	#define gprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
	#define SDIO_CMD_LOG_START	{	printf("(%04d)%s : START\n", __LINE__, __func__);	}
	#define SDIO_CMD_LOG_END	{	printf("(%04d)%s : E N D\n", __LINE__, __func__);	}
#else
	#define gprintf(fmt, args...) do {} while(0);
	#define SDIO_CMD_LOG_START	{ }
	#define SDIO_CMD_LOG_END	{ }
#endif

#ifdef ENX_SDIO_DAT_DEBUG
	#define tprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
	#define SDIO_DAT_LOG_START	{	printf("(%04d)%s : START\n", __LINE__, __func__);	}
	#define SDIO_DAT_LOG_END	{	printf("(%04d)%s : E N D\n", __LINE__, __func__);	}
#else
	#define tprintf(fmt, args...) do {} while(0);
	#define SDIO_DAT_LOG_START	{ }
	#define SDIO_DAT_LOG_END	{ }
#endif

#ifdef ENX_SDIO_ERR_DEBUG
	#define eprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define eprintf(fmt, args...) do {} while(0);
#endif

#ifdef ENX_DEBUG
#define SDIO_DEBUG
#endif

#ifdef SDIO_DEBUG
#define dev_dbg printf
#else
#define dev_dbg
#endif

#define enter() //printf("enter %s \n",__func__);
#define leave() //printf("leave %s \n",__func__);
//-------------------------------------------------------------------------------------------------
// Status Mask
#define RCA_RCA_MASK		0xffff0000


/* SD commands                           type  argument     response */
  /* class 0 */
/* This is basically the same command as for MMC with some quirks. */
#define SD_SEND_RELATIVE_ADDR     3   	 /* bcr                     R6  */
#define SD_SEND_IF_COND           CMD8   /* bcr  [11:0] See below   R7  */

/* SDIO commands                    type  argument     response */
#define SD_IO_SEND_OP_COND          5 /* bcr  [23:0] OCR         R4  */
#define SD_IO_RW_DIRECT            52 /* ac   [31:0] See below   R5  */
#define SD_IO_RW_EXTENDED          53 /* adtc [31:0] See below   R5  */

/*
 * SD_IO_RW_DIRECT argument format:
 *
 *      [31] R/W flag
 *      [30:28] Function number
 *      [27] RAW flag
 *      [25:9] Register address
 *      [7:0] Data
 */

/*
 * SD_IO_RW_EXTENDED argument format:
 *
 *      [31] R/W flag
 *      [30:28] Function number
 *      [27] Block mode
 *      [26] Increment address
 *      [25:9] Register address
 *      [8:0] Byte/block count
 */

/*
 * IO_SEND_OP_COND argument format (response)
 *  
 *  [31]    C Set to 1 if Card is ready to operate after initialization
 *  [30:28] Number of I/O functions
 *  [27]    Memory Present
 *  [26:24] Stuff Bits 
 *  [23:0]  I/O OCR operation Conditions Register
*/
 

#define R4_MEMORY_PRESENT (1 << 27)
/*Set to 1 if Card is ready to operate after initialization */
#define R4_CARD_READY     (1 << 31) 

/*
  SDIO status in R5
  Type
        e : error bit
        s : status bit
        r : detected and set for the actual command response
        x : detected and set during command execution. the host must poll
            the card by sending status command in order to read these bits.
  Clear condition
        a : according to the card state
        b : always related to the previous command. Reception of
            a valid command will clear it (with a delay of one command)
        c : clear by read
 */

#define R5_COM_CRC_ERROR        (1 << 15)       /* er, b */
#define R5_ILLEGAL_COMMAND      (1 << 14)       /* er, b */
#define R5_ERROR                (1 << 11)       /* erx, c */
#define R5_FUNCTION_NUMBER      (1 << 9)        /* er, c */
#define R5_OUT_OF_RANGE         (1 << 8)        /* er, c */
#define R5_STATUS(x)            (x & 0xCB00)
#define R5_IO_CURRENT_STATE(x)  ((x & 0x3000) >> 12) /* s, b */

/*
 * Card Common Control Registers (CCCR)
 */

#define SDIO_CCCR_CCCR          0x00

#define  SDIO_CCCR_REV_1_00     0       /* CCCR/FBR Version 1.00 */
#define  SDIO_CCCR_REV_1_10     1       /* CCCR/FBR Version 1.10 */
#define  SDIO_CCCR_REV_1_20     2       /* CCCR/FBR Version 1.20 */

#define  SDIO_SDIO_REV_1_00     0       /* SDIO Spec Version 1.00 */
#define  SDIO_SDIO_REV_1_10     1       /* SDIO Spec Version 1.10 */
#define  SDIO_SDIO_REV_1_20     2       /* SDIO Spec Version 1.20 */
#define  SDIO_SDIO_REV_2_00     3       /* SDIO Spec Version 2.00 */

#define SDIO_CCCR_SD            0x01

#define  SDIO_SD_REV_1_01       0       /* SD Physical Spec Version 1.01 */
#define  SDIO_SD_REV_1_10       1       /* SD Physical Spec Version 1.10 */
#define  SDIO_SD_REV_2_00       2       /* SD Physical Spec Version 2.00 */

#define SDIO_CCCR_IOEx          0x02
#define SDIO_CCCR_IORx          0x03

#define SDIO_CCCR_IENx          0x04    /* Function/Master Interrupt Enable */
#define SDIO_CCCR_INTx          0x05    /* Function Interrupt Pending */

#define SDIO_CCCR_ABORT         0x06    /* function abort/card reset */

#define SDIO_CCCR_IF            0x07    /* bus interface controls */

#define  SDIO_BUS_WIDTH_1BIT    0x00
#define  SDIO_BUS_WIDTH_4BIT    0x02
#define  SDIO_BUS_ECSI          0x20    /* Enable continuous SPI interrupt */
#define  SDIO_BUS_SCSI          0x40    /* Support continuous SPI interrupt */

#define  SDIO_BUS_ASYNC_INT     0x20

#define  SDIO_BUS_CD_DISABLE     0x80   /* disable pull-up on DAT3 (pin 1) */

#define SDIO_CCCR_CAPS          0x08

#define  SDIO_CCCR_CAP_SDC      0x01    /* can do CMD52 while data transfer */
#define  SDIO_CCCR_CAP_SMB      0x02    /* can do multi-block xfers (CMD53) */
#define  SDIO_CCCR_CAP_SRW      0x04    /* supports read-wait protocol */
#define  SDIO_CCCR_CAP_SBS      0x08    /* supports suspend/resume */
#define  SDIO_CCCR_CAP_S4MI     0x10    /* interrupt during 4-bit CMD53 */
#define  SDIO_CCCR_CAP_E4MI     0x20    /* enable ints during 4-bit CMD53 */
#define  SDIO_CCCR_CAP_LSC      0x40    /* low speed card */
#define  SDIO_CCCR_CAP_4BLS     0x80    /* 4 bit low speed card */

#define SDIO_CCCR_CIS           0x09    /* common CIS pointer (3 bytes) */

/* Following 4 regs are valid only if SBS is set */
#define SDIO_CCCR_SUSPEND       0x0c
#define SDIO_CCCR_SELx          0x0d
#define SDIO_CCCR_EXECx         0x0e
#define SDIO_CCCR_READYx        0x0f

#define SDIO_CCCR_BLKSIZE       0x10

#define SDIO_CCCR_POWER         0x12

#define  SDIO_POWER_SMPC        0x01    /* Supports Master Power Control */
#define  SDIO_POWER_EMPC        0x02    /* Enable Master Power Control */

#define SDIO_CCCR_SPEED         0x13

#define  SDIO_SPEED_SHS         0x01    /* Supports High-Speed mode */
#define  SDIO_SPEED_EHS         0x02    /* Enable High-Speed mode */

/*
 * Function Basic Registers (FBR)
 */

#define SDIO_FBR_BASE(f)        ((f) * 0x100) /* base of function f's FBRs */

#define SDIO_FBR_STD_IF         0x00

#define  SDIO_FBR_SUPPORTS_CSA  0x40    /* supports Code Storage Area */
#define  SDIO_FBR_ENABLE_CSA    0x80    /* enable Code Storage Area */

#define SDIO_FBR_STD_IF_EXT     0x01

#define SDIO_FBR_POWER          0x02

#define  SDIO_FBR_POWER_SPS     0x01    /* Supports Power Selection */
#define  SDIO_FBR_POWER_EPS     0x02    /* Enable (low) Power Selection */

#define SDIO_FBR_CIS            0x09    /* CIS pointer (3 bytes) */


#define SDIO_FBR_CSA            0x0C    /* CSA pointer (3 bytes) */

#define SDIO_FBR_CSA_DATA       0x0F

#define SDIO_FBR_BLKSIZE        0x10    /* block size (2 bytes) */

#define min(a, b)  (((a) < (b)) ? (a) : (b))

//sdio device global variable
struct sdio_func gsdio_func[7];
sdio_irq_handler_t *sdio_irq_handler[7] = {NULL,};
struct sdio_driver *gsdio_device = NULL;
static u32 bsdio_highspeed = 0;
/**
 * sdio_claim_host - exclusively claim a bus for a certain SDIO function
 * @func->num: SDIO function that will be accessed
 * 
 * Claim a bus for a set of operations. The SDIO function given
 * is used to figure out which bus is relevant.
 */
void sdio_claim_host(struct sdio_func *func)
{
    func->num = func->num;
}


/**
 * sdio_release_host - release a bus for a certain SDIO function
 * @func: SDIO function that was accessed
 * 
 * Release a bus, allowing others to claim the bus for their
 * operations.
 */
void sdio_release_host(struct sdio_func *func)
{
    //release lock
    func->num = func->num;
}

/*
  sdio_enable_func - enables a SDIO function for usage
  @func->num     sdio func->numuction number (1~7)
  @return 0 success, 1 fail
*/
int sdio_enable_func(struct sdio_func *func)
{
	int ret;
	BYTE reg;
	UINT timeout = 0xffffffff;

	enter();
	ret = SdioWfRegRead(&reg, 0, SDIO_CCCR_IOEx);
	if(ret)
		goto err;

	reg |= 1 << func->num;
	ret = SdioWfRegWrite(reg, 0, SDIO_CCCR_IOEx);
	if(ret)
		goto err;

	while (timeout--) 
	{
		ret = SdioWfRegRead(&reg, 0, SDIO_CCCR_IORx);
		if(ret)
			goto err;

		if(reg & (1 << func->num))
		{
			dev_dbg("SDIO: Enabled device reg %x, fn %d, timeout %x\n", reg, func->num, timeout);
			break;
		}

		ret = 1;
	}
	leave();
	return 0;

err:
	leave();
	dev_dbg("SDIO: Failed to enable device %d\n", func->num);
	return ret;
}

/**
 * sdio_disable_func - disable a SDIO function
 * @func->num: SDIO function to disable
 *
 * Powers down and deactivates a SDIO function. Register access
 * to this function will fail until the function is reenabled.
 */
int sdio_disable_func(struct sdio_func *func)
{
	int ret;
	unsigned char reg;
	enter();
	dev_dbg("SDIO: Disabling device %s...\n", func->num);

	ret = SdioWfRegRead(&reg, func->num, SDIO_CCCR_IOEx);
	if(ret)
		goto err;

	reg &= ~(1 << func->num);

	ret = SdioWfRegWrite(reg, func->num, SDIO_CCCR_IOEx);
	if(ret)
		goto err;

	dev_dbg("SDIO: Disabled device %s\n", func->num);
	leave();
	return 0;

err:
	leave();
	dev_dbg("SDIO: Failed to disable device %s\n", func->num);
	return 1;
}

 /**
 * sdio_set_block_size - set the block size of an SDIO function
 * @func: SDIO function to change
 * @blksz: new block size or 0 to use the default.
 *
 * The default block size is the largest supported by both the function
 * and the host, with a maximum of 512 to ensure that arbitrarily sized
 * data transfer use the optimal (least) number of commands.
 *
 * A driver may call this to override the default block size set by the
 * core. This can be used to set a block size greater than the maximum
 * that reported by the card; it is the driver's responsibility to ensure
 * it uses a value that the card supports.
 *
 * Returns 0 on success, -EINVAL if the host does not support the
 * requested block size, or -EIO (etc.) if one of the resultant FBR block
 * size register writes failed.
 *
 */
int sdio_set_block_size(struct sdio_func *func, u32 blksz)
{
	int ret;
	BYTE reg;   
	enter();

	func->blksz = blksz;

	ret = SdioWfRegWrite(blksz & 0xff, 0, SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE);
	SdioWfRegRead(&reg, 0, SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE);
	if(ret)
	{
		leave();
		return ret;
	}

	ret = SdioWfRegWrite((blksz >> 8) & 0xff, 0, SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE+1);
	SdioWfRegRead(&reg, 0, SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE+1);
	if(ret)
	{
		leave();
		return ret;
	}

	leave();
	return 0;
}

/*
 * Calculate the maximum byte mode transfer size
 */
static inline unsigned int sdio_max_byte_size(struct sdio_func *func)
{
	return min(func->blksz, 512); /* maximum size for byte mode */
}

 /**
 * sdio_claim_irq - claim the IRQ for a SDIO function
 * @func: SDIO function
 * @handler: IRQ handler callback
 *
 * Claim and activate the IRQ for the given SDIO function. The provided
 * handler will be called when that IRQ is asserted.  The host is always
 * claimed already when the handler is called so the handler must not
 * call sdio_claim_host() nor sdio_release_host().
 */
int sdio_claim_irq(struct sdio_func *func, sdio_irq_handler_t *handler)
{
    int ret;
    unsigned char reg;
    
    enter();
    dev_dbg("SDIO: Enabling IRQ for %d...\n", func->num);
    
    ret = SdioWfRegRead(&reg, 0, SDIO_CCCR_IENx);
    if (ret)
    {
        leave();
        return ret;   
    }    
    reg |= 1 << func->num;
    reg |= 1; /* Master interrupt enable */
    ret = SdioWfRegWrite(reg,0,SDIO_CCCR_IENx);

    if (ret)
    {
        leave();
        return ret;
    }
    
    ret = SdioWfRegRead(&reg, 0, SDIO_CCCR_IENx);
    if (ret)
    {
        leave();
        return ret;   
    }
    sdio_irq_handler[func->num-1] = handler;

    leave();
    return ret;
}
int sdio_irq = 0;

//int wireless_recv_event(void);
//-------------------------------------------------------------------------------------------------
// Irq from external chip (By pulling down DAT[1] pin)
extern int init_boot;

void disable_ext_sdio_irq(void)
{
	enter();
	Sdio1DiCip();
	leave();
}

void sd_irq_handler(void *dummy)
{
	enter();
	SDIO1_CHIP_IRQ_CLR = 1;
	leave();
	UNUSED(dummy);
}

void enabled_ext_sdio_irq(void)
{
    enter();
    Sdio1EiCip();
    leave();
}

/*
    handle sdio irq       
*/
extern void wifiPollGiveFromISR(void);
void ewl_sdio_ext_irq_handler(void *dummy)
{
    enter();
    if(!sdio_irq)
    {        
        sdio_irq = 1;
		wifiPollGiveFromISR();
        Sdio1DiCip(); //disable sdio ext irq
    }
    leave();
    UNUSED(dummy);
}

void sdio_fun_irq(void)
{
    int ret;
    unsigned char pending;

    //if(sdio_irq)
    {    
        ret = SdioWfRegRead(&pending, 0, SDIO_CCCR_INTx);
        if(pending&0x02)
        {
            sdio_irq_handler[0](&gsdio_func[0]);
            /* send work queue */
        }
		SDIO1_CHIP_IRQ_CLR = 1;
        //sdio_irq = 0;
        Sdio1EiCip(); /* enable irq */
    }
    //dev_dbg("Leave : %s \n", __func__);
}

 /**
  * sdio_release_irq - release the IRQ for a SDIO function
  * @func: SDIO function
  *
  * Disable and release the IRQ for the given SDIO function.
*/
int sdio_release_irq(struct sdio_func *func)
{
    int ret;
    unsigned char reg;
    enter();
    dev_dbg("SDIO: Disabling IRQ for %s...\n", func->num);
    
    sdio_irq_handler[func->num-1] = NULL;
    
    ret = SdioWfRegRead(&reg, func->num, SDIO_CCCR_IENx);
    if (ret)
    {
        leave();
        return ret;
    }
    
    reg &= ~(1 << func->num);
    
    /* Disable master interrupt with the last function interrupt */
    if (!(reg & 0xFE))
        reg = 0;
    
    ret = SdioWfRegWrite(reg,func->num,SDIO_CCCR_IENx);
    if (ret)
    {
        leave();
        return ret;
    }
    leave();
    return 0;
}



/**
 * sdio_readb - read a single byte from a SDIO function
 * @func->num  : SDIO function to access
 * @addr: address to read
 * @err_ret: optional status value from transfer
 * 
 * Reads a single byte from the address space of a given SDIO
 * function. If there is a problem reading the address, 0xff
 * is returned and @err_ret will contain the error code.
 */
unsigned char sdio_readb(struct sdio_func *func, unsigned int addr, int *err_ret)
{
    int ret;
    BYTE val;
    enter();
    if (err_ret)
    *err_ret = 0;
    ret = SdioWfRegRead(&val, func->num, addr);
    if (ret)
    {
        if (err_ret)
            *err_ret = ret;
        leave();    
        return 0xFF;
    }
    leave();
    return val;
}


/**
 *      sdio_writeb - write a single byte to a SDIO function
 *      @func: SDIO function to access
 *      @b: byte to write
 *      @addr: address to write to
 *      @err_ret: optional status value from transfer
 *
 *      Writes a single byte to the address space of a given SDIO
 *      function. @err_ret will contain the status of the actual
 *      transfer.
 */
 
void sdio_writeb(struct sdio_func *func, u8 b, unsigned int addr, int *err_ret)
{
    int ret;
    enter();
    ret = SdioWfRegWrite(b,func->num,addr);
    if (err_ret)
        *err_ret = ret;
    leave();
}



static int 
sdio_enable_wide(void)
{
    int ret;
    u8 ctrl;
    
    enter();
    ret = SdioWfRegRead(&ctrl, 0, SDIO_CCCR_IF);
    
    if (ret)
    {
        leave();
        return ret;
    }
    
    ctrl |= SDIO_BUS_WIDTH_4BIT;
    
    ret = SdioWfRegWrite(ctrl, 0, SDIO_CCCR_IF);
    if (ret)
    {
        leave();
        return ret;
    }
	leave();
    return ret;
}


/**
 * Test if the card supports high-speed mode and, if so, switch to it.
 */
static int 
sdio_enable_hs(void)
{
	int ret;
	u8 speed;
	enter();

	if (!bsdio_highspeed)
	{
		ewl_os_printf("%s(%d) : case1\n", __func__, __LINE__);
		SdioWfSetNormalSpeed();
		leave();
		return 0;
	}

	ret = SdioWfRegRead(&speed, 0, SDIO_CCCR_SPEED);
	if (ret)
	{
		ewl_os_printf("%s(%d) : case2\n", __func__, __LINE__);
		SdioWfSetNormalSpeed();
		leave();
		return ret;
	}

	speed |= SDIO_SPEED_EHS;

	ret = SdioWfRegWrite(speed, 0, SDIO_CCCR_SPEED);
	if (ret)
	{
		ewl_os_printf("%s(%d) : case3\n", __func__, __LINE__);
		SdioWfSetNormalSpeed();
		leave();
		return ret;
	}

	SdioWfSetHighSpeed();

	leave();
	return 0;
}



/*

example ocr valaue 3f8080 for i.mx51

*/

int mmc_send_if_cond(u32 ocr)
{
    UINT    nErr;
    UINT    nResp;
    UINT    arg;
    u8 result_pattern;
    u8 test_pattern = 0xAA;
    
    
    /*
     * To support SD 2.0 cards, we must always invoke SD_SEND_IF_COND
     * before SD_APP_OP_COND. This command will harmlessly fail for
     * SD 1.0 cards.
     */
    arg = ((ocr & 0xFF8000) != 0) << 8 | test_pattern;
    
    nErr = Sdio1Cmd( 8, arg, 1, 0, 0, 0);
    nResp = SDIO1_RESP0;
    
    if(nErr) return nErr;
    
    result_pattern = nResp & 0xFF;
    if (result_pattern != test_pattern)
        return -1;
    return 0;
}

/**
 *   The I/O aware host sends CMD5. If the card responds with response R4 within
 *   the timeout value of Ncr as defined in the SD Physical Specification. the
 *   host determines the card's configuration based on the data contained within
 *   the R4
 *   NCR : 2~64 clock cycles  
 *   if sdio clk is 24MHz, Ncr is 64/(24*10^6), 3 usec
 *
 * @ocr  : input ocr reg value for CMD5
 * @rocr : output ocr for CMD5
 * @return 0 OK, else failed 
*/
int mmc_send_io_op_cond(u32 ocr, u32 *rocr)
{
    int i;

    /*After reset, io device should be reponsed in 3 usec */
    for(i=10; i; i--)
    {
        Sdio1Cmd( 5, ocr, 1, 0, 0, 0);
        *rocr = SDIO1_RESP0;
        ewl_os_wait_us(100);
        if(*rocr & R4_CARD_READY) return 0;
    }
    dev_dbg("Card Power up status bit failed \n");
    return 1;
}

int mmc_send_relative_addr(u32 *rca)
{
    UINT    nResp;
    UINT    nErr;
    nErr = Sdio1Cmd( 3, 0, 1, 0, 0, 0); 
    nResp = SDIO1_RESP0;
    *rca = (nResp & RCA_RCA_MASK);
    
    return nErr;
} 

int mmc_select_card(u32 rca)
{
    UINT    nErr;  
    UINT    nResp;
    nErr = Sdio1Cmd( 7, (UINT)rca, 1, 0, 0, 0);
    nResp = SDIO1_RESP0;
    return nErr;
}

int sdio_reset(void)
{
	int ret;
	u8 abort;

	/* SDIO Simplified Specification V2.0, 4.4 Reset for SDIO */
	ret = SdioWfRegRead(&abort, 0, SDIO_CCCR_ABORT);
	if (ret)
		abort = 0x08;//this means RES bit in CCR I/O ABORT(0x6)
	else
		abort |= 0x08;

	return SdioWfRegWrite(abort, 0, SDIO_CCCR_ABORT);
}

/*
    CCCR(Card Common Control Registers)
*/

int sdio_read_cccr(void)
{
    int ret;
    int cccr_vsn;
    unsigned char data;
    
    ret = SdioWfRegRead(&data, 0, SDIO_CCCR_CCCR);
    
    cccr_vsn = (data&0x0f);

    ret = SdioWfRegRead(&data, 0, SDIO_CCCR_CAPS);
    
	if (data & SDIO_CCCR_CAP_SMB)	dev_dbg("WIFI init : multi block\n");
	if (data & SDIO_CCCR_CAP_LSC)	dev_dbg("WIFI init : low speed\n");
	if (data & SDIO_CCCR_CAP_4BLS)	dev_dbg("WIFI init : 4bit bus\n");
    
    if (cccr_vsn >= SDIO_CCCR_REV_1_10) {
        ret = SdioWfRegRead(&data, 0, SDIO_CCCR_POWER);
        if (ret)
            goto out;  
		if (data & SDIO_POWER_SMPC)	dev_dbg("WIFI init : high power\n");
    }
    if (cccr_vsn >= SDIO_CCCR_REV_1_20) {
        ret = SdioWfRegRead(&data, 0, SDIO_CCCR_SPEED);
        if (ret)
            goto out;
        if (data & SDIO_SPEED_SHS)
        {   
			dev_dbg("WIFI init : high speed\n");
            bsdio_highspeed = 1;
        }
    }    
out:
  return ret;  
}

static int sdio_read_fbr(struct sdio_func *func)
{
    int ret;
    unsigned char data;
	dev_dbg("WIFI init : num %d\n", func->num);
    ret = SdioWfRegRead(&data, 0, SDIO_FBR_BASE(func->num) + SDIO_FBR_STD_IF);
    
    if (ret)
        goto out;

    data &= 0x0f;

    if(data == 0x0f)
	{
        ret = SdioWfRegRead(&data, 0, SDIO_FBR_BASE(func->num) + SDIO_FBR_STD_IF_EXT);
        if (ret)
            goto out;
    }
    
    func->class = data;
	dev_dbg("WIFI init : class %d\n", func->class);
  
out:
  return ret;
}

static int sdio_read_cis(struct sdio_func *func)
{
  int ret;
  unsigned i, ptr = 0,fn,addr;
  u8 tpl_data[4];
  unsigned char x;
  /*
   * Note that this works for the common CIS (function number 0) as
   * well as a function's CIS * since SDIO_CCCR_CIS and SDIO_FBR_CIS
   * have the same offset.
   */
  //dev_dbg("%s %d func->num %d \n", __func__,__LINE__,func->num);
  fn = func->num;
  for (i = 0; i < 3; i++) {
    addr = SDIO_FBR_BASE(fn) + SDIO_FBR_CIS + i;
    ret = SdioWfRegRead(&x, 0, addr);
    if(ret) return ret;
    ptr |= x << (i * 8);
    //dev_dbg("fn %d x %x addr %x \n",fn, x,addr);
  }
  do {
    unsigned char tpl_code, tpl_link;
    //dev_dbg("%s %d func->num %d \n", __func__,__LINE__,func->num);
    ret = SdioWfRegRead(&tpl_code, 0, ptr++);
    if (ret)
      break;
  
    /* 0xff means we're done */
    if (tpl_code == 0xff)
      break;
  
    /* null entries have no link field or data */
    if (tpl_code == 0x00)
      continue;
  
    ret = SdioWfRegRead(&tpl_link, 0, ptr++);
    if (ret)
      break;
  
    /* a size of 0xff also means we're done */
    if (tpl_link == 0xff)
      break;
    
    //read tpl data by max 4 bytes  
    for (i=0; i< tpl_link && i< 4; i++)
    {
      unsigned char x;  
      ret = SdioWfRegRead(&x, 0, ptr+i);
      if(ret)
        break;

      tpl_data[i] = x;
    }
      
    switch(tpl_code)
    {
      case 0x20: //CISTPL_MANFID
        func->vendor = tpl_data[0] | (tpl_data[1]<<8);
        func->device = tpl_data[2] | (tpl_data[3]<<8);
        dev_dbg("WIFI init : vendor(0x%X), device(0x%X)\n", func->vendor, func->device);
        break;
      case 0x21: //CISTPL_FUNCID
        break;
      case 0x22: //CISTPL_FUNCE
        break;  
    }  
    
    ptr += tpl_link;
  } while (!ret);
  return ret;
} 

/**
 *      sdio_register_driver - register a function driver
 *      @drv: SDIO function driver
 */
void sdio_register_driver(struct sdio_driver *drv)
{
    if ( gsdio_device == NULL)
    {
        gsdio_device = drv;
        drv->next = NULL;
    }
    else
    {  
        gsdio_device->next = drv;
        drv->next = NULL; 
    }
}

/*
  @brife probe sdio device
  @func   detected sdio function info.
  @return 0: OK, 1: fail
*/
int sdio_probe_device(struct sdio_func *func)
{
    struct sdio_driver *sdrv = gsdio_device;
    struct sdio_device_id *ids;
    
    while(sdrv != NULL)
    {
        ids = sdrv->id_table;

        if (ids->class || ids->vendor || ids->device) 
        {
            if (ids->class != (u8)SDIO_ANY_ID && ids->class != func->class)
                goto out;    
            if (ids->vendor != (u16)SDIO_ANY_ID && ids->vendor != func->vendor)
                goto out; 
            if (ids->device != (u16)SDIO_ANY_ID && ids->device != func->device)
                goto out;
            
            sdrv->probe(func,ids);
            return 0;   
        }
    
        sdrv = sdrv->next;
    }
out:  
    return 1;
}

/*
  .Caution : About high speed mode,
             we are not support high speed mode, we only support ds mode.   
             DS - Default Speed up to 25MHz 3.3V signalling.
             HS - High Speed up to 50MHz 3.3V signalling.   

   @return 0 --success, otherwise fail 
*/
int sdio_init(struct sdio_func **pfunc)
{
    UINT nErr;
//  UINT nResp;
    u32 ocr, fn, rca, i;

    sdio_reset();
//	Sdio1Cmd( 0, 0, 1, 0, 0, 0); //GO IDLE STATE
//	nResp = SDIO1_RESP0;
    nErr = mmc_send_io_op_cond(0, &ocr);
    /* we don't have wifi module or WiFi failed to initialized  */
    if(nErr) 
        goto err;
    fn = (ocr & R4_FUNC_NUM) >> 28; //To get number of functions 

    mmc_send_io_op_cond(OCR_VOL_WIND, &ocr);
	mmc_send_relative_addr(&rca);
	ewl_os_wait_us(10);
	mmc_select_card(rca);

	sdio_read_cccr();
	sdio_enable_hs();		// set high speed mode
	sdio_enable_wide();		// 4bit bus mode

    for (i=1; i<=fn; i++)
    {
        gsdio_func[i-1].num = i;
        sdio_read_fbr(&gsdio_func[i-1]);
        sdio_read_cis(&gsdio_func[i-1]);
        nErr = sdio_probe_device((&gsdio_func[i-1]));
        *pfunc = &gsdio_func[i-1];
        if(!nErr)
            break;
    }

    return 0;
err:
    return -1;
}
 
void disable_int(void)
{
//	CRITICAL_BEGIN;
	portENTER_CRITICAL();
}

void restore_int(void)
{
//	CRITICAL_END;
	portEXIT_CRITICAL();
}

#if 0

#endif

ISRD static BYTE sdio_data_buffer[4096];

int sdio_write_pbuf(struct sdio_func *func, unsigned int addr, void *pbuf, int count)
{
    BOOL bRes = SDIO_FAIL;
	UINT nResp = 0, nArg = 0;
	struct pbuf *p = (struct pbuf *)pbuf;
	UINT remainder = count;
	UINT size = 0, blocks = 0;

	SDIO_DAT_LOG_START;

	disable_int();

	if(func->blksz != 256)
	{
		while(1)
			eprintf("blksz(%d)\n", func->blksz);
	}

	while(remainder > sdio_max_byte_size(func))	// 256보다 크거나 같으면...
    {
		blocks = remainder / 256;
		size = blocks * 256;
		if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_WPB)
		{
			eprintf("BLOCK size(%d) p->tot_len(%d) p->len(%d)\n", size, p->tot_len, p->len);
		}

		if(size > 2048)
		{
			while(1)
				eprintf("size(%d)\n", size);
		}

		while(SDIO1_DAT_EN);

		SDIO1_DATLEN = blocks;
		SDIO1_DAT_BL = func->blksz;

#if 0
		int qq = 0;
		while(p != NULL)
		{
			DmaMemCpy_isr((BYTE *)SDIO1_BASE, p->payload, p->len);
//			eprintf("Step%d! p->payload(0x%08X) p->len(%d)\n", qq++, p->payload, p->len);
			p = p->next;
		}
#endif

		nArg = CMD53_WRITE | func->num<<28 | addr<<9 | blocks | CMD53_BMODE;
		bRes = Sdio1Cmd(53, nArg, 1, 0, 1, 1);
		nResp = SDIO1_RESP0;

		if(nResp & 0x0000CB00)
			eprintf("Resp Err(0x%08X)\n", nResp);

#if 0
		if(p->len != p->tot_len)
		{
			struct pbuf *q = p;
			eprintf("step0 : q->tot_len(%d) q->len(%d)\n", q->tot_len, q->len);
			q = q->next;
			if(q)
			{
				eprintf("step1 : q->tot_len(%d) q->len(%d)\n", q->tot_len, q->len);
			}
			else
			{
				eprintf("step1 : q(NULL)\n");
			}
		}
#endif

		if(bRes == SDIO_OK)
		{
#if 0
			int qq = 0;
			while(p != NULL)
			{
				while(SDIO1_DAT_FULL)
					NOP();

				DmaMemCpy_isr((BYTE *)SDIO1_BASE, p->payload, p->len);

//				eprintf("Step%d! p->payload(0x%08X) p->len(%d)\n", qq++, p->payload, p->len);

				p = p->next;
			}
#else
			BYTE *ppos = sdio_data_buffer;
			while(p != NULL)
			{
				DmaMemCpy_isr((BYTE *)ppos, p->payload, p->len);
				ppos += p->len;
				p = p->next;
			}

			if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_WPB)
			{
				invalidate_dcache_range((UINT)sdio_data_buffer, ((UINT)sdio_data_buffer) + size);
				hexDump("read_pbuf-BLOCK", sdio_data_buffer, size);
			}

			while(SDIO1_DAT_FULL)
				NOP();

			DmaMemCpy_isr((BYTE *)SDIO1_BASE, sdio_data_buffer, size);
#endif
			while(SDIO1_DAT_EN);
			SDIO1_DAT_WE = 0;
			
			if(SDIO1_DAT_CRCERR)
			{
				eprintf("CRC Error\n");
				eprintf("Addr(0x%08X) BLOCK(%d) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, blocks, size, SDIO1_DAT_BL, count);
				eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			remainder -= size;
		}
		else
		{
			eprintf("CMD Error !!!!!!!!!!\n");
			SDIO1_DAT_EN = 0;
			SDIO1_DAT_WE = 0;
			break;
		}
	}

	while(remainder > 0)
	{
		size = min(remainder, sdio_max_byte_size(func));
		if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_WPB)
		{
			eprintf("BYTE size(%d) p->tot_len(%d) p->len(%d)\n", size, p->tot_len, p->len);
		}

		while(SDIO1_DAT_EN);

		SDIO1_DATLEN = 1;
		SDIO1_DAT_BL = size;

		nArg = CMD53_WRITE | func->num<<28 | addr<<9 | size;
		bRes = Sdio1Cmd(53, nArg, 1, 0, 1, 1);
		nResp = SDIO1_RESP0;

		if(nResp & 0x0000CB00)
			eprintf("Resp Err(0x%08X)\n", nResp);

		if(bRes == SDIO_OK)
		{
#if 0
			int qq = 0;
			while(p != NULL)
			{
				while(SDIO1_DAT_FULL)
					NOP();

				DmaMemCpy_isr((BYTE *)SDIO1_BASE, p->payload, p->len);
				
//				eprintf("Step%d! p->payload(0x%08X) p->len(%d)\n", qq++, p->payload, p->len);

				p = p->next;
			}
#else
			BYTE *ppos = sdio_data_buffer;
			while(p != NULL)
			{
				DmaMemCpy_isr((BYTE *)ppos, p->payload, p->len);
				ppos += p->len;
				p = p->next;
			}

			if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_WPB)
			{
				invalidate_dcache_range((UINT)sdio_data_buffer, ((UINT)sdio_data_buffer) + size);
				hexDump("write_pbuf-BYTE", sdio_data_buffer, size);
			}

			while(SDIO1_DAT_FULL)
				NOP();

			DmaMemCpy_isr((BYTE *)SDIO1_BASE, sdio_data_buffer, size);
#endif
			while(SDIO1_DAT_EN);
			SDIO1_DAT_WE = 0;

			if(SDIO1_DAT_CRCERR)
			{
				eprintf("CRC Error\n");
				eprintf("Addr(0x%08X) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, size, SDIO1_DAT_BL, count);
				eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			remainder -= size;
		}
		else
		{
			eprintf("CMD Error !!!!!!!!!!\n");
			SDIO1_DAT_EN = 0;
			SDIO1_DAT_WE = 0;
			break;
		}
	}

	if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_WSPB)
		eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');

	restore_int();

	SDIO_DAT_LOG_END;

	return bRes;
}

/**
 *      sdio_writesb - write to a FIFO of a SDIO function
 *      @func: SDIO function to access
 *      @addr: address of (single byte) FIFO
 *      @src: buffer that contains the data to write
 *      @count: number of bytes to write( count must be  1~511)
 *      
 *      @ret : 0->succeed, otherwise failed.
 *      Writes to the specified FIFO of a given SDIO function. Return
 *      value indicates if the transfer succeeded or not.
 */
int sdio_writesb(struct sdio_func *func, unsigned int addr, void *src,
        int count)
{
/*
@arg
the arg is already setup by the caller of owl_sdio_cmd(). For CMD53
the argument has the following semantics:

R/W flag:         1   [31]
Function:         3   [28:30]
Block mode:       1   [27]
OP code:          1   [26]
Address:          17  [9:25]
Byte/block count: 9   [0:8]     
*/
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0, nArg = 0;
	BYTE *buff = (BYTE *)src;
	UINT remainder = count;
	UINT size = 0, blocks = 0;
	UINT step2flag = 0;

	disable_int();

	SDIO_DAT_LOG_START;

	if(func->blksz != 256)
		eprintf("Black Size ????? (%d)\n", func->blksz);

	tprintf("Addr(0x%08X) count(%d) blksz(%d)\n", addr, count, func->blksz);

	while(remainder > sdio_max_byte_size(func))	// 256보다 크거나 같으면...
    {
		step2flag = 1;
	
		blocks = remainder / 256;
		size = blocks * 256;
		if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_WSB)
		{
			eprintf("BLOCK size(%d)\n", size);
		}

		if(size > 2048)
		{
			while(1)
				eprintf("size(%d)\n", size);
		}

		while(SDIO1_DAT_EN);

		SDIO1_DATLEN = blocks;
		SDIO1_DAT_BL = func->blksz;

		nArg = CMD53_WRITE | func->num<<28 | addr<<9 | blocks | CMD53_BMODE;
		bRes = Sdio1Cmd(53, nArg, 1, 0, 1, 1);
		nResp = SDIO1_RESP0;

		if(nResp & 0x0000CB00)
			eprintf("Resp Err(0x%08X)\n", nResp);

		if(bRes == SDIO_OK)
		{
#if 0
			while(SDIO1_DAT_FULL)
				NOP();

			DmaMemCpy_isr((BYTE *)SDIO1_BASE, buff, size);

			while(SDIO1_DAT_EN);
			SDIO1_DAT_WE = 0;

			if(SDIO1_DAT_CRCERR)
			{
				eprintf("CRC Error\n");
				eprintf("Addr(0x%08X) BLOCK(%d) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, blocks, size, SDIO1_DAT_BL, count);
				eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n",
					bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', 
					SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			remainder -= size;
			buff += size;
#else
#if 1
			UINT bl;
			for(bl=0;bl<blocks;bl++)
			{
				while(SDIO1_DAT_FULL)
					NOP();

				DmaMemCpy_isr((BYTE *)SDIO1_BASE, buff, SDIO1_DAT_BL);
				buff += SDIO1_DAT_BL;

				if(SDIO1_DAT_CRCERR)
				{
					eprintf("CRC Error\n");
					eprintf("Addr(0x%08X) BLOCK(%d) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, blocks, size, SDIO1_DAT_BL, count);
					eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
				}
			}
#endif

			while(SDIO1_DAT_EN);
			SDIO1_DAT_WE = 0;

			if(SDIO1_DAT_CRCERR)
			{
				eprintf("CRC Error\n");
				eprintf("Addr(0x%08X) BLOCK(%d) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, blocks, size, SDIO1_DAT_BL, count);
				eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			remainder -= size;
//			buff += size;
#endif
		}
		else
		{
			eprintf("CMD Error\n");
			SDIO1_DAT_EN = 0;
			SDIO1_DAT_WE = 0;
			break;
		}
	}

	while(remainder > 0)
	{
		if(step2flag != 0)
		{
			eprintf("2 Step Flag! size(%d)\n", count);
		}
	
		size = min(remainder, sdio_max_byte_size(func));
		if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_WSB)
		{
			eprintf("BYTE size(%d)\n", size);
		}
		while(SDIO1_DAT_EN);

		SDIO1_DATLEN = 1;
		SDIO1_DAT_BL = size;

		nArg = CMD53_WRITE | func->num<<28 | addr<<9 | size;
		bRes = Sdio1Cmd(53, nArg, 1, 0, 1, 1);
		nResp = SDIO1_RESP0;

		if(nResp & 0x0000CB00)
			eprintf("Resp Err(0x%08X)\n", nResp);

		if(bRes == SDIO_OK)
		{
			while(SDIO1_DAT_FULL)
				NOP();

			DmaMemCpy_isr((BYTE *)SDIO1_BASE, buff, size);
			while(SDIO1_DAT_EN);
			SDIO1_DAT_WE = 0;

			if(SDIO1_DAT_CRCERR)
			{
				eprintf("CRC Error\n");
				eprintf("Addr(0x%08X) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, size, SDIO1_DAT_BL, count);
				eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n",
					bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', 
					SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			remainder -= size;
			buff += size;
		}
		else
		{
			eprintf("CMD Error\n");
			SDIO1_DAT_EN = 0;
			SDIO1_DAT_WE = 0;
			break;
		}
	}

	if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_WSSB)
		eprintf("  [53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');

	restore_int();

	SDIO_DAT_LOG_END;

	return bRes;
}

/**
 *      sdio_readsb - read from a FIFO on a SDIO function
 *      @func: SDIO function to access
 *      @dst: buffer to store the data
 *      @addr: address of (single byte) FIFO
 *      @count: number of bytes to read
 *
 *      @ret : 0->succeed, otherwise failed.
 *
 *      Reads from the specified FIFO of a given SDIO function. Return
 *      value indicates if the transfer succeeded or not.
 */
int sdio_readsb(struct sdio_func *func, void *dst, unsigned int addr, int count)
{
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0, nArg = 0;
	BYTE *buf = (BYTE *)dst;
	UINT remainder = count;
	UINT size = 0, blocks = 0;

	SDIO_DAT_LOG_START;

	disable_int();

	if(func->blksz != 256)
	{
		while(1)
			eprintf("blksz(%d)\n", func->blksz);
	}

	tprintf("Addr(0x%08X) SDIO1_DAT_BL(%d) count(%d)\n", addr, SDIO1_DAT_BL, count);

	while(remainder > sdio_max_byte_size(func))
    {
		blocks = remainder / 256;
		size = blocks * 256;
		if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_RSB)
		{
			eprintf("BLOCK size(%d)\n", size);
		}

		if(size > 2048)
		{
			while(1)
				eprintf("size(%d)\n", size);
		}

		while(SDIO1_DAT_EN);

		SDIO1_DATLEN = blocks;
		SDIO1_DAT_BL = func->blksz;

		nArg = CMD53_READ | func->num<<28 | addr<<9 | blocks | CMD53_BMODE;
		bRes = Sdio1Cmd(53, nArg, 1, 0, 1, 0);	// CMD18과 비슷(read)
		nResp = SDIO1_RESP0;

		if(nResp & 0x0000CB00)
			eprintf("Resp Err(0x%08X)\n", nResp);

		if(bRes == SDIO_OK)
		{
#if 1
			while(SDIO1_DAT_EMPTY)
				NOP();

			if(SDIO1_DAT_CRCERR)
			{
				eprintf("CRC Error\n");
				eprintf("Addr(0x%08X) BLOCK(%d) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, blocks, size, SDIO1_DAT_BL, count);
				eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			DmaMemCpy_ip((BYTE *)buf, (BYTE *)SDIO1_BASE, size);
			invalidate_dcache_range((UINT)buf, ((UINT)buf) + size);

			remainder -= size;
			buf += size;
#else
			int bl;
			for(bl=0;bl<blocks;bl++)
			{
				while(SDIO1_DAT_EMPTY)
					NOP();

				if(SDIO1_DAT_CRCERR)
				{
					eprintf("CRC Error\n");
					eprintf("Addr(0x%08X) BLOCK(%d) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, blocks, size, SDIO1_DAT_BL, count);
					eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
				}

				DmaMemCpy_isr((BYTE *)buf, (BYTE *)SDIO1_BASE, SDIO1_DAT_BL);
				invalidate_dcache_range((UINT)buf, ((UINT)buf) + SDIO1_DAT_BL);

				buf += SDIO1_DAT_BL;
			}

			remainder -= size;
//			buf += size;
#endif
		}
		else
		{
			SDIO1_DAT_EN = 0;
			eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}

	while(remainder > 0)
	{
		size = min(remainder, sdio_max_byte_size(func));
		if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_RSB)
		{
			eprintf("BYTE size(%d)\n", size);
		}

		while(SDIO1_DAT_EN);

		SDIO1_DATLEN = 1;
		SDIO1_DAT_BL = size;

		nArg = CMD53_READ | func->num<<28 | addr<<9 | size;
		bRes = Sdio1Cmd(53, nArg, 1, 0, 1, 0);
		nResp = SDIO1_RESP0;
		
		if(nResp & 0x0000CB00)
			eprintf("Resp Err(0x%08X)\n", nResp);

		if(bRes == SDIO_OK)
		{
			while(SDIO1_DAT_EMPTY)
				NOP();

			if(SDIO1_DAT_CRCERR)
			{
				eprintf("CRC Error\n");
				eprintf("Addr(0x%08X) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, size, SDIO1_DAT_BL, count);
				eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			DmaMemCpy_isr((BYTE *)buf, (BYTE *)SDIO1_BASE, size);
			invalidate_dcache_range((UINT)buf, ((UINT)buf) + size);

			remainder -= size;
			buf += size;
		}
		else
		{
			SDIO1_DAT_EN = 0;
			eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}

	if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_RSSB)
		eprintf("  [53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');

	restore_int();

	SDIO_DAT_LOG_END;

	return bRes;
}

int sdio_read_pbuf(struct sdio_func *func, void *pbuf, unsigned int addr, int count)
{
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0, nArg = 0;
	struct pbuf *p = (struct pbuf *)pbuf;
	UINT remainder = count;
	UINT size = 0, blocks = 0;

	SDIO_DAT_LOG_START;

	disable_int();

	if(func->blksz != 256)
	{
		while(1)
			eprintf("blksz(%d)\n", func->blksz);
	}

	tprintf("Addr(0x%08X) SDIO1_DAT_BL(%d) count(%d) p->len(%d), p->tot_len(%d)\n", addr, SDIO1_DAT_BL, count, p->len, p->tot_len);

	while(remainder > sdio_max_byte_size(func))
    {
		blocks = remainder / 256;
		size = blocks * 256;
		if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_RPB)
		{
			eprintf("BLOCK size(%d) p->tot_len(%d) p->len(%d)\n", size, p->tot_len, p->len);
		}

		if(size > 2048)
		{
			while(1)
				eprintf("size(%d)\n", size);
		}

		while(SDIO1_DAT_EN);

		SDIO1_DATLEN = blocks;
		SDIO1_DAT_BL = func->blksz;

		nArg = CMD53_READ | func->num<<28 | addr<<9 | blocks | CMD53_BMODE;
		bRes = Sdio1Cmd(53, nArg, 1, 0, 1, 0);
		nResp = SDIO1_RESP0;

		if(nResp & 0x0000CB00)
			eprintf("Resp Err(0x%08X)\n", nResp);

		if(bRes == SDIO_OK)
		{
#if 0
			while(SDIO1_DAT_EMPTY)
				NOP();

			if(SDIO1_DAT_CRCERR)
			{
				eprintf("CRC Error\n");
				eprintf("Addr(0x%08X) BLOCK(%d) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, blocks, size, SDIO1_DAT_BL, count);
				eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			DmaMemCpy_isr((BYTE *)sdio_data_buffer, (BYTE *)SDIO1_BASE, size);

			if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_RPB)
			{
				invalidate_dcache_range((UINT)sdio_data_buffer, ((UINT)sdio_data_buffer) + size);
				hexDump("read_pbuf-BLOCK", sdio_data_buffer, size);
			}
#else
			UINT _i;
			BYTE *loopppos = sdio_data_buffer;
			for(_i=0;_i<blocks;_i++)
			{
				while(SDIO1_DAT_EMPTY)
					NOP();

				if(SDIO1_DAT_CRCERR)
				{
					eprintf("CRC Error\n");
					eprintf("Addr(0x%08X) BLOCK(%d) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, blocks, size, SDIO1_DAT_BL, count);
					eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
				}

				DmaMemCpy_isr((BYTE *)loopppos, (BYTE *)SDIO1_BASE, SDIO1_DAT_BL);

				if(SDIO1_DAT_CRCERR)
				{
					eprintf("CRC Error\n");
					eprintf("Addr(0x%08X) BLOCK(%d) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, blocks, size, SDIO1_DAT_BL, count);
					eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
				}

				loopppos += SDIO1_DAT_BL;
			}

			if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_RPB)
			{
				invalidate_dcache_range((UINT)sdio_data_buffer, ((UINT)sdio_data_buffer) + size);
				hexDump("read_pbuf-BLOCK", sdio_data_buffer, size);
			}

#endif
			BYTE *ppos = sdio_data_buffer;

			while(p != NULL)
			{
				DmaMemCpy_isr(p->payload, ppos, p->len);
				invalidate_dcache_range((UINT)p->payload, ((UINT)p->payload) + p->len);
				ppos += p->len;
				p = p->next;
			}

			remainder -= size;
		}
		else
		{
			SDIO1_DAT_EN = 0;
			eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}

	while(remainder > 0)
	{
		size = min(remainder, sdio_max_byte_size(func));
		if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_RPB)
		{
			eprintf("BYTE size(%d) p->tot_len(%d) p->len(%d)\n", size, p->tot_len, p->len);
		}

		while(SDIO1_DAT_EN);

		SDIO1_DATLEN = 1;
		SDIO1_DAT_BL = size;

		nArg = CMD53_READ | func->num<<28 | addr<<9 | size;
		bRes = Sdio1Cmd(53, nArg, 1, 0, 1, 0);
		nResp = SDIO1_RESP0;

		if(nResp & 0x0000CB00)
			eprintf("Resp Err(0x%08X)\n", nResp);

		if(bRes == SDIO_OK)
		{
			while(SDIO1_DAT_EMPTY)
				NOP();

			if(SDIO1_DAT_CRCERR)
			{
				eprintf("CRC Error\n");
				eprintf("Addr(0x%08X) size(%d) SDIO1_DAT_BL(%d) count(%d)\n", addr, size, SDIO1_DAT_BL, count);
				eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			DmaMemCpy_isr((BYTE *)sdio_data_buffer, (BYTE *)SDIO1_BASE, size);

			if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_RPB)
			{
				invalidate_dcache_range((UINT)sdio_data_buffer, ((UINT)sdio_data_buffer) + size);
				hexDump("read_pbuf-BYTE", sdio_data_buffer, size);
			}

			BYTE *ppos = sdio_data_buffer;

			while(p != NULL)
			{
				DmaMemCpy_isr(p->payload, ppos, p->len);
				invalidate_dcache_range((UINT)p->payload, ((UINT)p->payload) + p->len);
				ppos += p->len;
				p = p->next;
			}

			remainder -= size;
		}
		else
		{
			SDIO1_DAT_EN = 0;
			eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}

	if(debug_msg_t.WIFI_INTER_LOG & WIFI_LOG_RSPB)
		eprintf("[53] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');

	restore_int();

	SDIO_DAT_LOG_END;

	return bRes;
}
