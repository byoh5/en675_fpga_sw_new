/** @file  moal_wext.c
  *
  * @brief This file contains wireless extension standard ioctl functions
  *
  * Copyright (C) 2008-2011, Marvell International Ltd.
  *
  * This software file (the "File") is distributed by Marvell International
  * Ltd. under the terms of the GNU General Public License Version 2, June 1991
  * (the "License").  You may use, redistribute and/or modify this File in
  * accordance with the terms and conditions of the License, a copy of which
  * is available by writing to the Free Software Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
  * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
  *
  * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
  * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
  * this warranty disclaimer.
  *
  */

/* ------------------------ OR32 includes -------------------------------- */
#include "stdio.h"

#include "ewl_os.h"
#include <errno.h> //for ENOMEM
#include "ewl.h"
                     
#include "wireless.h"                                                         
#include "moal_main.h"                          
#include "moal_priv.h"                          
#include "moal_util.h"                          
#include "moal_wext.h" 
#include "wext.h"
#include "iw_handler.h"  

#ifdef STA_SUPPORT
/** Approximate amount of data needed to pass a scan result back to iwlist */
#define MAX_SCAN_CELL_SIZE  (IW_EV_ADDR_LEN           \
                             + MLAN_MAX_SSID_LENGTH   \
                             + IW_EV_UINT_LEN         \
                             + IW_EV_FREQ_LEN         \
                             + IW_EV_QUAL_LEN         \
                             + MLAN_MAX_SSID_LENGTH   \
                             + IW_EV_PARAM_LEN        \
                             + 40)      /* 40 for WPAIE */
                                               
/********************************************************
                Local Functions
********************************************************/
/**
 *  @brief This function validates a SSID as being able to be printed
 *
 *  @param pssid   SSID structure to validate
 *
 *  @return        MTRUE or MFALSE
 */
static BOOLEAN
woal_ssid_valid(mlan_802_11_ssid * pssid)
{
    unsigned int ssid_idx;

    ENTER();

    for (ssid_idx = 0; ssid_idx < pssid->ssid_len; ssid_idx++) {
        if ((pssid->ssid[ssid_idx] < 0x20) || (pssid->ssid[ssid_idx] > 0x7e)) {
            LEAVE();
            return MFALSE;
        }
    }
    LEAVE();
    return MTRUE;
}


/**
 *  @brief Sort Channels
 *
 *  @param freq                 A pointer to iw_freq structure
 *  @param num                  Number of Channels
 *
 *  @return                     N/A
 */
static inline void
woal_sort_channels(struct iw_freq *freq, int num)
{
    int i, j;
    struct iw_freq temp;

    for (i = 0; i < num; i++)
        for (j = i + 1; j < num; j++)
            if (freq[i].i > freq[j].i) {
                temp.i = freq[i].i;
                temp.m = freq[i].m;

                freq[i].i = freq[j].i;
                freq[i].m = freq[j].m;

                freq[j].i = temp.i;
                freq[j].m = temp.m;
            }
}

/**
 *  @brief Compare two SSIDs
 *
 *  @param ssid1    A pointer to ssid to compare
 *  @param ssid2    A pointer to ssid to compare
 *
 *  @return         0--ssid is same, otherwise is different
 */
t_s32
woal_ssid_cmp(mlan_802_11_ssid * ssid1, mlan_802_11_ssid * ssid2)
{
    ENTER();

    if (!ssid1 || !ssid2) {
        LEAVE();
        return -1;
    }
    if (ssid1->ssid_len != ssid2->ssid_len) {
        LEAVE();
        return -1;
    }

    LEAVE();
    return ewl_os_memcmp(ssid1->ssid, ssid2->ssid, ssid1->ssid_len);
}

/**
 *  @brief Convert RSSI to quality
 *
 *  @param rssi     RSSI in dBm
 *
 *  @return         Quality of the link (0-5)
 */
static t_u8
woal_rssi_to_quality(t_s16 rssi)
{
/** Macro for RSSI range */
#define MOAL_RSSI_NO_SIGNAL -90
#define MOAL_RSSI_VERY_LOW  -80
#define MOAL_RSSI_LOW       -70
#define MOAL_RSSI_GOOD      -60
#define MOAL_RSSI_VERY_GOOD -50
#define MOAL_RSSI_INVALID   0
    if (rssi <= MOAL_RSSI_NO_SIGNAL || rssi == MOAL_RSSI_INVALID)
        return 0;
    else if (rssi <= MOAL_RSSI_VERY_LOW)
        return 1;
    else if (rssi <= MOAL_RSSI_LOW)
        return 2;
    else if (rssi <= MOAL_RSSI_GOOD)
        return 3;
    else if (rssi <= MOAL_RSSI_VERY_GOOD)
        return 4;
    else
        return 5;
}

#if 0
/**
 *  @brief Scan Network
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param vwrq         A pointer to iw_param structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return             0--success, otherwise fail
 */
int
woal_set_scan(moal_private *priv)
{
    int ret = 0;
#ifdef REASSOCIATION
    moal_handle *handle = priv->phandle;
#endif
    mlan_802_11_ssid req_ssid;
    ENTER();
    if (priv->scan_pending_on_block == MTRUE) {
        PRINTM(MINFO, "scan already in processing...\n");
        LEAVE();
        return ret;
    }
    if ((priv->media_connected == MTRUE) && (priv->scan_block_flag == MTRUE)) {
        PRINTM(MIOCTL, "scan block on\n");
        LEAVE();
        return ret;
    }
    priv->report_scan_result = MTRUE;
    ewl_os_memset(&req_ssid, 0x00, sizeof(mlan_802_11_ssid));    
    if (MLAN_STATUS_SUCCESS != woal_request_scan(priv, MOAL_IOCTL_WAIT, NULL)) 
    {
        ret = -1;
        goto done;
    }

#ifdef REASSOCIATION    
    if (priv->phandle->surprise_removed) {
        ret = -1;
        goto done;
    }
#endif

  done:
    LEAVE();
    return ret;
}

/**
 *  @brief  Retrieve the scan table entries via wireless tools IOCTL call
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param dwrq         A pointer to iw_point structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return             0--success, otherwise fail
 */
int
woal_get_scan(moal_private *priv)
{
    
    int ret=0;
    unsigned int i;
    unsigned int j;
    mlan_scan_resp scan_resp;
    mlan_bss_info bss_info;
    BSSDescriptor_t *scan_table;
    struct  iw_quality qual;

    ENTER();

    if (priv->scan_pending_on_block == MTRUE)
        return -1;

    ewl_os_memset(&bss_info, 0, sizeof(bss_info));
    if (MLAN_STATUS_SUCCESS !=
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info)) {
        ret = -1;
        goto done;
    }
    
    ewl_os_memset(&scan_resp, 0, sizeof(scan_resp));
    if (MLAN_STATUS_SUCCESS != woal_get_scan_table(priv,
                                                   MOAL_IOCTL_WAIT,
                                                   &scan_resp)) {
        ret = -EFAULT;
        goto done;
    }
    scan_table = (BSSDescriptor_t *) scan_resp.pscan_table;
