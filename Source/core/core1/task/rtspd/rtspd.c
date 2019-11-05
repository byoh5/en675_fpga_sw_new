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

#include "msg.h"
#include "user.h"

#include "lwip/tcpip.h"
#include "lwip/errno.h"
#include "lwip/sockets.h"

#if (RTSPD_AUTH_MODE==RTSPD_AUTH_BASIC)
#include "Base64.h"
#elif (RTSPD_AUTH_MODE==RTSPD_AUTH_DIGEST)
#include "md5.h"
#endif

#include "rtspd.h"
#include "rtspd_process.h"
#include "rtcp_process.h"
#include "rtp_h264.h"
#include "rtp_h265.h"
#if (JPEG_STREAM==1)
#include "rtp_jpeg.h"
#endif
#if (SW_JPEG_ENCODER==1)
#include "rtp_jpeg_sw.h"
#endif
#ifdef __AUDIO__
#include "rtp_g711.h"
#endif

#if 0
#define ENTER() printf(" IN %04d:%s\n", __LINE__, __func__)
#define LEAVE() printf("OUT %04d:%s\n", __LINE__, __func__)
#else
#define ENTER()
#define LEAVE()
#endif

rtsp_server rsInfo;

int rtspd_connect_info_count(void)
{
	return rsInfo.client_count;
}

rtsp_connect_info *rtspd_connect_info_get_index(UINT i)
{
	if (i >= RTSP_info_MAX) {
		flprintf("index overflow(max:%d, input:%d)\n", RTSP_info_MAX, i);
		return NULL;
	}
	return &rsInfo.rclist[i].conn_info;
}

int rtspd_connect_kick(UINT index)
{
	return rtspd_client_rtsp_state(&(rsInfo.rclist[index]), ENX_RTSP_STATE_TEARDOWN);
}

int rtspd_player_count(void)
{
	int count = 0;
	for (int i = 0; i < RTSP_info_MAX; i++) {
		if (rsInfo.rclist[i].play_query != 0) {
			count++;
		}
	}
	return count;
}

int rtspd_socket_init(WORD nPort)
{
	struct sockaddr_in serv_addr;
	int res = 0, serv_sockfd, serv_reuse = 1;

	lwip_socket_thread_init();
	serv_sockfd = lwip_socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sockfd == -1) {
		flprintf("lwip_socket fail\n");
		return -1;
	}

	do {
		res = lwip_setsockopt(serv_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&serv_reuse, sizeof(int));
		if (res == -1) {
			flprintf("lwip_setsockopt fail(%d)\n", errno);
			break;
		}

		memset(&serv_addr, 0, sizeof(struct sockaddr_in));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(nPort);

		res = lwip_bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
		if (res == -1) {
			flprintf("lwip_bind fail(%d)\n", errno);
			break;
		}

		res = lwip_listen(serv_sockfd, 5);
		if (res == -1) {
			flprintf("lwip_listen fail(%d)\n", errno);
			break;
		}

		res = lwip_fcntl(serv_sockfd, F_GETFL, 0);
		if (res == -1) {
			flprintf("lwip_fcntl(GET) fail(%d)\n", errno);
			break;
		}

		res = lwip_fcntl(serv_sockfd, F_SETFL, res | O_NONBLOCK);
		if (res == -1) {
			flprintf("lwip_fcntl(SET) fail(%d)\n", errno);
			break;
		}

	} while (0);

	if (res == -1) {
		lwip_close(serv_sockfd);
		serv_sockfd = -1;
	}

	return serv_sockfd;
}

int rtspd_socket_udp_init(UINT nPort)
{
	struct sockaddr_in serv_addr;
	int res = 0, serv_sockfd, serv_reuse = 1;

	lwip_socket_thread_init();
	serv_sockfd = lwip_socket(AF_INET, SOCK_DGRAM, 0);
	if (serv_sockfd == -1) {
		flprintf("lwip_socket fail\n");
		return -1;
	}

	do {
		res = lwip_setsockopt(serv_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&serv_reuse, sizeof(int));
		if (res == -1) {
			flprintf("lwip_setsockopt fail(%d)\n", errno);
			break;
		}

		memset(&serv_addr, 0, sizeof(struct sockaddr_in));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(nPort);

		res = lwip_bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
		if (res == -1) {
			flprintf("lwip_bind fail(%d)\n", errno);
			break;
		}

		res = lwip_fcntl(serv_sockfd, F_GETFL, 0);
		if (res == -1) {
			flprintf("lwip_fcntl(GET) fail(%d)\n", errno);
			break;
		}

		res = lwip_fcntl(serv_sockfd, F_SETFL, res | O_NONBLOCK);
		if (res == -1) {
			flprintf("lwip_fcntl(SET) fail(%d)\n", errno);
			break;
		}

	} while (0);

	if (res == -1) {
		lwip_close(serv_sockfd);
		serv_sockfd = -1;
	}

	return serv_sockfd;
}

void rtspd_client_close(rtsp_client *prcInfo)
{
	lwip_close(prcInfo->sockfd);
	flprintf("\n");
	es_printf("USER Disconn(%d=>%d) : Local(%IP:%u) Remote(%IP:%u)\n", rsInfo.client_count, rsInfo.client_count - 1,
				prcInfo->conn_info.server_addr, prcInfo->conn_info.server_port,
				prcInfo->conn_info.client_addr, prcInfo->conn_info.client_port);

	vTaskDelay(50); // RTCP input..?

	rsInfo.client_count--;
	flprintf("\n");
	if (prcInfo->rtp_ss) {
		vPortFree(prcInfo->rtp_ss);
		prcInfo->rtp_ss = NULL;
	}
	flprintf("\n");
	_Gprintf("IDX(%u) ID(0x%08X)\n", prcInfo->index, prcInfo->clientid);
	if (prcInfo->rtp_queue_video) {
		rtp_queue_free(&prcInfo->rtp_queue_video);
		prcInfo->rtp_queue_video = NULL;
	}
#ifdef __AUDIO__
	if (prcInfo->rtp_queue_audio) {
		rtp_queue_free(&prcInfo->rtp_queue_audio);
		prcInfo->rtp_queue_audio = NULL;
	}
#endif
	flprintf("\n");
	memset((char *)prcInfo, 0, sizeof(rtsp_client));
	flprintf("\n");
}

