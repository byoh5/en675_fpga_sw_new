
#include <errno.h>
#include <ewl_os.h>
#include <ewl_pkt_header.h>

#include <sdio_func.h>      /* sdio driver */
#include <sdio.h>
#include <netif/etharp.h>   /* malloc */
#include <stdio.h>


#include "moal_util.h"
#include "moal_main.h"
#include "moal_sdio.h"
#include "moal_shim.h"
#include "moal_priv.h"

#include "moal_wext.h"
#include "wifi_firm.h" /* Marvell WiFi Firmware image */

/** Driver version */
const char driver_version[] = "SD8787-%s-M2614";

/* Here for now until needed in other places in lwIP */
#ifndef isprint
#define in_range(c, lo, up)  ((u8_t)c >= lo && (u8_t)c <= up)
#define isdigit(c)           in_range(c, '0', '9')
#define isxdigit(c)          (isdigit(c) || in_range(c, 'a', 'f') || in_range(c, 'A', 'F'))
#endif  


/********************************************************
        Local Variables
********************************************************/

/** Firmware name */
char *fw_name = NULL;
int req_fw_nowait = 0;

/** MAC address */
char *mac_addr = NULL;

#ifdef MFG_CMD_SUPPORT
/** Mfg mode */
int mfg_mode = 0;
#endif

/** SDIO interrupt mode (0: INT_MODE_SDIO, 1: INT_MODE_GPIO) */
int intmode = INT_MODE_SDIO;
/** GPIO interrupt pin number */
int gpiopin = 0;

/** Auto deep sleep */
int auto_ds = 0;

/** IEEE PS mode */
int ps_mode = 0;

/** Max Tx buffer size */
int max_tx_buf = 0;

#ifdef STA_SUPPORT
/** Max STA interfaces */
int max_sta_bss = DEF_STA_BSS;
#endif

#ifdef UAP_SUPPORT
/** Max uAP interfaces */
int max_uap_bss = DEF_UAP_BSS;
#endif

#if defined(WIFI_DIRECT_SUPPORT)
/** Max WIFIDIRECT interfaces */
int max_wfd_bss = DEF_WIFIDIRECT_BSS;
#endif

#ifdef SDIO_SUSPEND_RESUME
/** PM keep power */
int pm_keep_power = 1;
#endif

#if defined(STA_SUPPORT)
/** 802.11d configuration */
int cfg_11d = 0;
#endif

/** CAL data config file */
char *cal_data_cfg = NULL;
/** Init config file (MAC address, register etc.) */
char *init_cfg = NULL;

/** woal_callbacks */
static mlan_callbacks woal_callbacks = {
    .moal_get_fw_data = moal_get_fw_data,
    .moal_init_fw_complete = moal_init_fw_complete,
    .moal_shutdown_fw_complete = moal_shutdown_fw_complete,
    .moal_send_packet_complete = moal_send_packet_complete,
    .moal_recv_packet = moal_recv_packet,
    .moal_recv_event = moal_recv_event,
    .moal_ioctl_complete = moal_ioctl_complete,
    .moal_alloc_mlan_buffer = moal_alloc_mlan_buffer,
    .moal_free_mlan_buffer = moal_free_mlan_buffer,
    .moal_write_reg = moal_write_reg,
    .moal_read_reg = moal_read_reg,
    .moal_write_data_sync = moal_write_data_sync,
    .moal_read_data_sync = moal_read_data_sync,
    .moal_malloc = moal_malloc,
    .moal_mfree = moal_mfree,
	.moal_vmalloc = moal_vmalloc,
	.moal_vfree = moal_vfree,
    .moal_memset = moal_memset,
    .moal_memcpy = moal_memcpy,
    .moal_memmove = moal_memmove,
    .moal_memcmp = moal_memcmp,
    .moal_udelay = moal_udelay,
    .moal_get_system_time = moal_get_system_time,
    .moal_init_timer = moal_init_timer,
    .moal_free_timer = moal_free_timer,
    .moal_start_timer = moal_start_timer,
    .moal_stop_timer = moal_stop_timer,
    .moal_init_lock = moal_init_lock,
    .moal_free_lock = moal_free_lock,
    .moal_spin_lock = moal_spin_lock,
    .moal_spin_unlock = moal_spin_unlock,
    .moal_print = moal_print,
//    #.moal_print_netintf = moal_print_netintf,
    .moal_assert = moal_assert,
	.moal_tcp_ack_tx_ind = moal_tcp_ack_tx_ind,
};