//    if (key_len)
//        end_buf = extra + key_len;
    if (priv->media_connected == MTRUE) {
        PRINTM(MINFO, "Current Ssid: %-32s\n", bss_info.ssid.ssid);
    }
    PRINTM(MINFO, "Scan: Get: NumInScanTable = %d\n",
           (int) scan_resp.num_in_scan_table);
           
    for (i = 0; i < scan_resp.num_in_scan_table; i++) {

        if (!scan_table[i].freq) {
            PRINTM(MERROR, "Invalid channel number %d\n",
                   (int) scan_table[i].channel);
            continue;
        }
               
        /* check ssid is valid or not, ex. hidden ssid will be filter out */
        if (woal_ssid_valid(&scan_table[i].ssid) == MFALSE) {
            continue;
        }


        /* Add the ESSID */
        ewl_os_printf("Cell %02d ESSID: %-32s\n", i, scan_table[i].ssid.ssid);
        
        /* First entry *MUST* be the AP MAC address */
//      NetASCII_MAC_to_Str((char*)&scan_table[i].mac_address,temp,0,0,&err);
        
        /* Add mode */
        if (scan_table[i].bss_mode == MLAN_BSS_MODE_IBSS) 
            ewl_os_printf("        Mode:IBSS\n");
        else if (scan_table[i].bss_mode == MLAN_BSS_MODE_INFRA)
            ewl_os_printf("        Mode:Master\n");
        else
            ewl_os_printf("        Mode:Auto\n");;
                
        /* Frequency */
        ewl_os_printf("        Frequency : %4.3f GHz, (Channel %u) \n", 
                     (float)(scan_table[i].freq/1000.0), scan_table[i].channel);
        
        /* Add encryption capability */
        if (scan_table[i].privacy) {
            ewl_os_printf("        Encryption key:on\n");
        } else {
            ewl_os_printf("        Encryption key:off\n");
        }
        
        ewl_os_memset(&qual, 0, sizeof(struct  iw_quality));
        /* Add quality statistics */
        qual.level = SCAN_RSSI(scan_table[i].rssi);
        if (!bss_info.bcn_nf_last) {
            qual.noise = MRVDRV_NF_DEFAULT_SCAN_VALUE;
        } else {
            qual.noise = bss_info.bcn_nf_last;
        }
        qual.qual =
            woal_rssi_to_quality((t_s16) ((__u8)qual.level - 0x100));
        
        ewl_os_printf("        Quality:%u/5 Signal level: %d dBm, Noise level:%d dBm\n",                                   
                    qual.qual, qual.level, qual.noise);
        
        ewl_os_printf("        bit rate:"); 
        /* Bit rate given in 500 kb/s units (+ 0x80) */
        for (j = 0; j < sizeof(scan_table[i].supported_rates); j++) {
            if (!scan_table[i].supported_rates[j]) {
                break;
            }
            if(j>0 && (j%5)==0) ewl_os_printf("\n                 ");
            
            ewl_os_printf("%3.1f Mb/s ;", (scan_table[i].supported_rates[j]&0x7f)*5.0/10.0);
            //iwe.u.bitrate.value =
            //    (scan_table[i].supported_rates[j] & 0x7f) * 500000;
        
        }
        ewl_os_printf("\n");
        ewl_os_printf("        Beacon interval=%d\n", scan_table[i].beacon_period);
        if (scan_table[i].bss_band == BAND_A)
            ewl_os_printf("        band=a\n"); 
        else
            ewl_os_printf("        band=bg\n");                
        {
            u8 *iebuf;
            int k=0;
            
            iebuf = (u8*)scan_table[i].pwpa_ie;
            ewl_os_printf("iebuf %x \n", iebuf);
            if(iebuf != NULL)
            {
                for(k=0; k<10; k++)
                {
                    ewl_os_printf("%02x ", iebuf[k]);    
                }
            }
        }            
    }                   

  done:
    LEAVE();
    return ret;
}
#endif

/**
 *  @brief Set essid
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param dwrq         A pointer to iw_point structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return             0--success, otherwise fail
 */

int
woal_set_essid1(moal_private *priv, struct iw_request_info *info,
               struct iw_point *dwrq, char *extra)
{
    mlan_802_11_ssid req_ssid;
    mlan_ssid_bssid ssid_bssid;

    int ret = 0;
    t_u32 mode = 0;

    MLAN_UNUSED_ARG(info);
    ENTER();

    /* Check the size of the string */
    if (dwrq->length > IW_ESSID_MAX_SIZE + 1) {
        ret = -E2BIG;
        goto setessid_ret;
    }
    ewl_os_memset(&req_ssid, 0, sizeof(mlan_802_11_ssid));
    ewl_os_memset(&ssid_bssid, 0, sizeof(mlan_ssid_bssid));

    /* 
     * Check if we asked for `any' or 'particular'
     */
    if (!dwrq->flags) {
        /* Do normal SSID scanning */
        if (MLAN_STATUS_SUCCESS !=
            woal_request_scan(priv, MOAL_IOCTL_WAIT, NULL)) {
            ret = -EFAULT;
            goto setessid_ret;
        }
    } else {
        /* Set the SSID */
#if WIRELESS_EXT > 20
        req_ssid.ssid_len = dwrq->length;
#else
        req_ssid.ssid_len = dwrq->length - 1;
#endif
        ewl_os_memcpy(req_ssid.ssid, extra,
               min(req_ssid.ssid_len, MLAN_MAX_SSID_LENGTH));
        if (!req_ssid.ssid_len || (MFALSE == woal_ssid_valid(&req_ssid))) {
            PRINTM(MERROR, "Invalid SSID - aborting set_essid\n");
            ret = -EINVAL;
            goto setessid_ret;
        }
        PRINTM(MINFO, "Requested new SSID = %s\n", (char *) req_ssid.ssid);
        ewl_os_memcpy(&ssid_bssid.ssid, &req_ssid, sizeof(mlan_802_11_ssid));

        if (dwrq->flags != 0xFFFF) {
            if (MLAN_STATUS_SUCCESS !=
                woal_find_best_network(priv, MOAL_IOCTL_WAIT, &ssid_bssid)) {
                /* Do specific SSID scanning */
                if (MLAN_STATUS_SUCCESS !=
                    woal_request_scan(priv, MOAL_IOCTL_WAIT, &req_ssid)) {
                    ret = -EFAULT;
                    goto setessid_ret;
                }
            }
        }

    }

    /* disconnect before try to associate */
    woal_disconnect(priv, MOAL_IOCTL_WAIT, NULL);
    mode = woal_get_mode(priv, MOAL_IOCTL_WAIT);
    if (mode != IW_MODE_ADHOC) {
        if (MLAN_STATUS_SUCCESS !=
            woal_find_best_network(priv, MOAL_IOCTL_WAIT, &ssid_bssid)) {
            ret = -EFAULT;
            goto setessid_ret;
        }
    }

    /* Connect to BSS by ESSID */
    ewl_os_memset(&ssid_bssid.bssid, 0, MLAN_MAC_ADDR_LENGTH);
    if (MLAN_STATUS_SUCCESS != woal_bss_start(priv,
                                              MOAL_IOCTL_WAIT, &ssid_bssid)) {
        ret = -EFAULT;
        goto setessid_ret;
    }
setessid_ret:
    LEAVE();
    return ret;
}



/** 
 *  @brief Get current essid 
 *   
 *  @param dev      A pointer to net_device structure
 *  @param info     A pointer to iw_request_info structure
 *  @param dwrq     A pointer to iw_point structure
 *  @param extra    A pointer to extra data buf
 *
 *  @return         0--success, otherwise fail
 */
int
woal_get_essid(moal_private *priv, struct iw_request_info *info,
               struct iw_point *dwrq, char *extra)
{
    mlan_bss_info bss_info;
    int ret = 0;
    MLAN_UNUSED_ARG(info);
    
    ENTER();

    ewl_os_memset(&bss_info, 0, sizeof(bss_info));

    if (MLAN_STATUS_SUCCESS !=
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info)) {
        ret = -EFAULT;
        goto done;
    }

    if (bss_info.media_connected) {
        dwrq->length = min(dwrq->length, bss_info.ssid.ssid_len);
        ewl_os_memcpy(extra, bss_info.ssid.ssid, dwrq->length);
    } else
        dwrq->length = 0;

    if (bss_info.scan_table_idx)
        dwrq->flags = (bss_info.scan_table_idx + 1) & IW_ENCODE_INDEX;
    else
        dwrq->flags = 1;

done:
    LEAVE();
    return ret;
}

/**
 *  @brief Set encryption key
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param dwrq                 A pointer to iw_point structure
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_set_encode(moal_private * priv, struct iw_request_info *info,
                struct iw_point *dwrq, char *extra)
{
    int ret = 0;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_ds_encrypt_key *pkey = NULL;
    int index = 0;

    MLAN_UNUSED_ARG(info);

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    ewl_os_memset(sec,0,sizeof(mlan_ds_sec_cfg));
    sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    
    /* Check index */
    index = (dwrq->flags & IW_ENCODE_INDEX) - 1;

    if (index > 3) {
        PRINTM(MERROR, "Key index #%d out of range\n", index);
        ret = -EINVAL;
        goto done;
    }
    
    if (dwrq->length > MAX_WEP_KEY_SIZE) {
        pkey = (mlan_ds_encrypt_key *) extra;          
        if (pkey->key_len <= MAX_WEP_KEY_SIZE) {
            dwrq->length = pkey->key_len;
            dwrq->flags = pkey->key_index + 1;
        }
    }

    sec->param.encrypt_key.key_len = 0;

     if (dwrq->length) {
        if (dwrq->length > MAX_WEP_KEY_SIZE) {
            PRINTM(MERROR, "Key length (%d) out of range\n", dwrq->length);
            ret = -EINVAL;
            goto done;
        }
        if (index < 0)
            sec->param.encrypt_key.is_current_wep_key = MTRUE;
        else
            sec->param.encrypt_key.key_index = index;
        if (!(dwrq->flags & IW_ENCODE_NOKEY)) {
            ewl_os_memcpy(sec->param.encrypt_key.key_material, extra, dwrq->length);
            /* Set the length */
            if (dwrq->length > MIN_WEP_KEY_SIZE)
                sec->param.encrypt_key.key_len = MAX_WEP_KEY_SIZE;
            else
                sec->param.encrypt_key.key_len = MIN_WEP_KEY_SIZE;
        }
    } else {
        /* 
         * No key provided so it is either enable key, 
         * on or off
         */
        //printk("index %d \n", index);          
        if (dwrq->flags & IW_ENCODE_DISABLED) {
            PRINTM(MINFO, "*** iwconfig mlanX key off ***\n");
            sec->param.encrypt_key.key_disable = MTRUE;
        } else {
            /* 
             * iwconfig mlanX key [n]
             * iwconfig mlanX key on 
             * Do we want to just set the transmit key index ? 
             */
            if (index < 0) {
                PRINTM(MINFO, "*** iwconfig mlanX key on ***\n");
                sec->param.encrypt_key.is_current_wep_key = MTRUE;
            } else
                sec->param.encrypt_key.key_index = index;
        }
    }
 #if 0   
    pkey = &sec->param.encrypt_key;
    {
        t_u8 *p = (t_u8*)pkey;
        int i=0;

        for(i=0; i<sizeof(mlan_ds_encrypt_key); i++)
        {
            if((i%16)==0) ewl_os_printf("\n");
            ewl_os_printf("%02X ", p[i]);
        }
        ewl_os_printf("\n");
    }
