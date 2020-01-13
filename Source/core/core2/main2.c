#include "dev.h"

void main_2(int cpu_id)
{
	SYS_REG0 = 0;
	while(SYS_REG0 == 0x0) {} // Wait for CPU0 to be ready.

	enx_externalirq_init_cpu2();

	while (1) {
		if (SYS_REG0 == 0xB) {
#ifdef __USE_LED2__
			GpioSetOut(GPIO_LED2, GPIO_OUT_HI);
#endif
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xC;
		}
		WaitXms(1);
	}
}
