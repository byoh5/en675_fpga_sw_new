#include "dev.h"

#include <string.h>

#if defined(__WIFI__)
#include "enx_lwip.h"
#include "wifi.h"

void WifiCFG_Default_UAP(tWifiUAPcfg *uap)
{
	uap->un32State = WLIF_NONE;
	uap->un1Switch = WIFI_AP_SWITCH;
	strcpy((char *)uap->strSSID, WIFI_AP_SSID);
	uap->un8Channel = WIFI_AP_CH;
	uap->un32Authentication = WIFI_AP_AUTH;
	strcpy((char *)uap->strKey, WIFI_AP_KEY);
	uap->un8MaxSTA = WIFI_AP_MAXUSER;

	uap->un32Localip = ipaddr_addr("192.169.15.1");
	uap->un32Netmask = ipaddr_addr("255.255.255.0");
	uap->un8DHCPServer = ENX_ON; // DHCP server on
	uap->un32DefaultGateway = ipaddr_addr("192.169.15.1");
	uap->un32StartAddress = ipaddr_addr("192.169.15.2");
	uap->un32EndAddress = ipaddr_addr("192.169.15.254");
	uap->leaseTime = 864000; // 10day
}

void WifiCFG_Default_STA(tWifiSTAcfg *sta)
{
	sta->un32State = WLIF_NONE;
	sta->un1Switch = WIFI_STA_SWITCH;
	sta->un1Connect = sta->un1Switch;
	strcpy((char *)sta->strSSID, WIFI_STA_SSID);
	sta->un32Authentication = WIFI_STA_AUTH;
	strcpy((char *)sta->strKey, WIFI_STA_KEY);



	sta->un1DHCPSwitch = NET_DHCP;	// connect to DHCP server

	if (sta->un1DHCPSwitch == ENX_ON) {
		sta->un32Localip = 0;
		sta->un32Netmask = 0;
		sta->un32Gateway = 0;
		sta->un32DNS0 = 0;
		sta->un32DNS1 = 0;
	} else {
		sta->un32Localip = ipaddr_addr("192.168.0.196");
		sta->un32Netmask = ipaddr_addr("255.255.0.0");
		sta->un32Gateway = ipaddr_addr("192.168.0.1");
	}
}
#endif
