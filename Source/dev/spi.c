#include "dev.h"

#if USE_SPI0 | USE_SPI1 | USE_SPI2 | USE_SPI3 | USE_SPI4 | USE_SPI5 | USE_SPI6 | USE_SPI7 | USE_SPI8

static volatile _SPI_REG0 * const arrSPIRX[SPI_CNT] = {(_SPI_REG0 *)(REG_BASE_SPI0+(0<<3)), (_SPI_REG0 *)(REG_BASE_SPI1+(0<<3)), (_SPI_REG0 *)(REG_BASE_SPI2+(0<<3)), (_SPI_REG0 *)(REG_BASE_SPI3+(0<<3)), (_SPI_REG0 *)(REG_BASE_SPI4+(0<<3)), (_SPI_REG0 *)(REG_BASE_SPI5+(0<<3)), (_SPI_REG0 *)(REG_BASE_SPI6+(0<<3)), (_SPI_REG0 *)(REG_BASE_SPI7+(0<<3)), (_SPI_REG0 *)(REG_BASE_SPI8+(0<<3))};
static volatile _SPI_REG1 * const arrSPITX[SPI_CNT] = {(_SPI_REG1 *)(REG_BASE_SPI0+(1<<3)), (_SPI_REG1 *)(REG_BASE_SPI1+(1<<3)), (_SPI_REG1 *)(REG_BASE_SPI2+(1<<3)), (_SPI_REG1 *)(REG_BASE_SPI3+(1<<3)), (_SPI_REG1 *)(REG_BASE_SPI4+(1<<3)), (_SPI_REG1 *)(REG_BASE_SPI5+(1<<3)), (_SPI_REG1 *)(REG_BASE_SPI6+(1<<3)), (_SPI_REG1 *)(REG_BASE_SPI7+(1<<3)), (_SPI_REG1 *)(REG_BASE_SPI8+(1<<3))};
static volatile _SPI_REG2 * const arrSPI[SPI_CNT]   = {(_SPI_REG2 *)(REG_BASE_SPI0+(2<<3)), (_SPI_REG2 *)(REG_BASE_SPI1+(2<<3)), (_SPI_REG2 *)(REG_BASE_SPI2+(2<<3)), (_SPI_REG2 *)(REG_BASE_SPI3+(2<<3)), (_SPI_REG2 *)(REG_BASE_SPI4+(2<<3)), (_SPI_REG2 *)(REG_BASE_SPI5+(2<<3)), (_SPI_REG2 *)(REG_BASE_SPI6+(2<<3)), (_SPI_REG2 *)(REG_BASE_SPI7+(2<<3)), (_SPI_REG2 *)(REG_BASE_SPI8+(2<<3))};
ISRD static tIhnd arrSPIIrq[SPI_CNT];

void SpiInit(UINT nCH, UINT Speed_Hz, BYTE nBIT, BYTE isLSB)
{
	SpiSetWs(nCH, nBIT == 8 ? SPI_WS_8BIT : nBIT == 16 ? SPI_WS_16BIT : nBIT == 24 ? SPI_WS_24BIT : SPI_WS_32BIT);
	SpiSetBitmode(nCH, isLSB == 0 ? SPI_MSBfirst : SPI_LSBfirst);
	SpiSetEn(nCH, ENX_ON);						// SPI controller enabled
	SpiSetCs(nCH, SPI_CS_OUT_HIGH);				// SPI chip select output enable
	SpiSetOnebitmode(nCH, SPI_BYTE_MODE);		// SPI byte mode
	SpiSetClk(nCH, Speed_Hz);
	SpiSetIrqEn(nCH, ENX_OFF);					// Interrupt disabled
	SpiSetClkmode(nCH, SPI_CLKDIR_LOW_POSI);	// Low, Positive Edge clock

	arrSPIIrq[nCH].irqfn = NULL;
	arrSPIIrq[nCH].arg = NULL;

	// pin mux setting
	switch (nCH) {
		case 0:	SPI0_PIN_INIT;	break;
		case 1:	SPI1_PIN_INIT;	break;
		case 2:	SPI2_PIN_INIT;	break;
		case 3:	SPI3_PIN_INIT;	break;
		case 4:	SPI4_PIN_INIT;	break;
		case 5:	SPI5_PIN_INIT;	break;
		case 6:	SPI6_PIN_INIT;	break;
		case 7:	SPI7_PIN_INIT;	break;
		case 8:	SPI8_PIN_INIT;	break;
	}

	printf("SPI%u Init - %uHz\n", nCH, APB_FREQ / ((arrSPI[nCH]->CLK_DIV + 1) * 4));
}

