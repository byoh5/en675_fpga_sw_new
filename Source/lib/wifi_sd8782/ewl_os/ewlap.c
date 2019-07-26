#include <string.h>

#include "moal_main.h"
#include "moal_uap.h"
#include "moal_uap_priv.h"
#include "ewl_os.h"
#include "if.h"
//#include "mlan_fw.h"
//#include "mlan_ieee.h"
#include "uaputl.h"
#include "wlif_tools.h"

#define DEFAULT_AP_SSID "u2ap"

void *
ewl_get_uap_handle(void);

static mlan_uap_bss_param apcfg;



/**
	initialize apcfg.
	set default function for AP
	-default setting
	 ssid 			: DEFAULT_AP_SSID
	 security		: open
	 ch(frequency) 	: 6	
	 
	@return         0 --success, otherwise fail
*/
int 
ewlap_init_apcfg(char *ssid)
{
    int ret = 0;	
    moal_private *priv = (moal_private *)ewl_get_uap_handle();
    ewl_os_memset(&apcfg,0,sizeof(mlan_uap_bss_param));

    /* Initialize the uap bss values which are uploaded from firmware */
    woal_uap_get_bss_param(priv, &apcfg, MOAL_IOCTL_WAIT);
    
    /* Setting the default values */
    apcfg.channel = WIFI_AP_CH;
    apcfg.preamble_type = 0;
	apcfg.ssid.ssid_len = ewl_os_strlen(ssid);
    ewl_os_strncpy((char *) apcfg.ssid.ssid, 
    				ssid, 
    				ewl_os_strlen(ssid));

    /* If the security mode is configured as WEP or WPA-PSK, it will disable
       11n automatically, and if configured as open(off) or wpa2-psk, it will
       automatically enable 11n */
    if ((apcfg.protocol == PROTOCOL_STATIC_WEP) ||
        (apcfg.protocol == PROTOCOL_WPA)) {
        if (MLAN_STATUS_SUCCESS !=
            woal_uap_set_11n_status(&apcfg, MLAN_ACT_DISABLE)) {
            ret = -EFAULT;
            goto done;
        }
    } else {
        if (MLAN_STATUS_SUCCESS !=
            woal_uap_set_11n_status(&apcfg, MLAN_ACT_ENABLE)) {
            ret = -EFAULT;
            goto done;
        }
    }
    
    if (MLAN_STATUS_SUCCESS !=
        woal_set_get_sys_config(priv, MLAN_ACT_SET, &apcfg)) {
        ret = -EFAULT;
    }
done: 
    return ret;			
}
    
