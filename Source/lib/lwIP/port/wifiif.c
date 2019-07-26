//*************************************************************************************************
//
//*************************************************************************************************

#include "dev.h"

#if defined(__WIFI__)

#include "user.h"
#include "msg.h"

#include "enx_freertos.h"
#include "enx_lwip.h"

#include "networkif.h"
#include "wifiif.h"
#include "wifi.h"
#include "wlif_tools.h"
#include "ewl.h"

#include "dhcpd.h"
//#include "timer_task.h"

#define WIFNAME0 'w'
#define WIFNAME1 'l'

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Wi-Fi Mac Filter
err_t igmp_mac_filter_set_mlanif(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
	u8_t multicast_mac[6] = {0x01, 0x00, 0x5E};
	multicast_mac[3] = ip4_addr2(group) & 0x7F;
	multicast_mac[4] = ip4_addr3(group);
	multicast_mac[5] = ip4_addr4(group);

	if (action == NETIF_ADD_MAC_FILTER) {
		printf("IGMP : Add %IP to the Mac filter.\n", group->addr);
		ewl_set_multicast_addr(netif, multicast_mac);
		return ERR_OK;
	} else if (action == NETIF_DEL_MAC_FILTER) {
		printf("IGMP : Not define - mac filter delete function.(%IP)\n", group->addr);
	}

	return ERR_IF;
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Wi-Fi TX
/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and
 *        type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_wlif_output(struct netif *netif, struct pbuf *p)
{  
	int ret = 0;
	if (wltx_lock()) {
		if (p->next) {
			flprintf("len(%d/%d)\n", p->len, p->tot_len);
		}
		//hexDump("Wi-Fi TX", p->payload, p->len);
		ret = ewl_start_tx(p, netif);
		wltx_unlock();
		if (!ret) {
			return ERR_OK; /* no one seems to check this anyway */
		}
	}
	return ERR_IF;
}

////////////////////////////////////////////////////////////////////////////////
// Wi-Fi Poll Task
////////////////////////////////////////////////////////////////////////////////

static SemaphoreHandle_t semaWiFiPoll = NULL;

void wifiPollGiveFromISR(void)
{
	if (semaWiFiPoll != NULL) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(semaWiFiPoll, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;		// Task switch required ?
		}
	} else {
		printf("Wi-Fi netif not ready\n");
	}
}

static struct pbuf *low_level_wifiif_get(struct netif *netif)
{
	struct pbuf *p = NULL;
	if (wlrx_lock()) {
		portENTER_CRITICAL();
		p = ewl_pkt_rx(netif);
		portEXIT_CRITICAL();
		wlrx_unlock();
	}
	return p;
}

#define MSG_NETRX_NUM 150
void wifiPollTask(void *pvParameters)
{
	semaWiFiPoll = xSemaphoreCreateCounting(MSG_NETRX_NUM, 0);
	while (1) {
		if (xSemaphoreTake(semaWiFiPoll, portMAX_DELAY) == pdTRUE) {
			ewl_poll();

			// AP mode packet only
			err_t ert;
			struct eth_hdr *ethhdr;
			struct pbuf *p;
			struct netif *netif = netif_state[enlWIFIUAP]._netif;
			if (netif && netif_is_link_up(netif)) {
				while ((p = low_level_wifiif_get(netif)) != NULL) {
				// points to packet payload, which starts with an Ethernet header
#if ETH_PAD_SIZE
					//hexDump("Input", p->payload, p->len);
					if ((((UINT)p) & 3)) {
						flprintf("Input-P align error?(0x%08X)\n", p);
					}
					if (p->payload == NULL) {
						flprintf("Input-P->payload NULL\n");
					}
					if (p->next) {
						if (p->payload == NULL) {
							flprintf("Input-P->payload NULL\n");
						} else {
							flprintf("Input-P->payload 0x%08X\n", p->payload);
							hexDump("Input", p->payload, p->len);
						}
						flprintf("Input-P next use, len(%d) total(%d)\n", p->len, p->tot_len);
					}
					pbuf_header(p, ETH_PAD_SIZE);
#endif
					ethhdr = (struct eth_hdr *)p->payload;
					switch (htons(ethhdr->type)) {
						// IP or ARP packet ?
						case ETHTYPE_IP:
						case ETHTYPE_ARP:
							// full packet send to tcpip_thread to process
							if ((ert = netif->input(p, netif)) != ERR_OK) {
								_Rprintf("%s: IP/ARP input error(%d)\n", __func__, ert);
								LWIP_DEBUGF(NETIF_DEBUG, ("if_input: IP input error\n"));
								pbuf_free(p);
								p = NULL;
							}
							break;
						case ETHTYPE_IPV6:
							LWIP_DEBUGF(NETIF_DEBUG, ("if_input: IPv6 is not supported.\n"));
							pbuf_free(p);
							p = NULL;
							break;
						default:
							_Rprintf("%s: EtherType(0x%04X) is not supported.\n", __func__, htons(ethhdr->type));
							LWIP_DEBUGF(NETIF_DEBUG, ("if_input: inval type\n"));
							pbuf_free(p);
							p = NULL;
							break;
					}
				} // end of while((p = low_level_input(netif)))
			}
		}
	}
	UNUSED(pvParameters);
}

