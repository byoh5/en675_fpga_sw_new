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
// CRC
UINT crcTable[256] = {
		0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
		0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
		0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
		0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
		0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
		0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
		0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
		0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
		0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
		0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
		0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
		0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
		0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
		0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
		0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
		0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
		0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
		0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
		0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
		0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
		0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
		0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
		0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
		0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
		0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
		0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
		0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
		0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
		0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
		0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
		0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
		0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
		0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
		0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
		0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
		0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
		0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
		0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
		0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
		0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
		0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
		0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
		0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

UINT crcCompute(const BYTE *buf, size_t size)
{
	const BYTE *p = buf;
	UINT crc;

	crc = ~0U;
	while (size--)
		crc = crcTable[(crc ^ *p++) & 0xFF] ^ (crc >> 8);

	UINT celcrc = crc ^ ~0U;
	return htonl(celcrc);
}

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
void network_ethif_pkt_input_irq(void *ctx)
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

			if (gptMsgDebug.ETH_RX_CHECK) {
				hwflush_dcache_range((ULONG)pkinfo->data, (UINT)u32PktSize + 4);
				hexDump("Eth input(+CRC32)", pkinfo->data, u32PktSize + 4);
			}
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
			hwflush_dcache_range((ULONG)qEthernetRX.pkt_data[gRxPktTail].buffer, u32PktSize+4);

			//printf("H(%d) T(%d)\n", gRxPktHead, gRxPktTail);
			//hexDump("RECV", qEthernetRX.pkt_data[gRxPktTail].buffer, u32PktSize);
			if (gptMsgDebug.ETH_RX_CHECK) {
				printf("EthRX Idx[%s%3u%s/%s%3u%s] - %u\n", TTY_COLOR_YELLOW, gRxPktHead, TTY_COLOR_RESET, TTY_COLOR_GREEN, gRxPktTail, TTY_COLOR_RESET, u32PktSize);
				hexDump("ETH Input", qEthernetRX.pkt_data[gRxPktTail].buffer, u32PktSize+4);
			}

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
UINT _test_crc32;

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
#if ETH_PAD_SIZE
			pbuf_header(p, -ETH_PAD_SIZE); // Drop the padding word
#endif
			// Copy to pbuf
			WORD nTotLen = 0;
			for (struct pbuf *q = p; q != NULL; q = q->next) {
				if (q->len) { // Copy to "pbuf"
					hwflush_dcache_range((ULONG)q->payload, (UINT)q->len);
					//hwdiscard_dcache_range_rtos((ULONG)q->payload, (UINT)q->len);
					BDmaMemCpy_rtos(0, q->payload, pkt + nTotLen, (UINT)q->len);
					//hwdiscard_dcache_range_rtos((ULONG)q->payload, (UINT)q->len);
					hwflush_dcache_range((ULONG)q->payload, (UINT)q->len);
				}
				nTotLen += q->len;
			}
			/////////////////////////////////////////////////////////////////////////////////////
			// CRC
#if 0
			BYTE *data = pkt + wLen;
			_test_crc32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3]);
			UINT celcrc32 = crcCompute(pkt, wLen);
			if (_test_crc32 != celcrc32) {
				hwflush_dcache_range((ULONG)(intptr_t)pkt, wLen + 4);
				_test_crc32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3]);
				UINT celcrc32_re = crcCompute(pkt, wLen);
				if (_test_crc32 != celcrc32_re) {
					_Rprintf("Error! GetCRC:0x%08X CelCRC:0x%08X\n", _test_crc32, celcrc32);
					hexDump("CRC Error packet", pkt, wLen + 4);
					_Rprintf("Recheck fail! GetCRC:0x%08X CelCRC:0x%08X\n", _test_crc32, celcrc32_re);
				} else {
					printf("Recheck ok! GetCRC:0x%08X\n", _test_crc32);
				}
			}
