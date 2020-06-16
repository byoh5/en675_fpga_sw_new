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
#include "shell_cmd_gpio.h"
#include "shell_cmd_uart.h"
#include "shell_cmd_spi.h"
#include "shell_cmd_i2c.h"
#include "shell_cmd_isp.h"
#include "shell_cmd_util.h"
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
extern int cmd_irq_info(int argc, char *argv[]);
extern int cmd_perl_check(int argc, char *argv[]);
extern int cmd_perl_timer(int argc, char *argv[]);
extern int cmd_reboot(int argc, char *argv[]);
extern int cmd_task_status(int argc, char *argv[]);
extern int cmd_mem_ststus(int argc, char *argv[]);

// Test
extern int cmd_test_oic(int argc, char *argv[]);
extern int cmd_test_sysreg(int argc, char *argv[]);
extern int cmd_test_dump(int argc, char *argv[]);
extern int cmd_test_sfls(int argc, char *argv[]);
extern int cmd_test_i2s(int argc, char *argv[]);
extern int cmd_test_adc(int argc, char *argv[]);
extern int cmd_test_ir(int argc, char *argv[]);

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
const char *sIrqInfo[]      = {"Inturrupt Info",                                 (char*)0};
const char *sPerlCheck[]    = {"Check Interface",                                (char*)0};
const char *sPerlTimer[]    = {"Check Timer state",                              (char*)0};
const char *sReboot[]       = {"system reboot",                                  (char*)0};
const char *sTaskStatus[]   = {"show freeRTOS task status",                      (char*)0};
const char *sMemStatus[]    = {"show memory status",                             (char*)0};
const char *sOicCmd[]       = {"Check OIC",                                      (char*)0};
const char *sSysreg[]       = {"Test(System Reg)",                               (char*)0};
const char *sDump[]         = {"Test(Dump)",                                     (char*)0};
const char *sCDump[]        = {"Test(hwflush_dcache_range & Dump)",              (char*)0};
const char *sIDump[]        = {"Test(hwdiscard_dcache_range & Dump)",            (char*)0};
#if defined(__AUDIO__)
const char *sI2sTest[]      = {"Test(i2s)",                                      (char*)0};
#endif
const char *sAdcTest[]      = {"Test(adc)",                                      (char*)0};
#if USE_IR
const char *sIrTest[]       = {"Test(ir)",                                       (char*)0};
#endif

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
	{"irq",			cmd_irq_info,		sIrqInfo		},
	{"perl",		cmd_perl_check,		sPerlCheck		},
	{"gpio",		cmd_perl_gpio,		sPerlGpio		},
	{"timer",		cmd_perl_timer,		sPerlTimer		},
	{"uart",		cmd_perl_uart,		sPerlUart		},
	{"spi",			cmd_perl_spi,		sPerlSpi		},
	{"i2c",			cmd_perl_i2c,		sPerlI2c		},
	{"reboot",      cmd_reboot,         sReboot         },
	{"task",		cmd_task_status,	sTaskStatus		},
	{"memory",		cmd_mem_ststus,		sMemStatus		},
	{"time",		cmd_time,			sTimeCmd		},
	{"user",		cmd_userarea,		sUserAreaCmd	},

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
	{"sdsave",		UsrMakevidCmd,		sMakevidCmd		},
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
	{"idump",		cmd_test_dump,		sIDump			},
	{"sfls",		cmd_test_sfls,		sSflsTest		},
#if defined(__AUDIO__)
	{"i2s",			cmd_test_i2s,		sI2sTest		},
#endif
	{"adc",			cmd_test_adc,		sAdcTest		},
#if USE_IR
	{"ir",			cmd_test_ir,		sIrTest			},
#endif
	{"sysreg",		cmd_test_sysreg,	sSysreg			},
	{"oic",			cmd_test_oic,		sOicCmd			},
#if defined(__USE_SDIOCD__)
	{"sd",			cmd_test_sdio,		sSdioTest		},
#endif
	{"tmem",		cmd_test_memory,	sMemoryTest		},
	{"bdma",		cmd_test_dma,		sBDmaTest		},
	{"cdma",		cmd_test_dma,		sCDmaTest		},
	{"memset",		cmd_test_memset,	sMemsetTest		},
	{"chksum",		cmd_test_checksum,	sChecksumTest	},
	{"sha224",		cmd_test_sha,		sShaTest		},
	{"sha256",		cmd_test_sha,		sShaTest		},
	{"aes128",		cmd_test_aes,		sAesTest		},
	{"aes256",		cmd_test_aes,		sAesTest		},
#if (ETH_SHELL_TEST==1)
	{"eth",			cmd_test_eth,		sEthTest		},
#endif
#if (ETHPHY_SHELL_TEST==1)
	{"mdio",		cmd_test_ethphyreg, sEthphyRegTest	},
#endif
	{"video",		cmd_test_video,		sTestVideoCmd	},
	{"audio",		cmd_test_audio,		sTestAudioCmd	},
	{"jpeg",		cmd_test_jpeg,		sTestJpegCmd	},
	{"isp",			cmd_isp,			sIspCmd			},
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
		printf("ok1:0x%08X\n", (intptr_t)xTimer);
		xTimerStop( xTimer, 0 );
		printf("ok2:0x%08X\n", (intptr_t)xTimer);
	}
	else
	{
	   /* Store the incremented count back into the timer's ID field
	   so it can be read back again the next time this software timer
	   expires. */
	   vTimerSetTimerID( xTimer, ( void * ) ulCount );
	}
}

void vTimerCallback2(TimerHandle_t xTimer)
{
	static ULONG aaa_time = 0;
	static ULONG bbb_time = 0;
	ULONG a = rdcycle();
	ULONG b = *mtime;
	printf("rdcycle[%15lu], gap[%15lu] - mtime([%15lu], gap[%15lu]\n", a, a - aaa_time, b, b - bbb_time);
	aaa_time = a;
	bbb_time = b;
}
#endif

