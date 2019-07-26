#ifndef _WIFIIF_H_
#define _WIFIIF_H_

//*************************************************************************************************
// Macro
//-------------------------------------------------------------------------------------------------

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern void wifiPollTask(void *pvParameters);

extern void network_wlifuap_conn_list_setip(BYTE *mac, UINT ipaddr);
extern void network_wlifuap_conn_list_view(void);

extern void network_wlif_sta_start(void);
extern void network_wlif_uap_start(void);
extern err_t igmp_mac_filter_set_mlanif(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action);

//-------------------------------------------------------------------------------------------------
// Variable

#endif // _WIFIIF_H_
