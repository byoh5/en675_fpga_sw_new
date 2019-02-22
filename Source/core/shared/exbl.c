#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "dev.h"

void WaitXus(uint64_t us)
{	// Wait X usec
	if (us < 10) {											// min 10us
		us = 10;											// min 10us
	}														// min 10us
	uint64_t clk_period = MCK_FREQ / 10000000 * (us / 10);	// min 10us
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

void WaitXms(uint64_t ms)
{	// Wait X msec
	WaitXus(ms * 1000);
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
		printf("%s:\n", desc);
	}

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

void main_(void)
{
	_printf("exception\n");
	while (1) {
		WaitXms(1000);
	}
}
