/*
 * WPA Supplicant
 * Copyright (c) 2003-2007, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file implements functions for registering and unregistering
 * %wpa_supplicant interfaces. In addition, this file contains number of
 * functions for managing network connections.
 */
//
#include "includes.h"

#include "wireless.h"
#include "common.h"
#include "eloop.h"
#include "wpa.h"
#include "wpa_supplicant.h"
#include "config.h"
#include "wpa_supplicant_i.h"
#include "driver_wext.h"

extern struct wpa_driver_ops wpa_driver_wext_ops; /* driver_wext.c */

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"


int errno=0;

extern struct wpa_driver_ops *wpa_supplicant_drivers[];

static void wpa_supplicant_scan(void *eloop_ctx, void *timeout_ctx);


/* Configure default/group WEP key for static WEP */
static int wpa_set_wep_key(void *ctx, int set_tx, int keyidx, const u8 *key,
               size_t keylen)
{
    struct wpa_supplicant *wpa_s = ctx;
    return wpa_drv_set_key(wpa_s, WPA_ALG_WEP,
                   (u8 *) "\xff\xff\xff\xff\xff\xff",
                   keyidx, set_tx, (u8 *) "", 0, key, keylen);
}


static int wpa_supplicant_set_wpa_none_key(struct wpa_supplicant *wpa_s,
                       struct wpa_ssid *ssid)
{
    u8 key[32];
    size_t keylen;
    wpa_alg alg;
    u8 seq[6] = { 0 };

    /* IBSS/WPA-None uses only one key (Group) for both receiving and
     * sending unicast and multicast packets. */

    if (ssid->mode != IEEE80211_MODE_IBSS) {
        wpa_printf(MSG_INFO, "WPA: Invalid mode %d (not IBSS/ad-hoc) "
               "for WPA-None", ssid->mode);
        return -1;
    }

    if (!ssid->psk_set) {
        wpa_printf(MSG_INFO, "WPA: No PSK configured for WPA-None");
        return -1;
    }

    switch (wpa_s->group_cipher) {
    case WPA_CIPHER_CCMP:
        os_memcpy(key, ssid->psk, 16);
        keylen = 16;
        alg = WPA_ALG_CCMP;
        break;
    case WPA_CIPHER_TKIP:
        /* WPA-None uses the same Michael MIC key for both TX and RX */
        os_memcpy(key, ssid->psk, 16 + 8);
        os_memcpy(key + 16 + 8, ssid->psk + 16, 8);
        keylen = 32;
        alg = WPA_ALG_TKIP;
        break;
    default:
        wpa_printf(MSG_INFO, "WPA: Invalid group cipher %d for "
               "WPA-None", wpa_s->group_cipher);
        return -1;
    }

    /* TODO: should actually remember the previously used seq#, both for TX
     * and RX from each STA.. */

    return wpa_drv_set_key(wpa_s, alg, (u8 *) "\xff\xff\xff\xff\xff\xff",
                   0, 1, seq, 6, key, keylen);
}




/**
 * wpa_supplicant_req_scan - Schedule a scan for neighboring access points
 * @wpa_s: Pointer to wpa_supplicant data
 * @sec: Number of seconds after which to scan
 * @usec: Number of microseconds after which to scan
 *
 * This function is used to schedule a scan for neighboring access points after
 * the specified time.
 */
void wpa_supplicant_req_scan(struct wpa_supplicant *wpa_s, int sec, int usec)
{
    wpa_msg(wpa_s, MSG_DEBUG, "Setting scan request: %d sec %d usec",
        sec, usec);
    eloop_cancel_timeout(wpa_supplicant_scan, wpa_s, NULL);
    eloop_register_timeout(sec, usec, wpa_supplicant_scan, wpa_s, NULL);
}


/**
 * wpa_supplicant_cancel_scan - Cancel a scheduled scan request
 * @wpa_s: Pointer to wpa_supplicant data
 *
 * This function is used to cancel a scan request scheduled with
 * wpa_supplicant_req_scan().
 */
void wpa_supplicant_cancel_scan(struct wpa_supplicant *wpa_s)
{
    wpa_msg(wpa_s, MSG_DEBUG, "Cancelling scan request");
    eloop_cancel_timeout(wpa_supplicant_scan, wpa_s, NULL);
}


static void wpa_supplicant_timeout(void *eloop_ctx, void *timeout_ctx)
{
    struct wpa_supplicant *wpa_s = eloop_ctx;
    const u8 *bssid = wpa_s->bssid;

    if (os_memcmp(bssid, "\x00\x00\x00\x00\x00\x00", ETH_ALEN) == 0)
        bssid = wpa_s->pending_bssid;
    wpa_msg(wpa_s, MSG_INFO, "Authentication with " MACSTR " timed out.",
        MAC2STR(bssid));
        
    wpa_supplicant_disassociate(wpa_s, REASON_DEAUTH_LEAVING);
    wpa_s->reassociate = 1;
    wpa_supplicant_req_scan(wpa_s, 0, 0);
	UNUSED(timeout_ctx);
}


/**
 * wpa_supplicant_req_auth_timeout - Schedule a timeout for authentication
 * @wpa_s: Pointer to wpa_supplicant data
 * @sec: Number of seconds after which to time out authentication
 * @usec: Number of microseconds after which to time out authentication
 *
 * This function is used to schedule a timeout for the current authentication
 * attempt.
 */