////////////////////////////////////////////////////////////////////////////////
// Wi-Fi Status
////////////////////////////////////////////////////////////////////////////////
static void network_wlifsta_link_cb(struct netif *_netif)
{
	if (_netif == netif_state[enlWIFISTA]._netif) {
		netif_state_t *_state = &netif_state[enlWIFISTA];
		if (netif_is_link_up(_netif)) {
			if (gtNetwork.STAcfg.un1DHCPSwitch == ENX_ON) {
				printf("Start DHCP negotiation for a network interface.\n");
				dhcp_start(_netif);
			} else {
				ip_addr_t ipaddr, netmask, gw, dns0, dns1;
				ip4_addr_set_u32(&ipaddr, gtNetwork.STAcfg.un32Localip);
				ip4_addr_set_u32(&netmask, gtNetwork.STAcfg.un32Netmask);
				ip4_addr_set_u32(&gw, gtNetwork.STAcfg.un32Gateway);
				ip4_addr_set_u32(&dns0, gtNetwork.STAcfg.un32DNS0);
				ip4_addr_set_u32(&dns1, gtNetwork.STAcfg.un32DNS1);

				netif_set_addr(_netif, &ipaddr, &netmask, &gw);
				dns_setserver(0, &dns0);							// Set DNS server 0 addr
				dns_setserver(1, &dns1);							//		"		  1  "

				netif_set_up(_netif);
			}
			struct dhcp* dhcp = netif_dhcp_data(_netif);
			es_printf("%s link up, bound to %IP%s\n", _state->ifname, _netif->ip_addr.addr, dhcp ? (dhcp->state ? ", DHCP negotiating..." : "") : "");
		} else {
			if (gtNetwork.STAcfg.un1DHCPSwitch == ENX_ON) {
				dhcp_release_and_stop(_netif);
			} else {
				netif_set_addr(_netif, NULL, NULL, NULL);
			}
			dns_setserver(0, NULL);
			dns_setserver(1, NULL);
			printf("%s link down\n", _state->ifname);
		}
	} else {
		printf("%s: arguments(_netif) != %s\n", __func__, netif_state[enlWIFISTA].ifname);
	}

	network_default_netif();
}