/** Default Driver mode */
#if defined(STA_SUPPORT) && defined(UAP_SUPPORT)
#if defined(WIFI_DIRECT_SUPPORT)
int drv_mode = (DRV_MODE_STA | DRV_MODE_UAP | DRV_MODE_WIFIDIRECT);
#else
int drv_mode = (DRV_MODE_STA | DRV_MODE_UAP);
#endif
#else
#ifdef STA_SUPPORT
int drv_mode = DRV_MODE_STA;
#else
int drv_mode = DRV_MODE_UAP;
#endif /* STA_SUPPORT */
#endif /* STA_SUPPORT & UAP_SUPPORT */

/********************************************************
		Global Variables
********************************************************/

/**
 * the maximum number of adapter supported 
 **/
#define MAX_MLAN_ADAPTER    2
/**
 * The global variable of a pointer to moal_handle
 * structure variable
 **/
moal_handle *m_handle[MAX_MLAN_ADAPTER];

#ifdef DEBUG_LEVEL1
#ifdef DEBUG_LEVEL2
#define	DEFAULT_DEBUG_MASK	(0xffffffff)
#else
#define DEFAULT_DEBUG_MASK	(MMSG | MFATAL | MERROR)
#endif /* DEBUG_LEVEL2 */
t_u32 drvdbg = DEFAULT_DEBUG_MASK;
t_u32 ifdbg = 0;
#endif /* DEBUG_LEVEL1 */



mlan_debug_info info;

/********************************************************
		Local Functions
********************************************************/

/**
 *  @brief This function dynamically populates the driver mode table
 *
 *  @param handle           A pointer to moal_handle structure
 *  @param drv_mode_local   Driver mode
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
woal_update_drv_tbl(moal_handle * handle, int drv_mode_local)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    unsigned int intf_num = 0;
    int i = 0, j = 0;
    mlan_bss_attr *bss_tbl = NULL;

    ENTER();
    /* Calculate number of interfaces */
#ifdef STA_SUPPORT
    if (drv_mode_local & DRV_MODE_STA) {
        if ((max_sta_bss < 1) || (max_sta_bss > MAX_STA_BSS)) {
            PRINTM(MWARN, "Unsupported max_sta_bss (%d), setting to default\n",
                   max_sta_bss);
            max_sta_bss = DEF_STA_BSS;
        }
        intf_num += max_sta_bss;
    }
#endif /* STA_SUPPORT */

#ifdef UAP_SUPPORT
    if (drv_mode_local & DRV_MODE_UAP) {
        if ((max_uap_bss < 1) || (max_uap_bss > MAX_UAP_BSS)) {
            PRINTM(MWARN, "Unsupported max_uap_bss (%d), setting to default\n",
                   max_uap_bss);
            max_uap_bss = DEF_UAP_BSS;
        }
        intf_num += max_uap_bss;
    }
#endif /* UAP_SUPPORT */

#if defined(WIFI_DIRECT_SUPPORT)
    if (drv_mode_local & DRV_MODE_WIFIDIRECT) {
        if ((max_wfd_bss < 1) || (max_wfd_bss > MAX_WIFIDIRECT_BSS)) {
            PRINTM(MWARN, "Unsupported max_wfd_bss (%d), setting to default\n",
                   max_wfd_bss);
            max_wfd_bss = DEF_WIFIDIRECT_BSS;
        }
        intf_num += max_wfd_bss;
    }
#endif /* WIFI_DIRECT_SUPPORT && V14_FEATURE */

    /* Create BSS attribute table */
    if ((intf_num == 0) || (intf_num > MLAN_MAX_BSS_NUM)) {
        PRINTM(MERROR, "Unsupported number of BSS %d\n", intf_num);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    } else {
        /* Create new table */
        if (!
            (bss_tbl =
             (mlan_bss_attr *) ewl_os_malloc(sizeof(mlan_bss_attr)*intf_num))) {
            PRINTM(MERROR, "Could not create BSS attribute table\n");
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }
    }

    /* Populate BSS attribute table */
#ifdef STA_SUPPORT
    if (drv_mode_local & DRV_MODE_STA) {
        for (j = 0; j < max_sta_bss; j++) {
            if (i >= intf_num)
                break;
            bss_tbl[i].bss_type = MLAN_BSS_TYPE_STA;
            bss_tbl[i].frame_type = MLAN_DATA_FRAME_TYPE_ETH_II;
            bss_tbl[i].active = MTRUE;
            bss_tbl[i].bss_priority = 0;
            bss_tbl[i].bss_num = j;
			bss_tbl[i].bss_virtual = MFALSE;
            i++;
        }
    }
#endif /* STA_SUPPORT */

