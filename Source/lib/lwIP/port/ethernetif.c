#include "dev.h"
#include "enx_freertos.h"
#include "networkif.h"
#include "ethphy.h"

#ifdef __ETHERNET__
//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
QueueNetRx_t qEthernetRX;
QueueNetTx_t qEthernetTX;

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Mac Filter
err_t igmp_mac_filter_set(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
	//Make MAC Addr from IP Multicast Addr
	BYTE macAddr[6] = {0x01, 0x00, 0x5E};
	macAddr[3] = (BYTE)(0x7f & (group->addr >> 16));
	macAddr[4] = (BYTE)(0xff & (group->addr >> 8));
	macAddr[5] = (BYTE)(0xff & (group->addr));

	char strBuf[16] = {0};
	ip4addr_ntoa_r(group, strBuf, sizeof(strBuf));
	if (action == NETIF_ADD_MAC_FILTER) {
		if (EthRxFilterInsert(macAddr) == ENX_OK) {
			printf("IGMP: Add %s to the mac filter.\n", strBuf);
		} else {
			printf("IGMP: Failed to add %s.\n", strBuf);
		}
	} else {
		if (EthRxFilterDelete(macAddr) == ENX_OK) {
			printf("IGMP: Delete %s to the mac filter.\n", strBuf);
		} else {
			printf("IGMP: Failed to delete %s.\n", strBuf);

		}
	}

	return ERR_OK;
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Ethernet TX
static err_t low_level_ethif_output(struct netif *netif, struct pbuf *p)
{
	if (wltx_lock()) {
#if ETH_PAD_SIZE
		pbuf_header(p, -ETH_PAD_SIZE); // Drop the padding word
#endif

		struct pbuf *q = p;
#if 0
		while (q) {
			hexDump("Ethernet Packet TX", q->payload, q->len);
			q = q->next;
		}
#else
		UINT nTotLen = 0;
		BYTE *pBase = qEthernetTX.pkt_data[qEthernetTX.index].buffer;

#if 1
		while (q) {
			hwflush_dcache_range_rtos((ULONG)q->payload, q->len);
			BDmaMemCpy_rtos(0, (BYTE *)(pBase + nTotLen), q->payload, q->len);
			nTotLen += q->len;
			q = q->next;
		}
#else
		//portENTER_CRITICAL();
		hwflush_dcache_range_rtos((ULONG)pBase, p->tot_len);
		while (q) {
			//hexDump("Ethernet Packet TX", q->payload, q->len);
			memcpy(pBase + nTotLen, q->payload, q->len);
			//CDmaMemCpy_rtos(0, (BYTE *)(pBase + nTotLen), q->payload, q->len);
			nTotLen += q->len;
			q = q->next;
		}
		hwflush_dcache_range_rtos(pBase, nTotLen);
		//portEXIT_CRITICAL();
#endif

		if (nTotLen != p->tot_len) {
			printf("%s: nTotLen(%d) != p->tot_len(%d)\r\n", __func__, nTotLen, p->tot_len);
			nTotLen = p->tot_len;
		}

		//printf("TX: 0x%08X(%u/%u)\n", pBase, nTotLen, p->tot_len);
		//hexDump("TX", pBase, p->tot_len);
		if (p->tot_len < 60) {
			nTotLen = 60;
			//vTaskDelay(100);
		}
		EthTxPacket(pBase, nTotLen);

		num_loop(qEthernetTX.index, NETTX_BUF_COUNT);	// next head
#endif

		wltx_unlock();
#if ETH_PAD_SIZE
		pbuf_header(p, ETH_PAD_SIZE); // Reclaim the padding word
#endif
		return ERR_OK;
	}
    return ERR_IF;
	UNUSED(netif);
}


//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Ethernet RX
static UINT *pRX_LEN_INFO = (UINT *)(REG_BASE_ETH + 0x80000);
static BYTE gRxPktTail = 0;

static void network_ethif_pkt_input_irq(void *ctx)
{
//	struct netif *netif = netif_state[enlETHERNET]._netif;
	BYTE gRxPktHead = ((pRX_LEN_INFO[gRxPktTail]&0xff000000)>>24);
	while (gRxPktTail != gRxPktHead) {
		UINT u32PktSize = (pRX_LEN_INFO[gRxPktTail] & 0x7ff) - 4;
		if (gptMsgDebug.ETH_RX_CHECK) {
			printf("EthRX Idx[%s%3u%s/%s%3u%s] - %u\n", TTY_COLOR_YELLOW, gRxPktHead, TTY_COLOR_RESET, TTY_COLOR_GREEN, gRxPktTail, TTY_COLOR_RESET, u32PktSize);
		}
		pkt_info *pkinfo;
		if (u32PktSize <= 1514) {
			pkinfo = &(qEthernetRX.info[qEthernetRX.tail]);
			pkinfo->data = qEthernetRX.pkt_data[gRxPktTail].buffer;
			pkinfo->lenth = u32PktSize;
			pkinfo->type = enlETHERNET;
			pkinfo->flag = 0;
			num_loop(qEthernetRX.tail, NETRX_BUF_COUNT);	// next head

//			hwflush_dcache_range((ULONG)pkinfo->data, (UINT)u32PktSize+64);
//			hexDump("IP input", pkinfo->data, u32PktSize+64);
		} else {
			_Rprintf("EthRX Err(%u/%u)(Size:%u)\n", gRxPktHead, gRxPktTail, u32PktSize);
		}

#if 0
		hwflush_dcache_range((ULONG)pkinfo->data, 128);
		struct eth_hdr *ethhdr = (struct eth_hdr *)pkinfo->data;
		switch (htons(ethhdr->type)) {
			case ETHTYPE_IP:
			case ETHTYPE_ARP:
			case ETHTYPE_IPV6:
			case ETHTYPE_WOL: // Wake-on-LAN[9]
			case 0x22F3: // IETF TRILL Protocol
			case 0x22EA: // Stream Reservation Protocol
			case 0x6003: // DECnet Phase IV
			case ETHTYPE_RARP: // Reverse Address Resolution Protocol
			case 0x809B: // AppleTalk (Ethertalk)
			case 0x80F3: // AppleTalk Address Resolution Protocol (AARP)
			case ETHTYPE_VLAN: // VLAN-tagged frame (IEEE 802.1Q) and Shortest Path Bridging IEEE 802.1aq with NNI compatibility[10]
			case 0x8137: // IPX
			case 0x8204: // QNX Qnet
			case 0x8808: // Ethernet flow control
			case 0x8809: // Ethernet Slow Protocols[11]
			case 0x8819: // CobraNet
			case 0x8847: // MPLS unicast
			case 0x8848: // MPLS multicast
			case ETHTYPE_PPPOEDISC: // PPPoE Discovery Stage
			case ETHTYPE_PPPOE: // PPPoE Session Stage
			case 0x886D: // Intel Advanced Networking Services [12]
			case ETHTYPE_JUMBO: // Jumbo Frames (Obsoleted draft-ietf-isis-ext-eth-01)
			case 0x887B: // HomePlug 1.0 MME
			case 0x888E: // EAP over LAN (IEEE 802.1X)
			case ETHTYPE_PROFINET: // PROFINET Protocol
			case 0x8899: // Realtek Remote Control Protocol
			case 0x889A: // HyperSCSI (SCSI over Ethernet)
			case 0x88A2: // ATA over Ethernet
			case ETHTYPE_ETHERCAT: // EtherCAT Protocol
			case 0x88A8: // Provider Bridging (IEEE 802.1ad) & Shortest Path Bridging IEEE 802.1aq[10]
			case 0x88AB: // Ethernet Powerlink[citation needed]
			case 0x88B8: // GOOSE (Generic Object Oriented Substation event)
			case 0x88B9: // GSE (Generic Substation Events) Management Services
			case 0x88BA: // SV (Sampled Value Transmission)
			case ETHTYPE_LLDP: // Link Layer Discovery Protocol (LLDP)
			case ETHTYPE_SERCOS: // SERCOS III
			case 0x88DC: // WSMP, WAVE Short Message Protocol
			case 0x88E1: // HomePlug AV MME[citation needed]
			case ETHTYPE_MRP: // Media Redundancy Protocol (IEC62439-2)
			case 0x88E5: // MAC security (IEEE 802.1AE)
			case 0x88E7: // Provider Backbone Bridges (PBB) (IEEE 802.1ah)
			case ETHTYPE_PTP: // Precision Time Protocol (PTP) over Ethernet (IEEE 1588)
			case 0x88FB: // Parallel Redundancy Protocol (PRP)
			case 0x8902: // IEEE 802.1ag Connectivity Fault Management (CFM) Protocol / ITU-T Recommendation Y.1731 (OAM)
			case 0x8906: // Fibre Channel over Ethernet (FCoE)
			case 0x8914: // FCoE Initialization Protocol
			case 0x8915: // RDMA over Converged Ethernet (RoCE)
			case 0x891D: // TTEthernet Protocol Control Frame (TTE)
			case 0x892F: // High-availability Seamless Redundancy (HSR)
			case 0x9000: // Ethernet Configuration Testing Protocol[13]
			case ETHTYPE_QINQ: // VLAN-tagged (IEEE 802.1Q) frame with double tagging
				break;
			default:
				printf("payload(0x%08X) len(%u/%u)\n", (UINT)(intptr_t)pkinfo->data, 128, u32PktSize);
				hexDump("IP input error", pkinfo->data, 128);
//				portMEMORY_BARRIER();
//				hexDump("IP input error2", pkinfo->data, 128);

				EthSetRxIrqEn(ENX_OFF);
				EthSetRxEn(ENX_OFF);

				pkinfo = &(qEthernetRX.info[qEthernetRX.tail-2]);
				hexDump("IP input error1", pkinfo->data, pkinfo->lenth);

				break;
		}
#endif

		gRxPktTail++; // 0 ~ 255
		gRxPktHead = ((pRX_LEN_INFO[gRxPktTail]&0xff000000)>>24); // next header
	}
#if 1
	if (netif_state[enlETHERNET].xrx_notity) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(netif_state[enlETHERNET].xrx_notity, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;
		}
	}
#endif
}