int rtspd_client_udp_read(int rtcpfd, BYTE *buffer, UINT buf_len, rtsp_client **prcRef, int *media_type)
{
	ENTER();

	fd_set fd;
	struct timeval tout;
	int res = 0;

	tout.tv_sec = 2;
	tout.tv_usec = 0;

	FD_ZERO(&fd);
	FD_SET(rtcpfd, &fd);

	res = lwip_select(rtcpfd + 1, &fd, NULL, NULL, &tout);
	if (res == -1) {
		if (errno == EINTR) {
			return 0;
		} else {
			flprintf("lwip_select fail(errno:%d)\n", errno);
			return -1;
		}
	} else if (res == 0) {
		return 0;
	}

	if (FD_ISSET(rtcpfd, &fd)) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_size = sizeof(struct sockaddr_in);
		res = lwip_recvfrom(rtcpfd, buffer, buf_len, 0, (struct sockaddr *)&client_addr, &client_addr_size);
		if (res <= 0) {
			res = -1;
		} else {
			int i, j;
			for (i = 0; i < RTSP_info_MAX && (*prcRef == NULL); i++) {
				if (rsInfo.rclist[i].conn_info.client_addr == client_addr.sin_addr.s_addr) {
					for (j = 0; j < ENX_RTSP_STRTYPE_numNONE; j++) {
						if (rsInfo.rclist[i].conn_info.rtcp_port[j] == ntohs(client_addr.sin_port)) {
							*prcRef = &rsInfo.rclist[i];
							*media_type = j;
							break;
						}
					}
				}
			}
#if 1 // Debug Code, $CMT-hjlee-180919 - 
			if (*prcRef == NULL) {
				char strLog[64];
				es_sprintf(strLog, "Err, QueryIP: %IP:%u\n", client_addr.sin_addr.s_addr, ntohs(client_addr.sin_port));
				_Rprintf(strLog);
			}
#endif
		}
	}

	LEAVE();
	return res;
}


int rtspd_client_read(rtsp_client *prcInfo, BYTE *buffer, UINT buf_len)
{
	fd_set fd;
	struct timeval tout;
	int res = 0;

	tout.tv_sec = prcInfo->client_read_timeout_sec;
	tout.tv_usec = 0;

	FD_ZERO(&fd);
	FD_SET(prcInfo->sockfd, &fd);

	res = lwip_select(prcInfo->sockfd + 1, &fd, NULL, NULL, &tout);
	if (res == -1) {
		if (errno == EINTR) {
			return 0;
		} else {
			flprintf("lwip_select fail(errno:%d)\n", errno);
			return -1;
		}
	} else if (res == 0) {
		return 0;
	}

	if (FD_ISSET(prcInfo->sockfd, &fd)) {
		res = lwip_read(prcInfo->sockfd, buffer, buf_len);
		if (res <= 0) {
			res = -1;
		}
	}

	return res;
}

int rtspd_client_write(rtsp_client *prcInfo, BYTE *buffer, int buf_len)
{
	fd_set fd;
	struct timeval tout;
	int res = 0;

	tout.tv_sec = prcInfo->client_write_timeout_sec;
	tout.tv_usec = 0;

	FD_ZERO(&fd);
	FD_SET(prcInfo->sockfd, &fd);

	res = lwip_select(prcInfo->sockfd + 1, NULL, &fd, NULL, &tout);
	if (res == -1) {
		if (errno == EINTR) {
			return 0;
		} else {
			flprintf("lwip_select fail(errno:%d)\n", errno);
			return -1;
		}
	} else if (res == 0) {
		return 0;
	}

	if (FD_ISSET(prcInfo->sockfd, &fd)) {
		printf("SEND(fd:%d) size:%u\n", prcInfo->sockfd, buf_len);
		res = lwip_send(prcInfo->sockfd, buffer, buf_len, 0);
	}

	return res;
}

static int rtspd_client_rtp_tcp_send(rtsp_client *prcInfo, int media_type)
{
	fd_set fd;
	struct timeval tout;
	int res = 0;

	tout.tv_sec = prcInfo->client_write_timeout_sec;
	tout.tv_usec = 0;

	FD_ZERO(&fd);
	FD_SET(prcInfo->sockfd, &fd);
	res = lwip_select(prcInfo->sockfd + 1, NULL, &fd, NULL, &tout);
	if (res == -1) {
		if (errno == EINTR) {
			return -3;//0;
		} else {
			flprintf("lwip_select fail(errno:%d)\n", errno);
			return -1;
		}
	} else if (res == 0) {
		return -2;//0;
	}

	if (FD_ISSET(prcInfo->sockfd, &fd)) {
		if (prcInfo->rtp_ss == NULL) {
			flprintf("rtp_ss NULL!!!!!\n");
			while (1) {
				vTaskDelay(1);
			}
		}

		rtp_session *prtp_ss = &(prcInfo->rtp_ss[media_type]);
		res = lwip_send(prcInfo->sockfd, prtp_ss->buffer[prtp_ss->buf_idx], prtp_ss->buf_len[prtp_ss->buf_idx], 1);
		if (res == -1) {
			flprintf("lwip_send fail(errno:%d)\n", errno); // ECONNRESET?
		} else if (res != (int)prtp_ss->buf_len[prtp_ss->buf_idx]) {
			flprintf("some data can not be transferred(send:%d/%dbyte).\n", res, prtp_ss->buf_len[prtp_ss->buf_idx]);
		}
	} else {
		flprintf("not lwip_send call\n");
	}

	return res;
}

