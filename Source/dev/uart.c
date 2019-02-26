#include "dev.h"

static _UART_REG0 *arrUART[UART_CNT];
static _UART_REG1 *arrUARTRX[UART_CNT];
static _UART_REG2 *arrUARTTX[UART_CNT];
static _UART_REG3 *arrUARTRXLMT[UART_CNT];
static tIhnd arrUARTRXIrq[UART_CNT];
static tIhnd arrUARTTXIrq[UART_CNT];

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

	printf("UART%u Init - %uHz\n", nCH, MCK_FREQ / ((arrUART[nCH]->CLK_DIV + 1) * 16));
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

	arrUART[nCH] = NULL;
	arrUARTRX[nCH] = NULL;
	arrUARTTX[nCH] = NULL;
	arrUARTRXLMT[nCH] = NULL;

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
	while (arrUART[nCH]->TX_FULL);
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

void UartTxIrqOn(UINT nCH)
{	// Direction : Uart interrupt enable
	arrUART[nCH]->TX_IRQ_EN = 1;
}

void UartTxIrqOff(UINT nCH)
{	// Direction : Uart interrupt disable
	arrUART[nCH]->TX_IRQ_EN = 0;
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

void UartRxIrqOn(UINT nCH)
{	// Direction : Uart interrupt enable
	arrUART[nCH]->RX_IRQ_EN = 1;
}

void UartRxIrqOff(UINT nCH)
{	// Direction : Uart interrupt disable
	arrUART[nCH]->RX_IRQ_EN = 0;
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
	if (UartRxIsIrq(nCH)) {
		while (UartRxIsEmpty(nCH) == 0) {
			if (arrUARTRXIrq[nCH].irqfn) {
				arrUARTRXIrq[nCH].irqfn(arrUARTRXIrq[nCH].arg);
			}
		}
		UartRxIrqClear(nCH);
	}

	if (UartTxIsIrq(nCH)) {
		_printf("UART%d-TX IRQ Get\n", nCH);
		if (arrUARTTXIrq[nCH].irqfn) {
			arrUARTTXIrq[nCH].irqfn(arrUARTTXIrq[nCH].arg);
		}
		UartTxIrqClear(nCH);
	}
}
