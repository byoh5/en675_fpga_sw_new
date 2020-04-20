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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "main_helper.h"
#include "cnm_app.h"
#include "decoder_listener.h"
#include "encoder_listener.h"
//#include "vpuconfig.h"

enum waitStatus {
    WAIT_AFTER_INIT         = 0,

    WAIT_AFTER_DEC_OPEN     = 1,
    WAIT_AFTER_SEQ_INIT     = 2,
    WAIT_AFTER_REG_BUF      = 3,
    WAIT_BEFORE_DEC_START   = 4,
    WAIT_BEFORE_DEC_CLOSE   = 5,

    WAIT_AFTER_ENC_OPEN     = 6,
    WAIT_AFTER_INIT_SEQ     = 7,
    WAIT_AFTER_REG_FRAME    = 8,
    WAIT_BEFORE_ENC_START   = 9,
    WAIT_BEFORE_ENC_CLOSE   = 10,
    WAIT_NOT_DEFINED        = 11,
    WAIT_STATUS_MAX
};

enum get_result_status{
    GET_RESULT_INIT,
    GET_RESULT_BEFORE_WAIT_INT, //before calling wait interrupt.
    GET_RESULT_AFTER_WAIT_INT   //got interrupt.
};

static Uint32           sizeInWord;
static Uint16*          pusBitCode;


typedef struct {
    char             inputFilePath[256];
    char             localFile[256];
    char             outputFilePath[256];
    char             refFilePath[256];
    char             yuvpath[256];
    CodStd           stdMode;
    BOOL             afbce;
    BOOL             afbcd;
    BOOL             scaler;
    size_t           sclw;
    size_t           sclh;
    Int32            bsmode;
    BOOL             enableWTL;
    BOOL             enableMVC;
    int              compareType;
    TestDecConfig    decConfig;
    TestEncConfig    encConfig;
    BOOL             isEncoder;
    int 			 width;
    int				 height;
    int 			 outNum;
} InstTestConfig;

typedef struct {
    Uint32          totProcNum;             /* the number of process */
    Uint32          curProcNum;
    Uint32          numMulti;
    Uint32          cores;
    InstTestConfig  instConfig[MAX_NUM_INSTANCE];
} TestMultiConfig;

#ifdef NONOS_CODEC_RUNNING
DecListenerContext* decListenerCtx = NULL;
EncListenerContext* encListenerCtx = NULL;

TestMultiConfig multiConfig;
int task_index = 0;
Uint32          TaskTerminate[4];
#endif

static void WaitForNextStep(Int32 instIdx, Int32 curStepName)
{
}

static void Help(const char *programName)
{
    VLOG(INFO, "------------------------------------------------------------------------------\n");
    VLOG(INFO, "%s(API v%d.%d.%d)\n", GetBasename(programName), API_VERSION_MAJOR, API_VERSION_MINOR, API_VERSION_PATCH);
    VLOG(INFO, "\tAll rights reserved by Chips&Media(C)\n");
    VLOG(INFO, "\tSample program controlling the Chips&Media VPU\n");
    VLOG(INFO, "------------------------------------------------------------------------------\n");
    VLOG(INFO, "%s [option] --input <stream list file, aka cmd file>\n", GetBasename(programName));
    VLOG(INFO, "-h                          help\n");
    VLOG(INFO, "-c                          enable comparison mode\n");
    VLOG(INFO, "                            1 : compare with golden stream that specified --ref_file_path option\n");
    VLOG(INFO, "-e                          0 : decoder, 1: encoder\n");
    VLOG(INFO, "--codec                     The index of codec\n");
    VLOG(INFO, "                            AVC = 0, VC1 = 1, MPEG2 = 2, MPEG4 = 3, H263 = 4\n");
    VLOG(INFO, "                            DIV3 = 5, RV = 6, AVS = 7, THO = 9, VP3 = 10, VP8 = 11, HEVC = 12, VP9 = 13\n");
    VLOG(INFO, "                            AVC_ENC = 14, MP4_ENC = 17, H263_ENC = 18, HEVC_ENC = 26\n");
    VLOG(INFO, "--input                     bitstream(decoder) or cfg(encoder) path\n");
    VLOG(INFO, "--output                    yuv(decoder) or bitstream(encoder) path\n");
    VLOG(INFO, "--ref_file_path             Golden md5 or stream path\n");
    VLOG(INFO, "--sfs                       subFrameSync 0: off 1: on, default off\n");
    VLOG(INFO, "--bsmode                    set bitstream mode.\n");
    VLOG(INFO, "--enable-wtl                enable wtl option. default 0\n");
}

void SetEncMultiParam(
    TestMultiConfig* multiConfig,
    int idx,
    Int32 productId
    )
{
    InstTestConfig *instConfig = &multiConfig->instConfig[idx];
    TestEncConfig *encCfg = &instConfig->encConfig;

    strcpy(encCfg->yuvFileName, multiConfig->instConfig[idx].yuvpath);

    encCfg->stdMode       = multiConfig->instConfig[idx].stdMode;
    encCfg->frame_endian  = 0;
    encCfg->stream_endian = 0;
    encCfg->yuv_mode      = YUV_MODE_YUV;//YUV_MODE_YUV_LOADER;
    encCfg->mapType       = LINEAR_FRAME_MAP;

    encCfg->cbcrInterleave = 1;
    encCfg->nv21 = 0;

    if (TRUE == PRODUCT_ID_W_SERIES(productId)) {
        encCfg->mapType   = COMPRESSED_FRAME_MAP;
    }

    strcpy(encCfg->cfgFileName, multiConfig->instConfig[idx].inputFilePath);
    strcpy(encCfg->bitstreamFileName, multiConfig->instConfig[idx].outputFilePath);
    if (TRUE == PRODUCT_ID_W_SERIES(productId)) {
        if (multiConfig->instConfig[idx].compareType == MD5_COMPARE) {
            encCfg->compareType |= (1 << MODE_COMP_RECON);
            strcpy(encCfg->ref_recon_md5_path, multiConfig->instConfig[idx].refFilePath);
        }
        else if (multiConfig->instConfig[idx].compareType == STREAM_COMPARE) {
            encCfg->compareType |= (1 << MODE_COMP_ENCODED);
            strcpy(encCfg->ref_stream_path, multiConfig->instConfig[idx].refFilePath);
        }
    }
    else { // coda9 encoder case
        char imageRootDir[256]="./yuv";
        encCfg->compareType = (TRUE == (BOOL)multiConfig->instConfig[idx].compareType) ? STREAM_COMPARE : NO_COMPARE;
        strcpy(encCfg->ref_stream_path, multiConfig->instConfig[idx].refFilePath);
        strcpy(encCfg->yuvSourceBaseDir, imageRootDir);
        multiConfig->instConfig[idx].encConfig.lineBufIntEn = TRUE;
    }

    //encCfg->srcFormat = FORMAT_420_P10_16BIT_LSB;


    encCfg->outNum = multiConfig->instConfig[idx].outNum;//1;//0;

    encCfg->coreIdx = 0;

	encCfg->picWidth = multiConfig->instConfig[idx].width;
	encCfg->picHeight = multiConfig->instConfig[idx].height;

    encCfg->kbps = 1000;

    encCfg->subFrameSyncEn = 0;
    encCfg->lowLatencyMode = 0;
    encCfg->rotAngle = 0;
    encCfg->mirDir = 0;
    encCfg->secondaryAXI = 0;
    encCfg->frame_endian = VDI_128BIT_LITTLE_ENDIAN;
    encCfg->stream_endian = VDI_128BIT_LITTLE_ENDIAN;

    encCfg->source_endian = VDI_128BIT_LITTLE_ENDIAN;

    encCfg->srcFormat = FORMAT_420;
    ///////////////////
    multiConfig->instConfig[idx].encConfig.subFrameSyncMode = WIRED_BASE_SUB_FRAME_SYNC;//REGISTER_BASE_SUB_FRAME_SYNC;
    multiConfig->instConfig[idx].encConfig.productId = productId;
}

