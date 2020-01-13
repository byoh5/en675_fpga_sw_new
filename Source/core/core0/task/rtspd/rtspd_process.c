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

#include "networkif.h"	// network_default_netif_get_ip

#include "rtspd.h"
#include "rtspd_process.h"
#include "rtspd_authorization.h"

#if 0
#define ENTER() printf(" IN %04d:%s\n", __LINE__, __func__)
#define LEAVE() printf("OUT %04d:%s\n", __LINE__, __func__)
#else
#define ENTER()
#define LEAVE()
#endif

int rtspd_client_rtsp_header_parse(rtsp_client *prcInfo, char *buf, UINT buf_len)
{
	ENTER();

	int res = 0;
	char *strData, *strTemp;

	// 공통사항
	if (strncmp(ENX_RTSP_HEADER_strCSEQ, buf, sizeof(ENX_RTSP_HEADER_strCSEQ) - 1) == 0) {
		strData = buf + sizeof(ENX_RTSP_HEADER_strCSEQ);
		prcInfo->nCSeq = atoi(strData);
//		flprintf("Common : [%s, %d]\n", ENX_RTSP_HEADER_strCSEQ, prcInfo->nCSeq);
	} else if (strncmp(ENX_RTSP_HEADER_strSESSION, buf, sizeof(ENX_RTSP_HEADER_strSESSION) - 1) == 0) {
		strData = buf + sizeof(ENX_RTSP_HEADER_strSESSION);
		prcInfo->nSession = atoi(strData);
//		flprintf("Common : [%s, %d]\n", ENX_RTSP_HEADER_strSESSION, prcInfo->nSession);
#if (RTSPD_AUTH_MODE!=RTSPD_AUTH_NONE)
	} else if (strncmp(ENX_RTSP_HEADER_strAUTHORIZATION, buf, sizeof(ENX_RTSP_HEADER_strAUTHORIZATION) - 1) == 0) {
		prcInfo->login = rtspd_client_rtsp_authorization(buf, prcInfo->method);
#endif
	} else {
// Debug Print Start
		if (strncmp("User-Agent", buf, sizeof("User-Agent") - 1) == 0) {
		} else if (strncmp("Range", buf, sizeof("Range") - 1) == 0) {
		} else if (strncmp(ENX_RTSP_HEADER_strACCEPT, buf, sizeof(ENX_RTSP_HEADER_strACCEPT) - 1) == 0) {
		} else if (strncmp(ENX_RTSP_HEADER_strTRANSPORT, buf, sizeof(ENX_RTSP_HEADER_strTRANSPORT) - 1) == 0) {
		} else if (buf_len) {
			flprintf("Header : [%s][size:%d]\n", buf, buf_len);
		}
// Debug Print End
	}

	switch (prcInfo->method) {
		case ENX_RTSP_METHOD_OPTIONS:
			break;
		case ENX_RTSP_METHOD_DESCRIBE:
			if (strncmp(ENX_RTSP_HEADER_strACCEPT, buf, sizeof(ENX_RTSP_HEADER_strACCEPT) - 1) == 0) {
				strData = buf + sizeof(ENX_RTSP_HEADER_strACCEPT);
				if (strstr(strData, "application/sdp")) {
					prcInfo->isAccept = ENX_YES;
				} else {
					prcInfo->isAccept = ENX_NO;
				}
			}
			break;
		case ENX_RTSP_METHOD_SETUP:
			{
				if (prcInfo->setup_query == ENX_RTSP_STRTYPE_NONE) {
					char *strType = strstr(prcInfo->strUrl, "//");
					if (strType != NULL) {
						strType += 2;
						strType = strchr(strType, '/');
						if (strType != NULL) {
							strType += 1;
							strType = strchr(strType, '/');
							if (strType != NULL) {
								strType += 1;
							}
						}
					}

					if (strType != NULL) {
						if (strncmp(strType, ENX_RTSP_SDP_URL_VIDEO, sizeof(ENX_RTSP_SDP_URL_VIDEO) - 1) == 0) {
							prcInfo->setup_query = ENX_RTSP_STRTYPE_VIDEO;
							prcInfo->nQuery = ENX_RTSP_STRTYPE_numVIDEO;
						} else if (strncmp(strType, ENX_RTSP_SDP_URL_AUDIO, sizeof(ENX_RTSP_SDP_URL_AUDIO) - 1) == 0) {
							prcInfo->setup_query = ENX_RTSP_STRTYPE_AUDIO;
							prcInfo->nQuery = ENX_RTSP_STRTYPE_numAUDIO;
//						} else if (strncmp(strType, ENX_RTSP_SDP_URL_METADATA, sizeof(ENX_RTSP_SDP_URL_METADATA) - 1) == 0) {
//							prcInfo->setup_query = ENX_RTSP_STRTYPE_METADATA;
//							prcInfo->nQuery = ENX_RTSP_STRTYPE_numMETADATA;
//						} else if (strncmp(strType, ENX_RTSP_SDP_URL_BACKCHANNEL, sizeof(ENX_RTSP_SDP_URL_BACKCHANNEL) - 1) == 0) {
//							prcInfo->setup_query = ENX_RTSP_STRTYPE_BACKCHANNEL;
//							prcInfo->nQuery = ENX_RTSP_STRTYPE_numBACKCHANNEL;
						}
					} else {
						flprintf("Unknown Setup url(%s)\n", prcInfo->strUrl);
						break;
					}
				}

				if (strncmp(ENX_RTSP_HEADER_strTRANSPORT, buf, sizeof(ENX_RTSP_HEADER_strTRANSPORT) - 1) == 0) {
					prcInfo->isTransport = ENX_YES;
					int rtp_port = -1, rtcp_port = -1;
					strData = buf + sizeof(ENX_RTSP_HEADER_strTRANSPORT);
					hexDump("SETUP", strData, strlen(strData));
					if (strstr(strData, "RTP/AVP/TCP")) { // TCP
						prcInfo->eTransport = ENX_RTSP_TRANSPORT_TCP;
						if (prcInfo->setup_query == ENX_RTSP_STRTYPE_VIDEO) {
							printf("Transport TCP\n");
						}
						if ((strTemp = strstr(strData, "interleaved="))) {
							sscanf(strTemp, "interleaved=%d-%d", &rtp_port, &rtcp_port);
						}
					} else if (strstr(strData, "RTP/AVP")) { // UDP
						prcInfo->eTransport = ENX_RTSP_TRANSPORT_UDP;
						if (prcInfo->setup_query == ENX_RTSP_STRTYPE_VIDEO) {
							printf("Transport UDP\n");
						}
						if ((strTemp = strstr(strData, "client_port="))) {
							sscanf(strTemp, "client_port=%d-%d", &rtp_port, &rtcp_port);
						}
					} else {
						flprintf("Unknown Transport type(%s)\n", strData);
						break;
					}

					switch (prcInfo->eTransport) {
						case ENX_RTSP_TRANSPORT_UDP:
						case ENX_RTSP_TRANSPORT_TCP:
						case ENX_RTSP_TRANSPORT_HTTP:
							prcInfo->conn_info.rtp_port[prcInfo->nQuery] = rtp_port;
							prcInfo->conn_info.rtcp_port[prcInfo->nQuery] = rtcp_port;
							printf("Port: %d-%d\n", rtp_port, rtcp_port);
							break;
						case ENX_RTSP_TRANSPORT_NONE:
							break;
					}

//					if (strstr(strData, "unicast")) {
//						
//					} else if (strstr(strData, "multicast")) {
//						
//					}
				}
			}
			break;
		case ENX_RTSP_METHOD_PLAY:
			break;
		case ENX_RTSP_METHOD_PAUSE:
			break;
		case ENX_RTSP_METHOD_TEARDOWN:
			break;
		case ENX_RTSP_METHOD_GET_PARAMETER:
			break;
		case ENX_RTSP_METHOD_SET_PARAMETER:
			break;
		default:
			flprintf("Unknown Method(id) : %d\n", prcInfo->method);
			res = -1;
			break;
	}

	LEAVE();
	return res;
}