#if (ETHPHY_LOOPBACK_TEST==1)
void network_ethif_pkt_input_loopback_irq(void *ctx)
{
	EthLoopbackGp *ethlp = (EthLoopbackGp *)ctx;
	BYTE gRxPktHead = ((pRX_LEN_INFO[gRxPktTail]&0xff000000)>>24);
	while (gRxPktTail != gRxPktHead) {
		//UINT u32PktSize = (*pRX_LEN_INFO & 0x7ff) - 4;
		UINT u32PktSize = (pRX_LEN_INFO[gRxPktTail] & 0x7ff) - 4;

//		printf("RX: 0x%08X(%u)\n", qEthernetRX.pkt_data[gRxPktTail].buffer, u32PktSize);

		if (u32PktSize <= 1518 && u32PktSize >= 60) {
			// D-cache flush
			hwflush_dcache_range((ULONG)qEthernetRX.pkt_data[gRxPktTail].buffer, u32PktSize);

			//printf("H(%d) T(%d)\n", gRxPktHead, gRxPktTail);
			//hexDump("RECV", qEthernetRX.pkt_data[gRxPktTail].buffer, u32PktSize);

			// Data compare
			for (UINT i = 0; i < u32PktSize; i++) {
				if (qEthernetRX.pkt_data[gRxPktTail].buffer[i] != ethlp->arrBuffer[ethlp->u8Index][i]) {
					ethlp->eRes = ePlb_data;
					break;
				}
			}

			if (ethlp->eRes == ePlb_idle) {
				ethlp->eRes = ePlb_ok;
			}
		} else {
			if (u32PktSize == 0) {
				ethlp->eRes = ePlb_zero;
			} else {
				ethlp->eRes = ePlb_outside;
			}
		}

		gRxPktTail++;
		gRxPktHead = ((pRX_LEN_INFO[gRxPktTail]&0xff000000)>>24);
	}
}
#endif