#endif    

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    
done:
    if (req)
        ewl_os_free(req);

    LEAVE();
    return ret;
}

/** 
 *  @brief Get authentication mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param auth_mode            A pointer to authentication mode
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_auth_mode(moal_private * priv, t_u8 wait_option, t_u32 * auth_mode)
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
    ewl_os_memset(sec,0,sizeof(mlan_ds_sec_cfg));
    sec->sub_command = MLAN_OID_SEC_CFG_AUTH_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS && auth_mode) {
        *auth_mode = sec->param.auth_mode;
    }
done:
    if (req && (status != MLAN_STATUS_PENDING))
        ewl_os_free(req);
    LEAVE();
    return status;
}


/**
 *  @brief Get encryption key
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param dwrq                 A pointer to iw_point structure
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_get_encode(moal_private * priv, struct iw_request_info *info,
                struct iw_point *dwrq, char *extra)
{
    int ret = 0;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;
    t_u32 auth_mode;
    MLAN_UNUSED_ARG(info);

    int index = (dwrq->flags & IW_ENCODE_INDEX);

    ENTER();
    if (index < 0 || index > 4) {
        PRINTM(MERROR, "Key index #%d out of range\n", index);
        ret = -EINVAL;
        goto done;
    }
    if (MLAN_STATUS_SUCCESS !=
        woal_get_auth_mode(priv, MOAL_IOCTL_WAIT, &auth_mode)) {
        ret = -EFAULT;
        goto done;
    }
    dwrq->flags = 0;
  
    /* 
     * Check encryption mode 
     */
    switch (auth_mode) {
    case MLAN_AUTH_MODE_OPEN:
        dwrq->flags = IW_ENCODE_OPEN;
        break;

    case MLAN_AUTH_MODE_SHARED:
    case MLAN_AUTH_MODE_NETWORKEAP:
        dwrq->flags = IW_ENCODE_RESTRICTED;
        break;
    default:
        dwrq->flags = IW_ENCODE_DISABLED | IW_ENCODE_OPEN;
        break;
    }
  
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    sec = (mlan_ds_sec_cfg *) req->pbuf;
    ewl_os_memset(sec,0,sizeof(mlan_ds_sec_cfg));
    sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    if (!index)
        sec->param.encrypt_key.is_current_wep_key = MTRUE;
    else
        sec->param.encrypt_key.key_index = index - 1;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    ewl_os_memset(extra, 0, 16);
    if (sec->param.encrypt_key.key_len) {
        ewl_os_memcpy(extra, sec->param.encrypt_key.key_material,
               sec->param.encrypt_key.key_len);
        dwrq->length = sec->param.encrypt_key.key_len;
        dwrq->flags |= (sec->param.encrypt_key.key_index + 1);
        dwrq->flags &= ~IW_ENCODE_DISABLED;
    } else if (sec->param.encrypt_key.key_disable)
        dwrq->flags |= IW_ENCODE_DISABLED;
    else
        dwrq->flags &= ~IW_ENCODE_DISABLED;

    dwrq->flags |= IW_ENCODE_NOKEY;
  done:
    if (req)
        ewl_os_free(req);

    LEAVE();
    return ret;
}


/** 
 *  @brief  Request MLME operation 
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param dwrq         A pointer to iw_point structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return             0--success, otherwise fail
 */
int
woal_set_mlme(moal_private *priv,
              struct iw_request_info *info, struct iw_point *dwrq, char *extra)
{
    struct iw_mlme *mlme = (struct iw_mlme *) extra;
    int ret = 0;
    MLAN_UNUSED_ARG(info);
    MLAN_UNUSED_ARG(dwrq);

    ENTER();
    if ((mlme->cmd == IW_MLME_DEAUTH) || (mlme->cmd == IW_MLME_DISASSOC)) {
        if (MLAN_STATUS_SUCCESS !=
            woal_disconnect(priv, MOAL_IOCTL_WAIT, (t_u8 *) mlme->addr.sa_data))
            ret = -EFAULT;
    }
    LEAVE();
    return ret;
}


/** @brief Set authentication mode parameters
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param vwrq                 A pointer to iw_param structure
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_set_auth(moal_private * priv, struct iw_request_info *info,
              struct iw_param *vwrq, char *extra)
{
    int ret = 0;
    MLAN_UNUSED_ARG(info);
    MLAN_UNUSED_ARG(priv);
    MLAN_UNUSED_ARG(vwrq);
    MLAN_UNUSED_ARG(extra);
#if 0
    t_u32 auth_mode = 0;
    t_u32 encrypt_mode = 0;
    ENTER();

    switch (vwrq->flags & IW_AUTH_INDEX) {
    case IW_AUTH_CIPHER_PAIRWISE:
    case IW_AUTH_CIPHER_GROUP:
        if (vwrq->value & IW_AUTH_CIPHER_NONE)
            encrypt_mode = MLAN_ENCRYPTION_MODE_NONE;
        else if (vwrq->value & IW_AUTH_CIPHER_WEP40)
            encrypt_mode = MLAN_ENCRYPTION_MODE_WEP40;
        else if (vwrq->value & IW_AUTH_CIPHER_WEP104)
            encrypt_mode = MLAN_ENCRYPTION_MODE_WEP104;
        else if (vwrq->value & IW_AUTH_CIPHER_TKIP)
            encrypt_mode = MLAN_ENCRYPTION_MODE_TKIP;
        else if (vwrq->value & IW_AUTH_CIPHER_CCMP)
            encrypt_mode = MLAN_ENCRYPTION_MODE_CCMP;
        if (MLAN_STATUS_SUCCESS !=
            woal_set_encrypt_mode(priv, MOAL_IOCTL_WAIT, encrypt_mode))
            ret = -EFAULT;
        break;
    case IW_AUTH_80211_AUTH_ALG:
        switch (vwrq->value) {
        case IW_AUTH_ALG_SHARED_KEY:
            PRINTM(MINFO, "Auth mode shared key!\n");
            auth_mode = MLAN_AUTH_MODE_SHARED;
            break;
        case IW_AUTH_ALG_LEAP:
            PRINTM(MINFO, "Auth mode LEAP!\n");
            auth_mode = MLAN_AUTH_MODE_NETWORKEAP;
            break;
        case IW_AUTH_ALG_OPEN_SYSTEM:
            PRINTM(MINFO, "Auth mode open!\n");
            auth_mode = MLAN_AUTH_MODE_OPEN;
            break;
        case IW_AUTH_ALG_SHARED_KEY | IW_AUTH_ALG_OPEN_SYSTEM:
        default:
            PRINTM(MINFO, "Auth mode auto!\n");
            auth_mode = MLAN_AUTH_MODE_AUTO;
            break;
        }
        if (MLAN_STATUS_SUCCESS !=
            woal_set_auth_mode(priv, MOAL_IOCTL_WAIT, auth_mode))
            ret = -EFAULT;
        break;
    case IW_AUTH_WPA_ENABLED:
        if (MLAN_STATUS_SUCCESS !=
            woal_set_wpa_enable(priv, MOAL_IOCTL_WAIT, vwrq->value))
            ret = -EFAULT;
        break;
#define IW_AUTH_WAPI_ENABLED    0x20
    case IW_AUTH_WAPI_ENABLED:
        if (MLAN_STATUS_SUCCESS !=
            woal_set_wapi_enable(priv, MOAL_IOCTL_WAIT, vwrq->value))
            ret = -EFAULT;
        break;
    case IW_AUTH_WPA_VERSION:
        /* set WPA_VERSION_DISABLED/VERSION_WPA/VERSION_WP2 */
        priv->wpa_version = vwrq->value;
        break;
    case IW_AUTH_KEY_MGMT:
        /* set KEY_MGMT_802_1X/KEY_MGMT_PSK */
        priv->key_mgmt = vwrq->value;
        break;
    case IW_AUTH_TKIP_COUNTERMEASURES:
    case IW_AUTH_DROP_UNENCRYPTED:
    case IW_AUTH_RX_UNENCRYPTED_EAPOL:
    case IW_AUTH_ROAMING_CONTROL:
    case IW_AUTH_PRIVACY_INVOKED:
        break;
    default:
        ret = -EOPNOTSUPP;
        break;
    }
    LEAVE();
