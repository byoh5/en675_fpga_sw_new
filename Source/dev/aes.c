#include "dev.h"

#if 1

ISRD static tIhnd arrAESIrq;

void AesInit(void)
{
	AES_KEY_255_224 = 0x12345678;
	AES_KEY_223_192 = 0x12345678;
	AES_KEY_191_160 = 0x12345678;
	AES_KEY_159_128 = 0x12345678;
	AES_KEY_127_96  = 0x12345678;
	AES_KEY_95_64   = 0x12345678;
	AES_KEY_63_32   = 0x12345678;
	AES_KEY_31_0    = 0x12345678;
	AES_CBC_EN      = 1;
}

void Aes128Enc(BYTE *apbDst, BYTE *apbSrc, UINT len)
{
	AES_SRC = (intptr_t)apbSrc;
	AES_DST = (intptr_t)apbDst;
	AES_LEN = len;
	AES_IRQ_EN = 1;
	AES_KEY_LEN = 0;
	AES_MODE = 1;
	AES_REQ = 1;
	while(AES_REQ);
}

void Aes128Dec(BYTE *apbDst, BYTE *apbSrc, UINT len)
{
	AES_SRC = (intptr_t)apbSrc;
	AES_DST = (intptr_t)apbDst;
	AES_LEN = len;
	AES_IRQ_EN = 1;
	AES_KEY_LEN = 0;
	AES_MODE = 0;
	AES_REQ = 1;
	while(AES_REQ);
}

void Aes256Enc(BYTE *apbDst, BYTE *apbSrc, UINT len)
{
	AES_SRC = (intptr_t)apbSrc;
	AES_DST = (intptr_t)apbDst;
	AES_LEN = len;
	AES_IRQ_EN = 1;
	AES_KEY_LEN = 1;
	AES_MODE = 1;
	AES_REQ = 1;
	while(AES_REQ);
}

void Aes256Dec(BYTE *apbDst, BYTE *apbSrc, UINT len)
{
	AES_SRC = (intptr_t)apbSrc;
	AES_DST = (intptr_t)apbDst;
	AES_LEN = len;
	AES_IRQ_EN = 1;
	AES_KEY_LEN = 1;
	AES_MODE = 0;
	AES_REQ = 1;
	while(AES_REQ);
}

void AesIrqCallback(irq_fn irqfn, void *arg)
{
	arrAESIrq.irqfn = irqfn;
	arrAESIrq.arg = arg;
}

void AesSetIrqEn(ENX_SWITCH sw)
{
	AES_IRQ_EN = sw;
}

ENX_SWITCH AesGetIrqEn(void)
{
	return AES_IRQ_EN;
}

void AesIrqClear(void)
{
	AES_IRQ_CLR = 1;
}

UINT AesIsIrq(void)
{
	return AES_IRQ;
}

void IrqAes(void)
{
	if (AesIsIrq()) {
		printf("AES IRQ Get\n");
		if (arrAESIrq.irqfn) {
			arrAESIrq.irqfn(arrAESIrq.arg);
		}
		AesIrqClear();
	}
}
#else
void IrqAes(UINT nCH)
{
	_Rprintf("AES IRQ Get! AES is inactive.\n");
	ENX_ASSERT(0);
}
#endif
