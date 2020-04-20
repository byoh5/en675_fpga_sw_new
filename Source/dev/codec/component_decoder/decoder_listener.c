/*
 * Copyright (c) 2018, Chips&Media
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "cnm_app.h"
#include "decoder_listener.h"
#include "misc/debug.h"
#include "misc/bw_monitor.h"

static BOOL IsDifferentResolution(DecGetFramebufInfo* fbInfo, FrameBuffer* srcFb)
{
    FrameBuffer* dstFb = &fbInfo->framebufPool[srcFb->myIndex];

    return (dstFb->width != srcFb->width || dstFb->height != srcFb->height);
}

void HandleDecCompleteSeqEvent(Component com, CNMComListenerDecCompleteSeq* param, DecListenerContext* ctx)
{
    if (ctx->compareType == YUV_COMPARE) {
        Uint32 width  = param->initialInfo->picWidth;
        Uint32 height = param->initialInfo->picHeight;

        if ((ctx->comparator = Comparator_Create(YUV_COMPARE, param->refYuvPath, width, height, param->wtlFormat,
                                                 param->cbcrInterleave, param->bitstreamFormat==STD_VP9)) == NULL) {
            VLOG(ERR, "%s:%d Failed to Comparator_Create(%s)\n", __FUNCTION__, __LINE__, param->refYuvPath);
            CNMErrorSet(CNM_ERROR_FAILURE);
            CNMAppStop();
            return ;
        }
    }
}

static void HandleDecRegisterFbEvent(Component com, CNMComListenerDecRegisterFb* param, DecListenerContext* ctx)
{
    Uint32 fps = (ctx->fps == 0) ? 30 : ctx->fps;
    //SimpleRenderer_SetFrameRate(ctx->renderer, 0);
    if ( ctx->bandwidth ) {
        ctx->bwCtx = BWMonitorSetup(param->handle, TRUE, GetBasename((const char *)ctx->inputPath));
    }
    if ( ctx->performance ) {
        ctx->pfCtx = PFMonitorSetup(param->handle->coreIdx, param->handle->instIndex, ctx->pfClock, fps, GetBasename((const char *)ctx->inputPath), 0);
    }
}

static void HandleDecInterruptEvent(Component com, CNMComListenerDecInt* param, DecListenerContext* ctx)
{
    if ((1<<INT_WAVE5_DEC_PIC) == param->flag) {
        if (ctx->bwCtx != NULL) {
            BWMonitorUpdate(ctx->bwCtx, ctx->numVCores);
            BWMonitorReset(ctx->bwCtx);
        }
    }
}

//#define SAVE_FBC
#ifdef SAVE_FBC
static void SaveFBC(DecHandle handle, DecOutputInfo* outputInfo, Uint32 frameIdx)
{
    static Uint32 no = 0;

    if (outputInfo->indexFrameDisplayForTiled >= 0) {
        FrameBuffer     cfb;
        size_t          lumaTblSize, chromaTblSize;
        Uint32          frameIndex;
        char            fileName[MAX_FILE_PATH];
        int             fbc_endian;
        DecGetFramebufInfo  curFbInfo;
        Uint32          width=0;
        Uint32          height=0;
        Uint32          Bpp;
        Uint32          coreIdx = 0;
        VpuRect         rcDisplay = outputInfo->rcDisplay;
        size_t          sizeYuv;
        Uint8*          pYuv;
        Uint32          addr;

        VPU_DecGiveCommand(handle, DEC_GET_FRAMEBUF_INFO, (void*)&curFbInfo);

        pYuv = GetYUVFromFrameBuffer(handle, &outputInfo->dispFrame, rcDisplay, &width, &height, &Bpp, &sizeYuv);
        osal_free(pYuv);

        VLOG(INFO, "---> DUMP COMPRESSED FRAMEBUFFER #%d TO below log\n", outputInfo->indexFrameDisplayForTiled);
        VPU_DecGetFrameBuffer(handle, outputInfo->indexFrameDisplayForTiled, &cfb);

        fbc_endian = vdi_convert_endian(handle->coreIdx, cfb.endian);

        /* Dump Y compressed data */
        sprintf(fileName, "./%04d_%dx%d_%04d_%d_fbc_data_y.bin", no, cfb.width, outputInfo->dispPicHeight, frameIdx, fbc_endian);
        writeFilefromDram(coreIdx, fileName,  cfb.bufY, cfb.size, fbc_endian, 1);//bigger than real Y size
        /* Dump C compressed data */
        sprintf(fileName, "./%04d_%dx%d_%04d_%d_fbc_data_c.bin", no, cfb.width, outputInfo->dispPicHeight, frameIdx, fbc_endian);
        writeFilefromDram(coreIdx, fileName,  cfb.bufCb, cfb.size, fbc_endian, 1);//bigger than real C size

        /* Dump Y Offset table */
        VPU_GetFBCOffsetTableSize(STD_HEVC, (int)width, (int)height, (int*)&lumaTblSize, (int*)&chromaTblSize);
        frameIndex = outputInfo->indexFrameDisplayForTiled;
        sprintf(fileName, "./%04d_%dx%d_%04d_%d_fbc_table_y.bin", no, cfb.width, outputInfo->dispPicHeight, frameIdx, fbc_endian);
        addr = curFbInfo.vbFbcYTbl[frameIndex].phys_addr;
        writeFilefromDram(coreIdx, fileName,  addr, curFbInfo.vbFbcYTbl[0].size, fbc_endian, 1);

        /* Dump C Offset table */
        frameIndex = outputInfo->indexFrameDisplayForTiled;
        sprintf(fileName, "./%04d_%dx%d_%04d_%d_fbc_table_c.bin", no, cfb.width, outputInfo->dispPicHeight, frameIdx, fbc_endian);
        addr = curFbInfo.vbFbcCTbl[frameIndex].phys_addr;
        writeFilefromDram(coreIdx, fileName,  addr, curFbInfo.vbFbcCTbl[0].size, fbc_endian, 1);

        no++;
    }
}
#endif

