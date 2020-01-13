#include "dev.h"																
#include "enx_freertos.h"
#include "enx_file.h"

#include <string.h> // for strcmp, memset, memstr,
#include <time.h>

#if 0
#define ENTER() dprintf("%s IN\n", __func__);
#define LEAVE() dprintf("%s OUT\n", __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

#if (LOAD_FS_SDCARD==1)
#include "sdio_cd.h"

#include "muxer_videnc.h"
#include "videnc_info.h"
#if (VIDEO_SAVE_AVI==1)
#include "fmt_avienc.h"
#else
#include "fmt_mp4enc.h"
#endif

//#define ENX_VIDSV_DEBUG			// printf 주석용

#ifdef ENX_VIDSV_DEBUG
	int printf(const char *fmt, ...);
	#define tprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define tprintf(fmt, args...) do {} while(0);
#endif

enum {
	H264_STREAM = 0,	// Video
	H265_STREAM = 1,	// Video
	G711_STREAM = 2,	// Audio
	TXTS_STREAM = 3,	// G-sensor, GPS, etc...
	IDX1_ARRAY = 9		// index
};

t_videnc_totalbuf_t gVidencBuffer;
t_videnc_t gVidenc[eRecEnd];
#if 0
= {
	{
		.type = eRecNormal,
		.recording = 0,
		.state = VIDENC_IDLE,
		.pf_sd_free = NULL,
		.pf_get_file_name = NULL,
	}
#if (FAT_SDSAVE_EVENT==1)
	,{
		.type = eRecEvent,
		.recording = 0,
		.state = VIDENC_IDLE,
		.pf_sd_free = NULL,
		.pf_get_file_name = NULL,
	}
#endif
};
#endif

const int nFPSvalue[][12] = {
	{60, 30, 20, 15, 12, 10, 6, 5, 4, 3, 2, 1}, 
	{50, 25, 10, 5, 2, 1},
	{30, 15, 10, 6, 5, 3, 2, 1},
	{25, 5, 1},
	{15, 5, 3, 1},
	{12, 6, 4, 3, 2, 1},
};

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

static SemaphoreHandle_t bufSem = NULL;
UINT bSD_FAT_mounted = 0;
UINT nVidenc_Mode_now = 0;
UINT nVidenc_Mode_old = 0;
int nVidenc_Fps_old = 0;

typedef enum {
	VID_OK,
	VID_FULL,
	VID_OPENFAIL,
	VID_WRITEFAIL,
	VID_WRITEFAIL_DISKFULL,
	VID_IDXBUF_OVF,
	VID_CODE_ERROR,
} VID_ERROR;

t_videnc_t *enc_ing;

extern int muxer_videnc_change_savetime(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer, int start);

/**
    @return 
        1 : mounted.
        0 : nothing.
*/
int muxer_videnc_sdCardMounted(void)
{
    return bSD_FAT_mounted;
}

void muxer_videnc_go(int var)
{
	gVidenc[var].recording = 1;	// 녹화 활성화
}

void muxer_videnc_stop(int var)
{
	gVidenc[var].recording = 0;	// 녹화 비활성화
}

int muxer_videnc_state(int var)
{
	return gVidenc[var].recording;
}

void muxer_videnc_all_stop(void)
{
	muxer_videnc_stop(eRecNormal);
#if (FAT_SDSAVE_EVENT==1)
	muxer_videnc_stop(eRecEvent);
#endif
#if (JPEG_SDSAVE==1)
	muxer_videnc_stop(eRecSnapshot);
#endif
	gptMsgShare.VIDEO_REC = 0;
}

t_videnc_t *muxsr_videnc_get_t_videnc_t(void)
{
	return enc_ing;
}

void muxer_videnc_set_vcodec(int RecType, eVideoChannel vs)
{
	gVidenc[RecType].vidinfo.v_type = vs;
}

void muxer_videnc_Buffering(u32 addr, u32 size, u32 flag, u32 type)
{
	if (size) {
		videnc_info_put(&gVidenc[eRecNormal], addr, size, flag, type);
#if (FAT_SDSAVE_EVENT==1)
		videnc_info_put(&gVidenc[eRecEvent], addr, size, flag, type);
#endif
	}
}

void muxer_videnc_error(t_videnc_t *pvid, VID_ERROR state)
{
	ENTER();

	if (pvid == NULL) {
		if (state == VID_CODE_ERROR) {
			pvid->state = VIDENC_EXIT;
			muxer_videnc_all_stop();
			muxer_videnc_deinit();
			gtUser.bSdVidSave = ENX_OFF;
			printf("%s(%d) : VIDENC CODE ERROR\n", __func__, __LINE__);
		} else {
			printf("%s(%d) : VIDENC ERROR(%d)\n", __func__, __LINE__, state);
		}
	} else {
		if (state == VID_IDXBUF_OVF) {
			pvid->state = VIDENC_CLOSE;
		} else {
			f_close(&pvid->fpvid);

#ifdef GPIO_RECORDING_LED
			GpioSetLo(GPIO_RECORDING_LED);
#endif

			if (state == VID_FULL) {
				pvid->state = VIDENC_IDLE;
			} else {
				pvid->state = VIDENC_EXIT;
				muxer_videnc_all_stop();
				muxer_videnc_deinit();
				gtUser.bSdVidSave = ENX_OFF;
				printf("%s(%d) : VIDENC ERROR\n", __func__, __LINE__);
			}
		}
	}

	LEAVE();
}

