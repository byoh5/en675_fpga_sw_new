#include "dev.h"
#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif

#if 1
ISRD static tIhnd arrSHAShotIrq;
#if EN675_SINGLE
ISRD static tIhnd arrSHAChopIrq;

void ShaGetDigest(BYTE *data, SHAmode mode)
{
	UINT *arrRes = (UINT *)data;
#if 1
	arrRes[0] = ENX_HTONL(SHA_DIGEST_255_224);
	arrRes[1] = ENX_HTONL(SHA_DIGEST_223_192);
	arrRes[2] = ENX_HTONL(SHA_DIGEST_191_160);
	arrRes[3] = ENX_HTONL(SHA_DIGEST_159_128);
	arrRes[4] = ENX_HTONL(SHA_DIGEST_127_96);
	arrRes[5] = ENX_HTONL(SHA_DIGEST_95_64);
	arrRes[6] = ENX_HTONL(SHA_DIGEST_63_32);
	if (mode == eSHA256) {
		arrRes[7] = ENX_HTONL(SHA_DIGEST_31_0);
	}
#else
	// Test 필요. 성능? size?
	// -O2기준 0x1aeByte(위) -> 0x152Byte(아래)
	UINT k;
	k = SHA_DIGEST_255_224;
	arrRes[0] = ENX_HTONL(k);
	k = SHA_DIGEST_223_192;
	arrRes[1] = ENX_HTONL(k);
	k = SHA_DIGEST_191_160;
	arrRes[2] = ENX_HTONL(k);
	k = SHA_DIGEST_159_128;
	arrRes[3] = ENX_HTONL(k);
	k = SHA_DIGEST_127_96;
	arrRes[4] = ENX_HTONL(k);
	k = SHA_DIGEST_95_64;
	arrRes[5] = ENX_HTONL(k);
	k = SHA_DIGEST_63_32;
	arrRes[6] = ENX_HTONL(k);
	if (mode == eSHA256) {
		k = SHA_DIGEST_31_0;
		arrRes[7] = ENX_HTONL(k);
	}
#endif
}

inline void ShaChopCalc_isr_async(SHAChopitem *item, UINT len)
{
	SHA_SRC = (intptr_t)item->base;
	switch (item->cmd) {
	case eSHACmdInit:
		SHA_LEN_CUR = len;
		item->tot_len = len;
		break;
	case eSHACmdUpdate:
		SHA_LEN_CUR = len;
		item->tot_len += len;
		break;
	case eSHACmdFinal:
		SHA_LEN_CUR = len;
		item->tot_len += len;
		SHA_LEN_TOT = item->tot_len;
		break;
	}
	SHA_OP = 1;
	SHA_MODE = item->mode;
	SHA_CMD = item->cmd;
	SHA_REQ = 1;
}

inline void ShaChopCalc_isr(SHAChopitem *item, UINT len)
{
	ShaChopCalc_isr_async(item, len);
	while(SHA_REQ);
}

#ifdef __FREERTOS__
void ShaChopCalc_rtos_async(SHAChopitem *item, UINT len)
{
	portENTER_CRITICAL();
	ShaChopCalc_isr_async(item, len);
	portEXIT_CRITICAL();
}

void ShaChopCalc_rtos(SHAChopitem *item, UINT len)
{
	portENTER_CRITICAL();
	ShaChopCalc_isr(item, len);
	portEXIT_CRITICAL();
}
#endif

void ShaChopIrqCallback(irq_fn irqfn, void *arg)
{
	arrSHAChopIrq.irqfn = irqfn;
	arrSHAChopIrq.arg = arg;
}

void ShaSetChopIrqEn(ENX_SWITCH sw)
{
	SHA_CHOP_IRQ_EN = sw;
}

ENX_SWITCH ShaGetChopIrqEn(void)
{
	return SHA_CHOP_IRQ_EN;
}

void ShaChopIrqClear(void)
{
	SHA_CHOP_IRQ_CLR = 1;
}

UINT ShaIsChopIrq(void)
{
	return SHA_CHOP_IRQ;
}

void IrqShaChop(void)
{
	if (ShaIsChopIrq()) {
		if (arrSHAChopIrq.irqfn) {
			arrSHAChopIrq.irqfn(arrSHAChopIrq.arg);
		} else {
			printf("SHA CHOP IRQ Get\n");
		}
		ShaChopIrqClear();
	}
}

inline void ShaShotCalc_isr_async(BYTE *apbSrc, UINT len, SHAmode mode)
{
	SHA_SRC = (intptr_t)apbSrc;
	SHA_LEN_CUR = len;
	SHA_MODE = mode;
	SHA_OP = 0;
	SHA_REQ = 1;
}

inline void ShaShotCalc_isr(BYTE *apbSrc, UINT len, SHAmode mode)
{
	ShaShotCalc_isr_async(apbSrc, len, mode);
	while(SHA_REQ);
}

