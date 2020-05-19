#include "dev.h"

#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif

#if EN675_SINGLE
static volatile _DMA_REG0 * const arrBDMA[BDMA_CNT]    = {(_DMA_REG0 *)(REG_BASE_BDMA0+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA1+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA2+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA3+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA4+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA5+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA6+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA7+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA8+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA9+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA10+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA11+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA12+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA13+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA14+(0<<3)), (_DMA_REG0 *)(REG_BASE_BDMA15+(0<<3))};
static volatile _DMA_REG1 * const arrBDMASRC[BDMA_CNT] = {(_DMA_REG1 *)(REG_BASE_BDMA0+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA1+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA2+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA3+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA4+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA5+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA6+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA7+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA8+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA9+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA10+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA11+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA12+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA13+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA14+(1<<3)), (_DMA_REG1 *)(REG_BASE_BDMA15+(1<<3))};
static volatile _DMA_REG2 * const arrBDMADST[BDMA_CNT] = {(_DMA_REG2 *)(REG_BASE_BDMA0+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA1+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA2+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA3+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA4+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA5+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA6+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA7+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA8+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA9+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA10+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA11+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA12+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA13+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA14+(2<<3)), (_DMA_REG2 *)(REG_BASE_BDMA15+(2<<3))};
static volatile _DMA_REG3 * const arrBDMALEN[BDMA_CNT] = {(_DMA_REG3 *)(REG_BASE_BDMA0+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA1+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA2+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA3+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA4+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA5+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA6+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA7+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA8+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA9+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA10+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA11+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA12+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA13+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA14+(3<<3)), (_DMA_REG3 *)(REG_BASE_BDMA15+(3<<3))};
ISRD static tIhnd arrBDMAIrq[BDMA_CNT];

static volatile _DMA_REG0 * const arrCDMA[CDMA_CNT]    = {(_DMA_REG0 *)(REG_BASE_CDMA0+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA1+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA2+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA3+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA4+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA5+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA6+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA7+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA8+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA9+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA10+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA11+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA12+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA13+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA14+(0<<3)), (_DMA_REG0 *)(REG_BASE_CDMA15+(0<<3))};
static volatile _DMA_REG1 * const arrCDMASRC[CDMA_CNT] = {(_DMA_REG1 *)(REG_BASE_CDMA0+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA1+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA2+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA3+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA4+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA5+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA6+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA7+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA8+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA9+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA10+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA11+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA12+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA13+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA14+(1<<3)), (_DMA_REG1 *)(REG_BASE_CDMA15+(1<<3))};
static volatile _DMA_REG2 * const arrCDMADST[CDMA_CNT] = {(_DMA_REG2 *)(REG_BASE_CDMA0+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA1+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA2+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA3+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA4+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA5+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA6+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA7+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA8+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA9+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA10+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA11+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA12+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA13+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA14+(2<<3)), (_DMA_REG2 *)(REG_BASE_CDMA15+(2<<3))};
static volatile _DMA_REG3 * const arrCDMALEN[CDMA_CNT] = {(_DMA_REG3 *)(REG_BASE_CDMA0+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA1+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA2+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA3+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA4+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA5+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA6+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA7+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA8+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA9+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA10+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA11+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA12+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA13+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA14+(3<<3)), (_DMA_REG3 *)(REG_BASE_CDMA15+(3<<3))};
ISRD static tIhnd arrCDMAIrq[CDMA_CNT];
#else
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
#endif

UINT BDmaRegGetControl(UINT nCH)
{
	return arrBDMA[nCH]->a;
}

#if EN675_SINGLE
UINT BDmaRegGetFull(UINT nCH)
{
	return arrBDMA[nCH]->FULL;
}
#endif

UINT BDmaRegGetDoneVal(UINT nCH)
{
	return arrBDMA[nCH]->DONE_VAL;
}

UINT BDmaRegGetDonePtr(UINT nCH)
{
	return arrBDMA[nCH]->DONE_PTR;
}

UINT BDmaRegGetJobPtr(UINT nCH)
{
	return arrBDMA[nCH]->JOB_PTR;
}

