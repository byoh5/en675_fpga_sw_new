//*************************************************************************************************
// User shell commands
//*************************************************************************************************
#include "dev.h"
#include "enx_freertos.h"

#include "shell_cmd_common.h"

#include <sys/time.h> // gettimeofday
#include <stdlib.h> // atoi

const char *sTimeCmd[]      = {"System Time, TimeZone Get / Set",                (char*)0};

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
