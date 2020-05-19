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

#include "isp_jpeg.h"

#include "muxer_videnc.h"

#include "shell_cmd_media.h"

const char *sTestVideoCmd[] = {"Test Video",                    (char*)0};
const char *sTestJpegCmd[]  = {"Test Jpeg",                     (char*)0};

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

#ifdef EN675_HEVC_TEST
	//while(BIT_STREAM_LOCK==1)
	//	WaitXms(1);
	static UINT prev_STREAM_ADDR = 0;
	static UINT prev_BIT_STREAM_FRNUM = 0;
	//printf(">>BIT_STREAM_LOCK 0x%d BIT_STREAM_ADDR 0x%x \n", BIT_STREAM_LOCK, BIT_STREAM_ADDR);
	if(BIT_STREAM_LOCK == 0 && BIT_STREAM_ADDR != 0 && prev_STREAM_ADDR != BIT_STREAM_ADDR)
	{
		BIT_STREAM_LOCK = 1;
		//printf(">>BIT_STREAM_ADDR 0x%x BIT_STREAM_SIZE %d\n", BIT_STREAM_ADDR, BIT_STREAM_SIZE);
		//dbgprintf("[debug] %s %d SAVED BITSTREAM (%d) 0x%x size %d [offset 0x%x]\n",__func__,__LINE__, instanceIndex, TESTOUT+TESTOFFSET[instanceIndex], bitstreamSize, TESTOFFSET[instanceIndex] + bitstreamSize);
		vid_info[i].addr = BIT_STREAM_ADDR;
		vid_info[i].size = BIT_STREAM_SIZE;
		prev_STREAM_ADDR = BIT_STREAM_ADDR;

		if(BIT_STREAM_FRNUM - prev_BIT_STREAM_FRNUM > 1)
			printf("[pre %d cur %d frame gap [%d]]", prev_BIT_STREAM_FRNUM, BIT_STREAM_FRNUM, BIT_STREAM_FRNUM - prev_BIT_STREAM_FRNUM);
		prev_BIT_STREAM_FRNUM = BIT_STREAM_FRNUM;
		vid_info[i].ts = 30000;//3fps 22500;//fps 4   7500;//12fps//3000;//30fps 18000;//5fps 9000;//10fps  //6000;//15fps
		//hwflush_dcache_range(BIT_STREAM_ADDR, BIT_STREAM_SIZE);
		//hwflush_dcache_range_rtos((u32)BIT_STREAM_ADDR, BIT_STREAM_SIZE);
		if(BIT_STREAM_TYPE == 0) // i frame
		{//printf("BIT_STREAM_TYPE %d BIT_STREAM_ADDR 0x%x BIT_STREAM_SIZE %d\n",BIT_STREAM_TYPE,  BIT_STREAM_ADDR, BIT_STREAM_SIZE);
#if (VID_CODEC==0)
			cpu1_BIT_STREAM_TYPE = eSTREAMMSG_H264I;
#elif (VID_CODEC==1)
			vid_info[i].type = eSTREAMMSG_H265I;
#endif
		}
		else //p frame
		{
#if (VID_CODEC==0)
			cpu1_BIT_STREAM_TYPE = eSTREAMMSG_H264P;
#elif (VID_CODEC==1)
			vid_info[i].type = eSTREAMMSG_H265P;
#endif
		}


		BIT_STREAM_LOCK = 0;
	}
	else
	{
		return;
	}
#endif

#if defined(__NETWORK__)
	if (MsgStmPut(vid_info[i].addr, vid_info[i].size, vid_info[i].ts, vid_info[i].type) == ENX_OK) {
		IsrStreamdata();
		i++;
	} else {
		printf("%s(STM) Drop\n", __func__);
	}
	if (i == stream_count) {
		i = 0;
	}
#endif

#if (LOAD_FS_SDCARD==1)
	if (MsgRecPut(vid_info[j].addr, vid_info[j].size, vid_info[j].ts, vid_info[j].type) == ENX_OK) {
		IsrRecorddata();
		j++;
	} else {
		printf("%s(REC) Drop\n", __func__);
	}
	if (j == stream_count) {
		j = 0;
	}
#endif
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

#if (LOAD_FS_SDCARD==1)
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
#endif
#if (ENX_RTSP_use==1)
	} else if (strcmp("check", argv[1]) == 0) {
		printf("last number: %d\n", rtp_step);
	}