FRESULT muxer_videnc_bufferflush(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer)
{
	FRESULT fres;
	UINT writesize = 0;

#ifdef GPIO_RECORDING_LED
	if(GpioGetPin(GPIO_RECORDING_LED))	GpioSetLo(GPIO_RECORDING_LED);
	else								GpioSetHi(GPIO_RECORDING_LED);
#endif

	fres = f_write(&(pvid->fpvid), (uint8 *)buffer->base, buffer->length, &writesize);
	if (fres != FR_OK) {
		printf("%s Error : f_write File system fail(addr:0x%08X, size:%d)\n", __func__, (uint8 *)buffer->base, buffer->length);
		printf("Err Msg : %s\n", put_rc(fres));
		muxer_videnc_error(pvid, VID_WRITEFAIL);
		goto done;
	}

	if (buffer->length != writesize) {
		printf("%s Error : f_write All Write fail.(buff:%d)(write:%d)\n", __func__, buffer->length, writesize);
		if (fat_get_freesize(DEV_SD) == 0) {
			printf("%s : SD freesize zeroG\n", __func__);
			muxer_videnc_error(pvid, VID_WRITEFAIL_DISKFULL);
			goto done;
		}
	}

	if (writesize == 0) {
		if (fat_get_freesize(DEV_SD) == 0) {
			printf("%s : SD freesize zeroF\n", __func__);
			muxer_videnc_error(pvid, VID_WRITEFAIL_DISKFULL);
			goto done;
		}
	}

	buffer->buf = buffer->base;	// buffer의 처음으로 이동
	buffer->length -= writesize;		// buffer에 남은 데이터, 값이 0이여야 한다.
//	pvid->filesize += writesize;

	if (buffer->length != 0) {
		printf("%s Error : VID buffer no clear(%d)\n", __func__, buffer->length);
		buffer->length = 0;
	}

done:
	return fres;
}

// size : -1 : buffer write & buffer no add
// size : >0 : check buffer & buffer add
void muxer_videnc_bufferadd(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer, uint8 *data, int32 size)
{
	UINT left, wb;

	if (!data) {
		if (muxer_videnc_bufferflush(pvid, buffer) != FR_OK) {
			printf("%s: bufferflush fail1\n", __func__);
		}
		return;
	}

	while (size > 0) {
		left = buffer->total - buffer->length;

		if((UINT)size >= left)	wb = left;
		else					wb = size;

		//BDmaMemCpy_rtos_async(0, (BYTE *)buffer->buf, (BYTE *)data, wb);
		BDmaMemCpy_rtos_flush(0, (BYTE *)buffer->buf, (BYTE *)data, wb);
		buffer->length += wb;
		buffer->buf    += wb;
		pvid->filesize += wb;

		if (buffer->length == buffer->total) {
			if (muxer_videnc_bufferflush(pvid, buffer) != FR_OK) {
				printf("%s: bufferflush fail2(%dbyte)\n", __func__, pvid->filesize);
				break;
			}
		}

		data += wb;
		size -= wb;
	}

	return;
}

BYTE *muxer_videnc_makevidheader(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer)
{
	VideoResolution vr = gtUser.vcVideo[pvid->vidinfo.v_type].eResolution;
	switch (pvid->state) {
		case VIDENC_OPEN:
			pvid->vidinfo.f_fps = muxer_videnc_change_savetime(NULL, buffer, 1);
			pvid->vidinfo.i_width =	listResolution[vr].nWidth;
			pvid->vidinfo.i_height = listResolution[vr].nHeight;
			pvid->vidinfo.un32vidicount = 0;
			pvid->vidinfo.un32vidscount = 0;
			pvid->vidinfo.un32audscount = 0;
			pvid->vidinfo.un32txtscount = 0;
#if (VIDEO_SAVE_AVI==1)
			pvid->vidinfo.i_riff = 0;
			pvid->vidinfo.i_movi_end = 0;
			pvid->vidinfo.i_header_end = AVI_HEADSIZE - 4;
			pvid->vidinfo.option = (VID_VIDEO | VID_AUDIO | VID_TXTS);
			return avi_write_header(&(pvid->vidinfo));
#else
			pvid->vidinfo.mdat_size = 0;
//			buffer->Vstbl.stsd_len = 0;
//			buffer->Vstbl.stts_len = 0;
//			buffer->Vstbl.stsc_len = 0;
			buffer->Vstbl.stss_len = 0;
			buffer->Vstbl.stsz_len = 0;
			buffer->Vstbl.stco_len = 0;
//			buffer->Astbl.stsd_len = 0;
//			buffer->Astbl.stts_len = 0;
//			buffer->Astbl.stsc_len = 0;
			buffer->Astbl.stss_len = 0;
			buffer->Astbl.stsz_len = 0;
			buffer->Astbl.stco_len = 0;

			hwflush_dcache_range((ulong)buffer->hdr_info, buffer->hdr_size);
			BDmaMemSet_rtos(0, (BYTE *)buffer->hdr_info, 0, buffer->hdr_size);
			return mp4_write_header(&(pvid->vidinfo), (void *)buffer);
#endif
			break;
		case VIDENC_CLOSE:
		case VIDENC_EXIT:
			pvid->vidinfo.option = 0;
			if(pvid->vidinfo.un32vidscount != 0)	pvid->vidinfo.option |=  VID_VIDEO;
			if(pvid->vidinfo.un32audscount != 0)	pvid->vidinfo.option |=  VID_AUDIO;
			if(pvid->vidinfo.un32txtscount != 0)	pvid->vidinfo.option |=  VID_TXTS;
#if (VIDEO_SAVE_AVI==1)
			return avi_write_header(&(pvid->vidinfo));
#else
			return mp4_write_header(&(pvid->vidinfo), (void *)buffer);
#endif
		default:
			printf("%s Error Call(%d)\n", __func__, pvid->state);
			return NULL;
	}
}

