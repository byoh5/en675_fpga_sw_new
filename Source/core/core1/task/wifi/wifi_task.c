#include "dev.h"

#if defined(__WIFI__)

#include "enx_lwip.h"
#include "enx_freertos.h"

#include "msg.h"
#include "user.h"

#include "ethernetif.h"

#include "wlif_tools.h"
#include "wifi_cfg.h"
#include "wifi.h"

#include "networkif.h"
#ifndef __ETH__
//#include "ipchange.h"
#endif

#include "wifiif.h"
#include "ewl.h"

TaskHandle_t xwf_notity;

////////////////////////////////////////////////////////////////////////////////
// Wi-Fi Control Task
////////////////////////////////////////////////////////////////////////////////

void network_wlif_uap_set_default(void)
{
	char strValue[128];

#if WIFI_AP_SSID_MAC
#define DEF_SSID_LEN (sizeof(WIFI_AP_SSID)-1)	// define string length => sizeof, (-1) => delete NULL value
	struct netif * _netif = netif_state[enlWIFIUAP]._netif;
	ewl_get_hwaddr(_netif, _netif->hwaddr);
	gtNetwork.UAPcfg.strSSID[DEF_SSID_LEN+0] = hex2ascii((_netif->hwaddr[3]>>4)&0x0f);
	gtNetwork.UAPcfg.strSSID[DEF_SSID_LEN+1] = hex2ascii((_netif->hwaddr[3]   )&0x0f);
	gtNetwork.UAPcfg.strSSID[DEF_SSID_LEN+2] = hex2ascii((_netif->hwaddr[4]>>4)&0x0f);
	gtNetwork.UAPcfg.strSSID[DEF_SSID_LEN+3] = hex2ascii((_netif->hwaddr[4]   )&0x0f);
	gtNetwork.UAPcfg.strSSID[DEF_SSID_LEN+4] = hex2ascii((_netif->hwaddr[5]>>4)&0x0f);
	gtNetwork.UAPcfg.strSSID[DEF_SSID_LEN+5] = hex2ascii((_netif->hwaddr[5]   )&0x0f);
	gtNetwork.UAPcfg.strSSID[DEF_SSID_LEN+6] = '\0';
#endif

	ewlap_init_apcfg((char *)gtNetwork.UAPcfg.strSSID);

	sprintf(strValue, "%d", gtNetwork.UAPcfg.un8Channel);
	ewlap_apcfg(WIFI_UAP_SET_CMD_CHANNEL, strValue);

	sprintf(strValue, "%d", gtNetwork.UAPcfg.un8MaxSTA);
	ewlap_apcfg(WIFI_UAP_SET_CMD_MAX_SCB, strValue);

	int o_res = ewlap_set_apcfg();
	if (o_res) {
		printf("%s(%d) : Fail AP %s(%d)\n", __func__, __LINE__, WIFI_UAP_SET_CMD_SET, o_res);
	}

	ewlap_apcfg(WIFI_UAP_SET_CMD_KEY, (char *)gtNetwork.UAPcfg.strKey);

	switch(gtNetwork.UAPcfg.un32Authentication) {
		case wifi_OPEN:
			ewlap_apcfg(WIFI_UAP_SET_CMD_SEC, "open");
			break;
		case wifi_WPAnWPA2_PSK:
		case wifi_WPA_PSK:
			gtNetwork.UAPcfg.un32Authentication = wifi_WPA2_PSK;
		case wifi_WPA2_PSK:
			ewlap_apcfg(WIFI_UAP_SET_CMD_SEC, "wpa2-psk");
			break;
		default:
			printf("%s(%d) %s error(ver:%d)\n", __func__, __LINE__, WIFI_UAP_SET_CMD_SEC, gtNetwork.UAPcfg.un32Authentication);
			break;
	}

	o_res = ewlap_set_apcfg();
	if (o_res) {
		printf("%s(%d) : Fail AP %s(%d)\n", __func__, __LINE__, WIFI_UAP_SET_CMD_SET, o_res);
	}
}

#if 0
void wifi_ap_start(void)
{
	if(gtwlif_uap) {
		network_wlif_uap_start();
	}
}

