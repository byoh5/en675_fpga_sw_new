#include "dev.h"
#include "enx_freertos.h"

void main_3(int cpu_id)
{
#if 0
	write_csr(mip, 0);
	set_csr(mie, MIP_MSIP);
	set_csr(mstatus, MSTATUS_MIE);

	__asm__ __volatile__ ("wfi");
#endif
	SYS_REG0 = 0;
	while(SYS_REG0 == 0x0) {} // Wait for CPU0 to be ready.

	while (1) {
		if (SYS_REG0 == 0xC) {
#ifdef __USE_LED1__
			GpioSetOut(GPIO_LED1, GPIO_OUT_LOW);
#endif
#ifdef __USE_LED2__
			GpioSetOut(GPIO_LED2, GPIO_OUT_LOW);
#endif
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xA;
		}
		WaitXms(1);
	}
}
