#include "dev.h"

#include "enx_freertos.h"
#include "shell.h"

#include <stdio.h>

extern void trap_freertos(void); // mentry.S

#if 0
void main_1_bdma_test(void)
{
	char cMode = 'B';
	UINT u32CH = 3;
	UINT u32Move = 100;
	UINT u32TestSize = 65535;
	UINT u64BufSize = 4 * 1024 * 1024;

	BYTE *arrSrc = 0x83800000; // ~ 4MB
	//BYTE *arrDst = 0x83C00000; // ~ 4MB
	BYTE *arrDst = 0x90000000; // ~ 4MB
	BYTE *parrSrc = arrSrc;
	BYTE *parrDst = arrDst;

//	BDmaIrqCallback(nTestchannel, DmaTestIrq, NULL);
//	BDmaSetIrqEn(nTestchannel, ENX_ON);

	srand(1);

	hwflush_dcache_range_rtos((ULONG)arrSrc, 4*1024*1024);
	for (UINT i = 0; i < u64BufSize; i++) {
		arrSrc[i] = rand() % 255;
	}
	hwflush_dcache_range_rtos((ULONG)arrSrc, 4*1024*1024);

	UINT u32RelTestSize = 0;
	UINT pass = 0, fail = 0, flag = 0;
	for (UINT i = 0; i < u32Move; i++) {
		parrDst = arrDst;
		if (u32TestSize == -1) {
			u32RelTestSize = rand() % 65536 + 1;
		} else {
			u32RelTestSize = u32TestSize;
		}


		printf("%3u%% %4u/%4u TEST Dst(0x%08X~0x%08X) Src(0x%08X) - Size(%6u) -     ", ((i+1) * 100) / u32Move, i+1, u32Move, (UINT)(intptr_t)parrDst, (UINT)(intptr_t)(parrDst+u32Move), (UINT)(intptr_t)parrSrc, u32RelTestSize);


		ULONG total_time = 0;
		ULONG total_size = 0;
		for (UINT j = 0; j < u32Move; j++) {
			// Set array
			memset(parrDst, 0, u32RelTestSize);
			//portMEMORY_BARRIER();
			//hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);
#if 1
			ULONG tstart = 0;
			if (cMode == 'B') {
				// Set array
				hwflush_dcache_range_rtos((ULONG)parrDst, u32RelTestSize);

				tstart = BenchTimeStart();
				//BDmaMemCpy_rtos_async(u32CH, parrDst, parrSrc, u32RelTestSize);
				BDmaMemCpy_isr(u32CH, parrDst, parrSrc, u32RelTestSize);
			} else if (cMode == 'C') {
				// dma copy
				tstart = BenchTimeStart();
				//CDmaMemCpy_rtos_async(u32CH, parrDst, parrSrc, u32RelTestSize);
				CDmaMemCpy_isr(u32CH, parrDst, parrSrc, u32RelTestSize);
			}

			UINT tgap = BenchTimeStop(tstart);
			total_time += tgap;
			total_size += u32RelTestSize;

			if (cMode == 'B') {
				hwflush_dcache_range((ULONG)parrDst, u32RelTestSize);
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
				if (processrate != (j * 100 / u32Move)) {
					processrate = j * 100 / u32Move;
					char buf[64] = {0};
					snprintf(buf, 64, "%3u%%", processrate);
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
}
#endif

void main_1(int cpu_id)
{
#if 1
	while(SYS_REG0 == 0x0) {} // Wait for CPU0 to be ready.

	write_csr(mstatus, MSTATUS_FS); // Enable

	while (1) {
		if (SYS_REG0 == 0xA) {
			GpioSetOut(GPIO_LED1, GPIO_OUT_HI);
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xB;

			//main_1_bdma_test();
		}
		WaitXms(1);
	}
#else
	while(g_key == 0xFF) {} // Wait for CPU0 to be ready.

	_printf("HELLO EYENIX!(id:%d)(%x)\n", cpu_id, &cpu_id); // ADD some

	vMemoryHeapInit();

	vTaskCreate("shell", ShellTask, NULL, 8192, 3);

	vTaskStartScheduler();
#endif
}