int muxer_videnc_idle(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer);
int muxer_videnc_open(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer);
int muxer_videnc_frist_video(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer);
void muxer_videnc_write_process(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer, int type);
void muxer_videnc_write(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer);
void muxer_videnc_close(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer);

int muxer_videnc_idle(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer)
{
	// 초기화
	pvid->state = VIDENC_OPEN;
	return muxer_videnc_open(pvid, buffer);
}

int muxer_videnc_open(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer)
{
	ENTER();
	
	FRESULT fres;

	// Sd Free Size?
	if (pvid->pf_sd_free) {
		if (pvid->pf_sd_free(pvid, MAX_FILE_NUM) == 0) {
			printf("%s : SD freesize zeroA\n", __func__);
			muxer_videnc_error(pvid, VID_FULL);
			goto done;
		}
	}

	// File Name Get
	pvid->pf_get_file_name(pvid);

	// File Path Create
	char strBuf[FF_MAX_LFN+1] = {0};
	sprintf(strBuf, "%s/%s", pvid->dir_name, pvid->now_name);

	gptMsgDebug.MAKEVID_COUNT = 0;

	// Start Time Get
	pvid->stime = BenchTimeStart();
	ULONG stime_start = BenchTimeStart();

	// File Open
	if ((fres=f_open(&pvid->fpvid, strBuf, FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK) {
		printf("%s Error : f_open VID data File system fail : %s\n", __func__, strBuf);
		printf("Err Msg : %s\n", put_rc(fres));
		muxer_videnc_error(pvid, VID_OPENFAIL);
		goto done;
	} else {
#if (FAT_SDSAVE_FREE_ALLOC==1)
		FRESULT fexp = f_expand(&pvid->fpvid, FAT_SDSAVE_FREE_ALLOC_SIZE, 1);
		if (fexp != FR_OK) {
			printf("f_expand fail(%s)\n", put_rc(fexp));
			if (SdioCdDet() == 1) {
				printf("%s Error : No SD card.\n", __func__);
				muxer_videnc_error(pvid, VID_OPENFAIL);
				goto done;
			} else {
				printf("%s Error : Unknown case.\n", __func__);
				muxer_videnc_error(pvid, VID_OPENFAIL);
				goto done;
			}
		}
#endif
		UINT time_ms = BenchTimeStop(stime_start) / 1000;
		gptMsgDebug.MAKEVID_COUNT = 1;
		printf("f_open(%dms) : %s\n", time_ms, strBuf);
//		printf("fsize(%d) fp(0x%08X)\n", f_size(&pvid->fpvid), f_tell(&pvid->fpvid));
//		printf("clust(0x%08X) sect(0x%08X) sclust(0x%08X)\n", pvid->fpvid.clust, pvid->fpvid.sect, pvid->fpvid.obj.sclust);

		if (encHeap_addQueue(&pvid->filenamelist, pvid->now_name) == 1) {
			if (strcmp(pvid->now_name, pvid->filenamelist.delHData.filename) == 0) {
				// 방금 open한 파일이 삭제할 대상이 됨
				// Queue가 다른 사유로 full 상태가 되거나 입력된 file name이 가장 old 한 상태가 됨.
				// 해당 queue가 full 상태에서 시간정보가 잘못된 값이 입력될경우 발생할것으로 보임.
				printf("%s Error : f_open old file name??? : %s\n", __func__, pvid->now_name);
				muxer_videnc_error(pvid, VID_OPENFAIL);
				goto done;
			}

			sprintf(strBuf, "%s/%s", pvid->dir_name, pvid->filenamelist.delHData.filename);
			if(f_unlink(strBuf) != FR_OK) {
				printf("f_unlink fail %s\n", strBuf);
			} else {
				printf("f_unlink ok %s\n", strBuf);
			}
			memset(&pvid->filenamelist.delHData, 0, sizeof(encHData));
		}

		pvid->filesize = 0;
		buffer->buf = buffer->base;
		buffer->length = 0;
#if (VIDEO_SAVE_AVI==1)
		buffer->idx_len = 0;

		// Make AVI Header, Buffer Write AVI Header, Header Size Check
		BYTE *aviheader = muxer_videnc_makevidheader(pvid, buffer);
		muxer_videnc_bufferadd(pvid, buffer, aviheader, AVI_HEADSIZE);
#else
		// Make MP4 Header, Buffer Write MP4 Header, Header Size Check
		BYTE *mp4header = muxer_videnc_makevidheader(pvid, buffer);
		muxer_videnc_bufferadd(pvid, buffer, mp4header, buffer->hdr_size);
#endif
		pvid->state = VIDENC_FIRST;
		nVidenc_Mode_old = nVidenc_Mode_now;
		nVidenc_Mode_now = pvid->type;
		return 1;
	}

done:
	LEAVE();
	return 0;
}

int muxer_videnc_frist_video(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer)
{
	if (sbuf_get(&(pvid->sbuf), &(pvid->datablock.addr), &(pvid->datablock.size), &(pvid->datablock.flags), &(pvid->datablock.type)) == ENX_OK) { // 데이터 존재확인
		if (pvid->datablock.flags == 1) { // I frame 이라면 초기화
			pvid->state = VIDENC_FIRST_NEXT;
			pvid->datablock.state = 1;	// datablock에 데이터 존재(video)
			return 1;
		}
	}
	return 0;
	UNUSED(buffer);
}

int muxer_videnc_nextI_video(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer)
{
	while (sbuf_get(&(pvid->sbuf), &(pvid->datablock.addr), &(pvid->datablock.size), &(pvid->datablock.flags), &(pvid->datablock.type)) == ENX_OK) { // 데이터 존재확인
		if (pvid->datablock.flags == 1) { // I frame 이라면 초기화
			pvid->state = VIDENC_WRITE;
			pvid->datablock.state = 1;	// datablock에 데이터 존재(video)
			muxer_videnc_write(pvid, buffer);
			return 1;
		}
	}
	return 0;
}

void muxer_videnc_write_process(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer, int type)
{
	ENTER();
#if (VIDEO_SAVE_AVI==1)
	// index add : 00dc flags pos size 
	int pos = pvid->filesize - pvid->vidinfo.i_header_end;
	AVIINDEX_t avi_idx = avi_write_index(type, pos, pvid->datablock.size, pvid->datablock.flags); // 16byte
	if (buffer->idx_info != NULL) {
		uint8 *idx_pos = (buffer->idx_info + buffer->idx_len);
		if (buffer->idx_len >= (pvid->vidinfo.un32totalmax * sizeof(AVIINDEX_t))) {
			printf("OVF info(0x%08X) pos(0x%08X) len(%d) size(%d) totmax(%d)\n", buffer->idx_info, idx_pos, buffer->idx_len, buffer->idx_size, pvid->vidinfo.un32totalmax);
			muxer_videnc_error(pvid, VID_IDXBUF_OVF);
			muxer_videnc_close(pvid, buffer);
			return;
		}
		DmaMemCpy_ip_async((BYTE *)idx_pos, (BYTE *)&avi_idx, sizeof(AVIINDEX_t));
		buffer->idx_len += sizeof(AVIINDEX_t);
	}

	// data add : 00dc size data
	FCC_CB_t fcccb = avi_write_chunkheader(type, pvid->datablock.size);			// 8byte
	muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&fcccb, sizeof(FCC_CB_t));
#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
	if (type == G711_STREAM) {
		if (pvid->datablock.size == 800) {
			BYTE output[800];
			portENTER_CRITICAL();
			EndianConv(output, (BYTE *)pvid->datablock.addr, pvid->datablock.size, 0);
			invalidate_dcache_range(output, output+pvid->datablock.size);
			portEXIT_CRITICAL();
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)output, VID_ALIGN(pvid->datablock.size));
		} else {
			flprintf("audio size error: %d\n", pvid->datablock.size);
		}
	} else
