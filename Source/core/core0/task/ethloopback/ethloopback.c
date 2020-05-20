#include "dev.h"
#include "ethphy.h"

#include "enx_freertos.h"

#include "networkif.h"

#if (ETHPHY_LOOPBACK_TEST==1)
#include "ethloopback.h"

extern void network_ethif_pkt_input_irq(void *ctx);
extern void network_ethif_pkt_input_loopback_irq(void *ctx);

EthLoopbackGp ethlp;

static eEthphyLoopbackRes EthphyLoopbackTest(EthLoopbackGp *elg)
{
	srand(rdcycle());
	for (UINT i = 0; i < ETHPHY_LOOPBACK_PACKET_CNT; i++) {
		hwflush_dcache_range((ULONG)elg->arrBuffer[i], ETHPHY_LOOPBACK_PACKET_SIZE);
		for (UINT j = 0; j < ETHPHY_LOOPBACK_PACKET_SIZE; j++) {
//			elg->arrBuffer[i][j] = j;
			elg->arrBuffer[i][j] = rand();
		}
		hwflush_dcache_range((ULONG)elg->arrBuffer[i], ETHPHY_LOOPBACK_PACKET_SIZE);
	}

	for (UINT i = 0; i < elg->u32Loop; i++) {
		elg->eRes = ePlb_idle;
		elg->u8Index = i % ETHPHY_LOOPBACK_PACKET_CNT;

#if 0
		hwflush_dcache_range((ULONG)elg->arrSendBuffer, ETHPHY_LOOPBACK_PACKET_SIZE);
		memcpy(elg->arrSendBuffer, elg->arrBuffer[ethlp.u8Index], ETHPHY_LOOPBACK_PACKET_SIZE);
		hwflush_dcache_range((ULONG)elg->arrSendBuffer, ETHPHY_LOOPBACK_PACKET_SIZE);
#else
		BDmaMemCpy_rtos(0, elg->arrSendBuffer, elg->arrBuffer[ethlp.u8Index], ETHPHY_LOOPBACK_PACKET_SIZE);
#endif

		EthTxPacket(elg->arrSendBuffer, ETHPHY_LOOPBACK_PACKET_SIZE);

		UINT idleCnt = 0;
		while (elg->eRes == ePlb_idle) {
			idleCnt++;
			WaitXus(1);
			if (idleCnt >= 1000) {
				break;
			}
		}
		if (elg->eRes == ePlb_idle) {
			elg->eRes = ePlb_timeout;
			break;
		}
		if (elg->eRes > ePlb_ok) {
			if (elg->eRes == ePlb_data) {
				if (gptMsgDebug.ETH_RX_CHECK == 2) {
					printf("Eth Rx Err loop(%d)\n", i);
				}
			}
			break;
		}
	}
	return elg->eRes;
}