void wpa_supplicant_req_auth_timeout(struct wpa_supplicant *wpa_s,
                     int sec, int usec)
{
    if (wpa_s->conf && wpa_s->conf->ap_scan == 0 &&
        wpa_s->driver && os_strcmp(wpa_s->driver->name, "wired") == 0)
        return;

    wpa_msg(wpa_s, MSG_DEBUG, "Setting authentication timeout: %d sec "
        "%d usec", sec, usec);
    eloop_cancel_timeout(wpa_supplicant_timeout, wpa_s, NULL);
    eloop_register_timeout(sec, usec, wpa_supplicant_timeout, wpa_s, NULL);
}


/**
 * wpa_supplicant_cancel_auth_timeout - Cancel authentication timeout
 * @wpa_s: Pointer to wpa_supplicant data
 *
 * This function is used to cancel authentication timeout scheduled with
 * wpa_supplicant_req_auth_timeout() and it is called when authentication has
 * been completed.
 */
void wpa_supplicant_cancel_auth_timeout(struct wpa_supplicant *wpa_s)
{
    wpa_msg(wpa_s, MSG_DEBUG, "Cancelling authentication timeout");
    eloop_cancel_timeout(wpa_supplicant_timeout, wpa_s, NULL);
}


/**
 * wpa_supplicant_set_non_wpa_policy - Set WPA parameters to non-WPA mode
 * @wpa_s: Pointer to wpa_supplicant data
 * @ssid: Configuration data for the network
 *
 * This function is used to configure WPA state machine and related parameters
 * to a mode where WPA is not enabled. This is called as part of the
 * authentication configuration when the selected network does not use WPA.
 */
void wpa_supplicant_set_non_wpa_policy(struct wpa_supplicant *wpa_s,
                       struct wpa_ssid *ssid)
{
    int i;

    if (ssid->key_mgmt & WPA_KEY_MGMT_IEEE8021X_NO_WPA)
        wpa_s->key_mgmt = WPA_KEY_MGMT_IEEE8021X_NO_WPA;
    else
        wpa_s->key_mgmt = WPA_KEY_MGMT_NONE;

    wpa_s->pairwise_cipher = WPA_CIPHER_NONE;
    wpa_s->group_cipher = WPA_CIPHER_NONE;
    wpa_s->mgmt_group_cipher = 0;

    for (i = 0; i < NUM_WEP_KEYS; i++) {
        if (ssid->wep_key_len[i] > 5) {
            wpa_s->pairwise_cipher = WPA_CIPHER_WEP104;
            wpa_s->group_cipher = WPA_CIPHER_WEP104;
            break;
        } else if (ssid->wep_key_len[i] > 0) {
            wpa_s->pairwise_cipher = WPA_CIPHER_WEP40;
            wpa_s->group_cipher = WPA_CIPHER_WEP40;
            break;
        }
    }
}

#if 0
static void wpa_supplicant_cleanup(struct wpa_supplicant *wpa_s)
{
#if 0
    if (wpa_s->ctrl_iface) {
        wpa_supplicant_ctrl_iface_deinit(wpa_s->ctrl_iface);
        wpa_s->ctrl_iface = NULL;
    }
    if (wpa_s->conf != NULL) {
        wpa_config_free(wpa_s->conf);
        wpa_s->conf = NULL;
    }

    os_free(wpa_s->confname);
    wpa_s->confname = NULL;

    wpa_s->wpa = NULL;

    os_free(wpa_s->scan_results);
    wpa_s->scan_results = NULL;
    wpa_s->num_scan_results = 0;

    wpa_supplicant_cancel_scan(wpa_s);
    wpa_supplicant_cancel_auth_timeout(wpa_s);
#endif
	UNUSED(wpa_s);
}
#endif

/**
 * wpa_clear_keys - Clear keys configured for the driver
 * @wpa_s: Pointer to wpa_supplicant data
 * @addr: Previously used BSSID or %NULL if not available
 *
 * This function clears the encryption keys that has been previously configured
 * for the driver.
 */
void wpa_clear_keys(struct wpa_supplicant *wpa_s, const u8 *addr)
{
    u8 *bcast = (u8 *) "\xff\xff\xff\xff\xff\xff";

    if (wpa_s->keys_cleared) {
        /* Some drivers (e.g., ndiswrapper & NDIS drivers) seem to have
         * timing issues with keys being cleared just before new keys
         * are set or just after association or something similar. This
         * shows up in group key handshake failing often because of the
         * client not receiving the first encrypted packets correctly.
         * Skipping some of the extra key clearing steps seems to help
         * in completing group key handshake more reliably. */
        wpa_printf(MSG_DEBUG, "No keys have been configured - "
               "skip key clearing");
        return;
    }

    /* MLME-DELETEKEYS.request */
    wpa_drv_set_key(wpa_s, WPA_ALG_NONE, bcast, 0, 0, NULL, 0, NULL, 0);
    wpa_drv_set_key(wpa_s, WPA_ALG_NONE, bcast, 1, 0, NULL, 0, NULL, 0);
    wpa_drv_set_key(wpa_s, WPA_ALG_NONE, bcast, 2, 0, NULL, 0, NULL, 0);
    wpa_drv_set_key(wpa_s, WPA_ALG_NONE, bcast, 3, 0, NULL, 0, NULL, 0);
    
    if (addr) {
        wpa_drv_set_key(wpa_s, WPA_ALG_NONE, addr, 0, 0, NULL, 0, NULL,
                0);
        /* MLME-SETPROTECTION.request(None) */
        wpa_drv_mlme_setprotection(
            wpa_s, addr,
            MLME_SETPROTECTION_PROTECT_TYPE_NONE,
            MLME_SETPROTECTION_KEY_TYPE_PAIRWISE);
    }
    wpa_s->keys_cleared = 1;
}


/**
 * wpa_supplicant_state_txt - Get the connection state name as a text string
 * @state: State (wpa_state; WPA_*)
 * Returns: The state name as a printable text string
 */
