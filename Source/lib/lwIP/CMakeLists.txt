# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_lwIP )

# The minimum set of files needed for lwIP.
set(lwipcore_SRCS
	core/init.c
	core/def.c
	core/dns.c
	core/inet_chksum.c
	core/ip.c
	core/mem.c
	core/memp.c
	core/netif.c
	core/pbuf.c
	core/raw.c
	core/stats.c
	core/sys.c
	core/altcp.c
	core/altcp_alloc.c
	core/altcp_tcp.c
	core/tcp.c
	core/tcp_in.c
	core/tcp_out.c
	core/timeouts.c
	core/udp.c
)
set(lwipcore_HDRS
	include/lwip/init.h
	include/lwip/def.h
	include/lwip/dns.h
	include/lwip/inet_chksum.h
	include/lwip/ip.h
	include/lwip/mem.h
	include/lwip/memp.h
	include/lwip/netif.h
	include/lwip/pbuf.h
	include/lwip/raw.h
	include/lwip/stats.h
	include/lwip/sys.h
	include/lwip/altcp.h
	include/lwip/altcp_tcp.h
	include/lwip/altcp_tls.h
	include/lwip/tcp.h
	include/lwip/tcpbase.h
	include/lwip/timeouts.h
	include/lwip/udp.h
	include/lwip/inet.h
	include/lwip/snmp.h
	include/lwip/arch.h
	include/lwip/debug.h
	include/lwip/err.h
	include/lwip/ip_addr.h
	include/lwip/opt.h
)
set(lwipcore4_SRCS
	core/ipv4/autoip.c
	core/ipv4/dhcp.c
	core/ipv4/etharp.c
	core/ipv4/icmp.c
	core/ipv4/igmp.c
	core/ipv4/ip4_frag.c
	core/ipv4/ip4.c
	core/ipv4/ip4_addr.c
)
set(lwipcore4_HDRS
	include/lwip/autoip.h
	include/lwip/dhcp.h
	include/lwip/etharp.h
	include/lwip/icmp.h
	include/lwip/igmp.h
	include/lwip/ip4_frag.h
	include/lwip/ip4.h
	include/lwip/ip4_addr.h
)
set(lwipcore6_SRCS
	core/ipv6/dhcp6.c
	core/ipv6/ethip6.c
	core/ipv6/icmp6.c
	core/ipv6/inet6.c
	core/ipv6/ip6.c
	core/ipv6/ip6_addr.c
	core/ipv6/ip6_frag.c
	core/ipv6/mld6.c
	core/ipv6/nd6.c
)
set(lwipcore6_HDRS
	include/lwip/dhcp6.h
	include/lwip/ethip6.h
	include/lwip/icmp6.h
	include/lwip/ip6.h
	include/lwip/ip6_addr.h
	include/lwip/ip6_frag.h
	include/lwip/ip6_zone.h
	include/lwip/mld6.h
	include/lwip/nd6.h
)

# APIFILES: The files which implement the sequential and socket APIs.
set(lwipapi_SRCS
	api/api_lib.c
	api/api_msg.c
	api/err.c
	api/if_api.c
	api/netbuf.c
	api/netdb.c
	api/netifapi.c
	api/sockets.c
	api/tcpip.c
)
set(lwipapi_HDRS
	include/lwip/api.h
	include/lwip/if_api.h
	include/lwip/netbuf.h
	include/lwip/netdb.h
	include/lwip/netifapi.h
	include/lwip/sockets.h
	include/lwip/tcpip.h
	include/lwip/errno.h
)

# Files implementing various generic network interface functions
set(lwipnetif_SRCS
	netif/ethernet.c
	netif/bridgeif.c
	netif/bridgeif_fdb.c
	netif/slipif.c
)
set(lwipnetif_HDRS
	include/netif/bridgeif.h
	include/netif/bridgeif_opts.h
	include/netif/etharp.h
	include/netif/ethernet.h
	include/netif/slipif.h
	include/lwip/sio.h
)

# 6LoWPAN
set(lwipsixlowpan_SRCS
	netif/lowpan6_common.c
	netif/lowpan6.c
	netif/lowpan6_ble.c
	netif/zepif.c
)
set(lwipsixlowpan_HDRS
	include/netif/lowpan6.h
	include/netif/lowpan6_ble.h
	include/netif/lowpan6_common.h
	include/netif/lowpan6_opts.h
	include/netif/ieee802154.h
	include/netif/zepif.h
)

