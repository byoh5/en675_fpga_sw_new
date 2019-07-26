#ifndef __WLIF_TOOLS__
#define __WLIF_TOOLS__

#define ETH_MAC_ALEN 6

#define WIFI_UAP_SET_CMD_IP      "IP"
#define WIFI_UAP_SET_CMD_NETMASK "NETMASK"
#define WIFI_UAP_SET_CMD_SSID    "SSID"
#define WIFI_UAP_SET_CMD_KEY     "KEY"
#define WIFI_UAP_SET_CMD_CHANNEL "CHANNEL"
#define WIFI_UAP_SET_CMD_MAX_SCB "MAX_SCB"
#define WIFI_UAP_SET_CMD_SEC     "SEC"
#define WIFI_UAP_SET_CMD_SET     "SET"
#define WIFI_UAP_SET_CMD_BSSON		"BSSON"
#define WIFI_UAP_SET_CMD_BSSOFF		"BSSOFF"
#define WIFI_UAP_SET_CMD_BSSRESET	"BSSRESET"

#define WIFI_STA_SET_CMD_IP      "IP"
#define WIFI_STA_SET_CMD_NETMASK "NETMASK"
#define WIFI_STA_SET_CMD_GATEWAY "GATEWAY"
//#define WIFI_STA_SET_CMD_SSID    "SSID"
//#define WIFI_STA_SET_CMD_KEY     "KEY"
#define WIFI_STA_SET_CMD_LINKUP   "LINKUP"
#define WIFI_STA_SET_CMD_LINKDOWN "LINKDOWN"
#define WIFI_STA_SET_CMD_NETUP   "NETUP"
#define WIFI_STA_SET_CMD_NETDOWN "NETDOWN"

#define WIFI_SET_CMD_IGMPJOIN "IGMPJOIN"
#define WIFI_SET_CMD_IGMPLEAVE "IGMPLEAVE"

typedef struct {
	char ssid[32];
	unsigned char bssid[6];
	int freq;
	int level;
	char flags[64];	/* NULL terminated string */
} wl_scans_t ;

typedef struct  {
	int num;
	wl_scans_t wl_scans[1];
} wlif_scan_t;

typedef struct {
	int flags;
	char ssid[33];
	int enc_proto; 		/* 0 : open, 1: wpa2-psk */          
	char passphrase[65]; /* passphrase for wpa2-psk */
	int channel;
	int max_sta_count;  /* max station count */  
} wlif_ewlap_cfg;

void cpu2cop_wlif_scan(void *arg);
#endif //__WIFI_TOOLS__
