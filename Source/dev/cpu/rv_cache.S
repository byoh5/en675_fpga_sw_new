//******************************************************************************
//	Copyright(c) Young-Geun Kim 2003-
//
//	File Name:		rv_cache.S
//	Description:	risc-v cache related functions
//	Author:				Young Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}), Eyenix Co.,Ltd.
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	190215	ygkim			first designed
// -----------------------------------------------------------------------------
#include "asm.h"

	.section .text
FUNC_ENTRY(swflush_dcache_line)
// arguments: a0: uint sadr
// registers:	a5: adr
//						a4: rdat
//						a3: DC_BSSIZE (fixed) = 0x1000
// notes:			Flushing a whole cache line requires at least 4 consecutive memory r/w operations with DC_BSSIZE address increments
	// save stack: s0 = sp
	addi	sp,sp,-32
  sd		s0,24(sp)
  addi	s0,sp,32			

	// volatile uchar* adr = (uchar*)((sadr >> DC_BBITS) << DC_BBITS); // multiple of 64 B
	// adr+=DC_BSSIZE;                                                                 
	mv		a4,a0										// a4 = sadr
	li 		a5,1										// a5 = 1                                           
	slli	a5,a5,0x20              // a5 = a5 <<0x20 = 1_0000_0000h                     
	addi	a5,a5,-64               // a5 = a5 - 64		= 	ffff_ffc0h
	and		a5,a5,a4                // a5 = a5 & a4 : adr
	add		a5,a5,a3								// a5+= a3 = 				adr+0000_1000h

	//
	// 4 times required for counter based replacement policy
	//
	// sum = *adr;		                                                                                                      
	// adr+=DC_BSSIZE;                                                                 
	fence	rw,rw                                                                         
	lbu		a4,0(a5)                // a4 = M[a5] : memory read
	lui		a3,0x1									// a3 = 						0000_1000h
	add		a5,a5,a3								// a5+= a3 = 				adr+0000_1000h
	
	// sum = *adr;		                                                                                                      
	// adr+=DC_BSSIZE;                                                                 
	fence	rw,rw                                                                         
	lbu		a4,0(a5)                // a4 = M[a5] : memory read
	lui		a3,0x1									// a3 = 						0000_1000h
	add		a5,a5,a3								// a5+= a3 = 				adr+0000_1000h

	// sum = *adr;		                                                                                                      
	// adr+=DC_BSSIZE;                                                                 
	fence	rw,rw                                                                         
	lbu		a4,0(a5)                // a4 = M[a5] : memory read
	lui		a3,0x1									// a3 = 						0000_1000h
	add		a5,a5,a3								// a5+= a3 = 				adr+0000_1000h

	// sum = *adr;		                                                                                                      
	// adr+=DC_BSSIZE;                                                                 
	fence	rw,rw                                                                         
	lbu		a4,0(a5)                // a4 = M[a5] : memory read
	lui		a3,0x1									// a3 = 						0000_1000h
	add		a5,a5,a3								// a5+= a3 = 				adr+0000_1000h

	//
	// 4 more times required for random/LRU based replacement policy. but not for sure
	//
	/*
	// sum = *adr;		                                                                                                      
	// adr+=DC_BSSIZE;                                                                 
	fence	rw,rw                                                                         
	lbu		a4,0(a5)                // a4 = M[a5] : memory read
	lui		a3,0x1									// a3 = 						0000_1000h
	add		a5,a5,a3								// a5+= a3 = 				adr+0000_1000h

	// sum = *adr;		                                                                                                      
	// adr+=DC_BSSIZE;                                                                 
	fence	rw,rw                                                                         
	lbu		a4,0(a5)                // a4 = M[a5] : memory read
	lui		a3,0x1									// a3 = 						0000_1000h
	add		a5,a5,a3								// a5+= a3 = 				adr+0000_1000h

	// sum = *adr;		                                                                                                      
	// adr+=DC_BSSIZE;                                                                 
	fence	rw,rw                                                                         
	lbu		a4,0(a5)                // a4 = M[a5] : memory read
	lui		a3,0x1									// a3 = 						0000_1000h
	add		a5,a5,a3								// a5+= a3 = 				adr+0000_1000h

	// sum = *adr;		                                                                                                      
	// adr+=DC_BSSIZE;                                                                 
	fence	rw,rw                                                                         
	lbu		a4,0(a5)                // a4 = M[a5] : memory read
	lui		a3,0x1									// a3 = 						0000_1000h
	add		a5,a5,a3								// a5+= a3 = 				adr+0000_1000h
	*/
	
	//
	fence	rw,rw                                                                         

	// restore stack
	nop
	ld		s0,24(sp)
 	addi	sp,sp,32  
  ret
	

FUNC_ENTRY(hwflush_dcache_line)
// arguments: a0: uint sadr
// registers:	a5: adr
//						a4: rdat
//						a3: DC_BSSIZE (fixed) = 0x1000
// notes:			Flushing a whole cache line requires at least 4 consecutive memory r/w operations with DC_BSSIZE address increments
	// save stack: s0 = sp
	addi	sp,sp,-32
  sd		s0,24(sp)
  addi	s0,sp,32			

	// volatile uchar* adr = (uchar*)((sadr >> DC_BBITS) << DC_BBITS); // multiple of 64 B
	mv		a4,a0										// a4 = sadr
	li 		a5,1										// a5 = 1                                           
	slli	a5,a5,0x20              // a5 = a5 <<0x20 = 1_0000_0000h                     
	addi	a5,a5,-64               // a5 = a5 - 64		= 	ffff_ffc0h
	and		a5,a5,a4                // a5 = a5 & a4 : adr

	// asm volatile("CFLUSH.D.L1 r5");
	// def CFLUSH_D_L1        = BitPat("b 1111 1100 0000 ???? ?000 0000 0111 0011") @ Instructions.scala
	//fence	rw,rw                                                                         
	.word (0xfc000073 | 15<<15)		


	//
	fence	rw,rw                                                                         

	// restore stack
	nop
	ld		s0,24(sp)
 	addi	sp,sp,32  
  ret
	
	