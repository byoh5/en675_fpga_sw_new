#ifndef _SDCARD_H_
#define _SDCARD_H_

#include "sdio_cd.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
// 
enum {
	sd_OFF = 0,
	sd_INIT = 1,
	sd_READY = 2,
	sd_IDLE = 3,
	sd_SAVE = 4,
	sd_ERR = 5,
	sd_ERR_IDLE = 6
};

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern int SDcardGetFormatState(void);
extern void SDcardSetFormat(void);
extern void SdcardTask(void *pvParameters);
extern int getSDState(void);
extern void getSDGBSizeT(char *buf);
extern void getSDGBSizeU(UINT *buf);
extern ENX_OKFAIL getSDTimeCheck(void);

//-------------------------------------------------------------------------------------------------
// Variable


#endif // _SDCARD_H_
