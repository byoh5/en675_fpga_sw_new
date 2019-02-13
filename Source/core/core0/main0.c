#include "dev.h"

#include "enx_freertos.h"
#include "shell.h"

extern void trap_freertos(void); // mentry.S

char g_key = 0xFF;

extern volatile uint64_t* mtime;

extern void DdrTest(void); // ddr.c
extern void DdrInit(void); // ddr.c

#if 0
#define TEST_SIZE 1024
BYTE arrlist[TEST_SIZE];
void test_func(void)
{
	for (int i = 0; i < TEST_SIZE; i++) {
		arrlist[i] = i;
	}
	for (int i = 0; i < TEST_SIZE; i++) {
		_printf("[%x]\n", arrlist[i]);
	}
	_printf("1===========================\n");
	u8 *u8list = arrlist;
	u16 *u16list = arrlist;
	u32 *u32list = arrlist;
	u64 *u64list = arrlist;

	for (int i = 0; i < TEST_SIZE; i++) {
		_printf("[%x]\n", u8list[i]);
	}
	_printf("2===========================\n");
	for (int i = 0; i < TEST_SIZE/2; i++) {
		_printf("[%x]\n", u16list[i]);
	}
	_printf("3===========================\n");
	for (int i = 0; i < TEST_SIZE/4; i++) {
		_printf("[%x]\n", u32list[i]);
	}
	_printf("4===========================\n");
	for (int i = 0; i < TEST_SIZE/8; i++) {
		_printf("[%lx]\n", u64list[i]);
	}
	_printf("5===========================\n");
}
#else
void test_func(void)
{
	write_csr(mtvec, &trap_freertos);

	_printf("HELLO EYENIX!\n"); // ADD some

	vMemoryHeapInit();

	vTaskCreate("shell", ShellTask, NULL, 4096, 3);

	vTaskStartScheduler();
}
#endif

void test_eth(void)
{
	//GpioOutDir(GPIO_RST_ETHERNET);
	//GpioSetLo(GPIO_RST_ETHERNET);
	//WaitXms(10);
	//GpioSetHi(GPIO_RST_ETHERNET);
}

void main_0(int cpu_id)
{
	*mtime = 0; // timer init

	//uint64_t a = 100;
	//uint64_t b = 50;

	//_printf("%d\n", a-b);

	//Uart7_Init(UART7_SPEED);
	UartInit(7, UART7_SPEED);
	_printf("Start EN675\n");

	//DdrInit();
	//DdrTest();

	GpioInit();
	_printf("Init GPIO\n");

	GpioOutDir(55);
#if 0

	test_func();

#else

	//test_eth();
	//test_func();

	//g_key = 0xA; // CPU0 Ready!

	int k = 0;
	while (1) {
		_printf("%d:%lu\r\n", cpu_id, *mtime);
		if (k%2)
			GpioSetHi(55);
		else
			GpioSetLo(55);
		k++;

		WaitXms(1000);
	}
#endif
}
