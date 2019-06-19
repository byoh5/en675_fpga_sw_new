#ifndef _SHELL_USR_MEM_H_
#define _SHELL_USR_MEM_H_

#include "shell.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern int cmd_test_dma(int argc, char *argv[]);
extern int cmd_test_memory(int argc, char *argv[]);
extern int cmd_test_memset(int argc, char *argv[]);
extern int cmd_test_checksum(int argc, char *argv[]);

//-------------------------------------------------------------------------------------------------
// Variable
extern const char *sBDmaTest[];
extern const char *sCDmaTest[];
extern const char *sMemoryTest[];
extern const char *sMemsetTest[];
extern const char *sChecksumTest[];

#endif // _SHELL_USR_MEM_H_
