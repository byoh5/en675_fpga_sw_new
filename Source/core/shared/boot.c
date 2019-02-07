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

void _init_ddr_area(void)
{
	char *pDst = (char*)(intptr_t)(_bss_e);
	if( pDst == 0) return;
	int n = 0x80800000 - _bss_e;
	int i;
	for(i=0; i<n; i++) *pDst++ = 0;
}

#include "encoding.h"
extern volatile uint64_t* mtime;
extern volatile uint64_t* timecmp;
void trap_from_machine_mode(void)
{
	unsigned long mcause = read_csr(mcause);

	if ((mcause&0x8000000000000000) != 0x0) {
		volatile unsigned int * pAdr_prio =  (unsigned int *)0xc000004;
		volatile unsigned int * pAdr_mie =   (unsigned int *)0xc002000;
		volatile unsigned int * pAdr_sie =   (unsigned int *)0xc002080;
		volatile unsigned int * pAdr_mthre = (unsigned int *)0xc200000;
		volatile unsigned int * pAdr_sthre = (unsigned int *)0xc201000;
		volatile unsigned int * pAdr_pend =  (unsigned int *)0xc001000;
		volatile unsigned int * pAdr_claim =  (unsigned int *)0xc200004;

		volatile unsigned int source;

		_printf("IRQ.1 %x\n", mcause);
		if (mcause & 0x7) {
		    if (mtime && timecmp) {
		    	//*HLS()->timecmp = *mtime + (1000000) ; // / configTICK_RATE_HZ);
		    	//*timecmp = *mtime + (1000000);
		        //*timecmp1 = *mtime + (1000000);
		    }
		}

		//_printf("isr cause = %08lx\n", mcause);
		//_printf("isr plic pend = %08lx\n", *pAdr_pend);
extern void IsrUart(void);
		IsrUart();

		//vPortSysTickHandler();

		//TIMER_CMP_INT();

		source = *pAdr_claim;
		//  printm("isr plic claim = %08lx\n", source);
		*pAdr_claim = source;
	}
	else {
		_printf("IRQ.0 %x\n", mcause);
	}
}