static int rtspd_client_rtp_udp_send(int sockfd, rtsp_client *prcInfo, int media_type)
{
//	ENTER();

	fd_set fd;
	struct timeval tout;
	int res = 0;

	tout.tv_sec = prcInfo->client_write_timeout_sec;
	tout.tv_usec = 0;

	FD_ZERO(&fd);
	FD_SET(sockfd, &fd);
	res = lwip_select(sockfd + 1, NULL, &fd, NULL, &tout);
	if (res == -1) {
		if (errno == EINTR) {
			return 0;
		} else {
			flprintf("lwip_select fail(errno:%d)\n", errno);
			return -1;
		}
	} else if (res == 0) {
		return 0;
	}

	if (FD_ISSET(sockfd, &fd)) {
		if (prcInfo->rtp_ss == NULL) {
			flprintf("rtp_ss NULL!!!!!\n");
			while (1) {
				vTaskDelay(1);
			}
		}
		rtp_session *prtp_ss = &(prcInfo->rtp_ss[media_type]);
		struct sockaddr_in client_addr;
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(prcInfo->conn_info.rtp_port[media_type]);
		client_addr.sin_addr.s_addr = prcInfo->conn_info.client_addr;
		res = lwip_sendto(sockfd, prtp_ss->buffer[prtp_ss->buf_idx], prtp_ss->buf_len[prtp_ss->buf_idx], 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_in));
		if (res == -1) {
			prcInfo->errCount++;
			if (prcInfo->errCount > 30) {
				// 5sec내에 send 실패 30회의 경우 연결 종료.
				prcInfo->state = ENX_RTSP_STATE_ERROR;
			}
			if (errno != 12) {
				flprintf("lwip_send fail(errno:%d)(cnt:%d)\n", errno, prcInfo->errCount); // ECONNRESET?
			}
			if (prcInfo->erroldTime + 5 <= gptMsgShare.TIME) {
				prcInfo->erroldTime = gptMsgShare.TIME;
				prcInfo->errCount = 0;
			}
		} else if (res != (int)prtp_ss->buf_len[prtp_ss->buf_idx]) {
			flprintf("some data can not be transferred(send:%d/%dbyte).\n", res, prtp_ss->buf_len[prtp_ss->buf_idx]);
		} else {
			prcInfo->erroldTime = gptMsgShare.TIME;
			prcInfo->errCount = 0;
		}
	} else {
		flprintf("not lwip_send call\n");
	}

//	LEAVE();
	return res;
}

#if (RTSPD_RTCP_SR==1)
static int rtspd_client_rtcp_udp_send(int sockfd, rtsp_client *prcInfo, int media_type, BYTE *buffer, int buf_len)
{
//	ENTER();

	fd_set fd;
	struct timeval tout;
	int res = 0;

	tout.tv_sec = prcInfo->client_write_timeout_sec;
	tout.tv_usec = 0;

	FD_ZERO(&fd);
	FD_SET(sockfd, &fd);
	res = lwip_select(sockfd + 1, NULL, &fd, NULL, &tout);
	if (res == -1) {
		if (errno == EINTR) {
			return 0;
		} else {
			flprintf("lwip_select fail(errno:%d)\n", errno);
			return -1;
		}
	} else if (res == 0) {
		return 0;
	}

	if (FD_ISSET(sockfd, &fd)) {
		rtp_session *prtp_ss = &(prcInfo->rtp_ss[media_type]);
		struct sockaddr_in client_addr;
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(prtp_ss->rtcp_port);
		client_addr.sin_addr.s_addr = htonl(prcInfo->conn_info.client_addr);
		res = lwip_sendto(sockfd, buffer, buf_len, 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_in));
		if (res == -1) {
			flprintf("lwip_send fail(errno:%d)\n", errno); // ECONNRESET?
		} else if (res != (int)buf_len) {
			flprintf("some data can not be transferred(send:%d/%dbyte).\n", res, buf_len);
		}
	} else {
		flprintf("not lwip_send call\n");
	}

//	LEAVE();
	return res;
}
#endif

int rtspd_client_rtp_video_play(rtsp_client *prcInfo)
{
//	ENTER();

	int res = 0;
	if (prcInfo->rtp_ss == NULL) {
		flprintf("rtp_ss NULL!!!!!\n");
		while (1) {
			vTaskDelay(1);
		}
	}
	rtp_session *prtp_ss = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numVIDEO]);

	prtp_ss->buf_len[prtp_ss->buf_idx] = 0;
	if (prcInfo->play_query & ENX_RTSP_STRTYPE_VIDEO) {
		switch (prcInfo->isLive) {
			case ENX_RTSP_STREAM_FILE_H264_TYPE:
			case ENX_RTSP_STREAM_FILE_H265_TYPE:
				break;
			case ENX_RTSP_STREAM_LIVE_H264_1:
			case ENX_RTSP_STREAM_LIVE_H264_2:
				res = rtspd_client_rtp_h264_main(prcInfo);
				break;
			case ENX_RTSP_STREAM_LIVE_H265_1:
			case ENX_RTSP_STREAM_LIVE_H265_2:
				res = rtspd_client_rtp_h265_main(prcInfo);
				break;
			case ENX_RTSP_STREAM_LIVE_JPEG:
#if (JPEG_STREAM==1)
				res = rtspd_client_rtp_jpeg_main(prcInfo);
				break;
#endif
#if (SW_JPEG_ENCODER==1)
			case ENX_RTSP_STREAM_LIVE_JPEG_SW:
				res = rtspd_client_rtp_jpeg_sw_main(prcInfo);
//				res = rtspd_client_rtp_jpeg_sw_main(prcInfo);
				break;
#endif
			default:
				res = -1;
				flprintf("Err, isLive(%d)\n", prcInfo->isLive);
				break;
		}
	}

