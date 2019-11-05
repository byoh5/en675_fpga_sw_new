#include "dev.h"

#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif

static volatile _DMA_REG0 * const arrBDMA[BDMA_CNT]    = {(_DMA_REG0 *)(REG_BASE_BDMA0+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA1+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA2+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA3+(0<<3))};
static volatile _DMA_REG1 * const arrBDMASRC[BDMA_CNT] = {(_DMA_REG1 *)(REG_BASE_BDMA0+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA1+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA2+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA3+(1<<3))};
static volatile _DMA_REG2 * const arrBDMADST[BDMA_CNT] = {(_DMA_REG2 *)(REG_BASE_BDMA0+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA1+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA2+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA3+(2<<3))};
static volatile _DMA_REG3 * const arrBDMALEN[BDMA_CNT] = {(_DMA_REG3 *)(REG_BASE_BDMA0+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA1+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA2+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA3+(3<<3))};
ISRD static tIhnd arrBDMAIrq[BDMA_CNT];

static volatile _DMA_REG0 * const arrCDMA[CDMA_CNT]    = {(_DMA_REG0 *)(REG_BASE_CDMA0+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA1+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA2+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA3+(0<<3))};
static volatile _DMA_REG1 * const arrCDMASRC[CDMA_CNT] = {(_DMA_REG1 *)(REG_BASE_CDMA0+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA1+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA2+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA3+(1<<3))};
static volatile _DMA_REG2 * const arrCDMADST[CDMA_CNT] = {(_DMA_REG2 *)(REG_BASE_CDMA0+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA1+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA2+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA3+(2<<3))};
static volatile _DMA_REG3 * const arrCDMALEN[CDMA_CNT] = {(_DMA_REG3 *)(REG_BASE_CDMA0+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA1+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA2+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA3+(3<<3))};
ISRD static tIhnd arrCDMAIrq[CDMA_CNT];

void BDmaInit(void)
{
	for (uint64_t i = 0; i < BDMA_CNT; i++) {
		arrBDMAIrq[i].irqfn = NULL;
		arrBDMAIrq[i].arg = NULL;
	}
}

void BDmaMemCpy_isr(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->MODE = 0;
	arrBDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	while (arrBDMA[nCH]->DONE_PTR != arrBDMA[nCH]->JOB_PTR);
}

UINT BDmaMemCpy_isr_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	UINT doneID = arrBDMA[nCH]->JOB_PTR;
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->MODE = 0;
	arrBDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	return doneID;
}

#ifdef __FREERTOS__
void BDmaMemCpy_rtos(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	portENTER_CRITICAL();
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->MODE = 0;
	arrBDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	while (arrBDMA[nCH]->DONE_PTR != arrBDMA[nCH]->JOB_PTR);
	portEXIT_CRITICAL();
}

UINT BDmaMemCpy_rtos_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	portENTER_CRITICAL();
	UINT doneID = arrBDMA[nCH]->JOB_PTR;
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->MODE = 0;
	arrBDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	portEXIT_CRITICAL();
	return doneID;
}

void BDmaMemCpy_rtos_flush(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	portENTER_CRITICAL();
	hwflush_dcache_range((ulong)apbSrc, anNum);
	hwflush_dcache_range((ulong)apbDst, anNum);
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->MODE = 0;
	arrBDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	while (arrBDMA[nCH]->DONE_PTR != arrBDMA[nCH]->JOB_PTR);
	portEXIT_CRITICAL();
}

void BDmaMemCpy_rtos_discard_flush(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	portENTER_CRITICAL();
	hwflush_dcache_range((ulong)apbSrc, anNum);
	hwdiscard_dcache_range((ulong)apbDst, anNum);
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->MODE = 0;
	arrBDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	while (arrBDMA[nCH]->DONE_PTR != arrBDMA[nCH]->JOB_PTR);
	portEXIT_CRITICAL();
}
#endif

void BDmaMemSet_isr(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%02X, %uByte\n", nCH, apbDst, abVal, anNum);
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->VALUE = abVal;
	arrBDMA[nCH]->MODE = 1;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	//asm volatile ("fence iorw,iorw");
	arrBDMA[nCH]->GO = 1;
	while (arrBDMA[nCH]->DONE_PTR != arrBDMA[nCH]->JOB_PTR);
}

UINT BDmaMemSet_isr_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%02X, %uByte\n", nCH, apbDst, abVal, anNum);
	UINT doneID = arrBDMA[nCH]->JOB_PTR;
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->VALUE = abVal;
	arrBDMA[nCH]->MODE = 1;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	return doneID;
}

#ifdef __FREERTOS__
void BDmaMemSet_rtos(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%02X, %uByte\n", nCH, apbDst, abVal, anNum);
	portENTER_CRITICAL();
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->VALUE = abVal;
	arrBDMA[nCH]->MODE = 1;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	while (arrBDMA[nCH]->DONE_PTR != arrBDMA[nCH]->JOB_PTR);
	portEXIT_CRITICAL();
}
#endif

void BDmaIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrBDMAIrq[nCH].irqfn = irqfn;
	arrBDMAIrq[nCH].arg = arg;
}

void BDmaSetIrqEn(UINT nCH, ENX_SWITCH sw)
{	// Interrupt enable/disable
	arrBDMA[nCH]->IRQ_EN = sw;
}

ENX_SWITCH BDmaGetIrqEn(UINT nCH)
{
	return arrBDMA[nCH]->IRQ_EN;
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
//		printf("DMA%d IRQ Get\n", nCH);
		if (arrBDMAIrq[nCH].irqfn) {
			arrBDMAIrq[nCH].irqfn(arrBDMAIrq[nCH].arg);
		}
		BDmaIrqClear(nCH);
		arrBDMA[nCH]->IRQ_CLR = 0;
	}
}

