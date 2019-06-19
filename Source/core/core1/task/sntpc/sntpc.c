//*************************************************************************************************
// SNTP client task
//*************************************************************************************************
// This is simple "SNTP" client for socket or raw API. It is a minimal implementation of SNTPv4 as specified in RFC 4330.
// For a list of some public NTP servers, see this link : http://support.ntp.org/bin/view/Servers/NTPPoolServers
// @todo:
// - set/change servers at runtime
// - complete SNTP_CHECK_RESPONSE checks 3 and 4
// - support broadcast/multicast mode?

#include "dev.h"

#if (NET_SNTPC==1)

#include "lwip/apps/sntp.h"
#include "networkif.h"

#include <time.h>

#ifdef __RTC_LOAD__
#include "rtc.h"
#endif

#include "sntpc.h"

//#define ENX_SNTPC_DEBUG

#ifdef ENX_SNTPC_DEBUG
#define ENX_SNTPC_DGB_COMMAND		DEF_OFF
#define ENX_SNTPC_DGB_PROCESSING	DEF_OFF
#define ENX_SNTPC_DGB_RESPONSE		DEF_OFF
#define ENX_SNTPC_DGB_DEBUG			DEF_OFF
#define ENX_SNTPC_DGB_ERROR			DEF_ON

char *strSNTPSTATE[] = {
		"SNTP_INIT",
		"SNTP_PROCESS",
		"SNTP_TIMESET",
		"SNTP_DELAY",
		"SNTP_FAIL"
};
#define tprintf(debug, state, fmt, args...) do { if((debug)&DEF_ON) printf("%04d %s : %s"fmt, __LINE__, __func__, strSNTPSTATE[state], ##args); } while(0);
#else
#define tprintf(debug, state, fmt, args...) do {} while(0);
#endif

sntp_state sntp_info;				// sntp state, info, etc
static sys_thread_t stt_sntp;		// task handle

//-------------------------------------------------------------------------------------------------
// Task
void sntp_settime(UINT t)
{
	struct tm tmout;

	// gtUser.nSystemTime이 0인 것은 한번도 저장안함
	if (gtUser.u32SystemTime == 0) {
		gtUser.u32SystemTime = t;
	}

	gptMsgShare.TIME = t;		// Change system time and/or the update the RTC clock

#ifdef __RTC_LOAD__
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
	rtc_set_time(&tmout);
#endif

	// 실 구현시 불필요 코드 / 콘솔 확인 기능
#ifdef ENX_DEBUG
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
	printf("SNTP : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
#endif

	sntp_info.last_gettime = t;
	sntp_info.is_timeset = 1;	// Watch set end
	sntp_info.retry_cnt = 0;
	sntp_info.state = eSNTP_TIMESET;
	tprintf(ENX_SNTPC_DGB_PROCESSING, sntp_info.state, "\r\n");
}

static void SntpcTask(void *pvParameters)
{
	sntp_info.last_gettime = 0;
	sntp_info.is_timeset = 0;
	sntp_info.retry_cnt = 0;
	sntp_info.state = eSNTP_INIT;
	tprintf(ENX_SNTPC_DGB_DEBUG, sntp_info.state, "\r\n");

	network_check_netif();

#if LWIP_TCPIP_CORE_LOCKING
	LOCK_TCPIP_CORE();
#else
	SYS_ARCH_DECL_PROTECT(lev);
	SYS_ARCH_PROTECT(lev);
#endif

	if (sntp_enabled() == 0) {
		sntp_setoperatingmode(SNTP_OPMODE_POLL);
	}

	sntp_info.state = eSNTP_PROCESS;
	tprintf(ENX_SNTPC_DGB_PROCESSING, sntp_info.state, "\r\n");

	sntp_init();

#if LWIP_TCPIP_CORE_LOCKING
	UNLOCK_TCPIP_CORE();
#else
	SYS_ARCH_UNPROTECT(lev);
#endif

	stt_sntp.thread_handle = NULL;
	vTaskDelete(NULL);
	UNUSED(pvParameters);
}

void sntpc_start(void)
{
	sntpc_stop();
	if (stt_sntp.thread_handle == NULL) {
		tprintf(ENX_SNTPC_DGB_DEBUG, sntp_info.state, " : Task Start\r\n");
		stt_sntp = sys_thread_new("sntpc", SntpcTask, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
	}
}

void sntpc_stop(void)
{
#if LWIP_TCPIP_CORE_LOCKING
	LOCK_TCPIP_CORE();
#else
	SYS_ARCH_DECL_PROTECT(lev);
	SYS_ARCH_PROTECT(lev);
#endif
	sntp_stop();
#if LWIP_TCPIP_CORE_LOCKING
	UNLOCK_TCPIP_CORE();
#else
	SYS_ARCH_UNPROTECT(lev);
#endif
	if (stt_sntp.thread_handle != NULL) {
		vTaskDelete(stt_sntp.thread_handle);
		stt_sntp.thread_handle = NULL;
	}
}
#endif