/**
	Configure soft AP 
	However, This config is not downloaded to firmware of wifi.

    To set channel is best affort. 
    If sta mode is conneted to AP with another channel(frequency) 
	AP channel Should be  STA's channel and can't be changed.
	
	@name  : soft AP property 	    
	@value : soft AP property value
   +-----------+-----------------------+--------------------------------+ 
   | name      | value                 |                                | 
   +-----------+-----------------------+--------------------------------+
   | "SSID"    |                       |                                |
   | "SEC"     |"open" or "wpa2-psk"   | authentication protocol        |
   | "KEY"     |"passphrase"           | if wpa2-psk                    |  
   | "CHANNEL" |"1 ~ 13"               |                                |
   | "MAX_SCB  |"1~10"                 | number of sta to be connected. |
   +--------------------------------------------------------------------+
   
	@return         0 --success, otherwise fail
*/
int
ewlap_apcfg(char *name, char *value)
{
    int ret = 0;
    
	if (!ewl_os_strncmp(name, WIFI_UAP_SET_CMD_SSID, ewl_os_strlen(WIFI_UAP_SET_CMD_SSID)))
	{
        if (ewl_os_strlen(value) > MLAN_MAX_SSID_LENGTH) {
            PRINTM(MERROR, "SSID length exceeds max length\r\n");
            ret = -EFAULT;
            goto done;
        }
        apcfg.ssid.ssid_len = ewl_os_strlen(value);
        ewl_os_strncpy(apcfg.ssid.ssid, value, ewl_os_strlen(value));
        PRINTM(MINFO, "ssid=%s, len=%d\r\n", apcfg.ssid.ssid,(int)apcfg.ssid.ssid_len);
	}
	else if (!ewl_os_strncmp(name, WIFI_UAP_SET_CMD_SEC, ewl_os_strlen(WIFI_UAP_SET_CMD_SEC))) 
	{
        if (!ewl_os_strnicmp(value, "open", ewl_os_strlen("open"))) 
        {
            apcfg.auth_mode = MLAN_AUTH_MODE_OPEN;
            apcfg.wpa_cfg.length = 0;
            apcfg.key_mgmt = KEY_MGMT_NONE;
            apcfg.protocol = PROTOCOL_NO_SECURITY;
        } 
        else if (!ewl_os_strnicmp(value, "wpa2-psk", ewl_os_strlen("wpa2-psk"))) 
        {
            apcfg.auth_mode = MLAN_AUTH_MODE_OPEN;
            apcfg.protocol = PROTOCOL_WPA2;
            apcfg.key_mgmt = KEY_MGMT_PSK;
            apcfg.wpa_cfg.pairwise_cipher_wpa = CIPHER_AES_CCMP;
            apcfg.wpa_cfg.pairwise_cipher_wpa2 = CIPHER_AES_CCMP;
            apcfg.wpa_cfg.group_cipher = CIPHER_AES_CCMP;
        } 
#if 0
        else if (!strnicmp(value, "wpa-psk", strlen("wpa-psk"))) 
        {
            apcfg.auth_mode = MLAN_AUTH_MODE_OPEN;
            apcfg.protocol = PROTOCOL_WPA;
            apcfg.key_mgmt = KEY_MGMT_PSK;
            apcfg.wpa_cfg.pairwise_cipher_wpa = CIPHER_TKIP;
            apcfg.wpa_cfg.group_cipher = CIPHER_TKIP;
            
        } else if (!strnicmp(value, "wep128", strlen("wep128"))) 
        {
            apcfg.auth_mode = MLAN_AUTH_MODE_OPEN;
            apcfg.wpa_cfg.length = 0;
            apcfg.key_mgmt = KEY_MGMT_NONE;
            apcfg.protocol = PROTOCOL_STATIC_WEP;
        }
#endif
        else 
        {
            PRINTM(MERROR, "APCFG: Invalid value=%s for %s\r\n", value, name);
            ret = -EFAULT;
            goto done;
        }
    }
    else if (!ewl_os_strncmp(name, WIFI_UAP_SET_CMD_KEY, ewl_os_strlen(WIFI_UAP_SET_CMD_KEY))) 
    {
        if (ewl_os_strlen(value) < MLAN_MIN_PASSPHRASE_LENGTH ||
            ewl_os_strlen(value) > MLAN_PMK_HEXSTR_LENGTH) 
        {
            PRINTM(MERROR, "Invalid PSK/PMK length\r\n");
            ret = -EINVAL;
            goto done;
        }
        apcfg.wpa_cfg.length = ewl_os_strlen(value);
        ewl_os_memcpy(apcfg.wpa_cfg.passphrase, value, ewl_os_strlen(value));
        apcfg.wpa_cfg.passphrase[apcfg.wpa_cfg.length] = 0;
    }
    else if (!ewl_os_strncmp(name, WIFI_UAP_SET_CMD_CHANNEL, ewl_os_strlen(WIFI_UAP_SET_CMD_CHANNEL))) 
    {
        int atoi_ret=0;
        
        if (woal_atoi(&atoi_ret, value)) {
            ret = -EINVAL;
            goto done;
        }
        
       if (atoi_ret < 1 || atoi_ret > MLAN_MAX_CHANNEL) {
            PRINTM(MERROR, "APCFG: Channel must be between 1 and %d"
                   "(both included)\r\n", MLAN_MAX_CHANNEL);
            ret = -EINVAL;
            goto done;
        }
        apcfg.channel = atoi_ret;
    }
    else if (!ewl_os_strncmp(name, WIFI_UAP_SET_CMD_MAX_SCB, ewl_os_strlen(WIFI_UAP_SET_CMD_MAX_SCB))) 
    {
        int atoi_ret=0;
        
        if (woal_atoi(&atoi_ret, value)) {
            ret = -EINVAL;
            goto done;
        }

        if (atoi_ret < 1 || atoi_ret > MAX_STA_COUNT) {
            PRINTM(MERROR, "APCFG: MAX_SCB must be between 1 to %d "
                   "(both included)\r\n", MAX_STA_COUNT);
            ret = -EINVAL;
            goto done;
        }
        apcfg.max_sta_count = (t_u16)atoi_ret;
    }
    else 
    {
        PRINTM(MERROR, "Invalid option %s\r\n", name);
        ret = -EINVAL;
        goto done;
    }    
    
done:
    LEAVE();
    return ret;	
}