void SetEncMultiRandomParam(
    TestMultiConfig* multiConfig,
    int idx,
    ProductId productId
    )
{
    char img_base_dir[256]="/nstream/qctool/img/";
    char        tmp[200];
    InstTestConfig *instConfig = &multiConfig->instConfig[idx];
    TestEncConfig *encCfg = &instConfig->encConfig;

    UNREFERENCED_PARAMETER(tmp[0]);

    encCfg->stdMode       = instConfig->stdMode;
    encCfg->frame_endian  = 0;
    encCfg->stream_endian = 0;
    encCfg->yuv_mode      = YUV_MODE_YUV;//YUV_MODE_YUV_LOADER;
    encCfg->mapType       = LINEAR_FRAME_MAP;
    if (PRODUCT_ID_W_SERIES(productId)) {
        encCfg->mapType        = COMPRESSED_FRAME_MAP;
    }

    strcpy(encCfg->cfgFileName, instConfig->inputFilePath);
    strcpy(encCfg->bitstreamFileName, instConfig->outputFilePath);

    if (instConfig->stdMode == STD_HEVC || instConfig->stdMode == STD_SVAC || instConfig->stdMode == STD_AVC ) {
        if (instConfig->compareType == MD5_COMPARE) {
            encCfg->compareType |= (1 << MODE_COMP_RECON);
            strcpy(encCfg->ref_recon_md5_path, instConfig->refFilePath);
        }
        else {
            encCfg->compareType |= (1 << MODE_COMP_ENCODED);
            strcpy(encCfg->ref_stream_path, instConfig->refFilePath);
        }
    }
    else { // coda9 encoder case
        encCfg->compareType = (TRUE == (BOOL)instConfig->compareType) ? STREAM_COMPARE : NO_COMPARE;
        strcpy(encCfg->ref_stream_path, instConfig->refFilePath);
        strcpy(encCfg->yuvSourceBaseDir, img_base_dir);
    }
    multiConfig->instConfig[idx].encConfig.subFrameSyncMode = REGISTER_BASE_SUB_FRAME_SYNC;
}

void SetDecMultiParam(
    TestMultiConfig* multiConfig,
    int idx,
    Int32 productId
    )
{
    multiConfig->instConfig[idx].decConfig.bitFormat            = multiConfig->instConfig[idx].stdMode;
    multiConfig->instConfig[idx].decConfig.bitstreamMode        = multiConfig->instConfig[idx].bsmode;
    multiConfig->instConfig[idx].decConfig.feedingMode          = (multiConfig->instConfig[idx].bsmode == BS_MODE_INTERRUPT) ? FEEDING_METHOD_FIXED_SIZE : FEEDING_METHOD_FRAME_SIZE;
    multiConfig->instConfig[idx].decConfig.streamEndian         = VPU_STREAM_ENDIAN;
    multiConfig->instConfig[idx].decConfig.frameEndian          = VPU_FRAME_ENDIAN;
    multiConfig->instConfig[idx].decConfig.cbcrInterleave       = 1;//FALSE;
    multiConfig->instConfig[idx].decConfig.nv21                 = FALSE;
    multiConfig->instConfig[idx].decConfig.enableWTL            = multiConfig->instConfig[idx].enableWTL;
    multiConfig->instConfig[idx].decConfig.coda9.enableMvc      = multiConfig->instConfig[idx].enableMVC;
    multiConfig->instConfig[idx].decConfig.wtlMode              = FF_FRAME;
    multiConfig->instConfig[idx].decConfig.wtlFormat            = FORMAT_420;
    if (TRUE == PRODUCT_ID_W_SERIES(productId)) {
        multiConfig->instConfig[idx].decConfig.mapType          = COMPRESSED_FRAME_MAP;
        multiConfig->instConfig[idx].decConfig.wave.bwOptimization  = TRUE;
#if 0//def EYENIX_TEST
        if(multiConfig->instConfig[idx].decConfig.bitFormat==STD_AVC)
            multiConfig->instConfig[idx].decConfig.bsSize               = (2*1024*1024);
        else
#endif
        	multiConfig->instConfig[idx].decConfig.bsSize               = (12*1024*1024);
    }
    else {
        multiConfig->instConfig[idx].decConfig.mapType          = LINEAR_FRAME_MAP;
#if 0//def EYENIX_TEST
        if(multiConfig->instConfig[idx].decConfig.bitFormat==STD_AVC)
        	multiConfig->instConfig[idx].decConfig.bsSize           = (1*1024*1024);
        else
#endif
        	multiConfig->instConfig[idx].decConfig.bsSize           = (5*1024*1024);
    }
    multiConfig->instConfig[idx].decConfig.forceOutNum          = 0;
    multiConfig->instConfig[idx].decConfig.productId            = productId;


    strcpy(multiConfig->instConfig[idx].decConfig.inputPath, multiConfig->instConfig[idx].inputFilePath);
    if (multiConfig->instConfig[idx].localFile[0]) {
        strcpy(multiConfig->instConfig[idx].decConfig.inputPath, multiConfig->instConfig[idx].localFile);
    }
    strcpy(multiConfig->instConfig[idx].decConfig.outputPath, multiConfig->instConfig[idx].outputFilePath);
    if (multiConfig->instConfig[idx].compareType == MD5_COMPARE) {
        multiConfig->instConfig[idx].decConfig.compareType = MD5_COMPARE;
        strcpy(multiConfig->instConfig[idx].decConfig.md5Path, multiConfig->instConfig[idx].refFilePath);
    }
    else if (multiConfig->instConfig[idx].compareType == YUV_COMPARE) {
        multiConfig->instConfig[idx].decConfig.compareType = YUV_COMPARE;
        strcpy(multiConfig->instConfig[idx].decConfig.refYuvPath, multiConfig->instConfig[idx].refFilePath);
    }
    else {
        multiConfig->instConfig[idx].decConfig.compareType = 0;
    }




    multiConfig->instConfig[idx].decConfig.forceOutNum = multiConfig->instConfig[idx].outNum;//-1;


    /*0 : no rendering picture
      1 : render a picture with the framebuffer device */
    multiConfig->instConfig[idx].decConfig.renderType = 0;
    multiConfig->instConfig[idx].decConfig.coreIdx = 0;

    //integer number. loop test, default 0
    multiConfig->instConfig[idx].decConfig.loopCount = 0;

    multiConfig->instConfig[idx].decConfig.secondaryAXI = 0;

    //number vcores to be used, default 1
    multiConfig->instConfig[idx].decConfig.wave.numVCores = 1;

    //enable thumbnail mode. default off
    multiConfig->instConfig[idx].decConfig.thumbnailMode = FALSE;//TRUE;

    //golden yuv path
    //osal_memcpy(testConfig->refYuvPath, optarg, strlen(optarg));
    //ChangePathStyle(testConfig->refYuvPath);

    //0: off, 1: Non-IRAP, 2: Non-Ref, 4: skip-all
    multiConfig->instConfig[idx].decConfig.skipMode = 0;

    //bandwidth opimization
    multiConfig->instConfig[idx].decConfig.wave.bwOptimization = 0;

    //Handle CRA as BLA
    multiConfig->instConfig[idx].decConfig.wave.craAsBla = TRUE;//FALSE;//

    // hexadecimal           0 - disable userdata            4 - VUI                     10 - pic_timing                 20 - itu_t_35 prefix\n"
    //                      40 - unregistered prefix        80 - itu_t_35 suffix        100 - unregistered suffix       400 - mastering_col_vol\n"
    //                     800 - chroma resample hint     1000 - knee function         2000 - tone mapping             4000 - film grain\n"
    //                    8000 - content light level     10000 - color remapping   10000000 - itu_t_35 prefix 1    20000000 - itu_t_35 prefix 2\n"
    //                40000000 - itu_t_35 suffix 1    80000000 - itu_t_35 suffix 2 \n"
    //                All bits can be or-ing. ex) 404 -> mastering_color_volume | VUI\n"},
    multiConfig->instConfig[idx].decConfig.enableUserData = 0;


    //0: auto, 1: fixed-size, 2: ffempg, 3: size(4byte)+es
    //argFeeding = 0;

    //osal_memcpy(testConfig->fwPath, optarg, strlen(optarg));

    //random seed value, Hexadecimal
    //randomSeed = strtoul(optarg, NULL, 16);
    //srand(randomSeed);

    //testConfig->bsSize = (size_t)atoi(optarg);


	//switch (argFeeding) {
	//case 0: /* AUTO */
	if (multiConfig->instConfig[idx].decConfig.bitstreamMode == BS_MODE_INTERRUPT) {
		multiConfig->instConfig[idx].decConfig.feedingMode = FEEDING_METHOD_FIXED_SIZE;
	}
	else {
		multiConfig->instConfig[idx].decConfig.feedingMode = FEEDING_METHOD_FRAME_SIZE;
	}

}