void SpiDeinit(UINT nCH)
{
	SpiSetWs(nCH, SPI_WS_8BIT);
	SpiSetBitmode(nCH, SPI_MSBfirst);
	SpiSetCs(nCH, SPI_CS_HIGH_Z);
	SpiSetClkdiv(nCH, 0xFF);
	SpiSetIrqEn(nCH, ENX_OFF);
	SpiSetOnebitmode(nCH, SPI_BYTE_MODE);
	SpiSetClkmode(nCH, SPI_CLKDIR_LOW_POSI);
	SpiSetEn(nCH, ENX_OFF);

	arrSPIIrq[nCH].irqfn = NULL;
	arrSPIIrq[nCH].arg = NULL;

	// pin mux setting
	switch (nCH) {
		case 0:	SPI0_PIN_DEINIT;	break;
		case 1:	SPI1_PIN_DEINIT;	break;
		case 2:	SPI2_PIN_DEINIT;	break;
		case 3:	SPI3_PIN_DEINIT;	break;
		case 4:	SPI4_PIN_DEINIT;	break;
		case 5:	SPI5_PIN_DEINIT;	break;
		case 6:	SPI6_PIN_DEINIT;	break;
		case 7:	SPI7_PIN_DEINIT;	break;
		case 8:	SPI8_PIN_DEINIT;	break;
	}
}

void SpiSetEn(UINT nCH, ENX_SWITCH sw)
{
	arrSPI[nCH]->EN = sw;
}

ENX_SWITCH SpiGetEn(UINT nCH)
{
	return arrSPI[nCH]->EN;
}

void SpiSetClkdiv(UINT nCH, UINT Clkdiv)
{
	arrSPI[nCH]->CLK_DIV = Clkdiv;
}

UINT SpiGetClkdiv(UINT nCH)
{
	return arrSPI[nCH]->CLK_DIV;
}

void SpiSetClk(UINT nCH, UINT Speed_Hz)
{
	UINT u32Cal = (APB_FREQ / (4 * Speed_Hz)) - 1;
	if (u32Cal > 0xFF) {
		arrSPI[nCH]->CLK_DIV = 0xFF;
		ENX_DEBUGF(DBG_SPI_LOG, "SPI Clk Min.(%u/%u)\n", u32Cal, arrSPI[nCH]->CLK_DIV);
	} else {
		arrSPI[nCH]->CLK_DIV = u32Cal;
	}
}

UINT SpiGetClk(UINT nCH)
{
//	ENX_DEBUGF(DBG_SPI_LOG, "SPI Clk Set %uHz(%u)\n", APB_FREQ / ((arrSPI[nCH]->CLK_DIV + 1) * 4), arrSPI[nCH]->CLK_DIV);
	return APB_FREQ / ((arrSPI[nCH]->CLK_DIV + 1) * 4);
}

void SpiSetBitmode(UINT nCH, SPI_BITMODE bitmode)
{
	arrSPI[nCH]->BIT_MODE = bitmode;
}

SPI_BITMODE SpiGetBitmode(UINT nCH)
{
	return arrSPI[nCH]->BIT_MODE;
}

void SpiSetOnebitmode(UINT nCH, SPI_ONEBITMODE onebitmode)
{
	arrSPI[nCH]->ONE_BITMODE = onebitmode;
}

SPI_ONEBITMODE SpiGetOneBitmode(UINT nCH)
{
	return arrSPI[nCH]->ONE_BITMODE;
}

void SpiSetClkmode(UINT nCH, SPI_CLKDIR_MODE clkmode)
{
	arrSPI[nCH]->CLK_MODE = clkmode;
}