//	LEAVE();
	return res;
}

#ifdef __AUDIO__
int rtspd_client_rtp_audio_play(rtsp_client *prcInfo)
{
	ENTER();

	int res = 0;
	rtp_session *prtp_ss = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numAUDIO]);

	prtp_ss->buf_len[prtp_ss->buf_idx] = 0;
	if (prcInfo->play_query & ENX_RTSP_STRTYPE_AUDIO) {
		switch (prcInfo->isLive) {
			case ENX_RTSP_STREAM_FILE_H264_TYPE:
				break;
			case ENX_RTSP_STREAM_LIVE_H264_1:
			case ENX_RTSP_STREAM_LIVE_H264_2:
			case ENX_RTSP_STREAM_LIVE_H265_1:
			case ENX_RTSP_STREAM_LIVE_H265_2:
#if (JPEG_STREAM==1)
			case ENX_RTSP_STREAM_LIVE_JPEG:
#endif
#if (SW_JPEG_ENCODER==1)
			case ENX_RTSP_STREAM_LIVE_JPEG_SW:
#endif
				res = rtspd_client_rtp_g711_main(prcInfo);
				break;
			default:
				res = -1;
				flprintf("Err, isLive(%d)\n", prcInfo->isLive);
				break;
		}
	}

	LEAVE();
	return res;
}
#endif

void rtspd_client_rtp_main(void *ctx)
{
	ENTER();

	int res = 0, send_len, proc;
	rtsp_client *prcInfo = (rtsp_client *)ctx;
	rtp_session *prtp_ss;

	while (1) {
		proc = 0;
		switch (prcInfo->state) {
			case ENX_RTSP_STATE_NONE:
			case ENX_RTSP_STATE_OPTIONS:
			case ENX_RTSP_STATE_DESCRIBE:
			case ENX_RTSP_STATE_TEARDOWN:
			case ENX_RTSP_STATE_ERROR:
				res = -1;
				break;
			case ENX_RTSP_STATE_PLAY:
				if (prcInfo->rtp_ss == NULL) {
					flprintf("rtp_ss NULL!!!!!\n");
					while (1) {
						vTaskDelay(1);
					}
				}
				prtp_ss = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numVIDEO]);
				res = rtspd_client_rtp_video_play(prcInfo);
				if (prtp_ss->buf_len[prtp_ss->buf_idx] != 0) {
					send_len = rtspd_client_rtp_tcp_send(prcInfo, ENX_RTSP_STRTYPE_numVIDEO);
					if (send_len == -1) {
						flprintf("Error, rtp write\n");
						res = -1;
						break;
					} else if (send_len > 0) {
						prtp_ss->packet_cnt++;
						prtp_ss->total_length += send_len;
						proc++;
#if (RTSPD_RTCP_SR==1)
						UINT rtcp_current_time = gptMsgShare.TIME;
						if ((int)(rtcp_current_time - prtp_ss->sr_send_time) >= RTCP_SR_INTERVAL) { // RTCP_SR_INTERVAL초 마다 RTCP SR 전송
							prtp_ss->buf_idx = (prtp_ss->buf_idx + 1) % 2;
							BYTE *send_buffer = prtp_ss->buffer[prtp_ss->buf_idx];
							prtp_ss->buf_len[prtp_ss->buf_idx] = rtspd_client_rtcp_send_sr(prcInfo, ENX_RTSP_STRTYPE_numVIDEO, send_buffer);
							if (prtp_ss->buf_len[prtp_ss->buf_idx] > 0) {
								send_len = rtspd_client_rtp_tcp_send(prcInfo, ENX_RTSP_STRTYPE_numVIDEO);
								if (send_len == -1) {
									flprintf("Error, rtcp write\n");
									res = -1;
									break;
								} else if (send_len > 0) {
									prtp_ss->sr_send_time = rtcp_current_time;
								}
							}
						}
#endif
					} else {
						flprintf("Drop?(%d)\n", send_len);
					}
				}
#ifdef __AUDIO__
				prtp_ss = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numAUDIO]);
				res = rtspd_client_rtp_audio_play(prcInfo);
				if (prtp_ss->buf_len[prtp_ss->buf_idx] != 0) {
					send_len = rtspd_client_rtp_tcp_send(prcInfo, ENX_RTSP_STRTYPE_numAUDIO);
					if (send_len == -1) {
						flprintf("Error, rtp write\n");
						res = -1;
						break;
					} else if (send_len > 0) {
						prtp_ss->packet_cnt++;
						prtp_ss->total_length += send_len;
						proc++;
#if (RTSPD_RTCP_SR==1)
						UINT rtcp_current_time = gptMsgShare.TIME;
						if ((int)(rtcp_current_time - prtp_ss->sr_send_time) >= RTCP_SR_INTERVAL) { // RTCP_SR_INTERVAL초 마다 RTCP SR 전송
							prtp_ss->buf_idx = (prtp_ss->buf_idx + 1) % 2;
							BYTE *send_buffer = prtp_ss->buffer[prtp_ss->buf_idx];
							prtp_ss->buf_len[prtp_ss->buf_idx] = rtspd_client_rtcp_send_sr(prcInfo, ENX_RTSP_STRTYPE_numAUDIO, send_buffer);
							if (prtp_ss->buf_len[prtp_ss->buf_idx] > 0) {
								send_len = rtspd_client_rtp_tcp_send(prcInfo, ENX_RTSP_STRTYPE_numAUDIO);
								if (send_len == -1) {
									flprintf("Error, rtcp write\n");
									res = -1;
									break;
								} else if (send_len > 0) {
									prtp_ss->sr_send_time = rtcp_current_time;
								}
							}
						}
#endif

					}
				}