#ifdef __FREERTOS__
void ShaShotCalc_rtos_async(BYTE *apbSrc, UINT len, SHAmode mode)
{
	portENTER_CRITICAL();
	ShaShotCalc_isr_async(apbSrc, len, mode);
	portEXIT_CRITICAL();
}

void ShaShotCalc_rtos(BYTE *apbSrc, UINT len, SHAmode mode)
{
	portENTER_CRITICAL();
	ShaShotCalc_isr(apbSrc, len, mode);
	portEXIT_CRITICAL();
}
#endif

void ShaShotIrqCallback(irq_fn irqfn, void *arg)
{
	arrSHAShotIrq.irqfn = irqfn;
	arrSHAShotIrq.arg = arg;
}

void ShaSetShotIrqEn(ENX_SWITCH sw)
{
	SHA_SHOT_IRQ_EN = sw;
}

ENX_SWITCH ShaGetShotIrqEn(void)
{
	return SHA_SHOT_IRQ_EN;
}

void ShaShotIrqClear(void)
{
	SHA_SHOT_IRQ_CLR = 1;
}

UINT ShaIsShotIrq(void)
{
	return SHA_SHOT_IRQ;
}

void IrqShaOneshot(void)
{
	if (ShaIsShotIrq()) {
		if (arrSHAShotIrq.irqfn) {
			arrSHAShotIrq.irqfn(arrSHAShotIrq.arg);
		} else {
			printf("SHA SHOT IRQ Get\n");
		}
		ShaShotIrqClear();
	}
}

