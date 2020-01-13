#include "dev.h"

#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif

#include <string.h>

// Runtime data
VideoSource gvsVideo[VIDEO_SOURCE_CNT];		// source1, source2, source3, source4
//VideoEncoder gveVideo[VIDEO_ENCODER_CNT];

// Save/Runtime data
volatile tSystem gtSystem;		// System information
volatile tUser gtUser;			// User information
#if defined(__ISP__)
volatile BYTE *gtISP;			// ISP information
#endif
#if defined(__NETWORK__)
volatile tNetwork gtNetwork;	// Network information
#endif

const ResolutionInfo listResolution[ResolutionInfoLength] = {
	{"3840x2160", e_res3840x2160, 3840, 2160},
	{"2560x2048", e_res2560x2048, 2560, 2048},
	{"2560x1600", e_res2560x1600, 2560, 1600},
	{"2560x1440", e_res2560x1440, 2560, 1440},
	{"2048x1536", e_res2048x1536, 2048, 1536},
	{"1920x1200", e_res1920x1200, 1920, 1200},
	{"1920x1080", e_res1920x1080, 1920, 1080},
	{"1280x720", e_res1280x720, 1280, 720},
	{"800x600", e_res800x600, 800, 600},
	{"704x480", e_res704x480, 704, 480},
	{"704x400", e_res704x400, 704, 400},
	{"640x480", e_res640x480, 640, 480},
	{"640x360", e_res640x360, 640, 360},
	{"320x240", e_res320x240, 320, 240}
};

UINT getDateID(void)
{
	const char *sBuildDate = "FW Date : "__DATE__" "__TIME__;
	const UINT sBuildDateLen = strlen(sBuildDate);
	gptMsgShare.CMP_DATEID  = (*(sBuildDate + sBuildDateLen-1) - '0');
	gptMsgShare.CMP_DATEID += (*(sBuildDate + sBuildDateLen-2) - '0')<<4;
	gptMsgShare.CMP_DATEID += (*(sBuildDate + sBuildDateLen-4) - '0')<<8;
	gptMsgShare.CMP_DATEID += (*(sBuildDate + sBuildDateLen-5) - '0')<<12;
	return gptMsgShare.CMP_DATEID;
}

#if defined(__NETWORK__)
#include "md5.h"
//#include "sha256.h"
void UserPasswordSet(UserLoginData *user, char *pw)
{
	int nIDlen = strlen((char *)user->strUserID);
	int nREALMlen = sizeof(DEVICE_SYSTEM_REALM) - 1;
	int nPWlen = strlen(pw);

#if 1
	BYTE hash[16];
	MD5_CTX info;
	MD5Init(&info);
	MD5Update(&info, (unsigned char *)user->strUserID, nIDlen);
	MD5Update(&info, (unsigned char *)":", 1);
	MD5Update(&info, (unsigned char *)DEVICE_SYSTEM_REALM, nREALMlen);
	MD5Update(&info, (unsigned char *)":", 1);
	MD5Update(&info, (unsigned char *)pw, nPWlen);
	MD5Final(hash, &info);
	Hex2Str32B(info.digest, (char *)user->hashUserPW);
#else
	SHA256_INFO info;
	SHA256_Init(&info);
	SHA256_Process(&info, user->strUserID, nIDlen);
	SHA256_Process(&info, ":", 1);
	SHA256_Process(&info, DEVICE_SYSTEM_REALM, nREALMlen);
	SHA256_Process(&info, ":", 1);
	SHA256_Process(&info, pw, nPWlen);
	SHA256_Close(&info, user->hashUserPW);
#endif
}

int UserPasswordVerify(char *id, BYTE *hashUserPw)
{
	UserLoginData *user = NULL;
	for (int i = 0; i < USER_MAX_COUNT; i++) {
		if (strcmp((char *)gtNetwork.uldInfo[i].strUserID, id) == 0) {
			user = (UserLoginData *)&gtNetwork.uldInfo[i];
			break;
		}
	}

	if (user) {
		return memcmp(hashUserPw, (void *)user->hashUserPW, 32) == 0 ? ENX_OK : ENX_FAIL;
	} else {
		return ENX_FAIL;
	}
}

