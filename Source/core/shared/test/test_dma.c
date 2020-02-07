#include "dev.h"
#include "test_header.h"
#include "test_nonos.h"

#include <stdlib.h>

static DmaTestStr_nonos dmanonostest;

static void test_dma_irq(void *ctx)
{
	dmanonostest.flag = 1;
}

void test_dma_start(void)
{
	if (dmanonostest.target == eDMAtest_DDRtoDDR || dmanonostest.target == eDMAtest_DDRtoSRAM) {
		dmanonostest.arrSrc = malloc(dmanonostest.u64BufSize);
		if (dmanonostest.arrSrc == NULL) {
			printf("malloc error(arrSrc), size(%lu)\n", dmanonostest.u64BufSize);
			goto done;
		}
	} else {
		dmanonostest.arrSrc = (BYTE *)0xA0000000;
	}

	if (dmanonostest.target == eDMAtest_DDRtoDDR || dmanonostest.target == eDMAtest_SRAMtoDDR) {
		dmanonostest.arrDst = malloc(dmanonostest.u64BufSize);
		if (dmanonostest.arrDst == NULL) {
			printf("malloc error(arrDst), size(%lu)\n", dmanonostest.u64BufSize);
			goto done;
		}
	} else {
		dmanonostest.arrDst = (BYTE *)0xA0020000;
	}

	BYTE *parrSrc = dmanonostest.arrSrc;
	BYTE *parrDst = dmanonostest.arrDst;

	dmanonostest.flag = 0;
	if (dmanonostest.cMode == 'B') {
		BDmaIrqCallback(dmanonostest.u32CH, test_dma_irq, NULL);
		BDmaSetIrqEn(dmanonostest.u32CH, ENX_ON);
	} else if (dmanonostest.cMode == 'C') {
		CDmaIrqCallback(dmanonostest.u32CH, test_dma_irq, NULL);
		CDmaSetIrqEn(dmanonostest.u32CH, ENX_ON);
	} else {
		printf("mode error, '%c'\n", dmanonostest.cMode);
		goto done;
	}

	//srand(rdcycle());
	srand(1);

	printf("%cDMA%u Test Task(Move:%u)(Count:%u)", dmanonostest.cMode, dmanonostest.u32CH, dmanonostest.u32Move, dmanonostest.u32Move * dmanonostest.u32Move);

	if (dmanonostest.u32TestSize == -1) {
		printf("(Size:Random)\n");
	} else {
		printf("(Size:%ubyte)\n", dmanonostest.u32TestSize);
	}

	switch (dmanonostest.target) {
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
	if (dmanonostest.u64BufSize > 16 * 1024 * 1024) {
		u32RelTestSize = 16 * 1024 * 1024;
	} else {
		u32RelTestSize = dmanonostest.u64BufSize;
	}
	hwflush_dcache_range((ULONG)dmanonostest.arrSrc, u32RelTestSize);
	for (UINT i = 0; i < dmanonostest.u64BufSize; i++) {
		dmanonostest.arrSrc[i] = rand() % 255;
	}
	hwflush_dcache_range((ULONG)dmanonostest.arrSrc, u32RelTestSize);

	u32RelTestSize = 0;
	UINT pass = 0, fail = 0, flag = 0;
	for (UINT i = 0; i < dmanonostest.u32Move; i++) {
		parrDst = dmanonostest.arrDst;
		if (dmanonostest.u32TestSize == -1) {
			u32RelTestSize = rand() % 65536 + 1;
		} else {
			u32RelTestSize = dmanonostest.u32TestSize;
		}

		printf("DMA(%u/%u)\n", i+1, dmanonostest.u32Move);
//		printf("%3u%% %4u/%4u TEST Dst(0x%08X~0x%08X) Src(0x%08X) - Size(%6u) -     ", ((i+1) * 100) / dmanonostest.u32Move, i+1, dmanonostest.u32Move, (UINT)(intptr_t)parrDst, (UINT)(intptr_t)(parrDst+dmanonostest.u32Move), (UINT)(intptr_t)parrSrc, u32RelTestSize);

		ULONG total_time = 0;
		ULONG total_size = 0;
		for (UINT j = 0; j < dmanonostest.u32Move; j++) {
			dmanonostest.flag = 0;

			memset(parrDst, 0, u32RelTestSize);

			ULONG tstart = 0;
			if (dmanonostest.cMode == 'B') {
				hwflush_dcache_range((ULONG)parrDst, u32RelTestSize);
				tstart = BenchTimeStart();
				BDmaMemCpy_isr_async(dmanonostest.u32CH, parrDst, parrSrc, u32RelTestSize);
			} else if (dmanonostest.cMode == 'C') {
				tstart = BenchTimeStart();
				CDmaMemCpy_isr_async(dmanonostest.u32CH, parrDst, parrSrc, u32RelTestSize);
			}

			UINT timeout_cnt = 3 * 1000 * 1000;
			while (timeout_cnt) {
				if (dmanonostest.flag == 1) {
					break;
				}
				WaitXus(1);
				timeout_cnt--;
			}

			if (timeout_cnt && dmanonostest.flag) { // Timeout 3sec
				UINT tgap = BenchTimeStop(tstart);
				total_time += tgap;
				total_size += u32RelTestSize;

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
#if 0
					static UINT processrate = 0;
					if (processrate != (j * 100 / dmanonostest.u32Move)) {
						processrate = j * 100 / dmanonostest.u32Move;
						char buf[12] = {0};
						snprintf(buf, 12, "%3u%%", processrate);
						printf("\b\b\b\b");
						printf("%s", buf);
					}
#endif
				} else {
//					printf("\b\b\b\b");
					_Rprintf("Fail\nError(%u/%u) Move(%u) Dst(0x%08X)\n", k, u32RelTestSize, j, parrDst);
					hexCmpDump("Dma Test", parrSrc, parrDst, u32RelTestSize);
					i = j = -0xf;
					fail++;
					break;
				}
			} else {
				flag = 1;
//				printf("\b\b\b\b");
				_Rprintf("\nTimeout! 0x%08X <- 0x%08X, Len:%ubyte\n", parrDst, parrSrc, u32RelTestSize);
				if (dmanonostest.cMode == 'C') {
					extern void CDmaRegshow(UINT nCH);
					CDmaRegshow(dmanonostest.u32CH);
				}
				i = j = -0xf;
				fail++;
				break;
			}

			// next
			parrDst++;
		}

		if (flag == 0) {
//			char buf[64] = {0};
//			snprintf(buf, 64, "%.2f", (total_size / 1024.0 / 1024.0) / (total_time / 1000.0 / 1000.0));
//			printf("\b\b\b\b");
//			_Gprintf("OK, %sMbyte/s\n", buf);
		} else {
//			printf("\b\b\b\b");
			_Rprintf("Fail\n");
		}

		parrSrc++;
	}
	printf("\nTest End. Pass(%d) Fail(%d)\n", pass, fail);

done:
	if (dmanonostest.cMode == 'B') {
		BDmaIrqCallback(dmanonostest.u32CH, NULL, NULL);
		BDmaSetIrqEn(dmanonostest.u32CH, ENX_OFF);
	} else if (dmanonostest.cMode == 'C') {
		CDmaIrqCallback(dmanonostest.u32CH, NULL, NULL);
		CDmaSetIrqEn(dmanonostest.u32CH, ENX_OFF);
	}
	if (dmanonostest.target == eDMAtest_DDRtoDDR || dmanonostest.target == eDMAtest_DDRtoSRAM) {
		free(dmanonostest.arrSrc);
	}
	if (dmanonostest.target == eDMAtest_DDRtoDDR || dmanonostest.target == eDMAtest_SRAMtoDDR) {
		free(dmanonostest.arrDst);
	}
	dmanonostest.arrSrc = NULL;
	dmanonostest.arrDst = NULL;
}

void test_dma_init(ULONG bufsize, UINT testsize, UINT move, char mode, UINT ch, DMAtestTarget target)
{
	dmanonostest.arrDst = NULL;
	dmanonostest.arrSrc = NULL;
	dmanonostest.u64BufSize = bufsize;
	dmanonostest.u32TestSize = testsize;
	dmanonostest.u32Move = move;
	dmanonostest.cMode = mode;
	dmanonostest.u32CH = ch;
	dmanonostest.target = target;
}
