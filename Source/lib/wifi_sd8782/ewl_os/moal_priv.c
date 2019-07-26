
#include "ewl_os.h"
#include <errno.h> //for ENOMEM

#include "wireless.h"
#include "moal_main.h"
#include "moal_sdio.h"
#include "moal_util.h"
#include "moal_priv.h"
#include "moal_wext.h"

#ifdef STA_SUPPORT
/** 
 *  @brief Copy Rates
 *   
 *  @param dest    A pointer to destination buffer
 *  @param pos     The position for copy
 *  @param src     A pointer to source buffer
 *  @param len     Length of the source buffer
 *
 *  @return        Number of rates copied 
 */
static inline int
woal_copy_rates(t_u8 * dest, int pos, t_u8 * src, int len)
{
    int i;

    for (i = 0; i < len && src[i]; i++, pos++) {
        if (pos >= MLAN_SUPPORTED_RATES)
            break;
        dest[pos] = src[i];
    }
    return pos;
}

/** 
 *  @brief Request a scan
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option  
 *  @param req_ssid             A pointer to mlan_802_11_ssid structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_request_scan(moal_private * priv,
                  t_u8 wait_option, mlan_802_11_ssid * req_ssid)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_scan *scan = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();
    MOAL_ACQ_SEMAPHORE_BLOCK(priv->async_sem);
    priv->scan_pending_on_block = MTRUE;
    /* Allocate an IOCTL request buffer */
    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_scan));
    if (ioctl_req == NULL) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    scan = (mlan_ds_scan *) ioctl_req->pbuf;
    
    if (req_ssid && req_ssid->ssid_len != 0) {
        /* Specific SSID scan */
        ioctl_req->req_id = MLAN_IOCTL_SCAN;
        ioctl_req->action = MLAN_ACT_SET;
        
        scan->sub_command = MLAN_OID_SCAN_SPECIFIC_SSID;
        
        ewl_os_memcpy(scan->param.scan_req.scan_ssid.ssid,
               req_ssid->ssid, req_ssid->ssid_len);
        scan->param.scan_req.scan_ssid.ssid_len = req_ssid->ssid_len;
    } else {
        /* Normal scan */
        ioctl_req->req_id = MLAN_IOCTL_SCAN;
        ioctl_req->action = MLAN_ACT_SET;
        
        scan->sub_command = MLAN_OID_SCAN_NORMAL;
    }
    
    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, ioctl_req, wait_option);
    if (status == MLAN_STATUS_FAILURE) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

done:
    if ((ioctl_req) && (status != MLAN_STATUS_PENDING))
        ewl_os_free(ioctl_req);
    
    if (ret == MLAN_STATUS_FAILURE) {
        priv->scan_pending_on_block = MFALSE;
        MOAL_REL_SEMAPHORE(priv->async_sem);
    }
    LEAVE();
    return ret;
}

/** 
 *  @brief Get scan table
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wait_option  Wait option
 *  @param scan_resp    A pointer to mlan_scan_resp structure
 *
 *  @return             MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_get_scan_table(moal_private * priv, t_u8 wait_option,
                    mlan_scan_resp * scan_resp)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_scan *scan = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_scan));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    scan = (mlan_ds_scan *) req->pbuf;
    scan->sub_command = MLAN_OID_SCAN_NORMAL;
    req->req_id = MLAN_IOCTL_SCAN;
    req->action = MLAN_ACT_GET;
    ewl_os_memcpy((void *) &scan->param.scan_resp, (void *) scan_resp,
           sizeof(mlan_scan_resp));

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (scan_resp) {
            ewl_os_memcpy(scan_resp, &scan->param.scan_resp, sizeof(mlan_scan_resp));
        }
    }

  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}


/** 
 *  @brief Find the best network to associate
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option  
 *  @param ssid_bssid           A pointer to mlan_ssid_bssid structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_find_best_network(moal_private * priv, t_u8 wait_option,
                       mlan_ssid_bssid * ssid_bssid)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u8 *mac = 0;

    ENTER();

    if (!ssid_bssid) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;
    bss->sub_command = MLAN_OID_BSS_FIND_BSS;

    ewl_os_memcpy(&bss->param.ssid_bssid, ssid_bssid, sizeof(mlan_ssid_bssid));

    /* Send IOCTL request to MLAN */
    ret = woal_request_ioctl(priv, req, wait_option);
    if (ret == MLAN_STATUS_SUCCESS) {
        ewl_os_memcpy(ssid_bssid, &bss->param.ssid_bssid, sizeof(mlan_ssid_bssid));
        mac = (t_u8 *) & ssid_bssid->bssid;
        PRINTM(MINFO,
               "Find network: ssid=%s, %02x:%02x:%02x:%02x:%02x:%02x, idx=%d\n",
               ssid_bssid->ssid.ssid, mac[0], mac[1], mac[2], mac[3], mac[4],
               mac[5], (int) ssid_bssid->idx);
    }

  done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return ret;
}


