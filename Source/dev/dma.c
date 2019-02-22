#include "dev.h"

static _DMA_REG0 *arrDMA[DMA_CNT];
static _DMA_REG1 *arrDMASRC[DMA_CNT];
static _DMA_REG2 *arrDMADST[DMA_CNT];
static _DMA_REG3 *arrDMALEN[DMA_CNT];
static tIhnd arrDMAIrq[DMA_CNT];

void DmaInit(void)
{
	const uint64_t addrDMA[DMA_CNT] = {REG_BASE_DMA0, REG_BASE_DMA1};

	for (uint64_t i = 0; i < DMA_CNT; i++) {
		arrDMA[i] = (_DMA_REG0 *)(addrDMA[i] + (0 << 3));
		arrDMASRC[i] = (_DMA_REG1 *)(addrDMA[i] + (1 << 3));
		arrDMADST[i] = (_DMA_REG2 *)(addrDMA[i] + (2 << 3));
		arrDMALEN[i] = (_DMA_REG3 *)(addrDMA[i] + (3 << 3));
		arrDMAIrq[i].irqfn = NULL;
		arrDMAIrq[i].arg = NULL;
	}
}

void DmaMemCpy_isr(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	while(arrDMA[nCH]->GO);
	arrDMA[nCH]->MODE = 0;
	arrDMASRC[nCH]->SRC = (UINT)apbSrc;
	arrDMADST[nCH]->DST = (UINT)apbDst;
	arrDMALEN[nCH]->LEN = anNum;
	arrDMA[nCH]->GO = 1;
	while(arrDMA[nCH]->GO);
}

void DmaMemCpy_isr_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	while(arrDMA[nCH]->GO);
	arrDMA[nCH]->MODE = 0;
	arrDMASRC[nCH]->SRC = (UINT)apbSrc;
	arrDMADST[nCH]->DST = (UINT)apbDst;
	arrDMALEN[nCH]->LEN = anNum;
	arrDMA[nCH]->GO = 1;
}

void DmaMemSet_isr(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	while(arrDMA[nCH]->GO);
	arrDMA[nCH]->VALUE = abVal;
	arrDMA[nCH]->MODE = 1;
	arrDMADST[nCH]->DST = (UINT)apbDst;
	arrDMALEN[nCH]->LEN = anNum;
	arrDMA[nCH]->GO = 1;
	while(arrDMA[nCH]->GO);
}

void DmaMemSet_isr_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	while(arrDMA[nCH]->GO);
	arrDMA[nCH]->VALUE = abVal;
	arrDMA[nCH]->MODE = 1;
	arrDMADST[nCH]->DST = (UINT)apbDst;
	arrDMALEN[nCH]->LEN = anNum;
	arrDMA[nCH]->GO = 1;
}

void DmaIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrDMAIrq[nCH].irqfn = irqfn;
	arrDMAIrq[nCH].arg = arg;
}

void DmaIrqOn(UINT nCH)
{	// Interrupt enable
	arrDMA[nCH]->IRQ_EN = 1;
}

void DmaIrqOff(UINT nCH)
{	// Interrupt disable
	arrDMA[nCH]->IRQ_EN = 0;
}

void DmaIrqClear(UINT nCH)
{
	arrDMA[nCH]->IRQ_CLR = 1;
}

UINT DmaIsIrq(UINT nCH)
{
	return arrDMA[nCH]->IRQ;
}

void IrqDma(void)
{
	for (UINT nCH = 0; nCH < DMA_CNT; nCH++) {
		if (DmaIsIrq(nCH)) {
			_printf("Dma IRQ Get [%d]\n", nCH);
			if (arrDMAIrq[nCH].irqfn) {
				arrDMAIrq[nCH].irqfn(arrDMAIrq[nCH].arg);
			}
			DmaIrqClear(nCH);
		}
	}
}
