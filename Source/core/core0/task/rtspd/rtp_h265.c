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
#include "rtp_h265.h"

#if 0
#define ENTER() printf(" IN %04d:%s\n", __LINE__, __func__)
#define LEAVE() printf("OUT %04d:%s\n", __LINE__, __func__)
#else
#define ENTER()
#define LEAVE()
#endif

static void rtspd_client_rtp_h265_check_framenum(UINT *detectErr, BYTE *addr)
{
#if 0
	BYTE frameNum = 0;
	BYTE frameNal = *(addr + 4);
	if (frameNal == 0x61 || frameNal == 0x41) {			// P frame
		frameNum = (*(((int *)addr) + 1) >> 13) & 0xFF;
		// H.265 Frame Number Info
		// *(((int *)addr)+1) (0000 0000 0001 1111 1110 0000 0000 0000)
		if (frameNum - *detectErr == 1) {
			//ex)E0 A0 = (8*0) + ((1010 0000)>>5) = Frame 5,
			//   E1 20 = (8*1) + ((0010 0000)>>5) = Frame 9
			*detectErr = frameNum;
		} else {
			printf("addr(0x%08X), newNum(%02d), oldNum(%02d)\n    Frame Number Detect Error!!\n", (UINT)(intptr_t)addr, frameNum, *detectErr);
			*detectErr = 0xFFFFFFFF;
		}
	} else if (frameNal == 0x67) {	// SPS+PPS+I frame
		*detectErr = 0;
	}
#endif
}

