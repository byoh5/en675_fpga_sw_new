#include "dev.h"

char g_key = 0xFF;

void Uart7RxIrqTest(void *ctx)
{
	UINT bBuf = UartRxGetByte(7);
	UartTx(7, (char)bBuf);
}

#if 1
#include "enx_freertos.h"
#include "shell.h"
#include "ledblink.h"
#include "sdcard.h"
extern void trap_freertos(void); // mentry.S

void TestTask(void* pvParameters)
{
	while (1) {
		printf("Test Task\n");
		vTaskDelay(3000);
	}
}

void test_freertos(void)
{
	write_csr(mtvec, &trap_freertos);

	vMemoryHeapInit();

	vTaskCreate("shell", ShellTask, NULL, 4096, 3);
	vTaskCreate("ledblink", LedblinkTask, NULL, 1024, 3);
#ifdef __USE_SD__
	vTaskCreate("sdcard", SdcardTask, NULL, 4096, 3);
#endif


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
#if USE_SDIO0
	SdioInit(0, SDIO0_SPEED);
#endif
#if USE_SDIO1
	SdioInit(1, SDIO1_SPEED);
#endif
	//UartRxIrqCallback(7, Uart7RxIrqTest, NULL);
	//UartRxIrqOn(7);
}

void enx_device_init(void)
{
#if 0
	for (int i=0;i<256;i++) {
		UINT a = I2cCheck(7, i);
		if (a == DEF_OK) {
			printf("[%X]");
		} else {
			printf(".");
		}
	}
#endif

#ifdef __USE_LED0__
	GpioOutDir(GPIO_LED0);
#endif

#ifdef __USE_LED1__
	GpioOutDir(GPIO_LED1);
#endif

#ifdef __USE_LED2__
	GpioOutDir(GPIO_LED2);
#endif

#ifdef __ETHERNET__
	EthInit();
	EthphyInit(ETHPHY_MDIO_ADR);
#endif

#ifdef __RTC_LOAD__
	rtc_init();
#endif
	set_devicetime(TimeZone_GMT, 2019, 2, 25, 10, 0, 0);

#ifdef __USE_SD__
	GpioOutDir(SD_GPIO_RST);
	GpioInDir(SD_GPIO_IRQ);
	SdioCdInit(SD_SDIO_CH);
#endif

#ifdef __USE_WF__
	GpioOutDir(WF_GPIO_RST);
#endif
}

extern int cmd_test_sysreg(int argc, char *argv[]);

void main_0(int cpu_id)
{
	*mtime = 0; // timer init

	enx_peri_init();
	printf("Start EN675\n");

#if 0
	uint64_t a = 100;
	uint64_t b = 50;
	uint64_t c = a+b;
	printf("%d,%d\n", a-b, c);
#endif
#if 0
	float a = 3.2f;
	float b = 1.2f;
	printf("%d\n", (int)(a/b));
#endif

	enx_externalirq_init();
	//enx_timerirq_init();

	enx_device_init();

	//DdrInit();
	//DdrTest();

	g_key = 0xA; // CPU0 Ready!

	printf("Init Device\n");

	test_freertos();

#if 0
	int k = 0;
	while (1) {
		//IrqStatus();
		//IrqCheck();

		//unsigned long mstatus = read_csr(mstatus);
		//printf("mstatus : %lX\n", mstatus);
		//printf("mstatus : %lX, Uptime(%lu)\n", mstatus, _getuptime());

		static uint64_t oldt = 0;
		if (oldt != gptMsgShare.TIME) {
			oldt = gptMsgShare.TIME;
			extern INT16S cmd_time(INT32S argc, char *argv[]);
			cmd_time(1, NULL);

			//extern void IrqStatus(void);
			//IrqStatus();

			printf("%d:%16lu\n", cpu_id, *mtime);

			if (k++%2)	GpioSetHi(55);
			else		GpioSetLo(55);
		}

		//exit(1);

		WaitXms(1);
	}
#endif
}
