#include "dev.h"

#include "enx_freertos.h"

#include <string.h>									// for memory
#include <stdlib.h>									// for atoi, qsort

#if 0
#define ENTER() dprintf("%s IN\n", __func__);
#define LEAVE() dprintf("%s OUT\n", __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

#if (LOAD_FS_SDCARD==1)

#include "enx_file.h"

#include "muxer_videnc.h"
#include "videnc_info.h"
#include "videnc_heap.h"
#include "stream_buffering.h"

static void videnc_info_file_getname(void *v_enc_t)
{
	ENTER();

	t_videnc_t *enc_t = (t_videnc_t *)v_enc_t;
#if (VIDENC_STRING_FILENAME==1)
	struct tm tmtime;
	enx_get_tmtime(gptMsgShare.TIME, &tmtime, ENX_YES);
	sprintf(enc_t->now_name, enc_t->file_format, tmtime.tm_year+1900-2000, tmtime.tm_mon+1, tmtime.tm_mday, tmtime.tm_hour, tmtime.tm_min, tmtime.tm_sec);
#else
	++(enc_t->mLastFileNum);
	if (enc_t->mLastFileNum > MAX_FILE_NUM) {
		enc_t->mLastFileNum = 0;
	}
	sprintf(enc_t->now_name, enc_t->file_format, enc_t->mLastFileNum);
#endif

	LEAVE();
}

static int videnc_info_file_delete(void *v_enc_t, UINT nMaxDelete)
{
	ENTER();

	t_videnc_t *enc_t = (t_videnc_t *)v_enc_t;
	FRESULT fres;
	UINT cnt = 0;
	char strBuf[FF_MAX_LFN+1] = {0};
	char strDelBuf[FF_MAX_LFN+1] = {0};
	int nFreeSize;

	nFreeSize = fat_get_freesize(DEV_SD);
	while (nFreeSize < MIX_FREE_SD_4CENT) {
#if (VIDENC_STRING_FILENAME==1)
		if (encHeap_isEmpty(&enc_t->filenamelist)) {	// 파일리스트에 파일 없는데..? 이런 케이스는 발생하면 안된다.
			printf("%s(%d) : filenamelist empty / type(%d)\n", __func__, __LINE__, enc_t->type);
			break;
		}
		encHeap_deleteQueue(&enc_t->filenamelist, strBuf);
#else
		sprintf(strBuf, enc_t->file_format, enc_t->mFirstFileNum);

		++(enc_t->mFirstFileNum);
		if (enc_t->mFirstFileNum > MAX_FILE_NUM) {
			enc_t->mFirstFileNum = 0;
		}
#endif
		sprintf(strDelBuf, "%s/%s", enc_t->dir_name, strBuf);
		if ((fres=f_unlink(strDelBuf)) != FR_OK) {
			printf("f_unlink fail %s(%s)\n", strDelBuf, put_rc(fres));
		} else {
			printf("f_unlink ok %s\n", strDelBuf);
		}

		if (++cnt > nMaxDelete) {
			break;
		}

		nFreeSize = fat_get_freesize(DEV_SD);
	}

	LEAVE();
	return nFreeSize;
}

void videnc_info_sbuf_init(t_videnc_t *enc_t)
{
	ENTER();

	UINT video_frame_cnt = 0;
	UINT audio_frame_cnt = 0;
	UINT txts_frame_cnt = 0;
	UINT total_frame_cnt = 0;
	switch (enc_t->type) {
		case eRecNormal:
#if (FAT_SDSAVE_EVENT==1)
		case eRecEvent:
#endif
			video_frame_cnt = NUM_STREAM_VIDEO;
			audio_frame_cnt = NUM_STREAM_AUDIO;
			txts_frame_cnt = NUM_STREAM_TXTS;
			break;
		default:
			printf("%s(%d) : Type[%d] Error?\n", __func__, __LINE__, enc_t->type);
			video_frame_cnt = NUM_STREAM_VIDEO;
			audio_frame_cnt = NUM_STREAM_AUDIO;
			txts_frame_cnt = NUM_STREAM_TXTS;
			break;
	}

	total_frame_cnt = video_frame_cnt + audio_frame_cnt + txts_frame_cnt;
	if (sbuf_init(&enc_t->sbuf, total_frame_cnt) == ENX_FAIL) {
		printf("Type[%d] Video Total Sbuffer init Fail\n", enc_t->type);
	}

	LEAVE();
}