#endif    
    return ret;
}


/**
 *  @brief Get current BSSID
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param awrq         A pointer to sockaddr structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return             0 --success, otherwise fail
 */
int
woal_get_wap(moal_private * priv, struct iw_request_info *info,
             struct sockaddr *awrq, char *extra)
{
    int ret = 0;
    mlan_bss_info bss_info;

    MLAN_UNUSED_ARG(info);
    MLAN_UNUSED_ARG(extra);

    ENTER();

    ewl_os_memset(&bss_info, 0, sizeof(bss_info));

    woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);

    if (bss_info.media_connected == MTRUE) {
        ewl_os_memcpy(awrq->sa_data, &bss_info.bssid, MLAN_MAC_ADDR_LENGTH);
    } else {
        ewl_os_memset(awrq->sa_data, 0, MLAN_MAC_ADDR_LENGTH);
    }
    awrq->sa_family = ARPHRD_ETHER;

    LEAVE();
    return ret;
}


/**
 *  @brief  Retrieve the scan table entries via wireless tools IOCTL call
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param dwrq         A pointer to iw_point structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return             0--success, otherwise fail
 */
int
woal_get_scan1(moal_private * priv, struct iw_request_info *info,
              struct iw_point *dwrq, char *extra)
{
    int ret = 0;
    char *current_ev = extra;
    char *end_buf = extra + IW_SCAN_MAX_DATA;
    char *current_val;          /* For rates */
    struct iw_event iwe;        /* Temporary buffer */
    unsigned int i;
    unsigned int j;
    mlan_scan_resp scan_resp;
    mlan_bss_info bss_info;
    BSSDescriptor_t *scan_table;
    mlan_ds_get_signal rssi;
    t_u16 buf_size = 16 + 256 * 2;
    char *buf = NULL;
    char *ptr;
    t_u8 *praw_data;
    int beacon_size;
    t_u8 *pbeacon;
    IEEEtypes_ElementId_e element_id;
    t_u8 element_len;

    ENTER();
    
    if (priv->scan_pending_on_block == MTRUE)
    {
        return -EAGAIN;
    }
    
    if (!(buf = ewl_os_malloc((buf_size)))) {
        PRINTM(MERROR, "Cannot allocate buffer!\n");
        ret = -EFAULT;
        goto done;
    }

    ewl_os_memset(&bss_info, 0, sizeof(bss_info));
    if (MLAN_STATUS_SUCCESS !=
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info)) {
        ret = -EFAULT;
        goto done;
    }
    ewl_os_memset(&scan_resp, 0, sizeof(scan_resp));
    if (MLAN_STATUS_SUCCESS != woal_get_scan_table(priv,
                                                   MOAL_IOCTL_WAIT,
                                                   &scan_resp)) {
        ret = -EFAULT;
        goto done;
    }
    scan_table = (BSSDescriptor_t *) scan_resp.pscan_table;
    if (dwrq->length)
        end_buf = extra + dwrq->length;
    if (priv->media_connected == MTRUE) {
        PRINTM(MINFO, "Current Ssid: %-32s\n", bss_info.ssid.ssid);
    }
    PRINTM(MINFO, "NumInScanTable: %d\n", (int) scan_resp.num_in_scan_table);