static void EthLoopbackAuto(EthLoopbackGp *elg)
{
	//while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SW_VLOCK_IRQ, DEF_OFF) == ENX_FAIL) {
	//	vTaskDelay(1);
	//}

	int case_count = 0, pass_count = 0;
	int old_ok_acc = 0, ok_acc = 0, ok_txe = 0, ok_rxe = 0, ok_txd = 0, ok_rxd = 0;
	printf("Ethernet Loopback Test(step loop %d)\n", elg->u32Loop);
	printf("O:pass, 2:compare fail, 3:zero data, 4:data length underflow or overflow, X:timeout\n");
#if 0
	printf("          |            RX-Edge(1)           |\n");
	printf("RX-DLY	  | 0 1 2 3 4 5 6 7 8 9 A B C D E F |\n");
	for (BYTE u8TXd = 0; u8TXd < 16; u8TXd++) { // TX dly
		printf("\nTX-DLY(%X)\n", u8TXd);
		for(BYTE u8TXe = 1; u8TXe < 2; u8TXe++) { // TX edge
			printf("TX-Edge(%X)| ", u8TXe);
			for(BYTE u8RXe = 1; u8RXe < 2; u8RXe++) { // RX edge
				ok_acc = 0;
				for(BYTE u8RXd = 0; u8RXd < 16; u8RXd++) { // RX dly
					EthSetRxTxClockDly(u8TXe, u8TXd, u8RXe, u8RXd);
					UINT TestRes = EthphyLoopbackTest(elg);
					if (TestRes == ePlb_ok) {	// 0:none, idle
						_Gprintf("O ");			// 1:test oK
						ok_acc++;
						if (old_ok_acc < ok_acc) {
							old_ok_acc = ok_acc;
							ok_txd = u8TXd;
							ok_txe = u8TXe;
							ok_rxe = u8RXe;
							ok_rxd = u8RXd;
						}
					} else {
						if (TestRes == 5) {
							_Zprintf("X ");				// 5:timeout
						} else {						// 2:data error, compare fail
							_Rprintf("%d ", TestRes);	// 3:zero data
						}								// 4:data length underflow or overflow
					}
				}
				printf("| ");
			}
			printf("\n");
		}
	}
#else
	printf("          |            RX-Edge(0)           |            RX-Edge(1)           |\n");
	printf("RX-DLY	  | 0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3 4 5 6 7 8 9 A B C D E F |\n");
	for (BYTE u8TXd = 0; u8TXd < 16; u8TXd++) { // TX dly
		printf("\nTX-DLY(%X)\n", u8TXd);
		for(BYTE u8TXe = 0; u8TXe < 2; u8TXe++) { // TX edge
			printf("TX-Edge(%X)| ", u8TXe);
			for(BYTE u8RXe = 0; u8RXe < 2; u8RXe++) { // RX edge
				ok_acc = 0;
				for(BYTE u8RXd = 0; u8RXd < 16; u8RXd++) { // RX dly
					EthSetRxTxClockDly(u8TXe, u8TXd, u8RXe, u8RXd);
					UINT TestRes = EthphyLoopbackTest(elg);
					case_count++;
					if (TestRes == ePlb_ok) {	// 0:none, idle
						pass_count++;
						_Gprintf("O ");			// O:test oK
						ok_acc++;
						if (old_ok_acc < ok_acc) {
							old_ok_acc = ok_acc;
							ok_txd = u8TXd;
							ok_txe = u8TXe;
							ok_rxe = u8RXe;
							ok_rxd = u8RXd;
						}
					} else {
						if (TestRes == 5) {
							_Zprintf("X ");				// X:timeout
						} else {						// 2:data error, compare fail
							_Rprintf("%d ", TestRes);	// 3:zero data
						}								// 4:data length underflow or overflow
					}
				}
				printf("| ");
			}
			printf("\n");
		}
	}
#endif

//	printf("AccOK(%d) TXe(%d) TXd(%d) RXe(%d) RXd(%d)\n", old_ok_acc, ok_txe, ok_txd, ok_rxe, ok_rxd);
	{
		char buf[64] = {0};
		snprintf(buf, 64, "%4d/%4d, %.2f%%", pass_count, case_count, (pass_count / (float)case_count) * 100.0);
		printf("\nTest done: %s\n", buf);
	}

	//while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SW_VLOCK_IRQ, DEF_ON) == ENX_FAIL) {
	//	vTaskDelay(1);
	//}
}

static ENX_OKFAIL EthloopbackMalloc(EthLoopbackGp *elg)
{
	BYTE u8AllocReady = 0;

	if (elg->arrSendBuffer) {
		vPortFree(elg->arrSendBuffer);
		elg->arrSendBuffer = NULL;
	}
	elg->arrSendBuffer = pvPortMalloc(ETHPHY_LOOPBACK_PACKET_SIZE);
	u8AllocReady++;

	for (int i = 0; i < ETHPHY_LOOPBACK_PACKET_CNT; i++) {
		if (elg->arrBuffer[i]) {
			vPortFree(elg->arrBuffer[i]);
			elg->arrBuffer[i] = NULL;
		}
		elg->arrBuffer[i] = pvPortMalloc(ETHPHY_LOOPBACK_PACKET_SIZE);
		if (elg->arrBuffer[i]) {
			u8AllocReady++;
		}
	}

	if (u8AllocReady == ETHPHY_LOOPBACK_PACKET_CNT + 1) {
		return ENX_OK;
	} else {
		return ENX_FAIL;
	}
}

