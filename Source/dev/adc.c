#include "dev.h"

static volatile _ADC_REG * const arrADC[ADC_CNT] = {
		(_ADC_REG *)(REG_BASE_ADC+(1<<3)),
		(_ADC_REG *)(REG_BASE_ADC+(2<<3)),
		(_ADC_REG *)(REG_BASE_ADC+(3<<3)),
		(_ADC_REG *)(REG_BASE_ADC+(4<<3)),
		(_ADC_REG *)(REG_BASE_ADC+(5<<3)),
		(_ADC_REG *)(REG_BASE_ADC+(6<<3)),
		(_ADC_REG *)(REG_BASE_ADC+(7<<3)),
		(_ADC_REG *)(REG_BASE_ADC+(8<<3))};

void AdcInit(UINT Speed_Hz)
{
	ADC_CLK_LMT = (MCK_FREQ / Speed_Hz >> 1) - 1;
	ADC_CKEN = 1;
	for (uint64 i = 0; i < ADC_CNT; i++) {
		arrADC[i]->EN = 0;
	}
	ADC_EN = 1;

	printf("ADC Init - %uHz\n", MCK_FREQ / ((ADC_CLK_LMT + 1) * 2));
}

void AdcOn(UINT nCH)
{
	arrADC[nCH]->EN = 1;
}

void AdcOff(UINT nCH)
{
	arrADC[nCH]->EN = 0;
}

BYTE AdcIsEnable(UINT nCH)
{
	return arrADC[nCH]->EN;
}

WORD AdcGet(UINT nCH)
{
	return (WORD)arrADC[nCH]->DAT;
}