#endif
	{
		muxer_videnc_bufferadd(pvid, buffer, (uint8 *)pvid->datablock.addr, VID_ALIGN(pvid->datablock.size));
	}
#else
	if (type == H264_STREAM) {
		// H.264 NAL start Remake
		BYTE *k = (BYTE *)pvid->datablock.addr;
		BYTE nal_start[3] = {0x00, 0x00, 0x01};
		// I Frame
		if (pvid->datablock.flags) {
			// Search SPS/PPS/I nal start
			UINT sps_size = 0, pps_size = 0, ifr_size = pvid->datablock.size;
			BYTE *ifr_start = NULL;
			BYTE *pps_start = NULL;
			BYTE *sps_start = enx_memstr(k, 32, nal_start, 3);
			if (sps_start) {
				pps_start = enx_memstr(sps_start, 16, nal_start, 3);
				if (pps_start) {
					sps_size = pps_start - sps_start - 3;
					while (sps_start[sps_size-1] == 0x00) {
						sps_size--;
					}
					ifr_start = enx_memstr(pps_start, 16, nal_start, 3);
					if (ifr_start) {
						pps_size = ifr_start - pps_start - 3;
						while (pps_start[pps_size-1] == 0x00) {
							pps_size--;
						}
						ifr_size = pvid->datablock.size - (ifr_start - k);
					}
				}
			}

			UINT size_info = vidswapSB32(sps_size);
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&size_info, sizeof(size_info));
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)sps_start, sps_size);
			size_info = vidswapSB32(pps_size);
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&size_info, sizeof(size_info));
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)pps_start, pps_size);
			size_info = vidswapSB32(ifr_size);
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&size_info, sizeof(size_info));
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)ifr_start, ifr_size);

			/* Write Header _ STSS / STSZ / STCO __FrameDataSize And ChunkOffset */
			buffer->Vstbl.stss[buffer->Vstbl.stss_len++] = vidswapSB32(pvid->vidinfo.un32vidscount);
			buffer->Vstbl.stsz[buffer->Vstbl.stsz_len++] = vidswapSB32(sps_size + pps_size + ifr_size + (sizeof(size_info) * 3));
			buffer->Vstbl.stco[buffer->Vstbl.stco_len++] = vidswapSB32(pvid->vidinfo.mdat_size + buffer->hdr_size);
			pvid->vidinfo.mdat_size += (sps_size + pps_size + ifr_size + (sizeof(size_info) * 3));

			// MP4 Header SPS/PPS
			if (pvid->state == VIDENC_FIRST_NEXT) {
				UINT i;
				t_mp4_t *mp4info = &(pvid->vidinfo);
				mp4info->sps_size = sps_size;
				for (i = 0; i < mp4info->sps_size; i++) {
					mp4info->sps_data[i] = sps_start[i];
				}
				mp4info->pps_size = pps_size;
				for (i = 0; i < mp4info->pps_size; i++) {
					mp4info->pps_data[i] = pps_start[i];
				}
			}
		} else { // P Frame
			UINT pfr_size = pvid->datablock.size;
			BYTE *pfr_start = enx_memstr(k, 32, nal_start, 3);
			if (pfr_start) {
				pfr_size = pfr_size - (pfr_start - k);
			}

			UINT size_info = vidswapSB32(pfr_size);
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&size_info, sizeof(size_info));
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)pfr_start, pfr_size);

			/* Write Header _ STSZ / STCO __FrameDataSize And ChunkOffset */
			buffer->Vstbl.stsz[buffer->Vstbl.stsz_len++] = vidswapSB32(pfr_size + (sizeof(size_info) * 1));
			buffer->Vstbl.stco[buffer->Vstbl.stco_len++] = vidswapSB32(pvid->vidinfo.mdat_size + buffer->hdr_size);
			pvid->vidinfo.mdat_size += (pfr_size + (sizeof(size_info) * 1));
		}
	} else 	if (type == H265_STREAM) {
		// H.265 NAL start Remake
		BYTE *k = (BYTE *)pvid->datablock.addr;
		BYTE nal_start[3] = {0x00, 0x00, 0x01};
		// I Frame
		if (pvid->datablock.flags) {
			// Search VPS/SPS/PPS/I nal start
			UINT vps_size = 0, sps_size = 0, pps_size = 0, ifr_size = pvid->datablock.size;
			BYTE *ifr_start = NULL;
			BYTE *pps_start = NULL;
			BYTE *sps_start = NULL;
			BYTE *vps_start = enx_memstr(k, 32, nal_start, 3);
			if (vps_start) {
				sps_start = enx_memstr(vps_start, 64, nal_start, 3);
				if (sps_start) {
					vps_size = sps_start - vps_start - 3;
					while (vps_start[vps_size-1] == 0x00) {
						vps_size--;
					}
					pps_start = enx_memstr(sps_start, 128, nal_start, 3);
					if (pps_start) {
						sps_size = pps_start - sps_start - 3;
						while (sps_start[sps_size-1] == 0x00) {
							sps_size--;
						}
						ifr_start = enx_memstr(pps_start, 16, nal_start, 3);
						if (ifr_start) {
							pps_size = ifr_start - pps_start - 3;
							while (pps_start[pps_size-1] == 0x00) {
								pps_size--;
							}
							ifr_size = pvid->datablock.size - (ifr_start - k);
						}
					}
				}
			}

			UINT size_info = vidswapSB32(vps_size);
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&size_info, sizeof(size_info));
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)vps_start, vps_size);
			size_info = vidswapSB32(sps_size);
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&size_info, sizeof(size_info));
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)sps_start, sps_size);
			size_info = vidswapSB32(pps_size);
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&size_info, sizeof(size_info));
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)pps_start, pps_size);
			size_info = vidswapSB32(ifr_size);
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&size_info, sizeof(size_info));
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)ifr_start, ifr_size);

			/* Write Header _ STSS / STSZ / STCO __FrameDataSize And ChunkOffset */
			buffer->Vstbl.stss[buffer->Vstbl.stss_len++] = vidswapSB32(pvid->vidinfo.un32vidscount);
			buffer->Vstbl.stsz[buffer->Vstbl.stsz_len++] = vidswapSB32(vps_size + sps_size + pps_size + ifr_size + (sizeof(size_info) * 4));
			buffer->Vstbl.stco[buffer->Vstbl.stco_len++] = vidswapSB32(pvid->vidinfo.mdat_size + buffer->hdr_size);
			pvid->vidinfo.mdat_size += (vps_size + sps_size + pps_size + ifr_size + (sizeof(size_info) * 4));

			// MP4 Header SPS/PPS
			if (pvid->state == VIDENC_FIRST_NEXT) {
				UINT i;
				t_mp4_t *mp4info = &(pvid->vidinfo);
				mp4info->vps_size = vps_size;
				for (i = 0; i < mp4info->vps_size; i++) {
					mp4info->vps_data[i] = vps_start[i];
				}
				mp4info->sps_size = sps_size;
				for (i = 0; i < mp4info->sps_size; i++) {
					mp4info->sps_data[i] = sps_start[i];
				}
				mp4info->pps_size = pps_size;
				for (i = 0; i < mp4info->pps_size; i++) {
					mp4info->pps_data[i] = pps_start[i];
				}
			}
		} else { // P Frame
			UINT pfr_size = pvid->datablock.size;
			BYTE *pfr_start = enx_memstr(k, 32, nal_start, 3);
			if (pfr_start) {
				pfr_size = pfr_size - (pfr_start - k);
			}

			UINT size_info = vidswapSB32(pfr_size);
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&size_info, sizeof(size_info));
			muxer_videnc_bufferadd(pvid, buffer, (uint8 *)pfr_start, pfr_size);

			/* Write Header _ STSZ / STCO __FrameDataSize And ChunkOffset */
			buffer->Vstbl.stsz[buffer->Vstbl.stsz_len++] = vidswapSB32(pfr_size + (sizeof(size_info) * 1));
			buffer->Vstbl.stco[buffer->Vstbl.stco_len++] = vidswapSB32(pvid->vidinfo.mdat_size + buffer->hdr_size);
			pvid->vidinfo.mdat_size += (pfr_size + (sizeof(size_info) * 1));
		}
	} else if (type == G711_STREAM) {
		muxer_videnc_bufferadd(pvid, buffer, (uint8 *)pvid->datablock.addr, pvid->datablock.size);

		/* Write Header _ STSZ / STCO __FrameDataSize And ChunkOffset */
		buffer->Astbl.stsz_len++; // audio는 size정보를 넣지 않고 count만 한다. size는 고정형으로 사용함.
		buffer->Astbl.stco[buffer->Astbl.stco_len++] = vidswapSB32(pvid->vidinfo.mdat_size + buffer->hdr_size);
		pvid->vidinfo.mdat_size += pvid->datablock.size;
	}
