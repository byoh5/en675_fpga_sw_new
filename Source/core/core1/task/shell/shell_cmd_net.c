//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"
#include "ethphy.h"

#ifdef __NETWORK__

#include <string.h> // strcmp
#include <stdlib.h> // atoi

#include "enx_freertos.h"
#include "enx_lwip.h"

#include "shell_cmd_net.h"

const char *sIfconfigCmd[]  = {"Network Setting View",          (char*)0};
const char *sNetstatCmd[]   = {"show TCP/IP network state",     (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
int cmd_ifconfig(int argc, char *argv[])
{
#if 0 // defined(__ETHERNET__)
	if (argc >= 2) {
		const char *strHelp = "%s [-?] [-ip] [-sn] [-gw]\n"\
			"-ip 000.000.000.000 : Set IPAddress\n"\
			"-sn 000.000.000.000 : Set SubnetMask\n"\
			"-gw 000.000.000.000 : Set Gateway\n"\
			"ex) OK : %s -ip x.x.x.x\n"\
			"    OK : %s -ip x.x.x.x -sn x.x.x.x\n"\
			"    OK : %s -ip x.x.x.x -sn x.x.x.x -gw x.x.x.x\n";
		if (strcmp(argv[1], "-?") == 0) {
			printf(strHelp, argv[0], argv[0], argv[0], argv[0]);
		} else if(argc >= 3 && (argc & 1)) {	// 3개이상이고 명령이 홀수인 경우에만 ok
			UINT i, var = 0, unIpFlag = 0;
			for (i = 1; i < (UINT)argc; i += 2) {
				var = ipaddr_addr(argv[i+1]);
				if (strcmp(argv[i], "-ip") == 0) {
					unIpFlag |= UIN_NW_INT_IP;
					gtNetwork.nIpAddr = var;
					es_printf("Set IPAddress: %IP\n", gtNetwork.nIpAddr);
				} else if(strcmp(argv[i], "-sn") == 0) {
					unIpFlag |= UIN_NW_INT_SN;
					gtNetwork.nNetMask = var;
					es_printf("Set SubnetMask: %IP\n", gtNetwork.nNetMask);
				} else if(strcmp(argv[i], "-gw") == 0) {
					unIpFlag |= UIN_NW_INT_GW;
					gtNetwork.nGateway = var;
					es_printf("Set Gateway: %IP\n", gtNetwork.nGateway);
				}
			}
			if (unIpFlag != 0) {
				enx_ipchange_proc(enlETHERNET, unIpFlag);
			}
		} else {
			printf(strHelp, argv[0], argv[0], argv[0], argv[0]);
		}
	} else
#endif
	{
		struct netif *_netif;
#if LWIP_DNS
		es_printf("DNS	addr0:%IP addr1:%IP\n", ip4_addr_get_u32(dns_getserver(0)), ip4_addr_get_u32(dns_getserver(1)));
#endif
		NETIF_FOREACH(_netif) {
			printf("%c%c%d	HWaddr %02X:%02X:%02X:%02X:%02X:%02X\n", _netif->name[0], _netif->name[1], _netif->num,
				_netif->hwaddr[0], _netif->hwaddr[1], _netif->hwaddr[2], _netif->hwaddr[3], _netif->hwaddr[4], _netif->hwaddr[5]);
			es_printf("	inet addr:%IP Mask:%IP GW:%IP\n", ip4_addr_get_u32(&_netif->ip_addr), ip4_addr_get_u32(&_netif->netmask), ip4_addr_get_u32(&_netif->gw));
			printf("	%s%s%s%s%s%s%sMTU:%d %s%s\n",
				_netif->flags & NETIF_FLAG_UP ? "UP " : "",
				_netif->flags & NETIF_FLAG_BROADCAST ? "BROADCAST " : "",
				_netif->flags & NETIF_FLAG_LINK_UP ? "LINK-UP " : "",
				_netif->flags & NETIF_FLAG_ETHARP ? "ETHARP " : "",
				_netif->flags & NETIF_FLAG_ETHERNET ? "ETHERNET " : "",
				_netif->flags & NETIF_FLAG_IGMP ? "IGMP " : "",
				_netif->flags & NETIF_FLAG_MLD6 ? "MLD6 " : "",
				_netif->mtu,
#if LWIP_DHCP
				dhcp_supplied_address(_netif) ? "DHCP " : "",
#else
				"",
#endif
				_netif == netif_default ? "DEFIF" : "");
		}
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#include "lwip/priv/tcp_priv.h"

int cmd_netstat(int argc, char *argv[])
{
	struct tcp_pcb ** const sh_tcp_pcb_lists[] = {&tcp_listen_pcbs.pcbs, &tcp_bound_pcbs,
													&tcp_active_pcbs, &tcp_tw_pcbs};
	int i;
	char strBuffer[2][24];
	struct tcp_pcb *tpcb;
	struct udp_pcb *upcb;
	printf("------------------------------netstat------------------------------\n");
	printf("Protocol  Local                  Remote                 State\n");
	printf("-------------------------------------------------------------------\n");
	for (i = 0; i < 4; i++) {
		for (tpcb = *sh_tcp_pcb_lists[i]; tpcb != NULL; tpcb = tpcb->next) {
			es_snprintf(strBuffer[0], 24, "%IP:%d", tpcb->local_ip.addr, tpcb->local_port);
			if (i == 0) { // LISTEN
				es_snprintf(strBuffer[1], 24, "%IP:*", tpcb->remote_ip.addr);
			} else { // BOUND, ACTIVE, TW
				es_snprintf(strBuffer[1], 24, "%IP:%d", tpcb->remote_ip.addr, tpcb->remote_port);
			}
			printf("TCP       %-21s  %-21s  %s\n", strBuffer[0], strBuffer[1], tcp_debug_state_str(tpcb->state));
		}
	}
	for (upcb = udp_pcbs; upcb != NULL; upcb = upcb->next) {
		es_snprintf(strBuffer[0], 24, "%IP:%d", upcb->local_ip.addr, upcb->local_port);
		es_snprintf(strBuffer[1], 24, "%IP:%d", upcb->remote_ip.addr, upcb->remote_port);
		printf("UDP       %-21s  %-21s\n", strBuffer[0], strBuffer[1]);
	}
	printf("-------------------------------------------------------------------\n");

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}
#endif
