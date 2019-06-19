#include "dev.h"
#include "enx_freertos.h"

extern char g_key;

void main_3(int cpu_id)
{
	while(g_key == 0xFF) {} // Wait for CPU0 to be ready.

	while (1) {
		if (g_key == 0xC) {
			GpioSetOut(GPIO_LED1, GPIO_OUT_LOW);
			GpioSetOut(GPIO_LED2, GPIO_OUT_LOW);
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			g_key = 0xA;
		}
		WaitXms(1);
	}
}
