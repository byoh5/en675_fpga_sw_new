#include "dev.h"

#if USE_UART0 | USE_UART1 | USE_UART2 | USE_UART3 | USE_UART4 | USE_UART5 | USE_UART6 | USE_UART7 | USE_UART8

static volatile _UART_REG0 * const arrUART[UART_CNT]      = {(_UART_REG0 *)(REG_BASE_UART0+(0<<3)), (_UART_REG0 *)(REG_BASE_UART1+(0<<3)), (_UART_REG0 *)(REG_BASE_UART2+(0<<3)), (_UART_REG0 *)(REG_BASE_UART3+(0<<3)), (_UART_REG0 *)(REG_BASE_UART4+(0<<3)), (_UART_REG0 *)(REG_BASE_UART5+(0<<3)), (_UART_REG0 *)(REG_BASE_UART6+(0<<3)), (_UART_REG0 *)(REG_BASE_UART7+(0<<3)), (_UART_REG0 *)(REG_BASE_UART8+(0<<3))};
static volatile _UART_REG1 * const arrUARTRX[UART_CNT]    = {(_UART_REG1 *)(REG_BASE_UART0+(1<<3)), (_UART_REG1 *)(REG_BASE_UART1+(1<<3)), (_UART_REG1 *)(REG_BASE_UART2+(1<<3)), (_UART_REG1 *)(REG_BASE_UART3+(1<<3)), (_UART_REG1 *)(REG_BASE_UART4+(1<<3)), (_UART_REG1 *)(REG_BASE_UART5+(1<<3)), (_UART_REG1 *)(REG_BASE_UART6+(1<<3)), (_UART_REG1 *)(REG_BASE_UART7+(1<<3)), (_UART_REG1 *)(REG_BASE_UART8+(1<<3))};
static volatile _UART_REG2 * const arrUARTTX[UART_CNT]    = {(_UART_REG2 *)(REG_BASE_UART0+(2<<3)), (_UART_REG2 *)(REG_BASE_UART1+(2<<3)), (_UART_REG2 *)(REG_BASE_UART2+(2<<3)), (_UART_REG2 *)(REG_BASE_UART3+(2<<3)), (_UART_REG2 *)(REG_BASE_UART4+(2<<3)), (_UART_REG2 *)(REG_BASE_UART5+(2<<3)), (_UART_REG2 *)(REG_BASE_UART6+(2<<3)), (_UART_REG2 *)(REG_BASE_UART7+(2<<3)), (_UART_REG2 *)(REG_BASE_UART8+(2<<3))};
static volatile _UART_REG3 * const arrUARTRXLMT[UART_CNT] = {(_UART_REG3 *)(REG_BASE_UART0+(3<<3)), (_UART_REG3 *)(REG_BASE_UART1+(3<<3)), (_UART_REG3 *)(REG_BASE_UART2+(3<<3)), (_UART_REG3 *)(REG_BASE_UART3+(3<<3)), (_UART_REG3 *)(REG_BASE_UART4+(3<<3)), (_UART_REG3 *)(REG_BASE_UART5+(3<<3)), (_UART_REG3 *)(REG_BASE_UART6+(3<<3)), (_UART_REG3 *)(REG_BASE_UART7+(3<<3)), (_UART_REG3 *)(REG_BASE_UART8+(3<<3))};
ISRD static tIhnd arrUARTRXIrq[UART_CNT];
ISRD static tIhnd arrUARTTXIrq[UART_CNT];

void UartInit(UINT nCH, UINT Speed_Hz)
{
	arrUART[nCH]->CLK_DIV = (MCK_FREQ / (Speed_Hz << 4)) - 1;
	arrUART[nCH]->TX_TYPE = 0; // 0:open-drain 1:push-pull
	arrUART[nCH]->STOP_BIT = 0; // 0:1bit 1:2bit
	arrUART[nCH]->PARITY_EN = 0; // 0:off 1:on
	arrUART[nCH]->PARITY_TYPE = 0; // 0:even 1:odd
	arrUART[nCH]->TX_IRQ_EN = 0; // 0:normal 1:interrupt occurs
	arrUART[nCH]->RX_IRQ_EN = 0; // 0:normal 1:interrupt occurs

	arrUARTRXIrq[nCH].irqfn = NULL;
	arrUARTRXIrq[nCH].arg = NULL;
	arrUARTTXIrq[nCH].irqfn = NULL;
	arrUARTTXIrq[nCH].arg = NULL;

	// pin mux setting
	switch (nCH) {
		case 0:	UART0_PIN_INIT;	break;
		case 1:	UART1_PIN_INIT;	break;
		case 2:	UART2_PIN_INIT;	break;
		case 3:	UART3_PIN_INIT;	break;
		case 4:	UART4_PIN_INIT;	break;
		case 5:	UART5_PIN_INIT;	break;
		case 6:	UART6_PIN_INIT;	break;
		case 7:	UART7_PIN_INIT;	break;
		case 8:	UART8_PIN_INIT;	break;
	}

	//printf("UART%u Init - %uHz(%u)\n", nCH, MCK_FREQ / ((arrUART[nCH]->CLK_DIV + 1) * 16), arrUART[nCH]->CLK_DIV);
}

