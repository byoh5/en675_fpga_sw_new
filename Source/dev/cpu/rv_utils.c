//******************************************************************************
//	Copyright(c) Young-Geun Kim 2003-
//
//	File Name:		rv_utils.c
//	Description:	risc-v utilities
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}), Eyenix Co.,Ltd.
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	190215		ygkim		first designed
// -----------------------------------------------------------------------------
#include "dev.h"
#include "enx_freertos.h"

#include <sys/time.h>

extern void hwflush_dcache_line(uint);

//******************************************************************************
// Software Flush/Invalidate 16kB Data Cache
//------------------------------------------------------------------------------
// 16kB = 64B blocksize x 64 sets x 4 ways
#define DC_BBITS	6															// block address bits: 6
#define DC_SBITS	6															// set address bits: 6
#define	DC_BSIZE	(1<<DC_BBITS)									// block size: 64 B
#define	DC_SSIZE	(1<<DC_SBITS)									// set size: 64 sets
#define	DC_NWAYS	4															// no. of ways
#define	DC_BSSIZE	(DC_BSIZE*DC_SSIZE)						// block set size: 4096 B
#define	DC_SIZE		(DC_BSSIZE*DC_NWAYS)					// cache size: 16 kB

//------------------------------------------------------------------------------
//
inline void hwflush_dcache_range_all(void)
{
	asm volatile ("fence rw,rw");
	// def CFLUSH_D_L1 = BitPat("b 1111 1100 0000 ???? ?000 0000 0111 0011") @ Instructions.scala
	asm volatile (".word 0xfc000073");
}

void hwflush_dcache_range(ulong sadr, ulong size)
{
	if (size >= DC_SIZE) {
		hwflush_dcache_range_all();
		return;
	}

	ulong eadr = sadr + size;
#if 1
#if 1
	register ulong new_sadr asm("a0") = ((sadr >> DC_BBITS) << DC_BBITS);
	asm volatile ("fence rw,rw");
#if 1
	do {
		// def CFLUSH_D_L1 = BitPat("b 1111 1100 0000 ???? ?000 0000 0111 0011") @ Instructions.scala
		asm volatile (".word (0xfc000073 | 10<<15)");
		new_sadr += DC_BSIZE;
	} while (new_sadr < eadr);
#else
	while (new_sadr < eadr) {
		// def CFLUSH_D_L1 = BitPat("b 1111 1100 0000 ???? ?000 0000 0111 0011") @ Instructions.scala
		asm volatile (".word (0xfc000073 | 10<<15)");
		new_sadr += DC_BSIZE;
	}
#endif
#else
	sadr = ((sadr >> DC_BBITS) << DC_BBITS);
	while (sadr < eadr) {
		asm volatile ("fence rw,rw");
		// def CFLUSH_D_L1 = BitPat("b 1111 1100 0000 ???? ?000 0000 0111 0011") @ Instructions.scala
		asm volatile (".word (0xfc000073 | 15<<15)");
		sadr += DC_BSIZE;
	}
#endif
#else
	sadr = ((sadr >> DC_BBITS) << DC_BBITS);
	while (sadr < eadr) {
		hwflush_dcache_line(sadr);
		sadr += DC_BSIZE;
	}
#endif
}

#ifdef __FREERTOS__
void hwflush_dcache_range_rtos(ulong sadr, ulong size)
{
	portENTER_CRITICAL();
	hwflush_dcache_range(sadr, size);
	portEXIT_CRITICAL();
}
#endif

//------------------------------------------------------------------------------
//
void dmwrite8(ulong adr, uchar wdat)
{
	asm volatile("fence rw,rw");									// to finish prev read/write
	volatile uchar* wadr = (volatile uchar*)adr;						// write to DC
	*wadr = wdat;                             		
	                                          		
	//swflush_dcache_line(adr);										// sw flush DC to write to memory
	hwflush_dcache_line(adr);											// hw flush DC to write to memory
}

