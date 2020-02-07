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

/*
 * The FreeRTOS kernel's RISC-V port is split between the the code that is
 * common across all currently supported RISC-V chips (implementations of the
 * RISC-V ISA), and code which tailors the port to a specific RISC-V chip:
 *
 * + The code that is common to all RISC-V chips is implemented in
 *   FreeRTOS\Source\portable\GCC\RISC-V-RV32\portASM.S.  There is only one
 *   portASM.S file because the same file is used no matter which RISC-V chip is
 *   in use.
 *
 * + The code that tailors the kernel's RISC-V port to a specific RISC-V
 *   chip is implemented in freertos_risc_v_chip_specific_extensions.h.  There
 *   is one freertos_risc_v_chip_specific_extensions.h that can be used with any
 *   RISC-V chip that both includes a standard CLINT and does not add to the
 *   base set of RISC-V registers.  There are additional
 *   freertos_risc_v_chip_specific_extensions.h files for RISC-V implementations
 *   that do not include a standard CLINT or do add to the base set of RISC-V
 *   registers.
 *
 * CARE MUST BE TAKEN TO INCLDUE THE CORRECT
 * freertos_risc_v_chip_specific_extensions.h HEADER FILE FOR THE CHIP
 * IN USE.  To include the correct freertos_risc_v_chip_specific_extensions.h
 * header file ensure the path to the correct header file is in the assembler's
 * include path.
 *
 * This freertos_risc_v_chip_specific_extensions.h is for use on RISC-V chips
 * that include a standard CLINT and do not add to the base set of RISC-V
 * registers.
 *
 */
#if __riscv_xlen == 64
	#define portWORD_SIZE	8
	#define STORE			sd
	#define LOAD			ld
#elif __riscv_xlen == 32
	#define STORE			sw
	#define LOAD			lw
	#define portWORD_SIZE	4
#else
	#error Assembler did not define __riscv_xlen
#endif

/* Only the standard core registers are stored by default. where the global 
 * and thread pointers are currently assumed to be constant so are not saved.
 */
#define portCONTEXT_SIZE ( 32 * portWORD_SIZE )

.global xPortStartFirstTask
.global freertos_risc_v_trap_handler
.global pxPortInitialiseStack
.extern pxCurrentTCB

.extern prvTaskExitError
.extern pullMachineTimerCompareRegister
.extern pullNextTime

//.extern trap_from_machine_mode_freertos
.extern trap_from_machine_mode_freertos_sync
.extern trap_from_machine_mode_freertos_async

/*-----------------------------------------------------------*/

.align 8
.func
freertos_risc_v_trap_handler:
	addi	sp, sp, -portCONTEXT_SIZE
	STORE	x1,   1 * portWORD_SIZE( sp )
	STORE	x5,   2 * portWORD_SIZE( sp )
	STORE	x6,   3 * portWORD_SIZE( sp )
	STORE	x7,   4 * portWORD_SIZE( sp )
	STORE	x8,   5 * portWORD_SIZE( sp )
	STORE	x9,   6 * portWORD_SIZE( sp )
	STORE	x10,  7 * portWORD_SIZE( sp )
	STORE	x11,  8 * portWORD_SIZE( sp )
	STORE	x12,  9 * portWORD_SIZE( sp )
	STORE	x13, 10 * portWORD_SIZE( sp )
	STORE	x14, 11 * portWORD_SIZE( sp )
	STORE	x15, 12 * portWORD_SIZE( sp )
	STORE	x16, 13 * portWORD_SIZE( sp )
	STORE	x17, 14 * portWORD_SIZE( sp )
	STORE	x18, 15 * portWORD_SIZE( sp )
	STORE	x19, 16 * portWORD_SIZE( sp )
	STORE	x20, 17 * portWORD_SIZE( sp )
	STORE	x21, 18 * portWORD_SIZE( sp )
	STORE	x22, 19 * portWORD_SIZE( sp )
	STORE	x23, 20 * portWORD_SIZE( sp )
	STORE	x24, 21 * portWORD_SIZE( sp )
	STORE	x25, 22 * portWORD_SIZE( sp )
	STORE	x26, 23 * portWORD_SIZE( sp )
	STORE	x27, 24 * portWORD_SIZE( sp )
	STORE	x28, 25 * portWORD_SIZE( sp )
	STORE	x29, 26 * portWORD_SIZE( sp )
	STORE	x30, 27 * portWORD_SIZE( sp )
	STORE	x31, 28 * portWORD_SIZE( sp )

	csrr	t0, mstatus						/* Required for MPIE bit. */
	STORE	t0, 29 * portWORD_SIZE(sp)

	LOAD	t0, pxCurrentTCB				/* Load pxCurrentTCB. */
	STORE	sp, 0( t0 )				 		/* Write sp to first TCB member. */

	csrr	a0, mcause
	csrr	a1, mepc
	csrr	a2, mhartid
	mv		a3, sp

