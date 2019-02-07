#include "dev.h"

extern char g_key;

void main_2(int cpu_id)
{
	while(g_key == 0xFF) {} // Wait for CPU0 to be ready.

	while (1) {
		if (g_key == 0xA) {
			_printf("%d:%lu\r\n", cpu_id, *mtime);
			g_key = 0xB;
		}
		WaitXms(1000);
	}
}
