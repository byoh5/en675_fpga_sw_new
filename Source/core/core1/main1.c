#include "dev.h"

void main_1(int cpu_id)
{
	SYS_REG0 = 0;
	while(SYS_REG0 == 0x0) {} // Wait for CPU0 to be ready.

	enx_externalirq_init_cpu1();

	while (1) {
		if (SYS_REG0 == 0xA) {
#ifdef __USE_LED1__
			GpioSetOut(GPIO_LED1, GPIO_OUT_HI);
#endif
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xB;
		}
		WaitXms(1);
	}
}
