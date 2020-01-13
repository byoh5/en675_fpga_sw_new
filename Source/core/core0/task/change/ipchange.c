//*************************************************************************************************
// IpChange task
//*************************************************************************************************
#include "dev.h"

#if defined(__NETWORK__)

#include "enx_freertos.h"

#include "lwip/netifapi.h"
#include "lwip/dns.h"
#include "networkif.h"

#include "ipchange.h"

static UINT unIPAddressChangeFlag[enlEnd] = {0};

#define ENX_IPC_DGB_COMMAND		ENX_OFF
#define ENX_IPC_DGB_PROCESSING	ENX_OFF
#define ENX_IPC_DGB_RESPONSE	ENX_OFF
#define ENX_IPC_DGB_DEBUG		ENX_OFF
#define ENX_IPC_DGB_ERROR		ENX_ON

#define tprintf(debug, fmt, args...) do { if((debug)&ENX_ON) printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);

//*************************************************************************************************
// IpChange
//-------------------------------------------------------------------------------------------------
//
extern struct netif *netif_list;
void ChangeDefDeviceId(void)
{
	// strDeviceId의 앞내용이 DEVICE_ID와 같다면 변경한 적이 없음.
	int i, k = 0, id_len = strlen(DEVICE_ID);
	for (i = 0; i < id_len; i++) {
		if (DEVICE_ID[i] != gtUser.strDeviceId[i]) {
			k = 1;
			break;
		}
	}
	// k가 0이면 변경한 적이 없는 것이므로 UPNP를 위해 DeviceId를 변경한다.
	if (k == 0) {
		printf("ChangDefDeviceId : %s => ", gtUser.strDeviceId);
		struct netif *netif = netif_list;
		if (netif != NULL) {
			sprintf((char *)gtUser.strDeviceId, "%s%02X%02X%02X", (char *)DEVICE_ID, netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);
		} else {
			sprintf((char *)gtUser.strDeviceId, "%sFFFFFF", (char *)DEVICE_ID);
		}
		printf("%s\n", gtUser.strDeviceId);
	}
}

#if defined(__ETHERNET__)
static void enx_ipchange_ethernet(struct netif *_netif, UINT changeflag)
{
	printf("Call %s: 0x%08X\n", __func__, changeflag);

	if (changeflag != 0) {
		if (gtNetwork.naEthernet.u1UseDhcp == ENX_ON) {
			printf("Start DHCP negotiation for a network interface.\n");
			if (bitcheck(changeflag, UIN_NW_INT_DHCP)) {
				netifapi_dhcp_start(_netif);
			} else {
				tprintf(ENX_IPC_DGB_ERROR, "Error! DHCP is in use.\n");
			}
		} else {
			printf("Inform a DHCP server of our manual configuration.\n");
			network_check_netif();
			vTaskDelay(IpChangeDelay);	// 웹브라우저 변경 중....

			if (bitcheck(changeflag, UIN_NW_INT_DNS0)) {
				ip_addr_t dns;
				ip4_addr_set_u32(&dns, gtNetwork.naEthernet.u32DnsSvr0);
				dns_setserver(0, &dns);
			}

			if (bitcheck(changeflag, UIN_NW_INT_DNS1)) {
				ip_addr_t dns;
				ip4_addr_set_u32(&dns, gtNetwork.naEthernet.u32DnsSvr1);
				dns_setserver(1, &dns);
			}

			if (bitcheck(changeflag, UIN_NW_INT_IP) || bitcheck(changeflag, UIN_NW_INT_SN) || bitcheck(changeflag, UIN_NW_INT_GW)) {
				ip_addr_t ipaddr, netmask, gw;
				ip4_addr_set_u32(&ipaddr, gtNetwork.naEthernet.u32IpAddr);
				ip4_addr_set_u32(&netmask, gtNetwork.naEthernet.u32NetMask);
				ip4_addr_set_u32(&gw, gtNetwork.naEthernet.u32Gateway);
				netifapi_netif_set_addr(_netif, &ipaddr, &netmask, &gw);
			}

			netifapi_dhcp_inform(_netif);
		}
	}
}
#endif

#if defined(__WIFI__)
static void enx_ipchange_wifista(struct netif *_netif, UINT changeflag)
{
	if (changeflag != 0) {
		if (gtNetwork.STAcfg.un1DHCPSwitch == ENX_ON) {
			printf("Start DHCP negotiation for a network interface.\n");
			if (bitcheck(changeflag, UIN_NW_INT_DHCP)) {
				netifapi_dhcp_start(_netif);
			} else {
				tprintf(ENX_IPC_DGB_ERROR, "Error! DHCP is in use.\n");
			}
		} else {
			printf("Inform a DHCP server of our manual configuration.\n");
			network_check_netif();
			vTaskDelay(IpChangeDelay);	// 웹브라우저 변경 중....

			if (bitcheck(changeflag, UIN_NW_INT_DNS0)) {
				ip_addr_t dns;
				ip4_addr_set_u32(&dns, gtNetwork.STAcfg.un32DNS0);
				dns_setserver(0, &dns);
			}

			if (bitcheck(changeflag, UIN_NW_INT_DNS1)) {
				ip_addr_t dns;
				ip4_addr_set_u32(&dns, gtNetwork.STAcfg.un32DNS1);
				dns_setserver(1, &dns);
			}

			if (bitcheck(changeflag, UIN_NW_INT_IP) || bitcheck(changeflag, UIN_NW_INT_SN) || bitcheck(changeflag, UIN_NW_INT_GW)) {
				ip_addr_t ipaddr, netmask, gw;
				ip4_addr_set_u32(&ipaddr, gtNetwork.STAcfg.un32Localip);
				ip4_addr_set_u32(&netmask, gtNetwork.STAcfg.un32Netmask);
				ip4_addr_set_u32(&gw, gtNetwork.STAcfg.un32Gateway);
				netifapi_netif_set_addr(_netif, &ipaddr, &netmask, &gw);
			}

			netifapi_dhcp_inform(_netif);
		}
	}
}
#endif

static void enx_ipchange_task(void *ctx)
{
	NETIFLIST iftype = (NETIFLIST)ctx;
	if (unIPAddressChangeFlag[iftype] != 0) {
		switch (iftype) {
#if defined(__ETHERNET__)
			case enlETHERNET: // OK
				enx_ipchange_ethernet(netif_state[iftype]._netif, unIPAddressChangeFlag[iftype]);
				break;
#endif
#if defined(__WIFI__)
			case enlWIFISTA: // OK
				enx_ipchange_wifista(netif_state[iftype]._netif, unIPAddressChangeFlag[iftype]);
				break;
			case enlWIFIUAP: // ...Test
#endif
			default:
				printf("Wrong type(%d) entered.(key:0x%08X)\n", iftype, unIPAddressChangeFlag[iftype]);
				break;
		}

		unIPAddressChangeFlag[iftype] = 0;
	}
	vTaskDelete(NULL);
}

void enx_ipchange_proc(NETIFLIST iftype, UINT _key)
{
	unIPAddressChangeFlag[iftype] = _key;
	switch (iftype) {
#if defined(__ETHERNET__)
		case enlETHERNET:
#endif
#if defined(__WIFI__)
		case enlWIFISTA:
		case enlWIFIUAP:
#endif
			sys_thread_new("ipc", enx_ipchange_task, (void *)iftype, LV2_STACK_SIZE, LV5_TASK_PRIO);
			break;
		default:
			printf("Wrong type(%d) entered.(key:0x%08X)\n", iftype, _key);
			break;
	}
}
#endif
