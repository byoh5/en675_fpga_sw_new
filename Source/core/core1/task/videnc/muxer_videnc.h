#ifndef _SD_SAVE_H_
#define _SD_SAVE_H_

#if (VIDEO_SAVE_AVI==1)
#include "fmt_avienc.h"
#else
#include "fmt_mp4enc.h"
#endif
#include "stream_buffering.h"
#include "videnc_heap.h"

#define VID_BUF_SIZE	(512*1024)

#define VIDENC_STRING_FILENAME	1

#define VID_ALIGN_SIZE   	4
#define VID_ALIGN(x) 		(((x) + VID_ALIGN_SIZE - 1) & ~(VID_ALIGN_SIZE - 1))

#define VID_VIDEO		0x01
#define VID_AUDIO		0x02
#define VID_TXTS		0x04

enum {
	eRecNormal
#if (FAT_SDSAVE_EVENT==1)
	,eRecEvent
#endif
#if (JPEG_SDSAVE==1)
	,eRecSnapshot
#endif
	,eRecEnd
};

void muxer_videnc_init(void);
void muxer_videnc_deinit(void);
void muxer_videnc_h264Buffering(u32 h264_ptr, u32 h264_size, u32 idr);
void muxer_videnc_g711Buffering(u32 g711_ptr, u32 g711_size, u32 flags);
void muxer_videnc_txtsBuffering(u32 txts_ptr, u32 txts_size, u32 flags);
void muxer_videnc(void);
void muxer_videnc_task(void* pvParameters);

/**
	register irq and init sbuf for buffering
	this function should be called first before h264 working in cpu1 
*/
void muxer_init_buffering(void);

typedef enum {
	VIDENC_IDLE,
	VIDENC_OPEN,
	VIDENC_FIRST,
	VIDENC_FIRST_NEXT,
	VIDENC_WRITE,
	VIDENC_CLOSE,
	VIDENC_EXIT
} VIDENC_STATE;

typedef struct {
	uint32 flags; /*0 : i-frame, 1: p-frame */
	uint32 size;
	uint64 addr;
	uint32 type;
	uint32 state; // 0:data no, 1:data ok
} t_block_t;

typedef int (*sd_free_t) (void *enc_t, UINT nMaxDelete);
typedef void (*get_file_name_t) (void *enc_t);

typedef struct {
	int type;							// video save type(VID_PERIODIC_SW, VID_EVENT_SW)
	int recording;						// video rec?
	VIDENC_STATE state;					// videnc state

	char dir_name[80];					// file path
	char file_format[40];				// file name format
	FIL fpvid;							// video file
	char now_name[80];					// file name
	uint32 filesize;					// file size

	sbuf_t sbuf;						// H.264, G.711, etc. stream queue

	ULONG stime;						// start time of video encoding
#if (VIDENC_STRING_FILENAME==1)
	encHeap filenamelist;				// file path file name list
#else
	UINT mFirstFileNum;					// 
	UINT mLastFileNum;					// 
#endif
	UINT file_max_count;

#if (VIDEO_SAVE_AVI==1)
	t_avi_t vidinfo;					// H.264 FPS, Frame Size, FIle Size, etc...
#else
	t_mp4_t vidinfo;					// H.264 FPS, Frame Size, FIle Size, etc...
#endif
	t_block_t datablock;				// addr, size, flags

	// pointer to function
	sd_free_t       pf_sd_free;			// pointer to function to make space for SD CARD
	get_file_name_t pf_get_file_name;	// pointer to function to make name for video files
}t_videnc_t;

#if (VIDEO_SAVE_AVI==0)
typedef struct {
	UINT size;

//	UINT stsd_len;	// 137
//	BYTE stsd[140];
//	BYTE *stsd;

//	UINT stts_len;	// 24 ~ frame 변경시 8씩 증가
//	BYTE stts[32];
//	BYTE *stts;

//	UINT stsc_len;	// 28
//	BYTE stsc[32];
//	BYTE *stsc;

	UINT stss_len;	// IDR frame 개수, 만약 30sec/30fps에 IDR이 1이면 최대 900개
	UINT *stss;		// array

	UINT stsz_len;	// frame 개수 - size정보
	UINT *stsz;

	UINT stco_len;	// frame 개수 - offset정보
	UINT *stco;
} MP4HeaderSTBL;
#endif

typedef struct {
	// Write buffer
	uint8* buf;
	uint8* base;
	uint8* end;
	uint32 length;
	uint32 total;

	// VIDEO file index list(idx0)
#if (VIDEO_SAVE_AVI==1)
	UINT idx_size;
	UINT idx_len;						// length of idx_info
	UCHAR *idx_info;					// avi index buffer
#else
	UINT hdr_size;						// 
	UINT hdr_len;
	UCHAR *hdr_info;					// mp4 header buffer
	MP4HeaderSTBL Vstbl;
	MP4HeaderSTBL Astbl;
#endif
} t_videnc_totalbuf_t;

extern void muxer_videnc_go(int var);
extern void muxer_videnc_stop(int var);
extern int muxer_videnc_state(int var);
extern void muxer_videnc_all_stop(void);
extern t_videnc_t *muxsr_videnc_get_t_videnc_t(void);
extern void muxer_videnc_process(t_videnc_t *pvid, t_videnc_totalbuf_t *buffer);

extern void muxer_jpegstill_process(void *ctx);

extern t_videnc_t gVidenc[eRecEnd];

#endif
