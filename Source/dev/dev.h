#ifndef __DEV_H__
#define __DEV_H__

#include "dev_types.h"	// EN675
#include "dev_regs.h"	// EN675
#include "dev_peri.h"	// EN675
#include "dev_device.h"	// EN675

#include "asm.h"	// CPU
#include "bits.h"	// CPU
#include "encoding.h"	// CPU
#include "mtrap.h"	// CPU

#include "vsnprintf.h"	// dev/string
#include "boot.h"	// core/shared
#include "msg.h"	// core/shared

#include <time.h>	// time_t
#include "rtc.h"

// uart printf out /////////////////////////////////////////////////////////////

#define DEBUG_UART_NUM 7

#define printf _printf

// time ////////////////////////////////////////////////////////////////////////
#define TIME_TICK			1000							// 1ms
#define TIMECMP_NEXT_VAL	((MCK_FREQ / 100) / TIME_TICK)

#define TimeZone_GMT 1
#define TimeZone_LOC 0

extern const char* const strWeek[];
extern const char* const strMonth[];

extern void enx_get_tmtime(time_t get_time, struct tm *tmtime, UINT is_local);
extern int set_devicetime(int nTimezone, UINT nYear, UINT nMonth, UINT nDay, UINT nHour, UINT nMin, UINT nSec);

// irq.c ///////////////////////////////////////////////////////////////////////
extern void enx_timerirq_init(void);
extern void enx_externalirq_init(void);

#endif // __DEV_H__
