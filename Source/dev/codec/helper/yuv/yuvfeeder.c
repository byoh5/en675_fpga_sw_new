//--=========================================================================--
//  This file is a part of VPU Reference API project
//-----------------------------------------------------------------------------
//
//       This confidential and proprietary software may be used only
//     as authorized by a licensing agreement from Chips&Media Inc.
//     In the event of publication, the following notice is applicable:
//
//            (C) COPYRIGHT 2006 - 2013  CHIPS&MEDIA INC.
//                      ALL RIGHTS RESERVED
//
//       The entire notice above must be reproduced on all authorized
//       copies.
//
//--=========================================================================--

#include <string.h>
#include <stdarg.h>
#include "main_helper.h"

extern YuvFeederImpl loaderYuvFeederImpl;
extern YuvFeederImpl cfbcYuvFeederImpl;

BOOL loaderYuvFeeder_Create(
    YuvFeederImpl *impl,
    const char* path,
    Uint32   packed,
    Uint32   fbStride,
    Uint32   fbHeight
);

BOOL loaderYuvFeeder_Destory(
    YuvFeederImpl *impl
);

BOOL loaderYuvFeeder_Feed(
    YuvFeederImpl*  impl,
    Int32           coreIdx,
    FrameBuffer*    fb,
    size_t          picWidth,
    size_t          picHeight,
    Uint32          srcFbIndex,
    void*           arg
);

BOOL loaderYuvFeeder_Configure(
    YuvFeederImpl* impl,
    Uint32         cmd,
    YuvInfo        yuv
);

BOOL cfbcYuvFeeder_Create(
    YuvFeederImpl *impl,
    const char* path,
    Uint32   packed,
    Uint32   fbStride,
    Uint32   fbHeight
);

BOOL cfbcYuvFeeder_Destory(
    YuvFeederImpl *impl
);

BOOL cfbcYuvFeeder_Feed(
    YuvFeederImpl*  impl,
    Int32           coreIdx,
    FrameBuffer*    fb,
    size_t          picWidth,
    size_t          picHeight,
    Uint32          srcFbIndex,
    void*           arg
);

BOOL cfbcYuvFeeder_Configure(
    YuvFeederImpl* impl,
    Uint32         cmd,
    YuvInfo        yuv
);

static BOOL yuvYuvFeeder_Create(
    YuvFeederImpl *impl,
    const char*   path,
    Uint32        packed,
    Uint32        fbStride,
    Uint32        fbHeight)
{
    yuvContext*   ctx;
    osal_file_t  fp;
    Uint8*        pYuv;

    dbgprintf("[debug] %s %d\n",__func__,__LINE__);

#ifdef EYENIX_TEST
#ifndef EYENIX_TEST_HEAP
	//image
	//load_image G:/EN675/Eclipse_setup/BasketballPass_416x240_5.yuv 0x80500000 bin
	//verify_image G:/EN675/Eclipse_setup/BasketballPass_416x240_5.yuv 0x80500000 bin
	//mdb 0x80500000 16

    //if ((fp=osal_fopen(path, "rb")) == NULL) {
    if ((fp=osal_fopen(path, "yuv")) == NULL) {
        VLOG(ERR, "%s:%d failed to open yuv file: %s\n", __FUNCTION__, __LINE__, path);
        return FALSE;
    }

#endif
#else
    if ((fp=osal_fopen(path, "rb")) == NULL) {
            VLOG(ERR, "%s:%d failed to open yuv file: %s\n", __FUNCTION__, __LINE__, path);
        return FALSE;
    }
#endif

#ifdef EYENIX_TEST_HEAP

    if ((ctx=(yuvContext*)osal_malloc(sizeof(yuvContext))) == NULL) {
        //osal_free(pYuv);
        osal_fclose(fp);
        return FALSE;
    }

   // dbgprintf("\n[debug] yuvYuvFeeder_Create \n================== yuv address 0x%x , size %d =================\n", fp->_base, fp->_bufsiz);

    osal_memset(ctx, 0, sizeof(yuvContext));

    ctx->fp       = fp;
    //ctx->pYuv     = fp->_base;
    impl->context = ctx;

#else
    ctx->pYuv     = fp->_base;
    //1:YUYV, 2:YVYU, 3:UYVY, 4:VYUY
    if ( packed == 1 )
        pYuv = osal_malloc(fbStride*fbHeight*3*2*2);//packed, unsigned short
    else
        pYuv = osal_malloc(fbStride*fbHeight*3*2);//unsigned short

    if ((ctx=(yuvContext*)osal_malloc(sizeof(yuvContext))) == NULL) {
        osal_free(pYuv);
        osal_fclose(fp);
        return FALSE;
    }

    osal_memset(ctx, 0, sizeof(yuvContext));

    ctx->fp       = fp;
    ctx->pYuv     = pYuv;
    impl->context = ctx;
#endif
    return TRUE;
}

