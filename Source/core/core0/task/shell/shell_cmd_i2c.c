//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include <string.h> // strcmp
#include <stdlib.h> // atoi

#include "enx_freertos.h"

#include "shell_cmd_i2c.h"

const char *sPerlI2c[]      = {"Check I2C state",                                (char*)0};

typedef struct {
	UINT nCH;		// I2C ch
//	UINT nUARTCH;	// UART command message ch
//	TaskHandle_t xNotityRX;
//	QueueHandle_t qMessageRx;

//	BYTE arrMessageRxbuf[SHELL_UART_MSG_BUF_SIZE];

//	UINT nLoop;
//	UINT nBufSize;
//	BYTE *arrBufRX;
//	BYTE *arrBufTX;
} ShellI2cTest;

ShellI2cTest i2ctest = {
	.nCH = 8,
};

#if 0
#define i2c_getset(get_func, set_func, ch)		{	if (argc == 2) { \
														UINT getdValue = get_func(ch); \
														printf(#get_func"(%d)\n", getdValue); \
													} else { \
														UINT getdValue = atoi(argv[2]); \
														set_func(ch, getdValue); \
														printf("input(%d) => set (%d)\n", getdValue, get_func(ch)); \
													} \
												}
#endif
static void __i2c_getset(UINT (*get_func)(UINT nCH), void (*set_func)(UINT nCH, UINT nVal), UINT nVal, UINT nCH)
{
	if (nVal == -1) {

	}
}

int cmd_perl_i2c(int argc, char *argv[])
{
	if (argc == 1) {
		printf("I2C Status =======================================================================================\n");
		printf("    |               |        |       |     |       |     |   Master  |           Slave           |\n");
		printf("    | BaudRate(div) |  Mode  | Fisrt | ACT | IRQEN | IRQ | COL | ACK | SDA | SCL | ACK_IN | Addr |\n");
		printf("----|---------------|--------|-------|-----|-------|-----|-----|-----|-----|-----|--------|------|\n");
		for (uint32 i = 0; i < I2C_CNT; i++) {
			printf("I2C%u| %8u(%3u) | %6s |  %3s  |  %u  |  %3s  |  %u  |  %u  |  %u  |  %u  |  %u  |   %u    | 0x%02X |\n", i,
					I2cGetClk(i), I2cGetClkdiv(i),
					I2cGetMode(i) == I2C_Master ? "Master" : " Slave",
					I2cGetBitmode(i) == I2C_MSBfirst ? "MSB" : "LSB",
					I2cGetACT(i),
					I2cGetIrqEn(i) == ENX_ON ? " ON" : "OFF", I2cIsIrq(i),
					I2cGetMstCol(i), I2cGetMstAck(i),
					I2cGetSlvSDA(i), I2cGetSlvSCL(i), I2cGetSlvAckIn(i), I2cGetSlvAddr(i));
		}
		printf("==================================================================================================\n");
	} else if (argc >= 2) {
		if (strcmp(argv[1], "idx") == 0) {
			if (argc == 3) {
				printf("Old select I2C(%d)\n", i2ctest.nCH);
				i2ctest.nCH = atoi(argv[2]);
				if (i2ctest.nCH >= I2C_CNT || i2ctest.nCH < 0) {
					printf("Invalid channel selection.(%d)\n", i2ctest.nCH);
					i2ctest.nCH = -1;
				} else {
					printf("New select I2C(%d)\n", i2ctest.nCH);
				}
			} else {
				printf("Select I2C(%d)\n", i2ctest.nCH);
			}
			return 0;
		}

		// 하위 command 필수 검사 요소
		if (i2ctest.nCH >= I2C_CNT || i2ctest.nCH < 0) {
			printf("Select a channel!\n");
			return 0;
		}

		if (strcmp(argv[1], "div") == 0) {
#if 0
			i2c_getset(I2cGetClkdiv, I2cSetClkdiv, i2ctest.nCH);
		} else if (strcmp(argv[1], "clk") == 0) {		i2c_getset(I2cGetClk, I2cSetClk, i2ctest.nCH);
		} else if (strcmp(argv[1], "mode") == 0) {		i2c_getset(I2cGetMode, I2cSetMode, i2ctest.nCH);
		} else if (strcmp(argv[1], "bit") == 0) {		i2c_getset(I2cGetBitmode, I2cSetBitmode, i2ctest.nCH);
		} else if (strcmp(argv[1], "slvadr") == 0) {	i2c_getset(I2cGetSlvAddr, I2cSetSlvAddr, i2ctest.nCH);
		} else if (strcmp(argv[1], "irq") == 0) {		i2c_getset(I2cGetIrqEn, I2cSetIrqEn, i2ctest.nCH);
#endif
		} else if (strcmp(argv[1], "scan") == 0) {		I2cChCheck(i2ctest.nCH);
		} else if (strcmp(argv[1], "?") == 0) {
			printf("%s div [0~65535]", argv[0]);
			printf("%s clk [...,100000,...,400000,...]", argv[0]);
			printf("%s mode [0:master 1:slave]", argv[0]);
			printf("%s bit [0:MSB 1:LSB]", argv[0]);
			printf("%s slvadr [0~127]", argv[0]);
			printf("%s irq [0:off 1:on]", argv[0]);
			printf("%s scan", argv[0]);
		}
	} else {
		Shell_Unknown();
	}

	return 0;
}