test_if_asynchronous:
	srli	t0, a0, 0x3f					/* MSB of mcause is 1 if handing an asynchronous interrupt - shift to LSB to clear other bits. */
	bne		t0, x0, handle_asynchronous		/* Branch past interrupt handing if not synchronous. */

handle_synchronous:
	addi	a1, a1, 4						/* Synchronous so updated exception return address to the instruction after the instruction that generated the exeption. */
	STORE	a1, 0( sp )						/* So save exception return address. */
	LOAD	sp, xISRStackTop				/* Switch to ISR stack before function call. */
	jal		trap_from_machine_mode_freertos_sync
	jal		processed_source

handle_asynchronous:
	STORE	a1, 0( sp )						/* So save exception return address. */
	LOAD	sp, xISRStackTop				/* Switch to ISR stack before function call. */
	jal		trap_from_machine_mode_freertos_async

processed_source:
	LOAD	t1, pxCurrentTCB				/* Load pxCurrentTCB. */
	LOAD	sp, 0( t1 )				 		/* Read sp from first TCB member. */

	/* Load mret with the address of the next instruction in the task to run next. */
	LOAD	t0, 0( sp )
	csrw	mepc, t0

	/* Load mstatus with the interrupt enable bits used by the task. */
	LOAD	t0,  29 * portWORD_SIZE( sp )
	csrw	mstatus, t0						/* Required for MPIE bit. */

	LOAD	x1,   1 * portWORD_SIZE( sp )
	LOAD	x5,   2 * portWORD_SIZE( sp )	/* t0 */
	LOAD	x6,   3 * portWORD_SIZE( sp )	/* t1 */
	LOAD	x7,   4 * portWORD_SIZE( sp )	/* t2 */
	LOAD	x8,   5 * portWORD_SIZE( sp )	/* s0/fp */
	LOAD	x9,   6 * portWORD_SIZE( sp )	/* s1 */
	LOAD	x10,  7 * portWORD_SIZE( sp )	/* a0 */
	LOAD	x11,  8 * portWORD_SIZE( sp )	/* a1 */
	LOAD	x12,  9 * portWORD_SIZE( sp )	/* a2 */
	LOAD	x13, 10 * portWORD_SIZE( sp )	/* a3 */
	LOAD	x14, 11 * portWORD_SIZE( sp )	/* a4 */
	LOAD	x15, 12 * portWORD_SIZE( sp )	/* a5 */
	LOAD	x16, 13 * portWORD_SIZE( sp )	/* a6 */
	LOAD	x17, 14 * portWORD_SIZE( sp )	/* a7 */
	LOAD	x18, 15 * portWORD_SIZE( sp )	/* s2 */
	LOAD	x19, 16 * portWORD_SIZE( sp )	/* s3 */
	LOAD	x20, 17 * portWORD_SIZE( sp )	/* s4 */
	LOAD	x21, 18 * portWORD_SIZE( sp )	/* s5 */
	LOAD	x22, 19 * portWORD_SIZE( sp )	/* s6 */
	LOAD	x23, 20 * portWORD_SIZE( sp )	/* s7 */
	LOAD	x24, 21 * portWORD_SIZE( sp )	/* s8 */
	LOAD	x25, 22 * portWORD_SIZE( sp )	/* s9 */
	LOAD	x26, 23 * portWORD_SIZE( sp )	/* s10 */
	LOAD	x27, 24 * portWORD_SIZE( sp )	/* s11 */
	LOAD	x28, 25 * portWORD_SIZE( sp )	/* t3 */
	LOAD	x29, 26 * portWORD_SIZE( sp )	/* t4 */
	LOAD	x30, 27 * portWORD_SIZE( sp )	/* t5 */
	LOAD	x31, 28 * portWORD_SIZE( sp )	/* t6 */
	addi	sp, sp, portCONTEXT_SIZE

	mret
	.endfunc
/*-----------------------------------------------------------*/

