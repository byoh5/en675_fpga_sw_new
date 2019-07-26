//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"
#include "ethphy.h"

#include <string.h> // strcmp
#include <stdlib.h> // atoi

#include "enx_freertos.h"
#include "networkif.h"

#include "shell_cmd_eth.h"

#if (ETHPHY_LOOPBACK_TEST==1)
#include "ethloopback.h"
#endif

#if (ETH_SHELL_TEST==1)
const char *sEthTest[]      = {"Test Ethernet",                                  (char*)0};
#endif
#if (ETHPHY_SHELL_TEST==1)
const char *sEthphyRegTest[]= {"Test Ethernet PHY Register",                     (char*)0};
#endif
//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
#if 0
typedef ATTR_MALIGN64 struct {
	BYTE data[512];
} EthTestBuffer;
static EthTestBuffer Eth_data;
#endif

#if (ETH_SHELL_TEST==1)
int eth_test_task_sw;
int eth_test_task_txtck;
int eth_test_size = 1514;
//ISRD BYTE u8SBuf[2048];
//BYTE u8Buffer2[4096] = {1,2,3,4,5};
static UINT eth_tx_test(BYTE *u8BufferAlign, ULONG *count)
{
#if 0
	clear_csr(mie, MIP_MTIP);
	clear_csr(mie, MIP_MEIP);
#endif
	(*count) = 0;
	ULONG a = BenchTimeStart();
	while (eth_test_task_sw) {
		//hwflush_dcache_range(u8BufferAlign, 1514);
		//BDmaMemCpy_rtos(1, u8SendBufferAlign, u8BufferAlign, 1514);
		if (ETH_TX_PAUSE) {
			printf("1");
		}
		EthTxPacket(u8BufferAlign, eth_test_size);
		if (ETH_TX_PAUSE) {
			printf("2");
		}
		(*count)++;
	}
#if 0
	set_csr(mie, MIP_MTIP);
	set_csr(mie, MIP_MEIP);
#endif
	return BenchTimeStop(a);
}

void cmd_eth_test_task(void *ctx)
{
	eth_test_task_sw = 1;
	printf("%s(%d) : Start\n", __func__, __LINE__);
	//BYTE *u8Buffer = u8SBuf;//pvPortMalloc(1024*6);
	BYTE *u8Buffer = pvPortMalloc(1024*6);
	//BYTE u8Buffer2[4096] = {1};//pvPortMalloc(1024*3);
	if (u8Buffer) {
		//BYTE *u8BufferAlign = (BYTE *)((((ULONG)u8Buffer2 + 0x3FF) >> 10) << 10);
		BYTE *u8BufferAlign = (BYTE *)((((ULONG)u8Buffer + 0x3FF) >> 10) << 10);
		//BYTE *u8BufferAlign = u8Buffer;
		//BYTE *u8SendBufferAlign = (((ULONG)u8Buffer2 + 0x3F) >> 6) << 6;
		printf("u8Buffer         : 0x%08lX\n", (ULONG)u8Buffer);
		//printf("u8Buffer2         : 0x%08lX\n", (ULONG)u8Buffer2);
		printf("u8BufferAlign    : 0x%08lX\n", (ULONG)u8BufferAlign);
		printf("ETH_TX_IFGGAP    : 0x%08lX\n", (ULONG)ETH_TX_IFGGAP);
		//ETH_TX_IFGGAP = 9;
		printf("ETH_TX_IFGGAP    : 0x%08lX\n", (ULONG)ETH_TX_IFGGAP);

		//printf("u8Buffer2        : 0x%08X\n", u8Buffer2);
		//printf("u8SendBufferAlign: 0x%08X\n", u8SendBufferAlign);
		int i;
		for (i = 0; i < 1514; i++) {
			u8BufferAlign[i] = i;
	//		if(i%2==0)
	//			test_data[i] = 0xaa;
	//		else
	//			test_data[i] = 0x55;
		}
		ULONG a, count = 0;
		UINT end;
#if 0
		set_csr(mie, 0);
		set_csr(mie, 0);								// Enable External Interrupts
		set_csr(mstatus, 0);

		write_csr(mie, 0);
		write_csr(mie, 0);								// Enable External Interrupts
		write_csr(mstatus, 0);
#endif
		hwflush_dcache_range((ulong)u8BufferAlign, 1514);

		if (eth_test_task_txtck == 1) {
			for (int e = 0; e < 2; e++) {
				for (int i = 0; i < 16; i++) {
					ETH_TX_TCKDLY = i;
					printf("TX CLK Edge(%u) Dly(%u)\n", ETH_TX_CLKEDGE, ETH_TX_TCKDLY);
					for (int loop = 100; loop; loop--) {
						EthTxPacket(u8BufferAlign, eth_test_size);
					}
					vTaskDelay(100);
				}
				ETH_TX_CLKEDGE = ~ETH_TX_CLKEDGE;
			}
		} else {
			end = eth_tx_test(u8BufferAlign, &count);
			char buf[64] = {0};
			snprintf(buf, 64, "%.2f", ((1514 * count) / 1024.0 / (1024.0 / 8)) / (end / 1000.0 / 1000.0));
			printf("Send(%lu) TotalTime(%uus) AvgTime(%luus) Speed(%sMbps/s)\n", count, end, end / count, buf);
		}

#if 0
#if 0
		clear_csr(mie, MIP_MTIP);
		clear_csr(mie, MIP_MEIP);
#endif
		a = BenchTimeStart();
		while (eth_test_task_sw) {
			//hwflush_dcache_range(u8BufferAlign, 1514);
			//BDmaMemCpy_rtos(1, u8SendBufferAlign, u8BufferAlign, 1514);
			if (ETH_TX_PAUSE) {
				printf("1");
			}
			EthTxPacket(u8BufferAlign, eth_test_size);
			if (ETH_TX_PAUSE) {
				printf("2");
			}
			count++;
		}
#if 0
		set_csr(mie, MIP_MTIP);
		set_csr(mie, MIP_MEIP);
#endif
		end = BenchTimeStop(a);

		char buf[64] = {0};
		snprintf(buf, 64, "%.2f", ((1514 * count) / 1024.0 / (1024.0 / 8)) / (end / 1000.0 / 1000.0));
		printf("Send(%lu) TotalTime(%uus) AvgTime(%luus) Speed(%sMbps/s)\n", count, end, end / count, buf);
#endif
	} else {
		printf("Malloc Fail\n");
	}
	vTaskDelete(NULL);

	UNUSED(ctx);
}