#endif

	pvid->datablock.flags = 0;
	pvid->datablock.size = 0;
	pvid->datablock.addr = 0;
	pvid->datablock.type = 0;
	pvid->datablock.state = 0;

	LEAVE();
}

void muxer_videnc_write(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer)
{
	ENTER();

	if (pvid->datablock.state == 1) {
		int type = -1;
		switch (pvid->datablock.type) {
			case eSTREAMMSG_H264I:
				(pvid->vidinfo.un32vidicount)++; // @suppress("No break at end of case")
			case eSTREAMMSG_H264P:
				(pvid->vidinfo.un32vidscount)++;
				type = H264_STREAM;
				break;
			case eSTREAMMSG_H265I:
				(pvid->vidinfo.un32vidicount)++; // @suppress("No break at end of case")
			case eSTREAMMSG_H265P:
				(pvid->vidinfo.un32vidscount)++;
				type = H265_STREAM;
				break;
			case eSTREAMMSG_JPEG:
				break;
			case eSTREAMMSG_AUDIO:
				(pvid->vidinfo.un32audscount)++;
				type = G711_STREAM;
				break;
			case eSTREAMMSG_TXTS:
				(pvid->vidinfo.un32txtscount)++;
				type = TXTS_STREAM;
				break;
		}

		if (type != -1) {
			gptMsgDebug.MAKEVID_COUNT++;
			muxer_videnc_write_process(pvid, buffer, type);
		}
		pvid->datablock.state = 0;
	}

	LEAVE();
}