static void network_wlifsta_status_cb(struct netif *_netif)
{
	if (_netif == netif_state[enlWIFISTA]._netif) {
		netif_state_t *_state = &netif_state[enlWIFISTA];
		if (netif_is_up(_netif)) {
			printf("%s status up(link %s)", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
			if (netif_is_link_up(_netif)) {
				es_printf("(%s, %IP)\n", dhcp_supplied_address(_netif) ? "DHCP" : "static", _netif->ip_addr.addr);
				if (gtNetwork.STAcfg.un1DHCPSwitch == ENX_ON) {
					gtNetwork.STAcfg.un32Localip = ip4_addr_get_u32(&_netif->ip_addr);
					gtNetwork.STAcfg.un32Netmask = ip4_addr_get_u32(&_netif->netmask);
					gtNetwork.STAcfg.un32Gateway = ip4_addr_get_u32(&_netif->gw);
					gtNetwork.STAcfg.un32DNS0 = ip4_addr_get_u32(dns_getserver(0));
					gtNetwork.STAcfg.un32DNS1 = ip4_addr_get_u32(dns_getserver(1));
				} else {
					printf("Inform a DHCP server of our manual configuration.\n");
					dhcp_inform(_netif);
				}
			} else {
				printf("\n");
			}
		} else {
			printf("%s status down(link %s)\n", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
		}
	} else {
		printf("%s: arguments(_netif) != %s\n", __func__, netif_state[enlWIFISTA].ifname);
	}
}

static void network_wlifsta_disconn_cb(BYTE *buf, int buf_len)
{
	flprintf("\n");
	UNUSED(buf);
	UNUSED(buf_len);
}

static void network_wlifsta_conn_cb(BYTE *buf, int buf_len)
{
	flprintf("\n");
	UNUSED(buf);
	UNUSED(buf_len);
}

static void network_wlifuap_link_cb(struct netif *_netif)
{
	if (_netif == netif_state[enlWIFIUAP]._netif) {
		netif_state_t *_state = &netif_state[enlWIFIUAP];
		if (netif_is_link_up(_netif)) {
			ip_addr_t ipaddr, netmask, gw;
			ip4_addr_set_u32(&ipaddr, gtNetwork.UAPcfg.un32Localip);
			ip4_addr_set_u32(&netmask, gtNetwork.UAPcfg.un32Netmask);
			ip4_addr_set_u32(&gw, gtNetwork.UAPcfg.un32Localip);
			netif_set_addr(_netif, &ipaddr, &netmask, &gw);
			if (gtNetwork.UAPcfg.un8DHCPServer == ENX_ON) {
				dhcpd_start(_netif);
			}
			es_printf("%s link up, bound to %IP\n", _state->ifname, _netif->ip_addr.addr);
		} else {
			if (gtNetwork.UAPcfg.un8DHCPServer == ENX_ON) {
				dhcpd_stop(_netif);
			}
			netif_set_addr(_netif, NULL, NULL, NULL);
			printf("%s link down\n", _state->ifname);
		}
	} else {
		printf("%s: arguments(_netif) != %s\n", __func__, netif_state[enlWIFIUAP].ifname);
	}

	network_default_netif();
}

static void network_wlifuap_status_cb(struct netif *_netif)
{
	if (_netif == netif_state[enlWIFIUAP]._netif) {
		netif_state_t *_state = &netif_state[enlWIFIUAP];
		if (netif_is_up(_netif)) {
			printf("%s status up(link %s)", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
			if (netif_is_link_up(_netif)) {
				es_printf("(%IP)\n", _netif->ip_addr.addr);
			} else {
				printf("\n");
			}
		} else {
			printf("%s status down(link %s)\n", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
		}
	} else {
		printf("%s: arguments(_netif) != %s\n", __func__, netif_state[enlWIFIUAP].ifname);
	}
}

/*
    When AP is dis_connected, dhcp client is started.
    And lwip can be down.
*/
typedef struct {
	BYTE mac[6];
	UINT conntime;
	UINT ipaddr;
} WiFiAPConnectSTAitem;

typedef struct {
	UINT maccount;
	WiFiAPConnectSTAitem item[WIFI_AP_MAXUSER];
} WiFiAPConnectSTAlist;

WiFiAPConnectSTAlist wifi_ap_conn_list;

static void network_wlifuap_conn_list_add(BYTE *mac)
{
	UINT i;
	for (i = 0; i < gtNetwork.UAPcfg.un8MaxSTA; i++) {
		WiFiAPConnectSTAitem *mitem = &wifi_ap_conn_list.item[i];
		if ((mitem->mac[0] == 0x00) && (mitem->mac[1] == 0x00) && (mitem->mac[2] == 0x00) && (mitem->mac[3] == 0x00) && (mitem->mac[4] == 0x00) && (mitem->mac[5] == 0x00)) {
			mitem->mac[0] = mac[0];
			mitem->mac[1] = mac[1];
			mitem->mac[2] = mac[2];
			mitem->mac[3] = mac[3];
			mitem->mac[4] = mac[4];
			mitem->mac[5] = mac[5];
			mitem->ipaddr = 0;
			mitem->conntime = gptMsgShare.TIME;
			wifi_ap_conn_list.maccount++;
			break;
		}
	}
}

static UINT network_wlifuap_conn_list_del(BYTE *mac)
{
	UINT i;
	for (i = 0; i < gtNetwork.UAPcfg.un8MaxSTA; i++) {
		WiFiAPConnectSTAitem *mitem = &wifi_ap_conn_list.item[i];
		if ((mitem->mac[0] == mac[0]) && (mitem->mac[1] == mac[1]) && (mitem->mac[2] == mac[2]) && (mitem->mac[3] == mac[3]) && (mitem->mac[4] == mac[4]) && (mitem->mac[5] == mac[5])) {
			mitem->mac[0] = 0x00;
			mitem->mac[1] = 0x00;
			mitem->mac[2] = 0x00;
			mitem->mac[3] = 0x00;
			mitem->mac[4] = 0x00;
			mitem->mac[5] = 0x00;
			mitem->conntime = 0;
			mitem->ipaddr = 0;
			wifi_ap_conn_list.maccount--;
			return ENX_OK;
		}
	}
	return ENX_FAIL;
}

void network_wlifuap_conn_list_setip(BYTE *mac, UINT ipaddr)
{
	UINT i;
	for (i = 0; i < gtNetwork.UAPcfg.un8MaxSTA; i++) {
		WiFiAPConnectSTAitem *mitem = &wifi_ap_conn_list.item[i];
		if ((mitem->mac[0] == mac[0]) && (mitem->mac[1] == mac[1]) && (mitem->mac[2] == mac[2]) && (mitem->mac[3] == mac[3]) && (mitem->mac[4] == mac[4]) && (mitem->mac[5] == mac[5])) {
			mitem->ipaddr = ipaddr;
			break;
		}
	}
}

void network_wlifuap_conn_list_view(void)
{
	UINT i;
	struct tm tmout;
	printf("Max Connect: %u\n", gtNetwork.UAPcfg.un8MaxSTA);
	printf("Connect STA: %u\n", wifi_ap_conn_list.maccount);
	for (i = 0; i < gtNetwork.UAPcfg.un8MaxSTA; i++) {
		WiFiAPConnectSTAitem *mitem = &wifi_ap_conn_list.item[i];
		if (!((mitem->mac[0] == 0x00) && (mitem->mac[1] == 0x00) && (mitem->mac[2] == 0x00) && (mitem->mac[3] == 0x00) && (mitem->mac[4] == 0x00) && (mitem->mac[5] == 0x00))) {
			enx_get_tmtime(mitem->conntime, &tmout, ENX_YES);
			printf("Mac%u Remote(%02X:%02X:%02X:%02X:%02X:%02X/%IP)", i, mitem->mac[0], mitem->mac[1], mitem->mac[2], mitem->mac[3], mitem->mac[4], mitem->mac[5], mitem->ipaddr);
			printf(" Connect Time(%04d-%02d-%02d %02d:%02d:%02d)\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		}
	}
}

static void network_wlifuap_disconn_cb(BYTE *buf, int buf_len)
{
	if (buf) { // event : MLAN_EVENT_ID_UAP_FW_STA_DISCONNECT
		BYTE *mac = buf + 2;
		if (network_wlifuap_conn_list_del(buf + 2) == ENX_OK) {
			printf("%s device disconnect.\n", netif_state[enlWIFIUAP].ifname);
		} else {
			printf("%s device already disconnected(%02X:%02X:%02X:%02X:%02X:%02X).\n", netif_state[enlWIFIUAP].ifname, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	} else { // event : MLAN_EVENT_ID_UAP_FW_BSS_IDLE
		printf("%s BSS idle.\n", netif_state[enlWIFIUAP].ifname);
		netifapi_netif_set_link_down(netif_state[enlWIFIUAP]._netif);
		netifapi_netif_set_down(netif_state[enlWIFIUAP]._netif);
	}
	UNUSED(buf_len);
}

void network_wlifuap_conn_cb(BYTE *buf, int buf_len)
{
	if (buf) { // event : MLAN_EVENT_ID_UAP_FW_STA_CONNECT
		network_wlifuap_conn_list_add(buf);
		printf("%s device connect.\n", netif_state[enlWIFIUAP].ifname);
	} else { // event : MLAN_EVENT_ID_UAP_FW_BSS_ACTIVE
		printf("%s BSS active.\n", netif_state[enlWIFIUAP].ifname);
		netifapi_netif_set_up(netif_state[enlWIFIUAP]._netif);
		netifapi_netif_set_link_up(netif_state[enlWIFIUAP]._netif);
	}
	UNUSED(buf_len);
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t network_wlif_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));

	netif->output = etharp_output;
	netif->linkoutput = low_level_wlif_output;
#if LWIP_IGMP
	netif->igmp_mac_filter = igmp_mac_filter_set_mlanif;
#endif

	if (netif == netif_state[enlWIFISTA]._netif) {
		netif_set_link_callback(netif, network_wlifsta_link_cb);
		netif_set_status_callback(netif, network_wlifsta_status_cb);

		ewl_set_conn_cb(netif->state, network_wlifsta_conn_cb, network_wlifsta_disconn_cb);
	} else if (netif == netif_state[enlWIFIUAP]._netif) {
		netif_set_link_callback(netif, network_wlifuap_link_cb);
		netif_set_status_callback(netif, network_wlifuap_status_cb);

		ewl_set_conn_cb(netif->state, network_wlifuap_conn_cb, network_wlifuap_disconn_cb);
		ewlap_bss_start();
		ewl_uap_get_freq();
	}

	return low_level_init(netif, WIFNAME0, WIFNAME1);
}

void network_wlif_sta_start(void)
{
	static int funcfirst = 0;
	if (funcfirst == 0) {
		funcfirst++;
		netifapi_netif_add(netif_state[enlWIFISTA]._netif, NULL, NULL, NULL, netif_state[enlWIFISTA]._netif->state, network_wlif_init, tcpip_input);
	}
}

void network_wlif_uap_start(void)
{
	static int funcfirst = 0;
	if (funcfirst == 0) {
		funcfirst++;
		netifapi_netif_add(netif_state[enlWIFIUAP]._netif, NULL, NULL, NULL, netif_state[enlWIFIUAP]._netif->state, network_wlif_init, tcpip_input);
	}
}
#endif
