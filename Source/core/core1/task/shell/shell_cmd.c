//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include <stdio.h>
#include <stdlib.h>		// for atoi
#include <time.h>

#include "enx_freertos.h"

#include "shell.h"
#include "shell_cmd.h"
#include "shell_cmd_common.h"
#include "shell_cmd_fs.h"
#include "shell_cmd_sdio.h"
#include "shell_cmd_sfls.h"
#include "shell_cmd_eth.h"
#include "shell_cmd_net.h"
#include "shell_cmd_media.h"
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
extern int cmd_perl_timer(int argc, char *argv[]);
extern int cmd_reboot(int argc, char *argv[]);
extern int cmd_task_status(int argc, char *argv[]);
extern int cmd_mem_ststus(int argc, char *argv[]);

// Test
extern int cmd_test_bus(int argc, char *argv[]);
extern int cmd_test_sysreg(int argc, char *argv[]);
extern int cmd_test_dump(int argc, char *argv[]);
extern int cmd_test_sfls(int argc, char *argv[]);
extern int cmd_test_i2s(int argc, char *argv[]);
extern int cmd_test_adc(int argc, char *argv[]);

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
const char *sPerlTimer[]    = {"Check Timer state",                              (char*)0};
const char *sReboot[]       = {"system reboot",                                  (char*)0};
const char *sTaskStatus[]   = {"show freeRTOS task status",                      (char*)0};
const char *sMemStatus[]    = {"show memory status",                             (char*)0};
const char *sBusCmd[]       = {"show bus throughput",                            (char*)0};
const char *sSysreg[]       = {"Test(System Reg)",                               (char*)0};
const char *sDump[]         = {"Test(Dump)",                                     (char*)0};
const char *sCDump[]        = {"Test(hwflush_dcache_range & Dump)",              (char*)0};
const char *sI2sTest[]      = {"Test(i2s)",                                      (char*)0};
const char *sAdcTest[]      = {"Test(adc)",                                      (char*)0};

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
	{"timer",		cmd_perl_timer,		sPerlTimer		},
	{"reboot",      cmd_reboot,         sReboot         },
	{"task",		cmd_task_status,	sTaskStatus		},
	{"memory",		cmd_mem_ststus,		sMemStatus		},
	{"time",		cmd_time,			sTimeCmd		},

//FileSystem
#if defined(__FILESYSTEM__)
	{"ls",			UsrLSCmd,			sUsrLSCmd		},
	{"diskinit",	UsrDiskInitCmd,		sDiskInitCmd	},
	{"format",		UsrFormatCmd,		sFormatCmd		},
	{"mkdir",		UsrMKRMDIRCmd,		sMKDIRCmd		},
	{"rmdir",		UsrMKRMDIRCmd,		sRMDIRCmd		},
	{"rm",			UsrRMCmd,			sRMCmd			},
	{"pwd",			UsrPWDCmd,			sPWDCmd			},
	{"cd",			UsrCDCmd,			sCDCmd			},
	{"fcat",		UsrFCatCmd,			sFCatCmd		},
	{"fcreate",		UsrFCreateCmd,		sFCreateCmd		},
	{"fcopy",       UsrFCopyCmd,        sFCopyCmd		},
#if defined(__NETWORK__)
#if (ENX_FTPD_use==1)
	{"ftpd",		UsrFtpdCmd,			sFtpdCmd		},
#endif
#endif
	{"fstat",		UsrFstatCmd,		sFstatCmd		},
	{"fhash",       UsrFileHash,        sFileHash       },
#if (LOAD_FS_SDCARD==1)
	{"sdtest",      UsrSDCardSpeedTestCmd, sSDCardSpeedTestCmd},
//	{"sdsave",		UsrMakevidCmd,		sMakevidCmd		},
#endif
	{"ftest",		UseFatTest,			sFatTestCmd		},
#endif

//Network
#if defined(__NETWORK__)
	{"ifconfig",	cmd_ifconfig,		sIfconfigCmd	},
	{"netstat",		cmd_netstat,		sNetstatCmd		},
#endif

