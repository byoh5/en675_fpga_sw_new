// See LICENSE for license details.

#ifndef _EYENIX_UART_H
#define _EYENIX_UART_H

#include <stdint.h>

extern volatile uint32_t* uart;

#define UART_REG_CTRL		0
#define UART_REG_RXFIFO		2
#define UART_REG_TXFIFO		4

#define	UART_TX_FULL		0x20
#define UART_RX_EMPTY		0x02

void uart_putchar(uint8_t ch);
int uart_getchar();
void query_uart(uintptr_t dtb);

#endif