SPI_CLKDIR_MODE SpiGetClkmode(UINT nCH)
{
	return arrSPI[nCH]->CLK_MODE;
}

void SpiSetWs(UINT nCH, SPI_WORD_SIZE wdsize)
{
	arrSPI[nCH]->WS = wdsize;
}

SPI_WORD_SIZE SpiGetWs(UINT nCH)
{
	return arrSPI[nCH]->WS;
}

void SpiSetCs(UINT nCH, SPI_CSMODE cs)
{
	arrSPI[nCH]->CS_MODE = cs;
}

SPI_CSMODE SpiGetCs(UINT nCH)
{
	return arrSPI[nCH]->CS_MODE;
}

void SpiWrite8b(UINT nCH, BYTE *dat)
{
	arrSPITX[nCH]->TX_DAT = ((UINT)(*dat));
	printf("SPI-TX08b: 0x%08X\n", arrSPITX[nCH]->TX_DAT);
	arrSPI[nCH]->RW = 1;
	arrSPI[nCH]->GO = 1;
	while (arrSPI[nCH]->GO);
}

void SpiWrite16b(UINT nCH, BYTE *dat)
{
	arrSPITX[nCH]->TX_DAT = (((UINT)(*(dat+1))<<8)|((UINT)(*dat)));
	printf("SPI-TX16b: 0x%08X\n", arrSPITX[nCH]->TX_DAT);
	arrSPI[nCH]->RW = 1;
	arrSPI[nCH]->GO = 1;
	while (arrSPI[nCH]->GO);
}

void SpiWrite24b(UINT nCH, BYTE *dat)
{
	arrSPITX[nCH]->TX_DAT = (((UINT)(*(dat+2))<<16)|((UINT)(*(dat+1))<<8)|((UINT)(*dat)));
	printf("SPI-TX24b: 0x%08X\n", arrSPITX[nCH]->TX_DAT);
	arrSPI[nCH]->RW = 1;
	arrSPI[nCH]->GO = 1;
	while (arrSPI[nCH]->GO);
}

void SpiWrite32b(UINT nCH, BYTE *dat)
{
	arrSPITX[nCH]->TX_DAT = (((UINT)(*(dat+3))<<24)|((UINT)(*(dat+2))<<16)|((UINT)(*(dat+1))<<8)|((UINT)(*dat)));
	printf("SPI-TX32b: 0x%08X\n", arrSPITX[nCH]->TX_DAT);
	arrSPI[nCH]->RW = 1;
	arrSPI[nCH]->GO = 1;
	while (arrSPI[nCH]->GO);
}






void SpiWrite(UINT nCH, BYTE *dat)
{
	arrSPITX[nCH]->TX_DAT = (arrSPI[nCH]->WS==0)? ((UINT)(*dat)) :
							(arrSPI[nCH]->WS==1)? (((UINT)(*(dat+1))<<8)|((UINT)(*dat))) :
							(arrSPI[nCH]->WS==2)? (((UINT)(*(dat+2))<<16)|((UINT)(*(dat+1))<<8)|((UINT)(*dat))) :
							(arrSPI[nCH]->WS==3)? (((UINT)(*(dat+3))<<24)|((UINT)(*(dat+2))<<16)|((UINT)(*(dat+1))<<8)|((UINT)(*dat))) : arrSPITX[nCH]->TX_DAT;
//	printf("SPI-TX: 0x%08X\n", arrSPITX[nCH]->TX_DAT);
	arrSPI[nCH]->RW = 1;
	arrSPI[nCH]->GO = 1;
	while (arrSPI[nCH]->GO);
}