int UserPasswordCheck(char *id, char *pw)
{
	int nIDlen = strlen(id);
	int nREALMlen = sizeof(DEVICE_SYSTEM_REALM) - 1;
	int nPWlen = strlen(pw);
	BYTE hashUserPw[36] = {0};

#if 1
	BYTE hash[16];
	MD5_CTX info;
	MD5Init(&info);
	MD5Update(&info, (unsigned char *)id, nIDlen);
	MD5Update(&info, (unsigned char *)":", 1);
	MD5Update(&info, (unsigned char *)DEVICE_SYSTEM_REALM, nREALMlen);
	MD5Update(&info, (unsigned char *)":", 1);
	MD5Update(&info, (unsigned char *)pw, nPWlen);
	MD5Final(hash, &info);
	Hex2Str32B(info.digest, (char *)hashUserPw);
#else
	SHA256_INFO info;
	SHA256_Init(&info);
	SHA256_Process(&info, id, nIDlen);
	SHA256_Process(&info, ":", 1);
	SHA256_Process(&info, DEVICE_SYSTEM_REALM, nREALMlen);
	SHA256_Process(&info, ":", 1);
	SHA256_Process(&info, pw, nPWlen);
	SHA256_Close(&info, hashUserPw);
#endif

	return UserPasswordVerify(id, hashUserPw);
}
#endif

BYTE UserAreaCmd(sfls_cmd cmd, sfls_fixed_area index)
{
	ENX_OKFAIL bRes = ENX_FAIL;
	BYTE *pgtBase = (BYTE *)NULL; // gptMsgISPTbl.ISPTblAddr;
	UINT nBase = 0, nISP = 0, nCount = 1, nSize = 512, i;

	printf("%s - CMD:%d, Index:%d\n", __func__, cmd, index);


	switch (index) {
		case sfls_SYSTEM:	// FFF000
			nBase = SFLS_SYSTEM_BASE;
			pgtBase = (BYTE *)&gtSystem;
			nSize = sizeof(tSystem);
			break;
#ifndef DEF_BOOT
		case sfls_USER:		// FFE000
			nBase = SFLS_USER_BASE;
			pgtBase = (BYTE *)&gtUser;
			nSize = sizeof(tUser);
			break;
#if defined(__ISP__)
		case sfls_ISP_SHD: // 3-FFD000
			nBase = (SFLS_ISP_BASE + SFLS_ISP_UNIT_SIZE * 0);
			nISP = 1;
			break;
		case sfls_ISP_MENU: // 4-FFC000
			nBase = (SFLS_ISP_BASE + SFLS_ISP_UNIT_SIZE * 1);
			nISP = 1;
			break;
		case sfls_ISP_MENU_BAK: // 5-FFB000
			nBase = (SFLS_ISP_BASE + SFLS_ISP_UNIT_SIZE * 2);
			nISP = 1;
			break;
		case sfls_ISP_USER: // 6-FFA000
			nBase = (SFLS_ISP_BASE + SFLS_ISP_UNIT_SIZE * 3);
			nISP = 1;
			break;
		case sfls_ISP_DATA: // 7-FF9000
			nBase = (SFLS_ISP_BASE + SFLS_ISP_UNIT_SIZE * 4);
			nISP = 1;
			break;
		case sfls_ISP_BLK0: // 8-FF8000
			nBase = (SFLS_ISP_BASE + SFLS_ISP_UNIT_SIZE * 5);
			nISP = 1;
			break;
		case sfls_ISP_BLK1: // 9-FF7000
			nBase = (SFLS_ISP_BASE + SFLS_ISP_UNIT_SIZE * 6);
			nISP = 1;
			break;
		case sfls_ISP_BLK2: // 10-FF6000
			nBase = (SFLS_ISP_BASE + SFLS_ISP_UNIT_SIZE * 7);
			nISP = 1;
			break;
#endif // if defined(__ISP__)
#if defined(__NETWORK__)
		case sfls_NETWORK:	// FF5000
			nBase = SFLS_NETWORK_BASE;
			pgtBase = (BYTE *)&gtNetwork;
			nSize = sizeof(tNetwork);
			break;
#endif // if defined(__NETWORK__)
#endif // ifndef DEF_BOOT
		default:
			printf("Error Idx(%d)...", index);
			break;
	}

	if (nBase != 0) {
		switch (cmd) {
			case sflsc_Erase:
				for (i = 0; i < nCount; i++) {
					SflsSectErase(nBase + (i * 4096), ENX_YES);
				}
				bRes = ENX_OK;
				break;
			case sflsc_Save:
				hwflush_dcache_range((ULONG)pgtBase, nSize);
				for (i = 0; i < nCount; i++) {
					SflsSectErase(nBase + (i * 4096), ENX_YES);
				}
#ifdef __FREERTOS__
				BDmaMemCpy_rtos(0, (BYTE *)(intptr_t)nBase, pgtBase, nSize);
#else
				BDmaMemCpy_isr(0, (BYTE *)(intptr_t)nBase, pgtBase, nSize);
#endif

				hexDump("SFLS SAVE", (void *)(intptr_t)nBase, nSize);

				bRes = ENX_OK;
				break;
			case sflsc_Load:
				hwflush_dcache_range((ULONG)pgtBase, nSize);
#ifdef __FREERTOS__
				BDmaMemCpy_rtos(0, pgtBase, (BYTE *)(intptr_t)nBase, nSize);
#else
				BDmaMemCpy_isr(0, pgtBase, (BYTE *)(intptr_t)nBase, nSize);
#endif
				hexDump("SFLS READ", pgtBase, nSize);

				bRes = ENX_OK;
				break;
			case sflsc_Err:
			default:
				printf("Error Cmd(%d)...", cmd);
				break;
		}
	}

	return bRes;
}

