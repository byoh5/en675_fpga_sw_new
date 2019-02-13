#include "dev.h"

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
}

void Aes128Enc(BYTE *apbDst, BYTE *apbSrc, UINT len)
{
	AES_SRC = (UINT)apbSrc;
	AES_DST = (UINT)apbDst;
	AES_LEN = len;
	AES_IRQ_EN = 1;
	AES_KEY_LEN = 0;
	AES_MODE = 1;
	AES_REQ = 1;
	while(AES_REQ);
}

void Aes128Dec(BYTE *apbDst, BYTE *apbSrc, UINT len)
{
	AES_SRC = (UINT)apbSrc;
	AES_DST = (UINT)apbDst;
	AES_LEN = len;
	AES_IRQ_EN = 1;
	AES_KEY_LEN = 0;
	AES_MODE = 0;
	AES_REQ = 1;
	while(AES_REQ);
}

void Aes256Enc(BYTE *apbDst, BYTE *apbSrc, UINT len)
{
	AES_SRC = (UINT)apbSrc;
	AES_DST = (UINT)apbDst;
	AES_LEN = len;
	AES_IRQ_EN = 1;
	AES_KEY_LEN = 1;
	AES_MODE = 1;
	AES_REQ = 1;
	while(AES_REQ);
}

void Aes256Dec(BYTE *apbDst, BYTE *apbSrc, UINT len)
{
	AES_SRC = (UINT)apbSrc;
	AES_DST = (UINT)apbDst;
	AES_LEN = len;
	AES_IRQ_EN = 1;
	AES_KEY_LEN = 1;
	AES_MODE = 0;
	AES_REQ = 1;
	while(AES_REQ);
}