void videnc_info_sbuf_reset(t_videnc_t *enc_t)
{
	ENTER();

	portENTER_CRITICAL();

	enc_t->sbuf.head = enc_t->sbuf.tail = 0;
	if (enc_t->sbuf.data) {
		memset(enc_t->sbuf.data, 0, sizeof(sbuf_data) * enc_t->sbuf.tot_num);
	} else {
		printf("%s, sbuf.data NULL! Error!\n", __func__);
	}

	portEXIT_CRITICAL();

	LEAVE();
}

void videnc_info_default(t_videnc_t *enc_t, t_videnc_totalbuf_t *buffer)
{
	enc_t->state = VIDENC_IDLE;
	buffer->buf = buffer->base;
	buffer->length = 0;
}

ENX_OKFAIL videnc_info_init_item_malloc(UINT type, BYTE **data, UINT size)
{
	if (*data == NULL) {
		*data = (BYTE *)pvPortMalloc(size);
		if (*data == NULL) {
			printf("Type[%d] malloc fail(%dbyte)\n", type, size);
			return ENX_FAIL;
		}
	} else {
		*data = pvPortRealloc(*data, size);
		if (*data == NULL) {
			printf("Type[%d] realloc fail(%dbyte)\n", type, size);
			return ENX_FAIL;
		}
	}
	return ENX_OK;
}

void videnc_info_init_savetime(t_videnc_t *enc_t, t_videnc_totalbuf_t *buffer, UINT savetime, UINT video_fps, UINT audio_fps, UINT txts_fps)
{
	enc_t->vidinfo.un32maxtime = savetime;
	enc_t->vidinfo.un32vidsmax = savetime * video_fps;
	enc_t->vidinfo.un32totalmax = enc_t->vidinfo.un32vidsmax;

	enc_t->vidinfo.option = VID_VIDEO;

#if (FAT_SDSAVE_AUD==1)
	if (audio_fps) {
		enc_t->vidinfo.option |= VID_AUDIO;
		enc_t->vidinfo.un32audsmax = savetime * audio_fps;
		enc_t->vidinfo.un32totalmax += enc_t->vidinfo.un32audsmax;
	}
#endif

#if (VIDEO_SAVE_AVI==1)
#if (FAT_SDSAVE_TXT==1)
	if (txts_fps) {
		enc_t->vidinfo.option |= VID_TXTS;
		enc_t->vidinfo.un32txtsmax = savetime * txts_fps;
		enc_t->vidinfo.un32totalmax += enc_t->vidinfo.un32txtsmax;
	}
#endif
#endif
	// total값보다 10% 더 많이 index buffer을 할당해준다.
	// 이것은 Video frame의 기준으로 녹화를 하고 있으므로, 
	// Video frame이 아닌 다른 frame이 더 많을 때 발생할 오버플로우를 
	// 방지하기 위해 index buffer을 추가 할당한다.
	enc_t->vidinfo.un32totalmax += (enc_t->vidinfo.un32totalmax / 10);

#if (VIDEO_SAVE_AVI==1)
	buffer->idx_len = 0;
	if (buffer->idx_size < enc_t->vidinfo.un32totalmax) {
		buffer->idx_size = enc_t->vidinfo.un32totalmax;
		if (buffer->idx_info == NULL) {
			buffer->idx_info = pvPortMalloc(16 * buffer->idx_size);
			if (buffer->idx_info == NULL) {
				printf("Type[%d] idx_info malloc fail(%dbyte)\n", enc_t->type, 16 * buffer->idx_size);
			}
		} else {
			buffer->idx_info = pvPortRealloc(buffer->idx_info, 16 * buffer->idx_size);
			if (buffer->idx_info == NULL) {
				printf("Type[%d] idx_info realloc fail(%dbyte)\n", enc_t->type, 16 * buffer->idx_size);
			}
		}
	}
#else
	buffer->hdr_len = 0;
	if (buffer->hdr_info == NULL) {
		buffer->hdr_size = 64 * 1024;
		videnc_info_init_item_malloc(enc_t->type, &(buffer->hdr_info), buffer->hdr_size);
	}

	MP4HeaderSTBL *Vstbl = &(buffer->Vstbl);
//	Vstbl->stsd_len = 0;
//	Vstbl->stts_len = 0;
//	Vstbl->stsc_len = 0;
	Vstbl->stss_len = 0;
	Vstbl->stsz_len = 0;
	Vstbl->stco_len = 0;
	if (Vstbl->size < enc_t->vidinfo.un32vidsmax) {
		Vstbl->size = enc_t->vidinfo.un32vidsmax;
		videnc_info_init_item_malloc(enc_t->type, (BYTE **)&(Vstbl->stss), Vstbl->size * sizeof(UINT));
		videnc_info_init_item_malloc(enc_t->type, (BYTE **)&(Vstbl->stsz), Vstbl->size * sizeof(UINT));
		videnc_info_init_item_malloc(enc_t->type, (BYTE **)&(Vstbl->stco), Vstbl->size * sizeof(UINT));
	}

	MP4HeaderSTBL *Astbl = &(buffer->Astbl);
//	Astbl->stsd_len = 0;
//	Astbl->stts_len = 0;
//	Astbl->stsc_len = 0;
	Astbl->stss_len = 0;
	Astbl->stsz_len = 0;
	Astbl->stco_len = 0;
	if (Astbl->size < enc_t->vidinfo.un32audsmax) {
		Astbl->size = enc_t->vidinfo.un32audsmax + (enc_t->vidinfo.un32audsmax / 2);
		videnc_info_init_item_malloc(enc_t->type, (BYTE **)&(Astbl->stss), Astbl->size * sizeof(UINT));
		videnc_info_init_item_malloc(enc_t->type, (BYTE **)&(Astbl->stsz), Astbl->size * sizeof(UINT));
		videnc_info_init_item_malloc(enc_t->type, (BYTE **)&(Astbl->stco), Astbl->size * sizeof(UINT));
	}
#endif

	UNUSED(audio_fps);
	UNUSED(txts_fps);
}

