//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include <string.h> // strcmp
#include <stdlib.h> // atoi

#include "enx_freertos.h"

#include "shell_cmd_spi.h"

const char *sPerlSpi[]     = {"Check SPI state",                               (char*)0};

#define SHELL_UART_MSG_BUF_SIZE	128
#define SHELL_UART_MSG_LOOP		5

typedef struct {
	UINT nCH;		// SPI ch
	UINT nUARTCH;	// UART command message ch
	TaskHandle_t xNotityRX;
	QueueHandle_t qMessageRx;

	BYTE arrMessageRxbuf[SHELL_UART_MSG_BUF_SIZE];

	UINT nLoop;
	UINT nBufSize;
	BYTE *arrBufRX;
	BYTE *arrBufTX;
} ShellSpiTest;

ShellSpiTest spitest = {
	.nCH = 8,
};

static unsigned char test_make_chksum(unsigned char *data, unsigned int size)
{
	unsigned char cel_chksum = 0;
	unsigned int i;

	for (i = 0; i < size; i++) {
		cel_chksum ^= data[i];
	}

	return cel_chksum;
}

static int test_check_chksum(unsigned char *data, unsigned int size)
{
	unsigned char cel_chksum = 0;
	unsigned char get_chksum = 0;
	unsigned int i;

	if (size < 3) {
//		printf("fail0(size:%d))\n", size);
		return -1;
	}

	if (data[size-3] != '*') {
//		printf("fail1(* != %c)\n", data[size-3]);
		return -1;
	}

	get_chksum = strtol((char *)&data[size-2], 0, 16);

	for (i = 0; i < (size-3); i++) {
//		printf("[%c]", data[i]);
		cel_chksum ^= data[i];
	}

	if (cel_chksum != get_chksum) {
//		printf("fail2(0x%02X/0x%02X)\n", cel_chksum, get_chksum);
		return -1;
	}

	return 0;
}


static void uart_rx_irq_test(void *ctx)
{
	ShellSpiTest *test = (ShellSpiTest *)ctx;
	BYTE cGetdata;

	if (test->xNotityRX) {
		portBASE_TYPE xHigherPriorityTaskWoken1 = pdFALSE;
		portBASE_TYPE xHigherPriorityTaskWoken2 = pdFALSE;

		while (1) {
			if (UartRxIsEmpty(test->nUARTCH) == 0) {
				cGetdata = UartRxGetByte(test->nUARTCH);
//				printf("RX[0x%02X]\n", cGetdata);
				if (test->qMessageRx) {
					xQueueSendFromISR(test->qMessageRx, &cGetdata, &xHigherPriorityTaskWoken1);
				}
			} else {
				break;
			}
		}

		vTaskNotifyGiveFromISR(test->xNotityRX, &xHigherPriorityTaskWoken2);
		if (xHigherPriorityTaskWoken1 || xHigherPriorityTaskWoken2) {
			gbXsrTaskSwitchNeeded = 1;
		}
	} else {
		while (1) {
			if (UartRxIsEmpty(test->nUARTCH) == 0) {
				cGetdata = UartRxGetByte(test->nUARTCH);
			} else {
				break;
			}
		}
	}
}

void uart_rx_fflush(ShellSpiTest *test)
{
	BYTE rxchar;
	while (1) {
		if (xQueueReceive(test->qMessageRx, &rxchar, 0) != pdTRUE) {
			break;
		}
	}
}

int uart_rx_message(ShellSpiTest *test, char *buf, int max)
{
	BYTE rxchar;
	int idx = 0;
	char *base = buf;

	memset(buf, 0, max);
	max--;

	while (1) {
		if (ulTaskNotifyTake(pdTRUE, 1000) != pdTRUE) {
			return -100;
		}

		while (1) {
			if (xQueueReceive(test->qMessageRx, &rxchar, 0) != pdPASS) {
				break;
			}

			*buf = rxchar;
			if (!*buf) {
				*buf = 0;
//				hexDump("Rx message", base, idx + 1);
				if (test_check_chksum((unsigned char *)base, idx) == 0) {
					buf[-3] = 0;
					return idx - 3;
				} else {
					return -200;
				}
			}

			buf++;

			if (++idx >= max) {
				printf("\nInput too long (exceeds %d bytes).\n", max + 1);
				*buf = 0;
				return 0;
			}
		}
	}

	return 0;
}

