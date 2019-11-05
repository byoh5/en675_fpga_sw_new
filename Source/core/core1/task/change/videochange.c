//*************************************************************************************************
// VideoChange task
//*************************************************************************************************
#include "dev.h"

#include "enx_freertos.h"

#include "videochange.h"

UINT unVideoChangeFlag = 0;

void IsrVideo(void *ctx)
{
#if 0
	gtUser.uviJpeg.sfFPS = 0;
	if(gptMsgShare.VIDEO_FPS == 60 || gptMsgShare.VIDEO_FPS == 50)
	{
		if(gtUser.uviH264[0].eResolution == e_res1920x1080)
		{
			gtUser.uviH264[0].sfFPS = 1;
			gtUser.uviH264[1].sfFPS = 1;
		}
		else
		{
			gtUser.uviH264[0].sfFPS = 0;
			gtUser.uviH264[1].sfFPS = 0;
		}
	}
	else
	{
		gtUser.uviH264[0].sfFPS = 0;
		gtUser.uviH264[1].sfFPS = 0;
	}

	timer_sched_timeout_cb(10, TIMEOUT_ONESHOT, (void (*)(void *))UserSave, (void *)&gtUser);
#endif

	UNUSED(ctx);
}

void VideoChangeTask(void *pvParameters)
{
	printf("Call %s: 0x%08X\n", __func__, unVideoChangeFlag);
#if 0
	if (unVideoChangeFlag != 0) {
		while(MsgLock() == ENX_FAIL)	vTaskDelay(1);

//		if(bitcheck(unVideoChangeFlag, UIN_STREAM_PORT))				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_STREAM_PORT, gtUser.nRtspPort) == DEF_FAIL)					vTaskDelay(1);
#ifdef __JPEG__
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_JPEG_SIZE))			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_SIZE, gtUser.uviJpeg.eResolution) == DEF_FAIL) 			vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_JPEG_QUALITY))			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_QUALITY, gtUser.uviJpeg.nQuality) == DEF_FAIL) 			vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_JPEG_FPS))				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_FPS, gtUser.uviJpeg.sfFPS) == DEF_FAIL) 					vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_JPEG_BRMODE))			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_BRMODE, gtUser.uviJpeg.eBRMode) == DEF_FAIL) 				vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_JPEG_BRATE))			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_BRATE, gtUser.uviJpeg.nBitRate) == DEF_FAIL) 				vTaskDelay(1);
#endif
#ifdef __H264__
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_1CH_PROFILE))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_PROFILE, gtUser.uviH264[0].eProfileMode) == DEF_FAIL)	vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_1CH_SIZE))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_SIZE, gtUser.uviH264[0].eResolution) == DEF_FAIL) 	vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_1CH_QUALITY))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_QUALITY, gtUser.uviH264[0].nQuality) == DEF_FAIL) 	vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_1CH_IDRFRAME))	while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_IDRFRAME, gtUser.uviH264[0].nIDRFrame) == DEF_FAIL) 	vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_1CH_FPS))			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_FPS, gtUser.uviH264[0].sfFPS) == DEF_FAIL)			vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_1CH_BRMODE))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_BRMODE, gtUser.uviH264[0].eBRMode) == DEF_FAIL)		vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_1CH_BRATE))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_BRATE, gtUser.uviH264[0].nBitRate) == DEF_FAIL)		vTaskDelay(1);

#ifdef __H264_2CH__
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_2CH_PROFILE))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_PROFILE, gtUser.uviH264[1].eProfileMode) == DEF_FAIL)	vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_2CH_SIZE))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_SIZE, gtUser.uviH264[1].eResolution) == DEF_FAIL)		vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_2CH_QUALITY))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_QUALITY, gtUser.uviH264[1].nQuality) == DEF_FAIL)		vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_2CH_IDRFRAME))	while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_IDRFRAME, gtUser.uviH264[1].nIDRFrame) == DEF_FAIL)	vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_2CH_FPS))			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_FPS, gtUser.uviH264[1].sfFPS) == DEF_FAIL)			vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_2CH_BRMODE))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_BRMODE, gtUser.uviH264[1].eBRMode) == DEF_FAIL)		vTaskDelay(1);
		if(bitcheck(unVideoChangeFlag, UIN_VIDEO_H264_2CH_BRATE))		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_BRATE, gtUser.uviH264[1].nBitRate) == DEF_FAIL)		vTaskDelay(1);
#endif
#endif

#ifdef __AUDIO__
//		if(bitcheck(unVideoChangeFlag, UIN_STREAM_AUDIO))				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_AUDIO, gtUser.bAudio) == DEF_FAIL)								vTaskDelay(1);
#endif

		//for RTSP ID/PW
//		if(bitcheck(unVideoChangeFlag, UIN_STREAM_USER_ID))				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_USER_ID, (UINT)gtUser.strUserId) == DEF_FAIL)					vTaskDelay(1);
//		if(bitcheck(unVideoChangeFlag, UIN_STREAM_USER_PW))				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_USER_PW, (UINT)gtUser.strUserPw) == DEF_FAIL)					vTaskDelay(1);

		unVideoChangeFlag = 0;

		MsgUnlock();
	}
#endif
	vTaskDelete(NULL);
	UNUSED(pvParameters);
}
