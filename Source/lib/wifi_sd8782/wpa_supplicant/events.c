/*
 * WPA Supplicant - Driver event processing
 * Copyright (c) 2003-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include "includes.h"

#include "wireless.h"
#include "common.h"
#include "eloop.h"
#include "wpa.h"
#include "wpa_supplicant.h"
#include "config.h"
#include "wpa_supplicant_i.h"

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"


static int wpa_supplicant_select_config(struct wpa_supplicant *wpa_s)
{
	struct wpa_ssid *ssid;

	if (wpa_s->conf->ap_scan == 1 && wpa_s->current_ssid)
		return 0;

	ssid = wpa_supplicant_get_ssid(wpa_s);
	if (ssid == NULL) {
		wpa_printf(MSG_INFO, "No network configuration found for the "
			   "current AP");
		return -1;
	}

	if (ssid->disabled) {
		wpa_printf(MSG_DEBUG, "Selected network is disabled");
		return -1;
	}

	wpa_printf(MSG_DEBUG, "Network configuration found for the current "
		   "AP");
	if (ssid->key_mgmt & (WPA_KEY_MGMT_PSK | WPA_KEY_MGMT_IEEE8021X |
			      WPA_KEY_MGMT_WPA_NONE)) {
		u8 wpa_ie[80];
		size_t wpa_ie_len = sizeof(wpa_ie);
		wpa_supplicant_set_suites(wpa_s, NULL, ssid,
					  wpa_ie, &wpa_ie_len);
	} else {
		wpa_supplicant_set_non_wpa_policy(wpa_s, ssid);
	}
	wpa_s->current_ssid = ssid;
	
	return 0;
}

#if 0
static void wpa_supplicant_stop_countermeasures(void *eloop_ctx,
						void *sock_ctx)
{
	struct wpa_supplicant *wpa_s = eloop_ctx;

	if (wpa_s->countermeasures) {
		wpa_s->countermeasures = 0;
		wpa_drv_set_countermeasures(wpa_s, 0);
		wpa_msg(wpa_s, MSG_INFO, "WPA: TKIP countermeasures stopped");
		wpa_supplicant_req_scan(wpa_s, 0, 0);
	}
	UNUSED(sock_ctx);
}
#endif

void wpa_supplicant_mark_disassoc(struct wpa_supplicant *wpa_s)
{
	wpa_supplicant_set_state(wpa_s, WPA_DISCONNECTED);
	os_memset(wpa_s->bssid, 0, ETH_ALEN);
	os_memset(wpa_s->pending_bssid, 0, ETH_ALEN);
}


static int wpa_supplicant_dynamic_keys(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->key_mgmt == WPA_KEY_MGMT_NONE ||
	    wpa_s->key_mgmt == WPA_KEY_MGMT_WPA_NONE)
		return 0;

	return 1;
}


static int wpa_supplicant_match_privacy(struct wpa_scan_result *bss,
					struct wpa_ssid *ssid)
{
	int i, privacy = 0;

	for (i = 0; i < NUM_WEP_KEYS; i++) {
		if (ssid->wep_key_len[i]) {
			privacy = 1;
			break;
		}
	}

	if (bss->caps & IEEE80211_CAP_PRIVACY)
		return privacy;
	return !privacy;
}


static int wpa_supplicant_ssid_bss_match(struct wpa_ssid *ssid,
					 struct wpa_scan_result *bss)
{
	struct wpa_ie_data ie;
	int proto_match = 0;

	while ((ssid->proto & WPA_PROTO_RSN) && bss->rsn_ie_len > 0) {
		proto_match++;

		if (ewl_parse_wpa_ie(bss->rsn_ie, bss->rsn_ie_len, &ie)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - parse failed");
			break;
		}
		if (!(ie.proto & ssid->proto)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - proto "
				   "mismatch");
			break;
		}

		if (!(ie.pairwise_cipher & ssid->pairwise_cipher)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - PTK cipher "
				   "mismatch");
			break;
		}

		if (!(ie.group_cipher & ssid->group_cipher)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - GTK cipher "
				   "mismatch");
			break;
		}

		if (!(ie.key_mgmt & ssid->key_mgmt)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - key mgmt "
				   "mismatch");
			break;
		}

		wpa_printf(MSG_DEBUG, "   selected based on RSN IE");
		return 1;
	}

	while ((ssid->proto & WPA_PROTO_WPA) && bss->wpa_ie_len > 0) {
		proto_match++;

		if (ewl_parse_wpa_ie(bss->wpa_ie, bss->wpa_ie_len, &ie)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - parse failed");
			break;
		}
		if (!(ie.proto & ssid->proto)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - proto "
				   "mismatch");
			break;
		}

		if (!(ie.pairwise_cipher & ssid->pairwise_cipher)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - PTK cipher "
				   "mismatch");
			break;
		}

		if (!(ie.group_cipher & ssid->group_cipher)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - GTK cipher "
				   "mismatch");
			break;
		}

		if (!(ie.key_mgmt & ssid->key_mgmt)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - key mgmt "
				   "mismatch");
			break;
		}

		wpa_printf(MSG_DEBUG, "   selected based on WPA IE");
		return 1;
	}

	if (proto_match == 0)
		wpa_printf(MSG_DEBUG, "   skip - no WPA/RSN proto match");

	return 0;
}


static struct wpa_scan_result *
wpa_supplicant_select_bss(struct wpa_supplicant *wpa_s, struct wpa_ssid *group,
			  struct wpa_scan_result *results, int num,
			  struct wpa_ssid **selected_ssid)
{
	struct wpa_ssid *ssid;
	struct wpa_scan_result *bss, *selected = NULL;
	int i;

	wpa_printf(MSG_DEBUG, "Selecting BSS from priority group %d",
		   group->priority);

	bss = NULL;
	ssid = NULL;
	/* First, try to find WPA-enabled AP */
	wpa_printf(MSG_DEBUG, "Try to find WPA-enabled AP");
	for (i = 0; i < num && !selected; i++) {
		bss = &results[i];
		wpa_printf(MSG_DEBUG, "%d: " MACSTR " ssid='%s' "
			   "wpa_ie_len=%lu rsn_ie_len=%lu caps=0x%x",
			   i, MAC2STR(bss->bssid),
			   wpa_ssid_txt(bss->ssid, bss->ssid_len),
			   (unsigned long) bss->wpa_ie_len,
			   (unsigned long) bss->rsn_ie_len, bss->caps);

		if (bss->wpa_ie_len == 0 && bss->rsn_ie_len == 0) {
			wpa_printf(MSG_DEBUG, "   skip - no WPA/RSN IE");
			continue;
		}

		for (ssid = group; ssid; ssid = ssid->pnext) {
			if (ssid->disabled) {
				wpa_printf(MSG_DEBUG, "   skip - disabled");
				continue;
			}
			if (bss->ssid_len != ssid->ssid_len ||
			    os_memcmp(bss->ssid, ssid->ssid,
				      bss->ssid_len) != 0) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "SSID mismatch");
				continue;
			}
			if (ssid->bssid_set &&
			    os_memcmp(bss->bssid, ssid->bssid, ETH_ALEN) != 0)
			{
				wpa_printf(MSG_DEBUG, "   skip - "
					   "BSSID mismatch");
				continue;
			}
			if (wpa_supplicant_ssid_bss_match(ssid, bss)) {
				selected = bss;
				*selected_ssid = ssid;
				wpa_printf(MSG_DEBUG, "   selected WPA AP "
					   MACSTR " ssid='%s'",
					   MAC2STR(bss->bssid),
					   wpa_ssid_txt(bss->ssid,
							bss->ssid_len));
				break;
			}
		}
	}

	/* If no WPA-enabled AP found, try to find non-WPA AP, if configuration
	 * allows this. */
	wpa_printf(MSG_DEBUG, "Try to find non-WPA AP");
	for (i = 0; i < num && !selected; i++) {
		bss = &results[i];
		wpa_printf(MSG_DEBUG, "%d: " MACSTR " ssid='%s' "
			   "wpa_ie_len=%lu rsn_ie_len=%lu caps=0x%x",
			   i, MAC2STR(bss->bssid),
			   wpa_ssid_txt(bss->ssid, bss->ssid_len),
			   (unsigned long) bss->wpa_ie_len,
			   (unsigned long) bss->rsn_ie_len, bss->caps);

		for (ssid = group; ssid; ssid = ssid->pnext) {
			if (ssid->disabled) {
				wpa_printf(MSG_DEBUG, "   skip - disabled");
				continue;
			}
			
			if (ssid->ssid_len != 0 &&
			    (bss->ssid_len != ssid->ssid_len ||
			     os_memcmp(bss->ssid, ssid->ssid,
				       bss->ssid_len) != 0)) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "SSID mismatch");
				continue;
			}

			if (ssid->bssid_set &&
			    os_memcmp(bss->bssid, ssid->bssid, ETH_ALEN) != 0)
			{
				wpa_printf(MSG_DEBUG, "   skip - "
					   "BSSID mismatch");
				continue;
			}
			
			if (!(ssid->key_mgmt & WPA_KEY_MGMT_NONE) &&
			    !(ssid->key_mgmt & WPA_KEY_MGMT_IEEE8021X_NO_WPA))
			{
				wpa_printf(MSG_DEBUG, "   skip - "
					   "non-WPA network not allowed");
				continue;
			}

			if ((ssid->key_mgmt & 
			     (WPA_KEY_MGMT_IEEE8021X | WPA_KEY_MGMT_PSK)) &&
			    (bss->wpa_ie_len != 0 || bss->rsn_ie_len != 0)) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "WPA network");
				continue;
			}

			if (!wpa_supplicant_match_privacy(bss, ssid)) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "privacy mismatch");
				continue;
			}

			if (bss->caps & IEEE80211_CAP_IBSS) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "IBSS (adhoc) network");
				continue;
			}

			selected = bss;
			*selected_ssid = ssid;
			wpa_printf(MSG_DEBUG, "   selected non-WPA AP "
				   MACSTR " ssid='%s'",
				   MAC2STR(bss->bssid),
				   wpa_ssid_txt(bss->ssid, bss->ssid_len));
			break;
		}
	}

	return selected;
	UNUSED(wpa_s);
}


