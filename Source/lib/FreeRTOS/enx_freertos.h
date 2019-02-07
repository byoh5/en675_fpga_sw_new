#ifndef __ENX_FREERTOS_H__
#define __ENX_FREERTOS_H__

#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"

#define FreeRTOSVer "10.1.1"

extern void vMemoryHeapInit(void);
extern void vMemoryHeapInfoPrint(void);
extern void vTaskInfoPrint(void);
extern TaskHandle_t vTaskCreate(const char *name, TaskFunction_t thread, void *arg, int stacksize, int prio);

#endif
