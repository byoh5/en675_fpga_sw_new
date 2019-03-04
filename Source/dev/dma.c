#include "dev.h"

static _DMA_REG0 *arrBDMA[BDMA_CNT];
static _DMA_REG1 *arrBDMASRC[BDMA_CNT];
static _DMA_REG2 *arrBDMADST[BDMA_CNT];
static _DMA_REG3 *arrBDMALEN[BDMA_CNT];
static tIhnd arrBDMAIrq[BDMA_CNT];

void BDmaInit(void)
{
	const uint64_t addrDMA[BDMA_CNT] = {REG_BASE_BDMA0, REG_BASE_BDMA1, REG_BASE_BDMA2, REG_BASE_BDMA3};

	for (uint64_t i = 0; i < BDMA_CNT; i++) {
		arrBDMA[i] = (_DMA_REG0 *)(addrDMA[i] + (0 << 3));
		arrBDMASRC[i] = (_DMA_REG1 *)(addrDMA[i] + (1 << 3));
		arrBDMADST[i] = (_DMA_REG2 *)(addrDMA[i] + (2 << 3));
		arrBDMALEN[i] = (_DMA_REG3 *)(addrDMA[i] + (3 << 3));
		arrBDMAIrq[i].irqfn = NULL;
		arrBDMAIrq[i].arg = NULL;
	}
}

void BDmaMemCpy_isr(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	arrBDMA[nCH]->JOB_PTR = 1;
	arrBDMA[nCH]->MODE = 0;
	arrBDMASRC[nCH]->SRC = (UINT)apbSrc;
	arrBDMADST[nCH]->DST = (UINT)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	while (arrBDMA[nCH]->DONE_PTR!=1 | !arrBDMA[nCH]->DONE_VAL);
}

void BDmaMemCpy_isr_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	arrBDMA[nCH]->JOB_PTR = 1;
	arrBDMA[nCH]->MODE = 0;
	arrBDMASRC[nCH]->SRC = (UINT)apbSrc;
	arrBDMADST[nCH]->DST = (UINT)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
}

void BDmaMemSet_isr(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	arrBDMA[nCH]->JOB_PTR = 2;
	arrBDMA[nCH]->VALUE = abVal;
	arrBDMA[nCH]->MODE = 1;
	arrBDMADST[nCH]->DST = (UINT)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	while (arrBDMA[nCH]->DONE_PTR!=2 | !arrBDMA[nCH]->DONE_VAL);
}

void BDmaMemSet_isr_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	arrBDMA[nCH]->JOB_PTR = 2;
	arrBDMA[nCH]->VALUE = abVal;
	arrBDMA[nCH]->MODE = 1;
	arrBDMADST[nCH]->DST = (UINT)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
}

void BDmaIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrBDMAIrq[nCH].irqfn = irqfn;
	arrBDMAIrq[nCH].arg = arg;
}

void BDmaIrqOn(UINT nCH)
{	// Interrupt enable
	arrBDMA[nCH]->IRQ_EN = 1;
}

void BDmaIrqOff(UINT nCH)
{	// Interrupt disable
	arrBDMA[nCH]->IRQ_EN = 0;
}

void BDmaIrqClear(UINT nCH)
{
	arrBDMA[nCH]->IRQ_CLR = 1;
}

UINT BDmaIsIrq(UINT nCH)
{
	return arrBDMA[nCH]->IRQ;
}

void IrqBDma(UINT nCH)
{
	if (BDmaIsIrq(nCH)) {
		_printf("DMA%d IRQ Get\n", nCH);
		if (arrBDMAIrq[nCH].irqfn) {
			arrBDMAIrq[nCH].irqfn(arrBDMAIrq[nCH].arg);
		}
		BDmaIrqClear(nCH);
	}
}

static _DMA_REG0 *arrCDMA[CDMA_CNT];
static _DMA_REG1 *arrCDMASRC[CDMA_CNT];
static _DMA_REG2 *arrCDMADST[CDMA_CNT];
static _DMA_REG3 *arrCDMALEN[CDMA_CNT];
static tIhnd arrCDMAIrq[CDMA_CNT];

void CDmaInit(void)
{
	const uint64_t addrDMA[CDMA_CNT] = {REG_BASE_CDMA0, REG_BASE_CDMA1, REG_BASE_CDMA2, REG_BASE_CDMA3};

	for (uint64_t i = 0; i < CDMA_CNT; i++) {
		arrCDMA[i] = (_DMA_REG0 *)(addrDMA[i] + (0 << 3));
		arrCDMASRC[i] = (_DMA_REG1 *)(addrDMA[i] + (1 << 3));
		arrCDMADST[i] = (_DMA_REG2 *)(addrDMA[i] + (2 << 3));
		arrCDMALEN[i] = (_DMA_REG3 *)(addrDMA[i] + (3 << 3));
		arrCDMAIrq[i].irqfn = NULL;
		arrCDMAIrq[i].arg = NULL;
	}
}

void CDmaMemCpy_isr(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	arrCDMA[nCH]->JOB_PTR = 1;
	arrCDMA[nCH]->MODE = 0;
	arrCDMASRC[nCH]->SRC = (UINT)apbSrc;
	arrCDMADST[nCH]->DST = (UINT)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	while (arrCDMA[nCH]->DONE_PTR!=1 | !arrCDMA[nCH]->DONE_VAL);
}

void CDmaMemCpy_isr_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	arrCDMA[nCH]->JOB_PTR = 1;
	arrCDMA[nCH]->MODE = 0;
	arrCDMASRC[nCH]->SRC = (UINT)apbSrc;
	arrCDMADST[nCH]->DST = (UINT)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
}

void CDmaMemSet_isr(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	arrCDMA[nCH]->JOB_PTR = 2;
	arrCDMA[nCH]->VALUE = abVal;
	arrCDMA[nCH]->MODE = 1;
	arrCDMADST[nCH]->DST = (UINT)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	while (arrCDMA[nCH]->DONE_PTR!=2 | !arrCDMA[nCH]->DONE_VAL);
}

void CDmaMemSet_isr_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	arrCDMA[nCH]->JOB_PTR = 2;
	arrCDMA[nCH]->VALUE = abVal;
	arrCDMA[nCH]->MODE = 1;
	arrCDMADST[nCH]->DST = (UINT)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
}

void CDmaIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrCDMAIrq[nCH].irqfn = irqfn;
	arrCDMAIrq[nCH].arg = arg;
}

void CDmaIrqOn(UINT nCH)
{	// Interrupt enable
	arrCDMA[nCH]->IRQ_EN = 1;
}

void CDmaIrqOff(UINT nCH)
{	// Interrupt disable
	arrCDMA[nCH]->IRQ_EN = 0;
}

void CDmaIrqClear(UINT nCH)
{
	arrCDMA[nCH]->IRQ_CLR = 1;
}

UINT CDmaIsIrq(UINT nCH)
{
	return arrCDMA[nCH]->IRQ;
}

void IrqCDma(UINT nCH)
{
	if (CDmaIsIrq(nCH)) {
		_printf("DMA%d IRQ Get\n", nCH);
		if (arrCDMAIrq[nCH].irqfn) {
			arrCDMAIrq[nCH].irqfn(arrCDMAIrq[nCH].arg);
		}
		CDmaIrqClear(nCH);
	}
}