void dmwrite16(ulong adr, ushort wdat)
{
	asm volatile("fence rw,rw");
	volatile ushort* wadr = (volatile ushort*)adr;
	*wadr = wdat;
	
	//swflush_dcache_line(adr);										// sw flush DC to write to memory
	hwflush_dcache_line(adr);											// hw flush DC to write to memory
}

void dmwrite32(ulong adr, uint wdat)
{
	asm volatile("fence rw,rw");
	volatile uint* wadr = (volatile uint*)adr;
	*wadr = wdat;
	
	//swflush_dcache_line(adr);										// sw flush DC to write to memory
	hwflush_dcache_line(adr);											// hw flush DC to write to memory
}

void dmwrite64(ulong adr, ulong wdat)
{
	asm volatile("fence rw,rw");
	volatile ulong* wadr = (volatile ulong*)adr;
	*wadr = wdat;
	
	//swflush_dcache_line(adr);										// sw flush DC to write to memory
	hwflush_dcache_line(adr);											// hw flush DC to write to memory
}

//------------------------------------------------------------------------------
//
uchar dmread8(ulong adr)
{
	//swflush_dcache_line(adr);										// sw flush DC to write to memory
	hwflush_dcache_line(adr);											// hw flush DC to write to memory
                                          			
	asm volatile("fence rw,rw");            			
	volatile uchar* radr = (volatile uchar*)adr;						// read from memory
	return *radr;	
}

ushort dmread16(ulong adr)
{
	//swflush_dcache_line(adr);										// sw flush DC to write to memory
	hwflush_dcache_line(adr);											// hw flush DC to write to memory

	asm volatile("fence rw,rw");
	volatile ushort* radr = (volatile ushort*)adr;
	return *radr;	
}

uint dmread32(ulong adr)
{
	//swflush_dcache_line(adr);										// sw flush DC to write to memory
	hwflush_dcache_line(adr);											// hw flush DC to write to memory

	asm volatile("fence rw,rw");
	volatile uint* radr = (volatile uint*)adr;
	return *radr;	
}

ulong dmread64(ulong adr)
{
	//swflush_dcache_line(adr);										// sw flush DC to write to memory
	hwflush_dcache_line(adr);											// hw flush DC to write to memory

	asm volatile("fence rw,rw");
	volatile ulong* radr = (volatile ulong*)adr;
	return *radr;	
}


//******************************************************************************
// Misc.
//------------------------------------------------------------------------------
#define BASE_ADDR 0xA0018000

