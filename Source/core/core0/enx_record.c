//*************************************************************************************************
// Record
//*************************************************************************************************
#include "dev.h"

#if (LOAD_FS_SDCARD==1)
extern void muxer_videnc_Buffering(u32 addr, u32 size, u32 flag, u32 type);
extern void muxer_videnc_semgive(void);

msgq_record_t gptMsgRecInfo = {
	.tot_num = MSG_RECORD_NUM,
};

static int stmrecmsg_lock;

#define StmRecMsgLock()		if (stmrecmsg_lock) { while (stmrecmsg_lock); } stmrecmsg_lock = 1;
#define StmRecMsgUnlock()	stmrecmsg_lock = 0;

ENX_OKFAIL MsgRecPut(UINT addr, UINT size, UINT ts, UINT type)
{
	static time_t old_time = 0;
	static UINT drop_count = 0;

	if (gptMsgShare.TIME != old_time) {
		old_time = gptMsgShare.TIME;
		if (drop_count != 0) {
			printf("%u items for recordingare dropped.\n", drop_count);
			drop_count = 0;
		}
	}

	ENX_OKFAIL bRes = ENX_FAIL;
#if 1
	StmRecMsgLock();
#else
	while (StmRecMsgLock() == ENX_FAIL);
#endif
	if (cQueue_isfull(&gptMsgRecInfo) != ENX_OK) {
		stream_info *info = (stream_info *)&gptMsgRecInfo.info[gptMsgRecInfo.tail];
		info->addr = addr;
		info->size = size;
		info->ts = ts;
		info->type = type;
		num_loop(gptMsgRecInfo.tail, gptMsgRecInfo.tot_num);
		bRes = ENX_OK;
	} else {
		drop_count++;
	}
	StmRecMsgUnlock();
	return bRes;
}

ENX_OKFAIL MsgRecGet(stream_info *getInfo)
{
	UINT bRes = ENX_FAIL;

#if 1
	StmRecMsgLock();
#else
	while (StmRecMsgLock() == ENX_FAIL);
#endif
	if(cQueue_isempty(&gptMsgRecInfo) != ENX_OK) {
		stream_info *info = (stream_info *)&gptMsgRecInfo.info[gptMsgRecInfo.head];
		getInfo->addr = info->addr;
		getInfo->size = info->size;
		getInfo->ts = info->ts;
		getInfo->type = info->type;
		num_loop(gptMsgRecInfo.head, gptMsgRecInfo.tot_num);
		bRes = ENX_OK;
	}
	StmRecMsgUnlock();
	return bRes;
}

void IsrRecorddata(void *arg)
{
	stream_info info;
	while (MsgRecGet(&info) == ENX_OK) {
		switch (info.type) {
			case eSTREAMMSG_H264I:
			case eSTREAMMSG_H265I:
				muxer_videnc_Buffering(info.addr, info.size, 1, info.type);
				break;
			case eSTREAMMSG_H264P:
			case eSTREAMMSG_H265P:
#if (FAT_SDSAVE_AUD==1)
			case eSTREAMMSG_AUDIO:
#endif
#if (NUM_STREAM_TXTS==1)
			case eSTREAMMSG_TXTS:
#endif
				muxer_videnc_Buffering(info.addr, info.size, 0, info.type);
				break;
#if (FAT_SDSAVE_AUD!=1)
			case eSTREAMMSG_AUDIO:
#endif
#if (NUM_STREAM_TXTS!=1)
			case eSTREAMMSG_TXTS:
#endif
			case eSTREAMMSG_JPEG:
				break;
			default:
				printf("unknown r-type(%d)\n", info.type);
				break;
		}
	}

	muxer_videnc_semgive();
}
#else
void IsrRecorddata(void *arg)
{
	printf("%s(%d): TEST\n", __func__, __LINE__);
}
#endif
