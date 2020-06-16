//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include <string.h> // strcmp
#include <stdlib.h> // atoi

#include "enx_freertos.h"

#include "shell_cmd_util.h"

#include "aes.h" // minilib
#include "sha.h" // minilib

const char *sAesTest[]      = {"Test AES128/256",                                (char*)0};
const char *sShaTest[]      = {"Test SHA244/256",                                (char*)0};
const char *sChecksumTest[] = {"Test Checksum",                                  (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
// Checksum
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

#define CHKSUM_TEST_IRQ 1

typedef struct {
	TaskHandle_t taskHandle;

	BYTE *arrSrc;
	UINT u32BufSize;

	UINT u32TestLoop;
	UINT u32Move;
	UINT u32Close;
} ChksumAutoTestBuffer;

static ChksumAutoTestBuffer chksumitem;

#if CHKSUM_TEST_IRQ
static void ChksumTestIrq(void *ctx)
{
	if (chksumitem.taskHandle) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(chksumitem.taskHandle, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;
		}
	}
}
#endif

void ChksumTestTask(void *ctx)
{
	chksumitem.u32Close = 0;

	chksumitem.arrSrc = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(chksumitem.u32BufSize*2));
	if (chksumitem.arrSrc == NULL) {
		printf("malloc error(arrSrc), size(%u)\n", chksumitem.u32BufSize);
		goto done;
	}
	hwflush_dcache_range((ULONG)chksumitem.arrSrc, chksumitem.u32BufSize);

	BYTE *parrSrc = (BYTE *)ENX_MEM_ALIGN(chksumitem.arrSrc);

#if CHKSUM_TEST_IRQ
	ChksumIrqCallback(ChksumTestIrq, NULL);
	ChksumSetIrqEn(ENX_ON);
#endif

	printf("CHECKSUM Test Task(Start address: 0x%08X~0x%08X)(Buffer Size: %u)(Move: %u)(Loop: %u)\n", (UINT)(intptr_t)parrSrc, ((UINT)(intptr_t)parrSrc) + chksumitem.u32Move, chksumitem.u32BufSize, chksumitem.u32Move, chksumitem.u32TestLoop);

	UINT pass = 0, fail = 0;
	ULONG stime, hwsum, swsum, flag = 0;
	for (UINT i = 1; i <= chksumitem.u32BufSize; i++) {
		if (flag | (chksumitem.u32Close == 1)) {
			break;
		}
		UINT j, m, celcnt = 0;
		hwsum = swsum = flag = 0;
		printf("%3u%% %5u/%5u ", ((i) * 100) / chksumitem.u32BufSize, i, chksumitem.u32BufSize);
		printf("HW(avg:%3lu) SW(avg:%4lu) -     ", 0, 0);
		for (m = 0; m < chksumitem.u32Move; m++) {
			if (flag | (chksumitem.u32Close == 1)) {
				break;
			}
			BYTE *parrNewSrc = parrSrc + m;
			for (j = 0; j < chksumitem.u32TestLoop; j++) {
				if (flag | (chksumitem.u32Close == 1)) {
					break;
				}
				for (UINT l = 0; l < i+4; l++) {
					parrNewSrc[l] = rand();
				}

				stime = BenchTimeStart();
				WORD sw_chksum = lwip_standard_chksum(parrNewSrc, i);
				swsum += BenchTimeStop(stime);

				stime = BenchTimeStart();
				hwflush_dcache_range((ULONG)parrNewSrc, i);
#if CHKSUM_TEST_IRQ
				Checksum16_rtos_async(parrNewSrc, i);
#else
				WORD hw_chksum = Checksum16_rtos(parrNewSrc, i);
#endif

#if CHKSUM_TEST_IRQ
				if (ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
					WORD hw_chksum = ChksumGetDat();
#endif
					hwsum += BenchTimeStop(stime);

					if (hw_chksum != sw_chksum) {
						flag = 1;
					}

					static UINT processrate = 0;
					if (processrate != (j * 100 / chksumitem.u32TestLoop)) {
						processrate = j * 100 / chksumitem.u32TestLoop;
						char buf[8] = {0};
						snprintf(buf, 8, "%3u%%", processrate);
						printf("\b\b\b\b");
						printf("%s", buf);
					}
#if CHKSUM_TEST_IRQ
				} else {
					flag = 2;
				}
#endif
				celcnt++;
			}
		}
		printf("\r%3u%% %5u/%5u ", ((i) * 100) / chksumitem.u32BufSize, i, chksumitem.u32BufSize);
		printf("HW(avg:%3lu) SW(avg:%4lu) - ", hwsum / celcnt, swsum / celcnt);
		if (flag == 1) {
			_Rprintf("Fail\n");
			fail++;
#if CHKSUM_TEST_IRQ
		} else if (flag == 2) {
			_Rprintf("Timeout\n");
			fail++;
#endif
		} else {
			char buf[32] = {0};
			ULONG hwavg = hwsum / celcnt;
			ULONG swavg = swsum / celcnt;
			snprintf(buf, 32, "x%.2f", (float)swavg / (float)hwavg);
			_Gprintf(" OK ");
			printf("%s\n", buf);
			pass++;
		}
	}

	printf("\nTest End. Pass(%d) Fail(%d)\n", pass, fail);

done:
#if CHKSUM_TEST_IRQ
	ChksumSetIrqEn(ENX_OFF);
	ChksumIrqCallback(NULL, NULL);
#endif
	if (chksumitem.arrSrc) {
		vPortFree(chksumitem.arrSrc);
	}
	chksumitem.arrSrc = NULL;

	chksumitem.taskHandle = NULL;
	vTaskDelete(NULL);
}

