#include "dev.h"

#if USE_TIMER0 | USE_TIMER1 | USE_TIMER2 | USE_TIMER3 | USE_TIMER4 | USE_TIMER5 | USE_TIMER6 | USE_TIMER7 | USE_TIMER8 | USE_TIMER9 | USE_TIMER10 | USE_TIMER11 | USE_TIMER12 | USE_TIMER13 | USE_TIMER14 | USE_TIMER15 | USE_TIMER16 | USE_TIMER17 | USE_TIMER18 | USE_TIMER19 | USE_TIMER20 | USE_TIMER21 | USE_TIMER22 | USE_TIMER23 | USE_TIMER24 | USE_TIMER25 | USE_TIMER26 | USE_TIMER27 | USE_TIMER28 | USE_TIMER29 | USE_TIMER30 | USE_TIMER31 | USE_TIMER32 | USE_TIMER33 | USE_TIMER34 | USE_TIMER35 | USE_TIMER36 | USE_TIMER37 | USE_TIMER38

static volatile _TIMER_REG0 * const arrTMR0[TIMER_CNT] = {
		(_TIMER_REG0 *)(REG_BASE_TIMER0+(0<<3)),  (_TIMER_REG0 *)(REG_BASE_TIMER1+(0<<3)),  (_TIMER_REG0 *)(REG_BASE_TIMER2+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER3+(0<<3)),  (_TIMER_REG0 *)(REG_BASE_TIMER4+(0<<3)),  (_TIMER_REG0 *)(REG_BASE_TIMER5+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER6+(0<<3)),  (_TIMER_REG0 *)(REG_BASE_TIMER7+(0<<3)),  (_TIMER_REG0 *)(REG_BASE_TIMER8+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER9+(0<<3)),  (_TIMER_REG0 *)(REG_BASE_TIMER10+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER11+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER12+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER13+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER14+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER15+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER16+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER17+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER18+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER19+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER20+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER21+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER22+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER23+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER24+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER25+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER26+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER27+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER28+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER29+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER30+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER31+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER32+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER33+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER34+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER35+(0<<3)),
		(_TIMER_REG0 *)(REG_BASE_TIMER36+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER37+(0<<3)), (_TIMER_REG0 *)(REG_BASE_TIMER38+(0<<3))};
static volatile _TIMER_REG1 * const arrTMR1[TIMER_CNT] = {
		(_TIMER_REG1 *)(REG_BASE_TIMER0+(1<<3)),  (_TIMER_REG1 *)(REG_BASE_TIMER1+(1<<3)),  (_TIMER_REG1 *)(REG_BASE_TIMER2+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER3+(1<<3)),  (_TIMER_REG1 *)(REG_BASE_TIMER4+(1<<3)),  (_TIMER_REG1 *)(REG_BASE_TIMER5+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER6+(1<<3)),  (_TIMER_REG1 *)(REG_BASE_TIMER7+(1<<3)),  (_TIMER_REG1 *)(REG_BASE_TIMER8+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER9+(1<<3)),  (_TIMER_REG1 *)(REG_BASE_TIMER10+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER11+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER12+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER13+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER14+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER15+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER16+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER17+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER18+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER19+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER20+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER21+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER22+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER23+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER24+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER25+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER26+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER27+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER28+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER29+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER30+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER31+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER32+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER33+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER34+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER35+(1<<3)),
		(_TIMER_REG1 *)(REG_BASE_TIMER36+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER37+(1<<3)), (_TIMER_REG1 *)(REG_BASE_TIMER38+(1<<3))};
ISRD static tIhnd arrTIMERIrq[TIMER_CNT];

void TimerInit(UINT nCH)
{
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
	TimerSetPWMEn(nCH, ENX_OFF);
	arrTMR0[nCH]->CK_EN = 0;
	arrTMR0[nCH]->IRQ_EN = 0;
	arrTMR0[nCH]->DIV = 0;
	arrTMR0[nCH]->LMT = 0;
	arrTMR1[nCH]->TRIG = 0;
	arrTMR0[nCH]->EN = 0;

	arrTIMERIrq[nCH].irqfn = NULL;
	arrTIMERIrq[nCH].arg = NULL;
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

void TimerSetEn(UINT nCH, ENX_SWITCH sw)
{
	arrTMR0[nCH]->EN = sw;
}

ENX_SWITCH TimerGetEn(UINT nCH)
{
	return arrTMR0[nCH]->EN;
}

void TimerSetCken(UINT nCH, ENX_SWITCH sw)
{
	arrTMR0[nCH]->CK_EN = sw;
}

ENX_SWITCH TimerGetCken(UINT nCH)
{
	return arrTMR0[nCH]->CK_EN;
}

void TimerSetPWMEn(UINT nCH, ENX_SWITCH sw)
{
	arrTMR0[nCH]->PWM_EN = sw;
	if (sw == ENX_ON) {
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
	} else {
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
}

ENX_SWITCH TimerGetPWMEn(UINT nCH)
{
	return arrTMR0[nCH]->PWM_EN;
}

void TimerSetDiv(UINT nCH, UINT nDiv)
{
	arrTMR0[nCH]->DIV = nDiv;
}

UINT TimerGetDiv(UINT nCH)
{
	return arrTMR0[nCH]->DIV;
}

void TimerSetLmt(UINT nCH, UINT nLmt)
{
	arrTMR0[nCH]->LMT = nLmt;
}

UINT TimerGetLmt(UINT nCH)
{
	return arrTMR0[nCH]->LMT;
}

void TimerSetTrig(UINT nCH, UINT nTrig)
{
	arrTMR1[nCH]->TRIG = nTrig;
}

UINT TimerGetTrig(UINT nCH)
{
	return arrTMR1[nCH]->TRIG;
}

UINT TimerGetCount(UINT nCH)
{
	return arrTMR1[nCH]->CNT;
}

void TimerCountClear(UINT nCH)
{
	arrTMR0[nCH]->CNT_CLR = 1;
}

void TimerIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrTIMERIrq[nCH].irqfn = irqfn;
	arrTIMERIrq[nCH].arg = arg;
}

void TimerSetIrqEn(UINT nCH, ENX_SWITCH sw)
{
	arrTMR0[nCH]->IRQ_EN = sw;
}

ENX_SWITCH TimerGetIrqEn(UINT nCH)
{
	return arrTMR0[nCH]->IRQ_EN;
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
		if (IRQ_TIMER8 == 0) {
			printf("Timer%u IRQ Get\n", nCH);
		}
		if (arrTIMERIrq[nCH].irqfn) {
			arrTIMERIrq[nCH].irqfn(arrTIMERIrq[nCH].arg);
		}
		TimerIrqClear(nCH);
	}
}
#else
void IrqTimer(UINT nCH)
{
	_Rprintf("Timer%u IRQ Get! Timer%u is inactive.\n", nCH, nCH);
	ENX_ASSERT(0);
}
#endif