#endif
				break;
			case ENX_RTSP_STATE_SETUP:
			case ENX_RTSP_STATE_PAUSE:
			case ENX_RTSP_STATE_GET_PARAMETER:
			case ENX_RTSP_STATE_SET_PARAMETER:
				res = 0;
				break;
		}

		if (res == -1) {
			break;
		}
		if (proc == 0) {
			vTaskDelay(1);
		}
	}

	prcInfo->play_query = 0;

	LEAVE();
	vTaskDelete(NULL);
}

void rtp_queue_stream_video_put(UINT addr, UINT size, UINT flags, rtsp_stream_type video_type, UINT ts)
{
	int i;
	rtp_queue_data prefdata;
	prefdata.addr = addr;
	prefdata.size = size;
	prefdata.flags = flags;
	prefdata.ts = ts;
	for (i = 0; i < RTSP_info_MAX; i++) { // search rtsp user
		if (rsInfo.rclist[i].valid && rsInfo.rclist[i].state == ENX_RTSP_STATE_PLAY && (rsInfo.rclist[i].play_query & ENX_RTSP_STRTYPE_VIDEO)) {
			if(rsInfo.rclist[i].isLive == video_type) {
				rtp_queue_put(rsInfo.rclist[i].rtp_queue_video, &prefdata);
			}
		}
	}
}

#ifdef __AUDIO__
void rtp_queue_stream_audio_put(UINT addr, UINT size, UINT ts)
{
	int i;
	rtp_queue_data prefdata;
	prefdata.addr = addr;
	prefdata.size = size;
	prefdata.flags = 0;
	prefdata.ts = ts;
	for (i = 0; i < RTSP_info_MAX; i++) { // search rtsp user
		if (rsInfo.rclist[i].valid && rsInfo.rclist[i].state == ENX_RTSP_STATE_PLAY && (rsInfo.rclist[i].play_query & ENX_RTSP_STRTYPE_AUDIO)) {
			rtp_queue_put(rsInfo.rclist[i].rtp_queue_audio, &prefdata);
		}
	}
}
#endif