static BOOL yuvYuvFeeder_Destory(
    YuvFeederImpl *impl
    )
{
    yuvContext*    ctx = (yuvContext*)impl->context;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    osal_fclose(ctx->fp);

#ifndef EYENIX_TEST_HEAP
    osal_free(ctx->pYuv);
#endif

    osal_free(ctx);
    return TRUE;
}

#ifdef EYENIX_TEST
void make_raw_nv12(UINT SIZE_W, UINT SIZE_H, BYTE *pY,BYTE *pCb,BYTE *pCr){
/*
	UINT i;
	for (i=0;i<1920*1080;i++) {
		*(CHAR*)(RAWY0_IMG_1080BASE+i) = 0xff;
	}
	for (i=0;i<1920*1080;i++) {
		*(CHAR*)(RAWY1_IMG_1080BASE+i) = 0x00;
	}

	for (i=0;i<1920*1080/2;i++) {
		*(CHAR*)(RAWC0_IMG_1080BASE+i) = 0x00;
	}
	for (i=0;i<1920*1080/2;i++) {
		*(CHAR*)(RAWC1_IMG_1080BASE+i) = 0xff;
	}
*/

#if 1
#define __AAA 30
	UINT x,y;
	printf("pY %x, pCb %x, pCr %x\n", pY, pCb, pCr);
	//BYTE *pY, *pCb, *pCr;
	//pY = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[0].y_addr;
	//pCb = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[0].c_addr;
	//pCr = (BYTE *)(h264_enc.chinfo[0].imgbuf.cur[0].c_addr+1);
	pCr = (BYTE *)(pCb+1);
	static test_number = 0;

	if(test_number % 4 == 0||test_number % 4 == 1)
	{
		for (y = 0; y < SIZE_H; y++) {
			for (x = 0; x < SIZE_W; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pY = 16;
				else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 81;
				else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
				else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
				else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 55;
				else								*pY = 235;
				pY++;
			}
		}
		for (y = 0; y < SIZE_H / 2; y++) {
			for (x = 0; x < SIZE_W / 2; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 90;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 55;
				else								*pCb = 128;
				if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 240;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 110;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
				else								*pCr = 128;
				pCb+=2;
				pCr+=2;
			}
		}
	}
	else
	{
		//pY = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[1].y_addr;
		//pCb = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[1].c_addr;
		//pCr = (BYTE *)(h264_enc.chinfo[0].imgbuf.cur[1].c_addr+1);
		for (y = 0; y < SIZE_H; y++) {
			for (x = 0; x < SIZE_W; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pY = 16;
				else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 55;
				else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
				else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
				else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 81;
				else								*pY = 235;
				pY++;
			}
		}
		for (y = 0; y < SIZE_H / 2; y++) {
			for (x = 0; x < SIZE_W / 2; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 55;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 90;
				else								*pCb = 128;
				if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 110;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
				else								*pCr = 128;
				pCb+=2;
				pCr+=2;
			}
		}
	}
	test_number++;
	if(test_number > 10000)
		test_number = 0;
#else
#define PATTERN_NUM 4
#define FIXED_PATTERN
#ifdef FIXED_PATTERN
	UINT x,y;
	static unsigned int cot = 0;

	static UINT paddr[PATTERN_NUM];
	dbgprintf(" paddr[%d] 0x%x, pY 0x%x\n", cot, paddr[cot], pY);
	if(paddr[cot] != pY)
		paddr[cot] = pY;
	else
	{
		cot++;
		if(cot>PATTERN_NUM-1)	cot = 0;
		dbgprintf(" making skip \n");
		return;
	}
#endif

	dbgprintf(" new making... \n");
#define __AAA 30

	//BYTE *pY, *pCb, *pCr;
	//pY = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[0].y_addr;
	//pCb = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[0].c_addr;
	//pCr = (BYTE *)(h264_enc.chinfo[0].imgbuf.cur[0].c_addr+1);
	pCr = (BYTE *)(pCb+1);

	if (cot % PATTERN_NUM == 0) {

		for (y = 0; y < SIZE_H; y++) {
			for (x = 0; x < SIZE_W; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pY = 16;
				else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 81;
				else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
				else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
				else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 55;
				else								*pY = 235;
				pY++;
			}
		}
		for (y = 0; y < SIZE_H / 2; y++) {
			for (x = 0; x < SIZE_W / 2; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 90;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 55;
				else								*pCb = 128;
				if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 240;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 110;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
				else								*pCr = 128;
				pCb+=PATTERN_NUM;
				pCr+=PATTERN_NUM;
			}
		}

	}else	if (cot % PATTERN_NUM == 1) {

		for (y = 0; y < SIZE_H; y++) {
			for (x = 0; x < SIZE_W; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pY = 16;
				else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 81;
				else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
				else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
				else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 55;
				else								*pY = 235;
				pY++;
			}
		}
		for (y = 0; y < SIZE_H / 2; y++) {
			for (x = 0; x < SIZE_W / 2; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 90;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 55;
				else								*pCb = 128;
				if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 240;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 110;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
				else								*pCr = 128;
				pCb+=PATTERN_NUM;
				pCr+=PATTERN_NUM;
			}
		}

	}else	if (cot % PATTERN_NUM == 2) {

		for (y = 0; y < SIZE_H; y++) {
			for (x = 0; x < SIZE_W; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pY = 16;
				else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 81;
				else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
				else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
				else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 55;
				else								*pY = 235;
				pY++;
			}
		}
		for (y = 0; y < SIZE_H / 2; y++) {
			for (x = 0; x < SIZE_W / 2; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 55;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 90;
				else								*pCb = 128;
				if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 110;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
				else								*pCr = 128;
				pCb+=PATTERN_NUM;
				pCr+=PATTERN_NUM;
			}
		}
	}else	if (cot % PATTERN_NUM == 3) {

		for (y = 0; y < SIZE_H; y++) {
			for (x = 0; x < SIZE_W; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pY = 16;
				else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 81;
				else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
				else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
				else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 55;
				else								*pY = 235;
				pY++;
			}
		}
		for (y = 0; y < SIZE_H / 2; y++) {
			for (x = 0; x < SIZE_W / 2; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 55;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 90;
				else								*pCb = 128;
				if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 110;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
				else								*pCr = 128;
				pCb+=PATTERN_NUM;
				pCr+=PATTERN_NUM;
			}
		}

	}else	if (cot % PATTERN_NUM == 4) {

		for (y = 0; y < SIZE_H; y++) {
			for (x = 0; x < SIZE_W; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pY = 16;
				else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 81;
				else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
				else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
				else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 55;
				else								*pY = 235;
				pY++;
			}
		}
		for (y = 0; y < SIZE_H / 2; y++) {
			for (x = 0; x < SIZE_W / 2; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 90;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 33;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 55;
				else								*pCb = 128;
				if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 240;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 110;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
				else								*pCr = 128;
				pCb+=PATTERN_NUM;
				pCr+=PATTERN_NUM;
			}
		}

	}
	else {

		//pY = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[1].y_addr;
		//pCb = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[1].c_addr;
		//pCr = (BYTE *)(h264_enc.chinfo[0].imgbuf.cur[1].c_addr+1);
		for (y = 0; y < SIZE_H; y++) {
			for (x = 0; x < SIZE_W; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pY = 16;
				else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 55;
				else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
				else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
				else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 81;
				else								*pY = 235;
				pY++;
			}
		}
		for (y = 0; y < SIZE_H / 2; y++) {
			for (x = 0; x < SIZE_W / 2; x++) {
				if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 55;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 33;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 90;
				else								*pCb = 128;
				if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
				else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 110;
				else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
				else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 240;
				else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
				else								*pCr = 128;
				pCb+=PATTERN_NUM;
				pCr+=PATTERN_NUM;
			}
		}
	}

	cot++;

#ifdef FIXED_PATTERN
	if(cot>PATTERN_NUM-1)	cot = 0;
#endif
#endif
}
#endif

