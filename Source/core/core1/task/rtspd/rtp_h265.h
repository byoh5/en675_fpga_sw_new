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

#ifndef _RTP_H265_H_
#define _RTP_H265_H_

// 2 byte
typedef struct {
	BF_4(
	uint16 un1F:1,
	uint16 un6Type:6,
	uint16 un6Layerid:6,
	uint16 un3Tid:3
	)
}__attribute__ ((packed)) hevcPayloadHeader;

// 3 byte
typedef struct {
#if 1
	union {
		WORD un16PayloadHdr;
		hevcPayloadHeader PayloadHdr;
	};
	BF_3(
	uint8 un1Startbit:1,
	uint8 un1Endbit:1,
	uint8 un6Nalunittype:6
	)
#else
	uint16 un1F:1;
	uint16 un6Type:6;
	uint16 un6Layerid:6;
	uint16 un3Tid:3;
	uint8 un1Startbit:1;
	uint8 un1Endbit:1;
	uint8 un6Nalunittype:6;
#endif
}__attribute__ ((packed)) hevcFUHeader;

extern int rtspd_client_rtp_h265_main(rtsp_client *prcInfo);

#endif