static void wpa_supplicant_event_scan_results(struct wpa_supplicant *wpa_s)
{
	int num, prio, timeout;
	struct wpa_scan_result *selected = NULL;
	struct wpa_ssid *ssid = NULL;
	struct wpa_scan_result *results;
	
	if (wpa_supplicant_get_scan_results(wpa_s) < 0) {
		if (wpa_s->conf->ap_scan == 2)
			return;

		wpa_printf(MSG_DEBUG, "Failed to get scan results - try "
			   "scanning again");
		timeout = 1;
		goto req_scan;
	}

// hjlee. 15.3.20
	if(wpa_s->scan_state)
	{
		wpa_s->scan_state = 0;
		if(wpa_s->scan_complete_cb)
			wpa_s->scan_complete_cb();
	}

	if (wpa_s->conf->ap_scan == 2 || wpa_s->disconnected)
		return;
	results = wpa_s->scan_results;
	num = wpa_s->num_scan_results;

	while (selected == NULL) {
		for (prio = 0; prio < wpa_s->conf->num_prio; prio++) {
			selected = wpa_supplicant_select_bss(
				wpa_s, wpa_s->conf->pssid[prio], results, num,
				&ssid);
			if (selected)
				break;
		}
		if (selected == NULL) 
			break;		
	}

// hjlee. 15.3.20
//	if(wpa_s->scan_state)
//	{
//		wpa_s->scan_state = 0;
//		if(wpa_s->scan_complete_cb)
//			wpa_s->scan_complete_cb();
//	}

	if (selected) {
		/* Do not trigger new association unless the BSSID has changed
		 * or if reassociation is requested. If we are in process of
		 * associating with the selected BSSID, do not trigger new
		 * attempt. */
        wpa_hexdump(MSG_MSGDUMP, "selected->bssid",          	selected->bssid,        ETH_ALEN);
		wpa_hexdump(MSG_MSGDUMP, "wpa_s->bssid",             	wpa_s->bssid,           ETH_ALEN);
		wpa_hexdump(MSG_MSGDUMP, "wpa_s->pending_bssid",     	wpa_s->pending_bssid,   ETH_ALEN);
		
		if (wpa_s->reassociate ||
		    (os_memcmp(selected->bssid, wpa_s->bssid, ETH_ALEN) != 0 &&
		     (wpa_s->wpa_state != WPA_ASSOCIATING ||
		      os_memcmp(selected->bssid, wpa_s->pending_bssid,
				ETH_ALEN) != 0))) {
			wpa_supplicant_associate(wpa_s, selected, ssid);
		} else {
			wpa_printf(MSG_DEBUG, "Already associated with the "
				   "selected AP.");
		}
	} else {
		wpa_printf(MSG_DEBUG, "No suitable AP found. %d",wpa_s->scan_res_tried);
		if(wpa_s->scan_res_tried >= 5)
		{
            if(wpa_s->connect_failed)
                wpa_s->connect_failed();
            if(wpa_s->disconnect_cb)
                wpa_s->disconnect_cb(NO_AP);    
            //printf("scan .... timeout \n");
            wpa_s->scan_res_tried = 0;
            return;
		}
		timeout = 5;
		goto req_scan;
	}

	return;

req_scan:
	if (wpa_s->scan_res_tried >= 1 && wpa_s->conf->ap_scan == 1) {
		/*
		 * Quick recovery if the initial scan results were not
		 * complete when fetched before the first scan request.
		 */
		wpa_s->scan_res_tried++;
        if(wpa_s->scan_res_tried ==1)
		    timeout = 0;
	}
	
	wpa_supplicant_req_scan(wpa_s, timeout, 0);
}


