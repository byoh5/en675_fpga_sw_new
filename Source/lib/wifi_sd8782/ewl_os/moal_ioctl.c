
#include <ewl_os.h>
#include <errno.h> //for ENOMEM

//#include "or32_type.h"
#include "wireless.h"
#include "moal_main.h"
#include "moal_util.h"

#ifdef FREE_RTOS_SUPPORT
int wifi_send_signal(UINT bIRQ);
#endif
/** 
 *  @brief Fill in wait queue 
 *   
 *  @param priv         A pointer to moal_private structure
 *  @param wait         A pointer to wait_queue structure                 
 *  @param wait_option  Wait option
 *
 *  @return             None
 */
static inline void
woal_fill_wait_queue(moal_private * priv, wait_queue * wait, t_u8 wait_option)
{
    ENTER();
    
    switch (wait_option) {
	    case MOAL_NO_WAIT:
	        break;
	    case MOAL_IOCTL_WAIT:
#ifdef FREE_RTOS_SUPPORT   	
	        wait->wait = priv->ioctl_wait_q;
#endif                  
            wait->start_time = ewl_os_timer_get_ms();
	        priv->ioctl_wait_q_woken = MFALSE;
	        wait->condition = &priv->ioctl_wait_q_woken;
        	break;
    }
    
    LEAVE();
    return;
}

/** 
 *  @brief Wait mlan ioctl complete
 *   
 *  @param priv         A pointer to moal_private structure
 *  @param req          A pointer to mlan_ioctl_req structure   
 *  @param wait_option  Wait option
 *
 *  @return             None
 */
static inline void
woal_wait_ioctl_complete(moal_private * priv, mlan_ioctl_req * req,
                         t_u8 wait_option)
{
    moal_handle *phandle;
    BOOLEAN cancel_flag = MFALSE;
   
    ENTER();

    switch (wait_option) {
        case MOAL_NO_WAIT:
            break;
        case MOAL_IOCTL_WAIT:
        //wait_event_interruptible(priv->ioctl_wait_q, priv->ioctl_wait_q_woken);
        if (priv->ioctl_wait_q_woken == MFALSE)
            cancel_flag = MTRUE;
            break;
    }

    
    if (cancel_flag == MTRUE) {
        req->action = MLAN_ACT_CANCEL;
        phandle = priv->phandle;
        mlan_ioctl(phandle->pmlan_adapter, req);
        PRINTM(MIOCTL,
               "IOCTL cancel: id=0x%lx, sub_id=0x%lx wait_option=%d, action=%d\n",
               req->req_id, (*(t_u32 *) req->pbuf), wait_option,
               (int) req->action);
    }
    LEAVE();
    return;
}



extern moal_handle *m_handle[];     /** The global variable of a pointer to moal_handle  structure variable  **/

void ioctl_loop(moal_private * priv, wait_queue *wait)
{
    t_u32 timeout =0;
    
    MLAN_UNUSED_ARG(priv);
    ENTER();

    while(*wait->condition == MFALSE)
    {
        ewl_poll();
        mlan_main_process(m_handle[0]->pmlan_adapter);
        timeout = ewl_os_timer_get_ms() - wait->start_time;

        if (timeout > MOAL_IOCTL_TIMEOUT)
        {
            break;
        }
    }

    LEAVE();
}

/** 
 *  @brief Send ioctl request to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *  @param req           A pointer to mlan_ioctl_req buffer
 *  @param wait_option   Wait option (MOAL_WAIT or MOAL_NO_WAIT)
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_request_ioctl(moal_private * priv, mlan_ioctl_req * req, t_u8 wait_option)
{
    wait_queue *wait;
    mlan_status status;
    moal_handle *phandle = priv->phandle;
    
    ENTER();

    if (phandle->surprise_removed == MTRUE) {
        PRINTM(MERROR,
               "IOCTL is not allowed while the device is not present\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    wait = (wait_queue *) req->reserved_1;
    req->bss_index = priv->bss_index;
    if (wait_option)
        woal_fill_wait_queue(priv, wait, wait_option);
    else
        req->reserved_1 = 0;        
    /* Call MLAN ioctl handle */
    status = mlan_ioctl(phandle->pmlan_adapter, req);
    switch (status) {
        case MLAN_STATUS_PENDING:
            PRINTM(MIOCTL,
                    "IOCTL pending: %p id=0x%lx, sub_id=0x%lx wait_option=%d, action=%d\n",
                    req, req->req_id, (*(t_u32 *) req->pbuf), wait_option,
                    (int) req->action);
            phandle->ioctl_pending++;
            wait = (wait_queue *) req->reserved_1;
            /* Status pending, wake up main process */
            if (wait_option)
                ioctl_loop(priv,wait);               
            /* Wait for completion */
            if (wait_option) {
                woal_wait_ioctl_complete(priv, req, wait_option);
                status = wait->status;
            }
            break;
        case MLAN_STATUS_SUCCESS:
        case MLAN_STATUS_FAILURE:
        case MLAN_STATUS_RESOURCE:
            break;
        default:
            break;
    }
    LEAVE();
    return status;
}

