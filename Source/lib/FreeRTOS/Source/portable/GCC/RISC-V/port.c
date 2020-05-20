/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the RISC-V port.
 *----------------------------------------------------------*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "dev.h"
#include <string.h>

volatile BYTE gbXsrTaskSwitchNeeded;


/* The stack used by interrupt service routines.  Set configISR_STACK_SIZE_WORDS
to use a statically allocated array as the interrupt stack.  Alternative leave
configISR_STACK_SIZE_WORDS undefined and update the linker script so that a
linker variable names __freertos_irq_stack_top has the same value as the top
of the stack used by main.  Using the linker script method will repurpose the
stack that was used by main before the scheduler was started for use as the
interrupt stack after the scheduler has started. */
#ifdef configISR_STACK_SIZE_WORDS
	static __attribute__ ((aligned(portBYTE_ALIGNMENT))) StackType_t xISRStack[ configISR_STACK_SIZE_WORDS ] = { 0 };
	const StackType_t xISRStackTop = ( StackType_t ) &( xISRStack[ ( configISR_STACK_SIZE_WORDS & ~portBYTE_ALIGNMENT_MASK ) - 1 ] );
#else
	extern const uint32_t __freertos_irq_stack_top[];
	const StackType_t xISRStackTop = ( StackType_t ) __freertos_irq_stack_top;
#endif

void vPortSetupTimerInterrupt(void)
{	/* Sets and enable the timer interrupt */
	enx_timerirq_init();
}
/*-----------------------------------------------------------*/


void vPortSetupExternalInterrupt(void)
{	/* Sets and enable the external interrupt */
	enx_externalirq_init_cpu0();
}
/*-----------------------------------------------------------*/


BaseType_t xPortStartScheduler( void )
{
extern void xPortStartFirstTask( void );

	/* If there is a CLINT then it is ok to use the default implementation
	in this file, otherwise vPortSetupTimerInterrupt() must be implemented to
	configure whichever clock is to be used to generate the tick interrupt. */
	vPortSetupTimerInterrupt();

	/* Enable external interrupts. */
	vPortSetupExternalInterrupt();

	xPortStartFirstTask();

	/* Should not get here as after calling xPortStartFirstTask() only tasks
	should be executing. */
	return pdFAIL;
}
/*-----------------------------------------------------------*/


void vPortEndScheduler( void )
{
	/* Not implemented. */
	for( ;; );
}
/*-----------------------------------------------------------*/


void vPortSysTickHandler( void )
{	/* Handler for timer interrupt */
	enx_timerirq_next();

	/* Increment the RTOS tick. */
	if (xTaskIncrementTick() != pdFALSE) {
		vTaskSwitchContext();
	}
}
/*-----------------------------------------------------------*/


