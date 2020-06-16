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

#if EN675_SINGLE
static BYTE test_register_value[256] = {0};
static void i2c_callback(void *ctx)
{
	static BYTE reg_idx = 0;
	static BYTE i2c_step = 0;

	if (I2cGetSlvRW(i2ctest.nCH)) { // Master에서 read
		if (I2cSlvGetStopflagBuf(i2ctest.nCH) == 1) {
//			printf("[R/Stop]\n");
		} else {
			I2cSetTxData(i2ctest.nCH, test_register_value[reg_idx]);
//			printf("[R/Idx:%02X/Dat:%02x]", reg_idx, test_register_value[reg_idx]);
			reg_idx++;
		}
	} else {  // Master에서 write
		if (I2cSlvGetStartflagBuf(i2ctest.nCH) == 1) {
//			printf("  [W/Start]");
			reg_idx = 0;
			i2c_step = 0;
		} else {
			if (i2c_step == 0) {
				reg_idx = I2cGetRxData(i2ctest.nCH);
//				printf("[W/Idx:%02X]", reg_idx);
				i2c_step = 1;
			} else {
				test_register_value[reg_idx] = I2cGetRxData(i2ctest.nCH);
//				printf("[W/Idx:%02X/Dat:%02X]", reg_idx, test_register_value[reg_idx]);
				reg_idx++;
			}
		}
	}
}
#endif

