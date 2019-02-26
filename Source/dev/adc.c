#include "dev.h"

static _ADC_REG0 *arrADC0;
static _ADC_REG1 *arrADC1[ADC_CNT];

void AdcInit(UINT Speed_Hz)
{
	arrADC0->CLK_LMT = (MCK_FREQ / Speed_Hz >> 1) - 1;
	arrADC0->CKEN = 1;
	for (uint64_t i = 0; i < ADC_CNT; i++) {
		arrADC1[i] = (_ADC_REG1 *)(REG_BASE_ADC + ((i + 1) << 3));
		arrADC1[i]->CHEN = 0;
	}
	arrADC0->EN = 1;
}

void AdcOn(UINT nCH)
{
	arrADC1[nCH]->CHEN = 1;
}

void AdcOff(UINT nCH)
{
	arrADC1[nCH]->CHEN = 0;
}

WORD AdcGet(UINT nCH)
{
	return (WORD)arrADC1[nCH]->DAT;
}
