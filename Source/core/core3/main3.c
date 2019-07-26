#include "dev.h"
#include "enx_freertos.h"

void main_3(int cpu_id)
{
	while(SYS_REG0 == 0x0) {} // Wait for CPU0 to be ready.

	while (1) {
		if (SYS_REG0 == 0xC) {
			GpioSetOut(GPIO_LED1, GPIO_OUT_LOW);
			GpioSetOut(GPIO_LED2, GPIO_OUT_LOW);
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xA;
		}
		WaitXms(1);
	}
}
