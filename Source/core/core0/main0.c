#include "dev.h"

#include <time.h>

#include "enx_freertos.h"
#include "shell.h"

#include "rtc.h"

extern void trap_freertos(void); // mentry.S

char g_key = 0xFF;

extern volatile uint64_t* mtime;

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

void enx_peri_init(void)
{
	GpioInit();
#if USE_UART0
	UartInit(0, UART0_SPEED);
#endif
#if USE_UART1
	UartInit(1, UART1_SPEED);
#endif
#if USE_UART2
	UartInit(2, UART2_SPEED);
#endif
#if USE_UART3
	UartInit(3, UART3_SPEED);
#endif
#if USE_UART4
	UartInit(4, UART4_SPEED);
#endif
#if USE_UART5
	UartInit(5, UART5_SPEED);
#endif
#if USE_UART6
	UartInit(6, UART6_SPEED);
#endif
#if USE_UART7
	UartInit(7, UART7_SPEED);
#endif
#if USE_UART8
	UartInit(8, UART8_SPEED);
#endif
#if USE_I2C0
	I2cInit(0, I2C0_SPEED);
#endif
#if USE_I2C1
	I2cInit(1, I2C1_SPEED);
#endif
#if USE_I2C2
	I2cInit(2, I2C2_SPEED);
#endif
#if USE_I2C3
	I2cInit(3, I2C3_SPEED);
#endif
#if USE_I2C4
	I2cInit(4, I2C4_SPEED);
#endif
#if USE_I2C5
	I2cInit(5, I2C5_SPEED);
#endif
#if USE_I2C6
	I2cInit(6, I2C6_SPEED);
#endif
#if USE_I2C7
	I2cInit(7, I2C7_SPEED);
#endif
#if USE_I2C8
	I2cInit(8, I2C8_SPEED);
#endif
#if USE_SPI0
	SpiInit(0, SPI0_SPEED, 0, 0);
#endif
#if USE_SPI1
	SpiInit(1, SPI1_SPEED, 0, 0);
#endif
#if USE_SPI2
	SpiInit(2, SPI2_SPEED, 0, 0);
#endif
#if USE_SPI3
	SpiInit(3, SPI3_SPEED, 0, 0);
#endif
#if USE_SPI4
	SpiInit(4, SPI4_SPEED, 0, 0);
#endif
#if USE_SPI5
	SpiInit(5, SPI5_SPEED, 0, 0);
#endif
#if USE_SPI6
	SpiInit(6, SPI6_SPEED, 0, 0);
#endif
#if USE_SPI7
	SpiInit(7, SPI7_SPEED, 0, 0);
#endif
#if USE_SPI8
	SpiInit(8, SPI8_SPEED, 0, 0);
#endif

	UartRxEi(7);
}

void enx_device_init(void)
{
#if 0
	for (int i=0;i<256;i++) {
		UINT a = I2cCheck(7, i);
		if (a == DEF_OK) {
			_printf("[%X]");
		} else {
			_printf(".");
		}
	}
#endif

#ifdef __RTC_LOAD__
	rtc_init();
#endif
	set_devicetime(TimeZone_GMT, 2019, 2, 19, 10, 0, 0);
}

extern INT16S cmd_time(INT32S argc, char *argv[]);
extern void IrqStatus(void);
extern void enx_externalirq_init(void);
extern void IrqCheck(void);

void main_0(int cpu_id)
{
	*mtime = 0; // timer init

	uint64_t a = 100;
	uint64_t b = 50;
	_printf("%d\n", a-b);

	enx_peri_init();
	_printf("Start EN675\n");

	enx_externalirq_init();

	enx_device_init();

	//DdrInit();
	//DdrTest();

	GpioOutDir(55);

	//g_key = 0xA; // CPU0 Ready!

	_printf("Init Device\n");

	int k = 0;
	while (1) {
		_printf("%d:%lu\n", cpu_id, *mtime);
		if (k%2)
			GpioSetHi(55);
		else
			GpioSetLo(55);
		k++;

		WaitXms(1000);
		gptMsgShare.TIME++;
		gptMsgShare.UPTIME++;

		//IrqStatus();
		//IrqCheck();

		unsigned long mstatus = read_csr(mstatus);
		_printf("mstatus : %lX\n", mstatus);

		//cmd_time(1, NULL);
	}
}