void SetDecMultiRandomParam(
    TestMultiConfig* multiConfig,
    int idx,
    ProductId   productId
    )
{
    InstTestConfig *instConfig = &multiConfig->instConfig[idx];
    TestDecConfig *decCfg = &instConfig->decConfig;

    decCfg->bitFormat            = instConfig->stdMode;
    decCfg->bitstreamMode        = instConfig->bsmode;
    decCfg->feedingMode          = (instConfig->bsmode == BS_MODE_INTERRUPT) ? FEEDING_METHOD_FIXED_SIZE : FEEDING_METHOD_FRAME_SIZE;
    if (PRODUCT_ID_960 == productId || PRODUCT_ID_980 == productId) {
        decCfg->streamEndian         = GetRandom(0, 3);
        decCfg->frameEndian          = GetRandom(0, 3);
    }
    else {
        decCfg->streamEndian         = GetRandom(16, 31);
        decCfg->frameEndian          = GetRandom(16, 31);
    }

    decCfg->cbcrInterleave       = FALSE;
    decCfg->nv21                 = FALSE;
    decCfg->enableWTL            = instConfig->enableWTL;
    decCfg->coda9.enableMvc      = instConfig->enableMVC;
    decCfg->wtlMode              = FF_FRAME;
    decCfg->wtlFormat            = FORMAT_420;
    decCfg->mapType              = LINEAR_FRAME_MAP;
    if (TRUE == PRODUCT_ID_W_SERIES(productId)) {
        decCfg->mapType          = COMPRESSED_FRAME_MAP;
    }
    decCfg->bsSize               = (12*1024*1024);
    decCfg->forceOutNum          = 0;
    decCfg->wave.bwOptimization  = TRUE;      // see MIC-2704, MDR-28

    strcpy(decCfg->inputPath, instConfig->inputFilePath);
    if (instConfig->localFile[0]) {
        strcpy(decCfg->inputPath, instConfig->localFile);
    }
    strcpy(decCfg->outputPath, instConfig->outputFilePath);
    if (instConfig->compareType == MD5_COMPARE) {
        decCfg->compareType = MD5_COMPARE;
        strcpy(decCfg->md5Path, instConfig->refFilePath);
    }
    else if(instConfig->compareType == YUV_COMPARE ) {
        decCfg->compareType = YUV_COMPARE;
        strcpy(decCfg->refYuvPath, instConfig->refFilePath);
    }
    else {
        decCfg->compareType = 0;
        strcpy(decCfg->refYuvPath, instConfig->refFilePath);
    }
}

static void MultiDecoderListener(Component com, Uint64 event, void* data, void* context)
{
    DecHandle               decHandle = NULL;

    ComponentGetParameter(NULL, com, GET_PARAM_DEC_HANDLE, &decHandle);
    if (decHandle == NULL && event != COMPONENT_EVENT_DEC_DECODED_ALL) {
        // Terminated state
        return;
    }

    switch (event) {
    case COMPONENT_EVENT_DEC_OPEN:
        WaitForNextStep(decHandle->instIndex, WAIT_AFTER_DEC_OPEN);
        break;
    case COMPONENT_EVENT_DEC_COMPLETE_SEQ:
        WaitForNextStep(decHandle->instIndex, WAIT_AFTER_SEQ_INIT);
        HandleDecCompleteSeqEvent(com, (CNMComListenerDecCompleteSeq*)data, (DecListenerContext*)context);
        break;
    case COMPONENT_EVENT_DEC_REGISTER_FB:
        WaitForNextStep(decHandle->instIndex, WAIT_AFTER_REG_BUF);
        break;
    case COMPONENT_EVENT_DEC_READY_ONE_FRAME:
        WaitForNextStep(decHandle->instIndex, WAIT_BEFORE_DEC_START);
        break;
    case COMPONENT_EVENT_DEC_GET_OUTPUT_INFO:
        HandleDecGetOutputEvent(com, (CNMComListenerDecDone*)data, (DecListenerContext*)context);
        break;
    case COMPONENT_EVENT_DEC_DECODED_ALL:
        // It isn't possible to get handle when a component is terminated state.
        decHandle = (DecHandle)data;
        if (decHandle) WaitForNextStep(decHandle->instIndex, WAIT_BEFORE_DEC_CLOSE);
        break;
    default:
        break;
    }
}

static BOOL CreateDecoderTask(CNMTask task, CNMComponentConfig* config, DecListenerContext* lsnCtx)
{
    BOOL        success = FALSE;
    RetCode         ret = RETCODE_SUCCESS;

    if (RETCODE_SUCCESS != (ret=SetUpDecoderOpenParam(&(config->decOpenParam), &(config->testDecConfig)))) {
        VLOG(ERR, "%s:%d SetUpDecoderOpenParam failed Error code is 0x%x \n", __FUNCTION__, __LINE__, ret);
        return FALSE;
    }

    Component feeder   = ComponentCreate("feeder", config);
    Component renderer = ComponentCreate("renderer", config);
    Component decoder  = ComponentCreate("wave_decoder",  config);

    CNMTaskAdd(task, feeder);
    CNMTaskAdd(task, decoder);
    CNMTaskAdd(task, renderer);

    if ((success=SetupDecListenerContext(lsnCtx, config, renderer)) == TRUE) {
        ComponentRegisterListener(decoder, COMPONENT_EVENT_DEC_ALL, MultiDecoderListener, (void*)lsnCtx);
    }

    return success;
}

static void HandleEncFullEvent(Component com, CNMComListenerEncFull* param, EncListenerContext* ctx)
{
}


static void MultiEncoderListener(Component com, Uint64 event, void* data, void* context)
{
    EncHandle                       encHandle   = NULL;
    CNMComListenerEncStartOneFrame* lsnStartEnc = NULL;
    ProductId                       productId;

    UNREFERENCED_PARAMETER(lsnStartEnc);

    ComponentGetParameter(NULL, com, GET_PARAM_ENC_HANDLE, &encHandle);
    if (encHandle == NULL && event != COMPONENT_EVENT_ENC_ENCODED_ALL) {
        // Terminated state
        return;
    }

    switch (event) {
    case COMPONENT_EVENT_ENC_OPEN:
        WaitForNextStep(encHandle->instIndex, WAIT_AFTER_ENC_OPEN);
        break;
    case COMPONENT_EVENT_ENC_COMPLETE_SEQ:
        WaitForNextStep(encHandle->instIndex, WAIT_AFTER_INIT_SEQ);
        break;
    case COMPONENT_EVENT_ENC_REGISTER_FB:
        WaitForNextStep(encHandle->instIndex, WAIT_AFTER_REG_FRAME);
        break;
    case COMPONENT_EVENT_ENC_READY_ONE_FRAME:
        WaitForNextStep(encHandle->instIndex, WAIT_BEFORE_ENC_START);
        break;
    case COMPONENT_EVENT_ENC_START_ONE_FRAME:
        break;
    case COMPONENT_EVENT_ENC_GET_OUTPUT_INFO:
        encHandle = ((CNMComListenerEncDone*)data)->handle;
        productId = VPU_GetProductId(VPU_HANDLE_CORE_INDEX(encHandle));
        if (TRUE == PRODUCT_ID_W_SERIES(productId)) {
            HandleEncGetOutputEvent(com, (CNMComListenerEncDone*)data, (EncListenerContext*)context);
        }
        else {
            Coda9HandleEncGetOutputEvent(com, (CNMComListenerEncDone*)data, (EncListenerContext*)context);
        }
        break;
    case COMPONENT_EVENT_CODA9_ENC_MAKE_HEADER:
        Coda9HandleEncMakeHeaderEvent(com, (CNMComListenerEncMakeHeader*)data, (EncListenerContext*)context);
        break;
    case COMPONENT_EVENT_ENC_ENCODED_ALL:
        // It isn't possible to get handle when a component is terminated state.
        encHandle = (EncHandle)data;
        if (encHandle)
            WaitForNextStep(encHandle->instIndex, WAIT_BEFORE_ENC_CLOSE);
        break;
    case COMPONENT_EVENT_ENC_FULL_INTERRUPT:
        HandleEncFullEvent(com, (CNMComListenerEncFull*)data, (EncListenerContext*)context);
        break;
    default:
        break;
    }
}

static BOOL CreateEncoderTask(CNMTask task, CNMComponentConfig* config, EncListenerContext* lsnCtx)
{
    Component feeder;
    Component encoder;
    Component reader;
    BOOL      success = FALSE;

    if (SetupEncoderOpenParam(&config->encOpenParam, &config->testEncConfig, NULL) == FALSE) {
        return FALSE;
    }

    feeder  = ComponentCreate("yuvfeeder",      config);
    encoder = ComponentCreate("wave_encoder",   config);
    reader  = ComponentCreate("reader",         config);

    CNMTaskAdd(task, feeder);
    CNMTaskAdd(task, encoder);
    CNMTaskAdd(task, reader);

    if ((success=SetupEncListenerContext(lsnCtx, config)) == TRUE) {
        ComponentRegisterListener(encoder, COMPONENT_EVENT_ENC_ALL, MultiEncoderListener, (void*)lsnCtx);
    }

    return success;
}

