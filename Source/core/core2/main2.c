#include "dev.h"

#define SLEEP_CNT 1000000
extern char g_key;

void main_2(int cpu_id)
{
	while(g_key == 0xFF) {} // Wait for CPU0 to be ready.

	while (1) {
		if (g_key == 0xA) {
			_printf("%d:%lu\r\n", cpu_id, *mtime);
			g_key = 0xB;
		}
		sleep_(SLEEP_CNT);
	}
}
