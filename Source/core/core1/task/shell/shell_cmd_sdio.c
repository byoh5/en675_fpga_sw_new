//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include "enx_freertos.h"

#include "shell_cmd_sdio.h"

const char *sSdioTest[]      = {"Test SDIO",                                      (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
typedef ATTR_MALIGN64 struct {
	BYTE data[512];
} SdioTestBuffer;
static SdioTestBuffer sdcd_data;

int cmd_test_sdio(int argc, char *argv[])
{
	if(argc == 1) {
		SdioRegShow(0);
	} else {
		if (strcmp(argv[1], "test") == 0) {
			extern void SdioRegTest(UINT nCH);
			SdioRegTest(0);
		} else if (strcmp(argv[1], "c") == 0) {
			if (argc == 2) {
				SdioCdClockRestore();
			} else if (argc == 3) {
				SdioCdClockDown();
			} else {
				Shell_Unknown();
			}
		} else if (strcmp(argv[1], "r") == 0) {
			if (argc == 5) {
				UINT saddr = atoi(argv[3]);
				UINT eaddr = atoi(argv[4]);
				if (strcmp(argv[2], "s") == 0) {
					for (UINT i = saddr; i <= eaddr; i++) {
						printf("SDIO-CD Read[S] Addr(0x%08X) Dst(0x%08X)\n", i, sdcd_data.data);
						memset(sdcd_data.data, 0, sizeof(sdcd_data.data));
						SdioCdReadS(sdcd_data.data, i);
						//hexDump("SDIO-CD Read[S]", sdcd_data.data, 512);
					}
				} else if (strcmp(argv[2], "m") == 0) {
					flprintf("\n");
				} else {
					Shell_Unknown();
				}
			} else if (argc == 3) {
				UINT saddr = atoi(argv[2]);
				printf("SDIO-CD Read[S] Addr(0x%08X) Dst(0x%08X)\n", saddr, sdcd_data.data);
				memset(sdcd_data.data, 0, sizeof(sdcd_data.data));
				SdioCdReadS(sdcd_data.data, saddr);
				//hexDump("SDIO-CD Read[S]", sdcd_data.data, 512);
			} else {
				Shell_Unknown();
			}
		} else {
			Shell_Unknown();
		}
	}



	return 0;
}
