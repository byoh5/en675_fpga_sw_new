#include "dev.h"

static _TIMER_REG0 *arrTMR0[TIMER_CNT];
static _TIMER_REG1 *arrTMR1[TIMER_CNT];
static tIhnd arrTIMERIrq[UART_CNT];

void TimerInit(UINT nCH)
{
	const uint64_t addrTMR[TIMER_CNT] = {
			REG_BASE_TIMER0,  REG_BASE_TIMER1,  REG_BASE_TIMER2,  REG_BASE_TIMER3,  REG_BASE_TIMER4,
			REG_BASE_TIMER5,  REG_BASE_TIMER6,  REG_BASE_TIMER7,  REG_BASE_TIMER8,  REG_BASE_TIMER9,
			REG_BASE_TIMER10, REG_BASE_TIMER11, REG_BASE_TIMER12, REG_BASE_TIMER13, REG_BASE_TIMER14,
			REG_BASE_TIMER15, REG_BASE_TIMER16, REG_BASE_TIMER17, REG_BASE_TIMER18, REG_BASE_TIMER19,
			REG_BASE_TIMER20, REG_BASE_TIMER21, REG_BASE_TIMER22, REG_BASE_TIMER23, REG_BASE_TIMER24,
			REG_BASE_TIMER25, REG_BASE_TIMER26, REG_BASE_TIMER27, REG_BASE_TIMER28, REG_BASE_TIMER29,
			REG_BASE_TIMER30, REG_BASE_TIMER31, REG_BASE_TIMER32, REG_BASE_TIMER33, REG_BASE_TIMER34,
			REG_BASE_TIMER35, REG_BASE_TIMER36, REG_BASE_TIMER37, REG_BASE_TIMER38
	};

	arrTMR0[nCH] = (_TIMER_REG0 *)(addrTMR[nCH] + (0 << 3));
	arrTMR1[nCH] = (_TIMER_REG1 *)(addrTMR[nCH] + (1 << 3));

	arrTMR0[nCH]->CK_EN = 0;
	arrTMR0[nCH]->IRQ_EN = 0;
	arrTMR0[nCH]->PWM_EN = 0;
	arrTMR0[nCH]->DIV = 0;
	arrTMR0[nCH]->LMT = 0;
	arrTMR1[nCH]->TRIG = 0;
	arrTMR0[nCH]->EN = 0;

	arrTIMERIrq[nCH].irqfn = NULL;
	arrTIMERIrq[nCH].arg = NULL;
}

void TimerDeInit(UINT nCH)
{
	TimerPWMDisable(nCH);
	arrTMR0[nCH]->CK_EN = 0;
	arrTMR0[nCH]->IRQ_EN = 0;
	arrTMR0[nCH]->DIV = 0;
	arrTMR0[nCH]->LMT = 0;
	arrTMR1[nCH]->TRIG = 0;
	arrTMR0[nCH]->EN = 0;

	arrTMR0[nCH] = NULL;
	arrTMR1[nCH] = NULL;
}

void TimerSetFreq(UINT nCH, UINT nDiv, UINT nLmt, UINT nTrig)
{
	arrTMR0[nCH]->DIV = nDiv;	// FREQ = MCK /(DIV+1)*2)/(LMT+1)
	arrTMR0[nCH]->LMT = nLmt;	// ex: 3? -> 0 1 2 3 0 1 2 3
	arrTMR1[nCH]->TRIG = nTrig;	// ex: 1? -> - - _ _ - - _ _ PWM 50%
}

void TimerStart(UINT nCH)
{
	arrTMR0[nCH]->CK_EN = 1;
	arrTMR0[nCH]->EN = 1;
}

void TimerStop(UINT nCH)
{
	arrTMR0[nCH]->CK_EN = 0;
	arrTMR0[nCH]->EN = 0;
}