uint64 TTa, TTb;

static struct pbuf *low_level_input(struct netif *netif)
{
	TTa = *mtime;
	struct pbuf *p = NULL;
	if (cNRQueue_isempty(&qEthernetRX) == ENX_FAIL) {
		portENTER_CRITICAL();

		pkt_info *pkinfo = &(qEthernetRX.info[qEthernetRX.head]);
		WORD wLen = pkinfo->lenth;
		BYTE *pkt = (BYTE *)pkinfo->data;

//		printf("RX: 0x%08X(%u)\n", pkt, wLen);
#if 1
#if 0
		p = pbuf_alloc(PBUF_RAW, wLen + ETH_PAD_SIZE, PBUF_REF);
		if (p == NULL) {
			printf("pbuf_alloc fail H(%04d) T(%04d) Size(%04d)\n", qEthernetRX.head, qEthernetRX.tail, wLen);
		} else {
#if ETH_PAD_SIZE
			pbuf_header(p, -ETH_PAD_SIZE); // Drop the padding word
#endif
			hwflush_dcache_range((ULONG)pkt, wLen);
			p->payload = pkt;
			//printf("p->len(%u) wLen(%u)\n", p->len, wLen);

#if ETH_PAD_SIZE
			pbuf_header(p, ETH_PAD_SIZE); // Reclaim the padding word
#endif
		}
#else
		p = pbuf_alloc(PBUF_RAW, wLen + ETH_PAD_SIZE, PBUF_POOL);
		if (p == NULL) {
			printf("pbuf_alloc fail H(%04d) T(%04d) Size(%04d)\n", qEthernetRX.head, qEthernetRX.tail, wLen);
		} else {
			//if (p->len > 1536) {
			//	printf("0x%08X (%u)\n", p->payload, (UINT)p->len);
			//}
#if ETH_PAD_SIZE
			pbuf_header(p, -ETH_PAD_SIZE); // Drop the padding word
#endif
			WORD nTotLen = 0;
			//portENTER_CRITICAL();
			//if (p->next != NULL) {
			//	printf("P-next(0x%08X)\n", p->next);
			//}
			hwflush_dcache_range((ULONG)pkt, wLen);
			//portMEMORY_BARRIER();
			//if (p->next != NULL) {
			//	printf("P-next(0x%08X)\n", p->next);
			//}

			for (struct pbuf *q = p; q != NULL; q = q->next) {
				//if (q->next != NULL) {
				//	printf("Q-next(0x%08X)\n", q->next);
				//}
				if (q->len) { // Copy to "pbuf"
#if 0
					if (q->len > 1536) {
						printf("1 0x%08X (%u)\n", q->payload, (UINT)q->len);
					}
					hwflush_dcache_range((ULONG)q->payload, q->len);
					if (q->len > 1536) {
						printf("2 0x%08X <- 0x%08X (%u)(%u)\n", q->payload, pkt + nTotLen, (UINT)q->len, p->len);
						printf("3 p(0x%08X) q(0x%08X) %s %s \n", p, q, p->next == NULL ? "X" : "O", q->next == NULL ? "X" : "O");
						while(1);
					}
					memcpy(q->payload, pkt + nTotLen, (UINT)q->len);
					//CDmaMemCpy_isr(0, q->payload, pkt + nTotLen, (UINT)q->len);
					hwflush_dcache_range((ULONG)q->payload, q->len);
#else
					//if (q->len > 1536) {
					//	printf("1 0x%08X (%u/%u)\n", q->payload, (UINT)q->len, nTotLen);
					//}
					//printf("q->next(0x%08X-", q->next);
#if 0
					CDmaMemCpy_isr(0, q->payload, pkt + nTotLen, (UINT)q->len);
#else
					hwflush_dcache_range((ULONG)q->payload, (UINT)q->len);
					//portMEMORY_BARRIER();
					BDmaMemCpy_isr(0, q->payload, pkt + nTotLen, (UINT)q->len);
					//portMEMORY_BARRIER();
					//hwflush_dcache_range((ULONG)q->payload, (UINT)q->len);
#endif
					//printf("0x%08X)\n", q->next);
					//if (q->len > 1536) {
					//	printf("2 0x%08X (%u/%u)\n", q->payload, (UINT)q->len, nTotLen);
					//}
#endif
				}
				nTotLen += q->len;
			}
			//portEXIT_CRITICAL();
#if ETH_PAD_SIZE
			pbuf_header(p, ETH_PAD_SIZE); // Reclaim the padding word
#endif
		}
#endif
#endif
		num_loop(qEthernetRX.head, NETRX_BUF_COUNT);

		portEXIT_CRITICAL();
	}

