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

#include "lwip/tcpip.h"
#include "lwip/sockets.h"

#include "rtspd.h"
#include "rtp_g711.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__)
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__)
#else
#define ENTER()
#define LEAVE()
#endif

#ifdef __AUDIO__

BYTE tempAudioBuf[2048];

int rtspd_client_rtp_g711_main(rtsp_client *prcInfo)
{
	ENTER();

	switch (prcInfo->eTransport) {
		case ENX_RTSP_TRANSPORT_UDP:
		case ENX_RTSP_TRANSPORT_TCP:
			break;
		case ENX_RTSP_TRANSPORT_HTTP:
		default:
			LEAVE();
			return -1;
	}

	rtp_session *rtpSession = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numAUDIO]);
	rtsp_connect_info *rtspConnect = &(prcInfo->conn_info);
	rtpSession->buf_idx = (rtpSession->buf_idx + 1) % 2;

	UINT remaining = TCP_MSS;
	BYTE *send_buffer = rtpSession->buffer[rtpSession->buf_idx];
	rtp_queue_data pdata;
	BYTE *base_offset;

	rthInterleaved *rtspHead = NULL;
	rthRTPHeader *rtpHead = NULL;
	UINT g711left;

	switch (rtpSession->tx_ready) {
		case ENX_RTP_TXSTE_READY:
			if (rtp_queue_get(prcInfo->rtp_queue_audio, &pdata) == ENX_OK) {
				rtpSession->rtp_pk.base = pdata.addr;
				rtpSession->rtp_pk.size = pdata.size;
				rtpSession->rtp_pk.offset = 0;
				rtpSession->start_rtptime += pdata.ts;

				rtpSession->tx_ready = ENX_RTP_TXSTE_DOING;
				// This case goes directly to "ENX_RTP_TXSTE_DOING".
			} else {
				break; // switch break;
			} // @suppress("No break at end of case")
		case ENX_RTP_TXSTE_DOING:
			{
				// 남은 g711의 크기
				g711left = rtpSession->rtp_pk.size - rtpSession->rtp_pk.offset;

				// base기준 전송을 보내야 할 g711 주소
				base_offset = (BYTE *)(intptr_t)(rtpSession->rtp_pk.base + rtpSession->rtp_pk.offset);

				// RTSP Interleaved(4byte)
				if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_TCP || prcInfo->eTransport == ENX_RTSP_TRANSPORT_HTTP) {
					rtspHead = (rthInterleaved *)send_buffer;
					rtspHead->un8Magic = '$';
					rtspHead->un8Channel = rtspConnect->rtp_port[ENX_RTSP_STRTYPE_numAUDIO]; // TCP에서는 rtp_port는 Channel로 사용됨
					rtspHead->un16Length = 0;
					rtpSession->buf_len[rtpSession->buf_idx] = sizeof(rthInterleaved);
				}

				// RTP Header(12byte)
				rtpHead = (rthRTPHeader *)(send_buffer + rtpSession->buf_len[rtpSession->buf_idx]);
				rtpHead->un2RTPVersion = RTP_numRTPVer;
				rtpHead->un1Padding = 0;
				rtpHead->un1Extension = 0;
				rtpHead->un4CSRCcount = 0;
				rtpHead->un1Markerbit = 0;
				rtpHead->un7Payloadtype = rtpSession->payload_type;
				rtpHead->un16Sequencenum =  htons(++rtpSession->start_seq);
				rtpHead->un32Timestamp = htonl(rtpSession->start_rtptime);
				rtpHead->un32SSIdentifier = htonl(rtpSession->ssrc);
				rtpSession->buf_len[rtpSession->buf_idx] += sizeof(rthRTPHeader);

				remaining -= rtpSession->buf_len[rtpSession->buf_idx];
				if (g711left < remaining) {
					remaining = g711left;
				}

#if 1
				BDmaMemCpy_rtos_flush(RTSPD_USE_DMA, send_buffer + rtpSession->buf_len[rtpSession->buf_idx], base_offset, remaining);
				//memcpy(send_buffer + rtpSession->buf_len[rtpSession->buf_idx], base_offset, remaining);
#else
				BDmaMemCpy_rtos_flush(RTSPD_USE_DMA, tempAudioBuf, base_offset, remaining);
				//for (int k = 0; k < remaining; k += 2) {
					//BYTE temp = tempAudioBuf[k];
					//tempAudioBuf[k] = tempAudioBuf[k+1];
					//tempAudioBuf[k+1] = temp;
				//}
				BDmaMemCpy_rtos_flush(RTSPD_USE_DMA, send_buffer + rtpSession->buf_len[rtpSession->buf_idx], tempAudioBuf, remaining);
#endif

				rtpSession->rtp_pk.offset += remaining;
				rtpSession->buf_len[rtpSession->buf_idx] += remaining;

				if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_TCP || prcInfo->eTransport == ENX_RTSP_TRANSPORT_HTTP) {
					rtspHead->un16Length = htons(rtpSession->buf_len[rtpSession->buf_idx] - sizeof(rthInterleaved));
				}

				if (rtpSession->rtp_pk.offset == rtpSession->rtp_pk.size) {
					rtpHead->un1Markerbit = 1;
					rtpSession->tx_ready = ENX_RTP_TXSTE_END;
					// This case goes directly to "ENX_RTP_TXSTE_END".
				} else {
					break;
					// switch break;
				}
			} // @suppress("No break at end of case")
		case ENX_RTP_TXSTE_END:
			rtpSession->tx_ready = ENX_RTP_TXSTE_READY;
			break;
	}

	LEAVE();
	return (rtpSession->rtp_pk.size - rtpSession->rtp_pk.offset);
}
#endif
#endif