int rtspd_client_rtsp_state(rtsp_client *prcInfo, rtsp_client_state state)
{
	int res = -1;
	switch (state) {
		case ENX_RTSP_STATE_NONE:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_ERROR:
					res = 0;
					break;
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
					break;
			}
			break;

		case ENX_RTSP_STATE_OPTIONS:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
					res = 0;
					break;
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_ERROR:
					break;
			}
			break;

		case ENX_RTSP_STATE_DESCRIBE:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
					res = 0;
					break;
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_ERROR:
					break;
			}
			break;

		case ENX_RTSP_STATE_SETUP:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
					res = 0;
					break;
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_ERROR:
					break;
			}
			break;

		case ENX_RTSP_STATE_PLAY:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
					res = 0;
					break;
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_ERROR:
					break;
			}
			break;

		case ENX_RTSP_STATE_PAUSE:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
					res = 0;
					break;
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_ERROR:
					break;
			}
			break;			

		case ENX_RTSP_STATE_TEARDOWN:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
					res = 0;
					break;
				case ENX_RTSP_STATE_ERROR:
					break;
			}
			break;

		case ENX_RTSP_STATE_GET_PARAMETER:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
					res = 0;
					break;
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_ERROR:
					break;
			}
			break;

		case ENX_RTSP_STATE_SET_PARAMETER:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
					res = 0;
					break;
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_ERROR:
					break;
			}
			break;

		case ENX_RTSP_STATE_ERROR:
			switch (prcInfo->state) {
				case ENX_RTSP_STATE_NONE:
				case ENX_RTSP_STATE_OPTIONS:
				case ENX_RTSP_STATE_DESCRIBE:
				case ENX_RTSP_STATE_SETUP:
				case ENX_RTSP_STATE_PLAY:
				case ENX_RTSP_STATE_PAUSE:
				case ENX_RTSP_STATE_TEARDOWN:
				case ENX_RTSP_STATE_GET_PARAMETER:
				case ENX_RTSP_STATE_SET_PARAMETER:
				case ENX_RTSP_STATE_ERROR:
					res = 0;
					break;
			}
			break;
	}

	if (res == 0) {
		switch (state) { // 현재 state를 변경하지 않을 state들
			case ENX_RTSP_STATE_GET_PARAMETER:
			case ENX_RTSP_STATE_SET_PARAMETER:
				break;
			default: // state를 변경할 state들
				prcInfo->state = state;
				break;
		}
	}

	return res;
}