#ifdef NONOS_CODEC_RUNNING
static BOOL MultiInstanceInit(TestMultiConfig* multiConfig, Uint16* fw, Uint32 size)
{
    Uint32              i;
    CNMComponentConfig  config;
    CNMTask             task;
    BOOL                ret     = FALSE;
    BOOL                success = TRUE;
    BOOL                match   = TRUE;
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);

    if(decListenerCtx == NULL)
    	decListenerCtx = (DecListenerContext*)osal_malloc(sizeof(DecListenerContext) * MAX_NUM_INSTANCE);
    if(encListenerCtx == NULL)
    	encListenerCtx = (EncListenerContext*)osal_malloc(sizeof(EncListenerContext) * MAX_NUM_INSTANCE);

    CNMAppInit();
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    for (i=0; i < multiConfig->numMulti; i++) {
        task = CNMTaskCreate();
        memset((void*)&config, 0x00, sizeof(CNMComponentConfig));
        config.bitcode       = (Uint8*)fw;
        config.sizeOfBitcode = size;
        if (multiConfig->instConfig[i].isEncoder == TRUE) {
            memcpy((void*)&config.testEncConfig, &multiConfig->instConfig[i].encConfig, sizeof(TestEncConfig));
            success = CreateEncoderTask(task, &config, &encListenerCtx[i]);
        }
        else
        {
            memcpy((void*)&config.testDecConfig, &multiConfig->instConfig[i].decConfig, sizeof(TestDecConfig));
            success = CreateDecoderTask(task, &config, &decListenerCtx[i]);
        }
        CNMAppAdd(task);
        if (success == FALSE) {
            CNMAppStop();
            break;
        }
    }
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    return ret;
}

static BOOL MultiInstanceClear(TestMultiConfig* multiConfig)
{
    Uint32              i;
    CNMComponentConfig  config;
    CNMTask             task;
    BOOL                ret     = FALSE;
    BOOL                success = TRUE;
    BOOL                match   = TRUE;

    CNMAppTerminateTask();

    for (i=0; i<multiConfig->numMulti; i++) {
        if (multiConfig->instConfig[i].isEncoder == TRUE) {
            match &= (encListenerCtx[i].match == TRUE && encListenerCtx[i].matchOtherInfo == TRUE);
            ClearEncListenerContext(&encListenerCtx[i]);
        }
        else
        {
            match &= decListenerCtx[i].match;
            ClearDecListenerContext(&decListenerCtx[i]);
        }
    }dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    if (ret == TRUE) ret = match;
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    osal_free(decListenerCtx);
    osal_free(encListenerCtx);
    if (NULL != GetPendingInst(0)) {
        VLOG(ERR, "There are pending instances\n");
        while(TRUE) osal_msleep(100);
    }
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    return ret;
}

static BOOL MultiInstanceRun(int task_index)
{
	BOOL                ret     = FALSE;

	ret = CNMAppRunTask(task_index);

	return ret;
}

#endif

static BOOL MultiInstanceTest(TestMultiConfig* multiConfig, Uint16* fw, Uint32 size)
{
    Uint32              i;
    CNMComponentConfig  config;
    CNMTask             task;
    DecListenerContext* decListenerCtx = (DecListenerContext*)osal_malloc(sizeof(DecListenerContext) * MAX_NUM_INSTANCE);
    EncListenerContext* encListenerCtx = (EncListenerContext*)osal_malloc(sizeof(EncListenerContext) * MAX_NUM_INSTANCE);
    BOOL                ret     = FALSE;
    BOOL                success = TRUE;
    BOOL                match   = TRUE;
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    CNMAppInit();
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    for (i=0; i < multiConfig->numMulti; i++) {
        task = CNMTaskCreate();
        memset((void*)&config, 0x00, sizeof(CNMComponentConfig));
        config.bitcode       = (Uint8*)fw;
        config.sizeOfBitcode = size;
        if (multiConfig->instConfig[i].isEncoder == TRUE) {
            memcpy((void*)&config.testEncConfig, &multiConfig->instConfig[i].encConfig, sizeof(TestEncConfig));
            success = CreateEncoderTask(task, &config, &encListenerCtx[i]);
        }
        else
        {
            memcpy((void*)&config.testDecConfig, &multiConfig->instConfig[i].decConfig, sizeof(TestDecConfig));
            success = CreateDecoderTask(task, &config, &decListenerCtx[i]);
        }
        CNMAppAdd(task);
        if (success == FALSE) {
            CNMAppStop();
            break;
        }
    }

    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    ret = CNMAppRun();
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    for (i=0; i<multiConfig->numMulti; i++) {
        if (multiConfig->instConfig[i].isEncoder == TRUE) {
            match &= (encListenerCtx[i].match == TRUE && encListenerCtx[i].matchOtherInfo == TRUE);
            ClearEncListenerContext(&encListenerCtx[i]);
        }
        else
        {
            match &= decListenerCtx[i].match;
            ClearDecListenerContext(&decListenerCtx[i]);
        }
    }dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    if (ret == TRUE) ret = match;
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    osal_free(decListenerCtx);
    osal_free(encListenerCtx);
    if (NULL != GetPendingInst(0)) {
        VLOG(ERR, "There are pending instances\n");
        while(TRUE) osal_msleep(100);
    }
    dbg3printf("\n[%s, %d]\n", __func__, __LINE__);
    return ret;
}

#ifdef NONOS_CODEC_RUNNING

