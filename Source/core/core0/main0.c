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
	BDmaInit();
	CDmaInit();
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
	GpioInDir(SD_GPIO_RST);
	GpioInDir(SD_GPIO_IRQ);
	SdioCdInit(SD_SDIO_CH);
#endif

#ifdef __USE_WF__
	GpioOutDir(WF_GPIO_RST);
#endif
}

extern int cmd_test_sysreg(int argc, char *argv[]);

#if 0
typedef union {
	int test;
	struct {
		int a:4;
		int b:12;
		int c:8;
		int d:8;
	}
}TEST_A;

#pragma scalar_storage_order big-endian
typedef union {
	int test;
	struct {
		int a:4;
		int b:12;
		int c:8;
		int d:8;
	}
}TEST_B;
#endif

typedef union {
    unsigned long value[2];
    unsigned char byte[16];

    struct {
        unsigned int a : 2;
        unsigned int b : 1;
        unsigned int c : 7;
        unsigned int d : 6;
        unsigned int e : 16;

        unsigned int f;

        unsigned int g : 8;
        unsigned int h : 8;
        unsigned int i : 4;
        unsigned int j : 4;
        unsigned int k :16;

        unsigned int l : 6;
        unsigned int m : 2;
        unsigned int n : 4;
        unsigned int o : 4;
        unsigned int p : 8;
    } field;
} v;

void test_v_printf(v *ssr)
{
	printf("===================================================\n");
	printf("ssr->a	[127:126] : %u(0x%08X)\n", ssr->field.a, ssr->field.a);
	printf("ssr->b	[125:125] : %u(0x%08X)\n", ssr->field.b, ssr->field.b);
	printf("ssr->c	[124:118] : %u(0x%08X)\n", ssr->field.c, ssr->field.c);
	printf("ssr->d	[117:112] : %u(0x%08X)\n", ssr->field.d, ssr->field.d);
	printf("ssr->e	[111: 96] : %u(0x%08X)\n", ssr->field.e, ssr->field.e);
	printf("ssr->f	[ 95: 64] : %u(0x%08X)\n", ssr->field.f, ssr->field.f);
	printf("ssr->g	[ 63: 56] : %u(0x%08X)\n", ssr->field.g, ssr->field.g);
	printf("ssr->h	[ 55: 48] : %u(0x%08X)\n", ssr->field.h, ssr->field.h);
	printf("ssr->i	[ 47: 44] : %u(0x%08X)\n", ssr->field.i, ssr->field.i);
	printf("ssr->j	[ 43: 40] : %u(0x%08X)\n", ssr->field.j, ssr->field.j);
	printf("ssr->k	[ 39: 24] : %u(0x%08X)\n", ssr->field.k, ssr->field.k);
	printf("ssr->l	[ 23: 18] : %u(0x%08X)\n", ssr->field.l, ssr->field.l);
	printf("ssr->m	[ 17: 16] : %u(0x%08X)\n", ssr->field.m, ssr->field.m);
	printf("ssr->n	[ 15: 12] : %u(0x%08X)\n", ssr->field.n, ssr->field.n);
	printf("ssr->o	[ 11:  8] : %u(0x%08X)\n", ssr->field.o, ssr->field.o);
	printf("ssr->p	[  7:  0] : %u(0x%08X)\n", ssr->field.p, ssr->field.p);
}

#if 0
typedef union {
    unsigned long value[1];
    unsigned char byte[8];
    struct {
        unsigned int a1 : 1;
        unsigned int a2 : 1;
        unsigned int a3 : 1;
        unsigned int a4 : 1;
        unsigned int a5 : 1;
        unsigned int a6 : 1;
        unsigned int a7 : 1;
        unsigned int a8 : 1;

        unsigned int b1 : 1;
        unsigned int b2 : 1;
        unsigned int b3 : 1;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int b6 : 1;
        unsigned int b7 : 1;
        unsigned int b8 : 1;

        unsigned int c1 : 1;
        unsigned int c2 : 1;
        unsigned int c3 : 1;
        unsigned int c4 : 1;
        unsigned int c5 : 1;
        unsigned int c6 : 1;
        unsigned int c7 : 1;
        unsigned int c8 : 1;

        unsigned int d1 : 1;
        unsigned int d2 : 1;
        unsigned int d3 : 1;
        unsigned int d4 : 1;
        unsigned int d5 : 1;
        unsigned int d6 : 1;
        unsigned int d7 : 1;
        unsigned int d8 : 1;

        unsigned int e1 : 1;
        unsigned int e2 : 1;
        unsigned int e3 : 1;
        unsigned int e4 : 1;
        unsigned int e5 : 1;
        unsigned int e6 : 1;
        unsigned int e7 : 1;
        unsigned int e8 : 1;

        unsigned int f1 : 1;
        unsigned int f2 : 1;
        unsigned int f3 : 1;
        unsigned int f4 : 1;
        unsigned int f5 : 1;
        unsigned int f6 : 1;
        unsigned int f7 : 1;
        unsigned int f8 : 1;

        unsigned int g1 : 1;
        unsigned int g2 : 1;
        unsigned int g3 : 1;
        unsigned int g4 : 1;
        unsigned int g5 : 1;
        unsigned int g6 : 1;
        unsigned int g7 : 1;
        unsigned int g8 : 1;

        unsigned int h1 : 1;
        unsigned int h2 : 1;
        unsigned int h3 : 1;
        unsigned int h4 : 1;
        unsigned int h5 : 1;
        unsigned int h6 : 1;
        unsigned int h7 : 1;
        unsigned int h8 : 1;
    } field;
} q;