.align 8
.func
xPortStartFirstTask:
	la		t0, freertos_risc_v_trap_handler
	csrw	mtvec, t0

	LOAD	sp, pxCurrentTCB				/* Load pxCurrentTCB. */
	LOAD	sp, 0( sp )						/* Read sp from first TCB member. */

	LOAD	x1, 0( sp )	/* Note for starting the scheduler the exception return address is used as the function return address. */

	LOAD	t0,  29 * portWORD_SIZE( sp )	/* mstatus */
	csrrw	x0, mstatus, t0					/* Interrupts enabled from here! */

	LOAD	x5,   2 * portWORD_SIZE( sp )	/* t0 */
	LOAD	x6,   3 * portWORD_SIZE( sp )	/* t1 */
	LOAD	x7,   4 * portWORD_SIZE( sp )	/* t2 */
	LOAD	x8,   5 * portWORD_SIZE( sp )	/* s0/fp */
	LOAD	x9,   6 * portWORD_SIZE( sp )	/* s1 */
	LOAD	x10,  7 * portWORD_SIZE( sp )	/* a0 */
	LOAD	x11,  8 * portWORD_SIZE( sp )	/* a1 */
	LOAD	x12,  9 * portWORD_SIZE( sp )	/* a2 */
	LOAD	x13, 10 * portWORD_SIZE( sp )	/* a3 */
	LOAD	x14, 11 * portWORD_SIZE( sp )	/* a4 */
	LOAD	x15, 12 * portWORD_SIZE( sp )	/* a5 */
	LOAD	x16, 13 * portWORD_SIZE( sp )	/* a6 */
	LOAD	x17, 14 * portWORD_SIZE( sp )	/* a7 */
	LOAD	x18, 15 * portWORD_SIZE( sp )	/* s2 */
	LOAD	x19, 16 * portWORD_SIZE( sp )	/* s3 */
	LOAD	x20, 17 * portWORD_SIZE( sp )	/* s4 */
	LOAD	x21, 18 * portWORD_SIZE( sp )	/* s5 */
	LOAD	x22, 19 * portWORD_SIZE( sp )	/* s6 */
	LOAD	x23, 20 * portWORD_SIZE( sp )	/* s7 */
	LOAD	x24, 21 * portWORD_SIZE( sp )	/* s8 */
	LOAD	x25, 22 * portWORD_SIZE( sp )	/* s9 */
	LOAD	x26, 23 * portWORD_SIZE( sp )	/* s10 */
	LOAD	x27, 24 * portWORD_SIZE( sp )	/* s11 */
	LOAD	x28, 25 * portWORD_SIZE( sp )	/* t3 */
	LOAD	x29, 26 * portWORD_SIZE( sp )	/* t4 */
	LOAD	x30, 27 * portWORD_SIZE( sp )	/* t5 */
	LOAD	x31, 28 * portWORD_SIZE( sp )	/* t6 */
	addi	sp,	sp, portCONTEXT_SIZE
	ret
	.endfunc
/*-----------------------------------------------------------*/

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
 * pvParameters
 * x9
 * x8
 * x7
 * x6
 * x5
 * portTASK_RETURN_ADDRESS
 * [chip specific registers go here]
 * pxCode
 */
.align 8
.func
pxPortInitialiseStack:

	csrr	t0, mstatus						/* Obtain current mstatus value. */
	addi	t1, x0, 0x188					/* Generate the value 0x1880, which are the MPIE and MPP bits to set in mstatus. */
	slli	t1, t1, 4
	or		t0, t0, t1						/* Set MPIE and MPP bits in mstatus value. */

	la		t1, prvTaskExitError			/* Task return address */

	addi	a0, a0, -portWORD_SIZE
	STORE	t0, 0(a0)						/* mstatus onto the stack. */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x31 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x30 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x29 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x28 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x27 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x26 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x25 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x24 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x23 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x22 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x21 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x20 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x19 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x18 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x17 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x16 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x15 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x14 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x13 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x12 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x11 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	a2, 0(a0)						/* x10 - Task parameters (pvParameters parameter) goes into register X10/a0 on the stack. */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x9 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x8 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x7 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x6 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	x0, 0(a0)						/* x5 - zero */
	addi	a0, a0, -portWORD_SIZE
	STORE	t1, 0(a0)						/* Return address onto the stack, could be portTASK_RETURN_ADDRESS */
	addi	a0, a0, -portWORD_SIZE
	STORE	a1, 0(a0)						/* mret value (pxCode parameter) onto the stack. */
	ret
	.endfunc
/*-----------------------------------------------------------*/
