#ifndef _SHELL_USR_UTIL_H_
#define _SHELL_USR_UTIL_H_

#include "shell.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//


//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern int cmd_test_checksum(int argc, char *argv[]);
extern int cmd_test_sha(int argc, char *argv[]);
extern int cmd_test_aes(int argc, char *argv[]);

//-------------------------------------------------------------------------------------------------
// Variable
extern const char *sChecksumTest[];
extern const char *sShaTest[];
extern const char *sAesTest[];

#endif // _SHELL_USR_UTIL_H_