#if WIRELESS_EXT > 13
    /* The old API using SIOCGIWAPLIST had a hard limit of IW_MAX_AP. The new
       API using SIOCGIWSCAN is only limited by buffer size WE-14 -> WE-16 the
       buffer is limited to IW_SCAN_MAX_DATA bytes which is 4096. */
    for (i = 0; i < scan_resp.num_in_scan_table; i++) {
        if ((current_ev + MAX_SCAN_CELL_SIZE) >= end_buf) {
            PRINTM(MINFO, "i=%d break out: current_ev=%p end_buf=%p "
                   "MAX_SCAN_CELL_SIZE=%d\n",
                   i, current_ev, end_buf, MAX_SCAN_CELL_SIZE);
            ret = -E2BIG;
            break;
        }
        if (!scan_table[i].freq) {
            PRINTM(MERROR, "Invalid channel number %d\n",
                   (int) scan_table[i].channel);
            continue;
        }
        PRINTM(MINFO, "i=%d  Ssid: %-32s\n", i, scan_table[i].ssid.ssid);

        /* check ssid is valid or not, ex. hidden ssid will be filter out */
        if (woal_ssid_valid(&scan_table[i].ssid) == MFALSE) {
            continue;
        }

        /* First entry *MUST* be the AP MAC address */
        iwe.cmd = SIOCGIWAP;
        iwe.u.ap_addr.sa_family = ARPHRD_ETHER;
        ewl_os_memcpy(iwe.u.ap_addr.sa_data, &scan_table[i].mac_address, ETH_ALEN);

        iwe.len = IW_EV_ADDR_LEN;
        current_ev =
            IWE_STREAM_ADD_EVENT(info, current_ev, end_buf, &iwe, iwe.len);

        /* Add the ESSID */
        iwe.u.data.length = scan_table[i].ssid.ssid_len;

        if (iwe.u.data.length > 32) {
            iwe.u.data.length = 32;
        }

        iwe.cmd = SIOCGIWESSID;
        iwe.u.essid.flags = (i + 1) & IW_ENCODE_INDEX;
        iwe.len = IW_EV_POINT_LEN + iwe.u.data.length;
        current_ev =
            IWE_STREAM_ADD_POINT(info, current_ev, end_buf, &iwe,
                                 (t_s8 *) scan_table[i].ssid.ssid);

        /* Add mode */
        iwe.cmd = SIOCGIWMODE;
        if (scan_table[i].bss_mode == MLAN_BSS_MODE_IBSS)
            iwe.u.mode = IW_MODE_ADHOC;
        else if (scan_table[i].bss_mode == MLAN_BSS_MODE_INFRA)
            iwe.u.mode = IW_MODE_MASTER;
        else
            iwe.u.mode = IW_MODE_AUTO;

        iwe.len = IW_EV_UINT_LEN;
        current_ev =
            IWE_STREAM_ADD_EVENT(info, current_ev, end_buf, &iwe, iwe.len);

        /* Frequency */
        iwe.cmd = SIOCGIWFREQ;
        iwe.u.freq.m = (long) scan_table[i].freq * 100000;
        iwe.u.freq.e = 1;
        iwe.len = IW_EV_FREQ_LEN;
        current_ev =
            IWE_STREAM_ADD_EVENT(info, current_ev, end_buf, &iwe, iwe.len);

        ewl_os_memset(&iwe, 0, sizeof(iwe));
        /* Add quality statistics */
        iwe.cmd = IWEVQUAL;
        iwe.u.qual.level = SCAN_RSSI(scan_table[i].rssi);

        if (!bss_info.bcn_nf_last) {
            iwe.u.qual.noise = MRVDRV_NF_DEFAULT_SCAN_VALUE;
        } else {
            iwe.u.qual.noise = bss_info.bcn_nf_last;
        }
        if ((bss_info.bss_mode == MLAN_BSS_MODE_IBSS) &&
            !woal_ssid_cmp(&bss_info.ssid, &scan_table[i].ssid)
            && bss_info.adhoc_state == ADHOC_STARTED) {
            ewl_os_memset(&rssi, 0, sizeof(mlan_ds_get_signal));
//            if (MLAN_STATUS_SUCCESS !=
//                woal_get_signal_info(priv, MOAL_IOCTL_WAIT, &rssi)) {
//                ret = -EFAULT;
//                break;
//            }
            iwe.u.qual.level = rssi.data_rssi_avg;
        }
        iwe.u.qual.qual =
            woal_rssi_to_quality((t_s16) (iwe.u.qual.level - 0x100));
        iwe.len = IW_EV_QUAL_LEN;
        current_ev =
            IWE_STREAM_ADD_EVENT(info, current_ev, end_buf, &iwe, iwe.len);

        /* Add encryption capability */
        iwe.cmd = SIOCGIWENCODE;
        if (scan_table[i].privacy) {
            iwe.u.data.flags = IW_ENCODE_ENABLED | IW_ENCODE_NOKEY;
        } else {
            iwe.u.data.flags = IW_ENCODE_DISABLED;
        }
        iwe.u.data.length = 0;
        iwe.len = IW_EV_POINT_LEN + iwe.u.data.length;
        current_ev =
            IWE_STREAM_ADD_POINT(info, current_ev, end_buf, &iwe, NULL);

        current_val = current_ev + IW_EV_LCP_LEN;

        iwe.cmd = SIOCGIWRATE;

        iwe.u.bitrate.fixed = 0;
        iwe.u.bitrate.disabled = 0;
        iwe.u.bitrate.value = 0;

        /* Bit rate given in 500 kb/s units (+ 0x80) */
        for (j = 0; j < sizeof(scan_table[i].supported_rates); j++) {
            if (!scan_table[i].supported_rates[j]) {
                break;
            }

            iwe.u.bitrate.value =
                (scan_table[i].supported_rates[j] & 0x7f) * 500000;
            iwe.len = IW_EV_PARAM_LEN;
            current_val =
                IWE_STREAM_ADD_VALUE(info, current_ev, current_val, end_buf,
                                     &iwe, iwe.len);

        }
        if ((bss_info.bss_mode == MLAN_BSS_MODE_IBSS) &&
            !woal_ssid_cmp(&bss_info.ssid, &scan_table[i].ssid)
            && bss_info.adhoc_state == ADHOC_STARTED) {
            iwe.u.bitrate.value = 22 * 500000;
            iwe.len = IW_EV_PARAM_LEN;
            current_val =
                IWE_STREAM_ADD_VALUE(info, current_ev, current_val, end_buf,
                                     &iwe, iwe.len);
        }

        /* Check if an event is added */
        if ((unsigned int) (current_val - current_ev) >= IW_EV_PARAM_LEN)
            current_ev = current_val;

        /* Beacon Interval */
        ewl_os_memset(&iwe, 0, sizeof(iwe));
        ewl_os_memset(buf, 0, buf_size);
        ptr = buf;
        ptr += sprintf(ptr, "Beacon interval=%d", scan_table[i].beacon_period);

        iwe.u.data.length = ewl_os_strlen(buf);
        iwe.cmd = IWEVCUSTOM;
        iwe.len = IW_EV_POINT_LEN + iwe.u.data.length;
        current_ev = IWE_STREAM_ADD_POINT(info, current_ev, end_buf, &iwe, buf);
        current_val = current_ev + IW_EV_LCP_LEN + ewl_os_strlen(buf);

        /* Parse and send the IEs */
        pbeacon = scan_table[i].pbeacon_buf;
        beacon_size = scan_table[i].beacon_buf_size;

        /* Skip time stamp, beacon interval and capability */
        if (pbeacon) {
            pbeacon += sizeof(scan_table[i].beacon_period) +
                sizeof(scan_table[i].time_stamp) +
                sizeof(scan_table[i].cap_info);
            beacon_size -= sizeof(scan_table[i].beacon_period) +
                sizeof(scan_table[i].time_stamp) +
                sizeof(scan_table[i].cap_info);
        }

        while ((unsigned int) beacon_size >= sizeof(IEEEtypes_Header_t)) {
            element_id = (IEEEtypes_ElementId_e) (*(t_u8 *) pbeacon);
            element_len = *((t_u8 *) pbeacon + 1);
            if ((unsigned int) beacon_size <
                (unsigned int) element_len + sizeof(IEEEtypes_Header_t)) {
                PRINTM(MERROR,
                       "Get scan: Error in processing IE, "
                       "bytes left < IE length\n");
                break;
            }

            switch (element_id) {
            case VENDOR_SPECIFIC_221:
            case RSN_IE:
            case WAPI_IE:
                praw_data = (t_u8 *) pbeacon;
                ewl_os_memset(&iwe, 0, sizeof(iwe));
                ewl_os_memset(buf, 0, buf_size);
                ptr = buf;
                ewl_os_memcpy(buf, praw_data,
                       element_len + sizeof(IEEEtypes_Header_t));
                iwe.cmd = IWEVGENIE;
                iwe.u.data.length = element_len + sizeof(IEEEtypes_Header_t);
                iwe.len = IW_EV_POINT_LEN + iwe.u.data.length;
                current_ev =
                    IWE_STREAM_ADD_POINT(info, current_ev, end_buf, &iwe, buf);
                current_val = current_ev + IW_EV_LCP_LEN + ewl_os_strlen(buf);
                break;
            default:
                break;
            }
            pbeacon += element_len + sizeof(IEEEtypes_Header_t);
            beacon_size -= element_len + sizeof(IEEEtypes_Header_t);
        }

#if WIRELESS_EXT > 14
        ewl_os_memset(&iwe, 0, sizeof(iwe));
        ewl_os_memset(buf, 0, buf_size);
        ptr = buf;
        ptr += sprintf(ptr, "band=");
        ewl_os_memset(&iwe, 0, sizeof(iwe));
        if (scan_table[i].bss_band == BAND_A)
            ptr += sprintf(ptr, "a");
        else
            ptr += sprintf(ptr, "bg");
        iwe.u.data.length = ewl_os_strlen(buf);
        PRINTM(MINFO, "iwe.u.data.length %d\n", iwe.u.data.length);
        PRINTM(MINFO, "BUF: %s \n", buf);
        iwe.cmd = IWEVCUSTOM;
        iwe.len = IW_EV_POINT_LEN + iwe.u.data.length;
        current_ev = IWE_STREAM_ADD_POINT(info, current_ev, end_buf, &iwe, buf);
        current_val = current_ev + IW_EV_LCP_LEN + ewl_os_strlen(buf);
#endif
        current_val = current_ev + IW_EV_LCP_LEN;

        /* 
         * Check if we added any event
         */
        if ((unsigned int) (current_val - current_ev) > IW_EV_LCP_LEN)
            current_ev = current_val;
    }

    dwrq->length = (current_ev - extra);
    dwrq->flags = 0;
#endif

  done:
    if (buf)
        ewl_os_free(buf);
    LEAVE();
    return ret;
}


/**
 *  @brief Connect to the AP or Ad-hoc Network with specific bssid
 *
 * NOTE: Scan should be issued by application before this function is called
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param awrq         A pointer to iw_param structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return             0 --success, otherwise fail
 */
int
woal_set_wap(moal_private * priv, struct iw_request_info *info,
             struct sockaddr *awrq, char *extra)
{
    int ret = 0;
    const t_u8 bcast[MLAN_MAC_ADDR_LENGTH] = { 255, 255, 255, 255, 255, 255 };
    const t_u8 zero_mac[MLAN_MAC_ADDR_LENGTH] = { 0, 0, 0, 0, 0, 0 };
    mlan_ssid_bssid ssid_bssid;
    mlan_bss_info bss_info;

    MLAN_UNUSED_ARG(info);
    MLAN_UNUSED_ARG(extra);

    ENTER();

    if (awrq->sa_family != ARPHRD_ETHER) {
        ret = -EINVAL;
        goto done;
    }

    PRINTM(MINFO, "ASSOC: WAP: sa_data: %02x:%02x:%02x:%02x:%02x:%02x\n",
           (t_u8) awrq->sa_data[0], (t_u8) awrq->sa_data[1],
           (t_u8) awrq->sa_data[2], (t_u8) awrq->sa_data[3],
           (t_u8) awrq->sa_data[4], (t_u8) awrq->sa_data[5]);         

    if (MLAN_STATUS_SUCCESS !=
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info)) {
        ret = -EFAULT;
        goto done;
    }
#ifdef REASSOCIATION
    /* Cancel re-association */
    priv->reassoc_required = MFALSE;
#endif

    /* zero_mac means disconnect */
    if (!ewl_os_memcmp(zero_mac, awrq->sa_data, MLAN_MAC_ADDR_LENGTH)) {
        woal_disconnect(priv, MOAL_IOCTL_WAIT, NULL);
        goto done;
    }

    /* Broadcast MAC means search for best network */
    ewl_os_memset(&ssid_bssid, 0, sizeof(mlan_ssid_bssid));

    if (ewl_os_memcmp(bcast, awrq->sa_data, MLAN_MAC_ADDR_LENGTH)) {
        /* Check if we are already assoicated to the AP */
        if (bss_info.media_connected == MTRUE) {
            if (!ewl_os_memcmp(awrq->sa_data, &bss_info.bssid, ETH_ALEN))
                goto done;
            /* disconnect before try to assoicate to the new AP */
            woal_disconnect(priv, MOAL_IOCTL_WAIT, NULL);
        }
        ewl_os_memcpy(&ssid_bssid.bssid, awrq->sa_data, ETH_ALEN);
    }

    if (MLAN_STATUS_SUCCESS != woal_find_best_network(priv,
                                                      MOAL_IOCTL_WAIT,
                                                      &ssid_bssid)) {
        PRINTM(MERROR, "ASSOC: WAP: MAC address not found in BSSID List\n");
        ret = -ENETUNREACH;
        goto done;
    }
    /* Zero SSID implies use BSSID to connect */
    ewl_os_memset(&ssid_bssid.ssid, 0, sizeof(mlan_802_11_ssid));
    if (MLAN_STATUS_SUCCESS != woal_bss_start(priv,
                                              MOAL_IOCTL_WAIT, &ssid_bssid)) {
        ret = -EFAULT;
        goto done;
    }
