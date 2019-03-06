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
	{"sysreg",		cmd_test_sysreg,	sSysreg			},
	{"sd",			cmd_test_sdio,		sSdioTest		},
//	{"dma",			cmd_test_dma,		sDmaTest		},

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
	if (argc == 2) {
		UINT test_size = atoi(argv[1]);
		if (test_size == 0) {
			printf("test size error\n");
			return 0;
		}
		printf("DMA TEST(%dbyte)\n", test_size);

		BYTE *dataA = pvPortCalloc(test_size, 1);
		if (dataA == NULL) {
			printf("malloc error A\n");
			return 0;
		} else {
			printf("malloc A(0x%08X)\n", dataA);
		}
		BYTE *dataB = pvPortCalloc(test_size, 1);
		if (dataB == NULL) {
			printf("malloc error B\n");
			return 0;
		} else {
			printf("malloc B(0x%08X)\n", dataB);
		}

		for (ULONG i = 0; i < test_size; i++) {
			dataA[i] = i;
		}

		printf("malloc A address(0x%08X)\n", dataA);
		printf("malloc B address(0x%08X)\n", dataB);

		printf("dma copy start\n");
		//hwflush_dcache_range(dataB, dataB+test_size);

		hexDump("A", dataA, test_size);
		hexDump("B", dataB, test_size);

		CDmaMemCpy_isr(0, dataB, dataA, test_size);
		//hwflush_dcache_range(dataB, dataB+test_size);
		printf("dma copy end\n");

		hexDump("A", dataA, test_size);
		hexDump("B", dataB, test_size);

		vPortFree(dataA);
		vPortFree(dataB);
		printf("free ok\n");
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#define MALIGN16 __attribute__((__aligned__(16)))
MALIGN16 char SRAMDST2[64] = {0};

int UsrCmd_i(int argc, char *argv[])
{
	char *SRAMDST2;
	char *SRAMSRC2;

	int shift = 0;
	while(1) {
		// C DMA TEST//////////////////////////////////////////////////////////////////////////////////////////////////////////
		_printf("\n----------------------------------CPU DMA TEST----------------------------------\n");
		SRAMSRC2 = SRAM_BASE+(shift++);
		//SRAMDST2 = pvPortCalloc(16, 1);
		memset(SRAMDST2, 0, 16);

		_printf("SRAMSRC2 %x\n",SRAMSRC2);
		_printf("SRAMSRC2 DEFAULT : ");
		for(int i = 0; i<0x10; i++) {
			/**(SRAMSRC2+i) = i; */
			_printf("%02X ", *(SRAMSRC2+i));
		}
		_printf("\n\n");
		_printf("SRAMDST2 %x\n",SRAMDST2);
		_printf("SRAMDST2 DEFAULT : ");
		for(int i = 0; i<0x10; i++) {
			_printf("%02X ", *(SRAMDST2+i));
		}
		_printf("\n\n");

		CDmaMemCpy_isr(0, SRAMDST2, SRAMSRC2, 0x10);

		_printf("\nSRAMDST2 after DMA C : ");
		for(int i = 0; i<0x10; i++) {
			_printf("%02X ", *(SRAMDST2+i));
		}
		_printf("\n");
		for(int i=0; i<0x10;i++) {
			if(*(SRAMDST2+i) != *(SRAMSRC2+i)) {
				_printf("After DMA C SRAMSRC2 %x SRAMDST2 %x Failed Unmatched %dth %x %x!!!!!\n", SRAMSRC2, SRAMDST2, i,*(SRAMSRC2+i), *(SRAMDST2+i));
				while(1);
			}
		}
		vPortFree(SRAMDST2);
		_printf("\n----------------------------------CPU DMA PASS----------------------------------\n");

		// B DMA TEST//////////////////////////////////////////////////////////////////////////////////////////////////////////
		_printf("\n----------------------------------BUS DMA TEST----------------------------------\n");
		SRAMSRC2 = SRAM_BASE+(shift++);
		//SRAMDST2 = pvPortCalloc(16, 1);
		memset(SRAMDST2, 0, 16);

		_printf("SRAMSRC2 %x\n",SRAMSRC2);
		_printf("SRAMSRC2 DEFAULT : ");
		for(int i = 0; i<0x10; i++) {
			/**(SRAMSRC2+i) = i; */
			_printf("%02X ", *(SRAMSRC2+i));
		}
		_printf("\n\n");
		_printf("SRAMDST2 %x\n",SRAMDST2);
		_printf("SRAMDST2 DEFAULT : ");
		for(int i = 0; i<0x10; i++) {
			_printf("%02X ", *(SRAMDST2+i));
		}
		_printf("\n\n");

		BDmaMemCpy_isr(0, SRAMDST2, SRAMSRC2, 0x10);

		_printf("\nSRAMDST2 after DMA B : ");
		for(int i = 0; i<0x10; i++) {
			_printf("%02X ", *(SRAMDST2+i));
		}
		_printf("\n");

		//destination invalidate
		hwflush_dcache_range(SRAMDST2, SRAMDST2+0x10);
		_printf("SRAMDST2 after Flush : "); for(int i = 0; i<0x10; i++) {
			_printf("%02X ", *(SRAMDST2+i));
		}
		_printf("\n\n");
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for(int i=0; i<0x10;i++) {
			if(*(SRAMDST2+i) != *(SRAMSRC2+i)) {
				_printf("After DMA B SRAMSRC2 %x SRAMDST2 %x Failed Unmatched %dth %x %x!!!!!\n", SRAMSRC2, SRAMDST2, i,*(SRAMSRC2+i), *(SRAMDST2+i));
				while(1);
			}
		}

		_printf("\n----------------------------------BUS DMA PASS----------------------------------\n");

		vPortFree(SRAMDST2);

		if(shift > 0x1000) break;
    }

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

int cmd_perl_gpio(int argc, char *argv[])
{
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

int cmd_task_status(int argc, char *argv[])
{
	vTaskInfoPrint();
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
