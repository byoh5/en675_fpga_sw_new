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
#include "encoder_listener.h"
#include "misc/debug.h"
#include "misc/bw_monitor.h"



static void HandleEncHandlingIntEvent(Component com, CNMComListenerHandlingInt* param, EncListenerContext* ctx)
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (ctx->bwCtx != NULL) {
        BWMonitorUpdate(ctx->bwCtx, 1);
    }
}

static void HandleEncFullEvent(Component com, CNMComListenerEncFull* param, EncListenerContext* ctx)
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
}

static void HandleEncGetEncCloseEvent(Component com, CNMComListenerEncClose* param, EncListenerContext* ctx)
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (ctx->pfCtx != NULL) {
        PFMonitorRelease(ctx->pfCtx);
    }
    if (ctx->bwCtx != NULL) {
        BWMonitorRelease(ctx->bwCtx);
    }
}

void HandleEncCompleteSeqEvent(Component com, CNMComListenerEncCompleteSeq* param, EncListenerContext* ctx)
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (ctx->performance == TRUE) {
        Uint32 fps = (ctx->fps == 0) ? 30 : ctx->fps;
        ctx->pfCtx = PFMonitorSetup(ctx->coreIdx, 0, ctx->pfClock, fps, GetBasename((const char *)ctx->cfgFileName), 1);
    }
    if (ctx->bandwidth == TRUE) {
        ctx->bwCtx = BWMonitorSetup(param->handle, TRUE, GetBasename((const char *)ctx->cfgFileName));
    }
}
#include "dev.h"
void HandleEncGetOutputEvent(Component com, CNMComListenerEncDone* param, EncListenerContext* ctx)
{
    EncOutputInfo* output = param->output;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (output->reconFrameIndex == RECON_IDX_FLAG_ENC_END)
        return;
    //printf("codec done test_isp_codec_flag %d\n",test_isp_codec_flag);
	test_isp_codec_flag = 0;
	//printf(" codec done %dus\n",BenchTimeStop(CodecENCTimeStart));
	CodecENCTimeStart = 0;
    Visp();
#ifdef EYENIX_TEST
#if 1//def RTOS_BITSTREAM_TEST
//#define ENC_BITSTREAM_ADDR 		0x84000000
//#define ENC_BITSTREAM_SIZE 		0x5FFFFFF
    #define ENC_BITSTREAM_ADDR 		0x9C000000
    #define ENC_BITSTREAM_SIZE 		0x3000000

#define ENC_BITSTREAM_SEC_ADDR 	(ENC_BITSTREAM_ADDR+ENC_BITSTREAM_SIZE+1)//0x86D0_0000
//#define ENC_BITSTREAM_END_ADDR 	0x8FAFFFFF
	#define ENC_BITSTREAM_END_ADDR 	0x9FA00000
//#define BYTE_ALIGN_SIZE   	16
//#define BYTE_ALIGN(x) 		(((x) + BYTE_ALIGN_SIZE - 1) & ~(BYTE_ALIGN_SIZE - 1))
    	int instanceIndex = param->handle->instIndex;
    	int bitstreamSize = output->bitstreamSize;
    	static UINT maxBitSize = 0;

    	BYTE *bitstreamAddr = output->bitstreamBuffer;

		static UINT TESTOFFSET[4] = {0,0,0,0};
		static UINT TESTHEADER[2][200] = {0,};
		static UINT TESTHEADERSIZE[2] = {0,};

		static UINT TEST_BITS_ENDADDR[2];
		TEST_BITS_ENDADDR[0] = ENC_BITSTREAM_SEC_ADDR-(1024);
		TEST_BITS_ENDADDR[1] = ENC_BITSTREAM_END_ADDR -(1024);

		BYTE *TESTOUT = ENC_BITSTREAM_ADDR + (ENC_BITSTREAM_SIZE*instanceIndex);


		if(maxBitSize < bitstreamSize)
		{
			maxBitSize = bitstreamSize;
			printf("maxBitSize %d \n", maxBitSize);
		}

		if(output->reconFrameIndex == -3)
		{
			osal_memset(TESTHEADER[instanceIndex], 0x00, 200);
			osal_memcpy(TESTHEADER[instanceIndex], bitstreamAddr, bitstreamSize);
			TESTHEADERSIZE[instanceIndex] = bitstreamSize;
		}
		else
		{
			//printf("%d ",output->encPicCnt);
			if(output->picType == 0) // I frame
			{
				osal_memcpy(TESTOUT+TESTOFFSET[instanceIndex], TESTHEADER[instanceIndex], TESTHEADERSIZE[instanceIndex]);
				osal_memcpy(TESTOUT+TESTOFFSET[instanceIndex]+TESTHEADERSIZE[instanceIndex], bitstreamAddr, bitstreamSize);
				//bitstreamSize = BYTE_ALIGN(bitstreamSize);
				bitstreamSize += TESTHEADERSIZE[instanceIndex];
			}
			else // P frame
			{
				osal_memcpy(TESTOUT+TESTOFFSET[instanceIndex], bitstreamAddr, bitstreamSize);
				//bitstreamSize = BYTE_ALIGN(bitstreamSize);
			}
#if 1//def RTOS_BITSTREAM_TEST
			while(BIT_STREAM_LOCK==1)
				WaitXus(1);

			if(BIT_STREAM_LOCK == 0)
			{
				BIT_STREAM_LOCK = 1;

				//printf("[debug] %s %d SAVED BITSTREAM (%d) 0x%x size %d [offset 0x%x]\n",__func__,__LINE__, instanceIndex, TESTOUT+TESTOFFSET[instanceIndex], bitstreamSize, TESTOFFSET[instanceIndex] + bitstreamSize);
				BIT_STREAM_ADDR = TESTOUT+TESTOFFSET[instanceIndex];
				BIT_STREAM_SIZE = bitstreamSize;
				BIT_STREAM_TYPE = output->picType;
				BIT_STREAM_FRNUM = output->encPicCnt;
				BIT_STREAM_RECNUM = output->reconFrameIndex;

				BIT_STREAM_LOCK = 0;
			}
#else
			dbgprintf("[debug] %s %d SAVED BITSTREAM (%d) 0x%x size %d [offset 0x%x]\n",__func__,__LINE__, instanceIndex, TESTOUT+TESTOFFSET[instanceIndex], bitstreamSize, TESTOFFSET[instanceIndex] + bitstreamSize);
#endif



			TESTOFFSET[instanceIndex] += bitstreamSize;

			if((TESTOUT+TESTOFFSET[instanceIndex]) > TEST_BITS_ENDADDR[instanceIndex])
			{
				printf("buffer full addr 0x%x \n", TESTOUT+TESTOFFSET[instanceIndex]);
				TESTOFFSET[instanceIndex] = 0;
			}
		}
		//WaitXms(1);
#endif
#endif

    if (ctx->pfCtx != NULL) {
        PFMonitorUpdate(ctx->coreIdx, ctx->pfCtx, output->frameCycle, output->encPrepareEndTick - output->encPrepareStartTick,
            output->encProcessingEndTick - output->encProcessingStartTick, output->encEncodeEndTick- output->encEncodeStartTick);
    }
    if (ctx->bwCtx != NULL) {
        BWMonitorUpdatePrint(ctx->bwCtx, output->picType);
    }

    if (ctx->headerEncDone[param->handle->instIndex] == FALSE) {
        ctx->headerEncDone[param->handle->instIndex] = TRUE;
    }

    if (ctx->match == FALSE) CNMAppStop();
}