void HandleDecGetOutputEvent(Component com, CNMComListenerDecDone* param, DecListenerContext* ctx)
{
    DecOutputInfo*          output          = param->output;
    FrameBuffer*            pFb             = &output->dispFrame;
    void*                   decodedData     = NULL;
    Uint8*                  pYuv            = NULL;
    Uint32                  decodedDataSize = 0;
    VpuRect                 rcDisplay       = {0,};
    Uint32                  width=0, height = 0, Bpp;
    size_t                  frameSizeInByte = 0;
    if (param->ret != RETCODE_SUCCESS) return;

    if (ctx->pfCtx != NULL) {
        if (output->indexFrameDecoded >= 0)
            PFMonitorUpdate(param->handle->coreIdx, ctx->pfCtx, output->frameCycle);
    }
    if (ctx->bwCtx != NULL) {
        BWMonitorUpdatePrint(ctx->bwCtx, output->picType);
    }



    // Finished decoding a frame
    if (output->indexFrameDisplay < 0) {
        return;
    }

#ifdef SAVE_FBC
    SaveFBC(param->handle, output, param->numDecoded);
#endif

    if (ctx->lastSeqNo < pFb->sequenceNo || IsDifferentResolution(&ctx->fbInfo, pFb)) {
        /* When the video sequence or the resolution of picture was changed. */
        ctx->lastSeqNo  = pFb->sequenceNo;
        VPU_DecGiveCommand(param->handle, DEC_GET_FRAMEBUF_INFO, (void*)&ctx->fbInfo);
    }

    if (ctx->compareType == YUV_COMPARE) {
        rcDisplay.right  = output->dispPicWidth;
        rcDisplay.bottom = output->dispPicHeight;
        if (TRUE == PRODUCT_ID_W_SERIES(ctx->productId)) {
            pYuv = GetYUVFromFrameBuffer(param->handle, &output->dispFrame, rcDisplay, &width, &height, &Bpp, &frameSizeInByte);
        }
        decodedData     = (void*)pYuv;
        decodedDataSize = frameSizeInByte;
    }

    if (ctx->comparator) {
        if ((ctx->match=Comparator_Act(ctx->comparator, decodedData, decodedDataSize)) == FALSE) {
        }
    }

    if (ctx->compareType == YUV_COMPARE) {
        osal_free(decodedData);
    }

    /* decoding error check */
    if (output->indexFrameDecoded >= 0) {
        ctx->notDecodedCount = 0;
    }
    else {
        ctx->notDecodedCount++;
        if (ctx->notDecodedCount == MAX_NOT_DEC_COUNT) {
            VLOG(ERR, "Continuous not-decoded-count is %d\nThere is something problem in DPB control.\n", ctx->notDecodedCount);
            CNMAppStop();
        }
    }

    if (ctx->match == FALSE) CNMAppStop();
}