/**
    set current AP config to firware of wifi.
    and if AP is started.then AP is restarted.
    If you change your ap setting, 
    you should use this function with ewlap_apcfg.

    @return         0 --success, otherwise fail
*/
#if 0
int 
ewlap_set_apcfg(void)
{
    int restart = 0;
    int ret =0;
    
    moal_private *priv = (moal_private *)ewl_get_uap_handle();
    
    /* If BSS already started stop it first and restart after changing the
       setting */
    if (priv->bss_started == MTRUE) {
        if ((ret = woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_STOP)))
            goto done;
        restart = 1;
    }
    
    /* If the security mode is configured as WEP or WPA-PSK, it will disable
       11n automatically, and if configured as open(off) or wpa2-psk, it will
       automatically enable 11n */
    if ((apcfg.protocol == PROTOCOL_STATIC_WEP) ||
        (apcfg.protocol == PROTOCOL_WPA)) {
        if (MLAN_STATUS_SUCCESS !=
            woal_uap_set_11n_status(&apcfg, MLAN_ACT_DISABLE)) {
            ret = -EFAULT;
            goto done;
        }
    } else {
        if (MLAN_STATUS_SUCCESS !=
            woal_uap_set_11n_status(&apcfg, MLAN_ACT_ENABLE)) {
            ret = -EFAULT;
            goto done;
        }
    }    
    
    if (MLAN_STATUS_SUCCESS !=
        woal_set_get_sys_config(priv, MLAN_ACT_SET, &apcfg)) {
        ret = -EFAULT;
        goto done;
    }    

    /* Start the BSS after successful configuration */
    if (restart)
        ret = woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_START);
done:
    return ret;
}
#else
int 
ewlap_set_apcfg(void)
{
    int restart = 0;
    int ret =0;
    
    moal_private *priv = (moal_private *)ewl_get_uap_handle();
    
    /* If BSS already started stop it first and restart after changing the
       setting */
    if (priv->bss_started == MTRUE) {
        if ((ret = woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_STOP)))
        	{
            goto done;
        	}
        restart = 1;
    }
    /* If the security mode is configured as WEP or WPA-PSK, it will disable
       11n automatically, and if configured as open(off) or wpa2-psk, it will
       automatically enable 11n */
    if ((apcfg.protocol == PROTOCOL_STATIC_WEP) ||
        (apcfg.protocol == PROTOCOL_WPA)) {
        if (MLAN_STATUS_SUCCESS !=
            woal_uap_set_11n_status(&apcfg, MLAN_ACT_DISABLE)) {
            ret = -EFAULT;
            goto done;
        }
    } else {
        if (MLAN_STATUS_SUCCESS !=
            woal_uap_set_11n_status(&apcfg, MLAN_ACT_ENABLE)) {
            ret = -EFAULT;
            goto done;
        }
    }    
    
    if (MLAN_STATUS_SUCCESS !=
        woal_set_get_sys_config(priv, MLAN_ACT_SET, &apcfg)) {
        ret = -EFAULT;
        goto done;
    }    

    /* Start the BSS after successful configuration */
    if (restart)
    	{
        ret = woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_START);
    	}
done:
    return ret;
}
#endif

/**
	start SW AP 
	
	@return          0 --success, otherwise fail
*/
int 
ewlap_bss_start(void)
{
	moal_private *priv = (moal_private *)ewl_get_uap_handle();
	return 	woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_START);
}

/**
    Stop bss
    
	@return         0--success, otherwise fail
*/
int 
ewlap_bss_stop(void)
{
    moal_private *priv = (moal_private *)ewl_get_uap_handle();
	return 	woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_STOP);	
}

int 
ewlap_bss_reset(void)
{
    moal_private *priv = (moal_private *)ewl_get_uap_handle();
	return 	woal_uap_bss_ctrl(priv, MOAL_IOCTL_WAIT, UAP_BSS_RESET);	
}



