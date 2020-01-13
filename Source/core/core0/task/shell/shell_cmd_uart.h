#ifndef _SHELL_USR_UART_H_
#define _SHELL_USR_UART_H_

#include "shell.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//
#define uart_getset(get_func, set_func, ch)		{	if (argc == 3) { \
														UINT getdValue = get_func(ch); \
														printf(#get_func"(%d)\n", getdValue); \
													} else { \
														UINT getdValue = atoi(argv[3]); \
														set_func(ch, getdValue); \
														printf("input(%d) => set (%d)\n", getdValue, get_func(ch)); \
													} \
												}

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern int cmd_perl_uart(int argc, char *argv[]);

//-------------------------------------------------------------------------------------------------
// Variable
extern const char *sPerlUart[];

#endif // _SHELL_USR_UART_H_
