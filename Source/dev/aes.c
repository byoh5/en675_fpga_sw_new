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

void AesRegShow(ENX_YN isDetail)
{
	_Cprintf("AES Register View\n");
	printf("========================================\n");
	if (isDetail == ENX_YES) {
		_Yprintf(" 0:0x%08X\n", _cm(REG_BASE_AES, 0));
		printf("   %-20s: 0x%08X\n", "SRC", AES_SRC);
		_Yprintf(" 1:0x%08X\n", _cm(REG_BASE_AES, 1));
		printf("   %-20s: 0x%08X\n", "DST", AES_DST);
		_Yprintf(" 2:0x%08X\n", _cm(REG_BASE_AES, 2));
		printf("   %-20s: 0x%08X, %u\n", "LEN", AES_LEN, AES_LEN);
		_Yprintf(" 3:0x%08X\n", _cm(REG_BASE_AES, 3));
		printf("   %-20s: %u\n", "IRQ", AES_IRQ);
		printf("   %-20s: %u\n", "IRQ_EN", AES_IRQ_EN);
		printf("   %-20s: %u\n", "IRQ_CLR", AES_IRQ_CLR);
		printf("   %-20s: %u, %s\n", "KEY_LEN", AES_KEY_LEN, AES_KEY_LEN == 0 ? "128bit" : AES_KEY_LEN == 1 ? "256bit" : "error");
		printf("   %-20s: %u, %s\n", "CBC_EN", AES_CBC_EN, AES_CBC_EN == 0 ? "ECB" : AES_CBC_EN == 1 ? "CBC" : "error");
		printf("   %-20s: %u, %s\n", "MODE", AES_MODE, AES_MODE == 0 ? "Decrypt" : AES_MODE == 1 ? "Encrypt" : "error");
		printf("   %-20s: %u\n", "REQ", AES_REQ);
		_Yprintf(" 4:0x%08X\n", _cm(REG_BASE_AES, 4));
		printf("   %-20s: 0x%08X\n", "KEY_255_224", AES_KEY_255_224);
		_Yprintf(" 5:0x%08X\n", _cm(REG_BASE_AES, 5));
		printf("   %-20s: 0x%08X\n", "KEY_223_192", AES_KEY_223_192);
		_Yprintf(" 6:0x%08X\n", _cm(REG_BASE_AES, 6));
		printf("   %-20s: 0x%08X\n", "KEY_191_160", AES_KEY_191_160);
		_Yprintf(" 7:0x%08X\n", _cm(REG_BASE_AES, 7));
		printf("   %-20s: 0x%08X\n", "KEY_159_128", AES_KEY_159_128);
		_Yprintf(" 8:0x%08X\n", _cm(REG_BASE_AES, 8));
		printf("   %-20s: 0x%08X\n", "KEY_127_96", AES_KEY_127_96);
		_Yprintf(" 9:0x%08X\n", _cm(REG_BASE_AES, 9));
		printf("   %-20s: 0x%08X\n", "KEY_95_64", AES_KEY_95_64);
		_Yprintf("10:0x%08X\n", _cm(REG_BASE_AES, 10));
		printf("   %-20s: 0x%08X\n", "KEY_63_32", AES_KEY_63_32);
		_Yprintf("11:0x%08X\n", _cm(REG_BASE_AES, 11));
		printf("   %-20s: 0x%08X\n", "KEY_31_0", AES_KEY_31_0);
		_Yprintf("12:0x%08X\n", _cm(REG_BASE_AES, 12));
		printf("   %-20s: 0x%08X\n", "IV_127_96", AES_IV_127_96);
		_Yprintf("13:0x%08X\n", _cm(REG_BASE_AES, 13));
		printf("   %-20s: 0x%08X\n", "IV_95_64", AES_IV_95_64);
		_Yprintf("14:0x%08X\n", _cm(REG_BASE_AES, 14));
		printf("   %-20s: 0x%08X\n", "IV_63_32", AES_IV_63_32);
		_Yprintf("15:0x%08X\n", _cm(REG_BASE_AES, 15));
		printf("   %-20s: 0x%08X\n", "IV_31_0", AES_IV_31_0);
	} else {
		printf("AES  0:0x%08X  1:0x%08X  2:0x%08X  3:0x%08X  4:0x%08X\n", _cm(REG_BASE_AES, 0), _cm(REG_BASE_AES, 1), _cm(REG_BASE_AES, 2), _cm(REG_BASE_AES, 3), _cm(REG_BASE_AES, 4));
		printf("     5:0x%08X  6:0x%08X  7:0x%08X  8:0x%08X  9:0x%08X\n", _cm(REG_BASE_AES, 5), _cm(REG_BASE_AES, 6), _cm(REG_BASE_AES, 7), _cm(REG_BASE_AES, 8), _cm(REG_BASE_AES, 9));
		printf("    10:0x%08X 11:0x%08X 12:0x%08X 13:0x%08X 14:0x%08X\n", _cm(REG_BASE_AES, 10), _cm(REG_BASE_AES, 11), _cm(REG_BASE_AES, 12), _cm(REG_BASE_AES, 13), _cm(REG_BASE_AES, 14));
		printf("    15:0x%08X\n", _cm(REG_BASE_AES, 15));
	}
	printf("========================================\n");
}