int rtspd_client_rtsp_method_parse(rtsp_client *prcInfo, BYTE *buf, UINT buf_len)
{
	ENTER();

	char *strEnd, *strMethod, *strUrl, *strVersion, *strHeader;

	// Method parse
	strMethod = (char *)buf;
	strEnd = strchr((char *)buf, RTSP_chrSPACE);
	if (strEnd == NULL) {
		LEAVE();
		return -1;
	}
	*strEnd = RTSP_chrNULL;
	if (strcmp(ENX_RTSP_METHOD_strOPTIONS, strMethod) == 0) {
		prcInfo->method = ENX_RTSP_METHOD_OPTIONS;
	} else if (strcmp(ENX_RTSP_METHOD_strTEARDOWN, strMethod) == 0) {
		prcInfo->method = ENX_RTSP_METHOD_TEARDOWN;
	} else if (strcmp(ENX_RTSP_METHOD_strDESCRIBE, strMethod) == 0) {
		prcInfo->method = ENX_RTSP_METHOD_DESCRIBE;
	} else if (strcmp(ENX_RTSP_METHOD_strSETUP, strMethod) == 0) {
		prcInfo->method = ENX_RTSP_METHOD_SETUP;
	} else if (strcmp(ENX_RTSP_METHOD_strPLAY, strMethod) == 0) {
		prcInfo->method = ENX_RTSP_METHOD_PLAY;
	} else if (strcmp(ENX_RTSP_METHOD_strPAUSE, strMethod) == 0) {
		prcInfo->method = ENX_RTSP_METHOD_PAUSE;
	} else if (strcmp(ENX_RTSP_METHOD_strGET_PARAMETER, strMethod) == 0) {
		prcInfo->method = ENX_RTSP_METHOD_GET_PARAMETER;
	} else if (strcmp(ENX_RTSP_METHOD_steSET_PARAMETER, strMethod) == 0) {
		prcInfo->method = ENX_RTSP_METHOD_SET_PARAMETER;
	} else {
		flprintf("Unknown Method : %s\n", strMethod);
		LEAVE();
		return -1;
	}

	// URL parse
	strUrl = strEnd + 1;
	strEnd = strchr(strUrl, RTSP_chrSPACE);
	if (strEnd == NULL) {
		LEAVE();
		return -1;
	}
	*strEnd = RTSP_chrNULL;
	strlcpy(prcInfo->strUrl, strUrl, sizeof(prcInfo->strUrl));

	// Version parse
	strVersion = strEnd + 1;
	strEnd = strstr(strVersion, RTSP_strCRLF);
	if (strEnd == NULL) {
		LEAVE();
		return -1;
	}
	*strEnd = RTSP_chrNULL;
	strlcpy(prcInfo->strVersion, strVersion, sizeof(prcInfo->strVersion));	

//	flprintf(" Method : [%d/%s]\n", prcInfo->method, strMethod);
//	flprintf("    Url : [%s]\n", prcInfo->strUrl);
//	flprintf("Version : [%s]\n", prcInfo->strVersion);

	// 1회성 변수 초기화
	prcInfo->isAccept = ENX_NO;
	prcInfo->isTransport = ENX_NO;
	prcInfo->setup_query = ENX_RTSP_STRTYPE_NONE;
	prcInfo->nQuery = ENX_RTSP_STRTYPE_numNONE;
	flprintf("1(V:0x%08X)\n", prcInfo->rtp_queue_video);
	while (1) {
		strHeader = strEnd + 2; // CRLF 넘기기
		strEnd = strstr(strHeader, RTSP_strCRLF);
		if (strEnd == NULL) {
			break;
		}
		*strEnd = RTSP_chrNULL;
		if (rtspd_client_rtsp_header_parse(prcInfo, strHeader, strEnd - strHeader) == -1) {
			break;
		}
	}
	flprintf("2(V:0x%08X)\n", prcInfo->rtp_queue_video);
	LEAVE();
	return 0;

	UNUSED(prcInfo);
	UNUSED(buf_len);
}

int rtspd_client_rtsp_interleaved_parse(rtsp_client *prcInfo, BYTE *buf, UINT buf_len)
{
	ENTER();
	LEAVE();
	return 0;
	UNUSED(prcInfo);
	UNUSED(buf);
	UNUSED(buf_len);
}

int rtspd_client_rtsp_check_url(rtsp_client *prcInfo, char *url)
{
	ENTER();

	int res = -1;
	prcInfo->nVEncoderIdx = -1;
	if (url) {
		if (prcInfo->isLive == ENX_RTSP_STREAM_NONE) {
			prcInfo->isLive = ENX_RTSP_STREAM_ERROR;	// fail 될 경우 default 값

			char *strpIP, *strEnd;
			strpIP = strstr(url, "//");
			if (strpIP != NULL) {
				strpIP += 2;
				strEnd = strchr(strpIP, '/');
				if (strEnd != NULL) {
					if (strncmp(strEnd, "/sdcard/", sizeof("/sdcard/") - 1) == 0) {
						strEnd += 1;
						strEnd = strchr(strEnd, '/');
						flprintf("SDcard Path: [%s]\n", strEnd);

						// 해당 파일이 실제로 존재하는지 확인!
						res = 0;
						prcInfo->isLive = ENX_RTSP_STREAM_FILE_H264_TYPE;
					} else {
						char strUrlBuf[STREAM_URL_LENGTH + 2]; // '/'와 '\0'를 위해 +2를 추가함
						for (UINT i = 0 ; i < VIDEO_CHANNEL_CNT; i++) {
							snprintf(strUrlBuf, STREAM_URL_LENGTH + 2, "/%s", (char *)gtUser.vcVideo[i].strStmUrl);
							if (strcmp(strEnd, strUrlBuf) == 0) {
								res = 0;
								prcInfo->nVEncoderIdx = i;
								switch (gtUser.vcVideo[i].eCodec) {
								case e_vcodecH265:
									prcInfo->isLive = ENX_RTSP_STREAM_LIVE_H265_1;
									break;
								case e_vcodecH264:
									prcInfo->isLive = ENX_RTSP_STREAM_LIVE_H264_1;
									break;
								case e_vcodecJPEG:
									prcInfo->isLive = ENX_RTSP_STREAM_LIVE_JPEG;
									break;
								}
								break;
							}
						}
					}

					if (res == -1) {
						flprintf("Error URL\n");
					}
				} else {
					flprintf("Error URL\n");
				}
			} else {
				flprintf("Error URL\n");
			}
		} else {
			// 승인된 서비스 리스트
			switch (prcInfo->isLive) {
				case ENX_RTSP_STREAM_FILE_H264_TYPE:
				case ENX_RTSP_STREAM_FILE_H265_TYPE:
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
					prcInfo->conn_info.video_type = prcInfo->isLive;
					res = 0;
					break;
				default:
					break;
			}
		}
	}

	if (res == -1) {
		flprintf("Error URL: %s\n", url);
//	} else {
//		flprintf("Accept URL: %s\n", url);
	}

	LEAVE();
	return res;
}

int rtspd_client_rtsp_make_datetime(char *buf)
{
	ENTER();

	struct tm tmout;
	enx_get_tmtime(gptMsgShare.TIME, &tmout, ENX_NO);
	int len = sprintf(buf, ENX_RTSP_RESPONSE_DATE, strWeek[tmout.tm_wday], strMonth[tmout.tm_mon], tmout.tm_mday, tmout.tm_year+1900, tmout.tm_hour, tmout.tm_min, tmout.tm_sec);

	LEAVE();
	return len;
}

