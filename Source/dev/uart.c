/*
 * uart.c
 *
 *  Created on: 2018. 11. 28.
 *      Author: BYOH
 */
#include "dev.h"

#define UART_INT_ENABLE

static _UART_REG0 *arrUART[UART_CNT];
static _UART_REG1 *arrUARTRX[UART_CNT];
static _UART_REG2 *arrUARTTX[UART_CNT];
static _UART_REG3 *arrUARTRXLMT[UART_CNT];

void UartInit(UINT nCH, UINT Speed_Hz)
{
	const uint64_t addrUART[UART_CNT] = {
			REG_BASE_UART0, REG_BASE_UART1, REG_BASE_UART2, REG_BASE_UART3, REG_BASE_UART4,
			REG_BASE_UART5, REG_BASE_UART6, REG_BASE_UART7, REG_BASE_UART8
	};

	arrUART[nCH] = (_UART_REG0 *)(addrUART[nCH] + (0 << 3));
	arrUARTRX[nCH] = (_UART_REG1 *)(addrUART[nCH] + (1 << 3));
	arrUARTTX[nCH] = (_UART_REG2 *)(addrUART[nCH] + (2 << 3));
	arrUARTRXLMT[nCH] = (_UART_REG3 *)(addrUART[nCH] + (3 << 3));

	arrUART[nCH]->CLK_DIV = (MCK_FREQ / (Speed_Hz << 4)) - 1;
	arrUART[nCH]->TX_TYPE = 0; // 0:open-drain 1:push-pull
	arrUART[nCH]->STOP_BIT = 0; // 0:1bit 1:2bit
	arrUART[nCH]->PARITY_EN = 0; // 0:off 1:on
	arrUART[nCH]->PARITY_TYPE = 0; // 0:even 1:odd
	arrUART[nCH]->TX_IRQ_EN = 0; // 0:normal 1:interrupt occurs
	arrUART[nCH]->RX_IRQ_EN = 0; // 0:normal 1:interrupt occurs

	// pin mux setting
	switch (nCH) {
		case 0:
			UART0_PIN_INIT;
			break;
		case 1:
			UART1_PIN_INIT;
			break;
		case 2:
			UART2_PIN_INIT;
			break;
		case 3:
			UART3_PIN_INIT;
			break;
		case 4:
			UART4_PIN_INIT;
			break;
		case 5:
			UART5_PIN_INIT;
			break;
		case 6:
			UART6_PIN_INIT;
			break;
		case 7:
			UART7_PIN_INIT;
			break;
		case 8:
			UART8_PIN_INIT;
			break;
	}
}

void UartTxEi(UINT nCH)
{	// Direction : Uart interrupt enable
	arrUART[nCH]->TX_IRQ_EN = 1;
}

void UartTxDi(UINT nCH)
{	// Direction : Uart interrupt disable
	arrUART[nCH]->TX_IRQ_EN = 0;
}

void UartRxEi(UINT nCH)
{	// Direction : Uart interrupt enable
	arrUART[nCH]->RX_IRQ_EN = 1;
}

void UartRxDi(UINT nCH)
{	// Direction : Uart interrupt disable
	arrUART[nCH]->RX_IRQ_EN = 0;
}

void UartTx(UINT nCH, char data)
{
	while (arrUART[nCH]->TX_FULL);
	arrUARTTX[nCH]->TX_DAT = data;
}

UINT UartRx(UINT nCH)
{
	while (arrUART[nCH]->RX_EMPTY);
	return arrUARTRX[nCH]->RX_DAT;
}

UINT UartGetByte(UINT nCH)
{
	return arrUARTRX[nCH]->RX_DAT;
}

UINT UartRxExist(UINT nCH)
{
	return !arrUART[nCH]->RX_EMPTY;
}

UINT UartRxICheck(UINT nCH)
{
	return arrUART[nCH]->RX_IRQ;
}

void IrqUart(UINT nCH)
{
	while (UartRxExist(nCH) == 1) {
		UINT bBuf = UartGetByte(nCH);
		if (bBuf == '\r') {
			_printf("\r\n");
		} else {
			_printf("[%c]", bBuf);
		}
	}
	arrUART[nCH]->RX_IRQ_CLR = 1;
}

