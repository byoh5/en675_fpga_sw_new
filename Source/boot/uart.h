/*
 * uart.h
 *
 *  Created on: 2018. 11. 28.
 *      Author: BYOH
 */
#ifndef UART_H_
#define UART_H_

void Uart1_Init(void);
void Uart1_int_init(void);
int Uart1_Rx(void);
void Uart1_Tx(char data);
char Uart1_GetByte(void);
char Uart1_RxExist(void);
char Uart1_empty(void);
void Uart1_clear(void);

#endif /* UART_H_ */
