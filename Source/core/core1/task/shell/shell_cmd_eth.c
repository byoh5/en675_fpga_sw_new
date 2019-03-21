//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"
#include "ethphy.h"

#include "enx_freertos.h"

#include "shell_cmd_eth.h"

const char *sEthTest[]      = {"Test Ethernet",                                  (char*)0};
const char *sEthphyRegTest[]= {"Test Ethernet PHY Register",                     (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
typedef ATTR_MALIGN64 struct {
	BYTE data[512];
} EthTestBuffer;
static EthTestBuffer Eth_data;

int cmd_test_eth(int argc, char *argv[])
{
	Shell_Unknown();
	return 0;
}

int cmd_test_ethphyreg(int argc, char *argv[])
{
// tmdio
// tmdio 1
// tmdio 1 234
// tmdio clk
// tmdio clk 123
	UINT u32Reg = 0;
	WORD u16Data = 0;
	if (argc == 1) {
		MdioRead(ethphy_info.addr, u32Reg, &u16Data);
		EthphyRegView(u32Reg, u16Data);
	} else {
		if (strcmp("clk", argv[1]) == 0) {
			if (argc == 3) {
				UINT u32SetValue = atoi(argv[2]);
				if (u32SetValue > 255) {
					MdioSetClk(u32SetValue);
				} else {
					MdioSetClklmt(u32SetValue);
				}
			}

			UINT u32GetClock = MdioGetClk();
			UINT u32GetClklmt = MdioGetClklmt();
			printf("MDIO CLK: %uHz(%u)\n", u32GetClock, u32GetClklmt);
		} else {
			u32Reg = atoi(argv[1]);
			if (u32Reg > 31) {
				Shell_Unknown();
			} else {
				MdioRead(ethphy_info.addr, u32Reg, &u16Data);
				EthphyRegView(u32Reg, u16Data);
			}
		}
	}
	return 0;
}