/** 
 *  @brief Get BSS info
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param bss_info             A pointer to mlan_bss_info structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_bss_info(moal_private * priv, t_u8 wait_option,
                  mlan_bss_info * bss_info)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_get_info *info = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_BSS_INFO;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (bss_info) {
            ewl_os_memcpy(bss_info, &info->param.bss_info, sizeof(mlan_bss_info));
        }
    }
done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Send deauth command to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param mac           MAC address to deauthenticate
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_disconnect(moal_private * priv, t_u8 wait_option, t_u8 * mac)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_STOP;
    if (mac)
        ewl_os_memcpy((t_u8 *) & bss->param.bssid, mac, sizeof(mlan_802_11_mac_addr));
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);

  done:
    if (req)
        ewl_os_free(req);
#ifdef REASSOCIATION
    priv->reassoc_required = MFALSE;
#endif /* REASSOCIATION */
    LEAVE();
    return status;
}

/** 
 *  @brief Send bss_start command to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option          Wait option  
 *  @param ssid_bssid    A point to mlan_ssid_bssid structure
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_bss_start(moal_private * priv, t_u8 wait_option,
               mlan_ssid_bssid * ssid_bssid)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;

    ENTER();

    /* Stop the O.S. TX queue if needed */
    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }
    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_START;
    if (ssid_bssid)
        ewl_os_memcpy(&bss->param.ssid_bssid, ssid_bssid, sizeof(mlan_ssid_bssid));
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;
    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Send get FW info request to MLAN
 *   
 *  @param priv             A pointer to moal_private structure
 *  @param wait_option      Wait option  
 *
 *  @return      None
 */

void
woal_request_get_fw_info(moal_private * priv, t_u8 wait_option)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_get_info *info;
    mlan_status status;
    moal_handle *phandle = priv->phandle;
    
    ENTER();

    ewl_os_memset(priv->current_addr, 0xff, ETH_ALEN);

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    info = (mlan_ds_get_info *) req->pbuf;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;
    info->sub_command = MLAN_OID_GET_FW_INFO;
    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        phandle->fw_release_number = info->param.fw_info.fw_ver;
        if (priv->current_addr[0] == 0xff)
            ewl_os_memcpy(priv->current_addr, &info->param.fw_info.mac_addr,
                   sizeof(mlan_802_11_mac_addr));
        
    } 
    else
    {    
        PRINTM(MERROR, "get fw info failed! status=%d, error_code=0x%lx\n",
               status, req->status_code);
    }
  done:
    if (req)
        ewl_os_free(req);
                
    LEAVE();
    return;
}

/**
 *  @brief Set Deep Sleep
 *
 *  @param priv         Pointer to the moal_private driver data struct
 *  @param wait_option  wait option
 *  @param bdeep_sleep  TRUE--enalbe deepsleep, FALSE--disable deepsleep
 *  @param idletime     Idle time for optimized PS API
 *
 *  @return             0 --success, otherwise fail
 */
int
woal_set_deep_sleep(moal_private * priv, t_u8 wait_option, BOOLEAN bdeep_sleep,
                    t_u16 idletime)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pm = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    pm = (mlan_ds_pm_cfg *) req->pbuf;
    pm->sub_command = MLAN_OID_PM_CFG_DEEP_SLEEP;
    req->req_id = MLAN_IOCTL_PM_CFG;

    req->action = MLAN_ACT_SET;
    if (bdeep_sleep == MTRUE) {
        PRINTM(MIOCTL, "Deep Sleep: sleep\n");
        pm->param.auto_deep_sleep.auto_ds = DEEP_SLEEP_ON;
        if (idletime) {
            pm->param.auto_deep_sleep.idletime = idletime;
        }
        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, wait_option)) {
            ret = -EFAULT;
            goto done;
        }
    } else {
        PRINTM(MIOCTL, "Deep Sleep: wakeup\n");
        pm->param.auto_deep_sleep.auto_ds = DEEP_SLEEP_OFF;
        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, wait_option)) {
            ret = -EFAULT;
            goto done;
        }
    }
  done:
    if (req)
        ewl_os_free(req);

    LEAVE();
    return ret;
}


