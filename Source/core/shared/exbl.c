#include <stdint.h>
#include <string.h>

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

void main_(void)
{
	_printf("exception\n");
	while (1) {
		WaitXms(1000);
	}
}
