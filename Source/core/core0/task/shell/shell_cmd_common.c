//*************************************************************************************************
// User shell commands
//*************************************************************************************************
#include "dev.h"
#include "enx_freertos.h"

#include "shell_cmd_common.h"

#include <sys/time.h> // gettimeofday
#include <stdlib.h> // atoi
#include <string.h> // strcmp

const char *sTimeCmd[]      = {"System Time, TimeZone Get / Set",                (char*)0};
const char *sUserAreaCmd[]	= {"User Info Control",								 (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//

int cmd_time(int argc, char *argv[])
{
	if (argc == 1) {
		printf("gptMsgShare.UPTIME: %lus\n", gptMsgShare.UPTIME);
		printf("gptMsgShare.TIME  : %lus\n", gptMsgShare.TIME);
		struct timeval tv;
		gettimeofday(&tv, NULL);
		printf("gettimeofday      : %lus\n", tv.tv_sec);

		struct tm tmout;

#if defined(__ETH__) ||defined(__WIFI__)
#if (ENX_SNTPC_use==1)
		printf("SNTP timeset : %s\n", sntp_info.is_timeset == 1 ? "OK" : "FAIL");
		if (sntp_info.is_timeset == 1) {
			enx_get_tmtime(sntp_info.last_gettime, &tmout, ENX_YES);
			printf("SNTP lastget(LOT) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		}
#endif
#endif

		//printf("Summer Time : %s\n", gtUser.nTimeSummer ? "ON" : "OFF");
		//TimeZone tz = getTimeZone();
		//printf("TimeZone get : %d:%d(%d)\n", nTimeZoneSec[tz][0], nTimeZoneSec[tz][1], nTimeZoneSec[tz][2]);

		enx_get_tmtime(gptMsgShare.TIME, &tmout, ENX_NO);
		printf("GMT : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		enx_get_tmtime(gptMsgShare.TIME, &tmout, ENX_YES);
		printf("LOT : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);

#ifdef __RTC_LOAD__
		struct tm _tm;
		if (rtc_get_time(&_tm) == ENX_OK) {
			time_t tmk = mktime(&_tm);
			enx_get_tmtime(tmk, &tmout, ENX_NO);
			printf("GMT(RTC) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
			enx_get_tmtime(tmk, &tmout, ENX_YES);
			printf("LOT(RTC) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		} else {
			printf("RTC time get fail\n");
		}
#endif
	} else if (argc == 7) {
		int nYear, nMonth, nDay, nHour, nMin, nSec;
		nYear = atoi(argv[1]);
		nMonth = atoi(argv[2]);
		nDay = atoi(argv[3]);
		nHour = atoi(argv[4]);
		nMin = atoi(argv[5]);
		nSec = atoi(argv[6]);
		if (set_devicetime(TimeZone_LOC, nYear, nMonth, nDay, nHour, nMin, nSec) == ENX_FAIL) {
			return 0;
		}
	} else {
		printf("CMD : time\n");//, timezone get\n");
		printf("	ex) time\n");
		printf("CMD : time set\n");
		printf("	format : [time YYYY MM DD hh mm ss]\n");
		printf("	ex) time set 2014 4 12 22 45 43\n");
		//printf("CMD : time zone set\n");
		//printf("	format : [time zone hh]\n");
		//printf("	ex) time zone 9\n");
	}

	return 0;
}

int cmd_userarea(int argc, char *argv[])
{
#define SFLSUSERAREA_INFO(a, b, c) printf("%s %s %s\n", a, b, c)

	int err_flag = 0;
	const char *strUserInfo = "User Info";
	const char *strCommand[3] = {"Save", "Load", "Erase"};
	const char *strState[3] = {"Start", "OK", "Fail"};

	if (argc == 3) {
		sfls_cmd selCmd = sflsc_Err;
		if (strcmp(argv[1], "save") == 0) {
			selCmd = sflsc_Save;
		} else if (strcmp(argv[1], "load") == 0) {
			selCmd = sflsc_Load;
		} else if (strcmp(argv[1], "zero") == 0) {
			selCmd = sflsc_Erase;
		}

		if (selCmd != sflsc_Err) {
			SFLSUSERAREA_INFO(strUserInfo, strCommand[selCmd], strState[0]);
			sfls_fixed_area index = atoi(argv[2]);
			switch (index) {
				case sfls_SYSTEM:
				case sfls_USER:
#if defined(__ISP__)
				case sfls_ISP_SHD:
				case sfls_ISP_MENU:
				case sfls_ISP_MENU_BAK:
				case sfls_ISP_USER:
				case sfls_ISP_DATA:
				case sfls_ISP_BLK0:
				case sfls_ISP_BLK1:
				case sfls_ISP_BLK2:
#endif
#if defined(__NETWORK__)
				case sfls_NETWORK:
#if defined(__ONVIF__)
				case sfls_ONVIF:
#endif
#endif
					UserAreaCmd(selCmd, index);
					printf("%s %s(%d) %s\n", strUserInfo, strCommand[selCmd], index, strState[1]);
					break;
				default:
					printf("%s %s(%d) %s\n", strUserInfo, strCommand[selCmd], index, strState[2]);
					err_flag = 1;
					break;
			}
		} else {
			err_flag = 1;
		}
 	} else {
		err_flag = 1;
	}

	if (err_flag == 1) {
		const char *strZero = "           %2d  : sfls_%s\n";
		printf("%s save [num] : %s %s\n", argv[0], strUserInfo, strCommand[sflsc_Save]);
		printf("%s load [num] : %s %s\n", argv[0], strUserInfo, strCommand[sflsc_Load]);
		printf("%s zero [num] : %s %s\n", argv[0], strUserInfo, strCommand[sflsc_Erase]);
		printf(strZero, sfls_SYSTEM, "SYSTEM");
		printf(strZero, sfls_USER, "USER");
#if defined(__ISP__)
		printf(strZero, sfls_ISP_SHD, "ISP_SHD");
		printf(strZero, sfls_ISP_MENU, "ISP_MENU");
		printf(strZero, sfls_ISP_MENU_BAK, "ISP_MENU_BAK");
		printf(strZero, sfls_ISP_USER, "ISP_USER");
		printf(strZero, sfls_ISP_DATA, "ISP_DATA");
		printf(strZero, sfls_ISP_BLK0, "ISP_BLK0");
		printf(strZero, sfls_ISP_BLK1, "ISP_BLK1");
		printf(strZero, sfls_ISP_BLK2, "ISP_BLK2");
#endif
#if defined(__NETWORK__)
		printf(strZero, sfls_NETWORK, "NETWORK");
#if defined(__ONVIF__)
		printf(strZero, sfls_ONVIF, "ONVIF");
#endif
#endif
		printf("sizeof info\n");
		printf("VideoSource  :%u\n", sizeof(VideoSource));
		printf("VideoChannel :%u\n", sizeof(VideoChannel));
		printf("tSystem      :%u\n", sizeof(tSystem));
		printf("tUser        :%u\n", sizeof(tUser));
		printf("UserLoginData:%u\n", sizeof(UserLoginData));
		printf("NetifAddress :%u\n", sizeof(NetifAddress));
		printf("tWifiUAPcfg  :%u\n", sizeof(tWifiUAPcfg));
		printf("tWifiSTAcfg  :%u\n", sizeof(tWifiSTAcfg));
		printf("tNetwork     :%u\n", sizeof(tNetwork));
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}
