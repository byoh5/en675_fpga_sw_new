/* Copyright (c) 2013-18 Eyenix Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Eyenix may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Eyenix
* product.
*
* THIS SOFTWARE IS PROVIDED BY EYENIX "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EYENIX BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*/

#include "dev.h"

#if (ENX_RTSP_use==1)

#include <string.h> // memset

#include "enx_freertos.h"
#include "rtspd.h"

#if 1
#define ENTER() printf(" IN %04d:%s\n", __LINE__, __func__)
#define LEAVE() printf("OUT %04d:%s\n", __LINE__, __func__)
#else
#define ENTER()
#define LEAVE()
#endif

extern rtsp_server rsInfo;
SemaphoreHandle_t semRtpQ = NULL;
int rtp_queue_cnt = 0;

ENX_OKFAIL rtp_queue_init(rtp_queue **prqbuf, UINT tot_num)
{
	if (semRtpQ == NULL) {
		vSemaphoreCreateBinary(semRtpQ);
	}

	*prqbuf = (rtp_queue *)pvPortCalloc(1, sizeof(rtp_queue));
	if (*prqbuf != NULL) {
		_Rprintf("MALLOC RTP Queue : 0x%08X\n", *prqbuf);
		(*prqbuf)->data = (rtp_queue_data *)pvPortCalloc(tot_num, sizeof(rtp_queue_data));
		if ((*prqbuf)->data != NULL) {
			(*prqbuf)->tot_num = tot_num;
			return ENX_OK;
		}
	}
	return ENX_FAIL;
}

ENX_OKFAIL rtp_queue_put(rtp_queue *prqbuf, rtp_queue_data *prefdata)
{
	ENX_OKFAIL bRes = ENX_FAIL;

// 	flprintf("addr(0x%08X) size(%8d) flag(%d) ts(%5d)\n", prefdata->addr, prefdata->size, prefdata->flags, prefdata->ts);

	if (cQueue_isfull(prqbuf) == ENX_OK) {
		flprintf("full!\n");
	}
	// else
	{
//		if (xSemaphoreTake(semRtpQ, portMAX_DELAY)) {
			rtp_queue_data *pdata = &(prqbuf->data[prqbuf->tail]);
			pdata->addr = prefdata->addr;
			pdata->size = prefdata->size;
			pdata->flags = prefdata->flags;
			pdata->ts = prefdata->ts;

			num_loop(prqbuf->tail, prqbuf->tot_num);

		//	printf("P:%9u-%2d/%2d\n", *mtime, prqbuf->head, prqbuf->tail);

			bRes = ENX_OK;

//			xSemaphoreGive(semRtpQ);
//		}
	}

	return bRes;
}

ENX_OKFAIL rtp_queue_get(rtp_queue *prqbuf, rtp_queue_data *pdata)
{
	BYTE bRes = ENX_FAIL;

	if (cQueue_isempty(prqbuf) == ENX_FAIL) {
		if (xSemaphoreTake(semRtpQ, portMAX_DELAY)) {
			portENTER_CRITICAL();

			rtp_queue_data *prefdata = &(prqbuf->data[prqbuf->head]);
			pdata->addr = prefdata->addr;
			pdata->size = prefdata->size;
			pdata->flags = prefdata->flags;
			pdata->ts = prefdata->ts;

			num_loop(prqbuf->head, prqbuf->tot_num);

			bRes = ENX_OK;

			portEXIT_CRITICAL();
			xSemaphoreGive(semRtpQ);
		}
	}

//	printf("G:%9u-%2d/%2d-%s\n", *mtime, prqbuf->head, prqbuf->tail, bRes == ENX_OK ? "Get" : ".");

	return bRes;
}

ENX_OKFAIL rtp_queue_peek(rtp_queue *prqbuf, rtp_queue_data *pdata)
{
	ENX_OKFAIL bRes = ENX_FAIL;
	if (cQueue_isempty(prqbuf) == ENX_FAIL) {
		if (xSemaphoreTake(semRtpQ, portMAX_DELAY)) {
			portENTER_CRITICAL();

			rtp_queue_data *prefdata = &(prqbuf->data[prqbuf->head]);
			pdata->addr = prefdata->addr;
			pdata->size = prefdata->size;
			pdata->flags = prefdata->flags;
			pdata->ts = prefdata->ts;

			bRes = ENX_OK;

			portEXIT_CRITICAL();
			xSemaphoreGive(semRtpQ);
		}
	}
	return bRes;
}

ENX_OKFAIL rtp_queue_freebuffer(rtp_queue *prqbuf)
{
	ENX_OKFAIL bRes = ENX_FAIL;

	portENTER_CRITICAL();

	prqbuf->head = 0;
	prqbuf->tail = 0;
	memset(prqbuf->data, 0, sizeof(rtp_queue_data) * prqbuf->tot_num);

	bRes = ENX_OK;

	portEXIT_CRITICAL();

	return bRes;
}

ENX_OKFAIL rtp_queue_free(rtp_queue **prqbuf)
{
	BYTE bRes = ENX_FAIL;

	portENTER_CRITICAL();
	flprintf("\n");
	if (*prqbuf != NULL) {
		_Rprintf("FREE RTP Queue : 0x%08X\n", *prqbuf);
		flprintf("\n");
		(*prqbuf)->head = 0;
		flprintf("\n");
		(*prqbuf)->tail = 0;
		flprintf("\n");
		(*prqbuf)->tot_num = 0;
		flprintf("\n");
		if ((*prqbuf)->data != NULL) {
			flprintf("\n");
			vPortFree((*prqbuf)->data);
			flprintf("\n");
		}
		flprintf("\n");
		vPortFree(*prqbuf);
		*prqbuf = NULL;
		flprintf("\n");
	}

	bRes = ENX_OK;

	portEXIT_CRITICAL();

	return bRes;
}
#endif