const char * wpa_supplicant_state_txt(int state)
{
    switch (state) {
    case WPA_DISCONNECTED:
        return "DISCONNECTED";
    case WPA_INACTIVE:
        return "INACTIVE";
    case WPA_SCANNING:
        return "SCANNING";
    case WPA_ASSOCIATING:
        return "ASSOCIATING";
    case WPA_ASSOCIATED:
        return "ASSOCIATED";
    case WPA_4WAY_HANDSHAKE:
        return "4WAY_HANDSHAKE";
    case WPA_GROUP_HANDSHAKE:
        return "GROUP_HANDSHAKE";
    case WPA_COMPLETED:
        return "COMPLETED";
    default:
        return "UNKNOWN";
    }
}


/**
 * wpa_supplicant_set_state - Set current connection state
 * @wpa_s: Pointer to wpa_supplicant data
 * @state: The new connection state
 *
 * This function is called whenever the connection state changes, e.g.,
 * association is completed for WPA/WPA2 4-Way Handshake is started.
 */
void wpa_supplicant_set_state(struct wpa_supplicant *wpa_s, wpa_states state)
{
    wpa_printf(MSG_DEBUG, "State: %s -> %s",
           wpa_supplicant_state_txt(wpa_s->wpa_state),
           wpa_supplicant_state_txt(state));                

    if (state == WPA_COMPLETED && wpa_s->new_connection) {
        struct wpa_ssid *ssid = wpa_s->current_ssid;
        wpa_msg(wpa_s, MSG_INFO, "WPA_EVENT_CONNECTED - Connection to "
            MACSTR " completed %s [id=%d]",
            MAC2STR(wpa_s->bssid), wpa_s->reassociated_connection ?
            "(reauth)" : "(auth)",
            ssid ? ssid->id : -1);
		UNUSED(ssid);
        wpa_s->new_connection = 0;
        wpa_s->reassociated_connection = 1;
    } else if (state == WPA_DISCONNECTED || state == WPA_ASSOCIATING ||
           state == WPA_ASSOCIATED) {
        wpa_s->new_connection = 1;
    }
    wpa_s->wpa_state = state;
}


/**
 * wpa_supplicant_get_state - Get the connection state
 * @wpa_s: Pointer to wpa_supplicant data
 * Returns: The current connection state (WPA_*)
 */
wpa_states wpa_supplicant_get_state(struct wpa_supplicant *wpa_s)
{
    return wpa_s->wpa_state;
}

#if 0
static void wpa_supplicant_terminate(int sig, void *eloop_ctx,
                     void *signal_ctx)
{
#if 0
    struct wpa_global *global = eloop_ctx;
    struct wpa_supplicant *wpa_s;
    for (wpa_s = global->ifaces; wpa_s; wpa_s = wpa_s->next) {
        wpa_msg(wpa_s, MSG_INFO, WPA_EVENT_TERMINATING "- signal %d "
            "received", sig);
    }
    eloop_terminate();
#endif
	UNUSED(sig);
	UNUSED(eloop_ctx);
	UNUSED(signal_ctx);
}


static void wpa_supplicant_clear_status(struct wpa_supplicant *wpa_s)
{
    wpa_s->pairwise_cipher = 0;
    wpa_s->group_cipher = 0;
    wpa_s->mgmt_group_cipher = 0;
    wpa_s->key_mgmt = 0;
    wpa_s->wpa_state = WPA_DISCONNECTED;
}
#endif

static void wpa_supplicant_gen_assoc_event(struct wpa_supplicant *wpa_s)
{
    struct wpa_ssid *ssid;
    union wpa_event_data data;

    ssid = wpa_supplicant_get_ssid(wpa_s);
    if (ssid == NULL)
        return;

    if (wpa_s->current_ssid == NULL)
        wpa_s->current_ssid = ssid;
        
    wpa_printf(MSG_DEBUG, "Already associated with a configured network - "
           "generating associated event");
    os_memset(&data, 0, sizeof(data));
    wpa_supplicant_event(wpa_s, EVENT_ASSOC, &data);
}

/*******************************************************************************
 *
 * wpa_s->conf is assigned by  wpa_config_read() in wpa_supplicant_init_iface()
 * wpa_s->conf->ssid is assigned by wpa_config_read_network()
 * 
 * wpa_s->scan_req is set to '1' in wpa_supplicant_alloc()
********************************************************************************/