void Coda9HandleEncGetOutputEvent(Component com, CNMComListenerEncDone* param, EncListenerContext* ctx)
{
    EncOutputInfo* output = param->output;
    EncHandle      handle = param->handle;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);

    if (output->reconFrameIndex == RECON_IDX_FLAG_ENC_END) return;

    if (ctx->pfCtx != NULL) {
        PFMonitorUpdate(ctx->coreIdx, ctx->pfCtx, output->frameCycle, output->encPrepareEndTick - output->encPrepareStartTick,
            output->encProcessingEndTick - output->encProcessingStartTick, output->encEncodeEndTick- output->encEncodeStartTick);
    }
    if (ctx->bwCtx != NULL) {
        BWMonitorUpdatePrint(ctx->bwCtx, output->picType);
    }

    if (output->bitstreamSize > 0) {
        if (ctx->es != NULL) {
            Uint8*          buf      = NULL;
            int             compSize = output->bitstreamSize;
            PhysicalAddress addr;

            if (param->fullInterrupted == TRUE) {
                PhysicalAddress rd, wr;
                VPU_EncGetBitstreamBuffer(param->handle, &rd, &wr, &compSize);
                addr     = rd;
            }
            else {
                addr     = output->bitstreamBuffer;
                compSize = output->bitstreamSize;
            }

            buf = (Uint8*)osal_malloc(compSize);
            vdi_read_memory(ctx->coreIdx, addr, buf, compSize, ctx->streamEndian);

            if ( ctx->es != NULL ) {
                if ((ctx->match=Comparator_Act(ctx->es, buf, compSize)) == FALSE) {
                    VLOG(ERR, "<%s:%d> INSTANCE #%d Bitstream Mismatch\n", __FUNCTION__, __LINE__, handle->instIndex);
                }
            }
            osal_free(buf);
        }
    }
    else if (TRUE == param->encodedStreamInfo.ringBufferEnable && NULL != param->encodedStreamInfo.encodedStreamBuf) {

        if ( ctx->es != NULL ) {
            if ((ctx->match=Comparator_Act(ctx->es, param->encodedStreamInfo.encodedStreamBuf, param->encodedStreamInfo.encodedStreamBufLength)) == FALSE) {
                VLOG(ERR, "<%s:%d> INSTANCE #%d Bitstream Mismatch\n", __FUNCTION__, __LINE__, handle->instIndex);
            }
        }
    }

    if (ctx->headerEncDone[param->handle->instIndex] == FALSE) {
        ctx->headerEncDone[param->handle->instIndex] = TRUE;
    }

    if (ctx->match == FALSE) CNMAppStop();
}

