//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include <string.h> // strcmp
#include <stdlib.h> // atoi

#include "enx_freertos.h"
#include "enx_lwip.h"

#include "enx_stream.h"
#include "enx_record.h"

#include "muxer_videnc.h"

#include "shell_cmd_media.h"

const char *sTestVideoCmd[] = {"Test Video",                    (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
#define VID_MODEL 0
#if VID_MODEL==0
#include "vid_c_fhd_4m_hevc.h"
#define VID_CODEC	1
#define VID_SIZE	C_FHD_4M_HEVC_LEN
#define VID_HEADER	c_fhd_4m_hevc
#elif VID_MODEL==1
#include "vid_c_fhd_4m_mp.h"
#define VID_CODEC	0
#define VID_SIZE	C_FHD_4M_MP_LEN
#define VID_HEADER	c_fhd_4m_mp
#elif VID_MODEL==0
#include "vid_s_fhd_4m_mp.h"
#define VID_CODEC	0
#define VID_SIZE	S_FHD_4M_MP_LEN
#define VID_HEADER	s_fhd_4m_mp
#endif

#define TEST_VIDEO_TIMER_CH	8
#if 1 // 41.625MHz
#define TEST_VIDEO_TIMER_DIV	250
#define TEST_VIDEO_TIMER_LMT	2474
#define TEST_VIDEO_TIMER_TRIG	1387
#else // 50MHz
#define TEST_VIDEO_TIMER_DIV	250
#define TEST_VIDEO_TIMER_LMT	3332
#define TEST_VIDEO_TIMER_TRIG	1666
#endif
#define TEST_VIDEO_TIMER_HZ		(APB_FREQ/((TEST_VIDEO_TIMER_LMT+1)*2)/TEST_VIDEO_TIMER_DIV)

#define stream_count 120
stream_info vid_info[stream_count];

extern volatile int rtp_step;

static void testVideoTimer_irq(void *ctx)
{
	static int i = 0;
	static int j = 0;

	if (MsgStmPut(vid_info[i].addr, vid_info[i].size, vid_info[i].ts, vid_info[i].type) == ENX_OK) {
		IsrStreamdata(ctx);
		i++;
	} else {
		printf("%s(STM) Drop\n", __func__);
	}
	if (i == stream_count) {
		i = 0;
	}

	if (MsgRecPut(vid_info[j].addr, vid_info[j].size, vid_info[j].ts, vid_info[j].type) == ENX_OK) {
		IsrRecorddata(ctx);
		j++;
	} else {
		printf("%s(REC) Drop\n", __func__);
	}
	if (j == stream_count) {
		j = 0;
	}
}

int cmd_test_video(int argc, char *argv[])
{
	static int init = 0;
	if (strcmp("init", argv[1]) == 0) {
		BYTE *pvidbuf = (BYTE *)VID_HEADER;
		BYTE nal_start[4] = {0,0,0,1};

		printf("vid_info init: %lubyte\n", sizeof(vid_info));
		memset(vid_info, 0, sizeof(vid_info));

		int i = 0;
		ULONG total = 0;
		while (pvidbuf < (VID_HEADER + VID_SIZE)) {
			int sek = memcmp(pvidbuf, nal_start, 4);
			if (sek == 0) {
				BYTE nalvalue = pvidbuf[4];
#if VID_CODEC==0
				BYTE naltype = nalvalue & 0x1f;
				printf("%2d: [0x%02X/0x%02X]\n", i, nalvalue, naltype);
#elif VID_CODEC==1
				BYTE naltype = (nalvalue >> 1) & 0x3f;
				BYTE nalvalue2 = pvidbuf[5];
				printf("%2d: [0x%02X-0x%02X/0x%02X]\n", i, nalvalue, nalvalue2, naltype);
#endif
#if 0
				pvidbuf++;
#else
#if VID_CODEC==0
				if (naltype == 0x7) {
					vid_info[i].type = eSTREAMMSG_H264I;
#elif VID_CODEC==1
				if (naltype == 0x20) {
					vid_info[i].type = eSTREAMMSG_H265I;
#endif
					vid_info[i].addr = (UINT)(intptr_t)pvidbuf;
					vid_info[i].ts = 3000;
					BYTE *next = enx_memstr(pvidbuf+200, VID_SIZE-(pvidbuf+50-VID_HEADER), nal_start, 4);
					vid_info[i].size = (next - 4 - pvidbuf);
					total += vid_info[i].size;
					pvidbuf += 200;
					i++;
#if VID_CODEC==0
				} else if (naltype == 0x5 || naltype == 0x1) {
					if (naltype == 0x5) {
						vid_info[i].type = eSTREAMMSG_H264I;
					} else {
						vid_info[i].type = eSTREAMMSG_H264P;
					}
#elif VID_CODEC==1
				} else if (naltype == 0x5 || naltype == 0x1) {
					if (naltype == 0x5) {
						vid_info[i].type = eSTREAMMSG_H265I;
					} else {
						vid_info[i].type = eSTREAMMSG_H265P;
					}
#endif
					vid_info[i].addr = (UINT)(intptr_t)pvidbuf;
					vid_info[i].ts = 3000;
					BYTE *next = enx_memstr(pvidbuf+4, VID_SIZE-(pvidbuf+4-VID_HEADER), nal_start, 4);
					if (next == NULL) {
						vid_info[i].size = (VID_HEADER + VID_SIZE - pvidbuf);
					} else {
						vid_info[i].size = (next - 4 - pvidbuf);
					}
					total += vid_info[i].size;
					pvidbuf += 4;
					i++;
				} else {
					printf("%s %2d, 0x%02X, SEK: %ld\n", __func__, i, nalvalue, pvidbuf - VID_HEADER);
				}
#endif
			} else {
				pvidbuf++;
			}
		}
		printf("done: %lubyte\n", total);
		init = 1;
	} else if (strcmp("start", argv[1]) == 0) {
		if (init == 0) {
			char *arg[2] = {"video", "init"};
			cmd_test_video(2, arg);
		}
		TimerSetFreq(TEST_VIDEO_TIMER_CH, TEST_VIDEO_TIMER_DIV, TEST_VIDEO_TIMER_LMT, TEST_VIDEO_TIMER_TRIG);
		TimerIrqCallback(TEST_VIDEO_TIMER_CH, testVideoTimer_irq, NULL);
		TimerSetIrqEn(TEST_VIDEO_TIMER_CH, ENX_ON);
		printf("Start Video Timer: %uHz\n", TEST_VIDEO_TIMER_HZ);
		TimerStart(TEST_VIDEO_TIMER_CH);

#if (VID_CODEC==0)
		gtUser.vcVideo[e_vcVEncoder1].eCodec = e_vcodecH264;
		muxer_videnc_set_vcodec(eRecNormal, e_vcVEncoder1);
#if (FAT_SDSAVE_EVENT==1)
		muxer_videnc_set_vcodec(eRecEvent, e_vcVEncoder1);
#endif
#else
		gtUser.vcVideo[e_vcVEncoder1].eCodec = e_vcodecH265;
		muxer_videnc_set_vcodec(eRecNormal, e_vcVEncoder1);
#if (FAT_SDSAVE_EVENT==1)
		muxer_videnc_set_vcodec(eRecEvent, e_vcVEncoder1);
#endif
#endif
	} else if (strcmp("check", argv[1]) == 0) {
		printf("last number: %d\n", rtp_step);
	}
	return 0;
}
