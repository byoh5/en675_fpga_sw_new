#include "dev.h"
#include "uaputl.h"
#include "ewl_os.h"

/** custom IE, auto mask value */
#define	UAP_CUSTOM_IE_AUTO_MASK	0xffff

//#include "mlan_ieee.h"
/** 
 *  @brief Dump hex data
 *
 *  @param prompt	A pointer prompt buffer
 *  @param p		A pointer to data buffer
 *  @param len		The len of data buffer
 *  @param delim	Delim char
 *  @return            	None
 */
void
hexdump_data(char *prompt, void *p, int len, char delim)
{
    int i;
    unsigned char *s = p;

    if (prompt) {
        dprintf("%s: len=%d\n", prompt, (int) len);
    }
    for (i = 0; i < len; i++) {
        if (i != len - 1){
            dprintf("%02x%c", *s++, delim);
        }else{
            dprintf("%02x\n", *s);}
        if ((i + 1) % 16 == 0){
            dprintf("\n");}
    }
    dprintf("\n");
}


/** 
 *  @brief Prints a MAC address in colon separated form from hex data
 *
 *  @param raw      A pointer to the hex data buffer
 *  @return         N/A
 */
void
print_mac(t_u8 * raw)
{
    dprintf("%02x:%02x:%02x:%02x:%02x:%02x", (unsigned int) raw[0],
           (unsigned int) raw[1], (unsigned int) raw[2], (unsigned int) raw[3],
           (unsigned int) raw[4], (unsigned int) raw[5]);
    return;
}

/**
 *  @brief Show auth tlv 
 *
 *  @param tlv     Pointer to auth tlv
 *  
 *  $return         N/A
 */
void
print_auth(tlvbuf_auth_mode * tlv)
{
    switch (tlv->auth_mode) {
    case 0:
        dprintf("AUTHMODE = Open authentication\n");
        break;
    case 1:
        dprintf("AUTHMODE = Shared key authentication\n");
        break;
    case 255:
        dprintf("AUTHMODE = Auto (open and shared key)\n");
        break;
    default:
        dprintf("ERR: Invalid authmode=%d\n", tlv->auth_mode);
        break;
    }
}

/**
 *
 *  @brief Show cipher tlv 
 *
 *  @param tlv     Pointer to cipher tlv
 *  
 *  $return         N/A
 */
void
print_cipher(tlvbuf_cipher * tlv)
{
    switch (tlv->pairwise_cipher) {
    case CIPHER_TKIP:
        dprintf("PairwiseCipher = TKIP\n");
        break;
    case CIPHER_AES_CCMP:
        dprintf("PairwiseCipher = AES CCMP\n");
        break;
    case CIPHER_TKIP | CIPHER_AES_CCMP:
        dprintf("PairwiseCipher = TKIP + AES CCMP\n");
        break;
    case CIPHER_NONE:
        dprintf("PairwiseCipher =  None\n");
        break;
    default:
        dprintf("Unknown Pairwise cipher 0x%x\n", tlv->pairwise_cipher);
        break;
    }
    switch (tlv->group_cipher) {
    case CIPHER_TKIP:
        dprintf("GroupCipher = TKIP\n");
        break;
    case CIPHER_AES_CCMP:
        dprintf("GroupCipher = AES CCMP\n");
        break;
    case CIPHER_NONE:
        dprintf("GroupCipher = None\n");
        break;
    default:
        dprintf("Unknown Group cipher 0x%x\n", tlv->group_cipher);
        break;
    }
}

/**
 *  @brief Show pairwise cipher tlv 
 *
 *  @param tlv     Pointer to pairwise cipher tlv
 *  
 *  $return         N/A
 */
void
print_pwk_cipher(tlvbuf_pwk_cipher * tlv)
{
    switch (tlv->protocol) {
    case PROTOCOL_WPA:
        dprintf("Protocol WPA  : ");
        break;
    case PROTOCOL_WPA2:
        dprintf("Protocol WPA2 : ");
        break;
    default:
        dprintf("Unknown Protocol 0x%x\n", tlv->protocol);
        break;
    }

    switch (tlv->pairwise_cipher) {
    case CIPHER_TKIP:
        dprintf("PairwiseCipher = TKIP\n");
        break;
    case CIPHER_AES_CCMP:
        dprintf("PairwiseCipher = AES CCMP\n");
        break;
    case CIPHER_TKIP | CIPHER_AES_CCMP:
        dprintf("PairwiseCipher = TKIP + AES CCMP\n");
        break;
    case CIPHER_NONE:
        dprintf("PairwiseCipher =  None\n");
        break;
    default:
        dprintf("Unknown Pairwise cipher 0x%x\n", tlv->pairwise_cipher);
        break;
    }
}

/**
 *  @brief Show group cipher tlv 
 *
 *  @param tlv     Pointer to group cipher tlv
 *  
 *  $return         N/A
 */
void
print_gwk_cipher(tlvbuf_gwk_cipher * tlv)
{
    switch (tlv->group_cipher) {
    case CIPHER_TKIP:
        dprintf("GroupCipher = TKIP\n");
        break;
    case CIPHER_AES_CCMP:
        dprintf("GroupCipher = AES CCMP\n");
        break;
    case CIPHER_NONE:
        dprintf("GroupCipher = None\n");
        break;
    default:
        dprintf("Unknown Group cipher 0x%x\n", tlv->group_cipher);
        break;
    }
}