#define eth_getset(val)	{	if(argc == 3) {	\
								printf(#val"(%d)\n", val); \
							} else { \
								UINT getdValue = atoi(argv[3]); \
								val = getdValue; \
								printf("input(%d) => set "#val"(%d)\n", getdValue, val); \
							} \
						}

void eth_rx_state(void)
{
	printf("== RX state =================\n");
	printf("ETH_RX_EN       (rx en ) : %u\n", ETH_RX_EN);
	printf("ETH_RX_CRC_EN   (rx crc) : %u\n", ETH_RX_CRC_EN);
	printf("ETH_RX_ERR_EN   (rx err) : %u\n", ETH_RX_ERR_EN);
	printf("ETH_RX_ERTYPE   (rx er ) : %u\n", ETH_RX_ERTYPE);
	printf("ETH_RX_DATTYPE  (rx tp ) : %u\n", ETH_RX_DATTYPE);
	printf("== RX Clock & Delay =========\n");
	printf("ETH_RX_RCKEDGE	(rx e  ) : %u\n", ETH_RX_RCKEDGE);
	printf("ETH_RX_RCKDLY	(rx rck) : %u\n", ETH_RX_RCKDLY);
	printf("== RX buffer =================\n");
	printf("ETH_RX_ADR      (rx adr) : 0x%08X\n", ETH_RX_ADR);
	printf("ETH_RX_QOS      (rx qos) : %u\n", ETH_RX_QOS);
	printf("ETH_RX_LMT      (rx lmt) : %u\n", ETH_RX_LMT);

	UINT *pRX_LEN_INFO = (UINT *)(REG_BASE_ETH + 0x80000);
	BYTE gRxPktHead = ((pRX_LEN_INFO[0]&0xff000000)>>24);
	UINT u32PktSize = (pRX_LEN_INFO[gRxPktHead] & 0x7ff) - 4;
	printf("ETH_RX_IDX               : %u\n", gRxPktHead);
	printf("ETH_RX_SIZE              : %u\n", u32PktSize);
	printf("== RX Filter ================\n");
	printf("ETH_DSTMAC_BYP  (if mps) : %u\n", ETH_DSTMAC_BYP);
	es_printf("ETH_DSTMAC_EN   (if men) : %032bb\n", ETH_DSTMAC_EN);
#if 0
	printf("ETH_DSTMAC_ADR0          : %04X%08X\n", ETH_DSTMAC_ADR0_47_32, ETH_DSTMAC_ADR0_31_0);
	printf("ETH_DSTMAC_ADR1          : %04X%08X\n", ETH_DSTMAC_ADR1_47_32, ETH_DSTMAC_ADR1_31_0);
	printf("ETH_DSTMAC_ADR2          : %04X%08X\n", ETH_DSTMAC_ADR2_47_32, ETH_DSTMAC_ADR2_31_0);
	printf("ETH_DSTMAC_ADR3          : %04X%08X\n", ETH_DSTMAC_ADR3_47_32, ETH_DSTMAC_ADR3_31_0);
	printf("ETH_DSTMAC_ADR4          : %04X%08X\n", ETH_DSTMAC_ADR4_47_32, ETH_DSTMAC_ADR4_31_0);
	printf("ETH_DSTMAC_ADR5          : %04X%08X\n", ETH_DSTMAC_ADR5_47_32, ETH_DSTMAC_ADR5_31_0);
	printf("ETH_DSTMAC_ADR6          : %04X%08X\n", ETH_DSTMAC_ADR6_47_32, ETH_DSTMAC_ADR6_31_0);
	printf("ETH_DSTMAC_ADR7          : %04X%08X\n", ETH_DSTMAC_ADR7_47_32, ETH_DSTMAC_ADR7_31_0);
	printf("ETH_DSTMAC_ADR8          : %04X%08X\n", ETH_DSTMAC_ADR8_47_32, ETH_DSTMAC_ADR8_31_0);
	printf("ETH_DSTMAC_ADR9          : %04X%08X\n", ETH_DSTMAC_ADR9_47_32, ETH_DSTMAC_ADR9_31_0);
	printf("ETH_DSTMAC_ADR10         : %04X%08X\n", ETH_DSTMAC_ADR10_47_32, ETH_DSTMAC_ADR10_31_0);
	printf("ETH_DSTMAC_ADR11         : %04X%08X\n", ETH_DSTMAC_ADR11_47_32, ETH_DSTMAC_ADR11_31_0);
	printf("ETH_DSTMAC_ADR12         : %04X%08X\n", ETH_DSTMAC_ADR12_47_32, ETH_DSTMAC_ADR12_31_0);
	printf("ETH_DSTMAC_ADR13         : %04X%08X\n", ETH_DSTMAC_ADR13_47_32, ETH_DSTMAC_ADR13_31_0);
	printf("ETH_DSTMAC_ADR14         : %04X%08X\n", ETH_DSTMAC_ADR14_47_32, ETH_DSTMAC_ADR14_31_0);
	printf("ETH_DSTMAC_ADR15         : %04X%08X\n", ETH_DSTMAC_ADR15_47_32, ETH_DSTMAC_ADR15_31_0);
	printf("ETH_DSTMAC_ADR16         : %04X%08X\n", ETH_DSTMAC_ADR16_47_32, ETH_DSTMAC_ADR16_31_0);
	printf("ETH_DSTMAC_ADR17         : %04X%08X\n", ETH_DSTMAC_ADR17_47_32, ETH_DSTMAC_ADR17_31_0);
	printf("ETH_DSTMAC_ADR18         : %04X%08X\n", ETH_DSTMAC_ADR18_47_32, ETH_DSTMAC_ADR18_31_0);
	printf("ETH_DSTMAC_ADR19         : %04X%08X\n", ETH_DSTMAC_ADR19_47_32, ETH_DSTMAC_ADR19_31_0);
	printf("ETH_DSTMAC_ADR20         : %04X%08X\n", ETH_DSTMAC_ADR20_47_32, ETH_DSTMAC_ADR20_31_0);
	printf("ETH_DSTMAC_ADR21         : %04X%08X\n", ETH_DSTMAC_ADR21_47_32, ETH_DSTMAC_ADR21_31_0);
	printf("ETH_DSTMAC_ADR22         : %04X%08X\n", ETH_DSTMAC_ADR22_47_32, ETH_DSTMAC_ADR22_31_0);
	printf("ETH_DSTMAC_ADR23         : %04X%08X\n", ETH_DSTMAC_ADR23_47_32, ETH_DSTMAC_ADR23_31_0);
	printf("ETH_DSTMAC_ADR24         : %04X%08X\n", ETH_DSTMAC_ADR24_47_32, ETH_DSTMAC_ADR24_31_0);
	printf("ETH_DSTMAC_ADR25         : %04X%08X\n", ETH_DSTMAC_ADR25_47_32, ETH_DSTMAC_ADR25_31_0);
	printf("ETH_DSTMAC_ADR26         : %04X%08X\n", ETH_DSTMAC_ADR26_47_32, ETH_DSTMAC_ADR26_31_0);
	printf("ETH_DSTMAC_ADR27         : %04X%08X\n", ETH_DSTMAC_ADR27_47_32, ETH_DSTMAC_ADR27_31_0);
	printf("ETH_DSTMAC_ADR28         : %04X%08X\n", ETH_DSTMAC_ADR28_47_32, ETH_DSTMAC_ADR28_31_0);
	printf("ETH_DSTMAC_ADR29         : %04X%08X\n", ETH_DSTMAC_ADR29_47_32, ETH_DSTMAC_ADR29_31_0);
	printf("ETH_DSTMAC_ADR30         : %04X%08X\n", ETH_DSTMAC_ADR30_47_32, ETH_DSTMAC_ADR30_31_0);
	printf("ETH_DSTMAC_ADR31         : %04X%08X\n", ETH_DSTMAC_ADR31_47_32, ETH_DSTMAC_ADR31_31_0);
#endif
}

