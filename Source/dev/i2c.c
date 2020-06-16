#include "dev.h"

#if USE_I2C0 | USE_I2C1 | USE_I2C2 | USE_I2C3 | USE_I2C4 | USE_I2C5 | USE_I2C6 | USE_I2C7 | USE_I2C8

#if model_TgtBd == 1
	#undef REG_BASE_I2C0
	#define REG_BASE_I2C0	0x400FF000
#endif

static volatile _I2C_REG0 * const arrI2CCONT[I2C_CNT] = {(_I2C_REG0 *)(REG_BASE_I2C0+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C1+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C2+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C3+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C4+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C5+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C6+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C7+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C8+(0<<3))};
#if EN675_SINGLE_I2C_NEW
static volatile _I2C_REG1 * const arrI2CDAT[I2C_CNT]  = {(_I2C_REG1 *)(REG_BASE_I2C0+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C1+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C2+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C3+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C4+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C5+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C6+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C7+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C8+(1<<3))};
#else
static volatile _I2C_REG1 * const arrI2CCLK[I2C_CNT]  = {(_I2C_REG1 *)(REG_BASE_I2C0+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C1+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C2+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C3+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C4+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C5+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C6+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C7+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C8+(1<<3))};
#endif
static volatile _I2C_REG2 * const arrI2CMST[I2C_CNT]  = {(_I2C_REG2 *)(REG_BASE_I2C0+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C1+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C2+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C3+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C4+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C5+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C6+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C7+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C8+(2<<3))};
static volatile _I2C_REG3 * const arrI2CSLV[I2C_CNT]  = {(_I2C_REG3 *)(REG_BASE_I2C0+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C1+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C2+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C3+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C4+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C5+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C6+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C7+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C8+(3<<3))};
ISRD static tIhnd arrI2CIrq[I2C_CNT];
static UINT __i2c_start_flag[I2C_CNT];
static UINT __i2c_stop_flag[I2C_CNT];

void I2cInit(UINT nCH, UINT Speed_Hz)
{
	I2cSetClk(nCH, Speed_Hz);			// Clock divider for I2C controller
	I2cSetMode(nCH, I2C_Master);		// Controller mode / 0:Master, 1:Slave
	I2cSetBitmode(nCH, I2C_MSBfirst);	// Data bit direction / 0:MSB first, 1:LSB first
	I2cSetIrqEn(nCH, ENX_OFF);			// Interrupt enable / 0:Disabled, 1:Enabled

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

	ENX_DEBUGF(DBG_I2C_STATUS, "I2C%u(M) Init - %s%uHz%s\n", nCH, TTY_COLOR_GREEN, I2cGetClk(nCH), TTY_COLOR_RESET);
}