#ifdef REASSOCIATION
    ewl_os_memset(&bss_info, 0, sizeof(bss_info));
    if (MLAN_STATUS_SUCCESS != woal_get_bss_info(priv,
                                                 MOAL_IOCTL_WAIT, &bss_info)) {
        ret = -EFAULT;
        goto done;
    }
    ewl_os_memcpy(&priv->prev_ssid_bssid.ssid, &bss_info.ssid,
           sizeof(mlan_802_11_ssid));
    ewl_os_memcpy(&priv->prev_ssid_bssid.bssid, &bss_info.bssid, MLAN_MAC_ADDR_LENGTH);
#endif /* REASSOCIATION */

  done:

    LEAVE();
    return ret;
}

/**
 *  @brief Set wlan mode
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param uwrq                 Wireless mode to set
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_set_bss_mode(moal_private * priv, struct iw_request_info *info,
                  t_u32 * uwrq, char *extra)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    
    MLAN_UNUSED_ARG(info);
    MLAN_UNUSED_ARG(extra);

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_MODE;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;

    switch (*uwrq) {
    case IW_MODE_INFRA:
        bss->param.bss_mode = MLAN_BSS_MODE_INFRA;
        break;
    case IW_MODE_ADHOC:
        bss->param.bss_mode = MLAN_BSS_MODE_IBSS;
        break;
    case IW_MODE_AUTO:
        bss->param.bss_mode = MLAN_BSS_MODE_AUTO;
        break;
    default:
        ret = -EINVAL;
        break;
    }
    if (ret)
        goto done;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
  done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Get wlan mode
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param uwrq                 A pointer to t_u32 string
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_get_bss_mode(moal_private * priv, struct iw_request_info *info,
                  t_u32 * uwrq, char *extra)
{
    
    MLAN_UNUSED_ARG(info);
    MLAN_UNUSED_ARG(extra);

    ENTER();
    *uwrq = woal_get_mode(priv, MOAL_IOCTL_WAIT);
    LEAVE();
    return 0;
}


/**
 *  @brief Get IE
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param dwrq                 A pointer to iw_point structure
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_get_gen_ie(moal_private *priv, struct iw_request_info *info,
                struct iw_point *dwrq, char *extra)
{
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *req = NULL;
    int copy_size = 0;
    
    MLAN_UNUSED_ARG(info);

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    misc = (mlan_ds_misc_cfg *) req->pbuf;
    misc->sub_command = MLAN_OID_MISC_GEN_IE;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    req->action = MLAN_ACT_GET;
    misc->param.gen_ie.type = MLAN_IE_TYPE_GEN_IE;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    copy_size = min(misc->param.gen_ie.len, dwrq->length);
    ewl_os_memcpy(extra, misc->param.gen_ie.ie_data, copy_size);
    dwrq->length = copy_size;
  done:
    if (req)
        ewl_os_free(req);

    LEAVE();
    return ret;
}


/**
 *  @brief Set IE
 *
 *  Pass an opaque block of data, expected to be IEEE IEs, to the driver 
 *    for eventual passthrough to the firmware in an associate/join 
 *    (and potentially start) command.
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param dwrq                 A pointer to iw_point structure
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_set_gen_ie(moal_private * priv, struct iw_request_info *info,
                struct iw_point *dwrq, char *extra)
{
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *req = NULL;

    MLAN_UNUSED_ARG(info);

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    misc = (mlan_ds_misc_cfg *) req->pbuf;
    misc->sub_command = MLAN_OID_MISC_GEN_IE;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    req->action = MLAN_ACT_SET;

    if (dwrq->length > MAX_IE_SIZE) {
        ret = -EFAULT;
        goto done;
    }
    misc->param.gen_ie.type = MLAN_IE_TYPE_GEN_IE;
    misc->param.gen_ie.len = dwrq->length;
    ewl_os_memcpy(misc->param.gen_ie.ie_data, extra, misc->param.gen_ie.len);
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
  done:
    if (req)
        ewl_os_free(req);

    LEAVE();
    return ret;
}
/**
 *  @brief Get frequency
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param fwrq                 A pointer to iw_freq structure
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_get_freq(moal_private *priv, struct iw_freq *fwrq)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_CHANNEL;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    fwrq->m = (long) bss->param.bss_chan.freq;
    fwrq->i = (long) bss->param.bss_chan.channel;
    fwrq->e = 6;
    fwrq->flags = IW_FREQ_FIXED;
  done:
    if (req)
        ewl_os_free(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Set frequency
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param fwrq                 A pointer to iw_freq structure
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_set_freq(moal_private *priv, struct iw_request_info *info,
              struct iw_freq *fwrq, char *extra)
{
    int ret = 0;
    
    MLAN_UNUSED_ARG(priv);
    MLAN_UNUSED_ARG(fwrq);
    MLAN_UNUSED_ARG(info);
    MLAN_UNUSED_ARG(extra);
#if 0
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    bss = (mlan_ds_bss *) req->pbuf;
    /* 
     * If setting by frequency, convert to a channel 
     */
    if (fwrq->e == 1) {
        long f = fwrq->m / 100000;
        bss->param.bss_chan.freq = f;
    } else
        bss->param.bss_chan.channel = fwrq->m;

    bss->sub_command = MLAN_OID_BSS_CHANNEL;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (MLAN_STATUS_SUCCESS !=
        woal_change_adhoc_chan(priv, bss->param.bss_chan.channel))
        ret = -EFAULT;

  done:
    if (req)
        ewl_os_free(req);
#endif        
    LEAVE();
    return ret;
}

/**
 *  @brief Get authentication mode parameters
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param vwrq                 A pointer to iw_param structure
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_get_auth(moal_private *priv, struct iw_request_info *info,
              struct iw_param *vwrq, char *extra)
{
    int ret = 0;
    //t_u32 encrypt_mode = 0;
    t_u32 auth_mode;
    //t_u32 wpa_enable;
    
    MLAN_UNUSED_ARG(info);
    MLAN_UNUSED_ARG(extra);
    
    ENTER();
    switch (vwrq->flags & IW_AUTH_INDEX) {
    case IW_AUTH_CIPHER_PAIRWISE:
    case IW_AUTH_CIPHER_GROUP:
//        if (MLAN_STATUS_SUCCESS !=
//            woal_get_encrypt_mode(priv, MOAL_IOCTL_WAIT, &encrypt_mode))
//            ret = -EFAULT;
//        else {
//            if (encrypt_mode == MLAN_ENCRYPTION_MODE_NONE)
//                vwrq->value = IW_AUTH_CIPHER_NONE;
//            else if (encrypt_mode == MLAN_ENCRYPTION_MODE_WEP40)
//                vwrq->value = IW_AUTH_CIPHER_WEP40;
//            else if (encrypt_mode == MLAN_ENCRYPTION_MODE_TKIP)
//                vwrq->value = IW_AUTH_CIPHER_TKIP;
//            else if (encrypt_mode == MLAN_ENCRYPTION_MODE_CCMP)
//                vwrq->value = IW_AUTH_CIPHER_CCMP;
//            else if (encrypt_mode == MLAN_ENCRYPTION_MODE_WEP104)
//                vwrq->value = IW_AUTH_CIPHER_WEP104;
//        }
        break;
    case IW_AUTH_80211_AUTH_ALG:
        if (MLAN_STATUS_SUCCESS !=
            woal_get_auth_mode(priv, MOAL_IOCTL_WAIT, &auth_mode))
            ret = -EFAULT;
        else {
            if (auth_mode == MLAN_AUTH_MODE_SHARED)
                vwrq->value = IW_AUTH_ALG_SHARED_KEY;
            else if (auth_mode == MLAN_AUTH_MODE_NETWORKEAP)
                vwrq->value = IW_AUTH_ALG_LEAP;
            else
                vwrq->value = IW_AUTH_ALG_OPEN_SYSTEM;
        }
        break;
//    case IW_AUTH_WPA_ENABLED:
//        if (MLAN_STATUS_SUCCESS !=
//            woal_get_wpa_enable(priv, MOAL_IOCTL_WAIT, &wpa_enable))
//            ret = -EFAULT;
//        else
//            vwrq->value = wpa_enable;
        break;
    case IW_AUTH_WPA_VERSION:
        vwrq->value = priv->wpa_version;
        break;
    case IW_AUTH_KEY_MGMT:
        vwrq->value = priv->key_mgmt;
        break;
    case IW_AUTH_TKIP_COUNTERMEASURES:
    case IW_AUTH_DROP_UNENCRYPTED:
    case IW_AUTH_RX_UNENCRYPTED_EAPOL:
    case IW_AUTH_ROAMING_CONTROL:
    case IW_AUTH_PRIVACY_INVOKED:
    default:
        ret = -EOPNOTSUPP;
        goto done;
    }

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief  Extended version of encoding configuration 
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param dwrq         A pointer to iw_point structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return              0 --success, otherwise fail
 */