int cmd_test_checksum(int argc, char *argv[])
{
#if 1
	if (argc == 1) {
		printf("CHECKSUM test command\n");
		printf("- %s reg   : view register\n", argv[0]);
		printf("- %s start : auto test start\n", argv[0]);
		printf("- %s stop  : auto test stop\n", argv[0]);
	} else if (strcmp(argv[1], "reg") == 0) {
		ChksumRegShow(ENX_YES);
	} else if (strcmp(argv[1], "start") == 0) {
		if (chksumitem.taskHandle != NULL) {
			printf("CHKSUM Auto Test already!\n");
			return 0;
		}

#if EN675_SINGLE
		UINT u32Move = 16;
#else
		UINT u32Move = 1;
#endif
		UINT u32TestLoop = 10;
		if (argc == 3) {
			u32TestLoop = atoi(argv[2]);
		}
		chksumitem.u32BufSize = 2048;
		chksumitem.u32Move = u32Move;
		chksumitem.u32TestLoop = u32TestLoop;
		chksumitem.taskHandle = vTaskCreate("ChksumT", ChksumTestTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
	} else if (strcmp(argv[1], "stop") == 0) {
		chksumitem.u32Close = 1;
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

//-------------------------------------------------------------------------------------------------
// SHA
#define SHA_TEST_IRQ 1

typedef struct {
	TaskHandle_t taskHandle;
	SHAmode mode;

	BYTE *arrSrc;
	BYTE *arrALIGNSrc;
	BYTE arrSwHash[32];
	BYTE arrHwHash[32];
	UINT u32BufSize;

	UINT u32Move;
	UINT u32Close;
} ShaAutoTestBuffer;

static ShaAutoTestBuffer shaitem;
#define SHA_TEST_MALLOC_SIZE	(512*1024)
#define SHA_TEST_TEST_SIZE		(SHA_TEST_MALLOC_SIZE>>1)

#if SHA_TEST_IRQ
static void ShaTestIrq(void *ctx)
{
	if (shaitem.taskHandle) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(shaitem.taskHandle, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;
		}
	}
}
#endif

UINT ShaTestSWProcess(UINT testSize)
{
	ULONG stime;
	BYTE *parrSrc = (BYTE *)ENX_MEM_ALIGN(shaitem.arrSrc);

	memset(shaitem.arrSwHash, 0, sizeof(shaitem.arrSwHash));

	stime = BenchTimeStart();
	if (shaitem.mode == eSHA256) {
		SHA256(parrSrc, testSize, shaitem.arrSwHash);	// SW sha
	} else {
		SHA224(parrSrc, testSize, shaitem.arrSwHash);	// SW sha
	}
	return BenchTimeStop(stime);
}

void ShaTestTask(void *ctx)
{
	shaitem.u32Close = 0;

	shaitem.arrSrc = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(shaitem.u32BufSize));
	if (shaitem.arrSrc == NULL) {
		printf("malloc error(arrSrc), size(%u)\n", shaitem.u32BufSize);
		goto done;
	}
	hwflush_dcache_range((ULONG)shaitem.arrSrc, ENX_MEM_ALIGN_BUFFER(shaitem.u32BufSize));

	shaitem.arrALIGNSrc = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(shaitem.u32BufSize));
	if (shaitem.arrALIGNSrc == NULL) {
		printf("malloc error(arrALIGNSrc), size(%u)\n", shaitem.u32BufSize);
		goto done;
	}

	BYTE *parrSrc = (BYTE *)ENX_MEM_ALIGN(shaitem.arrSrc);
	BYTE *parrHwSrc = (BYTE *)ENX_MEM_ALIGN(shaitem.arrALIGNSrc);

#if SHA_TEST_IRQ
	ShaShotIrqCallback(ShaTestIrq, NULL);
	ShaSetShotIrqEn(ENX_ON);
#endif

	printf("SHA%u Test Task(Buffer Size: %u)(Loop: %u)\n",
			shaitem.mode == eSHA256 ? 256 : 224, shaitem.u32BufSize, shaitem.u32Move);
	printf("              S/W (SwSource: 0x%08X%11s)\n", (UINT)(intptr_t)parrSrc, " ");
	printf("              H/W (HwSource: 0x%08X~0x%08X)\n", (UINT)(intptr_t)parrHwSrc, ((UINT)(intptr_t)parrHwSrc) + shaitem.u32Move);

	hwflush_dcache_range_rtos((ULONG)parrSrc, shaitem.u32BufSize);
	for (UINT i = 0; i < shaitem.u32BufSize; i++) {
		parrSrc[i] = rand() % 255;
	}
	hwflush_dcache_range_rtos((ULONG)parrSrc, shaitem.u32BufSize);

	UINT pass = 0, fail = 0;
	ULONG stime, hwsum, swsum, flag = 0, celcnt;
	for (UINT testSize = 1; testSize < SHA_TEST_TEST_SIZE; testSize++) {
		hwsum = celcnt = 0;

		if (flag | (shaitem.u32Close == 1)) {
			break;
		}

		printf("%3u%% %5u/%5u ", (testSize * 100) / SHA_TEST_TEST_SIZE, testSize, SHA_TEST_TEST_SIZE);
		printf("HW(avg:%5lu) SW(%8lu) -     ", 0, 0);

		swsum = ShaTestSWProcess(testSize);

		printf("\r%3u%% %5u/%5u ", (testSize * 100) / SHA_TEST_TEST_SIZE, testSize, SHA_TEST_TEST_SIZE);
		printf("HW(avg:%5lu) SW(%8lu) -     ", 0, swsum);

		for (UINT i = 0; i < shaitem.u32Move; i++) {
#if EN675_SINGLE
			BYTE *parrNewHwSource = parrHwSrc + i; // new rtl (1byte align)
#else
			BYTE *parrNewHwSource = parrHwSrc + 0; // old rtl (16byte align)
#endif

			if (flag | (shaitem.u32Close == 1)) {
				break;
			}

			stime = BenchTimeStart();
			hwflush_dcache_range((ULONG)parrNewHwSource, testSize);
			BDmaMemSet_rtos(0, parrHwSrc, 0x00, testSize + shaitem.u32Move * 2); // size: or (testSize + shaitem.u32Move)
			BDmaMemCpy_rtos(0, parrNewHwSource, parrSrc, testSize);
#if SHA_TEST_IRQ
			ShaShotCalc_rtos_async(parrNewHwSource, testSize, shaitem.mode);	// HW sha
#else
			ShaShotCalc_rtos(parrNewHwSource, testSize, shaitem.mode);	// HW sha
#endif

#if SHA_TEST_IRQ
			if (ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
#endif
				ShaGetDigest(shaitem.arrHwHash, shaitem.mode);
				hwsum += BenchTimeStop(stime);
				celcnt++;

				for (int lop = 0; lop < sizeof(shaitem.arrHwHash); lop++) {
					if (shaitem.arrSwHash[lop] != shaitem.arrHwHash[lop]) {
						flag = 1;
						break;
					}
				}

				static UINT processrate = 0;
				if (processrate != (i * 100 / shaitem.u32Move)) {
					processrate = i * 100 / shaitem.u32Move;
					char buf[8] = {0};
					snprintf(buf, 8, "%3u%%", processrate);
					printf("\b\b\b\b");
					printf("%s", buf);
				}
#if SHA_TEST_IRQ
			} else {
				flag = 2;
			}
#endif
		}

		printf("\r%3u%% %5u/%5u ", (testSize * 100) / SHA_TEST_TEST_SIZE, testSize, SHA_TEST_TEST_SIZE);
		printf("HW(avg:%5lu) SW(%8lu) - ", hwsum / celcnt, swsum);
		if (flag == 0) {
			char buf[32] = {0};
			ULONG hwavg = hwsum / celcnt;
			ULONG swavg = swsum;
			snprintf(buf, 32, "x%.2f", (float)swavg / (float)hwavg);
			_Gprintf(" OK ");
			printf("%s\n", buf);
			pass++;
		} else {
			if (flag == 1) {
				_Rprintf("Fail\n");
#if SHA_TEST_IRQ
			} else if (flag == 2) {
				_Rprintf("Timeout\n");
#endif
			}
			fail++;
			break;
		}
	}

	printf("\nTest End. Pass(%d) Fail(%d)\n", pass, fail);

done:
#if SHA_TEST_IRQ
	ShaSetShotIrqEn(ENX_OFF);
	ShaShotIrqCallback(NULL, NULL);
#endif
	if (shaitem.arrSrc) {
		vPortFree(shaitem.arrSrc);
	}
	if (shaitem.arrALIGNSrc) {
		vPortFree(shaitem.arrALIGNSrc);
	}
	shaitem.arrSrc = NULL;
	shaitem.arrALIGNSrc = NULL;
	shaitem.taskHandle = NULL;
	vTaskDelete(NULL);
}

