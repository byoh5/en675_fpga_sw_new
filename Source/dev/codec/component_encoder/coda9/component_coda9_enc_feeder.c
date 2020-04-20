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
#include <string.h>
#include "component.h"

typedef enum {
    SRC_FB_TYPE_YUV,
    SRC_FB_TYPE_CFRAME
} SRC_FB_TYPE;

typedef enum {
    YUV_FEEDER_STATE_WAIT,
    YUV_FEEDER_STATE_FEEDING
} YuvFeederState;

typedef struct {
    EncSubFrameSyncState        subFrameSyncState;
    EncSubFrameSyncConfig       subFrameSyncCfg;
} SubFrameSyncFeederInfo;

typedef struct {
    EncHandle                   handle;
    char                        inputPath[MAX_FILE_PATH];
    EncOpenParam                encOpenParam;
    Int32                       rotAngle;
    Int32                       mirDir;
    Int32                       frameOutNum;
    Int32                       yuvMode;
    Uint32                      reconFbWidth;
    Uint32                      reconFbHeight;
    Uint32                      srcFbStride;
    FrameBufferAllocInfo        srcFbAllocInfo;
    FrameBufferAllocInfo        reconFbAllocInfo;
    BOOL                        fbAllocated;
    ParamEncNeedFrameBufferNum  fbCount;
    FrameBuffer                 pFbRecon[MAX_REG_FRAME];
    vpu_buffer_t                pFbReconMem[MAX_REG_FRAME];
    FrameBuffer                 pFbSrc[ENC_SRC_BUF_NUM];
    vpu_buffer_t                pFbSrcMem[ENC_SRC_BUF_NUM];
    FrameBuffer                 pFbOffsetTbl[ENC_SRC_BUF_NUM];
    vpu_buffer_t                pFbOffsetTblMem[ENC_SRC_BUF_NUM];
    YuvFeeder                   yuvFeeder;
    BOOL                        last;
    YuvFeederState              state;
    BOOL                        stateDoing;
    Int32                       feedingNum;
    TiledMapType                mapType;
    TiledMapConfig              mapConfig;
    BOOL                        subFrameSyncEn;
    SubFrameSyncFeederInfo      subFrameSyncInfo;
} YuvFeederContext;

static void InitYuvFeederContext(YuvFeederContext* ctx, CNMComponentConfig* componentParam) 
{
    osal_memset((void*)ctx, 0, sizeof(YuvFeederContext));
    strcpy(ctx->inputPath, componentParam->testEncConfig.yuvFileName);

    ChangePathStyle(ctx->inputPath);

    ctx->reconFbWidth       = 0;
    ctx->reconFbHeight      = 0;
    ctx->srcFbStride        = 0;
    ctx->fbAllocated        = FALSE;
    ctx->encOpenParam       = componentParam->encOpenParam;
    ctx->yuvMode            = componentParam->testEncConfig.yuv_mode;
    ctx->fbCount.reconFbNum = 0;
    ctx->fbCount.srcFbNum   = 0;
    ctx->frameOutNum        = componentParam->testEncConfig.outNum;
    ctx->rotAngle           = componentParam->testEncConfig.rotAngle;
    ctx->mirDir             = componentParam->testEncConfig.mirDir;
    ctx->mapType            = componentParam->testEncConfig.mapType;
    ctx->subFrameSyncEn     = componentParam->testEncConfig.subFrameSyncEn;

    osal_memset(&ctx->srcFbAllocInfo,          0x00, sizeof(FrameBufferAllocInfo));
    osal_memset(&ctx->reconFbAllocInfo,          0x00, sizeof(FrameBufferAllocInfo));
    osal_memset((void*)ctx->pFbRecon,          0x00, sizeof(ctx->pFbRecon));
    osal_memset((void*)ctx->pFbReconMem,       0x00, sizeof(ctx->pFbReconMem));
    osal_memset((void*)ctx->pFbSrc,            0x00, sizeof(ctx->pFbSrc));
    osal_memset((void*)ctx->pFbSrcMem,         0x00, sizeof(ctx->pFbSrcMem));
    osal_memset((void*)ctx->pFbOffsetTbl,      0x00, sizeof(ctx->pFbOffsetTbl));
    osal_memset((void*)ctx->pFbOffsetTblMem,   0x00, sizeof(ctx->pFbOffsetTblMem));
}

