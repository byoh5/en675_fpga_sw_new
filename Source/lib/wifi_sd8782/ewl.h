#ifndef __EUX_WIFI_H__
#define __EUX_WIFI_H__

#include "dev.h"
//#include <wireless.h>
#include <lwip/pbuf.h>
#include <lwip/netif.h>

//#define  MLAN_HEADER_SIZE				166

#define BSS_TYPE_STA                    0x0
#define BSS_TYPE_UAP                    0x1

#define ETH_ALEN                        6
#define ETH_FRAME_LEN                   1514      /* Max. octets in frame sans FCS */

#ifndef BIT
#define BIT(n) (1 << (n))
#endif

/**-----------------------------------------------------------------------------
    supplicant defines & macros
               api function lists
**-----------------------------------------------------------------------------*/

#define SSID_MAX_WPA_IE_LEN 40

#define WPA_CAPABILITY_PREAUTH BIT(0)
#define WPA_CAPABILITY_MGMT_FRAME_PROTECTION BIT(6)
#define WPA_CAPABILITY_PEERKEY_ENABLED BIT(9)

#define WPA_CIPHER_NONE BIT(0)
#define WPA_CIPHER_WEP40 BIT(1)
#define WPA_CIPHER_WEP104 BIT(2)
#define WPA_CIPHER_TKIP BIT(3)
#define WPA_CIPHER_CCMP BIT(4)
#ifdef CONFIG_IEEE80211W
#define WPA_CIPHER_AES_128_CMAC BIT(5)
#endif /* CONFIG_IEEE80211W */

#define WPA_KEY_MGMT_IEEE8021X BIT(0)
#define WPA_KEY_MGMT_PSK BIT(1)
#define WPA_KEY_MGMT_NONE BIT(2)
#define WPA_KEY_MGMT_IEEE8021X_NO_WPA BIT(3)
#define WPA_KEY_MGMT_WPA_NONE BIT(4)

#define WPA_PROTO_WPA BIT(0)
#define WPA_PROTO_RSN BIT(1)

#define WPA_AUTH_ALG_OPEN BIT(0)
#define WPA_AUTH_ALG_SHARED BIT(1)
#define WPA_AUTH_ALG_LEAP BIT(2)

#define IEEE80211_CAP_ESS	0x0001
#define IEEE80211_CAP_IBSS	0x0002
#define IEEE80211_CAP_PRIVACY	0x0010

#define MAX_SSID_LEN 32

/* API Error codes */
enum {
    EWL_FAILURE = -1,
    EWL_SUCCESS = 0,
};

//STA DISCONNECT ISSUE
enum
{
    NO_AP,
    WPA_AUTH_FAIL,
};

struct wpa_ie_data {
	int proto;
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
	int capabilities;
	int num_pmkid;
	const u8 *pmkid;
	int mgmt_group_cipher;
};


/**
 * struct wpa_scan_result - Scan results
 * @bssid: BSSID
 * @ssid: SSID
 * @ssid_len: length of the ssid
 * @wpa_ie: WPA IE
 * @wpa_ie_len: length of the wpa_ie
 * @rsn_ie: RSN IE
 * @rsn_ie_len: length of the RSN IE
 * @freq: frequency of the channel in MHz (e.g., 2412 = channel 1)
 * @caps: capability information field in host byte order
 * @qual: signal quality
 * @noise: noise level
 * @level: signal level
 * @maxrate: maximum supported rate
 *
 * This structure is used as a generic format for scan results from the
 * driver. Each driver interface implementation is responsible for converting
 * the driver or OS specific scan results into this format.
 */
struct wpa_scan_result {
	u8 bssid[ETH_ALEN];
	u8 ssid[32];
	size_t ssid_len;
	u8 wpa_ie[SSID_MAX_WPA_IE_LEN];
	size_t wpa_ie_len;
	u8 rsn_ie[SSID_MAX_WPA_IE_LEN];
	size_t rsn_ie_len;
	int freq;
	u16 caps;
	int qual;
	int noise;
	int level;
	int maxrate;
};

#define PMK_LEN 32

/**
 * struct wpa_ssid - Network configuration data
 *
 * This structure includes all the configuration variables for a network. This
 * data is included in the per-interface configuration data as an element of
 * the network list, struct wpa_config::ssid. Each network block in the
 * configuration is mapped to a struct wpa_ssid instance.

 *  @next - Next network in global list
 *  @id  Unique id for the network
 *  @priority - Priority group
 *  @ssid - Service set identifier (network name)
 *  @ssid_len - Length of the SSID
 *  @bssid_set - Whether BSSID is configured for this network
 *  @psk - WPA pre-shared key (256 bits)
 *  @psk_set - Whether PSK field is configured
 *  @passphrase - WPA ASCII passphrase
 *  @pairwise_cipher - Bitfield of allowed pairwise ciphers, WPA_CIPHER_*
 *  @group_cipher - Bitfield of allowed group ciphers, WPA_CIPHER_*
 *  @bssid - BSSID
 *  @key_mgmt - Bitfield of allowed key management protocols
 *  @proto - Bitfield of allowed protocols, WPA_PROTO_*
 *  @auth_alg -  Bitfield of allowed authentication algorithms
 *  @scan_ssid - Scan this SSID with Probe Requests
 *  @wep_tx_keyidx - Default key index for TX frames using WEP
 *  @mode - IEEE 802.11 operation mode (Infrastucture/IBSS)
 *      0 = infrastructure (Managed) mode, i.e., associate with an AP.
 *	    1 = IBSS (ad-hoc, peer-to-peer) : we don't support
 *	@disabled - Whether this network is currently disabled
 *	    0 = this network can be used (default).
 *	    1 = this network block is disabled (can be enabled through
 */
