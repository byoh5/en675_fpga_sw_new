//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#if defined(__USE_SDIOCD__)

#include "enx_freertos.h"

#include "shell_cmd_sdio.h"
#include <stdio.h>	// for snprintf
#include <string.h> // for memset
#include <stdlib.h> // for srand
#include <string.h> // for strcmp
#include "sdcard.h" // for getSDGBSizeT

const char *sSdioTest[]      = {"Test SDIO",                                      (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
#define SDIODATA_MAX_COUNT 8
typedef ATTR_MALIGN64 struct {
	BYTE *data;
	BYTE *alloc;
} SdioTestBuffer;
static SdioTestBuffer sdcd_data = {NULL, NULL};

typedef ATTR_MALIGN64 struct {
	TaskHandle_t taskHandle;

	BYTE *arrSrc;	// alloc or static memory address
	BYTE *arrDst;	// alloc or static memory address
	ULONG u64BufSize;

	UINT u32TestCount;
	UINT u32TestBlock;
	BYTE u8isAllocDst;
	BYTE u8isAllocSrc;
	BYTE u8Close;
} SdioAutoTestBuffer;

static SdioAutoTestBuffer sdcditem;

static void sdio_create_random(BYTE *data, uint64_t size)
{
	srand(rdcycle());
	for (uint64_t i = 0; i < size; i++) {
		data[i] = i;//rand() % 255;
	}
}

static void SdioCDTestTask(void *ctx)
{
	sdcditem.u8Close = 0;

	UINT u32DataBlockByte = SdioGetDataBlockByte(0);
	if (sdcditem.u64BufSize < u32DataBlockByte) {
		_Rprintf("Buffer size error, size(%u)\n", sdcditem.u64BufSize);
		goto done;
	}

#if 1
	sdcditem.arrSrc = pvPortMalloc(sdcditem.u64BufSize);
	if (sdcditem.arrSrc == NULL) {
		_Rprintf("malloc error(arrSrc), size(%u)\n", sdcditem.u64BufSize);
		goto done;
	}
	sdcditem.u8isAllocSrc = 1;

	sdcditem.arrDst = pvPortMalloc(sdcditem.u64BufSize);
	if (sdcditem.arrDst == NULL) {
		_Rprintf("malloc error(arrDst), size(%u)\n", sdcditem.u64BufSize);
		goto done;
	}
	sdcditem.u8isAllocDst = 1;

#else
	sdcditem.arrSrc = 0xA0000000;
	sdcditem.arrDst = 0xA0010000;
	sdcditem.u64BufSize = 0x10000;
	sdcditem.u8isAllocSrc = 0;
	sdcditem.u8isAllocDst = 0;
#endif

	BYTE *parrSrc = (BYTE *)ENX_MEM_ALIGN(sdcditem.arrSrc);
	BYTE *parrDst = (BYTE *)ENX_MEM_ALIGN(sdcditem.arrDst);

	printf("SDIO-CD Memory Src(0x%08lX->0x%08lX)\n", (intptr_t)sdcditem.arrSrc, (intptr_t)parrSrc);
	printf("SDIO-CD Memory Dst(0x%08lX->0x%08lX)\n", (intptr_t)sdcditem.arrDst, (intptr_t)parrDst);

	unsigned long seed = rdcycle();
	srand(seed);
	//srand(3);

	printf("SDIO-CD Test Task(Src:0x%08lX)(Dst:0x%08lX)(Size:%lu)(Count:%u)(Seed:%lu)\n", (intptr_t)parrSrc, (intptr_t)parrDst, sdcditem.u64BufSize, sdcditem.u32TestCount, seed);
	char strPrintSize[16] = {0};
	UINT u32MaxIdx = SdioCdGetSectorCnt() - 4000;
	getSDGBSizeT(strPrintSize);
	_Cprintf("Info:%s MaxIdx:%u\n", strPrintSize, u32MaxIdx);

	if (u32MaxIdx == 0) {
		_Rprintf("no sd-card?\n");
		goto done;
	}

	UINT u32RelTestSize = 0;
	if (sdcditem.u64BufSize > 16 * 1024 * 1024) {
		u32RelTestSize = 16 * 1024 * 1024;
	} else {
		u32RelTestSize = sdcditem.u64BufSize;
	}
	//hwflush_dcache_range((ULONG)sdcditem.arrSrc, u32RelTestSize);
	for (UINT i = 0; i < sdcditem.u64BufSize; i++) {
		sdcditem.arrSrc[i] = rand() % 255;
	}
	hwflush_dcache_range((ULONG)sdcditem.arrSrc, u32RelTestSize);
#define SDTEST_ALL_AREA 1
#if SDTEST_ALL_AREA
	UINT u32StartSector = 65536;
#else
	UINT u32StartSector = u32MaxIdx - sdcditem.u32TestBlock - 1 - sdcditem.u32TestBlock;
#endif
	UINT u32EndSector = u32MaxIdx - 1 - sdcditem.u32TestBlock;
	printf("Test Sector: 0x%08X ~ 0x%08X\n", u32StartSector, u32EndSector);

	u32RelTestSize = 0;
	UINT pass = 0, fail = 0, flag = 0;
#if 1
	UINT maxCount = sdcditem.u64BufSize / u32DataBlockByte - 1;
#else
	UINT maxCount = 8;
#endif
	char buf[64] = {0};
	for (UINT i = 0; i < sdcditem.u32TestCount; i++) {
		UINT testSector = rand() % ((u32EndSector - u32StartSector) + 1) + u32StartSector;
#if 0
		UINT readCount = 1;								// only single R/W
		//UINT readCount = rand() % (maxCount - 2) + 2;		// only multi R/W
#else
#if 1	// random R/W
		UINT readCount = 0;
		if ((rand() % 5) == 0) {
			readCount = 1;
		} else {
#if SDTEST_ALL_AREA
			readCount = rand() % (maxCount - 2) + 2;
#else
			UINT maxCount = u32EndSector - testSector;
			if (maxCount <= 2) {
				readCount = 1;
			} else {
				readCount = rand() % (maxCount - 2) + 2;
			}
#endif
		}
#else
		UINT readCount = i + 1;
		if ((testSector + readCount) >= u32EndSector) {
			testSector -= readCount;
		}
#endif
#endif

		printf("%3u%% %5u/%5u Test Sector(0x%08X ~ 0x%08X) Count(%8u) %6s- ", ((i+1) * 100) / sdcditem.u32TestCount, i+1, sdcditem.u32TestCount, testSector, testSector+readCount-1, readCount, readCount == 1 ? "single" : "multi");

		ULONG wstart = BenchTimeStart();
		SdioCdWrite(parrSrc, testSector, readCount);
		UINT wgap = BenchTimeStop(wstart);
		snprintf(buf, 64, "%5.2f", ((readCount * u32DataBlockByte) / 1024.0 / 1024.0) / (wgap / 1000.0 / 1000.0));
		printf("W:%sMB/s ", buf);
		//printf("W");

		ULONG rstart = BenchTimeStart();
		SdioCdRead(parrDst, testSector, readCount);
		UINT rgap = BenchTimeStop(rstart);
		snprintf(buf, 64, "%5.2f", ((readCount * u32DataBlockByte) / 1024.0 / 1024.0) / (rgap / 1000.0 / 1000.0));
		printf("R:%sMB/s ", buf);
		//printf("R");

//		vTaskDelay(1);

		//SdioSetTrg(0, 1);
		//printf(".");
		//SdioSetTrg(0, 0);

		// comp
		flag = 0;
		UINT u32Checksize = readCount * u32DataBlockByte;
		UINT k, errSet = 0;
		for (k = 0; k < u32Checksize; k++) {
			if (parrSrc[k] != parrDst[k]) {
				SdioSetTrg(0, 1);
				flag = 1;
				errSet = k / 512;
				SdioSetTrg(0, 0);
				break;
			}
		}

		if (flag == 0) {
			pass++;
			_Gprintf("- OK\n");
//			vTaskDelay(1);
		} else {
			fail++;
			_Rprintf("- Fail\nDet(%u), Error Sector(0x%08X)\n", k, testSector + errSet);
#if 1
			if (readCount > 2 && errSet > 2)
				hexCmpDump("SD-W/R test -2", parrSrc + ((errSet - 2) * 512), parrDst + ((errSet - 2) * 512), 512);
			if (readCount > 1 && errSet > 1)
				hexCmpDump("SD-W/R test -1", parrSrc + ((errSet - 1) * 512), parrDst + ((errSet - 1) * 512), 512);
			hexCmpDump("SD-W/R test  0", parrSrc + (errSet * 512), parrDst + (errSet * 512), 512);
			if (readCount > 1)
				hexCmpDump("SD-W/R test +1", parrSrc + ((errSet + 1) * 512), parrDst + ((errSet + 1) * 512), 512);
			if (readCount > 2)
				hexCmpDump("SD-W/R test +2", parrSrc + ((errSet + 2) * 512), parrDst + ((errSet + 2) * 512), 512);
#endif
			break;
		}

		if (sdcditem.u8Close == 1) {
			break;
		}
	}

	printf("\nTest End. Pass(%d) Fail(%d)\n", pass, fail);

	vTaskInfoPrint();

done:
	if (sdcditem.arrSrc && sdcditem.u8isAllocSrc) {
		vPortFree(sdcditem.arrSrc);
		sdcditem.u8isAllocSrc = 0;
	}
	sdcditem.arrSrc = NULL;
	if (sdcditem.arrDst && sdcditem.u8isAllocDst) {
		vPortFree(sdcditem.arrDst);
		sdcditem.u8isAllocDst = 0;
	}
	sdcditem.arrDst = NULL;
	sdcditem.taskHandle = NULL;
	vTaskDelete(NULL);
}

#if 0
static void SdioCDReadTestTask(void *ctx)
{
	UINT u32DataBlockByte = SdioGetDataBlockByte(0);
	if (sdcditem.u64BufSize < u32DataBlockByte) {
		printf("Buffer size\n");
		goto done;
	}

#if 1
	sdcditem.arrSrc = pvPortMalloc(sdcditem.u64BufSize);
	if (sdcditem.arrSrc == NULL) {
		printf("malloc error(arrSrc), size(%lu)\n", sdcditem.u64BufSize);
		goto done;
	}
	sdcditem.u8isAllocSrc = 1;

	sdcditem.arrDst = pvPortMalloc(sdcditem.u64BufSize);
	if (sdcditem.arrDst == NULL) {
		printf("malloc error(arrDst), size(%lu)\n", sdcditem.u64BufSize);
		goto done;
	}
	sdcditem.u8isAllocDst = 1;
#else
	sdcditem.arrSrc = 0xA0000000;
	sdcditem.arrDst = 0xA0010000;
	sdcditem.u8isAllocSrc = 0;
	sdcditem.u8isAllocDst = 0;
#endif

	BYTE *parrOrg = (BYTE *)ENX_MEM_ALIGN(sdcditem.arrSrc);
	BYTE *parrSdr = (BYTE *)ENX_MEM_ALIGN(sdcditem.arrDst);

	printf("SDIO-CD Read Test Task(Org: 0x%08lX)(SDr: 0x%08lX)(Buffer Size: %lu)\n", (intptr_t)parrOrg, (intptr_t)parrSdr, sdcditem.u64BufSize);
	char strPrintSize[16] = {0};
	UINT u32MaxIdx = SdioCdGetSectorCnt();
	getSDGBSizeT(strPrintSize);
	printf("Info:%s MaxIdx:%u\n", strPrintSize, u32MaxIdx);

	if (u32MaxIdx == 0) {
		printf("no sd-card?\n");
		goto done;
	}

	hwflush_dcache_range((ULONG)parrOrg, sdcditem.u64BufSize);

	// TEST code
	//u32MaxIdx = 10;
	// Test code
	u32MaxIdx = 120000;

	srand(1);

	printf("Read Test Sector: 0x%08X ~ 0x%08X\n", 0, 0 + u32MaxIdx);
	UINT pass = 0, fail = 0;
	for (UINT i = 0; i < sdcditem.u32TestCount; i++) {
		UINT *arr32Org = (UINT *)parrOrg;
		UINT *arr32Sdr = (UINT *)parrSdr;

		UINT readCount = 0;
		if ((rand() % 4) == 0) {
			readCount = 1;
		} else {
			readCount = rand() % (sdcditem.u32TestBlock - 2) + 2;
		}

		UINT startSector = rand() % (u32MaxIdx - readCount);

		printf("%3u%% %5u/%5u Test Sector(0x%08X ~ 0x%08X) Count(%8u) %6s-read - ", ((i+1) * 100) / sdcditem.u32TestCount, i+1, sdcditem.u32TestCount, startSector, startSector+readCount-1, readCount, readCount == 1 ? "single" : "multi");

		// 정답지 만들기
		for (UINT j = 0; j < readCount; j++) {
			for (UINT j2 = 0; j2 < (512/4); j2 += 4) {
				UINT inx = j * (512 / 4) + j2;
				arr32Org[inx+0] = startSector + j;
				arr32Org[inx+1] = 0;
				arr32Org[inx+2] = 0;
				//arr32Org[inx+3] = j2;
				arr32Org[inx+3] = (startSector + j) * 32 + (j2 / 4);
			}
		}
		hwflush_dcache_range((ULONG)parrOrg, readCount*512);

		// SD카드 읽기
		SdioCdRead(parrSdr, startSector, readCount);

		// comp
		UINT k, flag = 0, errSet = 0;
#if 1
		UINT testLen = readCount * (512 / 4);
		for (k = 0; k < testLen; k++) {
			if (arr32Org[k] != arr32Sdr[k]) {
				flag = 1;
				errSet = k / (512 / 4);
				printf("Det!(%u) - ", errSet);
				break;
			}
		}
#else
		for (UINT j = 0; j < readCount; j++) {
			for (UINT j2 = 0; j2 < (512/4); j2++) {
				k = j * (512 / 4) + j2;
				if (arr32Org[k] != arr32Sdr[k]) {
					flag = 1;
					errSet = j;
					printf("Det!(%u) - ", k);
					break;
				}
			}
		}
#endif

		if (flag == 0) {
			pass++;
			_Gprintf("OK\n");
			//printf(".");
		} else {
			fail++;
			_Rprintf("Fail\nError Sector(0x%08X)\n", startSector + errSet);
			//hexDump("parrSrc", parrSrc+k-8, 32);
			//hexDump("parrDst", parrDst+k-8, 32);

			hexCmpDump("SD-read test", parrOrg + (errSet * 512), parrSdr + (errSet * 512), 512);

			_Yprintf("Wait 1sec...hwflush!\n");
			hwflush_dcache_range((ULONG)parrSdr + (errSet * 512), 512);
			vTaskDelay(100);
			_Yprintf("recheck!\n");

			hexCmpDump("SD-read re-test", parrOrg + (errSet * 512), parrSdr + (errSet * 512), 512);

			//hexDump("CPU", parrOrg + (errSet * 512), 512);
			//hexDump("SD read", parrSdr + (errSet * 512), 512);
			break;
		}

		if (sdcditem.u8Close == 1) {
			break;
		}
	}
	printf("Read Done\n");

done:
	if (sdcditem.arrSrc && sdcditem.u8isAllocSrc) {
		vPortFree(sdcditem.arrSrc);
		sdcditem.u8isAllocSrc = 0;
	}
	sdcditem.arrSrc = NULL;
	if (sdcditem.arrDst && sdcditem.u8isAllocDst) {
		vPortFree(sdcditem.arrDst);
		sdcditem.u8isAllocDst = 0;
	}
	sdcditem.arrDst = NULL;
	sdcditem.taskHandle = NULL;
	vTaskDelete(NULL);
}

static void SdioCDWriteTestTask(void *ctx)
{
	UINT u32DataBlockByte = SdioGetDataBlockByte(0);
	if (sdcditem.u64BufSize < u32DataBlockByte) {
		printf("Buffer size\n");
		goto done;
	}

#if 1
	sdcditem.arrSrc = pvPortMalloc(sdcditem.u64BufSize);
	if (sdcditem.arrSrc == NULL) {
		printf("malloc error(arrSrc), size(%lu)\n", sdcditem.u64BufSize);
		goto done;
	}
	sdcditem.u8isAllocSrc = 1;
#else
	sdcditem.arrSrc = 0xA0000000;
	sdcditem.u8isAllocSrc = 0;
#endif
	BYTE *parrSrc = (BYTE *)ENX_MEM_ALIGN(sdcditem.arrSrc);

	printf("SDIO-CD Write Test Task(Base: 0x%08lX)\n", (intptr_t)parrSrc);
	char strPrintSize[16] = {0};
	UINT u32MaxIdx = SdioCdGetSectorCnt();
	getSDGBSizeT(strPrintSize);
	printf("Info:%s MaxIdx:%u\n", strPrintSize, u32MaxIdx);

	if (u32MaxIdx == 0) {
		printf("no sd-card?\n");
		goto done;
	}

	hwflush_dcache_range((ULONG)parrSrc, 512);

	// TEST code
	//u32MaxIdx = 10;
	// Test code

	printf("Write Test Sector: 0x%08X ~ 0x%08X\n", 0, 0 + u32MaxIdx);
	for (UINT i = 0; i < u32MaxIdx; i++) {
		UINT *arr32Src = (UINT *)parrSrc;
		for (UINT j = 0; j < (512/4); j += 4) {
			arr32Src[j+0] = i;
			arr32Src[j+1] = 0;
			arr32Src[j+2] = 0;
			arr32Src[j+3] = i * 32 + (j / 4);
		}
		hwflush_dcache_range((ULONG)parrSrc, 512);

		SdioCdWrite(parrSrc, i, 1);

		if (i % 1000 == 0) {
			printf("%8u/%8u ing... %u%%\n", i, u32MaxIdx, (i * 100) / u32MaxIdx);
		}

		if (sdcditem.u8Close == 1) {
			break;
		}
	}
	printf("Write Done\n");

done:
	if (sdcditem.arrSrc && sdcditem.u8isAllocSrc) {
		vPortFree(sdcditem.arrSrc);
		sdcditem.u8isAllocSrc = 0;
	}
	sdcditem.arrSrc = NULL;
	sdcditem.taskHandle = NULL;
	vTaskDelete(NULL);
}
#endif

int cmd_test_sdio(int argc, char *argv[])
{
	static UINT nBufferSize = 512 * SDIODATA_MAX_COUNT;

	if (sdcd_data.alloc == NULL) {
		sdcd_data.alloc = pvPortMalloc(nBufferSize+512);
		printf("SDIO-CD test memory Init (0x%08lX)\n", (intptr_t)sdcd_data.alloc);
		sdcd_data.data = (BYTE *)ENX_MEM_ALIGN(sdcd_data.alloc);
		printf("SDIO-CD test memory ALIGN64 address (0x%08lX)\n", (intptr_t)sdcd_data.data);
	}

	if(argc == 1) {
		SdioRegShow(ENX_YES);
	} else {
		if (strcmp(argv[1], "test") == 0) {
			extern void SdioRegTest(UINT nCH);
			SdioRegTest(0);
		} else if (strcmp(argv[1], "clk") == 0) {
			if (argc == 2) {
				SdioCdClockRestore();
			} else if (argc == 3) {
				WORD Clkdiv = atoi(argv[2]);
				SdioCdClockSet(Clkdiv);
			} else {
				Shell_Unknown();
			}
		} else if (strcmp(argv[1], "block") == 0) {
			if (strcmp(argv[2], "size") == 0) {
				if (argc == 3) {
					UINT nTemp = SdioGetDataBlockByte(0);
					printf("SDIO Get Data Block Size(%u)\n", nTemp);
				} else if (argc == 4) {
					UINT nSize = atoi(argv[3]);
					printf("SDIO Set Data Block Size(%u)\n", nSize);
					SdioSetDataBlockByte(0, nSize);	// Data Block Byte를 설정
					nSize = SdioGetDataBlockByte(0);
					printf("SDIO Set Data Block Size(%u) OK\n", nSize);
				} else {
					Shell_Unknown();
				}
			} else {
				Shell_Unknown();
			}
		} else if (strcmp(argv[1], "r") == 0 || strcmp(argv[1], "w") == 0) {
			const char *strMode[8] = {"Read", "Write"};
			UINT nMode = strcmp(argv[1], "r") == 0 ? 0 : 1; // 0:Read 1:Write
			if (argc == 5) {
				UINT saddr = strtol(argv[3], NULL, 16);
				UINT eaddr = strtol(argv[4], NULL, 16);
				if (strcmp(argv[2], "s") == 0) {
					for (UINT i = saddr; i <= eaddr; i++) {
						printf("SDIO-CD %s[S] Addr(0x%08X) Dst(0x%08lX)\n", strMode[nMode], i, (intptr_t)sdcd_data.data);
						if (nMode == 0) { // Read
							memset(sdcd_data.data, 0, nBufferSize);
							SdioCdRead(sdcd_data.data, i, 1);
							hexDump("SDIO-CD Read[S]", sdcd_data.data, 512);
						} else { // Write
							// create random data
							sdio_create_random(sdcd_data.data, 512);
							// sdio write
							SdioCdWrite(sdcd_data.data, i, 1);
						}
					}
				} else if (strcmp(argv[2], "m") == 0) {
					UINT nCount = eaddr - saddr + 1;
					if (nCount > SDIODATA_MAX_COUNT) {
						printf("Max Count 8, input(0x%08X/0x%08X=>%d)\n", saddr, eaddr, nCount);
						return 0;
					}
					printf("SDIO-CD %s[M] Addr(0x%08X~0x%08X) Dst(0x%08lX)\n", strMode[nMode], saddr, eaddr, (intptr_t)sdcd_data.data);
					if (nMode == 0) { // Read
						memset(sdcd_data.data, 0, nBufferSize);
						SdioCdRead(sdcd_data.data, saddr, nCount);
						for (UINT i = 0; i < nCount; i++) {
							hexDump("SDIO-CD Read[M]", sdcd_data.data + (i * 512), 512);
						}
					} else { // Write
						sdio_create_random(sdcd_data.data, 512 * nCount);
						for (UINT i = 0; i < nCount; i++) {
							hexDump("SDIO-CD Write[M]", sdcd_data.data + (i * 512), 512);
						}
						SdioCdWrite(sdcd_data.data, saddr, nCount);
					}
				} else {
					Shell_Unknown();
				}
			} else if (argc == 3) {
				UINT saddr = strtol(argv[2], NULL, 16);
				printf("SDIO-CD %s[S] Addr(0x%08X) Dst(0x%08lX)\n", strMode[nMode], saddr, (intptr_t)sdcd_data.data);
				if (nMode == 0) { // Read
					memset(sdcd_data.data, 0, nBufferSize);
					SdioCdRead(sdcd_data.data, saddr, 1);
					hexDump("SDIO-CD Read[S]", sdcd_data.data, 512);
				} else { // Write
					sdio_create_random(sdcd_data.data, 512);
					SdioCdWrite(sdcd_data.data, saddr, 1);
				}
			} else {
				Shell_Unknown();
			}
		} else if (strcmp(argv[1], "start") == 0) {
			if (sdcditem.taskHandle != NULL) {
				printf("SDIO Auto Test already!\n");
				return 0;
			}

			UINT u32TestBlock = 1024;
			UINT nLoop = 100;
			if (argc == 3) {
				nLoop = atoi(argv[2]);
			}
			if (argc == 4) {
				u32TestBlock = atoi(argv[3]);
				if (u32TestBlock > 1024) {
					printf("Max Test Block => 1024 / (input:%u)\n", u32TestBlock);
					return 0;
				}
			}

			sdcditem.u8Close = 0;
			sdcditem.u64BufSize = 512 * u32TestBlock + 512;
			sdcditem.u32TestCount = nLoop; // 반복 횟수 자체
			sdcditem.u32TestBlock = u32TestBlock; // 한번에 Write/Read할 단위 1(512B) ~ 1024(512KB)
			sdcditem.taskHandle = vTaskCreate("SdcdT", SdioCDTestTask, NULL, LV3_STACK_SIZE, LV4_TASK_PRIO);
#if 0
		} else if (strcmp(argv[1], "ws") == 0) {
			if (sdcditem.taskHandle != NULL) {
				printf("SDIO Auto Test already!\n");
				return 0;
			}

			sdcditem.u8Close = 0;
			sdcditem.u64BufSize = 512 * 2;
			sdcditem.u32TestCount = 0;
			sdcditem.u32TestBlock = 0;
			sdcditem.taskHandle = vTaskCreate("SdcdWT", SdioCDWriteTestTask, NULL, LV3_STACK_SIZE, LV4_TASK_PRIO);
		} else if (strcmp(argv[1], "rs") == 0) {
			if (sdcditem.taskHandle != NULL) {
				printf("SDIO Auto Test already!\n");
				return 0;
			}

			UINT u32TestBlock = 1024;
			UINT nLoop = 100;
			if (argc == 3) {
				nLoop = atoi(argv[2]);
			}
			if (argc == 4) {
				u32TestBlock = atoi(argv[3]);
				if (u32TestBlock > 2048) {
					printf("Max Test Block => 2048 / (input:%u)\n", u32TestBlock);
					return 0;
				}
			}

			sdcditem.u8Close = 0;
			sdcditem.u64BufSize = 512 * u32TestBlock + 512;
			sdcditem.u32TestCount = nLoop; // 반복 횟수 자체
			sdcditem.u32TestBlock = u32TestBlock; // 한번에 Read할 단위 1(512B) ~ 2048(1024KB)
			sdcditem.taskHandle = vTaskCreate("SdcdRT", SdioCDReadTestTask, NULL, LV3_STACK_SIZE, LV4_TASK_PRIO);
#endif
		} else if (strcmp(argv[1], "stop") == 0) {
			sdcditem.u8Close = 1;
		} else {
			Shell_Unknown();
		}
	}
	return 0;
}
#endif
