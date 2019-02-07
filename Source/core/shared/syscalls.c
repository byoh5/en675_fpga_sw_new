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

#include <stdint.h>
#include <string.h>
#include "syscalls.h"
#include "encoding.h"
#include "clib.h"

volatile uint64_t* mtime =      (uint64_t*)(CLINT_BASE + 0xbff8);
volatile uint64_t* timecmp =    (uint64_t*)(CLINT_BASE + 0x4000);

volatile uint64_t tohost __attribute__((aligned(64)));
volatile uint64_t fromhost __attribute__((aligned(64)));

uint64_t zeroExtend(long val)
{
	uint64_t ret = val;
#if __riscv_xlen == 32
	ret = (0x00000000ffffffff & val);
#endif
	return ret;
}

/* Relay syscall to host */
static uint64_t prvSyscallToHost(long which, long arg0, long arg1, long arg2)
{
	volatile uint64_t magic_mem[8] __attribute__((aligned(64)));
//    volatile uint64_t oldfromhost;
	magic_mem[0] = zeroExtend(which);
	magic_mem[1] = zeroExtend(arg0);
	magic_mem[2] = zeroExtend(arg1);
	magic_mem[3] = zeroExtend(arg2);
	__sync_synchronize();
    tohost = zeroExtend((long)magic_mem);
//    do
//    {
//        oldfromhost = fromhost;
//        fromhost = 0;
//    } while (oldfromhost == 0);
	return magic_mem[0];
}
/*-----------------------------------------------------------*/

/* Exit systemcall */
static void prvSyscallExit(long code)
{
	uint64_t zcode = zeroExtend(code);
	tohost = ((zcode << 1) | 1);
	for(;;) { }
}
/*-----------------------------------------------------------*/

/* Prints a string with a syscall  */
static void printstr(const char* s)
{
	syscall(SYS_write, 1, (long) s, strlen(s));
}
/*-----------------------------------------------------------*/

/* Fires a syscall */
long syscall(long num, long arg0, long arg1, long arg2)
{
	register long a7 asm("a7") = num;
	register long a0 asm("a0") = arg0;
	register long a1 asm("a1") = arg1;
	register long a2 asm("a2") = arg2;
	asm volatile ("scall":"+r"(a0) : "r"(a1), "r"(a2), "r"(a7));
	return a0;
}
/*-----------------------------------------------------------*/

/* Programs need to override this function. */
int __attribute__((weak)) main(__attribute__ ((unused)) int argc, __attribute__ ((unused)) char** argv)
{
	printstr("Implement a main function!\n");
	return -1;
}
/*-----------------------------------------------------------*/

/* Starts main function. */
void vSyscallInit(void)
{
	int ret = main(0, 0);
	exit(ret);
}
/*-----------------------------------------------------------*/

/* Trap handeler */
unsigned long ulSyscallTrap(long cause, long epc, long regs[32])
{
	long returnValue = 0;
	if (cause != CAUSE_MACHINE_ECALL) {
		prvSyscallExit(cause);
	} else if (regs[17] == SYS_exit) {
		prvSyscallExit(regs[10]);
	} else {
		returnValue = prvSyscallToHost(regs[17], regs[10], regs[11], regs[12]);
	}

	regs[10] = returnValue;
	return epc + 4;
}
/*-----------------------------------------------------------*/