void rtspd_client_main(void *ctx)
{
	ENTER();

	BYTE buffer[4096];
	int read_size, write_size;
	UINT read_offset = 0;

	// struct RTSP Client Init
	rtsp_client *prcInfo = (rtsp_client *)ctx;
	flprintf("\n");
	prcInfo->client_read_timeout_sec = 1;
	prcInfo->client_write_timeout_sec = 2;
#if (RTSPD_AUTH_MODE==RTSPD_AUTH_NONE)
	prcInfo->login = ENX_OK;
#else
	prcInfo->login = ENX_FAIL;
#endif
	prcInfo->lastTime = gptMsgShare.UPTIME;
	prcInfo->errCount = 0;
	prcInfo->state = ENX_RTSP_STATE_NONE;
	prcInfo->isLive = ENX_RTSP_STREAM_NONE;
	prcInfo->eTransport = ENX_RTSP_TRANSPORT_NONE;
	prcInfo->play_query = ENX_RTSP_STRTYPE_NONE;
	if (prcInfo->rtp_ss) {
		_Yprintf("rtp_ss not null...\n");
	}
	prcInfo->rtp_ss = NULL;

	// RTP queue buffer init
	flprintf("\n");
	_Gprintf("IDX(%u) ID(0x%08X)\n", prcInfo->index, prcInfo->clientid);
	rtp_queue_init(&prcInfo->rtp_queue_video, 60);
	rtp_queue_freebuffer(prcInfo->rtp_queue_video);
#ifdef __AUDIO__
	rtp_queue_init(&prcInfo->rtp_queue_audio, 100);
	rtp_queue_freebuffer(prcInfo->rtp_queue_audio);
	flprintf("\n");
#endif

	lwip_socket_thread_init();

	printf("Task %s Init\n", pcTaskGetName(NULL));
	while (1) {
		if (prcInfo->state == ENX_RTSP_STATE_TEARDOWN) {
			break;
		} else if (prcInfo->state == ENX_RTSP_STATE_ERROR) {
			flprintf("state ERROR, client close\n");
			break;
		} else if (prcInfo->lastTime + RTSP_TIMEOUT < gptMsgShare.UPTIME) {
			printf("RTSP Timeout\n");
			break;
		}

		read_size = rtspd_client_read(prcInfo, buffer + read_offset, 1460);	// read
		if (read_size == -1) {
			flprintf("read_size -1, client close\n");
			break;
		} else if (read_size == 0) {
			read_offset = 0;
			continue;
		}

		prcInfo->lastTime = gptMsgShare.UPTIME;

		read_offset += read_size;
		if (read_offset > 4096) {			// $CMT-hjlee-180212 - Test code
			flprintf("buffer overflow\n");	// $CMT-hjlee-180212 - Test code
		}									// $CMT-hjlee-180212 - Test code

		if (buffer[0] == '$') { // RTSP interleaved
//			flprintf("RTSP interleaved(read:%d/%d)\n", read_size, read_offset);
//			hexDump("interleaved", buffer, read_offset);

			rthInterleaved *pInter = (rthInterleaved *)buffer;
			if (pInter->un16Length == (read_offset - sizeof(rthInterleaved))) { // 모든 RTSP interleaved packet가 들어옴.
//				flprintf("RTSP interleaved done(read:%d)\n", read_offset);
				read_offset = 0;
			} else {
				// continue...
			}
		} else { // RTSP method
//			flprintf("RTSP method(read:%d/%d)\n", read_size, read_offset);
//			hexDump("method", buffer, read_offset);

			if ((buffer[read_offset-4] == '\r') && (buffer[read_offset-3] == '\n') && (buffer[read_offset-2] == '\r') && (buffer[read_offset-1] == '\n')) {
				buffer[read_offset] = '\0';
				if (rtspd_client_rtsp_method_parse(prcInfo, buffer, read_offset) == -1) {
					flprintf("RTSP Method parse fail\n");
				} else {
					if ((prcInfo->login == ENX_OK) || (prcInfo->method == ENX_RTSP_METHOD_OPTIONS) || (prcInfo->method == ENX_RTSP_METHOD_TEARDOWN)) {
						rtspd_client_rtsp_response(prcInfo, (char *)buffer, &write_size);
					} else {
						rtspd_client_rtsp_process_error(prcInfo, (char *)buffer, &write_size, ENX_RTSP_RESPONSE_401);
						write_size -= 2;
#if (RTSPD_AUTH_MODE==RTSPD_AUTH_BASIC)	
						write_size += sprintf((char *)&buffer[write_size], ENX_RTSP_RESPONSE_BAUTH RTSP_strCRLF);
#elif (RTSPD_AUTH_MODE==RTSPD_AUTH_DIGEST)
						write_size += sprintf((char *)&buffer[write_size], ENX_RTSP_RESPONSE_DAUTH RTSP_strCRLF, "7ypf/xlj9XXwfDPEoM4URrv/xwf94BcCAzFZH4GiTo0v", "FQhe/qaU925kfnzjCev0ciny7QMkPqMAFRtzCUYo5tdS");
#endif
					}
				}

				if (write_size) {
//					struct tm tmout;
//					enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES, DEF_YES);
//					flprintf("Output size: %d\n", write_size);
//					hexDump("RTSP TX", buffer, write_size);
//					int a = rtspd_client_write(prcInfo, buffer, write_size);
					rtspd_client_write(prcInfo, buffer, write_size);
//					printf("%04d-%02d-%02d %s %02d:%02d:%02d - RTSP TX res: [%d]\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec, a);
				}

				read_offset = 0;
			} else {
				// continue...
			}
		}
	}

//	flprintf("Client close wait\n");
	prcInfo->state = ENX_RTSP_STATE_TEARDOWN; // client_rtp_main이 close 되길 기다린다.
	while (prcInfo->play_query != 0) {
		vTaskDelay(1);
	}
//	flprintf("Client close\n");
	rtspd_client_close(prcInfo);

	lwip_socket_thread_cleanup();

	LEAVE();
	vTaskDelete(NULL);
}

int rtspd_socket_accept(rtsp_server *prsInfo, struct sockaddr_in *clie_addr)
{
	fd_set fd;
	struct timeval tout;
	int res = 0, clie_sockfd = -1, clie_len = sizeof(struct sockaddr_in);

	tout.tv_sec = prsInfo->client_accept_timeout_sec;
	tout.tv_usec = 0;

	FD_ZERO(&fd);
	FD_SET(prsInfo->sockfd, &fd);

	res = lwip_select(prsInfo->sockfd + 1, &fd, NULL, NULL, &tout);
	if (res == -1) {
		if (errno == EINTR) {
			return 0;
		} else {
			flprintf("lwip_select fail\n");
			return -1;
		}
	} else if (res == 0) {
		return 0;
	}

	do {
		if (FD_ISSET(prsInfo->sockfd, &fd)) {
			clie_sockfd = lwip_accept(prsInfo->sockfd, (struct sockaddr *)clie_addr, (socklen_t *)&clie_len);
			if (clie_sockfd == -1) {
				flprintf("lwip_accept fail, errno(%d)\n", errno);
				break;
			} else if (prsInfo->client_count >= RTSP_info_MAX) {
				flprintf("lwip_accept fail, client_count max(%d)\n", prsInfo->client_count);
				lwip_close(clie_sockfd);
				clie_sockfd = -2;
				break;
			} else {
				int opt_val = 1;
				res = lwip_setsockopt(clie_sockfd, IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(int));
				if (res == -1) {
					flprintf("lwip_setsockopt fail\n");
					lwip_close(clie_sockfd);
					clie_sockfd = -1;
					break;
				}

				res = lwip_fcntl(clie_sockfd, F_GETFL, 0);
				if (res == -1) {
					flprintf("lwip_fcntl fail\n");
					lwip_close(clie_sockfd);
					clie_sockfd = -1;
					break;
				}

				res = lwip_fcntl(clie_sockfd, F_SETFL, res | O_NONBLOCK);
				if (res == -1) {
					flprintf("lwip_fcntl fail\n");
					lwip_close(clie_sockfd);
					clie_sockfd = -1;
					break;
				}
			}
		}
	} while (0);

	return clie_sockfd;
}