#if EN675_SINGLE
void ShaChopTestTask(void *ctx)
{
	shaitem.u32Close = 0;

	shaitem.arrSrc = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(shaitem.u32BufSize));
	if (shaitem.arrSrc == NULL) {
		printf("malloc error(arrSrc), size(%u)\n", shaitem.u32BufSize);
		goto done;
	}

	BYTE *parrSrc = (BYTE *)ENX_MEM_ALIGN(shaitem.arrSrc);

#if SHA_TEST_IRQ
	ShaShotIrqCallback(ShaTestIrq, NULL);
	ShaSetShotIrqEn(ENX_ON);
	ShaChopIrqCallback(ShaTestIrq, NULL);
	ShaSetChopIrqEn(ENX_ON);
#endif

	printf("SHA%u Chop Test Task(Buffer Size: %u)(Maxlength: %u)\n",
			shaitem.mode == eSHA256 ? 256 : 224, shaitem.u32BufSize, shaitem.u32Move);
	printf("              H/W (Source: 0x%08X~0x%08X)\n", (UINT)(intptr_t)parrSrc, ((UINT)(intptr_t)parrSrc) + shaitem.u32Move);

	hwflush_dcache_range_rtos((ULONG)parrSrc, shaitem.u32BufSize);
	for (UINT i = 0; i < shaitem.u32BufSize; i++) {
		parrSrc[i] = rand() % 255;
	}
	hwflush_dcache_range_rtos((ULONG)parrSrc, shaitem.u32BufSize);


	// 1블록은 64byte
	// 전체 연산을 위해서 데이터 크기의 최소 크기는 65byte, 이 경우 64(init)+1(final) 형태만 가능
	// init과 update는 최소 1블록(64byte)을 넣어줘야 하고 이것의 배수여야 한다.

	// rand 함수를 돌려 아래 case 중 하나를 고른다. 7:3 비율 정도?
	// case0: init -> update -> final
	// case1: init -> final

	// rand 함수 3회(case0), 2회(case1)를 돌린다.
	// 설명은 case0을 기준으로..
	// 첫번쨰 rand결과는 init단계에서 돌릴 length -> 64byte단위로 align
	// 두번째 rand결과는 update단계에서 돌릴 length -> 64byte단위로 align
	// 세번째 rand결과는 final단계에서 돌릴 length -> 여기는 1부터 크기를 설정 가능함

	UINT pass = 0;
	UINT fail = 0;

	// max size 설정 (ex: 4096)
	// case: 64, 1~4032
	// case: 4032, 64
	// case: 64, 64, 1~3968
	SHAChopitem item;
	BYTE arrHwChop[32];
	UINT casecnt = 0, flag = 0;
	for (UINT iS = 64; iS < shaitem.u32Move; iS += 64) {
		if (flag | (shaitem.u32Close == 1)) {
			break;
		}

		for (UINT uS = 0; (iS+uS) < shaitem.u32Move; uS += 64) {
			if (flag | (shaitem.u32Close == 1)) {
				break;
			}

			UINT fS = 0;
			for (fS = 1; (iS+uS+fS) <= shaitem.u32Move; fS++) {
				if (flag | (shaitem.u32Close == 1)) {
					break;
				}

				memset(shaitem.arrHwHash, 0xff, 32);
				memset(arrHwChop, 0, 32);

				// One-shot sha test
#if SHA_TEST_IRQ
				ShaShotCalc_rtos_async(parrSrc, iS + uS + fS, shaitem.mode);
				if (!ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
					flag = 2;
					break;
				} else
#else
					ShaShotCalc_rtos(parrSrc, iS + uS + fS, shaitem.mode);
#endif
				{
					ShaGetDigest(shaitem.arrHwHash, shaitem.mode);
				}

				if (flag == 0) {
					// Chop sha test
					item.base = parrSrc;
					item.cmd = eSHACmdInit;
					item.mode = shaitem.mode;
#if SHA_TEST_IRQ
					ShaChopCalc_rtos_async(&item, iS);
					if (!ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
						flag = 3;
						break;
					}
#else
					ShaChopCalc_rtos(&item, iS);
#endif

					if (uS) {
						UINT u32Updatesize = 0;
						UINT u32TotBlock = 0;
						UINT u32ReqBlock = 0;

						u32TotBlock = uS / 64;
						while (u32TotBlock) {
							u32ReqBlock = (rand() % u32TotBlock) + 1;
							u32TotBlock -= u32ReqBlock;

							item.base = parrSrc + iS + u32Updatesize;
							item.cmd = eSHACmdUpdate;
#if SHA_TEST_IRQ
							ShaChopCalc_rtos_async(&item, u32ReqBlock * 64);
							if (!ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
								flag = 4;
								break;
							}
#else
							ShaChopCalc_rtos(&item, u32ReqBlock * 64);
#endif

							u32Updatesize += (u32ReqBlock * 64);
						}
						if (flag) {
							break;
						}
					}

					item.base = parrSrc + iS + uS;
					item.cmd = eSHACmdFinal;

#if SHA_TEST_IRQ
					ShaChopCalc_rtos_async(&item, fS);
					if (!ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
						flag = 5;
						break;
					}
#else
					ShaChopCalc_rtos(&item, fS);
#endif

					ShaGetDigest(arrHwChop, shaitem.mode);

					// Compare
					for (int lop = 0; lop < sizeof(shaitem.arrHwHash); lop++) {
						if (arrHwChop[lop] != shaitem.arrHwHash[lop]) {
							flag = 1;
							hexDump("Source", parrSrc, iS + uS + fS);
							hexCmpDump("SHA error", shaitem.arrHwHash, arrHwChop, 32);
							break;
						}
					}
				}
				casecnt++;
			}

			printf("%8u init(%6u) update(%6u) final(1~%6u) : ", casecnt, iS, uS, shaitem.u32Move - uS - iS);
			if (flag == 0) {
				_Gprintf(" OK\n");
			} else if (flag == 1) {
				_Rprintf("Fail\n");
#if SHA_TEST_IRQ
			} else if (flag == 2) {
				_Rprintf("One-Shot Timeout\n");
			} else if (flag == 3) {
				_Rprintf("Chop(init) Timeout\n");
			} else if (flag == 4) {
				_Rprintf("Chop(update) Timeout\n");
			} else if (flag == 5) {
				_Rprintf("Chop(final) Timeout\n");
#endif
			}
		}
	}
	printf("\nTest(count:%u) End\n", casecnt);

done:
#if SHA_TEST_IRQ
	ShaSetChopIrqEn(ENX_OFF);
	ShaChopIrqCallback(NULL, NULL);
	ShaSetShotIrqEn(ENX_OFF);
	ShaShotIrqCallback(NULL, NULL);
