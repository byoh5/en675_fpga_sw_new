//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include <string.h> // strcmp
#include <stdlib.h> // atoi

#include "enx_freertos.h"

#include "shell_cmd_gpio.h"

const char *sPerlGpio[]     = {"Check GPIO state",                               (char*)0};

#if 1
const char pin_group_name[8][5] = {"GPIO", "PWM", "SPI", "UART", "I2C", "I2S", "ETH", "ULPI"};

typedef enum {
	epgGPIO,
	epgPWM,
	epgSPI,
	epgUART,
	epgI2C,
	epgI2S,
	epgETH,
	epgULPI,
} PIN_GROUP;

const char pin_type_name[43][5] = {"", "", "CS", "MISO", "MOSI", "SCK", "RX", "TX", "SDA", "SCL", "WCK", "DIN", "DOUT",
									"BCK", "RXD0", "RXD1", "RXD2", "RXD3", "RXDV", "RXER", "COL", "CRS", "RCK", "TCK",
									"TXD0", "TXD1", "TXD2", "TXD3", "TXEN", "MDC", "MDIO", "NXT", "STP", "DIR", "CLK",
									"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"};

typedef enum {
	eptGPIO,
	eptPWM,
	eptSPI_CS,		eptSPI_MISO,	eptSPI_MOSI,	eptSPI_SCK,
	eptUART_RX,		eptUART_TX,
	eptI2C_SDA,		eptI2C_SCL,
	eptI2S_WCK,		eptI2S_DIN,		eptI2S_DOUT,	eptI2S_BCK,
	eptETH_RXD0,	eptETH_RXD1,	eptETH_RXD2,	eptETH_RXD3,	eptETH_RXDV,	eptETH_RXER,	eptETH_COL,		eptETH_CRS,		eptETH_RCK,		eptETH_TCK,		eptETH_TXD0,	eptETH_TXD1,	eptETH_TXD2,	eptETH_TXD3,	eptETH_TXEN,	eptETH_MDC,		eptETH_MDIO,
	eptULPI_NXT,	eptULPI_STP,	eptULPI_DIR,	eptULPI_CLK,	eptULPI_D0,		eptULPI_D1,		eptULPI_D2,		eptULPI_D3,		eptULPI_D4,		eptULPI_D5,		eptULPI_D6,		eptULPI_D7,
} PIN_TYPE;

typedef struct {
	BYTE pingroup;
	BYTE pintype;
	BYTE pinindex;
} pinctrl_fmt;

