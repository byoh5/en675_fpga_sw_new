#include "dev.h"

#include <time.h>
#include "rtc.h"

static const unsigned char rtc_days_in_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static const unsigned short rtc_ydays[2][13] = {
	/* Normal years */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
	/* Leap years */
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

#define LEAPS_THRU_END_OF(y) ((y)/4 - (y)/100 + (y)/400)

/*
 * The number of days in the month.
 */
int rtc_month_days(unsigned int month, unsigned int year)
{
	return rtc_days_in_month[month] + (is_leap_year(year) && month == 1);
}

/*
 * Does the rtc_time represent a valid date/time?
	@return 
		OK   : DEF_OK
		FAIL : DEF_FAIL
*/
int rtc_valid_tm(struct tm *_tm)
{
	_tm->tm_isdst = 1;	// 입력값이 GMT+0시간이므로 1을 적용
	if (_tm->tm_year < 70
		|| ((unsigned)_tm->tm_mon) >= 12
		|| _tm->tm_mday < 1
		|| _tm->tm_mday > rtc_month_days(_tm->tm_mon, _tm->tm_year + 1900)
		|| ((unsigned)_tm->tm_hour) >= 24
		|| ((unsigned)_tm->tm_min) >= 60
		|| ((unsigned)_tm->tm_sec) >= 60)
		return DEF_FAIL;
	return DEF_OK;
}