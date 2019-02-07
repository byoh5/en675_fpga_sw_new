/*
 * uart.c
 *
 *  Created on: 2018. 11. 28.
 *      Author: BYOH
 */
#include "dev.h"

#define UART_INT_ENABLE

void Uart7_Init(unsigned int BaudRate)
{
	UART7_CLK_DIV = (MCK_FREQ / (BaudRate << 4)) - 1;
	SYS_GPIO56_MUX = 3;
	SYS_GPIO57_MUX = 3;
}

void Uart7_Tx(char data)
{
	while(UART7_TX_FULL);
	UART7_TX_DAT = data;
}

int Uart7_Rx(void)
{
	while(UART7_RX_EMPTY);
	return UART7_RX_DAT;
}

char Uart7_GetByte(void)
{
	return UART7_RX_DAT;
}

char Uart7_RxExist(void)
{
	if (UART7_RX_EMPTY == 0) {
		return 1;
	} else {
		return 0;
	}

	return !UART7_RX_EMPTY;
}

char Uart7_empty(void)
{
	volatile unsigned int* pst = (unsigned int*)0x44a00000;
	return (*pst & 0x02);
}

void Uart7_clear(void)
{
	volatile unsigned int* ptr = (unsigned int*)0x44a00000;
	*ptr =(unsigned int)0x01a0204e;
}

#ifdef UART_INT_ENABLE
void Uart7_int_init(void){

  extern char _payload_start, _payload_end;

  volatile unsigned int * pAdr_prio1 = (unsigned int *)0xc000004;
  volatile unsigned int * pAdr_prio2 = (unsigned int *)0xc000008;
  volatile unsigned int * pAdr_prio3 = (unsigned int *)0xc00000C;
  volatile unsigned int * pAdr_prio4 = (unsigned int *)0xc000010;
  volatile unsigned int * pAdr_mie   = (unsigned int *)0xc002000;
  volatile unsigned int * pAdr_mie2   = (unsigned int *)0xc002004;
  volatile unsigned int * pAdr_mie3   = (unsigned int *)0xc002008;
  volatile unsigned int * pAdr_mie4   = (unsigned int *)0xc002010;
  volatile unsigned int * pAdr_sie   = (unsigned int *)0xc002080;
  volatile unsigned int * pAdr_mthre = (unsigned int *)0xc200000;
  volatile unsigned int * pAdr_sthre = (unsigned int *)0xc201000;
  volatile unsigned int * pAdr_pend  = (unsigned int *)0xc001000;

  _printf("%s prio1 : %X\n", __func__, *pAdr_prio1);
  _printf("%s prio2 : %X\n", __func__, *pAdr_prio2);
  _printf("%s mie   : %X\n", __func__, *pAdr_mie);
  _printf("%s sie   : %X\n", __func__, *pAdr_sie);
  _printf("%s mthre : %X\n", __func__, *pAdr_mthre);
  _printf("%s sthre : %X\n", __func__, *pAdr_sthre);
  _printf("%s pend  : %X\n", __func__, *pAdr_pend);
#if 1
  *pAdr_prio1 = 1; // 꼭 1이여야 한다.
  *pAdr_prio2 = 1;
  *pAdr_prio3 = 1;
  *pAdr_prio4 = 1;
  *pAdr_mie = 0x0; // 꼭 2여야 한다.
  *pAdr_mie2 = 0x2;
  *pAdr_mie3 = 0x2;
  *pAdr_mie4 = 0x2;
  //*pAdr_sie = 0xffffffff;
  *pAdr_mthre = 0x1;

 // *pAdr_sthre = 0x0;
#else
	*pAdr_prio1 = 0;
	//*pAdr_prio2 = 1;
	*pAdr_mie = 0x0;
	//*pAdr_sie = 0xffffffff;
	pAdr_mthre = 0x0;
#endif
  // Enable External Interrupts
  write_csr(mie, MIP_MEIP);

  // Enable Global Interrupts
  unsigned long mstatus = read_csr(mstatus);
  mstatus = INSERT_FIELD(mstatus, MSTATUS_MIE, 1);
  //mstatus = INSERT_FIELD(mstatus, MSTATUS_SIE, 1);
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

	while(!Uart7_empty()) {
		bBuf = Uart7_GetByte();

		if(bBuf == '\r') _printf("\r\n");
			else {
			_printf("%c",bBuf);
		}


  }
  Uart7_clear();
}
#endif