#ifdef UAP_SUPPORT
    if (drv_mode_local & DRV_MODE_UAP) {
        for (j = 0; j < max_uap_bss; j++) {
            if (i >= intf_num)
                break;
            bss_tbl[i].bss_type = MLAN_BSS_TYPE_UAP;
            bss_tbl[i].frame_type = MLAN_DATA_FRAME_TYPE_ETH_II;
            bss_tbl[i].active = MTRUE;
            bss_tbl[i].bss_priority = 0;
            bss_tbl[i].bss_num = j;
			bss_tbl[i].bss_virtual = MFALSE;
            i++;
        }
    }
#endif /* UAP_SUPPORT */

#if defined(WIFI_DIRECT_SUPPORT)
    if (drv_mode_local & DRV_MODE_WIFIDIRECT) {
        for (j = 0; j < max_wfd_bss; j++) {
            if (i >= intf_num)
                break;
            bss_tbl[i].bss_type = MLAN_BSS_TYPE_WIFIDIRECT;
            bss_tbl[i].frame_type = MLAN_DATA_FRAME_TYPE_ETH_II;
            bss_tbl[i].active = MTRUE;
            bss_tbl[i].bss_priority = 0;
            bss_tbl[i].bss_num = j;
			bss_tbl[i].bss_virtual = MFALSE;
            i++;
        }
#if defined(STA_CFG80211) && defined(UAP_CFG80211)
		last_wfd_index = j;
#endif
    }