static CNMComponentParamRet GetParameterYuvFeeder(ComponentImpl* from, ComponentImpl* com, GetParameterCMD commandType, void* data) 
{
    YuvFeederContext*    ctx = (YuvFeederContext*)com->context;
    BOOL                 result  = TRUE;
    ParamEncFrameBuffer* allocFb = NULL;

    switch(commandType) {
    case GET_PARAM_YUVFEEDER_FRAME_BUF:
        if (ctx->fbAllocated == FALSE) return CNM_COMPONENT_PARAM_NOT_READY;
        allocFb = (ParamEncFrameBuffer*)data;
        allocFb->reconFb          = ctx->pFbRecon;
        allocFb->reconFbStride    = ctx->reconFbWidth;
        allocFb->reconFbHeight    = ctx->reconFbHeight;
        allocFb->srcFb            = ctx->pFbSrc;
        allocFb->srcFbAllocInfo   = ctx->srcFbAllocInfo;
        allocFb->reconFbAllocInfo = ctx->reconFbAllocInfo;
        break;
    default:
        result = FALSE;
        break;
    }

    return (result == TRUE) ? CNM_COMPONENT_PARAM_SUCCESS : CNM_COMPONENT_PARAM_FAILURE;
}

static CNMComponentParamRet SetParameterYuvFeeder(ComponentImpl* from, ComponentImpl* com, SetParameterCMD commandType, void* data) 
{
    BOOL result = TRUE;

    switch(commandType) {
    default:
        VLOG(ERR, "Unknown SetParameterCMD Type : %d\n", commandType);
        result = FALSE;
        break;
    }

    return (result == TRUE) ? CNM_COMPONENT_PARAM_SUCCESS : CNM_COMPONENT_PARAM_FAILURE;
}

static BOOL AllocateReconFrameBuffer(ComponentImpl* com) {
    YuvFeederContext*       ctx = (YuvFeederContext*)com->context;
    EncOpenParam            encOpenParam = ctx->encOpenParam;
    Uint32                  i = 0;
    Uint32                  reconFbSize   = 0;
    Uint32                  reconFbStride = 0;
    Uint32                  reconFbWidth;
    Uint32                  reconFbHeight;
    FrameBufferAllocInfo    reconFbAllocInfo;
    FrameBufferFormat       reconFrameBufFormat = FORMAT_420;
    DRAMConfig              dramConfig;

    osal_memset(&reconFbAllocInfo,   0x00, sizeof(FrameBufferAllocInfo));
    VPU_EncGiveCommand(ctx->handle, GET_DRAM_CONFIG, &dramConfig);

    reconFbWidth     = (ctx->rotAngle==90||ctx->rotAngle ==270)?encOpenParam.picHeight:encOpenParam.picWidth;
    reconFbHeight    = (ctx->rotAngle==90||ctx->rotAngle ==270)?encOpenParam.picWidth:encOpenParam.picHeight;
    reconFbWidth     = VPU_ALIGN16(reconFbWidth);
    reconFbHeight    = VPU_ALIGN32(reconFbHeight); 

    /* Allocate framebuffers for recon. */
    reconFbStride = CalcStride(reconFbWidth, reconFbHeight, reconFrameBufFormat, encOpenParam.cbcrInterleave, ctx->mapType, FALSE);
    reconFbSize   = VPU_GetFrameBufSize(ctx->handle, encOpenParam.coreIdx, reconFbStride, reconFbHeight, ctx->mapType, reconFrameBufFormat, encOpenParam.cbcrInterleave, &dramConfig);
    
    for (i = 0; i < ctx->fbCount.reconFbNum; i++) {
        ctx->pFbReconMem[i].size = reconFbSize;
        if (vdi_allocate_dma_memory(encOpenParam.coreIdx, &ctx->pFbReconMem[i]) < 0) {
            VLOG(ERR, "fail to allocate recon buffer\n");
            return FALSE;
        }
        ctx->pFbRecon[i].bufY  = ctx->pFbReconMem[i].phys_addr;
        ctx->pFbRecon[i].bufCb = (PhysicalAddress) - 1;
        ctx->pFbRecon[i].bufCr = (PhysicalAddress) - 1;
        ctx->pFbRecon[i].size  = reconFbSize;
        ctx->pFbRecon[i].updateFbInfo = TRUE;
    }

    ctx->reconFbWidth = reconFbWidth;
    ctx->reconFbHeight = reconFbHeight;

    reconFbAllocInfo.mapType = ctx->mapType;
    reconFbAllocInfo.format  = reconFrameBufFormat;
    reconFbAllocInfo.cbcrInterleave = encOpenParam.cbcrInterleave;
    reconFbAllocInfo.stride  = reconFbStride;
    reconFbAllocInfo.height  = reconFbHeight;
    reconFbAllocInfo.endian  = encOpenParam.frameEndian;
    reconFbAllocInfo.type    = FB_TYPE_CODEC;
    reconFbAllocInfo.num     = ctx->fbCount.reconFbNum;
    reconFbAllocInfo.lumaBitDepth = 8;
    reconFbAllocInfo.chromaBitDepth = 8;

    ctx->reconFbAllocInfo = reconFbAllocInfo; 

    return TRUE;
}

