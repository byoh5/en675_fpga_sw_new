#ifndef __ENX_TIME_H__
#define __ENX_TIME_H__

typedef enum {
	e_tzM1200 = 0,
	e_tzM1100,
	e_tzM1000,
	e_tzM0900,
	e_tzM0800,
	e_tzM0700,
	e_tzM0600,
	e_tzM0500,
	e_tzM0400,
	e_tzM0300,
	e_tzM0200,
	e_tzM0100,
	e_tzN0000,
	e_tzP0100,
	e_tzP0200,
	e_tzP0300,
	e_tzP0330,
	e_tzP0400,
	e_tzP0500,
	e_tzP0530,
	e_tzP0600,
	e_tzP0700,
	e_tzP0800,
	e_tzP0900,
	e_tzP1000,
	e_tzP1100,
	e_tzP1200,
	e_tzP1300,
	e_tzP1400,
} TimeZone;

#define TimeZone_GMT 1
#define TimeZone_LOC 0

extern void enx_get_tmtime(time_t get_time, struct tm *tmtime, ENX_YN is_local);
extern ENX_OKFAIL set_devicetime(int nTimezone, UINT nYear, UINT nMonth, UINT nDay, UINT nHour, UINT nMin, UINT nSec);
extern int setTimeZone(TimeZone tzIndex);
extern TimeZone getTimeZone(void);

extern const char* const strWeek[];
extern const char* const strMonth[];
extern int nTimeZoneSec[][3];

#endif // __ENX_TIME_H__