int Codec_multi_instance_init(void)
{
    Int32           coreIndex   = 0;
    Uint32          productId   = 0;
    int             opt, index, i;
    char*           tempArg;
    char*           optString   = "fc:e:h";
    struct option   options[]   = {
        {"instance-num",          1, NULL, 0},
        {"codec",                 1, NULL, 0},
        {"input",                 1, NULL, 0},
        {"output",                1, NULL, 0},
        {"ref_file_path",         1, NULL, 0},
        {"bsmode",                1, NULL, 0},
        {"enable-wtl",            1, NULL, 0},
        {"sfs",                   1, NULL, 0},
        {"cores",                 1, NULL, 0},
        {"ring",                  1, NULL, 0},
        {NULL,                    0, NULL, 0},
    };
    const char*     name;
    char*           firmwarePath = NULL;
    Uint32          ret = 0;


    //dec only(0)ok
    //enc only(1)ok
    //enc+enc(2)ok
    //enc+dec(3)ok
    //dec+dec(4)ok check #define DEC_LOOP_TEST in vpuconfig.h
    //enc+enc+enc+enc(5)ok
    int codec_mode = 1;

    codec_multi_init(&multiConfig, codec_mode);

    //Codec_Check_HWID();
    ////////////////////////////////////////////////////////////////////////////////////////////////
    productId = VPU_GetProductId(coreIndex);
    switch (productId) {
    case PRODUCT_ID_521: firmwarePath = CORE_6_BIT_CODE_FILE_PATH; break;
    default:
        VLOG(ERR, "<%s:%d> Unknown productId(%d)\n", __FUNCTION__, __LINE__, productId);
        return 1;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////

    //Codec_Download_Firm();
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (LoadFirmware(productId, (Uint8**)&pusBitCode, &sizeInWord, firmwarePath) < 0) {
        VLOG(ERR, "%s:%d Failed to load firmware: %s\n", __FUNCTION__, __LINE__, firmwarePath);
        return 1;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////

    //Codec_Set_Config();
    ////////////////////////////////////////////////////////////////////////////////////////////////
    for (i = 0; i < MAX_NUM_INSTANCE; i++) {
        if (multiConfig.instConfig[i].isEncoder == FALSE)
            SetDecMultiParam(&multiConfig, i, productId);
        else
            SetEncMultiParam(&multiConfig, i, productId);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ret = MultiInstanceInit(&multiConfig, pusBitCode, sizeInWord);

    osal_free(pusBitCode);

    return ret;
}

int Codec_multi_instance_terminate(void)
{
	Uint32          ret = 0;
	ret = MultiInstanceClear(&multiConfig);
    return ret;
}

int Codec_multi_instance_run(void)
{
	int i;
	int checkterm = 0;

    TaskTerminate[task_index] = MultiInstanceRun(task_index);

    task_index++;

    if(task_index >= multiConfig.numMulti)
    	task_index = 0;

    for (i = 0; i < multiConfig.numMulti; i++) {
    	if(TaskTerminate[i] == TRUE)
    		checkterm++;
	}

    dbg3printf("\n___________________________________\n");

    if(checkterm == multiConfig.numMulti)
    	return TRUE;


	return FALSE;
}

#ifdef EN675_HEVC_TEST
int Vcodec(void)
{
	int codec_terminate = 0;
			if(codec_terminate == FALSE)
				codec_terminate = Codec_multi_instance_run();

			if(codec_terminate == TRUE)
				Codec_multi_instance_terminate();

			if(codec_terminate == TRUE)
		return 0;

	return 1;
}
#endif
#endif

void codec_multi_init(TestMultiConfig *multiConfig, int codec_mode)
{
    switch(codec_mode)
    {
    case 0://dec only(0)
    {
    	dbg3printf("Decoder only codec mode !\n");

        multiConfig->numMulti = 1;
        /////////////// 0th/////////////////////////dec
        multiConfig->instConfig[0].stdMode = STD_HEVC;//STD_AVC;////;//
        strcpy(multiConfig->instConfig[0].inputFilePath, "bit/main");
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[0].bsmode = BS_MODE_INTERRUPT;

        multiConfig->instConfig[0].enableWTL = TRUE;
        multiConfig->instConfig[0].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////
    }
    	break;
    case 1://enc only(1)
    	dbg3printf("Encoder only codec mode !\n");

        multiConfig->numMulti = 1;
        ///////////////// 0th///////////////////////enc
        multiConfig->instConfig[0].isEncoder = 1;
        multiConfig->instConfig[0].outNum = -1;//2;

        multiConfig->instConfig[0].stdMode = STD_HEVC;//STD_HEVC;//
        multiConfig->instConfig[0].width = 3840;//EYENIX_TEST_WIDTH;
        multiConfig->instConfig[0].height = 2160;//EYENIX_TEST_HEIGHT;
        strcpy(multiConfig->instConfig[0].yuvpath, "yuv/main");
        strcpy(multiConfig->instConfig[0].inputFilePath, "yuv/main");
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[0].bsmode = 0;//BS_MODE_INTERRUPT;

        multiConfig->instConfig[0].enableWTL = 0;//TRUE;
        multiConfig->instConfig[0].encConfig.subFrameSyncEn = 0;
        //////////////////////////////////////////////


    	break;
    case 2://enc+enc(2)
    	printf("2 Encoder codec mode !\n");

        multiConfig->numMulti = 2;
        ///////////////// 0th///////////////////////enc
        multiConfig->instConfig[0].isEncoder = 1;
        multiConfig->instConfig[0].outNum = -1;//2;

        multiConfig->instConfig[0].stdMode = STD_HEVC;//STD_AVC;//
        multiConfig->instConfig[0].width = EYENIX_TEST_WIDTH;
        multiConfig->instConfig[0].height = EYENIX_TEST_HEIGHT;
        strcpy(multiConfig->instConfig[0].yuvpath, "yuv/main");
        strcpy(multiConfig->instConfig[0].inputFilePath, "yuv/main");
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[0].bsmode = 0;//BS_MODE_INTERRUPT;

        multiConfig->instConfig[0].enableWTL = 0;//TRUE;
        multiConfig->instConfig[0].encConfig.subFrameSyncEn = 0;


        /////////////// 1th/////////////////////////enc
        multiConfig->instConfig[1].isEncoder = 1;
        multiConfig->instConfig[1].outNum = -1;//2;

        multiConfig->instConfig[1].stdMode = STD_AVC;//STD_HEVC;////
        multiConfig->instConfig[1].width = EYENIX_TEST_2CH_WIDTH;
        multiConfig->instConfig[1].height = EYENIX_TEST_2CH_HEIGHT;
        strcpy(multiConfig->instConfig[1].yuvpath, "yuv/sub");
        strcpy(multiConfig->instConfig[1].inputFilePath, "yuv/sub");

        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[1].bsmode = 0;//BS_MODE_INTERRUPT;

        multiConfig->instConfig[1].enableWTL = 0;//TRUE;
        multiConfig->instConfig[1].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////
    	break;
    case 3://enc+dec(3)
    	dbg3printf("Encoder&Decoder codec mode !\n");

        multiConfig->numMulti = 2;
        ///////////////// 0th///////////////////////enc
        multiConfig->instConfig[0].isEncoder = 1;
        multiConfig->instConfig[0].outNum = -1;//2;

        multiConfig->instConfig[0].stdMode = STD_HEVC;
        multiConfig->instConfig[0].width = EYENIX_TEST_WIDTH;
        multiConfig->instConfig[0].height = EYENIX_TEST_HEIGHT;
        strcpy(multiConfig->instConfig[0].yuvpath, "yuv/main");
        strcpy(multiConfig->instConfig[0].inputFilePath, "yuv/main");
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[0].bsmode = 0;//BS_MODE_INTERRUPT;

        multiConfig->instConfig[0].enableWTL = 0;//TRUE;
        multiConfig->instConfig[0].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////

        /////////////// 1th/////////////////////////dec
        multiConfig->instConfig[1].stdMode = STD_HEVC;//STD_AVC;
        strcpy(multiConfig->instConfig[1].inputFilePath, "bit/main");
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[1].bsmode = BS_MODE_INTERRUPT;

        multiConfig->instConfig[1].enableWTL = TRUE;
        multiConfig->instConfig[1].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////
    	break;
    case 4://dec+dec(4)
    	dbg3printf("2 decoder codec mode !\n");

        multiConfig->numMulti = 2;
#if 0

        /////////////// 1th/////////////////////////dec
        multiConfig->instConfig[0].stdMode = STD_AVC;
        strcpy(multiConfig->instConfig[0].inputFilePath, "bit/sub");

        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[0].bsmode = BS_MODE_INTERRUPT;

        multiConfig->instConfig[0].enableWTL = TRUE;
        multiConfig->instConfig[0].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////
#else
        /////////////// 0th/////////////////////////dec
        multiConfig->instConfig[0].stdMode = STD_HEVC;//STD_AVC;//
        strcpy(multiConfig->instConfig[0].yuvpath, "bit/main");
        strcpy(multiConfig->instConfig[0].inputFilePath, "bit/main");

        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[0].bsmode = BS_MODE_INTERRUPT;

        multiConfig->instConfig[0].enableWTL = TRUE;
        multiConfig->instConfig[0].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////

        /////////////// 1th/////////////////////////dec
        multiConfig->instConfig[1].stdMode = STD_HEVC;
        strcpy(multiConfig->instConfig[1].yuvpath, "bit/sub");
        strcpy(multiConfig->instConfig[1].inputFilePath, "bit/sub");

        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[1].bsmode = BS_MODE_INTERRUPT;

        multiConfig->instConfig[1].enableWTL = TRUE;
        multiConfig->instConfig[1].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////
#endif
    	break;
    case 5://enc+enc+enc+enc(5)
    	dbg3printf("4 Encoder codec mode !\n");

        multiConfig->numMulti = 4;
        ///////////////// 0th///////////////////////enc
        multiConfig->instConfig[0].isEncoder = 1;
        multiConfig->instConfig[0].outNum = -1;//2;

        multiConfig->instConfig[0].stdMode = STD_HEVC;
        multiConfig->instConfig[0].width = EYENIX_TEST_2CH_WIDTH;
        multiConfig->instConfig[0].height = EYENIX_TEST_2CH_WIDTH;
        strcpy(multiConfig->instConfig[0].yuvpath, "yuv/main");
        strcpy(multiConfig->instConfig[0].inputFilePath, "yuv/main");
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[0].bsmode = 0;//BS_MODE_INTERRUPT;

        multiConfig->instConfig[0].enableWTL = 0;//TRUE;
        multiConfig->instConfig[0].encConfig.subFrameSyncEn = 0;


        /////////////// 1th/////////////////////////enc
        multiConfig->instConfig[1].isEncoder = 1;
        multiConfig->instConfig[1].outNum = -1;//2;

        multiConfig->instConfig[1].stdMode = STD_HEVC;
        multiConfig->instConfig[1].width = EYENIX_TEST_2CH_WIDTH;
        multiConfig->instConfig[1].height = EYENIX_TEST_2CH_HEIGHT;
        strcpy(multiConfig->instConfig[1].yuvpath, "yuv/sub");
        strcpy(multiConfig->instConfig[1].inputFilePath, "yuv/sub");

        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[1].bsmode = 0;//BS_MODE_INTERRUPT;

        multiConfig->instConfig[1].enableWTL = 0;//TRUE;
        multiConfig->instConfig[1].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////


        /////////////// 2th/////////////////////////enc
        multiConfig->instConfig[2].isEncoder = 1;
        multiConfig->instConfig[2].outNum = -1;//2;

        multiConfig->instConfig[2].stdMode = STD_HEVC;
        multiConfig->instConfig[2].width = EYENIX_TEST_2CH_WIDTH;
        multiConfig->instConfig[2].height = EYENIX_TEST_2CH_HEIGHT;
        strcpy(multiConfig->instConfig[2].yuvpath, "yuv/sub");
        strcpy(multiConfig->instConfig[2].inputFilePath, "yuv/sub");

        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[2].bsmode = 0;//BS_MODE_INTERRUPT;

        multiConfig->instConfig[2].enableWTL = 0;//TRUE;
        multiConfig->instConfig[2].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////


        /////////////// 3th/////////////////////////enc
        multiConfig->instConfig[3].isEncoder = 1;
        multiConfig->instConfig[3].outNum = -1;//2;

        multiConfig->instConfig[3].stdMode = STD_HEVC;
        multiConfig->instConfig[3].width = EYENIX_TEST_2CH_WIDTH;
        multiConfig->instConfig[3].height = EYENIX_TEST_2CH_HEIGHT;
        strcpy(multiConfig->instConfig[3].yuvpath, "yuv/sub");
        strcpy(multiConfig->instConfig[3].inputFilePath, "yuv/sub");

        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[3].bsmode = 0;//BS_MODE_INTERRUPT;

        multiConfig->instConfig[3].enableWTL = 0;//TRUE;
        multiConfig->instConfig[3].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////
    	break;
    case 6://dec+dec+dec+dec(6)
    	dbg3printf("4 Decoder codec mode !\n");

        multiConfig->numMulti = 4;
        /////////////// 0th/////////////////////////dec
        multiConfig->instConfig[0].stdMode = STD_HEVC;//STD_AVC;
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[0].bsmode = BS_MODE_INTERRUPT;

        multiConfig->instConfig[0].enableWTL = TRUE;
        multiConfig->instConfig[0].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////


        /////////////// 1th/////////////////////////dec
        multiConfig->instConfig[1].stdMode = STD_HEVC;//STD_AVC;
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[1].bsmode = BS_MODE_INTERRUPT;

        multiConfig->instConfig[1].enableWTL = TRUE;
        multiConfig->instConfig[1].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////

        /////////////// 2th/////////////////////////dec
        multiConfig->instConfig[2].stdMode = STD_HEVC;//STD_AVC;
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[2].bsmode = BS_MODE_INTERRUPT;

        multiConfig->instConfig[2].enableWTL = TRUE;
        multiConfig->instConfig[2].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////

        /////////////// 3th/////////////////////////dec
        multiConfig->instConfig[3].stdMode = STD_HEVC;//STD_AVC;
        //osal_memcpy(multiConfig->instConfig[i].inputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].inputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].outputFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].outputFilePath);

        //osal_memcpy(multiConfig->instConfig[i].refFilePath, tempArg, strlen(tempArg));
        //ChangePathStyle(multiConfig->instConfig[i].refFilePath);

        multiConfig->instConfig[3].bsmode = BS_MODE_INTERRUPT;

        multiConfig->instConfig[3].enableWTL = TRUE;
        multiConfig->instConfig[3].encConfig.subFrameSyncEn = 0;

        //////////////////////////////////////////////
    	break;
    default:
    	dbg3printf("please enter codec mode !\n");
    	while(1);
    	break;
    }
}

int main_multi_instance(int argc, char **argv)
{
    Int32           coreIndex   = 0;
    Uint32          productId   = 0;
    TestMultiConfig multiConfig;
    int             opt, index, i;
    char*           tempArg;
    char*           optString   = "fc:e:h";
    struct option   options[]   = {
        {"instance-num",          1, NULL, 0},
        {"codec",                 1, NULL, 0},
        {"input",                 1, NULL, 0},
        {"output",                1, NULL, 0},
        {"ref_file_path",         1, NULL, 0},
        {"bsmode",                1, NULL, 0},
        {"enable-wtl",            1, NULL, 0},
        {"sfs",                   1, NULL, 0},
        {"cores",                 1, NULL, 0},
        {"ring",                  1, NULL, 0},
        {NULL,                    0, NULL, 0},
    };
    const char*     name;
    char*           firmwarePath = NULL;
    Uint32          ret = 0;

    osal_memset(&multiConfig, 0x00, sizeof(multiConfig));

#if defined(EYENIX_TEST)

    //dec only(0)ok
    //enc only(1)ok
    //enc+enc(2)ok
    //enc+dec(3)ok
    //dec+dec(4)ok check #define DEC_LOOP_TEST in vpuconfig.h
    //enc+enc+enc+enc(5)ok
    int codec_mode = 1;

    codec_multi_init(&multiConfig, codec_mode);

#else

    while ((opt=getopt_long(argc, argv, optString, options, &index)) != -1) {
        switch (opt) {
        case 'c':
            tempArg = strtok(optarg, ",");
            for(i = 0; i < MAX_NUM_INSTANCE; i++) {
                multiConfig.instConfig[i].compareType = atoi(tempArg);
                tempArg = strtok(NULL, ",");
                if (tempArg == NULL)
                    break;
            }
            break;
        case 'e':
            tempArg = strtok(optarg, ",");
            for(i = 0; i < MAX_NUM_INSTANCE; i++) {
                multiConfig.instConfig[i].isEncoder = atoi(tempArg);
                tempArg = strtok(NULL, ",");
                if (tempArg == NULL)
                    break;
            }
            break;
        case 'h':
            Help(argv[0]);
            return 0;
        case 0:
            name = options[index].name;
            if (strcmp("instance-num", name) == 0) {
                multiConfig.numMulti = atoi(optarg);
            }
            else if (strcmp("codec", name) == 0) {
                tempArg = strtok(optarg, ",");
                for(i = 0; i < MAX_NUM_INSTANCE; i++) {
                    multiConfig.instConfig[i].stdMode = (CodStd)atoi(tempArg);
                    tempArg = strtok(NULL, ",");
                    if (tempArg == NULL)
                        break;
                }
            }
            else if (strcmp("input", name) == 0) {
                tempArg = strtok(optarg, ",");
                for(i = 0; i < MAX_NUM_INSTANCE; i++) {
                    osal_memcpy(multiConfig.instConfig[i].inputFilePath, tempArg, strlen(tempArg));
                    ChangePathStyle(multiConfig.instConfig[i].inputFilePath);
                    tempArg = strtok(NULL, ",");
                    if (tempArg == NULL)
                        break;
                }
            }
            else if (strcmp("output", name) == 0) {
                tempArg = strtok(optarg, ",");
                for(i = 0; i < MAX_NUM_INSTANCE; i++) {
                    osal_memcpy(multiConfig.instConfig[i].outputFilePath, tempArg, strlen(tempArg));
                    ChangePathStyle(multiConfig.instConfig[i].outputFilePath);
                    tempArg = strtok(NULL, ",");
                    if (tempArg == NULL)
                        break;
                }
            }
            else if (strcmp("ref_file_path", name) == 0) {
                tempArg = strtok(optarg, ",");
                for(i = 0; i < MAX_NUM_INSTANCE; i++) {
                    osal_memcpy(multiConfig.instConfig[i].refFilePath, tempArg, strlen(tempArg));
                    ChangePathStyle(multiConfig.instConfig[i].refFilePath);
                    tempArg = strtok(NULL, ",");
                    if (tempArg == NULL)
                        break;
                }
            }
            else if (strcmp("bsmode", name) == 0) {
                tempArg = strtok(optarg, ",");
                for(i = 0; i < MAX_NUM_INSTANCE; i++) {
                    multiConfig.instConfig[i].bsmode = atoi(tempArg);
                    tempArg = strtok(NULL, ",");
                    if (tempArg == NULL)
                        break;
                }
            }
            else if (strcmp("enable-wtl", name) == 0) {
                tempArg = strtok(optarg, ",");
                for(i = 0; i < MAX_NUM_INSTANCE; i++) {
                    multiConfig.instConfig[i].enableWTL = atoi(tempArg);
                    tempArg = strtok(NULL, ",");
                    if (tempArg == NULL)
                        break;
                }
            }
            else if (strcmp("sfs", name) == 0) {
                tempArg = strtok(optarg, ",");
                for(i = 0; i < MAX_NUM_INSTANCE; i++) {
                    multiConfig.instConfig[i].encConfig.subFrameSyncEn = atoi(tempArg);
                    tempArg = strtok(NULL, ",");
                    if (tempArg == NULL) break;
                }
            }
            else {
                VLOG(ERR, "unknown --%s\n", name);
                Help(argv[0]);
                return 1;
            }
            break;
        case '?':
            return 1;
        }
    }

    InitLog();
    SetMaxLogLevel(ERR);
#endif

    productId = VPU_GetProductId(coreIndex);



    switch (productId) {
    case PRODUCT_ID_960: firmwarePath = CORE_0_BIT_CODE_FILE_PATH; break;
    case PRODUCT_ID_980: firmwarePath = CORE_1_BIT_CODE_FILE_PATH; break;
    case PRODUCT_ID_511: firmwarePath = CORE_6_BIT_CODE_FILE_PATH; break;
    case PRODUCT_ID_521: firmwarePath = CORE_6_BIT_CODE_FILE_PATH; break;
    default:
        VLOG(ERR, "<%s:%d> Unknown productId(%d)\n", __FUNCTION__, __LINE__, productId);
        return 1;
    }

    if (LoadFirmware(productId, (Uint8**)&pusBitCode, &sizeInWord, firmwarePath) < 0) {
        VLOG(ERR, "%s:%d Failed to load firmware: %s\n", __FUNCTION__, __LINE__, firmwarePath);
        return 1;
    }


    for (i = 0; i < MAX_NUM_INSTANCE; i++) {
        if (multiConfig.instConfig[i].isEncoder == FALSE)
            SetDecMultiParam(&multiConfig, i, productId);
        else
            SetEncMultiParam(&multiConfig, i, productId);
    }


    do {

        if (MultiInstanceTest(&multiConfig, pusBitCode, sizeInWord) == FALSE) {
            VLOG(ERR, "Failed to MultiInstanceTest()\n");
            ret = 1;
            break;
        }
    } while(FALSE);

    osal_free(pusBitCode);

    return ret;
}


void MakeTestPatter_byISP(void)
{
	const UINT nHW = 416;
	const UINT nVW = 240;
	const UINT nDdrAddrY = (DDR0_BASE +0xE700000);          // DDR 주소 설정
	const UINT nDdrAddrC = nDdrAddrY + (nHW * nVW);

	PSCK_SELw(2);
	PPCK_SELw(2);
	FN_CK0_SELw(2);
	PR_CK0_SELw(2);
	PSCK_PDw(1);
	PPCK_PDw(1);
	PR_CK0_PDw(1);
	FN_CK0_PDw(1);

	POS_HZw(1);
	HWOw(nHW);
	VWOw(nVW);
	HTWOw(nHW + 150);
	VTWOw(nVW + 16);
	OSYNC_MODw(1);
	POS_HZw(0);

	YC_OSELw(0x11);

	YCW_CK0_SELw(2);
	YCW_CK0_PDw(1);
	IM_YADR0w(nDdrAddrY>>4);
	IM_CADR0w(nDdrAddrC>>4);
	IM_HWI0w(nHW);
	IM_IVSEL0w(0);
	IM_ISEL0w(0xC);

	IM_GO0w(1);

}

#ifdef ISP_SENSOR_CODEC_TEST

//_regs_ BF_3(UINT _rev0 : 30, UINT PIN0_OUT_AUR : 1 ,UINT PIN0_OEN_AUR : 1 ) _rege_ _GPIO_0_AUR;
typedef union{UINT a; struct{ UINT PIN0_OEN_AUR:1;; UINT PIN0_OUT_AUR:1;;      UINT _rev0:30; };} _GPIO_0_AUR;
#define GPIO_PIN0_OUT_AUR _bm(_GPIO_0_AUR,0x400FE000, (0<<3),PIN0_OUT_AUR) // 1 Bit, 1'h0, RW
#define GPIO_PIN0_OEN_AUR _bm(_GPIO_0_AUR,0x400FE000, (0<<3),PIN0_OEN_AUR) // 1 Bit, 1'h1, RW

//_regs_ BF_3(UINT _rev0 : 30, UINT PIN1_OUT_AUR : 1 ,UINT PIN1_OEN_AUR : 1 ) _rege_ _GPIO_1_AUR;
typedef union{UINT a; struct{ UINT PIN1_OEN_AUR:1;; UINT PIN1_OUT_AUR:1;; UINT _rev0:30; };} _GPIO_1_AUR;
#define GPIO_PIN1_OUT_AUR _bm(_GPIO_1_AUR,0x400FE000, (1<<3),PIN1_OUT_AUR) // 1 Bit, 1'h0, RW
#define GPIO_PIN1_OEN_AUR _bm(_GPIO_1_AUR,0x400FE000, (1<<3),PIN1_OEN_AUR) // 1 Bit, 1'h1, RW


#define INIT_DELAY(T)  WaitXms(34*T)

const WORD gwTblIMX323[][2] = { // for TWI
                                                             {0x3000, 0x31}, // Valid register write / Standby
                                                             {0x0100, 0x00}, // Standby control (0:Standby, 1:Normal)

                                                             {0x3002, 0x0F}, // Readout mode designation (0x1:720P,0xF:1080P,Others:Invalid)
                                                             {0x3011, 0x00}, // FRSEL / Output data rate designation (10,12bit 15P:0x01,30P:0x00)
                                                             {0x3012, 0x82}, //1101Sony
                                                             {0x3016, 0x3C}, // 1080P:0x3C,720P:0xF0

                                                             {0x301f, 0x73}, // Fixed to 0x73
                                                             {0x3021, 0x00}, // 10BIA (10bit:1, 12bit:0)
                                                             {0x3027, 0x20}, // Fixed to 0x20

                                                             {0x302c, 0x00}, // XMSTA (0:Sensor master, 1:Trig Standby)

                                                             {0x303f, 0x0A}, // Fixed to 0x0A

                                                             {0x307a, 0x00}, // 10BIA (10bit:0x40, 12bit:0)
                                                             {0x307b, 0x00}, // 10BIA (10bit:0x02, 12bit:0)

                                                             {0x309a, 0x26}, // 12bit1080P[11:4] Adjustment registers for each operation mode
                                                             {0x309b, 0x02}, // 12bit1080P[ 3:0] Adjustment registers for each operation mode

                                                             // TWI Only Register
                                                             {0x0008, 0x00}, // Black Level[8]
                                                             {0x0009, 0x3C}, // Black Level[7:0] It's recommended to be set 10bit:0x3c, 12bit:0xf0

                                                             {0x0101, 0x00}, // H[0]/V[1] Scanning direction control (0:Normal, 1:Inverted)

                                                             {0x0104, 0x00}, // Register reflection timing hold (0:Normal, 1:Register setting hold)

                                                             {0x0112, 0x0C}, // TWI ADRES1 (0x0A:10bits, 0x0C:12bits)
                                                             {0x0113, 0x0C}, // TWI ADRES2 (0x0A:10bits, 0x0C:12bits)

                                                             {0x0202, 0x00}, // Integration time[15:8] adjustment direction line number designation
                                                             {0x0203, 0x00}, // Integration time[ 7:0] adjustment direction line number designation

                                                             {0x0340, 0x04}, // In master mode. Vertical(V) direction line number[15:8] designation
                                                             {0x0341, 0x65}, // In master mode. Vertical(V) direction line number[ 7:0] designation

                                                             //{0x0342, 0x08},      // In master mode. Horizontal(H) direction clock number[15:8] designation
                                                             //{0x0343, 0x98},      // In master mode. Horizontal(H) direction clock number[ 7:0] designation

                                                             {0x0342, 0x04}, // In master mode. Horizontal(H) direction clock number[15:8] designation
                                                             {0x0343, 0x4C} //1101Sony,     // In master mode. Horizontal(H) direction clock number[ 7:0] designation
                                                             //{0x0342, 0x05},      // In master mode. Horizontal(H) direction clock number[15:8] designation
                                                             //{0x0343, 0xa0} //1101Sony,   // In master mode. Horizontal(H) direction clock number[ 7:0] designation

                                                             //{0x3000, 0x30}       // Valid register write / Normal Operation
                                                             };

void SetSens(WORD awAddr, BYTE abData)
{       // Write to Sensor
        I2cWrite(0/*SENSOR_I2C_CH*/,0x34/*SENSOR_SONY_I2C_CA*/,0,0);
        I2cWrite(0/*SENSOR_I2C_CH*/,(awAddr>>8)&0xff, 0,0);
        I2cWrite(0/*SENSOR_I2C_CH*/,(awAddr>>0)&0xff, 0,0);
        I2cWrite(0/*SENSOR_I2C_CH*/,abData, 1,0);
}

void InitSensRun(void)
{
        for(UINT i=0;i<ARRAY_SIZE(gwTblIMX323); i++) SetSens(gwTblIMX323[i][0], (BYTE)gwTblIMX323[i][1]);
        WaitXus(1000);
        SetSens(0x3117, 0x0D);         // 추가 (IMX322 & IMX323)

        WaitXus(1000);
        WaitXus(1000);

        SetSens(0x0100, 1); // Standby control (0:Standby, 1:Normal) - i2c only
        WaitXus(1000);
}

void Isp_SensorRst(void)
{
        INIT_DELAY(2);

        //      Sensor Init
        SYS_GPIO0_MUX = 0;
        GPIO_PIN0_OEN_AUR = 0;
        GPIO_PIN0_OUT_AUR = 1;

        SYS_GPIO1_MUX = 0;
        GPIO_PIN1_OEN_AUR = 0;
        GPIO_PIN1_OUT_AUR = 1;

        INIT_DELAY(1);
        GPIO_PIN1_OUT_AUR = 0;
        INIT_DELAY(4);
        GPIO_PIN1_OUT_AUR = 1;

        INIT_DELAY(1);
}

void isp_init_test(void)
{
        //WaitXms(2000);

        #define DDR0_BASE      0x80000000
        #define DDR1_BASE      0x90000000

  #if 1
        const UINT nFR_HW = RP(FR_HW);
        const UINT nFR_VW = RP(FR_VW);
        const UINT nPO_HW = nFR_HW+8;
        const UINT nPO_VW = nFR_VW+8;
        const UINT nHTW = RP(FR_HTW60)-2;
        const UINT nVTW = RP(FR_VTW60)-1;
        #define R_LTC_T 0x500
  #elif 0
        const UINT nFR_HW = 416;
        const UINT nFR_VW = 240;
        const UINT nPO_HW = 416;
        const UINT nPO_VW = 240;
        const UINT nHTW = 416+300;
        const UINT nVTW = 240+32;
        #define R_LTC_T 200
  #else
        const UINT nFR_HW = 3840;
        const UINT nFR_VW = 2160;
        const UINT nPO_HW = nFR_HW+8;
        const UINT nPO_VW = nFR_VW+8;
        const UINT nHTW = 4400-2;
        const UINT nVTW = 2250-1;
        #define R_LTC_T 0xf80
  #endif

        const UINT nWdrAddr = DDR1_BASE;
        const UINT nFrc0Addr = nWdrAddr  + ((nPO_HW*nPO_VW*10)>>3);
        const UINT nFrc1Addr = nFrc0Addr + ((nPO_HW*nPO_VW*10)>>3);
        const UINT nDdrAddrY = nFrc1Addr + ((nPO_HW*nPO_VW*10)>>3);
        const UINT nDdrAddrC = nDdrAddrY + (nFR_HW*nFR_VW);

        Isp_SensorRst();

        PCKO_SELw(2);  // SP(SensorClk) -> SENS_37M -> 2
        PCKO_CK_PDw(1);
        PCKO_PDw(1);
        PCKO_OENw(0);

        PCLK_PDw(1);
        CH_DSELw(1);
        CH_HSSELw(1);
        CH_VSSELw(1);
        IBT_PCK_SELw(0);       // Parallel Clock
        IBT_PCK_PDw(1);
        IDDR_ONw(1);
        ISDR_OPw(1);
        DDRFF_RSw(1);
        DDRFF_RSw(0);
        PDI_MODw(1);   // BIT12TOBit12 -> 1
        PCLK_DLYw(0);

        InitSensRun();

        #define SENSOR_BLACK_LEVEL     0x3c    // 암부영역 컬러노이즈(자주색) 감소, 미적용 시 WDR Short 채널에 자주색 발생
        #define ISP_OFFSET     (1024-((SENSOR_BLACK_LEVEL)>>2))
        PRE_GROFSw(ISP_OFFSET);
        PRE_GBOFSw(ISP_OFFSET);
        PRE_ROFSw(ISP_OFFSET);
        PRE_BOFSw(ISP_OFFSET);

        HSUP_GAw(0x3F);
        LSUP_THw(1);

        DF_DIFTH_LEw(0);
        DF_DIFTH_SE0w(0);
        DF_TSEL_LEw(0);
        DF_TSEL_SE0w(0);

        HSUP_ONw(1);
        HSUP_THw(SP(HSUP_TH));

        ICSELw(1/*UP(ICSel)*/);
        OCSELw(3/*UP(OCSel)*/);

        RMYRw(0x20B);RMYGw(0x1DB);RMYBw(0x30);BMYRw(0x78);BMYGw(0x193);BMYBw(0x20B);

        BT_PCK_PDw(0);
        FPCK_PDw(0);
        BT_PCK_SELw(2); // Pixel Clock, ISP_CLK_74M -> 2
        FPCK_SELw(2);  // ISP_CLK_74M -> 2
        BT_PCK_PDw(1);
        FPCK_PDw(1);

        YCW_DCK2_PDw(0);
        YCW_DCK2_SELw(2);      // ISP_CLK_74M -> 2
        YCW_DCK2_PDw(1);       // WDR 사용 시 필요

        PSCK_SELw(2);
        PPCK_SELw(2);
        FN_CK0_SELw(2);
        PR_CK0_SELw(2);
        PSCK_PDw(1);
        PPCK_PDw(1);
        PR_CK0_PDw(1);
        FN_CK0_PDw(1);

        PRS_HZw(1);

        HSPIw(SP(PreHsp));
        VSPIw(SP(PreVsp));     // WDR 사용 시 2 이상 설정
        HWIw(nPO_HW);
        VWIw(nPO_VW);

        if(SP(IsSlave)) { SLVw(3); }
        else                   { HTWIw(nHTW); VTWIw(nVTW); SLVw(0); }

        VLOCKI_POSw(SP(PreVSyncOfs));
        HLOCKI_POSw(SP(PreHSyncOfs));

        ASYNC_ONw(SP(IsASync));
        VSYN_NAONw(SP(IsNSync));
        PRS_HZw(0);

        POL_HSIw(SP(PreHSyncPol));
        POL_VSIw(SP(PreVSyncPol));

        POS_HZw(1);
        HVWIO_SYNw(0);
        HWO_1D_ONw(0);
        HSPOw(0x6);
        VSPOw(0x4);
        HWOw(nPO_HW);
        VWOw(nPO_VW);
        HTWOw(nHTW);
        VTWOw(nVTW);
        OSYNC_MODw(1);
        VLOCKI2_POSw(0x2);
        HLOCKI2_POSw(0x3a);
        EXVSYNC_SELw(0);
        EXHSYNC_SELw(0);
        POS_HZw(0);

        ITU_HTWO0w(nHTW);
        VLOCKO_IT_POS0w(RP(OutVSyncOfs));
        HLOCKO_IT_POS0w(RP(OutHSyncOfs));
        HSPC0w(0);     VSPC0w(0x2a);
        HWC0w(nFR_HW);
        VWC0w(nFR_VW);

        ITU_HTWO1w(nHTW);
        VLOCKO_IT_POS1w(RP(OutVSyncOfs));
        HLOCKO_IT_POS1w(RP(OutHSyncOfs));
        HSPC1w(0);     VSPC1w(0x2a);
        HWC1w(nFR_HW);
        VWC1w(nFR_VW);

        BYHUE_LDw(((int)SP(Yel_RedToGrn))-128);
        BYGAIN_LDw(SP(Yel_Gain));
        RYHUE_LUw(((int)SP(Red_YelToBlu))-128);
        RYGAIN_LUw(SP(Red_Gain));
        BYHUE_LUw(((int)SP(Blu_GrnToRed))-128);
        BYGAIN_LUw(SP(Blu_Gain));
        RYHUE_LDw(((int)SP(Grn_BluToYel))-128);
        RYGAIN_LDw(SP(Grn_Gain));
        HUE_MODw(1);

        YGMA_ONw(1);
        CGMA_ONw(1);

        APT_ONw(1);
        APT_GAIN_ONw(1);

        APT_TH3w(0x80);
        APT_CLIP3w(0x80);

        FORCE_ABT_SOFFw(1);

        DNR_CSELw(3/*model_TgtBd ? 3 : SP(Dnr2dICSel)*/);
        DNR_RCSELw(0/*SP(Dnr2dOCSel)*/);
        DNR2D_ONw(1);

        WPRR_GAINw(0xbe);
        WPRG_GAINw(0x1030);
        WPRB_GAINw(0x85);
        WPGR_GAINw(0x1023);
        WPGG_GAINw(0x7c);
        WPGB_GAINw(0x39);
        WPBR_GAINw(0x1c);
        WPBG_GAINw(0x101f);
        WPBB_GAINw(0x13d);

        YCW_CK1_SELw(2);
        YCW_CK1_PDw(1);
        IM_YADR1_P0w(nDdrAddrY>>4);
        IM_CADR1_P0w(nDdrAddrC>>4);
        IM_HWI1w(nFR_HW);
        IM_IVSEL1w(0);
        IM_ISEL1w(0xC);
        #define IM_GO_W IM_GO1w(1)

        IM_GO_W;

        DNR3D_FKw(0xb0/*0x80*/);
        DNR3D_THw(0x00/*0x40*/);
        DNR3D_GAw(0x18/*0x20*/);

        DNR3D_RCH_ENw(1);
        YCR_CK4_PDw(0);
        YCRCK4_SELw(2); // ISP_CLK_74M -> 2
        YCR_CK4_PDw(1);
        DNR3D_ONw(1);

        WDR_ADR_LEw(nWdrAddr>>4);
        FRC_ADR0w(nFrc0Addr>>4);
        FRC_ADR1w(nFrc1Addr>>4);

        RD_MODw(0);
        AXI_IDSw(0);

        DDR_RDNR_LTCw(R_LTC_T);
        DDR_RWDR_LTCw(R_LTC_T);
        DDR_RFRC_LTCw(R_LTC_T);
        DDR_RYC_LTCw(R_LTC_T);
        DDR_RENC_LTCw(0x300);  // ENC 는 고정

        SD_MODw(0);                    // DDR OFF

        VIRQO_EN_Tw(1);

        CPU_FRC_ENw(1);        // DDR OFF,  SD_MODw(0) 이후 1 VLOCK Delay 후 설정해야 함!!!
        BUS_RD_RSTw(1);

        while (1)
        {
               if(ISP_RIRQ_VOr) {
                       CLI_VLOCKO_Tw(1);
                       IM_GO_W;
               }
        }
}


#endif
