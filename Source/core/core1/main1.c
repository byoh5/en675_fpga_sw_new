#include "dev.h"

#include "enx_freertos.h"
#include "shell.h"

#define SLEEP_CNT 1000000
extern char g_key;

extern void trap_freertos(void); // mentry.S

void main_1(int cpu_id)
{
#if 1
	while(g_key == 0xFF) {} // Wait for CPU0 to be ready.

	while (1) {
		if (g_key == 0xB) {
			_printf("%d:%lu\r\n", cpu_id, *mtime);
			g_key = 0xA;
		}
		sleep_(SLEEP_CNT);
	}
#else
	while(g_key == 0xFF) {} // Wait for CPU0 to be ready.

	// set TRAP handlers
	write_csr(mtvec, &trap_freertos);

	_printf("HELLO EYENIX!(id:%d)(%x)\n", cpu_id, &cpu_id); // ADD some

	vMemoryHeapInit();

	vTaskCreate("shell", ShellTask, NULL, 8192, 3);

	vTaskStartScheduler();
#endif
}
