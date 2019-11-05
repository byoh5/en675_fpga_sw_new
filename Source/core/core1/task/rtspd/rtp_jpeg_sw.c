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
#if (SW_JPEG_ENCODER==1)
#include "lwip/tcpip.h"
#include "lwip/sockets.h"

#include "rtspd.h"
#include "rtp_jpeg_sw.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__)
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__)
#else
#define ENTER()
#define LEAVE()
#endif

int rtspd_client_rtp_jpeg_sw_main(rtsp_client *prcInfo)
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

	rtp_session *rtpSession = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numVIDEO]);
	rtpSession->buf_idx = (rtpSession->buf_idx + 1) % 2;

	UINT remaining = TCP_MSS;
	BYTE *send_buffer = rtpSession->buffer[rtpSession->buf_idx];
	rtp_queue_data pdata;
	BYTE *base_offset;

	rthInterleaved *rtspHead = NULL;
	rthRTPHeader *rtpHead = NULL;
	jpgswHeader *jpgHead = NULL;
	jpgswQTHeader *jpgQTHerd = NULL;
	BYTE *jpgData = NULL;
	UINT jpegleft;

	switch (rtpSession->tx_ready) {
		case ENX_RTP_TXSTE_READY:
			if (rtp_queue_get(prcInfo->rtp_queue_video, &pdata) == ENX_OK) {
				rtpSession->rtp_pk.base = pdata.addr;
				rtpSession->rtp_pk.size = pdata.size;
				rtpSession->rtp_pk.offset = 0;
				rtpSession->start_rtptime += pdata.ts;

//				hexDump("JPEG_RAW", rtpSession->rtp_pk.base, 352);

				// SOFO를 찾아 JPEG의 size를 check
				BYTE *size_base = (BYTE *)(rtpSession->rtp_pk.base + JPGSW_numOffset_SOFO);
				if (size_base[0] == 0xFF && size_base[1] == 0xC0) {
					rtpSession->rtp_pk.width  = size_base[7] << 8 | size_base[8];
					rtpSession->rtp_pk.height = size_base[5] << 8 | size_base[6];
					rtpSession->rtp_pk.data_offset = 0;
					rtpSession->tx_ready = ENX_RTP_TXSTE_DOING;
				} else {
					flprintf("Error, SOFO, size check fail\r\n");
					break;
				}
				// This case goes directly to "ENX_RTP_TXSTE_DOING".
			} else {
				break; // switch break;
			}
		case ENX_RTP_TXSTE_DOING:
			{
				// 남은 jpeg의 크기
				jpegleft = rtpSession->rtp_pk.size - rtpSession->rtp_pk.offset;

				// base기준 전송을 보내야 할 jpeg 주소
				base_offset = (BYTE *)(rtpSession->rtp_pk.base + prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numVIDEO].rtp_pk.offset);

				// RTSP Interleaved(4byte)
				if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_TCP || prcInfo->eTransport == ENX_RTSP_TRANSPORT_HTTP) {
					rtspHead = (rthInterleaved *)send_buffer;
					rtspHead->un8Magic = '$';
					rtspHead->un8Channel = rtpSession->rtp_port;
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
				rtpHead->un16Sequencenum = ++rtpSession->start_seq;
				rtpHead->un32Timestamp = rtpSession->start_rtptime;
				rtpHead->un32SSIdentifier = rtpSession->ssrc;
				rtpSession->buf_len[rtpSession->buf_idx] += sizeof(rthRTPHeader);

				// JPEG Header(8byte)
				jpgHead = (jpgswHeader *)(send_buffer + rtpSession->buf_len[rtpSession->buf_idx]);
				jpgHead->un8TypeSpecific = 0;
				jpgHead->un24FragmentOffset = rtpSession->rtp_pk.data_offset;
				jpgHead->un8Type = 1;
				jpgHead->un8Q = 128;
				jpgHead->un8Width = rtpSession->rtp_pk.width >> 3;
				jpgHead->un8Height = rtpSession->rtp_pk.height >> 3;
				rtpSession->buf_len[rtpSession->buf_idx] += sizeof(jpgswHeader);

				if (rtpSession->rtp_pk.offset == 0) {
					// JPEG QT Header(4byte)
					jpgQTHerd = (jpgswQTHeader *)(send_buffer + rtpSession->buf_len[rtpSession->buf_idx]);
					jpgQTHerd->un8MBZ = 0;
					jpgQTHerd->un8Precision = 0;
					jpgQTHerd->un16Length = JPGSW_numYCQUNTSize;
					rtpSession->buf_len[rtpSession->buf_idx] += sizeof(jpgswQTHeader);
#if 1
					// Quantization table Copy
					jpgData = send_buffer + rtpSession->buf_len[rtpSession->buf_idx];
					BDmaMemCpy_rtos_flush(RTSPD_USE_DMA, jpgData					, base_offset + JPGSW_numOffset_YQUNT, JPGSW_numQUNTSize); // Y quantization table
					BDmaMemCpy_rtos_flush(RTSPD_USE_DMA, jpgData + JPGSW_numQUNTSize, base_offset + JPGSW_numOffset_CQUNT, JPGSW_numQUNTSize); // C quantization table
					rtpSession->buf_len[rtpSession->buf_idx] += JPGSW_numYCQUNTSize;

					remaining -= rtpSession->buf_len[rtpSession->buf_idx];
					if ((jpegleft - JPGSW_numHeaderSize) < remaining) {
						remaining = jpegleft - JPGSW_numHeaderSize;
					}
					BDmaMemCpy_rtos_flush(RTSPD_USE_DMA, jpgData + JPGSW_numYCQUNTSize, base_offset + JPGSW_numHeaderSize  , remaining);

					rtpSession->rtp_pk.offset = JPGSW_numHeaderSize + remaining;
					rtpSession->rtp_pk.data_offset += remaining + JPGSW_numYCQUNTSize;
					rtpSession->buf_len[rtpSession->buf_idx] += remaining;
#else
					// Quantization table Copy
					jpgData = send_buffer + rtpSession->buf_len[rtpSession->buf_idx];
					DmaMemCpy_ip(jpgData					, base_offset + JPGSW_numOffset_YQUNT, JPGSW_numQUNTSize); // Y quantization table
					rtpSession->buf_len[rtpSession->buf_idx] += JPGSW_numYCQUNTSize;

					remaining -= rtpSession->buf_len[rtpSession->buf_idx];
					DmaMemCpy_ip(jpgData + JPGSW_numYCQUNTSize, base_offset + JPGSW_numHeaderSize  , remaining);

					rtpSession->rtp_pk.offset = JPGSW_numHeaderSize + remaining;
					rtpSession->rtp_pk.data_offset += remaining + JPGSW_numYCQUNTSize;
					rtpSession->buf_len[rtpSession->buf_idx] += remaining;
#endif
				} else {
					remaining -= rtpSession->buf_len[rtpSession->buf_idx];
					if (jpegleft < remaining) {
						remaining = jpegleft;
					}

					BDmaMemCpy_rtos_flush(RTSPD_USE_DMA, send_buffer + rtpSession->buf_len[rtpSession->buf_idx], base_offset, remaining);

					rtpSession->rtp_pk.offset += remaining;
					rtpSession->rtp_pk.data_offset += remaining;
					rtpSession->buf_len[rtpSession->buf_idx] += remaining;
				}

				if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_TCP || prcInfo->eTransport == ENX_RTSP_TRANSPORT_HTTP) {
					rtspHead->un16Length = rtpSession->buf_len[rtpSession->buf_idx] - sizeof(rthInterleaved);
				}

				if (rtpSession->rtp_pk.offset == rtpSession->rtp_pk.size) {
					rtpHead->un1Markerbit = 1;
					rtpSession->tx_ready = ENX_RTP_TXSTE_END;
					// This case goes directly to "ENX_RTP_TXSTE_END".
				} else {
					break;
					// switch break;
				}
			}
		case ENX_RTP_TXSTE_END:
			rtpSession->tx_ready = ENX_RTP_TXSTE_READY;
			break;
	}

	LEAVE();
	return (rtpSession->rtp_pk.size - rtpSession->rtp_pk.offset);
}

#endif
#endif