int rtspd_client_rtsp_process_error(rtsp_client *prcInfo, char *buf, int *buf_len, char *strError)
{
	ENTER();

	int res = 0;

	if (strcmp(ENX_RTSP_RESPONSE_404, strError) == 0) {
		; // 404 error은 state를 ERROR로 변경하지 않는다.
	} else if (strcmp(ENX_RTSP_RESPONSE_461, strError) == 0) {
		; // 461 error은 state를 ERROR로 변경하지 않는다.
	} else if (strcmp(ENX_RTSP_RESPONSE_458, strError) == 0) {
		; // 458 error은 state를 ERROR로 변경하지 않는다. SET_PARAMETER은 항상 458이므로...
	} else if (strcmp(ENX_RTSP_RESPONSE_453, strError) == 0) {
		; // 453 error은 state를 ERROR로 변경하지 않는다.
	} else if (strcmp(ENX_RTSP_RESPONSE_401, strError) == 0) {
		; // 401 error은 state를 ERROR로 변경하지 않는다. RTSP Authorization.
	} else {
		flprintf("%s\n", strError);
		rtspd_client_rtsp_state(prcInfo, ENX_RTSP_STATE_ERROR);
	}

	char strDate[40] = {0};
	rtspd_client_rtsp_make_datetime(strDate);
	if (prcInfo->nSession == 0) {
		*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD "%s" RTSP_strCRLF, strError, prcInfo->nCSeq, strDate);
	} else {
		*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD_S "%s" RTSP_strCRLF, strError , prcInfo->nCSeq, prcInfo->nSession, strDate);
	}

	LEAVE();
	return res;
}

int rtspd_client_rtsp_process_options(rtsp_client *prcInfo, char *buf, int *buf_len)
{
	ENTER();

	int res = 0;

	if (rtspd_client_rtsp_state(prcInfo, ENX_RTSP_STATE_OPTIONS) == 0) {
		char strDate[40] = {0};
		rtspd_client_rtsp_make_datetime(strDate);
		if (prcInfo->nSession == 0) {
			*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD ENX_RTSP_RESPONSE_OPTIONS "%s" RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, strDate);
		} else {
			*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD_S ENX_RTSP_RESPONSE_OPTIONS "%s" RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, prcInfo->nSession, strDate);
		}
	} else {
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_400);
	}

	LEAVE();
	return res;
}