static void wpa_supplicant_scan(void *eloop_ctx, void *timeout_ctx)
{
    struct wpa_supplicant *wpa_s = eloop_ctx;
    struct wpa_ssid *ssid;
    int enabled, scan_req = 0, ret;

    if (wpa_s->disconnected)
        return;

    enabled = 0;
    ssid = wpa_s->conf->ssid;

    while (ssid) {
        wpa_printf(MSG_DEBUG, "ssid->disabled %d",ssid->disabled);
        if (!ssid->disabled) {
            enabled++;
            break;
        }
        ssid = ssid->next;
    }

    if (!enabled && !wpa_s->scan_req) {
        wpa_printf(MSG_DEBUG, "No enabled networks - do not scan");
        wpa_supplicant_set_state(wpa_s, WPA_INACTIVE);
        return;
    }

    scan_req = wpa_s->scan_req; //is set to '1'  in the wpa_supplicant_alloc() 
    wpa_s->scan_req = 0;

    if (wpa_s->conf->ap_scan != 0 &&
        wpa_s->driver && os_strcmp(wpa_s->driver->name, "wired") == 0) {
        wpa_printf(MSG_DEBUG, "Using wired driver - overriding "
               "ap_scan configuration");
        wpa_s->conf->ap_scan = 0;
    }

    if (wpa_s->conf->ap_scan == 0) {
        wpa_supplicant_gen_assoc_event(wpa_s);
        return;
    }

    if (wpa_s->wpa_state == WPA_DISCONNECTED ||
        wpa_s->wpa_state == WPA_INACTIVE)
        wpa_supplicant_set_state(wpa_s, WPA_SCANNING);

    ssid = wpa_s->conf->ssid;

    if (wpa_s->prev_scan_ssid != BROADCAST_SSID_SCAN) {
        while (ssid) {
            if (ssid == wpa_s->prev_scan_ssid) {
                ssid = ssid->next;
                break;
            }
            ssid = ssid->next;
        }
    }
    
    while (ssid) {
        if (!ssid->disabled &&
            (ssid->scan_ssid || wpa_s->conf->ap_scan == 2))
            break;
        ssid = ssid->next;
    }

    if (scan_req != 2 && wpa_s->conf->ap_scan == 2) {
        /*
         * ap_scan=2 mode - try to associate with each SSID instead of
         * scanning for each scan_ssid=1 network.
         */
        if (ssid == NULL) {
            wpa_printf(MSG_DEBUG, "wpa_supplicant_scan: Reached "
                   "end of scan list - go back to beginning");
            wpa_s->prev_scan_ssid = BROADCAST_SSID_SCAN;
            wpa_supplicant_req_scan(wpa_s, 0, 0);
            return;
        }
        if (ssid->next) {
            /* Continue from the next SSID on the next attempt. */
            wpa_s->prev_scan_ssid = ssid;
        } else {
            /* Start from the beginning of the SSID list. */
            wpa_s->prev_scan_ssid = BROADCAST_SSID_SCAN;
        }
        wpa_supplicant_associate(wpa_s, NULL, ssid);
        return;
    }

    wpa_printf(MSG_DEBUG,"Starting AP scan (%s SSID) \n",
           ssid ? "specific": "broadcast");
    if (ssid) {
        wpa_hexdump_ascii(MSG_DEBUG, "Scan SSID",
                  ssid->ssid, ssid->ssid_len);
        wpa_s->prev_scan_ssid = ssid;
    } else
        wpa_s->prev_scan_ssid = BROADCAST_SSID_SCAN;

    if (wpa_s->scan_res_tried == 0 && wpa_s->conf->ap_scan == 1) {
        wpa_s->scan_res_tried++;
        wpa_printf(MSG_DEBUG,"Trying to get current scan results "
               "first without requesting a new scan to speed up "
               "initial association \n");
        wpa_supplicant_event(wpa_s, EVENT_SCAN_RESULTS, NULL);
        return;
    }

    ret = wpa_drv_scan(wpa_s, ssid ? ssid->ssid : NULL,
                   ssid ? ssid->ssid_len : 0);

    if (ret) {
        wpa_printf(MSG_DEBUG,"Failed to initiate AP scan.");
        wpa_supplicant_req_scan(wpa_s, 10, 0);
    }
	UNUSED(timeout_ctx);
}


static wpa_cipher cipher_suite2driver(int cipher)
{
    switch (cipher) {
    case WPA_CIPHER_NONE:
        return CIPHER_NONE;
    case WPA_CIPHER_WEP40:
        return CIPHER_WEP40;
    case WPA_CIPHER_WEP104:
        return CIPHER_WEP104;
    case WPA_CIPHER_CCMP:
        return CIPHER_CCMP;
    case WPA_CIPHER_TKIP:
    default:
        return CIPHER_TKIP;
    }
}


static wpa_key_mgmt key_mgmt2driver(int key_mgmt)
{
    switch (key_mgmt) {
    case WPA_KEY_MGMT_NONE:
        return KEY_MGMT_NONE;
    case WPA_KEY_MGMT_IEEE8021X_NO_WPA:
        return KEY_MGMT_802_1X_NO_WPA;
    case WPA_KEY_MGMT_IEEE8021X:
        return KEY_MGMT_802_1X;
    case WPA_KEY_MGMT_WPA_NONE:
        return KEY_MGMT_WPA_NONE;
    case WPA_KEY_MGMT_PSK:
    default:
        return KEY_MGMT_PSK;
    }
}


static int wpa_supplicant_suites_from_ai(struct wpa_supplicant *wpa_s,
                     struct wpa_ssid *ssid,
                     struct wpa_ie_data *ie)
{
    wpa_printf(MSG_DEBUG, "WPA: Using WPA IE from AssocReq to set cipher "
           "suites");
    if (!(ie->group_cipher & ssid->group_cipher)) {
        wpa_msg(wpa_s, MSG_INFO, "WPA: Driver used disabled group "
            "cipher 0x%x (mask 0x%x) - reject",
            ie->group_cipher, ssid->group_cipher);
        return -1;
    }
    if (!(ie->pairwise_cipher & ssid->pairwise_cipher)) {
        wpa_msg(wpa_s, MSG_INFO, "WPA: Driver used disabled pairwise "
            "cipher 0x%x (mask 0x%x) - reject",
            ie->pairwise_cipher, ssid->pairwise_cipher);
        return -1;
    }
    if (!(ie->key_mgmt & ssid->key_mgmt)) {
        wpa_msg(wpa_s, MSG_INFO, "WPA: Driver used disabled key "
            "management 0x%x (mask 0x%x) - reject",
            ie->key_mgmt, ssid->key_mgmt);
        return -1;
    }

    return 0;
	UNUSED(wpa_s);
}

void printf_config_ssd(struct wpa_ssid *ssid);