static BOOL AllocateYuvBuffer(ComponentImpl* com, Uint32 srcFbWidth, Uint32 srcFbHeight) 
{
    YuvFeederContext*       ctx = (YuvFeederContext*)com->context;
    EncOpenParam            encOpenParam = ctx->encOpenParam;
    Uint32                  i = 0;
    FrameBufferAllocInfo    srcFbAllocInfo;
    Uint32                  srcFbSize    = 0;
    FrameBufferFormat       srcFrameFormat = FORMAT_420;
    DRAMConfig              dramConfig;

    osal_memset(&srcFbAllocInfo,   0x00, sizeof(FrameBufferAllocInfo));
    VPU_EncGiveCommand(ctx->handle, GET_DRAM_CONFIG, &dramConfig);

    /* Allocate framebuffers for source. */
    if (encOpenParam.linear2TiledEnable == TRUE) {
        srcFbAllocInfo.mapType = LINEAR_FRAME_MAP;
        srcFbAllocInfo.stride  = encOpenParam.picWidth;
    }
    else {
        srcFbAllocInfo.mapType = ctx->mapType;
        srcFbAllocInfo.stride  = CalcStride(encOpenParam.picWidth, encOpenParam.picHeight, srcFrameFormat, encOpenParam.cbcrInterleave, (TiledMapType)srcFbAllocInfo.mapType, FALSE);
    }

    srcFbSize   = VPU_GetFrameBufSize(ctx->handle, encOpenParam.coreIdx, srcFbAllocInfo.stride, srcFbHeight, srcFbAllocInfo.mapType, srcFrameFormat, encOpenParam.cbcrInterleave, &dramConfig);

    srcFbAllocInfo.format  = srcFrameFormat;
    srcFbAllocInfo.cbcrInterleave = encOpenParam.cbcrInterleave;
    srcFbAllocInfo.height  = srcFbHeight;
    srcFbAllocInfo.endian  = encOpenParam.frameEndian;
    srcFbAllocInfo.type    = FB_TYPE_PPU;
    srcFbAllocInfo.num     = ctx->fbCount.srcFbNum;
    srcFbAllocInfo.nv21    = encOpenParam.nv21;
    srcFbAllocInfo.lumaBitDepth = 8;
    srcFbAllocInfo.chromaBitDepth = 8;
    srcFbAllocInfo.size = srcFbSize;

    for (i = 0; i < ctx->fbCount.srcFbNum; i++) {
        ctx->pFbSrcMem[i].size = srcFbSize;
        if (vdi_allocate_dma_memory(encOpenParam.coreIdx, &ctx->pFbSrcMem[i]) < 0) {
            VLOG(ERR, "fail to allocate src buffer\n");
            return FALSE;
        }
        ctx->pFbSrc[i].bufY  = ctx->pFbSrcMem[i].phys_addr;
        ctx->pFbSrc[i].bufCb = (PhysicalAddress) - 1;
        ctx->pFbSrc[i].bufCr = (PhysicalAddress) - 1;
        ctx->pFbSrc[i].updateFbInfo = TRUE;
    }

    ctx->srcFbStride      = srcFbAllocInfo.stride;
    ctx->srcFbAllocInfo   = srcFbAllocInfo;

    return TRUE;
}

static BOOL AllocateSourceFrameBuffer(ComponentImpl* com) {
    YuvFeederContext* ctx = (YuvFeederContext*)com->context;
    EncOpenParam    encOpenParam = ctx->encOpenParam;
    Uint32          srcFbWidth   = VPU_ALIGN16(encOpenParam.picWidth);
    Uint32          srcFbHeight  = VPU_ALIGN16(encOpenParam.picHeight);
    SRC_FB_TYPE     srcFbType = SRC_FB_TYPE_YUV;

    if (srcFbType == SRC_FB_TYPE_YUV) {
        if (AllocateYuvBuffer(com, srcFbWidth, srcFbHeight) == FALSE) {
            return FALSE;
        }
    }

    return TRUE;
}

