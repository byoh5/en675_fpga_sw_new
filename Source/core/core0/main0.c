#include "dev.h"
#include "ethphy.h"

char g_key = 0xFF;

#ifdef __FREERTOS__
#include "enx_freertos.h"
#include "networkif.h"
#include "shell.h"
#include "ledblink.h"
#include "sdcard.h"
#include "iperf.h"
extern void trap_freertos(void); // mentry.S

void TestTask(void* pvParameters)
{
	while (1) {
		printf("Test Task\n");
		vTaskDelay(3000);
	}
}

#if defined(__ETHERNET__)
void startNetProtocol(void *arg)
{
	// Wait for network activation.
	network_check_netif();

#if (NET_LWIPERF==1)
	lwiperf_init();
#endif
#if (NET_LWIPHTTP==1)
	extern void httpd_init(void);
	httpd_init();
#endif
#if (NET_SNTPC==1)
	if (gtNetwork.u1UseSntp == DEF_ON) {
		sntpc_start();
	}
#endif

	vTaskDelete(NULL);
	UNUSED(arg);
}
#endif

void test_freertos(void)
{
	vMemoryHeapInit();

	vTaskCreate("shell", ShellTask, NULL, LV2_STACK_SIZE, LV6_TASK_PRIO);

#ifdef __USE_SDIOCD__
	vTaskCreate("sdcard", SdcardTask, NULL, LV1_STACK_SIZE, LV3_TASK_PRIO);
#endif
#ifdef __NETWORK__
	vTaskCreate("startLwip", startLwip, NULL, LV3_STACK_SIZE, LV7_TASK_PRIO);
#endif
	vTaskCreate("ledblink", LedblinkTask, NULL, LV0_STACK_SIZE, LV7_TASK_PRIO);
#ifdef __NETWORK__
	vTaskCreate("netProt", startNetProtocol, NULL, LV2_STACK_SIZE, LV5_TASK_PRIO);
#endif
	vTaskStartScheduler();
}


#if 0
#if 1
void *__mmap (void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __offset)
{
	flprintf("\n");
}

void *malloc(size_t x)
{
	return pvPortMalloc(x);
}

void *realloc(void *x, size_t y)
{
	return pvPortRealloc(x, y);
}

void *calloc(size_t x, size_t y)
{
	return pvPortCalloc(x, y);
}

void free(void *x)
{
	vPortFree(x);
}
#else

#define malloc(x) pvPortMalloc(x)
#define realloc(x, y) pvPortRealloc(x, y)
#define calloc(x, y) pvPortCalloc(x, y)
#define free(x) vPortFree(x)

#endif
#endif


#endif

