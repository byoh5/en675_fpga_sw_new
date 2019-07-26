//*************************************************************************************************
// Stream
//*************************************************************************************************
#include "dev.h"

#if defined(__NETWORK__)
#if (ENX_RTSP_use==1)
#include "rtspd.h"
#else
#define rtp_queue_stream_video_put(a, b, c, d, e)
#define rtp_queue_stream_audio_put(a, b, c)
#endif
//extern void http_queue_stream_video_put(UINT addr, UINT size, UINT flags, rtsp_stream_type video_type, UINT ts);
#if 1
#if defined(__NETWORK__)
msgq_stream_t gptMsgStmInfo = {
	.tot_num = MSG_STREAM_NUM,
};
#endif
#if (LOAD_FS_SDCARD==1)
msgq_record_t gptMsgRecInfo = {
	.tot_num = MSG_RECORD_NUM,
};
#endif

static int stmrecmsg_lock;

#define StmRecMsgLock()		if (stmrecmsg_lock == 0) { stmrecmsg_lock = 1; } else { while (stmrecmsg_lock); }
#define StmRecMsgUnlock()	stmrecmsg_lock = 0;

ENX_OKFAIL MsgStmPut(UINT addr, UINT size, UINT ts, UINT type)
{
	static time_t old_time = 0;
	static UINT drop_count = 0;

	if (gptMsgShare.TIME != old_time) {
		old_time = gptMsgShare.TIME;
		if (drop_count != 0) {
			printf("%u items for streaming are dropped.\r\n", drop_count);
			drop_count = 0;
		}
	}

	ENX_OKFAIL bRes = ENX_FAIL;
#if 1
	StmRecMsgLock();
#else
	while (StmRecMsgLock() == ENX_FAIL);
#endif
	if (cQueue_isfull(&gptMsgStmInfo) != ENX_OK) {
		stream_info *info = (stream_info *)&gptMsgStmInfo.info[gptMsgStmInfo.tail];
		info->addr = addr;
		info->size = size;
		info->ts = ts;
		info->type = type;
		num_loop(gptMsgStmInfo.tail, gptMsgStmInfo.tot_num);
		bRes = ENX_OK;
	} else {
		drop_count++;
	}
	StmRecMsgUnlock();
	return bRes;
}

ENX_OKFAIL MsgStmGet(stream_info *getInfo)
{
	UINT bRes = ENX_FAIL;

#if 1
	StmRecMsgLock();
#else
	while (StmRecMsgLock() == ENX_FAIL);
#endif
	if(cQueue_isempty(&gptMsgStmInfo) != ENX_OK) {
		stream_info *info = (stream_info *)&gptMsgStmInfo.info[gptMsgStmInfo.head];
		getInfo->addr = info->addr;
		getInfo->size = info->size;
		getInfo->ts = info->ts;
		getInfo->type = info->type;
		num_loop(gptMsgStmInfo.head, gptMsgStmInfo.tot_num);
		bRes = ENX_OK;
	}
	StmRecMsgUnlock();
	return bRes;
}

void IsrStreamdata(void *arg)
{
	stream_info info;
	while (MsgStmGet(&info) == ENX_OK) {
		switch (info.type) {
			case eSTREAMMSG_H264I:
				rtp_queue_stream_video_put(info.addr, info.size, 1, ENX_RTSP_STREAM_LIVE_H264_1, info.ts);
//				http_queue_stream_video_put(info.addr, info.size, 1, ENX_RTSP_STREAM_LIVE_H264_1_1, info.ts);
				break;
			case eSTREAMMSG_H264P:
				rtp_queue_stream_video_put(info.addr, info.size, 0, ENX_RTSP_STREAM_LIVE_H264_1, info.ts);
//				http_queue_stream_video_put(info.addr, info.size, 0, ENX_RTSP_STREAM_LIVE_H264_1_1, info.ts);
				break;
			case eSTREAMMSG_H265I:
				rtp_queue_stream_video_put(info.addr, info.size, 1, ENX_RTSP_STREAM_LIVE_H265_1, info.ts);
//				http_queue_stream_video_put(info.addr, info.size, 1, ENX_RTSP_STREAM_LIVE_H264_1_1, info.ts);
				break;
			case eSTREAMMSG_H265P:
				rtp_queue_stream_video_put(info.addr, info.size, 0, ENX_RTSP_STREAM_LIVE_H265_1, info.ts);
//				http_queue_stream_video_put(info.addr, info.size, 0, ENX_RTSP_STREAM_LIVE_H264_1_1, info.ts);
				break;
			case eSTREAMMSG_JPEG:
#ifdef __JPEG__
#if (JPEG_STREAM==1)
				rtp_queue_stream_video_put(info.addr, info.size, 1, ENX_RTSP_STREAM_LIVE_JPEG, info.ts);
#endif
#endif
				break;
//			case eSTREAMMSG_JPEG_SW:
#if (SW_JPEG_ENCODER==1)
				rtp_queue_stream_video_put(info.addr, info.size, 1, ENX_RTSP_STREAM_LIVE_JPEG_SW, info.ts);
				break;
#endif
			case eSTREAMMSG_AUDIO:
#ifdef __AUDIO__
				rtp_queue_stream_audio_put(info.addr, info.size, info.ts);
#endif
				break;
			case eSTREAMMSG_TXTS:
				break;
			default:
				printf("unknown s-type(%d)\n", info.type);
				break;
		}
	}

	UNUSED(arg);
}
#else
void IsrStreamdata(void *arg)
{
	printf("%s(%d): TEST\n", __func__, __LINE__);
}
#endif
#endif