#endif /* WIFI_DIRECT_SUPPORT && V14_FEATURE */

    /* Clear existing table, if any */
    if (handle->drv_mode.bss_attr != NULL) {
        ewl_os_free(handle->drv_mode.bss_attr);
        handle->drv_mode.bss_attr = NULL;
    }

    /* Create moal_drv_mode entry */
    handle->drv_mode.drv_mode = drv_mode;
    handle->drv_mode.intf_num = intf_num;
    handle->drv_mode.bss_attr = bss_tbl;
    if (fw_name) {
        handle->drv_mode.fw_name = fw_name;
    } else {
#if defined(UAP_SUPPORT) && defined(STA_SUPPORT)
        handle->drv_mode.fw_name = DEFAULT_AP_STA_FW_NAME;
#else
#ifdef UAP_SUPPORT
        handle->drv_mode.fw_name = DEFAULT_AP_FW_NAME;
#else
        handle->drv_mode.fw_name = DEFAULT_FW_NAME;
#endif /* UAP_SUPPORT */
#endif /* UAP_SUPPORT && STA_SUPPORT */
    }

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function initializes software
 *  
 *  @param handle A pointer to moal_handle structure
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_init_sw(moal_handle * handle)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    unsigned int i;
    mlan_device device;
    t_void *pmlan;

    ENTER();

    /* Initialize moal_handle structure */
    handle->hardware_status = HardwareStatusInitializing;
    handle->main_state = MOAL_STATE_IDLE;

    if (woal_update_drv_tbl(handle, drv_mode) != MLAN_STATUS_SUCCESS) {
        PRINTM(MERROR, "Could not update driver mode table\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    /* Initialize moal_handle structure */
    handle->hardware_status = HardwareStatusInitializing;
    /* PnP and power profile */
    handle->surprise_removed = MFALSE;

    /* Register to MLAN */
    ewl_os_memset(&device, 0, sizeof(mlan_device));
    device.pmoal_handle = handle;

#ifdef MFG_CMD_SUPPORT
    device.mfg_mode = (t_u32) mfg_mode;
#endif
#ifdef DEBUG_LEVEL1
    device.drvdbg = drvdbg;
#endif
    device.int_mode = (t_u32) intmode;
    device.gpio_pin = (t_u32) gpiopin;
    device.auto_ds = (t_u32) auto_ds;
    device.ps_mode = (t_u32) ps_mode;
    device.max_tx_buf = (t_u32) max_tx_buf;
#if defined(STA_SUPPORT)
    device.cfg_11d = (t_u32) cfg_11d;
#endif
#ifdef SDIO_MULTI_PORT_TX_AGGR
#ifdef MMC_QUIRK_BLKSZ_FOR_BYTE_MODE
    device.mpa_tx_cfg = MLAN_INIT_PARA_ENABLED;
#else
    device.mpa_tx_cfg = MLAN_INIT_PARA_DISABLED;
#endif
#endif
#ifdef SDIO_MULTI_PORT_RX_AGGR
#ifdef MMC_QUIRK_BLKSZ_FOR_BYTE_MODE
    device.mpa_rx_cfg = MLAN_INIT_PARA_ENABLED;
#else
    device.mpa_rx_cfg = MLAN_INIT_PARA_DISABLED;
#endif
#endif
	device.feature_control = FEATURE_CTRL_DEFAULT;
	device.feature_control = 0;
	device.rx_work = MFALSE;
    for (i = 0; i < handle->drv_mode.intf_num; i++) {
        device.bss_attr[i].bss_type = handle->drv_mode.bss_attr[i].bss_type;
        device.bss_attr[i].frame_type = handle->drv_mode.bss_attr[i].frame_type;
        device.bss_attr[i].active = handle->drv_mode.bss_attr[i].active;
        device.bss_attr[i].bss_priority =
            handle->drv_mode.bss_attr[i].bss_priority;
        device.bss_attr[i].bss_num = handle->drv_mode.bss_attr[i].bss_num;
		device.bss_attr[i].bss_virtual = handle->drv_mode.bss_attr[i].bss_virtual;
    }

    ewl_os_memcpy(&device.callbacks, &woal_callbacks, sizeof(mlan_callbacks));

    sdio_claim_host(((struct sdio_mmc_card *) handle->card)->func);
    if (MLAN_STATUS_SUCCESS == mlan_register(&device, &pmlan))
        handle->pmlan_adapter = pmlan;
    else
        ret = MLAN_STATUS_FAILURE;

    sdio_release_host(((struct sdio_mmc_card *) handle->card)->func);
    LEAVE();
    return ret;
}


/********************************************************
        Global Functions
********************************************************/
/** 
 *  @brief Convert ascii string to Hex integer
 *     
 *  @param d                    A pointer to integer buf
 *  @param s			A pointer to ascii string 
 *  @param dlen			The length of ascii string
 *
 *  @return 	   	        Number of integer  
 */
int
woal_ascii2hex(t_u8 * d, char *s, t_u32 dlen)
{
    unsigned int i;
    t_u8 n;

    ENTER();

    ewl_os_memset(d, 0x00, dlen);

    for (i = 0; i < dlen * 2; i++) {
        if ((s[i] >= 48) && (s[i] <= 57))
            n = s[i] - 48;
        else if ((s[i] >= 65) && (s[i] <= 70))
            n = s[i] - 55;
        else if ((s[i] >= 97) && (s[i] <= 102))
            n = s[i] - 87;
        else
            break;
        if (!(i % 2))
            n = n * 16;
        d[i / 2] += n;
    }

    LEAVE();
    return i;
}
/** 
 *  @brief Return hex value of a give character
 *
 *  @param chr      Character to be converted
 *
 *  @return         The converted character if chr is a valid hex, else 0
 */
int
woal_hexval(char chr)
{
    ENTER();

    if (chr >= '0' && chr <= '9')
        return chr - '0';
    if (chr >= 'A' && chr <= 'F')
        return chr - 'A' + 10;
    if (chr >= 'a' && chr <= 'f')
        return chr - 'a' + 10;

    LEAVE();
    return 0;
}

/**
 *  @brief Return hex value of a given ascii string
 *
 *  @param a        String to be converted to ascii
 *
 *  @return         The converted character if a is a valid hex, else 0
 */
int
woal_atox(char *a)
{
    int i = 0;
    
    ENTER();
    
    while (isxdigit(*a))
    i = i * 16 + woal_hexval(*a++);
    
    LEAVE();
    return i;
}


/**
 *  @brief Return integer value of a given ascii string
 *
 *  @param data    Converted data to be returned
 *  @param a       String to be converted
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_atoi(int *data, char *a)
{
    int i, val = 0, len;

    ENTER();

    len = ewl_os_strlen(a);
    if (!ewl_os_strncmp(a, "0x", 2)) {
        a = a + 2;
        len -= 2;
        *data = woal_atox(a);
        return MLAN_STATUS_SUCCESS;
    }
    for (i = 0; i < len; i++) {
        if (isdigit(a[i])) {
            val = val * 10 + (a[i] - '0');
        } else {
            PRINTM(MERROR, "Invalid char %c in string %s\n", a[i], a);
            return MLAN_STATUS_FAILURE;
        }
    }
    *data = val;

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}


/** 
 *  @brief Extension of strsep lib command. This function will also take care
 *     escape character
 *
 *  @param s         A pointer to array of chars to process
 *  @param delim     The delimiter character to end the string
 *  @param esc       The escape character to ignore for delimiter
 *
 *  @return          Pointer to the separated string if delim found, else NULL
 */
char *
woal_strsep(char **s, char delim, char esc)
{
    char *se = *s, *sb;
    
    ENTER();
    
    if (!(*s) || (*se == '\0')) {
        LEAVE();
        return NULL;
    }
    
    for (sb = *s; *sb != '\0'; ++sb) {
        
        if (*sb == esc && *(sb + 1) == esc) {
            /* 
            * We get a esc + esc seq then keep the one esc
            * and chop off the other esc character
            */
            ewl_os_memmove(sb, sb + 1, ewl_os_strlen(sb));
            continue;
        }
        
        if (*sb == esc && *(sb + 1) == delim) {
            /* 
            * We get a delim + esc seq then keep the delim
            * and chop off the esc character
            */
            ewl_os_memmove(sb, sb + 1, ewl_os_strlen(sb));
            continue;
        }
        
        if (*sb == delim)
            break;
    }
    
    if (*sb == '\0')
        sb = NULL;
    else
        *sb++ = '\0';
    
    *s = sb;
    
    LEAVE();
    return se;
}

/**
 *  @brief Convert mac address from string to t_u8 buffer.
 *
 *  @param mac_addr The buffer to store the mac address in.     
 *  @param buf      The source of mac address which is a string.        
 *
 *  @return         N/A
 */
void
woal_mac2u8(t_u8 * mac_addr, char *buf)
{
    char *begin = buf, *end;
    int i;
    
    ENTER();
    
    for (i = 0; i < ETH_ALEN; ++i) {
    end = woal_strsep(&begin, ':', '/');
    if (end)
      mac_addr[i] = woal_atox(end);
    }
    
    LEAVE();
}


void
woal_interrupt(moal_handle * handle)
{
    ENTER();
    handle->main_state = MOAL_RECV_INT;
    //PRINTM(MINTR, "*\n");
    if (handle->surprise_removed == MTRUE) {
        LEAVE();
        return;
    }
    
    /* call mlan_interrupt to read int status */
    mlan_interrupt(handle->pmlan_adapter);
    handle->main_state = MOAL_START_MAIN_PROCESS;
    /* Call MLAN main process */
    mlan_main_process(handle->pmlan_adapter);
    handle->main_state = MOAL_END_MAIN_PROCESS;
#ifdef FREE_RTOS_SUPPORT
    wifi_send_signal(0);
#endif

    LEAVE();
}


/** 
 *  @brief This function initializes firmware
 *  
 *  @param handle  A pointer to moal_handle structure
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 *  .
 *  .note:
 *  we alreay called woal_request_fw() --> woal_request_fw() 
 *                                --> woal_request_fw_dpc()
 *                                    -- woal_init_dpc() 
 *  even if we don't call these functions explicitly.
 *  And, To call woal_add_card_dpc() is substitued for 
 *    woal_add_interface() later.                                  
 **/
mlan_status
woal_init_fw(moal_handle * handle)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_fw_image fw;
 //   t_u32 revision_id = 0;
    
    ENTER();
    
    //the following is like woal_init_fw_dpc()    
    ewl_os_memset(&fw, 0, sizeof(mlan_fw_image));

    sdio_release_host(((struct sdio_mmc_card *) handle->card)->func);
    /*we read firmware. */
    fw.pfw_buf =(t_u8 *)wifi_firm;
    fw.fw_len = WIFI_FIRM_LEN;

    sdio_claim_host(((struct sdio_mmc_card *) handle->card)->func);
    ret = mlan_dnld_fw(handle->pmlan_adapter, &fw);
    sdio_release_host(((struct sdio_mmc_card *) handle->card)->func);

    if (ret == MLAN_STATUS_FAILURE)
      goto done;
    
    PRINTM(MMSG,"WLAN FW is active\n");

    handle->hardware_status = HardwareStatusFwReady;
    if (ret != MLAN_STATUS_SUCCESS)
      goto done;

    sdio_claim_host(((struct sdio_mmc_card *) handle->card)->func);
    ret = mlan_init_fw(handle->pmlan_adapter);
    sdio_release_host(((struct sdio_mmc_card *) handle->card)->func);

    if (ret == MLAN_STATUS_FAILURE) {
        goto done;
    } else if (ret == MLAN_STATUS_SUCCESS) {
        handle->hardware_status = HardwareStatusReady;
        goto done;
    }

    /* Wait for mlan_init to complete */
    while (handle->hardware_status != HardwareStatusReady)
    {
        if(sdio_irq)
        {
            sdio_irq = 0;
            sdio_fun_irq(); /*called when sdio data irq is occued */
        }
        mlan_main_process(handle->pmlan_adapter);
    }
    PRINTM(MMSG,"WLAN FIRMWARE INIT SUCCESS !!! \n");
    ret = MLAN_STATUS_SUCCESS;

done:
    if (ret != MLAN_STATUS_SUCCESS) ret = MLAN_STATUS_FAILURE;

    LEAVE();
    return ret;
}