void enx_peri_init(void)
{
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
	//UartInit(7, UART7_SPEED);
	//UartInit(7, 230400);
	//UartInit(7, 460800);
	//UartInit(7, 921600);
	UartInit(7, 625000);
#endif
#if USE_UART8
	UartInit(8, UART8_SPEED);
#endif

	BDmaInit();
	CDmaInit();


	//DDR_MEM_SEL = 0; //  16MByte(128Mb) S2/S4
	//DDR_MEM_SEL = 1; //  32MByte(256Mb) S2/S4
	//DDR_MEM_SEL = 2; //  64MByte(512Mb) S2/S4
	//DDR_MEM_SEL = 3; // 128MByte(1Gb) S2
	//DDR_MEM_SEL = 4; // 128MByte(1Gb) S4
	//DDR_MEM_SEL = 5; // 256MByte(2Gb) S2
	//DDR_MEM_SEL = 6; // 256MByte(2Gb) S4
	//DDR_MEM_SEL = 7; // 512MByte(4Gb) S2/S4
	//DdrInit(2);
	//DDR_PWR_REQ = 1;
	//DdrInit(2);
#if 1
	DdrInit(0,2);

#if 0
	UINT *test1 = 0x80000000;
	UINT *test2 = 0x80010000;

#if 1
	for (UINT i=0;i<4;i++) {
		BDmaMemCpy_isr(0, test1+(i*512), test2+(i*512), 512);
	}
#else
	BDmaMemCpy_isr(0, test1+(0*512), test2+(0*512), 512);
	BDmaMemCpy_isr(0, test1+(1*512), test2+(1*512), 512);
	BDmaMemCpy_isr(0, test1+(2*512), test2+(2*512), 512);
	BDmaMemCpy_isr(0, test1+(3*512), test2+(3*512), 512);
#endif
	printf("Done\n");
#endif
	hwflush_dcache_range(DDR0_BASE, 16*1024);
	BDmaMemSet_isr(0, (BYTE *)DDR0_BASE, 0x00, DDR0_SIZE);
	//memset((BYTE *)DDR0_BASE, 0x00, DDR0_SIZE);
	hwflush_dcache_range(DDR0_BASE, 16*1024);
#else
	register long t0 asm("t0") = 0;
	register long t1 asm("t1") = 0;
	asm("la t0, __bss_s");
	asm("la t1, __bss_e");

	hwflush_dcache_range(t0, 16*1024);
	memset(t0, 0, t1 - t0);
	hwflush_dcache_range(t0, 16*1024);
#endif

	DdrInit(1,2);

	hwflush_dcache_range(DDR1_BASE, 16*1024);
	BDmaMemSet_isr(0, (BYTE *)DDR1_BASE, 0x00, DDR1_SIZE);
	//memset((BYTE *)DDR1_BASE, 0x00, DDR1_SIZE);
	hwflush_dcache_range(DDR1_BASE, 16*1024);

	// DDR
	printf("%s", TTY_COLOR_RESET);

//	printf("UART7 Div: %u\n", UART7_CLK_DIV);

	//printf("ETH_MDIO_REG0_T: 0x%08X\n", ETH_MDIO_REG0_T);
	///ETH_MDIO_REG0_T = 0xffffffff;
	//printf("ETH_MDIO_REG0_T: 0x%08X\n", ETH_MDIO_REG0_T);

	//SFLS_IO_RXEDGE = 1;
	//SFLS_IO_TXEDGE = 0;
	SflsGetinfo();
	SflsInit();
	printf("set SFLS_IO_RXEDGE(%u) SFLS_IO_TXEDGE(%u)\n", SFLS_IO_RXEDGE, SFLS_IO_TXEDGE);
	SflsGetinfo();


#if USE_I2C0
	I2cInit(0, I2C0_SPEED);
//	I2cChCheck(0);
#endif
#if USE_I2C1
	I2cInit(1, I2C1_SPEED);
//	I2cChCheck(1);
#endif
#if USE_I2C2
	I2cInit(2, I2C2_SPEED);
//	I2cChCheck(2);
#endif
#if USE_I2C3
	I2cInit(3, I2C3_SPEED);
//	I2cChCheck(3);
#endif
#if USE_I2C4
	I2cInit(4, I2C4_SPEED);
//	I2cChCheck(4);
#endif
#if USE_I2C5
	I2cInit(5, I2C5_SPEED);
//	I2cChCheck(5);
#endif
#if USE_I2C6
	I2cInit(6, I2C6_SPEED);
//	I2cChCheck(6);
#endif
#if USE_I2C7
	I2cInit(7, I2C7_SPEED);
//	I2cChCheck(7);
#endif
#if USE_I2C8
	I2cInit(8, I2C8_SPEED);
//	I2cChCheck(8);
#endif


#if USE_SPI0
	SpiInit(0, SPI0_SPEED, SPI0_BIT, SPI0_LSB);
#endif
#if USE_SPI1
	SpiInit(1, SPI1_SPEED, SPI1_BIT, SPI1_LSB);
#endif
#if USE_SPI2
	SpiInit(2, SPI2_SPEED, SPI2_BIT, SPI2_LSB);
#endif
#if USE_SPI3
	SpiInit(3, SPI3_SPEED, SPI3_BIT, SPI3_LSB);
#endif
#if USE_SPI4
	SpiInit(4, SPI4_SPEED, SPI4_BIT, SPI4_LSB);
#endif
#if USE_SPI5
	SpiInit(5, SPI5_SPEED, SPI5_BIT, SPI5_LSB);
#endif
#if USE_SPI6
	SpiInit(6, SPI6_SPEED, SPI6_BIT, SPI6_LSB);
#endif
#if USE_SPI7
	SpiInit(7, SPI7_SPEED, SPI7_BIT, SPI7_LSB);
#endif
#if USE_SPI8
	SpiInit(8, SPI8_SPEED, SPI8_BIT, SPI8_LSB);
#endif


#if USE_SDIO0
	SdioInit(0, SDIO0_SPEED);
#endif
#if USE_SDIO1
	SdioInit(1, SDIO1_SPEED);
#endif


#if USE_TIMER0
	TimerInit(0);
#endif
#if USE_TIMER1
	TimerInit(1);
#endif
#if USE_TIMER2
	TimerInit(2);
#endif
#if USE_TIMER3
	TimerInit(3);
#endif
#if USE_TIMER4
	TimerInit(4);
#endif
#if USE_TIMER5
	TimerInit(5);
#endif
#if USE_TIMER6
	TimerInit(6);
#endif
#if USE_TIMER7
	TimerInit(7);
#endif
#if USE_TIMER8
	TimerInit(8);
#endif
#if USE_TIMER9
	TimerInit(9);
#endif
#if USE_TIMER10
	TimerInit(10);
#endif
#if USE_TIMER11
	TimerInit(11);
#endif
#if USE_TIMER12
	TimerInit(12);
#endif
#if USE_TIMER13
	TimerInit(13);
#endif
#if USE_TIMER14
	TimerInit(14);
#endif
#if USE_TIMER15
	TimerInit(15);
#endif
#if USE_TIMER16
	TimerInit(16);
#endif
#if USE_TIMER17
	TimerInit(17);
#endif
#if USE_TIMER18
	TimerInit(18);
#endif
#if USE_TIMER19
	TimerInit(19);
#endif
#if USE_TIMER20
	TimerInit(20);
#endif
#if USE_TIMER21
	TimerInit(21);
#endif
#if USE_TIMER22
	TimerInit(22);
#endif
#if USE_TIMER23
	TimerInit(23);
#endif
#if USE_TIMER24
	TimerInit(24);
#endif
#if USE_TIMER25
	TimerInit(25);
#endif
#if USE_TIMER26
	TimerInit(26);
#endif
#if USE_TIMER27
	TimerInit(27);
#endif
#if USE_TIMER28
	TimerInit(28);
#endif
#if USE_TIMER29
	TimerInit(29);
#endif
#if USE_TIMER30
	TimerInit(30);
#endif
#if USE_TIMER31
	TimerInit(31);
#endif
#if USE_TIMER32
	TimerInit(32);
#endif
#if USE_TIMER33
	TimerInit(33);
#endif
#if USE_TIMER34
	TimerInit(34);
#endif
#if USE_TIMER35
	TimerInit(35);
#endif
#if USE_TIMER36
	TimerInit(36);
#endif
#if USE_TIMER37
	TimerInit(37);
#endif
#if USE_TIMER38
	TimerInit(38);
#endif

	GpioInit();

	AdcInit(1000000);
//	AdcOn(0);
//	AdcOn(1);
//	AdcOn(2);
//	AdcOn(3);
//	AdcOn(4);
//	AdcOn(5);
//	AdcOn(6);
//	AdcOn(7);

#if USE_I2S
	I2sInit();
	I2sSlvInit();
#endif

	//BusInit();

#if USE_ETH
	EthInit();
	MdioInit(MDIO_SPEED);
#endif
}