//TEST
	{"dump",		cmd_test_dump,		sDump			},
	{"cdump",		cmd_test_dump,		sCDump			},
	{"sfls",		cmd_test_sfls,		sSflsTest		},
	{"i2s",			cmd_test_i2s,		sI2sTest		},
	{"adc",			cmd_test_adc,		sAdcTest		},
	{"sysreg",		cmd_test_sysreg,	sSysreg			},
	{"bus",			cmd_test_bus,		sBusCmd			},
#if defined(__USE_SDIOCD__)
	{"sd",			cmd_test_sdio,		sSdioTest		},
#endif
	{"tmem",		cmd_test_memory,	sMemoryTest		},
	{"bdma",		cmd_test_dma,		sBDmaTest		},
	{"cdma",		cmd_test_dma,		sCDmaTest		},
	{"memset",		cmd_test_memset,	sMemsetTest		},
	{"chksum",		cmd_test_checksum,	sChecksumTest	},
#if (ETH_SHELL_TEST==1)
	{"eth",			cmd_test_eth,		sEthTest		},
#endif
#if (ETHPHY_SHELL_TEST==1)
	{"mdio",		cmd_test_ethphyreg, sEthphyRegTest	},
#endif
	{"video",		cmd_test_video,		sTestVideoCmd	},
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
		_printf("%s:%u: %lu\n", __func__, __LINE__, t1);
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

#define regOut(reg) { uint64_t RESreg = read_csr(reg); printf(#reg ": %lX\n", RESreg); }