static int uart_tx_message(UINT nCH, char *buf)
{
	int nLen;
	char chksumbuf[4];

	for (nLen = 0; buf[nLen] != 0; nLen++) {
		if (UartTxIsFull(nCH) == 1) {
			vTaskDelay(1);
		}
		UartTxSetByte(nCH, buf[nLen]);
	}

	unsigned char chksum = test_make_chksum((unsigned char *)buf, nLen);
	sprintf(chksumbuf, "*%02x", chksum);

	while (UartTxIsFull(nCH) == 1) {
		vTaskDelay(1);
	}
	UartTxSetByte(nCH, chksumbuf[0]);

	while (UartTxIsFull(nCH) == 1) {
		vTaskDelay(1);
	}
	UartTxSetByte(nCH, chksumbuf[1]);

	while (UartTxIsFull(nCH) == 1) {
		vTaskDelay(1);
	}
	UartTxSetByte(nCH, chksumbuf[2]);

	// send null byte
	while (UartTxIsFull(nCH) == 1) {
		vTaskDelay(1);
	}
	UartTxSetByte(nCH, 0);
	nLen++;

	return nLen;
}

static void spi_test_task(void *ctx)
{
	ShellSpiTest *test = (ShellSpiTest *)ctx;
	test->arrBufRX = NULL;

	test->arrBufTX = NULL;

	_Cprintf("SPI%u self-test Task(Size:%lu)(Count:%u)\n", test->nCH, test->nBufSize, test->nLoop);

	// Command UART init
	test->nUARTCH = 8;
//	UartSetTxType(test->nUARTCH, 1);
	UartSetClk(test->nUARTCH, 9600);
//	UartSetTxType(test->nUARTCH, 0);
//	UartSetStopbit(test->nUARTCH, 0);
//	UartSetParityMode(test->nUARTCH, UART_PARITY_NONE);
	UartRxIrqCallback(test->nUARTCH, uart_rx_irq_test, ctx);
	UartRxSetIrqEn(test->nUARTCH, ENX_ON);

	test->arrBufTX = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(test->nBufSize));
	if (test->arrBufTX == NULL) {
		_Rprintf("malloc error(arrBufTX), size(%u)\n", ENX_MEM_ALIGN_BUFFER(test->nBufSize));
		goto done;
	}

	test->arrBufRX = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(test->nBufSize));
	if (test->arrBufRX == NULL) {
		_Rprintf("malloc error(arrBufRX), size(%u)\n", ENX_MEM_ALIGN_BUFFER(test->nBufSize));
		goto done;
	}

	BYTE *parrTX = (BYTE *)ENX_MEM_ALIGN(test->arrBufTX);
	BYTE *parrRX = (BYTE *)ENX_MEM_ALIGN(test->arrBufRX);

	printf("SPI Memory TX(0x%08lX) RX(0x%08lX)\n", (intptr_t)parrTX, (intptr_t)parrRX);

	if (test->qMessageRx == NULL) {
		test->qMessageRx = xQueueCreate(512, sizeof(char));
		if (test->qMessageRx == NULL) {
			_Rprintf("Queue allocation failed.\n");
			goto done;
		}
	}

	char strTxbuf[128] = {0};
	char strRxbuf[128] = {0};
	int nTxlen, nRxlen;

	// Init message
