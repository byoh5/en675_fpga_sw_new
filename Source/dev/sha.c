#include "dev.h"

#if 1

ISRD static tIhnd arrSHAIrq;

void Sha224(BYTE *apbSrc, UINT len)
{
	SHA_SRC = (intptr_t)apbSrc;
	SHA_LEN = len;
	SHA_IRQ_EN = 1;
	SHA_MODE = 0;
	SHA_REQ = 1;
	while(SHA_REQ);
}

void Sha256(BYTE *apbSrc, UINT len)
{
	SHA_SRC = (intptr_t)apbSrc;
	SHA_LEN = len;
	SHA_IRQ_EN = 1;
	SHA_MODE = 1;
	SHA_REQ = 1;
	while(SHA_REQ);
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
		printf("SHA IRQ Get\n");
		if (arrSHAIrq.irqfn) {
			arrSHAIrq.irqfn(arrSHAIrq.arg);
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
