/*
 * uart.h
 *
 *  Created on: 2018. 11. 28.
 *      Author: BYOH
 */
#ifndef UART_H_
#define UART_H_

void Uart7_Init(unsigned int BaudRate);
void Uart7_int_init(void);
int Uart7_Rx(void);
void Uart7_Tx(char data);
char Uart7_GetByte(void);
char Uart7_RxExist(void);
char Uart7_empty(void);
void Uart7_clear(void);

#endif /* UART_H_ */