void I2cDeInit(UINT nCH)
{
	I2cSetClkdiv(nCH, 0xFFFF);
	I2cSetMode(nCH, I2C_Master);
	I2cSetBitmode(nCH, I2C_MSBfirst);
	I2cSetIrqEn(nCH, ENX_OFF);

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

inline void I2cSetClkdiv(UINT nCH, UINT Clkdiv)
{
#if EN675_SINGLE_I2C_NEW
	arrI2CCONT[nCH]->CLK_DIV = Clkdiv;
#else
	arrI2CCLK[nCH]->CLK_DIV = Clkdiv;
#endif
}

inline UINT I2cGetClkdiv(UINT nCH)
{
#if EN675_SINGLE_I2C_NEW
	return arrI2CCONT[nCH]->CLK_DIV;
#else
	return arrI2CCLK[nCH]->CLK_DIV;
#endif
}

inline void I2cSetClk(UINT nCH, UINT Speed_Hz)
{
	UINT u32Cal = (APB_FREQ / (8 * Speed_Hz)) - 1;
	if (u32Cal > 0xFFFF) {
		I2cSetClkdiv(nCH, 0xFFFF);
		ENX_DEBUGF(DBG_I2C_LOG, "I2C Clk Min.(%u/%u)\n", u32Cal, I2cGetClkdiv(nCH));
	} else {
		I2cSetClkdiv(nCH, u32Cal);
	}
}

inline UINT I2cGetClk(UINT nCH)
{
//	ENX_DEBUGF(DBG_I2C_LOG, "I2C Clk Set %uHz(%u)\n", APB_FREQ / ((arrI2CCLK[nCH]->CLK_DIV + 1) * 8), arrI2CCLK[nCH]->CLK_DIV);
	return APB_FREQ / ((I2cGetClkdiv(nCH) + 1) * 8);
}

#if EN675_SINGLE_I2C_NEW
inline void I2cSetSync(UINT nCH, BYTE sync)
{
	arrI2CCONT[nCH]->SYNC = sync;
}

inline UINT I2cGetSync(UINT nCH)
{
	return arrI2CCONT[nCH]->SYNC;
}
#endif

inline void I2cSetTxData(UINT nCH, BYTE dat)
{
#if EN675_SINGLE_I2C_NEW
	arrI2CDAT[nCH]->TX_DAT = dat;
#else
	arrI2CCONT[nCH]->TX_DAT = dat;
#endif
}

inline UINT I2cGetTxData(UINT nCH)
{
#if EN675_SINGLE_I2C_NEW
	return arrI2CDAT[nCH]->TX_DAT;
#else
	return arrI2CCONT[nCH]->TX_DAT;
#endif
}

inline  UINT I2cGetRxData(UINT nCH)
{
#if EN675_SINGLE_I2C_NEW
	return arrI2CDAT[nCH]->RX_DAT;
#else
	return arrI2CCONT[nCH]->RX_DAT;
#endif
}

inline UINT I2cGetACT(UINT nCH)
{
#if EN675_SINGLE_I2C_NEW
	return arrI2CMST[nCH]->ACT;
#else
	return arrI2CCONT[nCH]->ACT;
#endif
}

inline void I2cSetMode(UINT nCH, I2C_MODE mode)
{
	arrI2CCONT[nCH]->MODE = mode;
}

inline I2C_MODE I2cGetMode(UINT nCH)
{
	return arrI2CCONT[nCH]->MODE;
}

inline void I2cSetBitmode(UINT nCH, I2C_BITMODE bitmode)
{
	arrI2CCONT[nCH]->BIT_MODE = bitmode;
}

inline I2C_BITMODE I2cGetBitmode(UINT nCH)
{
	return arrI2CCONT[nCH]->BIT_MODE;
}

inline UINT I2cGetMstCol(UINT nCH)
{
	return arrI2CMST[nCH]->MST_COL;
}

inline UINT I2cGetMstAck(UINT nCH)
{
	return arrI2CMST[nCH]->MST_ACK;
}

inline void I2cSetMstRepeat(UINT nCH, BYTE repeat)
{
	arrI2CMST[nCH]->MST_REPEAT = repeat;
}

inline UINT I2cGetMstRepeat(UINT nCH)
{
	return arrI2CMST[nCH]->MST_REPEAT;
}

inline void I2cSetMstLast(UINT nCH, BYTE last)
{
	arrI2CMST[nCH]->MST_LAST = last;
}

inline UINT I2cGetMstLast(UINT nCH)
{
	return arrI2CMST[nCH]->MST_LAST;
}

inline void I2cSetMstRW(UINT nCH, BYTE rw)
{
	arrI2CMST[nCH]->MST_RW = rw;
}

inline UINT I2cGetMstRW(UINT nCH)
{
	return arrI2CMST[nCH]->MST_RW;
}

inline void I2cSetMstGo(UINT nCH, BYTE go)
{
	arrI2CMST[nCH]->MST_GO = go;
}

inline UINT I2cGetMstGo(UINT nCH)
{
	return arrI2CMST[nCH]->MST_GO;
}

UINT I2cWrite(UINT nCH, BYTE dat, BYTE last, BYTE repeat)
{
	I2cSetTxData(nCH, dat);
	arrI2CMST[nCH]->a = ((repeat<<3) | ((repeat | last)<<2) | 3);
	while (arrI2CMST[nCH]->MST_GO);
	return I2cGetMstAck(nCH);
}

UINT I2cRead(UINT nCH, BYTE last, BYTE repeat)
{
	arrI2CMST[nCH]->a = ((repeat<<3) | ((repeat | last)<<2) | 1);
	while (arrI2CMST[nCH]->MST_GO);
	return I2cGetRxData(nCH);
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
#if 1
	for (UINT i = 1; i < 127; i++) {
		if (I2cCheck(nCH, i << 1) == ENX_OK) {
			_printf("[%X]", i);
		} else {
			_printf(".");
		}
	}
#else
	for (UINT i = 0; i < 256; i++) {
		if (I2cCheck(nCH, i) == ENX_OK) {
			_printf("[%X]", i);
		} else {
			_printf(".");
		}
	}
#endif
	printf("Test Done.\n");
}

void I2cSlvInit(UINT nCH, UINT Speed_Hz, UINT nAddr)
{
	I2cSetClk(nCH, Speed_Hz);			// Clock divider for I2C controller
	I2cSetMode(nCH, I2C_Slave);			// Controller mode / 0:Master, 1:Slave
	I2cSetSlvAddr(nCH, nAddr);
	I2cSetBitmode(nCH, I2C_MSBfirst);	// Data bit direction / 0:MSB first, 1:LSB first
//	arrI2CSLV[nCH]->SLV_ACK_OUT = 1;
//	arrI2CSLV[nCH]->SLV_GO = 1;
	I2cSetIrqEn(nCH, ENX_ON);			// Interrupt enable / 0:Disabled, 1:Enabled

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

	ENX_DEBUGF(DBG_I2C_STATUS, "I2C%u(S) Init - %s%uHz%s\n", nCH, TTY_COLOR_GREEN, I2cGetClk(nCH), TTY_COLOR_RESET);
}

inline void I2cSlvTest(UINT nCH)
{
	arrI2CSLV[nCH]->a = 0;
}

#if EN675_SINGLE
inline UINT I2cSlvGetStartflagBuf(UINT nCH)
{
	return __i2c_start_flag[nCH];
}

inline UINT I2cGetSlvStartflag(UINT nCH)
{
#if EN675_SINGLE_I2C_NEW
	__i2c_start_flag[nCH] = arrI2CSLV[nCH]->SLV_START;
#else
	__i2c_start_flag[nCH] = arrI2CCONT[nCH]->START_FLAG;
#endif
	return I2cSlvGetStartflagBuf(nCH);
}

inline UINT I2cSlvGetStopflagBuf(UINT nCH)
{
	return __i2c_stop_flag[nCH];
}

inline UINT I2cGetSlvStopflag(UINT nCH)
{
#if EN675_SINGLE_I2C_NEW
	__i2c_stop_flag[nCH] = arrI2CSLV[nCH]->SLV_STOP;
#else
	__i2c_stop_flag[nCH] = arrI2CCONT[nCH]->LAST_FLAG;
#endif
	return I2cSlvGetStopflagBuf(nCH);
}
#endif

inline UINT I2cGetSlvSDA(UINT nCH)
{
	return arrI2CSLV[nCH]->I2C_SDA;
}

inline UINT I2cGetSlvSCL(UINT nCH)
{
	return arrI2CSLV[nCH]->I2C_SCL;
}

inline UINT I2cGetSlvAckIn(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_ACK_IN;
}

inline void I2cSetSlvGo(UINT nCH, UINT go)
{
	arrI2CSLV[nCH]->SLV_GO = go;
}

inline UINT I2cGetSlvGo(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_GO;
}

inline void I2cSetSlvRW(UINT nCH, UINT rw)
{
	arrI2CSLV[nCH]->SLV_RW = rw;
}

inline UINT I2cGetSlvRW(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_RW;
}

inline void I2cSetSlvAckOut(UINT nCH, UINT ackout)
{
	arrI2CSLV[nCH]->SLV_ACK_OUT = ackout;
}

inline UINT I2cGetSlvAckOut(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_ACK_OUT;
}

inline void I2cSetSlvAddr(UINT nCH, UINT nAddr)
{
	arrI2CSLV[nCH]->SLV_ADR = nAddr;
}

inline UINT I2cGetSlvAddr(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_ADR;
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
#if EN675_SINGLE
		I2cGetSlvStartflag(nCH);
		I2cGetSlvStopflag(nCH);
#endif
		I2cIrqClear(nCH);
		if (arrI2CIrq[nCH].irqfn) {
			arrI2CIrq[nCH].irqfn(arrI2CIrq[nCH].arg);
		} else {
			printf("I2C%u IRQ Get\n", nCH);
		}
		I2cSetSlvGo(nCH, 1);
	}
}
#else
void IrqI2c(UINT nCH)
{
	_Rprintf("I2C%u IRQ Get! I2C%u is inactive.\n", nCH, nCH);
	ENX_ASSERT(0);
}
#endif

