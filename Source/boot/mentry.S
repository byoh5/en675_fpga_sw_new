/******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:     mentry.S
//	Description:   EN675 reset
//	Author:        Team 2
//
//	Note:
//
//	Ver   Date   Author  Changes
//	---  ------  ------  -------
//	0.1  190200  hjlee   support EN675
//	0.2  190220  hjlee   update trap_firmware
//------------------------------------------------------------------------------*/

#include "mtrap.h"
#include "bits.h"
#include "asm.h"

  .data
  .align 6
  WORD_ENTRY(_bss_s   )
  WORD_ENTRY(_bss_e   )
  WORD_ENTRY(heap_start)
  WORD_ENTRY(heap_end)

  .option norvc
  .section .text.init,"ax",@progbits
  .globl	reset_vector
  .globl	trap_firmware

reset_vector:
	j		do_reset

trap_firmware:
	/* Make room in stack */
	addi	sp, sp, -32 * REGBYTES

	/* Save Context */
	STORE	x1,   1 * REGBYTES(sp)
	STORE	x2,   2 * REGBYTES(sp)
	STORE	x3,   3 * REGBYTES(sp)
	STORE	x4,   4 * REGBYTES(sp)
	STORE	x5,   5 * REGBYTES(sp)
	STORE	x6,   6 * REGBYTES(sp)
	STORE	x7,   7 * REGBYTES(sp)
	STORE	x8,   8 * REGBYTES(sp)
	STORE	x9,   9 * REGBYTES(sp)
	STORE	x10, 10 * REGBYTES(sp)
	STORE	x11, 11 * REGBYTES(sp)
	STORE	x12, 12 * REGBYTES(sp)
	STORE	x13, 13 * REGBYTES(sp)
	STORE	x14, 14 * REGBYTES(sp)
	STORE	x15, 15 * REGBYTES(sp)
	STORE	x16, 16 * REGBYTES(sp)
	STORE	x17, 17 * REGBYTES(sp)
	STORE	x18, 18 * REGBYTES(sp)
	STORE	x19, 19 * REGBYTES(sp)
	STORE	x20, 20 * REGBYTES(sp)
	STORE	x21, 21 * REGBYTES(sp)
	STORE	x22, 22 * REGBYTES(sp)
	STORE	x23, 23 * REGBYTES(sp)
	STORE	x24, 24 * REGBYTES(sp)
	STORE	x25, 25 * REGBYTES(sp)
	STORE	x26, 26 * REGBYTES(sp)
	STORE	x27, 27 * REGBYTES(sp)
	STORE	x28, 28 * REGBYTES(sp)
	STORE	x29, 29 * REGBYTES(sp)
	STORE	x30, 30 * REGBYTES(sp)
	STORE	x31, 31 * REGBYTES(sp)

	csrr	a0, mcause
	csrr	a1, mepc
	csrr	a2, mhartid
	mv		a3, sp

fw_test_if_asynchronous:
	srli	t0, a0, 0x3f					/* MSB of mcause is 1 if handing an asynchronous interrupt - shift to LSB to clear other bits. */
	bne		t0, x0, fw_handle_asynchronous	/* Branch past interrupt handing if not synchronous. */

fw_handle_synchronous:
	call	trap_from_machine_mode_sync
	jal		fw_processed_source

fw_handle_asynchronous:
	call	trap_from_machine_mode_async

fw_processed_source:
	csrw	mepc, a0

	/* Remain in M-mode after mret */
	li		t0, MSTATUS_MPP
	csrs	mstatus, t0

	/* Load Context */
	LOAD	x1,   1 * REGBYTES(sp)
	LOAD	x2,   2 * REGBYTES(sp)
	LOAD	x3,   3 * REGBYTES(sp)
	LOAD	x4,   4 * REGBYTES(sp)
	LOAD	x5,   5 * REGBYTES(sp)
	LOAD	x6,   6 * REGBYTES(sp)
	LOAD	x7,   7 * REGBYTES(sp)
	LOAD	x8,   8 * REGBYTES(sp)
	LOAD	x9,   9 * REGBYTES(sp)
	LOAD	x10, 10 * REGBYTES(sp)
	LOAD	x11, 11 * REGBYTES(sp)
	LOAD	x12, 12 * REGBYTES(sp)
	LOAD	x13, 13 * REGBYTES(sp)
	LOAD	x14, 14 * REGBYTES(sp)
	LOAD	x15, 15 * REGBYTES(sp)
	LOAD	x16, 16 * REGBYTES(sp)
	LOAD	x17, 17 * REGBYTES(sp)
	LOAD	x18, 18 * REGBYTES(sp)
	LOAD	x19, 19 * REGBYTES(sp)
	LOAD	x20, 20 * REGBYTES(sp)
	LOAD	x21, 21 * REGBYTES(sp)
	LOAD	x22, 22 * REGBYTES(sp)
	LOAD	x23, 23 * REGBYTES(sp)
	LOAD	x24, 24 * REGBYTES(sp)
	LOAD	x25, 25 * REGBYTES(sp)
	LOAD	x26, 26 * REGBYTES(sp)
	LOAD	x27, 27 * REGBYTES(sp)
	LOAD	x28, 28 * REGBYTES(sp)
	LOAD	x29, 29 * REGBYTES(sp)
	LOAD	x30, 30 * REGBYTES(sp)
	LOAD	x31, 31 * REGBYTES(sp)

	addi	sp, sp, 32 * REGBYTES
	mret

