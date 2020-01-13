#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "dev.h"

#if 0 // version. mtime
void WaitXus(uint64_t us)
{	// Wait X usec
	//printf("Idle(%uus)\n", us);
	if (us < 10) {											// min 10us
		us = 10;											// min 10us
	}														// min 10us
	uint64_t clk_period = APB_FREQ / 10000000 * (us / 10);	// min 10us
	uint64_t start_clk = *mtime;
	uint64_t clk;
	do {
		uint64_t stop_clk = *mtime;
		clk = stop_clk - start_clk;
		if (stop_clk < start_clk) {
			clk = ~clk + 1;	// neg --> abs
		}
	} while(clk < clk_period);
}
#else // version. rdcycle
void WaitXus(uint64_t us)
{	// Wait X usec
	uint64_t clk_period = (CPU_FREQ / (1000 * 1000)) * us;
	uint64_t start_clk = rdcycle();
	uint64_t clk;
	do {
		uint64_t stop_clk = rdcycle();
		clk = stop_clk - start_clk;
		if (stop_clk < start_clk) {
			clk = ~clk + 1;	// neg --> abs
		}
	} while(clk < clk_period);
}
#endif

void WaitXms(uint64_t ms)
{	// Wait X msec
	WaitXus(ms * 1000);
}

ULONG TimeGetMs(void)
{
	return *mtime / TIME_1MS;
}

ULONG BenchTimeStart(void)
{
	return rdcycle();
}

#if 0
ULONG BenchTimeStop(ULONG start_time)
{
	ULONG end_time = rdcycle();
	ULONG dep, time_us;
	if (start_time < end_time) {
		dep = end_time - start_time;
	} else {
		dep = (0xFFFFFFFFFFFFFFFF - start_time) + end_time;
	}
	time_us = dep / (CPU_FREQ / (1000 * 1000));
	return time_us;
}
#else
UINT BenchTimeStop(ULONG start_time)
{
	const ULONG end_time = rdcycle();
	const ULONG dep = (start_time < end_time) ? end_time - start_time : (0xFFFFFFFFFFFFFFFF - start_time) + end_time + 1;
	return dep / (CPU_FREQ / (1000 * 1000));
}
#endif

void BenchTimeSpeed(UINT time_us, ULONG size)
{




}

BYTE hex2ascii(BYTE hex)
{
	if (hex <= 0x09)							return hex + '0';			// 0 ~ 9
	else if ((hex >= 0x0A) && (hex <= 0x0F))	return hex - 0x0A + 'A';	// 10 ~ 15
	return 'x';
}

void hexDump(char *desc, void *addr, int len)
{
	int i, pos = 0;
	char strbuff[80] = {0};
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	// Output description if given.
	if (desc != NULL) {
		_Cprintf("%-39s", desc);
	}
	printf("Addr(0x%08X)  Size(%10u)\n", addr, len);

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0) {
				strbuff[pos++] = ' ';
				strbuff[pos++] = ' ';
				strncpy(&strbuff[pos], (char *)buff, 16);
				printf("%s\n", strbuff);
				pos = 0;
			}

			// Output the offset.
			sprintf(&strbuff[pos], "  %04x ", i);
			pos += 7;
		}

		// Now the hex code for the specific character.
		sprintf(&strbuff[pos], " %02x", pc[i]);
		pos += 3;

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
			buff[i % 16] = '.';
		} else {
			buff[i % 16] = pc[i];
		}
		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		i++;
	}

	// And print the final ASCII bit.
	strbuff[pos++] = ' ';
	strbuff[pos++] = ' ';
	strncpy(&strbuff[pos], (char *)buff, 16);
	pos += 16;
	printf("%s\n", strbuff);
}

