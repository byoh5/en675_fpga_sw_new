#include "dev.h"

#include "enx_freertos.h"
#include "shell.h"

extern char g_key;

extern void trap_freertos(void); // mentry.S

void main_1(int cpu_id)
{
#if 1
	while(g_key == 0xFF) {} // Wait for CPU0 to be ready.

	while (1) {
		if (g_key == 0xA) {
			GpioSetOut(GPIO_LED1, GPIO_OUT_HI);
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			g_key = 0xB;
		}
		WaitXms(1);
	}
#else
	while(g_key == 0xFF) {} // Wait for CPU0 to be ready.

	_printf("HELLO EYENIX!(id:%d)(%x)\n", cpu_id, &cpu_id); // ADD some

	vMemoryHeapInit();

	vTaskCreate("shell", ShellTask, NULL, 8192, 3);

	vTaskStartScheduler();
#endif
}
