/*
 * FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "dev.h"

/* A variable is used to keep track of the critical section nesting.  This
variable has to be stored as part of the task context and must be initialised to
a non zero value to ensure interrupts don't inadvertently become unmasked before
the scheduler starts.  As it is stored as part of the task context it will
automatically be set to 0 when the first task is started. */
static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

/* Contains context when starting scheduler, save all 31 registers */
#ifdef __gracefulExit
BaseType_t xStartContext[31] = {0};
#endif

/*
 * Handler for timer interrupt
 */
void vPortSysTickHandler( void );

/*
 * Setup the timer to generate the tick interrupts.
 */
void vPortSetupTimer( void );

/*
 * Set the next interval for the timer
 */
static void prvSetNextTimerInterrupt( void );

/*
 * Used to catch tasks that attempt to return from their implementing function.
 */
static void prvTaskExitError( void );

/*-----------------------------------------------------------*/

/* Sets the next timer interrupt
 * Reads current timer register and adds tickrate
 * Does nothing if a Clint was not found in the hardware configuration string
 * Using previous timer compare may fail if interrupts were disabled for a long time,
 * which is likely for the very first interrupt. When that happens, compare timer + 
 * tickrate may already be behind current timer and prevent correctly programming
 * the 2nd interrupt
 */
static void prvSetNextTimerInterrupt(void)
{
    if (mtime && timecmp) {
    	int cpu_id = read_csr(mhartid);
    	timecmp[cpu_id] = *mtime + (configTICK_CLOCK_HZ / configTICK_RATE_HZ);
    }
}
/*-----------------------------------------------------------*/

/* Sets and enable the timer interrupt */
void vPortSetupTimer(void)
{
    /* reuse existing routine */
    prvSetNextTimerInterrupt();

    // Enable the Machine-Timer bit in MIE
    set_csr(mie, MIP_MTIP);
}
/*-----------------------------------------------------------*/

void prvTaskExitError( void )
{
	/* A function that implements a task must not exit or attempt to return to
	its caller as there is nothing to return to.  If a task wants to exit it
	should instead call vTaskDelete( NULL ).

	Artificially force an assert() to be triggered if configASSERT() is
	defined, then stop here so application writers can catch the error. */
	_printf("[OS] Task Exit Error: [%s]\n", pcTaskGetName(NULL));
	vTaskDelete(NULL);
	//configASSERT( uxCriticalNesting == ~0UL );
	//portDISABLE_INTERRUPTS();
	//for( ;; );
}
/*-----------------------------------------------------------*/

/* Clear current interrupt mask and set given mask */
#if 1
void vPortClearInterruptMask(int mask)
{
#if 1
	__asm volatile("csrw mie, %0"::"r"(mask));
#else
	write_csr(mie, mask);
#endif
}
#endif
/*-----------------------------------------------------------*/

/* Set interrupt mask and return current interrupt enable register */
#if 1
int vPortSetInterruptMask(void)
{
	int ret;
#if 1
	__asm volatile("csrr %0,mie":"=r"(ret));
	__asm volatile("csrc mie,%0"::"i"(7));
#else
	ret = read_csr(mie);	// 검증X
	clear_csr(mie, 7);		// 검증X
#endif
	return ret;
}
#endif
/*-----------------------------------------------------------*/


/*
 * See header file for description.
 */
/*
 * Unlike other ports pxPortInitialiseStack() is written in assembly code as it
 * needs access to the portasmADDITIONAL_CONTEXT_SIZE constant.  The prototype
 * for the function is as per the other ports:
 * StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters );
 *
 * As per the standard RISC-V ABI pxTopcOfStack is passed in in a0, pxCode in
 * a1, and pvParameters in a2.  The new top of stack is passed out in a0.
 *
 * RISC-V maps registers to ABI names as follows (X1 to X31 integer registers
 * for the 'I' profile, X1 to X15 for the 'E' profile, currently I assumed).
 *
 * Register		ABI Name	Description						Saver
 * x0			zero		Hard-wired zero					-
 * x1			ra			Return address					Caller
 * x2			sp			Stack pointer					Callee
 * x3			gp			Global pointer					-
 * x4			tp			Thread pointer					-
 * x5-7			t0-2		Temporaries						Caller
 * x8			s0/fp		Saved register/Frame pointer	Callee
 * x9			s1			Saved register					Callee
 * x10-11		a0-1		Function Arguments/return values Caller
 * x12-17		a2-7		Function arguments				Caller
 * x18-27		s2-11		Saved registers					Callee
 * x28-31		t3-6		Temporaries						Caller
 *
 * The RISC-V context is saved t FreeRTOS tasks in the following stack frame,
 * where the global and thread pointers are currently assumed to be constant so
 * are not saved:
 *
 * mstatus
 * x31
 * x30
 * x29
 * x28
 * x27
 * x26
 * x25
 * x24
 * x23
 * x22
 * x21
 * x20
 * x19
 * x18
 * x17
 * x16
 * x15
 * x14
 * x13
 * x12
 * x11
 * x10(pvParameters)
 * x9
 * x8
 * x7
 * x6
 * x5
 * portTASK_RETURN_ADDRESS
 * [chip specific registers go here]
 * pxCode
 */

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
	/* Simulate the stack frame as it would be created by a context switch
	interrupt. */
	register int *tp asm("x3");
	pxTopOfStack--;
	*pxTopOfStack = (portSTACK_TYPE)pxCode;			/* Start address */
	pxTopOfStack -= 22;
	*pxTopOfStack = (portSTACK_TYPE)pvParameters;	/* Register a0 */
	pxTopOfStack -= 6;
	*pxTopOfStack = (portSTACK_TYPE)tp; /* Register thread pointer */
	pxTopOfStack -= 3;
	*pxTopOfStack = (portSTACK_TYPE)prvTaskExitError; /* Register ra */
	
	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