void videnc_info_deinit_savetime(t_videnc_totalbuf_t *buf_t)
{
#if (VIDEO_SAVE_AVI==1)
	buf_t->idx_size = 0;
	buf_t->idx_len = 0;
	if (buf_t->idx_info) {
		printf("buf_t->idx_info Free\n");
		vPortFree(buf_t->idx_info);
		buf_t->idx_info = NULL;
	}
#else
	buf_t->hdr_len = 0;
	buf_t->hdr_size = 0;
	if (buf_t->hdr_info) {
		printf("buf_t->hdr_info Free\n");
		vPortFree(buf_t->hdr_info);
		buf_t->hdr_info = NULL;
	}
	MP4HeaderSTBL *Vstbl = &(buf_t->Vstbl);
	Vstbl->size = 0;
//	Vstbl->stsd_len = 0;
//	Vstbl->stts_len = 0;
//	Vstbl->stsc_len = 0;
	Vstbl->stss_len = 0;
	Vstbl->stsz_len = 0;
	Vstbl->stco_len = 0;
	if (Vstbl->stss) {
		printf("Vstbl->stss Free\n");
		vPortFree(Vstbl->stss);
		Vstbl->stss = NULL;
	}
	if (Vstbl->stsz) {
		printf("Vstbl->stsz Free\n");
		vPortFree(Vstbl->stsz);
		Vstbl->stsz = NULL;
	}
	if (Vstbl->stco) {
		printf("Vstbl->stco Free\n");
		vPortFree(Vstbl->stco);
		Vstbl->stco = NULL;
	}

	MP4HeaderSTBL *Astbl = &(buf_t->Astbl);
	Astbl->size = 0;
//	Astbl->stsd_len = 0;
//	Astbl->stts_len = 0;
//	Astbl->stsc_len = 0;
	Astbl->stss_len = 0;
	Astbl->stsz_len = 0;
	Astbl->stco_len = 0;
	if (Astbl->stss) {
		printf("Astbl->stss Free\n");
		vPortFree(Astbl->stss);
		Astbl->stss = NULL;
	}
	if (Astbl->stsz) {
		printf("Astbl->stsz Free\n");
		vPortFree(Astbl->stsz);
		Astbl->stsz = NULL;
	}
	if (Astbl->stco) {
		printf("Astbl->stco Free\n");
		vPortFree(Astbl->stco);
		Astbl->stco = NULL;
	}
#endif
}