void muxer_videnc_close(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer)
{
	ENTER();

	FRESULT fres;

#if (VIDEO_SAVE_AVI==1)
	pvid->vidinfo.i_movi_end = pvid->filesize;

	// Write Idx Data
	if (buffer->idx_info != NULL) {
		FCC_CB_t fcccb = avi_write_chunkheader(IDX1_ARRAY, buffer->idx_len);
		muxer_videnc_bufferadd(pvid, buffer, (uint8 *)&fcccb, sizeof(FCC_CB_t));		// 8byte
		muxer_videnc_bufferadd(pvid, buffer, (uint8 *)buffer->idx_info, VID_ALIGN(buffer->idx_len));	// 16byte * framecount
		muxer_videnc_bufferadd(pvid, buffer, NULL, -1);	// buffer write(size:-1)
	}

	// Make AVI Header, Write AVI Header
	pvid->filesize = f_tell(&(pvid->fpvid));
	pvid->vidinfo.i_riff = pvid->filesize;
	f_size(&pvid->fpvid) = pvid->vidinfo.i_riff;
	BYTE *aviheader = muxer_videnc_makevidheader(pvid, buffer);
	muxer_videnc_bufferadd(pvid, buffer, aviheader, AVI_HEADSIZE);
#else
	muxer_videnc_bufferadd(pvid, buffer, NULL, -1);	// buffer write(size:-1)

	// Make MP4 Header, Write MP4 Header
	pvid->filesize = f_tell(&(pvid->fpvid));
	f_size(&pvid->fpvid) = buffer->hdr_size + pvid->vidinfo.mdat_size;
	BYTE *mp4header = muxer_videnc_makevidheader(pvid, buffer);
	muxer_videnc_bufferadd(pvid, buffer, mp4header, buffer->hdr_size);
#endif
	f_lseek(&pvid->fpvid, 0);	// fpvid pos : 0
	muxer_videnc_bufferadd(pvid, buffer, NULL, -1);

	// File Close
	gptMsgDebug.MAKEVID_COUNT = 0xffffffff;
	ULONG etime_s = BenchTimeStart();
	if ((fres=f_close(&pvid->fpvid)) != FR_OK) {
		printf("%s Error : f_close File system fail\n", __func__);
		printf("Err Msg : %s\n", put_rc(fres));
	} else { // End Time Get
		UINT tot_time = BenchTimeStop(pvid->stime) / 1000;
		UINT time_ms = BenchTimeStop(etime_s) / 1000;
#if (VIDEO_SAVE_AVI==1)
		printf("f_close(%dms) : %s : %dbyte/%dms/%dKBps\n", time_ms, pvid->now_name, pvid->vidinfo.i_riff, tot_time, (UINT)(pvid->vidinfo.i_riff/tot_time*8));
#else
		printf("f_close(%dms) : %s : %dbyte/%dms/%dKBps\n", time_ms, pvid->now_name, buffer->hdr_size + pvid->vidinfo.mdat_size, tot_time, (UINT)((buffer->hdr_size + pvid->vidinfo.mdat_size)/tot_time*8));
#endif
	}

#if (FAT_SDSAVE_EVENT==1)
	if (pvid->type == eRecEvent) {
		muxer_videnc_stop(eRecEvent);
		if (gtUser.bSdVidSave == ENX_ON) {
			muxer_videnc_go(eRecNormal);
		}
	}
#endif

#ifdef GPIO_RECORDING_LED
	GpioSetLo(GPIO_RECORDING_LED);
#endif

	pvid->state = VIDENC_IDLE;

//	flprintf("TEST Close\n");
//	muxer_videnc_stop(eRecNormal);
//	pvid->state = VIDENC_EXIT;
//	gtUser.bSdVidSave = ENX_OFF;

	enc_ing = NULL;

	LEAVE();
}

int muxer_videnc_change_savetime(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer, int start)
{
	int fpsval, fpsidx = 2;
#if 1
	if (pvid == NULL) {
		fpsval = 30;
	} else {
		fpsval = gtUser.vcVideo[pvid->vidinfo.v_type].nFps;
	}
#else
	switch (gptMsgShare.VIDEO_FPS) {
		case 60:	fpsidx = 0;		break;
		case 50:	fpsidx = 1;		break;
		case 30:	fpsidx = 2;		break;
		case 25:	fpsidx = 3;		break;
		case 15:	fpsidx = 4;		break;
		case 12:	fpsidx = 5;		break;
	}
	fpsval = nFPSvalue[fpsidx][gtUser.vcVideo[pvid->vidinfo.v_type].nFps];
#endif
	if (fpsval != nVidenc_Fps_old) {
		nVidenc_Fps_old = fpsval;
#if (FAT_SDSAVE_AUD==1)
#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
		UINT audio_fps = PCM_RecordingHz;
#else
		UINT audio_fps = G711_RecordingHz;
#endif
#else
		UINT audio_fps = 0;
#endif
		UINT txts_fps = 0;

		if (start == 0) {
			if (pvid == NULL) {
				muxer_videnc_error(NULL, VID_CODE_ERROR);
			} else {
				pvid->state = VIDENC_CLOSE;
				muxer_videnc_close(pvid, buffer);
			}
		}

		videnc_info_init_savetime(&gVidenc[eRecNormal], buffer, SAVE_NOR_TIME_MAX, fpsval, audio_fps, txts_fps);
#if (FAT_SDSAVE_EVENT==1)
		videnc_info_init_savetime(&gVidenc[eRecEvent], buffer, SAVE_EVT_TIME_MAX, fpsval, audio_fps, txts_fps);
#endif
	}
	return fpsval;
}