int rtspd_socket_client_new(rtsp_server *prsInfo, int clie_sockfd)
{
	int i;
	for (i = 0; i < RTSP_info_MAX; i++) {
		if (prsInfo->rclist[i].valid == 0) {
			memset((char *)&(prsInfo->rclist[i]), 0, sizeof(rtsp_client));

			prsInfo->rclist[i].valid = i + 1;
			prsInfo->rclist[i].sockfd = clie_sockfd;

			struct sockaddr_in nameaddr;
			socklen_t namelen = sizeof(struct sockaddr_in);
			if (lwip_getsockname(clie_sockfd, (struct sockaddr *)&nameaddr, &namelen) == 0) {
				prsInfo->rclist[i].conn_info.server_addr = (nameaddr.sin_addr.s_addr);
				prsInfo->rclist[i].conn_info.server_port = (nameaddr.sin_port);
			}
			namelen = sizeof(struct sockaddr_in);
			if (lwip_getpeername(clie_sockfd, (struct sockaddr *)&nameaddr, &namelen) == 0) {
				prsInfo->rclist[i].conn_info.client_addr = (nameaddr.sin_addr.s_addr);//ntohl
				prsInfo->rclist[i].conn_info.client_port = (nameaddr.sin_port);//ntohs
			}

			prsInfo->rclist[i].conn_info.video_type = 0;
			prsInfo->rclist[i].conn_info.rtp_type = 0;
			prsInfo->rclist[i].conn_info.connect_time = gptMsgShare.TIME;

			es_printf("USER Conn(%d=>%d) : Local(%IP:%u) Remote(%IP:%u)\n", prsInfo->client_count, prsInfo->client_count+1,
					prsInfo->rclist[i].conn_info.server_addr, ntohs(prsInfo->rclist[i].conn_info.server_port),
					prsInfo->rclist[i].conn_info.client_addr, ntohs(prsInfo->rclist[i].conn_info.client_port));

			prsInfo->client_count++;

			return i;
		}
	}
	return -1;
}

volatile int rtp_step;

void rtspd_client_udp_rtp_main(void *ctx)
{
	int i, res = 0, proc, send_len;
	flprintf("\n");
	int rtpfd = rtspd_socket_udp_init(554);
	if (rtpfd < 0) {
		_Rprintf("ERROR Socket fd(%d)\n", rtpfd);
		while(1);
	}
	flprintf("rtpfd: %u\n", rtpfd);
	rtp_session *prtp_ss;

	while (1) {
		proc = 0;
		//printf("1");
		for (i = 0; i < RTSP_info_MAX; i++) {
			rtsp_client *prcInfo = (rtsp_client *)&rsInfo.rclist[i];
			if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_UDP) {
				switch (prcInfo->state) {
					case ENX_RTSP_STATE_PLAY:
						if (prcInfo->rtp_ss == NULL) {
							flprintf("rtp_ss NULL!!!!!\n");
							while (1) {
								vTaskDelay(1);
							}
						}
						prtp_ss = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numVIDEO]);
						rtp_step = 1;
						res = rtspd_client_rtp_video_play(prcInfo);
						rtp_step = 2;
						if (prtp_ss->buf_len[prtp_ss->buf_idx] != 0) {
							rtp_step = 3;
							send_len = rtspd_client_rtp_udp_send(rtpfd, prcInfo, ENX_RTSP_STRTYPE_numVIDEO);
							rtp_step = 4;
							if (send_len == -1) {
								rtp_step = 5;
								flprintf("Error, rtp write\n");
								res = -1;
								rtp_step = 6;
								break;
							} else if (send_len > 0) {
								rtp_step = 7;
								prtp_ss->packet_cnt++;
								prtp_ss->total_length += send_len;
								proc++;
								rtp_step = 8;
							}
						}
						rtp_step = 9;
#ifdef __AUDIO__
						prtp_ss = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numAUDIO]);
						res = rtspd_client_rtp_audio_play(prcInfo);
						if (prtp_ss->buf_len[prtp_ss->buf_idx] != 0) {
							send_len = rtspd_client_rtp_udp_send(rtpfd, prcInfo, ENX_RTSP_STRTYPE_numAUDIO);
							if (send_len == -1) {
								flprintf("Error, rtp write\n");
								res = -1;
								break;
							} else if (send_len > 0) {
								prtp_ss->packet_cnt++;
								prtp_ss->total_length += send_len;
								proc++;
							}
						}
#endif
						break;
					case ENX_RTSP_STATE_TEARDOWN:
						prcInfo->play_query = 0;
						break;
					default:
						break;
				}
			}
		}
		if (proc == 0) {
			vTaskDelay(1);
		}
		//printf("2");
	}

	while (1) {
		flprintf("why?");
		vTaskDelay(100);
	}
	lwip_socket_thread_cleanup();
	vTaskDelete(NULL);
	UNUSED(ctx);
}

