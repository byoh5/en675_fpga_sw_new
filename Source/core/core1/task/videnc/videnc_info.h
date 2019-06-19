#ifndef __VIDENC_INFO__H__
#define __VIDENC_INFO__H__

#define SAVE_FILENAME_LENS		21	// 17+1+3

#if (VIDEO_SAVE_AVI==1)
#define SAVE_FNE				".avi"
#else
#define SAVE_FNE				".mp4"
#endif

#define SAVE_NOR_DIR			SD_DIR_NOR
#define SAVE_NOR_NAME_FORMAT	"NOR_%02d%02d%02d_%02d%02d%02d" SAVE_FNE
#define SAVE_NOR_NAME_PATTERN	"NOR_*" SAVE_FNE
#define SAVE_NOR_TIME_MAX		30

#define SAVE_EVT_DIR			SD_DIR_EVT
#define SAVE_EVT_NAME_FORMAT	"EVT_%02d%02d%02d_%02d%02d%02d" SAVE_FNE
#define SAVE_EVT_NAME_PATTERN	"EVT_*" SAVE_FNE
#define SAVE_EVT_TIME_MAX		30

// About SAVE_NOR_TIME_MAX, SAVE_EVT_TIME_MAX
// You can adjust the time of the recording file. The default value is 30 seconds.
// It can be adjusted as much as the file system, disk, and memory allow.

void videnc_info_sbuf_init(t_videnc_t *enc_t);
void videnc_info_sbuf_reset(t_videnc_t *enc_t);

void videnc_info_default(t_videnc_t *enc_t, t_videnc_totalbuf_t *buffer);

void videnc_info_init_savetime(t_videnc_t *enc_t, t_videnc_totalbuf_t *buffer, UINT savetime, UINT video_fps, UINT audio_fps, UINT txts_fps);
void videnc_info_deinit_savetime(t_videnc_totalbuf_t *buf_t);

void videnc_info_init_filenamelist(t_videnc_t *enc_t);
void videnc_info_deinit_filenamelist(t_videnc_t *enc_t);

void videnc_info_init(t_videnc_t *enc_t, int type, char *dir_name, char *file_format, int maxfilecount);
void videnc_info_deinit(t_videnc_t *enc_t);

UINT videnc_info_init_buffer(t_videnc_totalbuf_t *buf_t);
void videnc_info_deinit_buffer(t_videnc_totalbuf_t *buf_t);

int videnc_info_put(t_videnc_t *enc_t, u32 addr, u32 size, u32 idr, u32 type);

#endif	// __VIDENC_INFO__H__
