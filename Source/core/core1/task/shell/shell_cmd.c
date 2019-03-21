//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include <stdlib.h>		// for atoi
#include <time.h>

#include "enx_freertos.h"

#include "shell.h"
#include "shell_cmd.h"
#include "shell_cmd_common.h"
#include "shell_cmd_fs.h"
#include "shell_cmd_sdio.h"
#include "shell_cmd_eth.h"
#include "shell_cmd_mem.h"
#include "string.h"

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
extern int HelpDisp(int argc, char *argv[]);
extern int UsrCmd_a(int argc, char *argv[]);
extern int UsrCmd_b(int argc, char *argv[]);
extern int UsrCmd_c(int argc, char *argv[]);
extern int UsrCmd_d(int argc, char *argv[]);
extern int UsrCmd_e(int argc, char *argv[]);
extern int UsrCmd_f(int argc, char *argv[]);
extern int UsrCmd_g(int argc, char *argv[]);
extern int UsrCmd_h(int argc, char *argv[]);
extern int UsrCmd_i(int argc, char *argv[]);
extern int UsrCmd_j(int argc, char *argv[]);
extern int cmd_info(int argc, char *argv[]);
extern int cmd_perl_check(int argc, char *argv[]);
extern int cmd_perl_gpio(int argc, char *argv[]);
extern int cmd_reboot(int argc, char *argv[]);
extern int cmd_task_status(int argc, char *argv[]);
extern int cmd_mem_ststus(int argc, char *argv[]);

// Test
extern int cmd_test_sysreg(int argc, char *argv[]);
extern int cmd_test_dump(int argc, char *argv[]);

const char *sHelpDisp[]	    = {"Shell command list (! : Repeat command)",        (char*)0};
const char *sUsrCmd_a[]     = {":",                                              (char*)0};
const char *sUsrCmd_b[]     = {":",                                              (char*)0};
const char *sUsrCmd_c[]     = {":",                                              (char*)0};
const char *sUsrCmd_d[]     = {":",                                              (char*)0};
const char *sUsrCmd_e[]     = {":",                                              (char*)0};
const char *sUsrCmd_f[]     = {":",                                              (char*)0};
const char *sUsrCmd_g[]     = {":",                                              (char*)0};
const char *sUsrCmd_h[]     = {":",                                              (char*)0};
const char *sUsrCmd_i[]     = {":",                                              (char*)0};
const char *sUsrCmd_j[]     = {":",                                              (char*)0};
const char *sInfo[]         = {"System Info",                                    (char*)0};
const char *sPerlCheck[]    = {"Check Interface",                                (char*)0};
const char *sPerlGPIO[]     = {"Check GPIO state",                               (char*)0};
const char *sReboot[]       = {"system reboot",                                  (char*)0};
const char *sTaskStatus[]   = {"show freeRTOS task status",                      (char*)0};
const char *sMemStatus[]    = {"show memory status",                             (char*)0};
const char *sSysreg[]       = {"Test(System Reg)",                               (char*)0};
const char *sDump[]         = {"Test(Dump)",                                     (char*)0};
const char *sCDump[]        = {"Test(hwflush_dcache_range & Dump)",              (char*)0};