static void HandleDecCloseEvent(Component com, CNMComListenerDecClose* param, DecListenerContext* ctx)
{
    if (ctx->bwCtx != NULL)
        BWMonitorRelease(ctx->bwCtx);
    if (ctx->pfCtx != NULL)
        PFMonitorRelease(ctx->pfCtx);
}

void DecoderListener(Component com, Uint64 event, void* data, void* context)
{
    int key=0;
    if (osal_kbhit()) {
        key = osal_getch();
        osal_flush_ch();

        switch (key) {
        case 'q':
        case 'Q':
            CNMAppStop();
            return;
        case 'f':
        case 'F':
            ComponentSetParameter(NULL, com, SET_PARAM_DEC_FLUSH, NULL);
            break;
        default:
            break;
        }
    }
    switch (event) {
    case COMPONENT_EVENT_DEC_OPEN:
        break;
    case COMPONENT_EVENT_DEC_ISSUE_SEQ:
        break;
    case COMPONENT_EVENT_DEC_COMPLETE_SEQ:
        HandleDecCompleteSeqEvent(com, (CNMComListenerDecCompleteSeq*)data, (DecListenerContext*)context);
        break;
    case COMPONENT_EVENT_DEC_REGISTER_FB:
        HandleDecRegisterFbEvent(com, (CNMComListenerDecRegisterFb*)data, (DecListenerContext*)context);
        break;
    case COMPONENT_EVENT_DEC_READY_ONE_FRAME:
        break;
    case COMPONENT_EVENT_DEC_START_ONE_FRAME:
        break;
    case COMPONENT_EVENT_DEC_INTERRUPT:
        HandleDecInterruptEvent(com, (CNMComListenerDecInt*)data, (DecListenerContext*)context);
        break;
    case COMPONENT_EVENT_DEC_GET_OUTPUT_INFO:
        HandleDecGetOutputEvent(com, (CNMComListenerDecDone*)data, (DecListenerContext*)context);
        break;
    case COMPONENT_EVENT_DEC_DECODED_ALL:
        break;
    case COMPONENT_EVENT_DEC_CLOSE:
        HandleDecCloseEvent(com, (CNMComListenerDecClose*)data, (DecListenerContext*)context);
        break;
    default:
        break;
    }
}

BOOL SetupDecListenerContext(DecListenerContext* ctx, CNMComponentConfig* config, Component renderer)
{
    TestDecConfig* decConfig = &config->testDecConfig;
    Int32 md5Size = 0;

    osal_memset((void*)ctx, 0x00, sizeof(DecListenerContext));

    md5Size = PRODUCT_ID_W_SERIES(decConfig->productId) ? 12 : 8 ;

    if (decConfig->compareType == MD5_COMPARE) {
        if ((ctx->comparator=Comparator_Create(MD5_COMPARE, decConfig->md5Path, md5Size)) == NULL) {
            VLOG(ERR, "%s:%d Failed to Comparator_Create(%s)\n", __FUNCTION__, __LINE__, decConfig->md5Path);
            return FALSE;
        }
    }


    ctx->renderer      = renderer;
    ctx->lastSeqNo     = -1;
    ctx->compareType   = decConfig->compareType;
    ctx->match         = TRUE;

    ctx->performance   = decConfig->performance;
    ctx->bandwidth     = decConfig->bandwidth;
    ctx->fps           = decConfig->fps;
    ctx->pfClock       = decConfig->pfClock;
    ctx->numVCores     = decConfig->wave.numVCores;
    ctx->bitFormat     = decConfig->bitFormat;
    ctx->productId     = decConfig->productId;
    osal_memcpy(ctx->inputPath, decConfig->inputPath, sizeof(ctx->inputPath));

    return TRUE;
}

void ClearDecListenerContext(DecListenerContext* ctx)
{
    if (ctx->comparator)    Comparator_Destroy(ctx->comparator);
}


