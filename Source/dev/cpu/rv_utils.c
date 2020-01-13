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
#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif
#include <sys/time.h>

extern void hwflush_dcache_line(uint);

//******************************************************************************
// Software Flush/Invalidate 16kB Data Cache
//------------------------------------------------------------------------------
// 16kB = 64B blocksize x 64 sets x 4 ways
#define DC_BBITS	6						// block address bits: 6
#define DC_SBITS	6						// set address bits: 6
#define DC_BSIZE	(1<<DC_BBITS)			// block size: 64 B
#define DC_SSIZE	(1<<DC_SBITS)			// set size: 64 sets
#define DC_NWAYS	4						// no. of ways
#define DC_BSSIZE	(DC_BSIZE*DC_SSIZE)		// block set size: 4096 B
#define DC_SIZE		(DC_BSSIZE*DC_NWAYS)	// cache size: 16 kB

//------------------------------------------------------------------------------
// cache flush function
inline void hwflush_dcache_all(void)
{	// def CFLUSH_D_L1 = BitPat("b 1111 1100 0000 ???? ?000 0000 0111 0011") @ Instructions.scala
	__asm__ __volatile__ (".word 0xfc000073");
	__asm__ __volatile__ ("fence.i");
}

void hwflush_dcache_range(ulong sadr, ulong size)
{
	if (size >= DC_SIZE) {
		hwflush_dcache_all();
		return;
	}

	ulong eadr = sadr + size;
	register ulong new_sadr asm("a0") = ((sadr >> DC_BBITS) << DC_BBITS);
	do {// def CFLUSH_D_L1 = BitPat("b 1111 1100 0000 ???? ?000 0000 0111 0011") @ Instructions.scala
		__asm__ __volatile__ (".word (0xfc000073 | 10<<15)");
		new_sadr += DC_BSIZE;
	} while (new_sadr < eadr);
	__asm__ __volatile__ ("fence.i");
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
// cache invalidate function
void hwdiscard_dcache_all(void)
{	// def CDISCARD_D_L1 = BitPat("b 1111 1100 0010 ???? ?000 0000 0111 0011") @ Instructions.scala
	__asm__ __volatile__ (".word 0xfc200073");
	__asm__ __volatile__ ("fence.i");
}

void hwdiscard_dcache_range(ulong sadr, ulong size)
{
	if (size >= DC_SIZE) {
		hwdiscard_dcache_all();
		return;
	}

	ulong eadr = sadr + size;
	register ulong new_sadr asm("a0") = ((sadr >> DC_BBITS) << DC_BBITS);
	do {// def CDISCARD_D_L1 = BitPat("b 1111 1100 0010 ???? ?000 0000 0111 0011") @ Instructions.scala
		__asm__ __volatile__ (".word (0xfc200073 | 10<<15)");
		new_sadr += DC_BSIZE;
	} while (new_sadr < eadr);
	__asm__ __volatile__ ("fence.i");
}

#ifdef __FREERTOS__
void hwdiscard_dcache_range_rtos(ulong sadr, ulong size)
{
	portENTER_CRITICAL();
	hwdiscard_dcache_range(sadr, size);
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

//------------------------------------------------------------------------------
//
UINT timeoffset(UINT offset)
{
	struct timeval t1;
	gettimeofday(&t1, NULL);
	return (t1.tv_sec + offset);
}