#endif
	if (shaitem.arrSrc) {
		vPortFree(shaitem.arrSrc);
	}
	shaitem.arrSrc = NULL;
	shaitem.taskHandle = NULL;
	vTaskDelete(NULL);
}
#endif

#define SHA_TEST_SIZE (512)
ATTR_MALIGN64 static BYTE arrShaSrcC1[SHA_TEST_SIZE] = {0};
ATTR_MALIGN64 static BYTE arrShaSrcC2[SHA_TEST_SIZE] = {0};
int cmd_test_sha(int argc, char *argv[])
{
	SHAmode mode;
	size_t output_size;
	if (strcmp(argv[0], "sha256") == 0) {
		mode = eSHA256;
		output_size = 256 / 8;
	} else if (strcmp(argv[0], "sha224") == 0) {
		mode = eSHA224;
		output_size = 224 / 8;
	} else {
		printf("Error, %s mode\n", argv[0]);
		return 0;
	}

	if (argc == 1) {
		printf("SHA test command\n");
		printf("- %s reg   : view register\n", argv[0]);
		printf("- %s start : auto test start (one-shot)\n", argv[0]);
#if EN675_SINGLE
		printf("- %s start2: auto test start (chop)\n", argv[0]);
#endif
		printf("- %s stop  : auto test stop\n", argv[0]);
#if 0
#if 1
		BYTE arrDst[32] = {0};
		BYTE arrHWDst[32] = {0};

		hwflush_dcache_range((ULONG)arrShaSrcC1, SHA_TEST_SIZE);
		for (int i = 0; i < 33; i++) {
			arrShaSrcC1[i] = 'A' + i;
		}
		hwflush_dcache_range((ULONG)arrShaSrcC1, SHA_TEST_SIZE);

		hexDump("arrSrc", arrShaSrcC1, 64);

		if (mode == eSHA256) {
			SHA256(arrShaSrcC1, 32, arrDst);
		} else {
			SHA224(arrShaSrcC1, 32, arrDst);
		}

		ShaShotCalc_rtos(arrShaSrcC1, 32, mode);
		ShaGetDigest(arrHWDst, mode);

		hexCmpDump("Cmp-Dst", arrDst, arrHWDst, output_size);


		if (mode == eSHA256) {
			SHA256(arrShaSrcC1, 33, arrDst);
		} else {
			SHA224(arrShaSrcC1, 33, arrDst);
		}

		ShaShotCalc_rtos(arrShaSrcC1, 33, mode);
		ShaGetDigest(arrHWDst, mode);

		hexCmpDump("Cmp-Dst", arrDst, arrHWDst, output_size);

#else
		BYTE arrDst[32] = {0};
		BYTE arrHWDst[32] = {0};

		hwflush_dcache_range((ULONG)arrShaSrcC1, SHA_TEST_SIZE);
		for (int i = 0; i < SHA_TEST_SIZE; i++) {
			arrShaSrcC1[i] = 'A' + i;
		}
//		arrShaSrc[0] = 'b';
//		arrShaSrc[256] = 'b';
		hwflush_dcache_range((ULONG)arrShaSrcC1, SHA_TEST_SIZE);

		hexDump("arrSrc", arrShaSrcC1, SHA_TEST_SIZE);

		if (mode == eSHA256) {
			SHA256(arrShaSrcC1, SHA_TEST_SIZE, arrDst);
		} else {
			SHA224(arrShaSrcC1, SHA_TEST_SIZE, arrDst);
		}
		hexDump("arrSWDst", arrDst, output_size);

		ShaShotCalc_rtos(arrShaSrcC1, SHA_TEST_SIZE, mode);
		ShaGetDigest(arrHWDst, mode);
		hexDump("arrHWDst", arrHWDst, output_size);

		hexCmpDump("Dst", arrDst, arrHWDst, output_size);

		memset(arrDst, 0, sizeof(arrDst));
		memset(arrHWDst, 0, sizeof(arrHWDst));

		hwflush_dcache_range((ULONG)arrShaSrcC2, SHA_TEST_SIZE);
		for (int i = 0; i < 256; i++) {
			arrShaSrcC2[i] = 'A' + i;
		}
		for (int i = 256; i < SHA_TEST_SIZE; i++) {
			arrShaSrcC2[i] = 0;
		}
		hwflush_dcache_range((ULONG)arrShaSrcC2, SHA_TEST_SIZE);

		hexDump("arrSrc", arrShaSrcC2, SHA_TEST_SIZE);

		if (mode == eSHA256) {
			SHA256(arrShaSrcC2, SHA_TEST_SIZE, arrDst);
		} else {
			SHA224(arrShaSrcC2, SHA_TEST_SIZE, arrDst);
		}
		hexDump("arrSWDst", arrDst, output_size);

		ShaShotCalc_rtos(arrShaSrcC2, SHA_TEST_SIZE, mode);
		ShaGetDigest(arrHWDst, mode);
		hexDump("arrHWDst", arrHWDst, output_size);

		hexCmpDump("Dst", arrDst, arrHWDst, output_size);
#endif
#endif
	} else if (strcmp(argv[1], "reg") == 0) {
		ShaRegShow(ENX_YES);
	} else if (strcmp(argv[1], "start") == 0) {
			if (shaitem.taskHandle != NULL) {
				printf("SHA%u Auto Test already!\n", mode == eSHA256 ? 256 : 224);
				return 0;
			}

			UINT u32TestLoop = 16;
			if (argc == 3) {
				u32TestLoop = atoi(argv[2]);
			}

			shaitem.mode = mode;
			shaitem.u32BufSize = SHA_TEST_MALLOC_SIZE;
			//shaitem.u32TestLoop = u32TestLoop;
			shaitem.u32Move = u32TestLoop;
			shaitem.taskHandle = vTaskCreate("ShaT", ShaTestTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
#if EN675_SINGLE
	} else if (strcmp(argv[1], "start2") == 0) {
		if (shaitem.taskHandle != NULL) {
			printf("SHA%u Auto Test already!\n", mode == eSHA256 ? 256 : 224);
			return 0;
		}

		UINT u32TestLoop = 4096;
		if (argc == 3) {
			u32TestLoop = atoi(argv[2]);
		}

		shaitem.mode = mode;
		shaitem.u32BufSize = SHA_TEST_MALLOC_SIZE;
		//shaitem.u32TestLoop = u32TestLoop;
		shaitem.u32Move = u32TestLoop;
		shaitem.taskHandle = vTaskCreate("ShaT", ShaChopTestTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
#endif
	} else if (strcmp(argv[1], "stop") == 0) {
		shaitem.u32Close = 1;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
// AES
#define AES_TEST_IRQ 1

typedef struct {
	TaskHandle_t taskHandle;
	AESmode mode;
	UINT res;

	BYTE key[32];
	BYTE iv[32];

	BYTE *arrSource;
	BYTE *arrAlignSource;

	BYTE *arrSWEnc;
	BYTE *arrSWDec;
	BYTE *arrHWEnc;
	BYTE *arrHWDec;

	UINT u32BufSize;
	UINT u32Move;
	UINT u32Close;
} AesAutoTestBuffer;

static AesAutoTestBuffer aesitem;
#define AES_TEST_MALLOC_SIZE	(32*1024)
#define AES_TEST_TEST_SIZE		(AES_TEST_MALLOC_SIZE>>1)

#if AES_TEST_IRQ
static void AesTestIrq(void *ctx)
{
	if (aesitem.taskHandle) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(aesitem.taskHandle, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;
		}
	}
}
#endif

UINT AesTestSWProcess(UINT testSize)
{
	ULONG stime;
	AES_KEY aes_swkey;
	BYTE iv[32];
	BYTE *parrSource = (BYTE *)ENX_MEM_ALIGN(aesitem.arrSource);
	BYTE *parrSWEnc = (BYTE *)ENX_MEM_ALIGN(aesitem.arrSWEnc);
	BYTE *parrSWDec = (BYTE *)ENX_MEM_ALIGN(aesitem.arrSWDec);

	memset(parrSWEnc, 0, testSize);
	memset(parrSWDec, 0, testSize);

	stime = BenchTimeStart();

	// S/W Encrypt
	memset(&aes_swkey, 0, sizeof(aes_swkey));
	memcpy(iv, aesitem.iv, 32);

	switch (aesitem.mode) {
	case eAES128_ECB_ENC:
	case eAES128_CBC_ENC:
		AES_set_encrypt_key(aesitem.key, 128, &aes_swkey);
		break;
	case eAES256_ECB_ENC:
	case eAES256_CBC_ENC:
		AES_set_encrypt_key(aesitem.key, 256, &aes_swkey);
		break;
	default:
		printf("AES mode error? (%d)\n", aesitem.mode);
		break;
	}
	switch (aesitem.mode) {
	case eAES128_ECB_ENC:
	case eAES256_ECB_ENC:
		for (UINT k = 0; k < testSize; k += 16) {
			AES_ecb_encrypt(parrSource + k, parrSWEnc + k, &aes_swkey, AES_ENCRYPT);
		}
		break;
	case eAES128_CBC_ENC:
	case eAES256_CBC_ENC:
		AES_cbc_encrypt(parrSource, parrSWEnc, testSize, &aes_swkey, iv, AES_ENCRYPT);
		break;
	default:
		printf("AES mode error? (%d)\n", aesitem.mode);
		break;
	}

	// S/W Decrypt
	memset(&aes_swkey, 0, sizeof(aes_swkey));
	memcpy(iv, aesitem.iv, 32);

	switch (aesitem.mode) {
	case eAES128_ECB_ENC:
	case eAES128_CBC_ENC:
		AES_set_decrypt_key(aesitem.key, 128, &aes_swkey);
		break;
	case eAES256_ECB_ENC:
	case eAES256_CBC_ENC:
		AES_set_decrypt_key(aesitem.key, 256, &aes_swkey);
		break;
	default:
		printf("AES mode error? (%d)\n", aesitem.mode);
		break;
	}
	switch (aesitem.mode) {
	case eAES128_ECB_ENC:
	case eAES256_ECB_ENC:
		for (UINT k = 0; k < testSize; k += 16) {
			AES_ecb_encrypt(parrSWEnc + k, parrSWDec + k, &aes_swkey, AES_DECRYPT);
		}
		break;
	case eAES128_CBC_ENC:
	case eAES256_CBC_ENC:
		AES_cbc_encrypt(parrSWEnc, parrSWDec, testSize, &aes_swkey, iv, AES_DECRYPT);
		break;
	default:
		printf("AES mode error? (%d)\n", aesitem.mode);
		break;
	}

	return BenchTimeStop(stime);
}

void AesTestTask(void *ctx)
{
	aesitem.u32Close = 0;

	aesitem.arrSource = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(aesitem.u32BufSize));
	if (aesitem.arrSource == NULL) {
		printf("malloc error(arrSource), size(%u)\n", aesitem.u32BufSize);
		goto done;
	}
	hwflush_dcache_range((ULONG)aesitem.arrSource, ENX_MEM_ALIGN_BUFFER(aesitem.u32BufSize));

	aesitem.arrAlignSource = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(aesitem.u32BufSize));
	if (aesitem.arrAlignSource == NULL) {
		printf("malloc error(arrAlignSource), size(%u)\n", aesitem.u32BufSize);
		goto done;
	}

	aesitem.arrSWEnc = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(aesitem.u32BufSize));
	if (aesitem.arrSWEnc == NULL) {
		printf("malloc error(arrSWEnc), size(%u)\n", aesitem.u32BufSize);
		goto done;
	}

	aesitem.arrSWDec = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(aesitem.u32BufSize));
	if (aesitem.arrSWDec == NULL) {
		printf("malloc error(arrSWDec), size(%u)\n", aesitem.u32BufSize);
		goto done;
	}

	aesitem.arrHWEnc = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(aesitem.u32BufSize));
	if (aesitem.arrHWEnc == NULL) {
		printf("malloc error(arrHWEnc), size(%u)\n", aesitem.u32BufSize);
		goto done;
	}

	aesitem.arrHWDec = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(aesitem.u32BufSize));
	if (aesitem.arrHWDec == NULL) {
		printf("malloc error(arrHWDec), size(%u)\n", aesitem.u32BufSize);
		goto done;
	}

	BYTE *parrSource = (BYTE *)ENX_MEM_ALIGN(aesitem.arrSource);
	BYTE *parrHwSource = (BYTE *)ENX_MEM_ALIGN(aesitem.arrAlignSource);
	BYTE *parrSWEnc = (BYTE *)ENX_MEM_ALIGN(aesitem.arrSWEnc);
	BYTE *parrSWDec = (BYTE *)ENX_MEM_ALIGN(aesitem.arrSWDec);
	BYTE *parrHWEnc = (BYTE *)ENX_MEM_ALIGN(aesitem.arrHWEnc);
	BYTE *parrHWDec = (BYTE *)ENX_MEM_ALIGN(aesitem.arrHWDec);

	char strbuf[8] = {0};
	switch (aesitem.mode) {
	case eAES128_ECB_DEC:
		aesitem.mode = eAES128_ECB_ENC; // @suppress("No break at end of case")
	case eAES128_ECB_ENC:
		sprintf(strbuf, "128 ECB");
		break;
	case eAES128_CBC_DEC:
		aesitem.mode = eAES128_CBC_ENC; // @suppress("No break at end of case")
	case eAES128_CBC_ENC:
		sprintf(strbuf, "128 CBC");
		break;
	case eAES256_ECB_DEC:
		aesitem.mode = eAES256_ECB_ENC; // @suppress("No break at end of case")
	case eAES256_ECB_ENC:
		sprintf(strbuf, "256 ECB");
		break;
	case eAES256_CBC_DEC:
		aesitem.mode = eAES256_CBC_ENC; // @suppress("No break at end of case")
	case eAES256_CBC_ENC:
		sprintf(strbuf, "256 CBC");
		break;
	}