/**
 *  @brief This function initializes the private structure 
 *          and dev structure for station mode
 *  
 *  @param dev     A pointer to net_device structure
 *  @param priv    A pointer to moal_private structure
 *
 *  @return        MLAN_STATUS_SUCCESS 
 */
#ifdef STA_SUPPORT
mlan_status
woal_init_sta_dev(moal_private * priv)
{
    ENTER();

    /* Initialize private structure */
    priv->current_key_index = 0;
    priv->rate_index = AUTO_RATE;
    priv->media_connected = MFALSE;
    priv->scan_block_flag = MFALSE;
    priv->is_adhoc_link_sensed = MFALSE;
    ewl_os_memset(&priv->current_addr, 0, sizeof(priv->current_addr));
    ewl_os_memset(&priv->nick_name, 0, sizeof(priv->nick_name));
    priv->num_tx_timeout = 0;
#ifdef EXT_WPA_SUPPLICANT    
    /*Initialize event_q */
    priv->event_q.first_item = NULL;
    priv->event_q.last_item = NULL;         
#endif    
    woal_request_get_fw_info(priv, MOAL_CMD_WAIT);
    priv->wireless_handler = &woal_handler_def;
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

/**
 *  @brief This function initializes the private structure 
 *  		and dev structure for uap mode
 *  
 *  @param priv    A pointer to moal_private structure
 *
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_init_uap_dev(moal_private * priv)
{
    priv->media_connected = MFALSE;
    ewl_os_memset(&priv->current_addr, 0, sizeof(priv->current_addr));
    woal_request_get_fw_info(priv, MOAL_CMD_WAIT);
    return MLAN_STATUS_SUCCESS;
}

/**
 * @brief This function adds a new interface. It will
 *      allocate, initialize and register the device.
 *      Here, 'moal_private priv' is allocated. 
 *  
 *  @param handle    A pointer to moal_handle structure
 *  @param bss_num   BSS number (0-7)
 *  @param bss_type  BSS type
 *
 *  @return          A pointer to the new priv structure
 */
moal_private *
woal_add_interface(moal_handle * handle, t_u8 bss_num, t_u8 bss_type)
{
    moal_private *priv = NULL;

    ENTER();
    
    /* Allocate an Ethernet device */
    if (!(priv = ewl_os_malloc(sizeof(moal_private)))) {
        PRINTM(MFATAL, "Init virtual ethernet device failed!\n");
        goto error;
    }

    /* Save the priv to handle */
    bss_num &= BSS_NUM_MASK;
    handle->priv[bss_type] = priv;

    /* Use the same handle structure */
    priv->phandle = handle;
    priv->bss_index = bss_type;
    priv->bss_type = bss_type;

    if (bss_type == MLAN_BSS_TYPE_STA)
        priv->bss_role = MLAN_BSS_ROLE_STA;
    else if (bss_type == MLAN_BSS_TYPE_UAP)
        priv->bss_role = MLAN_BSS_ROLE_UAP;

#ifdef STA_SUPPORT
    priv->scan_pending_on_block = MFALSE;
#endif

#ifdef STA_SUPPORT
    if (bss_type == MLAN_BSS_TYPE_STA)
        woal_init_sta_dev(priv);    
#endif

#ifdef UAP_SUPPORT
    if (bss_type == MLAN_BSS_TYPE_UAP) {
        if (MLAN_STATUS_SUCCESS != woal_init_uap_dev(priv))
        goto error;
    }
#endif

    /* initialize rx data queue */    
    priv->pqueue.first = -1;
    priv->pqueue.last  = -1;
    priv->pqueue.size  = 0;

    LEAVE();
    return priv;
error:
    LEAVE();
    return NULL;
}

/**
 * @brief This function adds the card. it will probe the
 *      card, allocate the mlan_private and initialize the device. 
 *  
 *  @param card    A pointer to card
 *
 *  @return        A pointer to moal_handle structure
 */
moal_handle *
woal_add_card(void *card)
{
    moal_handle *handle = NULL;
    int index = 0;
    
    ENTER();    
    
    /* Allocate buffer for moal_handle */
    if (!(handle = ewl_os_malloc(sizeof(moal_handle))) ){
        PRINTM(MERROR, "Allocate buffer for moal_handle failed!\n");
        goto err_handle;
    }
    
    /* Init moal_handle */
    ewl_os_memset(handle, 0, sizeof(moal_handle));
    handle->card = card;
  
    /* Save the handle */
    for (index = 0; index < MAX_MLAN_ADAPTER; index++) {
        if (m_handle[index] == NULL)
            break;
    }
    
    if (index < MAX_MLAN_ADAPTER) {
        m_handle[index] = handle;
    } else {
        PRINTM(MERROR, "Exceeded maximum cards supported!\n");
    }
  
    if (mac_addr) {
        t_u8 temp[20];
        t_u8 len =  (t_u8)ewl_os_strlen(mac_addr) + 1;
        if (len < sizeof(temp)) {
            ewl_os_memcpy(temp, mac_addr, len);
            handle->set_mac_addr = 1;
            /* note: the following function overwrites the temp buffer */
            woal_mac2u8(handle->mac_addr, temp);
        }
    }
    ((struct sdio_mmc_card *) card)->handle = handle;
    /* Init SW */
    if (MLAN_STATUS_SUCCESS != woal_init_sw(handle)) {
        PRINTM(MFATAL, "Software Init Failed\n");
        goto err_handle;
    }  

    /* Register the device. Fill up the private data structure with relevant
     information from the card and request for the required IRQ. */
    if (woal_register_dev(handle) != MLAN_STATUS_SUCCESS) {  
        PRINTM(MFATAL, "Failed to register wlan device!\n");
    }
    LEAVE();
    return handle;

err_handle:  
    LEAVE();
    return NULL;
}

/** 
 *  @brief This function return the point to structure moal_private 
 *  
 *  @param handle   Pointer to structure moal_handle
 *  @param bss_index    BSS index number
 *
 *  @return         moal_private pointer or NULL
 */
moal_private *
woal_bss_index_to_priv(moal_handle * handle, t_u8 bss_index)
{
    int i;

    ENTER();
    if (!handle) {
        LEAVE();
        return NULL;
    }
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        if (handle->priv[i] && (handle->priv[i]->bss_index == bss_index)) {
            LEAVE();
            return handle->priv[i];
        }
    }
    LEAVE();
    return NULL;
}

