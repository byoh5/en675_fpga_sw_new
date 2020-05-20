#include "dev.h"
#ifdef __RTC_LOAD__
#include "rtc.h"
#endif
#if 0
const char* const strWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char* const strMonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void enx_get_tmtime(time_t get_time, struct tm *tmtime, UINT is_local)
{
	if (tmtime == NULL) {
		return;
	}

	if (is_local == DEF_YES) {
		localtime_r(&get_time, tmtime);
	} else {
		gmtime_r(&get_time, tmtime);
	}
}

int set_devicetime(int nTimezone, UINT nYear, UINT nMonth, UINT nDay, UINT nHour, UINT nMin, UINT nSec)
{
	if (nYear < 1970 || nYear > 2030) {
		_printf("Error : you input(%d), Year 1970 ~ 2030\n", nYear);
		return DEF_FAIL;
	}
	if (nMonth < 1 || nMonth > 12) {
		_printf("Error : you input(%d), Month 1 ~ 12\n", nMonth);
		return DEF_FAIL;
	}
	if (nDay < 1 || nDay > 31) {
		_printf("Error : you input(%d), Day 1 ~ 31\n", nDay);
		return DEF_FAIL;
	}
	if (nHour > 23) {
		_printf("Error : you input(%d), Hour 0 ~ 23\n", nHour);
		return DEF_FAIL;
	}
	if (nMin > 59) {
		_printf("Error : you input(%d), Minute 0 ~ 59\n", nMin);
		return DEF_FAIL;
	}
	if (nSec > 59) {
		_printf("Error : you input(%d), Sec 0 ~ 59\n", nSec);
		return DEF_FAIL;
	}

	struct tm tmget;
	tmget.tm_year = nYear - 1900;
	tmget.tm_mon = nMonth - 1;
	tmget.tm_mday = nDay;
	tmget.tm_hour = nHour;
	tmget.tm_min = nMin;
	tmget.tm_sec = nSec;
	tmget.tm_isdst = 0;
//	if (nTimezone == TimeZone_GMT) {
//		tmget.tm_isdst = 1;	// 입력값이 GMT+0시간이므로 1을 적용 (원래 이 용도가 아니다. 이것은 summer time을 위한 것.)
//	} else if(nTimezone == TimeZone_LOC) {
//		tmget.tm_isdst = 0;	// 입력값이 GMT+9시간이므로 0을 적용 (원래 이 용도가 아니다. 이것은 summer time을 위한 것.)
//	}

	_printf("sizeof(time_t)2 : %d\n", sizeof(time_t));

	gptMsgShare.TIME = mktime(&tmget);
	//gptMsgShare.TIMEZONE = nTimeZoneSec[getTimeZone()][2] + (gtUser.nTimeSummer * 3600);
	_printf("MAKETIME : [%d]\n", gptMsgShare.TIME);

	struct tm tmout;
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
	_printf("INPUT(GMT) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
	_printf("INPUT(LOT) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);

#ifdef __RTC_LOAD__
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
	if (rtc_set_time(&tmout) == DEF_FAIL) {
		_printf("RTC time set fail\n");
	}
#endif

	return DEF_OK;
}
#endif