void test_q_printf(q *__q)
{
	printf("===================================================\n");
	printf("__q->a1 [127] : %u(0x%08X)\n", __q->field.a1, __q->field.a1);
	printf("__q->b	[125] : %u(0x%08X)\n", __q->field.b, __q->field.b);
	printf("__q->c	[124] : %u(0x%08X)\n", __q->field.c, __q->field.c);
	printf("__q->d	[117] : %u(0x%08X)\n", __q->field.d, __q->field.d);
	printf("__q->e	[111] : %u(0x%08X)\n", __q->field.e, __q->field.e);
	printf("__q->f	[ 95] : %u(0x%08X)\n", __q->field.f, __q->field.f);
	printf("__q->g	[ 63] : %u(0x%08X)\n", __q->field.g, __q->field.g);
	printf("__q->h	[ 55] : %u(0x%08X)\n", __q->field.h, __q->field.h);
	printf("__q->i	[ 47] : %u(0x%08X)\n", __q->field.i, __q->field.i);
	printf("__q->j	[ 43] : %u(0x%08X)\n", __q->field.j, __q->field.j);
	printf("__q->k	[ 39] : %u(0x%08X)\n", __q->field.k, __q->field.k);
	printf("__q->l	[ 23] : %u(0x%08X)\n", __q->field.l, __q->field.l);
	printf("__q->m	[ 17] : %u(0x%08X)\n", __q->field.m, __q->field.m);
	printf("__q->n	[ 15] : %u(0x%08X)\n", __q->field.n, __q->field.n);
	printf("__q->o	[ 11] : %u(0x%08X)\n", __q->field.o, __q->field.o);
	printf("__q->p	[  7] : %u(0x%08X)\n", __q->field.p, __q->field.p);
}
#endif

void main_0(int cpu_id)
{
	*mtime = 0; // timer init

	enx_peri_init();
	printf("Start EN675\n");
#if 0
	TEST_A a;
	TEST_B b;
	a.a = 0x1;		a.b = 0x234;		a.c = 0x56;		a.d = 0x78;
	b.a = 0x1;		b.b = 0x234;		b.c = 0x56;		b.d = 0x78;
	printf("Sizeof (%d / %d)\n", sizeof(a), sizeof(b));
	printf("addr A: 0x%08X\n", a.test);
	printf("addr B: 0x%08X\n", b.test);
#endif
#if 0
	UINT k[4] = {0x11223344, 0x55667788, 0x99aabbcc, 0xddeeff00};
	UINT j[4] = {0};

	hwflush_dcache_range((UINT)j, ((UINT)j)+16);
	DmaMemCpy_isr(0, j, k, 16);
	hwflush_dcache_range((UINT)j, ((UINT)j)+16);

	printf("j: %X %X %X %X\n", j[0], j[1], j[2], j[3]);
#endif
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
#if 0
	// 80 00 00 00 05 00 00 00
	// 04 00 90 00 00 01 1b 00  ................
	v test_v;
	test_v.value[0] = 0x8000000005000000;
	test_v.value[1] = 0x0400900000011b00;
	hexDump("V", &test_v, 16);

	printf("=======================================\n");
	int ss, ee;
	UINT *un32getData = &test_v;
	ss = 127;
	ee = 96;
	for (int k = 0; k < 4; k++) {
		printf("DATA[%3d:%3d] %u(0x%08X)\n", ss, ee, un32getData[k], un32getData[k]);
		ss -= 32;
		ee -= 32;
	}
	printf("=======================================\n");
	ss = 127; ee = 128;
	BYTE *un8getData = &test_v;
	for (int k = 0; k < 8; k++) {
		printf("DATA[%3d:%3d] %u(0x%02X)\n", ss, ee, un8getData[k], un8getData[k]);
		ss -= 8;
		ee -= 8;
	}
	printf("=======================================\n");
	test_v_printf(&test_v);
	printf("END====================================\n");
	while(1);
#endif
	enx_externalirq_init();
	//enx_timerirq_init();

	enx_device_init();

//	DdrInit();
//	DdrTest();

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
