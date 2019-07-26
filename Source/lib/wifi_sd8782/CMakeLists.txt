# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_wifi_sd8782 )

set(sd8782_HDRS
	config_ssid.h
	ewl.h
	ewl_pkt_header.h
)

set(sd8782_mlan_HDRS
	ewl_mlan/mlan.h
	ewl_mlan/mlan_11h.h
	ewl_mlan/mlan_11n.h
	ewl_mlan/mlan_11n_aggr.h
	ewl_mlan/mlan_11n_rxreorder.h
	ewl_mlan/mlan_decl.h
	ewl_mlan/mlan_fw.h
	ewl_mlan/mlan_ieee.h
	ewl_mlan/mlan_init.h
	ewl_mlan/mlan_ioctl.h
	ewl_mlan/mlan_join.h
	ewl_mlan/mlan_main.h
	ewl_mlan/mlan_meas.h
	ewl_mlan/mlan_sdio.h
	ewl_mlan/mlan_uap.h
	ewl_mlan/mlan_util.h
	ewl_mlan/mlan_wmm.h
)

set(sd8782_mlan_SRCS
	ewl_mlan/mlan_11d.c
	ewl_mlan/mlan_11h.c
	ewl_mlan/mlan_11n.c
	ewl_mlan/mlan_11n_aggr.c
	ewl_mlan/mlan_11n_rxreorder.c
	ewl_mlan/mlan_cfp.c
	ewl_mlan/mlan_cmdevt.c
	ewl_mlan/mlan_init.c
	ewl_mlan/mlan_join.c
	ewl_mlan/mlan_meas.c
	ewl_mlan/mlan_misc.c
	ewl_mlan/mlan_scan.c
	ewl_mlan/mlan_sdio.c
	ewl_mlan/mlan_shim.c
	ewl_mlan/mlan_sta_cmd.c
	ewl_mlan/mlan_sta_cmdresp.c
	ewl_mlan/mlan_sta_event.c
	ewl_mlan/mlan_sta_ioctl.c
	ewl_mlan/mlan_sta_rx.c
	ewl_mlan/mlan_sta_tx.c
	ewl_mlan/mlan_txrx.c
	ewl_mlan/mlan_uap_cmdevent.c
	ewl_mlan/mlan_uap_ioctl.c
	ewl_mlan/mlan_uap_txrx.c
	ewl_mlan/mlan_wmm.c
)

set(sd8782_os_HDRS
	ewl_os/80211d_domain.h
	ewl_os/ewl_os.h
	ewl_os/ewl_passphrase.h
	ewl_os/ewl_wext.h
	ewl_os/if.h
	ewl_os/iw_handler.h
	ewl_os/mlan_err.h
	ewl_os/moal_main.h
	ewl_os/moal_priv.h
	ewl_os/moal_sdio.h
	ewl_os/moal_shim.h
	ewl_os/moal_uap.h
	ewl_os/moal_uap_priv.h
	ewl_os/moal_util.h
	ewl_os/moal_wext.h
	ewl_os/uaputl.h
	ewl_os/wext.h
	ewl_os/wifi_firm.h
	ewl_os/wireless.h
)

set(sd8782_os_SRCS
	ewl_os/moal_main.c
	ewl_os/ewl_os.c
	ewl_os/moal_sdio_mmc.c
	ewl_os/moal_shim.c
	ewl_os/moal_priv.c
	ewl_os/moal_ioctl.c
	ewl_os/moal_wext.c
	ewl_os/moal_uap.c
	ewl_os/moal_uap_priv.c
	ewl_os/ewl.c
	ewl_os/ewlap.c
	ewl_os/ewl_wext.c
	ewl_os/ewl_passphrase.c
	ewl_os/uapcmd.c
	ewl_os/moal_uap_wext.c
)

set(sd8782_sdio_HDRS
	ewl_sdio/sdio.h
	ewl_sdio/sdio_func.h
)

set(sd8782_sdio_SRCS
	ewl_sdio/sdio.c
)

set(sd8782_wpa_supplicant_HDRS
	wpa_supplicant/common.h
	wpa_supplicant/config.h
	wpa_supplicant/defs.h
	wpa_supplicant/driver.h
	wpa_supplicant/driver_wext.h
	wpa_supplicant/eloop.h
	wpa_supplicant/includes.h
	wpa_supplicant/os.h
	wpa_supplicant/wpa.h
	wpa_supplicant/wpa_common.h
	wpa_supplicant/wpa_i.h
	wpa_supplicant/wpa_supplicant.h
	wpa_supplicant/wpa_supplicant_i.h
)

set(sd8782_wpa_supplicant_SRCS
	wpa_supplicant/common.c
	wpa_supplicant/config.c
	wpa_supplicant/driver_wext.c
	wpa_supplicant/drivers.c
	wpa_supplicant/eloop.c
	wpa_supplicant/events.c
	wpa_supplicant/ewl_supplicant.c
	wpa_supplicant/os_none.c
	wpa_supplicant/wpa.c
	wpa_supplicant/wpa_supplicant.c
)

set(sd8782_ALL_HDRS
	${sd8782_HDRS}
	${sd8782_mlan_HDRS}
	${sd8782_os_HDRS}
	${sd8782_sdio_HDRS}
	${sd8782_wpa_supplicant_HDRS}
)

set(sd8782_ALL_SRCS
	${sd8782_mlan_SRCS}
	${sd8782_os_SRCS}
	${sd8782_sdio_SRCS}
	${sd8782_wpa_supplicant_SRCS}
)

ADD_DEFINITIONS( -DDEBUG_LEVEL1 )
#ADD_DEFINITIONS( -DDEBUG_LEVEL2 )
ADD_DEFINITIONS( -DSTA_SUPPORT )
#ADD_DEFINITIONS( -DBIG_ENDIAN_SUPPORT )
ADD_DEFINITIONS( -DSDIO_MULTI_PORT_TX_AGGR )
ADD_DEFINITIONS( -DSDIO_MULTI_PORT_RX_AGGR )
ADD_DEFINITIONS( -DCONFIG_CTRL_IFACE )
ADD_DEFINITIONS( -DUAP_SUPPORT )
ADD_DEFINITIONS( -DDEBUG_MLAN )
ADD_DEFINITIONS( -DEXT_WPA_SUPPLICANT )
ADD_DEFINITIONS( -DMLAN_64BIT )

# 라이브러리 생성
ADD_LIBRARY ( wifisd8782 STATIC ${sd8782_ALL_SRCS} ${sd8782_ALL_HDRS} )
