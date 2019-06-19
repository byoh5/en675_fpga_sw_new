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

#ifndef _RTCP_PROCESS_H_
#define _RTCP_PROCESS_H_

/* Current protocol version */
#define RTCP_VERSION 2

typedef enum {
	RTCP_SR   = 200,
	RTCP_RR   = 201,
	RTCP_SDES = 202,
	RTCP_BYE  = 203,
	RTCP_APP  = 204,
} rtcp_type_t;

typedef enum {
	ENX_RTCP_SDES_numEND   = 0,
	ENX_RTCP_SDES_numCNAME = 1,
	ENX_RTCP_SDES_numNAME  = 2,
	ENX_RTCP_SDES_numEMAIL = 3,
	ENX_RTCP_SDES_numPHONE = 4,
	ENX_RTCP_SDES_numLOC   = 5,
	ENX_RTCP_SDES_numTOOL  = 6,
	ENX_RTCP_SDES_numNOTE  = 7,
	ENX_RTCP_SDES_numPRIV  = 8
} rtcp_sdes_type;

/* Macros for RTP structure */
#define RTCP_LENGTH_BYTE(rtcphd) (((rtcphd)->rtcp_length + 1) * 4) /* assumes appropriate byte order */

typedef struct {
	uint32 rtcp_ver:2;		// version
	uint32 rtcp_pad:1;		// padding flag
	uint32 rtcp_count:5;	// object count
	uint32 rtcp_type:8;		// Message type
	uint32 rtcp_length:16;	// Message length
}__attribute__ ((packed)) rtcppk_header;

/* RTCP Receiver report block */
typedef struct {
	uint32 rb_ssrc;			// SSRC to which this rr refers
	uint32 rb_fraclost:8;	// fraction lost
	int32  rb_cumlost:24;	// cumulative packets lost
	uint32 rb_hiseq;		// Extended highest seq recvd
	uint32 rb_jitter;		// Jitter measure
	uint32 rb_lastsrts;		// Middle 32 of last SR NTP
	uint32 rb_delay;		// Delay since lsr
}__attribute__ ((packed)) rtcppk_rblock;

/* RTCP Receiver report message */
typedef struct {
	uint32 rr_ssrc;			// SSRC of sender
	rtcppk_rblock* rr_rb;	// Pointer to 1st report block
}__attribute__ ((packed)) rtcppk_rr;

/* RTCP Sender report message */
typedef struct {
	uint32 sr_ssrc;			// SSRC of sender
	uint32 sr_intts;		// NTP Timestamp (high 32 bits)
	uint32 sr_fracts;		// NTP Timestamp (lo 32 bits)
	uint32 sr_rtpts;		// RTP media timestamp
	uint32 sr_packets;		// Sender's sent packet count
	uint32 sr_octets;		// Sender's sent octet count
//	uint8* sr_rblock;		// First reception report
}__attribute__ ((packed)) rtcppk_sr;

/* Struct for source description chunk item */
typedef struct {
	uint8  sdi_type;		// Type of SDES item
	uint8  sdi_len;			// Length of item data
	uint8* sdi_data;		// SDES item data
}__attribute__ ((packed)) rtcppk_sditem;

/* Struct for source description message `chunk' */
typedef struct {
	uint32 sdc_ssrc;		// SSRC to which chunk refers
	rtcppk_sditem* sdc_data;// Beginning of SDES items
}__attribute__ ((packed)) rtcppk_sdes;

/* Struct for reason in RTCP bye message */
typedef struct {
	uint8  bye_length;		// Length of reason text
	int8*  bye_reason;		// Reason for leaving.
}__attribute__ ((packed)) rtcppk_byereason;

typedef struct {
	uint32 bye_ssrc;
	rtcppk_byereason* bye_reason;
}__attribute__ ((packed)) rtcppk_bye;

/* Struct for RTCP application-specific message */
typedef struct {
	uint32 app_ssrc;		// SSRC of sender
	char   app_name[4];		// Name specififying app
	uint8* app_data;		// Application-specific data
}__attribute__ ((packed)) rtcppk_app;

typedef struct {
	rtcppk_header header;
	union {
		rtcppk_sr sr;		// 200
		rtcppk_rr rr;		// 201
		rtcppk_sdes sdrs;	// 202
		rtcppk_bye bye;		// 203
		rtcppk_app app;		// 204
	} data;
}__attribute__ ((packed)) rtcppk_t;

extern int rtspd_client_rtcp_send_sr(rtsp_client *prcInfo, int media_type, BYTE *send_buffer);
extern int rtspd_client_rtcp_process(rtsp_client *prcInfo, int media_type, BYTE *buf, int buf_len);

#endif