#if AES_TEST_IRQ
	AesIrqCallback(AesTestIrq, NULL);
	AesSetIrqEn(ENX_ON);
#endif

	printf("AES%s(%u) Test Task(Buffer Size: %u)(Loop: %u)\n", strbuf, aesitem.mode, aesitem.u32BufSize, aesitem.u32Move);
	printf("              S/W (SwSource: 0x%08X%11s) (Encrypt: 0x%08X%11s) (Decrypt: 0x%08X%11s)\n", (UINT)(intptr_t)parrSource, " ", parrSWEnc, " ", parrSWDec, " ");
	printf("              H/W (HwSource: 0x%08X~0x%08X) (Encrypt: 0x%08X~0x%08X) (Decrypt: 0x%08X~0x%08X)\n", (UINT)(intptr_t)parrHwSource, ((UINT)(intptr_t)parrHwSource) + aesitem.u32Move, parrHWEnc, parrHWEnc + aesitem.u32Move, parrHWDec, parrHWDec + aesitem.u32Move);

	hwflush_dcache_range_rtos((ULONG)parrSource, aesitem.u32BufSize);
	for (UINT i = 0; i < aesitem.u32BufSize; i++) {
		parrSource[i] = rand() % 255;
	}
	hwflush_dcache_range_rtos((ULONG)parrSource, aesitem.u32BufSize);

	UINT pass = 0, fail = 0;
	ULONG stime, hwsum, swsum, flag, celcnt, celcnt2;
	for (UINT testSize = 16; testSize <= AES_TEST_TEST_SIZE; testSize += 16) {
		UINT i, j, k;
		i = j = k = 0;
		celcnt = celcnt2 = hwsum = flag = 0;

		if (aesitem.u32Close == 1) {
			break;
		}

		// Init key, iv
		for (i = 0; i < 32; i++) {
#if 1
			aesitem.iv[i] = rand();
#else
			aesitem.iv[i] = 0;
#endif
			aesitem.key[i] = rand();
		}

		printf("%3u%% %5u/%5u ", ((testSize) * 100) / AES_TEST_TEST_SIZE, testSize, AES_TEST_TEST_SIZE);
		printf("HW(avg:%5lu) SW(%6lu) -     ", 0, 0);

		// S/H AES Encrypt/Decrypt
		swsum = AesTestSWProcess(testSize);

		printf("\r%3u%% %5u/%5u ", ((testSize) * 100) / AES_TEST_TEST_SIZE, testSize, AES_TEST_TEST_SIZE);
		printf("HW(avg:%5lu) SW(%6lu) -     ", 0, swsum);

		for (i = 0; i < aesitem.u32Move; i++) {
			BYTE *parrNewHwSource = parrHwSource + i;

			if (flag | (aesitem.u32Close == 1)) {
				break;
			}

			hwflush_dcache_range((ULONG)parrNewHwSource, testSize);
			BDmaMemSet_rtos(0, parrHwSource, 0x00, aesitem.u32BufSize); // size: or (testSize + aesitem.u32Move)
			BDmaMemCpy_rtos(0, parrNewHwSource, parrSource, testSize);

			for (j = 0; j < aesitem.u32Move; j++) {
				BYTE *parrNewHWEnc = parrHWEnc + j;

				if (flag | (aesitem.u32Close == 1)) {
					break;
				}

				stime = BenchTimeStart();
				AesSetKey(aesitem.key, 32);
				AesSetIv(aesitem.iv);
				hwflush_dcache_range((ULONG)parrNewHWEnc, testSize);
				BDmaMemSet_rtos(0, parrHWEnc, 0x00, aesitem.u32BufSize);
#if AES_TEST_IRQ
				AesCalc_rtos_async(parrNewHWEnc, parrNewHwSource, testSize, aesitem.mode);
#else
				AesCalc_rtos(parrNewHWEnc, parrNewHwSource, testSize, aesitem.mode);
#endif

#if AES_TEST_IRQ
				if (ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
#endif
					hwsum += BenchTimeStop(stime);
					celcnt++;

					// Comp
					for (int lop = 0; lop < testSize; lop++) {
						if (parrSWEnc[lop] != parrNewHWEnc[lop]) {
							flag = 1;
							hexCmpDump("ENC", parrSWEnc, parrNewHWEnc, testSize);
							break;
						}
					}

					if (flag == 0) { // decoding
						for (k = 0; k < aesitem.u32Move; k++) {
							BYTE *parrNewHWDec = parrHWDec + k;
//							printf("%5u - %4u/%4u/%4u - Encrypt(0x%08X->0x%08X) Decrypt(0x%08X->0x%08X)\n", testSize, i, j, k, parrNewHwSource, parrNewHWEnc, parrNewHWEnc, parrNewHWDec);

							stime = BenchTimeStart();
							hwflush_dcache_range((ULONG)parrNewHWDec, testSize);
							BDmaMemSet_rtos(0, parrHWDec, 0x00, aesitem.u32BufSize);
#if AES_TEST_IRQ
							AesCalc_rtos_async(parrNewHWDec, parrNewHWEnc, testSize, aesitem.mode - 1);
#else
							AesCalc_rtos(parrNewHWDec, parrHWEnc, testSize, aesitem.mode - 1);
#endif

#if AES_TEST_IRQ
							if (ulTaskNotifyTake(pdTRUE, 300)) { // Timeout 3sec
#endif
								hwsum += BenchTimeStop(stime);
								celcnt2++;

								for (int lop = 0; lop < testSize; lop++) {
									if (parrSWDec[lop] != parrNewHWDec[lop] || parrSource[lop] != parrNewHWDec[lop]) {
										flag = 1;
										break;
									}
								}
							} else {
								flag = 2;
								break;
							}
						}
#if AES_TEST_IRQ
					}
#endif
#if AES_TEST_IRQ
				} else {
					flag = 2;
				}
#endif

				static UINT processrate = 0;
				if (processrate != (((i * aesitem.u32Move + j) * 100) / (aesitem.u32Move * aesitem.u32Move))) {
					processrate = ((i * aesitem.u32Move + j) * 100) / (aesitem.u32Move * aesitem.u32Move);
					char buf[8] = {0};
					snprintf(buf, 8, "%3u%%", processrate);
					printf("\b\b\b\b");
					printf("%s", buf);
				}
			}

			printf("\r%3u%% %5u/%5u ", ((testSize) * 100) / AES_TEST_TEST_SIZE, testSize, AES_TEST_TEST_SIZE);
			printf("HW(avg:%5lu) SW(%6lu) -     ", hwsum / (celcnt + celcnt2), swsum);
		}

		printf("\b\b\b\b");
		if (flag == 0) {
			char buf[32] = {0};
			ULONG hwavg = hwsum / (celcnt + celcnt2);
			ULONG swavg = swsum;
			snprintf(buf, 32, "x%.2f", (float)swavg / (float)hwavg);
			_Gprintf(" OK ");
			printf("%s\n", buf);
			pass++;
		} else {
			if (flag == 1) {
				_Rprintf("Fail\n");
#if AES_TEST_IRQ
			} else if (flag == 2) {
				_Rprintf("Timeout\n");
#endif
			}
			fail++;
			break;
		}
	}

	printf("\nTest End. Pass(%d) Fail(%d)\n", pass, fail);