tMonCmd gCmdList[] =
{
	{"?",			HelpDisp,			sHelpDisp		},

//COMMON
	{"a",			UsrCmd_a,			sUsrCmd_a		},
	{"b",			UsrCmd_b,			sUsrCmd_b		},
	{"c",			UsrCmd_c,			sUsrCmd_c		},
	{"d",			UsrCmd_d,			sUsrCmd_d		},
	{"e",			UsrCmd_e,			sUsrCmd_e		},
	{"f",			UsrCmd_f,			sUsrCmd_f		},

	{"g",			UsrCmd_g,			sUsrCmd_g		},
	{"h",			UsrCmd_h,			sUsrCmd_h		},
	{"i",			UsrCmd_i,			sUsrCmd_i		},
	{"j",			UsrCmd_j,			sUsrCmd_j		},

	{"info",		cmd_info,			sInfo			},
	{"perl",		cmd_perl_check,		sPerlCheck		},
	{"gpio",		cmd_perl_gpio,		sPerlGPIO		},
	{"reboot",      cmd_reboot,         sReboot         },
	{"task",		cmd_task_status,	sTaskStatus		},
	{"memory",		cmd_mem_ststus,		sMemStatus		},
	{"time",		cmd_time,			sTimeCmd		},

//FileSystem
#if defined(__FILESYSTEM__)
	{"ls",			UsrLSCmd,			sUsrLSCmd		},
//	{"diskinit",	UsrDiskInitCmd,		sDiskInitCmd	},
//	{"format",		UsrFormatCmd,		sFormatCmd		},
//	{"mkdir",		UsrMKRMDIRCmd,		sMKDIRCmd		},
//	{"rmdir",		UsrMKRMDIRCmd,		sRMDIRCmd		},
//	{"rm",			UsrRMCmd,			sRMCmd			},
	{"pwd",			UsrPWDCmd,			sPWDCmd			},
	{"cd",			UsrCDCmd,			sCDCmd			},
//	{"fcat",		UsrFCatCmd,			sFCatCmd		},
//	{"fcreate",		UsrFCreateCmd,		sFCreateCmd		},
//	{"fcopy",       UsrFCopyCmd,        sFCopyCmd		},
#if defined(__ETH__) || defined(__WIFI__)
#if (ENX_FTPD_use==1)
	{"ftpd",		UsrFtpdCmd,			sFtpdCmd		},
#endif
#endif
//	{"fstat",		UsrFstatCmd,		sFstatCmd		},
//	{"fhash",       UsrFileHash,        sFileHash       },
#if (LOAD_FS_SDCARD==1)
//	{"sdtest",      UsrSDCardSpeedTestCmd, sSDCardSpeedTestCmd},
//	{"sdsave",		UsrMakevidCmd,		sMakevidCmd		},
#endif
//	{"ftest",		UseFatTest,			sFatTestCmd		},
#endif

//TEST
	{"dump",		cmd_test_dump,		sDump			},
	{"cdump",		cmd_test_dump,		sCDump			},
	{"sysreg",		cmd_test_sysreg,	sSysreg			},
#if defined(__USE_SDIOCD__)
	{"tsd",			cmd_test_sdio,		sSdioTest		},
#endif
	{"tmem",		cmd_test_memory,	sMemoryTest		},
	{"tdma",		cmd_test_dma,		sDmaTest		},
	{"teth",		cmd_test_eth,		sEthTest		},
	{"tmdio",		cmd_test_ethphyreg, sEthphyRegTest	},

	{0,				0,					0				}
};


