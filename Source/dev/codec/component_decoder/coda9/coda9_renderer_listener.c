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
#include "coda960_renderer_listener.h"
#include "misc/debug.h"

void RendererListener(Component com, Uint32 event, void* data, void* context)
{
    int key=0;
    if (osal_kbhit()) {
        key = osal_getch();
        osal_flush_ch();
        if (key == 'q' || key == 'Q') {
            CNMAppStop();
            return;
        }
    }
    switch (event) {
    case COMPONENT_EVENT_RENDER_ALLOCATE_FRAMEBUFER :
        HandleRenderAllocateFramebufferEvent(com, data, context);
        break;
    default:
        break;
    }
}

BOOL SetupRendererListenerContext(RendererListenerContext* ctx, CNMComponentConfig* config, Component encoder)
{
    osal_memset((void*)ctx, 0x00, sizeof(RendererListenerContext));

    ctx->encoder       = encoder;
    ctx->srcFbNum       = 0;
    if(NULL != ctx->encoder) ctx->trans = TRUE;

    return TRUE;
}

void ClearRendererListenerContext(RendererListenerContext* ctx)
{
    //Do nothing yet
}

void HandleRenderAllocateFramebufferEvent(Component com, ParamRenderAllocInfo* param, RendererListenerContext* ctx)
{
    CNMComponentParamRet ret;
    ParamEncNeedFrameBufferNum fbCnt;

    osal_memset(&fbCnt, 0x00, sizeof(ParamEncNeedFrameBufferNum));

    param->isConnectedEnc = ctx->trans;

    if(ctx->trans) {
        ret =ComponentGetParameter(com, ctx->encoder, GET_PARAM_ENC_FRAME_BUF_NUM, &fbCnt);
        osal_memcpy((void*)&(param->fbCnt), (void*)&fbCnt, sizeof(ParamEncNeedFrameBufferNum));
        param->ret = ComponentParamReturnTest(ret, &(param->success));
        }
    else {
        param->ret = FALSE;
        param->success = FALSE;
    }
}


