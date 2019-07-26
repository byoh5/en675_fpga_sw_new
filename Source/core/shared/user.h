#ifndef __USER_H__
#define __USER_H__

#include "dev.h"
#include "wifi_cfg.h"

typedef struct{
	VideoResolution	eResolution;	//  4Byte
	VideoBitRateMode eBRMode;		//	4Byte
	VideoBitRateIdx eBitRate;		//	4Byte
	H264ProfileMode eProfileMode;	//  4Byte, H.264 only
	UINT nIDRFrame;					//  4Byte, H.265/H.264, max
	WORD nFps;						//	2Byte, max 240?
	WORD nQuality;					//  2Byte, 0 ~ 51, 0 ~ 99
} VideoInfo;						// 24Byte

// Board global information
typedef struct {
	ULONG			*pBootAddr;				// fixed! 0xC0FFF000, ECMb - System boot address
	UINT			u32MagicNum;			// fixed! 0xC0FFF008, ECMb - Magic number
	UINT			u32DateID;				// fixed! 0xC0FFF00C, Compile Datetime ID
	UINT			u32Version;				// fixed! 0xC0FFF010, Bootloader Version
	BYTE			arr8MacAddress[8];		// fixed! 0xC0FFF018, ECMb - Network - Mac - use 6byte / alloc 8byte / 00 00 00 00 00 00 xx xx
	UINT			u32Res;					// fixed! 0xC0FFF01C, Customer Value
	BYTE			arr8SN[32];				// fixed! 0xC0FFF020, Customer Serial Number
} tSystem; // Size: 64byte(0x40)

#ifndef DEF_BOOT
typedef struct {
	UINT			u32MagicNum;			// fixed! Magic Number
	UINT			u32DateID;				// fixed! Compile Datetime ID

	char			strDeviceId[16];		// fixed! System/Network - Device ID
	char			strDeviceName[64];		// fixed! System/Network - Device Name

	UINT			u32SystemTime;			// fixed! System - Date - 기기 처음 킨 시간
	UINT			nTimeSummer;			// fixed! System - Date - use Summer Time
	TimeZone		nTimeZone;				// fixed! System - Date - TimeZone

	VideoInfo		viH265;					//
	VideoInfo		viH264;					//
	VideoInfo		viJPEG;					//








#if (LOAD_FS_SDCARD==1)
	ENX_SWITCH		bSdVidSave;
#else
	ENX_SWITCH		_dummy_bSdVidSave;
#endif











} tUser; // Size: ____byte(0x__)

#if defined(__NETWORK__)
typedef struct {
	UINT			u32IpAddr;				// Network - IP Address
	UINT			u32NetMask;				// Network - Netmask Address
	UINT			u32Gateway;				// Network - Gateway Address
	UINT			u32DnsSvr0;				// Network - DNS Server Address 1
	UINT			u32DnsSvr1;				// Network - DNS Server Address 2
	UINT			u1UseDhcp;				// Network - DHCP enable 0:off, 1:on
} NetifAddress;

typedef struct {
	NetifAddress	naEthernet;
//	NetifAddress	naWifiuap;
//	NetifAddress	naWifista;

#if defined(__WIFI__)
	tWifiUAPcfg		UAPcfg;
	tWifiSTAcfg		STAcfg;
#else
	tWifiUAPcfg		_Dummy_tWifiUAPcfg;
	tWifiSTAcfg		_Dummy_tWifiSTAcfg;
#endif

#if (NET_SNTPC==1)
	UINT			u1UseSntp;				// Network - Date - SNTP enable 0:off, 1:on
	char			strSntpSvr[64];			// Network - Date - SNTP Server Address
	UINT			u32SntpPeriodic;		// Network - Date - SNTP Periodic Time
	UINT			u32SntpRetrySec;		// Network - Date - SNTP Retry Sec
	UINT			u32SntpRetryMaxcnt;		// Network - Date - SNTP Retry Max Count
#else
	UINT			nDummy2[20];
#endif

	WORD			portnumRTSP;

} tNetwork; // Size: ____byte(0x__)
#endif
#endif

typedef enum {
	sflsc_Save,
	sflsc_Load,
	sflsc_Erase,
	sflsc_Err,
} sfls_cmd;

typedef enum {
	sfls_SYSTEM = 1,		//  4KB(- 4KB)
	sfls_USER,				//  4KB(- 8KB)
#if defined(__ISP__)
	sfls_ISP_SHD,			//  4KB(-40KB)
	sfls_ISP_MENU,			//  4KB(-36KB)
	sfls_ISP_MENU_BAK,		//  4KB(-32KB)
	sfls_ISP_USER,			//  4KB(-28KB)
	sfls_ISP_DATA,			//  4KB(-24KB)
	sfls_ISP_BLK0,			//  4KB(-20KB)
	sfls_ISP_BLK1,			//  4KB(-16KB)
	sfls_ISP_BLK2,			//  4KB(-12KB)
#endif
#if defined(__NETWORK__)
	sfls_NETWORK,			//	4KB(-44KB)
#if defined(__ONVIF__)
	sfls_ONVIF,				// 12KB(-56KB)
#endif
#endif
} sfls_fixed_area;

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function


//-------------------------------------------------------------------------------------------------
// Variable
extern volatile tSystem gtSystem;
#ifndef DEF_BOOT
extern volatile tUser gtUser;
#if defined(__ISP__)
extern volatile BYTE *gtISP;
#endif
#if defined(__NETWORK__)
extern volatile tNetwork gtNetwork;
#endif
#endif

//-------------------------------------------------------------------------------------------------
typedef struct {
	char strResolution[12];
	VideoResolution rIndex;
	UINT nWidth;
	UINT nHeight;
} ResolutionInfo;

#define ResolutionInfoLength e_resEndorUnknown

extern const ResolutionInfo listResolution[ResolutionInfoLength];


#endif //__USER_H__