void muxer_videnc_process(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer)
{
	switch (pvid->state) {
		case VIDENC_IDLE: // => VIDENC_OPEN call => VIDENC_FIRST call
//			dprintf("VIDENC_IDLE call\n");
#if (FAT_SDSAVE_EVENT==1)
			if (muxer_videnc_state(eRecEvent) == 0) {
				if (gtUser.bSdVidSave == ENX_OFF) {
					muxer_videnc_all_stop();
					muxer_videnc_deinit();
					printf("%s(%d) : VIDENC EXIT\n", __func__, __LINE__);
					break;
				}
			}
#endif
			enc_ing = pvid;
			if(muxer_videnc_idle(pvid, buffer) == 0)
				break; // @suppress("No break at end of case")
		case VIDENC_FIRST: // first_video로 data align을 맞추고(1이 리턴)
			if(muxer_videnc_frist_video(pvid, buffer) == 0)
				break; // @suppress("No break at end of case")
		case VIDENC_FIRST_NEXT:
			if (nVidenc_Mode_now != nVidenc_Mode_old) { // 현재 인코딩 영상이 이전과 다른 것이라면 1s 버리기
				muxer_videnc_nextI_video(pvid, buffer);
				break;
			} else {
				// 현재 인코딩 영상이 이전과 같은 것이라면 녹화 시작
				muxer_videnc_write(pvid, buffer);
				pvid->state = VIDENC_WRITE;
			} // @suppress("No break at end of case")
		case VIDENC_WRITE: // => VIDENC_WRITE loop => VIDENC_CLOSE call
//			dprintf("VIDENC_WRITE call\n");
			if (gtUser.bSdVidSave == ENX_ON
#if (FAT_SDSAVE_EVENT==1)
					|| muxer_videnc_state(eRecEvent)
#endif
					) {
				gptMsgShare.VIDEO_REC = 1;
				enc_ing = pvid;

				muxer_videnc_change_savetime(pvid, buffer, 0);

				if ((pvid->state == VIDENC_IDLE) || (pvid->state == VIDENC_CLOSE) || (pvid->state == VIDENC_EXIT)) {
					break;
				} else if ((gtUser.bSdVidSave == ENX_OFF)
#if (FAT_SDSAVE_EVENT==1)
						&& (muxer_videnc_state(eRecEvent) == 0)
#endif
						) {
					pvid->state = VIDENC_EXIT;
					break;
				}

				while (sbuf_peek(&pvid->sbuf, &(pvid->datablock.addr), &(pvid->datablock.size), &(pvid->datablock.flags), &(pvid->datablock.type)) == ENX_OK) {
					if ((pvid->datablock.type == eSTREAMMSG_H264I || pvid->datablock.type == eSTREAMMSG_H265I) && (pvid->vidinfo.un32vidscount >= pvid->vidinfo.un32vidsmax)) {
						pvid->state = VIDENC_CLOSE;
						muxer_videnc_close(pvid, buffer);
					} else if (sbuf_get(&pvid->sbuf, &(pvid->datablock.addr), &(pvid->datablock.size), &(pvid->datablock.flags), &(pvid->datablock.type)) == ENX_OK) {
						pvid->datablock.state = 1;
						muxer_videnc_write(pvid, buffer);
					} else {
						break;
					}

					if ((pvid->state == VIDENC_IDLE) || (pvid->state == VIDENC_CLOSE) || (pvid->state == VIDENC_EXIT)) {
						break;
					} else if ((gtUser.bSdVidSave == ENX_OFF)
#if (FAT_SDSAVE_EVENT==1)
							&& (muxer_videnc_state(eRecEvent) == 0)
#endif
							) {
						pvid->state = VIDENC_EXIT;
						break;
					}
				}
			} else {
				pvid->state = VIDENC_EXIT;
			}
			break;
		case VIDENC_CLOSE:	// CLOSE 작업중이다.
			break;
		case VIDENC_EXIT:	// User Command(Shell or GPIO)
//			dprintf("VIDENC_EXIT call\n");
			muxer_videnc_close(pvid, buffer);
			muxer_videnc_all_stop();
			muxer_videnc_deinit();
			printf("%s(%d) : VIDENC EXIT\n", __func__, __LINE__);
			break;
		default:
			printf("%s Error Call\n", __func__);
			break;
	}
}

void muxer_videnc_semgive(void)
{
	if(bufSem == NULL) // not ready!
		return;

	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(bufSem, &xHigherPriorityTaskWoken);
	if (xHigherPriorityTaskWoken) {
		gbXsrTaskSwitchNeeded = 1;	// Task switch required?
	}
}

