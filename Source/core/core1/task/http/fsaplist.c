#include "dev.h"

#ifdef __WIFI__

#include "msg.h"
#include "user.h"

#include "lwip/mem.h"

#include "wifi.h"
#include "wlif_tools.h"
#include "wifi_task.h"
#include "fsaplist.h"

// scanap.js Header
#define SCANAPJShead		"var sta_ssidsel = %d;\r\nvar aplist = [\r\n"
#define SCANAPJShead_size	37

// scanap.js Footer
#define SCANAPJSfoot		"];"

// scanap.js Body Dir type (40 + (SSID(string) + FREQ(uint + RSSI(int) + FLAG(string))byte
#define SCANAPJSbody		"{\"SSID\":\"%s\",\"FREQ\":%u,\"RSSI\":%d,\"FLAG\":[%s]},\r\n"
#define SCANAPJSbody_size	40

static int web_number_count(UINT number)
{
	int i;
	for (i = 0; number / 10 != 0; i++) {
		number = number / 10;
	}
	return i + 1;
}

char *enx_web_get_aplist(UINT *allocsize)
{
	char *data = NULL;
	if (gtNetwork.STAcfg.un1Switch == DEF_OFF) {
		data = (char *)mem_calloc(1, 44);
		if (data == NULL) {
			printf("%s(%d) : malloc fail(4byte) state(STA switch off)\r\n", __func__, __LINE__);
			return NULL;
		}
		*allocsize = sprintf(data, "var sta_ssidsel = -1;\r\nvar aplist = [];\r\n");
	} else {
#if 0
		gptMsgWlifCmd.resp = 0; // ask cpu1 to reply for cpu0
		cpu2cop_wlif_execmd("scanned");
		while (gptMsgWlifCmd.resp != 1) vTaskDelay(1);
		wlif_scan_t *pwlif_scan = (wlif_scan_t *)gptMsgWlifCmd.cmd;
		flprintf("get scan AP list(%d)\r\n", pwlif_scan->num);
		if (!pwlif_scan->num) {
			flprintf("not APs\r\n");
			data = (char *)mem_calloc(1, 44);
			if (data == NULL) {
				printf("%s(%d) : malloc fail : aplist(%d) contents_len(%d)\r\n", __func__, __LINE__, pwlif_scan->num, 44);
				return NULL;
			}
			*allocsize = sprintf(data, "var sta_ssidsel = -1;\r\nvar aplist = [];\r\n");
		} else {
			int i, j, selectIndex = -1;
			char strbuffer[160];
			UINT contents_len = SCANAPJShead_size + (sizeof(SCANAPJSfoot)-1) + (SCANAPJSbody_size * pwlif_scan->num);
			for (i = 0; i < pwlif_scan->num; i++) {
				wl_scans_t *pscan = &pwlif_scan->wl_scans[i];

				contents_len += strlen(pscan->ssid);
				if (strcmp(gtNetwork.STAcfg.strSSID, pscan->ssid) == 0) {
					selectIndex = i;
				}
				contents_len += web_number_count(pscan->freq); // uint to string 
				contents_len += sprintf(strbuffer, "%d", ((char)pscan->level)); // int to string

				int nlen = strlen(pscan->flags);
				int ncnt = 0; // 인증방식 수
				for (j = 0; j < nlen; j++) {
					if (pscan->flags[j] == '[') {
						ncnt++;
					} else if (pscan->flags[j] == ']') {
						;
					} else {
						contents_len++;
					}
				}
				contents_len += (3 * ncnt);
			}
			contents_len += web_number_count(selectIndex);

			data = (char *)mem_calloc(1, contents_len);
			if (data == NULL) {
				printf("%s(%d) : malloc fail : aplist(%d) contents_len(%d)\r\n", __func__, __LINE__, pwlif_scan->num, contents_len);
				return NULL;
			}

			char *pdata = data;
			UINT realdata_len = 0, onetime_len = 0;

			// Add Header
			onetime_len = sprintf(pdata, SCANAPJShead, selectIndex);
			realdata_len += onetime_len;
			pdata += onetime_len;

			// Add Body
			for (i = 0; i < pwlif_scan->num; i++) {
				wl_scans_t *pscan = &pwlif_scan->wl_scans[i];

				int npos = 0;
				int nlen = strlen(pscan->flags);
				for (j = 0; j < nlen; j++) {
					if (pscan->flags[j] == '[') {
						strbuffer[npos] = '"';
					} else if (pscan->flags[j] == ']') {
						strbuffer[npos] = '"';
						npos++;
						strbuffer[npos] = ',';
					} else {
						strbuffer[npos] = pscan->flags[j];
					}
					npos++;
				}
				strbuffer[npos] = '\0';

				onetime_len = sprintf(pdata, SCANAPJSbody, pscan->ssid, pscan->freq, ((char)pscan->level), strbuffer);
				realdata_len += onetime_len;
				pdata += onetime_len;
			}

			// Add Footer
			onetime_len = sprintf(pdata, SCANAPJSfoot);
			realdata_len += onetime_len;
			pdata += onetime_len;

			*allocsize = realdata_len;

			if (contents_len != realdata_len) {
				printf("	%04d:%s: contents_len(%d) != realdata_len(%d)\r\n", __LINE__, __func__, contents_len, realdata_len);
			}

			// data는 fs_close_custom함수에서 free한다.
		}
#else
		if (1) {
			web_number_count(0); // 경고 제거용...
			flprintf("Not configured!!!\r\n");
			data = (char *)mem_calloc(1, 44);
			if (data == NULL) {
				printf("%s(%d) : malloc fail(44)\r\n", __func__, __LINE__);
				return NULL;
			}
			*allocsize = sprintf(data, "var sta_ssidsel = -1;\r\nvar aplist = [];\r\n");
		}
#endif
	}
	return data;
}
#endif