UINT BDmaRegGetIrq(UINT nCH)
{
	return arrBDMA[nCH]->IRQ;
}

UINT BDmaRegGetIrqEn(UINT nCH)
{
	return arrBDMA[nCH]->IRQ_EN;
}

UINT BDmaRegGetIrqClr(UINT nCH)
{
	return arrBDMA[nCH]->IRQ_CLR;
}

UINT BDmaRegGetValue(UINT nCH)
{
	return arrBDMA[nCH]->VALUE;
}

UINT BDmaRegGetMode(UINT nCH)
{
	return arrBDMA[nCH]->MODE;
}

UINT BDmaRegGetGo(UINT nCH)
{
	return arrBDMA[nCH]->GO;
}

UINT BDmaRegGetSrc(UINT nCH)
{
	return arrBDMASRC[nCH]->SRC;
}

UINT BDmaRegGetDst(UINT nCH)
{
	return arrBDMADST[nCH]->DST;
}

UINT BDmaRegGetLen(UINT nCH)
{
	return arrBDMALEN[nCH]->LEN;
}

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

UINT BDmaMemSet_rtos_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum)
{
	ENX_DEBUGF(DGB_DMA_MSG, "%u, 0x%08X <- 0x%02X, %uByte\n", nCH, apbDst, abVal, anNum);
	portENTER_CRITICAL();
	UINT doneID = arrBDMA[nCH]->JOB_PTR;
	arrBDMA[nCH]->JOB_PTR++;
	arrBDMA[nCH]->VALUE = abVal;
	arrBDMA[nCH]->MODE = 1;
	arrBDMADST[nCH]->DST = (intptr_t)apbDst;
	arrBDMALEN[nCH]->LEN = anNum;
	arrBDMA[nCH]->GO = 1;
	portEXIT_CRITICAL();
	return doneID;
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
	if (arrBDMA[nCH]->IRQ_CLR == 1) {
		printf("BDMA irq clear == 1?\n");
	}
}

UINT BDmaIsIrq(UINT nCH)
{
	return arrBDMA[nCH]->IRQ;
}

void IrqBDma(UINT nCH)
{
	if (BDmaIsIrq(nCH)) {
		if (arrBDMAIrq[nCH].irqfn) {
			arrBDMAIrq[nCH].irqfn(arrBDMAIrq[nCH].arg);
		} else {
			printf("DMA%d IRQ Get\n", nCH);
		}
		BDmaIrqClear(nCH);
	}
}

UINT CDmaRegGetControl(UINT nCH)
{
	return arrCDMA[nCH]->a;
}

#if EN675_SINGLE
UINT CDmaRegGetFull(UINT nCH)
{
	return arrCDMA[nCH]->FULL;
}
#endif

UINT CDmaRegGetDoneVal(UINT nCH)
{
	return arrCDMA[nCH]->DONE_VAL;
}

UINT CDmaRegGetDonePtr(UINT nCH)
{
	return arrCDMA[nCH]->DONE_PTR;
}

UINT CDmaRegGetJobPtr(UINT nCH)
{
	return arrCDMA[nCH]->JOB_PTR;
}

UINT CDmaRegGetIrq(UINT nCH)
{
	return arrCDMA[nCH]->IRQ;
}

UINT CDmaRegGetIrqEn(UINT nCH)
{
	return arrCDMA[nCH]->IRQ_EN;
}

UINT CDmaRegGetIrqClr(UINT nCH)
{
	return arrCDMA[nCH]->IRQ_CLR;
}

UINT CDmaRegGetValue(UINT nCH)
{
	return arrCDMA[nCH]->VALUE;
}

UINT CDmaRegGetMode(UINT nCH)
{
	return arrCDMA[nCH]->MODE;
}

UINT CDmaRegGetGo(UINT nCH)
{
	return arrCDMA[nCH]->GO;
}

UINT CDmaRegGetSrc(UINT nCH)
{
	return arrCDMASRC[nCH]->SRC;
}

UINT CDmaRegGetDst(UINT nCH)
{
	return arrCDMADST[nCH]->DST;
}

UINT CDmaRegGetLen(UINT nCH)
{
	return arrCDMALEN[nCH]->LEN;
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