int rtspd_client_rtsp_process_describe(rtsp_client *prcInfo, char *buf, int *buf_len)
{
	ENTER();

	int res = 0;

	if (rtspd_client_rtsp_state(prcInfo, ENX_RTSP_STATE_DESCRIBE) == 0) {
		if (prcInfo->isAccept == ENX_YES) {
			char strSdp[1024] = {0};
			char strIP[16] = {0};
			char *strpIP, *strEnd;
			int sdp_len = 0;

			strpIP = strstr(prcInfo->strUrl, "//");
			if (strpIP != NULL) {
				strpIP += 2;
				strEnd = strchr(strpIP, ':');
				if (strEnd == NULL) {
					strEnd = strchr(strpIP, '/');
				}

				if (strEnd != NULL) {
					strlcpy(strIP, strpIP, min(sizeof(strIP), (UINT)(strEnd - strpIP + 1)));
	 			} else {
					printf("Error MY IP(%s)\n", prcInfo->strUrl);
					UINT getDefaultIP;
					network_default_netif_get_ip(&getDefaultIP);
					es_sprintf(strIP, "%IP", getDefaultIP);
				}
			}

			const char* sdpSessionFmt =	"v=0\n"
										"o=- 0 0 IN IP4 %s\n"
										"s=Session streamed by \""RTSPD_SERVER_AGENT"\"\n"
										"i=%s\n" // Filename or H264Stream or JPEGStream
										"c=IN IP4 0.0.0.0\n"
										"t=0 0\n"
										"a=tool:"RTSPD_SERVER_AGENT"\n"
										"a=control:%s\n"
										"a=range:npt=0-\n"
										"a=x-qt-text-nam:Session streamed by \""RTSPD_SERVER_AGENT"\"\n"
										"a=x-qt-text-inf:%s\n";

			switch (prcInfo->isLive) {
				case ENX_RTSP_STREAM_FILE_H264_TYPE:
				case ENX_RTSP_STREAM_FILE_H265_TYPE:
					flprintf("Not support(%d)\n", prcInfo->isLive);
					break;
				case ENX_RTSP_STREAM_LIVE_H264_1:
				case ENX_RTSP_STREAM_LIVE_H264_2:
					{
//						flprintf("Live Type(%d)\n", prcInfo->isLive);
						const char* sdpH264Line =	"m=video 0 RTP/AVP %u\n" // mediaType(video, audio), port(def 0), payloadType
													"b=AS:20480\n"
													"a=rtpmap:%u H264/90000\n" // payloadType, payloadFormatName(H.264, H.265, JPEG, etc..), TimestampFrequency(H.264/H.265 def 90kHZ, JPEG...?),
													"a=control:%s/%s\n" // track1 ~ track?? => session 당 1
													"a=recvonly\n"
													"a=fmtp:%u packetization-mode=1\n"
//													"a=fmtp:96 packetization-mode=1;profile-level-id=420029;sprop-parameter-sets=Z0IAKZWgFAFsQA==,aM4eDA==\n" // sps(720), pps(q25)
//													"a=fmtp:96 packetization-mode=1;profile-level-id=420029;sprop-parameter-sets=Z0IAKZWgHgCJeVA=,aM4eDA==\n" // sps(1080), pps(q25)
													;
						sdp_len  = sprintf(strSdp, sdpSessionFmt, strIP, "H264Stream", prcInfo->strUrl, "H264Stream");
						sdp_len += sprintf(strSdp + sdp_len, sdpH264Line, RTP_numPayload_H264, RTP_numPayload_H264, prcInfo->strUrl, ENX_RTSP_SDP_URL_VIDEO, RTP_numPayload_H264);
					}
					break;
				case ENX_RTSP_STREAM_LIVE_H265_1:
				case ENX_RTSP_STREAM_LIVE_H265_2:
					{
//						flprintf("Live Type(%d)\n", prcInfo->isLive);
						const char* sdpH265Line =	"m=video 0 RTP/AVP %u\n" // mediaType(video, audio), port(def 0), payloadType
													"b=AS:20480\n"
													"a=rtpmap:%u H265/90000\n" // payloadType, payloadFormatName(H.264, H.265, JPEG, etc..), TimestampFrequency(H.264/H.265 def 90kHZ, JPEG...?),
													"a=control:%s/%s\n" // track1 ~ track?? => session 당 1
													"a=recvonly\n"
													"a=fmtp:%u sprop-sps=QgEBAWAAAAMAsAAAAwAAAwB7oAPAgBDlja5JMvwCAAADAAIAAAMAeUI=; sprop-pps=RAHA8vA8kAA=\n"
													"a=Media_header:MEDIAINFO=494D4B48010200000400050000000000000000000000000000000000000000000000000000000000;\n"
													"a=appversion:1.0\n"
//													"a=fmtp:%u packetization-mode=1\n"
													;
						sdp_len  = sprintf(strSdp, sdpSessionFmt, strIP, "H265Stream", prcInfo->strUrl, "H265Stream");
						sdp_len += sprintf(strSdp + sdp_len, sdpH265Line, RTP_numPayload_H265, RTP_numPayload_H265, prcInfo->strUrl, ENX_RTSP_SDP_URL_VIDEO, RTP_numPayload_H265);
					}
					break;
#if (JPEG_STREAM==1)
				case ENX_RTSP_STREAM_LIVE_JPEG:
					{
//						flprintf("Live Type(%d)\n", prcInfo->isLive);
						const char* sdpJPEGLine =	"m=video 0 RTP/AVP %u\n" // mediaType(video, audio), port(def 0), payloadType
													"b=AS:20480\n"
													"a=rtpmap:%u JPEG/90000\n" // payloadType, payloadFormatName(H264, JPEG, etc..), TimestampFrequency(H264 def 90kHZ, JPEG...?),
													"a=control:%s/%s\n" // track1 ~ track?? => session 당 1
													"a=recvonly\n"
													;
						sdp_len  = sprintf(strSdp, sdpSessionFmt, strIP, "JPEGStream", prcInfo->strUrl, "JPEGStream");
						sdp_len += sprintf(strSdp + sdp_len, sdpJPEGLine, RTP_numPayload_JPEG, RTP_numPayload_JPEG, prcInfo->strUrl, ENX_RTSP_SDP_URL_VIDEO);
					}
					break;
#endif
#if (SW_JPEG_ENCODER==1)
				case ENX_RTSP_STREAM_LIVE_JPEG_SW:
					{
//						flprintf("Live Type(%d)\n", prcInfo->isLive);
						const char* sdpJPEGLine =	"m=video 0 RTP/AVP %u\n" // mediaType(video, audio), port(def 0), payloadType
													"b=AS:2048\n"
													"a=rtpmap:%u JPEG/90000\n" // payloadType, payloadFormatName(H264, JPEG, etc..), TimestampFrequency(H264 def 90kHZ, JPEG...?),
													"a=control:%s/%s\n" // track1 ~ track?? => session 당 1
													"a=recvonly\n"
													;
						sdp_len  = sprintf(strSdp, sdpSessionFmt, strIP, "JPEGStream", prcInfo->strUrl, "JPEGStream");
						sdp_len += sprintf(strSdp + sdp_len, sdpJPEGLine, RTP_numPayload_JPEG, RTP_numPayload_JPEG, prcInfo->strUrl, ENX_RTSP_SDP_URL_VIDEO);
					}
				break;
#endif
				default:
					flprintf("Error(%d)\n", prcInfo->isLive);
					break;
			}

#ifdef __AUDIO__
#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
			const char* sdpAudLine =	"m=audio 0 RTP/AVP %u\n"
#if (PCM_FREQUENCY==PCM_16BIT_8000HZ)
										"b=AS:128\n"
										"a=rtpmap:%u L16/8000\n"
#elif (PCM_FREQUENCY==PCM_16BIT_16000HZ)
										"b=AS:256\n"
										"a=rtpmap:%u L16/16000\n"
#elif (PCM_FREQUENCY==PCM_16BIT_32000HZ)
										"b=AS:512\n"
										"a=rtpmap:%u L16/32000\n"
#elif (PCM_FREQUENCY==PCM_16BIT_11025HZ)
										"b=AS:176\n"
										"a=rtpmap:%u L16/11025\n"
#elif (PCM_FREQUENCY==PCM_16BIT_22050HZ)
										"b=AS:352\n"
										"a=rtpmap:%u L16/22050\n"
#elif (PCM_FREQUENCY==PCM_16BIT_44100HZ)
										"b=AS:705\n"
										"a=rtpmap:%u L16/44100\n"
#endif
										"a=control:%s/%s\n"
										"a=recvonly\n";
			// raw
			sdp_len += sprintf(strSdp + sdp_len, sdpAudLine, RTP_numPayload_L16S, RTP_numPayload_L16S, prcInfo->strUrl, ENX_RTSP_SDP_URL_AUDIO);
#elif (AUDIO_CODEC==AUDIO_CODEC_G711A)
			const char* sdpG711Line =	"m=audio 0 RTP/AVP %u\n"
										"b=AS:64\n"
										"a=rtpmap:%u PCM%c/8000\n"
										"a=control:%s/%s\n"
										"a=recvonly\n";
			sdp_len += sprintf(strSdp + sdp_len, sdpG711Line, RTP_numPayload_PCMA, RTP_numPayload_PCMA, 'A', prcInfo->strUrl, ENX_RTSP_SDP_URL_AUDIO);
#elif (AUDIO_CODEC==AUDIO_CODEC_G711U)
			const char* sdpAudLine =	"m=audio 0 RTP/AVP %u\n"
										"b=AS:64\n"
										"a=rtpmap:%u PCM%c/8000\n"
										"a=control:%s/%s\n"
										"a=recvonly\n";
			sdp_len += sprintf(strSdp + sdp_len, sdpAudLine, RTP_numPayload_PCMU, RTP_numPayload_PCMU, 'U', prcInfo->strUrl, ENX_RTSP_SDP_URL_AUDIO);
#endif
#endif

			char strDate[40] = {0};
			rtspd_client_rtsp_make_datetime(strDate);
			if (prcInfo->nSession == 0) {
				*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD "%s" ENX_RTSP_RESPONSE_DESCRIBE RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, strDate, sdp_len);
			} else {
				*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD_S "%s" ENX_RTSP_RESPONSE_DESCRIBE RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, prcInfo->nSession, strDate, sdp_len);
			}
			enx_strcat(buf, strSdp);
			*buf_len += sdp_len;
		} else {
			rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_406);
		}
	} else {
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_412);
	}

	LEAVE();
	return res;
}

