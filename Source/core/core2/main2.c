#include "dev.h"
#include "test_nonos.h"

void main_2(int cpu_id)
{
	SYS_REG0 = 0xf;
	while(SYS_REG0 == 0xf) {} // Wait for CPU0 to be ready.

	enx_externalirq_init_cpu2();

	while (1) {
#if TEST_CPU2
		testloop2();
#else
		if (SYS_REG0 == 0xB) {
#ifdef __USE_LED2__
			GpioSetOut(GPIO_LED2, GPIO_OUT_HI);
#endif
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xC;
		}
#endif
	}
}