/**
 * wpa_supplicant_set_suites - Set authentication and encryption parameters
 * @wpa_s: Pointer to wpa_supplicant data
 * @bss: Scan results for the selected BSS, or %NULL if not available
 * @ssid: Configuration data for the selected network
 * @wpa_ie: Buffer for the WPA/RSN IE
 * @wpa_ie_len: Maximum wpa_ie buffer size on input. This is changed to be the
 * used buffer length in case the functions returns success.
 * Returns: 0 on success or -1 on failure
 *
 * This function is used to configure authentication and encryption parameters
 * based on the network configuration and scan result for the selected BSS (if
 * available).
 */
int wpa_supplicant_set_suites(struct wpa_supplicant *wpa_s,
                  struct wpa_scan_result *bss,
                  struct wpa_ssid *ssid,
                  u8 *wpa_ie, size_t *wpa_ie_len)
{
    struct wpa_ie_data ie;
    int sel, proto;

    if (bss && bss->rsn_ie_len && (ssid->proto & WPA_PROTO_RSN) &&
        ewl_parse_wpa_ie(bss->rsn_ie, bss->rsn_ie_len, &ie) == 0 &&
        (ie.group_cipher & ssid->group_cipher) &&
        (ie.pairwise_cipher & ssid->pairwise_cipher) &&
        (ie.key_mgmt & ssid->key_mgmt)) {
        wpa_msg(wpa_s, MSG_DEBUG, "RSN: using IEEE 802.11i/D9.0");
        proto = WPA_PROTO_RSN;
    } else if (bss && bss->wpa_ie_len && (ssid->proto & WPA_PROTO_WPA) &&
           ewl_parse_wpa_ie(bss->wpa_ie, bss->wpa_ie_len, &ie) == 0 &&
           (ie.group_cipher & ssid->group_cipher) &&
           (ie.pairwise_cipher & ssid->pairwise_cipher) &&
           (ie.key_mgmt & ssid->key_mgmt)) {
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using IEEE 802.11i/D3.0");
        proto = WPA_PROTO_WPA;
    } else if (bss) {
        wpa_msg(wpa_s, MSG_WARNING, "WPA: Failed to select WPA/RSN");
        return -1;
    } else {
        if (ssid->proto & WPA_PROTO_RSN)
            proto = WPA_PROTO_RSN;
        else
            proto = WPA_PROTO_WPA;
        if (wpa_supplicant_suites_from_ai(wpa_s, ssid, &ie) < 0) {
            os_memset(&ie, 0, sizeof(ie));
            ie.group_cipher = ssid->group_cipher;
            ie.pairwise_cipher = ssid->pairwise_cipher;
            ie.key_mgmt = ssid->key_mgmt;
            wpa_printf(MSG_DEBUG, "WPA: Set cipher suites based "
                   "on configuration");
        } else
            proto = ie.proto;
    }

    wpa_printf(MSG_DEBUG, "WPA: Selected cipher suites: group %d "
           "pairwise %d key_mgmt %d proto %d",
           ie.group_cipher, ie.pairwise_cipher, ie.key_mgmt, proto);

    sel = ie.group_cipher & ssid->group_cipher;
    if (sel & WPA_CIPHER_CCMP) {
        wpa_s->group_cipher = WPA_CIPHER_CCMP;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using GTK CCMP");
    } else if (sel & WPA_CIPHER_TKIP) {
        wpa_s->group_cipher = WPA_CIPHER_TKIP;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using GTK TKIP");
    } else if (sel & WPA_CIPHER_WEP104) {
        wpa_s->group_cipher = WPA_CIPHER_WEP104;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using GTK WEP104");
    } else if (sel & WPA_CIPHER_WEP40) {
        wpa_s->group_cipher = WPA_CIPHER_WEP40;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using GTK WEP40");
    } else {
        wpa_printf(MSG_WARNING, "WPA: Failed to select group cipher.");
        return -1;
    }

    sel = ie.pairwise_cipher & ssid->pairwise_cipher;
    if (sel & WPA_CIPHER_CCMP) {
        wpa_s->pairwise_cipher = WPA_CIPHER_CCMP;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using PTK CCMP");
    } else if (sel & WPA_CIPHER_TKIP) {
        wpa_s->pairwise_cipher = WPA_CIPHER_TKIP;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using PTK TKIP");
    } else if (sel & WPA_CIPHER_NONE) {
        wpa_s->pairwise_cipher = WPA_CIPHER_NONE;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using PTK NONE");
    } else {
        wpa_printf(MSG_WARNING, "WPA: Failed to select pairwise "
               "cipher.");
        return -1;
    }

    sel = ie.key_mgmt & ssid->key_mgmt;
    if (sel & WPA_KEY_MGMT_IEEE8021X) {
        wpa_s->key_mgmt = WPA_KEY_MGMT_IEEE8021X;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using KEY_MGMT 802.1X");
    } else if (sel & WPA_KEY_MGMT_PSK) {
        wpa_s->key_mgmt = WPA_KEY_MGMT_PSK;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using KEY_MGMT WPA-PSK");
    } else if (sel & WPA_KEY_MGMT_WPA_NONE) {
        wpa_s->key_mgmt = WPA_KEY_MGMT_WPA_NONE;
        wpa_msg(wpa_s, MSG_DEBUG, "WPA: using KEY_MGMT WPA-NONE");
    } else {
        wpa_printf(MSG_WARNING, "WPA: Failed to select authenticated "
               "key management type.");
        return -1;
    }

    return 0;
	UNUSED(wpa_ie);
	UNUSED(wpa_ie_len);
}




/**
 * wpa_supplicant_associate - Request association
 * @wpa_s: Pointer to wpa_supplicant data
 * @bss: Scan results for the selected BSS, or %NULL if not available
 * @ssid: Configuration data for the selected network
 *
 * This function is used to request %wpa_supplicant to associate with a BSS.
 */