int rtspd_client_rtsp_process_setup(rtsp_client *prcInfo, char *buf, int *buf_len)
{
	ENTER();

	int res = 0;

	if (rtspd_client_rtsp_state(prcInfo, ENX_RTSP_STATE_SETUP) == 0) {
		static SemaphoreHandle_t xSemSetup = NULL;
		if (xSemSetup == NULL) {
			xSemSetup = xSemaphoreCreateMutex();
		}

		xSemaphoreTake(xSemSetup, portMAX_DELAY);

		do {
			// 접속이 추가로 가능한지 판단한다.
			int check_count = rtspd_player_count();
			_Yprintf("Check Count: %u\n", check_count);
			if (check_count >= RTSP_play_MAX) {
				rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_453);
				break;
			}

			if (prcInfo->rtp_ss == NULL) {
				prcInfo->rtp_ss = (rtp_session *)pvPortCalloc(ENX_RTSP_STRTYPE_numNONE, sizeof(rtp_session));
				if (prcInfo->rtp_ss == NULL) {
					flprintf("rtp_ss NULL response 500\n");
					rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_500);
					break;
				}
				_Gprintf("rtpss[0](0x%08X) rtpss[1](0x%08X)\n", &prcInfo->rtp_ss[0], &prcInfo->rtp_ss[1]);
			}
//			_Yprintf("rtp_ss(%ubyte)(0x%08X) setup_query(%u) nQuery(%u) isLive(%u)\n",
//					sizeof(rtp_session), prcInfo->rtp_ss, prcInfo->setup_query, prcInfo->nQuery, prcInfo->isLive);

			if (prcInfo->isTransport == ENX_YES) {
				// 전송 가능한 통신프로토콜로 요청했는지 판단한다.
				switch (prcInfo->eTransport) {
					case ENX_RTSP_TRANSPORT_TCP:
					case ENX_RTSP_TRANSPORT_UDP:
						break;
					case ENX_RTSP_TRANSPORT_HTTP:
					case ENX_RTSP_TRANSPORT_NONE:
						res = -1;
						break;
				}

				if (res == -1) {
					rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_461);
					break;
				}

				// 올바른 URL을 요청였는지 확인한다.
				switch (prcInfo->setup_query) {
					case ENX_RTSP_STRTYPE_VIDEO:
						switch (prcInfo->isLive) {
							case ENX_RTSP_STREAM_FILE_H264_TYPE:
							case ENX_RTSP_STREAM_LIVE_H264_1:
							case ENX_RTSP_STREAM_LIVE_H264_2:
								prcInfo->rtp_ss[prcInfo->nQuery].payload_type = RTP_numPayload_H264;
								break;
							case ENX_RTSP_STREAM_FILE_H265_TYPE:
							case ENX_RTSP_STREAM_LIVE_H265_1:
							case ENX_RTSP_STREAM_LIVE_H265_2:
								prcInfo->rtp_ss[prcInfo->nQuery].payload_type = RTP_numPayload_H265;
								break;
#if (JPEG_STREAM==1)
							case ENX_RTSP_STREAM_LIVE_JPEG:
#endif
#if (SW_JPEG_ENCODER==1)
							case ENX_RTSP_STREAM_LIVE_JPEG_SW:
#endif
#if ((JPEG_STREAM==1) || (SW_JPEG_ENCODER==1))
								prcInfo->rtp_ss[prcInfo->nQuery].payload_type = RTP_numPayload_JPEG;
								break;
#endif
							default:
								res = -1;
								break;
						}
						break;
#ifdef __AUDIO__
					case ENX_RTSP_STRTYPE_AUDIO:
#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
						prcInfo->rtp_ss[prcInfo->nQuery].payload_type = RTP_numPayload_L16S;
#elif (AUDIO_CODEC==AUDIO_CODEC_G711A)
						prcInfo->rtp_ss[prcInfo->nQuery].payload_type = RTP_numPayload_PCMA;
#elif (AUDIO_CODEC==AUDIO_CODEC_G711U)
						prcInfo->rtp_ss[prcInfo->nQuery].payload_type = RTP_numPayload_PCMU;
#endif
						break;
#endif
//					case ENX_RTSP_STRTYPE_METADATA:
//					case ENX_RTSP_STRTYPE_BACKCHANNEL:
					case ENX_RTSP_STRTYPE_NONE:
					default:
						res = -1;
						break;
				}

				if (res == -1) {
					flprintf("Query(%d) StreamType(%d)\n", prcInfo->setup_query, prcInfo->isLive);
					rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_404);
					break;
				}

				// 모두가 올바르므로 전송할 구문 작성 및 client 전송세션의 초기화 작업을 한다.
				if (prcInfo->nSession == 0) {
					// sessionID는 랜덤하게 결정된 clientID값으로 설정한다.
					prcInfo->nSession = prcInfo->clientid;
				}

				prcInfo->rtp_ss[prcInfo->nQuery].ssrc = rand() * rand();
#if 1
				_Yprintf("Warning: TEST CODE: RTP start_rtptime\n");
				prcInfo->rtp_ss[prcInfo->nQuery].start_rtptime = (*mtime * 9) / 50;
#else
				prcInfo->rtp_ss[prcInfo->nQuery].start_rtptime = 1 + (UINT)(rand() % (0xFFFFFFFF));