#else
#include "dev.h"

#if USE_I2C0 | USE_I2C1 | USE_I2C2 | USE_I2C3 | USE_I2C4 | USE_I2C5 | USE_I2C6 | USE_I2C7 | USE_I2C8

#if model_TgtBd == 1
	#undef REG_BASE_I2C0
	#define REG_BASE_I2C0	0x400FF000
#endif

static volatile _I2C_REG0 * const arrI2CCONT[I2C_CNT] = {(_I2C_REG0 *)(REG_BASE_I2C0+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C1+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C2+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C3+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C4+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C5+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C6+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C7+(0<<3)), (_I2C_REG0 *)(REG_BASE_I2C8+(0<<3))};
#if EN675_SINGLE
static volatile _I2C_REG1 * const arrI2CDAT[I2C_CNT]  = {(_I2C_REG1 *)(REG_BASE_I2C0+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C1+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C2+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C3+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C4+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C5+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C6+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C7+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C8+(1<<3))};
#else
static volatile _I2C_REG1 * const arrI2CCLK[I2C_CNT]  = {(_I2C_REG1 *)(REG_BASE_I2C0+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C1+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C2+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C3+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C4+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C5+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C6+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C7+(1<<3)), (_I2C_REG1 *)(REG_BASE_I2C8+(1<<3))};
#endif
static volatile _I2C_REG2 * const arrI2CMST[I2C_CNT]  = {(_I2C_REG2 *)(REG_BASE_I2C0+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C1+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C2+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C3+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C4+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C5+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C6+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C7+(2<<3)), (_I2C_REG2 *)(REG_BASE_I2C8+(2<<3))};
static volatile _I2C_REG3 * const arrI2CSLV[I2C_CNT]  = {(_I2C_REG3 *)(REG_BASE_I2C0+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C1+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C2+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C3+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C4+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C5+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C6+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C7+(3<<3)), (_I2C_REG3 *)(REG_BASE_I2C8+(3<<3))};
ISRD static tIhnd arrI2CIrq[I2C_CNT];
#if EN675_SINGLE
static UINT __i2c_start_flag[I2C_CNT];
static UINT __i2c_stop_flag[I2C_CNT];
#endif