void muxer_videnc_init(void)
{
	ENTER();

	if (!SdioCdDet()) {
		ULONG ts_start = BenchTimeStart();

		// 일반 녹화
		videnc_info_default(&gVidenc[eRecNormal], &gVidencBuffer);
		videnc_info_init_filenamelist(&gVidenc[eRecNormal]);

		// 이벤트 녹화
#if (FAT_SDSAVE_EVENT==1)
		videnc_info_default(&gVidenc[eRecEvent], &gVidencBuffer);
		videnc_info_init_filenamelist(&gVidenc[eRecEvent]);
#endif

		muxer_videnc_change_savetime(NULL, &gVidencBuffer, 1);

		UINT time_us = BenchTimeStop(ts_start);
		flprintf("%dus\n", time_us);

		bSD_FAT_mounted = 1;
	}

	LEAVE();
}

void muxer_videnc_deinit(void)
{
	ENTER();

//	Nothing to do.

	LEAVE();
}

int muxer_videnc_sbuf_depth_sum(void)
{
	int a = 0, b = 0;
	a = (cQueue_isempty((&gVidenc[eRecNormal].sbuf)) ? ENX_FAIL : ENX_OK);
#if (FAT_SDSAVE_EVENT==1)
	b = (cQueue_isempty((&gVidenc[eRecEvent].sbuf)) ? ENX_FAIL : ENX_OK);
#endif
	return (a | b);
}

#if (JPEG_SDSAVE==1)
void muxer_jpegstill_process(void *ctx)
{
	FIL fp;
	char filename[128];
	UINT nByteWritten;

	while (gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_PROC) {
		WaitXus(10);
	}
	gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_STR;			// JPEG snapshot start
	gptMsgShare.JPEG_STILL_REF++;

	struct tm tmout;
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES, DEF_YES);
	sprintf(filename, "%d:/%s/%04d%02d%02d_%02d%02d%02d.jpg", DEV_SD, SD_DIR_SNAP, tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, tmout.tm_hour, tmout.tm_min, tmout.tm_sec);				// File name gen
	if (f_open(&fp, filename, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
		printf("Error : f_open File system fail\n");
	} else {
		BYTE* src_addr = (BYTE *)gptMsgShare.JPEG_STILL_ADDR;
		if (f_write(&fp, src_addr, gptMsgShare.JPEG_STILL_SIZE, &nByteWritten) != FR_OK) {
			printf("Error : f_write File system fail\n");
		}
		f_close(&fp);
		printf("Save: %s\n", filename);
	}

	gptMsgShare.JPEG_STILL_REF--;
	if (gptMsgShare.JPEG_STILL_REF == 0) {
		gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;			// JPEG snapshot idle state
	} else if (gptMsgShare.JPEG_STILL_REF < 0) {
		printf("%s(%d) : Error : JPEG_STILL_REF(%d)\n", __func__, __LINE__, gptMsgShare.JPEG_STILL_REF);
		gptMsgShare.JPEG_STILL_REF = 0;
	}

	UNUSED(ctx);
}
#endif


/**
	h.264 stream is buffered in ddr and written to sd card. 	
*/
void muxer_videnc(void)
{
	ENTER();

	if (bSD_FAT_mounted == 1) {
#if (FAT_SDSAVE_EVENT==1)
		if (muxer_videnc_state(eRecEvent)) { // 이벤트 녹화가 활성화 중이라면
			if (muxer_videnc_state(eRecNormal)) { // 일반 녹화는
				switch (gVidenc[eRecNormal].state) {
					case VIDENC_OPEN:
					case VIDENC_FIRST:
					case VIDENC_FIRST_NEXT:
					case VIDENC_WRITE:
						gVidenc[eRecNormal].state = VIDENC_CLOSE;
						muxer_videnc_close(&gVidenc[eRecNormal], &gVidencBuffer);	// 종료하고
						break;
					default:
						break;
				}
				muxer_videnc_stop(eRecNormal);	// 일반 녹화 중지
			}
			muxer_videnc_process(&gVidenc[eRecEvent], &gVidencBuffer); 	// 이벤트 녹화 진행
		}
		else 
#endif
		if (muxer_videnc_state(eRecNormal)) { // 일반 녹화 활성화 중이라면
			muxer_videnc_process(&gVidenc[eRecNormal], &gVidencBuffer); // 일반 녹화 진행
		}

#if (JPEG_SDSAVE==1)
		if (muxer_videnc_state(eRecSnapshot) == 1) {
			muxer_jpegstill_process(NULL);
			muxer_videnc_stop(eRecSnapshot);
		}
#endif
	}

	LEAVE();
}

/**
	Register irq and init sbuf for buffering.
	This function is called only once at boot time.
*/
void muxer_init_buffering(void)
{
	ENTER();

	videnc_info_init(&gVidenc[eRecNormal], eRecNormal, SAVE_NOR_DIR, SAVE_NOR_NAME_FORMAT, MAX_FILE_NUM + 1);
	videnc_info_sbuf_init(&gVidenc[eRecNormal]);

#if (FAT_SDSAVE_EVENT==1)
	videnc_info_init(&gVidenc[eRecEvent], eRecEvent, SAVE_EVT_DIR, SAVE_EVT_NAME_FORMAT, MAX_FILE_NUM + 1);
	videnc_info_sbuf_init(&gVidenc[eRecEvent]);
#endif

	videnc_info_init_buffer(&gVidencBuffer);

	bufSem = xSemaphoreCreateCounting(NUM_STREAM_TOTAL, 0);

	LEAVE();
}

void muxer_videnc_task(void* pvParameters)
{
	muxer_init_buffering();

	nVidenc_Mode_now = 0;
	nVidenc_Mode_old = -1;

	while (1) {
		if (xSemaphoreTake(bufSem, portMAX_DELAY) == pdTRUE) {
			if (muxer_videnc_sbuf_depth_sum()) {
				muxer_videnc();
			}
		}
	}

	UNUSED(pvParameters);
}
#endif
