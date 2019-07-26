#include "dev.h"

#ifdef __RTC_LOAD__
#ifdef __RTC_PCF8563__
#include <string.h>
#include <time.h>
#include "pcf8563.h"

#define I2C_LOCK	//while (CS_I2C2)
#define I2C_UNLOCK	//CS_I2C2 = 0
#define I2C_CHECK(addr)				I2cCheck(RTC_I2C_CH, addr)
#define I2C_WRITE(addr, reg, data)	I2cWrite(RTC_I2C_CH, addr, reg, data)
#define I2C_READ(addr, reg)			I2cRead(RTC_I2C_CH, addr, reg)

/* Cache VL bit value read at driver init since writing the RTC_SECOND
 * register clears the VL status.
 */
static int voltage_low;
static const unsigned char days_in_month[] =
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static UINT pcf8563_check(void)
{
	I2C_LOCK;
	UINT nCkechk = I2C_CHECK(PCF8563_I2C_WRITE);
	I2C_UNLOCK;
	return nCkechk;
}

/**
    @return 
        ENX_OK is ok else ENX_FAIL.
*/
static ENX_OKFAIL pcf8563_write_reg(BYTE reg, BYTE data)
{
	I2C_LOCK;
	while (I2C_WRITE(PCF8563_I2C_WRITE, 0, 0));
	if (I2C_WRITE(reg, 0, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	if (I2C_WRITE(data, 1, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	I2C_UNLOCK;
	return ENX_OK;
}

/**
    @return 
        ENX_OK is ok else ENX_FAIL.
*/
static ENX_OKFAIL pcf8563_write_tm(struct tm *_tm)
{
	I2C_LOCK;
	while (I2C_WRITE(PCF8563_I2C_WRITE, 0, 0));
	if (I2C_WRITE(PCF8563_SECONDS, 0, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	if (I2C_WRITE(_tm->tm_sec, 0, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	if (I2C_WRITE(_tm->tm_min, 0, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	if (I2C_WRITE(_tm->tm_hour, 0, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	if (I2C_WRITE(_tm->tm_mday, 0, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	if (I2C_WRITE(_tm->tm_wday, 0, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	if (I2C_WRITE(_tm->tm_mon, 0, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	if (I2C_WRITE(_tm->tm_year, 1, 0)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	I2C_UNLOCK;
	return ENX_OK;
}

/**
    @return 
        ENX_OK is ok else ENX_FAIL.
*/
static ENX_OKFAIL pcf8563_read_reg(BYTE reg, BYTE *data)
{
	I2C_LOCK;
	while (I2C_WRITE(PCF8563_I2C_WRITE, 0, 0));
	if (I2C_WRITE(reg, 1, 1)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	while (I2C_WRITE(PCF8563_I2C_READ, 0, 0));
	*data = I2C_READ(1, 1);
	I2C_UNLOCK;
	return ENX_OK;
}

/**
    @return 
        ENX_OK is ok else ENX_FAIL.
*/
static ENX_OKFAIL pcf8563_read_tm(struct tm *_tm)
{
	I2C_LOCK;
	while (I2C_WRITE(PCF8563_I2C_WRITE, 0, 0));
	if (I2C_WRITE(PCF8563_SECONDS, 1, 1)) {
		I2C_UNLOCK;
		return ENX_FAIL;
	}
	while (I2C_WRITE(PCF8563_I2C_READ, 0, 0));
	_tm->tm_sec = I2C_READ(0, 0);
	_tm->tm_min = I2C_READ(0, 0);
	_tm->tm_hour = I2C_READ(0, 0);
	_tm->tm_mday = I2C_READ(0, 0);
	_tm->tm_wday = I2C_READ(0, 0);
	_tm->tm_mon = I2C_READ(0, 0);
	_tm->tm_year = I2C_READ(1, 1);
	I2C_UNLOCK;
	return ENX_OK;
}

static ENX_OKFAIL pcf8563_get_datetime(struct tm *_tm)
{
	if(pcf8563_read_tm(_tm) == ENX_FAIL)
		return ENX_FAIL;

	_tm->tm_year  = bcd2bin(_tm->tm_year) + ((_tm->tm_mon & 0x80) ? 100 : 0);
	_tm->tm_sec = bcd2bin(_tm->tm_sec & 0x7F);
	_tm->tm_min = bcd2bin(_tm->tm_min & 0x7F);
	_tm->tm_hour = bcd2bin(_tm->tm_hour & 0x3F);
	_tm->tm_mday = bcd2bin(_tm->tm_mday & 0x3F); /* Not coded in BCD. */
	_tm->tm_mon = bcd2bin(_tm->tm_mon & 0x1F);
	_tm->tm_mon--; /* Month is 1..12 in RTC but 0..11 in linux */

//	_printf("%s(%d) : sec(%d) min(%d) hour(%d) wday(%d) mday(%d) mon(%d) year(%d)\r\n", __func__, __LINE__, _tm->tm_sec, _tm->tm_min, _tm->tm_hour, _tm->tm_wday, _tm->tm_mday, _tm->tm_mon, _tm->tm_year);
	return rtc_valid_tm(_tm);
}

static ENX_OKFAIL pcf8563_set_datetime(struct tm *_tm)
{
    int leap;
    int year;
    int century;    
//	_printf("%s(%d) : sec(%d) min(%d) hour(%d) wday(%d) mday(%d) mon(%d) year(%d)\r\n", __func__, __LINE__, _tm->tm_sec, _tm->tm_min, _tm->tm_hour, _tm->tm_wday, _tm->tm_mday, _tm->tm_mon, _tm->tm_year);

	if(rtc_valid_tm(_tm) == ENX_FAIL)
		return ENX_FAIL;

    /* Convert from struct tm to struct rtc_time. */
    _tm->tm_year += 1900;
    _tm->tm_mon += 1;
    
    /*
     * Check if _tm->tm_year is a leap year. A year is a leap
     * year if it is divisible by 4 but not 100, except
     * that years divisible by 400 _are_ leap years.
     */
    year = _tm->tm_year;
    leap = (_tm->tm_mon == 2) && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);

	/* Perform some sanity checks. */
	if ((_tm->tm_year < 1970) ||
	    (_tm->tm_mon > 12) ||
	    (_tm->tm_mday == 0) ||
	    (_tm->tm_mday > days_in_month[_tm->tm_mon] + leap) ||
	    (_tm->tm_wday >= 7) ||
	    (_tm->tm_hour >= 24) ||
	    (_tm->tm_min >= 60) ||
	    (_tm->tm_sec >= 60))
		return ENX_FAIL;

	century = (_tm->tm_year >= 2000) ? 0x80 : 0;
	_tm->tm_year = _tm->tm_year % 100;

	_tm->tm_year = bin2bcd(_tm->tm_year);
	_tm->tm_mon = bin2bcd(_tm->tm_mon);
	_tm->tm_mday = bin2bcd(_tm->tm_mday);
	_tm->tm_hour = bin2bcd(_tm->tm_hour);
	_tm->tm_min = bin2bcd(_tm->tm_min);
	_tm->tm_sec = bin2bcd(_tm->tm_sec);
	_tm->tm_mon |= century;

    return pcf8563_write_tm(_tm);
}

/**
	register pcf8563 to rtc_device.
	and init pcf8563

	@return
		0 : OK
		1 : FAIL
*/
ENX_OKFAIL pcf8563_init(void)
{
	BYTE u8Data;
#if 0
	if(pcf8563_check() == ENX_FAIL) 
	{
		_Rprintf("  >>PCF8563 Not Connected...\n");
		goto err;
	}
#endif
	_Gprintf("  >>PCF8563 Connected...\n");

	/*
	* First of all we need to reset the chip. This is done by
	* clearing control1, control2 and clk freq and resetting
	* all alarms.
	*/
	if(pcf8563_write_reg(PCF8563_CONTROL1, 0x00) == ENX_FAIL)
	{
		_Rprintf("%s(%d) : RTC error RTC_CONTROL1\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_CONTROL2, 0x00) == ENX_FAIL)
	{
		_Rprintf("%s(%d) : RTC error RTC_CONTROL2\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_CLOCKOUT_FREQ, 0x00) == ENX_FAIL)
	{
		_Rprintf("%s(%d) : RTC error RTC_CLOCKOUT_FREQ\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_TIMER_CONTROL, 0x03) == ENX_FAIL)
	{
		_Rprintf("%s(%d) : RTC error RTC_TIMER_CONTROL\n", __func__, __LINE__);
		goto err;
	}

	/* Reset the alarms. */
	if(pcf8563_write_reg(PCF8563_MINUTE_ALARM, 0x80) == ENX_FAIL)
	{
		_Rprintf("%s(%d) : RTC error RTC_MINUTE_ALARM\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_HOUR_ALARM, 0x80) == ENX_FAIL)
	{
		_Rprintf("%s(%d) : RTC error RTC_HOUR_ALARM\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_DAY_ALARM, 0x80) == ENX_FAIL)
	{
		_Rprintf("%s(%d) : RTC error RTC_DAY_ALARM\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_WEEKDAY_ALARM, 0x80) == ENX_FAIL)
	{
		_Rprintf("%s(%d) : RTC error RTC_WEEKDAY_ALARM\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_read_reg(PCF8563_SECONDS, &u8Data) == ENX_FAIL)
	{
		_Rprintf("%s(%d) : RTC error PCF8563_SECONDS\n", __func__, __LINE__);
		goto err;
	}
	else
	{
		voltage_low = 0;
		if(u8Data & 0x80)
		{
			voltage_low = 1;
			_Rprintf("RTC Voltage Low - reliable date/time information\n");
		}
	}

	rtc_device.read_time = pcf8563_get_datetime;
	rtc_device.set_time  = pcf8563_set_datetime;
	strcpy(rtc_device.name, "PCF8563");

	return ENX_OK;
err:
    _Rprintf("rtc init is error\n");
    return ENX_FAIL;	 
}
#endif
#endif