#endif
				prcInfo->rtp_ss[prcInfo->nQuery].start_seq = 1 + (WORD)(rand() % (0xFFFF));
				prcInfo->rtp_ss[prcInfo->nQuery].packet_cnt = 0;
				prcInfo->rtp_ss[prcInfo->nQuery].total_length = 0;
				prcInfo->rtp_ss[prcInfo->nQuery].rtp_pk.nalnum = 0xFFFFFFFF; // H.264 only

				char strDate[40] = {0};
				rtspd_client_rtsp_make_datetime(strDate);
				switch (prcInfo->eTransport) {
					case ENX_RTSP_TRANSPORT_UDP:
						*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD_S ENX_RTSP_RESPONSE_SETUP_UDP "%s" RTSP_strCRLF,
							ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, prcInfo->nSession,
							prcInfo->conn_info.rtp_port[prcInfo->nQuery],prcInfo->conn_info.rtcp_port[prcInfo->nQuery],
							554, 555, prcInfo->rtp_ss[prcInfo->nQuery].ssrc, strDate);
						break;
					case ENX_RTSP_TRANSPORT_TCP:
					case ENX_RTSP_TRANSPORT_HTTP:
						*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD_S ENX_RTSP_RESPONSE_SETUP_TCP "%s" RTSP_strCRLF,
							ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, prcInfo->nSession,
							prcInfo->conn_info.rtp_port[prcInfo->nQuery],prcInfo->conn_info.rtcp_port[prcInfo->nQuery],
							prcInfo->rtp_ss[prcInfo->nQuery].ssrc, strDate);
						break;
					case ENX_RTSP_TRANSPORT_NONE:
						flprintf("Error Case\n");
						break;
				}

				prcInfo->conn_info.rtp_type = prcInfo->eTransport;

				// init
				if ((prcInfo->eTransport == ENX_RTSP_TRANSPORT_TCP) || (prcInfo->eTransport == ENX_RTSP_TRANSPORT_HTTP)) {
					char strbuf[8] = {0};
					sprintf(strbuf, "rtp%d", prcInfo->index);
					prcInfo->rtp_ss[prcInfo->nQuery].tx_ready = ENX_RTP_TXSTE_READY;
					if (prcInfo->play_query == ENX_RTSP_STRTYPE_NONE) {
//						_Cprintf("prcInfo->rtp_ss[0](0x%08X) [1](0x%08X)", &prcInfo->rtp_ss[0], &prcInfo->rtp_ss[1]);
						vTaskCreate(strbuf, rtspd_client_rtp_main, prcInfo, LV4_STACK_SIZE, LV5_TASK_PRIO);
					}
					prcInfo->play_query |= prcInfo->setup_query;
				} else if (prcInfo->eTransport == ENX_RTSP_TRANSPORT_UDP) {
					prcInfo->play_query |= prcInfo->setup_query;
					prcInfo->rtp_ss[prcInfo->nQuery].tx_ready = ENX_RTP_TXSTE_READY;
				}
			} else {
				rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_400);
			}
		} while (0);

		xSemaphoreGive(xSemSetup);
	} else {
		switch (prcInfo->state) {
			case ENX_RTSP_STATE_PLAY:
			case ENX_RTSP_STATE_PAUSE:
				rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_455);
				break;
			case ENX_RTSP_STATE_NONE:
			case ENX_RTSP_STATE_TEARDOWN:
			case ENX_RTSP_STATE_ERROR:
				rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_412);
				break;
			default:
				flprintf("Error: case fail\n");
				break;
		}
	}

	LEAVE();
	return res;

	UNUSED(prcInfo);
	UNUSED(buf);
	UNUSED(buf_len);
}

int rtspd_client_rtsp_process_play(rtsp_client *prcInfo, char *buf, int *buf_len)
{
	ENTER();

	int res = 0;
	if (prcInfo->play_query != ENX_RTSP_STRTYPE_NONE) {
		if (rtspd_client_rtsp_state(prcInfo, ENX_RTSP_STATE_PLAY) == 0) {
			char strDate[40] = {0};
			rtspd_client_rtsp_make_datetime(strDate);
			if (prcInfo->nSession != 0) {
				char strRtpinfo[1024] = {0};
				int nLen, offset = sprintf(strRtpinfo, ENX_RTSP_RESPONSE_RTPINFO_H);

				if (prcInfo->rtp_ss == NULL) {
					flprintf("rtp_ss NULL!!!!!\n");
					while (1) {
						vTaskDelay(1);
					}
				}

				if (prcInfo->play_query & ENX_RTSP_STRTYPE_VIDEO) {
					nLen = sprintf(strRtpinfo + offset, ENX_RTSP_RESPONSE_RTPINFO_B, prcInfo->strUrl, ENX_RTSP_SDP_URL_VIDEO, prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numVIDEO].start_seq, prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numVIDEO].start_rtptime);
					offset += nLen;
				}
				if (prcInfo->play_query & ENX_RTSP_STRTYPE_AUDIO) {
					nLen = sprintf(strRtpinfo + offset, ENX_RTSP_RESPONSE_RTPINFO_B, prcInfo->strUrl, ENX_RTSP_SDP_URL_AUDIO, prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numAUDIO].start_seq, prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numAUDIO].start_rtptime);
					offset += nLen;
				}
//				if (prcInfo->play_query & ENX_RTSP_STRTYPE_METADATA) {
//					nLen = sprintf(strRtpinfo + offset, ENX_RTSP_RESPONSE_RTPINFO_B, prcInfo->strUrl, ENX_RTSP_SDP_URL_METADATA, prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numMETADATA].start_seq, prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numMETADATA].start_rtptime);
//					offset += nLen;
//				}
//				if (prcInfo->play_query & ENX_RTSP_STRTYPE_BACKCHANNEL) {
//					nLen = sprintf(strRtpinfo + offset, ENX_RTSP_RESPONSE_RTPINFO_B, prcInfo->strUrl, ENX_RTSP_SDP_URL_BACKCHANNEL, prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numBACKCHANNEL].start_seq, prcInfo->rtp_ss[ENX_RTSP_STRTYPE_numBACKCHANNEL].start_rtptime);
//					offset += nLen;
//				}
				nLen = sprintf(strRtpinfo + offset - 1, RTSP_strCRLF); // - 1 => 문자열 끝에 있는 ','를 제거하고 CLRF를 붙인다.
				offset += nLen;											// $CMT-hjlee-180220 - 
				if (offset >= 1024) {									// $CMT-hjlee-180220 - 
					flprintf("buffer overflow(%dbyte)\n", offset);	// $CMT-hjlee-180220 -
				}														// $CMT-hjlee-180220 - 

				*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD_S "%s%s" RTSP_strCRLF, 
					ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, prcInfo->nSession, strRtpinfo, strDate);
			} else {
				res = -1;	
			}
		} else {
			res = -1;
			rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_400);
		}
	} else {
		res = -1;
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_400);
	}

	LEAVE();
	return res;

	UNUSED(prcInfo);
	UNUSED(buf);
	UNUSED(buf_len);
}

