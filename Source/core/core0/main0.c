#include "dev.h"
#include "enx_rtos.h"

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
	UartInit(7, UART7_SPEED);
	//UartInit(7, 230400);
	//UartInit(7, 460800);
	//UartInit(7, 921600);
	//UartInit(7, 625000);
#endif
#if USE_UART8
	UartInit(8, UART8_SPEED);
#endif

	BDmaInit();
	CDmaInit();

#if 0
	DdrInit(0, 2);
	hwflush_dcache_range_all();
	BDmaMemSet_isr(0, (BYTE *)DDR0_BASE, 0x00, DDR0_SIZE);
#if 1
	DdrInit(1, 2);
	BDmaMemSet_isr(0, (BYTE *)DDR1_BASE, 0x00, DDR1_SIZE);
#endif
	hwflush_dcache_range_all();
#else
	register ULONG hs, he;
	asm volatile("la %0, __bss_s" : "=r"(hs));
	asm volatile("la %0, __bss_e" : "=r"(he));
	hwflush_dcache_range(hs, 16*1024);
	//memset((void *)hs, 0, he - hs);
	BDmaMemSet_isr(0, (BYTE *)hs, 0x00, he - hs);
	hwflush_dcache_range(hs, 16*1024);
#endif

	printf("%s\n\n", TTY_COLOR_RESET);

	SflsInit();

#if 0//model_TgtBd == 2		// RX Phy Reset
	GPIO_PIN2_OEN	=	0;		//	Output
	GPIO_PIN3_OEN	=	0;		//	Output
	GPIO_PIN2_OUT	=	0;
	GPIO_PIN3_OUT	=	0;
	WaitXms(10);
	GPIO_PIN2_OUT	=	1;
	GPIO_PIN3_OUT	=	1;
	INIT_DELAY(1);	// TODO KSH x 필요?
#endif

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


#if USE_SDIO0 && (model_TgtBd != 1)
	SdioInit(0, SDIO0_SPEED);
#endif
#if USE_SDIO1 && (model_TgtBd != 1)
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

//	AdcInit(1000000);
//	AdcOn();

#if USE_I2S
	I2sInit();
	I2sSlvInit();
#endif

#if USE_ETH && (model_TgtBd != 1)
	EthInit();
	MdioInit(MDIO_SPEED);
#endif

	extern void ISPtoCPUcallback(void *ctx);
	BtoAIrqCallback(ISPtoCPUcallback, NULL);
	BtoASetIrqEn(ENX_ON);
}

#if EN675_SINGLE
char* enx_ip_status(UINT ip_enable)
{
	if (ip_enable) {
		return TTY_COLOR_GREEN"  +  "TTY_COLOR_RESET;
	} else {
		return TTY_COLOR_BLACK"  -  "TTY_COLOR_RESET;
	}
}
#endif