#if (VIDENC_STRING_FILENAME==1)
void videnc_info_init_filenamelist(t_videnc_t *enc_t)
{
	encHeap_resetQueue(&enc_t->filenamelist); // reset queue

	char strBuf[FF_MAX_LFN+1] = {0};
	FRESULT fres;
	FILINFO finfo;
	DIR dir;

	fres = f_opendir(&dir, enc_t->dir_name);
	for (;;) {
		fres = f_readdir(&dir, &finfo);
		if ((fres != FR_OK) || !finfo.fname[0]) { // file 없음
			fres = FR_OK;
			break;
		}

		if ((finfo.fattrib & AM_DIR) == 0) {
			// format : [XXX_YYMMDD_hhmmss.xxx(avi/mp4)] <- 21byte
			if (strlen(finfo.fname) == SAVE_FILENAME_LENS) {
				if(finfo.fsize == 0) { // file size zero / how? -> delete?
					sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.fname);
					if(f_unlink(strBuf) != FR_OK) {
						printf("f_unlink(zero size) fail %s\n", strBuf);
					} else {
						printf("f_unlink(zero size) ok %s\n", strBuf);
					}
				} else if (encHeap_addQueue(&enc_t->filenamelist, finfo.fname) == 1) { // addQueue하고 리턴이 1이라면 heap이 overflow 되어(삽입은 성공상태) 삭제할 file이 존재함
					sprintf(strBuf, "%s/%s", enc_t->dir_name, enc_t->filenamelist.delHData.filename);
					if (f_unlink(strBuf) != FR_OK) {
						printf("f_unlink(heap over) fail %s\n", strBuf);
					} else {
						printf("f_unlink(heap over) ok %s\n", strBuf);
					}
					memset(&enc_t->filenamelist.delHData, 0, sizeof(encHData));
				}
			} else {
				// 15.05.13 : long file name중 21byte가 아닌것은 존재할 수 없다. 해당 파일 삭제
				sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.fname);
				if(f_unlink(strBuf) != FR_OK) {
					printf("f_unlink(name format) fail %s\n", strBuf);
				} else {
					printf("f_unlink(name format) ok %s\n", strBuf);
				}
			}
		}
	}
	f_closedir(&dir);
}
#else
static int filenumber_compar(const void *a, const void *b)
{
	UINT wa = *(UINT *)a;
	UINT wb = *(UINT *)b;
	if(wa >= wb)	return 1;
	else			return -1;
}

void videnc_info_init_filenamelist(t_videnc_t *enc_t)
{
	char strBuf[FF_MAX_LFN+1] = {0};
	FRESULT fres;
	FILINFO finfo;
	DIR dir;
	char *fn, *pcP;
	UINT nNum, nMaxNum = 0, nMinNum = enc_t->file_max_count - 1;
	UINT nCount = 0;

	fres = f_opendir(&dir, enc_t->dir_name);
	for (;;) {
		fres = f_readdir(&dir, &finfo);
		if ((fres != FR_OK) || !finfo.fname[0]) { // file 없음
			fres = FR_OK;
			break;
		}
 
		if ((finfo.fattrib & AM_DIR) == 0) {
			// format : [XXX_YYMMDD_hhmmss.xxx(avi/mp4)] <- 21byte
			if (strlen(finfo.fname) == SAVE_FILENAME_LENS) {
				if (finfo.fsize == 0) { // file size zero / how? -> delete?
					sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.fname);
					if (f_unlink(strBuf) != FR_OK) {
						printf("f_unlink(zero size) fail %s\n", strBuf);
					} else {
						printf("f_unlink(zero size) ok %s\n", strBuf);
					}
				} else {
					fn = finfo.fname;
					pcP = strchr(fn, '.');	// Find first char of suffix
					if (pcP) {
						if (strcmp(pcP, SAVE_FNE) == 0) { // Suffix is corresponding ?
							fn[pcP-fn] = '\0';	// Add null character
							nNum = (UINT)atoi(strchr(fn, '_')+1);	// Ascii -> int
							if (nMaxNum <= nNum) { nMaxNum = nNum; }
							if (nMinNum >= nNum) { nMinNum = nNum; }
							nCount++;
						}
					} 
				}
			} else {
				// 15.05.13 : short file name중 12byte가 아닌것은 존재할 수 없다. 해당 파일 삭제
				sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.fname);
				if (f_unlink(strBuf) != FR_OK) {
					printf("f_unlink(name format) fail %s\n", strBuf);
				} else {
					printf("f_unlink(name format) ok %s\n", strBuf);
				}
			}
		}
	}
	fres = f_closedir(&dir);

	if (nMinNum == MAX_FILE_NUM) {
		nMinNum = 0;
	}

	printf("%s : min %d max %d cnt %d\n", enc_t->dir_name, nMinNum, nMaxNum, nCount);
	enc_t->mFirstFileNum = nMinNum;
	enc_t->mLastFileNum = nMaxNum;

	if (nMinNum == 0 && nMaxNum == MAX_FILE_NUM) { // 위의 조건이 참이라면 한바퀴 돈 상태로 nMaxNum가 Max인 경우가 아니다.
		UINT *nfilelist = (UINT *)pvPortMalloc(nCount * sizeof(UINT));
		UINT i, nGap = 0, nBigGap = 0, nGapMax = 0x00000000, nGpaMin = 0xFFFFFFFF;
		if (nfilelist == NULL) {
			printf("%s(%d) : malloc fail(%s)(%d)\n", __func__, __LINE__, enc_t->dir_name, nCount);
			return;
		}

		fres = f_opendir(&dir, enc_t->dir_name);
		for (i = 0; ; ) {
			fres = f_readdir(&dir, &finfo);
			if ((fres != FR_OK) || !finfo.fname[0]) {
				break;
			}

			if ((finfo.fattrib & AM_DIR) == 0) {
				if (strlen(finfo.fname) == SAVE_FILENAME_LENS) { // format : [XXX_YYMMDD_hhmmss.xxx(avi/mp4)] <- 21byte
					fn = finfo.fname;
					pcP = strchr(fn, '.'); // Find first char of suffix
					if (pcP) {
						if (strcmp(pcP, SAVE_FNE) == 0) { // Suffix is corresponding ?
							fn[pcP-fn] = '\0'; // Add null character
							nNum = (UINT)atoi(strchr(fn, '_')+1); // Ascii -> int
							nfilelist[i] = nNum;
							i++;
						}
					} 
				}
			}
		}
		fres = f_closedir(&dir);
		qsort((char *)nfilelist, nCount, sizeof(UINT), filenumber_compar);
		for (i = 0; i < nCount - 1; i++) {
			nGap = nfilelist[i+1] - nfilelist[i];
			if (nGap > nBigGap) {
				nGapMax = nfilelist[i];
				nGpaMin = nfilelist[i+1];
				nBigGap = nGap;
			}
		}
		printf("%s : min %d max %d cnt %d\n", enc_t->dir_name, nGpaMin, nGapMax, nCount);
		enc_t->mFirstFileNum = nGpaMin;
		enc_t->mLastFileNum = nGapMax;

		vPortFree(nfilelist);
	}
}
#endif

