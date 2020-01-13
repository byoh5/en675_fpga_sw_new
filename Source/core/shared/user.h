#ifndef __USER_H__
#define __USER_H__

#include "dev.h"
#include "wifi_cfg.h"

typedef struct {
	ENX_SWITCH bSwitch;				//  4Byte
	char strName[32];				// 32Byte
	VideoResolution	eResolution;	//  4Byte
	SHORT nFps;						//	2Byte
} VideoSource;						// 44Byte

#if 0
typedef struct {
	INT nVSourceIdx;				//  4Byte, VideoSource index, Enable

	// channelㅇ memory buffer
	// channel
} VideoEncoder;
#endif

#define STREAM_URL_LENGTH			16

typedef struct {
	INT nVSourceIdx;				//  4Byte, VideoSource index, Enable
	//INT nVEncoderIdx;				//  4Byte, VideoEncoder index, Enable
	char strStmUrl[STREAM_URL_LENGTH];	// 16Byte
	VideoCodec eCodec;				//	4Byte
	VideoResolution	eResolution;	//  4Byte
	VideoBitRateMode eBRMode;		//	4Byte
	VideoBitRateIdx eBitRate;		//	4Byte
	H264ProfileMode eProfileMode;	//  4Byte, H.264 only
	UINT nIDRFrame;					//  4Byte, H.265/H.264, max
	WORD nFps;						//	2Byte
	WORD nQuality;					//  2Byte, (H.265/H.264)0 ~ 51, (JPEG)0 ~ 99
} VideoChannel;						// 32Byte

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

	VideoChannel	vcVideo[VIDEO_CHANNEL_CNT];// stream1(H.264 or H.265), stream2(H.264 or H.265), stream3(JPEG)








#if (LOAD_FS_SDCARD==1)
	ENX_SWITCH		bSdVidSave;
	//UINT			nSDVidRecNum;
#else
	ENX_SWITCH		_dummy_bSdVidSave;
#endif











} tUser; // Size: ____byte(0x__)

#if defined(__NETWORK__)
typedef struct {
	char strUserID[16];
	char hashUserPW[36];
} UserLoginData;
#define USER_MAX_COUNT	5

typedef struct {
	UINT			u32IpAddr;				// Network - IP Address
	UINT			u32NetMask;				// Network - Netmask Address
	UINT			u32Gateway;				// Network - Gateway Address
	UINT			u32DnsSvr0;				// Network - DNS Server Address 1
	UINT			u32DnsSvr1;				// Network - DNS Server Address 2
	UINT			u1UseDhcp;				// Network - DHCP enable 0:off, 1:on
} NetifAddress;

typedef struct {
	UserLoginData	uldInfo[USER_MAX_COUNT];

	NetifAddress	naEthernet;
//	NetifAddress	naWifiuap;
//	NetifAddress	naWifista;

#if defined(__ETHERNET__)
	UINT			u3EthAutoNegotiation;
#else
	UINT			_Dummy_u3EthAutoNegotiation;
#endif

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
extern UINT getDateID(void);
extern BYTE UserAreaCmd(sfls_cmd cmd, sfls_fixed_area index);

#if defined(__NETWORK__)
extern void UserPasswordSet(UserLoginData *user, char *pw);
extern int UserPasswordVerify(char *id, BYTE *hashUserPw);
extern int UserPasswordCheck(char *id, char *pw);
#endif
//-------------------------------------------------------------------------------------------------
// Variable
extern VideoSource gvsVideo[VIDEO_SOURCE_CNT];

// sfls save/load data
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