void rtspd_client_udp_rtcp_main(void *ctx)
{
	BYTE buffer[4096];
	int i, read_size, read_offset;
	flprintf("\n");
	int rtcpfd = rtspd_socket_udp_init(555);
	flprintf("\n");
#if (RTSPD_RTCP_SR==1)
	int res = 0, send_len;
	rtp_session *prtp_ss;
	UINT rtcp_current_time;
#endif

	// 시간되면 SR을 보내고, RR을 받기도 해야 한다.
	// BYE 받으면 해당 세션에 대해서 TEARDOWN을 하도록 한다.
	flprintf("\n");
	while (1) {
		int media_type = -1;
		rtsp_client *prcRef = NULL;
		read_size = rtspd_client_udp_read(rtcpfd, buffer, 4096, &prcRef, &media_type); // read
		if (read_size == -1) {
			flprintf("read_size -1, client close\n");
			break;
		} else if (read_size == 0) {
			read_offset = 0;
			continue;
		}
//		flprintf("\n");
		if (prcRef != NULL) {
			// RTSP Timeout reset
			prcRef->lastTime = gptMsgShare.UPTIME;
			rtspd_client_rtcp_process(prcRef, media_type, buffer, read_size);
//			hexDump("UDP RX", buffer, read_size);
		} else {
			int logflag = 0;
			if (buffer[0] & 0x80) { // RTP 형태의 packet (0x80 ~ 0x8F)
				//int lens = buffer[3];
				switch (buffer[1]) {
					case RTCP_SR:
						printf("RTCP SR RX (No destination.)\n");
						break;
					case RTCP_RR:
						printf("RTCP RR RX (No destination.)\n");
						break;
					case RTCP_SDES:
						printf("RTCP SDES RX (No destination.)\n");
						break;
					case RTCP_BYE:
						printf("RTCP BYE RX (No destination.)\n");
						break;
					case RTCP_APP:
						printf("RTCP APP RX (No destination.)\n");
						break;
					default:
						logflag = 1;
						break;
				}
			} else {
				logflag = 1;
			}

			if (logflag) {
				hexDump("RTSP UDP RX (No destination.)", buffer, read_size);
			}
		}
//		flprintf("\n");
		// RTCP SR Send
		for (i = 0; i < RTSP_info_MAX; i++) {
			rtsp_client *prcInfo = (rtsp_client *)&rsInfo.rclist[i];
			if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_UDP) {
				switch (prcInfo->state) {
					case ENX_RTSP_STATE_PLAY:
#if (RTSPD_RTCP_SR==1)
						prtp_ss = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numVIDEO]);
						rtcp_current_time = gptMsgShare.TIME;
						if ((int)(rtcp_current_time - prtp_ss->sr_send_time) >= RTCP_SR_INTERVAL) { // RTCP_SR_INTERVAL초 마다 RTCP SR 전송
							res = rtspd_client_rtcp_send_sr(prcInfo, ENX_RTSP_STRTYPE_numVIDEO, buffer);
							if (res > 0) {
								send_len = rtspd_client_rtcp_udp_send(rtcpfd, prcInfo, ENX_RTSP_STRTYPE_numVIDEO, buffer, res);
								if (send_len == -1) {
									flprintf("Error, rtp write\n");
									res = -1;
									break;
								} else if (send_len > 0) {
									prtp_ss->sr_send_time = rtcp_current_time;
								}
							}
						}
#ifdef __AUDIO__
						rtp_session *prtp_ss = &(prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numAUDIO]);
						rtcp_current_time = gptMsgShare.TIME;
						if ((int)(rtcp_current_time - prtp_ss->sr_send_time) >= RTCP_SR_INTERVAL) { // RTCP_SR_INTERVAL초 마다 RTCP SR 전송
							res = rtspd_client_rtcp_send_sr(prcInfo, ENX_RTSP_STRTYPE_numAUDIO, buffer);
							if (res > 0) {
								send_len = rtspd_client_rtcp_udp_send(rtcpfd, prcInfo, ENX_RTSP_STRTYPE_numAUDIO, buffer, res);
								if (send_len == -1) {
									flprintf("Error, rtp write\n");
									res = -1;
									break;
								} else if (send_len > 0) {
									prtp_ss->sr_send_time = rtcp_current_time;
								}
							}
						}
#endif
#endif
						break;
					default:
						break;
				}
			}
		}
	}

	lwip_socket_thread_cleanup();
	vTaskDelete(NULL);
	UNUSED(ctx);
}

void rtspd_socket_server(void *ctx)
{
	memset(&rsInfo, 0, sizeof(rsInfo));
	rsInfo.client_accept_timeout_sec = 3;
	rsInfo.client_count = 0;
	rsInfo.client_last_id = rand() * rand();
	rsInfo.sockfd = rtspd_socket_init(gtNetwork.portnumRTSP);
	if (rsInfo.sockfd == -1) {
		flprintf("RTSP server start fail\n");
	} else {
		flprintf("RTSP rtsp_client sizeof: %lubyte\n", sizeof(rtsp_client));

		vTaskCreate("rtp(u)", rtspd_client_udp_rtp_main, NULL, LV5_STACK_SIZE, LV4_TASK_PRIO);
		vTaskCreate("rtcp(u)", rtspd_client_udp_rtcp_main, NULL, LV5_STACK_SIZE, LV5_TASK_PRIO);

		while (1) {
			struct sockaddr_in client_addr;
			int client_sockfd, res;
			client_sockfd = rtspd_socket_accept(&rsInfo, &client_addr);
			if (client_sockfd == -1) {
				flprintf("RTSP server accept error\n");
				vTaskDelay(100);
				continue;
			} else if (client_sockfd == -2) {
				vTaskDelay(100);
				continue;
			} else if (client_sockfd == 0) {
				// etc process?
				continue;
			}

			if ((res = rtspd_socket_client_new(&rsInfo, client_sockfd)) != -1) {
				char strbuf[12] = {0};
				sprintf(strbuf, "rtspc%d", res);
				rsInfo.rclist[res].index = res;
				rsInfo.rclist[res].clientid = rsInfo.client_last_id++;
				vTaskCreate(strbuf, rtspd_client_main, &rsInfo.rclist[res], LV5_STACK_SIZE, LV5_TASK_PRIO);
			} else {
				flprintf("unchecked, why?\n");
			}
		}
	}

	flprintf("RTSP server stop\n");
	lwip_socket_thread_cleanup();
	vTaskDelete(NULL);

	UNUSED(ctx);
}
#endif