int
woal_set_encode_ext(moal_private *priv ,
                    struct iw_request_info *info,
                    struct iw_point *dwrq, char *extra)
{
    struct iw_encode_ext *ext = (struct iw_encode_ext *) extra;
    int key_index;
    t_u8 *pkey_material = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    int ret = 0;
    
    MLAN_UNUSED_ARG(info);
    
    ENTER();
    key_index = (dwrq->flags & IW_ENCODE_INDEX) - 1;
    if (key_index < 0 || key_index > 3) {
        ret = -EINVAL;
        goto done;
    }
    if (ext->key_len > (dwrq->length - sizeof(struct iw_encode_ext))) {
        ret = -EINVAL;
        goto done;
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    pkey_material = (t_u8 *) (ext + 1);
    sec->param.encrypt_key.key_len = ext->key_len;

    /* Disable Key */
    if ((dwrq->flags & IW_ENCODE_DISABLED) && !ext->key_len) {
        sec->param.encrypt_key.key_disable = MTRUE;
    } else if (ext->key_len <= MAX_WEP_KEY_SIZE) {
        /* Set WEP key */
        sec->param.encrypt_key.key_index = key_index;
        ewl_os_memcpy(sec->param.encrypt_key.key_material, pkey_material,
               ext->key_len);
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
        if (ext->ext_flags & IW_ENCODE_EXT_SET_TX_KEY) {
            sec->param.encrypt_key.key_len = 0;
        }
    } else {
        /* Set WPA key */
        sec->param.encrypt_key.key_index = key_index;
        ewl_os_memcpy(sec->param.encrypt_key.key_material, pkey_material,
               ext->key_len);
      
#define IW_ENCODE_ALG_SMS4   0x20
        /* Set WAPI key */
        if (ext->alg == IW_ENCODE_ALG_SMS4) {
            sec->param.encrypt_key.is_wapi_key = MTRUE;
            ewl_os_memcpy(sec->param.encrypt_key.mac_addr, (u8 *) ext->addr.sa_data,
                   ETH_ALEN);
            ewl_os_memcpy(sec->param.encrypt_key.pn, ext->tx_seq, PN_SIZE);
        } else if ((ext->ext_flags & IW_ENCODE_EXT_GROUP_KEY) && !key_index) {
            key_index = 1;
            sec->param.encrypt_key.key_index = key_index;
            PRINTM(MWARN, "Key index changed for GTK: %ld\n",
                   sec->param.encrypt_key.key_index);
        }
    }
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT))
        ret = -EFAULT;
  done:
    if (req)
        ewl_os_free(req);
    LEAVE();
    return ret;
}

/** 
 *  @brief  Extended version of encoding configuration 
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param dwrq         A pointer to iw_point structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return             -EOPNOTSUPP
 */
int
woal_get_encode_ext(moal_private *priv,
                    struct iw_request_info *info,
                    struct iw_point *dwrq, char *extra)
{
    
    MLAN_UNUSED_ARG(priv);
    MLAN_UNUSED_ARG(dwrq);
    MLAN_UNUSED_ARG(info);
    MLAN_UNUSED_ARG(extra);

    ENTER();
    LEAVE();
    return -EOPNOTSUPP;
}

/* Data rate listing
 *      MULTI_BANDS:
 *              abg             a       b       b/g
 *  Infra       G(12)           A(8)    B(4)    G(12)
 *  Adhoc       A+B(12)         A(8)    B(4)    B(4)
 *      non-MULTI_BANDS:
                                        b       b/g
 *  Infra                               B(4)    G(12)
 *  Adhoc                               B(4)    B(4)
 */
/**
 *  @brief Get Range Info
 *
 *  @param dev                  A pointer to net_device structure
 *  @param info                 A pointer to iw_request_info structure
 *  @param dwrq                 A pointer to iw_point structure
 *  @param extra                A pointer to extra data buf
 *
 *  @return                     0 --success, otherwise fail
 */
int
woal_get_range(moal_private *priv, struct iw_request_info *info,
               struct iw_point *dwrq, char *extra)
{
    int i;
    struct iw_range *range = (struct iw_range *) extra;
    moal_802_11_rates rates;
    mlan_chan_list *pchan_list = NULL;
    mlan_bss_info bss_info;

    MLAN_UNUSED_ARG(info);

    ENTER();
    
    if (!(pchan_list = ewl_os_malloc(sizeof(mlan_chan_list)))) {
        LEAVE();
        return -ENOMEM;
    }

    dwrq->length = sizeof(struct iw_range);
    ewl_os_memset(range, 0, sizeof(struct iw_range));

    range->min_nwid = 0;
    range->max_nwid = 0;

    ewl_os_memset(&rates, 0, sizeof(rates));
    woal_get_data_rates(priv, MOAL_IOCTL_WAIT, &rates);
    range->num_bitrates = rates.num_of_rates;

    for (i = 0; i < min(range->num_bitrates, IW_MAX_BITRATES) && rates.rates[i];
         i++) {
        range->bitrate[i] = (rates.rates[i] & 0x7f) * 500000;
    }
    
    range->num_bitrates = i;
    PRINTM(MINFO, "IW_MAX_BITRATES=%d num_bitrates=%d\n", IW_MAX_BITRATES,
           range->num_bitrates);      

    range->num_frequency = 0;

    ewl_os_memset(pchan_list, 0, sizeof(mlan_chan_list));

    woal_get_channel_list(priv, MOAL_IOCTL_WAIT, pchan_list);

    range->num_frequency = min(pchan_list->num_of_chan, IW_MAX_FREQUENCIES);

    for (i = 0; i < range->num_frequency; i++) {
        range->freq[i].i = (long) pchan_list->cf[i].channel;
        range->freq[i].m = (long) pchan_list->cf[i].freq * 100000;
        range->freq[i].e = 1;
    }

    ewl_os_free(pchan_list);

    range->num_channels = range->num_frequency;

    woal_sort_channels(&range->freq[0], range->num_frequency);

    /* 
     * Set an indication of the max TCP throughput in bit/s that we can
     * expect using this interface
     */
    if (i > 2)
        range->throughput = 5000 * 1000;
    else
        range->throughput = 1500 * 1000;

    range->min_rts = MLAN_RTS_MIN_VALUE;
    range->max_rts = MLAN_RTS_MAX_VALUE;
    range->min_frag = MLAN_FRAG_MIN_VALUE;
    range->max_frag = MLAN_FRAG_MAX_VALUE;

    range->encoding_size[0] = 5;
    range->encoding_size[1] = 13;
    range->num_encoding_sizes = 2;
    range->max_encoding_tokens = 4;

/** Minimum power period */
#define IW_POWER_PERIOD_MIN 1000000     /* 1 sec */
/** Maximum power period */
#define IW_POWER_PERIOD_MAX 120000000   /* 2 min */
/** Minimum power timeout value */
#define IW_POWER_TIMEOUT_MIN 1000       /* 1 ms */
/** Maximim power timeout value */
#define IW_POWER_TIMEOUT_MAX 1000000    /* 1 sec */

    /* Power Management duration & timeout */
    range->min_pmp = IW_POWER_PERIOD_MIN;
    range->max_pmp = IW_POWER_PERIOD_MAX;
    range->min_pmt = IW_POWER_TIMEOUT_MIN;
    range->max_pmt = IW_POWER_TIMEOUT_MAX;
    range->pmp_flags = IW_POWER_PERIOD;
    range->pmt_flags = IW_POWER_TIMEOUT;
    range->pm_capa = IW_POWER_PERIOD | IW_POWER_TIMEOUT | IW_POWER_ALL_R;

    /* 
     * Minimum version we recommend
     */
    range->we_version_source = 15;

    /* 
     * Version we are compiled with
     */
    range->we_version_compiled = WIRELESS_EXT;

    range->retry_capa = IW_RETRY_LIMIT;
    range->retry_flags = IW_RETRY_LIMIT | IW_RETRY_MAX;

    range->min_retry = MLAN_TX_RETRY_MIN;
    range->max_retry = MLAN_TX_RETRY_MAX;

    /* 
     * Set the qual, level and noise range values
     */
    /* 
     * need to put the right values here
     */
/** Maximum quality percentage */
#define IW_MAX_QUAL_PERCENT 5
/** Average quality percentage */
#define IW_AVG_QUAL_PERCENT 3
    range->max_qual.qual = IW_MAX_QUAL_PERCENT;
    range->max_qual.level = 0;
    range->max_qual.noise = 0;

