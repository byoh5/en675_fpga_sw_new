#include "dev.h"

extern char g_key;

void main_2(int cpu_id)
{
	while(g_key == 0xFF) {} // Wait for CPU0 to be ready.

	while (1) {
		if (g_key == 0xB) {
			GpioSetOut(GPIO_LED2, GPIO_OUT_HI);
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			g_key = 0xC;
		}
		WaitXms(1);
	}
}
