#include "dev.h"
#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif

void WdtInit(ULONG milsec)
{
	SYS_WDT_LMT = (milsec * OSC_FREQ / 1000);
	SYS_WDT_EN = 1;
}

void WdtReset(void)
{
	SYS_WDT_CLR = 1;
}

void WdtReboot(void)
{
	WdtReset();
	WdtInit(0);
}

#ifdef __FREERTOS__
static void vTimerRebootCallback(TimerHandle_t xTimer)
{
	uint64_t ulCount = (uint64_t )pvTimerGetTimerID(xTimer);
	if (ulCount == 0) {
		WdtReboot();
	}
	ulCount--;
	vTimerSetTimerID(xTimer, (void *)ulCount );
}

void WdtWaitReboot(ULONG milsec)
{
	_Rprintf("Reboot after %lu ms\n", milsec);
	TimerHandle_t xTimersReboot = xTimerCreate("SysReboot", 1, pdTRUE, (void *)(intptr_t)(milsec/TIME_1TICK_TO_MS), vTimerRebootCallback);
	xTimerStart(xTimersReboot, 0);
}
#endif

ISRD static tIhnd hAtoBIrq;
ISRD static tIhnd hBtoAIrq;

void AtoBIrqCall(void)
{
	SYS_ATOB_CALL = 1;
}

void BtoAIrqCall(void)
{
	SYS_BTOA_CALL = 1;
}

void AtoBIrqCallback(irq_fn irqfn, void *arg)
{
	hAtoBIrq.irqfn = irqfn;
	hAtoBIrq.arg = arg;
}

void BtoAIrqCallback(irq_fn irqfn, void *arg)
{
	hBtoAIrq.irqfn = irqfn;
	hBtoAIrq.arg = arg;
}

void AtoBSetIrqEn(ENX_SWITCH sw)
{
	SYS_ATOB_IRQ_EN = sw;
}

void BtoASetIrqEn(ENX_SWITCH sw)
{
	SYS_BTOA_IRQ_EN = sw;
}

ENX_SWITCH AtoBGetIrqEn(void)
{
	return SYS_ATOB_IRQ_EN;
}

ENX_SWITCH BtoAGetIrqEn(void)
{
	return SYS_BTOA_IRQ_EN;
}

void AtoBIrqClear(void)
{
	SYS_ATOB_IRQ_CLR = 1;
}

void BtoAIrqClear(void)
{
	SYS_BTOA_IRQ_CLR = 1;
}

UINT AtoBIsIrq(void)
{
	return SYS_ATOB_IRQ;
}

UINT BtoAIsIrq(void)
{
	return SYS_BTOA_IRQ;
}

void IrqAtoB(void)
{
	if (AtoBIsIrq()) {
		if (hAtoBIrq.irqfn) {
			hAtoBIrq.irqfn(hAtoBIrq.arg);
		} else {
			printf("AtoB IRQ Get\n");
		}
		AtoBIrqClear();
	}
}

void IrqBtoA(void)
{
	if (BtoAIsIrq()) {
		if (hBtoAIrq.irqfn) {
			hBtoAIrq.irqfn(hBtoAIrq.arg);
		} else {
			printf("BtoA IRQ Get\n");
		}
		BtoAIrqClear();
	}
}