void ShaRegShow(ENX_YN isDetail)
{
	_Cprintf("SHA Register View\n");
	printf("========================================\n");
	if (isDetail == ENX_YES) {
		_Yprintf(" 0:0x%08X\n", _cm(REG_BASE_SHA, 0));
		printf("   %-20s: 0x%08X\n", "SRC", SHA_SRC);
		_Yprintf(" 1:0x%08X\n", _cm(REG_BASE_SHA, 1));
		printf("   %-20s: 0x%08X, %u\n", "LEN_TOT", SHA_LEN_TOT, SHA_LEN_TOT);
		_Yprintf(" 2:0x%08X\n", _cm(REG_BASE_SHA, 2));
		printf("   %-20s: 0x%08X, %u\n", "LEN_CUR", SHA_LEN_CUR, SHA_LEN_CUR);
		_Yprintf(" 3:0x%08X\n", _cm(REG_BASE_SHA, 3));
		printf("   %-20s: %u\n", "CHOP_BUSY", SHA_CHOP_BUSY);
		printf("   %-20s: %u\n", "SHOT_BUSY", SHA_SHOT_BUSY);
		printf("   %-20s: %u\n", "CHOP_IRQ", SHA_CHOP_IRQ);
		printf("   %-20s: %u\n", "CHOP_IRQ_EN", SHA_CHOP_IRQ_EN);
		printf("   %-20s: %u\n", "CHOP_IRQ_CLR", SHA_CHOP_IRQ_CLR);
		printf("   %-20s: %u\n", "SHOT_IRQ", SHA_SHOT_IRQ);
		printf("   %-20s: %u\n", "SHOT_IRQ_EN", SHA_SHOT_IRQ_EN);
		printf("   %-20s: %u\n", "SHOT_IRQ_CLR", SHA_SHOT_IRQ_CLR);
		printf("   %-20s: %u, %s\n", "CMD", SHA_CMD, SHA_CMD == 0 ? "Initial" : SHA_CMD == 1 ? "Intermediate" : SHA_CMD == 2 ? "Final" : "error");
		printf("   %-20s: %u, %s\n", "OP", SHA_OP, SHA_OP == 0 ? "One-Shot" : SHA_OP == 1 ? "Chop" : "error");
		printf("   %-20s: %u, %s\n", "MODE", SHA_MODE, SHA_MODE == 0 ? "SHA224" : SHA_MODE == 1 ? "SHA256" : "error");
		printf("   %-20s: %u\n", "REQ", SHA_REQ);
		_Yprintf(" 4:0x%08X\n", _cm(REG_BASE_SHA, 4));
		printf("   %-20s: 0x%08X\n", "DIGEST_255_224", SHA_DIGEST_255_224);
		_Yprintf(" 5:0x%08X\n", _cm(REG_BASE_SHA, 5));
		printf("   %-20s: 0x%08X\n", "DIGEST_223_192", SHA_DIGEST_223_192);
		_Yprintf(" 6:0x%08X\n", _cm(REG_BASE_SHA, 6));
		printf("   %-20s: 0x%08X\n", "DIGEST_191_160", SHA_DIGEST_191_160);
		_Yprintf(" 7:0x%08X\n", _cm(REG_BASE_SHA, 7));
		printf("   %-20s: 0x%08X\n", "DIGEST_159_128", SHA_DIGEST_159_128);
		_Yprintf(" 8:0x%08X\n", _cm(REG_BASE_SHA, 8));
		printf("   %-20s: 0x%08X\n", "DIGEST_127_96", SHA_DIGEST_127_96);
		_Yprintf(" 9:0x%08X\n", _cm(REG_BASE_SHA, 9));
		printf("   %-20s: 0x%08X\n", "DIGEST_95_64", SHA_DIGEST_95_64);
		_Yprintf("10:0x%08X\n", _cm(REG_BASE_SHA, 10));
		printf("   %-20s: 0x%08X\n", "DIGEST_63_32", SHA_DIGEST_63_32);
		_Yprintf("11:0x%08X\n", _cm(REG_BASE_SHA, 11));
		printf("   %-20s: 0x%08X\n", "DIGEST_31_0", SHA_DIGEST_31_0);
	} else {
		printf("SHA  0:0x%08X  1:0x%08X  2:0x%08X  3:0x%08X  4:0x%08X\n", _cm(REG_BASE_SFLS, 0), _cm(REG_BASE_SFLS, 1), _cm(REG_BASE_SFLS, 2), _cm(REG_BASE_SFLS, 3), _cm(REG_BASE_SFLS, 4));
		printf("     5:0x%08X  6:0x%08X  7:0x%08X  8:0x%08X  9:0x%08X\n", _cm(REG_BASE_SFLS, 5), _cm(REG_BASE_SFLS, 6), _cm(REG_BASE_SFLS, 7), _cm(REG_BASE_SFLS, 8), _cm(REG_BASE_SFLS, 9));
		printf("    10:0x%08X 11:0x%08X\n", _cm(REG_BASE_SFLS, 10), _cm(REG_BASE_SFLS, 11));
	}
	printf("========================================\n");
}
#else
void ShaGetDigest(BYTE *data, SHAmode mode)
{
	UINT *arrRes = (UINT *)data;
#if 1
	arrRes[0] = ENX_HTONL(SHA_DIGEST_255_224);
	arrRes[1] = ENX_HTONL(SHA_DIGEST_223_192);
	arrRes[2] = ENX_HTONL(SHA_DIGEST_191_160);
	arrRes[3] = ENX_HTONL(SHA_DIGEST_159_128);
	arrRes[4] = ENX_HTONL(SHA_DIGEST_127_96);
	arrRes[5] = ENX_HTONL(SHA_DIGEST_95_64);
	arrRes[6] = ENX_HTONL(SHA_DIGEST_63_32);
	if (mode == eSHA256) {
		arrRes[7] = ENX_HTONL(SHA_DIGEST_31_0);
	}
#else
	// Test 필요. 성능? size?
	// -O2기준 0x1aeByte(위) -> 0x152Byte(아래)
	UINT k;
	k = SHA_DIGEST_255_224;
	arrRes[0] = ENX_HTONL(k);
	k = SHA_DIGEST_223_192;
	arrRes[1] = ENX_HTONL(k);
	k = SHA_DIGEST_191_160;
	arrRes[2] = ENX_HTONL(k);
	k = SHA_DIGEST_159_128;
	arrRes[3] = ENX_HTONL(k);
	k = SHA_DIGEST_127_96;
	arrRes[4] = ENX_HTONL(k);
	k = SHA_DIGEST_95_64;
	arrRes[5] = ENX_HTONL(k);
	k = SHA_DIGEST_63_32;
	arrRes[6] = ENX_HTONL(k);
	if (mode == eSHA256) {
		k = SHA_DIGEST_31_0;
		arrRes[7] = ENX_HTONL(k);
	}
#endif
}

inline void ShaShotCalc_isr_async(BYTE *apbSrc, UINT len, SHAmode mode)
{
	SHA_SRC = (intptr_t)apbSrc;
	SHA_LEN = len;
	SHA_MODE = mode;
	SHA_REQ = 1;
}

inline void ShaShotCalc_isr(BYTE *apbSrc, UINT len, SHAmode mode)
{
	ShaShotCalc_isr_async(apbSrc, len, mode);
	while(SHA_REQ);
}

#ifdef __FREERTOS__
void ShaShotCalc_rtos_async(BYTE *apbSrc, UINT len, SHAmode mode)
{
	portENTER_CRITICAL();
	ShaShotCalc_isr_async(apbSrc, len, mode);
	portEXIT_CRITICAL();
}

void ShaShotCalc_rtos(BYTE *apbSrc, UINT len, SHAmode mode)
{
	portENTER_CRITICAL();
	ShaShotCalc_isr(apbSrc, len, mode);
	portEXIT_CRITICAL();
}
#endif