void wpa_supplicant_associate(struct wpa_supplicant *wpa_s,
                  struct wpa_scan_result *bss,
                  struct wpa_ssid *ssid)
{
    u8 wpa_ie[80];
    size_t wpa_ie_len;
    int use_crypt, ret, i;
    int algs = AUTH_ALG_OPEN_SYSTEM;
    wpa_cipher cipher_pairwise, cipher_group;
    struct wpa_driver_associate_params params;
    int wep_keys_set = 0;
    struct wpa_driver_capa capa;
    int assoc_failed = 0;

    wpa_s->reassociate = 0;
    if (bss) {
        wpa_msg(wpa_s, MSG_INFO, "Trying to associate with " MACSTR
            " (SSID='%s' freq=%d MHz)", MAC2STR(bss->bssid),
            wpa_ssid_txt(bss->ssid, bss->ssid_len), bss->freq);
        os_memset(wpa_s->bssid, 0, ETH_ALEN);
        os_memcpy(wpa_s->pending_bssid, bss->bssid, ETH_ALEN);
    } else {
        wpa_msg(wpa_s, MSG_INFO, "Trying to associate with SSID '%s'",
            wpa_ssid_txt(ssid->ssid, ssid->ssid_len));
        os_memset(wpa_s->pending_bssid, 0, ETH_ALEN);
    }
    wpa_supplicant_cancel_scan(wpa_s);

    if (ssid->auth_alg) {
        algs = 0;
        if (ssid->auth_alg & WPA_AUTH_ALG_OPEN)
            algs |= AUTH_ALG_OPEN_SYSTEM;
        if (ssid->auth_alg & WPA_AUTH_ALG_SHARED)
            algs |= AUTH_ALG_SHARED_KEY;
        if (ssid->auth_alg & WPA_AUTH_ALG_LEAP)
            algs |= AUTH_ALG_LEAP;
        wpa_printf(MSG_DEBUG, "Overriding auth_alg selection: 0x%x",
               algs);
    }
    
    wpa_drv_set_auth_alg(wpa_s, algs);

    if (bss && (bss->wpa_ie_len || bss->rsn_ie_len) &&
        (ssid->key_mgmt & (WPA_KEY_MGMT_IEEE8021X | WPA_KEY_MGMT_PSK))) {
            int try_opportunistic;
            try_opportunistic = (ssid->proto & WPA_PROTO_RSN);
            wpa_ie_len = sizeof(wpa_ie);

            if (wpa_supplicant_set_suites(wpa_s, bss, ssid,
                          wpa_ie, &wpa_ie_len)) {
            wpa_printf(MSG_WARNING, "WPA: Failed to set WPA key "
                   "management and encryption suites");
            return;
        }

        wpa_driver_wext_set_passphrase(wpa_s->drv_priv,ssid);
        use_crypt = 0;
    }
    else if (ssid->key_mgmt &
           (WPA_KEY_MGMT_PSK | WPA_KEY_MGMT_IEEE8021X |
            WPA_KEY_MGMT_WPA_NONE)) {

        wpa_ie_len = sizeof(wpa_ie);

        if (wpa_supplicant_set_suites(wpa_s, NULL, ssid,
                          wpa_ie, &wpa_ie_len)) {
            wpa_printf(MSG_WARNING, "WPA: Failed to set WPA key "
                   "management and encryption suites (no scan "
                   "results)");
            return;
        }
        wpa_driver_wext_set_passphrase(wpa_s->drv_priv,ssid);
        use_crypt = 0;
    } else {
        wpa_supplicant_set_non_wpa_policy(wpa_s, ssid);
        wpa_ie_len = 0;
    }
    
    //use_crypt = 1;
    cipher_pairwise = cipher_suite2driver(wpa_s->pairwise_cipher);
    cipher_group = cipher_suite2driver(wpa_s->group_cipher);
    if (wpa_s->key_mgmt == WPA_KEY_MGMT_NONE ||
        wpa_s->key_mgmt == WPA_KEY_MGMT_IEEE8021X_NO_WPA) {     
        wpa_clear_keys(wpa_s, bss ? bss->bssid : NULL);
        if (wpa_s->key_mgmt == WPA_KEY_MGMT_NONE)
            use_crypt = 0;
        for (i = 0; i < NUM_WEP_KEYS; i++) {
            if (ssid->wep_key_len[i]) {
                use_crypt = 1;
                wep_keys_set = 1;
                wpa_set_wep_key(wpa_s,
                        i == ssid->wep_tx_keyidx,
                        i, ssid->wep_key[i],
                        ssid->wep_key_len[i]);
            }
        }
    }

    if (wpa_s->key_mgmt == WPA_KEY_MGMT_WPA_NONE) {
        /* Set the key before (and later after) association */
        wpa_supplicant_set_wpa_none_key(wpa_s, ssid);
    }

