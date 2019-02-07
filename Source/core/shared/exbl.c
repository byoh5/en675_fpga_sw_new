#include <stdint.h>
#include <string.h>

#include "dev.h"
void sleep_(int cnt)
{
	for (int i = 0; i < cnt; i++) {
		__asm("C.NOP");
	}
}

void main_(void)
{
	_printf("exception\n");
	while (1) {
		sleep_(1000);
	}
}