/**
 *  @brief Show mac filter tlv 
 *
 *  @param tlv     Pointer to filter tlv
 *  
 *  $return         N/A
 */
void
print_mac_filter(tlvbuf_sta_mac_addr_filter * tlv)
{
    int i;
    switch (tlv->filter_mode) {
    case 0:
        dprintf("Filter Mode = Filter table is disabled\n");
        return;
    case 1:
        if (!tlv->count) {
            dprintf("No mac address is allowed to connect\n");
        } else {
            dprintf("Filter Mode = Allow mac address specified in the allowed list\n");
        }
        break;
    case 2:
        if (!tlv->count) {
            dprintf("No mac address is blocked\n");
        } else {
            dprintf("Filter Mode = Block MAC addresses specified in the banned list\n");
        }
        break;
    }
    for (i = 0; i < tlv->count; i++) {
        dprintf("MAC_%d = ", i);
        print_mac(&tlv->mac_address[i * ETH_ALEN]);
        dprintf("\n");
    }
}

/**
 *  @brief Show protocol tlv 
 *
 *  @param protocol Protocol number
 *  
 *  $return         N/A
 */
void
print_protocol(t_u16 protocol)
{
    switch (protocol) {
    case 0:
    case PROTOCOL_NO_SECURITY:
        dprintf("PROTOCOL = No security\n");
        break;
    case PROTOCOL_STATIC_WEP:
        dprintf("PROTOCOL = Static WEP\n");
        break;
    case PROTOCOL_WPA:
        dprintf("PROTOCOL = WPA \n");
        break;
    case PROTOCOL_WPA2:
        dprintf("PROTOCOL = WPA2 \n");
        break;
    case PROTOCOL_WPA | PROTOCOL_WPA2:
        dprintf("PROTOCOL = WPA/WPA2 \n");
        break;
    default:
        dprintf("Unknown PROTOCOL: 0x%x \n", protocol);
        break;
    }
}

/**
 *  @brief Show wep tlv 
 *
 *  @param tlv     Pointer to wep tlv
 *  
 *  $return         N/A
 */
void
print_wep_key(tlvbuf_wep_key * tlv)
{
    int i;
    t_u16 tlv_len;

    tlv_len = *(t_u8 *) & tlv->length;
    tlv_len |= (*((t_u8 *) & tlv->length + 1) << 8);

    if (tlv_len <= 2) {
        dprintf("wrong wep_key tlv: length=%d\n", tlv_len);
        return;
    }
    dprintf("WEP KEY_%d = ", tlv->key_index);
    for (i = 0; i < tlv_len - 2; i++){
        dprintf("%02x ", tlv->key[i]);}
    if (tlv->is_default){
        dprintf("\nDefault WEP Key = %d\n", tlv->key_index);
    }else{
        dprintf("\n");}
}

/**
 *  @brief Show rate tlv 
 *
 *  @param tlv      Pointer to rate tlv
 *  
 *  $return         N/A
 */
void
print_rate(tlvbuf_rates * tlv)
{
    int flag = 0;
    int i;
    t_u16 tlv_len;

    tlv_len = *(t_u8 *) & tlv->length;
    tlv_len |= (*((t_u8 *) & tlv->length + 1) << 8);

    dprintf("Basic Rates =");
    for (i = 0; i < tlv_len; i++) {
        if (tlv->operational_rates[i] > (BASIC_RATE_SET_BIT - 1)) {
            flag = flag ? : 1;
            dprintf(" 0x%x", tlv->operational_rates[i]);
        }
    }
    dprintf("%s\nNon-Basic Rates =", flag ? "" : " ( none ) ");
    for (flag = 0, i = 0; i < tlv_len; i++) {
        if (tlv->operational_rates[i] < BASIC_RATE_SET_BIT) {
            flag = flag ? : 1;
            dprintf(" 0x%x", tlv->operational_rates[i]);
        }
    }
    dprintf("%s\n", flag ? "" : " ( none ) ");
}

/**
 *  @brief Show all the tlv in the buf
 *
 *  @param buf     Pointer to tlv buffer
 *  @param len     Tlv buffer len
 *  
 *  $return         N/A
 */