    wpa_drv_set_drop_unencrypted(wpa_s, use_crypt);
    wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATING);
    os_memset(&params, 0, sizeof(params));
    if (bss) {
        params.bssid = bss->bssid;
        params.ssid = bss->ssid;
        params.ssid_len = bss->ssid_len;
        params.freq = bss->freq;
    } else {
        params.ssid = ssid->ssid;
        params.ssid_len = ssid->ssid_len;
    }
    params.wpa_ie = wpa_ie;
    params.wpa_ie_len = wpa_ie_len;
    params.pairwise_suite = cipher_pairwise;
    params.group_suite = cipher_group;
    params.key_mgmt_suite = key_mgmt2driver(wpa_s->key_mgmt);
    params.auth_alg = algs;
    params.mode = ssid->mode;
    for (i = 0; i < NUM_WEP_KEYS; i++) {
        if (ssid->wep_key_len[i])
            params.wep_key[i] = ssid->wep_key[i];
        params.wep_key_len[i] = ssid->wep_key_len[i];
    }
    params.wep_tx_keyidx = ssid->wep_tx_keyidx;
    ret = wpa_drv_associate(wpa_s, &params);

    if (ret < 0) {
        wpa_msg(wpa_s, MSG_INFO, "Association request to the driver "
            "failed");
        /* try to continue anyway; new association will be tried again
         * after timeout */
        assoc_failed = 1;
    }

    if (wpa_s->key_mgmt == WPA_KEY_MGMT_WPA_NONE) {
        /* Set the key after the association just in case association
         * cleared the previously configured key. */
        wpa_supplicant_set_wpa_none_key(wpa_s, ssid);
        /* No need to timeout authentication since there is no key
         * management. */
        wpa_supplicant_cancel_auth_timeout(wpa_s);
        wpa_supplicant_set_state(wpa_s, WPA_COMPLETED);
    } else {
    
        /* Timeout for IEEE 802.11 authentication and association */
        
        int timeout;
        if (assoc_failed)
            timeout = 5;
        else if (wpa_s->conf->ap_scan == 1)
            timeout = 10;
        else
            timeout = 60;
        wpa_supplicant_req_auth_timeout(wpa_s, timeout, 0);
        wpa_supplicant_cancel_auth_timeout(wpa_s);
        wpa_supplicant_set_state(wpa_s, WPA_COMPLETED);
    }

    if (wep_keys_set && wpa_drv_get_capa(wpa_s, &capa) == 0 &&
        capa.flags & WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC) {
        /* Set static WEP keys again */
        int j;
        for (j = 0; j < NUM_WEP_KEYS; j++) {
            if (ssid->wep_key_len[j]) {
                wpa_set_wep_key(wpa_s,
                        j == ssid->wep_tx_keyidx,
                        j, ssid->wep_key[j],
                        ssid->wep_key_len[j]);
            }
        }
    }
    
    wpa_s->current_ssid = ssid;
}

int wpa_config_write(const char *name, struct wpa_config *config);

/**
 * wpa_supplicant_disassociate - Disassociate the current connection
 * @wpa_s: Pointer to wpa_supplicant data
 * @reason_code: IEEE 802.11 reason code for the disassociate frame
 *
 * This function is used to request %wpa_supplicant to disassociate with the
 * current AP.
 */
void wpa_supplicant_disassociate(struct wpa_supplicant *wpa_s,
                 int reason_code)
{
    u8 *addr = NULL;
    if (os_memcmp(wpa_s->bssid, "\x00\x00\x00\x00\x00\x00", ETH_ALEN) != 0)
    {
        wpa_drv_disassociate(wpa_s, wpa_s->bssid, reason_code);
        addr = wpa_s->bssid;
    }
    wpa_clear_keys(wpa_s, addr);
    wpa_supplicant_mark_disassoc(wpa_s);
    wpa_s->current_ssid = NULL;
}


/**
 * wpa_supplicant_deauthenticate - Deauthenticate the current connection
 * @wpa_s: Pointer to wpa_supplicant data
 * @reason_code: IEEE 802.11 reason code for the deauthenticate frame
 *
 * This function is used to request %wpa_supplicant to disassociate with the
 * current AP.
 */
void wpa_supplicant_deauthenticate(struct wpa_supplicant *wpa_s,
                   int reason_code)
{
    u8 *addr = NULL;
    wpa_supplicant_set_state(wpa_s, WPA_DISCONNECTED);
    if (os_memcmp(wpa_s->bssid, "\x00\x00\x00\x00\x00\x00", ETH_ALEN) != 0)
    {

        wpa_drv_deauthenticate(wpa_s, wpa_s->bssid,reason_code);
        addr = wpa_s->bssid;
    }
    wpa_clear_keys(wpa_s, addr);
    wpa_s->current_ssid = NULL;
}


/**
 * wpa_supplicant_get_scan_results - Get scan results
 * @wpa_s: Pointer to wpa_supplicant data
 * Returns: 0 on success, -1 on failure
 *
 * This function is request the current scan results from the driver and stores
 * a local copy of the results in wpa_s->scan_results.
 */
int wpa_supplicant_get_scan_results(struct wpa_supplicant *wpa_s)
{
#define SCAN_AP_LIMIT 128
    struct wpa_scan_result *results, *tmp;
    int num;

    results = os_malloc(SCAN_AP_LIMIT * sizeof(struct wpa_scan_result));
    if (results == NULL) {
        wpa_printf(MSG_WARNING, "Failed to allocate memory for scan "
               "results");
        return -1;
    }

    num = wpa_drv_get_scan_results(wpa_s, results, SCAN_AP_LIMIT);

    if (num < 0) {
        wpa_printf(MSG_DEBUG, "Failed to get scan results");
        os_free(results);
        return -1;
    }
    if (num > SCAN_AP_LIMIT) {
        wpa_printf(MSG_INFO, "Not enough room for all APs (%d < %d)",
               num, SCAN_AP_LIMIT);
        num = SCAN_AP_LIMIT;
    }
	
    /* Free unneeded memory for unused scan result entries */
    tmp = os_realloc(results, num * sizeof(struct wpa_scan_result));
    if (tmp || num == 0) {
        results = tmp;
    }
    if(wpa_s->scan_results)
        os_free(wpa_s->scan_results);
    wpa_s->scan_results = results;
    wpa_s->num_scan_results = num;
    return 0;
}

/**
 * wpa_supplicant_get_ssid - Get a pointer to the current network structure
 * @wpa_s: Pointer to wpa_supplicant data
 * Returns: A pointer to the current network structure or %NULL on failure
 */