int rtspd_client_rtp_h265_main(rtsp_client *prcInfo)
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
	rtsp_connect_info *rtspConnect = &(prcInfo->conn_info);

	UINT remaining;
	BYTE *send_buffer = NULL;
	rtp_queue_data pdata;
	BYTE *base_offset = NULL;

	rthInterleaved *rtspHead = NULL;
	rthRTPHeader *rtpHead = NULL;
	hevcFUHeader *fuHead = NULL;
	BYTE *h265Data = NULL;
	UINT h265left, i, j, nalzerocnt = 0;
	UINT fuSize = 0, spsppsSize = 0;

	if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_UDP) {
		remaining = TCP_MSS + 12; // 12 = (TCP header size - UDP header size)
	} else {
		remaining = TCP_MSS;
	}

	switch (rtpSession->tx_ready) {
		case ENX_RTP_TXSTE_READY:
			if (rtp_queue_get(prcInfo->rtp_queue_video, &pdata) == ENX_OK) {
				rtpSession->rtp_pk.base = pdata.addr;
				rtpSession->rtp_pk.size = pdata.size;
				rtpSession->rtp_pk.offset = 0;
#if 0
				//_Yprintf("Warning: TEST CODE: RTP start_rtptime\n");
				rtpSession->start_rtptime = (*mtime * 9) / 50;
#else
				rtpSession->start_rtptime += pdata.ts;
#endif

				if (rtpSession->rtp_pk.nalfirst == 0) {
					if (pdata.flags == 1) {
						rtpSession->rtp_pk.nalfirst = 1;
					} else {
						break;
					}
				}

				hwflush_dcache_range((u32)rtpSession->rtp_pk.base, 64);
#if 0
				rtspd_client_rtp_h265_check_framenum(&rtpSession->rtp_pk.nalnum, (BYTE *)(intptr_t)rtpSession->rtp_pk.base);
				if (rtpSession->rtp_pk.nalnum == 0xFFFFFFFF) { // nalnum(sequence number)에 오차가 생긴다면 다음 I까지 pass
					printf("%s(%d) : slow network speed? small memory allocation?\n", __func__, __LINE__);
					printf("%s(%d) : or changed IDR(gop)?\n", __func__, __LINE__);
					break;
				}
#endif
				rtpSession->tx_ready = ENX_RTP_TXSTE_DOING;
				// This case goes directly to "ENX_RTP_TXSTE_DOING".
			} else {
				return 0;
			} // @suppress("No break at end of case")
		case ENX_RTP_TXSTE_DOING:
			{
				// 남은 h265의 크기
				h265left = rtpSession->rtp_pk.size - rtpSession->rtp_pk.offset;
				if (((int)h265left) < 0) {
					flprintf("h265left error(%d)\n", h265left);
					flprintf("rtp_pk.size(%d) rtp_pk.offset(%d)\n", rtpSession->rtp_pk.size, rtpSession->rtp_pk.offset);
				}

				// base기준 전송을 보내야 할 h265 주소
				base_offset = (BYTE *)(intptr_t)(rtpSession->rtp_pk.base + rtpSession->rtp_pk.offset);

				// NAL unit test
				if (rtpSession->rtp_pk.offset != 0) {
					// invalidate_dcache_range는 offset == 0인 단계에서 하므로 할 필요 없음(ENX_RTP_TXSTE_READY단계에서 진행)
					hwflush_dcache_range(rtpSession->rtp_pk.base, 64);
				}
				if (base_offset[0] == 0x00) {
					UINT loop = LWIP_MIN(h265left, 64);
					for (i = 0; i < loop; i++) {
						if (base_offset[i] == 0x00 && base_offset[i + 1] == 0x00 && base_offset[i + 2] == 0x01) {
							nalzerocnt = i + 3;
							break;
						}
					}
					if ((loop == h265left) && (nalzerocnt != 0) && (rtpSession->rtp_pk.size != h265left)) {
						printf("left(%d/%d) nalzerocnt(%d)\n", h265left, rtpSession->rtp_pk.size, nalzerocnt);
					}
				}

				if (nalzerocnt) {
					// NAL unit(0x00 0x00 0x00 0x01 or 0x00 0x00 0x01) does not transmit
					rtpSession->rtp_pk.offset += nalzerocnt;
					if (nalzerocnt > 4) {
						flprintf("NAL Jump(%d)\n", nalzerocnt);
						hexDump("Base", (void *)(intptr_t)rtpSession->rtp_pk.base, 64);
					}
					h265left -= nalzerocnt;
					base_offset = (BYTE *)(intptr_t)(rtpSession->rtp_pk.base + rtpSession->rtp_pk.offset);

					// Get : NAL unit type
					rtpSession->rtp_pk.nalunithdr = base_offset[0] << 8 | base_offset[1];
					rtpSession->rtp_pk.naltype = (base_offset[0] >> 1) & 0x3F;
					//_Yprintf("NAL Hdr(0x%04X) Type(0x%02X) - ", rtpSession->rtp_pk.nalunithdr, rtpSession->rtp_pk.naltype);
					switch(rtpSession->rtp_pk.naltype) {
						case 0x01:	// P
						case 0x13:	// I
							if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_TCP || prcInfo->eTransport == ENX_RTSP_TRANSPORT_HTTP) {
								if (h265left > (remaining - sizeof(rthInterleaved) - sizeof(rthRTPHeader))) {
									fuSize = sizeof(hevcFUHeader);
								}
							} else {
								if (h265left > (remaining - sizeof(rthRTPHeader))) {
									fuSize = sizeof(hevcFUHeader);
								}
							}
							break;
						case 0x20:	// VPS
						case 0x21:	// SPS
						case 0x22:	// PPS
							// VPS/SPS/PPS payload size check(Next nal)
							for (i = 0, j = 0; i < 64; i++) {
								if (base_offset[i] == 0x00) {
									j++;
									if (base_offset[i + 1] == 0x01 && j >= 2) {
										spsppsSize = i + 1 - j;
										break;
									}
								} else {
									j = 0;
								}
							}
							break;
						default:
							break;
					}
					//printf("FUsize(%d)\n", fuSize);
				} else {
					fuSize = sizeof(hevcFUHeader);
				}
				/////////////////////////////////////////////////////////////////////////////////

				rtpSession->buf_idx = (rtpSession->buf_idx + 1) % 2;
				rtpSession->buf_len[rtpSession->buf_idx] = 0;
				send_buffer = rtpSession->buffer[rtpSession->buf_idx];

				// RTSP Interleaved(4byte)
				if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_TCP || prcInfo->eTransport == ENX_RTSP_TRANSPORT_HTTP) {
					rtspHead = (rthInterleaved *)send_buffer;
					rtspHead->un8Magic = '$';
					rtspHead->un8Channel = rtspConnect->rtp_port[ENX_RTSP_STRTYPE_numVIDEO]; // TCP에서는 rtp_port는 Channel로 사용됨
					rtspHead->un16Length = 0; // packet가 모두 생성되고 나서 다시 설정 해주어야 한다.
					rtpSession->buf_len[rtpSession->buf_idx] = sizeof(rthInterleaved);
				}

				// RTP Header(12byte)
				if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_UDP) {
					rtpHead = (rthRTPHeader *)send_buffer;
				} else {
					rtpHead = (rthRTPHeader *)(send_buffer + rtpSession->buf_len[rtpSession->buf_idx]);
				}
				rtpHead->un2RTPVersion = RTP_numRTPVer;
				rtpHead->un1Padding = 0;
				rtpHead->un1Extension = 0;
				rtpHead->un4CSRCcount = 0;
				rtpHead->un1Markerbit = 0;
				rtpHead->un7Payloadtype = rtpSession->payload_type;
				rtpHead->un16Sequencenum = htons(++rtpSession->start_seq);
				rtpHead->un32Timestamp = htonl(rtpSession->start_rtptime);
				rtpHead->un32SSIdentifier = htonl(rtpSession->ssrc);
				//hexDump("rtpHead", rtpHead, sizeof(rthRTPHeader));
				//_Yprintf("SEQ(0x%04X) TS(0x%08X) SSID(0x%08X)\n", rtpSession->start_seq, rtpSession->start_rtptime, rtpSession->ssrc);
				if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_UDP) {
					rtpSession->buf_len[rtpSession->buf_idx] = sizeof(rthRTPHeader);
				} else {
					rtpSession->buf_len[rtpSession->buf_idx] += sizeof(rthRTPHeader);
				}

				if (spsppsSize) {
					// VPS or SPS or PPS
					h265Data = send_buffer + rtpSession->buf_len[rtpSession->buf_idx];
#if 1
					BDmaMemCpy_rtos_flush(RTSPD_USE_DMA, h265Data, base_offset, spsppsSize);
#else
					memcpy(h265Data, base_offset, spsppsSize);
#endif
					//hexDump("IDR", base_offset, spsppsSize);

					rtpSession->rtp_pk.offset += spsppsSize;
					rtpSession->buf_len[rtpSession->buf_idx] += spsppsSize;
					// end
				} else {
					if (fuSize) {
						// FU Header(2+1byte)
						fuHead = (hevcFUHeader *)(send_buffer + rtpSession->buf_len[rtpSession->buf_idx]);
						fuHead->un16PayloadHdr = htons(rtpSession->rtp_pk.nalunithdr & 0x81FF) | (49 << 1);
						if (nalzerocnt) {
							fuHead->un1Startbit = 1;
							rtpSession->rtp_pk.offset += 2;	// NAL type delete(0x61, 0x65)
							base_offset += 2;				// NAL type delete(0x61, 0x65)
							h265left -= 2;					// NAL type delete(0x61, 0x65)
						} else {
							fuHead->un1Startbit = 0;
						}
						fuHead->un1Endbit = 0;
						fuHead->un6Nalunittype = rtpSession->rtp_pk.naltype;
						rtpSession->buf_len[rtpSession->buf_idx] += sizeof(hevcFUHeader);
						//BYTE *pfuHdad = fuHead;
						//_Yprintf("FU %02X %02X %02X\n", pfuHdad[0], pfuHdad[1], pfuHdad[2]);
					}

					remaining -= rtpSession->buf_len[rtpSession->buf_idx];
					if (h265left < remaining) {
						remaining = h265left;
					}
#if 1
					BDmaMemCpy_rtos_flush(RTSPD_USE_DMA, send_buffer + rtpSession->buf_len[rtpSession->buf_idx], base_offset, remaining);
#else
					memcpy(send_buffer + rtpSession->buf_len[rtpSession->buf_idx], base_offset, remaining);
#endif

					rtpSession->rtp_pk.offset += remaining;
					rtpSession->rtp_pk.data_offset += remaining;
					rtpSession->buf_len[rtpSession->buf_idx] += remaining;

					//hexDump("send_buffer", send_buffer, 32);
				}

				if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_TCP || prcInfo->eTransport == ENX_RTSP_TRANSPORT_HTTP) {
					if (ntohs(rtspHead->un16Length) > 1456) {
						flprintf("TEST! Size: %d\n", ntohs(rtspHead->un16Length));
					}
					rtspHead->un16Length = htons(rtpSession->buf_len[rtpSession->buf_idx] - sizeof(rthInterleaved));
				}


				if (rtpSession->rtp_pk.offset == rtpSession->rtp_pk.size) {
					if (fuSize) {
						if (fuHead->un1Startbit == 1) {
							flprintf("TEST! H.265 Size: %d\n", rtpSession->rtp_pk.size);
						}
						fuHead->un1Endbit = 1;
					}
					rtpHead->un1Markerbit = 1;
					rtpSession->tx_ready = ENX_RTP_TXSTE_END;
					// This case goes directly to "ENX_RTP_TXSTE_END".
					if (h265left <= 100) {
//						flprintf("H.265 tail packet(%d byte)\n", h265left);
					}
				} else {
					if (h265left <= 100) {
//						flprintf("H.265 tail packet(%d byte)\n", h265left);
					}
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