void vPortSysTickHandler( void )
{
	//clear_csr(mie, MIP_MTIP);

	prvSetNextTimerInterrupt();

	/* Increment the RTOS tick. */
	if( xTaskIncrementTick() != pdFALSE )
	{
		//_printf("%s Task Switch, %lu\n", __func__, *mtime);
		vTaskSwitchContext();
	}

	//set_csr(mie, MIP_MTIP);
}
/*-----------------------------------------------------------*/


void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	_printf("%s:%d\n", __func__, __LINE__);
	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/


void vApplicationMallocFailedHook( void )
{
	_printf("%s:%d\n", __func__, __LINE__);
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


void portTrapFromMachineMode( void )
{
	unsigned long mcause = read_csr(mcause);
	unsigned int irq_num = mcause & 0x7fffffff;
	if ((mcause&0x8000000000000000) != 0x0) {
		//volatile unsigned int * pAdr_prio =  (unsigned int *)0xc000004;
		//volatile unsigned int * pAdr_mie =   (unsigned int *)0xc002000;
		//volatile unsigned int * pAdr_sie =   (unsigned int *)0xc002080;
		//volatile unsigned int * pAdr_mthre = (unsigned int *)0xc200000;
		//volatile unsigned int * pAdr_sthre = (unsigned int *)0xc201000;
		//volatile unsigned int * pAdr_pend =  (unsigned int *)0xc001000;
		//volatile unsigned int * pAdr_claim =  (unsigned int *)0xc200004;
		//volatile unsigned int source = *pAdr_claim;
		switch (irq_num) {
			case IRQ_S_SOFT:
				_printf("call IRQ_S_SOFT\n");
				break;
			case IRQ_H_SOFT:
				_printf("call IRQ_H_SOFT\n");
				break;
			case IRQ_M_SOFT:
				_printf("call IRQ_M_SOFT\n");
				break;
			case IRQ_S_TIMER:
				_printf("call IRQ_S_TIMER\n");
				break;
			case IRQ_H_TIMER:
				_printf("call IRQ_H_TIMER\n");
				break;
			case IRQ_M_TIMER:
				_printf("call IRQ_M_TIMER\n");
				break;
			case IRQ_S_EXT:
				_printf("call IRQ_S_EXT\n");
				break;
			case IRQ_H_EXT:
				_printf("call IRQ_H_EXT\n");
				break;
			case IRQ_M_EXT:
				_printf("call IRQ_M_EXT\n");
				break;
			case IRQ_COP:
				_printf("call IRQ_COP\n");
				break;
			case IRQ_HOST:
				_printf("call IRQ_HOST\n");
				break;
			default:
				_printf("call IRQ1-default(%d)\n", irq_num);
				break;
		}
		//*pAdr_claim = source;
	} else {
		switch (irq_num) {
			case CAUSE_MISALIGNED_FETCH:
				_printf("call CAUSE_MISALIGNED_FETCH\n");
				break;
			case CAUSE_FETCH_ACCESS:
				_printf("call CAUSE_FETCH_ACCESS\n");
				break;
			case CAUSE_ILLEGAL_INSTRUCTION:
				_printf("call CAUSE_ILLEGAL_INSTRUCTION\n");
				break;
			case CAUSE_BREAKPOINT:
				_printf("call CAUSE_BREAKPOINT\n");
				break;
			case CAUSE_MISALIGNED_LOAD:
				_printf("call CAUSE_MISALIGNED_LOAD\n");
				break;
			case CAUSE_LOAD_ACCESS:
				_printf("call CAUSE_LOAD_ACCESS\n");
				break;
			case CAUSE_MISALIGNED_STORE:
				_printf("call CAUSE_MISALIGNED_STORE\n");
				break;
			case CAUSE_STORE_ACCESS:
				_printf("call CAUSE_STORE_ACCESS\n");
				break;
			case CAUSE_USER_ECALL:
				_printf("call CAUSE_USER_ECALL\n");
				break;
			case CAUSE_SUPERVISOR_ECALL:
				_printf("call CAUSE_SUPERVISOR_ECALL\n");
				break;
			case CAUSE_HYPERVISOR_ECALL:
				_printf("call CAUSE_HYPERVISOR_ECALL\n");
				break;
			case CAUSE_MACHINE_ECALL:
				_printf("call CAUSE_MACHINE_ECALL\n");
				break;
			case CAUSE_FETCH_PAGE_FAULT:
				_printf("call CAUSE_FETCH_PAGE_FAULT\n");
				break;
			case CAUSE_LOAD_PAGE_FAULT:
				_printf("call CAUSE_LOAD_PAGE_FAULT\n");
				break;
			case CAUSE_STORE_PAGE_FAULT:
				_printf("call CAUSE_STORE_PAGE_FAULT\n");
				break;
			default:
				_printf("call IRQ0-default(%d)\n", irq_num);
		}
	}
}
/*-----------------------------------------------------------*/


static uint32_t uiTotalHeapMemorySize = 0;
void vMemoryHeapInit(void)
{
	/* Allocate two blocks of RAM for use by the heap.  The first is a block of
	0x10000 bytes starting from address 0x80000000, and the second a block of
	0xa0000 bytes starting from address 0x90000000.  The block starting at
	0x80000000 has the lower start address so appears in the array fist. */
	const HeapRegion_t xHeapRegions[] = {
/*	    { ( uint8_t * ) 0x80100000UL, 0x100000 }, */
	    { ( uint8_t * ) 0x80200000UL, 0x100000 },
	    { NULL, 0 } /* Terminates the array. */
	};

	uiTotalHeapMemorySize = 0;
	int len = sizeof(xHeapRegions) / sizeof(xHeapRegions[0]);
	for (int i = 0; i < len; i++) {
		_printf("Heap[%d] memory 0x%08X ~ 0x%08X\r\n", i, xHeapRegions[i].pucStartAddress, xHeapRegions[i].pucStartAddress + xHeapRegions[i].xSizeInBytes);
		memset(xHeapRegions[i].pucStartAddress, 0, xHeapRegions[i].xSizeInBytes);
		uiTotalHeapMemorySize += xHeapRegions[i].xSizeInBytes;
	}

	/* Pass the array into vPortDefineHeapRegions(). */
	vPortDefineHeapRegions( xHeapRegions );
}
/*-----------------------------------------------------------*/


void vMemoryHeapInfoPrint(void)
{
	size_t nPortGetFreeHeapSize = xPortGetFreeHeapSize();
	size_t nPortGetMinimumEverFreeHeapSize = xPortGetMinimumEverFreeHeapSize();
	_printf("Total Heap Size            : %lu\n", uiTotalHeapMemorySize);
	_printf(" Free Heap Size            : %lu\n", nPortGetFreeHeapSize);
	_printf("Minimum Ever Free Heap Size: %lu\n", nPortGetMinimumEverFreeHeapSize);
	_printf("                      Used : %lu%%\n", 100 - (nPortGetFreeHeapSize * 100) / uiTotalHeapMemorySize);
}
/*-----------------------------------------------------------*/


void vTaskInfoPrint(void)
{
#define TaskStatusCount 40
	uint32_t uiTotal = 0;
	UBaseType_t uxTask = 0;
	TaskStatus_t etiList[TaskStatusCount];

	memset(etiList, 0, sizeof(etiList));
	uxTask = uxTaskGetSystemState(etiList, TaskStatusCount, &uiTotal);

	_printf("---------------------------------Task List(%02d)----------------------------------\n", uxTask);
	_printf(" TaskID  Task     State  Priority  Remained/Stack    RunTimeCounter  Percentage\n");
	_printf("--------------------------------------------------------------------------------\n");
	for (UBaseType_t i = 0; i < uxTask; i++) {
		_printf(" %-6lu %-8s    %d       %lu/%lu    %4lu/0x%08X   %14lu     %-lu%%\n",
				etiList[i].xTaskNumber,
				etiList[i].pcTaskName,
				etiList[i].eCurrentState,
				etiList[i].uxCurrentPriority,
				etiList[i].uxBasePriority,
				etiList[i].usStackHighWaterMark,
				etiList[i].pxStackBase,
				etiList[i].ulRunTimeCounter,
				(etiList[i].ulRunTimeCounter * 100) / uiTotal
				);
	}
	_printf("--------------------------------------------------------------------------------\n");
	_printf("-State(0:Run 1:Ready 2:Blocked 4:Deleted 3:Suspended) TotalRunTime(%10lu)--\n", uiTotal);
}
/*-----------------------------------------------------------*/


TaskHandle_t vTaskCreate(const char *name, TaskFunction_t thread, void *arg, int stacksize, int prio)
{
	TaskHandle_t objTask;
	int result = xTaskCreate(thread, (const char * const)name, stacksize/4, arg, prio, &objTask);
	if (result == pdPASS) {
		_printf("vTaskCreate : pass : %s(%x) -size(%d)\n", name, objTask, stacksize);
		return objTask;
	} else {
		_printf("vTaskCreate : fail : %s(%x) -size(%d)\n", name, objTask, stacksize);
		return NULL;
	}
}
/*-----------------------------------------------------------*/