struct wpa_ssid * wpa_supplicant_get_ssid(struct wpa_supplicant *wpa_s)
{
    struct wpa_ssid *entry;
    u8 ssid[MAX_SSID_LEN];
    int res;
    size_t ssid_len;
    u8 bssid[ETH_ALEN];
    int wired;


    res = wpa_drv_get_ssid(wpa_s, ssid);
    if (res < 0) {
        wpa_printf(MSG_WARNING, "Could not read SSID from "
               "driver.");
        return NULL;
    }
    ssid_len = res;

    if (wpa_drv_get_bssid(wpa_s, bssid) < 0) {
        wpa_printf(MSG_WARNING, "Could not read BSSID from driver.");
        return NULL;
    }

    wired = wpa_s->conf->ap_scan == 0 && wpa_s->driver &&
        os_strcmp(wpa_s->driver->name, "wired") == 0;

    entry = wpa_s->conf->ssid;
    while (entry) {
        if (!entry->disabled &&
            ((ssid_len == entry->ssid_len &&
              os_memcmp(ssid, entry->ssid, ssid_len) == 0) || wired) &&
            (!entry->bssid_set ||
             os_memcmp(bssid, entry->bssid, ETH_ALEN) == 0))
            return entry;
        entry = entry->next;
    }

    return NULL;
}

int 
wpa_supplicant_set_driver(struct wpa_supplicant *wpa_s)
{
//    int i;

    /* default to first driver in the list */
    wpa_s->driver = &wpa_driver_wext_ops;
    
    return 0;
}


/**
 * wpa_supplicant_driver_init - Initialize driver interface parameters
 * @wpa_s: Pointer to wpa_supplicant data
 * @wait_for_interface: 0 = do not wait for the interface (reports a failure if
 * the interface is not present), 1 = wait until the interface is available
 * Returns: 0 on success, -1 on failure
 *
 * This function is called to initialize driver interface parameters.
 * wpa_drv_init() must have been called before this function to initialize the
 * driver interface.
 */
int wpa_supplicant_driver_init(struct wpa_supplicant *wpa_s)
{
    static int interface_count = 0;

    /* Backwards compatibility call to set_wpa() handler. This is called
     * only just after init and just before deinit, so these handler can be
     * used to implement same functionality. */

    wpa_clear_keys(wpa_s, NULL);

    /* Make sure that TKIP countermeasures are not left enabled (could
     * happen if wpa_supplicant is killed during countermeasures. */
    wpa_drv_set_countermeasures(wpa_s, 0);

    wpa_drv_set_drop_unencrypted(wpa_s, 1);

    //wpa_s->prev_scan_ssid = BROADCAST_SSID_SCAN;
    /* 100000 means 0.1 second */
    //wpa_supplicant_req_scan(wpa_s, interface_count, 100000);
    interface_count++;

    return 0;
}

static struct wpa_supplicant * wpa_supplicant_alloc(void)
{
    struct wpa_supplicant *wpa_s;

    wpa_s = os_zalloc(sizeof(*wpa_s));
    if (wpa_s == NULL)
        return NULL;
    wpa_s->scan_req = 1;

    return wpa_s;
}


static inline void 
wpa_supplicant_deinit_iface(struct wpa_supplicant *wpa_s)
{
	UNUSED(wpa_s);
}


/**
 * wpa_supplicant_add_iface - Add wpa supplicant 
 *
 */
struct wpa_supplicant * 
wpa_supplicant_init(void)
{
    struct wpa_supplicant *wpa_s;
    
    eloop_init();
    wpa_s = wpa_supplicant_alloc();
    if (wpa_s == NULL)
        return NULL;

    wpa_s->current_id = -1;
    wpa_s->scan_results = NULL;

    if (wpa_supplicant_set_driver(wpa_s) < 0) {
        goto err;
    }

    wpa_s->conf = os_zalloc(sizeof(struct wpa_config));
    wpa_s->conf->ap_scan = DEFAULT_AP_SCAN;;

    if (wpa_s->conf == NULL) {
        wpa_printf(MSG_ERROR, "Failed to read configuration ");
        goto err;;
    }
    /*
        we don't use L2 Message.
    */
    wpa_s->drv_priv = wpa_drv_init(wpa_s,NULL);
    if (wpa_s->drv_priv == NULL) {
        wpa_printf(MSG_ERROR, "Failed to initialize driver interface");
        goto err;;
    }

    /*
        start connect to AP
    */
	if (wpa_supplicant_driver_init(wpa_s) < 0) {
	    wpa_printf(MSG_ERROR, "wpa supplicant driver init failed ");
		goto err;
	}

    return wpa_s;

err:
    os_free(wpa_s);
    return NULL;
}

#if 0
/**
 * wpa_supplicant_run - Run the %wpa_supplicant main event loop
 * @global: Pointer to global data from wpa_supplicant_init()
 * Returns: 0 after successful event loop run, -1 on failure
 *
 * This function starts the main event loop and continues running as long as
 * there are any remaining events. In most cases, this function is running as
 * long as the %wpa_supplicant process in still in use.
 */
int wpa_supplicant_run(struct wpa_global *global)
{
    eloop_run();
    return 0;
}
#endif


/**
 * wpa_supplicant_deinit - Deinitialize %wpa_supplicant
 * @global: Pointer to global data from wpa_supplicant_init()
 *
 * This function is called to deinitialize %wpa_supplicant and to free all
 * allocated resources. Remaining network interfaces will also be removed.
 */
inline void wpa_supplicant_deinit(struct wpa_global *global)
{
	UNUSED(global);
}
