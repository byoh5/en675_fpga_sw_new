#include "dev.h"
#ifdef __RTC_LOAD__
#include "rtc.h"
#endif

#include <stdio.h> // sscanf
#include <stdlib.h> // setenv/getenv

const char* const strWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char* const strMonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void enx_get_tmtime(time_t get_time, struct tm *tmtime, ENX_YN is_local)
{
	if (tmtime == NULL) {
		return;
	}

	if (is_local == ENX_YES) {
		localtime_r(&get_time, tmtime);
	} else {
		gmtime_r(&get_time, tmtime);
	}
}

ENX_OKFAIL set_devicetime(int nTimezone, UINT nYear, UINT nMonth, UINT nDay, UINT nHour, UINT nMin, UINT nSec)
{
	if (nYear < 1970 || nYear > 2030) {
		printf("Error : you input(%d), Year 1970 ~ 2030\n", nYear);
		return ENX_FAIL;
	}
	if (nMonth < 1 || nMonth > 12) {
		printf("Error : you input(%d), Month 1 ~ 12\n", nMonth);
		return ENX_FAIL;
	}
	if (nDay < 1 || nDay > 31) {
		printf("Error : you input(%d), Day 1 ~ 31\n", nDay);
		return ENX_FAIL;
	}
	if (nHour > 23) {
		printf("Error : you input(%d), Hour 0 ~ 23\n", nHour);
		return ENX_FAIL;
	}
	if (nMin > 59) {
		printf("Error : you input(%d), Minute 0 ~ 59\n", nMin);
		return ENX_FAIL;
	}
	if (nSec > 59) {
		printf("Error : you input(%d), Sec 0 ~ 59\n", nSec);
		return ENX_FAIL;
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

	gptMsgShare.TIME = mktime(&tmget);
	//gptMsgShare.TIMEZONE = nTimeZoneSec[getTimeZone()][2] + (gtUser.nTimeSummer * 3600);
	printf("MAKETIME : [%lu]\n", gptMsgShare.TIME);

	struct tm tmout;
	enx_get_tmtime(gptMsgShare.TIME, &tmout, ENX_NO);
	printf("INPUT(GMT) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
	enx_get_tmtime(gptMsgShare.TIME, &tmout, ENX_YES);
	printf("INPUT(LOT) : %04d-%02d-%02d %s %02d:%02d:%02d\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);

#ifdef __RTC_LOAD__
	enx_get_tmtime(gptMsgShare.TIME, &tmout, ENX_NO);
	if (rtc_set_time(&tmout) == ENX_FAIL) {
		printf("RTC time set fail\n");
	}
#endif

	return ENX_OK;
}

// TIME
int nTimeZoneSec[][3] = {	//
	{-12, 0, -43200},
	{-11, 0, -39600},
	{-10, 0, -36000},
	{-9, 0, -32400},
	{-8, 0, -28800},
	{-7, 0, -25200},
	{-6, 0, -21600},
	{-5, 0, -18000},
	{-4, 0, -14400},
	{-3, 0, -10800},
	{-2, 0, -7200},
	{-1, 0, -3600},
	{0, 0, 0},
	{1, 0, 3600},
	{2, 0, 7200},
	{3, 0, 10800},
	{3, 30, 12600},
	{4, 0, 14400},
	{5, 0, 18000},
	{5, 30, 19800},
	{6, 0, 21600},
	{7, 0, 25200},
	{8, 0, 28800},
	{9, 0, 32400},
	{10, 0, 36000},
	{11, 0, 39600},
	{12, 0, 43200},
	{13, 0, 46800},
	{14, 0, 50400}
};

int setTimeZone(TimeZone tzIndex)
{
	char strTimeZone[10] = {0};
	gtUser.nTimeZone = tzIndex;
	gptMsgShare.TIMEZONE = nTimeZoneSec[tzIndex][2] + (gtUser.nTimeSummer * 3600);
	sprintf(strTimeZone, "UTC%d:%d", -nTimeZoneSec[tzIndex][0], nTimeZoneSec[tzIndex][1]);
	return setenv("TZ", strTimeZone, 1);
}

TimeZone getTimeZone(void)
{
	int nHour = 0, nMin = 0, Mux = 0, i;
	char *out = getenv("TZ");
	if (out != NULL) {
		sscanf(out, "UTC%d:%d", &nHour, &nMin);
		if (nHour >= 0) { Mux = (-nHour * 3600) - (nMin * 30); }
		else			{ Mux = (-nHour * 3600) + (nMin * 30); }
		for (i = e_tzM1200; i <= e_tzP1400; i++) {
			if (nTimeZoneSec[i][2] == Mux) {
				if (gtUser.nTimeZone != (UINT)i) {
					gtUser.nTimeZone = i;
				}
				if (gptMsgShare.TIMEZONE != Mux) {
					gptMsgShare.TIMEZONE = Mux + (gtUser.nTimeSummer * 3600);
				}
				return i;
			}
		}
	}
	return e_tzN0000;
}
