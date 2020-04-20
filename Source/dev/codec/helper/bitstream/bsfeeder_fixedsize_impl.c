//--=========================================================================--
//  This file is a part of VPU Reference API project
//-----------------------------------------------------------------------------
//
//       This confidential and proprietary software may be used only
//     as authorized by a licensing agreement from Chips&Media Inc.
//     In the event of publication, the following notice is applicable:
//
//            (C) COPYRIGHT CHIPS&MEDIA INC.
//                      ALL RIGHTS RESERVED
//
//       The entire notice above must be reproduced on all authorized
//       copies.
//
//--=========================================================================--
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "vpuapifunc.h"
#include "main_helper.h"

#define MAX_FEEDING_SIZE        0x400000        /* 4MBytes */
#define DEFAULT_FEEDING_SIZE    0x40000         /* 128KBytes */

typedef struct FeederFixedContext {
	fileio_buf_t *    fp;
    Uint32          feedingSize;
    BOOL            eos;
} FeederFixedContext;

void* BSFeederFixedSize_Create(
    const char* path,
    CodStd      codecId
    )
{
    osal_file_t    fp = NULL;
    FeederFixedContext*  context=NULL;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    UNREFERENCED_PARAMETER(codecId);

    //if ((fp=osal_fopen(path, "rb")) == NULL) {
    if ((fp=osal_fopen(path, "bit")) == NULL) {
        VLOG(ERR, "%s:%d failed to open %s\n", __FUNCTION__, __LINE__, path);
        return NULL;
    }

    context = (FeederFixedContext*)osal_malloc(sizeof(FeederFixedContext));
    if (context == NULL) {
        VLOG(ERR, "%s:%d failed to allocate memory\n", __FUNCTION__, __LINE__);
        osal_fclose(fp);
        return NULL;
    }

    context->fp          = fp;
    context->feedingSize = DEFAULT_FEEDING_SIZE;
    context->eos         = FALSE;

    return (void*)context;
}

BOOL BSFeederFixedSize_Destroy(
    void* feeder
    )
{
    FeederFixedContext* context = (FeederFixedContext*)feeder;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (context == NULL) {
        VLOG(ERR, "%s:%d Null handle\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (context->fp) 
        osal_fclose(context->fp);

    osal_free(context);

    return TRUE;
}

Int32 BSFeederFixedSize_Act(
    void*       feeder,
    BSChunk*    chunk
    )
{
    FeederFixedContext*  context = (FeederFixedContext*)feeder;
    size_t          nRead;
    Uint32          size;
    Uint32          feedingSize;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (context == NULL) {
        VLOG(ERR, "%s:%d Null handle\n", __FUNCTION__, __LINE__);
        return 0;
    }

    if (context->eos == TRUE) {
        chunk->eos = TRUE;
        return 0;
    }

    feedingSize = context->feedingSize;
    if (feedingSize == 0) {
        Uint32  KB = 1024;
        BOOL    probability10;

        srand((Uint32)time(NULL));
        feedingSize   = rand() % MAX_FEEDING_SIZE;
        probability10 = (BOOL)((feedingSize%100) < 10);
        if (feedingSize < KB) {
            if (probability10 == FALSE) 
                feedingSize *= 100;
        }
    }

    size = (chunk->size < feedingSize) ? chunk->size : feedingSize;

    do {
#ifdef EYENIX_TEST_HEAP
        //nRead = osal_fread(chunk->data, 1, size, context->fp);
    	chunk->data = (uint8_t *)context->fp->_base +(unsigned int)context->fp->_ptr;
    	context->fp->_ptr += size;
    	nRead = size;

    	if(context->fp->_ptr> DEC_TEST_SIZE)
    	{
    		dbgprintf("TEST DECODE BITSTREAM OVER !! reset file pointer back!\n");
    		context->fp->_ptr = 0;
    	}
#else
        nRead = osal_fread(chunk->data, 1, size, context->fp);
#endif
        if ((Int32)nRead < 0) {
            VLOG(ERR, "%s:%d failed to read bitstream(errno: %d)\n", __FUNCTION__, __LINE__, errno);
            return 0;
        } 
        else if (nRead < size) {
            context->eos = TRUE;
            chunk->eos   = TRUE;

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

    if (byteChange)  byte_swap(chunk->data, (int)nRead);
    if (wordChange)  word_swap(chunk->data, (int)nRead);
    if (dwordChange) dword_swap(chunk->data, (int)nRead);
    if (lwordChange) lword_swap(chunk->data, (int)nRead);

#endif

        }
    } while (FALSE);

    return nRead;
}

BOOL BSFeederFixedSize_Rewind(
    void*       feeder
    )
{
    FeederFixedContext*  context = (FeederFixedContext*)feeder;
    Int32           ret;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if ((ret=osal_fseek(context->fp, 0, SEEK_SET)) != 0) {
        VLOG(ERR, "%s:%d Failed to fseek(ret: %d)\n", __FUNCTION__, __LINE__, ret);
        return FALSE;
    }
    context->eos = FALSE;

    return TRUE;
}

void BSFeederFixedSize_SetFeedingSize(
    void*   feeder,
    Uint32  feedingSize
    )
{
    FeederFixedContext*  context = (FeederFixedContext*)feeder;
    dbgprintf("[debug] %s %d\n",__func__,__LINE__);
    if (feedingSize > 0) {
        context->feedingSize = feedingSize;
    }
}