/** 
 *  @brief Get mode
 *
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option   Wait option (MOAL_WAIT or MOAL_NO_WAIT)   
 *
 *  @return              Wireless mode
 */
t_u32
woal_get_mode(moal_private * priv, t_u8 wait_option)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    t_u32 mode = priv->w_stats.status;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_MODE;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        switch (bss->param.bss_mode) {
        case MLAN_BSS_MODE_INFRA:
            mode = IW_MODE_INFRA;
            break;
        case MLAN_BSS_MODE_IBSS:
            mode = IW_MODE_ADHOC;
            break;
        default:
            mode = IW_MODE_AUTO;
            break;
        }
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return mode;
}
/** 
 *  @brief Set encrypt mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param encrypt_mode         Encryption mode
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_encrypt_mode(moal_private * priv, t_u8 wait_option, t_u32 encrypt_mode)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    sec->param.encrypt_mode = encrypt_mode;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Get encrypt mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param encrypt_mode         A pointer to encrypt mode
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_encrypt_mode(moal_private * priv, t_u8 wait_option,
                      t_u32 * encrypt_mode)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS && encrypt_mode) {
        *encrypt_mode = sec->param.encrypt_mode;
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Set authentication mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param auth_mode            Authentication mode
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_auth_mode(moal_private * priv, t_u8 wait_option, t_u32 auth_mode)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_AUTH_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    sec->param.auth_mode = auth_mode;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Set wpa enable
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param enable               MTRUE or MFALSE
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_wpa_enable(moal_private * priv, t_u8 wait_option, t_u32 enable)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_WPA_ENABLED;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    sec->param.wpa_enabled = enable;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Get WPA enable
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param enable               A pointer to wpa enable status
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_wpa_enable(moal_private * priv, t_u8 wait_option, t_u32 * enable)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_WPA_ENABLED;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS && enable) {
        *enable = sec->param.wpa_enabled;
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Set wapi enable
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param enable               MTRUE or MFALSE
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_wapi_enable(moal_private * priv, t_u8 wait_option, t_u32 enable)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_WAPI_ENABLED;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    sec->param.wapi_enabled = enable;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}


/** 
 *  @brief Get RSSI info
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wait_option  Wait option
 *  @param signal       A pointer tp mlan_ds_get_signal structure
 *
 *  @return             MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_signal_info(moal_private * priv, t_u8 wait_option,
                     mlan_ds_get_signal * signal)
{
    int ret = 0;
    mlan_ds_get_info *info = NULL;
    mlan_ioctl_req *req = NULL;
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
    info->sub_command = MLAN_OID_GET_SIGNAL;
    info->param.signal.selector = ALL_RSSI_INFO_MASK;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (signal)
            ewl_os_memcpy(signal, &info->param.signal, sizeof(mlan_ds_get_signal));
        if (info->param.signal.selector & BCN_RSSI_AVG_MASK)
            priv->w_stats.qual.level = info->param.signal.bcn_rssi_avg;
        if (info->param.signal.selector & BCN_NF_AVG_MASK)
            priv->w_stats.qual.noise = info->param.signal.bcn_nf_avg;
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Change Adhoc Channel
 *   
 *  @param priv         A pointer to moal_private structure
 *  @param channel      The channel to be set. 
 *
 *  @return             MLAN_STATUS_SUCCESS--success, MLAN_STATUS_FAILURE--fail
 */