void rv_util_test()
{
	dmwrite8(BASE_ADDR	, 0xef);
	dmwrite8(BASE_ADDR+1, 0xcd);
	dmwrite8(BASE_ADDR+2, 0xab);
	dmwrite8(BASE_ADDR+3, 0x89);
	dmwrite8(BASE_ADDR+4, 0x67);
	dmwrite8(BASE_ADDR+5, 0x45);
	dmwrite8(BASE_ADDR+6, 0x23);
	dmwrite8(BASE_ADDR+7, 0x01);

	dmwrite16(BASE_ADDR+0x100  , 0xcdef);
	dmwrite16(BASE_ADDR+0x100+2, 0x789a);
	dmwrite16(BASE_ADDR+0x100+4, 0x4567);
	dmwrite16(BASE_ADDR+0x100+6, 0x0123);
	
	dmwrite32(BASE_ADDR+0x200  , 0x89abcdef);
	dmwrite32(BASE_ADDR+0x200+4, 0x01234567);

	dmwrite64(BASE_ADDR+0x300  , 0x0123456789abcdef);

	uchar rdat8 = 0;
	rdat8 = dmread8(BASE_ADDR	);
	rdat8 = dmread8(BASE_ADDR+1);
	rdat8 = dmread8(BASE_ADDR+2);
	rdat8 = dmread8(BASE_ADDR+3);
	rdat8 = dmread8(BASE_ADDR+4);
	rdat8 = dmread8(BASE_ADDR+5);
	rdat8 = dmread8(BASE_ADDR+6);
	rdat8 = dmread8(BASE_ADDR+7);

	ushort rdat16;
	rdat16 = dmread16(BASE_ADDR+0x100  );
	rdat16 = dmread16(BASE_ADDR+0x100+2);
	rdat16 = dmread16(BASE_ADDR+0x100+4);
	rdat16 = dmread16(BASE_ADDR+0x100+6);
	
	uint rdat32;
	rdat32 = dmread32(BASE_ADDR+0x200  );
	rdat32 = dmread32(BASE_ADDR+0x200+4);

	ulong rdat64;
	rdat64 = dmread64(BASE_ADDR+0x300  );
}
/*
uint flush_dcache_line_old2(uint sadr)
{
	volatile uchar* adr = (uchar*)((sadr >> DC_BBITS) << DC_BBITS); // multiple of 64 B
	uint sum;
	
	// at least 4 times required 
	asm volatile("fence rw,rw");
	sum = *adr;		
	adr+=DC_BSSIZE;
	
	asm volatile("fence rw,rw");
	sum+= *adr;		
	adr+=DC_BSSIZE;

	asm volatile("fence rw,rw");
	sum+= *adr;		
	adr+=DC_BSSIZE;

	asm volatile("fence rw,rw");
	sum+= *adr;		
	adr+=DC_BSSIZE;

	asm volatile("fence rw,rw");
	sum+= *adr;		
	adr+=DC_BSSIZE;

	asm volatile("fence rw,rw");
	sum+= *adr;		
	adr+=DC_BSSIZE;

	asm volatile("fence rw,rw");
	sum+= *adr;		
	adr+=DC_BSSIZE;

	asm volatile("fence rw,rw");
	sum+= *adr;		
	adr+=DC_BSSIZE;

	asm volatile("fence rw,rw");
	return sum;
}

uint flush_dcache_line_old(uint sadr)
{
	volatile uchar* adr = (uchar*)((sadr >> DC_BBITS) << DC_BBITS); // multiple of 64 B
	uint sum = 0;
	
	for(int w=0; w<DC_NWAYS*2; w++, adr+=DC_BSSIZE) { // at least: +1
		asm volatile("fence rw,rw");
		sum += *adr;
	}
	asm volatile("fence rw,rw");

	//_printf("sum: %d\n",sum);
	return sum;
}

#define flush_page(addr) asm volatile ("sfence.vma %0" : : "r" (addr) : "memory") 

static inline void flush_tlb()
{
  asm volatile ("sfence.vma");
}	

void test_dcache()
{
	volatile uint* adr = (uint*)MEM_BASE;

//	asm volatile("fence");
//	asm volatile("fence rw,rw");
//	//asm volatile("sfence.vm");
	*adr++ = MEM_BASE;
	flush_page(adr-1);
//	asm volatile("fence");
//	asm volatile("fence rw,rw");
//	//asm volatile("sfence.vm");
	*adr++ = MEM_BASE+4;
	flush_page(adr-1);
//	asm volatile("fence");
//	asm volatile("fence rw,rw");
//	//asm volatile("sfence.vm");
	*adr++ = MEM_BASE+8;
	flush_page(adr-1);
//	asm volatile("fence");
//	asm volatile("fence rw,rw");
//	//asm volatile("sfence.vm");
	*adr++ = MEM_BASE+12;
	flush_page(adr-1);
//	asm volatile("fence");
//	asm volatile("fence rw,rw");
//	//asm volatile("sfence.vm");
	*adr++ = MEM_BASE+16;	
	flush_page(adr-1);
}
*/	

UINT timeoffset(UINT offset)
{
	struct timeval t1;
	gettimeofday(&t1, NULL);
	return (t1.tv_sec + offset);
}