static void i2c_status_print(char *title, UINT type, UINT (*func)(UINT ch), char *text_type[2])
{
	if (type == 1) {
		_Gprintf("%-8s|", title);
	} else {
		printf("%-8s|", title);
	}
	for (uint32 i = 0; i < I2C_CNT; i++) {
		if (type == 16) {
			printf("    0x%02X |", func(i));
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

int cmd_perl_i2c(int argc, char *argv[])
{
	if (argc == 1) {
		i2c_status_print("I2C", 1, NULL, NULL);
		i2c_status_print("", 0, NULL, NULL);
		i2c_status_print("Control", 1, NULL, NULL);
		i2c_status_print("MODE", 0xff, I2cGetMode, ((char*[2]){"Master", "Slave"}));
		i2c_status_print("BIT_MODE", 0xff, I2cGetBitmode, ((char*[2]){"MSB", "LSB"}));
#if EN675_SINGLE
#if EN675_SINGLE_I2C_NEW
		i2c_status_print("SYNC", 10, I2cGetSync, NULL);
#endif
#endif
		i2c_status_print("IRQ_EN", 0xff, I2cGetIrqEn, ((char*[2]){"OFF", "ON"}));
		i2c_status_print("IRQ", 10, I2cIsIrq, NULL);
		i2c_status_print("CLK_DIV", 10, I2cGetClkdiv, NULL);
		i2c_status_print("Clock", 10, I2cGetClk, NULL);
		i2c_status_print("Data", 1, NULL, NULL);
		i2c_status_print("RX_DAT", 16, I2cGetRxData, NULL);
		i2c_status_print("TX_DAT", 16, I2cGetTxData, NULL);
		i2c_status_print("Master", 1, NULL, NULL);
		i2c_status_print("ACT", 10, I2cGetACT, NULL);
		i2c_status_print("COL", 10, I2cGetMstCol, NULL);
		i2c_status_print("ACK", 10, I2cGetMstAck, NULL);
		i2c_status_print("REPEAT", 10, I2cGetMstRepeat, NULL);
		i2c_status_print("LAST", 10, I2cGetMstLast, NULL);
		i2c_status_print("RW", 0xff, I2cGetMstRW, ((char*[2]){"Read", "Write"}));
		i2c_status_print("GO", 10, I2cGetMstGo, NULL);
		i2c_status_print("Slave", 1, NULL, NULL);
#if EN675_SINGLE
#if EN675_SINGLE_I2C_NEW
		i2c_status_print("STOP", 10, I2cGetSlvStopflag, NULL);
		i2c_status_print("START", 10, I2cGetSlvStartflag, NULL);
#endif
#endif
		i2c_status_print("SDA", 10, I2cGetSlvSDA, NULL);
		i2c_status_print("SCL", 10, I2cGetSlvSCL, NULL);
		i2c_status_print("ACK_IN", 10, I2cGetSlvAckIn, NULL);
		i2c_status_print("GO", 10, I2cGetSlvGo, NULL);
		i2c_status_print("RW", 0xff, I2cGetSlvRW, ((char*[2]){"Read", "Write"}));
		i2c_status_print("ACK_OUT", 10, I2cGetSlvAckOut, NULL);
		i2c_status_print("ADR", 16, I2cGetSlvAddr, NULL);
		i2c_status_print("--------", 1, NULL, NULL);
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

		if (i2ctest.nCH >= I2C_CNT || i2ctest.nCH < 0) {
			printf("Select a channel!\n");
			return 0;
		}

		if (strcmp(argv[1], "div") == 0) {				i2c_getset(I2cGetClkdiv, I2cSetClkdiv, i2ctest.nCH);
		} else if (strcmp(argv[1], "clk") == 0) {		i2c_getset(I2cGetClk, I2cSetClk, i2ctest.nCH);
#if EN675_SINGLE
#if EN675_SINGLE_I2C_NEW
		} else if (strcmp(argv[1], "sync") == 0) {		i2c_getset(I2cGetSync, I2cSetSync, i2ctest.nCH);
#endif
#endif
		} else if (strcmp(argv[1], "txd") == 0) {		i2c_getset(I2cGetTxData, I2cSetTxData, i2ctest.nCH);
		} else if (strcmp(argv[1], "mode") == 0) {		i2c_getset(I2cGetMode, I2cSetMode, i2ctest.nCH);
		} else if (strcmp(argv[1], "bit") == 0) {		i2c_getset(I2cGetBitmode, I2cSetBitmode, i2ctest.nCH);
		} else if (strcmp(argv[1], "mrep") == 0) {		i2c_getset(I2cGetMstRepeat, I2cSetMstRepeat, i2ctest.nCH);
		} else if (strcmp(argv[1], "mlat") == 0) {		i2c_getset(I2cGetMstLast, I2cSetMstLast, i2ctest.nCH);
		} else if (strcmp(argv[1], "mrw") == 0) {		i2c_getset(I2cGetMstRW, I2cSetMstRW, i2ctest.nCH);
		} else if (strcmp(argv[1], "mgo") == 0) {		i2c_getset(I2cGetMstGo, I2cSetMstGo, i2ctest.nCH);
		} else if (strcmp(argv[1], "sgo") == 0) {		i2c_getset(I2cGetSlvGo, I2cSetSlvGo, i2ctest.nCH);
		} else if (strcmp(argv[1], "srw") == 0) {		i2c_getset(I2cGetSlvRW, I2cSetSlvRW, i2ctest.nCH);
		} else if (strcmp(argv[1], "saout") == 0) {		i2c_getset(I2cGetSlvAckOut, I2cSetSlvAckOut, i2ctest.nCH);
		} else if (strcmp(argv[1], "sadr") == 0) {		i2c_getset(I2cGetSlvAddr, I2cSetSlvAddr, i2ctest.nCH);
		} else if (strcmp(argv[1], "irq") == 0) {		i2c_getset(I2cGetIrqEn, I2cSetIrqEn, i2ctest.nCH);
		} else if (strcmp(argv[1], "scan") == 0) {		I2cChCheck(i2ctest.nCH);
#if EN675_SINGLE
		} else if (strcmp(argv[1], "slave") == 0) {
			I2cSlvInit(i2ctest.nCH, I2C2_SPEED*4, 0x20);
			I2cIrqCallback(i2ctest.nCH, i2c_callback, NULL);
			I2cSetIrqEn(i2ctest.nCH, ENX_ON);
		} else if (strcmp(argv[1], "dump") == 0) {
			hexDump("TEST_REG", test_register_value, sizeof(test_register_value));
		} else if (strcmp(argv[1], "bufreset") == 0) {
			memset(test_register_value, 0, sizeof(test_register_value));
#endif
		} else if (strcmp(argv[1], "?") == 0) {
			printf("%s div [0~65535]", argv[0]);
			printf("%s sync [0~7]", argv[0]);
			printf("%s clk [...,100000,...,400000,...]", argv[0]);
			printf("%s txd [0~255]", argv[0]);
			printf("%s mode [0:master 1:slave]", argv[0]);
			printf("%s bit [0:MSB 1:LSB]", argv[0]);
			printf("%s mrep [0~1]", argv[0]);
			printf("%s mlat [0~1]", argv[0]);
			printf("%s mrw [0:Read 1:Write]", argv[0]);
			printf("%s mgo [0:Idle 1:Go]", argv[0]);
			printf("%s sgo [0:Idle 1:Go]", argv[0]);
			printf("%s srw [0:Read 1:Write]", argv[0]);
			printf("%s saout [0:NoAck 1:AckEn]", argv[0]);
			printf("%s sadr [0~127]", argv[0]);
			printf("%s irq [0:off 1:on]", argv[0]);
			printf("%s scan", argv[0]);
		}
	} else {
		Shell_Unknown();
	}

	return 0;
}