void WiFiSTA_start(void)
{
	char cmd[80];

	if(gtNetwork.STAcfg.un8DHCPSwitch == WIFI_OFF)
	{
		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_IP, gtNetwork.STAcfg.iptLocalip.addr);
		cpu2cop_wlif_execmd(cmd);

		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_NETMASK, gtNetwork.STAcfg.iptNetmask.addr);
		cpu2cop_wlif_execmd(cmd);

		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_GATEWAY, gtNetwork.STAcfg.iptGateway.addr);
		cpu2cop_wlif_execmd(cmd);
	}

	cpu2cop_wlif_execmd("stastart"); /*this call should be the last. */
}

void sta_netif_status_cb(struct netif* netif)
{
	char cmd[80];

	if (netif_is_up(netif))
	{
		gtNetwork.STAcfg.iptLocalip.addr = netif->ip_addr.addr;
		gtNetwork.STAcfg.iptNetmask.addr = netif->netmask.addr;
		gtNetwork.STAcfg.iptGateway.addr = netif->gw.addr;
		gtNetwork.STAcfg.iptDNS0 = dns_getserver(0);
		gtNetwork.STAcfg.iptDNS1 = dns_getserver(1);

		printf("bound to %IP\n", netif->ip_addr.addr);
		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_IP, netif->ip_addr.addr);
		cpu2cop_wlif_execmd(cmd);

		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_NETMASK, netif->netmask.addr);
		cpu2cop_wlif_execmd(cmd);

		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_GATEWAY, netif->gw.addr);
		cpu2cop_wlif_execmd(cmd);

		sprintf(cmd, "staset %s", WIFI_STA_SET_CMD_NETUP);
		cpu2cop_wlif_execmd(cmd);

#ifdef __ETH__
		// ETH irq ON
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_ETH_IRQ, ENX_ON) == ENX_FAIL)	vTaskDelay(1);	// Time update request to Cop
#endif
    }
    else
	{
    	printf("sta down");
    }
}

void get_cpu0status_cb(void *ctx)
{
	cpu2cop_wlif_execmd("wifistatus");
	UNUSED(ctx);
}
#endif

//gtNetwork.STAcfg.un32State
const char *WLIF_STATE_MSG[17] = {
	"WLIF UAP START",			// 0
	"WLIF_STA_START",			// 1
	"WLIF_UAP_ACTIVE",			// 2
    "WLIF_STA_ACTIVE",			// 3 
    "WLIF_STA_CONNECT_WAIT",	// 4
	"WLIF_CONNECTED",			// 5
	"WLIF_DIS_CONNECTED",		// 6
	"WLIF_SCAN",				// 7
	"WLIF_UAP_BSS_ON",			// 8
	"WLIF_UAP_BSS_OFF",			// 9
	"WLIF_UAP_BSS_ERROR",		// 10
	"WLIF_UAP_DISCONN",			// 11
	"WLIF_INIT_FAIL",			// 12
	"WLIF_STA_NO_AP",			// 13
	"WLIF_STA_WPA_AUTH_FAIL",	// 14
	"WLIF_NONE",				// 15
	"WLIF_ERRMSG"				// 16
};

const char *getWlifStateMsg(UINT state)
{
	switch(state)
	{
		case WLIF_UAP_START:			return WLIF_STATE_MSG[0];
		case WLIF_STA_START:			return WLIF_STATE_MSG[1];
		case WLIF_UAP_ACTIVE:			return WLIF_STATE_MSG[2];
		case WLIF_STA_ACTIVE:			return WLIF_STATE_MSG[3];
		case WLIF_STA_CONNECT_WAIT:		return WLIF_STATE_MSG[4];
		case WLIF_CONNECTED:			return WLIF_STATE_MSG[5];
		case WLIF_DIS_CONNECTED:		return WLIF_STATE_MSG[6];
		case WLIF_SCAN:					return WLIF_STATE_MSG[7];
		case WLIF_UAP_BSS_ON:			return WLIF_STATE_MSG[8];
		case WLIF_UAP_BSS_OFF:			return WLIF_STATE_MSG[9];
		case WLIF_UAP_BSS_ERROR:		return WLIF_STATE_MSG[10];
		case WLIF_UAP_DISCONN:			return WLIF_STATE_MSG[11];
		case WLIF_INIT_FAIL:			return WLIF_STATE_MSG[12];
		case WLIF_STA_NO_AP:			return WLIF_STATE_MSG[13];
		case WLIF_STA_WPA_AUTH_FAIL:	return WLIF_STATE_MSG[14];
		case WLIF_NONE:					return WLIF_STATE_MSG[15];
		default:						return WLIF_STATE_MSG[16];
	}
}

