#include "dev.h"

static _TIMER_REG0 *arrTMR0[TIMER_CNT];
static _TIMER_REG1 *arrTMR1[TIMER_CNT];

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

	arrTMR0[nCH]->CK_EN = 1;
	arrTMR0[nCH]->IRQ_EN = 0;
	arrTMR0[nCH]->PWM_EN = 1;
	arrTMR0[nCH]->DIV = 1;	// FREQ = MCK /(DIV+1)*2)/(LMT+1)
	arrTMR0[nCH]->LMT = 3;	// 0 1 2 3 0 1 2 3
	arrTMR1[nCH]->TRIG = 1;	// - - _ _ - - _ _	PWM 50% @ 5.1MHz
	arrTMR0[nCH]->EN = 1;

	// pin mux setting
	// PWM EN이 1일 때만, Pin mux 세팅을 한다.
}