do_reset:
  li x1, 0
  li x2, 0
  li x3, 0
  li x4, 0
  li x5, 0
  li x6, 0
  li x7, 0
  li x8, 0
  li x9, 0
  li x10, 0
  li x11, 0
  li x12, 0
  li x13, 0
  li x14, 0
  li x15, 0
  li x16, 0
  li x17, 0
  li x18, 0
  li x19, 0
  li x20, 0
  li x21, 0
  li x22, 0
  li x23, 0
  li x24, 0
  li x25, 0
  li x26, 0
  li x27, 0
  li x28, 0
  li x29, 0
  li x30, 0
  li x31, 0

  csrw mie, x0			// 190117 hjlee, Disable all interrupts.
  csrw mip, x0			// 190418 hjlee, Disable all interrupts.
  csrw mideleg, x0		// 190418 hjlee, Disable all interrupts.
  csrw medeleg, x0		// 190418 hjlee, Disable all interrupts.
  csrw mscratch, x0

#init_bss: // cpu0 only
#  /* cpu0? */
#  csrr a0, mhartid
#  li a4, 0 // If you are debugging a specific cpu, put the cpuid you are debugging in. Default CPU0 == 0
#  bne a0, a4, init_next
#  /* init bss section */
#  la a0, __bss_s
#  la a1, __bss_e-4 /* section end is actually the start of the next section */
#  li a2, 0x0
#  jal fill_block

init_next:
  # write mtvec and make sure it sticks
  la t0, trap_firmware
  csrw mtvec, t0
  csrr t1, mtvec
1:bne t0, t1, 1b

  #la sp, stacks + RISCV_PGSIZE - MENTRY_FRAME_SIZE + 0x2D140
  #li sp, 0xA003C000
  #li sp, 0xA00FC000
  li sp, 0xA0000000 + (1*1024*1024) - (RISCV_PGSIZE * MAX_HARTS)
  csrr a0, mhartid
  slli a2, a0, RISCV_PGSHIFT
  add sp, sp, a2

# fill stacks
  mv a0, sp
  li a2, RISCV_PGSIZE
  sub a0, a0, a2
  mv a1, sp
  li a2, 0x0
  jal fill_block

  csrr a0, mhartid

  #csrr a0, mhartid
  # j main_all

  # Boot on the first hart
  # Run hart 3
  li a4, 3
  beq a0, a4, main_bootloader
  # Run hart 2
  li a4, 2
  beq a0, a4, main_bootloader
  # Run hart 1
  li a4, 1
  beq a0, a4, main_bootloader
  # Run hart 0
  li a4, 0
  beq a0, a4, main_bootloader
  # beqz a3, init_first_hart

  # set MSIE bit to receive IPI
  li a2, MIP_MSIP
  csrw mie, a2

.LmultiHart:
#if MAX_HARTS > 1
  # wait for an IPI to signal that it's safe to boot
  wfi

  # masked harts never start
  la a4, main_
  LOAD a4, 0(a4)
  srl a4, a4, a3
  andi a4, a4, 1
  bnez a4, .LmultiHart

  # only start if mip is set
  csrr a2, mip
  andi a2, a2, MIP_MSIP
  beqz a2, .LmultiHart

  # make sure our hart id is within a valid range
  fence
  li a2, MAX_HARTS
  bltu a3, a2, main_
#endif
  wfi
  j .LmultiHart

/* Fills memory blocks */
fill_block:
	sw		a2, 0(a0)
	bgeu	a0, a1, fb_end
	addi	a0, a0, 4
	j		fill_block
fb_end:
	ret

	.bss
	.align RISCV_PGSHIFT
#stacks:
#	.skip RISCV_PGSIZE * MAX_HARTS
