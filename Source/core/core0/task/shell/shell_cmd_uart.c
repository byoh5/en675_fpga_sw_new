//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include <string.h> // strcmp
#include <stdlib.h> // atoi

#include "enx_freertos.h"

#include "shell_cmd_uart.h"

const char *sPerlUart[]     = {"Check UART state",                               (char*)0};

#define SHELL_UART_TEST_BUF_SIZE	512
#define SHELL_UART_TEST_LOOP		5

static UINT arrUartTestSpeed[] = {1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 230400};//, 460800, 921600};
static UINT arrUartTestStopbit[] = {0, 1};		// 0: 1bit, 1: 2bit
static UINT arrUartTestParity[] = {0, 1, 2};	// 0: Disable, 1: Enable-even, 2: Enable-odd

typedef struct {
	UINT nCH;
	TaskHandle_t xNotityRX;
	QueueHandle_t qRx;

	UINT nCount;
	BYTE arrRxbuf[SHELL_UART_TEST_BUF_SIZE];

	UINT nSpeedIdx;
	UINT nStopbitIdx;
	UINT nParityIdx;
} ShellUartTest;

ShellUartTest uarttest;

static void uart_rx_irq_test(void *ctx)
{
	ShellUartTest *test = (ShellUartTest *)ctx;
	BYTE cGetdata;

	if (test->xNotityRX) {
		portBASE_TYPE xHigherPriorityTaskWoken1 = pdFALSE;
		portBASE_TYPE xHigherPriorityTaskWoken2 = pdFALSE;

		while (1) {
			if (UartRxIsEmpty(test->nCH) == 0) {
				cGetdata = UartRxGetByte(test->nCH);
//				printf("RX[0x%02X]\n", cGetdata);
				if (test->qRx) {
					xQueueSendFromISR(test->qRx, &cGetdata, &xHigherPriorityTaskWoken1);
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
			if (UartRxIsEmpty(test->nCH) == 0) {
				cGetdata = UartRxGetByte(test->nCH);
			} else {
				break;
			}
		}
	}
}

static void uart_tx_irq_test(void *ctx)
{
	ShellUartTest *test = (ShellUartTest *)ctx;
	UartTxIrqClear(test->nCH);
}

void uart_test_rx_fflush(ShellUartTest *test)
{
	BYTE rxchar;
	while (1) {
		if (xQueueReceive(test->qRx, &rxchar, 0) != pdTRUE) {
			break;
		}
	}
}

int uart_test_rx_message(ShellUartTest *test, char *buf, int max)
{
	BYTE rxchar;
	int idx = 0;
	char *base = buf;

	memset(buf, 0, max);
	max--;

	while (1) {
		if (ulTaskNotifyTake(pdTRUE, 250) != pdTRUE) {
			return -1;
		}

		while (1) {
			if (xQueueReceive(test->qRx, &rxchar, 0) != pdPASS) {
				break;
			}

			*buf = rxchar;
			if (!*buf) {
				*buf = 0;
//				hexDump("Rx message", base, idx + 1);
				return idx;
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

static int uart_test_tx_message(UINT nCH, char *buf)
{
	int nLen;

	for (nLen = 0; buf[nLen] != 0; nLen++) {
		if (UartTxIsFull(nCH) == 1) {
			vTaskDelay(1);
		}
		UartTxSetByte(nCH, buf[nLen]);
	}

	// send null byte
	if (UartTxIsFull(nCH) == 1) {
		vTaskDelay(1);
	}
	UartTxSetByte(nCH, 0);
	nLen++;

	return nLen;
}

static void uart_test_task(void *ctx)
{
	ShellUartTest *test = (ShellUartTest *)ctx;

	printf("uart_test_task start\n");

	// Init
	test->nSpeedIdx = 0;
	test->nStopbitIdx = 0;
	test->nParityIdx = 0;

	UartSetClk(test->nCH, 115200);
	UartSetTxType(test->nCH, 0);
	UartSetStopbit(test->nCH, 0);
	UartSetParityMode(test->nCH, UART_PARITY_NONE);

	UartRxIrqCallback(test->nCH, uart_rx_irq_test, ctx);
	UartRxSetIrqEn(test->nCH, ENX_ON);
//	UartTxIrqCallback(test->nCH, uart_tx_irq_test, ctx);
//	UartTxSetIrqEn(test->nCH, ENX_ON);

	printf("uart_test_task init\n");

	if (test->qRx == NULL) {
		test->qRx = xQueueCreate(512, sizeof(char));
		if (test->qRx == NULL) {
			printf("Queue allocation failed.\n");
			goto done;
		}
	}

	char strTxbuf[128] = {0};
	char strRxbuf[128] = {0};
	int nTxlen, nRxlen;

	// Init message
	sprintf(strTxbuf, "$init");
	uart_test_rx_fflush(test);
	nTxlen = uart_test_tx_message(test->nCH, strTxbuf);			// Init message TX
	nRxlen = uart_test_rx_message(test, strRxbuf, 128);			// Init message RX
	if (nRxlen == -1) {
		printf("Init message timeout\n");
		goto done;
	}
	if (strcmp(strTxbuf, strRxbuf) != 0) {						// Check
		printf("Init message fail\n");
		goto done;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int timeout = 0;
	for (int i = 0; ARRAY_SIZE(arrUartTestSpeed) > i; i++) {
		for (int j = 0; ARRAY_SIZE(arrUartTestStopbit) > j; j++) {
			for (int k = 0; ARRAY_SIZE(arrUartTestParity) > k; k++) {
				// Set mode,speed,stopbit,parityen,paritytype
				printf("%7ubps Stopbit(%ubit) Parity(%4s) -   0%%", arrUartTestSpeed[i], arrUartTestStopbit[j] == 0 ? 1 : 2,
						arrUartTestParity[k] == 0 ? "None" : arrUartTestParity[k] == 1 ? "Even" : arrUartTestParity[k] == 2 ? "Odd" : "Error");
				sprintf(strTxbuf, "$mode,%u,%u,%u", arrUartTestSpeed[i], arrUartTestStopbit[j] == 0 ? 1 : 2, arrUartTestParity[k]);
				uart_test_rx_fflush(test);
				nTxlen = uart_test_tx_message(test->nCH, strTxbuf);		// Mode message TX
				nRxlen = uart_test_rx_message(test, strRxbuf, 128);		// Mode message RX
				if (nRxlen == -1) {
					printf("\b\b\b\b");
					_Rprintf("Fail");
					printf(" - mode message timeout\n");
					timeout = 1;
					goto timeout;
				}
				if (strncmp("$set", strRxbuf, 4) != 0) {
					printf("\b\b\b\b");
					_Rprintf("Fail");
					printf(" - mode message fail\n");
					timeout = 1;
					goto timeout;
				}

				UartSetClk(test->nCH, arrUartTestSpeed[i]);
				UartSetStopbit(test->nCH, arrUartTestStopbit[j]);
				if (arrUartTestParity[k] == 0) {
					UartSetParityMode(test->nCH, UART_PARITY_NONE);
				} else {
					if (arrUartTestParity[k] == 1) {
						UartSetParityMode(test->nCH, UART_PARITY_EVEN);
					} else if (arrUartTestParity[k] == 2) {
						UartSetParityMode(test->nCH, UART_PARITY_ODD);
					} else {
						printf("\b\b\b\b");
						_Rprintf("error code\n");
					}
				}
				vTaskDelay(100);

				// Data message
				int loop, processrate = 0;
				for (loop = 0; loop < SHELL_UART_TEST_LOOP; loop++) {
					sprintf(strTxbuf, "$data,0123456789012345678901234");
					uart_test_rx_fflush(test);
					nTxlen = uart_test_tx_message(test->nCH, strTxbuf);		// Data message TX
					nRxlen = uart_test_rx_message(test, strRxbuf, 128);		// Data message RX
					if (nRxlen == -1) {
						printf("\b\b\b\b");
						_Rprintf("Fail");
						printf(" - data message timeout\n");
						timeout = 1;
						goto timeout;
					}
					if (strcmp(strTxbuf, strRxbuf) != 0) {
						printf("\b\b\b\b");
						_Rprintf("Fail");
						printf(" - data message not equal\n");
						hexCmpDump("DATA", strTxbuf, strRxbuf, MAX(nTxlen, nRxlen));
						timeout = 1;
						goto timeout;
					} else {
						timeout = 0;
						if (processrate != (loop * 100 / SHELL_UART_TEST_LOOP)) {
							processrate = loop * 100 / SHELL_UART_TEST_LOOP;
							char buf[64] = {0};
							snprintf(buf, 64, "%3u%%", processrate);
							printf("\b\b\b\b");
							printf("%s", buf);
						}
					}
				}
				if (loop == SHELL_UART_TEST_LOOP) {
					printf("\b\b\b\b");
					_Gprintf("OK  \n");
				}

timeout:
				if (timeout == 1) {
					timeout = 0;

					// reset
					UartSetClk(test->nCH, 115200);
					UartSetTxType(test->nCH, 0);
					UartSetStopbit(test->nCH, 0);
					UartSetParityMode(test->nCH, UART_PARITY_NONE);

					while (1) {
						sprintf(strTxbuf, "$ping");
						uart_test_rx_fflush(test);
						nTxlen = uart_test_tx_message(test->nCH, strTxbuf);		// Mode message TX
						nRxlen = uart_test_rx_message(test, strRxbuf, 128);		// Mode message RX
						if (strncmp("$ping", strRxbuf, 6) == 0) {
							break;
						}
						vTaskDelay(100); // reset command 3sec
					}
				}
			}
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

done:
	printf("UART test task done!\n");
	vQueueDelete(test->qRx);
	test->qRx = NULL;
	test->xNotityRX = NULL;
	vTaskDelete(NULL);
}

static void uart_status_print(char *title, UINT type, UINT (*func)(UINT ch), char *text_type[4])
{
	if (type == 1) {
		_Gprintf("%-8s|", title);
	} else {
		printf("%-8s|", title);
	}
	for (uint32 i = 0; i < UART_CNT; i++) {
		if (type == 162) {
			printf("    0x%02X |", func(i));
		} else if (type == 165) {
			printf(" 0x%05X |", func(i));
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

int cmd_perl_uart(int argc, char *argv[])
{
	// uart0: gpio	4,5		0,1
	// uart1: gpio	12,13	8,9
	// uart2: gpio	20,21	16,17
	// uart3: gpio	28,29	24,25
	// uart4: gpio	36,37	32,33
	// uart5: gpio	44,45	40,41
	// uart6: gpio	52,53	48,49
	// uart7: gpio	60,61	56,57
	// uart8: gpio	68,69	64,65

	if (argc == 1) {
		uart_status_print("UART", 1, NULL, NULL);
		uart_status_print("", 0, NULL, NULL);
		uart_status_print("Control", 1, NULL, NULL);
#if EN675_SINGLE
		uart_status_print("SYNC", 10, UartGetSync, NULL);
#endif
		uart_status_print("STOP_BIT", 0xff, UartGetStopbit, ((char*[4]){"1bit", "2bit", "err", "err"}));
		uart_status_print("PARITY", 0xff, UartGetParityMode, ((char*[4]){"None", "None", "Even", "Odd"}));
		uart_status_print("CLK_DIV", 10, UartGetClkdiv, NULL);
		uart_status_print("Clock", 10, UartGetClk, NULL);
		uart_status_print("Data", 1, NULL, NULL);
		uart_status_print("TX_DAT", 162, UartTxGetByte, NULL);
		uart_status_print("RX_DAT", 162, UartRxGetByte, NULL);
		uart_status_print("TX", 1, NULL, NULL);
		uart_status_print("TX_TYPE", 0xff, UartGetTxType, ((char*[4]){"Open", "Pushpull", "err", "err"}));
		uart_status_print("EMPTY", 10, UartTxIsEmpty, NULL);
		uart_status_print("FULL", 10, UartTxIsFull, NULL);
		uart_status_print("IRQ_EN", 10, UartTxGetIrqEn, NULL);
		uart_status_print("IRQ", 10, UartTxIsIrq, NULL);
		uart_status_print("RX", 1, NULL, NULL);
		uart_status_print("RX_LMT", 165, UartRxGetLmt, NULL);
		uart_status_print("EMPTY", 10, UartRxIsEmpty, NULL);
		uart_status_print("FULL", 10, UartRxIsFull, NULL);
		uart_status_print("IRQ_EN", 10, UartRxGetIrqEn, NULL);
		uart_status_print("IRQ", 10, UartRxIsIrq, NULL);
		uart_status_print("--------", 1, NULL, NULL);
	} else if (argc >= 2) {
		if (strcmp(argv[1], "idx") == 0) {
			if (argc == 3) {
				printf("Old select UART(%d)\n", uarttest.nCH);
				uarttest.nCH = atoi(argv[2]);
				if (uarttest.nCH > 8 || uarttest.nCH < 0) {
					printf("Invalid channel selection.(%d)\n", uarttest.nCH);
					uarttest.nCH = -1;
				} else {
					printf("New select UART(%d)\n", uarttest.nCH);
				}
			} else {
				printf("Select UART(%d)\n", uarttest.nCH);
			}
			return 0;
		} else if (strcmp("auto", argv[1]) == 0) {
			if (uarttest.xNotityRX == NULL) {
				printf("UART test task create!\n");
				uarttest.xNotityRX = vTaskCreate("shUart", uart_test_task, &uarttest, LV3_STACK_SIZE, LV3_TASK_PRIO);
			} else {
				printf("UART test task already create!\n");
			}
			return 0;
		} else if (strcmp("stop", argv[1]) == 0) {
			if (uarttest.xNotityRX) {
				vTaskDelete(uarttest.xNotityRX);
				uarttest.xNotityRX = NULL;
				printf("UART test task kill!\n");
			} else {
				printf("UART test task already kill!\n");
			}
			return 0;
		}

		if (uarttest.nCH > 8 || uarttest.nCH < 0) {
			printf("Select a channel!\n");
			return 0;
		}

		if (strcmp("init", argv[1]) == 0) {
			UartInit(uarttest.nCH, 115200);
		} else if (strcmp("deinit", argv[1]) == 0) {
			UartDeinit(uarttest.nCH);
		} else if (strcmp("clk", argv[1]) == 0) {
			UINT Speed_Hz = 0, flag = 0;
			if (argc == 3) {
				flag = 1;
				Speed_Hz = atoi(argv[2]);
				UartSetClk(uarttest.nCH, Speed_Hz);
			} else if (argc == 2) {

			} else {
				Shell_Unknown();
				return 0;
			}
			Speed_Hz = UartGetClk(uarttest.nCH);
			printf("%s UART Speed: %uHz\n", flag == 0 ? "Get" : "Set", Speed_Hz);
		} else if (strcmp("div", argv[1]) == 0) {
			UINT Div = 0, flag = 0;
			if (argc == 3) {
				flag = 1;
				Div = atoi(argv[2]);
				UartSetClkdiv(uarttest.nCH, Div);
			} else if (argc == 2) {

			} else {
				Shell_Unknown();
				return 0;
			}
			Div = UartGetClkdiv(uarttest.nCH);
			printf("%s UART Div: %u\n", flag == 0 ? "Get" : "Set", Div);
		} else if (argc == 3 && strcmp("tx", argv[1]) == 0) {
			char tx = argv[2][0];
			if (UartTxIsFull(uarttest.nCH) == 1) {
				printf("UART%u, send full\n", uarttest.nCH);
			} else {
				printf("UART%u send '%c'\n", uarttest.nCH, tx);
				UartTxSetByte(uarttest.nCH, tx);
			}
		} else if (strcmp("rx", argv[1]) == 0) {
			if (UartRxIsEmpty(uarttest.nCH) == 1) {
				printf("UART%u, recv empty\n", uarttest.nCH);
			} else {
				UINT rx = UartRxGetByte(uarttest.nCH);
				printf("UART%u, recv '%c'\n", uarttest.nCH, (char)rx);
			}
#if EN675_SINGLE
		} else if (strcmp("sync", argv[1]) == 0) {
			if (argc == 2) {
				UINT getdValue = UartGetSync(uarttest.nCH);
				printf("UartGetSync(%d)\n", getdValue);
			} else {
				UINT getdValue = atoi(argv[2]);
				UartSetSync(uarttest.nCH, getdValue);
				printf("input(%d) => set (%d)\n", getdValue, UartGetSync(uarttest.nCH));
			}
#endif
		} else if (strcmp("irq", argv[1]) == 0) {
			if (strcmp("rx", argv[2]) == 0) {
				uart_getset(UartRxGetIrqEn, UartRxSetIrqEn, uarttest.nCH);
			} else if (strcmp("tx", argv[2]) == 0) {
				uart_getset(UartTxGetIrqEn, UartTxSetIrqEn, uarttest.nCH);
			}
		}
	} else {
		Shell_Unknown();
	}

	return 0;
}
