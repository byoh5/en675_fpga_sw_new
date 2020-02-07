#include "dev.h"
#include "test_nonos.h"

void main_1(int cpu_id)
{
	SYS_REG0 = 0xf;
	while(SYS_REG0 == 0xf) {} // Wait for CPU0 to be ready.

	enx_externalirq_init_cpu1();

	while (1) {
#if TEST_CPU1
		testloop1();
#else
		if (SYS_REG0 == 0xA) {
#ifdef __USE_LED1__
			GpioSetOut(GPIO_LED1, GPIO_OUT_HI);
#endif
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xB;
		}
#endif
	}
}
