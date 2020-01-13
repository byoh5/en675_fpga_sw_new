#ifndef _SHELL_USR_I2C_H_
#define _SHELL_USR_I2C_H_

#include "shell.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//
#define i2c_getset(get_func, set_func, ch)		{	if (argc == 2) { \
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
extern int cmd_perl_i2c(int argc, char *argv[]);

//-------------------------------------------------------------------------------------------------
// Variable
extern const char *sPerlI2c[];

#endif // _SHELL_USR_I2C_H_