void ShaShotIrqCallback(irq_fn irqfn, void *arg)
{
	arrSHAShotIrq.irqfn = irqfn;
	arrSHAShotIrq.arg = arg;
}

void ShaSetShotIrqEn(ENX_SWITCH sw)
{
	SHA_IRQ_EN = sw;
}

ENX_SWITCH ShaGetShotIrqEn(void)
{
	return SHA_IRQ_EN;
}

void ShaShotIrqClear(void)
{
	SHA_IRQ_CLR = 1;
}

UINT ShaIsShotIrq(void)
{
	return SHA_IRQ;
}

void IrqSha(void)
{
	if (ShaIsShotIrq()) {
		if (arrSHAShotIrq.irqfn) {
			arrSHAShotIrq.irqfn(arrSHAShotIrq.arg);
		} else {
			printf("SHA IRQ Get\n");
		}
		ShaShotIrqClear();
	}
}

void ShaRegShow(ENX_YN isDetail)
{
	_Cprintf("SHA Register View\n");
	printf("========================================\n");
	if (isDetail == ENX_YES) {
		_Yprintf(" 0:0x%08X\n", _cm(REG_BASE_SHA, 0));
		printf("   %-20s: 0x%08X\n", "SRC", SHA_SRC);
		_Yprintf(" 2:0x%08X\n", _cm(REG_BASE_SHA, 2));
		printf("   %-20s: 0x%08X, %u\n", "LEN_CUR", SHA_LEN, SHA_LEN);
		_Yprintf(" 3:0x%08X\n", _cm(REG_BASE_SHA, 3));
		printf("   %-20s: %u\n", "IRQ", SHA_IRQ);
		printf("   %-20s: %u\n", "IRQ_EN", SHA_IRQ_EN);
		printf("   %-20s: %u\n", "IRQ_CLR", SHA_IRQ_CLR);
		printf("   %-20s: %u, %s\n", "MODE", SHA_MODE, SHA_MODE == 0 ? "SHA224" : SHA_MODE == 1 ? "SHA256" : "error");
		printf("   %-20s: %u\n", "REQ", SHA_REQ);
		_Yprintf(" 4:0x%08X\n", _cm(REG_BASE_SHA, 4));
		printf("   %-20s: 0x%08X\n", "DIGEST_255_224", SHA_DIGEST_255_224);
		_Yprintf(" 5:0x%08X\n", _cm(REG_BASE_SHA, 5));
		printf("   %-20s: 0x%08X\n", "DIGEST_223_192", SHA_DIGEST_223_192);
		_Yprintf(" 6:0x%08X\n", _cm(REG_BASE_SHA, 6));
		printf("   %-20s: 0x%08X\n", "DIGEST_191_160", SHA_DIGEST_191_160);
		_Yprintf(" 7:0x%08X\n", _cm(REG_BASE_SHA, 7));
		printf("   %-20s: 0x%08X\n", "DIGEST_159_128", SHA_DIGEST_159_128);
		_Yprintf(" 8:0x%08X\n", _cm(REG_BASE_SHA, 8));
		printf("   %-20s: 0x%08X\n", "DIGEST_127_96", SHA_DIGEST_127_96);
		_Yprintf(" 9:0x%08X\n", _cm(REG_BASE_SHA, 9));
		printf("   %-20s: 0x%08X\n", "DIGEST_95_64", SHA_DIGEST_95_64);
		_Yprintf("10:0x%08X\n", _cm(REG_BASE_SHA, 10));
		printf("   %-20s: 0x%08X\n", "DIGEST_63_32", SHA_DIGEST_63_32);
		_Yprintf("11:0x%08X\n", _cm(REG_BASE_SHA, 11));
		printf("   %-20s: 0x%08X\n", "DIGEST_31_0", SHA_DIGEST_31_0);
	} else {
		printf("SHA  0:0x%08X  1:0x%08X  2:0x%08X  3:0x%08X  4:0x%08X\n", _cm(REG_BASE_SHA, 0), 0, _cm(REG_BASE_SHA, 2), _cm(REG_BASE_SHA, 3), _cm(REG_BASE_SHA, 4));
		printf("     5:0x%08X  6:0x%08X  7:0x%08X  8:0x%08X  9:0x%08X\n", _cm(REG_BASE_SHA, 5), _cm(REG_BASE_SHA, 6), _cm(REG_BASE_SHA, 7), _cm(REG_BASE_SHA, 8), _cm(REG_BASE_SHA, 9));
		printf("    10:0x%08X 11:0x%08X\n", _cm(REG_BASE_SHA, 10), _cm(REG_BASE_SHA, 11));
	}
	printf("========================================\n");
}
#endif
#else
void IrqSha(void)
{
	_Rprintf("SHA IRQ Get! SHA is inactive.\n");
	ENX_ASSERT(0);
}
#endif