void eth_tx_state(void)
{
	printf("== TX state ===============\n");
	printf("ETH_TX_DATBIT   (tx db ) : %u\n", ETH_TX_DATBIT);
	printf("ETH_TX_DATTYPE  (tx dt ) : %u\n", ETH_TX_DATTYPE);
	printf("ETH_TX_CLKOE    (tx ce ) : %u\n", ETH_TX_CLKOE);
	printf("ETH_TX_CLKSEL   (tx cs ) : %u\n", ETH_TX_CLKSEL);
	printf("ETH_TX_CRSCHK   (tx cr ) : %u\n", ETH_TX_CRSCHK);
	printf("ETH_TX_COLCHK   (tx co ) : %u\n", ETH_TX_COLCHK);
	printf("ETH_TX_RTYEN    (tx rn ) : %u\n", ETH_TX_RTYEN);
	printf("ETH_TX_RTYLMT   (tx rl ) : %u\n", ETH_TX_RTYLMT);
	printf("ETH_TX_EMPTY             : %u\n", ETH_TX_EMPTY);
	printf("ETH_TX_FULL              : %u\n", ETH_TX_FULL);
	printf("== TX buffer ==============\n");
	printf("ETH_TX_ADR      (tx adr) : 0x%08X\n", ETH_TX_ADR);
	printf("ETH_TX_LEN      (tx len) : %u\n", ETH_TX_LEN);
	printf("ETH_TX_QOS      (tx qos) : %u\n", ETH_TX_QOS);
	printf("ETH_TX_VAL      (tx val) : %u\n", ETH_TX_VAL);
	printf("ETH_TX_PAUSE_EN (tx p  ) : %u\n", ETH_TX_PAUSE_EN);
	printf("ETH_TX_PAUSE             : %u\n", ETH_TX_PAUSE);
	printf("== TX Clock & Delay =======\n");
	printf("ETH_IFG_GAP     (tx ifg) : %u\n", ETH_TX_IFGGAP);
	printf("ETH_TX_CLKEDGE  (tx e  ) : %u\n", ETH_TX_CLKEDGE);
	printf("ETH_TX_TCKDLY   (tx tck) : %u\n", ETH_TX_TCKDLY);
	printf("ETH_TX_TXENDLY  (tx n  ) : %u\n", ETH_TX_TXENDLY);
	printf("ETH_TX_TXD0DLY  (tx 0  ) : %u\n", ETH_TX_TXD0DLY);
	printf("ETH_TX_TXD1DLY  (tx 1  ) : %u\n", ETH_TX_TXD1DLY);
	printf("ETH_TX_TXD2DLY  (tx 2  ) : %u\n", ETH_TX_TXD2DLY);
	printf("ETH_TX_TXD3DLY  (tx 3  ) : %u\n", ETH_TX_TXD3DLY);
}

