#ifndef _SHELL_USR_MEDIA_H_
#define _SHELL_USR_MEDIA_H_

#include "shell.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern int cmd_test_video(int argc, char *argv[]);
extern int cmd_test_jpeg(int argc, char *argv[]);
extern int cmd_test_audio(int argc, char *argv[]);

//-------------------------------------------------------------------------------------------------
// Variable
extern const char *sTestVideoCmd[];
extern const char *sTestJpegCmd[];
extern const char *sTestAudioCmd[];

#endif // _SHELL_USR_MEDIA_H_