/** 
 *  @brief Get Host Sleep parameters
 *
 *  @param priv         A pointer to moal_private structure
 *  @param action       Action: set or get
 *  @param wait_option  Wait option (MOAL_WAIT or MOAL_NO_WAIT)
 *  @param hscfg        A pointer to mlan_ds_hs_cfg structure
 *
 *  @return             MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_get_hs_params(moal_private * priv, t_u16 action, t_u8 wait_option,
                       mlan_ds_hs_cfg * hscfg)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ds_pm_cfg *pmcfg = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    pmcfg = (mlan_ds_pm_cfg *) req->pbuf;
    pmcfg->sub_command = MLAN_OID_PM_CFG_HS_CFG;
    req->req_id = MLAN_IOCTL_PM_CFG;
    req->action = action;
    if (action == MLAN_ACT_SET)
        ewl_os_memcpy(&pmcfg->param.hs_cfg, hscfg, sizeof(mlan_ds_hs_cfg));

    /* Send IOCTL request to MLAN */
    ret = woal_request_ioctl(priv, req, wait_option);
    if (ret == MLAN_STATUS_SUCCESS) {
        if (hscfg && action == MLAN_ACT_GET) {
            ewl_os_memcpy(hscfg, &pmcfg->param.hs_cfg, sizeof(mlan_ds_hs_cfg));
        }
    }
  done:
    if (req && (ret != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return ret;
}


/**
 *  @brief Cancel Host Sleep configuration
 *
 *  @param priv             A pointer to moal_private structure
 *  @param wait_option      wait option
 *
 *  @return      MLAN_STATUS_SUCCESS, MLAN_STATUS_PENDING,
 *                      or MLAN_STATUS_FAILURE          
 */
mlan_status
woal_cancel_hs(moal_private * priv, t_u8 wait_option)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ds_hs_cfg hscfg;

    ENTER();

    /* Cancel Host Sleep */
    hscfg.conditions = HOST_SLEEP_CFG_CANCEL;
    hscfg.is_invoke_hostcmd = MTRUE;
    ret = woal_set_get_hs_params(priv, MLAN_ACT_SET, wait_option, &hscfg);

    LEAVE();
    return ret;
}


#if defined(UAP_SUPPORT)
/** 
 *  @brief host command ioctl function
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
/*********  format of ifr_data *************/
/*    buf_len + Hostcmd_body 		   */
/*    buf_len: 4 bytes                     */
/*             the length of the buf which */
/*             can be used to return data  */
/*             to application		   */
/*    Hostcmd_body       	           */
/*******************************************/
int
woal_hostcmd_ioctl(moal_private *priv, struct ifreq *req)
{
    t_u32 buf_len = 0;
    HostCmd_Header cmd_header;
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *ioctl_req = NULL;

    ENTER();

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_hostcmd_ioctl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }

    ewl_os_memcpy(&buf_len, req->ifr_data, sizeof(buf_len));

    ewl_os_memset(&cmd_header, 0, sizeof(cmd_header));

    /* get command header */
    ewl_os_memcpy(&cmd_header, req->ifr_data + sizeof(buf_len),sizeof(HostCmd_Header));

    PRINTM(MINFO, "Host command len = %d\n", woal_le16_to_cpu(cmd_header.size));

    if (woal_le16_to_cpu(cmd_header.size) > MLAN_SIZE_OF_CMD_BUFFER) {
        ret = -EINVAL;
        goto done;
    }

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    misc = (mlan_ds_misc_cfg *) ioctl_req->pbuf;

    misc->param.hostcmd.len = woal_le16_to_cpu(cmd_header.size);

    /* get the whole command from user */
    ewl_os_memcpy(misc->param.hostcmd.cmd, req->ifr_data + sizeof(buf_len),
                                    misc->param.hostcmd.len); 
    misc->sub_command = MLAN_OID_MISC_HOST_CMD;
    ioctl_req->req_id = MLAN_IOCTL_MISC_CFG;

    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (misc->param.hostcmd.len > buf_len) {
        PRINTM(MERROR, "buf_len is too small, resp_len=%d, buf_len=%d\n",
               (int) misc->param.hostcmd.len, (int) buf_len);
        ret = -EFAULT;
        goto done;
    }
    ewl_os_memcpy(req->ifr_data + sizeof(buf_len), (t_u8 *) misc->param.hostcmd.cmd,
         misc->param.hostcmd.len); 
done:
    if (ioctl_req)
        ewl_os_free(ioctl_req);
    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief CUSTOM_IE ioctl handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
