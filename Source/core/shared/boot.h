/*
 * boot.h
 *
 *  Created on: 2018. 11. 29.
 *      Author: BYOH
 */

#ifndef BOOT_H_
#define BOOT_H_

void _init_bss_section(void);
void WaitXus(uint64_t us);
void WaitXms(uint64_t ms);
void hexDump(char *desc, void *addr, int len);


#endif /* BOOT_H_ */