//*************************************************************************************************
// Display help
//-------------------------------------------------------------------------------------------------
//
int HelpDisp(int argc, char *argv[])
{
	for (int i = 0; gCmdList[i].name != 0; i++) {
		_printf("[%02d]:[%10s] - [%-57s]\n", i, gCmdList[i].name, *(gCmdList[i].helphelp));
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

void timer4Task(void* pvParameters)
{
	vTaskDelay(100);
	while(1) {
		uint64_t t1 = *mtime;
		_printf("%s:%d: %lu - %lu\n", __func__, __LINE__, t1);
		vTaskDelay(1000);
	}
	vTaskDelete(NULL);
}

TaskHandle_t thTimer4id = NULL;

int UsrCmd_a(int argc, char *argv[])
{
	if (thTimer4id == NULL) {
		_printf("Create timer4Task\r\n");
		thTimer4id = vTaskCreate("timer4", timer4Task, NULL, 1024, 3);
		_printf("Create OK\r\n");
	} else {
		_printf("Already Created.\n");
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrCmd_b(int argc, char *argv[])
{
#if (configUSE_TIMERS==1)
	if (thTimer4id) {
		_printf("Delete timer4Task\n");
		vTaskDelete(thTimer4id);
		thTimer4id = NULL;
		_printf("Delete OK\n");
	} else {
		_printf("Already removed.\n");
	}
#endif
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#if (configUSE_TIMERS==1)
TimerHandle_t xTimersTest;

void vTimerCallback(TimerHandle_t xTimer)
{
	const uint32_t ulMaxExpiryCountBeforeStopping = 100;
	uint64_t ulCount;

	ulCount = ( uint64_t ) pvTimerGetTimerID( xTimer );

	ulCount++;
	_printf("Count: %ld\n", ulCount);

	if( ulCount >= ulMaxExpiryCountBeforeStopping )
	{
		/* Do not use a block time if calling a timer API function
		from a timer callback function, as doing so could cause a
		deadlock! */
		xTimerStop( xTimer, 0 );
	}
	else
	{
	   /* Store the incremented count back into the timer's ID field
	   so it can be read back again the next time this software timer
	   expires. */
	   vTimerSetTimerID( xTimer, ( void * ) ulCount );
	}
}
#endif

int UsrCmd_c(int argc, char *argv[])
{
	_printf("Shell Cmd %s\n", __func__);
#if (configUSE_TIMERS==1)
	if (xTimersTest == NULL) {
		xTimersTest = xTimerCreate("MyTimer", 500, pdTRUE, ( void * ) 0, vTimerCallback);
		if (xTimersTest != NULL) {
			_printf("Timer Create OK!\n");
		} else {
			_printf("Timer Create Fail!\n");
		}
	} else {
		_printf("Timer Already!\n");
	}
#endif
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrCmd_d(int argc, char *argv[])
{
	_printf("Shell Cmd %s\n", __func__);
#if (configUSE_TIMERS==1)
	if (xTimersTest != NULL) {
		int timewait = 0;
		if (argc == 2) {
			timewait = atoi(argv[1]);
		}
		BaseType_t xReturn = xTimerStart(xTimersTest, timewait);
		if (xReturn == pdPASS) {
			_printf("Timer Start OK!(Wait:%d)\n", timewait);
		} else {
			_printf("Timer Start Fail!\n");
		}
	} else {
		_printf("Timer Not Ready!\n");
	}
#endif
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrCmd_e(int argc, char *argv[])
{
	_printf("Shell Cmd %s\n", __func__);
#if (configUSE_TIMERS==1)
	if (xTimersTest != NULL) {
		xTimerDelete( xTimersTest, 0 );
		xTimersTest = NULL;
	} else {

	}
#endif
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#define regOut(reg) { uint64_t RESreg = read_csr(reg); _printf(#reg ": %X\n", RESreg); }

int UsrCmd_f(int argc, char *argv[])
{
	_printf("Shell Cmd %s\n", __func__);
	_printf("My name [%s]\n", pcTaskGetName(NULL));

	uint64_t RESmisa = read_csr(misa);
	_printf("misa: %X\n", RESmisa);

	uint64_t RESmvendorid = read_csr(mvendorid);
	_printf("mvendorid: %X\n", RESmvendorid);

	uint64_t RESmarchid = read_csr(marchid);
	_printf("marchid: %X\n", RESmarchid);

	uint64_t RESmimpid = read_csr(mimpid);
	_printf("mimpid: %X\n", RESmimpid);

	uint64_t RESmhartid = read_csr(mhartid);
	_printf("mhartid: %X\n", RESmhartid);

	uint64_t RESmstatus = read_csr(mstatus);
	_printf("mstatus: %X\n", RESmstatus);

	regOut(mtvec);
	regOut(medeleg);
	regOut(mideleg);
	regOut(mip);
	regOut(mie);
	//regOut(mtime);
	//regOut(mtimecmp);
	regOut(mcounteren);
	regOut(mscratch);
	regOut(mepc);
	regOut(mcause);
	regOut(mtval);

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrCmd_g(int argc, char *argv[])
{
	float t = 3.0f;
	float q = 0.0f;
	_printf("t/q = %f\n", t / q);
	_printf("q/t = %f\n", q / t);
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrCmd_h(int argc, char *argv[])
{
	int *a = malloc(64);
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrCmd_i(int argc, char *argv[])
{
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrCmd_j(int argc, char *argv[])
{
extern void DdrTestOff(void);
//extern void DdrTestOn(int var);
extern void DdrTestOn(int var, int RD_mask, int RD_edge, int RD_ltc, int WR_ltc); // 0x0~0xf, 0x0~0xff, 0~1, 0~15, 0~7
extern void DdrTestProcess(void);

	//for (int j = 0; j < 256; j++) {
	int j = 0x20;
		for (int k = 0; k < 2; k++) {
			for (int l = 0; l < 16; l++) {
				for (int m = 0; m < 8; m++) {
					for (int i = 0; i < 16; i++) {
						DdrTestOn(i, j, k, l, m);
						vTaskDelay(1);
						DdrTestProcess();
						DdrTestOff();
						//vTaskDelay(10);
					}
				}
			}
		}
	//}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int cmd_info(int argc, char *argv[])
{
	_printf("H/W info ===================================\n");
	_printf("CPU Clock             : %ukHz\n", MCK_FREQ / 1000);
	_printf("ISP Clock             : %ukHz\n", 0);
	char strName[20] = {0};
	UINT *p = (UINT *)strName;
	p[0] = SYS_MARK0;
	p[1] = SYS_MARK1;
	p[2] = SYS_MARK2;
	p[3] = SYS_MARK3;
	p[4] = SYS_MARK4;
	_printf("Model name            : [%s]\n", strName);

	_printf("S/W info ===================================\n");
	_printf("Firmware compile date : %s %s\n", __DATE__, __TIME__);
	_printf("EN675 Firmware Ver    : %X.%X.%X\n", (0>>8)&0xf, (0>>4)&0xf, 0&0xf);
	_printf("freeRTOS Ver          : %s\n", FreeRTOSVer);

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int cmd_perl_check(int argc, char *argv[])
{
	_printf("I2C info ===================================\n");
	_printf("SPI info ===================================\n");
	_printf("UART info ==================================\n");
	_printf("I2S info ===================================\n");
	_printf("SDIO info ==================================\n");
	_printf("ETH info ===================================\n");
	_printf("USB info ===================================\n");
	_printf("Timer info =================================\n");
	_printf("ADC info ===================================\n");
	_printf("WDT info ===================================\n");
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}
const char * const gpio_mux_name[][4] = {
	{"GPIO00", "PWM00", "SPI0_CS",   "UART0_RX"},
	{"GPIO01", "PWM01", "SPI0_MISO", "UART0_TX"},
	{"GPIO02", "PWM02", "SPI0_MOSI", "I2C0_SDA"},
	{"GPIO03", "PWM03", "SPI0_SCK",  "I2C0_SCL"},
	{"GPIO04", "PWM04", "UART0_RX",  "SPI0_CS"},
	{"GPIO05", "PWM05", "UART0_TX",  "SPI0_MISO"},
	{"GPIO06", "PWM06", "I2C0_SDA",  "SPI0_MOSI"},
	{"GPIO07", "PWM07", "I2C0_SCL",  "SPI0_SCK"},

	{"GPIO08", "PWM08", "SPI1_CS",   "UART1_RX"},
	{"GPIO09", "PWM09", "SPI1_MISO", "UART1_TX"},
	{"GPIO10", "PWM10", "SPI1_MOSI", "I2C1_SDA"},
	{"GPIO11", "PWM11", "SPI1_SCK",  "I2C1_SCL"},
	{"GPIO12", "PWM12", "UART1_RX",  "SPI1_CS"},
	{"GPIO13", "PWM13", "UART1_TX",  "SPI1_MISO"},
	{"GPIO14", "PWM14", "I2C1_SDA",  "SPI1_MOSI"},
	{"GPIO15", "PWM15", "I2C1_SCL",  "SPI1_SCK"},

	{"GPIO16", "I2S_WCK", "SPI2_CS",   "UART2_RX"},
	{"GPIO17", "I2S_DIN", "SPI2_MISO", "UART2_TX"},
	{"GPIO18", "I2S_DOUT", "SPI2_MOSI", "I2C2_SDA"},
	{"GPIO19", "I2S_BCK", "SPI2_SCK",  "I2C2_SCL"},
	{"GPIO20", "PWM16", "UART2_RX",  "SPI2_CS"},
	{"GPIO21", "PWM17", "UART2_TX",  "SPI2_MISO"},
	{"GPIO22", "PWM18", "I2C2_SDA",  "SPI2_MOSI"},
	{"GPIO23", "PWM19", "I2C2_SCL",  "SPI2_SCK"},

	{"GPIO24", "PWM20", "SPI3_CS",   "UART3_RX"},
	{"GPIO25", "PWM21", "SPI3_MISO", "UART3_TX"},
	{"GPIO26", "PWM22", "SPI3_MOSI", "I2C3_SDA"},
	{"GPIO27", "PWM23", "SPI3_SCK",  "I2C3_SCL"},
	{"GPIO28", "PWM24", "UART3_RX",  "SPI3_CS"},
	{"GPIO29", "PWM25", "UART3_TX",  "SPI3_MISO"},
	{"GPIO30", "PWM26", "I2C3_SDA",  "SPI3_MOSI"},
	{"GPIO31", "PWM27", "I2C3_SCL",  "SPI3_SCK"},

	{"GPIO32", "PWM28", "SPI4_CS",   "UART4_RX"},
	{"GPIO33", "PWM29", "SPI4_MISO", "UART4_TX"},
	{"GPIO34", "PWM30", "SPI4_MOSI", "I2C4_SDA"},
	{"GPIO35", "PWM31", "SPI4_SCK",  "I2C4_SCL"},
	{"GPIO36", "ETH_RXD0", "UART4_RX",  "SPI4_CS"},
	{"GPIO37", "ETH_RXD1", "UART4_TX",  "SPI4_MISO"},
	{"GPIO38", "ETH_RXD2", "I2C4_SDA",  "SPI4_MOSI"},
	{"GPIO39", "ETH_RXD3", "I2C4_SCL",  "SPI4_SCK"},

	{"GPIO40", "ETH_RXDV", "SPI5_CS",   "UART5_RX"},
	{"GPIO41", "ETH_RXER", "SPI5_MISO", "UART5_TX"},
	{"GPIO42", "ETH_COL", "SPI5_MOSI", "I2C5_SDA"},
	{"GPIO43", "ETH_CRS", "SPI5_SCK",  "I2C5_SCL"},
	{"GPIO44", "ETH_RCK", "UART5_RX",  "SPI5_CS"},
	{"GPIO45", "ETH_TCK", "UART5_TX",  "SPI5_MISO"},
	{"GPIO46", "ETH_TXD0", "I2C5_SDA",  "SPI5_MOSI"},
	{"GPIO47", "ETH_TXD1", "I2C5_SCL",  "SPI5_SCK"},

	{"GPIO48", "ETH_TXD2", "SPI6_CS",   "UART6_RX"},
	{"GPIO49", "ETH_TXD3", "SPI6_MISO", "UART6_TX"},
	{"GPIO50", "ETH_TXEN", "SPI6_MOSI", "I2C6_SDA"},
	{"GPIO51", "ETH_MDC", "SPI6_SCK",  "I2C6_SCL"},
	{"GPIO52", "ETH_MDIO", "UART6_RX",  "SPI6_CS"},
	{"GPIO53", "PWM32", "UART6_TX",  "SPI6_MISO"},
	{"GPIO54", "PWM33", "I2C6_SDA",  "SPI6_MOSI"},
	{"GPIO55", "PWM34", "I2C6_SCL",  "SPI6_SCK"},

	{"GPIO56", "PWM35", "SPI7_CS",   "UART7_RX"},
	{"GPIO57", "PWM36", "SPI7_MISO", "UART7_TX"},
	{"GPIO58", "PWM37", "SPI7_MOSI", "I2C7_SDA"},
	{"GPIO59", "PWM38", "SPI7_SCK",  "I2C7_SCL"},
	{"GPIO60", "ULPI_NXT", "UART7_RX",  "SPI7_CS"},
	{"GPIO61", "ULPI_STP", "UART7_TX",  "SPI7_MISO"},
	{"GPIO62", "ULPI_DIR", "I2C7_SDA",  "SPI7_MOSI"},
	{"GPIO63", "ULPI_CLK", "I2C7_SCL",  "SPI7_SCK"},

	{"GPIO64", "ULPI_D0", "SPI8_CS",   "UART8_RX"},
	{"GPIO65", "ULPI_D1", "SPI8_MISO", "UART8_TX"},
	{"GPIO66", "ULPI_D2", "SPI8_MOSI", "I2C8_SDA"},
	{"GPIO67", "ULPI_D3", "SPI8_SCK",  "I2C8_SCL"},
	{"GPIO68", "ULPI_D4", "UART8_RX",  "SPI8_CS"},
	{"GPIO69", "ULPI_D5", "UART8_TX",  "SPI8_MISO"},
	{"GPIO70", "ULPI_D6", "I2C8_SDA",  "SPI8_MOSI"},
	{"GPIO71", "ULPI_D7", "I2C8_SCL",  "SPI8_SCK"}
};

int cmd_perl_gpio(int argc, char *argv[])
{

	if (argc == 1) {
		printf("GPIO Status =============================================\n");
		printf("      | IN  | OUT | OEN | DIR | IRQEN | IRQ |  Function |\n");
		printf("======|=====|=====|=====|=====|=======|=====|===========|\n");
		for (uint64_t i = 0; i < GPIO_CNT; i++) {
			printf("GPIO%02u|  %u  |  %u  |  %u  |  %2u |   %u   |  %u  | %9s |\n", i,
					GpioGetPin(i), GpioGetOut(i), GpioGetEdge(i), GpioGetDir(i), GpioGetIrqEn(i), GpioIsIrq(i), gpio_mux_name[i][GpioGetFuncPin(i)]);
		}
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int cmd_reboot(int argc, char *argv[])
{
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

void test(void)
{
	vTaskInfoPrint();
}

int cmd_task_status(int argc, char *argv[])
{
	vMemoryHeapInfoPrint();
	vTaskInfoPrint();
	//vMemoryHeapInfoPrint();
	test();
	vMemoryHeapInfoPrint();
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int cmd_mem_ststus(int argc, char *argv[])
{
	vMemoryHeapInfoPrint();
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int cmd_test_sysreg(int argc, char *argv[])
{
#if 0 // Test Code
	cmd_test_sysreg(1, NULL);

	char *str0[10] = {"NULL", "0","1111"};
	cmd_test_sysreg(3, str0);
	cmd_test_sysreg(1, NULL);

	char *str1[10] = {"NULL", "1","2222"};
	cmd_test_sysreg(3, str1);
	cmd_test_sysreg(1, NULL);

	char *str2[10] = {"NULL", "2","3333"};
	cmd_test_sysreg(3, str2);
	cmd_test_sysreg(1, NULL);

	char *str3[10] = {"NULL", "3","4444"};
	cmd_test_sysreg(3, str3);
	cmd_test_sysreg(1, NULL);
#endif
	if (argc == 1) {
		char strName[20] = {0};
		UINT *p = (UINT *)strName;
		p[0] = SYS_MARK0;
		p[1] = SYS_MARK1;
		p[2] = SYS_MARK2;
		p[3] = SYS_MARK3;
		p[4] = SYS_MARK4;
		_printf("SYS_MAKR [%s]\n", strName);
		hexDump("SYS_MARK", strName, 20);
		_printf("SYS_REG0 [0x%08X]\n", SYS_REG0);
		_printf("SYS_REG1 [0x%08X]\n", SYS_REG1);
		_printf("SYS_REG2 [0x%08X]\n", SYS_REG2);
		_printf("SYS_REG3 [0x%08X]\n", SYS_REG3);
	} else if (argc == 3) {
		UINT id = atoi(argv[1]);
		UINT var = atoi(argv[2]);
		_printf("Input ID(%d) VAR(0x%08X)\n", id, var);
		switch (id) {
		case 0:
			SYS_REG0 = var;
			break;
		case 1:
			SYS_REG1 = var;
			break;
		case 2:
			SYS_REG2 = var;
			break;
		case 3:
			SYS_REG3 = var;
			break;
		default:
			Shell_Unknown();
			break;
		}
	} else {
		Shell_Unknown();
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int cmd_test_dump(int argc, char *argv[])
{
	if (argc == 2 || argc == 3) {
		UINT getLen = 512;
		if (argc == 3) {
			getLen = atoi(argv[2]);
		}
		printf("      Input: 0x%s\n", argv[1]);
		ULONG getData = strtol(argv[1], NULL, 16);
		printf("Check Input: 0x%08lX\n", getData);
		printf("Check Size : %u\n", getLen);

		if (strcmp(argv[0], "cdump") == 0) {
			printf("call hwflush_dcache_range\n");
			hwflush_dcache_range(getData, getLen);
		}
		hexDump("Memory Dump", (void *)getData, getLen);
	} else {
		Shell_Unknown();
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}
