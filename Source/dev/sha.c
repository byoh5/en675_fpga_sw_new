#include "dev.h"
#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif

#if 1
ISRD static tIhnd arrSHAIrq;

inline void ShaCalc_isr_async(BYTE *apbSrc, UINT len, SHAmode mode)
{
	SHA_SRC = (intptr_t)apbSrc;
	SHA_LEN = len;
	SHA_MODE = mode;
	SHA_REQ = 1;
}

inline void ShaCalc_isr(BYTE *apbSrc, UINT len, SHAmode mode)
{
	ShaCalc_isr_async(apbSrc, len, mode);
	while(SHA_REQ);
}

#ifdef __FREERTOS__
void ShaCalc_rtos_async(BYTE *apbSrc, UINT len, SHAmode mode)
{
	portENTER_CRITICAL();
	ShaCalc_isr_async(apbSrc, len, mode);
	portEXIT_CRITICAL();
}

void ShaCalc_rtos(BYTE *apbSrc, UINT len, SHAmode mode)
{
	portENTER_CRITICAL();
	ShaCalc_isr(apbSrc, len, mode);
	portEXIT_CRITICAL();
}
#endif

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
	// Test가 필요합니다. 아직 테스트 해보지 못했습니다.
	// -O2 기준, 기존  0x1aebyte에서 0x152byte으로 코드 크기가 줄어듭니다.
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

void ShaIrqCallback(irq_fn irqfn, void *arg)
{
	arrSHAIrq.irqfn = irqfn;
	arrSHAIrq.arg = arg;
}

void ShaSetIrqEn(ENX_SWITCH sw)
{
	SHA_IRQ_EN = sw;
}

ENX_SWITCH ShaGetIrqEn(void)
{
	return SHA_IRQ_EN;
}

void ShaIrqClear(void)
{
	SHA_IRQ_CLR = 1;
}

UINT ShaIsIrq(void)
{
	return SHA_IRQ;
}

void IrqSha(void)
{
	if (ShaIsIrq()) {
		if (arrSHAIrq.irqfn) {
			arrSHAIrq.irqfn(arrSHAIrq.arg);
		} else {
			printf("SHA IRQ Get\n");
		}
		ShaIrqClear();
	}
}
#else
void IrqSha(UINT nCH)
{
	_Rprintf("SHA IRQ Get! SHA is inactive.\n");
	ENX_ASSERT(0);
}
#endif