/** 
 *  @brief This function will fill in the mlan_buffer
 *  
 *  @param pmbuf   A pointer to mlan_buffer
 *  @param skb     A pointer to struct sk_buff 
 *
 *  @return        N/A
 */
 /* pmbuf & p->payload memory layout
 
    p->payload
    
    +----------------------+------------------------
    |MLAN_HEADER_SIZE(164) |ETH PACKET
    +----------------------+-----------------------
    |mlan_buffer|          |
    |           |          |
    |  mbuf->pbuf          |      
    |                  |
    |       |<-------->|peth    
    |   mpbuf->data_offset  
 pmbuf
 
 */
static void
woal_fill_mlan_buffer(mlan_buffer * pmbuf, struct pbuf* p)
{
    struct eth_hdr *peth = NULL;
    struct ip_hdr *iph;
    t_u8 *peth_data;
    t_u8 tid = 0;
    t_u8 priority =0;
    t_u32 ms = 0;

    ENTER();

    peth = (struct eth_hdr *)p->payload;
    peth_data = (t_u8*)p->payload+MLAN_HEADER_SIZE; //start of eth packet
    
    switch (htons(peth->type)) 
    {
        case ETHTYPE_IP:
            iph = (struct ip_hdr*)((t_u8*)peth+sizeof(struct eth_hdr));
            tid = IPH_TOS(iph);
            break;
        case ETHTYPE_ARP:
        default:
        	tid = 0;
            break;
    }
    
/** Offset for TOS field in the IP header */
#define IPTOS_OFFSET 5
    priority = tid = (tid >> IPTOS_OFFSET);
    pmbuf->pdesc = p;
    //pmbuf->buf_type = MLAN_BUF_TYPE_DATA;
    pmbuf->pbuf  = (t_u8*)p->payload + sizeof(mlan_buffer);
    pmbuf->data_offset = (t_u8*)peth_data - ((t_u8*)p->payload + sizeof(mlan_buffer));
    pmbuf->data_len = p->tot_len-MLAN_HEADER_SIZE;
    pmbuf->priority = priority;
    pmbuf->buf_type = 0;
    pbuf_header(p,(s16)(-sizeof(mlan_buffer)));
    ms     = ewl_os_timer_get_ms();
    pmbuf->in_ts_sec = ms/1000;
    pmbuf->in_ts_usec = (ms%1000)*1000;

    LEAVE();
      
    return;
}