void hexCmpDump(char *desc, void *addr1, void*addr2, int len)
{
	int i, pos = 0, flag = 0, pos2 = 0;
	char strbuff[160] = {0};
	char strbuff2[160] = {0};
	unsigned char buff[17];
	unsigned char buff2[17];
	unsigned char *pc1 = (unsigned char*)addr1;
	unsigned char *pc2 = (unsigned char*)addr2;

	// Output description if given.
	if (desc != NULL) {
		printf("%s\n", desc);
	}
	printf("Addr1(0x%08X) Addr2(0x%08X) Size(%u)\n", addr1, addr2, len);

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0) {
				strbuff[pos++] = ' ';
				strbuff[pos++] = ' ';
				strncpy(&strbuff[pos], (char *)buff, 16);
				if (flag == 1) {
					printf("%s%s%s\n", TTY_COLOR_YELLOW, strbuff, TTY_COLOR_RESET);
				} else {
					printf("%s\n", strbuff);
				}
				pos = 0;

				if (flag == 1) {
					strbuff2[pos2++] = ' ';
					strbuff2[pos2++] = ' ';
					strncpy(&strbuff2[pos2], (char *)buff2, 16);
					printf("%s%s%s\n", TTY_COLOR_YELLOW, strbuff2, TTY_COLOR_RESET);
					flag = 0;
				}
				pos2 = 0;
			}

			// Output the offset.
			sprintf(&strbuff[pos], "  %04x ", i);
			pos += 7;
			sprintf(&strbuff2[pos2], "       ");
			pos2 += 7;
		}

		// Now the hex code for the specific character.
		sprintf(&strbuff[pos], " %02x", pc1[i]);
		pos += 3;

		if (pc1[i] != pc2[i]) {
			sprintf(&strbuff2[pos2], " %02x", pc2[i]);
			pos2 += 3;
			flag = 1;
		} else {
			sprintf(&strbuff2[pos2], "   ");
			pos2 += 3;
		}

		// And store a printable ASCII character for later.
		if ((pc1[i] < 0x20) || (pc1[i] > 0x7e)) {
			buff[i % 16] = '.';
		} else {
			buff[i % 16] = pc1[i];
		}
		buff[(i % 16) + 1] = '\0';

		if ((pc2[i] < 0x20) || (pc2[i] > 0x7e)) {
			buff2[i % 16] = '.';
		} else {
			buff2[i % 16] = pc2[i];
		}
		buff2[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		strbuff2[pos2++] = ' ';
		strbuff2[pos2++] = ' ';
		strbuff2[pos2++] = ' ';
		i++;
	}

	// And print the final ASCII bit.
	strbuff[pos++] = ' ';
	strbuff[pos++] = ' ';
	strncpy(&strbuff[pos], (char *)buff, 16);
	pos += 16;
	if (flag == 1) {
		printf("%s%s%s\n", TTY_COLOR_YELLOW, strbuff, TTY_COLOR_RESET);
	} else {
		printf("%s\n", strbuff);
	}

	if (flag == 1) {
		strbuff2[pos2++] = ' ';
		strbuff2[pos2++] = ' ';
		strncpy(&strbuff2[pos2], (char *)buff2, 16);
		printf("%s%s%s\n", TTY_COLOR_YELLOW, strbuff2, TTY_COLOR_RESET);
		flag = 0;
	}
}

#if 0
void sleep_(int cnt)
{
	for(int i=0;i<cnt;i++){
		__asm("C.NOP");			// 1 cycle = 15 clock = 15/CPU_FREQ sec,  컴파일 옵션 : -O0
	}

}

void sleep_test(void)
{
	const ULONG sleep1Ksta = BenchTimeStart();
	sleep_(1000);
	const UINT sleep1Ktime = BenchTimeStop(sleep1Ksta);
	_printf("sleep_(1000) : %dus\r\n", sleep1Ktime);

	const ULONG sleep10Ksta = BenchTimeStart();
	sleep_(10000);
	const UINT sleep10Ktime = BenchTimeStop(sleep10Ksta);
	_printf("sleep_(10000) : %dus\r\n", sleep10Ktime);

	const ULONG sleep100Ksta = BenchTimeStart();
	sleep_(100000);
	const UINT sleep100Ktime = BenchTimeStop(sleep100Ksta);
	_printf("sleep_(100000) : %dus\r\n", sleep100Ktime);

	const ULONG sleep1Msta = BenchTimeStart();
	sleep_(1000000);
	const UINT sleep1Mtime = BenchTimeStop(sleep1Msta);
	_printf("sleep_(1000000) : %dus\r\n", sleep1Mtime);
}
#endif

void main_(void)
{
	_printf("exception\n");
	while (1) {
		WaitXms(1000);
	}
}
