#include "dev.h"
#include "enx_freertos.h"
#include "enx_lwip.h"
#include "networkif.h"

#if defined(__ETHERNET__)
static struct netif gnif_eth; // Ethernet
#endif
#if defined(__WIFI__)
#include "wifi_task.h"
//struct netif *gtwlif_sta = NULL;// Wi-Fi STA mode
//struct netif *gtwlif_uap = NULL;// Wi-Fi UAP mode
#endif

SemaphoreHandle_t xtxwl_sem = NULL;
SemaphoreHandle_t xrxwl_sem = NULL;

netif_state_t netif_state[enlEnd] = {
#if defined(__ETHERNET__)
{	.ifname = "Ethernet",
	._netif = &gnif_eth,
	.xrx_notity = NULL,
	.iftype = enlETHERNET,	},
#endif
#if defined(__WIFI__)
{	.ifname = "Wi-Fi(STA)",
	._netif = NULL,
	.xrx_notity = NULL,
	.iftype = enlWIFISTA,	},
{	.ifname = "Wi-Fi(UAP)",
	._netif = NULL,
	.xrx_notity = NULL,
	.iftype = enlWIFIUAP,	},
#endif
};

err_t low_level_init(struct netif *netif, char name0, char name1)
{
	static int oneshot = 0;

	netif->name[0] = name0;
	netif->name[1] = name1;
	netif->hwaddr_len = NETIF_MAX_HWADDR_LEN;	// Set MAC hardware address length
	netif->mtu = 1500;							// Maximum transfer unit
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP;	// Don't set NETIF_FLAG_ETHARP if this device is not an ethernet one
#if defined(__ETHERNET__)
	if (netif == &gnif_eth) {
		netif->flags |= NETIF_FLAG_UP;
	}
#endif

	if (!oneshot) {
		wltx_init_lock();
		wlrx_init_lock();
		oneshot = 1;
	}

	return ERR_OK;
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Check Network Interface
void network_check_netif(void)
{
	do {
		struct netif *nif;
		NETIF_FOREACH(nif) {
			if (nif && netif_is_up(nif) && netif_is_link_up(nif)) {
				return;
			}
		}
		vTaskDelay(10);
	} while(1);
}

void network_default_netif(void)
{
	const char *strDefnetif = "%s is set as default interface.\n";
#if defined(__ETHERNET__)
	if (netif_is_up(&gnif_eth) && netif_is_link_up(&gnif_eth)) {
		if (netif_default != &gnif_eth) {
			netif_set_default(&gnif_eth);
			printf(strDefnetif, netif_state[enlETHERNET].ifname);
			return;
		}
	}
#endif
#ifdef __WIFI__
	if (netif_state[enlWIFISTA]._netif && netif_is_up(netif_state[enlWIFISTA]._netif) && netif_is_link_up(netif_state[enlWIFISTA]._netif)) {
		if (netif_default != netif_state[enlWIFISTA]._netif) {
			netif_set_default(netif_state[enlWIFISTA]._netif);
			printf(strDefnetif, netif_state[enlWIFISTA].ifname);
			return;
		}
	}

	if (netif_state[enlWIFIUAP]._netif && netif_is_up(netif_state[enlWIFIUAP]._netif) && netif_is_link_up(netif_state[enlWIFIUAP]._netif)) {
		if (netif_default != netif_state[enlWIFIUAP]._netif) {
			netif_set_default(netif_state[enlWIFIUAP]._netif);
			printf(strDefnetif, netif_state[enlWIFIUAP].ifname);
			return;
		}
	}
#endif

	printf("%s: Set NULL netif!\n", __func__);
	netif_set_default(NULL);
}

void network_default_netif_get_ip(UINT *pIP)
{
	if (netif_default) {
		*pIP = netif_default->ip_addr.addr;
	} else {
		*pIP = 0;
	}
}

void network_interface_link(NETIFLIST iftype, ENX_SWITCH u32Switch)
{
	switch (u32Switch) {
	case ENX_ON:
		netifapi_netif_set_link_up(netif_state[iftype]._netif);
		break;
	case ENX_OFF:
		netifapi_netif_set_link_down(netif_state[iftype]._netif);
		break;
	}
}

void network_interface_list(void)
{
	for (UINT i = 0; i < enlEnd; i++) {
		printf("ifname: %s\n", netif_state[i].ifname);
		printf("_netif: 0x%08lX\n", (ULONG)netif_state[i]._netif);
	}
}

static void tcpip_init_done_func(void *ctx)
{
#if defined(__ETHERNET__)
	network_ethif_start();
#endif

	UNUSED(ctx);
}

void startLwip(void *arg)
{
	tcpip_init(tcpip_init_done_func, NULL);

	vTaskDelete(NULL);
	UNUSED(arg);
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Option
ENX_OKFAIL network_igmp_join(char *strIP)
{
	ip_addr_t groupaddr;
    err_t err;

    groupaddr.addr = ipaddr_addr(strIP);
	printf("%s => 0x%08X\n", strIP, groupaddr.addr);
	err = igmp_joingroup(IP_ADDR_ANY, (ip_addr_t *)&groupaddr);
    if (err != ERR_OK) {
        printf("%s(%d) : err(%d)\n", __func__, __LINE__, err);
        return ENX_FAIL;
    }
	return ENX_OK;
}

ENX_OKFAIL network_igmp_leave(char *strIP)
{
	ip_addr_t groupaddr;
    err_t err;

    groupaddr.addr = ipaddr_addr(strIP);
	printf("%s => 0x%08X\n", strIP, groupaddr.addr);
	err = igmp_leavegroup(IP_ADDR_ANY, (ip_addr_t *)&groupaddr);
    if (err != ERR_OK) {
        printf("%s(%d) : err(%d)\n", __func__, __LINE__, err);
        return ENX_FAIL;
    }
    return ENX_OK;
}
