#include "dev.h"

static _SPI_REG0 *arrSPIRX[SPI_CNT];
static _SPI_REG1 *arrSPITX[SPI_CNT];
static _SPI_REG2 *arrSPI[SPI_CNT];
static tIhnd arrSPIIrq[SPI_CNT];

void SpiInit(UINT nCH, UINT Speed_Hz, UINT WordSize, UINT BitDirection)
{
	const uint64_t addrSPI[UART_CNT] = {
			REG_BASE_SPI0, REG_BASE_SPI1, REG_BASE_SPI2, REG_BASE_SPI3, REG_BASE_SPI4,
			REG_BASE_SPI5, REG_BASE_SPI6, REG_BASE_SPI7, REG_BASE_SPI8
	};

	arrSPIRX[nCH] = (_SPI_REG0 *)(addrSPI[nCH] + (0 << 3));
	arrSPITX[nCH] = (_SPI_REG1 *)(addrSPI[nCH] + (1 << 3));
	arrSPI[nCH] = (_SPI_REG2 *)(addrSPI[nCH] + (2 << 3));

	arrSPI[nCH]->EN = 1;				// SPI controller enabled
	arrSPI[nCH]->WS = WordSize;			// SPI word size / 0:8bit, 1:16bit, 2:24bit, 3:32bit
	arrSPI[nCH]->CS_OEN = 0;			// SPI chip select output enable
	arrSPI[nCH]->CS_OUT = 1;			// SPI chip select output High
	arrSPI[nCH]->ONE_BITMODE = BitDirection;// SPI bit direction / 0:MSB First, 1:LSB First
	arrSPI[nCH]->CLK_DIV = (MCK_FREQ / (4 * Speed_Hz) - 1);
	arrSPI[nCH]->IRQ_EN = 0;			// Interrupt Disabled
	// L(Low) H(High) Pec(Positive Edge clock) Nec(Negative Edge clock)
	arrSPI[nCH]->CLK_MODE = 0;			// 0:L+Pec, 1:L+Nec, 2:H+Pec, 3:H+Nec

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

	printf("SPI%u Init - %uKHz\n", nCH, MCK_FREQ / ((arrSPI[nCH]->CLK_DIV + 1) * 4) / 1000);
}

void SpiDeinit(UINT nCH)
{
	arrSPI[nCH]->WS          = 0;
	arrSPI[nCH]->CS_OEN      = 1;
	arrSPI[nCH]->CS_OUT      = 1;
	arrSPI[nCH]->ONE_BITMODE = 0;
	arrSPI[nCH]->CLK_DIV     = 0;
	arrSPI[nCH]->IRQ_EN      = 0;
	arrSPI[nCH]->CLK_MODE    = 0;
	arrSPI[nCH]->EN          = 0;

	arrSPIIrq[nCH].irqfn = NULL;
	arrSPIIrq[nCH].arg = NULL;

	arrSPIRX[nCH] = NULL;
	arrSPITX[nCH] = NULL;
	arrSPI[nCH] = NULL;

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

void SpiCsLo(UINT nCH)
{
	arrSPI[nCH]->CS_OUT = 0;
}

void SpiCsHi(UINT nCH)
{
	arrSPI[nCH]->CS_OUT = 1;
}

void SpiWrite(UINT nCH, BYTE *dat)
{
	arrSPITX[nCH]->TX_DAT = (arrSPI[nCH]->WS==0)? ((UINT)(*dat)) :
							(arrSPI[nCH]->WS==1)? (((UINT)(*dat)<<8)|((UINT)(*(dat+1)))) :
							(arrSPI[nCH]->WS==2)? (((UINT)(*dat)<<16)|((UINT)(*(dat+1))<<8)|((UINT)(*(dat+2)))) :
							(arrSPI[nCH]->WS==3)? (((UINT)(*dat)<<24)|((UINT)(*(dat+1))<<16)|((UINT)(*(dat+2))<<8)|((UINT)(*(dat+3)))) : arrSPITX[nCH]->TX_DAT;
	arrSPI[nCH]->RW = 1;
	arrSPI[nCH]->GO = 1;
	while (arrSPI[nCH]->GO);
}

void SpiRead(UINT nCH, BYTE *dat)
{
	arrSPI[nCH]->RW = 2;
	arrSPI[nCH]->GO = 1;
	while (arrSPI[nCH]->GO);
	*dat++ = (arrSPIRX[nCH]->RX_DAT&0xff000000)>>24;
	if (arrSPI[nCH]->WS>0) *dat++ = (arrSPIRX[nCH]->RX_DAT&0x00ff0000)>>16;
	if (arrSPI[nCH]->WS>1) *dat++ = (arrSPIRX[nCH]->RX_DAT&0x0000ff00)>>8;
	if (arrSPI[nCH]->WS>2) *dat++ = (arrSPIRX[nCH]->RX_DAT&0x000000ff);
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
	*RdDat++ = (arrSPIRX[nCH]->RX_DAT&0xff000000)>>24;
	if (arrSPI[nCH]->WS>0) *RdDat++ = (arrSPIRX[nCH]->RX_DAT&0x00ff0000)>>16;
	if (arrSPI[nCH]->WS>1) *RdDat++ = (arrSPIRX[nCH]->RX_DAT&0x0000ff00)>>8;
	if (arrSPI[nCH]->WS>2) *RdDat++ = (arrSPIRX[nCH]->RX_DAT&0x000000ff);
}

void SpiIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrSPIIrq[nCH].irqfn = irqfn;
	arrSPIIrq[nCH].arg = arg;
}

void SpiIrqOn(UINT nCH)
{
	arrSPI[nCH]->IRQ_EN = 1;
}

void SpiIrqOff(UINT nCH)
{
	arrSPI[nCH]->IRQ_EN = 0;
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
		_printf("SPI%d IRQ Get\n", nCH);
		if (arrSPIIrq[nCH].irqfn) {
			arrSPIIrq[nCH].irqfn(arrSPIIrq[nCH].arg);
		}
		SpiIrqClear(nCH);
	}
}