#endif
			/////////////////////////////////////////////////////////////////////////////////////
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

	EthRxSetAddrOffset(NETRX_BUF_GAP);

	BYTE *pBase = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(NETRX_BUF_COUNT*NETRX_BUF_GAP)+NETRX_BUF_GAP);
	//BYTE *pBase = pvPortMalloc(ENX_MEM_ALIGN_BUFFER(NETRX_BUF_COUNT*NETRX_BUF_GAP)+NETRX_BUF_GAP+524288);
	if (pBase == NULL) {
		printf("Error! Ethernet Rx Base malloc Fail!\n");
		while (1);
	} else {
		qEthernetRX.base = (BYTE *)(((intptr_t)(pBase) + NETRX_BUF_GAP - 1) & ~(intptr_t)(NETRX_BUF_GAP - 1));
		//qEthernetRX.base = (BYTE *)(((intptr_t)(pBase) + 524288 - 1) & ~(intptr_t)(524288 - 1));
		_Cprintf("Ethernet Rx Base(0x%08lX) gap(%lu) count(%lu) size(%lu)\n", (ULONG)qEthernetRX.base, NETRX_BUF_GAP, NETRX_BUF_COUNT, NETRX_BUF_COUNT * NETRX_BUF_GAP);
		EthRxSetBuffer(qEthernetRX.base, NETRX_BUF_COUNT);
		qEthernetRX.pkt_data = (void *)qEthernetRX.base;
		qEthernetRX.head = 0;
		qEthernetRX.tail = 0;

//		*qEthernetRX.base = 0xaabbccdd;
	}

	hwflush_dcache_range_rtos((ULONG)pBase, 16*1024);
	memset(pBase, 0xAA, ENX_MEM_ALIGN_BUFFER(NETRX_BUF_COUNT*NETRX_BUF_GAP));
	hwflush_dcache_range_rtos((ULONG)pBase, 16*1024);

//	pmp_entry_set(4, PMP_R|PMP_L, (ULONG)qEthernetRX.base, 524288); 			// DDR enabled area
//	pmp_entry_set(5, PMP_R|PMP_W|PMP_X|PMP_L, 0x80000000ul, DDR1_SIZE); // DDR enabled area
//	pmp_entry_set(6, PMP_L, 0x80000000ul, 0x20000000ul);				// DDR disabled area

	EthRxIrqCallback(network_ethif_pkt_input_irq, NULL);
	EthSetRxIrqEn(ENX_ON);
	EthSetRxEn(ENX_ON);

	for (;;) {
		if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
			while ((p = low_level_input(netif))) {
#if 1
				// hwflush_dcache_range((ULONG)p->payload, (UINT)p->len);
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
					case 0x8033: // HikVision SADP (discovery protocol) tool, for managing settings on Hikvision IP network cameras
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
						//hwflush_dcache_range((ULONG)p->payload, (UINT)p->len);
						UINT celcrc32 = crcCompute(p->payload, p->len);
						_Rprintf("Error! GetCRC:0x%08X CelCRC:0x%08X\n", _test_crc32, celcrc32);
						hexDump("IP input error", p->payload, p->len);
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
static int network_ethif_phy_restart_flag = 0;

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
init:
	EthphyInit(ETHPHY_MDIO_ADR, network_ethif_ethphy_irq);

	if (EthphyReset() == ENX_FAIL) {
		printf("Ethphy reset Fail, retry in 5 seconds.\n");
		vTaskDelay(500); // wait 5sec
		goto init; // reset
	}

	EthphyGetPHYID();

	EthphySetting();

	EthphyAutoNeg(gtNetwork.u3EthAutoNegotiation);

	while (1) {
		if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
			if (network_ethif_phy_restart_flag == 1) {
				printf("Ethphy restart\n");
				network_ethif_phy_restart_flag = 0;
				goto init; // reset
			}
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

void network_ethif_phy_restart(void)
{
	network_ethif_phy_restart_flag = 1;
	xTaskNotifyGive(netif_state[enlETHERNET].link_notity);
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
		qEthernetTX.base = (BYTE *)(((intptr_t)(pBase) + NETTX_BUF_GAP - 1) & ~(intptr_t)(NETTX_BUF_GAP - 1));
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
	netif_state[enlETHERNET].xrx_notity = vTaskCreate("eifRx", network_ethif_pkt_input, netif_state[enlETHERNET]._netif, LV5_STACK_SIZE, LV6_TASK_PRIO);
}
#endif