static BOOL PrepareYuvFeeder(ComponentImpl* com, BOOL* done) 
{
    CNMComponentParamRet    ret;
    YuvFeederContext*       ctx = (YuvFeederContext*)com->context;
    EncOpenParam*           encOpenParam = &ctx->encOpenParam;
    BOOL                    success;
    Uint32                  i;
    YuvFeeder               yuvFeeder    = NULL;
    YuvInfo                 yuvFeederInfo;

    *done = FALSE;
    // wait signal GET_PARAM_ENC_FRAME_BUF_NUM
    ret = ComponentGetParameter(com, com->sinkPort.connectedComponent, GET_PARAM_ENC_FRAME_BUF_NUM, &ctx->fbCount);
    if (ComponentParamReturnTest(ret, &success) == FALSE) {
        return success;
    }

    ret = ComponentGetParameter(com, com->sinkPort.connectedComponent, GET_PARAM_ENC_HANDLE, &ctx->handle);
    if (ComponentParamReturnTest(ret, &success) == FALSE) {
        return success;
    }

    if (AllocateReconFrameBuffer(com) == FALSE) {
        VLOG(ERR, "AllocateReconFrameBuffer error\n");
        return FALSE;
    }

    if (AllocateSourceFrameBuffer(com) == FALSE) {
        VLOG(ERR, "AllocateSourceFrameBuffer error\n");
        return FALSE;
    }

    osal_memset(&yuvFeederInfo,   0x00, sizeof(YuvInfo));

    yuvFeederInfo.cbcrInterleave = encOpenParam->cbcrInterleave;
    yuvFeederInfo.packedFormat   = encOpenParam->packedFormat;
    yuvFeederInfo.srcFormat      = encOpenParam->srcFormat;
    yuvFeederInfo.srcStride      = ctx->srcFbStride;
    yuvFeederInfo.srcHeight      = encOpenParam->picHeight;
    // If YUV is NV12 or NV21 format, the value of srcPlanar should be FALSE.
    yuvFeederInfo.srcPlanar      = TRUE;
    yuvFeederInfo.nv21           = encOpenParam->nv21;
    yuvFeeder = YuvFeeder_Create(ctx->yuvMode, ctx->inputPath, yuvFeederInfo);
    if (yuvFeeder == NULL) {
        VLOG(ERR, "YuvFeeder_Create Error\n");
        return FALSE;
    }

    ((AbstractYuvFeeder*)yuvFeeder)->impl->handle = ctx->handle;
    ctx->yuvFeeder = yuvFeeder;

    // Fill data into the input queue of the sink port.
    // The empty input queue of the sink port occurs a hangup problem in this example.
    while (Queue_Dequeue(com->sinkPort.inputQ) != NULL);
    for (i = 0; i < com->sinkPort.inputQ->size; i++) {
        PortContainerYuv defaultData; 

        osal_memset(&defaultData, 0x00, sizeof(PortContainerYuv));
        defaultData.srcFbIndex   = -1;
        if (i<ctx->fbCount.srcFbNum) {
            //put index value in srcFbindex
            //and srcFbindex value is reused in out Q sink-port
            defaultData.srcFbIndex   = i;

        Queue_Enqueue(com->sinkPort.inputQ, (void*)&defaultData);
        }
    }

    ctx->fbAllocated = TRUE;

    *done = TRUE;

    return TRUE;
}