//*************************************************************************************************
// Video Source Set
//-------------------------------------------------------------------------------------------------
//
void VideoSourceSetSwitch(eVideoSource vsIndex, ENX_SWITCH bSwitch)
{
	gvsVideo[vsIndex].bSwitch = bSwitch;
}

void VideoSourceSetName(eVideoSource vsIndex, char *strName)
{
	strlcpy(gvsVideo[vsIndex].strName, strName, 32);
}

void VideoSourceSetResolution(eVideoSource vsIndex, VideoResolution eResolution)
{
	gvsVideo[vsIndex].eResolution = eResolution;
}

void VideoSourceSetFps(eVideoSource vsIndex, SHORT nFps)
{
	gvsVideo[vsIndex].nFps = nFps;
}

void VideoSourceChangeEncoderCheck(eVideoSource vsIndex)
{
	// Encoder Check
	for (int i = 0; i < VIDEO_CHANNEL_CNT; i++) {
		if (gtUser.vcVideo[i].nVSourceIdx == vsIndex) {
			// 만약 source가 disable 될 경우
			if (gvsVideo[vsIndex].bSwitch == ENX_OFF) {
				gtUser.vcVideo[i].nVSourceIdx = -1;

				// 해당 Encoder는 더 이상 일하지 않는다.
				// ...
			}

			// 만약, resolution이 이전에 비해 작아질 경우
			if (gvsVideo[vsIndex].eResolution > gtUser.vcVideo[i].eResolution) {
				gtUser.vcVideo[i].eResolution = gvsVideo[vsIndex].eResolution;

				// 해당 Encoder는 해상도를 변경한다.
				// ...
			}

			// 만약, FPS가 이전에 비해 느려지거나 다를 경우
			if (gvsVideo[vsIndex].nFps < gtUser.vcVideo[i].nFps) {
				gtUser.vcVideo[i].nFps = gvsVideo[vsIndex].nFps;

				// 해당 Encoder는 FPS를 변경한다.
				// ...
			} else if (gvsVideo[vsIndex].nFps != gtUser.vcVideo[i].nFps) {
				// 맞지 않을 경우? ex) 25fps -> 30fps 가는 case
				// 기존에 30fps모드에서 10fps로 설정해둔 상태일 때,
				// 갱신이 25fps모드라면, 10fps는 25fps가 되야할까 5fps가 되야할까?
				gtUser.vcVideo[i].nFps = gvsVideo[vsIndex].nFps; // 일단은 그냥 현재 모드의 fps로 재설정

				// 해당 Encoder는 FPS를 변경한다.
				// ...
			}
		}
	}
}