done:
#if AES_TEST_IRQ
	AesSetIrqEn(ENX_OFF);
	AesIrqCallback(NULL, NULL);
#endif
	if (aesitem.arrSource) {
		vPortFree(aesitem.arrSource);
	}
	if (aesitem.arrAlignSource) {
		vPortFree(aesitem.arrAlignSource);
	}
	if (aesitem.arrSWEnc) {
		vPortFree(aesitem.arrSWEnc);
	}
	if (aesitem.arrSWDec) {
		vPortFree(aesitem.arrSWDec);
	}
	if (aesitem.arrHWEnc) {
		vPortFree(aesitem.arrHWEnc);
	}
	if (aesitem.arrHWDec) {
		vPortFree(aesitem.arrHWDec);
	}
	aesitem.arrSource = NULL;
	aesitem.arrAlignSource = NULL;
	aesitem.arrSWEnc = NULL;
	aesitem.arrSWDec = NULL;
	aesitem.arrHWEnc = NULL;
	aesitem.arrHWDec = NULL;
	aesitem.taskHandle = NULL;
	vTaskDelete(NULL);
}

#include "enx_freertos.h"
#include "task.h"

int aestestirqflag = 0;

static void AesTestIrq2(void *ctx)
{
#if 0
	printf("irq(/req(%u)", AES_REQ);
	aestestirqflag = 0;
#else
	if (aesitem.taskHandle) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(aesitem.taskHandle, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;
		}
	}