void prvTaskExitError(void)
{
    /* A function that implements a task must not exit or attempt to return to
    its caller as there is nothing to return to.  If a task wants to exit it
    should instead call vTaskDelete( NULL ).

    Artificially force an assert() to be triggered if configASSERT() is
    defined, then stop here so application writers can catch the error. */
	printf("The task was incorrectly closed. [%s]\n", pcTaskGetName(NULL) == NULL ? "null" : pcTaskGetName(NULL));
    vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/


/* Clear current interrupt mask and set given mask */
void vPortClearInterruptMask(int mask)
{
#if 0
	__asm volatile( "csrs mstatus, 8" );
#else
    //__asm volatile("csrw mie, %0" ::"r"(mask));
	write_csr(mie, mask);
#endif
}
/*-----------------------------------------------------------*/

/* Set interrupt mask and return current interrupt enable register */
int vPortSetInterruptMask(void)
{
#if 0
	__asm volatile( "csrc mstatus, 8" );
#else
    //__asm volatile("csrr %0, mie" : "=r"(ret));
    //__asm volatile("csrc mie, %0" ::"i"(7));
	int ret = read_csr(mie);
 	write_csr(mie, 0);
	return ret;
#endif
}
/*-----------------------------------------------------------*/


void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	_printf("%s:%d: %s\n", __func__, __LINE__, pcTaskName);
	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/


void vApplicationMallocFailedHook( void )
{
	_printf("%s:%d: %s\n", __func__, __LINE__, pcTaskGetName(NULL));
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/


static uint32_t uiTotalHeapMemorySize = 0;
void vMemoryHeapInit(void)
{
#if 1
	/* Allocate two blocks of RAM for use by the heap.  The first is a block of
	0x10000 bytes starting from address 0x80000000, and the second a block of
	0xa0000 bytes starting from address 0x90000000.  The block starting at
	0x80000000 has the lower start address so appears in the array fist. */

	register ULONG hs, he;
	asm volatile("la %0, _heap_os_start" : "=r"(hs));
	asm volatile("la %0, _heap_os_end" : "=r"(he));

	HeapRegion_t xHeapRegions[2] = {
	    { ( uint8_t * ) hs, he - hs },
	    { NULL, 0 } /* Terminates the array. */
	};

	uiTotalHeapMemorySize = 0;
	int len = sizeof(xHeapRegions) / sizeof(xHeapRegions[0]);
	for (int i = 0; i < len; i++) {
		if (xHeapRegions[i].xSizeInBytes > 0) {
			printf("Heap[%d] memory 0x%08X ~ 0x%08X, %ubyte\n", i, xHeapRegions[i].pucStartAddress, xHeapRegions[i].pucStartAddress + xHeapRegions[i].xSizeInBytes, xHeapRegions[i].xSizeInBytes);
#if 0
			memset(xHeapRegions[i].pucStartAddress, 0, xHeapRegions[i].xSizeInBytes);
#else
			hwflush_dcache_range(xHeapRegions[i].pucStartAddress, xHeapRegions[i].xSizeInBytes);
			BDmaMemSet_isr(0, (BYTE *)xHeapRegions[i].pucStartAddress, 0x00, xHeapRegions[i].xSizeInBytes);
			hwflush_dcache_range(xHeapRegions[i].pucStartAddress, xHeapRegions[i].xSizeInBytes);
#endif
		}
		uiTotalHeapMemorySize += xHeapRegions[i].xSizeInBytes;
	}

//	Test code
	printf("xISRStack: 0x%08X, %uByte.\n", xISRStack, sizeof(xISRStack));
	SYS_REG7 = (UINT)(intptr_t)xISRStack;
//////////////////////////


	/* Pass the array into vPortDefineHeapRegions(). */
	vPortDefineHeapRegions( xHeapRegions );
#endif
}
/*-----------------------------------------------------------*/


void vMemoryHeapInfoPrint(void)
{
#if 1
	size_t nPortGetFreeHeapSize = xPortGetFreeHeapSize();
	size_t nPortGetMinimumEverFreeHeapSize = xPortGetMinimumEverFreeHeapSize();
	_printf("Total Heap Size            : %lu\n", uiTotalHeapMemorySize);
	_printf(" Free Heap Size            : %lu\n", nPortGetFreeHeapSize);
	_printf("Minimum Ever Free Heap Size: %lu\n", nPortGetMinimumEverFreeHeapSize);
	_printf("                      Used : %lu%%\n", 100 - (nPortGetFreeHeapSize * 100) / uiTotalHeapMemorySize);
#endif
}

uint32_t xPortGetTotalHeapSize(void)
{
	return uiTotalHeapMemorySize;
}
/*-----------------------------------------------------------*/


void vTaskInfoPrint(void)
{
//#define TaskStatusCount 40
	const char * const task_state[5] = {"Run", "Ready", "Blocked", "Suspended", "Deleted"};
	uint32_t uiTotal = 0;
	UBaseType_t uxTask = 0;
	//TaskStatus_t etiList[TaskStatusCount];
	TaskStatus_t *etiList;
	UBaseType_t TaskStatusCount = uxTaskGetNumberOfTasks();
	etiList = pvPortCalloc(sizeof(TaskStatus_t), TaskStatusCount);

//	memset(etiList, 0, sizeof(etiList));
//	printf("TaskStatus: %u\n", sizeof(etiList));
	uxTask = uxTaskGetSystemState(etiList, TaskStatusCount, &uiTotal);

	printf("----------------------------------Task List(%02d)---------------------------------\n", uxTask);
	printf("ID Task       Handle     State   Priority  Remained/Stack     Counter Percentage\n");
	printf("--------------------------------------------------------------------------------\n");
	for (UBaseType_t i = 0; i < uxTask; i++) {
		printf("%2lu %-8s 0x%08X %9s    %lu/%lu    %4lu/0x%08X %10u      %3lu%%\n",
				etiList[i].xTaskNumber,
				etiList[i].pcTaskName,
				etiList[i].xHandle,
				task_state[etiList[i].eCurrentState],
				etiList[i].uxCurrentPriority,
				etiList[i].uxBasePriority,
				etiList[i].usStackHighWaterMark,
				etiList[i].pxStackBase,
				etiList[i].ulRunTimeCounter,
				(etiList[i].ulRunTimeCounter * 100) / uiTotal
				);
	}
	printf("----------------------------------------------TotalRunTime(%10lu)----------\n", uiTotal);

	vPortFree(etiList);
}
/*-----------------------------------------------------------*/


TaskHandle_t vTaskCreate(const char *name, TaskFunction_t thread, void *arg, int stacksize, int prio)
{
	TaskHandle_t objTask;
	int result = xTaskCreate(thread, (const char * const)name, stacksize/sizeof(StackType_t), arg, prio, &objTask);
	if (result == pdPASS) {
		_printf("vTaskCreate : pass : %s(%x) -size(%d)\n", name, objTask, stacksize);
		return objTask;
	} else {
		_printf("vTaskCreate : fail : %s(%x) -size(%d)\n", name, objTask, stacksize);
		return NULL;
	}
}
/*-----------------------------------------------------------*/