int UsrCmd_f(int argc, char *argv[])
{
	printf("start\n");
	uint64 a = *mtime;
	vTaskDelay(TIME_TICK);
	uint64 b = *mtime;
	printf("end: %lu\n", b-a);

	_printf("Shell Cmd %s\n", __func__);
	_printf("My name [%s]\n", pcTaskGetName(NULL));

	uint64_t RESmisa = read_csr(misa);
	_printf("misa: %lX\n", RESmisa);

	uint64_t RESmvendorid = read_csr(mvendorid);
	_printf("mvendorid: %lX\n", RESmvendorid);

	uint64_t RESmarchid = read_csr(marchid);
	_printf("marchid: %lX\n", RESmarchid);

	uint64_t RESmimpid = read_csr(mimpid);
	_printf("mimpid: %lX\n", RESmimpid);

	uint64_t RESmhartid = read_csr(mhartid);
	_printf("mhartid: %lX\n", RESmhartid);

	uint64_t RESmstatus = read_csr(mstatus);
	_printf("mstatus: %lX\n", RESmstatus);

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
#if 1
	BYTE *abc = pvPortMalloc(1000000);

	ULONG a = BenchTimeStart();
	for (UINT i = 999999; i != 0; i--) {
		abc[i] = i % 2 ? 0xaa : 0x55;
	}
	abc[0] = 0xaa;
	ULONG b = BenchTimeStop(a);
	printf("TestA: %lu\n", b);

	for (UINT i = 0; i < 1000000; i++) {
		abc[i] = i % 2 ? 0xaa : 0x55;
	}
	printf("TestB: %lu\n", b);

	a = BenchTimeStart();
	for (UINT i = 999999; i != 0; i--) {
		abc[i] = i % 2 ? 0xaa : 0x55;
	}
	abc[0] = 0xaa;
	b = BenchTimeStop(a);
	printf("TestA: %lu\n", b);

	for (UINT i = 0; i < 1000000; i++) {
		abc[i] = i % 2 ? 0xaa : 0x55;
	}
	printf("TestB: %lu\n", b);



#endif
#if 0
#undef printf

	char buf[8];
	snprintf(buf, 8, "test");

//	void *ctx = getbuf(stdout);
//	_printf("0x%08X\n", (intptr_t)ctx);

	BYTE *tt = pvPortMalloc(1024);

	_REENT->_stdout->_write;

	printf("0x%08X\n", (intptr_t)_REENT);

	setbuf(stdout, NULL);

	char test[64] = {0};
	for(int i = 0 ; i < 49; i++) {
		test[i] = rand() % 26 + 'A';
	}

	ULONG a, cnt, b;

	cnt = _printf("%s\n", test);
	cnt = printf("%s\n", test);

	a = BenchTimeStart();
	cnt = _printf("%s\n", test);
	b = BenchTimeStop(a);
	printf("output:%uus, cnt(%u)\n", b, cnt);

	a = BenchTimeStart();
	cnt = printf("%s\n", test);
	b = BenchTimeStop(a);
	printf("output:%uus, cnt(%u)\n", b, cnt);
#endif
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrCmd_h(int argc, char *argv[])
{
	write_csr(mstatus, MSTATUS_FS);
#define CEL_TEST_SIZE 100000
	uint64_t a = 0, b = 0;
	srand(*mtime);
	if (strcmp(argv[1], "f") == 0) {
		float *data1 = pvPortMalloc(sizeof(float *) * CEL_TEST_SIZE);
		float *data2 = pvPortMalloc(sizeof(float *) * CEL_TEST_SIZE);
		float *data3 = pvPortMalloc(sizeof(float *) * CEL_TEST_SIZE);
		if (data1 == NULL || data2 == NULL || data3 == NULL) {
			printf("malloc fail(%c,%c,%c)\n", data1 == NULL ? 'X' : 'O', data2 == NULL ? 'X' : 'O', data3 == NULL ? 'X' : 'O');
			return 0;
		}
		for (int mode = 0; mode <= 3; mode++) {
			for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
				data1[i] = rand() + 1;
				data2[i] = rand() + 1;
				data3[i] = 0;
			}
			printf("TestMode: ");
			switch (mode) {
			case 0:
				printf("Add\n");
				break;
			case 1:
				printf("Sub\n");
				break;
			case 2:
				printf("Mul\n");
				break;
			case 3:
				printf("Div\n");
				break;
			}
			vTaskEnterCritical();
			a = *mtime;
			switch (mode) {
			case 0:
				for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
					data3[i] = data1[i] + data2[i];
				}
				break;
			case 1:
				for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
					data3[i] = data1[i] - data2[i];
				}
				break;
			case 2:
				for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
					data3[i] = data1[i] * data2[i];
				}
				break;
			case 3:
				for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
					data3[i] = data1[i] / data2[i];
				}
				break;
			}
			b = *mtime;
			vTaskExitCritical();
			printf("%s : %10lu - %8lums\n", argv[1], b - a, (b - a) / 500);
		}
		vPortFree(data1);
		vPortFree(data2);
		vPortFree(data3);
	} else if (strcmp(argv[1], "d") == 0) {
		int *data1 = pvPortMalloc(sizeof(int *) * CEL_TEST_SIZE);
		int *data2 = pvPortMalloc(sizeof(int *) * CEL_TEST_SIZE);
		int *data3 = pvPortMalloc(sizeof(int *) * CEL_TEST_SIZE);
		if (data1 == NULL || data2 == NULL || data3 == NULL) {
			printf("malloc fail(%c,%c,%c)\n", data1 == NULL ? 'X' : 'O', data2 == NULL ? 'X' : 'O', data3 == NULL ? 'X' : 'O');
			return 0;
		}
		for (int mode = 0; mode <= 4; mode++) {
			for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
				data1[i] = rand() + 1;
				data2[i] = rand() + 1;
				data3[i] = 0;
			}
			printf("TestMode: ");
			switch (mode) {
			case 0:
				printf("Add\n");
				break;
			case 1:
				printf("Sub\n");
				break;
			case 2:
				printf("Mul\n");
				break;
			case 3:
				printf("Div\n");
				break;
			case 4:
				printf("Mod\n");
				break;
			}
			vTaskEnterCritical();
			a = *mtime;
			switch (mode) {
			case 0:
				for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
					data3[i] = data1[i] + data2[i];
				}
				break;
			case 1:
				for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
					data3[i] = data1[i] - data2[i];
				}
				break;
			case 2:
				for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
					data3[i] = data1[i] * data2[i];
				}
				break;
			case 3:
				for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
					data3[i] = data1[i] / data2[i];
				}
				break;
			case 4:
				for (uint64 i = 0; i < CEL_TEST_SIZE; i++) {
					data3[i] = data1[i] % data2[i];
				}
				break;
			}
			b = *mtime;
			vTaskExitCritical();
			printf("%s : %10lu - %8lums\n", argv[1], b - a, (b - a) / 500);
		}
		vPortFree(data1);
		vPortFree(data2);
		vPortFree(data3);
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