void UartDeinit(UINT nCH)
{
	arrUART[nCH]->CLK_DIV = 0;
	arrUART[nCH]->TX_TYPE = 0;
	arrUART[nCH]->STOP_BIT = 0;
	arrUART[nCH]->PARITY_EN = 0;
	arrUART[nCH]->PARITY_TYPE = 0;
	arrUART[nCH]->TX_IRQ_EN = 0;
	arrUART[nCH]->RX_IRQ_EN = 0;

	arrUARTRXIrq[nCH].irqfn = NULL;
	arrUARTRXIrq[nCH].arg = NULL;
	arrUARTTXIrq[nCH].irqfn = NULL;
	arrUARTTXIrq[nCH].arg = NULL;

	// pin mux setting
	switch (nCH) {
		case 0:	UART0_PIN_DEINIT;	break;
		case 1:	UART1_PIN_DEINIT;	break;
		case 2:	UART2_PIN_DEINIT;	break;
		case 3:	UART3_PIN_DEINIT;	break;
		case 4:	UART4_PIN_DEINIT;	break;
		case 5:	UART5_PIN_DEINIT;	break;
		case 6:	UART6_PIN_DEINIT;	break;
		case 7:	UART7_PIN_DEINIT;	break;
		case 8:	UART8_PIN_DEINIT;	break;
	}
}

void UartTx(UINT nCH, char data)
{
	while (arrUART[nCH]->TX_FULL || arrUART[nCH]->TX_IRQ_EN);
	arrUARTTX[nCH]->TX_DAT = data;
}

void UartTxSetByte(UINT nCH, char data)
{
	arrUARTTX[nCH]->TX_DAT = data;
}

UINT UartTxIsEmpty(UINT nCH)
{
	return arrUART[nCH]->TX_EMPTY;
}

UINT UartTxIsFull(UINT nCH)
{
	return arrUART[nCH]->TX_FULL;
}

void UartTxIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrUARTTXIrq[nCH].irqfn = irqfn;
	arrUARTTXIrq[nCH].arg = arg;
}

void UartTxSetIrqEn(UINT nCH, ENX_SWITCH sw)
{	// Direction : Uart interrupt enable/disable
	arrUART[nCH]->TX_IRQ_EN = sw;
}

ENX_SWITCH UartTxGetIrqEn(UINT nCH)
{
	return arrUART[nCH]->TX_IRQ_EN;
}

void UartTxIrqClear(UINT nCH)
{
	arrUART[nCH]->TX_IRQ_CLR = 1;
}

UINT UartTxIsIrq(UINT nCH)
{
	return arrUART[nCH]->TX_IRQ;
}

UINT UartRx(UINT nCH)
{
	while (arrUART[nCH]->RX_EMPTY);
	return arrUARTRX[nCH]->RX_DAT;
}

UINT UartRxGetByte(UINT nCH)
{
	return arrUARTRX[nCH]->RX_DAT;
}

UINT UartRxIsEmpty(UINT nCH)
{
	return arrUART[nCH]->RX_EMPTY;
}

UINT UartRxIsFull(UINT nCH)
{
	return arrUART[nCH]->RX_FULL;
}

void UartRxIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrUARTRXIrq[nCH].irqfn = irqfn;
	arrUARTRXIrq[nCH].arg = arg;
}

void UartRxSetIrqEn(UINT nCH, ENX_SWITCH sw)
{	// Direction : Uart interrupt enable/disable
	arrUART[nCH]->RX_IRQ_EN = sw;
}

ENX_SWITCH UartRxGetIrqEn(UINT nCH)
{
	return arrUART[nCH]->RX_IRQ_EN;
}

void UartRxIrqClear(UINT nCH)
{
	arrUART[nCH]->RX_IRQ_CLR = 1;
}

UINT UartRxIsIrq(UINT nCH)
{
	return arrUART[nCH]->RX_IRQ;
}

void IrqUart(UINT nCH)
{
	while (UartRxIsIrq(nCH)) {
		UartRxIrqClear(nCH);
		while (UartRxIsEmpty(nCH) == 0) {
			if (arrUARTRXIrq[nCH].irqfn) {
				arrUARTRXIrq[nCH].irqfn(arrUARTRXIrq[nCH].arg);
			} else {
				printf("UART%u-RX IRQ Get: [%c]\n", UartRxGetByte(nCH)); // rx drop
			}
		}
	}

	while (UartTxIsIrq(nCH)) {
//		UartTxIrqClear(nCH);
//		_printf("UART%d-TX IRQ Get\n", nCH);
		if (arrUARTTXIrq[nCH].irqfn) {
			arrUARTTXIrq[nCH].irqfn(arrUARTTXIrq[nCH].arg);
		}
	}
}
#else
void IrqUart(UINT nCH)
{
	_Rprintf("UART%u IRQ Get! UART%u is inactive.\n", nCH, nCH);
	ENX_ASSERT(0);
}
#endif
