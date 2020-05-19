//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include "enx_freertos.h"

#include "shell_cmd_mem.h"
#include <stdio.h> // snprintf
#include <stdlib.h> // atoi
#include <string.h> // strcmp

const char *sMemoryTest[]   = {"Test Memory",                                     (char*)0};
const char *sBDmaTest[]     = {"Test BDMA",                                       (char*)0};
const char *sCDmaTest[]     = {"Test CDMA",                                       (char*)0};
const char *sMemsetTest[]   = {"Test Memset",                                     (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
#if 0
typedef ATTR_MALIGN64 struct {
	BYTE data[512];
} DmaTestBuffer;
static DmaTestBuffer dma_data;
#endif

#if 0
static void TestDataWrite(BYTE *addr, BYTE startVar, UINT size)
{
	for (UINT i = 0; i < size; i++) {
		addr[i] = startVar++;
	}
}
#endif

typedef enum {
	eDMAtest_DDRtoDDR,
	eDMAtest_DDRtoSRAM,
	eDMAtest_SRAMtoDDR,
	eDMAtest_SRAMtoSRAM,
} DMAtestTarget;

typedef struct {
	TaskHandle_t taskHandle;

	BYTE *arrSrc;
	BYTE *arrDst;
	ULONG u64BufSize;

	UINT u32TestSize;
	UINT u32Move;
	UINT cMode;
	UINT u32CH;
	UINT u32Close;

	DMAtestTarget target;
} DmaTestStr;

DmaTestStr dmaitem;
#define DMA_TEST_MALLOC_SIZE (256*1024) // SRAM
//#define DMA_TEST_MALLOC_SIZE (1024*1024) // DDR

static void DmaTestIrq(void *ctx)
{
	if (dmaitem.taskHandle) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(dmaitem.taskHandle, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;
		}
	}
}

static void DmaTestTask(void *ctx)
{
	dmaitem.u32Close = 0;

	if (dmaitem.target == eDMAtest_DDRtoDDR || dmaitem.target == eDMAtest_DDRtoSRAM) {
		dmaitem.arrSrc = pvPortMalloc(dmaitem.u64BufSize);
		if (dmaitem.arrSrc == NULL) {
			printf("malloc error(arrSrc), size(%lu)\n", dmaitem.u64BufSize);
			dmaitem.taskHandle = NULL;
			vTaskDelete(NULL);
		}
	} else {
		dmaitem.arrSrc = (BYTE *)0xA0000000;
	}

	if (dmaitem.target == eDMAtest_DDRtoDDR || dmaitem.target == eDMAtest_SRAMtoDDR) {
		dmaitem.arrDst = pvPortMalloc(dmaitem.u64BufSize);
		if (dmaitem.arrDst == NULL) {
			printf("malloc error(arrDst), size(%lu)\n", dmaitem.u64BufSize);
			vPortFree(dmaitem.arrDst);
			dmaitem.taskHandle = NULL;
			vTaskDelete(NULL);
		}
	} else {
		dmaitem.arrDst = (BYTE *)0xA0020000;
	}

	BYTE *parrSrc = dmaitem.arrSrc;
	BYTE *parrDst = dmaitem.arrDst;

	if (dmaitem.cMode == 'B') {
		BDmaIrqCallback(dmaitem.u32CH, DmaTestIrq, NULL);
		BDmaSetIrqEn(dmaitem.u32CH, ENX_ON);
	} else if (dmaitem.cMode == 'C') {
		CDmaIrqCallback(dmaitem.u32CH, DmaTestIrq, NULL);
		CDmaSetIrqEn(dmaitem.u32CH, ENX_ON);
	} else {
		printf("mode error, '%c'\n", dmaitem.cMode);
		vPortFree(dmaitem.arrSrc);
		vPortFree(dmaitem.arrDst);
		dmaitem.taskHandle = NULL;
		vTaskDelete(NULL);
	}

	//srand(rdcycle());
	srand(1);

	printf("%cDMA%u Test Task(Move:%u)(Count:%u)", dmaitem.cMode, dmaitem.u32CH, dmaitem.u32Move, dmaitem.u32Move * dmaitem.u32Move);

	if (dmaitem.u32TestSize == -1) {
		printf("(Size:Random)\n");
	} else {
		printf("(Size:%ubyte)\n", dmaitem.u32TestSize);
	}

	switch (dmaitem.target) {
		case eDMAtest_DDRtoDDR:
			_Cprintf("      DDR to DDR\n");
			break;
		case eDMAtest_DDRtoSRAM:
			_Cprintf("      DDR to SRAM\n");
			break;
		case eDMAtest_SRAMtoDDR:
			_Cprintf("      SRAM to DDR\n");
			break;
		case eDMAtest_SRAMtoSRAM:
			_Cprintf("      SRAM to SRAM\n");
			break;
	}

	UINT u32RelTestSize = 0;
	if (dmaitem.u64BufSize > 16 * 1024 * 1024) {
		u32RelTestSize = 16 * 1024 * 1024;
	} else {
		u32RelTestSize = dmaitem.u64BufSize;
	}
	hwflush_dcache_range_rtos((ULONG)dmaitem.arrSrc, u32RelTestSize);
	for (UINT i = 0; i < dmaitem.u64BufSize; i++) {
		dmaitem.arrSrc[i] = rand() % 255;
	}
	hwflush_dcache_range_rtos((ULONG)dmaitem.arrSrc, u32RelTestSize);

	u32RelTestSize = 0;
	UINT pass = 0, fail = 0, flag = 0;
	for (UINT i = 0; i < dmaitem.u32Move; i++) {
		if (dmaitem.u32Close == 1) {
			break;
		}

		parrDst = dmaitem.arrDst;
		if (dmaitem.u32TestSize == -1) {
			u32RelTestSize = rand() % 65536 + 1;
		} else {
			u32RelTestSize = dmaitem.u32TestSize;
		}

		printf("%3u%% %4u/%4u TEST Dst(0x%08X~0x%08X) Src(0x%08X) - Size(%6u) -     ", ((i+1) * 100) / dmaitem.u32Move, i+1, dmaitem.u32Move, (UINT)(intptr_t)parrDst, (UINT)(intptr_t)(parrDst+dmaitem.u32Move), (UINT)(intptr_t)parrSrc, u32RelTestSize);

		ULONG total_time = 0;
		ULONG total_size = 0;
		for (UINT j = 0; j < dmaitem.u32Move; j++) {
			if (dmaitem.u32Close == 1) {
				break;
			}
			// Set array
			//if (dmaitem.cMode == 'B') {
			BDmaMemSet_rtos_async(dmaitem.u32CH, parrDst, 0, u32RelTestSize);
			if (!ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
				flag = 1;
				printf("\b\b\b\b");
				_Rprintf("\nTimeout! memset 0x%08X, Len:%ubyte\n", parrDst, u32RelTestSize);
				if (dmaitem.cMode == 'C') {
					extern void CDmaRegshow(UINT nCH);
					CDmaRegshow(dmaitem.u32CH);
				}
				i = j = -0xf;
				fail++;
				break;
			}
			//} else if (dmaitem.cMode == 'C') {
			//	CDmaMemSet_rtos(dmaitem.u32CH, parrDst, 0, u32RelTestSize);
			//} else {
			//memset(parrDst, 0, u32RelTestSize);
			//}
			//portMEMORY_BARRIER();
			//hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);
#if 1
			ULONG tstart = 0;
			if (dmaitem.cMode == 'B') {
				// Set array
				hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);
//				portMEMORY_BARRIER();
				//hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);

				tstart = BenchTimeStart();
				BDmaMemCpy_rtos_async(dmaitem.u32CH, parrDst, parrSrc, u32RelTestSize);
				//portMEMORY_BARRIER();
//				hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);

			} else if (dmaitem.cMode == 'C') {
				// dma copy
				tstart = BenchTimeStart();
				CDmaMemCpy_rtos_async(dmaitem.u32CH, parrDst, parrSrc, u32RelTestSize);
			}

			if (ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
				UINT tgap = BenchTimeStop(tstart);
				total_time += tgap;
				total_size += u32RelTestSize;

				if (dmaitem.cMode == 'B') {
					//portMEMORY_BARRIER();
					//hwflush_dcache_range((ULONG)parrDst, u32RelTestSize);
					//asm volatile ("fence rw,rw");
					//asm volatile ("fence w,r");
				}

				// comp
				flag = 0;
				UINT k;
				for (k = 0; k < u32RelTestSize; k++) {
					if (parrSrc[k] != parrDst[k]) {
						flag = 1;
						break;
					}
				}

				if (flag == 0) {
					pass++;
					static UINT processrate = 0;
					if (processrate != (j * 100 / dmaitem.u32Move)) {
						processrate = j * 100 / dmaitem.u32Move;
						char buf[12] = {0};
						snprintf(buf, 12, "%3u%%", processrate);
						printf("\b\b\b\b");
						printf("%s", buf);
					}
					//printf("OK\n");
					//printf(".");
				} else {
					printf("\b\b\b\b");
					_Rprintf("Fail\nError(%u/%u) Move(%u) Dst(0x%08X)\n", k, u32RelTestSize, j, parrDst);
					//int l = 0;
					//if (k < 64) {
					//	l = 64 - k;
					//}
					//hexCmpDump("Dma Test", parrSrc + k - 64, parrDst + k - 64, u32RelTestSize - k + 64);
					hexCmpDump("Dma Test", parrSrc, parrDst, u32RelTestSize);
					i = j = -0xf;
					fail++;
					break;
				}
			} else {
				flag = 1;
				printf("\b\b\b\b");
				_Rprintf("\nTimeout! memcpy 0x%08X <- 0x%08X, Len:%ubyte\n", parrDst, parrSrc, u32RelTestSize);
				if (dmaitem.cMode == 'C') {
					extern void CDmaRegshow(UINT nCH);
					CDmaRegshow(dmaitem.u32CH);
				}
				i = j = -0xf;
				fail++;
				break;
			}
#endif
			// next
			parrDst++;
		}

		if (flag == 0) {
			char buf[64] = {0};
			snprintf(buf, 64, "%.2f", (total_size / 1024.0 / 1024.0) / (total_time / 1000.0 / 1000.0));
			printf("\b\b\b\b");
			_Gprintf("OK, %sMbyte/s\n", buf);
		} else {
			printf("\b\b\b\b");
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
	if (dmaitem.target == eDMAtest_DDRtoDDR || dmaitem.target == eDMAtest_DDRtoSRAM) {
		vPortFree(dmaitem.arrSrc);
	}
	if (dmaitem.target == eDMAtest_DDRtoDDR || dmaitem.target == eDMAtest_SRAMtoDDR) {
		vPortFree(dmaitem.arrDst);
	}
	dmaitem.arrSrc = NULL;
	dmaitem.arrDst = NULL;
	dmaitem.taskHandle = NULL;
	vTaskDelete(NULL);
}

static ATTR_MALIGN64 BYTE dmaendsrc[32];
static ATTR_MALIGN64 BYTE dmaenddst[64+64];

int cmd_test_dma(int argc, char *argv[])
{
	char cMode = 'B';
	static UINT nCH = 0;
	static int nTestsize = -1; // random

//	nCH = CPU_ID;

	if (strcmp(argv[0], "bdma") == 0) {
		cMode = 'B';
	} else if (strcmp(argv[0], "cdma") == 0) {
		cMode = 'C';
	} else {
		Shell_Unknown();
		return 0;
	}

	if (argc == 1) {
		printf("== %cDMA Register view ==\n", cMode);
		if (cMode == 'B') {
#if EN675_SINGLE
			printf("%-21s: %u\n", "FULL", BDMA0_FULL); // BDMA0_FULL == BDMA1_FULL == BDMA2_FULL == ... == BDMA15_FULL
#endif
			printf("%-21s: %u\n", "W_ENDIAN", BDMA_W_ENDIAN);
			printf("%-21s: %u\n", "R_ENDIAN", BDMA_R_ENDIAN);
		} else {
#if EN675_SINGLE
			printf("%-21s: %u\n", "FULL", CDMA0_FULL); // BDMA0_FULL == BDMA1_FULL == BDMA2_FULL == ... == BDMA15_FULL
#endif
		}
		printf("   |      PROCESS_PTR     |      IRQ       |                       |\n");
		printf("CH | DONVAL | DONE | JOB  | IRQ | EN | CLR | VALUE |  MODE  | BUSY |\n");
		printf("---|---------------------------------------------------------------|\n");
		if (cMode == 'B') {
			for (UINT i = 0; i < BDMA_CNT; i++) {
				printf("%2d |  0x%02X  | 0x%02X | 0x%02X |  %d  | %d  |  %d  | 0x%02X  | %6s |  %d   |\n",
						i, BDmaRegGetDoneVal(i), BDmaRegGetDonePtr(i), BDmaRegGetJobPtr(i), BDmaRegGetIrq(i), BDmaRegGetIrqEn(i), BDmaRegGetIrqClr(i),
						BDmaRegGetValue(i), BDmaRegGetMode(i) == 0 ? "memcpy" : BDmaRegGetMode(i) == 1 ? "memset" : "??????", BDmaRegGetGo(i));
				printf("   | SRC: 0x%08X  DST: 0x%08X  LEN: 0x%08X, %9u  |\n", BDmaRegGetSrc(i), BDmaRegGetDst(i), BDmaRegGetLen(i), BDmaRegGetLen(i), " ");
				if ((i + 1) % 4 == 0) {
					printf("---|---------------------------------------------------------------|\n");
				}
			}
		} else {
			for (UINT i = 0; i < CDMA_CNT; i++) {
				printf("%2d |  0x%02X  | 0x%02X | 0x%02X |  %d  | %d  |  %d  | 0x%02X  | %6s |  %d   |\n",
						i, CDmaRegGetDoneVal(i), CDmaRegGetDonePtr(i), CDmaRegGetJobPtr(i), CDmaRegGetIrq(i), CDmaRegGetIrqEn(i), CDmaRegGetIrqClr(i),
						CDmaRegGetValue(i), CDmaRegGetMode(i) == 0 ? "memcpy" : CDmaRegGetMode(i) == 1 ? "memset" : "??????", CDmaRegGetGo(i));
				printf("   | SRC: 0x%08X  DST: 0x%08X  LEN: 0x%08X, %9u  |\n", CDmaRegGetSrc(i), CDmaRegGetDst(i), CDmaRegGetLen(i), CDmaRegGetLen(i), " ");
				if ((i + 1) % 4 == 0) {
					printf("---|---------------------------------------------------------------|\n");
				}
			}
		}
	} else {
		if (argc == 2) {
			if (strcmp(argv[1], "endtest") == 0) {
				hwflush_dcache_range((ULONG)dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaenddst, sizeof(dmaenddst));

#if 0
				for (int i = 0; i < sizeof(dmaendsrc); i++) {
					dmaendsrc[i] = i;
				}
				for (int i = 0; i < sizeof(dmaenddst); i++) {
					dmaenddst[i] = 0;
				}
				hwflush_dcache_range((ULONG)dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaenddst, sizeof(dmaenddst));
				BDMA_W_ENDIAN = 0;
				BDMA_R_ENDIAN = 0;
				BDmaMemCpy_rtos(0, dmaenddst, dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaenddst, sizeof(dmaenddst));
				hexCmpDump("W-lit, R-lit", dmaendsrc, dmaenddst, sizeof(dmaendsrc));
#endif

#if 0
				for (int i = 0; i < sizeof(dmaendsrc); i++) {
					dmaendsrc[i] = i;
				}
				for (int i = 0; i < sizeof(dmaenddst); i++) {
					dmaenddst[i] = 0;
				}
				hwflush_dcache_range((ULONG)dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaenddst, sizeof(dmaenddst));
				BDMA_W_ENDIAN = 1;
				BDMA_R_ENDIAN = 0;
				BDmaMemCpy_rtos(0, dmaenddst, dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaenddst, sizeof(dmaenddst));
				for (UINT k = 0; k < sizeof(dmaendsrc); k++) {
//					printf("SRC(%2u:0x%02X) DST(%2d:0x%02X)\n", (((k/16)+1)*16)-1-(k%16), dmaendsrc[(((k/16)+1)*16)-1-(k%16)], k, dmaenddst[k]);
					if (dmaendsrc[(((k/16)+1)*16)-1-(k%16)] != dmaenddst[k]) {
						//printf("error[%u]\n", k);
						//hexCmpDump("W-big, R-big", dmaendsrc, dmaenddst, sizeof(dmaendsrc));
						break;
					}
				}
#endif

#if 0
				for (int i = 0; i < sizeof(dmaendsrc); i++) {
					dmaendsrc[i] = i;
				}
				for (int i = 0; i < sizeof(dmaenddst); i++) {
					dmaenddst[i] = 0;
				}
				hwflush_dcache_range((ULONG)dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaenddst, sizeof(dmaenddst));
				BDMA_W_ENDIAN = 0;
				BDMA_R_ENDIAN = 1;
				BDmaMemCpy_rtos(0, dmaenddst + 1, dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaenddst, sizeof(dmaenddst));
				for (UINT k = 0; k < sizeof(dmaendsrc); k++) {
					if (dmaendsrc[k] != dmaenddst[k+1]) {
						printf("error[%u]\n", k);
						hexCmpDump("W-big, R-big", dmaendsrc, dmaenddst + 1, sizeof(dmaendsrc));
						break;
					}
				}
#endif

#if 1
				for (int i = 0; i < sizeof(dmaendsrc); i++) {
					dmaendsrc[i] = i;
				}
				for (int i = 0; i < sizeof(dmaenddst); i++) {
					dmaenddst[i] = 0;
				}
				hwflush_dcache_range((ULONG)dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaenddst, sizeof(dmaenddst));
				BDMA_W_ENDIAN = 1;
				BDMA_R_ENDIAN = 1;
				BDmaMemCpy_rtos(0, dmaenddst+1, dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaendsrc, sizeof(dmaendsrc));
				hwflush_dcache_range((ULONG)dmaenddst, sizeof(dmaenddst));
//				hexCmpDump("W-big, R-big", dmaendsrc, dmaenddst, sizeof(dmaenddst));
				for (UINT k = 0; k < sizeof(dmaendsrc); k++) {
					if (dmaendsrc[k] != dmaenddst[k+1]) {
						printf("error[%u]\n", k);
						hexCmpDump("W-big, R-big", dmaendsrc, dmaenddst + 1, sizeof(dmaendsrc));
						break;
					}
				}
#endif

				printf("done\n");

				BDMA_W_ENDIAN = 0;
				BDMA_R_ENDIAN = 0;

			} else if (strcmp(argv[1], "end") == 0) {
				printf("== BDMA  Register view ==\n");
				printf("%-22s: %u\n", "W_ENDIAN", BDMA_W_ENDIAN);
				printf("%-22s: %u\n", "R_ENDIAN", BDMA_R_ENDIAN);
			} else if (strcmp(argv[1], "man") == 0) {
				UINT testSize = 128;

				BYTE *arrSrc = pvPortMalloc(testSize);
				if (arrSrc == NULL) {
					printf("malloc error(arrSrc), size(%u)\n", testSize);
					return 0;
				}

				BYTE *arrDst = pvPortMalloc(testSize);
				if (arrDst == NULL) {
					printf("malloc error(arrDst), size(%u)\n", testSize);
					vPortFree(arrSrc);
					return 0;
				}

				printf("arrSrc: 0x%08X\n", (UINT)(intptr_t)arrSrc);
				printf("arrDst: 0x%08X\n", (UINT)(intptr_t)arrDst);

				for (int i = 0; i < 64; i++) {
					printf("STEP%u\n", i);

					hwflush_dcache_range((ULONG)arrSrc, testSize);
//					hwflush_dcache_range((ULONG)arrDst, testSize);
					for (int j = 0; j < testSize; j++) {
						arrSrc[j] = j;
						arrDst[j] = 0x00;
					}
					hwflush_dcache_range((ULONG)arrSrc, testSize);
//					hwflush_dcache_range((ULONG)arrDst, testSize);

					CDmaMemCpy_rtos(0, arrDst+i, arrSrc+i, 64);

//					hwflush_dcache_range((ULONG)arrDst, testSize);

					hexDump("arrSrc", arrSrc, testSize);
					hexDump("arrDst", arrDst, testSize);

					printf("================================================\n");
				}

				vPortFree(arrSrc);
				vPortFree(arrDst);
			} else if (strcmp(argv[1], "stop") == 0) {
				dmaitem.u32Close = 1;
			} else {
				Shell_Unknown();
			}
		} else if (argc == 3 || argc == 4) {
			if (strcmp(argv[1], "wend") == 0) {
				UINT setval = atoi(argv[2]);
				BDMA_W_ENDIAN = setval;
				printf("%-22s: %u\n", "W_ENDIAN", BDMA_W_ENDIAN);
			} else if (strcmp(argv[1], "rend") == 0) {
				UINT setval = atoi(argv[2]);
				BDMA_R_ENDIAN = setval;
				printf("%-22s: %u\n", "R_ENDIAN", BDMA_R_ENDIAN);
			} else if (strcmp(argv[1], "ch") == 0) {
				UINT getCH = atoi(argv[2]);
				if (getCH < BDMA_CNT) {
					nCH = getCH;
					printf("Set %cDMA%u, Testsize(%u)\n", cMode, nCH, nTestsize);
				} else {
					Shell_Unknown();
				}
			} else if (strcmp(argv[1], "size") == 0) {
				if (strcmp(argv[1], "rand") == 0) {
					nTestsize = -1;
				} else {
					nTestsize = atoi(argv[2]);
					printf("Set %cDMA%u, Testsize(%u)\n", cMode, nCH, nTestsize);
				}
			} else if (strcmp(argv[1], "start") == 0) {
				if (dmaitem.taskHandle != NULL) {
					printf("%cDMA Test already!\n", cMode);
					return 0;
				}

				DMAtestTarget target = eDMAtest_DDRtoDDR;
				UINT nLoop = atoi(argv[2]);
				if (nLoop > 131072) {
					printf("Max Test Count => 131072\n");
					return 0;
				}
				if (argc == 4) {
					if (strcmp("ddrtoddr", argv[3]) == 0) {
						target = eDMAtest_DDRtoDDR;
					} else if (strcmp("ddrtosram", argv[3]) == 0) {
						target = eDMAtest_DDRtoSRAM;
					} else if (strcmp("sramtoddr", argv[3]) == 0) {
						target = eDMAtest_SRAMtoDDR;
					} else if (strcmp("sramtosram", argv[3]) == 0) {
						target = eDMAtest_SRAMtoSRAM;
					}
				}

				dmaitem.arrDst = NULL;
				dmaitem.arrSrc = NULL;
				dmaitem.u64BufSize = DMA_TEST_MALLOC_SIZE;
				dmaitem.u32TestSize = nTestsize;
				dmaitem.u32Move = nLoop;
				dmaitem.cMode = cMode;
				dmaitem.u32CH = nCH;
				dmaitem.target = target;
				dmaitem.taskHandle = vTaskCreate("DmaT", DmaTestTask, NULL, LV3_STACK_SIZE, LV4_TASK_PRIO);
			} else {
				Shell_Unknown();
			}
		} else if (argc == 5) {
			if (strcmp(argv[1], "man") == 0) {
				ULONG getDst = strtol(argv[2], NULL, 16);
				ULONG getSrc = strtol(argv[3], NULL, 16);
				ULONG getSize = strtol(argv[4], NULL, 16);

				memset((void *)getDst, 0, getSize);
				hwflush_dcache_range((ULONG)getSrc, getSize);
				for (UINT k = 0; k < getSize; k++) {
					((BYTE *)getSrc)[k] = rand();
				}
				hwflush_dcache_range((ULONG)getSrc, getSize);

				// dma copy
				if (cMode == 'B') {
					hwflush_dcache_range((ULONG)getDst, getSize);
					//hwflush_dcache_range_all();
					BDmaMemCpy_isr(nCH, (void *)getDst, (void *)getSrc, getSize);
					//hwflush_dcache_range_all();
					hwflush_dcache_range((ULONG)getDst, getSize);
				} else if (cMode == 'C') {
					CDmaMemCpy_isr(nCH, (void *)getDst, (void *)getSrc, getSize);
				}

				// comp
				UINT k, flag = 0;
				for (k = 0; k < getSize; k++) {
					if (((BYTE *)getSrc)[k] != ((BYTE *)getDst)[k]) {
						flag = 1;
						break;
					}
				}

				if (flag == 0) {
					printf("OK\n");
				} else {
					printf("Fail(%u/%lu)\n", k, getSize);
					hexDump("parrSrc", (BYTE *)getSrc, 16);
					hexDump("parrDst", (BYTE *)getDst, 16);
				}
			} else {
				Shell_Unknown();
			}
		} else {
			Shell_Unknown();
		}
	}
	return 0;
}

static ENX_OKFAIL shellDdrTest(UINT u32Size)
{
#define TEST_SIZE ULONG

	//dma_set(DDR_BASE, DDR_SIZE, 0);
	volatile TEST_SIZE *pDDR = (volatile TEST_SIZE *)(DDR0_BASE + 0x200000);

	printf("DDR Test - Write\n");
	for (pDDR = (volatile TEST_SIZE *)(DDR0_BASE + 0x200000); (intptr_t)pDDR < ((intptr_t)(DDR0_BASE+u32Size));) {
#if 0
		if (((TEST_SIZE)pDDR) % 64 == 0) {
			hwflush_dcache_range((TEST_SIZE)pDDR, 64);
		}
#endif
		//dmwrite32((uint)pDDR, (uint)pDDR);
		*pDDR = (TEST_SIZE)pDDR;

		pDDR++;
#if 0
		if (((TEST_SIZE)pDDR) % (TEST_SIZE)(1024*1024) == 0) {
			if ((((TEST_SIZE)pDDR)-0) % (TEST_SIZE)(16*1024*1024) == 0) {
				printf("DDR Test - Write(0x%08X)...%uMB\n", pDDR, (((TEST_SIZE)pDDR)-DDR_BASE) / 1024 / 1024);
			} else {
				printf("DDR Test - Write(0x%08X)...\n", pDDR);
			}
		}
#endif
	}
	printf("DDR Test - Check\n");
	ULONG ok = 0, fail = 0;
	for (pDDR = (volatile TEST_SIZE *)(DDR0_BASE + 0x200000); (intptr_t)pDDR < ((intptr_t)(DDR0_BASE+u32Size));) {
#if 0
		if (((TEST_SIZE)pDDR) % 64 == 0) {
			hwflush_dcache_range((TEST_SIZE)pDDR, 64);
		}
#endif
		//long getdata = dmread32((uint)pDDR);
		TEST_SIZE getdata = *pDDR;
		if (getdata != (TEST_SIZE)pDDR) {
			//printf("DDR Test - X: 0x%08X 0x%08X\n", pDDR, getdata);
			fail++;
		} else {
			//printf("DDR Test - O: 0x%08X 0x%08X\n", pDDR, getdata);
			ok++;
		}

		pDDR++;
#if 0
		if (((long)pDDR) % (long)(1024*1024) == 0) {
			if ((((long)pDDR)-0) % (long)(16*1024*1024) == 0) {
				printf("DDR Test - Read(0x%08X)...(O:%10lu X:%10lu) %uMB\n", pDDR, ok, fail, (((long)pDDR)-DDR_BASE) / 1024 / 1024);
			} else {
				printf("DDR Test - Read(0x%08X)...(O:%10lu X:%10lu)\n", pDDR, ok, fail);
			}
		}
#endif
	}
	printf("DDR Test - Done(O:%lu X:%lu)\n", ok, fail);
	if (fail > 0) {
		return ENX_FAIL;
	} else {
		return ENX_OK;
	}
}

int cmd_test_memory(int argc, char *argv[])
{
	UINT u32Size = 3 * 1024 * 1024;
	if (argc >= 2) {
		u32Size = atoi(argv[1]) * 1024 * 1024;
		if (u32Size < 3) {
			printf("Min 3MByte\n");
			return 0;
		}
	}
	printf("Memory Test Size: %uMbyte\n", u32Size / 1024 / 1024);
	if (argc == 3) {
		UINT u32Count = atoi(argv[2]), u32FailCount = 0;
		for (UINT i = 0; i < u32Count; i++) {
			ENX_OKFAIL ofRes = shellDdrTest(u32Size);
			if (ofRes == ENX_FAIL) {
				u32FailCount++;
			}
			printf("Memory Test Loop(%u/%u), Fail(%u)\n", i + 1, u32Count, u32FailCount);
			vTaskDelay(1);
		}
		printf("Memory Test Loop Done\n");
	} else {
		shellDdrTest(u32Size);
	}
	return 0;
}

#define mem_cel_time(text, S, E) \
	printf("%-15s: %10lu - %6lums %3luus\n", text, E - S, (E - S) / 500, ((E - S) % 500) * 2);

int cmd_test_memset(int argc, char *argv[])
{
#if 0
	static intptr_t pos = 0;
	BYTE *src_base = (BYTE *)(DDR0_BASE + (16 * 1024 * 1024) + pos);
	BYTE *dst_base = (BYTE *)(DDR0_BASE + (32 * 1024 * 1024) + pos);
	size_t size = 16 * 1024 * 1024;
	if (argc == 2) {
		size = atoi(argv[1]);
	} else if (argc == 3) {
		size = atoi(argv[1]);
		pos = 0;
	}

	for (int i = 0; i < size; i++) {
		src_base[i] = rand();
		dst_base[i] = rand();
	}

	pos += ENX_MEM_ALIGN_SIZE(size);
	printf("Memcpy Speed Test (Base: 0x%08X->0x%08X, Size:%lu)\n", (UINT)(intptr_t)src_base, (UINT)(intptr_t)dst_base, size);

	vTaskEnterCritical();

	ULONG a = *mtime;
	memcpy(dst_base, src_base, size);
	ULONG b = *mtime;
	hwflush_dcache_range((ulong)dst_base, size);
	ULONG b_1 = *mtime;
	hwflush_dcache_range((ulong)src_base, size);
	ULONG b_2 = *mtime;
	BDmaMemCpy_isr(0, dst_base, src_base, size);
	ULONG c = *mtime;
	CDmaMemCpy_isr(0, dst_base, src_base, size);
	ULONG d = *mtime;

	vTaskExitCritical();
#if 0
	printf("memcpy    : %10u - %8ums\n", b - a, (b - a) >= 500 ? (b - a) / 500 : (b - a) * 2);
	printf("BDmaMemCpy: %10u - %8ums\n", c - b, (c - b) >= 500 ? (c - b) / 500 : (c - b) * 2);
	printf(" hwflush 1: %10u - %8ums\n", b_1 - b, (b_1 - b) / 500);
	printf(" dmacpy   : %10u - %8ums\n", b_2 - b_1, (b_2 - b_1) / 500);
	printf(" hwflush 2: %10u - %8ums\n", c - b_2, (c - b_2) / 500);
	printf("CDmaMemCpy: %10u - %8ums\n", d - c, (d - c) / 500);
#endif
	mem_cel_time("memcpy", a, b);
	mem_cel_time("BDmaMemCpy", b, c);
	mem_cel_time(" - hwflush src", b, b_1);
	mem_cel_time(" - hwflush dst", b_1, b_2);
	mem_cel_time(" - dmacpy", b_2, c);
	mem_cel_time("CDmaMemCpy", c, d);

#else
	//BYTE *base = (BYTE *)(DDR1_BASE + (32 * 1024 * 1024));
	BYTE *base = (BYTE *)(DDR1_BASE);
	size_t size = 128 * 1024 * 1024;
	printf("Memset Speed Test (Base: 0x%08X)\n", base);

	vTaskEnterCritical();

	ULONG a = *mtime;
	memset(base, 0x00, size);
	ULONG b = *mtime;
	BDmaMemSet_isr(0, base, 0x00, size);
	ULONG c = *mtime;
	CDmaMemSet_isr(0, base, 0x00, size);
	ULONG d = *mtime;

	vTaskExitCritical();

	printf("memset    : %10u - %8ums\n", b - a, (b - a) / 500);
	printf("BDmaMemSet: %10u - %8ums\n", c - b, (c - b) / 500);
	printf("CDmaMemSet: %10u - %8ums\n", d - c, (d - c) / 500);
#endif
	return 0;
}
