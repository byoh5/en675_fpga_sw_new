#include "dev.h"

#include "ewl_os.h"
#include "ewl_pkt_header.h"
#include "ewl.h"
#include "ewl_sdio/sdio.h"
#include "ewl_sdio/sdio_func.h"

#include "netif/etharp.h"
#include "wireless.h"
#include "moal_util.h"
#include "moal_main.h"
#include "moal_sdio.h"
#include "moal_shim.h"
#include "moal_priv.h"

#include "moal_wext.h"
#include "wext.h"                        
#include "iw_handler.h"  

#define MIN_WIFI_PKTLEN (MLAN_HEADER_SIZE + 60)


static moal_private *sta_priv=NULL;
static moal_private *uap_priv=NULL;
extern moal_handle *m_handle[];    /** The global variable of a pointer to moal_handle  structure variable  **/

/*
    Get WiFi Driver handle for STA MODE
*/

void *
ewl_get_sta_handle(void)
{
    return (void *)sta_priv;
}

void *
ewl_get_uap_handle(void)
{
    return (void *)uap_priv;
}

                        
/*
   This function return MAC address for WiFi
*/
void 
ewl_get_hwaddr(struct netif *netif, u8* pmac)
{
    moal_private *priv=netif->state;
    if (priv)
        ewl_os_memcpy(pmac,priv->current_addr,MLAN_MAC_ADDR_LENGTH);
    else
        ewl_os_memset(pmac,0,MLAN_MAC_ADDR_LENGTH);
    return;
}

/**
 *  @brief This function sets the MAC address to firmware.
 *
 *  @param netif  the network interface to change
 *  @param addr   MAC address to set
 *
 *  @return        0 --success, otherwise fail
 */
int
ewl_set_hwaddr(struct netif *netif, void *addr)
{
    int ret = 0;
    moal_private *priv = (moal_private *)netif->state;
    t_u8 prev_addr[ETH_ALEN];

    ENTER();
    ewl_os_memcpy(prev_addr, priv->current_addr, ETH_ALEN);
    ewl_os_memset(priv->current_addr, 0, ETH_ALEN);

    ewl_os_memcpy(priv->current_addr, addr, ETH_ALEN);
#if defined(WIFI_DIRECT_SUPPORT)
#if defined(STA_CFG80211) && defined(UAP_CFG80211)
#if LINUX_VERSION_CODE >= WIFI_DIRECT_KERNEL_VERSION
    if (priv->bss_type == MLAN_BSS_TYPE_WIFIDIRECT) {
        priv->current_addr[0] |= 0x02;
        PRINTM(MCMND, "Set WFD device addr: %pM\n", priv->current_addr);
    }
#endif
#endif
#endif
    if (MLAN_STATUS_SUCCESS != woal_request_set_mac_address(priv)) {
        PRINTM(MERROR, "Set MAC address failed\n");
        /* For failure restore the MAC address */
        ewl_os_memcpy(priv->current_addr, prev_addr, ETH_ALEN);
        ret = -EFAULT;
        goto done;
    }

    ewl_os_memcpy(netif->hwaddr, priv->current_addr, ETH_ALEN);
  done:
    LEAVE();
    return ret;  
}

int
ewl_get_fwhwaddr(struct netif *netif, void *addr)
{
    int ret = 0;
    t_u8 data[ETH_ALEN];  
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    moal_private *priv = (moal_private *)netif->state;    
    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    
    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_MAC_ADDR;
    req->req_id = MLAN_IOCTL_BSS;

    /* GET operation */
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        printf("%s %d \n",__func__,__LINE__);
        goto done;
    }

    ewl_os_memcpy(addr, bss->param.mac_addr, sizeof(data));
    ret = sizeof(data);    

done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return ret;
}
/**
    This function initializes the SDIO driver & WiFi.

    @return EWL_SUCCESS 
            EWL_FAILURE 
 */
 
int 
ewl_init(void)
{   
//    int ret = EWL_SUCCESS;
    struct sdio_func *func = NULL;
    struct sdio_mmc_card *card = NULL;

    ENTER();
    woal_sdio_register();
    if(sdio_init(&func))
        return EWL_FAILURE;
    card = (struct sdio_mmc_card*)func->priv_data;

    if (MLAN_STATUS_SUCCESS != woal_init_fw(card->handle)) {
        PRINTM(MFATAL, "Firmware Init Failed\n");
        return EWL_FAILURE;
    }

    return EWL_SUCCESS;
    LEAVE();
}

/*
    This functions adds a new netif(for lwip) 
    It allocates, initializes and registers the interface by performing
    the follwing operations-
        - Allocate a new netif structure
        - Register the netif with lwip

    @init_netif_cb : finishe to initialize netif allocated this function.
    @bss_type : 0 -> STA, 1 -> Mobile AP

    @return if success, struct netif else NULL     
*/

