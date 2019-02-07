/*
 * uart.c
 *
 *  Created on: 2018. 11. 28.
 *      Author: BYOH
 */
//#include "uart.h"
#include "bits.h"
#include "encoding.h"
#include "print.h"

#define UART_INT_ENABLE


void Uart1_Init(){
	volatile unsigned int* ptr = (unsigned int*)0x44a00000;
	*ptr =(unsigned int)0x01a02048;
}

void Uart1_Tx(char data){
	volatile unsigned int* ptr = (unsigned int*)0x44a00010;
	volatile unsigned int* pst = (unsigned int*)0x44a00000;
	while(*pst & 0x20);
	*ptr = (data<<8);
}

int Uart1_Rx(){
	volatile unsigned int* ptr = (unsigned int*)0x44a00008;
	volatile unsigned int* pst = (unsigned int*)0x44a00000;
	if((*pst & 0x02)) return -1;
	else return (int)(*ptr & 0xff);
}

char Uart1_get(){
	volatile unsigned int* ptr = (unsigned int*)0x44a00008;
	return (char)*ptr;
}

char Uart1_empty(){
	volatile unsigned int* pst = (unsigned int*)0x44a00000;
	return (*pst & 0x02);
}

void Uart1_clear(){
	volatile unsigned int* ptr = (unsigned int*)0x44a00000;
	*ptr =(unsigned int)0x01a0204e;
}

#ifdef UART_INT_ENABLE
void Uart1_int_init(void){

  extern char _payload_start, _payload_end;

  volatile unsigned int * pAdr_prio1 = (unsigned int *)0xc000004;
  volatile unsigned int * pAdr_prio2 = (unsigned int *)0xc000008;
  volatile unsigned int * pAdr_mie   = (unsigned int *)0xc002000;
  volatile unsigned int * pAdr_sie   = (unsigned int *)0xc002080;
  volatile unsigned int * pAdr_mthre = (unsigned int *)0xc200000;
  volatile unsigned int * pAdr_sthre = (unsigned int *)0xc201000;
  volatile unsigned int * pAdr_pend  = (unsigned int *)0xc001000;


  *pAdr_prio1 = 1;
  //*pAdr_prio2 = 1;
  *pAdr_mie = 0x2;
  //*pAdr_sie = 0xffffffff;
  *pAdr_mthre = 0x0;
 // *pAdr_sthre = 0x0;

  // Enable External Interrupts
  write_csr(mie, 0x800);

  // Enable Global Interrupts
  unsigned long mstatus = read_csr(mstatus);
  mstatus = INSERT_FIELD(mstatus, MSTATUS_MIE, 1);
  mstatus = INSERT_FIELD(mstatus, MSTATUS_SIE, 1);
  //printm("mstatus = %08lx\n", mstatus);
  write_csr(mstatus, mstatus);

  //printm("plic_m_ie = %08lx\n", *pAdr_mie);
  //printm("plic_s_ie = %08lx\n", *pAdr_sie);
  //printm("plic_prio_sourc1 = %08lx\n", *pAdr_prio1);
  //printm("plic_prio_sourc2 = %08lx\n", *pAdr_prio2);
  //printm("plic m_thre = %08lx\n", *pAdr_mthre);
  //printm("plic s_thre = %08lx\n", *pAdr_sthre);
  //printm("plic pend = %08lx\n", *pAdr_pend);
  unsigned int temp;

}


void IsrUart(void)
{
	char bBuf;

	while(!Uart1_empty()) {
		bBuf = Uart1_get();

		if(bBuf == '\r') _printf("\r\n");
			else {
			_printf("%c",bBuf);
		}


  }
  Uart1_clear();
}


void trap_from_machine_mode()
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



 // printm("isr cause = %08lx\n", mcause);
 // printm("isr plic pend = %08lx\n", *pAdr_pend);

  IsrUart();


  source = *pAdr_claim;
//  printm("isr plic claim = %08lx\n", source);
  *pAdr_claim = source;

	  }

}
#endif

