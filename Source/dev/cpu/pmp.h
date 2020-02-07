#ifndef __PMP_H__
#define __PMP_H__

#include "dev_types.h"
#include "encoding.h"

// Physical Memory Protection

#define	PMPADDR_COUNT 8

void setup_pmp(void);
int pmp_entry_set(unsigned n, BYTE prot, ULONG addr, ULONG len);

#endif //__PMP_H__