//#include "networkif.h"
int UsrCmd_i(int argc, char *argv[])
{
#if 0
	printf("netifapi_netif_set_link_up start\n");
	netifapi_netif_set_link_up(netif_state[0]._netif);
	printf("netifapi_netif_set_link_up end\n");
#endif

	hwflush_dcache_range_all();

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrCmd_j(int argc, char *argv[])
{
	//set_csr(mie, MIP_MTIP|MIP_MEIP);
	//clear_csr(mie, MIP_MTIP|MIP_MEIP);

#if 0
	printf("netifapi_netif_set_link_down start\n");
	netifapi_netif_set_link_down(netif_state[0]._netif);
	printf("netifapi_netif_set_link_down end\n");
#endif
#if 0
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
#endif
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#include "lwip/init.h"
#include "ff.h"

int cmd_info(int argc, char *argv[])
{
	_Gprintf("H/W info ===================================\n");
	printf("MCK Clock             : %ukHz\n", MCK_FREQ / 1000);
	printf("CPU Clock             : %ukHz\n", CPU_FREQ / 1000);
	printf("ISP Clock             : %ukHz\n", 0);

	UINT arrMark[5];
	arrMark[0] = SYS_MARK0;
	arrMark[1] = SYS_MARK1;
	arrMark[2] = SYS_MARK2;
	arrMark[3] = SYS_MARK3;
	arrMark[4] = SYS_MARK4;
	char *strName = (char *)arrMark;
	printf("Model name            : [");
	for (int i = 0; i < sizeof(arrMark); i++) {
		printf("%c", strName[i]);
	}
	printf("]\n");
	printf("Model date            : %04X-%02X-%02X %02X:%02X:%02X\n", SYS_RTL_YEAR, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MINUTE, SYS_RTL_SECOND);

	_Gprintf("S/W info ===================================\n");
	printf("Firmware compile date : %s %s\n", __DATE__, __TIME__);
	printf("EN675 Firmware Ver    : %X.%X.%X\n", EN675_FW_VERSION_MAJOR, EN675_FW_VERSION_MINOR, EN675_FW_VERSION_PATCH);
	printf("freeRTOS Ver          : %s\n", FreeRTOSVer);
#ifdef __NETWORK__
	 printf("lwIP Ver              : %s\n", LWIP_VERSION_STRING);
#endif
#ifdef __FILESYSTEM__
	 printf("FatFS Ver             : %u.%u%c patch%u\n", FATFS_VERSION_MAJOR, FATFS_VERSION_MINOR, FATFS_VERSION_REVISION, FATFS_VERSION_RC);
#endif

	_Gprintf("Status info ================================\n");
	size_t dc = rdcycle();
	size_t di = rdinstret();
	printf("cycles                : %ld\n", dc);
	printf("instructions          : %ld\n", di);
	printf("CPI                   : %lu.%lu%lu\n", dc/di, 10UL*dc/di % 10, (100UL*dc + di/2)/di % 10);

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
		printf("------|-----|-----|-----|-----|-------|-----|-----------|\n");
		for (uint32 i = 0; i < GPIO_CNT; i++) {
			printf("GPIO%02u|  %u  |  %u  |  %u  |  %2u |   %u   |  %u  | %-9s |\n", i,
					GpioGetPin(i), GpioGetOut(i), GpioGetEdge(i), GpioGetDir(i), GpioGetIrqEn(i), GpioIsIrq(i), gpio_mux_name[i][GpioGetFuncPin(i)]);
			if (((i + 1) % 8) == 0) {
				printf("------|-----|-----|-----|-----|-------|-----|-----------|\n");
			}
		}
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#define TEST_TIMER_DIV 250
#define TEST_TIMER_LMT 49999
#define TEST_TIMER_TRIG 25000

typedef struct {
	ENX_YN automode;
	int index;
	int count;

	int reg_div;
	int reg_lmt;
	int reg_trig;

	ULONG stime;
} TimerTestStr;

TimerTestStr ttsTest = {
	.automode = ENX_NO,
	.index = -1,
	.count = 0,

	.reg_div = TEST_TIMER_DIV,
	.reg_lmt = TEST_TIMER_LMT,
	.reg_trig = TEST_TIMER_TRIG,
};

void testTimer_irq(void *ctx)
{
	TimerTestStr *tts = (TimerTestStr *)ctx;
	UINT etime = BenchTimeStop(ttsTest.stime);
	ttsTest.stime = BenchTimeStart();
	printf("IRQ is called.(%u us)\n", etime);
	if (tts->automode == ENX_YES) {
		tts->count++;
		if (tts->count > 10) {
			TimerStop(tts->index);
			tts->index++;
			tts->count = 0;
			if (tts->index > 38) {
				printf("Timer Auto Test End!\n");
			} else {
				TimerSetFreq(tts->index, tts->reg_div, tts->reg_lmt, tts->reg_trig);
				TimerIrqCallback(tts->index, testTimer_irq, tts);
				TimerSetIrqEn(tts->index, ENX_ON);
				TimerStart(tts->index);
			}
		}
	}
}

int cmd_perl_timer(int argc, char *argv[])
{
	const BYTE nGpIdx[TIMER_CNT] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 53, 54, 55, 56, 57, 58, 59};
	if (argc == 1) {
		printf("Timer Status =====================================================================\n");
		printf("               | EN  | CKEN | DIV |  LMT  |  TRIG  | COUNT | IRQEN | IRQ |  Mode |\n");
		printf("---------------|-----|------|-----|-------|--------|-------|-------|-----|-------|\n");
		for (uint32 i = 0; i < TIMER_CNT; i++) {
			printf("Timer%02u(GPIO%02u)|  %u  |  %2u  | %3u | %5u | %5u  | %5u |   %u   |  %u  | %-5s |\n", i, nGpIdx[i], TimerGetEn(i), TimerGetCken(i), TimerGetDiv(i), TimerGetLmt(i), TimerGetTrig(i), TimerGetCount(i), TimerGetIrqEn(i), TimerIsIrq(i), TimerGetPWMEn(i) ? " PWM " : "TIMER");
			if (((i + 1) % 8) == 0) {
				printf("---------------|-----|------|-----|-------|--------|-------|-------|-----|-------|\n");
			}
		}
		printf("==================================================================================\n");
	} else if (strcmp(argv[1], "on") == 0) {
		if (ttsTest.index > 38 && ttsTest.index < 0) {
			printf("Select a channel!\n");
			return 0;
		}
		ttsTest.automode = ENX_NO;
		TimerSetFreq(ttsTest.index, ttsTest.reg_div, ttsTest.reg_lmt, ttsTest.reg_trig);
		TimerIrqCallback(ttsTest.index, testTimer_irq, &ttsTest);
		TimerSetIrqEn(ttsTest.index, ENX_ON);
		ttsTest.stime = BenchTimeStart();
		TimerStart(ttsTest.index);
	} else if (strcmp(argv[1], "off") == 0) {
		if (ttsTest.index > 38 && ttsTest.index < 0) {
			printf("Select a channel!\n");
			return 0;
		}
		TimerStop(ttsTest.index);
	} else if (strcmp(argv[1], "idx") == 0) {
		if (argc == 3) {
			printf("Old select Timer(%d)\n", ttsTest.index);
			ttsTest.index = atoi(argv[2]);
			if (ttsTest.index > 38 && ttsTest.index < 0) {
				printf("Invalid channel selection.(%d)\n", ttsTest.index);
				ttsTest.index = -1;
			} else {
				printf("New select Timer(%d)\n", ttsTest.index);
			}
		} else {
			printf("Select Timer(%d)\n", ttsTest.index);
		}
	} else if (strcmp(argv[1], "div") == 0 && argc == 3) {
		if (ttsTest.index > 38 && ttsTest.index < 0) {
			printf("Select a channel!\n");
			return 0;
		}

		int nDiv = atoi(argv[2]);
		if (nDiv >= 0 && nDiv <= 255) {
			ttsTest.reg_div = nDiv;
			TimerSetDiv(ttsTest.index, ttsTest.reg_div);
		} else {
			printf("Input value(%d) is out of range.(0~255)\n", nDiv);
		}
	} else if (strcmp(argv[1], "lmt") == 0 && argc == 3) {
		if (ttsTest.index > 38 && ttsTest.index < 0) {
			printf("Select a channel!\n");
			return 0;
		}

		int nLmt = atoi(argv[2]);
		if (nLmt >= 0 && nLmt <= 65535) {
			ttsTest.reg_lmt = nLmt;
			TimerSetLmt(ttsTest.index, ttsTest.reg_lmt);
		} else {
			printf("Input value(%d) is out of range.(0~65535)\n", nLmt);
		}
	} else if (strcmp(argv[1], "trig") == 0 && argc == 3) {
		if (ttsTest.index > 38 && ttsTest.index < 0) {
			printf("Select a channel!\n");
			return 0;
		}

		int nTrig = atoi(argv[2]);
		if (nTrig >= 0 && nTrig <= 65535) {
			ttsTest.reg_trig = nTrig;
			TimerSetTrig(ttsTest.index, ttsTest.reg_trig);
		} else {
			printf("Input value(%d) is out of range.(0~65535)\n", nTrig);
		}
	} else if (strcmp(argv[1], "pwm") == 0) {
		if (ttsTest.index > 38 && ttsTest.index < 0) {
			printf("Select a channel!\n");
			return 0;
		}
		if (TimerGetPWMEn(ttsTest.index) == ENX_ON) {
			TimerSetPWMEn(ttsTest.index, ENX_OFF);
		} else {
			TimerSetPWMEn(ttsTest.index, ENX_ON);
		}
	} else if (strcmp(argv[1], "auto") == 0) {
		ttsTest.index = 0;
		ttsTest.automode = ENX_YES;
		TimerSetFreq(ttsTest.index, ttsTest.reg_div, ttsTest.reg_lmt, ttsTest.reg_trig);
		TimerIrqCallback(ttsTest.index, testTimer_irq, &ttsTest);
		TimerSetIrqEn(ttsTest.index, ENX_ON);
		TimerStart(ttsTest.index);
	} else {

	}
	return 0;
}

int cmd_reboot(int argc, char *argv[])
{
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int cmd_task_status(int argc, char *argv[])
{
	vTaskInfoPrint();
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

int cmd_test_bus(int argc, char *argv[])
{
	BusInit();
	return 0;
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

int cmd_test_i2s(int argc, char *argv[])
{
	UINT u32GetData = 0;
	if (argc > 2) {
		u32GetData = atoi(argv[2]);
	}
	if (argc == 2 || argc == 3) {
		if (argc == 3) {
			printf("Set Input: %s, Value: %u\n", argv[1], u32GetData);
		} else {
			printf("Get Input: %s\n", argv[1]);
		}

		if (strcmp("txmode", argv[1]) == 0) {
			if (argc == 3) {
				I2sSetTxMode(u32GetData);
			}
			u32GetData = I2sGetTxMode();
		} else if (strcmp("txcd", argv[1]) == 0) {
			if (argc == 3) {
				I2sSetTxCodec(u32GetData);
			}
			u32GetData = I2sGetTxCodec();
		} else if (strcmp("txdw", argv[1]) == 0) {
			if (argc == 3) {
				I2sSetTxDw(u32GetData);
			}
			u32GetData = I2sGetTxDw();
		} else if (strcmp("rdbyte", argv[1]) == 0) {
			if (argc == 3) {
				I2sSetRdByte(u32GetData);
			}
			u32GetData = I2sGetRdByte();
		} else if (strcmp("rddw", argv[1]) == 0) {
			if (argc == 3) {
				I2sSetRdDw(u32GetData);
			}
			u32GetData = I2sGetRdDw();
		} else if (strcmp("rdlen", argv[1]) == 0) {
			if (argc == 2) {
				I2sSetRdLen(u32GetData);
			}
			u32GetData = I2sGetRdLen();
		} else if (strcmp("txlr", argv[1]) == 0) {
			if (argc == 2) {
				I2sSetTxLr(u32GetData);
			}
			u32GetData = I2sGetTxLr();
		}
		printf("%s : %u\n", argv[1], u32GetData);
	} else if (argc == 8) {
		UINT tx_mode = atoi(argv[1]);
		UINT tx_cd = atoi(argv[2]);
		UINT tx_dw = atoi(argv[3]);
		UINT rd_byte = atoi(argv[4]);
		UINT rd_dw = atoi(argv[5]);
		UINT rd_len = atoi(argv[6]);
		UINT tx_lr = atoi(argv[7]);
		//tx_mode : 0: L, 1: R, 2: L+R/2, 3: Stereo -> 데이터를 전송할 방향, 2는 한 word의 데이터를 읽은 후 2로 divide, 양방향으로 전송.
		//tx_cd : 0 or 1: PCM, 2: G711-a, 3: G711-u)
		//tx_dw : 0->8 , 1->16, 2->24, 3->32 : Tx의 데이터 width
		//rd_byte : 0: 128B, 1: 256B, 2: 512B, 3: 1KB -> 한번 request에서 읽는 데이터 량
		//rd_dw : 0->8 , 1->16, 2->24, 3->32 : Rx의 데이터 width
		//rd_len : 0: 128KB, 1: 256KB, 2: 512KB, 3: 1MB -> Loop를 도는 최대 데이터 량
		//tx_lr : 0 : Mute(0), 1: Left, 2: Right, 3: Both -> TX할 때 mute 또는 unmute 선택
		I2sTxCfg(tx_mode, tx_cd, tx_dw, rd_byte, rd_dw, rd_len, tx_lr);
	} else {
		printf("tx_mode: %u\n", I2S_TXMODE);
		printf("tx_cd  : %u\n", I2S_TXCODEC);
		printf("tx_dw  : %u\n", I2S_TXDW);
		printf("rd_byte: %u\n", I2S_RDBYTE);
		printf("rd_dw  : %u\n", I2S_RDDW);
		printf("rd_len : %u\n", I2S_RDLEN);
		printf("tx_lr  : %u\n", I2S_TXLR);
	}
	return 0;
}

TimerHandle_t xTimersADCTest;
static void test_adc_process(TimerHandle_t xTimer)
{
	for (int i = 0; i < ADC_CNT; i++) {
		printf("CH%u:%3s %4u|", i, AdcIsEnable(i) == 0 ? "OFF" : "ON ", AdcGet(i));
	}
	printf("\n");
}

int cmd_test_adc(int argc, char *argv[])
{
	if (strcmp(argv[1], "start") == 0) {
		if (xTimersADCTest == NULL) {
			xTimersADCTest = xTimerCreate("AdcTest", 50, pdTRUE, ( void * ) 0, test_adc_process);
			if (xTimersADCTest != NULL) {
				printf("ADC Timer create ok!\n");
			} else {
				printf("ADC Timer create fail!\n");
			}
		}

		BaseType_t xReturn = xTimerStart(xTimersADCTest, 0);
		if (xReturn == pdPASS) {
			printf("ADC Timer start ok!\n");
		} else {
			printf("ADC Timer start fail!\n");
		}
	} else if (strcmp(argv[1], "stop") == 0) {
		BaseType_t xReturn = xTimerStop( xTimersADCTest, 0 );
		if (xReturn == pdPASS) {
			printf("ADC Timer stop ok!\n");
		} else {
			printf("ADC Timer stop fail!\n");
		}
	} else if (argc == 3 && strcmp(argv[1], "on") == 0) {
		if (strcmp(argv[2], "all") == 0) {
			for (int i = 0; i < ADC_CNT; i++) {
				AdcOn(i);
			}
			printf("ADC All channels on.\n");
		} else {
			UINT u32Ch = atoi(argv[2]);
			if (u32Ch < ADC_CNT) {
				AdcOn(u32Ch);
				printf("ADC %u channel on.\n", u32Ch);
			} else {
				printf("Invalid %u channel input.\n", u32Ch);
			}
		}
	} else if (argc == 3 && strcmp(argv[1], "off") == 0) {
		if (strcmp(argv[2], "all") == 0) {
			for (int i = 0; i < ADC_CNT; i++) {
				AdcOff(i);
			}
			printf("ADC All channels off.\n");
		} else {
			UINT u32Ch = atoi(argv[2]);
			if (u32Ch < ADC_CNT) {
				AdcOff(u32Ch);
				printf("ADC %u channel off.\n", u32Ch);
			} else {
				printf("Invalid %u channel input.\n", u32Ch);
			}
		}
	} else {
		printf("|");
		for (int i = 0; i < ADC_CNT; i++) {
			printf("CH%u:%3s %4u|", i, AdcIsEnable(i) == 0 ? "OFF" : "ON ", AdcGet(i));
		}
		printf("\n");
	}
	return 0;
}
