#include "dev.h"

void WdtInit(uint64_t milsec)
{
	SYS_WDT_LMT = (milsec * OSC_FREQ / 1000);
	SYS_WDT_EN = 1;
}

void WdtReset(void)
{
	SYS_WDT_CLR = 1;
//	(SYS_WDT_LMT - SYS_WDT_CNT)*1000/(OSC_FREQ) = remain time (ms)
}
