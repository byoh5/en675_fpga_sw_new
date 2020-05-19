#include "dev.h"
#include "test_header.h"
#include "test_nonos.h"

#include <stdlib.h>

#if (USE_ETH==1)
static EthTestStr_nonos ethnonostest;

static UINT *pRX_LEN_INFO = (UINT *)(REG_BASE_ETH + 0x80000);
static BYTE gRxPktTail = 0;
static void test_eth_irq(void *ctx)
{
	BYTE gRxPktHead = ((pRX_LEN_INFO[gRxPktTail]&0xff000000)>>24);
	while (gRxPktTail != gRxPktHead) {
		UINT u32PktSize = (pRX_LEN_INFO[gRxPktTail] & 0x7ff); // +CRC(4byte)
		if (u32PktSize <= (1514+4) && u32PktSize >= (60+4)) {
			// D-cache flush
			hwflush_dcache_range((ULONG)ethnonostest.RxData[gRxPktTail].buffer, u32PktSize);

			if (gptMsgDebug.ETH_RX_CHECK == 1) {
				printf("EthRX Idx[%s%3u%s/%s%3u%s] - %u\n", TTY_COLOR_YELLOW, gRxPktHead, TTY_COLOR_RESET, TTY_COLOR_GREEN, gRxPktTail, TTY_COLOR_RESET, u32PktSize);
				hexDump("ETH Input", ethnonostest.RxData[gRxPktTail].buffer, u32PktSize);
			}

			// Data compare
			u32PktSize -= 4; // delete CRC(4byte)
			for (UINT i = 0; i < u32PktSize; i++) {
				if (ethnonostest.RxData[gRxPktTail].buffer[i] != ethnonostest.TxData[i]) {
					if (gptMsgDebug.ETH_RX_CHECK == 2) {
						hexCmpDump("Eth Rx Error", ethnonostest.RxData[gRxPktTail].buffer, ethnonostest.TxData, u32PktSize);
					}
					ethnonostest.err = -1;
					break;
				}
			}

			if (ethnonostest.err == 0) {
				ethnonostest.err = 1;
			}
		} else {
			if (u32PktSize == 0) {
				ethnonostest.err = -2;
			} else {
				ethnonostest.err = -3;
			}
		}

		gRxPktTail++;
		gRxPktHead = ((pRX_LEN_INFO[gRxPktTail]&0xff000000)>>24);
	}
}

void test_eth_start(void)
{
	ULONG err = 0, ok = 0;

	while (1) {
		ethnonostest.err = 0;
		EthTxPacket(ethnonostest.TxData, 1514);

		UINT timeout_cnt = 3 * 1000 * 1000;
		while (timeout_cnt) {
			if (ethnonostest.err != 0) {
				break;
			}
			WaitXus(1);
			timeout_cnt--;
		}

		if (ethnonostest.err == 1) {
			ok++;
		} else {
			printf("EthRxErr(%u)\n", ethnonostest.err);


			printf("=======================================\n");
			printf("SYS_RST_RID(%u)\n", SYS_RST_RID);
			printf("SYS_RST_PERI_REG(%u)\n", SYS_RST_PERI_REG);
			printf("SYS_RST_H265_REG(%u)\n", SYS_RST_H265_REG);
			printf("SYS_MCU_PHY_RST(%u)\n", SYS_MCU_PHY_RST);
			printf("SYS_RST_NPU_REG(%u)\n", SYS_RST_NPU_REG);
			printf("SYS_RST_CPU0(%u)\n", SYS_RST_CPU0);
			printf("SYS_RST_CPU1(%u)\n", SYS_RST_CPU1);
			printf("SYS_RST_CPU2(%u)\n", SYS_RST_CPU2);
			printf("SYS_RST_CPU3(%u)\n", SYS_RST_CPU3);
			printf("SYS_RST_ISP(%u)\n", SYS_RST_ISP);
			printf("=======================================\n");
			printf("SYS_NPU  PD(%u) SEL(%u)\n", SYS_NPU_PD, SYS_NPU_CKSEL);
			printf("SYS_CPU0 PD(%u) SEL(%u)\n", SYS_CPU0_PD, SYS_CPU0_CKSEL);
			printf("SYS_CPU1 PD(%u) SEL(%u)\n", SYS_CPU1_PD, SYS_CPU1_CKSEL);
			printf("SYS_CPU2 PD(%u) SEL(%u)\n", SYS_CPU2_PD, SYS_CPU2_CKSEL);
			printf("SYS_CPU3 PD(%u) SEL(%u)\n", SYS_CPU3_PD, SYS_CPU3_CKSEL);
			printf("=======================================\n");
			printf("SYS_WDT CNT(%u)\n", SYS_WDT_CNT);
			printf("        LMT(%u)\n", SYS_WDT_LMT);
			printf("        EN (%u)\n", SYS_WDT_EN);
			printf("        CLR(%u)\n", SYS_WDT_CLR);
			printf("=======================================\n");
			char strName[20] = {0};
			UINT *p = (UINT *)strName;
			p[0] = SYS_MARK0;
			p[1] = SYS_MARK1;
			p[2] = SYS_MARK2;
			p[3] = SYS_MARK3;
			p[4] = SYS_MARK4;
			printf("SYS_MAKR [%s]\n", strName);
			hexDump("SYS_MARK", strName, 20);
#if EN675_SINGLE
			printf("SYS_RTL  %04u-%02u-%02u %02u:%02u:%02u\n", SYS_RTL_YEAR + 2000, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MIN, SYS_RTL_SEC);
#else
			printf("SYS_RTL  %04X-%02X-%02X %02X:%02X:%02X\n", SYS_RTL_YEAR, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MINUTE, SYS_RTL_SECOND);
#endif
			printf("=======================================\n");
			printf("SYS_REG0(0x%08X) ", SYS_REG0);
			printf("SYS_REG1(0x%08X) ", SYS_REG1);
			printf("SYS_REG2(0x%08X) ", SYS_REG2);
			printf("SYS_REG3(0x%08X)\n", SYS_REG3);
			printf("SYS_REG4(0x%08X) ", SYS_REG4);
			printf("SYS_REG5(0x%08X) ", SYS_REG5);
			printf("SYS_REG6(0x%08X) ", SYS_REG6);
			printf("SYS_REG7(0x%08X)\n", SYS_REG7);
			printf("=======================================\n");
			enx_exirq_view();


			err++;
		}

		if ((err + ok) % 10000 == 0) {
			printf("ETH:%8lu: ok(%lu) err(%lu)\n", ok + err, ok, err);
		}
	}
}

void test_eth_init(void)
{
	hwflush_dcache_all();
	for (int i = 0 ; i < sizeof(ethnonostest.TxData); i++) {
		ethnonostest.TxData[i] = rand();
	}
	ethnonostest.RxData = calloc(1536, 256);
	hwflush_dcache_all();

	EthRxSetBuffer((BYTE *)ethnonostest.RxData, 256);

	BYTE gRxPktHead = ((pRX_LEN_INFO[gRxPktTail]&0xff000000)>>24);
	gRxPktTail = gRxPktHead;

	EthSetRxEn(ENX_ON);
	EthSetRxIrqEn(ENX_ON);
	enx_externalirq_perl(eigiETH, ENX_ON, 0);
	EthRxIrqCallback(test_eth_irq, NULL);

	WaitXus(100);
}
#endif