#else
	}
#endif
	return 0;
}

int cmd_test_jpeg(int argc, char *argv[])
{
#if 1
	if (argc == 2 && (strcmp(argv[1], "start") == 0)) {
		printf("jpeg on!\n");
		enx_jpeg_on();
#if (LOAD_FS_SDCARD==1)
	} else if (argc == 2 && (strcmp(argv[1], "save") == 0)) {
		if (muxer_jpegstill_request() == ENX_OK) {
			printf("Save ok!\n");
		}
#endif
	} else if (argc == 3 && (strcmp(argv[1], "q") == 0)) {
		int qp = atoi(argv[2]);
		enx_jpeg_set_quantize(qp);
#if 1
		vTaskDelay(50);

		while (gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_PROC) {
			vTaskDelay(1);
		}
		gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_STR;
		printf("ADDR(0x%08X~0x%08X) SIZE(%u)\n", gptMsgShare.JPEG_STILL_ADDR, gptMsgShare.JPEG_STILL_ADDR + gptMsgShare.JPEG_STILL_SIZE, gptMsgShare.JPEG_STILL_SIZE);
		gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;

		UINT endaddr = gptMsgShare.JPEG_STILL_ADDR + gptMsgShare.JPEG_STILL_SIZE;
		UINT newaddr = ENX_MEM_ALIGN(endaddr) - 64;
		hwflush_dcache_range(newaddr, 64);

		BYTE *pEndAddr = (BYTE *)(intptr_t)(endaddr - 2);
		if (pEndAddr[0] == 0xff && pEndAddr[1] == 0xd9) {
			printf("Length OK\n");
		} else {
			hexDump("Memory Dump", (void *)(intptr_t)newaddr, 64);
		}
#endif
#if defined(__FILESYSTEM__)
	} else if (argc == 2 && (strcmp(argv[1], "dec") == 0)) {
		FIL fp;
		FRESULT fres;
		if ((fres=f_open(&fp, "1:/snapshot/2m.jpg", FA_READ)) != FR_OK) {
			printf("file open fail\n");
		} else {
			UINT read_size = 0;
			FSIZE_t jpeg_size = f_size(&fp);
			printf("file open ok(%ubyte)\n", jpeg_size);
			BYTE *src_addr = pvPortMalloc(jpeg_size);
			if (src_addr == NULL) {
				printf("src malloc file\n");
				f_close(&fp);
				return 0;
			}

			fres = f_read(&fp, src_addr, jpeg_size, &read_size);
			if (fres != FR_OK) {
				printf("file read fail(read:%u)\n", read_size);
			} else {
				printf("file read ok(read:%u)\n", read_size);
				if (jpeg_size == read_size) {
					printf("read ok!\n");
				}
			}

			hexDump("JPEG", src_addr, 512);

			f_close(&fp);
			vPortFree(src_addr);

			if ((fres=f_open(&fp, "1:/snapshot/2m.raw", FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK) {
				printf("dec file open fail\n");
			} else {
#define JPEG_DEC_YUV_SIZE (4*1024*1024)
				UINT write_size = 0;
				BYTE *dst_addr = pvPortCalloc(1, JPEG_DEC_YUV_SIZE);
				if (dst_addr == NULL) {
					printf("dst malloc file\n");
					f_close(&fp);
					return 0;
				}

				flprintf("\n");
				FRESULT fexp = f_expand(&fp, JPEG_DEC_YUV_SIZE, 1);
				if (fexp != FR_OK) {
					printf("f_expand fail\n");
				} else {
					printf("f_expand ok\n");
				}

				flprintf("JPEG DEC START\n");

				if (f_write(&fp, dst_addr, JPEG_DEC_YUV_SIZE, &write_size) != FR_OK) {
					printf("file write fail(write:%u)\n", write_size);
				} else {
					printf("file write ok(write:%u)\n", write_size);
				}

				f_close(&fp);
				vPortFree(dst_addr);
			}
		}
#endif
	} else {
		while (gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_PROC) {
			vTaskDelay(1);
		}
		gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_STR;

		printf("ADDR(0x%08X) SIZE(%u)\n", gptMsgShare.JPEG_STILL_ADDR, gptMsgShare.JPEG_STILL_SIZE);

		gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;
	}


#endif
	return 0;
}