static void wpa_supplicant_event_associnfo(struct wpa_supplicant *wpa_s,
					   union wpa_event_data *data)
{
	int l;//, len, found = 0, wpa_found, rsn_found;
	u8 *p;

	wpa_printf(MSG_DEBUG, "Association info event");
	if (data->assoc_info.req_ies)
		wpa_hexdump(MSG_DEBUG, "req_ies", data->assoc_info.req_ies,
			    data->assoc_info.req_ies_len);
	if (data->assoc_info.resp_ies)
		wpa_hexdump(MSG_DEBUG, "resp_ies", data->assoc_info.resp_ies,
			    data->assoc_info.resp_ies_len);
	if (data->assoc_info.beacon_ies)
		wpa_hexdump(MSG_DEBUG, "beacon_ies",
			    data->assoc_info.beacon_ies,
			    data->assoc_info.beacon_ies_len);

	p = data->assoc_info.req_ies;
	l = data->assoc_info.req_ies_len;
	UNUSED(wpa_s);
}


static void wpa_supplicant_event_assoc(struct wpa_supplicant *wpa_s,
				       union wpa_event_data *data)
{
	u8 bssid[ETH_ALEN];
   
	if (data)
		wpa_supplicant_event_associnfo(wpa_s, data);

	wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATED);
	

	if ((wpa_drv_get_bssid(wpa_s, bssid) >= 0 &&
	     os_memcmp(bssid, wpa_s->bssid, ETH_ALEN) != 0)) {
		wpa_msg(wpa_s, MSG_DEBUG, "Associated to a new BSS: BSSID="
			MACSTR, MAC2STR(bssid));
		os_memcpy(wpa_s->bssid, bssid, ETH_ALEN);
		os_memset(wpa_s->pending_bssid, 0, ETH_ALEN);
		if (wpa_supplicant_dynamic_keys(wpa_s)) {
			wpa_clear_keys(wpa_s, bssid);
		}
		if (wpa_supplicant_select_config(wpa_s) < 0) {
			wpa_supplicant_disassociate(wpa_s,
						    REASON_DEAUTH_LEAVING);
			return;
		}
	}
	
	wpa_msg(wpa_s, MSG_INFO, "Associated with " MACSTR, MAC2STR(bssid));

	if (wpa_s->key_mgmt == WPA_KEY_MGMT_NONE ||
	    wpa_s->key_mgmt == WPA_KEY_MGMT_WPA_NONE) {
		wpa_supplicant_cancel_auth_timeout(wpa_s);
		wpa_supplicant_set_state(wpa_s, WPA_COMPLETED);
	} else {
		/* Timeout for receiving the first EAPOL packet */
		//wpa_supplicant_req_auth_timeout(wpa_s, 10, 0);
		wpa_supplicant_cancel_auth_timeout(wpa_s);
		wpa_supplicant_set_state(wpa_s, WPA_COMPLETED);
	}

	// AP connect OK / disconnect count init : hjlee(160201)
	wpa_s->disconnect_cnt = 0;

	wpa_supplicant_cancel_scan(wpa_s);

}