int UsrCmd_c(int argc, char *argv[])
{
	_printf("Shell Cmd %s\n", __func__);
#if (configUSE_TIMERS==1)
	if (xTimersTest == NULL) {
		xTimersTest = xTimerCreate("MyTimer", 100, pdTRUE, ( void * ) 0, vTimerCallback2);
		// pdFALSE pdTRUE
		if (xTimersTest != NULL) {
			_printf("Timer Create OK!(0x%08X)\n", (intptr_t)xTimersTest);
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
			_printf("Timer Start OK!(0x%08X)(Wait:%d)\n", (intptr_t)xTimersTest, timewait);
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
	printf("Timer Stop!(0x%08X)\n", (intptr_t)xTimersTest);
	if (xTimersTest != NULL) {
		int res = xTimerDelete( xTimersTest, 0 );
		printf("Timer Delete(%u)\n", res);
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
static TimerHandle_t xTimersUptime;

void vTimerUptimeCallback(TimerHandle_t xTimer)
{
	UINT temp, h, m, s;
	temp = gptMsgShare.UPTIME;
	s = temp % 60;
	temp = temp / 60;
	m = temp % 60;
	temp = temp / 60;
	h = temp;
	printf("UPTIME: %uH %uM %uS\n", h, m, s);
}

int UsrCmd_i(int argc, char *argv[])
{
#if 0
	printf("netifapi_netif_set_link_up start\n");
	netifapi_netif_set_link_up(netif_state[0]._netif);
	printf("netifapi_netif_set_link_up end\n");
#endif

	if (argc == 1) {
		hwflush_dcache_all();
	} else {
		xTimersUptime = xTimerCreate("MyTimer", 6000, pdTRUE, ( void * ) 0, vTimerUptimeCallback);
		if (xTimersUptime) {
			BaseType_t xReturn = xTimerStart(xTimersUptime, 0);
			if (xReturn == pdPASS) {
				printf("Timer Start OK!(0x%08X)\n", (intptr_t)xTimersUptime);
			} else {
				printf("Timer Start Fail!\n");
			}
		} else {
			printf("Timer Create Fail!\n");
		}
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#if 0
#define NUM32(ch4) ((((DWORD)(ch4) & 0xFF) << 24) |     \
                  (((DWORD)(ch4) & 0xFF00) << 8) |    \
                  (((DWORD)(ch4) & 0xFF0000) >> 8) |  \
                  (((DWORD)(ch4) & 0xFF000000) >> 24))


void TestUartTask(void *ctx)
{
	GpioFuncPin(8, 3);
	UartInit(1, 2400);
	printf("UART1 Clk: %uHz\n", UartGetClk(1));

	while (1) {
		while (UartRxIsEmpty(1) == 1) {
			vTaskDelay(1);
		}
		char in = UartRxGetByte(1);
		printf("UART1 RX: [\t0x%02X\t]\n", in);
	}

	vTaskDelete(NULL);
}
#endif

void kkkTask(void *ctx)
{
	printf("kkkTask Start\n");
	vTaskDelay(100);
	printf("kkkTask End\n");
	vTaskDelay(10);
}

int UsrCmd_j(int argc, char *argv[])
{
#if 0
	if (argc == 3) {
		int cpuidx = atoi(argv[1]);
		if (strcmp("on", argv[2]) == 0) {
			switch (cpuidx) {
			case 0:
				SYS_CPU0_PD = 1;
				break;
			case 1:
				SYS_CPU1_PD = 1;
				break;
			case 2:
				SYS_CPU2_PD = 1;
				break;
			case 3:
				SYS_CPU3_PD = 1;
				break;
			default:
				printf("Error CPU index(%u)\n", cpuidx);
				break;
			}
		} else if (strcmp("off", argv[2]) == 0) {
			switch (cpuidx) {
			case 0:
				SYS_CPU0_PD = 0;
				break;
			case 1:
				SYS_CPU1_PD = 0;
				break;
			case 2:
				SYS_CPU2_PD = 0;
				break;
			case 3:
				SYS_CPU3_PD = 0;
				break;
			default:
				printf("Error CPU index(%u)\n", cpuidx);
				break;
			}
		} else {
			printf("Error command(%s)\n", argv[2]);
		}
	} else {
		uint64_t a = read_csr(mstatus);
		printf("mstatus: 0x%16X\n", a);
		printf("SYS_CPU0_PD: %u\n", SYS_CPU0_PD);
		printf("SYS_CPU1_PD: %u\n", SYS_CPU1_PD);
		printf("SYS_CPU2_PD: %u\n", SYS_CPU2_PD);
		printf("SYS_CPU3_PD: %u\n", SYS_CPU3_PD);
	}
#endif

#if 0
	AtoBSetIrqEn(ENX_ON);
	BtoASetIrqEn(ENX_ON);

	if (argc == 2) {
		ULONG tstart = 0;
		if (strcmp("0", argv[1]) == 0) {
			AtoBIrqCall();
			tstart = BenchTimeStart();
			while (SYS_ATOB_BUSY);
			UINT tgap = BenchTimeStop(tstart);
			printf("gap: %uus\n", tgap);
		} else if (strcmp("1", argv[1]) == 0) {
			BtoAIrqCall();
			tstart = BenchTimeStart();
			while (SYS_BTOA_BUSY);
			UINT tgap = BenchTimeStop(tstart);
			printf("gap: %uus\n", tgap);
		}
	} else {

	}
#endif

#if 1
	if (argc == 1) {

	} else {
		if (strcmp("wdtapb", argv[1]) == 0) {
			printf("WDT APB start\n");
			WdtInit(10000);
		} else
#if EN675_SINGLE
		if (strcmp("wdtosc", argv[1]) == 0) {
			printf("WDT OSC start\n");
			WdtoscInit(10000);
		} else
#endif
		if (strcmp("dma", argv[1]) == 0) {
			SYS_REG5 = 0x10;
		} else if (strcmp("eth", argv[1]) == 0) {
#if (ETHPHY_LOOPBACK_TEST==1)
			EthSetRxEn(ENX_OFF);
			EthSetRxIrqEn(ENX_OFF);
			enx_externalirq_perl(eigiETH, ENX_OFF, 0);
			vTaskDelay(1);
			EthRxIrqCallback(NULL, NULL);
extern void EthphyLoopbackMode(UINT speed, UINT duplex);
			EthphyLoopbackMode(1000, 2);
			SYS_REG6 = 0x10;
#else
			printf("eth loopback mode disabled!\n");
#endif
		} else if (strcmp("err", argv[1]) == 0) {
			vTaskCreate("kkk", kkkTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
		} else if (strcmp("size", argv[1]) == 0) {
			UINT nHW = atoi(argv[2]);
			UINT nVW = atoi(argv[3]);
			UINT nHTW = 4400 - 2; // nHW + 4400;
			UINT nVTW = 2250 - 1; // nVW + 2250;

			printf("INPUT: HW(%4u,0x%03X) VW(%4u,0x%03X) HTW(%4u,0x%03X) VTW(%4u,0x%03X)\n", nHW, nHW, nVW, nVW, nHTW, nHTW, nVTW, nVTW);

			SYS_REG6 = 1;

			vTaskDelay(100);

			while(SYS_MUTEX10);

			HWIw(nHW);
			VWIw(nVW);
			HTWIw(nHTW);
			VTWIw(nVTW);

			HWOw(nHW);
			VWOw(nVW);
			HTWOw(nHTW);
			VTWOw(nVTW);

			JPG_HWw(nHW);
			JPG_VWw(nVW);

			SYS_MUTEX10 = 0;

			vTaskDelay(100);

			SYS_REG6 = 0;

		} else if (strcmp("memset", argv[1]) == 0) {
			hwflush_dcache_all();
			BDmaMemSet_isr(CPU_ID, (BYTE *)0x8A000000, 0, 4*3*1024*1024);
			hwflush_dcache_all();
		}
	}









#else
	ULONG tstart = 0;
	if (argc == 1) {
		enx_wake_cpu(3);
	} else if (argc == 2) {
		tstart = BenchTimeStart();
		if (strcmp("all", argv[1]) == 0) {
			enx_wake_cpu(0);
			enx_wake_cpu(1);
			enx_wake_cpu(2);
			enx_wake_cpu(3);
		} else if (strcmp("0", argv[1]) == 0) {
			enx_wake_cpu(0);
			while (enx_get_wake_cpu(0));
		} else if (strcmp("1", argv[1]) == 0) {
			enx_wake_cpu(1);
			while (enx_get_wake_cpu(1));
		} else if (strcmp("2", argv[1]) == 0) {
			enx_wake_cpu(2);
			while (enx_get_wake_cpu(2));
		} else if (strcmp("3", argv[1]) == 0) {
			enx_wake_cpu(3);
			while (enx_get_wake_cpu(3));
		}
		UINT tgap = BenchTimeStop(tstart);
		printf("gap: %uus\n", tgap);
	}
#endif

//	vTaskCreate("uart1rx", TestUartTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);

#if 0
	UINT a, b, c;

	a = SYS_MUTEX0;
	b = SYS_MUTEX0;
	c = SYS_MUTEX0;
	printf("a(%d) b(%d) c(%d)\n", a, b, c);
	SYS_MUTEX0 = 0;

	a = SYS_MUTEX1;
	b = SYS_MUTEX1;
	c = SYS_MUTEX1;
	printf("a(%d) b(%d) c(%d)\n", a, b, c);
	SYS_MUTEX1 = 0;

	a = SYS_MUTEX2;
	b = SYS_MUTEX2;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX2 = 0;

	a = SYS_MUTEX3;
	b = SYS_MUTEX3;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX3 = 0;

	a = SYS_MUTEX4;
	b = SYS_MUTEX4;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX4 = 0;

	a = SYS_MUTEX5;
	b = SYS_MUTEX5;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX5 = 0;

	a = SYS_MUTEX6;
	b = SYS_MUTEX6;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX6 = 0;

	a = SYS_MUTEX7;
	b = SYS_MUTEX7;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX7 = 0;

	a = SYS_MUTEX8;
	b = SYS_MUTEX8;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX8 = 0;

	a = SYS_MUTEX9;
	b = SYS_MUTEX9;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX9 = 0;

	a = SYS_MUTEX10;
	b = SYS_MUTEX10;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX10 = 0;

	a = SYS_MUTEX11;
	b = SYS_MUTEX11;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX11 = 0;

	a = SYS_MUTEX12;
	b = SYS_MUTEX12;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX12 = 0;

	a = SYS_MUTEX13;
	b = SYS_MUTEX13;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX13 = 0;

	a = SYS_MUTEX14;
	b = SYS_MUTEX14;
	printf("a(%d) b(%d)\n", a, b);
	SYS_MUTEX14 = 0;
#endif

	//set_csr(mie, MIP_MTIP|MIP_MEIP);
	//clear_csr(mie, MIP_MTIP|MIP_MEIP);

	//TimerSetFreq(8, 250, 1000000000, 500000000);
	//TimerSetPWMEn(8, ENX_ON);
	//TimerStart(8);

#if 0
	// 정답지 만들기
	BYTE *arrSrc = pvPortMalloc(512*1024+1024);
	if (arrSrc == NULL) {
		printf("malloc error(arrSrc), size(%lu)\n", 512*1024+1024);
		return 0;
	}
	UINT *parrOrg = (BYTE *)ENX_MEM_ALIGN(arrSrc);

	hwflush_dcache_range((ULONG)parrOrg, 512*1024);

	for (UINT j = 0; j < 32768; j+=4) {
		parrOrg[j+0] = NUM32(0x87c00000 + (j * 4));
		parrOrg[j+1] = NUM32(0x01234567);
		parrOrg[j+2] = NUM32(0x89abcdef);
		parrOrg[j+3] = NUM32(j / 4);
	}
	hwflush_dcache_range((ULONG)parrOrg, 512*1024);

	hexDump("TEST", parrOrg, 512);
#endif

#if 0
	WdtInit(3000);
	while(1) {
		printf("SYS_WDT_CNT: %u/%u\n", SYS_WDT_CNT, SYS_WDT_LMT);
		vTaskDelay(10);
	}
#endif

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
	if (argc == 2 && strcmp(argv[1], "binary") == 0) {
		unsigned int *pBinary = (unsigned int*)0x80000000;
		tCodeMemInfo *pCodeMemInfo = (tCodeMemInfo *)pBinary;	// Get code memory information
		PRINT_CODE_MEM_INFO("Core", 0);
	} else {
		char strBuf[64] = {0};
		_Gprintf("H/W info ===================================\n");
		snprintf(strBuf, 64, "%.2f", 0.0f);
		printf("ISP Clock             : %sMHz\n", strBuf);
		snprintf(strBuf, 64, "%.2f", CPU_FREQ / 1000.0 / 1000.0);
		printf("CPU Clock             : %sMHz\n", strBuf);
		snprintf(strBuf, 64, "%.2f", AXI_FREQ / 1000.0 / 1000.0);
		printf("AXI Clock             : %sMHz\n", strBuf);
		snprintf(strBuf, 64, "%.2f", APB_FREQ / 1000.0 / 1000.0);
		printf("APB Clock             : %sMHz\n", strBuf);

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
#if EN675_SINGLE
		printf("Model date            : %04u-%02u-%02u %02u:%02u:%02u\n", SYS_RTL_YEAR + 2000, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MIN, SYS_RTL_SEC);
#else
		printf("Model date            : %04X-%02X-%02X %02X:%02X:%02X\n", SYS_RTL_YEAR, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MINUTE, SYS_RTL_SECOND);
#endif

		_Gprintf("S/W info ===================================\n");
		printf("Firmware compile date : %s %s\n", __DATE__, __TIME__);
		printf("EN675 Firmware Ver    : %u.%u.%u\n", EN675_FW_VERSION_MAJOR, EN675_FW_VERSION_MINOR, EN675_FW_VERSION_PATCH);
		printf("freeRTOS Ver          : %u.%u.%u\n", tskKERNEL_VERSION_MAJOR, tskKERNEL_VERSION_MINOR, tskKERNEL_VERSION_BUILD);
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
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

extern irq_count core_irq_count[4];

static void cmd_irq_view(ULONG count)
{
	if (count == 0) {
		_Zprintf("%15u", count);
	} else {
		_Gprintf("%15u", count);
	}
	printf(" | ");
}

int cmd_irq_info(int argc, char *argv[])
{
	char *irqname[41] = {
			"",
			"ISP0~30",
			"VCODEC",
			"DMA0~3",
			"DMA4~7",
			"DMA8~11",
			"DMA12~15",
			"CORE0",
			"CORE1",
			"Ethernet",
			"SDIO0",
			"SDIO1",
			"NPU",
			"I2S",
			"USB",
			"CHKSUM",
			"SHA",
			"AES",
			"OIC",
			"GPIO0~3, PWM0~3, SPI0",
			"GPIO4~7, PWM4~7, I2C0, UART0",
			"GPIO8~11, PWM8~11, SPI1",
			"GPIO12~15, PWM12~15, I2C1, UART1",
			"GPIO16~19, SPI2",
			"GPIO20~23, PWM16~19, I2C2, UART2",
			"GPIO24~27, PWM20~23, SPI3",
			"GPIO28~31, PWM24~27, I2C3, UART3",
			"GPIO32~35, PWM28~31, SPI4",
			"GPIO36~39, I2C4, UART4",
			"GPIO40~43, SPI5",
			"GPIO44~47, I2C5, UART5",
			"GPIO48~51, SPI6",
			"GPIO52~55, PWM32~34, I2C6, UART6",
			"GPIO56~59, PWM35~38, SPI7",
			"GPIO60~63, I2C7, UART7",
			"GPIO64~67, SPI8",
			"GPIO68~71, I2C8, UART8",
			"IR",
			"OMC",
			"Reserved",
			"Reserved",
	};

	printf("       | %15s | %15s | %15s | %15s |\n", "Core0", "Core1", "Core2", "Core3");
	shell_line_print('-', NULL);
#if 1
	printf("  Sync | ");
	for (int i = 0; i < 4; i++) {
		cmd_irq_view(core_irq_count[i].sync_count);
	}
	printf("\n");

	printf("   S/W | ");
	for (int i = 0; i < 4; i++) {
		cmd_irq_view(core_irq_count[i].swirq_count);
	}
	printf("\n");

	printf("  Time | ");
	for (int i = 0; i < 4; i++) {
		cmd_irq_view(core_irq_count[i].timeirq_count);
	}
	printf("\n");
	shell_line_print('-', NULL);
	for (int j = 1; j <= IRQ_SOURCE_COUNT; j++) {
		printf("  Ex%02u | ", j);
		for (int k = 0; k < 4; k++) {
			_Yprintf("%d", enx_externalirq_is_enable(k, j, 0));
			if (core_irq_count[k].exirq_count[j] == 0) {
				_Zprintf("%14u", core_irq_count[k].exirq_count[j]);
			} else {
				_Gprintf("%14u", core_irq_count[k].exirq_count[j]);
			}
			printf(" | ");
		}
		printf("%s\n", irqname[j]);
	}
#else
	printf("  Sync | %15u | %15u | %15u | %15u |\n",
			core_irq_count[0].sync_count, core_irq_count[1].sync_count, core_irq_count[2].sync_count, core_irq_count[3].sync_count);
	printf("   S/W | %15u | %15u | %15u | %15u |\n",
				core_irq_count[0].swirq_count, core_irq_count[1].swirq_count, core_irq_count[2].swirq_count, core_irq_count[3].swirq_count);
	printf("  Time | %15u | %15u | %15u | %15u |\n",
				core_irq_count[0].timeirq_count, core_irq_count[1].timeirq_count, core_irq_count[2].timeirq_count, core_irq_count[3].timeirq_count);
	shell_line_print('-', NULL);
	for (int j = 1; j <= IRQ_SOURCE_COUNT; j++) {
		printf("  Ex%02u | %15u | %15u | %15u | %15u |\n", j,
				core_irq_count[0].exirq_count[j], core_irq_count[1].exirq_count[j], core_irq_count[2].exirq_count[j], core_irq_count[3].exirq_count[j]);
	}
#endif
	shell_line_print('-', NULL);
	return 0;
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

#define TEST_TIMER_DIV 250
#define TEST_TIMER_LMT 2474
#define TEST_TIMER_TRIG (TEST_TIMER_LMT>>1)

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
				ttsTest.index = -1;
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
		if (ttsTest.index > 38 || ttsTest.index < 0) {
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
		if (ttsTest.index > 38 || ttsTest.index < 0) {
			printf("Select a channel!\n");
			return 0;
		}
		TimerStop(ttsTest.index);
	} else if (strcmp(argv[1], "idx") == 0) {
		if (argc == 3) {
			printf("Old select Timer(%d)\n", ttsTest.index);
			ttsTest.index = atoi(argv[2]);
			if (ttsTest.index > 38 || ttsTest.index < 0) {
				printf("Invalid channel selection.(%d)\n", ttsTest.index);
				ttsTest.index = -1;
			} else {
				printf("New select Timer(%d)\n", ttsTest.index);
			}
		} else {
			printf("Select Timer(%d)\n", ttsTest.index);
		}
	} else if (strcmp(argv[1], "div") == 0 && argc == 3) {
		if (ttsTest.index > 38 || ttsTest.index < 0) {
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
		if (ttsTest.index > 38 || ttsTest.index < 0) {
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
		if (ttsTest.index > 38 || ttsTest.index < 0) {
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
		if (ttsTest.index > 38 || ttsTest.index < 0) {
			printf("Select a channel!\n");
			return 0;
		}
		if (TimerGetPWMEn(ttsTest.index) == ENX_ON) {
			TimerSetPWMEn(ttsTest.index, ENX_OFF);
		} else {
			TimerSetPWMEn(ttsTest.index, ENX_ON);
		}
	} else if (strcmp(argv[1], "auto") == 0) {
		if (ttsTest.index == -1) {
			ttsTest.index = 0;
		}
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
#if EN675_SINGLE
//	WdtoscWaitReboot(3000);

	if (argc == 2) {
		if (strcmp(argv[1], "1") == 0) {
			WDT_OSC_LMT = OSC_FREQ;
			WDT_OSC_EN = 1;
		} else if (strcmp(argv[1], "2") == 0) {
			SYS_WDT_LMT = OSC_FREQ;
			SYS_WDT_EN = 1;
		}
		while(1) {
			printf(".");
		}
	} else {
		WdtoscWaitReboot(3000);
	}
#else
#if 1
	WdtWaitReboot(3000);
#else
	WdtInit(0);
	while(1);
#endif
#endif
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

int cmd_test_oic(int argc, char *argv[])
{
	printf("OIC is inactive.\n");
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
		printf("=======================================\n");
		printf("SYS_RST_RID(%u)\n", SYS_RST_RID);
		printf("SYS_RST_PERI_REG(%u)\n", SYS_RST_PERI_REG);
		printf("SYS_RST_H265_REG(%u)\n", SYS_RST_H265_REG);
		printf("SYS_MCU_PHY_RST(%u)\n", SYS_MCU_PHY_RST);
		printf("SYS_RST_NPU_REG(%u)\n", SYS_RST_NPU_REG);
		printf("SYS_RST_CPU0(%u)\n", SYS_RST_CPU0);
		printf("SYS_RST_CPU1(%u)\n", SYS_RST_CPU1);
		printf("SYS_RST_CPU2(%u)\n", SYS_RST_CPU2);
		printf("SYS_RST_CPU3(%u)\n", SYS_RST_CPU3);
		printf("SYS_RST_ISP(%u)\n", SYS_RST_ISP);
		printf("=======================================\n");
		printf("SYS_NPU  PD(%u) SEL(%u)\n", SYS_NPU_PD, SYS_NPU_CKSEL);
		printf("SYS_CPU0 PD(%u) SEL(%u)\n", SYS_CPU0_PD, SYS_CPU0_CKSEL);
		printf("SYS_CPU1 PD(%u) SEL(%u)\n", SYS_CPU1_PD, SYS_CPU1_CKSEL);
		printf("SYS_CPU2 PD(%u) SEL(%u)\n", SYS_CPU2_PD, SYS_CPU2_CKSEL);
		printf("SYS_CPU3 PD(%u) SEL(%u)\n", SYS_CPU3_PD, SYS_CPU3_CKSEL);
		printf("=======================================\n");
		printf("SYS_WDT CNT(%u)\n", SYS_WDT_CNT);
		printf("        LMT(%u)\n", SYS_WDT_LMT);
		printf("        EN (%u)\n", SYS_WDT_EN);
		printf("        CLR(%u)\n", SYS_WDT_CLR);
		printf("=======================================\n");
		char strName[20] = {0};
		UINT *p = (UINT *)strName;
		p[0] = SYS_MARK0;
		p[1] = SYS_MARK1;
		p[2] = SYS_MARK2;
		p[3] = SYS_MARK3;
		p[4] = SYS_MARK4;
		printf("SYS_MAKR [%s]\n", strName);
		hexDump("SYS_MARK", strName, 20);
#if EN675_SINGLE
		printf("SYS_RTL  %04u-%02u-%02u %02u:%02u:%02u\n", SYS_RTL_YEAR + 2000, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MIN, SYS_RTL_SEC);
#else
		printf("SYS_RTL  %04X-%02X-%02X %02X:%02X:%02X\n", SYS_RTL_YEAR, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MINUTE, SYS_RTL_SECOND);
#endif
		printf("=======================================\n");
		printf("SYS_REG0(0x%08X) ", SYS_REG0);
		printf("SYS_REG1(0x%08X) ", SYS_REG1);
		printf("SYS_REG2(0x%08X) ", SYS_REG2);
		printf("SYS_REG3(0x%08X)\n", SYS_REG3);
		printf("SYS_REG4(0x%08X) ", SYS_REG4);
		printf("SYS_REG5(0x%08X) ", SYS_REG5);
		printf("SYS_REG6(0x%08X) ", SYS_REG6);
		printf("SYS_REG7(0x%08X)\n", SYS_REG7);
		printf("=======================================\n");
		enx_exirq_view();

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
		case 4:
			SYS_REG4 = var;
			break;
		case 5:
			SYS_REG5 = var;
			break;
		case 6:
			SYS_REG6 = var;
			break;
		case 7:
			SYS_REG7 = var;
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
		} else if (strcmp(argv[0], "idump") == 0) {
			printf("call hwdiscard_dcache_range\n");
			hwdiscard_dcache_range(getData, getLen);
		}
		hexDump("Memory Dump", (void *)getData, getLen);
	} else {
		Shell_Unknown();
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#if defined(__AUDIO__)
static void i2s_status_print(char *title, char *cmd1, char *cmd2, UINT type, UINT val, char *text_type[4], int end)
{
	if (cmd1 == NULL) {
		printf(" %-12s         : ", title);
	} else {
		printf(" %-12s(%-2s %-3s) : ", title, cmd1, cmd2);
	}

	if (type == 16) {
		printf("0x%-13X", val);
	} else if (type == 10) {
		printf("%-15u", val);
	} else if (type == 0xff) {
		printf("%u(%-12s)", val, text_type[val]);
	}

	if (end == 1) {
		printf("\n");
	} else if (end == 2) {
		printf("|\n");
	} else {
		printf("|");
	}
}

#define i2s_getset(val)	{if (argc == 4) { val = u32SetData; } u32GetData = val;}

#include "audtxrx.h"
int cmd_test_i2s(int argc, char *argv[])
{
	if (argc == 1) {
		printf("== I2S State ===================================================================\r\n");
		tlv320aic3206_print_name();
		printf("== Common State ================================================================\r\n");
#if EN675_SINGLE
		i2s_status_print("PD", "ct", "pd", 10, I2S_PD, NULL, 0);
#endif
		i2s_status_print("MODE", "ct", "mod", 0xff, I2S_MODE, ((char*[4]){"Slave mode", "Master mode", "error", "error"}), 1);
		i2s_status_print("BYTE", "ct", "byt", 0xff, I2S_BYTE, ((char*[4]){"8byte", "16byte", "24byte", "32byte"}), 0);				i2s_status_print("SCKCNT", "ct", "sck", 10, I2S_SCKCNT, NULL, 1);
		printf("== TX State ===========================|== RX State ============================\r\n");
		i2s_status_print("TXEN", "tx", "en", 10, I2S_TXEN, NULL, 0);																i2s_status_print("RXEN", "rx", "en", 10, I2S_RXEN, NULL, 1);
		i2s_status_print("ADDR", "tx", "adr", 16, I2S_ADRR, NULL, 0);																i2s_status_print("ADRW", "rx", "adr", 16, I2S_ADRW, NULL, 1);
		i2s_status_print("TX_ADDR", NULL, NULL, 16, I2S_TX_ADDR, NULL, 0);															i2s_status_print("RX_ADDR", NULL, NULL, 16, I2S_RX_ADDR, NULL, 1);
		i2s_status_print("RDBYTE", "tx", "rdb", 0xff, I2S_RDBYTE, ((char*[4]){"128byte", "256byte", "512byte", "1024byte"}), 0);	i2s_status_print("WRBYTE", "rx", "wrb", 0xff, I2S_WRBYTE, ((char*[4]){"128byte", "256byte", "512byte", "1024byte"}), 1);
		i2s_status_print("TXEDN", "tx", "edn", 0xff, I2S_TXEDN, ((char*[4]){"Big-endian", "Little-endian", "error", "error"}), 0);	i2s_status_print("RXEDN", "rx", "edn", 0xff, I2S_RXEDN, ((char*[4]){"Big-endian", "Little-endian", "error", "error"}), 1);
		i2s_status_print("TXCODEC", "tx", "cd", 0xff, I2S_TXCODEC, ((char*[4]){"PCM", "PCM", "G711-A", "G711-u"}), 0);				i2s_status_print("RXCODEC", "rx", "cd", 0xff, I2S_RXCODEC, ((char*[4]){"PCM", "PCM", "G711-A", "G711-u"}), 1);
		i2s_status_print("RDDW", "tx", "rdw", 0xff, I2S_RDDW, ((char*[4]){"8bit", "16bit", "24bit", "32bit"}), 0);					i2s_status_print("WRDW", "rx", "wdw", 0xff, I2S_RXDW, ((char*[4]){"8bit", "16bit", "24bit", "32bit"}), 1);
		i2s_status_print("TXDW", "tx", "tdw", 0xff, I2S_TXDW, ((char*[4]){"8bit", "16bit", "24bit", "32bit"}), 0);					i2s_status_print("RXDW", "rx", "rdw", 0xff, I2S_WRDW, ((char*[4]){"8bit", "16bit", "24bit", "32bit"}), 1);
		i2s_status_print("TXMODE", "tx", "md", 0xff, I2S_TXMODE, ((char*[4]){"Left", "Right", "(L+R)/2", "Stereo"}), 0);			i2s_status_print("RXMODE", "rx", "md", 0xff, I2S_RXMODE, ((char*[4]){"Left", "Right", "(L+R)/2", "Stereo"}), 1);
		i2s_status_print("RXLEN", "tx", "rdl", 0xff, I2S_RDLEN, ((char*[4]){"128KB", "256KB", "512KB", "1024KB"}), 0);				i2s_status_print("WRLEN", "rx", "wrl", 0xff, I2S_WRLEN, ((char*[4]){"128KB", "256KB", "512KB", "1024KB"}), 1);
		i2s_status_print("TXLR", "tx", "lr", 0xff, I2S_TXLR, ((char*[4]){"Mute", "Left", "Right", "Both"}), 2);
#if EN675_SINGLE
		printf("== TX-Plat State ===============================================================\r\n");
		i2s_status_print("PLAT_TXEN", "tx", "pen", 10, I2S_PLAT_TXEN, NULL, 0);														i2s_status_print("PLAT_LENGTH", "tx", "psz", 10, I2S_PLAT_LENGTH, NULL, 1);
		i2s_status_print("PLAT_MODE", "tx", "pmd", 0xff, I2S_PLAT_MODE, ((char*[4]){"Loop", "One-shot", "error", "error"}), 2);
		i2s_status_print("PLAT_IRQ_1", "tx", "pwr", 10, I2S_PLAT_IRQ_BUF_WR, NULL, 0);												i2s_status_print("PLAT_IRQ_2", "tx", "prd", 10, I2S_PLAT_IRQ_BUF_RD, NULL, 1);
		i2s_status_print("PLAT_IRQ_3", "tx", "plw", 10, I2S_PLAT_IRQ_LAST_WORD, NULL, 0);											i2s_status_print("PLAT_IRQ_4", "tx", "pfn", 10, I2S_PLAT_IRQ_FINISH, NULL, 1);
//		i2s_status_print("I2S_PLAT_IRQ_UP", "tx", "pup", 10, I2S_PLAT_IRQ_UP, NULL, 0);												i2s_status_print("I2S_PLAT_IRQ_DOWN", "tx", "pdn", 10, I2S_PLAT_IRQ_DOWN, NULL, 1);
//		i2s_status_print("I2S_PLAT_IRQ_DOWN1", "tx", "pd1", 10, I2S_PLAT_IRQ_DOWN1, NULL, 0);										i2s_status_print("I2S_PLAT_IRQ_DOWN2", "tx", "pd2", 10, I2S_PLAT_IRQ_DOWN2, NULL, 1);
#endif
		printf("== IRQ state ===================================================================\r\n");
		i2s_status_print("TX_IRQ_EN", "tx", "irq", 10, I2S_TX_IRQ_EN, NULL, 0);														i2s_status_print("RX_IRQ_EN", "rx", "irq", 10, I2S_RX_IRQ_EN, NULL, 1);
		i2s_status_print("TX_IRQ", NULL, NULL, 10, I2S_TX_IRQ, NULL, 0);															i2s_status_print("RX_IRQ", NULL, NULL, 10, I2S_RX_IRQ, NULL, 1);
		printf("== AUD Pool ====================================================================\r\n");
		i2s_status_print("TX NEW POS", NULL, NULL, 16, audpool.audtx_now_pos, NULL, 0);												i2s_status_print("RX NEW POS", NULL, NULL, 16, audpool.audrx_now_pos, NULL, 1);
		i2s_status_print("TX IRQ COUNT", NULL, NULL, 10, audpool.audtx_irq_count, NULL, 0);											i2s_status_print("RX IRQ COUNT", NULL, NULL, 10, audpool.audrx_irq_count, NULL, 1);
		printf("================================================================================\r\n");

#if EN675_SINGLE
	} else if (strcmp(argv[1], "pt") == 0) {
		I2S_PLAT_IRQ_T = 0xffffffff;
		I2S_PLAT_IRQ_UP = 0x0;
		I2S_PLAT_IRQ_DOWN2 = 0x0;
#endif
	} else if (argc == 3 || argc == 4) {
		UINT u32GetData = 0, u32SetData = 0, bErr = 0;
		if (argc > 3) {
			u32SetData = atoi(argv[3]);
		}

		if (strcmp(argv[1], "ct") == 0) {
			if (strcmp(argv[2], "byt") == 0)		i2s_getset(I2S_BYTE)
			else if (strcmp(argv[2], "mod") == 0)	i2s_getset(I2S_MODE)
			else if (strcmp(argv[2], "sck") == 0)	i2s_getset(I2S_SCKCNT)
			else {									bErr = 1;	}
		} else if (strcmp(argv[1], "tx") == 0) {
			if (strcmp(argv[2], "en") == 0)			i2s_getset(I2S_TXEN)
			else if (strcmp(argv[2], "adr") == 0)	i2s_getset(I2S_ADRR)
			else if (strcmp(argv[2], "rdb") == 0)	i2s_getset(I2S_RDBYTE)
			else if (strcmp(argv[2], "edn") == 0)	i2s_getset(I2S_TXEDN)
			else if (strcmp(argv[2], "tdw") == 0)	i2s_getset(I2S_TXDW)
			else if (strcmp(argv[2], "cd") == 0)	i2s_getset(I2S_TXCODEC)
			else if (strcmp(argv[2], "md") == 0)	i2s_getset(I2S_TXMODE)
			else if (strcmp(argv[2], "rdl") == 0)	i2s_getset(I2S_RDLEN)
			else if (strcmp(argv[2], "rdw") == 0)	i2s_getset(I2S_RDDW)
			else if (strcmp(argv[2], "lr") == 0)	i2s_getset(I2S_TXLR)
#if EN675_SINGLE
			else if (strcmp(argv[2], "pen") == 0)	i2s_getset(I2S_PLAT_TXEN)
			else if (strcmp(argv[2], "psz") == 0)	i2s_getset(I2S_PLAT_LENGTH)
			else if (strcmp(argv[2], "pmd") == 0)	i2s_getset(I2S_PLAT_MODE)
			else if (strcmp(argv[2], "pwr") == 0)	i2s_getset(I2S_PLAT_IRQ_BUF_WR)
			else if (strcmp(argv[2], "prd") == 0)	i2s_getset(I2S_PLAT_IRQ_BUF_RD)
			else if (strcmp(argv[2], "plw") == 0)	i2s_getset(I2S_PLAT_IRQ_LAST_WORD)
			else if (strcmp(argv[2], "pfn") == 0)	i2s_getset(I2S_PLAT_IRQ_FINISH)
			else if (strcmp(argv[2], "pal") == 0)	i2s_getset(I2S_PLAT_IRQ_T)
			else if (strcmp(argv[2], "pup") == 0)	i2s_getset(I2S_PLAT_IRQ_UP)
			else if (strcmp(argv[2], "pdn") == 0)	i2s_getset(I2S_PLAT_IRQ_DOWN)
			else if (strcmp(argv[2], "pd1") == 0)	i2s_getset(I2S_PLAT_IRQ_DOWN1)
			else if (strcmp(argv[2], "pd2") == 0)	i2s_getset(I2S_PLAT_IRQ_DOWN2)
#endif
			else if (strcmp(argv[2], "irq") == 0)	i2s_getset(I2S_TX_IRQ_EN)
			else {									bErr = 1;	}
		} else if (strcmp(argv[1], "rx") == 0) {
			if (strcmp(argv[2], "en") == 0)			i2s_getset(I2S_RXEN)
			else if (strcmp(argv[2], "adr") == 0)	i2s_getset(I2S_ADRW)
			else if (strcmp(argv[2], "wrb") == 0)	i2s_getset(I2S_WRBYTE)
			else if (strcmp(argv[2], "edn") == 0)	i2s_getset(I2S_RXEDN)
			else if (strcmp(argv[2], "rdw") == 0)	i2s_getset(I2S_WRDW)
			else if (strcmp(argv[2], "cd") == 0)	i2s_getset(I2S_RXCODEC)
			else if (strcmp(argv[2], "md") == 0)	i2s_getset(I2S_RXMODE)
			else if (strcmp(argv[2], "wrl") == 0)	i2s_getset(I2S_WRLEN)
			else if (strcmp(argv[2], "wdw") == 0)	i2s_getset(I2S_RDDW)
			else if (strcmp(argv[2], "irq") == 0)	i2s_getset(I2S_RX_IRQ_EN)
			else {									bErr = 1;	}
		} else {
			bErr = 1;
		}

		if (bErr == 1) {
			Shell_Unknown();
		} else {
			if (argc == 4) {
				printf("[SET] Cmd:%s_%s, Value:%u\n", argv[1], argv[2], u32SetData);
			}
			printf("[GET] Cmd:%s_%s, Value:%u\n", argv[1], argv[2], u32GetData);
		}
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

	}
	return 0;
}
#endif

TimerHandle_t xTimersADCTest;
static void test_adc_process(TimerHandle_t xTimer)
{
	for (int i = 0; i < ADC_CNT; i++) {
		printf("CH%u %4u|", i, AdcGet(i));
	}
	printf("\n");
}

int cmd_test_adc(int argc, char *argv[])
{
	if (argc == 2 && strcmp(argv[1], "start") == 0) {
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
	} else if (argc == 2 && strcmp(argv[1], "stop") == 0) {
		BaseType_t xReturn = xTimerStop( xTimersADCTest, 0 );
		if (xReturn == pdPASS) {
			printf("ADC Timer stop ok!\n");
		} else {
			printf("ADC Timer stop fail!\n");
		}
	} else if (argc == 2 && strcmp(argv[1], "on") == 0) {
		AdcOn();
		printf("ADC on\n");
	} else if (argc == 3 && strcmp(argv[1], "off") == 0) {
		AdcOff();
		printf("ADC off\n");
	} else {
		printf("|");
		for (int i = 0; i < ADC_CNT; i++) {
			printf("CH%u %4u|", i, AdcGet(i));
		}
		printf("\n");
	}
	return 0;
}

#if USE_IR
int cmd_test_ir(int argc, char *argv[])
{
	if (argc == 1) {
		printf("== IR =============\n");
		printf("(RW) IR_CLK (clk [val])           : %u\n", IR_CLK);
		printf("(R ) IR_IRQ                       : %u\n", IR_IRQ);
		printf("(RW) IR_IRQ_EN (irq on / off)     : %u\n", IR_IRQ_EN);
		printf("(R ) IR_IRQ_CLR                   : %u\n", IR_IRQ_CLR);
		printf("(RW) IR_EN (on / off)             : %u\n", IR_EN);
		printf("(RW) IR_IRQ_ADDR (irq addr [val]) : 0x%02X, %u\n", IR_IRQ_ADDR, IR_IRQ_ADDR);
		printf("(RW) IR_MARGIN (m [val])          : %u\n", IR_MARGIN);
		printf("(RW) IR_RPT_MG (rm [val])         : %u\n", IR_RPT_MG);
#if EN675_SINGLE
		printf("(RW) IR_SYNC (s [val])            : %u\n", IR_SYNC);
#endif
		printf("(R ) IR_RPT                       : %u\n", IR_RPT);
		printf("(R ) IR_ADDR                      : 0x%02X, %u\n", IR_ADDR, IR_ADDR);
		printf("(R ) IR_DAT                       : 0x%02X, %u\n", IR_DAT, IR_DAT);
	} else {
		if (argc == 2 && strcmp("init", argv[1]) == 0) {
			IrInit(0);
		} else if (argc == 2 && strcmp("on", argv[1]) == 0) {
			IrSetEn(ENX_ON);
		} else if (argc == 2 && strcmp("off", argv[1]) == 0) {
			IrSetEn(ENX_OFF);
		} else if (argc == 3 && strcmp("clk", argv[1]) == 0) {
			UINT val = atoi(argv[2]);
			IrSetClk(val);
			printf("IrSetClk(%u) IrGetClk(%u)\n", val, IrGetClk());
		} else if (argc == 3 && strcmp("irq", argv[1]) == 0) {
			if (strcmp("on", argv[2]) == 0) {
				IrSetIrqEn(ENX_ON);
			} else if (strcmp("off", argv[2]) == 0) {
				IrSetIrqEn(ENX_OFF);
			} else {
				Shell_Unknown();
			}
		} else if (argc == 4 && strcmp("irq", argv[1]) == 0) {
			if (strcmp("addr", argv[2]) == 0) {
				UINT val = atoi(argv[3]);
				IrSetIrqAddr(val);
				printf("IrSetIrqAddr(%u) IrGetIrqAddr(%u)\n", val, IrGetIrqAddr());
			} else {
				Shell_Unknown();
			}
		} else if (argc == 3 && strcmp("m", argv[1]) == 0) {
			UINT val = atoi(argv[2]);
			IrSetMargin(val);
			printf("IrSetMargin(%u) IrGetMargin(%u)\n", val, IrGetMargin());
		} else if (argc == 3 && strcmp("rm", argv[1]) == 0) {
			UINT val = atoi(argv[2]);
			IrSetRepeatMargin(val);
			printf("IrSetRepeatMargin(%u) IrGetRepeatMargin(%u)\n", val, IrGetRepeatMargin());
#if EN675_SINGLE
		} else if (argc == 3 && strcmp("s", argv[1]) == 0) {
			UINT val = atoi(argv[2]);
			IrSetSync(val);
			printf("IrSetSync(%u) IrGetSync(%u)\n", val, IrGetSync());
#endif
		} else {
			Shell_Unknown();
		}
	}
	return 0;
}
#endif
