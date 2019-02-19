#include "dev.h"

static _I2C_REG0 *arrI2CCONT[I2C_CNT];
static _I2C_REG1 *arrI2CCLK[I2C_CNT];
static _I2C_REG2 *arrI2CMST[I2C_CNT];
static _I2C_REG3 *arrI2CSLV[I2C_CNT];

void I2cInit(UINT nCH, UINT Speed_Hz)
{
	const uint64_t addrI2C[UART_CNT] = {
			REG_BASE_I2C0, REG_BASE_I2C1, REG_BASE_I2C2, REG_BASE_I2C3, REG_BASE_I2C4,
			REG_BASE_I2C5, REG_BASE_I2C6, REG_BASE_I2C7, REG_BASE_I2C8
	};

	arrI2CCONT[nCH] = (_I2C_REG0 *)(addrI2C[nCH] + (0 << 3));
	arrI2CCLK[nCH] = (_I2C_REG1 *)(addrI2C[nCH] + (1 << 3));
	arrI2CMST[nCH] = (_I2C_REG2 *)(addrI2C[nCH] + (2 << 3));
	arrI2CSLV[nCH] = (_I2C_REG3 *)(addrI2C[nCH] + (3 << 3));

	arrI2CCLK[nCH]->CLK_DIV = (MCK_FREQ / (8 * Speed_Hz) - 1);	// Clock divider for I2C controller
	arrI2CCONT[nCH]->MODE = 0;		// Controller mode / 0:Master, 1:Slave
	arrI2CCONT[nCH]->BIT_MODE = 0;	// Data bit direction / 0:MSB first, 1:LSB first
	arrI2CCONT[nCH]->IRQ_EN = 0;	// Interrupt enable / 0:Disabled, 1:Enabled

	// pin mux setting
	switch (nCH) {
		case 0:
			I2C0_PIN_INIT;
			break;
		case 1:
			I2C1_PIN_INIT;
			break;
		case 2:
			I2C2_PIN_INIT;
			break;
		case 3:
			I2C3_PIN_INIT;
			break;
		case 4:
			I2C4_PIN_INIT;
			break;
		case 5:
			I2C5_PIN_INIT;
			break;
		case 6:
			I2C6_PIN_INIT;
			break;
		case 7:
			I2C7_PIN_INIT;
			break;
		case 8:
			I2C8_PIN_INIT;
			break;
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

UINT I2cCheck(UINT nCH, BYTE addr)
{
	UINT i, flag = DEF_FAIL;
	for (UINT i = 0; i < 10; i++) {
		if (I2cWrite(nCH, addr, 1, 0) == 0) {
			flag = DEF_OK;
			break;
		}
		WaitXus(10);
	}
	return flag;
}
