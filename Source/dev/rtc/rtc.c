#include "dev.h"
#include <time.h>	// struct tm
#include <string.h> // memset

#include "pcf8563.h"
#include "s35390a.h"

rtc_device_t rtc_device= {
	.name = {0},
	.read_time = NULL,
	.set_time = NULL
};

void rtc_clean(void)
{
	memset(&rtc_device, 0, sizeof(rtc_device));
}

void rtc_init(void)
{
	rtc_clean();
#ifdef __RTC_S35390A__
	if(s35390a_init() == ENX_FAIL) {	// RTC Init
		rtc_clean();					// init fail => rtc func pointer clean
	}
#endif
#ifdef __RTC_PCF8563__
	if (pcf8563_init() == ENX_FAIL) {	// RTC Init
		rtc_clean();					// init fail => rtc func pointer clean
	}
#endif
}

/**
	@return
		FAIL : ENX_FAIL
		OK   : ENX_OK
*/
ENX_OKFAIL rtc_set_time(struct tm *_tm)
{
	if(rtc_device.set_time)
	{
		return rtc_device.set_time(_tm);
	}
	return ENX_FAIL;
}

/**
	@return
		FAIL : ENX_FAIL
		OK   : ENX_OK
*/
ENX_OKFAIL rtc_get_time(struct tm *_tm)
{
	if(rtc_device.read_time)
	{
		return rtc_device.read_time(_tm);
	}
	return ENX_FAIL;
}
