/*
 * boot.c
 *
 *  Created on: 2018. 11. 29.
 *      Author: BYOH
 */
#include <stdint.h>

const unsigned int _bss_s;
const unsigned int _bss_e;

void _init_bss_section(void)
{
	char *pDst = (char*)(intptr_t)(_bss_s);
	if( pDst == 0) return;
	int n = _bss_e - _bss_s;
	int i;
	for(i=0; i<n; i++) *pDst++ = 0;

	return;
}