/** 
 *  @brief This function handles packet transmission
 *  
 *  @param skb     A pointer to sk_buff structure
 *  @param dev     A pointer to net_device structure
 *
 *  @return        0 --success, otherwise fail
 */ 

int
woal_hard_start_xmit(struct pbuf* p, moal_private *priv)
{

    mlan_buffer *pmbuf = NULL;
    mlan_status status;
    moal_handle *phandle = priv->phandle;

    ENTER();

    if (!p->tot_len || (p->tot_len > ETH_FRAME_LEN+MLAN_HEADER_SIZE)) {
        PRINTM(MERROR, "Tx Error: Bad skb length %d : %d\n",
               p->tot_len, ETH_FRAME_LEN);
		printf("FREE: p(0x%08X) - %s - error\n", p, __func__);
        pbuf_free(p);
        goto done;
    }

    pmbuf = (mlan_buffer *)p->payload;
    memset((t_u8 *)pmbuf, 0, sizeof(mlan_buffer));
    pmbuf->bss_index = priv->bss_index;
    pmbuf->buf_type = MLAN_BUF_TYPE_DATA;
    woal_fill_mlan_buffer(pmbuf, p);
    status = mlan_send_packet(phandle->pmlan_adapter, pmbuf);

    /* Return value is all MLAN_STATUS_PEDGING  */
    switch (status) {
        case MLAN_STATUS_PENDING:
            mlan_main_process(m_handle[0]->pmlan_adapter);
            break;
        case MLAN_STATUS_SUCCESS:
            pbuf_free(p);
            break;
        case MLAN_STATUS_FAILURE:
        default:
			printf("%s Error: %d\n", __func__, status);
            PRINTM(MERROR,"%s : err %d \n",__func__,status);   
            pbuf_free(p);
            return -1;
            break;
    }
done:   
    LEAVE();
    return 0;
}

