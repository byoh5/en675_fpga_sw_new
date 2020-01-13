#include "dev.h"

void AdcInit(UINT Speed_Hz)
{
	ADC_CLK_DIV = (APB_FREQ / Speed_Hz >> 1) - 1;
	ADC_CLK_PD = 1;
	ADC_PD = 1;

	ENX_DEBUGF(DBG_ADC_LOG, "ADC Init - %uHz\n", APB_FREQ / ((ADC_CLK_DIV + 1) << 1));
}

void AdcOn(void)
{
	ADC_CLK_PD = 1;
	ADC_PD = 1;
}

void AdcOff(void)
{
	ADC_PD = 0;
	ADC_CLK_PD = 0;
}

BYTE AdcIsEnable(void)
{
	return (ADC_PD && ADC_CLK_PD);
}

void AdcSetClkdiv(UINT Clkdiv)
{
	ADC_CLK_DIV = Clkdiv;
}

UINT AdcGetClkdiv(void)
{
	return ADC_CLK_DIV;
}

void AdcSetClk(UINT Speed_Hz)
{
	UINT u32Cal = (APB_FREQ / (2 * Speed_Hz)) - 1;
	if (u32Cal > 0xFF) {
		ADC_CLK_DIV = 0xFF;
		ENX_DEBUGF(DBG_ADC_LOG, "ADC Clk Min.(%u/%u)\n", u32Cal, ADC_CLK_DIV);
	} else {
		ADC_CLK_DIV = u32Cal;
	}
}

UINT AdcGetClk(void)
{
//	ENX_DEBUGF(DBG_ADC_LOG, "ADC Clk Set %uHz(%u)\n", APB_FREQ / ((ADC_CLK_DIV + 1) * 2), ADC_CLK_DIV);
	return APB_FREQ / ((ADC_CLK_DIV + 1) * 4);
}

void AdcSetLockCnt(UINT lock_cnt)
{
	ADC_LOCK_CNT = lock_cnt;
}

UINT AdcGetLockCnt(void)
{
	return ADC_LOCK_CNT;
}

void AdcSetCapPos(UINT cappos)
{
	ADC_CAP_POS = cappos;
}

UINT AdcGetCapPos(void)
{
	return ADC_CAP_POS;
}

void AdcSetCtrlpos(UINT Ctrlpos)
{
	ADC_CTRL_POS = Ctrlpos;
}

UINT AdcGetCtrlpos(void)
{
	return ADC_CTRL_POS;
}

void AdcSetMode(UINT mode)
{
	ADC_MODE = mode;
}

UINT AdcGetMode(void)
{
	return ADC_MODE;
}

void AdcSetIirCoef(UINT nCH, UINT iir_coef)
{
	switch (nCH) {
	case 0:
		ADC_IIR_COEF0 = iir_coef;
		break;
	case 1:
		ADC_IIR_COEF1 = iir_coef;
		break;
	case 2:
		ADC_IIR_COEF2 = iir_coef;
		break;
	case 3:
		ADC_IIR_COEF3 = iir_coef;
		break;
	}
}

UINT AdcGetIirCoef(UINT nCH)
{
	switch (nCH) {
	case 0:
		return ADC_IIR_COEF0;
	case 1:
		return ADC_IIR_COEF1;
	case 2:
		return ADC_IIR_COEF2;
	case 3:
		return ADC_IIR_COEF3;
	}
	return 0xFFFFFFFF;
}

void AdcSetOsel(UINT nCH, UINT osel)
{
	switch (nCH) {
	case 0:
		ADC_OSEL0 = osel;
		break;
	case 1:
		ADC_OSEL1 = osel;
		break;
	case 2:
		ADC_OSEL2 = osel;
		break;
	case 3:
		ADC_OSEL3 = osel;
		break;
	}
}

UINT AdcGetOsel(UINT nCH)
{
	switch (nCH) {
	case 0:
		return ADC_OSEL0;
	case 1:
		return ADC_OSEL1;
	case 2:
		return ADC_OSEL2;
	case 3:
		return ADC_OSEL3;
	}
	return 0xFFFFFFFF;
}

void AdcSetEsel(UINT esel)
{
	ADC_ESEL = esel;
}

UINT AdcGetEsel(void)
{
	return ADC_ESEL;
}

void AdcSetHighf(UINT highf)
{
	ADC_HIGHF = highf;
}

UINT AdcGetHighf(void)
{
	return ADC_HIGHF;
}

void AdcSetOselIirInv(UINT oseliirinv)
{
	ADC_OSEL_IIR_INV = oseliirinv;
}

UINT AdcGetOselIirInv(void)
{
	return ADC_OSEL_IIR_INV;
}

WORD AdcGet(UINT nCH)
{
	switch (nCH) {
	case 0:
		return ADC_DATA0;
	case 1:
		return ADC_DATA1;
	case 2:
		return ADC_DATA2;
	case 3:
		return ADC_DATA3;
	}
	return 0xFFFF;
}
