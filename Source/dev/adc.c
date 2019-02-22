#include "dev.h"

void AdcInit(UINT AdcClkFreq)
{
	ADC_CLK_LMT = (MCK_FREQ / AdcClkFreq>>1) - 1;
	ADC_CKEN = 1;
	ADC_CH0_EN = 1;
	ADC_CH1_EN = 1;
	ADC_CH2_EN = 1;
	ADC_CH3_EN = 1;
	ADC_CH4_EN = 1;
	ADC_CH5_EN = 1;
	ADC_CH6_EN = 1;
	ADC_CH7_EN = 1;
	ADC_EN = 1;
}

WORD adc_get_ch0()
{
	return (WORD) ADC_CH0_DAT;
}
WORD adc_get_ch1()
{
	return (WORD) ADC_CH1_DAT;
}
WORD adc_get_ch2()
{
	return (WORD) ADC_CH2_DAT;
}
WORD adc_get_ch3()
{
	return (WORD) ADC_CH3_DAT;
}
WORD adc_get_ch4()
{
	return (WORD) ADC_CH4_DAT;
}
WORD adc_get_ch5()
{
	return (WORD) ADC_CH5_DAT;
}
WORD adc_get_ch6()
{
	return (WORD) ADC_CH6_DAT;
}
WORD adc_get_ch7()
{
	return (WORD) ADC_CH7_DAT;
}