void TimerPWMEnable(UINT nCH)
{
	arrTMR0[nCH]->PWM_EN = 1;
	switch (nCH) {
		case 0:		TIMER0_PIN_INIT;	break;
		case 1:		TIMER1_PIN_INIT;	break;
		case 2:		TIMER2_PIN_INIT;	break;
		case 3:		TIMER3_PIN_INIT;	break;
		case 4:		TIMER4_PIN_INIT;	break;
		case 5:		TIMER5_PIN_INIT;	break;
		case 6:		TIMER6_PIN_INIT;	break;
		case 7:		TIMER7_PIN_INIT;	break;
		case 8:		TIMER8_PIN_INIT;	break;
		case 9:		TIMER9_PIN_INIT;	break;
		case 10:	TIMER10_PIN_INIT;	break;
		case 11:	TIMER11_PIN_INIT;	break;
		case 12:	TIMER12_PIN_INIT;	break;
		case 13:	TIMER13_PIN_INIT;	break;
		case 14:	TIMER14_PIN_INIT;	break;
		case 15:	TIMER15_PIN_INIT;	break;
		case 16:	TIMER16_PIN_INIT;	break;
		case 17:	TIMER17_PIN_INIT;	break;
		case 18:	TIMER18_PIN_INIT;	break;
		case 19:	TIMER19_PIN_INIT;	break;
		case 20:	TIMER20_PIN_INIT;	break;
		case 21:	TIMER21_PIN_INIT;	break;
		case 22:	TIMER22_PIN_INIT;	break;
		case 23:	TIMER23_PIN_INIT;	break;
		case 24:	TIMER24_PIN_INIT;	break;
		case 25:	TIMER25_PIN_INIT;	break;
		case 26:	TIMER26_PIN_INIT;	break;
		case 27:	TIMER27_PIN_INIT;	break;
		case 28:	TIMER28_PIN_INIT;	break;
		case 29:	TIMER29_PIN_INIT;	break;
		case 30:	TIMER30_PIN_INIT;	break;
		case 31:	TIMER31_PIN_INIT;	break;
		case 32:	TIMER32_PIN_INIT;	break;
		case 33:	TIMER33_PIN_INIT;	break;
		case 34:	TIMER34_PIN_INIT;	break;
		case 35:	TIMER35_PIN_INIT;	break;
		case 36:	TIMER36_PIN_INIT;	break;
		case 37:	TIMER37_PIN_INIT;	break;
		case 38:	TIMER38_PIN_INIT;	break;
	}
}

void TimerPWMDisable(UINT nCH)
{
	arrTMR0[nCH]->PWM_EN = 0;
	switch (nCH) {
		case 0:		TIMER0_PIN_DEINIT;	break;
		case 1:		TIMER1_PIN_DEINIT;	break;
		case 2:		TIMER2_PIN_DEINIT;	break;
		case 3:		TIMER3_PIN_DEINIT;	break;
		case 4:		TIMER4_PIN_DEINIT;	break;
		case 5:		TIMER5_PIN_DEINIT;	break;
		case 6:		TIMER6_PIN_DEINIT;	break;
		case 7:		TIMER7_PIN_DEINIT;	break;
		case 8:		TIMER8_PIN_DEINIT;	break;
		case 9:		TIMER9_PIN_DEINIT;	break;
		case 10:	TIMER10_PIN_DEINIT;	break;
		case 11:	TIMER11_PIN_DEINIT;	break;
		case 12:	TIMER12_PIN_DEINIT;	break;
		case 13:	TIMER13_PIN_DEINIT;	break;
		case 14:	TIMER14_PIN_DEINIT;	break;
		case 15:	TIMER15_PIN_DEINIT;	break;
		case 16:	TIMER16_PIN_DEINIT;	break;
		case 17:	TIMER17_PIN_DEINIT;	break;
		case 18:	TIMER18_PIN_DEINIT;	break;
		case 19:	TIMER19_PIN_DEINIT;	break;
		case 20:	TIMER20_PIN_DEINIT;	break;
		case 21:	TIMER21_PIN_DEINIT;	break;
		case 22:	TIMER22_PIN_DEINIT;	break;
		case 23:	TIMER23_PIN_DEINIT;	break;
		case 24:	TIMER24_PIN_DEINIT;	break;
		case 25:	TIMER25_PIN_DEINIT;	break;
		case 26:	TIMER26_PIN_DEINIT;	break;
		case 27:	TIMER27_PIN_DEINIT;	break;
		case 28:	TIMER28_PIN_DEINIT;	break;
		case 29:	TIMER29_PIN_DEINIT;	break;
		case 30:	TIMER30_PIN_DEINIT;	break;
		case 31:	TIMER31_PIN_DEINIT;	break;
		case 32:	TIMER32_PIN_DEINIT;	break;
		case 33:	TIMER33_PIN_DEINIT;	break;
		case 34:	TIMER34_PIN_DEINIT;	break;
		case 35:	TIMER35_PIN_DEINIT;	break;
		case 36:	TIMER36_PIN_DEINIT;	break;
		case 37:	TIMER37_PIN_DEINIT;	break;
		case 38:	TIMER38_PIN_DEINIT;	break;
	}
}

void TimerIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrTIMERIrq[nCH].irqfn = irqfn;
	arrTIMERIrq[nCH].arg = arg;
}

void TimerIrqOn(UINT nCH)
{
	arrTMR0[nCH]->IRQ_EN = 1;
}

void TimerIrqOff(UINT nCH)
{
	arrTMR0[nCH]->IRQ_EN = 0;
}

void TimerIrqClear(UINT nCH)
{
	arrTMR0[nCH]->IRQ_CLR = 1;
}

UINT TimerIsIrq(UINT nCH)
{
	return arrTMR0[nCH]->IRQ;
}

void IrqTimer(UINT nCH)
{
	if (TimerIsIrq(nCH)) {
		_printf("Timer IRQ Get [%d]\n", nCH);
		if (arrTIMERIrq[nCH].irqfn) {
			arrTIMERIrq[nCH].irqfn(arrTIMERIrq[nCH].arg);
		}
		TimerIrqClear(nCH);
	}
}
