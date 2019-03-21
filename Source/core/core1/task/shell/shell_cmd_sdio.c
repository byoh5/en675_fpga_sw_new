//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#if defined(__USE_SDIOCD__)

#include "enx_freertos.h"

#include "shell_cmd_sdio.h"
#include <string.h> // for memset
#include <stdlib.h> // for srand
#include <string.h> // for strcmp

const char *sSdioTest[]      = {"Test SDIO",                                      (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
#define SDIODATA_MAX_COUNT 8
typedef ATTR_MALIGN64 struct {
	BYTE *data;
	BYTE *alloc;
} SdioTestBuffer;
static SdioTestBuffer sdcd_data = {NULL, NULL};

static void sdio_create_random(BYTE *data, uint64_t size)
{
	uint64_t t = time(NULL);
	srand(t);
	for (uint64_t i = 0; i < size; i++) {
		data[i] = rand() % 255;
	}
}

int cmd_test_sdio(int argc, char *argv[])
{
	static UINT nBufferSize = 512 * SDIODATA_MAX_COUNT;

	if (sdcd_data.alloc == NULL) {
		sdcd_data.alloc = pvPortMalloc(nBufferSize+512);
		printf("SDIO-CD test memory Init (0x%08X)\n", sdcd_data.alloc);
		uint64_t celaddr = ((((uint64_t)sdcd_data.alloc) + 63) >> 6) << 6;
		printf("SDIO-CD test memory ALIGN64 address (0x%08X)\n", celaddr);
		sdcd_data.data = (BYTE *)celaddr;
	}

	if(argc == 1) {
		SdioRegShow(0, DEF_YES);
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
		} else if (strcmp(argv[1], "b") == 0) {
			if (strcmp(argv[2], "size") == 0) {
				if (argc == 3) {
					UINT nTemp = SdioGetDataBlockByte(0);
					printf("SDIO Get Data Block Size(%u)\n", nTemp);
				} else if (argc == 4) {
					UINT nSize = atoi(argv[3]);
					printf("SDIO Set Data Block Size(%u)\n", nSize);
					SdioSetDataBlockByte(0, nSize);	// Data Block Byte¸¦ ¼³Á¤
					nSize = SdioGetDataBlockByte(0);
					printf("SDIO Set Data Block Size(%u) OK\n", nSize);
				} else {
					Shell_Unknown();
				}
			} else {
				Shell_Unknown();
			}
		} else if (strcmp(argv[1], "r") == 0 || strcmp(argv[1], "w") == 0) {
			const char *strMode[8] = {"Read", "Write"};
			UINT nMode = strcmp(argv[1], "r") == 0 ? 0 : 1; // 0:Read 1:Write
			if (argc == 5) {
				UINT saddr = atoi(argv[3]);
				UINT eaddr = atoi(argv[4]);
				if (strcmp(argv[2], "s") == 0) {
					for (UINT i = saddr; i <= eaddr; i++) {
						printf("SDIO-CD %s[S] Addr(0x%08X) Dst(0x%08X)\n", strMode[nMode], i, sdcd_data.data);
						if (nMode == 0) { // Read
							memset(sdcd_data.data, 0, nBufferSize);
							SdioCdRead(sdcd_data.data, i, 1);
							//hexDump("SDIO-CD Read[S]", sdcd_data.data, 512);
						} else { // Write
							// create random data
							sdio_create_random(sdcd_data.data, 512);
							// sdio write
							SdioCdWrite(sdcd_data.data, i, 1);
						}
					}
				} else if (strcmp(argv[2], "m") == 0) {
					UINT nCount = eaddr - saddr + 1;
					if (nCount > SDIODATA_MAX_COUNT) {
						printf("Max Count 8, input(0x%08X/0x%08X=>%d)\n", saddr, eaddr, nCount);
						return 0;
					}
					printf("SDIO-CD %s[M] Addr(0x%08X~0x%08X) Dst(0x%08X)\n", strMode[nMode], saddr, eaddr, sdcd_data.data);
					if (nMode == 0) { // Read
						memset(sdcd_data.data, 0, nBufferSize);
						SdioCdRead(sdcd_data.data, saddr, nCount);
						//for (UINT i = saddr; i <= eaddr; i++) {
						//	hexDump("SDIO-CD Read[S]", sdcd_data.data, 512);
						//}
					} else { // Write
						sdio_create_random(sdcd_data.data, 512 * nCount);
						SdioCdWrite(sdcd_data.data, saddr, nCount);
					}
				} else {
					Shell_Unknown();
				}
			} else if (argc == 3) {
				UINT saddr = atoi(argv[2]);
				printf("SDIO-CD %s[S] Addr(0x%08X) Dst(0x%08X)\n", strMode[nMode], saddr, sdcd_data.data);
				if (nMode == 0) { // Read
					memset(sdcd_data.data, 0, nBufferSize);
					SdioCdRead(sdcd_data.data, saddr, 1);
					//hexDump("SDIO-CD Read[S]", sdcd_data.data, 512);
				} else { // Write
					sdio_create_random(sdcd_data.data, 512);
					SdioCdWrite(sdcd_data.data, saddr, 1);
				}
			} else {
				Shell_Unknown();
			}
		} else {
			Shell_Unknown();
		}
	}



	return 0;
}
#endif