int rtspd_client_rtsp_process_pause(rtsp_client *prcInfo, char *buf, int *buf_len)
{
	ENTER();

	int res = 0;

	if (rtspd_client_rtsp_state(prcInfo, ENX_RTSP_STATE_PAUSE) == 0) {
//		flprintf("state1(%d)\n", prcInfo->state);
		char strDate[40] = {0};
		rtspd_client_rtsp_make_datetime(strDate);
		if (prcInfo->nSession == 0) {
			*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD "%s" RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, strDate);
		} else {
			*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD_S "%s" RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, prcInfo->nSession, strDate);
		}
	} else {
		flprintf("state2(%d)\n", prcInfo->state);
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_400);
	}

	LEAVE();
	return res;

	UNUSED(prcInfo);
	UNUSED(buf);
	UNUSED(buf_len);
}

int rtspd_client_rtsp_process_teardown(rtsp_client *prcInfo, char *buf, int *buf_len)
{
	ENTER();

	int res = 0;

	if (rtspd_client_rtsp_state(prcInfo, ENX_RTSP_STATE_TEARDOWN) == 0) {
//		flprintf("state1(%d)\n", prcInfo->state);
		char strDate[40] = {0};
		rtspd_client_rtsp_make_datetime(strDate);
		if (prcInfo->nSession == 0) {
			*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD "%s" RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, strDate);
		} else {
			*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD_S "%s" RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, prcInfo->nSession, strDate);
		}

		if (prcInfo->rtp_ss) {
			vPortFree(prcInfo->rtp_ss);
			prcInfo->rtp_ss = NULL;
		}
	} else {
		flprintf("state2(%d)\n", prcInfo->state);
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_400);
	}

	LEAVE();
	return res;

	UNUSED(prcInfo);
	UNUSED(buf);
	UNUSED(buf_len);
}

int rtspd_client_rtsp_process_get_parameter(rtsp_client *prcInfo, char *buf, int *buf_len)
{
	ENTER();

	int res = 0;

	if (rtspd_client_rtsp_state(prcInfo, ENX_RTSP_STATE_GET_PARAMETER) == 0) {
		char strDate[40] = {0};
		rtspd_client_rtsp_make_datetime(strDate);
		if (prcInfo->nSession == 0) {
			*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD "%s" RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, strDate);
		} else {
			*buf_len = sprintf(buf, ENX_RTSP_RESPONSE_HEAD_S "%s" RTSP_strCRLF, ENX_RTSP_RESPONSE_200, prcInfo->nCSeq, prcInfo->nSession, strDate);
		}
	} else {
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_400);
	}

	LEAVE();
	return res;

	UNUSED(prcInfo);
	UNUSED(buf);
	UNUSED(buf_len);
}

int rtspd_client_rtsp_process_set_parameter(rtsp_client *prcInfo, char *buf, int *buf_len)
{
	ENTER();

	int res = 0;

	if (rtspd_client_rtsp_state(prcInfo, ENX_RTSP_STATE_SET_PARAMETER) == 0) {
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_458);
	} else {
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_400);
	}

	LEAVE();
	return res;

	UNUSED(prcInfo);
	UNUSED(buf);
	UNUSED(buf_len);
}

int rtspd_client_rtsp_response(rtsp_client *prcInfo, char *buf, int *buf_len)
{
	ENTER();

	int res = 0;
	*buf_len = 0;

	if (prcInfo->state == ENX_RTSP_STATE_TEARDOWN) {
		flprintf("TEARDOWN(%d)(412) : %s\n", prcInfo->state, prcInfo->strUrl);
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_412);
		res = -1;
	} else if (rtspd_client_rtsp_check_url(prcInfo, prcInfo->strUrl) != -1) {
		switch (prcInfo->method) {
			case ENX_RTSP_METHOD_OPTIONS:
				rtspd_client_rtsp_process_options(prcInfo, buf, buf_len);
				break;
			case ENX_RTSP_METHOD_DESCRIBE:
				rtspd_client_rtsp_process_describe(prcInfo, buf, buf_len);
				break;
			case ENX_RTSP_METHOD_SETUP:
				rtspd_client_rtsp_process_setup(prcInfo, buf, buf_len);
				break;
			case ENX_RTSP_METHOD_PLAY:
				rtspd_client_rtsp_process_play(prcInfo, buf, buf_len);
				break;
			case ENX_RTSP_METHOD_PAUSE:
				rtspd_client_rtsp_process_pause(prcInfo, buf, buf_len);
				break;
			case ENX_RTSP_METHOD_TEARDOWN:
				rtspd_client_rtsp_process_teardown(prcInfo, buf, buf_len);
				break;
			case ENX_RTSP_METHOD_GET_PARAMETER:
				rtspd_client_rtsp_process_get_parameter(prcInfo, buf, buf_len);
				break;
			case ENX_RTSP_METHOD_SET_PARAMETER:
				rtspd_client_rtsp_process_set_parameter(prcInfo, buf, buf_len);
				break;
			default:
				flprintf("Unknown Method(id) : %d\n", prcInfo->method);
				rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_400);
				res = -1;
				break;
		}
	} else {
		flprintf("Unknown URL(501) : %s\n", prcInfo->strUrl);
		rtspd_client_rtsp_process_error(prcInfo, buf, buf_len, ENX_RTSP_RESPONSE_501);
		res = -1;
	}

	LEAVE();
	return res;
}
#endif