static void EthloopbackFree(EthLoopbackGp *elg)
{
	for (int i = 0; i < ETHPHY_LOOPBACK_PACKET_CNT; i++) {
		if (ethlp.arrBuffer[i]) {
			vPortFree(ethlp.arrBuffer[i]);
			ethlp.arrBuffer[i] = NULL;
		}
	}

	if (elg->arrSendBuffer) {
		vPortFree(elg->arrSendBuffer);
		elg->arrSendBuffer = NULL;
	}
}

void EthloopbackTask(void *ctx)
{
	vTaskDelay(100);
	eEthphyLoopbackRes lpRes;
#if EN675_SINGLE
	ENX_SWITCH txpd, rxpd;
	txpd = EthGetTxClockPowerEn();
	rxpd = EthGetRxClockPowerEn();
	EthSetTxClockPowerEn(ENX_ON);
	EthSetRxClockPowerEn(ENX_ON);
#endif
	ethlp.eRunMode = ePlk_init;
	ethlp.u32Loop = 0;
	ethlp.u8Index = 0;

	ENX_OKFAIL nRes = EthloopbackMalloc(&ethlp);
	if (nRes == ENX_OK) {
		EthSetRxEn(ENX_OFF);
		vTaskDelay(1);
		EthSetRxIrqEn(ENX_OFF);
		vTaskDelay(1);

		netifapi_netif_set_down(netif_state[enlETHERNET]._netif);
		while (netif_is_up(netif_state[enlETHERNET]._netif)) {
			vTaskDelay(1);
		}

		EthphyLoopbackMode(ethlp.u32Speed, ethlp.u32Duplex);

		EthRxIrqCallback(network_ethif_pkt_input_loopback_irq, &ethlp);
		EthSetRxIrqEn(ENX_ON);

		do {
			printf("Ready!\n");
			ethlp.eRunMode = ePlk_ready;
			while (ethlp.eRunMode == ePlk_ready) {
				vTaskDelay(100);
			}

			switch (ethlp.eRunMode) {
			case ePlk_single:
				EthSetRxEn(ENX_ON);
				lpRes = EthphyLoopbackTest(&ethlp);
				EthSetRxEn(ENX_OFF);
				printf("The test is complete(%d).\n", lpRes);
				break;
			case ePlk_auto:
				EthSetRxEn(ENX_ON);
				EthLoopbackAuto(&ethlp);
				EthSetRxEn(ENX_OFF);
				printf("The test is complete.\n");
				break;
			case ePlk_stop:
				printf("Ethernet Loopback Test: kill\n");
				break;
			default:
				printf("Invalid mode selected.\n");
				break;
			}
		} while (ethlp.eRunMode != ePlk_stop);

		EthSetRxEn(ENX_OFF);
		vTaskDelay(1);
		EthSetRxIrqEn(ENX_OFF);
		vTaskDelay(1);
	} else {
		printf("Ethernet Loopback Test: Malloc fail\n");
	}

	EthloopbackFree(&ethlp);

#if EN675_SINGLE
	EthSetTxClockPowerEn(txpd);
	EthSetRxClockPowerEn(rxpd);
#endif

	ethlp.eRunMode = ePlk_off;
	vTaskDelete(NULL);
}

void EthloopbackSetSpeed(UINT speed)
{
	ethlp.u32Speed = speed;
}

void EthloopbackSetDuplex(UINT duplex)
{
	ethlp.u32Duplex = duplex;
}

eEthphyLoopbackMode EthloopbackGetMode(void)
{
	return ethlp.eRunMode;
}

void EthloopbackSetMode(eEthphyLoopbackMode eRunMode)
{
	ethlp.eRunMode = eRunMode;
}

void EthloopbackSetLoop(UINT u32Loop)
{
	ethlp.u32Loop = u32Loop;
}

void EthloopbackSetOption(BYTE u8Option)
{
	ethlp.u8Option = u8Option;
}
#endif
