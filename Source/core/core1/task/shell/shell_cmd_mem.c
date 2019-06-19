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
const char *sChecksumTest[] = {"Test Checksum",                                   (char*)0};

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

typedef struct {
	TaskHandle_t taskHandle;

	BYTE *arrSrc;
	BYTE *arrDst;
	ULONG u64BufSize;

	UINT u32TestSize;
	UINT u32Move;
	UINT cMode;
	UINT u32CH;
} DmaTestStr;

DmaTestStr dmaitem;
#define DMA_TEST_MALLOC_SIZE 16*1024
#define DMA_TEST_CH 3

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
	dmaitem.arrSrc = pvPortMalloc(dmaitem.u64BufSize);
	if (dmaitem.arrSrc == NULL) {
		printf("malloc error(arrSrc), size(%lu)\n", dmaitem.u64BufSize);
		dmaitem.taskHandle = NULL;
		vTaskDelete(NULL);
	}

	dmaitem.arrDst = pvPortMalloc(dmaitem.u64BufSize);
	if (dmaitem.arrDst == NULL) {
		printf("malloc error(arrDst), size(%lu)\n", dmaitem.u64BufSize);
		vPortFree(dmaitem.arrDst);
		dmaitem.taskHandle = NULL;
		vTaskDelete(NULL);
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
		parrDst = dmaitem.arrDst;
		if (dmaitem.u32TestSize == -1) {
			u32RelTestSize = rand() % 8192 + 1;
		} else {
			u32RelTestSize = dmaitem.u32TestSize;
		}

		printf("%3u%% %4u/%4u TEST Dst(0x%08X~0x%08X) Src(0x%08X) - Size(%4u) - ", ((i+1) * 100) / dmaitem.u32Move, i+1, dmaitem.u32Move, (UINT)(intptr_t)parrDst, (UINT)(intptr_t)(parrDst+dmaitem.u32Move), (UINT)(intptr_t)parrSrc, u32RelTestSize);


		ULONG total_time = 0;
		ULONG total_size = 0;
		for (UINT j = 0; j < dmaitem.u32Move; j++) {
			// Set array
			memset(parrDst, 0, u32RelTestSize);
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
				BDmaMemCpy_isr_async(dmaitem.u32CH, parrDst, parrSrc, u32RelTestSize);
				//portMEMORY_BARRIER();
//				hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);

			} else if (dmaitem.cMode == 'C') {
				// dma copy
				tstart = BenchTimeStart();
				CDmaMemCpy_isr_async(dmaitem.u32CH, parrDst, parrSrc, u32RelTestSize);
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
					//printf("OK\n");
					//printf(".");
				} else {
					_Rprintf("Fail\nError(%u/%u) Move(%u) Dst(0x%08X)\n", k, u32RelTestSize, j, parrDst);
					hexCmpDump("Dma Test", parrSrc + k - 64, parrDst + k - 64, u32RelTestSize - k + 64);
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
#endif
			// next
			parrDst++;
		}

		if (flag == 0) {
			char buf[64] = {0};
			snprintf(buf, 64, "%.2f", (total_size / 1024.0 / 1024.0) / (total_time / 1000.0 / 1000.0));
			_Gprintf("OK, %sMbyte/s\n", buf);
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
	vPortFree(dmaitem.arrSrc);
	vPortFree(dmaitem.arrDst);
	dmaitem.taskHandle = NULL;
	vTaskDelete(NULL);
}

int cmd_test_dma(int argc, char *argv[])
{
	char cMode = 'B';
	static UINT nCH = 0;
	static int nTestsize = -1; // random

	if (strcmp(argv[0], "bdma") == 0) {
		cMode = 'B';
	} else if (strcmp(argv[0], "cdma") == 0) {
		cMode = 'C';
	} else {
		Shell_Unknown();
		return 0;
	}

	if (argc == 1) {
		Shell_Unknown();
	} else {
		if (argc == 2) {
			if (strcmp(argv[1], "man") == 0) {
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
					for (int i = 0; i < testSize; i++) {
						arrSrc[i] = i;
						arrDst[i] = 0x00;
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
			} else {
				Shell_Unknown();
			}
		} else if (argc == 3) {
			if (strcmp(argv[1], "ch") == 0) {
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
			} else if (strcmp(argv[1], "stop") == 0) {
				if (dmaitem.taskHandle != NULL) {
					printf("%cDMA Test Stop\n", cMode);
					vTaskDelete(dmaitem.taskHandle);
					dmaitem.taskHandle = NULL;
				}
			} else if (strcmp(argv[1], "start") == 0) {
				if (dmaitem.taskHandle != NULL) {
					printf("%cDMA Test already!\n", cMode);
					return 0;
				}

				UINT nLoop = atoi(argv[2]);
				if (nLoop > 8192) {
					printf("Max Test Count => 8192\n");
					return 0;
				}

				dmaitem.arrDst = NULL;
				dmaitem.arrSrc = NULL;
				dmaitem.u64BufSize = DMA_TEST_MALLOC_SIZE;
				dmaitem.u32TestSize = nTestsize;
				dmaitem.u32Move = nLoop;
				dmaitem.cMode = cMode;
				dmaitem.u32CH = nCH;
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
#if 1
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
	pos += ENX_MEM_ALIGN_SIZE(size);
	printf("Memcpy Speed Test (Base: 0x%08X->0x%08X, Size:%lu)\n", (UINT)(intptr_t)src_base, (UINT)(intptr_t)dst_base, size);

	vTaskEnterCritical();

	ULONG a = *mtime;
	memcpy(dst_base, src_base, size);
	ULONG b = *mtime;
	hwflush_dcache_range((ulong)dst_base, size);
	ULONG b_1 = *mtime;
	BDmaMemCpy_isr(0, dst_base, src_base, size);
	ULONG b_2 = *mtime;
	hwflush_dcache_range((ulong)dst_base, size);
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
	mem_cel_time("memcpy",     a, b);
	mem_cel_time("BDmaMemCpy", b, c);
	mem_cel_time(" - hwflush 1", b, b_1);
	mem_cel_time(" - dmacpy",    b_1, b_2);
	mem_cel_time(" - hwflush 2", b_2, c);
	mem_cel_time("CDmaMemCpy", c, d);

#else
	BYTE *base = (BYTE *)(DDR0_BASE + (32 * 1024 * 1024));
	size_t size = 32 * 1024 * 1024;
	printf("Memset Speed Test (Base: 0x%08X)\n", base);

	vTaskEnterCritical();

	ULONG a = *mtime;
	memset(base, 0x12, size);
	ULONG b = *mtime;
	BDmaMemSet_isr(0, base, 0x34, size);
	ULONG c = *mtime;
	CDmaMemSet_isr(0, base, 0x56, size);
	ULONG d = *mtime;

	vTaskExitCritical();

	printf("memset    : %10u - %8ums\n", b - a, (b - a) / 500);
	printf("BDmaMemSet: %10u - %8ums\n", c - b, (c - b) / 500);
	printf("CDmaMemSet: %10u - %8ums\n", d - c, (d - c) / 500);
#endif
	return 0;
}

#define FOLD_U32T(u)          (((u) >> 16) + ((u) & 0x0000ffffUL))
#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)
static WORD lwip_standard_chksum(const void *dataptr, int len)
{
  u8 *pb = (u8 *)dataptr;
  u16 *ps, t = 0;
  u32 sum = 0;
  int odd = ((intptr_t)pb & 1);

  /* Get aligned to u16_t */
  if (odd && len > 0) {
    ((u8 *)&t)[1] = *pb++;
    len--;
  }

  /* Add the bulk of the data */
  ps = (u16 *)(void *)pb;
  while (len > 1) {
    sum += *ps++;
    len -= 2;
  }

  /* Consume left-over byte, if any */
  if (len > 0) {
    ((u8 *)&t)[0] = *(u8 *)ps;
  }

  /* Add end bytes */
  sum += t;

  /* Fold 32-bit sum to 16 bits
     calling this twice is propably faster than if statements... */
  sum = FOLD_U32T(sum);
  sum = FOLD_U32T(sum);

  /* Swap if alignment was odd */
  if (odd) {
    sum = SWAP_BYTES_IN_WORD(sum);
  }

  return (u16)sum;
}

//ATTR_MALIGN64 BYTE arr[] = {	0x45, 0x00, 0x05, 0xd0, 0xd1, 0x93, 0x00, 0x00, 0xff, 0x01,	0x00, 0x00, 0xc0, 0xa9, 0x0a, 0xc3, 0xc0, 0xa9, 0x0a, 0x65};
//ATTR_MALIGN64 BYTE arr[] = {	0x45, 0x00, 0x00, 0x3c, 0xd1, 0x94, 0x00, 0x00, 0x80, 0x01,	0xd2, 0xb1, 0xc0, 0xa9, 0x0a, 0x65, 0xc0, 0xa9, 0x0a, 0xc3};
//ATTR_MALIGN64 BYTE arr[] = {	0x45, 0x00, 0x00, 0x4e, 0x45, 0x2a, 0x00, 0x00, 0x80, 0x11, 0x69, 0xbd, 0xc0, 0xa9, 0x0a, 0x65, 0xc0, 0xa9, 0xff, 0xff};
#if 0
CHKSUM:
0000  45 00 00 4e 4a 6f 00 00 80 11 64 78 c0 a9 0a 65  E..NJo....dx...e
0010  c0 a9 ff ff                                      ....
[CHKSUM:0x8008052E] SW(0xFFFF) HW2(0x55BA) HW(0x12A8) len(  20)
#endif

typedef struct {
	TaskHandle_t taskHandle;

	BYTE *arrSrc;
	BYTE *arrDst;
	UINT u32BufSize;

	UINT u32TestLoop;
} ChksumAutoTestBuffer;

static ChksumAutoTestBuffer chksumitem;

void ChksumTestTask(void *ctx)
{
	if (chksumitem.arrSrc) {
		vPortFree(chksumitem.arrSrc);
	}
	chksumitem.arrSrc = pvPortMalloc(chksumitem.u32BufSize + 64); // +64 -> ENX_MEM_ALIGN
	if (chksumitem.arrSrc == NULL) {
		printf("malloc error(arrSrc), size(%u)\n", chksumitem.u32BufSize);
		chksumitem.taskHandle = NULL;
		vTaskDelete(NULL);
	}
	hwflush_dcache_range((ULONG)chksumitem.arrSrc, chksumitem.u32BufSize);

	if (chksumitem.arrDst) {
		vPortFree(chksumitem.arrDst);
	}
	chksumitem.arrDst = pvPortMalloc(chksumitem.u32BufSize + 64); // +64 -> ENX_MEM_ALIGN
	if (chksumitem.arrDst == NULL) {
		printf("malloc error(arrSrc), size(%u)\n", chksumitem.u32BufSize);
		chksumitem.taskHandle = NULL;
		vTaskDelete(NULL);
	}

	BYTE *parrSrc = (BYTE *)ENX_MEM_ALIGN(chksumitem.arrSrc);
	BYTE *parrDst = (BYTE *)ENX_MEM_ALIGN(chksumitem.arrDst);

	printf("CHKSUM Test Task(Src: 0x%08X)(Dst: 0x%08X)(Buffer Size: %u)\n", (UINT)(intptr_t)parrSrc, (UINT)(intptr_t)parrDst, chksumitem.u32BufSize);

	UINT pass = 0, fail = 0;
	ULONG stime, hwsum, swsum, flag;
	for (UINT i = 1; i <= chksumitem.u32BufSize; i++) {
		hwsum = swsum = flag = 0;
		printf("%3u%% %5u/%5u ", ((i) * 100) / chksumitem.u32BufSize, i, chksumitem.u32BufSize);
		for (UINT j = 0; j < chksumitem.u32TestLoop; j++) {
			for (UINT k = 0; k < i+4; k++) {
				parrSrc[k] = rand();
			}

			stime = BenchTimeStart();
			WORD sw_chksum = lwip_standard_chksum(parrSrc, i);
			swsum += BenchTimeStop(stime);

			stime = BenchTimeStart();
			hwflush_dcache_range((ULONG)parrSrc, i);
			BDmaMemCpy_isr(0, parrDst, parrSrc, i);
			WORD hw_chksum = Checksum16(parrDst, i);
			hwsum += BenchTimeStop(stime);

			if (hw_chksum != sw_chksum) {
				flag = 1;
			}
		}
		printf("HW(sum:%10lu/avg:%5lu) SW(sum:%10lu/avg:%5lu) - ", hwsum, hwsum / chksumitem.u32TestLoop, swsum, swsum / chksumitem.u32TestLoop);
		if (flag == 1) {
			_Rprintf("Fail\n");
			fail++;
		} else {
			_Gprintf("OK\n");
			pass++;
		}
	}

	printf("\nTest End. Pass(%d) Fail(%d)\n", pass, fail);

	chksumitem.taskHandle = NULL;
	vTaskDelete(NULL);
}

int cmd_test_checksum(int argc, char *argv[])
{
#if 1
	if (argc == 1) {

	} else if (strcmp(argv[1], "start") == 0) {
		chksumitem.u32BufSize = 2048;
		chksumitem.u32TestLoop = 1000;
		vTaskCreate("ChksumT", ChksumTestTask, NULL, LV3_STACK_SIZE, LV4_TASK_PRIO);
	}
#else
#if 1
	if (argc == 1) {
		portENTER_CRITICAL();
		hwflush_dcache_range((ULONG)arr, sizeof(arr));
		WORD sw_chksum = lwip_standard_chksum(arr, sizeof(arr));
		WORD hw_chksum = Checksum16(arr, sizeof(arr));
		hwflush_dcache_range((ULONG)arr, sizeof(arr));
		hexDump("arr", arr, sizeof(arr));
		portEXIT_CRITICAL();
		printf("ADDR(0x%08X) SW[0x%04X] HW[0x%04X] Size(%u) ", arr, sw_chksum, hw_chksum, sizeof(arr));
		if (sw_chksum == hw_chksum) {
			printf("OK\n");
		} else {
			printf("Fail\n");
		}

		return 0;
	}
#endif

	UINT u32Len = 100;
	UINT u32Loop = 1;
	UINT u32Ok = 0;
	UINT u32Fail = 0;
	if (argc >= 2) {
		u32Len = atoi(argv[1]);
	}
	if (argc == 3) {
		u32Loop = atoi(argv[2]);
	}

	BYTE *u8Data = pvPortMalloc(ENX_MEM_ALIGN_SIZE(u32Len));
	BYTE *u8CelData = pvPortMalloc(ENX_MEM_ALIGN_SIZE(u32Len));
	ULONG sumSW = 0, sumHW = 0;
	if (u8Data) {
		//BYTE *u8AliData = ENX_MEM_ALIGN(u8Data);
		BYTE *u8AliData = (u8Data);
		printf("Checksum Test (Base: 0x%08X/0x%08X) (Size:%u/%u) (Loop:%u)\n", u8Data, u8AliData, u32Len, ENX_MEM_ALIGN_SIZE(u32Len), u32Loop);
		for (UINT i = 0; i < u32Loop; i++) {
			hwflush_dcache_range((ULONG)u8AliData, u32Len);
			for (int i = 0 ; i < u32Len; i++) {
				u8AliData[i] = rand();
			}
			//hexDump("Rand", u8Data, u32Len);

			ULONG a = *mtime;
			WORD sw_chksum = lwip_standard_chksum(u8AliData, u32Len);
			ULONG b = *mtime;
			hwflush_dcache_range((ULONG)u8AliData, u32Len);
			BDmaMemCpy_isr(0, u8CelData, u8AliData, u32Len);
			WORD hw_chksum = Checksum16(u8CelData, u32Len);
			ULONG c = *mtime;

			sumSW += ((b - a) * 2);
			sumHW += ((c - b) * 2);

			//printf("SW[0x%04X] HW[0x%04X] ", sw_chksum, hw_chksum);
			if (sw_chksum == hw_chksum) {
				//printf("OK\n");
				u32Ok++;
			} else {
				//printf("Fail\n");
				u32Fail++;
			}
		}
		printf("OK(%u) Fail(%u)\n", u32Ok, u32Fail);

		printf("SW sum(%uus) avg(%uus)\n", sumSW, sumSW / u32Loop);
		printf("HW sum(%uus) avg(%uus)\n", sumHW, sumHW / u32Loop);

		vPortFree(u8Data);
	} else {
		printf("Malloc error\n");
	}
#endif

	return 0;
}