void enx_sys_init(void)
{
	write_csr(mstatus, MSTATUS_FS); // Enable
	//for (int i = 0; i < 32; i++) {
	//	init_fp_reg(i);
	//}
	//write_csr(fcsr, 0);

	UINT arrMark[5];
	arrMark[0] = SYS_MARK0;
	arrMark[1] = SYS_MARK1;
	arrMark[2] = SYS_MARK2;
	arrMark[3] = SYS_MARK3;
	arrMark[4] = SYS_MARK4;
	char *strName = (char *)arrMark;
	printf("%s", TTY_COLOR_CYAN);
	for (int i = 0; i < sizeof(arrMark); i++) {
		printf("%c", strName[i]);
	}
	printf("%04X-%02X-%02X %02X:%02X:%02X%s\n", SYS_RTL_YEAR, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MINUTE, SYS_RTL_SECOND, TTY_COLOR_RESET);

#ifdef __FREERTOS__
	;
#else
	UartRstQue();
	UartRxIrqCallback(DEBUG_UART_NUM, UartDebugRxIrq, NULL);
	UartRxSetIrqEn(DEBUG_UART_NUM,1);
	UartTxIrqCallback(DEBUG_UART_NUM, UartDebugTxIrq, NULL);
#endif
}

void enx_device_init(void)
{
#ifdef __USE_LED0__
	GpioSetDir(GPIO_LED0, GPIO_DIR_OUT);
#endif

#ifdef __USE_LED1__
	GpioSetDir(GPIO_LED1, GPIO_DIR_OUT);
#endif

#ifdef __USE_LED2__
	GpioSetDir(GPIO_LED2, GPIO_DIR_OUT);
#endif

#ifdef __HDMI__
	InitHdmi();
	D_ITU_YCCH0w(1);				//	For Changing Y/C -> Schematics
	//	D_BT20_ISEL0w(1);			//	Select YCBCR1 Image	-> Schematics
#endif

#ifdef __SENSOR__
	GpioFuncPinOff(SENSOR_RESET_GPIO_CH);	//	GPIO
	GpioSetDir(SENSOR_RESET_GPIO_CH, GPIO_DIR_OUT);		//	GPIO 1 Output
	GpioSetOut(SENSOR_RESET_GPIO_CH,1);		//	Sensor Reset High
#endif

#ifdef __RTC_LOAD__
	rtc_init();
#endif
	set_devicetime(TimeZone_GMT, 2019, 5, 9, 21, 0, 0);

#ifdef __AUDIO__
	GpioSetDir(AUDIO_GPIO_RST, GPIO_DIR_OUT);
	GpioSetDir(AUDIO_GPIO_IRQ, GPIO_DIR_IN);
	//audio_init();
#endif

#ifdef __USE_SDIOCD__
	GpioSetDir(SD_GPIO_RST, GPIO_DIR_IN);
	GpioSetDir(SD_GPIO_IRQ, GPIO_DIR_IN);
	SdioCdInit(SD_SDIO_CH);
#endif

#ifdef __USE_WF__
	GpioSetDir(WF_GPIO_RST, GPIO_DIR_OUT);
#endif

#ifdef __EEPROM__
//	I2cInit(EEPROM_I2C_CH, EEPROM_I2C_SPEED);	// FPGA 보드에서 사용하는 24AA64 는 1.8V로 동작하며 100KHz로 동작함
	EepromInit();
#endif
}

