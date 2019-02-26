//*************************************************************************************************
// User shell commands
//*************************************************************************************************
#include "dev.h"
#include "enx_freertos.h"

#include "shell_cmd_common.h"

#include <sys/time.h> // gettimeofday

const char *sTimeCmd[]      = {"System Time, TimeZone Get / Set",                (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//

int cmd_time(int argc, char *argv[])
{
	if (argc == 1) {
		_printf("gptMsgShare.UPTIME: %lus\n", gptMsgShare.UPTIME);
		_printf("gptMsgShare.TIME  : %lus\n", gptMsgShare.TIME);
		struct timeval tv;
		gettimeofday(&tv, NULL);
		_printf("gettimeofday      : %lus\n", tv.tv_sec);

		struct tm tmout;

#if defined(__ETH__) ||defined(__WIFI__)
#if (ENX_SNTPC_use==1)
		printf("SNTP timeset : %s\n", sntp_info.is_timeset == 1 ? "OK" : "FAIL");
		if (sntp_info.is_timeset == 1) {
			enx_get_tmtime(sntp_info.last_gettime, &tmout, DEF_YES);
			printf("SNTP lastget(LOT) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		}
#endif
#endif

		//_printf("Summer Time : %s\n", gtUser.nTimeSummer ? "ON" : "OFF");
		//TimeZone tz = getTimeZone();
		//_printf("TimeZone get : %d:%d(%d)\n", nTimeZoneSec[tz][0], nTimeZoneSec[tz][1], nTimeZoneSec[tz][2]);

		enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
		_printf("GMT : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
		_printf("LOT : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);

#ifdef __RTC_LOAD__
		struct tm _tm;
		if (rtc_get_time(&_tm) == DEF_OK) {
			time_t tmk = mktime(&_tm);
			enx_get_tmtime(tmk, &tmout, DEF_NO);
			_printf("GMT(RTC) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
			enx_get_tmtime(tmk, &tmout, DEF_YES);
			_printf("LOT(RTC) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		} else {
			_printf("RTC time get fail\n");
		}
#endif
	}

	return 0;
}
