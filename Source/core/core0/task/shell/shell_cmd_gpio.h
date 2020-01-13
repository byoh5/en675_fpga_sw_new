#ifndef _SHELL_USR_GPIO_H_
#define _SHELL_USR_GPIO_H_

#include "shell.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//
#define gpio_getset(get_func, set_func, ch)			{	if (argc == 2) { \
															UINT getdValue = get_func(ch); \
															printf(#get_func"(%d)\n", getdValue); \
														} else { \
															UINT getdValue = atoi(argv[2]); \
															set_func(ch, getdValue); \
															printf("input(%d) => set (%d)\n", getdValue, get_func(ch)); \
														} \
													}

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern int cmd_perl_gpio(int argc, char *argv[]);

//-------------------------------------------------------------------------------------------------
// Variable
extern const char *sPerlGpio[];

#endif // _SHELL_USR_GPIO_H_