static BOOL yuvYuvFeeder_Feed(
    YuvFeederImpl*  impl,
    Int32           coreIdx,
    FrameBuffer*    fb,
    size_t          picWidth,
    size_t          picHeight,
    Uint32          srcFbIndex,
    void*           arg
    )
{
    yuvContext* ctx = (yuvContext*)impl->context;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);

    Uint8*      pYuv = ctx->pYuv;
    size_t      frameSize;
    size_t      frameSizeY;
    size_t      frameSizeC;
    Int32       bitdepth=0;
    Int32       yuv3p4b=0;
    Int32       packedFormat=0;
    Uint32      outWidth=0;
    Uint32      outHeight=0;
    EncInfo*    pEncInfo = VPU_HANDLE_TO_ENCINFO(impl->handle);
    BOOL        subFrameSyncSrcWriteMode = pEncInfo->subFrameSyncConfig.subFrameSyncSrcWriteMode;
    BOOL        subFrameSyncEn = pEncInfo->subFrameSyncConfig.subFrameSyncOn; 
    Uint32      writeSrcLine = pEncInfo->subFrameSyncConfig.subFrameSyncSrcWriteMode & ~REMAIN_SRC_DATA_WRITE;
    CalcYuvSize(fb->format, picWidth, picHeight, fb->cbcrInterleave, &frameSizeY, &frameSizeC, &frameSize, &bitdepth, &packedFormat, &yuv3p4b);

	//mdb 0x80231720 16

    // Load source one picture image to encode to SDRAM frame buffer.


