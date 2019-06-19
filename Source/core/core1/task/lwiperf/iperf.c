//*************************************************************************************************
// Iperf task
//*************************************************************************************************
#include "dev.h"

#include "enx_freertos.h"
#include "enx_lwip.h"

#include "lwip/apps/lwiperf.h"
#include "iperf.h"

#if (NET_LWIPERF==1)
void lwiperf_report(void *arg, enum lwiperf_report_type report_type,
  const ip_addr_t* local_addr, u16_t local_port, const ip_addr_t* remote_addr, u16_t remote_port,
  u32_t bytes_transferred, u32_t ms_duration, u32_t bandwidth_kbitpsec)
{
	char modearrow[4] = {0};
	switch(report_type)
	{
		case LWIPERF_TCP_DONE_SERVER:
			printf("The server side test is done\n");
			sprintf(modearrow, "<-");
			break;
		case LWIPERF_TCP_DONE_CLIENT:
			printf("The client side test is done\n");
			sprintf(modearrow, "->");
			break;
		case LWIPERF_TCP_ABORTED_LOCAL:
			printf("Local error lead to test abort\n");
			break;
		case LWIPERF_TCP_ABORTED_LOCAL_DATAERROR:
			printf("Data check error lead to test abort\n");
			break;
		case LWIPERF_TCP_ABORTED_LOCAL_TXERROR:
			printf("Transmit error lead to test abort\n");
			break;
		case LWIPERF_TCP_ABORTED_REMOTE:
			printf("Remote side aborted the test\n");
			break;
	}

	es_printf("local_addr(%IP:%d) %s remote_addr(%IP:%d)\n", local_addr->addr, local_port, modearrow, remote_addr->addr, remote_port);
	printf("Interval     Transfer       Bandwidth\n");
	//printf("%4.1f sec   %5.1f MBytes   %5.1f Mbits/sec\n\n", ms_duration / 1000.0, bytes_transferred / (1024.0 * 1024.0), bandwidth_kbitpsec / 1024.0);
	printf("%u sec   %u MBytes   %5u Mbits/sec\n\n", ms_duration / 1000, bytes_transferred / (1024 * 1024) , bandwidth_kbitpsec / 1024);

	UNUSED(arg);
}

void lwiperf_init(void)
{
#if LWIP_TCPIP_CORE_LOCKING
	LOCK_TCPIP_CORE();
#else
	SYS_ARCH_DECL_PROTECT(lev);
	SYS_ARCH_PROTECT(lev);
#endif
	lwiperf_start_tcp_server_default(lwiperf_report, NULL);
#if LWIP_TCPIP_CORE_LOCKING
	UNLOCK_TCPIP_CORE();
#else
	SYS_ARCH_UNPROTECT(lev);
#endif
}
#endif