void* 
//ewl_add_netif(ewl_init_netif_cb_t init_netif_cb, u8 bss_type)
ewl_add_netif(u8 bss_type)
{
    moal_private *priv;
    int bss_index = 0;
    bss_index = m_handle[0]->priv_num++;

    priv = woal_add_interface(m_handle[0],
                             bss_index,
                             bss_type);
                             
    if(priv == NULL)
        return NULL;

    if(bss_type == 0)
        sta_priv = priv;
    else if (bss_type == BSS_TYPE_UAP)
        uap_priv = priv;
	
    priv->netif = ewl_os_calloc(1,sizeof(struct netif));
    
    if(priv->netif)
    {
        priv->netif->state = priv;    
        //ewl_os_printf("%s %p, riv->netif %p,bss_type %x \n", __func__,priv,priv->netif,bss_type);
//        if(init_netif_cb)
//            init_netif_cb(priv->netif);
    }
    else
        return NULL;

    return priv->netif;    
    
}


/*
 * This is wifi polling function.
 *
 * It handles the main process, which in turn handles the complete
 * driver operations.
 * poll wifi device periodically
 *  -----------------------------
 *
 *  It runs in a loop and provides the core functionalities.
 *
 * The main responsibilities of this function are -
 *      - Ensure concurrency control
 *      - Handle pending interrupts and call interrupt handlers
 *      - Wake up the card if required
 *      - Handle command responses and call response handlers
 *      - Handle events and call event handlers
 *      - Execute pending commands
 *      - Transmit pending data packets
 *
*/
void ewl_poll(void)
{
    ENTER();

    if(sdio_irq)
    {  
//        CRITICAL_BEGIN;
        sdio_irq--;
//        CRITICAL_END;
        sdio_fun_irq();                 /*called when sdio data irq is occued */        
        mlan_main_process(m_handle[0]->pmlan_adapter);
    }
    
    LEAVE();    
}

/**
    regiset callback function to priv when wifi is connected or disconnected.

    @conn_cb : is called when wifi port is released.
    @disconn_cb : is called when wifi port is closed.
**/

void
ewl_set_conn_cb(void *handle,
                void (*conn_cb)(BYTE *buf, int buf_len),void (*disconn_cb)(BYTE *buf, int buf_len))
{
    moal_private *priv = (moal_private *)handle;
    priv->connected_cb = conn_cb;
    priv->dis_connected_cb = disconn_cb;
}

/**
    @brief GET tx packet pending count in mlan
      
    @param netif  A pointer to netif structure  
    @parm tx_pending pending pbuf on wifi to transmit 
    
    @return 0 : wifi tx enable, 1: wifi busy.  
*/
int
ewl_is_tx_pending(struct netif *netif, u32 *tx_pending)
{
	return 0;
	UNUSED(netif);UNUSED(tx_pending);
}


/** 
 *  @brief This function handles packet transmission
 *  
 *  @param p      A pointer to pbuf structure
 *  @param netif  A pointer to netif structure
 *
 *  @return        0 --success, otherwise fail
 */ 

int 
ewl_start_tx(struct pbuf* p, struct netif *netif)
{
    moal_private *priv = netif->state;
    if (priv->media_connected) {
//      if (priv->tx_enable)
//          goto err;
        pbuf_ref(p);
        if (p->tot_len < MIN_WIFI_PKTLEN) {
            p->tot_len = MIN_WIFI_PKTLEN;
        }
        return woal_hard_start_xmit(p, priv);        
    }
//err:
    return -1;
}

/** 
 *  @brief This function return a pbuf filled with the received packet from WiFi
 *  
 *  @param netif  A pointer to netif structure
 *
 *  @return        pbuf -- success,
                   NULL -- receive no packet.
 */ 

void *
ewl_pkt_rx(struct netif *netif)
{
    moal_private *priv = netif->state;
    struct pbuf *q=NULL;

    if(!PQUEUE_EMPTY(priv->pqueue))
    {
        q = PQUEUE_FIRST(priv->pqueue);
        PQUEUE_DEQUEUE(priv->pqueue);        
    }

    return q;
}

/** 
 *  @ Send multicast address to WiFi
 *   
 *  @param priv     A pointer to netif structure
 *  @multicast_mac  A pointer to multicast mac address that size is 6                  
 *
 *  @return       None
 */
void
ewl_set_multicast_addr(struct netif *netif, u8 *multicast_mac)
{
    moal_private * priv = netif->state;
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        PRINTM(MERROR, "%s:Fail to allocate ioctl req buffer\n", __FUNCTION__);
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_MULTICAST_LIST;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;

    bss->param.multicast_list.mode = MLAN_MULTICAST_MODE;
    bss->param.multicast_list.num_multicast_addr = 1;
    memcpy(&bss->param.multicast_list.mac_list[0],
           multicast_mac,
           ETH_ALEN);

 /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_NO_WAIT);
    if (status != MLAN_STATUS_PENDING)
        ewl_os_free(req);
  done:
    LEAVE();           
}    

/***
    @return 1 = connected;
            0 = is not;
*/
#ifdef STA_SUPPORT
int
ewl_sta_is_connected(void)
{
    moal_private * priv = ewl_get_sta_handle();
    if (priv)
        return priv->media_connected;
    else
        return 0;
}

int
ewl_sta_get_freq(struct netif *netif)
{
    struct iw_freq fwrq;
    moal_private * priv = netif->state;
    woal_get_freq(priv,&fwrq);
    dprintf("m %d, e %d, i %d, flags %x \n",fwrq.m, fwrq.e, fwrq.i, fwrq.flags);
    return 0;
}
#endif