#if 0//def STREAM_ENCODE_DUAL
    static int test_yuv_count = 0;

    if(test_yuv_count++>20)
    {
    	dbgprintf("IMG IN pYuv addr %x test_yuv_count %d \n",pYuv, test_yuv_count);
    }
    else
    {
    	dbgprintf("NO IMAGE pYuv addr %x test_yuv_count %d \n",pYuv, test_yuv_count);
    	return FALSE;
    }
#endif

#ifdef EYENIX_TEST
#ifdef EYENIX_TEST_HEAP
#include "isp_reg.h"
    //BYTE *Y0addr = IM_YADR1_P0r<<4;
    //dbgprintf("make raw bufY %x width %d height %d isp y addr %x \n", fb->bufY, picWidth, picHeight, Y0addr);
    //make_raw_nv12(picWidth, picHeight, fb->bufY, fb->bufCb, fb->bufCr);
    //dbgprintf("dma dump Y from 0x9078_0780 to 0x%x \n", fb->bufY);
    //BDmaMemCpy_isr(CPU_ID, fb->bufY, 0x90780780, 2073600);
    //dbgprintf("dma dump Cbcr from 0x9097_AB80 to 0x%x \n", fb->bufCb);
    //BDmaMemCpy_isr(CPU_ID, fb->bufCb, 0x9097AB80, 1036800);

    //BDmaMemCpy_isr(CPU_ID, fb->bufY, 0x90780780, 3110400);
