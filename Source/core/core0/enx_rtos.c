#include "dev.h"

#include "enx_freertos.h"
#include "networkif.h"
#include "shell.h"
#include "ledblink.h"
#include "sdcard.h"
#include "iperf.h"
#include "chargen.h"
#if defined(__WIFI__)
#include "wifi_task.h"
#endif
#if USE_IR
#include "ir_recv.h"
#endif

#include "pmp.h"
#include "rtspd.h"
#include "audtxrx.h"
#include "muxer_videnc.h"

#include <stdio.h>

#if defined(__ETHERNET__)
void startNetProtocol(void *arg)
{
	// Wait for network activation.
	network_check_netif();

	chargen_init();

#if (NET_LWIPERF==1)
	lwiperf_init();
#endif
#if (NET_LWIPHTTP==1)
	extern void httpd_init(void);
	httpd_init();
#endif
#if (NET_SNTPC==1)
	if (gtNetwork.u1UseSntp == DEF_ON) {
		sntpc_start();
	}
#endif
#if (ENX_RTSP_use==1)
	vTaskCreate("rtspd", rtspd_socket_server, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
#endif

	vTaskDelete(NULL);
	UNUSED(arg);
}
#endif

#ifdef EN675_HEVC_TEST
void testapp(void)
{
	vTaskDelay(500);
	printf("testApp!!!!!!!!!\n");
	char *arg[2] = {"video", "start"};
	cmd_test_video(2, arg);
	vTaskDelete(NULL);
}
#endif


void main_os(void)
{
#ifdef __FREERTOS__
	vMemoryHeapInit();

	vTaskCreate("shell", ShellTask, NULL, LV2_STACK_SIZE, LV6_TASK_PRIO);

#ifdef __USE_SDIOCD__
	vTaskCreate("sdcard", SdcardTask, NULL, LV1_STACK_SIZE, LV3_TASK_PRIO);
	vTaskCreate("videnc", muxer_videnc_task, NULL, LV5_STACK_SIZE, LV5_TASK_PRIO);
#endif
#if defined(__NETWORK__) && (model_TgtBd != 1)
	vTaskCreate("startLwip", startLwip, NULL, LV3_STACK_SIZE, LV7_TASK_PRIO);
#endif
	vTaskCreate("ledblink", LedblinkTask, NULL, LV0_STACK_SIZE, LV7_TASK_PRIO);
#if defined(__NETWORK__) && (model_TgtBd != 1)
	vTaskCreate("netProt", startNetProtocol, NULL, LV2_STACK_SIZE, LV5_TASK_PRIO);
#endif
#if defined(__WIFI__) && (model_TgtBd != 1)
	vTaskCreate("Wifi", wifiTask, NULL, LV6_STACK_SIZE, LV7_TASK_PRIO);
#endif
#ifdef __AUDIO__
	vTaskCreate("audRxTx", audtxrx_task, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
#endif
#if USE_IR
	vTaskCreate("IrRx", ir_recv_task, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
#endif
#ifdef EN675_HEVC_TEST
	vTaskCreate("testapp", testapp, NULL, LV3_STACK_SIZE, LV5_TASK_PRIO);
#endif
	vTaskStartScheduler();
#endif
}