/** 
 *  @brief This function alloc mlan_buffer.
 *  @param handle  A pointer to moal_handle structure 
 *  @param size    buffer size to allocate
 *
 *  @return        mlan_buffer pointer or NULL
 */
pmlan_buffer
woal_alloc_mlan_buffer(moal_handle * handle, int size)
{
    mlan_buffer *pmbuf = NULL;
    struct pbuf *p;

    ewl_os_disable_int();
    ENTER();
    if (!(pmbuf = (mlan_buffer *)ewl_os_calloc(1,sizeof(mlan_buffer)))) {
        PRINTM(MERROR, "%s: Fail to alloc mlan buffer\n", __FUNCTION__);
        ewl_os_restore_int();
        return NULL;
    }

    if(!(p = pbuf_alloc(PBUF_RAW,size+PBUF_HEAD_ROOM,PBUF_RAM)))
    {
        PRINTM(MERROR, "%s: Fail to pbuf_alloc allocnt %d\n", __FUNCTION__,handle->mbufalloc_count);
        if(!(p = pbuf_alloc(PBUF_RAW,size+PBUF_HEAD_ROOM,PBUF_POOL)))
            ewl_os_free(pmbuf);
        ewl_os_restore_int();
        return NULL;
    }
    pbuf_header(p,-PBUF_HEAD_ROOM);
    pmbuf->pdesc = p;
    pmbuf->data_offset += PBUF_HEAD_ROOM;
    pmbuf->pbuf  = p->payload;
    handle->mbufalloc_count++;
    ewl_os_restore_int();   
    LEAVE();
    return pmbuf;
}


/** 
 *  @brief This function frees mlan_buffer.
 *  @param handle  A pointer to moal_handle structure 
 *  @param pmbuf   Pointer to mlan_buffer
 *
 *  @return        N/A
 */
void
woal_free_mlan_buffer(moal_handle * handle, pmlan_buffer pmbuf)
{

    ENTER();
    struct pbuf *p;
    if (!pmbuf)
        return;

    if (pmbuf->pdesc)
    {
        p = pmbuf->pdesc;
//    	printf("FREE: p(0x%08X) - %s\n", p, __func__);
        pbuf_free(p);
    }
    ewl_os_free(pmbuf);
    handle->mbufalloc_count--;

    LEAVE();
    return;
}

/** 
 *  @brief This function alloc mlan_ioctl_req.
 *
 *  @param size    buffer size to allocate
 *
 *  @return        mlan_ioctl_req pointer or NULL
 */
pmlan_ioctl_req
woal_alloc_mlan_ioctl_req(int size)
{
    mlan_ioctl_req *req = NULL;
    int mem_size = sizeof(mlan_ioctl_req) + size + sizeof(int) + sizeof(wait_queue);

    ENTER();

    if (!(req =(mlan_ioctl_req *)ewl_os_malloc(mem_size))) { 
        PRINTM(MERROR, "%s: Fail to alloc ioctl buffer\n", __FUNCTION__);
        LEAVE();
        return NULL;
    }
    ewl_os_memset(req,0,mem_size);
    req->pbuf = (t_u8 *) req + sizeof(mlan_ioctl_req);
    req->buf_len = (t_u32) size;
    req->reserved_1 =
        ALIGN_ADDR((t_u8 *) req + sizeof(mlan_ioctl_req) + size, sizeof(int));

    LEAVE();
    return req;
}