void SpiRead(UINT nCH, BYTE *dat)
{
	arrSPI[nCH]->RW = 2;
	arrSPI[nCH]->GO = 1;
	while (arrSPI[nCH]->GO);
//	printf("SPI-RX: 0x%08X\n", arrSPIRX[nCH]->RX_DAT);
#if 1
	*dat++ = (arrSPIRX[nCH]->RX_DAT&0x000000ff);
	if (arrSPI[nCH]->WS>0) *dat++ = (arrSPIRX[nCH]->RX_DAT&0x0000ff00)>>8;
	if (arrSPI[nCH]->WS>1) *dat++ = (arrSPIRX[nCH]->RX_DAT&0x00ff0000)>>16;
	if (arrSPI[nCH]->WS>2) *dat++ = (arrSPIRX[nCH]->RX_DAT&0xff000000)>>24;
#else
	*dat++ = (arrSPIRX[nCH]->RX_DAT&0xff000000)>>24;
	if (arrSPI[nCH]->WS>0) *dat++ = (arrSPIRX[nCH]->RX_DAT&0x00ff0000)>>16;
	if (arrSPI[nCH]->WS>1) *dat++ = (arrSPIRX[nCH]->RX_DAT&0x0000ff00)>>8;
	if (arrSPI[nCH]->WS>2) *dat++ = (arrSPIRX[nCH]->RX_DAT&0x000000ff);
#endif
}

void SpiRW(UINT nCH, BYTE *WrDat, BYTE *RdDat)
{
	arrSPITX[nCH]->TX_DAT = (arrSPI[nCH]->WS==0)? ((UINT)(*WrDat)) :
							(arrSPI[nCH]->WS==1)? (((UINT)(*WrDat)<<8)|((UINT)(*(WrDat+1)))) :
							(arrSPI[nCH]->WS==2)? (((UINT)(*WrDat)<<16)|((UINT)(*(WrDat+1))<<8)|((UINT)(*(WrDat+2)))) :
							(arrSPI[nCH]->WS==3)? (((UINT)(*WrDat)<<24)|((UINT)(*(WrDat+1))<<16)|((UINT)(*(WrDat+2))<<8)|((UINT)(*(WrDat+3)))) : arrSPITX[nCH]->TX_DAT;
	arrSPI[nCH]->RW = 3;
	arrSPI[nCH]->GO = 1;
	while (arrSPI[nCH]->GO);
#if 1
	*RdDat++ = (arrSPIRX[nCH]->RX_DAT&0x000000ff);
	if (arrSPI[nCH]->WS>0) *RdDat++ = (arrSPIRX[nCH]->RX_DAT&0x0000ff00)>>8;
	if (arrSPI[nCH]->WS>1) *RdDat++ = (arrSPIRX[nCH]->RX_DAT&0x00ff0000)>>16;
	if (arrSPI[nCH]->WS>2) *RdDat++ = (arrSPIRX[nCH]->RX_DAT&0xff000000)>>24;
#else
	*RdDat++ = (arrSPIRX[nCH]->RX_DAT&0xff000000)>>24;
	if (arrSPI[nCH]->WS>0) *RdDat++ = (arrSPIRX[nCH]->RX_DAT&0x00ff0000)>>16;
	if (arrSPI[nCH]->WS>1) *RdDat++ = (arrSPIRX[nCH]->RX_DAT&0x0000ff00)>>8;
	if (arrSPI[nCH]->WS>2) *RdDat++ = (arrSPIRX[nCH]->RX_DAT&0x000000ff);
#endif
}

void SpiIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrSPIIrq[nCH].irqfn = irqfn;
	arrSPIIrq[nCH].arg = arg;
}

void SpiSetIrqEn(UINT nCH, ENX_SWITCH sw)
{
	arrSPI[nCH]->IRQ_EN = sw;
}

ENX_SWITCH SpiGetIrqEn(UINT nCH)
{
	return arrSPI[nCH]->IRQ_EN;
}

void SpiIrqClear(UINT nCH)
{
	arrSPI[nCH]->IRQ_CLR = 1;
}

UINT SpiIsIrq(UINT nCH)
{
	return arrSPI[nCH]->IRQ;
}

void IrqSpi(UINT nCH)
{
	if (SpiIsIrq(nCH)) {
		if (arrSPIIrq[nCH].irqfn) {
			arrSPIIrq[nCH].irqfn(arrSPIIrq[nCH].arg);
		} else {
			printf("SPI%u IRQ Get\n", nCH);
		}
		SpiIrqClear(nCH);
	}
}
#else
void IrqSpi(UINT nCH)
{
	_Rprintf("SPI%u IRQ Get! SPI%u is inactive.\n", nCH, nCH);
	ENX_ASSERT(0);
}
#endif
