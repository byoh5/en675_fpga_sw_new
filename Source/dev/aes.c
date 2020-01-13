#include "dev.h"
#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif

#if 1

ISRD static tIhnd arrAESIrq;

void AesInit(void)
{
	arrAESIrq.irqfn = NULL;
	arrAESIrq.arg = NULL;
}

void AesSetKey(BYTE *key, UINT size)
{
	AES_KEY_255_224 = key[ 0] << 24 | key[ 1] << 16 | key[ 2] << 8 | key[ 3];
	AES_KEY_223_192 = key[ 4] << 24 | key[ 5] << 16 | key[ 6] << 8 | key[ 7];
	AES_KEY_191_160 = key[ 8] << 24 | key[ 9] << 16 | key[10] << 8 | key[11];
	AES_KEY_159_128 = key[12] << 24 | key[13] << 16 | key[14] << 8 | key[15];
	if (size == 32) {
		AES_KEY_127_96  = key[16] << 24 | key[17] << 16 | key[18] << 8 | key[19];
		AES_KEY_95_64   = key[20] << 24 | key[21] << 16 | key[22] << 8 | key[23];
		AES_KEY_63_32   = key[24] << 24 | key[25] << 16 | key[26] << 8 | key[27];
		AES_KEY_31_0    = key[28] << 24 | key[29] << 16 | key[30] << 8 | key[31];
	}
}

void AesSetIv(BYTE *iv)
{
	AES_IV_127_96  = iv[ 0] << 24 | iv[ 1] << 16 | iv[ 2] << 8 | iv[ 3];
	AES_IV_95_64   = iv[ 4] << 24 | iv[ 5] << 16 | iv[ 6] << 8 | iv[ 7];
	AES_IV_63_32   = iv[ 8] << 24 | iv[ 9] << 16 | iv[10] << 8 | iv[11];
	AES_IV_31_0    = iv[12] << 24 | iv[13] << 16 | iv[14] << 8 | iv[15];
}

inline void AesCalc_isr_async(BYTE *apbDst, BYTE *apbSrc, UINT len, AESmode mode)
{
	AES_SRC = (intptr_t)apbSrc;
	AES_DST = (intptr_t)apbDst;
	AES_LEN = len;
	AES_MODE = mode & 0x1;
	AES_CBC_EN = (mode >> 1) & 0x1;
	AES_KEY_LEN = (mode >> 2) & 0x1;
	AES_REQ = 1;
}

inline void AesCalc_isr(BYTE *apbDst, BYTE *apbSrc, UINT len, AESmode mode)
{
	AesCalc_isr_async(apbDst, apbSrc, len, mode);
	while(AES_REQ);
}

#ifdef __FREERTOS__
void AesCalc_rtos_async(BYTE *apbDst, BYTE *apbSrc, UINT len, AESmode mode)
{
	portENTER_CRITICAL();
	AesCalc_isr_async(apbDst, apbSrc, len, mode);
	portEXIT_CRITICAL();
}

void AesCalc_rtos(BYTE *apbDst, BYTE *apbSrc, UINT len, AESmode mode)
{
	portENTER_CRITICAL();
	AesCalc_isr(apbDst, apbSrc, len, mode);
	portEXIT_CRITICAL();
}
#endif

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
		if (arrAESIrq.irqfn) {
			arrAESIrq.irqfn(arrAESIrq.arg);
		} else {
			printf("AES IRQ Get\n");
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

#if 0
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
#endif