struct wpa_ssid {
	struct wpa_ssid *next;
	struct wpa_ssid *pnext;
	int id;
	int priority;
	u8 *ssid;
	size_t ssid_len;
	u8 bssid[ETH_ALEN];
	int bssid_set;
	u8 psk[PMK_LEN]; /*we don't support that passphrase convert into psk*/
	int psk_set;
	char *passphrase;
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
	int proto;
	int auth_alg;
	int scan_ssid;

#define NUM_WEP_KEYS 4
#define MAX_WEP_KEY_LEN 16
	u8 wep_key[NUM_WEP_KEYS][MAX_WEP_KEY_LEN];
	size_t wep_key_len[NUM_WEP_KEYS];
	int wep_tx_keyidx;
	int mode;
	int disabled;
};


/**
    This function initializes the SDIO driver & WiFi.

    @return 0 --success, otherwise fail
 */
 
int 
ewl_init(void);


int 
ewl_parse_wpa_ie(const u8 *wpa_ie, size_t wpa_ie_len, struct wpa_ie_data *data);

void
ewl_scan(void (*scan_cb)(void));

int 
ewl_get_scan_results(struct wpa_scan_result **results);

void 
ewl_disconnect(void);

int
ewl_add_netowrk(void);

int
ewl_set_network(int id, char *name, char *value);

struct wpa_ssid * 
ewl_get_network_lists(void);

struct wpa_ssid *
ewl_get_current_network_list(void);

int 
ewl_select_network(int id);

int 
ewl_enable_network(int id);

int 
ewl_remove_network(int id);

int
ewl_reassociate_network(void);

void 
ewl_supplicant_run(void);

int
ewl_supplicant_init(void);

/**
    called when disconnect occur.
    This situation occur when wpax password is incorrect.
    
    @param disconnect_cb : callback function.
*/
void 
ewl_set_disconnect_cb(void (*disconnect_cb)(int));

/**----------------------------------------------------------------------------
 *WiFi driver library
**-----------------------------------------------------------------------------*/
/*! \brief Initialization complete callback function.
 *
 * Invoked when WiFi initialization is complete.
 *
 * @param ctx Opaque context pointer as provided to \a wl_init() that will be
 *            passed back to the callback.
 */
typedef void   (*ewl_init_netif_cb_t)(void* ctx);

                
void *
ewl_get_sta_handle(void);

void 
ewl_poll(void);


void* 
ewl_add_netif(u8 bss_type);

void 
ewl_get_hwaddr(struct netif *netif, u8* pmac);

void
ewl_set_conn_cb(void *handle,
                void (*conn_cb)(BYTE *buf, int buf_len),void (*disconn_cb)(BYTE *buf, int buf_len));

void 
ewl_sdio_ext_irq_handler(void *dummy);


void *
ewl_pkt_rx(struct netif *netif);

int 
ewl_start_tx(struct pbuf* p, struct netif *netif);

/** 
 *  @ Send multicast address to WiFi
 *   
 *  @param priv     A pointer to netif structure
 *  @multicast_mac  A pointer to multicast mac address that size is 6                  
 *
 *  @return       None
 */
void
ewl_set_multicast_addr(struct netif *netif, u8 *multicast_mac);



/**
	S/W AP commands 
*/

/**
	WiFi AP concise configuration Information. 
*/

struct ewlap_cfg
{
    char ssid[33];
    int enc_proto; 		/* 0 : open, 1: wpa2-psk */          
    char passphrase[65]; /* passphrase for wpa2-psk */
    int channel;
    int max_sta_count;  /* max station count */  
};

/** station info */
typedef struct 
{
    /** STA MAC address */
    u8 mac_address[ETH_ALEN];
    /** Power mfg status */
    u8 power_mfg_status;
    /** RSSI */
    s8 rssi;
} ewl_sta_info;

/** sta_list structure */
typedef struct
{
    /** station count */
    u16 sta_count;
    /** station list */
    ewl_sta_info info[32];
} ewl_sta_list;



int 
ewlap_init_apcfg(char *ssid);

int 
ewlap_bss_start(void);

int 
ewlap_bss_stop(void);

int 
ewlap_bss_reset(void);

/**
    Get current concise AP cfg.

	@cfg : concise ap configuration information 
	       should be allocated.
	@return         0--success, otherwise fail
*/
int 
ewlap_get_apcfg(struct ewlap_cfg *cfg);


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
ewlap_apcfg(char *name, char *value);

/**
    set current AP config to firware of wifi.
    and if AP is started.then AP is restarted.
    If you change your ap setting, 
    you should use this function with ewlap_apcfg.

    @return         0 --success, otherwise fail
*/
int 
ewlap_set_apcfg(void);

int
ewlap_get_sta_list(ewl_sta_list *plist);

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
ewlap_sys_cfg_11n(int enable, u16 ht_cap);

/**
 *  @brief This function sets the MAC address to firmware.
 *
 *  @param netif  the network interface to change
 *  @param addr   MAC address to set
 *
 *  @return        0 --success, otherwise fail
 */
int
ewl_set_hwaddr(struct netif *netif, void *addr);

int
ewl_get_fwhwaddr(struct netif *netif, void *addr);

/**
    check uap connect state.
    @@return : 1 = connected,
               0 = is not 
*/




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
ewlap_get_cfg_11n(void);

/**
    @parm enable : 0  = 11n disable
                   1  = 11n enable 
*/

int
set_sys_cfg_11n(int enable);

int 
ewlap_is_connected(void);

/***
    @return 1 = connected;
            0 = is not;
*/

int
ewl_sta_is_connected(void);

int
ewl_sta_get_freq(struct netif *netif);

int
ewl_uap_get_freq(void);
#endif