static BOOL ExecuteYuvFeeder(ComponentImpl* com, PortContainer* in, PortContainer* out) 
{
    YuvFeederContext*   ctx          = (YuvFeederContext*)com->context;
    EncOpenParam*       encOpenParam = &ctx->encOpenParam;
    int                 ret          = 0;
    PortContainerYuv*   sinkData     = (PortContainerYuv*)out;

    out->reuse = FALSE;
    if (ctx->state == YUV_FEEDER_STATE_WAIT) {
        CNMComponentParamRet ParamRet;
        BOOL                 success, done = FALSE;

        out->reuse = TRUE;
        ParamRet = ComponentGetParameter(com, com->sinkPort.connectedComponent, GET_PARAM_ENC_FRAME_BUF_REGISTERED, &done);
        if (ComponentParamReturnTest(ParamRet, &success) == FALSE) {
            return success;
        } else if (done == FALSE) {
            return TRUE;
        } else {
            VPU_EncGiveCommand(ctx->handle, GET_TILEDMAP_CONFIG, &(ctx->mapConfig));
            ctx->state = YUV_FEEDER_STATE_FEEDING;
            out->reuse = FALSE;
        }
    }

    if ( ctx->last ) {
        sinkData->last = TRUE;
        return TRUE;
    }

    sinkData->fb              = ctx->pFbSrc[sinkData->srcFbIndex];

    ctx->feedingNum++;
    if (ctx->feedingNum > ctx->frameOutNum && ctx->frameOutNum != -1) {
        ctx->last = sinkData->last = TRUE;
        return TRUE;
    }

    ret = YuvFeeder_Feed(ctx->yuvFeeder, encOpenParam->coreIdx, &sinkData->fb, encOpenParam->picWidth, encOpenParam->picHeight, sinkData->srcFbIndex, &(ctx->mapConfig));
    //VLOG(TRACE, "feeding Num (%d), srcIndex (%d)\n", ctx->feedingNum, sinkData->srcFbIndex);

    if (FALSE == ret) {
        ctx->last = sinkData->last = TRUE;
    }

    if (TRUE == ret) {
        if (ctx->subFrameSyncEn == TRUE) {
            ctx->subFrameSyncInfo.subFrameSyncCfg.sourceBufIndexBase  = ctx->pFbSrc[sinkData->srcFbIndex].myIndex;
            VPU_EncGiveCommand(ctx->handle, ENC_CONFIG_SUB_FRAME_SYNC, &(ctx->subFrameSyncInfo.subFrameSyncCfg));
            ctx->subFrameSyncInfo.subFrameSyncState.ipuEndOfRow = 0;
            ctx->subFrameSyncInfo.subFrameSyncState.ipuNewFrame = 1;
            ctx->subFrameSyncInfo.subFrameSyncState.ipuCurFrameIndex = 1;	// 
            VPU_EncGiveCommand(ctx->handle, ENC_SET_SUB_FRAME_SYNC, &(ctx->subFrameSyncInfo.subFrameSyncState));
            ComponentSetParameter(com, com->sinkPort.connectedComponent, SET_PARAM_ENC_SUBFRAMESYNC, &(ctx->subFrameSyncInfo));
        }
    }


    return TRUE;
}

static void ReleaseYuvFeeder(ComponentImpl* com)
{
    YuvFeederContext* ctx = (YuvFeederContext*)com->context;
    Uint32          i   = 0;

    for (i = 0; i < ctx->fbCount.reconFbNum*2; i++) {
        if (ctx->pFbReconMem[i].size)
            vdi_free_dma_memory(ctx->encOpenParam.coreIdx, &ctx->pFbReconMem[i]);
    }
    for (i = 0; i < ctx->fbCount.srcFbNum; i++) {
        if (ctx->pFbSrcMem[i].size)
            vdi_free_dma_memory(ctx->encOpenParam.coreIdx, &ctx->pFbSrcMem[i]);
        if (ctx->pFbOffsetTblMem[i].size)
            vdi_free_dma_memory(ctx->encOpenParam.coreIdx, &ctx->pFbOffsetTblMem[i]);
    }
}

static BOOL DestroyYuvFeeder(ComponentImpl* com) 
{
    YuvFeederContext* ctx = (YuvFeederContext*)com->context;

    if (ctx->yuvFeeder)   YuvFeeder_Destroy(ctx->yuvFeeder);

    osal_free(ctx);

    return TRUE;
}

static Component CreateYuvFeeder(ComponentImpl* com, CNMComponentConfig* componentParam) 
{
    YuvFeederContext*   ctx; 

    com->context = osal_malloc(sizeof(YuvFeederContext));
    ctx          = (YuvFeederContext*)com->context;

    InitYuvFeederContext(ctx, componentParam);

    if ( ctx->encOpenParam.sourceBufCount > com->numSinkPortQueue )
        com->numSinkPortQueue = ctx->encOpenParam.sourceBufCount;//set requested sourceBufCount

    return (Component)com;
}

ComponentImpl coda9YuvFeederComponentImpl = {
    "coda9_yuvfeeder",
    NULL,
    {0,},
    {0,},
    sizeof(PortContainerYuv),
    ENC_SRC_BUF_NUM,              /* minimum feeder's numSinkPortQueue(relates to source buffer count)*/
    CreateYuvFeeder,
    GetParameterYuvFeeder,
    SetParameterYuvFeeder,
    PrepareYuvFeeder,
    ExecuteYuvFeeder,
    ReleaseYuvFeeder,
    DestroyYuvFeeder
};

