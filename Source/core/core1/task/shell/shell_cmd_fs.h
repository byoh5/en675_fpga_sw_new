#ifndef _SHELL_USR_FS_H_
#define _SHELL_USR_FS_H_

#include "shell.h"


//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//



//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
#if defined(__FILESYSTEM__)
extern int UsrDiskInitCmd(INT32S argc, char *argv[]);
extern int UsrLSCmd(INT32S argc, char *argv[]);
extern int UsrFormatCmd(INT32S argc, char *argv[]);
extern int UsrMKRMDIRCmd(INT32S argc, char *argv[]);
extern int UsrRMCmd(INT32S argc, char *argv[]);
extern int UsrPWDCmd(INT32S argc, char *argv[]);
extern int UsrCDCmd(INT32S argc, char *argv[]);
extern int UsrFCatCmd(INT32S argc, char *argv[]);
extern int UsrFCreateCmd(INT32S argc, char *argv[]);
extern int UsrFCopyCmd(INT32S argc, char *argv[]);
extern int UsrFstatCmd(INT32S argc, char *argv[]);
extern int UsrFileHash(INT32S argc, char *argv[]);
#if (LOAD_FS_SDCARD==1)
extern int UsrSDCardSpeedTestCmd(INT32S argc, char *argv[]);
extern int UsrMakevidCmd(INT32S argc, char *argv[]);
#endif
extern int UseFatTest(INT32S argc, char *argv[]);
#endif

//-------------------------------------------------------------------------------------------------
// Variable
#if defined(__FILESYSTEM__)
extern const char *sUsrLSCmd[];
extern const char *sDiskInitCmd[];
extern const char *sFormatCmd[];
extern const char *sMKDIRCmd[];
extern const char *sRMDIRCmd[];
extern const char *sRMCmd[];
extern const char *sPWDCmd[];
extern const char *sCDCmd[];
extern const char *sFCatCmd[];
extern const char *sFCreateCmd[];
extern const char *sFCopyCmd[];
extern const char *sFstatCmd[];
extern const char *sFileHash[];
#if (LOAD_FS_SDCARD==1)
extern const char *sSDCardSpeedTestCmd[];
extern const char *sMakevidCmd[];
#endif
extern const char *sFatTestCmd[];
#endif

#endif
