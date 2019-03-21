//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include "enx_freertos.h"

#include "shell_cmd_mem.h"
#include <stdlib.h> // atoi
#include <string.h> // strcmp

const char *sMemoryTest[]   = {"Test Memory",                                     (char*)0};
const char *sDmaTest[]      = {"Test DMA",                                        (char*)0};

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

static void TestDataWrite(BYTE *addr, BYTE startVar, UINT size)
{
	for (UINT i = 0; i < size; i++) {
		addr[i] = startVar++;
	}
}

int cmd_test_dma(int argc, char *argv[])
{
	static char cMode = 'B';
	static UINT nCH = 0;
	static UINT nTestsize = 1024;

	if (argc == 1) {
		Shell_Unknown();
	} else {
		if (argc == 3) {
			if (strcmp(argv[1], "mode") == 0) {
				if (strcmp(argv[2], "b") == 0) {
					cMode = 'B';
					printf("Set %cDMA%u, Testsize(%u)\n", cMode, nCH, nTestsize);
				} else if (strcmp(argv[2], "c") == 0) {
					cMode = 'C';
					printf("Set %cDMA%u, Testsize(%u)\n", cMode, nCH, nTestsize);
				} else {
					Shell_Unknown();
				}
			} else if (strcmp(argv[1], "ch") == 0) {
				UINT getCH = atoi(argv[2]);
				if (getCH < BDMA_CNT) {
					nCH = getCH;
					printf("Set %cDMA%u, Testsize(%u)\n", cMode, nCH, nTestsize);
				} else {
					Shell_Unknown();
				}
			} else if (strcmp(argv[1], "size") == 0) {
				UINT getTestsize = atoi(argv[2]);
				if (getTestsize < 8192) {
					nTestsize = getTestsize;
					printf("Set %cDMA%u, Testsize(%u)\n", cMode, nCH, nTestsize);
				} else {
					Shell_Unknown();
				}
			} else if (strcmp(argv[1], "test") == 0) {
				UINT nLoop = atoi(argv[2]);
				if (nLoop > 8192) {
					printf("Max Test Count => 8192\n");
					return 0;
				}
				printf("%cDMA Test(Count:%u)\n", cMode, nLoop);
				BYTE *arrSrc = (BYTE *)SRAM_BASE;
				BYTE *arrDst = pvPortMalloc(8192);
				BYTE *parrDst = arrDst;
				if (arrDst == NULL) {
					printf("malloc error arrDst\n");
					return 0;
				} else {
					printf("malloc arrDst(0x%08X)\n", arrDst);
				}
				UINT pass = 0, fail = 0, flag = 0;
				for (UINT i = 0; i < nLoop; i++) {
					printf("============================\n");
					parrDst = arrDst;
					for (UINT j = 0; j < nLoop; j++) {
						printf("TEST Dst(0x%08X) Src(0x%08X) - ", parrDst, arrSrc);

						// Set array
						memset(arrDst, 0, 8192);

						// dma copy
						if (cMode == 'B') {
							hwflush_dcache_range((ULONG)parrDst, nTestsize);
							BDmaMemCpy_isr(nCH, parrDst, arrSrc, nTestsize);
							hwflush_dcache_range((ULONG)parrDst, nTestsize);
						} else if (cMode == 'C') {
							CDmaMemCpy_isr(nCH, parrDst, arrSrc, nTestsize);
						} else {
							printf("Error Mode\n");
							vPortFree(arrDst);
							return 0;
						}

						// comp
						flag = 0;
						for (UINT k = 0; k < nTestsize; k++) {
							if (arrSrc[k] != parrDst[k]) {
								flag = 1;
								break;
							}
						}

						if (flag == 0) {
							pass++;
							printf("OK\n");
						} else {
							fail++;
							printf("Fail\n");
							hexDump("arrSrc", arrSrc, 16);
							hexDump("parrDst", parrDst, 16);
						}

						// next
						parrDst++;
					}
					arrSrc++;
				}
				vPortFree(arrDst);
				printf("pass(%d) fail(%d)\n", pass, fail);
			} else {
				Shell_Unknown();
			}
		} else if (argc == 4) {
			//if (strcmp(argv[1], "test") == 0) {
				//BYTE *arrSrc = (BYTE *)strtol(argv[2], NULL, 16);
				//BYTE *arrDst = pvPortMalloc(8192);
			//}
			Shell_Unknown();
		} else {
			Shell_Unknown();
		}
	}
	return 0;
}

static UINT shellDdrTest(UINT u32Size)
{
#define TEST_SIZE ULONG

	//dma_set(DDR_BASE, DDR_SIZE, 0);
	volatile TEST_SIZE *pDDR = (volatile TEST_SIZE *)(DDR_BASE + 0x200000);

	printf("DDR Test - Write\n");
	for (pDDR = (volatile TEST_SIZE *)(DDR_BASE + 0x200000); pDDR < (volatile TEST_SIZE *)(DDR_BASE+u32Size);) {
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
	for (pDDR = (volatile TEST_SIZE *)(DDR_BASE + 0x200000); pDDR < (volatile TEST_SIZE *)(DDR_BASE+u32Size);) {
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
		return DEF_FAIL;
	} else {
		return DEF_OK;
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
			UINT u32Res = shellDdrTest(u32Size);
			if (u32Res == DEF_FAIL) {
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