mlan_status
woal_change_adhoc_chan(moal_private * priv, int channel)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_bss_info bss_info;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    ewl_os_memset(&bss_info, 0, sizeof(bss_info));

    /* Get BSS information */
    if (MLAN_STATUS_SUCCESS !=
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info)) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    if (bss_info.bss_mode == MLAN_BSS_MODE_INFRA) {
        ret = MLAN_STATUS_SUCCESS;
        goto done;
    }

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Get current channel */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_IBSS_CHANNEL;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    if (bss->param.bss_chan.channel == (unsigned int) channel) {
        ret = MLAN_STATUS_SUCCESS;
        goto done;
    }
    PRINTM(MINFO, "Updating Channel from %d to %d\n",
           (int) bss->param.bss_chan.channel, channel);

    if (bss_info.media_connected != MTRUE) {
        ret = MLAN_STATUS_SUCCESS;
        goto done;
    }

    /* Do disonnect */
    bss->sub_command = MLAN_OID_BSS_STOP;
    ewl_os_memset((t_u8 *) & bss->param.bssid, 0, ETH_ALEN);

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Do specific SSID scanning */
    if (MLAN_STATUS_SUCCESS !=
        woal_request_scan(priv, MOAL_IOCTL_WAIT, &bss_info.ssid)) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    /* Start/Join Adhoc network */
    bss->sub_command = MLAN_OID_BSS_START;
    ewl_os_memset(&bss->param.ssid_bssid, 0, sizeof(mlan_ssid_bssid));
    ewl_os_memcpy(&bss->param.ssid_bssid.ssid, &bss_info.ssid,
           sizeof(mlan_802_11_ssid));

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = MLAN_STATUS_FAILURE;
    }

  done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return ret;
}

/** 
 *  @brief Get data rates
 *
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option   Wait option
 *  @param m_rates       A pointer to moal_802_11_rates structure
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_data_rates(moal_private * priv, t_u8 wait_option,
                    moal_802_11_rates * m_rates)
{
    int ret = 0;
    mlan_ds_rate *rate = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_rate));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    rate = (mlan_ds_rate *) req->pbuf;
    rate->sub_command = MLAN_OID_SUPPORTED_RATES;
    req->req_id = MLAN_IOCTL_RATE;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (m_rates)
            m_rates->num_of_rates =
                woal_copy_rates(m_rates->rates, m_rates->num_of_rates,
                                rate->param.rates, MLAN_SUPPORTED_RATES);
    }

done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}


/** 
 *  @brief Get channel list
 *
 *  @param priv            A pointer to moal_private structure
 *  @param wait_option     Wait option
 *  @param chan_list       A pointer to mlan_chan_list structure
 *
 *  @return                MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_channel_list(moal_private * priv, t_u8 wait_option,
                      mlan_chan_list * chan_list)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_CHANNEL_LIST;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (chan_list) {            
            ewl_os_memcpy(chan_list, &bss->param.chanlist, sizeof(mlan_chan_list));
        }
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}




/** 
 *  @brief This function sends customized event to application.
 *  
 *  @param priv    A pointer to moal_private structure
 *  @param str     A pointer to event string
 *
 *  @return        N/A
 */
void
woal_send_iwevcustom_event(moal_private * priv, t_s8 * str)
{
    MLAN_UNUSED_ARG(priv);
    MLAN_UNUSED_ARG(str);
#if 0    
    union iwreq_data iwrq;
    char buf[50];

    ENTER();

    ewl_os_memset(&iwrq, 0, sizeof(union iwreq_data));
    ewl_os_memset(buf, 0, sizeof(buf));
    
    ewl_os_snprintf(buf, sizeof(buf) - 1, "%s", str);

    iwrq.data.pointer = buf;
    iwrq.data.length = ewl_os_strlen(buf) + 1 + IW_EV_LCP_LEN;
    
    /* Send Event to upper layer */
    //wireless_send_event(priv, IWEVCUSTOM, &iwrq, buf);
    PRINTM(MINFO, "\n-->Wireless event %s is sent to application\n", str);
    LEAVE();
#endif    
    return;
}


