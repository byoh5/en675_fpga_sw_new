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

#if __riscv_xlen == 64
#define STORE			sd
#define LOAD			ld
#define REGBYTES		8
#else
#define STORE			sw
#define LOAD			lw
#define REGBYTES		4
#endif

#define INT_MASK		0x7FFFFFFF

/* Enable interrupts when returning from the handler */
#define MSTATUS_PRV1	0x1880

.global xPortStartScheduler
.global vPortYield
.global vTaskIncrementTick
.global vPortEndScheduler
.global xExitStack
.global portInterrupt

/* Macro for saving task context */
.macro pushREGFILE
	/* make room in stack */
	addi	sp, sp, -REGBYTES * 32

	/* Save Context */
	STORE	x1, 0x0(sp)
	STORE	x2, 1 * REGBYTES(sp)
	STORE	x3, 2 * REGBYTES(sp)
	STORE	x4, 3 * REGBYTES(sp)
	STORE	x5, 4 * REGBYTES(sp)
	STORE	x6, 5 * REGBYTES(sp)
	STORE	x7, 6 * REGBYTES(sp)
	STORE	x8, 7 * REGBYTES(sp)
	STORE	x9, 8 * REGBYTES(sp)
	STORE	x10, 9 * REGBYTES(sp)
	STORE	x11, 10 * REGBYTES(sp)
	STORE	x12, 11 * REGBYTES(sp)
	STORE	x13, 12 * REGBYTES(sp)
	STORE	x14, 13 * REGBYTES(sp)
	STORE	x15, 14 * REGBYTES(sp)
	STORE	x16, 15 * REGBYTES(sp)
	STORE	x17, 16 * REGBYTES(sp)
	STORE	x18, 17 * REGBYTES(sp)
	STORE	x19, 18 * REGBYTES(sp)
	STORE	x20, 19 * REGBYTES(sp)
	STORE	x21, 20 * REGBYTES(sp)
	STORE	x22, 21 * REGBYTES(sp)
	STORE	x23, 22 * REGBYTES(sp)
	STORE	x24, 23 * REGBYTES(sp)
	STORE	x25, 24 * REGBYTES(sp)
	STORE	x26, 25 * REGBYTES(sp)
	STORE	x27, 26 * REGBYTES(sp)
	STORE	x28, 27 * REGBYTES(sp)
	STORE	x29, 28 * REGBYTES(sp)
	STORE	x30, 29 * REGBYTES(sp)
	STORE	x31, 30 * REGBYTES(sp)
.endm

/* Saves mstatus and TCB */
.macro portSAVE_CONTEXT
	.global	pxCurrentTCB

	/* Store current stackpointer in task control block (TCB) */
	LOAD	t0, pxCurrentTCB	// pointer
	STORE	sp, 0x0(t0)
.endm

/* Saves current error program counter (EPC) as task program counter */
.macro portSAVE_EPC
  	csrr	t0, mepc
	STORE	t0, 31 * REGBYTES(sp)
.endm

/* Saves current return address (RA) as task program counter */
.macro portSAVE_RA
	STORE	ra, 31 * REGBYTES(sp)
.endm

/* Macro for restoring task context */
.macro portRESTORE_CONTEXT
	.global	pxCurrentTCB

	/* Load stack pointer from the current TCB */
	LOAD	sp, pxCurrentTCB
	LOAD	sp, 0x0(sp)

	/* Load task program counter */
	LOAD	t0, 31 * REGBYTES(sp)
	csrw	mepc, t0

	/* Run in machine mode */
  	li 		t0, MSTATUS_PRV1
  	csrs	mstatus, t0
.endm

/* Macro for restoring task context */
.macro popREGFILE
	/* Restore registers,
	Skip global pointer because that does not change */
	LOAD	x1, 0x0(sp)
	LOAD	x4, 3 * REGBYTES(sp)
	LOAD	x5, 4 * REGBYTES(sp)
	LOAD	x6, 5 * REGBYTES(sp)
	LOAD	x7, 6 * REGBYTES(sp)
	LOAD	x8, 7 * REGBYTES(sp)
	LOAD	x9, 8 * REGBYTES(sp)
	LOAD	x10, 9 * REGBYTES(sp)
	LOAD	x11, 10 * REGBYTES(sp)
	LOAD	x12, 11 * REGBYTES(sp)
	LOAD	x13, 12 * REGBYTES(sp)
	LOAD	x14, 13 * REGBYTES(sp)
	LOAD	x15, 14 * REGBYTES(sp)
	LOAD	x16, 15 * REGBYTES(sp)
	LOAD	x17, 16 * REGBYTES(sp)
	LOAD	x18, 17 * REGBYTES(sp)
	LOAD	x19, 18 * REGBYTES(sp)
	LOAD	x20, 19 * REGBYTES(sp)
	LOAD	x21, 20 * REGBYTES(sp)
	LOAD	x22, 21 * REGBYTES(sp)
	LOAD	x23, 22 * REGBYTES(sp)
	LOAD	x24, 23 * REGBYTES(sp)
	LOAD	x25, 24 * REGBYTES(sp)
	LOAD	x26, 25 * REGBYTES(sp)
	LOAD	x27, 26 * REGBYTES(sp)
	LOAD	x28, 27 * REGBYTES(sp)
	LOAD	x29, 28 * REGBYTES(sp)
	LOAD	x30, 29 * REGBYTES(sp)
	LOAD	x31, 30 * REGBYTES(sp)

	addi	sp, sp, REGBYTES * 32
.endm

/*
	function. xPortStartScheduler
	function. vPortEndScheduler
	function. vPortYield
	function. portMtimeIRQ
	function. portHandleInterrupt
	function. portInterrupt
*/