void enx_sys_init(void)
{
	UINT arrMark[5];
	arrMark[0] = SYS_MARK0;
	arrMark[1] = SYS_MARK1;
	arrMark[2] = SYS_MARK2;
	arrMark[3] = SYS_MARK3;
	arrMark[4] = SYS_MARK4;
	char *strName = (char *)arrMark;
#if EN675_SINGLE
	printf("================================================================================\n");
	printf("%s ", TTY_COLOR_CYAN);
	for (int i = 0; i < sizeof(arrMark); i++) {
		printf("%c", strName[i]);
	}
	printf("                                      %04u-%02u-%02u %02u:%02u:%02u%s |\n", SYS_RTL_YEAR + 2000, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MIN, SYS_RTL_SEC, TTY_COLOR_RESET);
	printf("-------------------------------------------------------------------------------|\n");
	printf(" %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s |\n", "Board", " Core", "Codec", " NPU ", " BDMA", " CDMA", " SDIO", " ETH ", " I2S ", "RESV0");
	printf(" %5s |   %c   | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s |\n", SYS_IP_0 ? "  KU " : SYS_IP_1 ? "  VU " : "?", (SYS_IP_5 && SYS_IP_6) ? '4' : SYS_IP_5 ? '1' : SYS_IP_6 ? '2' : 'E',
			enx_ip_status(SYS_IP_3), enx_ip_status(SYS_IP_4), enx_ip_status(SYS_IP_7), enx_ip_status(SYS_IP_8), enx_ip_status(SYS_IP_9), enx_ip_status(SYS_IP_10), enx_ip_status(SYS_IP_11), enx_ip_status(SYS_IP_24));
	printf("-------------------------------------------------------------------------------|\n");
	printf(" %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s |\n", " AES ", " SHA ", "CKSUM", " MAPC", " ADC ", "  IR ", " USB ", " OIC ", " OMC ", "RESV1");
	printf(" %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s |\n",
			enx_ip_status(SYS_IP_13), enx_ip_status(SYS_IP_14), enx_ip_status(SYS_IP_15), enx_ip_status(SYS_IP_16), enx_ip_status(SYS_IP_17), enx_ip_status(SYS_IP_18), enx_ip_status(SYS_IP_12), enx_ip_status(SYS_IP_20), enx_ip_status(SYS_IP_21), enx_ip_status(SYS_IP_23));
	printf("-------------------------------------------------------------------------------|\n");
	printf(" %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s |\n", " ISP ", "IGMER", "LCDIS", " 148M", " 74M ", " ENC ", "JPDEC", "JPENC", " AUR ", "RESV2");
	printf(" %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s | %5s |\n",
			enx_ip_status(SYS_IP_2), enx_ip_status(SYS_IP_2), enx_ip_status(SYS_IP_26), enx_ip_status(SYS_IP_27), enx_ip_status(SYS_IP_28), enx_ip_status(SYS_IP_29), enx_ip_status(SYS_IP_30), enx_ip_status(SYS_IP_31), enx_ip_status(SYS_IP_19), enx_ip_status(SYS_IP_22));
	printf("================================================================================\n");
#else
	printf("%s", TTY_COLOR_CYAN);
	for (int i = 0; i < sizeof(arrMark); i++) {
		printf("%c", strName[i]);
	}
	printf("%04X-%02X-%02X %02X:%02X:%02X%s\n", SYS_RTL_YEAR, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MINUTE, SYS_RTL_SECOND, TTY_COLOR_RESET);
#endif

#ifdef __ECM_STRING__
	UartRstQue();
	UartRxIrqCallback(DEBUG_UART_NUM, UartDebugRxIrq, NULL);
	UartRxSetIrqEn(DEBUG_UART_NUM,1);
	UartTxIrqCallback(DEBUG_UART_NUM, UartDebugTxIrq, NULL);
#endif
}

void enx_device_init(void)
{
#ifdef __USE_IRIS_PWM__
	TimerSetFreq(TIMER_PWM_IRIS, 1, 0x400, 0);
	TimerStart(TIMER_PWM_IRIS);
	TimerSetPWMEn(TIMER_PWM_IRIS, ENX_ON);
#endif

#ifdef __USE_IR_LED_LPWM__

#endif

#ifdef __USE_IR_LED_GPIO__
	GpioSetDir(GPIO_IR_LED, GPIO_DIR_OUT);
#endif

#ifdef __USE_LED0__
	GpioSetDir(GPIO_LED0, GPIO_DIR_OUT);
#endif

#ifdef __USE_LED1__
	GpioSetDir(GPIO_LED1, GPIO_DIR_OUT);
#endif

#ifdef __USE_LED2__
	GpioSetDir(GPIO_LED2, GPIO_DIR_OUT);
#endif

#ifdef HDMI_I2C_CH
  #ifdef __HDMI_SILICON__
	InitHdmi();
  #endif

  #ifdef __HDMI_LONTIUM__
	#if   model_8M && model_15fps
	LT8618SX_Init(14);
	#elif model_4M && model_15fps
	LT8618SX_Init(16);
	#elif model_4M && model_30fps
	LT8618SX_Init(12);
	#elif model_2M && model_60fps
	LT8618SX_Init(10);
	#elif (model_2M||model_2M30p) && model_30fps
	LT8618SX_Init(6);
	#else
	#error "HDMI" is not supported.
	#endif
  #endif

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
	set_devicetime(TimeZone_GMT, 2020, 5, 19, 12, 0, 0);

#ifdef __AUDIO__
	GpioSetDir(AUDIO_GPIO_RST, GPIO_DIR_OUT);
	GpioSetDir(AUDIO_GPIO_IRQ, GPIO_DIR_IN);
	audio_init();
#endif

#if defined(__USE_SDIOCD__) && (model_TgtBd != 1)
#if 0
	GpioSetDir(SD_GPIO_RST, GPIO_DIR_OUT);	// New Peri B/d
#else
	GpioSetDir(SD_GPIO_RST, GPIO_DIR_IN);	// Old Peri B/d
#endif
	GpioSetDir(SD_GPIO_IRQ, GPIO_DIR_IN);
	SdioCdInit(SD_SDIO_CH);
#endif

#ifdef __WIFI__
	GpioSetDir(WF_GPIO_RST, GPIO_DIR_OUT);
	SdioWfInit(WF_SDIO_CH);
#endif

#ifdef __EEPROM__
//	I2cInit(EEPROM_I2C_CH, EEPROM_I2C_SPEED);	// FPGA 보드에서 사용하는 24AA64 는 1.8V로 동작하며 100KHz로 동작함
	EepromInit();
#endif
}