/**
 *  @brief Set WPA passphrase and SSID
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to user data
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_passphrase(moal_private * priv, struct iwreq *wrq)
{
    t_u16 len = 0;
    static char buf[256];
    char *begin, *end, *opt;
    int ret = 0, action = -1, i;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;
    t_u8 zero_mac[] = { 0, 0, 0, 0, 0, 0 };
    t_u8 *mac = NULL;

    ENTER();
    
    if (!wrq->u.data.length) {
        PRINTM(MERROR, "Argument missing for setpassphrase\n");
        ret = -EINVAL;
        goto done;
    }
    ewl_os_memcpy(buf, wrq->u.data.pointer, wrq->u.data.length);
    buf[wrq->u.data.length] = '\0';

    if (wrq->u.data.length <= 1) {
        PRINTM(MERROR, "No valid arguments\n");
        ret = -EINVAL;
        goto done;
    }

    /* Parse the buf to get the cmd_action */
    begin = buf;
    end = woal_strsep(&begin, ';', '/');
    if (end)
        action = woal_atox(end);
        //printk("end %x \n", action);
    if (action < 0 || action > 2 || end[1] != '\0') {
        PRINTM(MERROR, "Invalid action argument %s\n", end);
        ret = -EINVAL;
        goto done;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    if (action == 0)
        req->action = MLAN_ACT_GET;
    else
        req->action = MLAN_ACT_SET;
    while (begin) {
        end = woal_strsep(&begin, ';', '/');
        opt = woal_strsep(&end, '=', '/');
        if (!opt || !end || !end[0]) {
            PRINTM(MERROR, "Invalid option\n");
            ret = -EINVAL;
            break;
        } else if (!ewl_os_strnicmp(opt, "ssid", ewl_os_strlen(opt))) {
            if (ewl_os_strlen(end) > MLAN_MAX_SSID_LENGTH) {
                PRINTM(MERROR, "SSID length exceeds max length\n");
                ret = -EFAULT;
                break;
            }
            sec->param.passphrase.ssid.ssid_len = ewl_os_strlen(end);
            ewl_os_strncpy((char *) sec->param.passphrase.ssid.ssid, end, ewl_os_strlen(end));
            PRINTM(MINFO, "ssid=%s, len=%d\n", sec->param.passphrase.ssid.ssid,
                   (int) sec->param.passphrase.ssid.ssid_len);
        } else if (!ewl_os_strnicmp(opt, "bssid", ewl_os_strlen(opt))) {
            woal_mac2u8((t_u8 *) & sec->param.passphrase.bssid, end);
        } else if (!ewl_os_strnicmp(opt, "psk", ewl_os_strlen(opt)) &&
                   req->action == MLAN_ACT_SET) {
            if (ewl_os_strlen(end) != MLAN_PMK_HEXSTR_LENGTH) {
                PRINTM(MERROR, "Invalid PMK length\n");
                ret = -EINVAL;
                break;
            }
            woal_ascii2hex(sec->param.passphrase.psk.pmk.pmk, end,
                           MLAN_PMK_HEXSTR_LENGTH);
            sec->param.passphrase.psk_type = MLAN_PSK_PMK;
        } else if (!ewl_os_strnicmp(opt, "passphrase", ewl_os_strlen(opt)) &&
                   req->action == MLAN_ACT_SET) {
            if (ewl_os_strlen(end) < MLAN_MIN_PASSPHRASE_LENGTH ||
                ewl_os_strlen(end) > MLAN_MAX_PASSPHRASE_LENGTH) {
                PRINTM(MERROR, "Invalid length for passphrase\n");
                ret = -EINVAL;
                break;
            }
            sec->param.passphrase.psk_type = MLAN_PSK_PASSPHRASE;
            ewl_os_strcpy(sec->param.passphrase.psk.passphrase.passphrase, end);
            sec->param.passphrase.psk.passphrase.passphrase_len = ewl_os_strlen(end);
            PRINTM(MINFO, "passphrase=%s, len=%d\n",
                   sec->param.passphrase.psk.passphrase.passphrase,
                   (int) sec->param.passphrase.psk.passphrase.passphrase_len);
        } else {
            PRINTM(MERROR, "Invalid option %s\n", opt);
            ret = -EINVAL;
            break;
        }
    }
    if (ret)
        goto done;

    if (action == 2)
        sec->param.passphrase.psk_type = MLAN_PSK_CLEAR;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (action == 0) {
        ewl_os_memset(buf, 0, sizeof(buf));
        if (sec->param.passphrase.ssid.ssid_len) {
            len += sprintf(buf + len, "ssid:");
            ewl_os_memcpy(buf + len, sec->param.passphrase.ssid.ssid,
                   sec->param.passphrase.ssid.ssid_len);
            len += sec->param.passphrase.ssid.ssid_len;
            len += sprintf(buf + len, " ");
        }
        if (ewl_os_memcmp(&sec->param.passphrase.bssid, zero_mac, sizeof(zero_mac))) {
            mac = (t_u8 *) & sec->param.passphrase.bssid;
            len += sprintf(buf + len, "bssid:");
            for (i = 0; i < ETH_ALEN - 1; ++i)
                len += sprintf(buf + len, "%02x:", mac[i]);
            len += sprintf(buf + len, "%02x ", mac[i]);
        }
        if (sec->param.passphrase.psk_type == MLAN_PSK_PMK) {
            len += sprintf(buf + len, "psk:");
            for (i = 0; i < MLAN_MAX_KEY_LENGTH; ++i)
                len +=
                    sprintf(buf + len, "%02x",
                            sec->param.passphrase.psk.pmk.pmk[i]);
            len += sprintf(buf + len, "\n");
        }
        if (sec->param.passphrase.psk_type == MLAN_PSK_PASSPHRASE) {
            len +=
                sprintf(buf + len, "passphrase:%s \n",
                        sec->param.passphrase.psk.passphrase.passphrase);
        }
        if (wrq->u.data.pointer) {
            if (ewl_os_memcpy(wrq->u.data.pointer, buf, len)) {
                PRINTM(MERROR, "Copy to user failed, len %d\n", len);
                ret = -EFAULT;
                goto done;
            }
            wrq->u.data.length = len;
        }

    }
  done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return ret;
}