init_start:
	sprintf(strTxbuf, "$init");
	uart_rx_fflush(test);
	nTxlen = uart_tx_message(test->nUARTCH, strTxbuf);		// Init message TX
	nRxlen = uart_rx_message(test, strRxbuf, 128);			// Init message RX
	if (nRxlen == -100) {
//		printf("Init message timeout\n");
		goto init_start;
	}
	if (nRxlen == -200) {
//		printf("Init message error chksum\n");
		goto init_start;
	}
	if (strncmp("$retry", strRxbuf, 6) == 0) {				// Check
//		printf("Init message retry\n");
		goto init_start;
	}
	if (strcmp(strTxbuf, strRxbuf) != 0) {					// Check
//		printf("Init message fail(%d/%d)\n", nTxlen, nRxlen);
//		hexCmpDump("INIT err", strTxbuf, strRxbuf, max(nTxlen, nRxlen));
		goto init_start;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UINT fail = 0, pass = 0;
	ULONG stime, txsum, rxsum, flag;
	char strSpdbuf[40] = {0};

	// mode
	// 1. MSB/LSB first
	// 2. Byte/Bit mode
	// 3. LOW/HIGH and POSI/NEGA
	// 4. 8/16/24/32bit
#if 0
	extern void SpiSetBitmode(UINT nCH, SPI_BITMODE bitmode);
	extern SPI_BITMODE SpiGetBitmode(UINT nCH);
	extern void SpiSetOnebitmode(UINT nCH, SPI_ONEBITMODE onebitmode);
	extern SPI_ONEBITMODE SpiGetOneBitmode(UINT nCH);
	extern void SpiSetClkmode(UINT nCH, SPI_CLKDIR_MODE clkmode);
	extern SPI_CLKDIR_MODE SpiGetClkmode(UINT nCH);
	extern void SpiSetWs(UINT nCH, SPI_WORD_SIZE wdsize);
	extern SPI_WORD_SIZE SpiGetWs(UINT nCH);
#endif

#if 0
	// msb/lsb
	// low/high and posi/nega
	// 8/16/24/32
#if 0
	typedef enum {
		SPI_WS_8BIT = 0,
		SPI_WS_16BIT = 1,
		SPI_WS_24BIT = 2,
		SPI_WS_32BIT = 3,
	} SPI_WORD_SIZE;
#endif
	//for (UINT nWordSize = SPI_WS_8BIT; nWordSize <= SPI_WS_32BIT; nWordSize++) {
	for (UINT nWordSize = SPI_WS_8BIT; nWordSize <= SPI_WS_16BIT; nWordSize++) {
		SpiSetWs(test->nCH, nWordSize);
	}


	sprintf(strTxbuf, "$mode,%u,%u,%u", 0, 0, 0, 0);
	uart_rx_fflush(test);
	nTxlen = uart_tx_message(test->nUARTCH, strTxbuf);		// Mode message TX
	nRxlen = uart_rx_message(test, strRxbuf, 128);			// Mode message RX
	if (nRxlen == -1) {
		printf("\b\b\b\b");
		_Rprintf("Fail");
		printf(" - mode message timeout\n");
//		timeout = 1;
//		goto timeout;
	}
	if (strncmp("$set", strRxbuf, 4) != 0) {
		printf("\b\b\b\b");
		_Rprintf("Fail");
		printf(" - mode message fail\n");
//		timeout = 1;
//		goto timeout;
	}

	// mode ���� time~
	vTaskDelay(100);

	//SpiSetWs(test->nCH, SPI_WS_8BIT);


#endif

	for (UINT nLoop = 0; nLoop < test->nLoop; nLoop++) {
		txsum = rxsum = flag = 0;

		// Test message (data tx/rx)
		UINT test_size = rand() % test->nBufSize;
		if (test_size <= SpiGetWs(test->nCH)) {
			test_size = SpiGetWs(test->nCH) + 1; // �ּ����� SPI word size��ŭ�� ����
		}

		printf("%3u%% %5u/%5u %4ubyte - ", ((nLoop + 1) * 100) / test->nLoop, nLoop + 1, test->nLoop, test_size);

test_start:
		sprintf(strTxbuf, "$test,%u", test_size);
		uart_rx_fflush(test);
		nTxlen = uart_tx_message(test->nUARTCH, strTxbuf);		// Test message TX
		nRxlen = uart_rx_message(test, strRxbuf, 128);			// Test message RX
		if (nRxlen == -100) {
//			printf("Test message timeout\n");
			goto test_start;
		}
		if (nRxlen == -200) {
//			printf("Test message error chksum\n");
//			hexDump("RX messsage", strRxbuf, nRxlen);
			goto test_start;
		}
		if (strncmp("$retry", strRxbuf, 6) == 0) {					// Check
//			printf("Test message retry\n");
			goto test_start;
		}
		if (strncmp("$set", strRxbuf, 4) != 0) {					// Check
//			printf("Test message fail\n", strRxbuf);
//			hexDump("RX messsage", strRxbuf, nRxlen);
			goto test_start;
		}

		printf(".");

		for (UINT i = 0; i < test_size; i++) {
			parrTX[i] = rand() % 256;
		}

		stime = BenchTimeStart();
		SpiSetCs(test->nCH, SPI_CS_OUT_LOW);
		for (UINT i = 0; i < test_size; i++) {
			SpiWrite(test->nCH, &parrTX[i]);
		}
		SpiSetCs(test->nCH, SPI_CS_OUT_HIGH);
		txsum += BenchTimeStop(stime);

		printf(".");

		vTaskDelay(1);

		stime = BenchTimeStart();
		SpiSetCs(test->nCH, SPI_CS_OUT_LOW);
		for (UINT i = 0; i < test_size; i++) {
			SpiRead(test->nCH, &(parrRX[i]));
		}
		SpiSetCs(test->nCH, SPI_CS_OUT_HIGH);
		rxsum += BenchTimeStop(stime);

		printf(".");

		for (int cklop = 0; cklop < test_size; cklop++) {
			if (parrRX[cklop] != parrTX[cklop]) {
				flag = 1;
				break;
			}
		}

		snprintf(strSpdbuf, 40, "TX(%8.2fKB/s) RX(%8.2fKB/s) - ", (test_size / 1024.0) / (txsum / 1000.0 / 1000.0), (test_size / 1024.0) / (rxsum / 1000.0 / 1000.0));
		printf(strSpdbuf);
		if (flag == 1) {
			_Rprintf("Fail\n");
			hexCmpDump("SPI Test", parrTX, parrRX, test_size);
			fail++;
		} else {
			_Gprintf(" OK \n");
			pass++;
		}

		vTaskDelay(1);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

done:
	printf("SPI test task done!\n");
	if (test->arrBufRX) {
		vPortFree(test->arrBufRX);
	}
	if (test->arrBufTX) {
		vPortFree(test->arrBufTX);
	}
	vQueueDelete(test->qMessageRx);
	test->arrBufRX = NULL;
	test->arrBufTX = NULL;
	test->qMessageRx = NULL;
	test->xNotityRX = NULL;
	vTaskDelete(NULL);
}

static void spi_status_print(char *title, UINT type, UINT (*func)(UINT ch), char *text_type[4])
{
	if (type == 1) {
		_Gprintf("%-8s|", title);
	} else {
		printf("%-8s|", title);
	}
	for (uint32 i = 0; i < SPI_CNT; i++) {
		if (type == 16) {
			printf("%08Xh|", func(i));
		} else if (type == 10) {
			printf("%8u |", func(i));
		} else if (type == 0xff) {
			printf("%8s |", text_type[func(i)]);
		} else if (type == 1) {
			_Gprintf("---------|");
		} else if (type == 0) {
			printf("     CH%u |", i);
		}
	}
	printf("\n");
}

int cmd_perl_spi(int argc, char *argv[])
{
	if (argc == 1) {
		spi_status_print("SPI", 1, NULL, NULL);
		spi_status_print("", 0, NULL, NULL);
		spi_status_print("Control", 1, NULL, NULL);
		spi_status_print("EN", 0xff, SpiGetEn, ((char*[4]){"OFF", "ON", "err", "err"}));
		spi_status_print("BIT_MODE", 0xff, SpiGetBitmode, ((char*[4]){"MSB", "LSB", "err", "err"}));
		spi_status_print("CS", 0xff, SpiGetCs, ((char*[4]){"Out-Low", "Out-High", "High-z", "err"}));
		spi_status_print("ONEBIT", 0xff, SpiGetOneBitmode, ((char*[4]){"BYTE", "BIT", "err", "err"}));
		spi_status_print("CLK_MODE", 0xff, SpiGetClkmode, ((char*[4]){"Low+Pos", "Low+Neg", "High+Pos", "High+Neg"}));
		spi_status_print("WS", 0xff, SpiGetWs, ((char*[4]){"8b", "16b", "24b", "32b"}));
		spi_status_print("RW", 0xff, SpiGetRW, ((char*[4]){"err", "Write", "Read", "R/W"}));
		spi_status_print("GO", 10, SpiGetGo, NULL);
		spi_status_print("IRQ_EN", 10, SpiGetIrqEn, NULL);
		spi_status_print("IRQ", 10, SpiIsIrq, NULL);
		spi_status_print("CLK_DIV", 10, SpiGetClkdiv, NULL);
		spi_status_print("Clock", 10, SpiGetClk, NULL);
		spi_status_print("Data", 1, NULL, NULL);
		spi_status_print("TX_DAT", 16, SpiGetTxdata, NULL);
		spi_status_print("RX_DAT", 16, SpiGetRxdata, NULL);
		spi_status_print("--------", 1, NULL, NULL);
	} else if (argc >= 2) {
		if (strcmp(argv[1], "idx") == 0) {
			if (argc == 3) {
				printf("Old select SPI(%d)\n", spitest.nCH);
				spitest.nCH = atoi(argv[2]);
				if (spitest.nCH > 8 || spitest.nCH < 0) {
					printf("Invalid channel selection.(%d)\n", spitest.nCH);
					spitest.nCH = -1;
				} else {
					printf("New select SPI(%d)\n", spitest.nCH);
				}
			} else {
				printf("Select SPI(%d)\n", spitest.nCH);
			}
			return 0;
		} else if (strcmp("auto", argv[1]) == 0) {
			if (spitest.xNotityRX == NULL) {
				printf("SPI test task create!\n");

				spitest.nBufSize = 1024;
				spitest.nLoop = 100;
				spitest.xNotityRX = vTaskCreate("shSPI", spi_test_task, &spitest, LV3_STACK_SIZE, LV3_TASK_PRIO);
			} else {
				printf("SPI test task already create!\n");
			}
			return 0;
		} else if (strcmp("stop", argv[1]) == 0) {
			if (spitest.xNotityRX) {
				vTaskDelete(spitest.xNotityRX);
				spitest.xNotityRX = NULL;
				printf("SPI test task kill!\n");
			} else {
				printf("SPI test task already kill!\n");
			}
			return 0;
		}

		if (spitest.nCH > 8 || spitest.nCH < 0) {
			printf("Select a channel!\n");
			return 0;
		}

		if (strcmp(argv[1], "clk") == 0) {
			UINT Speed_Hz = 0, flag = 0;
			if (argc == 3) {
				flag = 1;
				Speed_Hz = atoi(argv[2]);
				SpiSetClk(8, Speed_Hz);
			} else if (argc == 2) {

			} else {
				Shell_Unknown();
				return 0;
			}
			Speed_Hz = SpiGetClk(8);
			printf("%s SPI Speed: %uHz\n", flag == 0 ? "Get" : "Set", Speed_Hz);
		} else if (strcmp(argv[1], "cs") == 0) {
//			UINT in = atoi(argv[2]);
//			printf("SPI CS %u\n", in);
//			SpiSetCs(8, in);
		} else if (strcmp(argv[1], "tx") == 0) {
			BYTE aa[16] = "abcdefgh";
			BYTE bb[16] = {0};
			UINT size = 8;

			SpiSetWs(8, SPI_WS_16BIT);

			size = size / (SpiGetWs(8)+1);

			SpiSetCs(8, SPI_CS_OUT_LOW);
			for (int i = 0; i < size; i++) {
				SpiWrite(8, &(aa[i*2]));
			}
			SpiSetCs(8, SPI_CS_OUT_HIGH);

			vTaskDelay(10);

			SpiSetCs(8, SPI_CS_OUT_LOW);
			for (int i = 0; i < size; i++) {
				SpiRead(8, &(bb[i*2]));
			}
			SpiSetCs(8, SPI_CS_OUT_HIGH);

			hexDump("AA", aa, 16);
			hexDump("BB", bb, 16);
		}
	} else {
		Shell_Unknown();
	}

	return 0;
}