void Coda9HandleEncMakeHeaderEvent(Component com, CNMComListenerEncMakeHeader* param, EncListenerContext* ctx)
{
    EncodedHeaderBufInfo encHeaderInfo = param->encHeaderInfo;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (0 < encHeaderInfo.encodedHeaderBufSize && NULL != encHeaderInfo.encodedHeaderBuf) {
        if (NULL != ctx->es) {
                if ((ctx->match=Comparator_Act(ctx->es, encHeaderInfo.encodedHeaderBuf, encHeaderInfo.encodedHeaderBufSize)) == FALSE) {
                    VLOG(ERR, "<%s:%d> Header Mismatch\n", __FUNCTION__, __LINE__, param->handle->instIndex);
                    VLOG(ERR, "<%s:%d> INSTANCE #%d Bitstream Mismatch\n", __FUNCTION__, __LINE__, param->handle->instIndex);
                }
        }
        if (encHeaderInfo.fp) {
            osal_fwrite(encHeaderInfo.encodedHeaderBuf, encHeaderInfo.encodedHeaderBufSize, 1, encHeaderInfo.fp);
            osal_fflush(encHeaderInfo.fp);
        }
    } else {
        VLOG(WARN, "<%s:%d> INSTANCE #%d Empty Header\n", __FUNCTION__, __LINE__, param->handle->instIndex);
    }

    if (ctx->match == FALSE) CNMAppStop();
}