pinctrl_fmt pinctrl_mux[72][4] = {
	{{epgGPIO,eptGPIO,0},	{epgPWM,eptPWM,0},			{epgSPI,eptSPI_CS,0},	{epgUART,eptUART_RX,0}},
	{{epgGPIO,eptGPIO,1},	{epgPWM,eptPWM,1},			{epgSPI,eptSPI_MISO,0},	{epgUART,eptUART_TX,0}},
	{{epgGPIO,eptGPIO,2},	{epgPWM,eptPWM,2},			{epgSPI,eptSPI_MOSI,0},	{epgI2C,eptI2C_SDA,0}},
	{{epgGPIO,eptGPIO,3},	{epgPWM,eptPWM,3},			{epgSPI,eptSPI_SCK,0},	{epgI2C,eptI2C_SCL,0}},
	{{epgGPIO,eptGPIO,4},	{epgPWM,eptPWM,4},			{epgUART,eptUART_RX,0},	{epgSPI,eptSPI_CS,0}},
	{{epgGPIO,eptGPIO,5},	{epgPWM,eptPWM,5},			{epgUART,eptUART_TX,0},	{epgSPI,eptSPI_MISO,0}},
	{{epgGPIO,eptGPIO,6},	{epgPWM,eptPWM,6},			{epgI2C,eptI2C_SDA,0},	{epgSPI,eptSPI_MOSI,0}},
	{{epgGPIO,eptGPIO,7},	{epgPWM,eptPWM,7},			{epgI2C,eptI2C_SCL,0},	{epgSPI,eptSPI_SCK,0}},

	{{epgGPIO,eptGPIO,8},	{epgPWM,eptPWM,8},			{epgSPI,eptSPI_CS,1},	{epgUART,eptUART_RX,1}},
	{{epgGPIO,eptGPIO,9},	{epgPWM,eptPWM,9},			{epgSPI,eptSPI_MISO,1},	{epgUART,eptUART_TX,1}},
	{{epgGPIO,eptGPIO,10},	{epgPWM,eptPWM,10},			{epgSPI,eptSPI_MOSI,1},	{epgI2C,eptI2C_SDA,1}},
	{{epgGPIO,eptGPIO,11},	{epgPWM,eptPWM,11},			{epgSPI,eptSPI_SCK,1},	{epgI2C,eptI2C_SCL,1}},
	{{epgGPIO,eptGPIO,12},	{epgPWM,eptPWM,12},			{epgUART,eptUART_RX,1},	{epgSPI,eptSPI_CS,1}},
	{{epgGPIO,eptGPIO,13},	{epgPWM,eptPWM,13},			{epgUART,eptUART_TX,1},	{epgSPI,eptSPI_MISO,1}},
	{{epgGPIO,eptGPIO,14},	{epgPWM,eptPWM,14},			{epgI2C,eptI2C_SDA,1},	{epgSPI,eptSPI_MOSI,1}},
	{{epgGPIO,eptGPIO,15},	{epgPWM,eptPWM,15},			{epgI2C,eptI2C_SCL,1},	{epgSPI,eptSPI_SCK,1}},

	{{epgGPIO,eptGPIO,16},	{epgI2S,eptI2S_WCK,0xff},	{epgSPI,eptSPI_CS,2},	{epgUART,eptUART_RX,2}},
	{{epgGPIO,eptGPIO,17},	{epgI2S,eptI2S_DIN,0xff},	{epgSPI,eptSPI_MISO,2},	{epgUART,eptUART_TX,2}},
	{{epgGPIO,eptGPIO,18},	{epgI2S,eptI2S_DOUT,0xff},	{epgSPI,eptSPI_MOSI,2},	{epgI2C,eptI2C_SDA,2}},
	{{epgGPIO,eptGPIO,19},	{epgI2S,eptI2S_BCK,0xff},	{epgSPI,eptSPI_SCK,2},	{epgI2C,eptI2C_SCL,2}},
	{{epgGPIO,eptGPIO,20},	{epgPWM,eptPWM,16},			{epgUART,eptUART_RX,2},	{epgSPI,eptSPI_CS,2}},
	{{epgGPIO,eptGPIO,21},	{epgPWM,eptPWM,17},			{epgUART,eptUART_TX,2},	{epgSPI,eptSPI_MISO,2}},
	{{epgGPIO,eptGPIO,22},	{epgPWM,eptPWM,18},			{epgI2C,eptI2C_SDA,2},	{epgSPI,eptSPI_MOSI,2}},
	{{epgGPIO,eptGPIO,23},	{epgPWM,eptPWM,19},			{epgI2C,eptI2C_SCL,2},	{epgSPI,eptSPI_SCK,2}},

	{{epgGPIO,eptGPIO,24},	{epgPWM,eptPWM,20},			{epgSPI,eptSPI_CS,3},	{epgUART,eptUART_RX,3}},
	{{epgGPIO,eptGPIO,25},	{epgPWM,eptPWM,21},			{epgSPI,eptSPI_MISO,3},	{epgUART,eptUART_TX,3}},
	{{epgGPIO,eptGPIO,26},	{epgPWM,eptPWM,22},			{epgSPI,eptSPI_MOSI,3},	{epgI2C,eptI2C_SDA,3}},
	{{epgGPIO,eptGPIO,27},	{epgPWM,eptPWM,23},			{epgSPI,eptSPI_SCK,3},	{epgI2C,eptI2C_SCL,3}},
	{{epgGPIO,eptGPIO,28},	{epgPWM,eptPWM,24},			{epgUART,eptUART_RX,3},	{epgSPI,eptSPI_CS,3}},
	{{epgGPIO,eptGPIO,29},	{epgPWM,eptPWM,25},			{epgUART,eptUART_TX,3},	{epgSPI,eptSPI_MISO,3}},
	{{epgGPIO,eptGPIO,30},	{epgPWM,eptPWM,26},			{epgI2C,eptI2C_SDA,3},	{epgSPI,eptSPI_MOSI,3}},
	{{epgGPIO,eptGPIO,31},	{epgPWM,eptPWM,27},			{epgI2C,eptI2C_SCL,3},	{epgSPI,eptSPI_SCK,3}},

	{{epgGPIO,eptGPIO,32},	{epgPWM,eptPWM,28},			{epgSPI,eptSPI_CS,4},	{epgUART,eptUART_RX,4}},
	{{epgGPIO,eptGPIO,33},	{epgPWM,eptPWM,29},			{epgSPI,eptSPI_MISO,4},	{epgUART,eptUART_TX,4}},
	{{epgGPIO,eptGPIO,34},	{epgPWM,eptPWM,30},			{epgSPI,eptSPI_MOSI,4},	{epgI2C,eptI2C_SDA,4}},
	{{epgGPIO,eptGPIO,35},	{epgPWM,eptPWM,31},			{epgSPI,eptSPI_SCK,4},	{epgI2C,eptI2C_SCL,4}},
	{{epgGPIO,eptGPIO,36},	{epgETH,eptETH_RXD0,0xff},	{epgUART,eptUART_RX,4},	{epgSPI,eptSPI_CS,4}},
	{{epgGPIO,eptGPIO,37},	{epgETH,eptETH_RXD1,0xff},	{epgUART,eptUART_TX,4},	{epgSPI,eptSPI_MISO,4}},
	{{epgGPIO,eptGPIO,38},	{epgETH,eptETH_RXD2,0xff},	{epgI2C,eptI2C_SDA,4},	{epgSPI,eptSPI_MOSI,4}},
	{{epgGPIO,eptGPIO,39},	{epgETH,eptETH_RXD3,0xff},	{epgI2C,eptI2C_SCL,4},	{epgSPI,eptSPI_SCK,4}},

	{{epgGPIO,eptGPIO,40},	{epgETH,eptETH_RXDV,0xff},	{epgSPI,eptSPI_CS,5},	{epgUART,eptUART_RX,5}},
	{{epgGPIO,eptGPIO,41},	{epgETH,eptETH_RXER,0xff},	{epgSPI,eptSPI_MISO,5},	{epgUART,eptUART_TX,5}},
	{{epgGPIO,eptGPIO,42},	{epgETH,eptETH_COL,0xff},	{epgSPI,eptSPI_MOSI,5},	{epgI2C,eptI2C_SDA,5}},
	{{epgGPIO,eptGPIO,43},	{epgETH,eptETH_CRS,0xff},	{epgSPI,eptSPI_SCK,5},	{epgI2C,eptI2C_SCL,5}},
	{{epgGPIO,eptGPIO,44},	{epgETH,eptETH_RCK,0xff},	{epgUART,eptUART_RX,5},	{epgSPI,eptSPI_CS,5}},
	{{epgGPIO,eptGPIO,45},	{epgETH,eptETH_TCK,0xff},	{epgUART,eptUART_TX,5},	{epgSPI,eptSPI_MISO,5}},
	{{epgGPIO,eptGPIO,46},	{epgETH,eptETH_TXD0,0xff},	{epgI2C,eptI2C_SDA,5},	{epgSPI,eptSPI_MOSI,5}},
	{{epgGPIO,eptGPIO,47},	{epgETH,eptETH_TXD1,0xff},	{epgI2C,eptI2C_SCL,5},	{epgSPI,eptSPI_SCK,5}},

	{{epgGPIO,eptGPIO,48},	{epgETH,eptETH_TXD2,0xff},	{epgSPI,eptSPI_CS,6},	{epgUART,eptUART_RX,6}},
	{{epgGPIO,eptGPIO,49},	{epgETH,eptETH_TXD3,0xff},	{epgSPI,eptSPI_MISO,6},	{epgUART,eptUART_TX,6}},
	{{epgGPIO,eptGPIO,50},	{epgETH,eptETH_TXEN,0xff},	{epgSPI,eptSPI_MOSI,6},	{epgI2C,eptI2C_SDA,6}},
	{{epgGPIO,eptGPIO,51},	{epgETH,eptETH_MDC,0xff},	{epgSPI,eptSPI_SCK,6},	{epgI2C,eptI2C_SCL,6}},
	{{epgGPIO,eptGPIO,52},	{epgETH,eptETH_MDIO,0xff},	{epgUART,eptUART_RX,6},	{epgSPI,eptSPI_CS,6}},
	{{epgGPIO,eptGPIO,53},	{epgPWM,eptPWM,32},			{epgUART,eptUART_TX,6},	{epgSPI,eptSPI_MISO,6}},
	{{epgGPIO,eptGPIO,54},	{epgPWM,eptPWM,33},			{epgI2C,eptI2C_SDA,6},	{epgSPI,eptSPI_MOSI,6}},
	{{epgGPIO,eptGPIO,55},	{epgPWM,eptPWM,34},			{epgI2C,eptI2C_SCL,6},	{epgSPI,eptSPI_SCK,6}},

	{{epgGPIO,eptGPIO,56},	{epgPWM,eptPWM,35},			{epgSPI,eptSPI_CS,7},	{epgUART,eptUART_RX,7}},
	{{epgGPIO,eptGPIO,57},	{epgPWM,eptPWM,36},			{epgSPI,eptSPI_MISO,7},	{epgUART,eptUART_TX,7}},
	{{epgGPIO,eptGPIO,58},	{epgPWM,eptPWM,37},			{epgSPI,eptSPI_MOSI,7},	{epgI2C,eptI2C_SDA,7}},
	{{epgGPIO,eptGPIO,59},	{epgPWM,eptPWM,38},			{epgSPI,eptSPI_SCK,7},	{epgI2C,eptI2C_SCL,7}},
	{{epgGPIO,eptGPIO,60},	{epgULPI,eptULPI_NXT,0xff},	{epgUART,eptUART_RX,7},	{epgSPI,eptSPI_CS,7}},
	{{epgGPIO,eptGPIO,61},	{epgULPI,eptULPI_STP,0xff},	{epgUART,eptUART_TX,7},	{epgSPI,eptSPI_MISO,7}},
	{{epgGPIO,eptGPIO,62},	{epgULPI,eptULPI_DIR,0xff},	{epgI2C,eptI2C_SDA,7},	{epgSPI,eptSPI_MOSI,7}},
	{{epgGPIO,eptGPIO,63},	{epgULPI,eptULPI_CLK,0xff},	{epgI2C,eptI2C_SCL,7},	{epgSPI,eptSPI_SCK,7}},

	{{epgGPIO,eptGPIO,64},	{epgULPI,eptULPI_D0,0xff},	{epgSPI,eptSPI_CS,8},	{epgUART,eptUART_RX,8}},
	{{epgGPIO,eptGPIO,65},	{epgULPI,eptULPI_D1,0xff},	{epgSPI,eptSPI_MISO,8},	{epgUART,eptUART_TX,8}},
	{{epgGPIO,eptGPIO,66},	{epgULPI,eptULPI_D2,0xff},	{epgSPI,eptSPI_MOSI,8},	{epgI2C,eptI2C_SDA,8}},
	{{epgGPIO,eptGPIO,67},	{epgULPI,eptULPI_D3,0xff},	{epgSPI,eptSPI_SCK,8},	{epgI2C,eptI2C_SCL,8}},
	{{epgGPIO,eptGPIO,68},	{epgULPI,eptULPI_D4,0xff},	{epgUART,eptUART_RX,8},	{epgSPI,eptSPI_CS,8}},
	{{epgGPIO,eptGPIO,69},	{epgULPI,eptULPI_D5,0xff},	{epgUART,eptUART_TX,8},	{epgSPI,eptSPI_MISO,8}},
	{{epgGPIO,eptGPIO,70},	{epgULPI,eptULPI_D6,0xff},	{epgI2C,eptI2C_SDA,8},	{epgSPI,eptSPI_MOSI,8}},
	{{epgGPIO,eptGPIO,71},	{epgULPI,eptULPI_D7,0xff},	{epgI2C,eptI2C_SCL,8},	{epgSPI,eptSPI_SCK,8}},
};