void I2cInit(UINT nCH, UINT Speed_Hz)
{
	I2cSetClk(nCH, Speed_Hz);			// Clock divider for I2C controller
	I2cSetMode(nCH, I2C_Master);		// Controller mode / 0:Master, 1:Slave
	I2cSetBitmode(nCH, I2C_MSBfirst);	// Data bit direction / 0:MSB first, 1:LSB first
	I2cSetIrqEn(nCH, ENX_OFF);			// Interrupt enable / 0:Disabled, 1:Enabled

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

	ENX_DEBUGF(DBG_I2C_STATUS, "I2C%u(M) Init - %s%uHz%s\n", nCH, TTY_COLOR_GREEN, I2cGetClk(nCH), TTY_COLOR_RESET);
}

void I2cDeInit(UINT nCH)
{
	I2cSetClkdiv(nCH, 0xFFFF);
	I2cSetMode(nCH, I2C_Master);
	I2cSetBitmode(nCH, I2C_MSBfirst);
	I2cSetIrqEn(nCH, ENX_OFF);

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

inline void I2cSetClkdiv(UINT nCH, UINT Clkdiv)
{
#if EN675_SINGLE
	arrI2CCONT[nCH]->CLK_DIV = Clkdiv;
#else
	arrI2CCLK[nCH]->CLK_DIV = Clkdiv;
#endif
}

inline UINT I2cGetClkdiv(UINT nCH)
{
#if EN675_SINGLE
	return arrI2CCONT[nCH]->CLK_DIV;
#else
	return arrI2CCLK[nCH]->CLK_DIV;
#endif
}

inline void I2cSetClk(UINT nCH, UINT Speed_Hz)
{
	UINT u32Cal = (APB_FREQ / (8 * Speed_Hz)) - 1;
	if (u32Cal > 0xFFFF) {
		I2cSetClkdiv(nCH, 0xFFFF);
		ENX_DEBUGF(DBG_I2C_LOG, "I2C Clk Min.(%u/%u)\n", u32Cal, I2cGetClkdiv(nCH));
	} else {
		I2cSetClkdiv(nCH, u32Cal);
	}
}

inline UINT I2cGetClk(UINT nCH)
{
//	ENX_DEBUGF(DBG_I2C_LOG, "I2C Clk Set %uHz(%u)\n", APB_FREQ / ((arrI2CCLK[nCH]->CLK_DIV + 1) * 8), arrI2CCLK[nCH]->CLK_DIV);
	return APB_FREQ / ((I2cGetClkdiv(nCH) + 1) * 8);
}

inline void I2cSetTxData(UINT nCH, BYTE dat)
{
#if EN675_SINGLE
	arrI2CDAT[nCH]->TX_DAT = dat;
#else
	arrI2CCONT[nCH]->TX_DAT = dat;
#endif
}

inline UINT I2cGetTxData(UINT nCH)
{
#if EN675_SINGLE
	return arrI2CDAT[nCH]->TX_DAT;
#else
	return arrI2CCONT[nCH]->TX_DAT;
#endif
}

inline  UINT I2cGetRxData(UINT nCH)
{
#if EN675_SINGLE
	return arrI2CDAT[nCH]->RX_DAT;
#else
	return arrI2CCONT[nCH]->RX_DAT;
#endif
}

inline UINT I2cGetACT(UINT nCH)
{
#if EN675_SINGLE
	return arrI2CMST[nCH]->ACT;
#else
	return arrI2CCONT[nCH]->ACT;
#endif
}

inline void I2cSetMode(UINT nCH, I2C_MODE mode)
{
	arrI2CCONT[nCH]->MODE = mode;
}

inline I2C_MODE I2cGetMode(UINT nCH)
{
	return arrI2CCONT[nCH]->MODE;
}

inline void I2cSetBitmode(UINT nCH, I2C_BITMODE bitmode)
{
	arrI2CCONT[nCH]->BIT_MODE = bitmode;
}

inline I2C_BITMODE I2cGetBitmode(UINT nCH)
{
	return arrI2CCONT[nCH]->BIT_MODE;
}

inline UINT I2cGetMstCol(UINT nCH)
{
	return arrI2CMST[nCH]->MST_COL;
}

inline UINT I2cGetMstAck(UINT nCH)
{
	return arrI2CMST[nCH]->MST_ACK;
}

inline void I2cSetMstRepeat(UINT nCH, BYTE repeat)
{
	arrI2CMST[nCH]->MST_REPEAT = repeat;
}

inline UINT I2cGetMstRepeat(UINT nCH)
{
	return arrI2CMST[nCH]->MST_REPEAT;
}

inline void I2cSetMstLast(UINT nCH, BYTE last)
{
	arrI2CMST[nCH]->MST_LAST = last;
}

inline UINT I2cGetMstLast(UINT nCH)
{
	return arrI2CMST[nCH]->MST_LAST;
}

inline void I2cSetMstRW(UINT nCH, BYTE rw)
{
	arrI2CMST[nCH]->MST_RW = rw;
}

inline UINT I2cGetMstRW(UINT nCH)
{
	return arrI2CMST[nCH]->MST_RW;
}

inline void I2cSetMstGo(UINT nCH, BYTE go)
{
	arrI2CMST[nCH]->MST_GO = go;
}

inline UINT I2cGetMstGo(UINT nCH)
{
	return arrI2CMST[nCH]->MST_GO;
}

UINT I2cWrite(UINT nCH, BYTE dat, BYTE last, BYTE repeat)
{
	I2cSetTxData(nCH, dat);
	arrI2CMST[nCH]->a = ((repeat<<3) | ((repeat | last)<<2) | 3);
	while (arrI2CMST[nCH]->MST_GO);
	return I2cGetMstAck(nCH);
}

UINT I2cRead(UINT nCH, BYTE last, BYTE repeat)
{
	arrI2CMST[nCH]->a = ((repeat<<3) | ((repeat | last)<<2) | 1);
	while (arrI2CMST[nCH]->MST_GO);
	return I2cGetRxData(nCH);
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

void I2cSlvInit(UINT nCH, UINT Speed_Hz, UINT nAddr)
{
	I2cSetClk(nCH, Speed_Hz);			// Clock divider for I2C controller
	I2cSetMode(nCH, I2C_Slave);			// Controller mode / 0:Master, 1:Slave
	I2cSetSlvAddr(nCH, nAddr);
	I2cSetBitmode(nCH, I2C_MSBfirst);	// Data bit direction / 0:MSB first, 1:LSB first
//	arrI2CSLV[nCH]->SLV_ACK_OUT = 1;
//	arrI2CSLV[nCH]->SLV_GO = 1;
	I2cSetIrqEn(nCH, ENX_ON);			// Interrupt enable / 0:Disabled, 1:Enabled

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

	ENX_DEBUGF(DBG_I2C_STATUS, "I2C%u(S) Init - %s%uHz%s\n", nCH, TTY_COLOR_GREEN, I2cGetClk(nCH), TTY_COLOR_RESET);
}

inline void I2cSlvTest(UINT nCH)
{
	arrI2CSLV[nCH]->a = 0;
}

#if EN675_SINGLE
inline UINT I2cSlvGetStartflagBuf(UINT nCH)
{
	return __i2c_start_flag[nCH];
}

inline UINT I2cGetSlvStartflag(UINT nCH)
{
	__i2c_start_flag[nCH] = arrI2CSLV[nCH]->SLV_START;
	return I2cSlvGetStartflagBuf(nCH);
}

inline UINT I2cSlvGetStopflagBuf(UINT nCH)
{
	return __i2c_stop_flag[nCH];
}

inline UINT I2cGetSlvStopflag(UINT nCH)
{
	__i2c_stop_flag[nCH] = arrI2CSLV[nCH]->SLV_STOP;
	return I2cSlvGetStopflagBuf(nCH);
}
#endif

inline UINT I2cGetSlvSDA(UINT nCH)
{
	return arrI2CSLV[nCH]->I2C_SDA;
}

inline UINT I2cGetSlvSCL(UINT nCH)
{
	return arrI2CSLV[nCH]->I2C_SCL;
}

inline UINT I2cGetSlvAckIn(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_ACK_IN;
}

inline void I2cSetSlvGo(UINT nCH, UINT go)
{
	arrI2CSLV[nCH]->SLV_GO = go;
}

inline UINT I2cGetSlvGo(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_GO;
}

inline void I2cSetSlvRW(UINT nCH, UINT rw)
{
	arrI2CSLV[nCH]->SLV_RW = rw;
}

inline UINT I2cGetSlvRW(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_RW;
}

inline void I2cSetSlvAckOut(UINT nCH, UINT ackout)
{
	arrI2CSLV[nCH]->SLV_ACK_OUT = ackout;
}

inline UINT I2cGetSlvAckOut(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_ACK_OUT;
}

inline void I2cSetSlvAddr(UINT nCH, UINT nAddr)
{
	arrI2CSLV[nCH]->SLV_ADR = nAddr;
}

inline UINT I2cGetSlvAddr(UINT nCH)
{
	return arrI2CSLV[nCH]->SLV_ADR;
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
#if EN675_SINGLE
		I2cGetSlvStartflag(nCH);
		I2cGetSlvStopflag(nCH);
#endif
		I2cIrqClear(nCH);
		if (arrI2CIrq[nCH].irqfn) {
			arrI2CIrq[nCH].irqfn(arrI2CIrq[nCH].arg);
		} else {
			printf("I2C%u IRQ Get\n", nCH);
		}
		I2cSetSlvGo(nCH, 1);
	}
}
#else
void IrqI2c(UINT nCH)
{
	_Rprintf("I2C%u IRQ Get! I2C%u is inactive.\n", nCH, nCH);
	ENX_ASSERT(0);
}
#endif