	return p;
	UNUSED(netif);
}

void network_ethif_check_buffer(void)
{
	printf("qEthernetRX.pkt_data: 0x%08lX\n", (intptr_t)qEthernetRX.pkt_data);
	printf("qEthernetRX.base    : 0x%08lX\n", (intptr_t)qEthernetRX.base);
	char buf[64];
	for (int i = 0; i < 256; i++) {
		sprintf(buf, "Idx:%u", i);
		hexDump(buf, qEthernetRX.pkt_data[i].buffer, 1536);
	}
}

static void network_ethif_pkt_input(void *ctx)
{
	struct netif *netif;
	struct eth_hdr *ethhdr;
	struct pbuf *p;
	netif_state_t *priv;
	err_t ert;

	netif = (struct netif *)ctx;
	ENX_ASSERT(netif);
	priv = (netif_state_t *)netif->state;
	ENX_ASSERT(priv);

	BYTE *pBase = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(NETRX_BUF_COUNT*NETRX_BUF_GAP)+NETRX_BUF_GAP);
	if (pBase == NULL) {
		printf("Error! Ethernet Rx Base malloc Fail!\n");
		while (1);
	} else {
		qEthernetRX.base = (BYTE *)(((intptr_t)(pBase) + NETRX_BUF_GAP - 1) & ~(intptr_t)(NETRX_BUF_GAP - 1));
		_Cprintf("Ethernet Rx Base(0x%08lX) gap(%lu) count(%lu) size(%lu)\n", (ULONG)qEthernetRX.base, NETRX_BUF_GAP, NETRX_BUF_COUNT, NETRX_BUF_COUNT * NETRX_BUF_GAP);
		EthRxSetBuffer(qEthernetRX.base, NETRX_BUF_COUNT);
		qEthernetRX.pkt_data = (void *)qEthernetRX.base;
		qEthernetRX.head = 0;
		qEthernetRX.tail = 0;
	}

	hwflush_dcache_range_rtos((ULONG)pBase, 16*1024);
	memset(pBase, 0xAA, ENX_MEM_ALIGN_BUFFER(NETRX_BUF_COUNT*NETRX_BUF_GAP));
	hwflush_dcache_range_rtos((ULONG)pBase, 16*1024);

	EthRxIrqCallback(network_ethif_pkt_input_irq, NULL);
	EthSetRxIrqEn(ENX_ON);
	EthSetRxEn(ENX_ON);

	for (;;) {
		if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
			while ((p = low_level_input(netif))) {
#if 1
				hwflush_dcache_range((ULONG)p->payload, (UINT)p->len);
				// points to packet payload, which starts with an Ethernet header
				ethhdr = (struct eth_hdr *)p->payload;
				switch (htons(ethhdr->type)) {
					// IP or ARP packet ?
					case ETHTYPE_IP:
					case ETHTYPE_ARP:
						// full packet send to tcpip_thread to process
						if ((ert = netif->input(p, netif)) != ERR_OK) {
							LWIP_DEBUGF(NETIF_DEBUG, ("if_input: IP input error\n"));
							pbuf_free(p);
							p = NULL;
						}
						TTb = *mtime;
						if((TTb - TTa) > 15000) {
							printf("b-a:%lu\n", TTb-TTa);
						}
						break;
					case ETHTYPE_IPV6:
						LWIP_DEBUGF(NETIF_DEBUG, ("if_input: IPv6 is not supported.\n"));
						pbuf_free(p);
						p = NULL;
						break;
					case ETHTYPE_WOL: // Wake-on-LAN[9]
					case 0x22F3: // IETF TRILL Protocol
					case 0x22EA: // Stream Reservation Protocol
					case 0x6003: // DECnet Phase IV
					case ETHTYPE_RARP: // Reverse Address Resolution Protocol
					case 0x809B: // AppleTalk (Ethertalk)
					case 0x80F3: // AppleTalk Address Resolution Protocol (AARP)
					case ETHTYPE_VLAN: // VLAN-tagged frame (IEEE 802.1Q) and Shortest Path Bridging IEEE 802.1aq with NNI compatibility[10]
					case 0x8137: // IPX
					case 0x8204: // QNX Qnet
					case 0x8808: // Ethernet flow control
					case 0x8809: // Ethernet Slow Protocols[11]
					case 0x8819: // CobraNet
					case 0x8847: // MPLS unicast
					case 0x8848: // MPLS multicast
					case ETHTYPE_PPPOEDISC: // PPPoE Discovery Stage
					case ETHTYPE_PPPOE: // PPPoE Session Stage
					case 0x886D: // Intel Advanced Networking Services [12]
					case ETHTYPE_JUMBO: // Jumbo Frames (Obsoleted draft-ietf-isis-ext-eth-01)
					case 0x887B: // HomePlug 1.0 MME
					case 0x888E: // EAP over LAN (IEEE 802.1X)
					case ETHTYPE_PROFINET: // PROFINET Protocol
					case 0x8899: // Realtek Remote Control Protocol
					case 0x889A: // HyperSCSI (SCSI over Ethernet)
					case 0x88A2: // ATA over Ethernet
					case ETHTYPE_ETHERCAT: // EtherCAT Protocol
					case 0x88A8: // Provider Bridging (IEEE 802.1ad) & Shortest Path Bridging IEEE 802.1aq[10]
					case 0x88AB: // Ethernet Powerlink[citation needed]
					case 0x88B8: // GOOSE (Generic Object Oriented Substation event)
					case 0x88B9: // GSE (Generic Substation Events) Management Services
					case 0x88BA: // SV (Sampled Value Transmission)
					case ETHTYPE_LLDP: // Link Layer Discovery Protocol (LLDP)
					case ETHTYPE_SERCOS: // SERCOS III
					case 0x88DC: // WSMP, WAVE Short Message Protocol
					case 0x88E1: // HomePlug AV MME[citation needed]
					case ETHTYPE_MRP: // Media Redundancy Protocol (IEC62439-2)
					case 0x88E5: // MAC security (IEEE 802.1AE)
					case 0x88E7: // Provider Backbone Bridges (PBB) (IEEE 802.1ah)
					case ETHTYPE_PTP: // Precision Time Protocol (PTP) over Ethernet (IEEE 1588)
					case 0x88FB: // Parallel Redundancy Protocol (PRP)
					case 0x8902: // IEEE 802.1ag Connectivity Fault Management (CFM) Protocol / ITU-T Recommendation Y.1731 (OAM)
					case 0x8906: // Fibre Channel over Ethernet (FCoE)
					case 0x8914: // FCoE Initialization Protocol
					case 0x8915: // RDMA over Converged Ethernet (RoCE)
					case 0x891D: // TTEthernet Protocol Control Frame (TTE)
					case 0x892F: // High-availability Seamless Redundancy (HSR)
					case 0x9000: // Ethernet Configuration Testing Protocol[13]
					case ETHTYPE_QINQ: // VLAN-tagged (IEEE 802.1Q) frame with double tagging
						LWIP_DEBUGF(NETIF_DEBUG, ("if_input: inval type\n"));
						pbuf_free(p);
						p = NULL;
						break;
					default:
						printf("payload(0x%08lX) len(%u)\n", (intptr_t)p->payload, p->len);
						hwflush_dcache_range((ULONG)p->payload, (UINT)p->len+64);
						hexDump("IP input error + 64byte", p->payload, p->len+64);
#if 0
						hwflush_dcache_range((ULONG)p->payload, (UINT)p->len);
						portMEMORY_BARRIER();
						hexDump("IP input error", p->payload, p->len);
#endif
						LWIP_DEBUGF(NETIF_DEBUG, ("if_input: inval type\n"));
						pbuf_free(p);
						p = NULL;
						break;
				}
#else
				printf("payload(0x%08X) len(%u)\n", p->payload, p->len);
				pbuf_free(p);
				p = NULL;
#endif
			} // end of while((p = low_level_input(netif)))
		}
	}
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Ethernet Link/Status Callback
static void network_ethif_link_cb(struct netif *_netif)
{
	netif_state_t *_state = (netif_state_t *)_netif->state;
	if (netif_is_link_up(_netif)) {
		if (gtNetwork.naEthernet.u1UseDhcp == ENX_ON) {
#if LWIP_DHCP
			printf("Start DHCP negotiation for a network interface.\n");
			dhcp_start(_netif);
#endif
		} else {
			ip_addr_t ipaddr, netmask, gw;
			ip4_addr_set_u32(&ipaddr, gtNetwork.naEthernet.u32IpAddr);
			ip4_addr_set_u32(&netmask, gtNetwork.naEthernet.u32NetMask);
			ip4_addr_set_u32(&gw, gtNetwork.naEthernet.u32Gateway);
			netif_set_addr(_netif, &ipaddr, &netmask, &gw);
#if LWIP_DNS
			ip_addr_t dns0, dns1;
			ip4_addr_set_u32(&dns0, gtNetwork.naEthernet.u32DnsSvr0);
			ip4_addr_set_u32(&dns1, gtNetwork.naEthernet.u32DnsSvr1);
			dns_setserver(0, &dns0);							// Set DNS server 0 addr
			dns_setserver(1, &dns1);							//		"		  1  "
#endif
			netif_set_up(_netif);
		}
#if LWIP_DHCP
		struct dhcp* dhcp = netif_dhcp_data(_netif);
		es_printf("%s link up, bound to %IP%s\n", _state->ifname, _netif->ip_addr.addr, dhcp ? (dhcp->state ? ", DHCP negotiating..." : "") : "");
#else
		es_printf("%s link up, bound to %IP\n", _state->ifname, _netif->ip_addr.addr);
#endif
	} else {
		if (gtNetwork.naEthernet.u1UseDhcp == ENX_ON) {
#if LWIP_DHCP
			dhcp_release_and_stop(_netif);
#endif
		} else {
			netif_set_addr(_netif, NULL, NULL, NULL);
		}
#if LWIP_DNS
		dns_setserver(0, NULL);
		dns_setserver(1, NULL);
#endif
		printf("%s link down\n", _state->ifname);
	}

	network_default_netif();
}