.align 8
xPortStartScheduler:
#ifdef __gracefulExit
	/* Stores context when starting the scheduler in xStartContext.
	   This is used for when you want to gracefully exit the scheduler.
	   For example if you want to test multiple instances after each other in one test suite.
     */
	la		t0, xStartContext
	STORE	x1, 0x0(t0)
	STORE	x2, 1 * REGBYTES(t0)
	STORE	x3, 2 * REGBYTES(t0)
	STORE	x4, 3 * REGBYTES(t0)
	STORE	x5, 4 * REGBYTES(t0)
	STORE	x6, 5 * REGBYTES(t0)
	STORE	x7, 6 * REGBYTES(t0)
	STORE	x8, 7 * REGBYTES(t0)
	STORE	x9, 8 * REGBYTES(t0)
	STORE	x10, 9 * REGBYTES(t0)
	STORE	x11, 10 * REGBYTES(t0)
	STORE	x12, 11 * REGBYTES(t0)
	STORE	x13, 12 * REGBYTES(t0)
	STORE	x14, 13 * REGBYTES(t0)
	STORE	x15, 14 * REGBYTES(t0)
	STORE	x16, 15 * REGBYTES(t0)
	STORE	x17, 16 * REGBYTES(t0)
	STORE	x18, 17 * REGBYTES(t0)
	STORE	x19, 18 * REGBYTES(t0)
	STORE	x20, 19 * REGBYTES(t0)
	STORE	x21, 20 * REGBYTES(t0)
	STORE	x22, 21 * REGBYTES(t0)
	STORE	x23, 22 * REGBYTES(t0)
	STORE	x24, 23 * REGBYTES(t0)
	STORE	x25, 24 * REGBYTES(t0)
	STORE	x26, 25 * REGBYTES(t0)
	STORE	x27, 26 * REGBYTES(t0)
	STORE	x28, 27 * REGBYTES(t0)
	STORE	x29, 28 * REGBYTES(t0)
	STORE	x30, 29 * REGBYTES(t0)
	STORE	x31, 30 * REGBYTES(t0)
#endif
	jal		vPortSetupTimer
	portRESTORE_CONTEXT
	popREGFILE
	mret
	//portRESTORE_CONTEXT

vPortEndScheduler:
#ifdef __gracefulExit
	/* Load current context from xStartContext */
	la		t0, xStartContext
	LOAD	x1, 0x0(t0)
	LOAD	x2, 1 * REGBYTES(t0)
	LOAD	x3, 2 * REGBYTES(t0)
	LOAD	x4, 3 * REGBYTES(t0)
	LOAD	x5, 4 * REGBYTES(t0)
	LOAD	x6, 5 * REGBYTES(t0)
	LOAD	x7, 6 * REGBYTES(t0)
	LOAD	x8, 7 * REGBYTES(t0)
	LOAD	x9, 8 * REGBYTES(t0)
	LOAD	x10, 9 * REGBYTES(t0)
	LOAD	x11, 10 * REGBYTES(t0)
	LOAD	x12, 11 * REGBYTES(t0)
	LOAD	x13, 12 * REGBYTES(t0)
	LOAD	x14, 13 * REGBYTES(t0)
	LOAD	x15, 14 * REGBYTES(t0)
	LOAD	x16, 15 * REGBYTES(t0)
	LOAD	x17, 16 * REGBYTES(t0)
	LOAD	x18, 17 * REGBYTES(t0)
	LOAD	x19, 18 * REGBYTES(t0)
	LOAD	x20, 19 * REGBYTES(t0)
	LOAD	x21, 20 * REGBYTES(t0)
	LOAD	x22, 21 * REGBYTES(t0)
	LOAD	x23, 22 * REGBYTES(t0)
	LOAD	x24, 23 * REGBYTES(t0)
	LOAD	x25, 24 * REGBYTES(t0)
	LOAD	x26, 25 * REGBYTES(t0)
	LOAD	x27, 26 * REGBYTES(t0)
	LOAD	x28, 27 * REGBYTES(t0)
	LOAD	x29, 28 * REGBYTES(t0)
	LOAD	x30, 39 * REGBYTES(t0)
	LOAD	x31, 30 * REGBYTES(t0)
#endif
	ret

vPortYield:
	/*
	*  This routine can be called from outside of interrupt handler. This means
	*  interrupts may be enabled at this point. This is probably okay for registers and
	*  stack. However, "mepc" will be overwritten by the interrupt handler if a timer
	*  interrupt happens during the yield. To avoid this, prevent interrupts before starting.
	*  The write to mstatus in the restore context routine will enable interrupts after the
	*  mret. A more fine-grain lock may be possible.
	*/
	csrci	mstatus, 8

	pushREGFILE
	portSAVE_CONTEXT
	portSAVE_RA
	jal		vTaskSwitchContext
	portRESTORE_CONTEXT
	popREGFILE
	mret

/* Interupt is m-time  */
portMtimeIRQ:
	// pushREGFILE
	portSAVE_CONTEXT
	portSAVE_EPC
	jal		vPortSysTickHandler
	portRESTORE_CONTEXT
	popREGFILE
	mret

portHandleInterrupt:
	j		portTrapFromMachineMode

/* async interrupt, this function is called */
portInterrupt:
	li      t0, INT_MASK
	and     a0, a0, t0
	li      t0, 0x7
	beq     a0, t0, portMtimeIRQ
	/* Interupt not m-time interrupt */
	/* pass the exception code in a0 */
	jal 	portHandleInterrupt
	popREGFILE
	mret