    range->avg_qual.qual = IW_AVG_QUAL_PERCENT;
    range->avg_qual.level = 0;
    range->avg_qual.noise = 0;

    range->sensitivity = 0;
    /* 
     * Setup the supported power level ranges
     */
    ewl_os_memset(range->txpower, 0, sizeof(range->txpower));

    ewl_os_memset(&bss_info, 0, sizeof(bss_info));

    woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);

    range->txpower[0] = bss_info.min_power_level;
    range->txpower[1] = bss_info.max_power_level;
    range->num_txpower = 2;
    range->txpower_capa = IW_TXPOW_DBM | IW_TXPOW_RANGE;

    LEAVE();

    return 0;
}


/**
 *  @brief Scan Network
 *
 *  @param dev          A pointer to net_device structure
 *  @param info         A pointer to iw_request_info structure
 *  @param vwrq         A pointer to iw_param structure
 *  @param extra        A pointer to extra data buf
 *
 *  @return             0--success, otherwise fail
 */
int
woal_set_scan1(moal_private *priv, struct iw_request_info *info,
              struct iw_param *vwrq, char *extra)
{
    int ret = 0;    
    MLAN_UNUSED_ARG(info);
#ifdef REASSOCIATION
    moal_handle *handle = priv->phandle;
#endif
#if WIRELESS_EXT >= 18
    struct iw_scan_req *req;
    struct iw_point *dwrq = (struct iw_point *) vwrq;
#endif
    mlan_802_11_ssid req_ssid;

    ENTER();
    if (priv->scan_pending_on_block == MTRUE) {
        PRINTM(MINFO, "scan already in processing...\n");
        LEAVE();
        return ret;
    }
    if ((priv->media_connected == MTRUE) && (priv->scan_block_flag == MTRUE)) {
        PRINTM(MIOCTL, "scan block on\n");
        LEAVE();
        return ret;
    }
#ifdef REASSOCIATION
    if (MOAL_ACQ_SEMAPHORE_BLOCK(&handle->reassoc_sem)) {
        PRINTM(MERROR, "Acquire semaphore error, woal_set_scan\n");
        LEAVE();
        return -EBUSY;
    }
#endif /* REASSOCIATION */
    priv->report_scan_result = MTRUE;

    ewl_os_memset(&req_ssid, 0x00, sizeof(mlan_802_11_ssid));

#if WIRELESS_EXT >= 18
    if ((dwrq->flags & IW_SCAN_THIS_ESSID) &&
        (dwrq->length == sizeof(struct iw_scan_req))) {
        req = (struct iw_scan_req *) extra;

        if (req->essid_len <= MLAN_MAX_SSID_LENGTH) {

            req_ssid.ssid_len = req->essid_len;
            ewl_os_memcpy(req_ssid.ssid, (t_u8 *) req->essid, req->essid_len);
            if (MLAN_STATUS_SUCCESS !=
                woal_request_scan(priv, MOAL_NO_WAIT, &req_ssid)) {
                ret = -EFAULT;
                goto done;
            }
        }
    } else {
#endif
        if (MLAN_STATUS_SUCCESS != woal_request_scan(priv, MOAL_IOCTL_WAIT, NULL)) {
            ret = -EFAULT;
            goto done;
        }
#if WIRELESS_EXT >= 18
    }
#endif

#ifdef REASSOCIATION
    if (priv->phandle->surprise_removed) {
        ret = -EFAULT;
        goto done;
    }
#endif

  done:
#ifdef REASSOCIATION
    MOAL_REL_SEMAPHORE(&handle->reassoc_sem);
#endif

    LEAVE();
    return ret;
}






/**                                             
 * iwconfig settable callbacks
 */
static const iw_handler woal_handler[] = {
    (iw_handler) NULL,                  /* SIOCSIWCOMMIT      0*/
    (iw_handler) NULL,                  /* SIOCGIWNAME        1*/
    (iw_handler) NULL,                  /* SIOCSIWNWID        2*/
    (iw_handler) NULL,                  /* SIOCGIWNWID        3*/
    (iw_handler) woal_set_freq,         /* SIOCSIWFREQ        4*/                                                                                   
    (iw_handler) NULL,                  /* SIOCGIWFREQ        5*/
    (iw_handler) woal_set_bss_mode,     /* SIOCSIWMODE        6*/                                                                                   
    (iw_handler) woal_get_bss_mode,     /* SIOCGIWMODE        7*/                                                                                   
    (iw_handler) NULL,                  /* SIOCSIWSENS        8*/
    (iw_handler) NULL,                  /* SIOCGIWSENS        9*/
    (iw_handler) NULL,                  /* SIOCSIWRANGE       A*/
    (iw_handler) woal_get_range,        /* SIOCGIWRANGE       B*/                                                                                   
    (iw_handler) NULL,                  /* SIOCSIWPRIV        C*/
    (iw_handler) NULL,                  /* SIOCGIWPRIV        D*/
    (iw_handler) NULL,                  /* SIOCSIWSTATS       E*/
    (iw_handler) NULL,                  /* SIOCGIWSTATS       F*/
    (iw_handler) NULL,                  /* SIOCSIWSPY        10*/
    (iw_handler) NULL,                  /* SIOCGIWSPY        11*/
    (iw_handler) NULL,                  /* SIOCSIWTHRSPY     12*/
    (iw_handler) NULL,                  /* SIOCGIWTHRSPY     13*/
    (iw_handler) woal_set_wap,          /* SIOCSIWAP         14*/                                                                                   
    (iw_handler) woal_get_wap,          /* SIOCGIWAP         15*/                                                                                   
    (iw_handler) woal_set_mlme,         /* SIOC IWMLME       16*/                                                                                   
    (iw_handler) NULL,                  /* SIOCGIWAPLIST     17*/
    (iw_handler) woal_set_scan1,        /* SIOCSIWSCAN       18*/                                                                                   
    (iw_handler) woal_get_scan1,        /* SIOCGIWSCAN       19*/
    (iw_handler) woal_set_essid1,       /* SIOCSIWESSID      1A*/                                                                                   
    (iw_handler) woal_get_essid,        /* SIOCGIWESSID      1B*/                                                                                   
    (iw_handler) NULL,                  /* SIOCSIWNICKN      1C*/
    (iw_handler) NULL,                  /* SIOCGIWNICKN      1D*/
    (iw_handler) NULL,                  /* -- hole --        1E*/
    (iw_handler) NULL,                  /* -- hole --        1F*/
    (iw_handler) NULL,                  /* SIOCSIWRATE       20*/
    (iw_handler) NULL,                  /* SIOCGIWRATE       21*/
    (iw_handler) NULL,                  /* SIOCSIWRTS        22*/
    (iw_handler) NULL,                  /* SIOCGIWRTS        23*/
    (iw_handler) NULL,                  /* SIOCSIWFRAG       24*/
    (iw_handler) NULL,                  /* SIOCGIWFRAG       25*/
    (iw_handler) NULL,                  /* SIOCSIWTXPOW      26*/
    (iw_handler) NULL,                  /* SIOCGIWTXPOW      27*/
    (iw_handler) NULL,                  /* SIOCSIWRETRY      28*/
    (iw_handler) NULL,                  /* SIOCGIWRETRY      29*/
    (iw_handler) woal_set_encode,       /* SIOCSIWENCODE     2A*/
    (iw_handler) woal_get_encode,       /* SIOCGIWENCODE     2B*/
    (iw_handler) NULL,                  /* SIOCSIWPOWER      2C*/
    (iw_handler) NULL,                  /* SIOCGIWPOWER      2D*/
    (iw_handler) NULL,                  /* -- hole -- */                                                                                    
    (iw_handler) NULL,                  /* -- hole -- */                                                                                    
    (iw_handler) woal_set_gen_ie,       /* SIOCSIWGENIE      0x8b30*/                                                                                   
    (iw_handler) woal_get_gen_ie,       /* SIOCGIWGENIE      0x8b31*/                                                                                   
    (iw_handler) woal_set_auth,         /* SIOCSIWAUTH       0x8b32*/                                                                                   
    (iw_handler) woal_get_auth,         /* SIOCGIWAUTH       0x8b33*/                                                                                   
    (iw_handler) woal_set_encode_ext,   /* SIOCSIWENCODEEXT  0x8b34*/                                                                                   
    (iw_handler) woal_get_encode_ext,   /* SIOCGIWENCODEEXT  0x8b35*/                                                                                   
};  




/** wlan_handler_def */
struct iw_handler_def woal_handler_def = {
  num_standard:sizeof(woal_handler) / sizeof(iw_handler),
  standard:(iw_handler *) woal_handler,
};
                            
#endif
