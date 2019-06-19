#include "dev.h"
#include <string.h>

int IPtoStr(unsigned int addr, char* buf, int buflen, int type)
{
	unsigned int s_addr = addr;
	char inv[3];
	char *rp = buf;
	unsigned char *ap = (unsigned char *)&s_addr;
	unsigned char rem, n, i, t;
	int len = 0;

	for(n = 0; n < 4; n++)
	{
		i = 0;
		do {
			rem = *ap % (unsigned char)10;
			*ap /= (unsigned char)10;
			inv[i++] = '0' + rem;
		} while(*ap);
		if(type & 1)
		{
			t = 3 - i;
			while(t--)
			{
				if (len++ >= buflen)
				{
					return (rp-buf);
				}
				*rp++ = '0';
			}
		}
		while(i--)
		{
			if (len++ >= buflen)
			{
				return (rp-buf);
			}
			*rp++ = inv[i];
		}
		if (len++ >= buflen)
		{
			return (rp-buf);
		}
		*rp++ = '.';
		ap++;
	}
	*--rp = 0;
	return (rp-buf);
}

char* enx_strcat(char* dest, char* src)
{
	while (*dest) dest++;
	while ((*dest++ = *src++));
	return --dest;
}

BYTE* enx_memstr(BYTE* src, int srcSize, BYTE* search, int searchSize)
{
	BYTE *p;
	if (src != NULL && search != NULL) {
		for (p = src; p <= (src+srcSize-searchSize); p++) {
			if (memcmp(p, search, searchSize) == 0) {
				return p + searchSize;
			}
		}
	}
	return NULL;
}

void Hex2Str32B(unsigned char* digest, char* rchar)
{
	int i, j;
	unsigned char ctemp;

	for(j=0,i=0;i<16;i++)
	{
		ctemp = (digest[i] & 0xf0) >> 4;
		if(ctemp <= 0x9)		rchar[j] = ctemp + 0x30;
		else if(ctemp <= 0xF)	rchar[j] = (ctemp - 0xA) + 0x61;
		else					rchar[j] = 0x30;
		j++;

		ctemp = (digest[i] & 0x0f);
		if(ctemp <= 0x9)		rchar[j] = ctemp + 0x30;
		else if(ctemp <= 0xF)	rchar[j] = (ctemp - 0xA) + 0x61;
		else					rchar[j] = 0x30;
		j++;
	}
	rchar[32] = 0;
}

void Dec2Str64B(char *str, ULONG val)
{
	if (str == NULL) {
		return;
	}

	int first = 0, idx = 0, k;
	for (int i = 63; i >= 0; i--) {
		k = val >> i;

		if (k & 1) {
			first = 1;
			str[idx] = '1';
		} else {
			if (first == 0) {
				continue;
			}
			str[idx] = '0';
		}
		idx++;
	}
	str[idx] = '\0';
}