void IrqCheck(void)
{
	// Enable Global Interrupts
	volatile unsigned long mstatus = read_csr(mstatus);
	//mstatus = INSERT_FIELD(mstatus, MSTATUS_MIE, 1); // Machine Interrupt Enable
//	_printf("Enable Global Interrupts 1 (0x%08X)\n", mstatus);
//	mstatus = INSERT_FIELD(mstatus, MSTATUS_MPIE, 1); // Machine Previous Interrupt Enabler
//	_printf("Enable Global Interrupts 2 (0x%08X)\n", mstatus);
	//mstatus = INSERT_FIELD(mstatus, MSTATUS_MPP, 1); // Machine Previous Interrupt Enabler
	//_printf("Enable Global Interrupts 3 (0x%08X)\n", mstatus);
//	write_csr(mstatus, mstatus);

	_printf("3 IRQ(%d) IRQ(%d) mstatus(0x%08X)\n", arrUART[7]->RX_IRQ, UART7_RX_IRQ, mstatus);
	if (arrUART[7]->RX_IRQ == 1 || UART7_RX_IRQ == 1) {
		unsigned long mstatus = read_csr(mstatus);
		_printf("Enable Global Interrupts 0 (0x%08X)\n", mstatus);
		mstatus |= MSTATUS_MPP;
//		mstatus = INSERT_FIELD(mstatus, MSTATUS_MPP, 1); // Machine Previous Interrupt Enabler
		_printf("Enable Global Interrupts 1 (0x%08X)\n", mstatus);
		write_csr(mstatus, mstatus);


		//UartInit(7, UART7_SPEED);
	}
}


#if 0
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
#endif
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


void Uart7_int_init(void)
{
#if 0
  extern char _payload_start, _payload_end;

  volatile unsigned int *iPrioBase = (unsigned int *)IRQ_PRIO_BASE;
  volatile uintptr_t *iMieBase = (uintptr_t *)IRQ_MIE_BASE;
  volatile unsigned int *iMThreBase = (unsigned int *)IRQ_MTHRE_BASE;

  volatile unsigned int *iPanding = (unsigned int *)IRQ_PENDING_BASE;

#if 1
  for (int i = 0; i < 8; i++) {
	  _printf("%X - %d\n", &iPanding[i], iPanding[i]);
#if 0
	  _printf("%3d: %X-%X-%X, P(%d)I(%d)T(%d) -> ", i, &iPrioBase[i], &iMieBase[i], &iMThreBase[i], iPrioBase[i], iMieBase[i], iMThreBase[i]);
	  iPrioBase[i] = ~0;
	  iMieBase[i] = ~0;
	  iMThreBase[i] = 0;
	  _printf("P(%d)I(%d)T(%d)\n", iPrioBase[i], iMieBase[i], iMThreBase[i]);
#endif
  }
#endif
#endif
#if 0
  _printf("sizeof(uintptr_t) : %d\n", sizeof(uintptr_t));
  for (int i = 0; i <= 171*2; i++) {
	  iPrioBase[i] = 1;
	  iMieBase[i] = 2;
	  iMThreBase[i] = 0;
  }
#endif

#if 0
#if 0
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
#endif

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
  //*pAdr_prio2 = 1;
  //*pAdr_prio3 = 1;
  //*pAdr_prio4 = 1;
  *pAdr_mie = 0x2; // 꼭 2여야 한다.
  //*pAdr_mie2 = 0x2;
  //*pAdr_mie3 = 0x2;
  //*pAdr_mie4 = 0x2;
  //*pAdr_sie = 0xffffffff;
  *pAdr_mthre = 0x0;

 // *pAdr_sthre = 0x0;
#else
	*pAdr_prio1 = 0;
	//*pAdr_prio2 = 1;
	*pAdr_mie = 0x0;
	//*pAdr_sie = 0xffffffff;
	pAdr_mthre = 0x0;
#endif
#endif

  // Enable External Interrupts
  write_csr(mie, MIP_MEIP);

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
  //unsigned int temp;

}




void IsrUart(void)
{
	char bBuf;

	while(!Uart7_empty()) {
		bBuf = UartGetByte(7);

		if(bBuf == '\r') _printf("\r\n");
			else {
			_printf("%c",bBuf);
		}


  }
  Uart7_clear();
}
#endif

