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
#include "vpuconfig.h"

typedef enum {
    READER_STATE_OPEN,
    READER_STATE_READING,
} EncReaderState;

typedef struct {
    EncHandle       handle;
    Uint32          streamBufCount;
    Uint32          streamBufSize;
    vpu_buffer_t*   bsBuffer;
    Uint32          coreIdx;
    EndianMode      streamEndian;
    EncReaderState  state;
    char            bitstreamFileName[MAX_FILE_PATH];
    osal_file_t     fp;
    BOOL            ringBuffer;
    BOOL            ringBufferWrapEnable;
    Int32           productID;
} ReaderContext;

static CNMComponentParamRet GetParameterReader(ComponentImpl* from, ComponentImpl* com, GetParameterCMD commandType, void* data) 
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    ReaderContext*           ctx    = (ReaderContext*)com->context;
    BOOL                     result = TRUE;
    ParamEncBitstreamBuffer* bsBuf  = NULL;
    PortContainer*          container;

    switch(commandType) {
    case GET_PARAM_COM_IS_CONTAINER_CONUSUMED:
        container = (PortContainer*)data;
        container->consumed = TRUE;
        break;
    case GET_PARAM_READER_BITSTREAM_BUF:
        if (ctx->bsBuffer == NULL) return CNM_COMPONENT_PARAM_NOT_READY;
        bsBuf      = (ParamEncBitstreamBuffer*)data;
        bsBuf->bs  = ctx->bsBuffer;
        bsBuf->num = ctx->streamBufCount;
        break;
    default:
        return CNM_COMPONENT_PARAM_NOT_FOUND;
    }

    return (result == TRUE) ? CNM_COMPONENT_PARAM_SUCCESS : CNM_COMPONENT_PARAM_FAILURE;
}

static CNMComponentParamRet SetParameterReader(ComponentImpl* from, ComponentImpl* com, SetParameterCMD commandType, void* data) 
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    BOOL result = TRUE;

    switch(commandType) {
    default:
        result = FALSE;
        break;
    }

    return (result == TRUE) ? CNM_COMPONENT_PARAM_SUCCESS : CNM_COMPONENT_PARAM_FAILURE;
}

static BOOL ExecuteReader(ComponentImpl* com, PortContainer* in, PortContainer* out) 
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    ReaderContext*          ctx     = (ReaderContext*)com->context;
    PortContainerES*        srcData = (PortContainerES*)in;
    BOOL                    success = TRUE;
    CNMComponentParamRet    ret;

#if 0//def EYENIX_TEST
    dbgprintf("[%s,  %d]\n", __func__, __LINE__);
    return TRUE;