void
print_tlv(t_u8 * buf, t_u16 len)
{
    tlvbuf_header *pcurrent_tlv = (tlvbuf_header *) buf;
    int tlv_buf_left = len;
    t_u16 tlv_type;
    t_u16 tlv_len;
    t_u16 tlv_val_16;
    t_u32 tlv_val_32;
    t_u8 ssid[33];
    int i = 0;
    tlvbuf_ap_mac_address *mac_tlv;
    tlvbuf_ssid *ssid_tlv;
    tlvbuf_beacon_period *beacon_tlv;
    tlvbuf_dtim_period *dtim_tlv;
    tlvbuf_rates *rates_tlv;
    tlvbuf_tx_power *txpower_tlv;
    tlvbuf_bcast_ssid_ctl *bcast_tlv;
    tlvbuf_preamble_ctl *preamble_tlv;
    tlvbuf_bss_status *bss_status_tlv;
    tlvbuf_antenna_ctl *antenna_tlv;
    tlvbuf_rts_threshold *rts_tlv;
    tlvbuf_tx_data_rate *txrate_tlv;
    tlvbuf_mcbc_data_rate *mcbcrate_tlv;
    tlvbuf_pkt_fwd_ctl *pkt_fwd_tlv;
    tlvbuf_sta_ageout_timer *ageout_tlv;
    tlvbuf_ps_sta_ageout_timer *ps_ageout_tlv;
    tlvbuf_auth_mode *auth_tlv;
    tlvbuf_protocol *proto_tlv;
    tlvbuf_akmp *akmp_tlv;
    tlvbuf_cipher *cipher_tlv;
    tlvbuf_pwk_cipher *pwk_cipher_tlv;
    tlvbuf_gwk_cipher *gwk_cipher_tlv;
    tlvbuf_group_rekey_timer *rekey_tlv;
    tlvbuf_wpa_passphrase *psk_tlv;
    tlvbuf_wep_key *wep_tlv;
    tlvbuf_frag_threshold *frag_tlv;
    tlvbuf_sta_mac_addr_filter *filter_tlv;
    tlvbuf_max_sta_num *max_sta_tlv;
    tlvbuf_retry_limit *retry_limit_tlv;
    tlvbuf_eapol_pwk_hsk_timeout *pwk_timeout_tlv;
    tlvbuf_eapol_pwk_hsk_retries *pwk_retries_tlv;
    tlvbuf_eapol_gwk_hsk_timeout *gwk_timeout_tlv;
    tlvbuf_eapol_gwk_hsk_retries *gwk_retries_tlv;
    tlvbuf_channel_config *channel_tlv;
    tlvbuf_channel_list *chnlist_tlv;
    channel_list *pchan_list;
    t_u16 custom_ie_len;
    tlvbuf_rsn_replay_prot *replay_prot_tlv;
    tlvbuf_custom_ie *custom_ie_tlv;
    custom_ie *custom_ie_ptr;
    tlvbuf_wmm_para_t *wmm_para_tlv;
    int flag = 0;
    tlvbuf_htcap_t *ht_cap_tlv;
    tlvbuf_htinfo_t *ht_info_tlv;
    tlvbuf_2040_coex *coex_2040_tlv;

#ifdef DEBUG
    uap_printf(MSG_DEBUG, "tlv total len=%d\n", len);
#endif
    while (tlv_buf_left >= (int) sizeof(tlvbuf_header)) {
        tlv_type = *(t_u8 *) & pcurrent_tlv->type;
        tlv_type |= (*((t_u8 *) & pcurrent_tlv->type + 1) << 8);
        tlv_len = *(t_u8 *) & pcurrent_tlv->len;
        tlv_len |= (*((t_u8 *) & pcurrent_tlv->len + 1) << 8);
        if ((sizeof(tlvbuf_header) + tlv_len) > (unsigned int) tlv_buf_left) {
            dprintf("wrong tlv: tlv_len=%d, tlv_buf_left=%d\n", tlv_len,
                   tlv_buf_left);
            break;
        }
        switch (tlv_type) {
        case MRVL_AP_MAC_ADDRESS_TLV_ID:
            mac_tlv = (tlvbuf_ap_mac_address *) pcurrent_tlv;
            dprintf("AP MAC address = ");
            print_mac(mac_tlv->ap_mac_addr);
            dprintf("\n");
            break;
        case MRVL_SSID_TLV_ID:
            ewl_os_memset(ssid, 0, sizeof(ssid));
            ssid_tlv = (tlvbuf_ssid *) pcurrent_tlv;
            ewl_os_memcpy(ssid, ssid_tlv->ssid, tlv_len);
            dprintf("SSID = %s\n", ssid);
            break;
        case MRVL_BEACON_PERIOD_TLV_ID:
            beacon_tlv = (tlvbuf_beacon_period *) pcurrent_tlv;
            tlv_val_16 = *(t_u8 *) & beacon_tlv->beacon_period_ms;
            tlv_val_16 |= (*((t_u8 *) & beacon_tlv->beacon_period_ms + 1) << 8);
            dprintf("Beacon period = %d\n", tlv_val_16);
            break;
        case MRVL_DTIM_PERIOD_TLV_ID:
            dtim_tlv = (tlvbuf_dtim_period *) pcurrent_tlv;
            dprintf("DTIM period = %d\n", dtim_tlv->dtim_period);
            break;
        case MRVL_CHANNELCONFIG_TLV_ID:
            channel_tlv = (tlvbuf_channel_config *) pcurrent_tlv;
            dprintf("Channel = %d\n", channel_tlv->chan_number);
            dprintf("Band = %s\n",
                   (channel_tlv->
                    band_config_type & BAND_CONFIG_5GHZ) ? "5GHz" : "2.4GHz");
            dprintf("Channel Select Mode = %s\n",
                   (channel_tlv->
                    band_config_type & BAND_CONFIG_ACS_MODE) ? "ACS" :
                   "Manual");
            channel_tlv->band_config_type &= 0x30;
            if (channel_tlv->band_config_type == 0){
                dprintf("no secondary channel\n");
            }else if (channel_tlv->band_config_type == SECOND_CHANNEL_ABOVE){
                dprintf("secondary channel is above primary channel\n");
            }else if (channel_tlv->band_config_type == SECOND_CHANNEL_BELOW){
                dprintf("secondary channel is below primary channel\n");}
            break;
        case MRVL_CHANNELLIST_TLV_ID:
            chnlist_tlv = (tlvbuf_channel_list *) pcurrent_tlv;
            dprintf("Channels List = ");
            pchan_list = (channel_list *) & (chnlist_tlv->chan_list);
            if (tlv_len % sizeof(channel_list)) {
                break;
            }
            for (i = 0; (unsigned int) i < (tlv_len / sizeof(channel_list));
                 i++) {
                dprintf("\n%d\t%sGHz", pchan_list->chan_number,
                       (pchan_list->
                        band_config_type & BAND_CONFIG_5GHZ) ? "5" : "2.4");
                pchan_list++;
            }
            dprintf("\n");
            break;
        case MRVL_RSN_REPLAY_PROT_TLV_ID:
            replay_prot_tlv = (tlvbuf_rsn_replay_prot *) pcurrent_tlv;
            dprintf("RSN replay protection = %s\n",
                   replay_prot_tlv->rsn_replay_prot ? "enabled" : "disabled");
            break;
        case MRVL_RATES_TLV_ID:
            rates_tlv = (tlvbuf_rates *) pcurrent_tlv;
            print_rate(rates_tlv);
            break;
        case MRVL_TX_POWER_TLV_ID:
            txpower_tlv = (tlvbuf_tx_power *) pcurrent_tlv;
            dprintf("Tx power = %d dBm\n", txpower_tlv->tx_power_dbm);
            break;
        case MRVL_BCAST_SSID_CTL_TLV_ID:
            bcast_tlv = (tlvbuf_bcast_ssid_ctl *) pcurrent_tlv;
            dprintf("SSID broadcast = %s\n",
                   (bcast_tlv->bcast_ssid_ctl == 1) ? "enabled" : "disabled");
            break;
        case MRVL_PREAMBLE_CTL_TLV_ID:
            preamble_tlv = (tlvbuf_preamble_ctl *) pcurrent_tlv;
            dprintf("Preamble type = %s\n", (preamble_tlv->preamble_type == 0) ?
                   "auto" : ((preamble_tlv->preamble_type == 1) ? "short" :
                             "long"));
            break;
        case MRVL_BSS_STATUS_TLV_ID:
            bss_status_tlv = (tlvbuf_bss_status *) pcurrent_tlv;
            dprintf("BSS status = %s\n",
                   (bss_status_tlv->bss_status == 0) ? "stopped" : "started");
            break;
        case MRVL_ANTENNA_CTL_TLV_ID:
            antenna_tlv = (tlvbuf_antenna_ctl *) pcurrent_tlv;
            dprintf("%s antenna = %s\n", (antenna_tlv->which_antenna == 0) ?
                   "Rx" : "Tx", (antenna_tlv->antenna_mode == 0) ? "A" : "B");
            break;
        case MRVL_RTS_THRESHOLD_TLV_ID:
            rts_tlv = (tlvbuf_rts_threshold *) pcurrent_tlv;
            tlv_val_16 = *(t_u8 *) & rts_tlv->rts_threshold;
            tlv_val_16 |= (*((t_u8 *) & rts_tlv->rts_threshold + 1) << 8);
            dprintf("RTS threshold = %d\n", tlv_val_16);
            break;
        case MRVL_FRAG_THRESHOLD_TLV_ID:
            frag_tlv = (tlvbuf_frag_threshold *) pcurrent_tlv;
            tlv_val_16 = *(t_u8 *) & frag_tlv->frag_threshold;
            tlv_val_16 |= (*((t_u8 *) & frag_tlv->frag_threshold + 1) << 8);
            dprintf("Fragmentation threshold = %d\n", tlv_val_16);
            break;
        case MRVL_TX_DATA_RATE_TLV_ID:
            txrate_tlv = (tlvbuf_tx_data_rate *) pcurrent_tlv;
            tlv_val_16 = *(t_u8 *) & txrate_tlv->tx_data_rate;
            tlv_val_16 |= (*((t_u8 *) & txrate_tlv->tx_data_rate + 1) << 8);
            if (txrate_tlv->tx_data_rate == 0){
                dprintf("Tx data rate = auto\n");
            }else{
                dprintf("Tx data rate = 0x%x\n", tlv_val_16);}
            break;
        case MRVL_MCBC_DATA_RATE_TLV_ID:
            mcbcrate_tlv = (tlvbuf_mcbc_data_rate *) pcurrent_tlv;
            tlv_val_16 = *(t_u8 *) & mcbcrate_tlv->mcbc_datarate;
            tlv_val_16 |= (*((t_u8 *) & mcbcrate_tlv->mcbc_datarate + 1) << 8);
            if (mcbcrate_tlv->mcbc_datarate == 0){
                dprintf("MCBC data rate = auto\n");
            }else{
                dprintf("MCBC data rate = 0x%x\n", tlv_val_16);}
            break;
        case MRVL_PKT_FWD_CTL_TLV_ID:
            pkt_fwd_tlv = (tlvbuf_pkt_fwd_ctl *) pcurrent_tlv;
            dprintf("Firmware = %s\n", (pkt_fwd_tlv->pkt_fwd_ctl == 0) ?
                   "forwards all packets to the host" :
                   "handles intra-BSS packets");
            break;
        case MRVL_STA_AGEOUT_TIMER_TLV_ID:
            ageout_tlv = (tlvbuf_sta_ageout_timer *) pcurrent_tlv;
            tlv_val_32 = *(t_u8 *) & ageout_tlv->sta_ageout_timer_ms;
            tlv_val_32 |=
                (*((t_u8 *) & ageout_tlv->sta_ageout_timer_ms + 1) << 8);
            tlv_val_32 |=
                (*((t_u8 *) & ageout_tlv->sta_ageout_timer_ms + 2) << 16);
            tlv_val_32 |=
                (*((t_u8 *) & ageout_tlv->sta_ageout_timer_ms + 3) << 24);
            dprintf("STA ageout timer = %d\n", (int) tlv_val_32);
            break;
        case MRVL_PS_STA_AGEOUT_TIMER_TLV_ID:
            ps_ageout_tlv = (tlvbuf_ps_sta_ageout_timer *) pcurrent_tlv;
            tlv_val_32 = *(t_u8 *) & ps_ageout_tlv->ps_sta_ageout_timer_ms;
            tlv_val_32 |=
                (*((t_u8 *) & ps_ageout_tlv->ps_sta_ageout_timer_ms + 1) << 8);
            tlv_val_32 |=
                (*((t_u8 *) & ps_ageout_tlv->ps_sta_ageout_timer_ms + 2) << 16);
            tlv_val_32 |=
                (*((t_u8 *) & ps_ageout_tlv->ps_sta_ageout_timer_ms + 3) << 24);
            dprintf("PS STA ageout timer = %d\n", (int) tlv_val_32);
            break;
        case MRVL_AUTH_TLV_ID:
            auth_tlv = (tlvbuf_auth_mode *) pcurrent_tlv;
            print_auth(auth_tlv);
            break;
        case MRVL_PROTOCOL_TLV_ID:
            proto_tlv = (tlvbuf_protocol *) pcurrent_tlv;
            tlv_val_16 = *(t_u8 *) & proto_tlv->protocol;
            tlv_val_16 |= (*((t_u8 *) & proto_tlv->protocol + 1) << 8);
            print_protocol(tlv_val_16);
            break;
        case MRVL_AKMP_TLV_ID:
            akmp_tlv = (tlvbuf_akmp *) pcurrent_tlv;
            tlv_val_16 = *(t_u8 *) & akmp_tlv->key_mgmt;
            tlv_val_16 |= (*((t_u8 *) & akmp_tlv->key_mgmt + 1) << 8);
            if (tlv_val_16 == KEY_MGMT_PSK) {
                dprintf("KeyMgmt = PSK\n");
                tlv_val_16 = *(t_u8 *) & akmp_tlv->key_mgmt_operation;
                if (tlv_len > sizeof(t_u16)) {
                    tlv_val_16 |=
                        (*((t_u8 *) & akmp_tlv->key_mgmt_operation + 1) << 8);
                    dprintf("Key Exchange on : %s.\n",
                           (tlv_val_16 & 0x01) ? "Host" : "Device");
                    dprintf("1x Authentication on : %s.\n",
                           (tlv_val_16 & 0x10) ? "Host" : "Device");
                }
            } else {
                dprintf("KeyMgmt = NONE\n");
            }
            break;
        case MRVL_CIPHER_TLV_ID:
            cipher_tlv = (tlvbuf_cipher *) pcurrent_tlv;
            print_cipher(cipher_tlv);
            break;
        case MRVL_CIPHER_PWK_TLV_ID:
            pwk_cipher_tlv = (tlvbuf_pwk_cipher *) pcurrent_tlv;
            pwk_cipher_tlv->protocol =
                uap_le16_to_cpu(pwk_cipher_tlv->protocol);
            print_pwk_cipher(pwk_cipher_tlv);
            break;
        case MRVL_CIPHER_GWK_TLV_ID:
            gwk_cipher_tlv = (tlvbuf_gwk_cipher *) pcurrent_tlv;
            print_gwk_cipher(gwk_cipher_tlv);
            break;
        case MRVL_GRP_REKEY_TIME_TLV_ID:
            rekey_tlv = (tlvbuf_group_rekey_timer *) pcurrent_tlv;
            tlv_val_32 = *(t_u8 *) & rekey_tlv->group_rekey_time_sec;
            tlv_val_32 |=
                (*((t_u8 *) & rekey_tlv->group_rekey_time_sec + 1) << 8);
            tlv_val_32 |=
                (*((t_u8 *) & rekey_tlv->group_rekey_time_sec + 2) << 16);
            tlv_val_32 |=
                (*((t_u8 *) & rekey_tlv->group_rekey_time_sec + 3) << 24);
            if (tlv_val_32 == 0){
                dprintf("Group re-key time = disabled\n");
            }else{
                dprintf("Group re-key time = %ld second\n", tlv_val_32);}
            break;
        case MRVL_WPA_PASSPHRASE_TLV_ID:
            psk_tlv = (tlvbuf_wpa_passphrase *) pcurrent_tlv;
            if (tlv_len > 0) {
                dprintf("WPA passphrase = ");
                for (i = 0; i < tlv_len; i++)
                    printf("%c", psk_tlv->passphrase[i]);
                dprintf("\n");
            } else
                dprintf("WPA passphrase = None\n");
            break;
        case MRVL_WEP_KEY_TLV_ID:
            wep_tlv = (tlvbuf_wep_key *) pcurrent_tlv;
            print_wep_key(wep_tlv);
            break;
        case MRVL_STA_MAC_ADDR_FILTER_TLV_ID:
            filter_tlv = (tlvbuf_sta_mac_addr_filter *) pcurrent_tlv;
            print_mac_filter(filter_tlv);
            break;
        case MRVL_MAX_STA_CNT_TLV_ID:
            max_sta_tlv = (tlvbuf_max_sta_num *) pcurrent_tlv;
            tlv_val_16 = *(t_u8 *) & max_sta_tlv->max_sta_num_configured;
            tlv_val_16 |=
                (*((t_u8 *) & max_sta_tlv->max_sta_num_configured + 1) << 8);
            dprintf("Max Station Number configured = %d\n", tlv_val_16);
            if (max_sta_tlv->length == 4) {
                tlv_val_16 = *(t_u8 *) & max_sta_tlv->max_sta_num_supported;
                tlv_val_16 |=
                    (*((t_u8 *) & max_sta_tlv->max_sta_num_supported + 1) << 8);
                dprintf("Max Station Number supported = %d\n", tlv_val_16);
            }
            break;
        case MRVL_RETRY_LIMIT_TLV_ID:
            retry_limit_tlv = (tlvbuf_retry_limit *) pcurrent_tlv;
            dprintf("Retry Limit = %d\n", retry_limit_tlv->retry_limit);
            break;
        case MRVL_EAPOL_PWK_HSK_TIMEOUT_TLV_ID:
            pwk_timeout_tlv = (tlvbuf_eapol_pwk_hsk_timeout *) pcurrent_tlv;
            pwk_timeout_tlv->pairwise_update_timeout =
                uap_le32_to_cpu(pwk_timeout_tlv->pairwise_update_timeout);
            dprintf("Pairwise handshake timeout = %ld\n",
                   pwk_timeout_tlv->pairwise_update_timeout);
            break;
        case MRVL_EAPOL_PWK_HSK_RETRIES_TLV_ID:
            pwk_retries_tlv = (tlvbuf_eapol_pwk_hsk_retries *) pcurrent_tlv;
            pwk_retries_tlv->pwk_retries =
                uap_le32_to_cpu(pwk_retries_tlv->pwk_retries);
            dprintf("Pairwise handshake retries = %ld\n",
                   pwk_retries_tlv->pwk_retries);
            break;
        case MRVL_EAPOL_GWK_HSK_TIMEOUT_TLV_ID:
            gwk_timeout_tlv = (tlvbuf_eapol_gwk_hsk_timeout *) pcurrent_tlv;
            gwk_timeout_tlv->groupwise_update_timeout =
                uap_le32_to_cpu(gwk_timeout_tlv->groupwise_update_timeout);
            dprintf("Groupwise handshake timeout = %ld\n",
                   gwk_timeout_tlv->groupwise_update_timeout);
            break;
        case MRVL_EAPOL_GWK_HSK_RETRIES_TLV_ID:
            gwk_retries_tlv = (tlvbuf_eapol_gwk_hsk_retries *) pcurrent_tlv;
            gwk_retries_tlv->gwk_retries =
                uap_le32_to_cpu(gwk_retries_tlv->gwk_retries);
            dprintf("Groupwise handshake retries = %ld\n",
                   gwk_retries_tlv->gwk_retries);
            break;
        case MRVL_MGMT_IE_LIST_TLV_ID:
            custom_ie_tlv = (tlvbuf_custom_ie *) pcurrent_tlv;
            custom_ie_len = tlv_len;
            custom_ie_ptr = (custom_ie *) (custom_ie_tlv->ie_data);
            while (custom_ie_len >= sizeof(custom_ie)) {
                custom_ie_ptr->ie_index =
                    uap_le16_to_cpu(custom_ie_ptr->ie_index);
                custom_ie_ptr->ie_length =
                    uap_le16_to_cpu(custom_ie_ptr->ie_length);
                custom_ie_ptr->mgmt_subtype_mask =
                    uap_le16_to_cpu(custom_ie_ptr->mgmt_subtype_mask);
                dprintf("Index [%d]\n", custom_ie_ptr->ie_index);
                if (custom_ie_ptr->ie_length){
                    dprintf("Management Subtype Mask = 0x%02x\n",
                           custom_ie_ptr->mgmt_subtype_mask == 0 ?
                           UAP_CUSTOM_IE_AUTO_MASK :
                           custom_ie_ptr->mgmt_subtype_mask);
                }else{
                    dprintf("Management Subtype Mask = 0x%02x\n",
                           custom_ie_ptr->mgmt_subtype_mask);}
                hexdump_data("IE Buffer", (void *) custom_ie_ptr->ie_buffer,
                             (custom_ie_ptr->ie_length), ' ');
                custom_ie_len -= sizeof(custom_ie) + custom_ie_ptr->ie_length;
                custom_ie_ptr =
                    (custom_ie *) ((t_u8 *) custom_ie_ptr + sizeof(custom_ie) +
                                   custom_ie_ptr->ie_length);
            }
            break;

        case HT_CAPABILITY_TLV_ID:
            dprintf("\nHT Capability Info: \n");
            ht_cap_tlv = (tlvbuf_htcap_t *) pcurrent_tlv;
            if (!ht_cap_tlv->ht_cap.supported_mcs_set[0]) {
                dprintf("802.11n is disabled\n");
            } else {
                dprintf("802.11n is enabled\n");
                dprintf("ht_cap_info=0x%x, ampdu_param=0x%x tx_bf_cap=%#lx\n",
                       uap_le16_to_cpu(ht_cap_tlv->ht_cap.ht_cap_info),
                       ht_cap_tlv->ht_cap.ampdu_param,
                       ht_cap_tlv->ht_cap.tx_bf_cap);
            }
            break;
        case HT_INFO_TLV_ID:
            ht_info_tlv = (tlvbuf_htinfo_t *) pcurrent_tlv;
            if (ht_info_tlv->length) {
                dprintf("\nHT Information Element: \n");
                dprintf("Primary channel = %d\n", ht_info_tlv->ht_info.pri_chan);
                dprintf("Secondary channel offset = %d\n",
                       (int) GET_SECONDARY_CHAN(ht_info_tlv->ht_info.field2));
                dprintf("STA channel width = %dMHz\n",
                       IS_CHANNEL_WIDTH_40(ht_info_tlv->ht_info.
                                           field2) ? 40 : 20);
                dprintf("RIFS  %s\n",
                       IS_RIFS_ALLOWED(ht_info_tlv->ht_info.
                                       field2) ? "Allowed" : "Prohibited");
                ht_info_tlv->ht_info.field3 =
                    uap_le16_to_cpu(ht_info_tlv->ht_info.field3);
                ht_info_tlv->ht_info.field4 =
                    uap_le16_to_cpu(ht_info_tlv->ht_info.field4);
                dprintf("HT Protection = %d\n",
                       (int) GET_HT_PROTECTION(ht_info_tlv->ht_info.field3));
                dprintf("Non-Greenfield HT STAs present: %s\n",
                       NONGF_STA_PRESENT(ht_info_tlv->ht_info.
                                         field3) ? "Yes" : "No");
                dprintf("OBSS Non-HT STAs present: %s\n",
                       OBSS_NONHT_STA_PRESENT(ht_info_tlv->ht_info.
                                              field3) ? "Yes" : "No");
                for (i = 0; i < MCS_SET_LEN; i++) {
                    if (ht_info_tlv->ht_info.basic_mcs_set[i]) {
                        dprintf("Basic_mcs_set: \n");
                        flag = 1;
                        break;
                    }
                }
                if (flag) {
                    for (i = 0; i < MCS_SET_LEN; i++)
                        dprintf("%x ", ht_info_tlv->ht_info.basic_mcs_set[i]);
                    dprintf("\n");
                }
            }
            break;
        case MRVL_2040_BSS_COEX_CONTROL_TLV_ID:
            coex_2040_tlv = (tlvbuf_2040_coex *) pcurrent_tlv;
            dprintf("20/40 coex = %s\n",
                   (coex_2040_tlv->enable) ? "enabled" : "disabled");
            break;
        case VENDOR_SPECIFIC_IE_TLV_ID:
            wmm_para_tlv = (tlvbuf_wmm_para_t *) pcurrent_tlv;
            dprintf("wmm parameters:\n");
            dprintf("\tqos_info = 0x%x\n", wmm_para_tlv->wmm_para.qos_info);
            dprintf("\tBE: AIFSN=%d, CW_MAX=%d CW_MIN=%d, TXOP=%d\n",
                   wmm_para_tlv->wmm_para.ac_params[AC_BE].aci_aifsn.aifsn,
                   wmm_para_tlv->wmm_para.ac_params[AC_BE].ecw.ecw_max,
                   wmm_para_tlv->wmm_para.ac_params[AC_BE].ecw.ecw_min,
                   uap_le16_to_cpu(wmm_para_tlv->wmm_para.ac_params[AC_BE].
                                   tx_op_limit));
            dprintf("\tBK: AIFSN=%d, CW_MAX=%d CW_MIN=%d, TXOP=%d\n",
                   wmm_para_tlv->wmm_para.ac_params[AC_BK].aci_aifsn.aifsn,
                   wmm_para_tlv->wmm_para.ac_params[AC_BK].ecw.ecw_max,
                   wmm_para_tlv->wmm_para.ac_params[AC_BK].ecw.ecw_min,
                   uap_le16_to_cpu(wmm_para_tlv->wmm_para.ac_params[AC_BK].
                                   tx_op_limit));
            dprintf("\tVI: AIFSN=%d, CW_MAX=%d CW_MIN=%d, TXOP=%d\n",
                   wmm_para_tlv->wmm_para.ac_params[AC_VI].aci_aifsn.aifsn,
                   wmm_para_tlv->wmm_para.ac_params[AC_VI].ecw.ecw_max,
                   wmm_para_tlv->wmm_para.ac_params[AC_VI].ecw.ecw_min,
                   uap_le16_to_cpu(wmm_para_tlv->wmm_para.ac_params[AC_VI].
                                   tx_op_limit));
            dprintf("\tVO: AIFSN=%d, CW_MAX=%d CW_MIN=%d, TXOP=%d\n",
                   wmm_para_tlv->wmm_para.ac_params[AC_VO].aci_aifsn.aifsn,
                   wmm_para_tlv->wmm_para.ac_params[AC_VO].ecw.ecw_max,
                   wmm_para_tlv->wmm_para.ac_params[AC_VO].ecw.ecw_min,
                   uap_le16_to_cpu(wmm_para_tlv->wmm_para.ac_params[AC_VO].
                                   tx_op_limit));
            break;
        default:
            break;
        }
        tlv_buf_left -= (sizeof(tlvbuf_header) + tlv_len);
        pcurrent_tlv = (tlvbuf_header *) (pcurrent_tlv->data + tlv_len);
    }
    return;
}

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
        dprintf("ERR:Cannot allocate buffer for command!\n");
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
            dprintf("ERR:Corrupted response! cmd_code=%x, Tlv->tag=%x\n",
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
            dprintf("ERR:Could not get HT capability!\n");
        }
    } else {
        dprintf("ERR:Command sending failed!\n");
        ret = UAP_FAILURE;
    }
    if (buffer)
        ewl_os_free(buffer);
    return ret;
}

