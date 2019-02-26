// See LICENSE for license details.

#ifndef _EXBL_H
#define _EXBL_H

extern void WaitXus(uint64_t us);
extern void WaitXms(uint64_t ms);
extern BYTE hex2ascii(BYTE hex);
extern void hexDump(char *desc, void *addr, int len);

#endif
