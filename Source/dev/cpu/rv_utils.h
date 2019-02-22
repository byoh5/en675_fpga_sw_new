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

void flush_dcache_line(unsigned int);

// direct memory access using software flush/invalidation
void dmwrite8(uint adr, uchar wdat);
void dmwrite16(uint adr, ushort wdat);
void dmwrite32(uint adr, uint wdat);
void dmwrite64(uint adr, ulong wdat);

uchar dmread8(uint adr);
ushort dmread16(uint adr);
uint dmread32(uint adr);
ulong dmread64(uint adr);

// misc
void rv_util_test();

#endif//_RV_UTILS_H_