#endif
}

#define AES_TEST_SIZE 1024

__attribute__((__aligned__(64))) unsigned char pSrc[AES_TEST_SIZE];
__attribute__((__aligned__(64))) unsigned char pEnc[AES_TEST_SIZE];
__attribute__((__aligned__(64))) unsigned char pDec[AES_TEST_SIZE];
void AesTestMain(void)
{
	int i, flag = 0;
	unsigned char aeskey[32];
	unsigned char aesiv[16];

	for (i = 0; i < 32; i++) {
		aeskey[i] = i;
	}
	for (i = 0; i < 16; i++) {
		aesiv[i] = 0;
	}

	hwflush_dcache_range((ULONG)pDec, AES_TEST_SIZE);
	hwflush_dcache_range((ULONG)pSrc, AES_TEST_SIZE);
	for (i = 0; i < AES_TEST_SIZE; i++) {
		pSrc[i] = i;
	}
	hwflush_dcache_range((ULONG)pSrc, AES_TEST_SIZE);

	AesSetKey(aeskey, 32);
	AesSetIv(aesiv);

	aesitem.taskHandle = xTaskGetHandle(pcTaskGetName(NULL));
	AesIrqCallback(AesTestIrq2, NULL);
	AES_IRQ_EN = 1;

	int lop = 0;
	for (lop = 16; lop <= 1024; lop+=16) {
		aestestirqflag = 1;
		hwflush_dcache_range((ULONG)pEnc, lop);
		AesCalc_isr_async(pEnc, pSrc, lop, eAES256_CBC_ENC);
		ulTaskNotifyTake(pdTRUE, 300);
		//while(aestestirqflag);
		//while(AES_REQ);

	//	hwflush_dcache_range((ULONG)pEnc, lop);
	//	hexDump("TEST", pEnc, lop);

		aestestirqflag = 1;
		AesCalc_isr_async(pDec, pEnc, lop, eAES256_CBC_DEC);
		ulTaskNotifyTake(pdTRUE, 300);
		//while(aestestirqflag);
		//while(AES_REQ);

		hwflush_dcache_range((ULONG)pDec, lop);

		for (i = 0; i < lop; i++) {
			if (pSrc[i] != pDec[i]) {
				flag = 1;
				break;
			}
		}

		if (flag == 1) {
			printf("Error\n");
			//break;
		} else {
			printf("OK\n");
		}
	}


	hexCmpDump("TEST", pSrc, pDec, AES_TEST_SIZE);
}

#if 1
int cmd_test_aes(int argc, char *argv[])
{
#if 0
	typedef enum {				//				bit		type	mode
		eAES128_ECB_ENC = 0,	// AES_MODE=0	128		ecb		enc
		eAES128_ECB_DEC = 1,	// AES_MODE=1	128		ecb		dec
		eAES128_CBC_ENC = 2,	// AES_MODE=2	128		cbc		enc
		eAES128_CBC_DEC = 3,	// AES_MODE=3	128		cbc		dec
		eAES256_ECB_ENC = 4,	// AES_MODE=4	256		ecb		enc
		eAES256_ECB_DEC = 5,	// AES_MODE=5	256		ecb		dec
		eAES256_CBC_ENC = 6,	// AES_MODE=6	256		cbc		enc
		eAES256_CBC_DEC = 7,	// AES_MODE=7	256		cbc		dec
	} AESmode;
#endif
	static UINT bit_flag = 0;
	static UINT type_flag = 0x2;
	if (strcmp(argv[0], "aes128") == 0) {
		bit_flag = 0x0;
	} else if (strcmp(argv[0], "aes256") == 0) {
		bit_flag = 0x4;
	} else {
		printf("Error, %s mode\n", argv[0]);
		return 0;
	}

	if (argc == 1) {
		printf("AES test command\n");
		printf("- %s reg   : view register\n", argv[0]);
		printf("- %s type  : auto test type(ecb, cbc)\n", argv[0]);
		printf("- %s start : auto test start\n", argv[0]);
		printf("- %s stop  : auto test stop\n", argv[0]);
	} else if ((argc >= 2 && strcmp(argv[1], "reg") == 0)) {
		AesRegShow(ENX_YES);
	} else if (argc >= 2 && strcmp(argv[1], "type") == 0) {
		if (argc > 2) {
			if (strcmp(argv[2], "ecb") == 0) {
				type_flag = 0x0;
			} else if (strcmp(argv[2], "cbc") == 0) {
				type_flag = 0x2;
			}
		}
		printf("AES type: %s\n", type_flag == 0x0 ? "EBC" : "CBC");
	} else if (argc >= 2 && strcmp(argv[1], "start") == 0) {
		if (aesitem.taskHandle != NULL) {
			printf("AES Auto Test already!\n");
			return 0;
		}

#if EN675_SINGLE
		UINT u32TestLoop = 16;
		if (argc == 3) {
			u32TestLoop = atoi(argv[2]);
		}
#else
		UINT u32TestLoop = 1;
#endif

		aesitem.mode = bit_flag | type_flag;
		aesitem.u32BufSize = AES_TEST_MALLOC_SIZE;
		aesitem.u32Move = u32TestLoop;
		aesitem.taskHandle = vTaskCreate("AesT", AesTestTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
	} else if (argc == 2 && strcmp(argv[1], "stop") == 0) {
		aesitem.u32Close = 1;
	}

	return 0;
}
#else
#define MAX_BLOCK_SIZE 128
#define AES_TEST_SIZE (512)
ATTR_MALIGN64 BYTE arrSrc[AES_TEST_SIZE] = {0};

ATTR_MALIGN64 BYTE arrSWDst128[AES_TEST_SIZE] = {0};
ATTR_MALIGN64 BYTE arrSWDec128[AES_TEST_SIZE] = {0};

ATTR_MALIGN64 BYTE arrSWDst256[AES_TEST_SIZE] = {0};
ATTR_MALIGN64 BYTE arrSWDec256[AES_TEST_SIZE] = {0};

ATTR_MALIGN64 BYTE arrHWDst128[AES_TEST_SIZE] = {0};
ATTR_MALIGN64 BYTE arrHWDst256[AES_TEST_SIZE] = {0};

ATTR_MALIGN64 BYTE arrHWDst128_DES[AES_TEST_SIZE] = {0};
ATTR_MALIGN64 BYTE arrHWDst256_DES[AES_TEST_SIZE] = {0};

int cmd_test_aes(int argc, char *argv[])
{
	unsigned char iv[2 * MAX_BLOCK_SIZE / 8] = {0};
	AES_KEY aes_key16, aes_key32;
#if 0
    static const unsigned char key16[16] = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
        0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12
    };
    static const unsigned char key32[32] = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
        0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12,
        0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34,
        0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34, 0x56
    };