#endif

    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    srcData->reuse = FALSE;

    switch (ctx->state) {
    case READER_STATE_OPEN:
    	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
        ret = ComponentGetParameter(com, com->srcPort.connectedComponent, GET_PARAM_ENC_HANDLE, &ctx->handle); 
        dbgprintf("[debug] %s %d\n",__func__,__LINE__);
        if (ComponentParamReturnTest(ret, &success) == FALSE) {
            return success;
        }

        ctx->state = READER_STATE_READING;
        srcData->reuse  = TRUE;
        break;
    case READER_STATE_READING:
    	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
        if (srcData->size > 0 || 
            (ctx->ringBuffer == TRUE && ctx->ringBufferWrapEnable == FALSE && srcData->last == TRUE) ) {
            if ( ctx->ringBuffer == TRUE) {
            	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
//                Uint8* buf = (Uint8*)osal_malloc(srcData->size);
                PhysicalAddress rd, paBsBufStart, paBsBufEnd;
                Int32           readSize;
                Uint32          room;

                rd = srcData->rdPtr;
                paBsBufStart = srcData->paBsBufStart;
                paBsBufEnd   = srcData->paBsBufEnd;
                readSize = srcData->size;

                if (ctx->ringBufferWrapEnable == TRUE ) {
                	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
                    if ((rd+readSize) > paBsBufEnd) {
                    room = paBsBufEnd - rd;
//                    vdi_read_memory(ctx->coreIdx, rd, buf, room,  ctx->streamEndian);
//                    vdi_read_memory(ctx->coreIdx, paBsBufStart, buf+room, (readSize-room), ctx->streamEndian);
                    } 
                    else {
//                        vdi_read_memory(ctx->coreIdx, rd, buf, readSize, ctx->streamEndian);
                    }
                    VPU_EncUpdateBitstreamBuffer(ctx->handle, readSize);
                }
                else { //ring=1, wrap=0
                	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
                    if (srcData->streamBufFull == TRUE || srcData->last == TRUE) { // read whole data at once and no UpdateBS
//                        vdi_read_memory(ctx->coreIdx, rd, buf, readSize, ctx->streamEndian);
                    }
                }
                if (srcData->streamBufFull == TRUE) {
                	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
                    if (ctx->ringBufferWrapEnable == FALSE )
                        vdi_free_dma_memory(ctx->coreIdx, &srcData->buf, ENC_BS, ctx->handle->instIndex);
                    srcData->buf.size = 0;
                }
               if (ctx->fp) {
//                    osal_fwrite(buf, srcData->size, 1, ctx->fp);
                }
//                osal_free(buf);
            }
            else {
            	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
//                Uint8* buf = (Uint8*)osal_malloc(srcData->size);
//                vdi_read_memory(ctx->coreIdx, srcData->buf.phys_addr, buf, srcData->size, ctx->streamEndian);
                VPU_EncUpdateBitstreamBuffer(ctx->handle, srcData->size);

                dbgprintf("[debug] %s %d\n",__func__,__LINE__);
                if(PRODUCT_ID_NOT_W_SERIES(ctx->productID) && TRUE == srcData->streamBufFull) {
                    VPU_ClearInterrupt(ctx->coreIdx);
                }
                if (srcData->streamBufFull == TRUE) {
                    srcData->buf.phys_addr = 0;
                }
                if (ctx->fp) {
#ifdef EYENIX_TEST
//                	ctx->fp = osal_fopen(NULL, "reader");
//                	osal_fwrite(buf, srcData->size, 1, ctx->fp);
#else
                    osal_fwrite(buf, srcData->size, 1, ctx->fp);
#endif

                }
//                osal_free(buf);



#if 0//def EYENIX_TEST
#define ENC_BITSTREAM_ADDR 0x84000000//0x90000000//0x8F300000//0X80600000
#define ENC_BITSTREAM_END_ADDR 0x8FF00000//0x87800000//0x90400000//0x8FFFFFFF//0X80600000
#define BYTE_ALIGN_SIZE   	16
#define BYTE_ALIGN(x) 		(((x) + BYTE_ALIGN_SIZE - 1) & ~(BYTE_ALIGN_SIZE - 1))
                	static UINT TESTOFFSET[4] = {0,0,0,0};

                	BYTE *TESTOUT = ENC_BITSTREAM_ADDR + (0x1000000*ctx->handle->instIndex);

                	dbgprintf("\nInstance Index %d TESTOUT %x offset %x buf addr %x size %d[%d]\n", ctx->handle->instIndex, TESTOUT, TESTOFFSET[ctx->handle->instIndex], srcData->buf.phys_addr, srcData->size,BYTE_ALIGN(srcData->size));

                	osal_memcpy(TESTOUT+TESTOFFSET[ctx->handle->instIndex], srcData->buf.phys_addr, BYTE_ALIGN(srcData->size));

                	dbgprintf("write done addr %x ", TESTOUT+TESTOFFSET[ctx->handle->instIndex]);

                	TESTOFFSET[ctx->handle->instIndex] += BYTE_ALIGN(srcData->size);

                	dbgprintf("total size %d 0x%x\n", TESTOFFSET[ctx->handle->instIndex], TESTOFFSET[ctx->handle->instIndex]);

					if((TESTOUT+TESTOFFSET[ctx->handle->instIndex]) > TESTOUT+0xFF0000)
					{
						dbgprintf(" inst(%d) Bitstream area over %x !!!!!!!!!!\n",  ctx->handle->instIndex, (TESTOUT+TESTOFFSET[ctx->handle->instIndex]));
						//while(1);
						TESTOFFSET[ctx->handle->instIndex] = 0;
						dbgprintf("Reset TESTOUT %x !!!!!!!!!!\n", TESTOUT + TESTOFFSET[ctx->handle->instIndex]);
					}

#if 0
					static UINT TESTOFFSET[4] = {0,0,0,0};

					if(encOutputInfo.reconFrameIndex == RECON_IDX_FLAG_ENC_END)
						dbgprintf("done total size %d\n", TESTOFFSET[ctx->handle->instIndex] + BYTE_ALIGN(encOutputInfo.bitstreamSize));

					if(encOutputInfo.reconFrameIndex == RECON_IDX_FLAG_HEADER_ONLY)
					{


						BYTE *TESTOUT = ENC_BITSTREAM_ADDR;//0X8dd00000; //0x85A00000;

						dbgprintf("\nInstance Index %d\n", ctx->handle->instIndex);

						if(ctx->handle->instIndex > 0)
						{
							TESTOUT+=(0x1000000*ctx->handle->instIndex);
							dbgprintf("\nInstance Index %d TESTOUT offset %x \n", ctx->handle->instIndex, TESTOUT);
						}

						osal_memcpy(TESTOUT, encOutputInfo.bitstreamBuffer, BYTE_ALIGN(encOutputInfo.bitstreamSize));
						TESTOFFSET[ctx->handle->instIndex] = BYTE_ALIGN(encOutputInfo.bitstreamSize);
						dbgprintf("TESTOUT %x size %d TESTOFFSET[%d] %d \n", TESTOUT, BYTE_ALIGN(encOutputInfo.bitstreamSize), ctx->handle->instIndex,TESTOFFSET[ctx->handle->instIndex]);

						dbgprintf("\n");
						for(i=0;i<32;i++)
							dbgprintf("%x ", *(TESTOUT+i));
						dbgprintf("\n");

					}


					if(encOutputInfo.reconFrameIndex >= 0)
					{
						BYTE *TESTOUT = ENC_BITSTREAM_ADDR;//0X8dd00000; //0x85A00000;

						dbgprintf("\nInstance Index %d\n", ctx->handle->instIndex);

						if(ctx->handle->instIndex > 0)
						{
							TESTOUT+=(0x1000000*ctx->handle->instIndex);
							dbgprintf("\nInstance Index %d TESTOUT offset %x \n", ctx->handle->instIndex, TESTOUT);
						}

						if((TESTOUT+TESTOFFSET[ctx->handle->instIndex]) + BYTE_ALIGN(encOutputInfo.bitstreamSize) > TESTOUT+0x1000000)
						{
							dbgprintf(" inst(%d) Bitstream area over %x !!!!!!!!!!\n",  ctx->handle->instIndex, TESTOUT + BYTE_ALIGN(encOutputInfo.bitstreamSize));
							//while(1);
							TESTOFFSET[ctx->handle->instIndex] = 0;
							dbgprintf("Reset TESTOUT %x !!!!!!!!!!\n", TESTOUT + BYTE_ALIGN(TESTOFFSET[ctx->handle->instIndex]));
						}

						dbgprintf("TESTOUT+TESTOFFSET[%d] %x size %d TESTOFFSET[%d] %d \n", ctx->handle->instIndex, TESTOUT+TESTOFFSET[ctx->handle->instIndex], BYTE_ALIGN(encOutputInfo.bitstreamSize), ctx->handle->instIndex,TESTOFFSET[ctx->handle->instIndex]);
						osal_memset(TESTOUT+TESTOFFSET[ctx->handle->instIndex], 0x0, BYTE_ALIGN(encOutputInfo.bitstreamSize));
						osal_memcpy(TESTOUT+TESTOFFSET[ctx->handle->instIndex], encOutputInfo.bitstreamBuffer,BYTE_ALIGN(encOutputInfo.bitstreamSize));
						dbgprintf("TESTOUT %x size %d TESTOFFSET[%d] %d \n", TESTOUT+TESTOFFSET[ctx->handle->instIndex], BYTE_ALIGN(encOutputInfo.bitstreamSize), ctx->handle->instIndex, TESTOFFSET[ctx->handle->instIndex]);
				#if 0

				#include "main_helper.h"
						// if(encOutputInfo.reconFrameIndex == ctx->testEncConfig.outNum-1)
						if(encOutputInfo.encPicCnt == ctx->testEncConfig.outNum)
						{
							int         changes;
							int         sys_endian;
							int 		endian;
							BOOL        byteChange, wordChange, dwordChange, lwordChange;

							byteChange  = 1;
							wordChange  = 1;
							dwordChange = 1;
							lwordChange = 1;

							dbgprintf("byteChange %d wordChange %d dwordChange %d lwordChange %d\n", byteChange,wordChange, dwordChange, lwordChange);
							TESTOFFSET[ctx->handle->instIndex] += BYTE_ALIGN(16);
							if (byteChange)  byte_swap(TESTOUT, (int)BYTE_ALIGN(encOutputInfo.bitstreamSize)+TESTOFFSET[ctx->handle->instIndex]); //encOutputInfo.bitstreamSize
							if (wordChange)  word_swap(TESTOUT, (int)BYTE_ALIGN(encOutputInfo.bitstreamSize)+TESTOFFSET[ctx->handle->instIndex]); //encOutputInfo.bitstreamSize
							if (dwordChange) dword_swap(TESTOUT, (int)BYTE_ALIGN(encOutputInfo.bitstreamSize)+TESTOFFSET[ctx->handle->instIndex]); //encOutputInfo.bitstreamSize
							if (lwordChange) lword_swap(TESTOUT, (int)BYTE_ALIGN(encOutputInfo.bitstreamSize)+TESTOFFSET[ctx->handle->instIndex]); //encOutputInfo.bitstreamSize

							dbgprintf("swaping... \n");

							for(i=0;i<16;i++)
								dbgprintf("%x ", *(TESTOUT+i));

						}
#endif

						//if(encOutputInfo.encPicCnt == ctx->testEncConfig.outNum)
						if(encOutputInfo.reconFrameIndex == RECON_IDX_FLAG_ENC_END)
							dbgprintf("done total size %d\n", TESTOFFSET[ctx->handle->instIndex] + BYTE_ALIGN(encOutputInfo.bitstreamSize));

						TESTOFFSET[ctx->handle->instIndex] += BYTE_ALIGN(encOutputInfo.bitstreamSize);
					}
#endif
#endif
            }
            if (TRUE == srcData->streamBufFull) {
                srcData->streamBufFull = FALSE;
            }
        }

        dbgprintf("[debug] %s %d\n",__func__,__LINE__);
        srcData->consumed = TRUE;
        com->terminate    = srcData->last;
        break;
    default:
        success = FALSE;
        break;
    }

    return success;
}

