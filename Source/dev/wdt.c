#include "dev.h"
#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif

#if EN675_SINGLE
void WdtoscInit(ULONG milsec)
{
	WDT_OSC_LMT = (milsec * OSC_FREQ / 1000);
	WDT_OSC_EN = 1;
}

void WdtoscReset(void)
{
	WDT_OSC_CLR = 1;
}

void WdtoscReboot(void)
{
	WdtoscReset();
	WdtoscInit(0);
}

#ifdef __FREERTOS__
static void vTimerRebootCallback(TimerHandle_t xTimer)
{
	uint64_t ulCount = (uint64_t )pvTimerGetTimerID(xTimer);
	if (ulCount == 0) {
		WdtoscReboot();
	}
	ulCount--;
	vTimerSetTimerID(xTimer, (void *)ulCount );
}

void WdtoscWaitReboot(ULONG milsec)
{
	_Rprintf("Reboot after %lu ms\n", milsec);
	TimerHandle_t xTimersReboot = xTimerCreate("WdtoscReboot", 1, pdTRUE, (void *)(intptr_t)(milsec/TIME_1TICK_TO_MS), vTimerRebootCallback);
	xTimerStart(xTimersReboot, 0);
}
#endif
#endif