void videnc_info_deinit_filenamelist(t_videnc_t *enc_t)
{	
#if (VIDENC_STRING_FILENAME==1)
	encHeap_destroy(&enc_t->filenamelist);
#else
	UNUSED(enc_t);
#endif
}

void videnc_info_init(t_videnc_t *enc_t, int type, char *dir_name, char *file_format, int maxfilecount)
{
	ENTER();

	memset((char *)enc_t, 0, sizeof(t_videnc_t));

	enc_t->type = type;
	enc_t->state = VIDENC_IDLE;

	sprintf(enc_t->dir_name, "%d:/%s", DEV_SD, dir_name);
	strcpy(enc_t->file_format, file_format);
	enc_t->pf_sd_free = videnc_info_file_delete;
	enc_t->pf_get_file_name = videnc_info_file_getname;

	enc_t->file_max_count = maxfilecount;
#if (VIDENC_STRING_FILENAME==1)
	ENX_OKFAIL res = encHeap_create(&enc_t->filenamelist, enc_t->file_max_count);
	if (res == ENX_FAIL) {
		printf("Type[%d] videnc filenamelist init fail\n", enc_t->type);
	}
#endif

	LEAVE();
}

ENX_OKFAIL videnc_info_init_buffer(t_videnc_totalbuf_t *buf_t)
{
	buf_t->base = pvPortMalloc(VID_BUF_SIZE);
	if (buf_t->base == NULL) {
		printf("videnc buffer malloc fail(%dbyte)\n", VID_BUF_SIZE);
		return ENX_FAIL;
	}
	buf_t->end = buf_t->base + VID_BUF_SIZE;
	buf_t->buf = buf_t->base;
	buf_t->length = 0;
	buf_t->total = VID_BUF_SIZE;

	return ENX_OK;
}

void videnc_info_deinit(t_videnc_t *enc_t)
{
	ENTER();

	UNUSED(enc_t);

	LEAVE();
}

void videnc_info_deinit_buffer(t_videnc_totalbuf_t *buf_t)
{
	buf_t->buf = NULL;
	buf_t->end = NULL;
	buf_t->length = 0;
	buf_t->total = 0;
	if (buf_t->base) {
		vPortFree(buf_t->base);
		buf_t->base = NULL;
	}
}

/**
	@return 
	0    : fail
	else : success 
*/
int videnc_info_put(t_videnc_t *enc_t, u32 addr, u32 size, u32 idr, u32 type)
{
	return sbuf_put(&enc_t->sbuf, addr, size, idr, type);
}
#endif