int
woal_custom_ie_ioctl(moal_private *priv, struct ifreq *req)
{
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ds_misc_custom_ie *custom_ie = NULL;
    int ret = 0;

    ENTER();

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "woal_custom_ie_ioctl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }

    if (!(custom_ie = ewl_os_malloc(sizeof(mlan_ds_misc_custom_ie)))) {
        ret = -ENOMEM;
        goto done;
    }

    ewl_os_memset(custom_ie, 0, sizeof(mlan_ds_misc_custom_ie));
    ewl_os_memcpy(custom_ie, req->ifr_data, sizeof(mlan_ds_misc_custom_ie));
    

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    misc = (mlan_ds_misc_cfg *) ioctl_req->pbuf;
    misc->sub_command = MLAN_OID_MISC_CUSTOM_IE;
    ioctl_req->req_id = MLAN_IOCTL_MISC_CFG;
    if ((custom_ie->len == 0) ||
        (custom_ie->len == sizeof(custom_ie->ie_data_list[0].ie_index)))
        ioctl_req->action = MLAN_ACT_GET;
    else
        ioctl_req->action = MLAN_ACT_SET;

    ewl_os_memcpy(&misc->param.cust_ie, custom_ie, sizeof(mlan_ds_misc_custom_ie));

    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (ioctl_req->action == MLAN_ACT_GET)
        ewl_os_memcpy(req->ifr_data, &misc->param.cust_ie, sizeof(mlan_ds_misc_custom_ie));

done:
    if (ioctl_req)
        ewl_os_free(ioctl_req);
    if (custom_ie)
        ewl_os_free(custom_ie);
    LEAVE();
    return ret;
}

/** 
 *  @brief ioctl function get BSS type
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
int
woal_get_bss_type(moal_private *priv, struct ifreq *req)
{
    int ret = 0;
    int bss_type;

    ENTER();

    bss_type = (int) priv->bss_type;
    ewl_os_memcpy(req->ifr_data, &bss_type, sizeof(int));
    
    LEAVE();
    return ret;
}

/** 
 *  @brief host command ioctl function
 *   
 *  @param priv		A pointer to moal_private structure
 *  @param wrq 		A pointer to iwreq structure
 *  @return    		0 --success, otherwise fail
 */
int
woal_host_command(moal_private * priv, struct iwreq *wrq)
{
    HostCmd_Header cmd_header;
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    /* Sanity check */
    if (wrq->u.data.pointer == NULL) {
        PRINTM(MERROR, "hostcmd IOCTL corrupt data\n");
        ret = -EINVAL;
        goto done;
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    misc = (mlan_ds_misc_cfg *) req->pbuf;
    ewl_os_memset(&cmd_header, 0, sizeof(cmd_header));

    /* get command header */
    ewl_os_memcpy(&cmd_header, wrq->u.data.pointer, sizeof(HostCmd_Header));
    
    misc->param.hostcmd.len = woal_le16_to_cpu(cmd_header.size);

    PRINTM(MINFO, "Host command len = %lu\n", misc->param.hostcmd.len);

    if (!misc->param.hostcmd.len ||
        misc->param.hostcmd.len > MLAN_SIZE_OF_CMD_BUFFER) {
        PRINTM(MERROR, "Invalid data buffer length\n");
        ret = -EINVAL;
        goto done;
    }

    /* get the whole command from user */
    ewl_os_memcpy(misc->param.hostcmd.cmd, wrq->u.data.pointer,
                                    woal_le16_to_cpu(cmd_header.size));
    misc->sub_command = MLAN_OID_MISC_HOST_CMD;
    req->req_id = MLAN_IOCTL_MISC_CFG;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    ewl_os_memcpy(wrq->u.data.pointer, (t_u8 *) misc->param.hostcmd.cmd,
                                misc->param.hostcmd.len); 
    wrq->u.data.length = misc->param.hostcmd.len;
  done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return ret;
}

/** 
 *  @brief Send set MAC address request to MLAN
 *   
 *  @param priv   A pointer to moal_private structure
 *
 *  @return       MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING -- success, otherwise fail
 */
mlan_status
woal_request_set_mac_address(moal_private * priv)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_MAC_ADDR;
    ewl_os_memcpy(&bss->param.mac_addr, priv->current_addr,
           sizeof(mlan_802_11_mac_addr));
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
    if (status == MLAN_STATUS_SUCCESS) {
        //ewl_os_memcpy(priv->netdev->dev_addr, priv->current_addr, ETH_ALEN);
       // HEXDUMP("priv->MacAddr:", priv->current_addr, ETH_ALEN);
    } else {
        PRINTM(MERROR, "set mac address failed! status=%d, error_code=0x%x\n",
               status, req->status_code);
    }
done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return status;
}