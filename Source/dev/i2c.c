#include "dev.h"

#if USE_I2C0 | USE_I2C1 | USE_I2C2 | USE_I2C3 | USE_I2C4 | USE_I2C5 | USE_I2C6 | USE_I2C7 | USE_I2C8

static volatile _I2C_REG0 * const arrI2CCONT[I2C_CNT] = {(_I2C_REG0 *)(REG_BASE_I2C0+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C1+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C2+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C3+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C4+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C5+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C6+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C7+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C8+(0<<3))};
static volatile _I2C_REG1 * const arrI2CCLK[I2C_CNT]  = {(_I2C_REG1 *)(REG_BASE_I2C0+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C1+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C2+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C3+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C4+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C5+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C6+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C7+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C8+(1<<3))};
static volatile _I2C_REG2 * const arrI2CMST[I2C_CNT]  = {(_I2C_REG2 *)(REG_BASE_I2C0+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C1+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C2+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C3+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C4+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C5+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C6+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C7+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C8+(2<<3))};
static volatile _I2C_REG3 * const arrI2CSLV[I2C_CNT]  = {(_I2C_REG3 *)(REG_BASE_I2C0+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C1+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C2+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C3+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C4+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C5+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C6+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C7+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C8+(3<<3))};
ISRD static tIhnd arrI2CIrq[I2C_CNT];

void I2cInit(UINT nCH, UINT Speed_Hz)
{
	arrI2CCLK[nCH]->CLK_DIV = (MCK_FREQ / (8 * Speed_Hz) - 1);	// Clock divider for I2C controller
	arrI2CCONT[nCH]->MODE = 0;		// Controller mode / 0:Master, 1:Slave
	arrI2CCONT[nCH]->BIT_MODE = 0;	// Data bit direction / 0:MSB first, 1:LSB first
	arrI2CCONT[nCH]->IRQ_EN = 0;	// Interrupt enable / 0:Disabled, 1:Enabled

	arrI2CIrq[nCH].irqfn = NULL;
	arrI2CIrq[nCH].arg = NULL;

	// pin mux setting
	switch (nCH) {
		case 0:	I2C0_PIN_INIT;	break;
		case 1:	I2C1_PIN_INIT;	break;
		case 2:	I2C2_PIN_INIT;	break;
		case 3:	I2C3_PIN_INIT;	break;
		case 4:	I2C4_PIN_INIT;	break;
		case 5:	I2C5_PIN_INIT;	break;
		case 6:	I2C6_PIN_INIT;	break;
		case 7:	I2C7_PIN_INIT;	break;
		case 8:	I2C8_PIN_INIT;	break;
	}

	ENX_DEBUGF(DBG_I2C_LOG, "I2C%u Init - %uHz\n", nCH, MCK_FREQ / ((arrI2CCLK[nCH]->CLK_DIV + 1) * 8));
}

void I2cDeInit(UINT nCH)
{
	arrI2CCLK[nCH]->CLK_DIV   = 0xFFFF;
	arrI2CCONT[nCH]->MODE     = 0;
	arrI2CCONT[nCH]->BIT_MODE = 0;
	arrI2CCONT[nCH]->IRQ_EN   = 0;

	arrI2CIrq[nCH].irqfn = NULL;
	arrI2CIrq[nCH].arg = NULL;

	// pin mux setting
	switch (nCH) {
		case 0:	I2C0_PIN_DEINIT;	break;
		case 1:	I2C1_PIN_DEINIT;	break;
		case 2:	I2C2_PIN_DEINIT;	break;
		case 3:	I2C3_PIN_DEINIT;	break;
		case 4:	I2C4_PIN_DEINIT;	break;
		case 5:	I2C5_PIN_DEINIT;	break;
		case 6:	I2C6_PIN_DEINIT;	break;
		case 7:	I2C7_PIN_DEINIT;	break;
		case 8:	I2C8_PIN_DEINIT;	break;
	}
}

UINT I2cWrite(UINT nCH, BYTE dat, BYTE last, BYTE repeat)
{
	arrI2CCONT[nCH]->TX_DAT = dat;
	arrI2CMST[nCH]->a = ((repeat<<3) | ((repeat | last)<<2) | 3);
	while (arrI2CMST[nCH]->MST_GO);
	return arrI2CMST[nCH]->MST_ACK;
}

UINT I2cRead(UINT nCH, BYTE last, BYTE repeat)
{
	arrI2CMST[nCH]->a = ((repeat<<3) | ((repeat | last)<<2) | 1);
	while (arrI2CMST[nCH]->MST_GO);
	return arrI2CCONT[nCH]->RX_DAT;
}

ENX_OKFAIL I2cCheck(UINT nCH, BYTE addr)
{
	for (UINT i = 0; i < 10; i++) {
		if (I2cWrite(nCH, addr, 1, 0) == 0) {
			return ENX_OK;
		}
		WaitXus(10);
	}
	return ENX_FAIL;
}

void I2cChCheck(UINT nCH)
{
	printf("I2C%u Address Test\n", nCH);
	for (UINT i = 0; i < 256; i++) {
		if (I2cCheck(nCH, i) == ENX_OK) {
			_printf("[%X]", i);
		} else {
			_printf(".");
		}
	}
	printf("Test Done.\n");
}

void I2cSlvTest(UINT nCH)
{
	arrI2CSLV[nCH]->a = 0;
}

void I2cIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrI2CIrq[nCH].irqfn = irqfn;
	arrI2CIrq[nCH].arg = arg;
}

void I2cSetIrqEn(UINT nCH, ENX_SWITCH sw)
{
	arrI2CCONT[nCH]->IRQ_EN = sw;
}

ENX_SWITCH I2cGetIrqEn(UINT nCH)
{
	return arrI2CCONT[nCH]->IRQ_EN;
}

void I2cIrqClear(UINT nCH)
{
	arrI2CCONT[nCH]->IRQ_CLR = 1;
}

UINT I2cIsIrq(UINT nCH)
{
	return arrI2CCONT[nCH]->IRQ;
}

void IrqI2c(UINT nCH)
{
	if (I2cIsIrq(nCH)) {
		printf("I2C%u IRQ Get\n", nCH);
		if (arrI2CIrq[nCH].irqfn) {
			arrI2CIrq[nCH].irqfn(arrI2CIrq[nCH].arg);
		}
		I2cIrqClear(nCH);
	}
}
#else
void IrqI2c(UINT nCH)
{
	_Rprintf("I2C%u IRQ Get! I2C%u is inactive.\n", nCH, nCH);
	ENX_ASSERT(0);
}
#endif
