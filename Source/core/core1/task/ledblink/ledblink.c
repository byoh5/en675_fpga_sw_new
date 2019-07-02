//*************************************************************************************************
// Led task
//*************************************************************************************************
#include "dev.h"
#include "enx_freertos.h"

#include "ledblink.h"

#include <stdio.h>	// for snprintf
#include <math.h>	// for M_PI

#if 0
int setTimeZone(void)
{
	char strTimeZone[10] = {0};
	//gtUser.nTimeZone = tzIndex;
	//gptMsgShare.TIMEZONE = nTimeZoneSec[tzIndex][2] + (gtUser.nTimeSummer * 3600);
	//, -nTimeZoneSec[tzIndex][0], nTimeZoneSec[tzIndex][1]
	sprintf(strTimeZone, "UTC9:0");
	return setenv("TZ", strTimeZone, 1);
}
#endif

//#include <malloc.h>
void LedblinkTask(void *pvParameters)
{
#if 0
	BYTE *bas = 0x80800000;

	BYTE *a = malloc(16);
	BYTE *b = malloc(16);
	printf("a:0x%08X b:0x%08X\n", a, b);
	for(int i = 0; i < 16; i++) {
		a[i] = i;
	}
	for(int i = 0; i < 16; i++) {
		b[i] = 128 + i;
	}
	free(a);
	BYTE *c = malloc(32);
	for(int i = 0; i < 32; i++) {
		c[i] = 64 + i;
	}

	BYTE *d = malloc(4);
	for(int i = 0; i < 4; i++) {
		d[i] = 0xdd;
	}

	BYTE *e = malloc(4);
	for(int i = 0; i < 4; i++) {
		e[i] = 0xee;
	}

	hexDump("Test", bas, 512);

	d = realloc(d, 25);
	for(int i = 0; i < 25; i++) {
		d[i] = 0xff;
	}

	hexDump("Test", bas, 512);
#endif

#if 0
	BYTE **test = (BYTE **)pvPortMalloc(4096*sizeof(void *));
	for (int i = 0; i < 4096; i++) {
		test[i] = NULL;
	}

	struct mallinfo minfo;
	minfo = mallinfo();
	printf("\r\n********************CPU%d Memory state********************\r\n",0);
	printf("arena : %u\r\n", (unsigned int) minfo.arena);			// This is the total size of memory allocated with sbrk by malloc, in bytes.
	printf("sbmlks : %u\r\n", (unsigned int) minfo.smblks);			// This field is unused.
	printf("hblks : %u\r\n", (unsigned int) minfo.hblks); 			// This is the total number of chunks allocated with mmap.
	printf("ordblks : %u\r\n", (unsigned int) minfo.ordblks);		// This is the number of chunks not in use.
	printf("hblkhd : %u\r\n", (unsigned int) minfo.hblkhd);			// This is the total size of memory allocated with mmap, in bytes.
	printf("usmblks : %u\r\n", (unsigned int) minfo.usmblks); 		// This field is unused.
	printf("fsmblks : %u\r\n", (unsigned int) minfo.fsmblks); 		// This field is unused.
	printf("uordblks : %u\r\n", (unsigned int) minfo.uordblks);		// This is the total size of memory occupied by chunks handed out by malloc.
	printf("fordblks : %u\r\n", (unsigned int) minfo.fordblks);		// This is the total size of memory occupied by free (not in use) chunks.
	printf("keepcost : %u\r\n", (unsigned int) minfo.keepcost);		// This is the size of the top-most releasable chunk that normally borders the end of the heap.
	printf("*********************************************************\r\n");

	for (int i = 0; i < 4096; i++) {
		test[i] = malloc(16*1024);
		if (test[i] == NULL) {
			printf("malloc fail\n");
			break;
		}
		printf("%8u New 0x%08X\n", i, test[i]);
	}

	minfo = mallinfo();
	printf("\r\n********************CPU%d Memory state********************\r\n",0);
	printf("arena : %u\r\n", (unsigned int) minfo.arena);			// This is the total size of memory allocated with sbrk by malloc, in bytes.
	printf("sbmlks : %u\r\n", (unsigned int) minfo.smblks);			// This field is unused.
	printf("hblks : %u\r\n", (unsigned int) minfo.hblks); 			// This is the total number of chunks allocated with mmap.
	printf("ordblks : %u\r\n", (unsigned int) minfo.ordblks);		// This is the number of chunks not in use.
	printf("hblkhd : %u\r\n", (unsigned int) minfo.hblkhd);			// This is the total size of memory allocated with mmap, in bytes.
	printf("usmblks : %u\r\n", (unsigned int) minfo.usmblks); 		// This field is unused.
	printf("fsmblks : %u\r\n", (unsigned int) minfo.fsmblks); 		// This field is unused.
	printf("uordblks : %u\r\n", (unsigned int) minfo.uordblks);		// This is the total size of memory occupied by chunks handed out by malloc.
	printf("fordblks : %u\r\n", (unsigned int) minfo.fordblks);		// This is the total size of memory occupied by free (not in use) chunks.
	printf("keepcost : %u\r\n", (unsigned int) minfo.keepcost);		// This is the size of the top-most releasable chunk that normally borders the end of the heap.
	printf("*********************************************************\r\n");

	for (int i = 0; i < 4096; i++) {
		if (test[i] == NULL) {
			printf("free fail\n");
			break;
		} else {
			printf("%8u Free 0x%08X\n", i, test[i]);
			free(test[i]);
		}
	}

	minfo = mallinfo();
	printf("\r\n********************CPU%d Memory state********************\r\n",0);
	printf("arena : %u\r\n", (unsigned int) minfo.arena);			// This is the total size of memory allocated with sbrk by malloc, in bytes.
	printf("sbmlks : %u\r\n", (unsigned int) minfo.smblks);			// This field is unused.
	printf("hblks : %u\r\n", (unsigned int) minfo.hblks); 			// This is the total number of chunks allocated with mmap.
	printf("ordblks : %u\r\n", (unsigned int) minfo.ordblks);		// This is the number of chunks not in use.
	printf("hblkhd : %u\r\n", (unsigned int) minfo.hblkhd);			// This is the total size of memory allocated with mmap, in bytes.
	printf("usmblks : %u\r\n", (unsigned int) minfo.usmblks); 		// This field is unused.
	printf("fsmblks : %u\r\n", (unsigned int) minfo.fsmblks); 		// This field is unused.
	printf("uordblks : %u\r\n", (unsigned int) minfo.uordblks);		// This is the total size of memory occupied by chunks handed out by malloc.
	printf("fordblks : %u\r\n", (unsigned int) minfo.fordblks);		// This is the total size of memory occupied by free (not in use) chunks.
	printf("keepcost : %u\r\n", (unsigned int) minfo.keepcost);		// This is the size of the top-most releasable chunk that normally borders the end of the heap.
	printf("*********************************************************\r\n");
#endif

	char buf[64] = {0};
	snprintf(buf, 64, "PI:%.6f / test float printf\n", M_PI);
	flprintf("%s", buf);

	//int res = setTimeZone();
	//printf("res : %d\n", res);
	ULONG ledcnt = 0;
	while (1) {
		ledcnt++;

		if (ledcnt % 10 == 0) {
			static uint64_t k = 0;
			if (k++%2)	GpioSetOut(GPIO_LED0, GPIO_OUT_HI);
			else		GpioSetOut(GPIO_LED0, GPIO_OUT_LOW);
		}
#if 0
		if (ledcnt % 100 == 0) {
			static uint64_t k = 0;
			if (k++%2)	GpioSetOut(GPIO_LED1, GPIO_OUT_HI);
			else		GpioSetOut(GPIO_LED1, GPIO_OUT_LOW);
		}

		if (ledcnt % 50 == 0) {
			static uint64_t k = 0;
			if (k++%2)	GpioSetOut(GPIO_LED2, GPIO_OUT_HI);
			else		GpioSetOut(GPIO_LED2, GPIO_OUT_LOW);
		}
#endif
		vTaskDelay(1);
	}
	UNUSED(pvParameters);
}