/**
    Get current concise AP cfg.

	@cfg : concise ap configuration information 
	       should be allocated.
	@return         0--success, otherwise fail
*/
int 
ewlap_get_apcfg(struct ewlap_cfg *cfg)
{
 	moal_private *priv = (moal_private *)ewl_get_uap_handle();
 	mlan_uap_bss_param ap_cfg;

	ENTER();

    if (MLAN_STATUS_SUCCESS !=
        woal_set_get_sys_config(priv, MLAN_ACT_GET, &ap_cfg)) {
        PRINTM(MERROR, "Error getting AP confiruration\r\n");
        LEAVE();
        return -EFAULT;
    }

    ewl_os_strncpy(cfg->ssid, ap_cfg.ssid.ssid, ap_cfg.ssid.ssid_len);
	cfg->ssid[ap_cfg.ssid.ssid_len] = '\0';
    
    if(ap_cfg.protocol == PROTOCOL_WPA2)
    {
        cfg->enc_proto = 1;
        ewl_os_strncpy(cfg->passphrase,
                       ap_cfg.wpa_cfg.passphrase,
                       ap_cfg.wpa_cfg.length);
        cfg->passphrase[ap_cfg.wpa_cfg.length]=0;
    }
    else if (ap_cfg.protocol == PROTOCOL_NO_SECURITY)
        cfg->enc_proto = 0;
        
    cfg->channel       = ap_cfg.channel;
    cfg->max_sta_count = ap_cfg.max_sta_count;
    
    //ewl_os_printf("%s, channel %d\r\n", __func__, ap_cfg.channel);
	
	LEAVE();
	return 0; 
}


/** 
 *  @brief uap get station list handler
 *   
 *  @plist : information for sta list connected to current AP
 *  @
 *  @return         0 --success, otherwise fail
 */
