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

// time.c //////////////////////////////////////////////////////////////////////
#define TimeZone_GMT 1
#define TimeZone_LOC 0

extern const char* const strWeek[];
extern const char* const strMonth[];

extern void enx_get_tmtime(time_t get_time, struct tm *tmtime, UINT is_local);
extern int set_devicetime(int nTimezone, UINT nYear, UINT nMonth, UINT nDay, UINT nHour, UINT nMin, UINT nSec);

#endif // __DEV_H__
