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
#include "dev.h"

extern void flush_dcache_line(unsigned int);
extern void hwflush_dcache_range(uint sadr, uint eadr);

// direct memory access using software flush/invalidation
extern void dmwrite8(uint adr, uchar wdat);
extern void dmwrite16(uint adr, ushort wdat);
extern void dmwrite32(uint adr, uint wdat);
extern void dmwrite64(uint adr, ulong wdat);

extern uchar dmread8(uint adr);
extern ushort dmread16(uint adr);
extern uint dmread32(uint adr);
extern ulong dmread64(uint adr);

// misc
extern void rv_util_test();

#endif//_RV_UTILS_H_