int
ewlap_get_sta_list(ewl_sta_list *plist)
{
    int ret = 0;
    moal_private *priv = (moal_private *)ewl_get_uap_handle();
    mlan_ds_get_info *info = NULL;
    mlan_ioctl_req *ioctl_req = NULL;

    ENTER();

    /* Sanity check */
    if (plist == NULL) {
        PRINTM(MERROR, "ewlap_get_sta_list() corrupt data\r\n");
        ret = -EFAULT;
        goto done;
    }

    /* Allocate an IOCTL request buffer */
    ioctl_req =
        (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    info = (mlan_ds_get_info *) ioctl_req->pbuf;
    info->sub_command = MLAN_OID_UAP_STA_LIST;
    ioctl_req->req_id = MLAN_IOCTL_GET_INFO;
    ioctl_req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (ioctl_req->action == MLAN_ACT_GET) {
        /* Copy to user : sta_list */
        ewl_os_memcpy
            (plist, &info->param.sta_list, sizeof(mlan_ds_sta_list));
    }
done:
    if (ioctl_req)
        ewl_os_free(ioctl_req);
    LEAVE();
    return ret;
}

/**
    @@return : 1 = connected,
               0 = is not 
*/

int 
ewlap_is_connected(void)
{
    moal_private *priv = (moal_private *)ewl_get_uap_handle();    
    return priv->bss_started;
}

/**

*/
int
uap_ioctl(t_u8 * cmd, t_u16 * size, t_u16 buf_size)
{
    moal_private *priv = (moal_private *)ewl_get_uap_handle();
    struct ifreq ifr;
    apcmdbuf *header = NULL;

    if (buf_size < *size) {
        ewl_os_printf("buf_size should not less than cmd buffer size\r\n");
        return UAP_FAILURE;
    }

    *(t_u32 *) cmd = buf_size - BUF_HEADER_SIZE;

    /* Initialize the ifr structure */
    ewl_os_memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_ifru.ifru_data = (void *) cmd;
    header = (apcmdbuf *) cmd;

    header->size = *size - BUF_HEADER_SIZE;

    if (header->cmd_code == APCMD_SYS_CONFIGURE) {
        apcmdbuf_sys_configure *sys_cfg;
        sys_cfg = (apcmdbuf_sys_configure *) cmd;
        sys_cfg->action = wlan_cpu_to_le16(sys_cfg->action);
    }

    endian_convert_request_header(header);

    woal_hostcmd_ioctl(priv, &ifr);

    endian_convert_response_header(header);
    header->cmd_code &= HostCmd_CMD_ID_MASK;
    header->cmd_code |= APCMD_RESP_CHECK;
    *size = header->size;    

    /* Validate response size */
    if (*size > (buf_size - BUF_HEADER_SIZE)) {
        ewl_os_printf
            ("ERR:Response size (%d) greater than buffer size (%d)! Aborting!\r\n",
             *size, buf_size);
        return UAP_FAILURE;
    }

    return UAP_SUCCESS;
    
}

#if 0

/**
 *  @brief  Get Ht capability Info from firmware
 *
 *  @param  pHtCap  A pointer to HTCap_t structure
 *  @return         UAP_SUCCESS/UAP_FAILURE
 *
 *
    default value 
    HT Capability Info:
    802.11n is enabled
    ht_cap_info=0x110c, ampdu_param=0x3 tx_bf_cap=0

 */
int
get_sys_cfg_11n(HTCap_t * pHtCap)
{
    apcmdbuf_sys_configure *cmd_buf = NULL;
    tlvbuf_htcap_t *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len;
    int ret = UAP_FAILURE;
    /* Initialize the command length */
    cmd_len = sizeof(apcmdbuf_sys_configure) + sizeof(tlvbuf_htcap_t);
    /* Initialize the command buffer */
    buffer = (t_u8 *) ewl_os_malloc(cmd_len);
    if (!buffer) {
        printf("ERR:Cannot allocate buffer for command!\r\n");
        return -1;
    }
    ewl_os_memset(buffer, 0, cmd_len);
    /* Locate headers */
    cmd_buf = (apcmdbuf_sys_configure *) buffer;
    tlv = (tlvbuf_htcap_t *) (buffer + sizeof(apcmdbuf_sys_configure));

    /* Fill the command buffer */
    cmd_buf->cmd_code = APCMD_SYS_CONFIGURE;
    cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
    cmd_buf->seq_num = 0;
    cmd_buf->result = 0;
    tlv->tag = HT_CAPABILITY_TLV_ID;
    cmd_buf->action = ACTION_GET;
    tlv->length = sizeof(HTCap_t);

    endian_convert_tlv_header_out(tlv);
    /* Send the command */
    ret = uap_ioctl((t_u8 *) cmd_buf, &cmd_len, cmd_len);
    endian_convert_tlv_header_in(tlv);
    /* Process response */
    if (ret == UAP_SUCCESS) {
        /* Verify response */
        if ((cmd_buf->cmd_code != (APCMD_SYS_CONFIGURE | APCMD_RESP_CHECK)) ||
            (tlv->tag != HT_CAPABILITY_TLV_ID)) {
            printf("ERR:Corrupted response! cmd_code=%x, Tlv->tag=%x\r\n",
                   cmd_buf->cmd_code, tlv->tag);
            ewl_os_free(buffer);
            return UAP_FAILURE;
        }
        /* Copy response */
        if (cmd_buf->result == CMD_SUCCESS) {
            ewl_os_memcpy(pHtCap, &tlv->ht_cap, sizeof(HTCap_t));
            pHtCap->ht_cap_info = uap_le16_to_cpu(pHtCap->ht_cap_info);
            pHtCap->tx_bf_cap = uap_le32_to_cpu(pHtCap->tx_bf_cap);
            ret = UAP_SUCCESS;
        } else {
            ret = UAP_FAILURE;
            printf("ERR:Could not get HT capability!\r\n");
        }
    } else {
        printf("ERR:Command sending failed!\r\n");
        ret = UAP_FAILURE;
    }
    
    if (buffer)
        ewl_os_free(buffer);
    return ret;
}

/**
    @parm enable : 0  = 11n disable
                   1  = 11n enable 
*/

int
set_sys_cfg_11n(int enable)
{
    apcmdbuf_sys_configure *cmd_buf = NULL;
    tlvbuf_htcap_t *tlv = NULL;
    HTCap_t htcap;
    t_u8 *buffer = NULL;
    t_u16 cmd_len;
    int ret = UAP_FAILURE;
    /* Initialize the command length */
    cmd_len = sizeof(apcmdbuf_sys_configure) + sizeof(tlvbuf_htcap_t);
    /* Initialize the command buffer */
    buffer = (t_u8 *) ewl_os_malloc(cmd_len);
    if (!buffer) {
        printf("ERR:Cannot allocate buffer for command!\r\n");
        return -1;
    }
    ewl_os_memset(buffer, 0, cmd_len);
    /* Locate headers */
    cmd_buf = (apcmdbuf_sys_configure *) buffer;
    tlv = (tlvbuf_htcap_t *) (buffer + sizeof(apcmdbuf_sys_configure));

    /* Fill the command buffer */
    cmd_buf->cmd_code = APCMD_SYS_CONFIGURE;
    cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
    cmd_buf->seq_num = 0;
    cmd_buf->result = 0;
    tlv->tag = HT_CAPABILITY_TLV_ID;
    cmd_buf->action = ACTION_SET;
    tlv->length = sizeof(HTCap_t);
    
    memset(&htcap, 0, sizeof(htcap));
    if (UAP_FAILURE == get_sys_cfg_11n(&htcap)) {
        printf("Fail to get 11n parameters from firmware\r\n");
        return UAP_FAILURE;
    }
    if (!htcap.supported_mcs_set[0]) {
        printf("802.11n is disabled\r\n");        
    }        
    else
    {
        printf("802.11n is enabled\r\n");
        printf("ht_cap_info=0x%x, ampdu_param=0x%x tx_bf_cap=%x\r\n",
           htcap.ht_cap_info,
           htcap.ampdu_param,
           htcap.tx_bf_cap);
    }
    
    
    if(enable == 0)
    {
        memcpy(&tlv->ht_cap, &htcap, sizeof(HTCap_t));
        /* disable mcs rate */
        tlv->ht_cap.supported_mcs_set[0] = 0;
        tlv->ht_cap.supported_mcs_set[4] = 0;        
    }
    else
    {
        /* enable 802.11n */
        memcpy(&tlv->ht_cap, &htcap, sizeof(HTCap_t));
        /* enable mcs rate */
        tlv->ht_cap.supported_mcs_set[0] = DEFAULT_MCS_SET_0;
        tlv->ht_cap.supported_mcs_set[4] = DEFAULT_MCS_SET_4;        
    }
    
    endian_convert_tlv_header_out(tlv);
    /* Send the command */
    ret = uap_ioctl((t_u8 *) cmd_buf, &cmd_len, cmd_len);
    endian_convert_tlv_header_in(tlv);
    /* Process response */
    if (ret == UAP_SUCCESS) {
        /* Verify response */
        if ((cmd_buf->cmd_code != (APCMD_SYS_CONFIGURE | APCMD_RESP_CHECK)) ||
            (tlv->tag != HT_CAPABILITY_TLV_ID)) {
            printf("ERR:Corrupted response! cmd_code=%x, Tlv->tag=%x\r\n",
                   cmd_buf->cmd_code, tlv->tag);
            ewl_os_free(buffer);
            return UAP_FAILURE;
        }
        /* Copy response */
        if (cmd_buf->result == CMD_SUCCESS) {
            printf("Configure 802.11n parameters successful\r\n");
            ret = UAP_SUCCESS;
        } else {
            ret = UAP_FAILURE;
            printf("ERR:Could not get HT capability!\r\n");
        }
    } else {
        printf("ERR:Command sending failed!\r\n");
        ret = UAP_FAILURE;
    }
    
    if (buffer)
        ewl_os_free(buffer);
    return ret;
}

/**
 *  @brief  Get Ht capability Info from firmware
 *
 *  @param  pHtCap  A pointer to HTCap_t structure
 *  @return         
        -1 : failed
        else 
        UAP_SUCCESS
            struct ifreq *req;  

    LENGTH =4
    CMD HDR
    - cmd_code
    - size
    - seq
    - result
    CMD BODY
    TLV
    - TLV HDR
    - CONTENT

 */

 
int 
ewlap_get_cfg_11n(void)
{
    HTCap_t HtCap;
    get_sys_cfg_11n(&HtCap); 

    if (!HtCap.supported_mcs_set[0]) {
        printf("802.11n is disabled\r\n");        
    }        
    else
    {
        printf("802.11n is enabled\r\n");
        printf("ht_cap_info=0x%x, ampdu_param=0x%x tx_bf_cap=%x\r\n",
           HtCap.ht_cap_info,
           HtCap.ampdu_param,
           HtCap.tx_bf_cap);
    }
    return 0;
}

int
woal_uap_get_freq(moal_private *priv, struct iw_freq *fwrq);
#endif
int
ewl_uap_get_freq(void)
{
    struct iw_freq fwrq;
    moal_private * priv = (moal_private *)ewl_get_uap_handle();
    woal_uap_get_freq(priv, &fwrq);
	ewl_os_printf("freq(%d), e(%d), ch(%d), flags(0x%x)\r\n", fwrq.m, fwrq.e, fwrq.i, fwrq.flags);
	return 0;
}