void CDmaInit(void)
{
	for (uint64_t i = 0; i < CDMA_CNT; i++) {
		arrCDMAIrq[i].irqfn = NULL;
		arrCDMAIrq[i].arg = NULL;
	}
}

void CDmaRegshow(UINT nCH)
{
	printf("CDMAreg0: 0x%08X\n", arrCDMA[nCH]->a);
	printf("JOB_PTR: 0x%02X\n", arrCDMA[nCH]->JOB_PTR);
	printf("DONE_PTR: 0x%02X\n", arrCDMA[nCH]->DONE_PTR);
	printf("MODE: 0x%02X\n", arrCDMA[nCH]->MODE);
	printf("SRC: 0x%08X\n", arrCDMASRC[nCH]->SRC);
	printf("DST: 0x%08X\n", arrCDMADST[nCH]->DST);
	printf("LEN: 0x%04X\n", arrCDMALEN[nCH]->LEN);
	printf("GO: 0x%02X\n", arrCDMA[nCH]->GO);
}

void CDmaMemCpy_isr(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	arrCDMA[nCH]->JOB_PTR++;
	arrCDMA[nCH]->MODE = 0;
	arrCDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrCDMADST[nCH]->DST = (intptr_t)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	while (arrCDMA[nCH]->DONE_PTR != arrCDMA[nCH]->JOB_PTR);
}

UINT CDmaMemCpy_isr_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	UINT doneID = arrCDMA[nCH]->JOB_PTR;
	arrCDMA[nCH]->JOB_PTR++;
	arrCDMA[nCH]->MODE = 0;
	arrCDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrCDMADST[nCH]->DST = (intptr_t)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	return doneID;
}

#ifdef __FREERTOS__
void CDmaMemCpy_rtos(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	portENTER_CRITICAL();
	arrCDMA[nCH]->JOB_PTR++;
	arrCDMA[nCH]->MODE = 0;
	arrCDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrCDMADST[nCH]->DST = (intptr_t)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	while (arrCDMA[nCH]->DONE_PTR != arrCDMA[nCH]->JOB_PTR);
	portEXIT_CRITICAL();
}

UINT CDmaMemCpy_rtos_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%08X, %uByte\n", nCH, apbDst, apbSrc, anNum);
	portENTER_CRITICAL();
	UINT doneID = arrCDMA[nCH]->JOB_PTR;
	arrCDMA[nCH]->JOB_PTR++;
	arrCDMA[nCH]->MODE = 0;
	arrCDMASRC[nCH]->SRC = (intptr_t)apbSrc;
	arrCDMADST[nCH]->DST = (intptr_t)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	portEXIT_CRITICAL();
	return doneID;
}
#endif

void CDmaMemSet_isr(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%02X, %uByte\n", nCH, apbDst, abVal, anNum);
	arrCDMA[nCH]->JOB_PTR++;
	arrCDMA[nCH]->VALUE = abVal;
	arrCDMA[nCH]->MODE = 1;
	arrCDMADST[nCH]->DST = (intptr_t)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	while (arrCDMA[nCH]->DONE_PTR != arrCDMA[nCH]->JOB_PTR);
}

UINT CDmaMemSet_isr_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%02X, %uByte\n", nCH, apbDst, abVal, anNum);
	UINT doneID = arrCDMA[nCH]->JOB_PTR;
	arrCDMA[nCH]->JOB_PTR++;
	arrCDMA[nCH]->VALUE = abVal;
	arrCDMA[nCH]->MODE = 1;
	arrCDMADST[nCH]->DST = (intptr_t)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	return doneID;
}

#ifdef __FREERTOS__
void CDmaMemSet_rtos(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%02X, %uByte\n", nCH, apbDst, abVal, anNum);
	portENTER_CRITICAL();
	arrCDMA[nCH]->JOB_PTR++;
	arrCDMA[nCH]->VALUE = abVal;
	arrCDMA[nCH]->MODE = 1;
	arrCDMADST[nCH]->DST = (intptr_t)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	while (arrCDMA[nCH]->DONE_PTR != arrCDMA[nCH]->JOB_PTR);
	portEXIT_CRITICAL();
}

UINT CDmaMemSet_rtos_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%02X, %uByte\n", nCH, apbDst, abVal, anNum);
	portENTER_CRITICAL();
	UINT doneID = arrCDMA[nCH]->JOB_PTR;
	arrCDMA[nCH]->JOB_PTR++;
	arrCDMA[nCH]->VALUE = abVal;
	arrCDMA[nCH]->MODE = 1;
	arrCDMADST[nCH]->DST = (intptr_t)apbDst;
	arrCDMALEN[nCH]->LEN = anNum;
	arrCDMA[nCH]->GO = 1;
	portEXIT_CRITICAL();
	return doneID;
}
#endif

void CDmaIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrCDMAIrq[nCH].irqfn = irqfn;
	arrCDMAIrq[nCH].arg = arg;
}

void CDmaSetIrqEn(UINT nCH, ENX_SWITCH sw)
{	// Interrupt enable/disable
	arrCDMA[nCH]->IRQ_EN = sw;
}

ENX_SWITCH CDmaGetIrqEn(UINT nCH)
{
	return arrCDMA[nCH]->IRQ_EN;
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
//		printf("DMA%d IRQ Get\n", nCH);
		if (arrCDMAIrq[nCH].irqfn) {
			arrCDMAIrq[nCH].irqfn(arrCDMAIrq[nCH].arg);
		}
		CDmaIrqClear(nCH);
	}
}
