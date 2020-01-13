/*
 * boot.c
 *
 *  Created on: 2018. 11. 29.
 *      Author: BYOH
 */
#include <stdint.h>

#include "dev.h"

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

const unsigned int _ispm_text_s;
const unsigned int _ispm_text_e;
const unsigned int _ispm_text_lma_s;

void _init_text_section(void)
{
//	printf("ISPM size: %ubyte\n", (size_t)(_ispm_text_e - _ispm_text_s));
//	printf("vma start: %x\n", _ispm_text_s);
//	printf("lma start: %x\n", _ispm_text_lma_s);
	if ((size_t)(_ispm_text_e - _ispm_text_s) != 0) {
		memcpy((void*)(intptr_t)_ispm_text_s, (void*)(intptr_t)_ispm_text_lma_s, (size_t)(_ispm_text_e - _ispm_text_s));
	}
}

const unsigned int _ispm_data_s;
const unsigned int _ispm_data_e;
const unsigned int _ispm_data_lma_s;

void _init_data_section(void)
{
//	printf("ISPM size: %ubyte\n", (size_t)(_ispm_data_e - _ispm_data_s));
//	printf("vma start: %x\n", _ispm_data_s);
//	printf("lma start: %x\n", _ispm_data_lma_s);
	if ((size_t)(_ispm_data_e - _ispm_data_s) != 0) {
		memcpy((void*)(intptr_t)_ispm_data_s, (void*)(intptr_t)_ispm_data_lma_s, (size_t)(_ispm_data_e - _ispm_data_s));
	}
}

void _init_ddr_area(void)
{
	char *pDst = (char*)(intptr_t)(_bss_e);
	if( pDst == 0) return;
	int n = 0x80800000 - _bss_e;
	int i;
	for(i=0; i<n; i++) *pDst++ = 0;
}
