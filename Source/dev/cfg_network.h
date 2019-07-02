//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:     cfg_network.h
//	Description:   EN675 Network configure
//	Author:        Team 2
//
//	Note:          Here you will check the settings.
//
//	Ver   Date   Author  Changes
//	---  ------  ------  -------
//  0.1  190329  hjlee   support EN675
//------------------------------------------------------------------------------
#ifndef __CFG_NETWORK_H__
#define __CFG_NETWORK_H__

////////////////////////////////////////////////////////////////////////////////
// Device System(REALM)
////////////////////////////////////////////////////////////////////////////////
#define DEVICE_SYSTEM_REALM				"Eyenix Network Camera System"

////////////////////////////////////////////////////////////////////////////////
// Ethernet
////////////////////////////////////////////////////////////////////////////////
#ifdef __ETHERNET__

// IP address
#define	ETH_IPADDR						"192.168.53.195"	// Ip address(rel)
#define	ETH_NETMASK						"255.255.0.0"		// Subnet mask
#define	ETH_GWADDR						"192.168.0.1"		// Gateway

#define ETHPHY_AUTONEG					1			// 0 : No use
													//*1 : Ethernet PHY auto negotiation


// Debug flag
#define ETHPHY_MDIO_TEST				0			//*0 : No use
													// 1 : PHY chip address(MDIO) check mode

#define ETH_SHELL_TEST					1			//*0 : No use
													// 1 : Shell Test Code Enable

#define ETHPHY_SHELL_TEST				1			//*0 : No use
													// 1 : Shell Test Code Enable

#define ETHPHY_LOOPBACK_TEST			1			//*0 : No use
													// 1 : Include phy loopback test code

//#define ETH_EXCESSIVE_PACKETS_CK		1			// 0 : No use
//													//*1 : Blocks too many packets. However, there is a scenario where the system reboots.

#endif

////////////////////////////////////////////////////////////////////////////////
// Wi-Fi
////////////////////////////////////////////////////////////////////////////////
#ifdef __WIFI__
// Wi-Fi Configuration
#define WIFI_DEFAULT					1			//*0 : flash memory load data(gtWificfg)
													// 1 : default load data(board.h setting)

// AP(Access Point) mode
#define WIFI_AP_SSID_MAC				1			// 0 : user Wi-Fi AP SSID => EN673_WIFI_TEST
													//*1 : AP SSID + MAC Address(LSB 24bit) => EN673_xxxxxx

#if WIFI_AP_SSID_MAC
#define WIFI_AP_SSID					"EN673_"	// broadcast to SSID
#else
#define WIFI_AP_SSID					"EN673_WIFI_TEST"	// broadcast to SSID
#endif
#define WIFI_AP_SWITCH					DEF_ON		// on(DEF_ON)? off(DEF_OFF)?
#define WIFI_AP_AUTH					wifi_WPA2_PSK	// (wifi_OPEN) or (wifi_WPA2_PSK)
#define WIFI_AP_KEY						"1234567890"	// KEY
#define WIFI_AP_CH						5			// default channel
#define WIFI_AP_MAXUSER					5			// default max user

//#define custom_MAC_UAP
#ifdef custom_MAC_UAP
#define WIFI_UAP_MACADDRESS				{0x00,0x11,0x22,0x33,0x55,0x35}
#endif

// Station mode
#define WIFI_STA_SWITCH					DEF_OFF		// on(DEF_ON)? off(DEF_OFF)?
#define WIFI_STA_SSID					"eyenix_1005"	// connect to SSID
#define WIFI_STA_AUTH					wifi_WPA2_PSK	// (wifi_OPEN) or (wifi_WPA2_PSK)
#define WIFI_STA_KEY					"eyenix12"	// KEY
//#define custom_MAC_STA
#ifdef custom_MAC_STA
#define WIFI_STA_MACADDRESS				{0x00,0x11,0x22,0x33,0x55,0x34}
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Service
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// DHCP
#define NET_DHCP						ENX_OFF		// DHCP Switch

////////////////////////////////////////////////////////////////////////////////
// DNS
#define	NET_DNS_ADDR0					"8.8.8.8"	// Dns server 0
#define	NET_DNS_ADDR1					"8.8.4.4"	// Dns server 1

////////////////////////////////////////////////////////////////////////////////
// SNTP
#define NET_SNTPC						0			// 0 : SNTPC unuse
													//*1 : SNTPC use

#define NET_SNTP_START					ENX_OFF		// DEF_OFF(0): not work when booting.
													//*DEF_ON(1) : work when booting.

#define	NET_SNTP_SRVR					"0.asia.pool.ntp.org"	// SNTP server address "pool.ntp.org"
#define NET_SNTP_PERIODIC				3600		// SNTP server Periodic Time (15sec ~ )
#define NET_SNTP_RETRY_SEC				10			// SNTP server retry sec
#define NET_SNTP_RETRY_CNT				2			// SNTP server retry max count (3sec ~ )

////////////////////////////////////////////////////////////////////////////////
// RTSP server
#define ENX_RTSP_use					1			// 0 : RTSP server unuse
													//*1 : RTSP server use

#if (ENX_RTSP_use==1)
#define RTSP_info_MAX					(2)			// RTSP User Count

#define H264URL							"h264"		// H264 CH.1
#define HSUBURL 						"hsub"		// H264 CH.2
#define JPEGURL							"jpeg"		// M-JPEG
#define JSUBURL							"jsub"		// M-JPEG(SW)
//#define PLAYBACK						"sdcard"	// playback
//#define PLAYBACKRETRY					"sdcard/video"	// playback

#define RTSPoverHTTP					0			//*0 : RTSP over HTTP unuse
													// 1 : It will be added later...

#define RTSPD_AUTH_NONE					0
#define RTSPD_AUTH_BASIC				1
#define RTSPD_AUTH_DIGEST				2

#define RTSPD_AUTH_MODE					RTSPD_AUTH_NONE	// 0 : RTSP Authorization unuse
													// 1 : RTSP Authorization(Basic)
													// 2 : RTSP Authorization(Digest)

#define RTSPD_RTCP_SR					0			// 0 : not supported RTCP SR
													// 1 : supported RTCP SR
#else
#define RTSP_info_MAX					(0)			// RTSP User Count
#endif

////////////////////////////////////////////////////////////////////////////////
// HTTP
#define NET_LWIPHTTP					1			//*0 : not include HTTP
													// 1 : include HTTP

////////////////////////////////////////////////////////////////////////////////
// Iperf
#define NET_LWIPERF						1			//*0 : not include iPerf
													// 1 : include iPerf(TCP only)

#endif //__CFG_NETWORK_H__
