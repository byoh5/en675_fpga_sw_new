#ifndef _SHELL_H_
#define _SHELL_H_

#include "dev.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
// Command structure
typedef struct{
	char *name;
	int (*func)(int argc, char *argv[]);
	const char **helphelp;
} tMonCmd;

//-------------------------------------------------------------------------------------------------
// Constants
#define	CMD_SUCCESS			 0
#define	CMD_FAILURE			-1
#define	CMD_PARAM_ERROR     -2
#define	CMD_LINE_ERROR      -3
#define	CMD_ULVL_DENIED     -4
#define	CMD_NOT_FOUND       -5
#define	CMD_PREFILL         -6

#define	CMDLINESIZE			64
#define	ARGCNT				16

#define	CTLC				0x3

//-------------------------------------------------------------------------------------------------
//
#define PROMPT				"RISCV > "
#define PROMPT_SIZE			30

#define Shell_Unknown()	{	int __unknown; \
							_printf("    %4u:%s: errer case : input => ", __LINE__, __func__); \
							for(__unknown=0;__unknown<argc;__unknown++) \
								_printf("%s ", argv[__unknown]); \
							_printf("\n"); \
						}

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern void IsrShell(void);
extern void shell_line_print(char line, char *title);
extern void T_ShellExe(char *shellBuf);
extern void ShellTask(void *pvParameters);

//-------------------------------------------------------------------------------------------------
// Variable


#endif