/**
 *  @brief Set/Get Usr 11n configuration request
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
int
woal_11n_htcap_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data[2];
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;
    int data_length = wrq->u.data.length;

    ENTER();

    if (data_length > 2) {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto done;
    }

    if (((req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg))) == NULL)) {
        ret = -ENOMEM;
        goto done;
    }

    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_HTCAP_CFG;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (data_length == 0) {
        /* Get 11n tx parameters from MLAN */
        req->action = MLAN_ACT_GET;
        cfg_11n->param.htcap_cfg.misc_cfg = BAND_SELECT_BG;
    } else {
        if (ewl_os_memcpy
            (data, wrq->u.data.pointer, data_length * sizeof(int))) {
            PRINTM(MERROR, "ewl_os_memcpy failed\n");
            ret = -EFAULT;
            goto done;
        }

        cfg_11n->param.htcap_cfg.htcap = data[0];
        PRINTM(MINFO, "SET: htcapinfo:0x%x\n", data[0]);
        cfg_11n->param.htcap_cfg.misc_cfg = BAND_SELECT_BOTH;
        if (data_length == 2) {
            if (data[1] != BAND_SELECT_BG &&
                data[1] != BAND_SELECT_A && data[1] != BAND_SELECT_BOTH) {
                PRINTM(MERROR, "Invalid band selection\n");
                ret = -EINVAL;
                goto done;
            }
            cfg_11n->param.htcap_cfg.misc_cfg = data[1];
            PRINTM(MINFO, "SET: htcapinfo band:0x%x\n", data[1]);
        }
        /* Update 11n tx parameters in MLAN */
        req->action = MLAN_ACT_SET;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    data[0] = cfg_11n->param.htcap_cfg.htcap;

    if (req->action == MLAN_ACT_GET) {
        data_length = 1;
        cfg_11n->param.htcap_cfg.htcap = 0;
        cfg_11n->param.htcap_cfg.misc_cfg = BAND_SELECT_A;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
        if (cfg_11n->param.htcap_cfg.htcap != data[0]) {
            data_length = 2;
            data[1] = cfg_11n->param.htcap_cfg.htcap;
            PRINTM(MINFO, "GET: htcapinfo for 2.4GHz:0x%x\n", data[0]);
            PRINTM(MINFO, "GET: htcapinfo for 5GHz:0x%x\n", data[1]);
        } else
            PRINTM(MINFO, "GET: htcapinfo:0x%x\n", data[0]);
    }

    if (ewl_os_memcpy(wrq->u.data.pointer, data, sizeof(data))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

    wrq->u.data.length = data_length;

  done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return ret;
}

/**
 *  @brief ioctl function - entry point
 *
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @param cmd      Command
 *
 *  @return          0 --success, otherwise fail
 *  we implements minimum ioctl.
 */
int
woal_do_ioctl(moal_private * priv, struct ifreq *req, int cmd)
{
    struct iwreq *wrq = (struct iwreq *) req;
    int ret = 0;

    ENTER();

    PRINTM(MINFO, "woal_do_ioctl: ioctl cmd = 0x%x\n", cmd);
    switch (cmd) {
    case WOAL_SET_GET_256_CHAR:
        switch (wrq->u.data.flags) {
        case WOAL_PASSPHRASE:
            ret = woal_passphrase(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;
    }    
    LEAVE();
    return ret;
}
#endif