# PPP
set(lwipppp_SRCS
	netif/ppp/auth.c
	netif/ppp/ccp.c
	netif/ppp/chap-md5.c
	netif/ppp/chap_ms.c
	netif/ppp/chap-new.c
	netif/ppp/demand.c
	netif/ppp/eap.c
	netif/ppp/ecp.c
	netif/ppp/eui64.c
	netif/ppp/fsm.c
	netif/ppp/ipcp.c
	netif/ppp/ipv6cp.c
	netif/ppp/lcp.c
	netif/ppp/magic.c
	netif/ppp/mppe.c
	netif/ppp/multilink.c
	netif/ppp/ppp.c
	netif/ppp/pppapi.c
	netif/ppp/pppcrypt.c
	netif/ppp/pppoe.c
	netif/ppp/pppol2tp.c
	netif/ppp/pppos.c
	netif/ppp/upap.c
	netif/ppp/utils.c
	netif/ppp/vj.c
	netif/ppp/polarssl/arc4.c
	netif/ppp/polarssl/des.c
	netif/ppp/polarssl/md4.c
	netif/ppp/polarssl/md5.c
	netif/ppp/polarssl/sha1.c
)
set(lwipppp_HDRS
	include/netif/ppp/ccp.h
	include/netif/ppp/chap_ms.h
	include/netif/ppp/chap-md5.h
	include/netif/ppp/chap-new.h
	include/netif/ppp/eap.h
	include/netif/ppp/ecp.h
	include/netif/ppp/eui64.h
	include/netif/ppp/fsm.h
	include/netif/ppp/ipcp.h
	include/netif/ppp/ipv6cp.h
	include/netif/ppp/lcp.h
	include/netif/ppp/magic.h
	include/netif/ppp/mppe.h
	include/netif/ppp/ppp.h
	include/netif/ppp/ppp_impl.h
	include/netif/ppp/ppp_opts.h
	include/netif/ppp/pppapi.h
	include/netif/ppp/pppcrypt.h
	include/netif/ppp/pppdebug.h
	include/netif/ppp/pppoe.h
	include/netif/ppp/pppol2tp.h
	include/netif/ppp/pppos.h
	include/netif/ppp/upap.h
	include/netif/ppp/vj.h
	include/netif/ppp/polarssl/arc4.h
	include/netif/ppp/polarssl/des.h
	include/netif/ppp/polarssl/md4.h
	include/netif/ppp/polarssl/md5.h
	include/netif/ppp/polarssl/sha1.h
)

# Port
set(lwipport_HDRS
	port/lwipopts.h
	port/networkif.h
	port/ethernetif.h
	port/wifiif.h
	port/include/arch/cc.h
	port/include/arch/perf.h
	port/include/arch/sys_arch.h
	port/dhcpd/debug.h
	port/dhcpd/dhcpd.h
	port/dhcpd/files.h
	port/dhcpd/leases.h
	port/dhcpd/options.h
	port/dhcpd/packet.h
	port/dhcpd/serverpacket.h
)
set(lwipport_SRCS
	port/sys_arch.c
	port/networkif.c
	port/ethernetif.c
	port/wifiif.c
	port/dhcpd/dhcpd.c
	port/dhcpd/files.c
	port/dhcpd/leases.c
	port/dhcpd/options.c
	port/dhcpd/serverpacket.c
)

# Apps
set(lwipapps_HDRS
	include/lwip/apps/altcp_proxyconnect.h
	include/lwip/apps/altcp_tls_mbedtls_opts.h
	include/lwip/apps/fs.h
	include/lwip/apps/http_client.h
	include/lwip/apps/httpd.h
	include/lwip/apps/httpd_opts.h
	include/lwip/apps/lwiperf.h
	include/lwip/apps/mdns.h
	include/lwip/apps/mdns_opts.h
	include/lwip/apps/mdns_priv.h
	include/lwip/apps/mqtt.h
	include/lwip/apps/mqtt_opts.h
	include/lwip/apps/mqtt_priv.h
	include/lwip/apps/netbiosns.h
	include/lwip/apps/netbiosns_opts.h
	include/lwip/apps/smtp.h
	include/lwip/apps/smtp_opts.h
	include/lwip/apps/snmp.h
	include/lwip/apps/snmp_core.h
	include/lwip/apps/snmp_mib2.h
	include/lwip/apps/snmp_opts.h
	include/lwip/apps/snmp_scalar.h
	include/lwip/apps/snmp_snmpv2_framework.h
	include/lwip/apps/snmp_snmpv2_usm.h
	include/lwip/apps/snmp_table.h
	include/lwip/apps/snmp_threadsync.h
	include/lwip/apps/snmpv3.h
	include/lwip/apps/sntp.h
	include/lwip/apps/sntp_opts.h
	include/lwip/apps/tftp_opts.h
	include/lwip/apps/tftp_server.h
)

SET ( LWIP_HDRS
	${lwipcore_HDRS}
	${lwipcore4_HDRS}
#	${lwipcore6_HDRS}
	${lwipapi_HDRS}
	${lwipnetif_HDRS}
#	${lwipsixlowpan_HDRS}
#	${lwipppp_HDRS}
	${lwipport_HDRS}
	${lwipapps_HDRS}
)

SET ( LWIP_SRCS
	${lwipcore_SRCS}
	${lwipcore4_SRCS}
#	${lwipcore6_SRCS}
	${lwipapi_SRCS}
	${lwipnetif_SRCS}
#	${lwipsixlowpan_SRCS}
#	${lwipppp_SRCS}
	${lwipport_SRCS}
)

# 라이브러리 생성
ADD_LIBRARY ( lwip STATIC ${LWIP_SRCS} ${LWIP_HDRS} )