//    osal_memcpy(fb->bufY, 0x90aaa780, 3110400); //FHD
      //osal_memcpy(fb->bufY, 0x920EFE00, 149760); //UHD
	//osal_memcpy(fb->bufY, 0x920EFE00, 12441600); //UHD
    //BDmaMemCpy_isr(CPU_ID, fb->bufY, 0x90aaa780, 3110400);

    //
    //BDmaMemCpy_isr(CPU_ID, fb->bufY, IM_YADR1_P0r<<4, 12441600);
    //printf("codec test_isp_codec_flag %d test_isp_done_flag %d \n", test_isp_codec_flag, test_isp_done_flag);
    Visp();
    if(test_isp_done_flag == 1)
    {
    	test_isp_codec_flag = 1;
    	test_isp_done_flag = 0;

    	osal_memcpy(fb->bufY, 0x920efe00, 12441600);
    }
    //printf("raw copy isp done %d \n", test_isp_done_flag);
#include "dev.h"
    CodecENCTimeStart = BenchTimeStart();
    return TRUE;

#else

    if (!osal_fread(pYuv, 1, frameSize, ctx->fp)) {
        if (osal_feof(ctx->fp) == 0) 
            VLOG(ERR, "Yuv Data osal_fread failed file handle is 0x%x\n", ctx->fp);
        return FALSE;
    }
#endif

#if 0//def EYENIX_TEST

#include "main_helper.h"

    int         changes;
    int         sys_endian;
	int 		endian;
    BOOL        byteChange, wordChange, dwordChange, lwordChange;

	endian = VDI_128BIT_LITTLE_ENDIAN;
	sys_endian = VDI_128BIT_BIG_ENDIAN;// VDI_128BIT_BUS_SYSTEM_ENDIAN;
	//dbgprintf("VpuWriteMem config.bitcode addr %x data1[%x] data2[%x] \n",config.bitcode, *(config.bitcode), *(config.bitcode+1));

    changes     = endian ^ sys_endian;
    byteChange  = changes&0x01;
    wordChange  = ((changes&0x02) == 0x02);
    dwordChange = ((changes&0x04) == 0x04);
    lwordChange = ((changes&0x08) == 0x08);

    dbgprintf("byteChange %d wordChange %d dwordChange %d lwordChange %d\n", byteChange,wordChange, dwordChange, lwordChange);

    if (byteChange)  byte_swap(pYuv, (int)frameSize);
    if (wordChange)  word_swap(pYuv, (int)frameSize);
    if (dwordChange) dword_swap(pYuv, (int)frameSize);
    if (lwordChange) lword_swap(pYuv, (int)frameSize);

#endif

    if ( subFrameSyncEn ==TRUE && (subFrameSyncSrcWriteMode & REMAIN_SRC_DATA_WRITE) == 0)
        osal_fseek(ctx->fp, -((long)frameSize), SEEK_CUR);

    //Already wrote in SRC_XXXLINE_WRITE mode
    if ( subFrameSyncEn && (subFrameSyncSrcWriteMode & REMAIN_SRC_DATA_WRITE) && writeSrcLine == picHeight)
        return TRUE;
    if (fb->mapType == LINEAR_FRAME_MAP ) {
        outWidth  = (yuv3p4b&&packedFormat==0) ? ((picWidth+31)/32)*32  : picWidth;
        outHeight = (yuv3p4b) ? ((picHeight+7)/8)*8 : picHeight;

        if ( yuv3p4b  && packedFormat) {
            outWidth = ((picWidth*2)+2)/3*4;
        }
        else if(packedFormat) {
            outWidth *= 2;           // 8bit packed mode(YUYV) only. (need to add 10bit cases later)
            if (bitdepth != 0)      // 10bit packed
                outWidth *= 2;
        }
        LoadYuvImageByYCbCrLine(impl->handle, coreIdx, pYuv, outWidth, outHeight, fb, srcFbIndex);
        //LoadYuvImageBurstFormat(coreIdx, pYuv, outWidth, outHeight, fb, ctx->srcPlanar);
    }
    else {
        TiledMapConfig  mapConfig;

        osal_memset((void*)&mapConfig, 0x00, sizeof(TiledMapConfig));
        if (arg != NULL) {
            osal_memcpy((void*)&mapConfig, arg, sizeof(TiledMapConfig));
        }

        LoadTiledImageYuvBurst(impl->handle, coreIdx, pYuv, picWidth, picHeight, fb, mapConfig);
    }
