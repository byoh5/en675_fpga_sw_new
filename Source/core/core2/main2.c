#include "dev.h"

void main_2(int cpu_id)
{
	SYS_REG0 = 0;
	while(SYS_REG0 == 0x0) {} // Wait for CPU0 to be ready.

	while (1) {
		if (SYS_REG0 == 0xB) {
			GpioSetOut(GPIO_LED2, GPIO_OUT_HI);
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xC;
		}
		WaitXms(1);
	}
}