/**
 *  @brief Creates a sys_cfg request for 11n parameters
 *   and sends to the driver
 *
 *  @param enable   0 - disable 802.11n in uap
                    1 - enable 802.11n in uap
 *  @param ht_cap   HT Capabilites info(default value is 0x110c)
 *  @return         UAP_SUCCESS/UAP_FAILURE
 */
int
ewlap_sys_cfg_11n(int enable, u16 ht_cap)
{
    apcmdbuf_sys_configure *cmd_buf = NULL;
    tlvbuf_htcap_t *tlv = NULL;
//    tlvbuf_htinfo_t *ht_info_tlv = NULL;
    HTCap_t htcap;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    t_u16 buf_len = MRVDRV_SIZE_OF_CMD_BUFFER;
    int ret = UAP_SUCCESS;

    ewl_os_memset(&htcap, 0, sizeof(htcap));
    if (UAP_FAILURE == get_sys_cfg_11n(&htcap)) {
        dprintf("Fail to get 11n parameters from firmware\n");
        return UAP_FAILURE;
    }

    /* Initialize the command buffer */
    buffer = (t_u8 *) ewl_os_malloc(buf_len);

    if (!buffer) {
        dprintf("ERR:Cannot allocate buffer for command!\n");
        return UAP_FAILURE;
    }
    ewl_os_memset(buffer, 0, buf_len);

    /* Locate headers */
    cmd_buf = (apcmdbuf_sys_configure *) buffer;
    tlv = (tlvbuf_htcap_t *) (buffer + sizeof(apcmdbuf_sys_configure));
    tlv->tag = HT_CAPABILITY_TLV_ID;

    /* Initialize the command length */
    cmd_len = sizeof(apcmdbuf_sys_configure) + sizeof(tlvbuf_htcap_t);
    cmd_buf->action = ACTION_SET;

    /* Fill the command buffer */
    cmd_buf->cmd_code = APCMD_SYS_CONFIGURE;
    cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
    cmd_buf->seq_num = 0;
    cmd_buf->result = 0;  

    tlv->length = sizeof(HTCap_t);

    if(!enable)
    {
        ewl_os_memcpy(&tlv->ht_cap, &htcap, sizeof(HTCap_t));
        /* disable mcs rate */
        tlv->ht_cap.supported_mcs_set[0] = 0;
        tlv->ht_cap.supported_mcs_set[4] = 0;
    }
    else
    {
        /* enable 802.11n */
        ewl_os_memcpy(&tlv->ht_cap, &htcap, sizeof(HTCap_t));
        /* enable mcs rate */
        tlv->ht_cap.supported_mcs_set[0] = DEFAULT_MCS_SET_0;
        tlv->ht_cap.supported_mcs_set[4] = DEFAULT_MCS_SET_4;
        tlv->ht_cap.ht_cap_info = DEFAULT_HT_CAP_VALUE & ~HT_CAP_CONFIG_MASK;
        tlv->ht_cap.ht_cap_info |= ht_cap & HT_CAP_CONFIG_MASK;
        tlv->ht_cap.ht_cap_info = uap_cpu_to_le16(tlv->ht_cap.ht_cap_info);
    }
    endian_convert_tlv_header_out(tlv);
    /* Send the command */
    ret = uap_ioctl((t_u8 *) cmd_buf, &cmd_len, buf_len);
    /* Process response */
   if (ret == UAP_SUCCESS) {
       /* Verify response */
       if (cmd_buf->cmd_code != (APCMD_SYS_CONFIGURE | APCMD_RESP_CHECK)) {
           dprintf("ERR:Corrupted response! cmd_code=%x\n", cmd_buf->cmd_code);
           ewl_os_free(buffer);
           return UAP_FAILURE;
       }
       dprintf("cmd_buf result %d \n", cmd_buf->result);
       /* Print response */
       //if (cmd_buf->result == CMD_SUCCESS) 
       //{

        print_tlv((t_u8 *) tlv, cmd_buf->size -
                         sizeof(apcmdbuf_sys_configure) + BUF_HEADER_SIZE);
      // } else {
      //         printf("ERR:Could not set 802.11n parameters!\n");
      //     ret = UAP_FAILURE;
       //}
   } else {
       dprintf("ERR:Command sending failed!\n");
   }
   if (buffer)
       ewl_os_free(buffer);
   return ret;

}