int cmd_test_eth(int argc, char *argv[])
{
	flprintf("Start\n");
	if (argc == 1) { // state print
		printf("== Ethernet State ==========\n");
		printf("ETH Type                 : ");
		if (ETH_RX_DATTYPE == 0 && ETH_TX_DATTYPE == 0 && ETH_TX_DATBIT == 1) {
			printf("MII Type\n");
		} else if(ETH_RX_DATTYPE == 1 && ETH_TX_DATTYPE == 1 && ETH_TX_DATBIT == 1) {
			printf("RGMII Type\n");
		} else if(ETH_RX_DATTYPE == 2 && ETH_TX_DATTYPE == 0 && ETH_TX_DATBIT == 0) {
			printf("RMII Type\n");
		} else {
			printf("Unknown Type(RX_DATTYPE:%d, TX_DATTYPE:%d, TX_DATBIT:%d)\n", ETH_RX_DATTYPE, ETH_TX_DATTYPE, ETH_TX_DATBIT);
		}
		eth_rx_state();
		eth_tx_state();
		printf("== IRQ state ==============\n");
		printf("ETH_IRQ_RX               : %u\n", ETH_IRQ_RX);
		printf("ETH_IRQ_TX               : %u\n", ETH_IRQ_TX);
		printf("ETH_IRQ_RX_EN            : %u\n", ETH_IRQ_RX_EN);
		printf("ETH_IRQ_TX_EN            : %u\n", ETH_IRQ_TX_EN);
		printf("ETH_IRQ_RX_CLR           : %u\n", ETH_IRQ_RX_CLR);
		printf("ETH_IRQ_TX_CLR           : %u\n", ETH_IRQ_TX_CLR);
		printf("===========================\n");
	} else {
		if (strcmp(argv[1], "rx") == 0) {
			if (argc == 2) {
				eth_rx_state();
			} else {
				if (strcmp(argv[2], "en") == 0) {			eth_getset(ETH_RX_EN);		}
				else if (strcmp(argv[2], "crc") == 0) {		eth_getset(ETH_RX_CRC_EN);	}
				else if (strcmp(argv[2], "err") == 0) {		eth_getset(ETH_RX_ERR_EN);	}
				else if (strcmp(argv[2], "er") == 0) {		eth_getset(ETH_RX_ERTYPE);	}
				else if (strcmp(argv[2], "tp") == 0) {		eth_getset(ETH_RX_DATTYPE);	}
				else if (strcmp(argv[2], "rck") == 0) {		eth_getset(ETH_RX_RCKDLY);	}
				else if (strcmp(argv[2], "e") == 0)	{		eth_getset(ETH_RX_RCKEDGE);	}
				else if (strcmp(argv[2], "adr") == 0) {		eth_getset(ETH_RX_ADR);		}
				else if (strcmp(argv[2], "qos") == 0) {		eth_getset(ETH_RX_QOS);		}
				else if (strcmp(argv[2], "lmt") == 0) {		eth_getset(ETH_RX_LMT);		}
				else if (strcmp(argv[2], "on") == 0) {
					gptMsgDebug.ETH_RX_CHECK = 1;
				} else if (strcmp(argv[2], "off") == 0) {
					gptMsgDebug.ETH_RX_CHECK = 0;
				}
#if 1
				else if (strcmp(argv[2], "auto") == 0) {
					gptMsgDebug.ETH_RX_CHECK = 1;
					for (int i = 0; i < 2; i++) {
						for (int j = 0; j < 16; j++) {
							netifapi_netif_set_down(netif_state[enlETHERNET]._netif);
							ETH_RX_RCKDLY = j;
							printf("\nETH EDGE_RXCLK(%d) ETH_RX_RCKDLY(%d)\n", ETH_RX_RCKEDGE, ETH_RX_RCKDLY);
							netifapi_netif_set_up(netif_state[enlETHERNET]._netif);
							vTaskDelay(500);
						}
						ETH_RX_RCKEDGE = ~ETH_RX_RCKEDGE;
					}
					gptMsgDebug.ETH_RX_CHECK = 0;
				}
#endif
#if 0
				else if(strcmp(argv[2], "reset") == 0)
				{
					printf("Ethernet RX IRQ Stop!\n");
					IrqDisable(IRQ_ETH);
					IntAdd(IRQ_ETH, NULL, NULL);

					printf("Ethernet RX Sram Buffer Reset\n");
					portENTER_CRITICAL();
					DmaMemSet_isr((BYTE *)&gptMsgNetworkRX, 0, sizeof(gptMsgNetworkRX));
					gptMsgNetworkRX.base = NETRX_BUF_BASE;
					gptMsgNetworkRX.end = NETRX_BUF_END;
					gptMsgNetworkRX.head_pos = NETRX_BUF_BASE;
#if (ETH_MAC_PAUSE==1)
					gptMsgNetworkRX.tx_go = 0;
					gptMsgNetworkRX.tx_pause = 0;
					gptMsgNetworkRX.macp_time = 0;
					gptMsgNetworkRX.macp_count = 0;
#endif
					portEXIT_CRITICAL();

					printf("Ethernet RX IRQ Start!\n");
					IntAdd(IRQ_ETH, (void*)isr_oeth_rx, NULL);
					IrqEnable(IRQ_ETH);
				}
#endif
				else if (strcmp(argv[2], "buf") == 0) {
					if (strcmp(argv[3], "all") == 0) {
						for (UINT idx = 0; idx < 256; idx++) {
							printf("Show buffer[%u]\n", idx);
							hwflush_dcache_range((ULONG)qEthernetRX.pkt_data[idx].buffer, 1536);
							hexDump("Show buffer", qEthernetRX.pkt_data[idx].buffer, 1536);
						}
					} else {
						BYTE idx = atoi(argv[3]);
						printf("Show buffer[%u]\n", idx);
						hwflush_dcache_range((ULONG)qEthernetRX.pkt_data[idx].buffer, 1536);
						hexDump("Show buffer", qEthernetRX.pkt_data[idx].buffer, 1536);
					}
				} else {
					Shell_Unknown();
				}
			}
		} else if (strcmp(argv[1], "tx") == 0) {
			if (argc == 2) {
				eth_tx_state();
			} else {
				if (strcmp(argv[2], "db") == 0) {			eth_getset(ETH_TX_DATBIT);	}
				else if (strcmp(argv[2], "dt") == 0) {		eth_getset(ETH_TX_DATTYPE);	}
				else if(strcmp(argv[2], "ce") == 0) {		eth_getset(ETH_TX_CLKOE);	}
				else if(strcmp(argv[2], "cs") == 0) {		eth_getset(ETH_TX_CLKSEL);	}
				else if(strcmp(argv[2], "cr") == 0) {		eth_getset(ETH_TX_CRSCHK);	}
				else if(strcmp(argv[2], "co") == 0) {		eth_getset(ETH_TX_COLCHK);	}
				else if(strcmp(argv[2], "rn") == 0) {		eth_getset(ETH_TX_RTYEN);	}
				else if(strcmp(argv[2], "rl") == 0) {		eth_getset(ETH_TX_RTYLMT);	}
				else if(strcmp(argv[2], "adr") == 0) {		eth_getset(ETH_TX_ADR);	}
				else if(strcmp(argv[2], "len") == 0) {		eth_getset(ETH_TX_LEN);	}
				else if(strcmp(argv[2], "qos") == 0) {		eth_getset(ETH_TX_QOS);	}
				else if(strcmp(argv[2], "val") == 0) {		eth_getset(ETH_TX_VAL);	}
				else if(strcmp(argv[2], "p") == 0) {		eth_getset(ETH_TX_PAUSE_EN);}
				else if (strcmp(argv[2], "ifg") == 0) {		eth_getset(ETH_TX_IFGGAP);	}
				else if(strcmp(argv[2], "e") == 0) {		eth_getset(ETH_TX_CLKEDGE);	}
				else if (strcmp(argv[2], "tck") == 0) {		eth_getset(ETH_TX_TCKDLY);	}
				else if (strcmp(argv[2], "n") == 0) {		eth_getset(ETH_TX_TXENDLY);	}
				else if(strcmp(argv[2], "0") == 0) {		eth_getset(ETH_TX_TXD0DLY);	}
				else if(strcmp(argv[2], "1") == 0) {		eth_getset(ETH_TX_TXD1DLY);	}
				else if(strcmp(argv[2], "2") == 0) {		eth_getset(ETH_TX_TXD2DLY);	}
				else if(strcmp(argv[2], "3") == 0) {		eth_getset(ETH_TX_TXD3DLY);	}
				else if(strcmp(argv[2], "t") == 0) {		eth_getset(ETH_TX_TXENDLY);
															eth_getset(ETH_TX_TXD0DLY);
															eth_getset(ETH_TX_TXD1DLY);
															eth_getset(ETH_TX_TXD2DLY);
															eth_getset(ETH_TX_TXD3DLY);	}
#if 0
				else if(strcmp(argv[2], "dstck") == 0)
				{
					eth_getset(PAD_ETH_TCK_DS);
				}
				else if(strcmp(argv[2], "dsdata") == 0)
				{
					eth_getset(PAD_ETH_TXD3_DS);
					eth_getset(PAD_ETH_TXD2_DS);
					eth_getset(PAD_ETH_TXD1_DS);
					eth_getset(PAD_ETH_TXD0_DS);
					eth_getset(PAD_ETH_TXEN_DS);
				}
#endif
#if 0
				else if(strcmp(argv[2], "on") == 0)
				{
					gptMsgDebug.ETH_TX_CHECK = 1;
				}
				else if(strcmp(argv[2], "off") == 0)
				{
					gptMsgDebug.ETH_TX_CHECK = 0;
				}
				else
#endif
				else if (strcmp(argv[2], "start") == 0) {
					if (argc == 4) {
						eth_test_task_txtck = 1;
					} else {
						eth_test_task_txtck = 0;
					}
					vTaskCreate("eth_tx", cmd_eth_test_task, NULL, LV2_STACK_SIZE, LV5_TASK_PRIO);
				} else if (strcmp(argv[2], "stop") == 0) {
					eth_test_task_sw = 0;
				} else if (strcmp(argv[2], "size") == 0) {
					eth_getset(eth_test_size);
				} else {
					Shell_Unknown();
				}
			}
		} else if (strcmp(argv[1], "if") == 0) {
			if (argc == 3 && strcmp(argv[2], "on") == 0) {
				netifapi_netif_set_up(netif_state[enlETHERNET]._netif);
			} else if (argc == 3 && strcmp(argv[2], "off") == 0) {
				netifapi_netif_set_down(netif_state[enlETHERNET]._netif);
			} else if (argc == 3 && strcmp(argv[2], "auto") == 0) {
				netifapi_netif_set_down(netif_state[enlETHERNET]._netif);
				while (netif_is_up(netif_state[enlETHERNET]._netif)) {
					vTaskDelay(1);
				}
				EthphyAutoNeg(ENX_ON);
				//netifapi_netif_set_up(netif_state[enlETHERNET]._netif);
			}
#if 0
			else if(argc == 4 && strcmp(argv[2], "t1000") == 0)
			{
				if(strcmp(argv[3], "1000f") == 0)
				{
					EthRxTxinit(1, 1000, PHY_DUPLEX_FULL);
					printf("input(%s) => set 1000PHY 1000Mbps FULL\n", argv[3]);
				}
				else if(strcmp(argv[3], "100f") == 0)
				{
					EthRxTxinit(1, 100, PHY_DUPLEX_FULL);
					printf("input(%s) => set 1000PHY 100Mbps FULL\n", argv[3]);
				}
				else if(strcmp(argv[3], "100h") == 0)
				{
					EthRxTxinit(1, 100, PHY_DUPLEX_HALF);
					printf("input(%s) => set 1000PHY 100Mbps HALF\n", argv[3]);
				}
				else if(strcmp(argv[3], "10f") == 0)
				{
					EthRxTxinit(1, 10, PHY_DUPLEX_FULL);
					printf("input(%s) => set 1000PHY 10Mbps FULL\n", argv[3]);
				}
				else if(strcmp(argv[3], "10h") == 0)
				{
					EthRxTxinit(1, 10, PHY_DUPLEX_HALF);
					printf("input(%s) => set 1000PHY 10Mbps HALF\n", argv[3]);
				}
				else
				{
					Shell_Unknown();
				}
			}
			else if(argc == 4 && strcmp(argv[2], "t100") == 0)
			{
				if(strcmp(argv[3], "100f") == 0)
				{
					EthRxTxinit(0, 100, PHY_DUPLEX_FULL);
					printf("input(%s) => set 100PHY 100Mbps FULL\n", argv[3]);
				}
				else if(strcmp(argv[3], "100h") == 0)
				{
					EthRxTxinit(0, 100, PHY_DUPLEX_HALF);
					printf("input(%s) => set 100PHY 100Mbps HALF\n", argv[3]);
				}
				else if(strcmp(argv[3], "10f") == 0)
				{
					EthRxTxinit(0, 10, PHY_DUPLEX_FULL);
					printf("input(%s) => set 100PHY 10Mbps FULL\n", argv[3]);
				}
				else if(strcmp(argv[3], "10h") == 0)
				{
					EthRxTxinit(0, 10, PHY_DUPLEX_HALF);
					printf("input(%s) => set 100PHY 10Mbps HALF\n", argv[3]);
				}
				else
				{
					Shell_Unknown();
				}
			}
#endif
			else if (strcmp(argv[2], "mps") == 0) {		eth_getset(ETH_DSTMAC_BYP);	}
			else if (strcmp(argv[2], "men") == 0) {		eth_getset(ETH_DSTMAC_EN);	}
			else if (strcmp(argv[2], "mlist") == 0) {	EthRxFilterList();			}
			else if (argc == 9) {
				BYTE arrMac[6];
				arrMac[0] = strtol(argv[3], NULL, 16);
				arrMac[1] = strtol(argv[4], NULL, 16);
				arrMac[2] = strtol(argv[5], NULL, 16);
				arrMac[3] = strtol(argv[6], NULL, 16);
				arrMac[4] = strtol(argv[7], NULL, 16);
				arrMac[5] = strtol(argv[8], NULL, 16);
				printf("Input: %02X:%02X:%02X:%02X:%02X:%02X\n", arrMac[0], arrMac[1], arrMac[2], arrMac[3], arrMac[4], arrMac[5]);
				if (strcmp(argv[2], "madd") == 0) {
					ENX_OKFAIL ofRes = EthRxFilterInsert(arrMac);
					printf("EthMulFiltInsert: (%s)\n", ofRes == ENX_OK ? "OK" : "FAIL");
				} else if (strcmp(argv[2], "mdel") == 0) {
					ENX_OKFAIL ofRes = EthRxFilterDelete(arrMac);
					printf("EthRxFilterDelete: (%s)\n", ofRes == ENX_OK ? "OK" : "FAIL");
				}
			}
#if 0
			else if(argc == 4 && strcmp(argv[2], "mufj") == 0)
			{
				err_t ert = IGMP_Join(argv[3]);
				if(ert == ERR_OK)	printf("IGMP Join OK(%s)\n", argv[3]);
				else				printf("IGMP Join Error(%d)\n", ert);
			}
			else if(argc == 4 && strcmp(argv[2], "mufl") == 0)
			{
				err_t ert = IGMP_Leave(argv[3]);
				if(ert == ERR_OK)	printf("IGMP Leave OK(%s)\n", argv[3]);
				else				printf("IGMP Leave Error(%d)\n", ert);
			}
			else if(argc == 3 && strcmp(argv[2], "muflist") == 0)
			{
				igmp_mac_filter_list();
			}
#endif
			else if(argc == 5 && strcmp(argv[2], "mufvar") == 0)
			{
				UINT getValue = atoi(argv[3]);
				UINT setValue = atoi(argv[4]);
				printf("Set ETH_RX_MUL%dADR : %d\n", getValue, setValue);
#if 0
				switch(getValue)
				{
					case 0:		ETH_RX_MUL0ADR = setValue;		break;
					case 1:		ETH_RX_MUL1ADR = setValue;		break;
					case 2:		ETH_RX_MUL2ADR = setValue;		break;
					case 3:		ETH_RX_MUL3ADR = setValue;		break;
					case 4:		ETH_RX_MUL4ADR = setValue;		break;
					case 5:		ETH_RX_MUL5ADR = setValue;		break;
					case 6:		ETH_RX_MUL6ADR = setValue;		break;
					case 7:		ETH_RX_MUL7ADR = setValue;		break;
					case 8:		ETH_RX_MUL8ADR = setValue;		break;
					case 9:		ETH_RX_MUL9ADR = setValue;		break;
					case 10:	ETH_RX_MUL10ADR = setValue;		break;
					case 11:	ETH_RX_MUL11ADR = setValue;		break;
				}
#endif
			}
			else if(argc == 5 && strcmp(argv[2], "mufsw") == 0)
			{
				UINT getValue = atoi(argv[3]);
				UINT setValue = atoi(argv[4]);
				printf("Set ETH_RX_MUL%dFILT : %d\n", getValue, setValue);
#if 0
				switch(getValue)
				{
					case 0:		ETH_RX_MUL0FILT = setValue;		break;
					case 1:		ETH_RX_MUL1FILT = setValue;		break;
					case 2:		ETH_RX_MUL2FILT = setValue;		break;
					case 3:		ETH_RX_MUL3FILT = setValue;		break;
					case 4:		ETH_RX_MUL4FILT = setValue;		break;
					case 5:		ETH_RX_MUL5FILT = setValue;		break;
					case 6:		ETH_RX_MUL6FILT = setValue;		break;
					case 7:		ETH_RX_MUL7FILT = setValue;		break;
					case 8:		ETH_RX_MUL8FILT = setValue;		break;
					case 9:		ETH_RX_MUL9FILT = setValue;		break;
					case 10:	ETH_RX_MUL10FILT = setValue;	break;
					case 11:	ETH_RX_MUL11FILT = setValue;	break;
				}
#endif
			}
			else
			{
				Shell_Unknown();
			}
		}
#if (PHY_DEBUG==1)
		else if(argc == 3 && strcmp(argv[1], "phy") == 0)
		{
			BYTE getValue = (BYTE)(atoi(argv[2]) & 0xFF);
			WORD wBuf;
			MdioRead(phy_info.addr, getValue, &wBuf);
			PhyRegView(getValue, wBuf);
		}
#endif
#if (ETHPHY_LOOPBACK_TEST==1)
#if defined(__ETHPHY_KSZ8081MNX__)
		else if (argc == 2 && strcmp(argv[1], "lbm") == 0) {
			if (EthloopbackGetMode() == ePlk_off) {
				vTaskCreate("eth_lb", EthloopbackTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
			} else {
				printf("Ethernet loopback mode is ready.\n");
			}
		}
#elif defined(__ETHPHY_KSZ8081RNB__)
		else if (argc == 2 && strcmp(argv[1], "lbm") == 0) {
			if (EthloopbackGetMode() == ePlk_off) {
				vTaskCreate("eth_lb", EthloopbackTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
			} else {
				printf("Ethernet loopback mode is ready.\n");
			}
		}
#elif defined(__ETHPHY_KSZ9031RNX__)
		else if (argc == 3 && strcmp(argv[1], "lbm") == 0) {
			UINT nSpeed = atoi(argv[2]);
			if (nSpeed != 100 && nSpeed != 1000) {
				printf("Speed Error(%d)\n", nSpeed);
			} else {
				if (EthloopbackGetMode() == ePlk_off) {
					vTaskCreate("eth_lb", EthloopbackTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
				} else {
					printf("Ethernet loopback mode is ready.\n");
				}
			}
		}
#endif
		else if (argc == 3 && strcmp(argv[1], "lbt") == 0) {
			if (EthloopbackGetMode() == ePlk_ready) {
				UINT u32Loop = atoi(argv[2]);
				EthloopbackSetLoop(u32Loop);
				EthloopbackSetMode(ePlk_auto);
			} else {
				printf("Ethernet loopback mode is not ready.\n");
			}
		} else if (argc == 7 && strcmp(argv[1], "lbc") == 0) {
			if (EthloopbackGetMode() == ePlk_ready) {
				UINT u32Loop = atoi(argv[2]);
				BYTE u8TXe = atoi(argv[3]);
				BYTE u8TXd = atoi(argv[4]);
				BYTE u8RXe = atoi(argv[5]);
				BYTE u8RXd = atoi(argv[6]);
				EthloopbackSetLoop(u32Loop);
				EthRxTxClockDly(u8TXe, u8TXd, u8RXe, u8RXd);
				EthloopbackSetMode(ePlk_single);
			} else {
				printf("Ethernet loopback mode is not ready.\n");
			}
		}
#endif
		else
		{
			Shell_Unknown();
		}
	}
	flprintf("End\n");
	return 0;
}
#endif

#if (ETHPHY_SHELL_TEST==1)
static int ethphy_test_task_key = 0;

static void ethphy_test_irq(void *ctx)
{
	printf("Ethphy Test IRQ\n");
}

void ethphy_test_task(void *ctx)
{
	UINT u32Data = 0;
	while (ethphy_test_task_key) {
		u32Data = EthphyGetPHYID();
		printf("[0x%08X]\n", u32Data);
		vTaskDelay(100);
	}
	vTaskDelete(NULL);
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
	UINT u32Data = 0;
	if (argc == 1) {
		MdioRead(ethphy_info.addr, u32Reg, &u16Data);
		EthphyRegView(u32Reg, u16Data);
	} else {
		if (strcmp("init", argv[1]) == 0) {
			EthphyInit(ETHPHY_MDIO_ADR, ethphy_test_irq);
		} else if (strcmp("get", argv[1]) == 0) {
#if 1
			u32Data = EthphyGetPHYID();
			printf("[0x%08X]\n", u32Data);
#else
			UINT u32LinkStatus = EthphyLinkCheck();
			switch (u32LinkStatus) {
				case ETHPHY_LINKSTATUS_UP:
					printf("ETHPHY_LINKSTATUS_UP\n");
					break;
				case ETHPHY_LINKSTATUS_DOWN:
					printf("ETHPHY_LINKSTATUS_DOWN\n");
					break;
				case ETHPHY_LINKSTATUS_ERROR:
					printf("ETHPHY_LINKSTATUS_ERROR\n");
					break;
				default:
					printf("ETHPHT_case_errror\n");
					break;
				}
#endif
		} else if (strcmp("clk", argv[1]) == 0) {
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
		} else if (strcmp("reg0", argv[1]) == 0) {
			//ETH_MDIO_REG0_T = atoi(argv[2]);
		} else if (strcmp("show", argv[1]) == 0) {
			//printf("ETH_MDIO_REG0_T: 0x%08X\n", ETH_MDIO_REG0_T);
			printf("ETH_MDIO_RXEDGE: %u\n", ETH_MDIO_RXEDGE);
			printf("ETH_MDIO_TXEDGE: %u\n", ETH_MDIO_TXEDGE);
			printf("ETH_MDIO_RDLTC: %u\n", ETH_MDIO_RDLTC);
		} else if (strcmp("rxe", argv[1]) == 0) {
			ETH_MDIO_RXEDGE = atoi(argv[2]);
			printf("ETH_MDIO_RXEDGE: %u\n", ETH_MDIO_RXEDGE);
		} else if (strcmp("txe", argv[1]) == 0) {
			ETH_MDIO_TXEDGE = atoi(argv[2]);
			printf("ETH_MDIO_TXEDGE: %u\n", ETH_MDIO_TXEDGE);
		} else if (strcmp("ltc", argv[1]) == 0) {
			ETH_MDIO_RDLTC = atoi(argv[2]);
			printf("ETH_MDIO_RDLTC: %u\n", ETH_MDIO_RDLTC);
		} else if (strcmp("loopstart", argv[1]) == 0) {
			ethphy_test_task_key = 1;
			vTaskCreate("ethphy_test", ethphy_test_task, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
		} else if (strcmp("loopstop", argv[1]) == 0) {
			ethphy_test_task_key = 0;
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
#endif