char *PinctrlGetNameForFmt(char *buf, pinctrl_fmt fmt)
{
	if (pin_type_name[fmt.pintype][0] == 0) {
		if (fmt.pinindex == 0xff) {
			sprintf(buf, "%s", pin_group_name[fmt.pingroup]);
		} else {
			sprintf(buf, "%s%u", pin_group_name[fmt.pingroup], fmt.pinindex);
		}
	} else if (fmt.pinindex == 0xff) {
		sprintf(buf, "%s_%s", pin_group_name[fmt.pingroup], pin_type_name[fmt.pintype]);
	} else {
		sprintf(buf, "%s%u_%s", pin_group_name[fmt.pingroup], fmt.pinindex, pin_type_name[fmt.pintype]);
	}
	return buf;
}

char *PinctrlGetNameForCh(char *buf, UINT nCH)
{
	pinctrl_fmt fmt = pinctrl_mux[nCH][GpioGetFuncPin(nCH)];
	return PinctrlGetNameForFmt(buf, fmt);
}
#else
const char * const gpio_mux_name[72][4] = {
	{"GPIO00", "PWM00", "SPI0_CS",   "UART0_RX"},
	{"GPIO01", "PWM01", "SPI0_MISO", "UART0_TX"},
	{"GPIO02", "PWM02", "SPI0_MOSI", "I2C0_SDA"},
	{"GPIO03", "PWM03", "SPI0_SCK",  "I2C0_SCL"},
	{"GPIO04", "PWM04", "UART0_RX",  "SPI0_CS"},
	{"GPIO05", "PWM05", "UART0_TX",  "SPI0_MISO"},
	{"GPIO06", "PWM06", "I2C0_SDA",  "SPI0_MOSI"},
	{"GPIO07", "PWM07", "I2C0_SCL",  "SPI0_SCK"},

	{"GPIO08", "PWM08", "SPI1_CS",   "UART1_RX"},
	{"GPIO09", "PWM09", "SPI1_MISO", "UART1_TX"},
	{"GPIO10", "PWM10", "SPI1_MOSI", "I2C1_SDA"},
	{"GPIO11", "PWM11", "SPI1_SCK",  "I2C1_SCL"},
	{"GPIO12", "PWM12", "UART1_RX",  "SPI1_CS"},
	{"GPIO13", "PWM13", "UART1_TX",  "SPI1_MISO"},
	{"GPIO14", "PWM14", "I2C1_SDA",  "SPI1_MOSI"},
	{"GPIO15", "PWM15", "I2C1_SCL",  "SPI1_SCK"},

	{"GPIO16", "I2S_WCK", "SPI2_CS",   "UART2_RX"},
	{"GPIO17", "I2S_DIN", "SPI2_MISO", "UART2_TX"},
	{"GPIO18", "I2S_DOUT", "SPI2_MOSI", "I2C2_SDA"},
	{"GPIO19", "I2S_BCK", "SPI2_SCK",  "I2C2_SCL"},
	{"GPIO20", "PWM16", "UART2_RX",  "SPI2_CS"},
	{"GPIO21", "PWM17", "UART2_TX",  "SPI2_MISO"},
	{"GPIO22", "PWM18", "I2C2_SDA",  "SPI2_MOSI"},
	{"GPIO23", "PWM19", "I2C2_SCL",  "SPI2_SCK"},

	{"GPIO24", "PWM20", "SPI3_CS",   "UART3_RX"},
	{"GPIO25", "PWM21", "SPI3_MISO", "UART3_TX"},
	{"GPIO26", "PWM22", "SPI3_MOSI", "I2C3_SDA"},
	{"GPIO27", "PWM23", "SPI3_SCK",  "I2C3_SCL"},
	{"GPIO28", "PWM24", "UART3_RX",  "SPI3_CS"},
	{"GPIO29", "PWM25", "UART3_TX",  "SPI3_MISO"},
	{"GPIO30", "PWM26", "I2C3_SDA",  "SPI3_MOSI"},
	{"GPIO31", "PWM27", "I2C3_SCL",  "SPI3_SCK"},

	{"GPIO32", "PWM28", "SPI4_CS",   "UART4_RX"},
	{"GPIO33", "PWM29", "SPI4_MISO", "UART4_TX"},
	{"GPIO34", "PWM30", "SPI4_MOSI", "I2C4_SDA"},
	{"GPIO35", "PWM31", "SPI4_SCK",  "I2C4_SCL"},
	{"GPIO36", "ETH_RXD0", "UART4_RX",  "SPI4_CS"},
	{"GPIO37", "ETH_RXD1", "UART4_TX",  "SPI4_MISO"},
	{"GPIO38", "ETH_RXD2", "I2C4_SDA",  "SPI4_MOSI"},
	{"GPIO39", "ETH_RXD3", "I2C4_SCL",  "SPI4_SCK"},

	{"GPIO40", "ETH_RXDV", "SPI5_CS",   "UART5_RX"},
	{"GPIO41", "ETH_RXER", "SPI5_MISO", "UART5_TX"},
	{"GPIO42", "ETH_COL", "SPI5_MOSI", "I2C5_SDA"},
	{"GPIO43", "ETH_CRS", "SPI5_SCK",  "I2C5_SCL"},
	{"GPIO44", "ETH_RCK", "UART5_RX",  "SPI5_CS"},
	{"GPIO45", "ETH_TCK", "UART5_TX",  "SPI5_MISO"},
	{"GPIO46", "ETH_TXD0", "I2C5_SDA",  "SPI5_MOSI"},
	{"GPIO47", "ETH_TXD1", "I2C5_SCL",  "SPI5_SCK"},

	{"GPIO48", "ETH_TXD2", "SPI6_CS",   "UART6_RX"},
	{"GPIO49", "ETH_TXD3", "SPI6_MISO", "UART6_TX"},
	{"GPIO50", "ETH_TXEN", "SPI6_MOSI", "I2C6_SDA"},
	{"GPIO51", "ETH_MDC", "SPI6_SCK",  "I2C6_SCL"},
	{"GPIO52", "ETH_MDIO", "UART6_RX",  "SPI6_CS"},
	{"GPIO53", "PWM32", "UART6_TX",  "SPI6_MISO"},
	{"GPIO54", "PWM33", "I2C6_SDA",  "SPI6_MOSI"},
	{"GPIO55", "PWM34", "I2C6_SCL",  "SPI6_SCK"},

	{"GPIO56", "PWM35", "SPI7_CS",   "UART7_RX"},
	{"GPIO57", "PWM36", "SPI7_MISO", "UART7_TX"},
	{"GPIO58", "PWM37", "SPI7_MOSI", "I2C7_SDA"},
	{"GPIO59", "PWM38", "SPI7_SCK",  "I2C7_SCL"},
	{"GPIO60", "ULPI_NXT", "UART7_RX",  "SPI7_CS"},
	{"GPIO61", "ULPI_STP", "UART7_TX",  "SPI7_MISO"},
	{"GPIO62", "ULPI_DIR", "I2C7_SDA",  "SPI7_MOSI"},
	{"GPIO63", "ULPI_CLK", "I2C7_SCL",  "SPI7_SCK"},

	{"GPIO64", "ULPI_D0", "SPI8_CS",   "UART8_RX"},
	{"GPIO65", "ULPI_D1", "SPI8_MISO", "UART8_TX"},
	{"GPIO66", "ULPI_D2", "SPI8_MOSI", "I2C8_SDA"},
	{"GPIO67", "ULPI_D3", "SPI8_SCK",  "I2C8_SCL"},
	{"GPIO68", "ULPI_D4", "UART8_RX",  "SPI8_CS"},
	{"GPIO69", "ULPI_D5", "UART8_TX",  "SPI8_MISO"},
	{"GPIO70", "ULPI_D6", "I2C8_SDA",  "SPI8_MOSI"},
	{"GPIO71", "ULPI_D7", "I2C8_SCL",  "SPI8_SCK"}
};
#endif
unsigned int gpio_irq_count = 0;
unsigned int gpio_tx_ch = 0;
unsigned int gpio_rx_ch = 0;