#else
    static const unsigned char key16[16] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    static const unsigned char key32[32] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
#endif
	AES_KEY_255_224 = key16[ 0] << 24 | key16[ 1] << 16 | key16[ 2] << 8 | key16[ 3];
	AES_KEY_223_192 = key16[ 4] << 24 | key16[ 5] << 16 | key16[ 6] << 8 | key16[ 7];
	AES_KEY_191_160 = key16[ 8] << 24 | key16[ 9] << 16 | key16[10] << 8 | key16[11];
	AES_KEY_159_128 = key16[12] << 24 | key16[13] << 16 | key16[14] << 8 | key16[15];
	AES_KEY_127_96  = key32[16] << 24 | key32[17] << 16 | key32[18] << 8 | key32[19];
	AES_KEY_95_64   = key32[20] << 24 | key32[21] << 16 | key32[22] << 8 | key32[23];
	AES_KEY_63_32   = key32[24] << 24 | key32[25] << 16 | key32[26] << 8 | key32[27];
	AES_KEY_31_0    = key32[28] << 24 | key32[29] << 16 | key32[30] << 8 | key32[31];
	AES_CBC_EN      = 1;

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	hwflush_dcache_range((ULONG)arrSrc, AES_TEST_SIZE);
	for (int i = 0; i < AES_TEST_SIZE; i++) {
		arrSrc[i] = 'A' + i;
	}
	hwflush_dcache_range((ULONG)arrSrc, AES_TEST_SIZE);
	hexDump("arrSrc", arrSrc, AES_TEST_SIZE);

	////////////////////////////////////////////////////////////////////////////////////
	printf("\n===================================================================\n\n");
	////////////////////////////////////////////////////////////////////////////////////

	memset(&aes_key16, 0, sizeof(aes_key16));
	memset(iv, 0, sizeof(iv));
	AES_set_encrypt_key(key16, 128, &aes_key16);
    AES_cbc_encrypt(arrSrc, arrSWDst128, AES_TEST_SIZE, &aes_key16, iv, AES_ENCRYPT);
	hexDump("AES128_ENCRYPT_SW", arrSWDst128, AES_TEST_SIZE);

	memset(&aes_key16, 0, sizeof(aes_key16));
	memset(iv, 0, sizeof(iv));
	AES_set_decrypt_key(key16, 128, &aes_key16);
	AES_cbc_encrypt(arrSWDst128, arrSWDec128, AES_TEST_SIZE, &aes_key16, iv, AES_DECRYPT);
	hexDump("AES128_DECRYPT_SW", arrSWDec128, AES_TEST_SIZE);

	hwflush_dcache_range((ULONG)arrHWDst128, AES_TEST_SIZE);
	AesCalc_rtos(arrHWDst128, arrSrc, AES_TEST_SIZE, eAES128_CBC_ENC);
	hwflush_dcache_range((ULONG)arrHWDst128, AES_TEST_SIZE);
	hexDump("AES128_ENCRYPT_HW", arrHWDst128, AES_TEST_SIZE);

	AesCalc_rtos(arrHWDst128_DES, arrHWDst128, AES_TEST_SIZE, eAES128_CBC_DEC);
	hexDump("AES128_DECRYPT_HW", arrHWDst128_DES, AES_TEST_SIZE);

	hexCmpDump("AES128_ENCRYPT-COMP", arrHWDst128, arrSWDst128, AES_TEST_SIZE);

	hexCmpDump("AES128_DECRYPT_COMP", arrHWDst128_DES, arrSWDec128, AES_TEST_SIZE);

	////////////////////////////////////////////////////////////////////////////////////
	printf("\n===================================================================\n\n");
	hexDump("arrSrc", arrSrc, AES_TEST_SIZE);
	printf("\n===================================================================\n\n");
	////////////////////////////////////////////////////////////////////////////////////

	memset(&aes_key32, 0, sizeof(aes_key32));
	memset(iv, 0, sizeof(iv));
    AES_set_encrypt_key(key32, 256, &aes_key32);
    AES_cbc_encrypt(arrSrc, arrSWDst256, AES_TEST_SIZE, &aes_key32, iv, AES_ENCRYPT);
	hexDump("AES256_ENCRYPT_SW", arrSWDst256, AES_TEST_SIZE);

	memset(&aes_key32, 0, sizeof(aes_key32));
	memset(iv, 0, sizeof(iv));
	AES_set_decrypt_key(key32, 256, &aes_key32);
	AES_cbc_encrypt(arrSWDst256, arrSWDec256, AES_TEST_SIZE, &aes_key32, iv, AES_DECRYPT);
	hexDump("AES256_DECRYPT_SW", arrSWDec256, AES_TEST_SIZE);

	hwflush_dcache_range((ULONG)arrHWDst256, AES_TEST_SIZE);
	AesCalc_rtos(arrHWDst256, arrSrc, AES_TEST_SIZE, eAES256_CBC_ENC);
	hwflush_dcache_range((ULONG)arrHWDst256, AES_TEST_SIZE);
	hexDump("AES256_ENCRYPT_HW", arrHWDst256, AES_TEST_SIZE);

	AesCalc_rtos(arrHWDst256_DES, arrHWDst256, AES_TEST_SIZE, eAES256_CBC_DEC);
	hexDump("AES256_DECRYPT_HW", arrHWDst256_DES, AES_TEST_SIZE);

	hexCmpDump("AES256_ENCRYPT-COMP", arrHWDst256, arrSWDst256, AES_TEST_SIZE);

	hexCmpDump("AES256_DECRYPT_COMP", arrHWDst256_DES, arrSWDec256, AES_TEST_SIZE);

	return 0;
}
#endif
