#include "dev.h"

char g_key = 0xFF;

extern volatile uint64_t* mtime;

void main_0(int cpu_id)
{
	*mtime = 0; // timer init

	_init_bss_section();

	Uart7_Init(115200);

	GPIO_PIN55_OEN = 0;

	g_key = 0xA; // CPU0 Ready!

	int k = 0;
	while (1) {
		_printf("%d:%lu\r\n", cpu_id, *mtime);
		GPIO_PIN55_OUT = k % 2;
		k++;

		WaitXms(1000);
	}
}