static BOOL PrepareReader(ComponentImpl* com, BOOL* done)
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    ReaderContext*       ctx = (ReaderContext*)com->context;
    Uint32               i;
    vpu_buffer_t*        bsBuffer;
    Uint32               num = ctx->streamBufCount;


    *done = FALSE;
    bsBuffer = (vpu_buffer_t*)osal_malloc(num*sizeof(vpu_buffer_t));
    for (i = 0; i < num; i++) {
        bsBuffer[i].size = ctx->streamBufSize;
        if (vdi_allocate_dma_memory(ctx->coreIdx, &bsBuffer[i], ENC_BS, /*ctx->handle->instIndex*/0) < 0) {
            VLOG(ERR, "%s:%d fail to allocate bitstream buffer\n", __FUNCTION__, __LINE__);
            osal_free(bsBuffer);
            return FALSE;
        }
    }
    ctx->bsBuffer   = bsBuffer;
    ctx->state      = READER_STATE_OPEN;
    if ( ctx->bitstreamFileName[0] ) {
#ifdef EYENIX_TEST
    	//if ((ctx->fp=osal_fopen(ctx->bitstreamFileName, "wb")) == NULL) {
    	if ((ctx->fp=osal_fopen(ctx->bitstreamFileName, "reader")) == NULL) {
#else
        if ((ctx->fp=osal_fopen(ctx->bitstreamFileName, "wb")) == NULL) {
#endif
            return FALSE;
        }
        VLOG(INFO, "output bin file: %s\n", ctx->bitstreamFileName);
    }

    *done = TRUE;

    return TRUE;
}