static int wifi_uap_init(void)
{
	netif_state[enlWIFIUAP]._netif = ewl_add_netif(BSS_TYPE_UAP);
	if (netif_state[enlWIFIUAP]._netif) {
#ifdef custom_MAC_UAP
		u8 uap_mac[6] = WIFI_UAP_MACADDRESS;	//	AP mode MAC address
		ewl_set_hwaddr(gtwlif_uap, uap_mac);	// AP mode MAC address set
#endif
#ifdef SD8782_ENABLE
//		ewlap_sys_cfg_11n(1, 0x117e); //bw 75Mbps
#endif
		return ENX_OK;
	}
	return ENX_FAIL;
}

static int wifi_sta_init(void)
{
	netif_state[enlWIFISTA]._netif = ewl_add_netif(BSS_TYPE_STA);
	if (netif_state[enlWIFISTA]._netif) {
#ifdef custom_MAC_STA
		u8 sta_mac[6] = WIFI_STA_MACADDRESS;	// STA mode MAC address
		ewl_set_hwaddr(gtwlif_sta, sta_mac);	// STA mode MAC address set
#endif
		return ENX_OK;
	}
	return ENX_FAIL;
}

static int wifi_init(void)
{
#ifdef GPIO_SDIO1_RST    
	GpioSetLo(GPIO_SDIO1_RST); 
	vTaskDelay(1);
	GpioSetHi(GPIO_SDIO1_RST);
	vTaskDelay(1);
#endif

	SdioWfIoIrqCallback(ewl_sdio_ext_irq_handler, NULL);
	SdioWfSetIoIrqEn(ENX_ON);

	GpioSetOut(WF_GPIO_RST, GPIO_OUT_LOW); // LO:ON HI:OFF
	vTaskDelay(1);

	if (ewl_init() == EWL_SUCCESS) {	/* wifi initialize*/
		printf("Wi-Fi start complete.\n");
		return ENX_OK;
	}

	printf("Wi-Fi start failed.\n");
	return ENX_FAIL;
}

#define WLIF_NUM_EVENT 10

typedef struct {
	UINT head;
	UINT tail;
	UINT tot_num;
	wlif_priv_t event[WLIF_NUM_EVENT];
	SemaphoreHandle_t sema;
} wlif_event_t;

static wlif_event_t wlif_event;

void wlif_init_event(void)
{
	memset((void *)&wlif_event, 0, sizeof(wlif_event_t));
	wlif_event.tot_num = WLIF_NUM_EVENT;
	wlif_event.sema = xSemaphoreCreateCounting(wlif_event.tot_num, 0);
}

int wlif_put_event(UINT type, UINT event, UINT netif)
{
	int bRes = ENX_FAIL;
	if (cQueue_isfull(&wlif_event) != ENX_OK) {
		portENTER_CRITICAL();
		wlif_priv_t *evt = &wlif_event.event[wlif_event.tail];
		evt->type = type;
		evt->event = event;
		evt->netif = netif;
		num_loop(wlif_event.tail, wlif_event.tot_num);
		portEXIT_CRITICAL();
		xSemaphoreGive(wlif_event.sema);
		bRes = ENX_OK;
	}
	return bRes;
}

int wlif_get_event(wlif_priv_t *priv)
{
	int	bRes = ENX_FAIL;
	if (cQueue_isempty(&wlif_event) == ENX_FAIL) {
		portENTER_CRITICAL();
		wlif_priv_t *evt = &wlif_event.event[wlif_event.head];
		priv->type = evt->type;
		priv->event = evt->event;
		priv->netif = evt->netif;
		num_loop(wlif_event.head, wlif_event.tot_num);
		portEXIT_CRITICAL();
		bRes = ENX_OK;
	}
	return bRes;
}