void enx_default_userinfo(void)
{
#ifdef __ETHERNET__
	gtSystem.arr8MacAddress[0] = 0x00;
	gtSystem.arr8MacAddress[1] = 0x01;
	gtSystem.arr8MacAddress[2] = 0x52;
	gtSystem.arr8MacAddress[3] = 0x34;
	gtSystem.arr8MacAddress[4] = 0x35;
	gtSystem.arr8MacAddress[5] = 0x37;
	EthMacAddrCheck((BYTE *)gtSystem.arr8MacAddress);

	gtNetwork.naEthernet.u1UseDhcp = NET_DHCP;
	if (gtNetwork.naEthernet.u1UseDhcp == ENX_ON) {
		gtNetwork.naEthernet.u32IpAddr = 0;
		gtNetwork.naEthernet.u32NetMask = 0;
		gtNetwork.naEthernet.u32Gateway = 0;
		gtNetwork.naEthernet.u32DnsSvr0 = 0;
		gtNetwork.naEthernet.u32DnsSvr1 = 0;
	} else {
		gtNetwork.naEthernet.u32IpAddr = ipaddr_addr(ETH_IPADDR);
		gtNetwork.naEthernet.u32NetMask = ipaddr_addr(ETH_NETMASK);
		gtNetwork.naEthernet.u32Gateway = ipaddr_addr(ETH_GWADDR);
		gtNetwork.naEthernet.u32DnsSvr0 = ipaddr_addr(NET_DNS_ADDR0);
		gtNetwork.naEthernet.u32DnsSvr1 = ipaddr_addr(NET_DNS_ADDR1);
	}

#if (NET_SNTPC==1)
	gtNetwork.u1UseSntp = NET_SNTP_START;
	strcpy((char *)gtNetwork.strSntpSvr, NET_SNTP_SRVR);
	gtNetwork.u32SntpPeriodic = NET_SNTP_PERIODIC;
	gtNetwork.u32SntpRetrySec = NET_SNTP_RETRY_SEC;
	gtNetwork.u32SntpRetryMaxcnt = NET_SNTP_RETRY_CNT;
#endif
#endif
}