void EncoderListener(Component com, Uint64 event, void* data, void* context)
{
    int         productId;
    EncHandle   handle;
    int         key=0;

    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (osal_kbhit()) {
        key = osal_getch();
        osal_flush_ch();
        if (key) {
            if (key == 'q' || key == 'Q') {
                CNMAppStop();
                return;
            }
        }
    }
    switch (event) {
    case COMPONENT_EVENT_ENC_OPEN:
        break;
    case COMPONENT_EVENT_ENC_ISSUE_SEQ:
        break;
    case COMPONENT_EVENT_ENC_COMPLETE_SEQ:
        HandleEncCompleteSeqEvent(com, (CNMComListenerEncCompleteSeq*)data, (EncListenerContext*)context);
        break;
    case COMPONENT_EVENT_ENC_REGISTER_FB:
        break;
    case COMPONENT_EVENT_CODA9_ENC_MAKE_HEADER:
        Coda9HandleEncMakeHeaderEvent(com, (CNMComListenerEncMakeHeader*)data, (EncListenerContext*)context);
        break;
    case COMPONENT_EVENT_ENC_READY_ONE_FRAME:
        break;
    case COMPONENT_EVENT_ENC_START_ONE_FRAME:
        break;
    case COMPONENT_EVENT_ENC_HANDLING_INT:
        HandleEncHandlingIntEvent(com, (CNMComListenerHandlingInt*)data, (EncListenerContext*)context);
        break;
    case COMPONENT_EVENT_ENC_GET_OUTPUT_INFO:
        handle = ((CNMComListenerEncDone*)data)->handle;
        productId = VPU_GetProductId(VPU_HANDLE_CORE_INDEX(handle));
        if (TRUE == PRODUCT_ID_W_SERIES(productId)) {
            HandleEncGetOutputEvent(com, (CNMComListenerEncDone*)data, (EncListenerContext*)context);
        }
        else {
            Coda9HandleEncGetOutputEvent(com, (CNMComListenerEncDone*)data, (EncListenerContext*)context);
        }
        break;
    case COMPONENT_EVENT_ENC_ENCODED_ALL:
        break;
    case COMPONENT_EVENT_ENC_CLOSE:
        HandleEncGetEncCloseEvent(com, (CNMComListenerEncClose*)data, (EncListenerContext*)context);
        break;
    case COMPONENT_EVENT_ENC_FULL_INTERRUPT:
        HandleEncFullEvent(com, (CNMComListenerEncFull*)data, (EncListenerContext*)context);
        break;
    default:
        break;
    }
}

BOOL SetupEncListenerContext(EncListenerContext* ctx, CNMComponentConfig* config)
{
    TestEncConfig* encConfig = &config->testEncConfig;

    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    osal_memset((void*)ctx, 0x00, sizeof(EncListenerContext));

    if (encConfig->compareType & (1 << MODE_COMP_ENCODED)) {
        if ((ctx->es=Comparator_Create(STREAM_COMPARE, encConfig->ref_stream_path, encConfig->cfgFileName)) == NULL) {
            VLOG(ERR, "%s:%d Failed to Comparator_Create(%s)\n", __FUNCTION__, __LINE__, encConfig->ref_stream_path);
            return FALSE;
        }
    }
    ctx->coreIdx       = encConfig->coreIdx;
    ctx->streamEndian  = encConfig->stream_endian;
    ctx->match         = TRUE;
    ctx->matchOtherInfo= TRUE;
    ctx->performance   = encConfig->performance;
    ctx->bandwidth     = encConfig->bandwidth;
    ctx->pfClock       = encConfig->pfClock;
    osal_memcpy(ctx->cfgFileName, encConfig->cfgFileName, sizeof(ctx->cfgFileName));
    ctx->ringBufferEnable     = encConfig->ringBufferEnable;
    ctx->ringBufferWrapEnable = encConfig->ringBufferWrapEnable;

    return TRUE;
}

void ClearEncListenerContext(EncListenerContext* ctx)
{
    if (ctx->es)    Comparator_Destroy(ctx->es);
}

