#ifndef __ENX_FREERTOS_H__
#define __ENX_FREERTOS_H__

#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"

extern void vMemoryHeapInit(void);
extern void vMemoryHeapInfoPrint(void);
extern uint32_t xPortGetTotalHeapSize(void);
extern void vTaskInfoPrint(void);
extern TaskHandle_t vTaskCreate(const char *name, TaskFunction_t thread, void *arg, int stacksize, int prio);

//------------------------------------------------------------------------------
// Task priority
#define LV7_TASK_PRIO			(tskIDLE_PRIORITY + 7)
#define LV6_TASK_PRIO			(tskIDLE_PRIORITY + 6)
#define LV5_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define LV4_TASK_PRIO			(tskIDLE_PRIORITY + 4)
#define LV3_TASK_PRIO			(tskIDLE_PRIORITY + 3)
#define LV2_TASK_PRIO			(tskIDLE_PRIORITY + 2)
#define LV1_TASK_PRIO			(tskIDLE_PRIORITY + 1)

// Task stack size
#define LV6_STACK_SIZE		(128*1024)
#define LV5_STACK_SIZE		(64*1024)
#define LV4_STACK_SIZE		(48*1024)
#define LV3_STACK_SIZE		(32*1024)
#define LV2_STACK_SIZE		(16*1024)
#define LV1_STACK_SIZE		( 8*1024)
#define LV0_STACK_SIZE		( 4*1024)

extern void vPortSysTickHandler( void );

extern volatile BYTE gbXsrTaskSwitchNeeded; // freeRTOS

#endif