static void ReleaseReader(ComponentImpl* com)
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    ReaderContext*  ctx = (ReaderContext*)com->context;
    Uint32          i   = 0;

    if (ctx->bsBuffer != NULL) {
        for (i = 0; i < ctx->streamBufCount ; i++) {
            if (ctx->bsBuffer[i].size)
                vdi_free_dma_memory(ctx->coreIdx, &ctx->bsBuffer[i], ENC_BS, 0);
        }
    }

}

static BOOL DestroyReader(ComponentImpl* com) 
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    ReaderContext*  ctx = (ReaderContext*)com->context;

    if (ctx->bsBuffer != NULL) osal_free(ctx->bsBuffer);
    if (ctx->fp) osal_fclose(ctx->fp);
    osal_free(ctx);

    return TRUE;
}

static Component CreateReader(ComponentImpl* com, CNMComponentConfig* componentParam) 
{
	dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    ReaderContext* ctx;

    com->context = osal_malloc(sizeof(ReaderContext));
    ctx = (ReaderContext*)com->context;
    osal_memset((void*)ctx, 0, sizeof(ReaderContext));

    strcpy(ctx->bitstreamFileName, componentParam->testEncConfig.bitstreamFileName);
    ctx->handle       = NULL;
    ctx->coreIdx      = componentParam->testEncConfig.coreIdx;
    ctx->productID    = componentParam->testEncConfig.productId;
    ctx->streamEndian = (EndianMode)componentParam->testEncConfig.stream_endian;

    ctx->streamBufCount = componentParam->encOpenParam.streamBufCount;
    ctx->streamBufSize  = componentParam->encOpenParam.streamBufSize;
    ctx->ringBuffer     = componentParam->encOpenParam.ringBufferEnable;
    ctx->ringBufferWrapEnable = componentParam->encOpenParam.ringBufferWrapEnable;

    return (Component)com;
}

ComponentImpl readerComponentImpl = {
    "reader",
    NULL,
    {0,},
    {0,},
    sizeof(PortContainer),
    5,
    CreateReader,
    GetParameterReader,
    SetParameterReader,
    PrepareReader,
    ExecuteReader,
    ReleaseReader,
    DestroyReader,
};