void enx_default_userinfo(void)
{
	gptMsgShare.VIDEO_FPS = 30;

	gvsVideo[e_vsVSource1].bSwitch = ENX_ON;
	strcpy(gvsVideo[e_vsVSource1].strName, "Unknown Sensor");
	gvsVideo[e_vsVSource1].eResolution = e_res1920x1080;
	gvsVideo[e_vsVSource1].nFps = 30;

	gvsVideo[e_vsVSource2].bSwitch = ENX_OFF;
	strcpy(gvsVideo[e_vsVSource2].strName, "Not connected");
	gvsVideo[e_vsVSource2].eResolution = e_resEndorUnknown;
	gvsVideo[e_vsVSource2].nFps = -1;

	gvsVideo[e_vsVSource3].bSwitch = ENX_OFF;
	strcpy(gvsVideo[e_vsVSource3].strName, "Not connected");
	gvsVideo[e_vsVSource3].eResolution = e_resEndorUnknown;
	gvsVideo[e_vsVSource3].nFps = -1;

	gvsVideo[e_vsVSource4].bSwitch = ENX_OFF;
	strcpy(gvsVideo[e_vsVSource4].strName, "Not connected");
	gvsVideo[e_vsVSource4].eResolution = e_resEndorUnknown;
	gvsVideo[e_vsVSource4].nFps = -1;

#if defined(__ETHERNET__)
	gtSystem.arr8MacAddress[0] = 0x00;
	gtSystem.arr8MacAddress[1] = 0x01;
	gtSystem.arr8MacAddress[2] = 0x52;
	gtSystem.arr8MacAddress[3] = 0x34;
	gtSystem.arr8MacAddress[4] = 0x35;
	gtSystem.arr8MacAddress[5] = 0x37;
	EthMacAddrCheck((BYTE *)gtSystem.arr8MacAddress);

#if 1
	gtNetwork.u3EthAutoNegotiation = ETHPHY_AUTONEG;
#else
	gtNetwork.u3EthAutoNegotiation = ENIF_MAN_100M_FULL;
#endif

	gtNetwork.naEthernet.u1UseDhcp = NET_DHCP;
	if (gtNetwork.naEthernet.u1UseDhcp == ENX_ON) {
		gtNetwork.naEthernet.u32IpAddr = 0;
		gtNetwork.naEthernet.u32NetMask = 0;
		gtNetwork.naEthernet.u32Gateway = 0;
		gtNetwork.naEthernet.u32DnsSvr0 = 0;
		gtNetwork.naEthernet.u32DnsSvr1 = 0;
	} else {
extern UINT ipaddr_addr(const char *cp);
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

#if defined(__WIFI__)
	WifiCFG_Default_UAP((tWifiUAPcfg *)&gtNetwork.UAPcfg);
	WifiCFG_Default_STA((tWifiSTAcfg *)&gtNetwork.STAcfg);
#endif

	gtNetwork.portnumRTSP = RTSP_portnum;
#endif

	strcpy((char *)gtUser.strDeviceId, DEVICE_ID);
	strcpy((char *)gtUser.strDeviceName, DEVICE_NAME);

	gtUser.vcVideo[e_vcVEncoder1].nVSourceIdx = e_vsVSource1;
#if defined(__NETWORK__)
	snprintf((char *)gtUser.vcVideo[e_vcVEncoder1].strStmUrl, STREAM_URL_LENGTH, "%s%u", RTSP_STREAMURL, e_vcVEncoder1 + 1);
#endif
	gtUser.vcVideo[e_vcVEncoder1].eCodec = e_vcodecH265;
	gtUser.vcVideo[e_vcVEncoder1].eResolution = e_res1920x1080;
	gtUser.vcVideo[e_vcVEncoder1].eBRMode = e_brmCVBR;
	gtUser.vcVideo[e_vcVEncoder1].eBitRate = e_br10m;
	gtUser.vcVideo[e_vcVEncoder1].eProfileMode = -1;
	gtUser.vcVideo[e_vcVEncoder1].nIDRFrame = 30;
	gtUser.vcVideo[e_vcVEncoder1].nFps = gvsVideo[gtUser.vcVideo[e_vcVEncoder1].nVSourceIdx].nFps;
	gtUser.vcVideo[e_vcVEncoder1].nQuality = 30;

	gtUser.vcVideo[e_vcVEncoder2].nVSourceIdx = e_vsVSource1;
#if defined(__NETWORK__)
	snprintf((char *)gtUser.vcVideo[e_vcVEncoder2].strStmUrl, STREAM_URL_LENGTH, "%s%u", RTSP_STREAMURL, e_vcVEncoder2 + 1);
#endif
	gtUser.vcVideo[e_vcVEncoder2].eCodec = e_vcodecH264;
	gtUser.vcVideo[e_vcVEncoder2].eResolution = e_res640x360;
	gtUser.vcVideo[e_vcVEncoder2].eBRMode = e_brmCBR;
	gtUser.vcVideo[e_vcVEncoder2].eBitRate = e_br6m;
	gtUser.vcVideo[e_vcVEncoder2].eProfileMode = e_pmMainCB;
	gtUser.vcVideo[e_vcVEncoder2].nIDRFrame = 30;
	gtUser.vcVideo[e_vcVEncoder2].nFps = gvsVideo[gtUser.vcVideo[e_vcVEncoder2].nVSourceIdx].nFps;
	gtUser.vcVideo[e_vcVEncoder2].nQuality = 30;

	gtUser.vcVideo[e_vcVEncoder3].nVSourceIdx = e_vsVSource1;
#if defined(__NETWORK__)
	snprintf((char *)gtUser.vcVideo[e_vcVEncoder3].strStmUrl, STREAM_URL_LENGTH, "%s%u", RTSP_STREAMURL, e_vcVEncoder3 + 1);
#endif
	gtUser.vcVideo[e_vcVEncoder3].eCodec = e_vcodecJPEG;
	gtUser.vcVideo[e_vcVEncoder3].eResolution = e_res1920x1080;
	gtUser.vcVideo[e_vcVEncoder3].eBRMode = e_brmCBR;
	gtUser.vcVideo[e_vcVEncoder3].eBitRate = e_br20m;
	gtUser.vcVideo[e_vcVEncoder3].eProfileMode = -1;
	gtUser.vcVideo[e_vcVEncoder3].nIDRFrame = -1;
	gtUser.vcVideo[e_vcVEncoder3].nFps = gvsVideo[gtUser.vcVideo[e_vcVEncoder3].nVSourceIdx].nFps;
	gtUser.vcVideo[e_vcVEncoder3].nQuality = 50;
}

#ifdef __AUDIO__
#if 0
void audtx11_irq(void *ctx)
{
	UINT pos = I2sTxPos();
	printf("AUDTX: 0x%08X\n", pos);
}

void audrx11_irq(void *ctx)
{
	UINT pos = I2sRxPos();
	printf("AUDRX: 0x%08X\n", pos);
}

void audio_test(void)
{
//	enx_externalirq_init();
	//tx_mode : 0: L, 1: R, 2: L+R/2, 3: Stereo -> 데이터를 전송할 방향, 2는 한 word의 데이터를 읽은 후 2로 divide, 양방향으로 전송.
	//tx_cd : 0 or 1: PCM, 2: G711-a, 3: G711-u)
	//tx_dw : 0->8 , 1->16, 2->24, 3->32 : Tx의 데이터 width
	//rd_byte : 0: 128B, 1: 256B, 2: 512B, 3: 1KB -> 한번 request에서 읽는 데이터 량
	//rd_dw : 0->8 , 1->16, 2->24, 3->32 : Rx의 데이터 width
	//rd_len : 0: 128KB, 1: 256KB, 2: 512KB, 3: 1MB -> Loop를 도는 최대 데이터 량
	//tx_lr : 0 : Mute(0), 1: Left, 2: Right, 3: Both -> TX할 때 mute 또는 unmute 선택

	//I2sTxCfg(3, 0, 1, 2, 0, 0, 3);	// Mono 8KHz, G.711-u, 16bit, Rd:512B Buf:128KB
	//I2sRxCfg(3, 0, 1, 2, 0, 0);		// Mono 8KHz, G.711-u, 16bit, Wr:512B Buf:128KB

	I2sTxCfg(3, 0, 1, 1, 1, 3, 3);	// Mono 8KHz, G.711-u, 16bit, Rd:512B Buf:128KB
	I2sRxCfg(3, 0, 1, 1, 1, 3);		// Mono 8KHz, G.711-u, 16bit, Wr:512B Buf:128KB

	I2S_ADRW = 0x80800000;
	I2S_ADRR = 0x80800000;

	I2sTxIrqCallback(audtx11_irq, NULL);
	I2sSetTxIrqEn(ENX_ON);

	I2sRxIrqCallback(audrx11_irq, NULL);
	I2sSetRxIrqEn(ENX_ON);

//	I2sSetRxEn(ENX_ON);
//	I2sSetTxEn(ENX_ON);

	//hwflush_dcache_range_all();
	//hwflush_dcache_range(0x80800000, 256);
	//hexDump("aa", 0x80800000, 256);
	I2S_RXEN = 1;
	WaitXms(100);
	//hwflush_dcache_range_all();
	//hwflush_dcache_range(0x80800000, 256);
	//hexDump("aa", 0x80800000, 256);
	I2S_TXEN = 1;

	while(1);
}
#endif
#endif

void enx_pmp_init(void)
{
// PMP test
#if 1
	setup_pmp();
#if 1
	pmp_entry_set(0, PMP_R|PMP_W|PMP_X|PMP_L, 0xa0000000ul, SRAM_SIZE); // SRAM enabled area
	pmp_entry_set(1, PMP_L, 0xa0000000ul, 0x20000000ul);				// SRAM disabled area
	pmp_entry_set(2, PMP_R|PMP_W|PMP_X|PMP_L, 0xc0000000ul, SFLS_SIZE); // FLASH enabled area
	pmp_entry_set(3, PMP_L, 0xc0000000ul, 0x20000000ul);				// FLASH disabled area
#endif
	printf("PMP setting\n");

#if 0
	volatile UINT *ptest;
//	ptest = (UINT *)0x70000000;
	ptest = (UINT *)0xb0200000;
	*ptest = 0xdeadc0de;
#endif
#endif
}

void main_0(int cpu_id)
{
	*mtime = 0; // timer init

	//main_ddr_init();

	_init_text_section();
	_init_data_section();

	enx_peri_init();
	enx_sys_init();

	enx_device_init();
	enx_default_userinfo();

	enx_msgshell_init(&gptMsgShell);

	printf("Init Device - RTL-200108-1309\n");

//	FORCE_ABT_SOFFw(1); // I2S-BCK pin muxer issus

	enx_pmp_init();

#if 0//model_TgtBd == 2	// CPU 1core
	if(SYS_REG0 == 0xff22) main_os();
	enx_externalirq_init_cpu0();

	enx_externalirq_perl(eigiISP, ENX_ON, 0);							// Enable ISP Interrupts
	enx_externalirq_perl(eigiVCODEC, ENX_ON, 0);						// Enable Codec Interrupts

	Init_Visp(); INIT_STR("Init_Visp...");	// ISP initial
	Init_Vcap(); INIT_STR("Init_Vcap...");	// Video path set
	Init_Vout(); INIT_STR("Init_Vout...");	// Digital/Analog Output set
	Init_Virq(); INIT_STR("Init_Virq...");	// Video interrupt enable

	INIT_STR("--------- Main Loop Start ---------");

	while (1)
	{
		Visp();
		Vcap();
		//Venc();
		//Vdec();
	}
#endif

	SYS_REG0 = 0x0; // CPU0 Ready!

#if defined(__FREERTOS__)
	main_os();
	while(1);
#else
	enx_externalirq_init_cpu0();

	int tick = 0;
	while (1) {
#ifdef __USE_LED0__
		GpioSetOut(GPIO_LED0, GPIO_OUT_HI);
		WaitXms(100);
		GpioSetOut(GPIO_LED1, GPIO_OUT_LOW);
		WaitXms(100);
#endif
	}
#endif
}
