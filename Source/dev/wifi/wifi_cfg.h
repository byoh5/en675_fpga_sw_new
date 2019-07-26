#ifndef _WIFI_CFG_H_
#define _WIFI_CFG_H_

#define WIFI_SSID_MAX			32	// 1~32byte
#define WIFI_KEY_WPAnWPA2_MAX	63	// 8~63byte
#define WIFI_NAME_MAX			32	// 1~32byte

/*
typedef enum {
	wifi_SouthKorea,
	wifi_China,
	wifi_Japan,
	wifi_USAnCanada
}eZone;
*/
typedef enum {
	wifi_OPEN,
	wifi_WPA_PSK,
	wifi_WPA2_PSK,
	wifi_WPAnWPA2_PSK
} eAuthentication;
/*
typedef enum {
	wifi_Off,
	wifi_WEP64,
	wifi_WEP128,
	wifi_TKIP,
	wifi_AES,
	wifi_TKIPnAES
}eEncryption;
*/

typedef struct {
	UINT un32State;						// State				//4(1)
	UINT un1Switch;						// ON/OFF				//4(1)
	char strSSID[WIFI_SSID_MAX+4];		// SSID					//36(32)
	UINT un8Channel;						// Channel(1~13)		//4(1)
	eAuthentication un32Authentication;	// Authentication		//4(4)
	char strKey[WIFI_KEY_WPAnWPA2_MAX+1];	// KEY(size:5,13,63)	//64(63)
	UINT un8MaxSTA;						// Max station count	//4(1)

	UINT un32Localip;				// 						//4(4)
	UINT un32Netmask;				// 						//4(4)

	UINT un8DHCPServer;					//						//4(1)
	UINT un32DefaultGateway;		//						//4(4)
	UINT un32StartAddress;			//						//4(4)
	UINT un32EndAddress;			//						//4(4)
	UINT leaseTime;						//						//4(4)
} tWifiUAPcfg;

typedef struct {
	UINT un32State;						//						//4(1)
	UINT un1Switch;						// ON/OFF				//4(1)
	UINT un1Connect;						// Connect / Disconnect	//4(1)
	char strSSID[WIFI_SSID_MAX+4];		// SSID					//36(32)
	eAuthentication un32Authentication;	// Authentication		//4(4)
	char strKey[WIFI_KEY_WPAnWPA2_MAX+1];	// KEY(size:5,13,63)	//64(63)

	UINT un32Localip;				// 						//4(4)
	UINT un32Netmask;				// 						//4(4)
	UINT un32Gateway;				// 						//4(4)
	UINT un32DNS0;					// 						//4(4)
	UINT un32DNS1;					// 						//4(4)
	UINT un1DHCPSwitch;				// 						//4(1)
} tWifiSTAcfg;

#endif // _WIFI_H_
