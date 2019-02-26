//*************************************************************************************************
// Sdcard task
//*************************************************************************************************
#include "dev.h"
#include "enx_freertos.h"

#include "sdcard.h"

static UINT bSDState = sd_OFF;

void SdcardInit(void)
{
	int flag = 1;
init_start:
	vTaskDelay(10);
//	FRESULT fres;
	bSDState = sd_INIT;
	if (SdioCdInitProcess() == DEF_FAIL) {
		printf("SD card IN(init fail)\n");
		bSDState = sd_ERR;
		goto init_end;
	}
	if (flag == 2) {
		SdioCdClockDown();
	}





init_err:
	if (flag == 1) {
		flag = 2;
		goto init_start;
	}

init_end:
	return;
}

void SdcardTask(void *pvParameters)
{
	vTaskDelay(2000); // Log 겹치기 방지용

	while (1) {
		UINT SDDet = SdioCdDet();
		if (SDDet == 0) { // SD card IN
			switch (bSDState) {
			case sd_OFF:	// Initialization... This is done by "SDcardInit"
				SdcardInit();
				break;
			case sd_INIT:	// Do nothing until initialization is finished.
				break;
			case sd_READY:
				break;
			case sd_IDLE:	// sd_IDLE state by "SDcardInit" or "sdsave off"
				break;
			case sd_SAVE:	// sd_SAVE state by "SDcardInitTask" or "sdsave on"
				break;
			case sd_ERR:
				printf("SD state Error!\n");
				bSDState = sd_ERR_IDLE;
				break;
			case sd_ERR_IDLE:
				break;
			}
		} else if (SDDet == 1 && bSDState != sd_OFF) { // SD card OUT
			bSDState = sd_OFF;
			printf("SD card OUT\n");
		} else if (bSDState == sd_ERR) {
			static int cnt = 0;
			cnt++;
			if (cnt == 10) {
				printf("SDcard Error / reload\n");
				bSDState = sd_OFF;
				cnt = 0;
			}
		}
		vTaskDelay(200);
	}
	UNUSED(pvParameters);
}
