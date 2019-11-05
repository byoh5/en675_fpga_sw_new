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

#ifndef _RTP_QUEUE_H_
#define _RTP_QUEUE_H_

typedef struct {
	UINT addr;
	UINT size;
	UINT flags;
	UINT ts;
} rtp_queue_data;

typedef struct {
	UINT head;
	UINT tail;
	UINT tot_num; // total num of buffer
	rtp_queue_data *data;
} rtp_queue;

extern ENX_OKFAIL rtp_queue_init(rtp_queue **prqbuf, UINT tot_num);

extern ENX_OKFAIL rtp_queue_put(rtp_queue *prqbuf, rtp_queue_data *prefdata);

extern ENX_OKFAIL rtp_queue_get(rtp_queue *prqbuf, rtp_queue_data *pdata);

extern ENX_OKFAIL rtp_queue_peek(rtp_queue *prqbuf, rtp_queue_data *pdata);

extern ENX_OKFAIL rtp_queue_freebuffer(rtp_queue *prqbuf);

extern ENX_OKFAIL rtp_queue_free(rtp_queue **prqbuf);

#endif