static void network_ethif_status_cb(struct netif *_netif)
{
	netif_state_t *_state = (netif_state_t *)_netif->state;
	if (netif_is_up(_netif)) {
		printf("%s status up(link %s)", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
		if (netif_is_link_up(_netif)) {
#if LWIP_DHCP
			es_printf("(%s, %IP)\n", dhcp_supplied_address(_netif) ? "DHCP" : "static", _netif->ip_addr.addr);
#else
			es_printf("(%s, %IP)\n", "static", _netif->ip_addr.addr);
#endif
			if (gtNetwork.naEthernet.u1UseDhcp == ENX_ON) {
				gtNetwork.naEthernet.u32IpAddr = ip4_addr_get_u32(&_netif->ip_addr);
				gtNetwork.naEthernet.u32NetMask = ip4_addr_get_u32(&_netif->netmask);
				gtNetwork.naEthernet.u32Gateway = ip4_addr_get_u32(&_netif->gw);
#if LWIP_DNS
				gtNetwork.naEthernet.u32DnsSvr0 = ip4_addr_get_u32(dns_getserver(0));
				gtNetwork.naEthernet.u32DnsSvr1 = ip4_addr_get_u32(dns_getserver(1));
#endif
			} else {
#if LWIP_DHCP
				printf("Inform a DHCP server of our manual configuration.\n");
				dhcp_inform(_netif);
#endif
			}
		} else {
			printf("\n");
		}
	} else {
		printf("%s status down(link %s)\n", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
	}
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Ethernet Init
static err_t network_ethif_init(struct netif *netif)
{
//	int i;
//	for (i = 0; i < IGMP_LIST_LENS; i++) {
//		igmp_list[i].a = 0;
//	}

	LWIP_ASSERT("netif != NULL", (netif != NULL));

	netif->output = etharp_output;
	netif->linkoutput = low_level_ethif_output;
#if LWIP_IGMP
	netif_set_igmp_mac_filter(netif, igmp_mac_filter_set);
#endif

	netif_set_status_callback(netif, network_ethif_status_cb);
	netif_set_link_callback(netif, network_ethif_link_cb);

	/* Set MAC hardware address */
	netif->hwaddr[0] = gtSystem.arr8MacAddress[0];
	netif->hwaddr[1] = gtSystem.arr8MacAddress[1];
	netif->hwaddr[2] = gtSystem.arr8MacAddress[2];
	netif->hwaddr[3] = gtSystem.arr8MacAddress[3];
	netif->hwaddr[4] = gtSystem.arr8MacAddress[4];
	netif->hwaddr[5] = gtSystem.arr8MacAddress[5];
	EthRxFilterMacAdr(netif->hwaddr);
	printf("MacAddress : %02X:%02X:%02X:%02X:%02X:%02X\n", netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2], netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);

	return low_level_init(netif, EIFNAME0, EIFNAME1);
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Ethernet PHY link
static void network_ethif_ethphy_irq(void *ctx)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(netif_state[enlETHERNET].link_notity, &xHigherPriorityTaskWoken);
	if (xHigherPriorityTaskWoken) {
		gbXsrTaskSwitchNeeded = 1;
	}
}

static void network_ethif_link(void *ctx)
{
	vTaskDelay(10);
	EthphyInit(ETHPHY_MDIO_ADR, network_ethif_ethphy_irq);

	while (1) {
		if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
			UINT u32LinkStatus = EthphyLinkCheck();
			switch (u32LinkStatus) {
			case ETHPHY_LINKSTATUS_UP:
				network_interface_link(enlETHERNET, ENX_ON);
				break;
			case ETHPHY_LINKSTATUS_DOWN:
				network_interface_link(enlETHERNET, ENX_OFF);
				break;
			case ETHPHY_LINKSTATUS_ERROR:
				printf("Ethernet PHY restart!");
				EthphyInit(ETHPHY_MDIO_ADR, network_ethif_ethphy_irq);
				break;
			default:
				break;
			}
		}
	}
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
void network_ethif_start(void)
{
	BYTE *pBase = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(NETTX_BUF_COUNT*NETTX_BUF_GAP)+NETTX_BUF_GAP);
	if (pBase == NULL) {
		printf("Error! Ethernet Tx Base malloc Fail!\n");
		while (1);
	} else {
		qEthernetTX.base = (BYTE *)(((intptr_t)(pBase) + NETRX_BUF_GAP - 1) & ~(intptr_t)(NETRX_BUF_GAP - 1));
		_Cprintf("Ethernet Tx Base(0x%08lX) gap(%lu) count(%lu) size(%lu)\n", (ULONG)qEthernetTX.base, NETTX_BUF_GAP, NETTX_BUF_COUNT, NETTX_BUF_COUNT * NETTX_BUF_GAP);
		qEthernetTX.pkt_data = (void *)qEthernetTX.base;
		qEthernetTX.index = 0;
	}

	//EthTxIrqCallback(NULL, NULL);
	//EthTxIrqOn();

	memset(netif_state[enlETHERNET]._netif, 0, sizeof(struct netif));
	netif_add(netif_state[enlETHERNET]._netif, NULL, NULL, NULL, &netif_state[enlETHERNET], network_ethif_init, tcpip_input);

//	ChangeDefDeviceId();

	netif_state[enlETHERNET].link_notity = vTaskCreate("eifLink", network_ethif_link, NULL, LV2_STACK_SIZE, LV7_TASK_PRIO);
	netif_state[enlETHERNET].xrx_notity = vTaskCreate("eifRx", network_ethif_pkt_input, netif_state[enlETHERNET]._netif, LV5_STACK_SIZE, LV7_TASK_PRIO);
}
#endif