extern int cmd_test_sysreg(int argc, char *argv[]);

#if 0
void main_ddr_init(void)
{
	UartInit(7, 625000);

	DdrInit(2);

	//hwflush_dcache_range(DDR0_BASE, 16*1024);
	BDmaMemSet_isr(0, (BYTE *)DDR0_BASE, 0x00, DDR0_SIZE);
	//hwflush_dcache_range(DDR0_BASE, 16*1024);

	printf("DDR Init\n");

	while (1) {
		asm ("EBREAK");
	}

	printf("Out\n");
}
#endif

#if 0
typedef struct {
    BYTE *arrSrc;
    BYTE *arrDst;
    ULONG u64BufSize;

    UINT u32TestSize;
    UINT u32Move;
    UINT cMode;
    UINT u32CH;
} DmaTestStrfw;

DmaTestStrfw dmaitem;

UINT dma_done_flag  = 0;

void dma_irq(void *ctx)
{
    dma_done_flag = 1;
}

void dmatest(UINT nCH, char cMode, UINT nLoop)
{
    dmaitem.u64BufSize = 16 * 1024;
    dmaitem.u32TestSize = -1;
    dmaitem.u32Move = nLoop;
    dmaitem.cMode = cMode;
    dmaitem.u32CH = nCH;

    dmaitem.arrSrc = 0x80020000;
//    dmaitem.arrSrc = 0x8015ADC0;
    if (dmaitem.arrSrc == NULL) {
        printf("malloc error(arrSrc), size(%u)\n", dmaitem.u64BufSize);
        return;
    }

    dmaitem.arrDst = 0x80030000;
//    dmaitem.arrDst = 0x8015EDD0;
    if (dmaitem.arrDst == NULL) {
        printf("malloc error(arrDst), size(%u)\n", dmaitem.u64BufSize);
        return;
    }

    BYTE *parrSrc = dmaitem.arrSrc;
    BYTE *parrDst = dmaitem.arrDst;

    if (dmaitem.cMode == 'B') {
        BDmaIrqCallback(dmaitem.u32CH, dma_irq, NULL);
        BDmaSetIrqEn(dmaitem.u32CH, ENX_ON);
    } else if (dmaitem.cMode == 'C') {
        CDmaIrqCallback(dmaitem.u32CH, dma_irq, NULL);
        CDmaSetIrqEn(dmaitem.u32CH, ENX_ON);
    } else {
        printf("mode error, '%c'\n", dmaitem.cMode);
        return;
    }

    //srand(rdcycle());
    srand(1);

    printf("%cDMA%u Test Task(Move:%u)(Count:%u)", dmaitem.cMode, dmaitem.u32CH, dmaitem.u32Move, dmaitem.u32Move * dmaitem.u32Move);
    if (dmaitem.u32TestSize == -1) {
        printf("(Size:Random)\n");
    } else {
        printf("(Size:%ubyte)\n", dmaitem.u32TestSize);
    }

    UINT u32RelTestSize = 0;
    if (dmaitem.u64BufSize > 16 * 1024 * 1024) {
        u32RelTestSize = 16 * 1024 * 1024;
    } else {
        u32RelTestSize = dmaitem.u64BufSize;
    }
    hwflush_dcache_range((ULONG)dmaitem.arrSrc, u32RelTestSize);
    for (UINT i = 0; i < dmaitem.u64BufSize; i++) {
        dmaitem.arrSrc[i] = rand() % 255;
        //dmaitem.arrSrc[i] = 0x22;
    }
    //hwflush_dcache_range((ULONG)dmaitem.arrSrc, u32RelTestSize);
    hwflush_dcache_range_all();

    //hexDump("ORG", parrSrc, 5100);
    BYTE *pError = parrSrc + 5099;
    printf("[%02X]", *pError);


    u32RelTestSize = 0;
    UINT pass = 0, fail = 0, flag = 0;
    for (UINT i = 0; i < dmaitem.u32Move; i++) {
        parrDst = dmaitem.arrDst;
        if (dmaitem.u32TestSize == -1) {
            //u32RelTestSize = rand() % 8192 + 1;
            u32RelTestSize = rand() % 1024 + 5120;
        } else {
            u32RelTestSize = dmaitem.u32TestSize;
        }

        u32RelTestSize = 5099+1;

        printf("%3u%% %4u/%4u TEST Dst(0x%08X~0x%08X) Src(0x%08X) - Size(%4u) - ", ((i+1) * 100) / dmaitem.u32Move, i+1, dmaitem.u32Move, parrDst, parrDst+dmaitem.u32Move, parrSrc, u32RelTestSize);
        for (UINT j = 0; j < dmaitem.u32Move; j++) {
            // Set array
            //hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);
            BDmaMemSet_isr(dmaitem.u32CH, parrDst, 0x0, u32RelTestSize);
            //hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);

            //memset(parrDst, 0, u32RelTestSize);

            //portMEMORY_BARRIER();
#if 1
//            printf(".");
            dma_done_flag = 0;
            if (dmaitem.cMode == 'B') {
                // Set array
                //hwflush_dcache_range((ULONG)parrDst, u32RelTestSize);
//                portMEMORY_BARRIER();
                //hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);
                //BDmaMemSet_isr(dmaitem.u32CH, parrSrc, 0x00, 7000);
                //BDmaMemSet_isr(dmaitem.u32CH, parrDst, 0x0F, 7000);
        //        hwflush_dcache_range((ULONG)parrSrc, u32RelTestSize);
                BDmaMemCpy_isr(dmaitem.u32CH, parrDst, parrSrc, u32RelTestSize);
                printf("[%02X]", *pError);
                //BDmaMemCpy_isr(dmaitem.u32CH, parrDst, parrSrc, u32RelTestSize);

                //memcpy(parrDst, parrSrc, u32RelTestSize);
                //dma_done_flag = 1;

                //portMEMORY_BARRIER();
                //hwflush_dcache_range((ULONG)parrDst, u32RelTestSize);

            } else if (dmaitem.cMode == 'C') {
                // dma copy
//                hwflush_dcache_range((ULONG)parrDst, u32RelTestSize);
                CDmaMemCpy_isr_async(dmaitem.u32CH, parrDst, parrSrc, u32RelTestSize);
            }

//            printf(",");
            int dma_tick = 0;
            int dma_timeout = 0;
            while (dma_done_flag == 0) {
                dma_tick++;
                WaitXms(1);
                if (dma_tick > 3000) {
                  //  dma_timeout = 1;
                	printf("Timeout\n");
                    break;
                }
            }
//            printf("+");

            if (dma_timeout == 0) { // Timeout 3sec
                if (dmaitem.cMode == 'B') {
                    //portMEMORY_BARRIER();
                    printf("[1:%02X]", *pError);
                    hwflush_dcache_range((ULONG)parrDst, u32RelTestSize);
                    //hwflush_dcache_range_all();
                    printf("[2:%02X]", *pError);
                    //asm volatile ("fence rw,rw");
                    //asm volatile ("fence w,r");
                }

                // comp
                flag = 0;
                UINT k;
#if 0
                k = memcmp(parrSrc, parrDst, u32RelTestSize);
                if (k != 0) {
                    flag = 1;
                }
#else
                for (k = 0; k < u32RelTestSize; k++) {
//                    printf("%x, %x",parrSrc[k],parrDst[k]);
//                    printf("[%02X]", *pError);
                    if (parrSrc[k] != parrDst[k]) {
                        flag = 1;
//                        printf("%x, %x\n",parrSrc[k-1],parrDst[k-1]);
                        printf("%x, %x\n",parrSrc[k],parrDst[k]);
//                        printf("%x, %x\n",parrSrc[k+1],parrDst[k+1]);
                        break;
                    }
                }
                printf("[%02X]", *pError);
#endif

                if (flag == 0) {
                    pass++;
                    //printf("OK\n");
                    //printf(".");
#if 1
                    k = 5099;
                    hexCmpDump("Dma Test", parrSrc + k - 64, parrDst + k - 64, u32RelTestSize - k + 128);
                    while(1);
#endif
                } else {
                    _Rprintf("Fail\nError(%u/%u) Move(%u) Dst(0x%08X)\n", k, u32RelTestSize, j, parrDst);
#if 0
                    hexCmpDump("Dma Test", parrSrc, parrDst, u32RelTestSize);
#else


                    hexCmpDump("Dma Test", parrSrc + k - 64, parrDst + k - 64, u32RelTestSize - k + 64);

                    hwflush_dcache_range_all();

                    //hwflush_dcache_range(parrSrc + k, 256);
                    //hwflush_dcache_range(parrDst + k, 256);
                    hexDump("parrSrc", parrSrc + k - 64, 256);
                    hexDump("parrDst", parrDst + k - 64, 256);
#endif

                    i = j = -0xf;
                    fail++;
                    break;
                }
            } else {
                flag = 1;
                _Rprintf("\nTimeout! 0x%08X <- 0x%08X, Len:%ubyte\n", parrDst, parrSrc, u32RelTestSize);
                if (dmaitem.cMode == 'C') {
                    extern void CDmaRegshow(UINT nCH);
                    CDmaRegshow(dmaitem.u32CH);
                }
                i = j = -0xf;
                fail++;
                break;
            }
//            printf("-");
#endif
            // next
            parrDst++;
        }

        if (flag == 0) {
            _Gprintf("OK\n");
        } else {
            _Rprintf("Fail\n");
        }

        parrSrc++;
    }
    printf("\nTest End. Pass(%d) Fail(%d)\n", pass, fail);

    if (dmaitem.cMode == 'B') {
        BDmaIrqCallback(dmaitem.u32CH, NULL, NULL);
        BDmaSetIrqEn(dmaitem.u32CH, ENX_OFF);
    } else if (dmaitem.cMode == 'C') {
        CDmaIrqCallback(dmaitem.u32CH, NULL, NULL);
        CDmaSetIrqEn(dmaitem.u32CH, ENX_OFF);
    }
}
#endif
void main_0(int cpu_id)
{
	*mtime = 0; // timer init

	//main_ddr_init();

	enx_peri_init();
	enx_sys_init();

	//enx_externalirq_init();
	//enx_timerirq_init();

	enx_device_init();
	enx_default_userinfo();

	g_key = 0xA; // CPU0 Ready!

	printf("Init Device\n");

#if 0
	enx_externalirq_init();
	dmatest(0, 'B', 200);
	while(1);
#endif
#ifdef __FREERTOS__
	test_freertos();
#elif defined(__SENSOR__)
	enx_externalirq_init();

	Isp_init();

	_printf("--------- Main Loop Start --------- \r\n");

	while (1)
	{
	#ifdef	ISP_VLOCKO
		Wait_VLOCKO();
	#else
		Wait_VLOCKO1();
	#endif

		isp_main();

		IF_Funcs();

//		ddr_control();	// for DDR Test, 사용하지 않음
	}
#else
	while (1) {
		printf(".");
		WaitXms(1000);
	}
#endif
}