#include "mlan_decl.h"
extern t_u32 mlan_drvdbg;

extern void wifiPollTask(void *pvParameters);
void wifiTask(void *pvParameters)
{
	wlif_priv_t wlif_priv;

	wlif_init_event();

	gtNetwork.UAPcfg.un32State = WLIF_NONE;
	gtNetwork.STAcfg.un32State = WLIF_NONE;

	mlan_drvdbg = 0xffffffff;

	vTaskCreate("wifipoll", wifiPollTask, NULL, LV5_STACK_SIZE, LV7_TASK_PRIO);

	if (wifi_init() == ENX_FAIL) {
		vTaskDelete(NULL);
	}

	if (gtNetwork.STAcfg.un1Switch == ENX_ON) {
		if (wifi_sta_init() == ENX_OK) {
			printf("%s initialization complete.\n", netif_state[enlWIFISTA].ifname);
			wlif_put_event(enlWIFISTA, WLIF_STA_START, (UINT)netif_state[enlWIFISTA]._netif); // WLIF_STA
		} else {
			printf("%s initialization failed.\n", netif_state[enlWIFISTA].ifname);
		}
	}

	if (gtNetwork.UAPcfg.un1Switch == ENX_ON) {
		if (wifi_uap_init() == ENX_OK) {
			printf("%s initialization complete.\n", netif_state[enlWIFIUAP].ifname);
#if 0
			wlif_put_event(enlWIFIUAP, WLIF_UAP_START, (UINT)netif_state[enlWIFIUAP]._netif); // WLIF_UAP
#else
			network_wlif_uap_set_default();
			if (gtNetwork.UAPcfg.un32State == WLIF_NONE) {
				if (gtNetwork.UAPcfg.un1Switch == ENX_ON) {
					network_wlif_uap_start();

					printf("%s %s start.\n", netif_state[enlWIFIUAP].ifname, gtNetwork.UAPcfg.strSSID);
					gtNetwork.UAPcfg.un32State = WLIF_UAP_ACTIVE;


					printf("mlan_drvdbg: 0x%08X\n", mlan_drvdbg);


				} else {
					printf("%s(%d) : AP switch state : off(state:%s)\n", __func__, __LINE__, getWlifStateMsg(gtNetwork.UAPcfg.un32State));
				}
			} else {
				printf("%s(%d) : case error(state:%s)\n", __func__, __LINE__, getWlifStateMsg(gtNetwork.UAPcfg.un32State));
			}
#endif
		} else {
			printf("%s initialization failed.\n", netif_state[enlWIFIUAP].ifname);
		}
	}

	vTaskDelete(NULL);

#if 0
	while (1) {
		if (xSemaphoreTake(wlif_event.sema, portMAX_DELAY) == pdTRUE) {
//		if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
			printf("!");
			vTaskDelay(100);
#if 0
			if (wlif_get_event(&wlif_priv) == ENX_OK) {
				printf("Wi-Fi event [%u/%u]\n", wlif_priv.event, WLIF_UAP_START);
				switch (wlif_priv.event) {
					case WLIF_UAP_START:
						network_wlif_uap_set_default();
						if (gtNetwork.UAPcfg.un32State == WLIF_NONE) {
							if (gtNetwork.UAPcfg.un1Switch == ENX_ON) {
								network_wlif_uap_start();

								printf("%s %s start.\n", netif_state[enlWIFIUAP].ifname, gtNetwork.UAPcfg.strSSID);
								gtNetwork.UAPcfg.un32State = WLIF_UAP_ACTIVE;
							} else {
								printf("%s(%d) : AP switch state : off(state:%s)\n", __func__, __LINE__, getWlifStateMsg(gtNetwork.UAPcfg.un32State));
							}
						} else {
							printf("%s(%d) : case error(state:%s)\n", __func__, __LINE__, getWlifStateMsg(gtNetwork.UAPcfg.un32State));
						}
						break;
					default:
						printf("%s(%d) : switch default(input:%d)\n", __func__, __LINE__, wlif_priv.event);
						break;
				}
			}
#endif
		}
	}
#endif
	UNUSED(pvParameters);
}
#endif