#endif
    return TRUE;
}

static BOOL yuvYuvFeeder_Configure(
    YuvFeederImpl* impl,
    Uint32         cmd,
    YuvInfo        yuv
    )
{
    yuvContext* ctx = (yuvContext*)impl->context;
    UNREFERENCED_PARAMETER(cmd);
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    ctx->fbStride       = yuv.srcStride;
    ctx->cbcrInterleave = yuv.cbcrInterleave;
    ctx->srcPlanar      = yuv.srcPlanar;

    return TRUE;
}

YuvFeederImpl yuvYuvFeederImpl = {
    NULL,
    yuvYuvFeeder_Create,
    yuvYuvFeeder_Feed,
    yuvYuvFeeder_Destory,
    yuvYuvFeeder_Configure
};

/*lint -esym(438, ap) */
YuvFeeder YuvFeeder_Create(
    Uint32          type,
    const char*     srcFilePath,
    YuvInfo         yuvInfo
    )
{
    AbstractYuvFeeder *feeder;
    YuvFeederImpl     *impl;
    BOOL              success = FALSE;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (srcFilePath == NULL) {
        VLOG(ERR, "%s:%d src path is NULL\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    // Create YuvFeeder for type.
    switch (type) {
    case SOURCE_YUV:
        impl = osal_malloc(sizeof(YuvFeederImpl));
        impl->Create    = yuvYuvFeeder_Create;
        impl->Feed      = yuvYuvFeeder_Feed;
        impl->Destroy   = yuvYuvFeeder_Destory;
        impl->Configure = yuvYuvFeeder_Configure;
        if ((success=impl->Create(impl, srcFilePath, yuvInfo.packedFormat, yuvInfo.srcStride, yuvInfo.srcHeight)) == TRUE) {
            impl->Configure(impl, 0, yuvInfo);
        }
        break;       
    case SOURCE_YUV_WITH_LOADER:
        impl = osal_malloc(sizeof(YuvFeederImpl));
        impl->Create    = loaderYuvFeeder_Create;
        impl->Feed      = loaderYuvFeeder_Feed;
        impl->Destroy   = loaderYuvFeeder_Destory;
        impl->Configure = loaderYuvFeeder_Configure;
        if ((success=impl->Create(impl, srcFilePath, yuvInfo.packedFormat, yuvInfo.srcStride, yuvInfo.srcHeight)) == TRUE) {
            impl->Configure(impl, 0, yuvInfo);
        }
        break;
    default:
        VLOG(ERR, "%s:%d Unknown YuvFeeder Type\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    if (success == FALSE)
        return NULL;

    feeder = (AbstractYuvFeeder*)osal_malloc(sizeof(AbstractYuvFeeder));
    if ( !feeder )
        return NULL;
    feeder->impl = impl;

    return feeder;
}
/*lint +esym(438, ap) */

BOOL YuvFeeder_Destroy(
    YuvFeeder feeder
    )
{
    YuvFeederImpl*     impl = NULL;
    AbstractYuvFeeder* yuvfeeder =  (AbstractYuvFeeder*)feeder;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (yuvfeeder == NULL) {
        VLOG(ERR, "%s:%d Invalid handle\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    impl = yuvfeeder->impl;

    impl->Destroy(impl);
    osal_free(impl);
    return TRUE;
}

BOOL YuvFeeder_Feed(
    YuvFeeder       feeder,
    Uint32          coreIdx,
    FrameBuffer*    fb,
    size_t          picWidth,
    size_t          picHeight,
    Uint32          srcFbIndex,
    void*           arg
    )
{
    YuvFeederImpl*      impl = NULL;
    AbstractYuvFeeder*  absFeeder = (AbstractYuvFeeder*)feeder;
    Int32               ret;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (absFeeder == NULL) {
        VLOG(ERR, "%s:%d Invalid handle\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    impl = absFeeder->impl;

    ret = impl->Feed(impl, coreIdx, fb, picWidth, picHeight, srcFbIndex, arg);
    return ret;
}