static void gpio_irq_test(void *ctx)
{
	gpio_irq_count++;
	//if (gpio_irq_count % 10000 == 0) {
	printf(" %u\r", gpio_irq_count);
	//}
	if (gpio_irq_count <= 10000) {
		if (gpio_irq_count == 10000) {
			GpioSetEdge(gpio_rx_ch, GPIO_EDGE_RISE);
			printf("\r\nChange mode: GPIO_EDGE_RISE\r\n");
		}
		GpioSetOut(gpio_tx_ch, GPIO_OUT_HI);
		GpioSetOut(gpio_tx_ch, GPIO_OUT_LOW);

	} else if (gpio_irq_count <= 20000) {
		if (gpio_irq_count == 20000) {
			GpioSetEdge(gpio_rx_ch, GPIO_EDGE_BOTHE);
			printf("\r\nChange mode: GPIO_EDGE_BOTHE\r\n");
		}

		GpioSetOut(gpio_tx_ch, GPIO_OUT_LOW);
		GpioSetOut(gpio_tx_ch, GPIO_OUT_HI);
	} else if (gpio_irq_count <= 30000) {
		if (gpio_irq_count % 2 == 0) {
			GpioSetOut(gpio_tx_ch, GPIO_OUT_HI);
		} else {
			GpioSetOut(gpio_tx_ch, GPIO_OUT_LOW);
		}
	} else {
		GpioSetIrqEn(gpio_rx_ch, ENX_OFF);
	}
}