static void wpa_supplicant_event_disassoc(struct wpa_supplicant *wpa_s)
{
	const u8 *bssid;

	if (wpa_s->key_mgmt == WPA_KEY_MGMT_WPA_NONE) {
		/*
		 * At least Host AP driver and a Prism3 card seemed to be
		 * generating streams of disconnected events when configuring
		 * IBSS for WPA-None. Ignore them for now.
		 */
		wpa_printf(MSG_DEBUG, "Disconnect event - ignore in "
			   "IBSS/WPA-None mode");
		return;
	}

    //we use sd8787's internal supplicant which process 4-hand shake.
	if (wpa_s->key_mgmt == WPA_KEY_MGMT_PSK) {
		wpa_msg(wpa_s, MSG_INFO, "WPA: 4-Way Handshake failed - "
			"pre-shared key may be incorrect");
	}
	
	if (wpa_s->wpa_state >= WPA_ASSOCIATED)
	{   
	    if(wpa_s->disconnect_cnt++ < MAX_DISCONNECT_CNT) 
			wpa_supplicant_req_scan(wpa_s, 0, 100000);
        else
        {
            //we can't try connect to ap.
            //password may be incorrect.
            if (wpa_s->disconnect_cb)
                wpa_s->disconnect_cb(WPA_AUTH_FAIL);
        }    
    }
	bssid = wpa_s->bssid;
	if (os_memcmp(bssid, "\x00\x00\x00\x00\x00\x00", ETH_ALEN) == 0)
		bssid = wpa_s->pending_bssid;

	wpa_msg(wpa_s, MSG_INFO, "WPA_EVENT_DISCONNECTED - Disconnect event - "
		"remove keys");
	if (wpa_supplicant_dynamic_keys(wpa_s)) {
		wpa_s->keys_cleared = 0;
		wpa_clear_keys(wpa_s, wpa_s->bssid);
	}
	wpa_supplicant_mark_disassoc(wpa_s);
}

void wpa_supplicant_event(struct wpa_supplicant *wpa_s, wpa_event_type event,
			  union wpa_event_data *data)
{
	switch (event) {
	case EVENT_ASSOC:
		wpa_supplicant_event_assoc(wpa_s, data);
		break;

	case EVENT_DISASSOC:
		wpa_supplicant_event_disassoc(wpa_s);
		break;

	case EVENT_SCAN_RESULTS:
		wpa_supplicant_event_scan_results(wpa_s);
		break;

	case EVENT_ASSOCINFO:
		wpa_supplicant_event_associnfo(wpa_s, data);
		break;

	default:
		wpa_printf(MSG_INFO, "Unknown event %d", event);
		break;
	}
}
