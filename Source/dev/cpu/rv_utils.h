//******************************************************************************
//	Copyright(c) Young-Geun Kim 2003-
//
//	File Name:		rv_utils.h
//	Description:	risc-v utilities
//	Author:				Young Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}), Eyenix Co.,Ltd.
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	190215	ygkim		first designed
// -----------------------------------------------------------------------------
#ifndef _RV_UTILS_H_
#define	_RV_UTILS_H_

// cache flush function
void hwflush_dcache_range_all(void);
extern void hwflush_dcache_all(void);
extern void hwflush_dcache_range(ulong sadr, ulong size);
extern void hwflush_dcache_range_rtos(ulong sadr, ulong size);

// cache invalidate function
extern void hwdiscard_dcache_all(void);
extern void hwdiscard_dcache_range(ulong sadr, ulong size);
extern void hwdiscard_dcache_range_rtos(ulong sadr, ulong size);

// direct memory access using software flush/invalidation
extern void dmwrite8(ulong adr, uchar wdat);
extern void dmwrite16(ulong adr, ushort wdat);
extern void dmwrite32(ulong adr, uint wdat);
extern void dmwrite64(ulong adr, ulong wdat);

extern uchar dmread8(ulong adr);
extern ushort dmread16(ulong adr);
extern uint dmread32(ulong adr);
extern ulong dmread64(ulong adr);

extern UINT timeoffset(UINT offset);

#endif//_RV_UTILS_H_