static void gpio_irq_test_init(UINT nCH_TX, UINT nCH_RX)
{
	// RX
	gpio_rx_ch = nCH_RX;
	gpio_irq_count = 0;
	GpioSetDir(nCH_RX, GPIO_DIR_IN);
	GpioSetEdge(nCH_RX, GPIO_EDGE_FALL);	//	GPIO_EDGE_FALL = 0,	GPIO_EDGE_RISE = 1,	GPIO_EDGE_BOTHE = 2
	printf("\r\nChange mode: GPIO_EDGE_FALL\r\n");
	GpioSetIrqEn(nCH_RX, ENX_ON);
	GpioIrqCallback(nCH_RX, gpio_irq_test, NULL);
	// TX
	gpio_tx_ch = nCH_TX;
	GpioSetOut(nCH_TX, GPIO_OUT_LOW);
	GpioSetDir(nCH_TX, GPIO_DIR_OUT);

	// Start
	GpioSetOut(nCH_TX, GPIO_OUT_LOW);
	GpioSetOut(nCH_TX, GPIO_OUT_HI);
}

int cmd_perl_gpio(int argc, char *argv[])
{
	if (argc == 1) {
		char strbuf[16] = {0};
		printf("GPIO Status =============================================\n");
		printf("      | IN  | OUT | OEN | DIR | IRQEN | IRQ |  Function |\n");
		printf("------|-----|-----|-----|-----|-------|-----|-----------|\n");
		for (uint32 i = 0; i < GPIO_CNT; i++) {
			printf("GPIO%02u|  %u  |  %u  |  %u  |  %2u |   %u   |  %u  | %-9s |\n", i,
#if 1
					GpioGetPin(i), GpioGetOut(i), GpioGetEdge(i), GpioGetDir(i), GpioGetIrqEn(i), GpioIsIrq(i), PinctrlGetNameForCh(strbuf, i));
#else
					GpioGetPin(i), GpioGetOut(i), GpioGetEdge(i), GpioGetDir(i), GpioGetIrqEn(i), GpioIsIrq(i), gpio_mux_name[i][GpioGetFuncPin(i)]);
#endif
			if (((i + 1) % 8) == 0) {
				printf("------|-----|-----|-----|-----|-------|-----|-----------|\n");
			}
		}
	} else {
		static int gpio_idx = -1;
		UINT getVal = 0;
		if (argc == 3) {
			getVal = atoi(argv[2]);
		}

		if (strcmp("idx", argv[1]) == 0) {
			if (argc == 3) {
				printf("Old select GPIO(%d)\n", gpio_idx);
				gpio_idx = atoi(argv[2]);
				if (gpio_idx > 71 || gpio_idx < 0) {
					printf("Invalid channel selection.(%d)\n", gpio_idx);
					gpio_idx = -1;
				} else {
					printf("New select GPIO(%d)\n", gpio_idx);
				}
			} else {
				printf("Select GPIO(%d)\n", gpio_idx);
			}
		} else if (strcmp("auto", argv[1]) == 0) {
			gpio_irq_test_init(65, 64);
			return 0;
		} else if (strcmp("stop", argv[1]) == 0) {
			GpioSetIrqEn(gpio_rx_ch, ENX_OFF);
			printf("\n");
			return 0;
		}

		if (gpio_idx > 71 || gpio_idx < 0) {
			printf("Select a channel!\n");
			return 0;
		}

		if (strcmp("e", argv[1]) == 0) {
			gpio_getset(GpioGetEdge, GpioSetEdge, gpio_idx);
		} else if (strcmp("dir", argv[1]) == 0) {
			gpio_getset(GpioGetDir, GpioSetDir, gpio_idx);
		} else if (strcmp("out", argv[1]) == 0) {
			gpio_getset(GpioGetOut, GpioSetOut, gpio_idx);
		} else if (strcmp("func", argv[1]) == 0) {
			gpio_getset(GpioGetFuncPin, GpioFuncPin, gpio_idx);
		} else if (strcmp("irq", argv[1]) == 0) {
			gpio_getset(GpioGetIrqEn, GpioSetIrqEn, gpio_idx);
		} else {

		}
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}
