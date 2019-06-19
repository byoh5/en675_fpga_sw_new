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

#ifndef _RTSPD_PROCESS_H_
#define _RTSPD_PROCESS_H_

extern void rtspd_client_rtp_main(void *ctx);

extern int rtspd_client_rtsp_header_parse(rtsp_client *prcInfo, char *buf, UINT buf_len);
extern int rtspd_client_rtsp_state(rtsp_client *prcInfo, rtsp_client_state state);
extern int rtspd_client_rtsp_method_parse(rtsp_client *prcInfo, BYTE *buf, UINT buf_len);
extern int rtspd_client_rtsp_interleaved_parse(rtsp_client *prcInfo, BYTE *buf, UINT buf_len);
extern int rtspd_client_rtsp_check_url(rtsp_client *prcInfo, char *url);

extern int rtspd_client_rtsp_make_datetime(char *buf);
extern int rtspd_client_rtsp_process_error(rtsp_client *prcInfo, char *buf, int *buf_len, char *strError);
extern int rtspd_client_rtsp_process_options(rtsp_client *prcInfo, char *buf, int *buf_len);
extern int rtspd_client_rtsp_process_describe(rtsp_client *prcInfo, char *buf, int *buf_len);
extern int rtspd_client_rtsp_process_setup(rtsp_client *prcInfo, char *buf, int *buf_len);
extern int rtspd_client_rtsp_process_play(rtsp_client *prcInfo, char *buf, int *buf_len);
extern int rtspd_client_rtsp_process_pause(rtsp_client *prcInfo, char *buf, int *buf_len);
extern int rtspd_client_rtsp_process_teardown(rtsp_client *prcInfo, char *buf, int *buf_len);

extern int rtspd_client_rtsp_process_get_parameter(rtsp_client *prcInfo, char *buf, int *buf_len);
extern int rtspd_client_rtsp_process_set_parameter(rtsp_client *prcInfo, char *buf, int *buf_len);
extern int rtspd_client_rtsp_response(rtsp_client *prcInfo, char *buf, int *buf_len);

#endif