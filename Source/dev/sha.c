#include "dev.h"

void Sha224(BYTE *apbSrc, UINT len)
{
	SHA_SRC = (UINT)apbSrc;
	SHA_LEN = len;
	SHA_IRQ_EN = 1;
	SHA_MODE = 0;
	SHA_REQ = 1;
	while(SHA_REQ);
}

void Sha256(BYTE *apbSrc, UINT len)
{
	SHA_SRC = (UINT)apbSrc;
	SHA_LEN = len;
	SHA_IRQ_EN = 1;
	SHA_MODE = 1;
	SHA_REQ = 1;
	while(SHA_REQ);
}
