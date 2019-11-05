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

#include <sys/time.h> // gettimeofday

#include "enx_freertos.h"

#include "rtspd.h"
#include "rtcp_process.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__)
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__)
#else
#define ENTER()
#define LEAVE()
#endif

static int rtspd_client_rtcp_make_sr(rtp_session *prtp_ss, BYTE *buffer)
{
	rtcppk_header *head = (rtcppk_header *)buffer;
	rtcppk_sr *sr = (rtcppk_sr *)(buffer + sizeof(rtcppk_header));

	head->rtcp_ver = 2;
	head->rtcp_pad = 0;
	head->rtcp_count = 0;
	head->rtcp_type = RTCP_SR;
	head->rtcp_length = 6;	// 6*4 = sr(24)

// http://lists.live555.com/pipermail/live-devel/2010-May/012207.html
	struct timeval now;
	gettimeofday(&now, NULL);
	UINT ntp_msw = gptMsgShare.TIME + 0x83AA7E80; // NTP timestamp most-significant word (1970 epoch -> 1900 epoch)
	UINT ntp_lsw = (UINT)(now.tv_usec / 15625.0 * 0x04000000 + 0.5); // NTP timestamp least-significant word
//	flprintf("RTCP[%10u][%10u][%10u][%10u][%10u]\r\n", prtp_ss->start_rtptime, ntp_msw, ntp_lsw, prtp_ss->packet_cnt, prtp_ss->total_length);

	sr->sr_ssrc = prtp_ss->ssrc;
	sr->sr_intts = ntp_msw;
	sr->sr_fracts = ntp_lsw;
	sr->sr_rtpts = prtp_ss->start_rtptime;
	sr->sr_packets = prtp_ss->packet_cnt;
	sr->sr_octets = prtp_ss->total_length;

	return sizeof(rtcppk_header) + sizeof(rtcppk_sr);
}

int rtspd_client_rtcp_send_sr(rtsp_client *prcInfo, int media_type, BYTE *send_buffer)
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

	rthInterleaved *rtspHead = NULL;

	int buf_len = 0;

	// RTSP Interleaved(4byte)
	if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_TCP || prcInfo->eTransport == ENX_RTSP_TRANSPORT_HTTP) {
		rtspHead = (rthInterleaved *)send_buffer;
		rtspHead->un8Magic = '$';
		rtspHead->un8Channel = prcInfo->conn_info.rtcp_port[media_type];
		rtspHead->un16Length = 28;
		buf_len = sizeof(rthInterleaved);
	}

	// make SR(24byte)
	if (prcInfo->rtp_ss == NULL) {
		flprintf("rtp_ss NULL!!!!!\n");
		while (1) {
			vTaskDelay(1);
		}
	}
	rtp_session *prtp_ss = &(prcInfo->rtp_ss[media_type]);
	buf_len += rtspd_client_rtcp_make_sr(prtp_ss, send_buffer + buf_len);

	LEAVE();
	return buf_len;
}

int rtspd_client_rtcp_process(rtsp_client *prcInfo, int media_type, BYTE *buf, int buf_len)
{
	return 0;

	UNUSED(prcInfo);
	UNUSED(media_type);
	UNUSED(buf);
	UNUSED(buf_len);
}
#endif
