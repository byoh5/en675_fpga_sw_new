// See LICENSE for license details.

#ifndef _EXBL_H
#define _EXBL_H

extern void WaitXus(uint64_t us);
extern void WaitXms(uint64_t ms);
extern ULONG TimeGetMs(void);
extern ULONG BenchTimeStart(void);
//extern ULONG BenchTimeStop(ULONG start_time);
extern UINT BenchTimeStop(ULONG start_time);
extern BYTE hex2ascii(BYTE hex);
extern void hexDump(char *desc, void *addr, int len);
extern void hexCmpDump(char *desc, void *addr1, void*addr2, int len);

//extern void sleep_(int cnt);
//#define sleep_(CNT)		WaitXus((CNT*15) / 81/*(CPU_FREQ/(1000*1000))*/)

#endif
