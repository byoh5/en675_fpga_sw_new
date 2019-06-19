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

#ifndef _RTP_JPEG_SW_H_
#define _RTP_JPEG_SW_H_

// 8 byte
typedef struct {
	uint32 un8TypeSpecific:8;		/* type-specific field */
	uint32 un24FragmentOffset:24;	/* fragment byte offset */
	uint8 un8Type;					/* id of jpeg decoder params */
	uint8 un8Q;						/* quantization factor (or table id) */
	uint8 un8Width;					/* frame width in 8 pixel blocks */
	uint8 un8Height;				/* frame height in 8 pixel blocks */
}__attribute__ ((packed)) jpgswHeader;

// 4 byte
typedef struct {
	uint8 un8MBZ;
	uint8 un8Precision;
	uint16 un16Length;
}__attribute__ ((packed)) jpgswQTHeader;

#if 0
#define JPG_numHeaderSize   613		/* jpeg header size -- Add header */
#define JPG_numOffset_YQUNT	0x19	/* Y quantization offset in header of jpeg */
#define JPG_numOffset_CQUNT	0x5a	/* Y quantization offset in header of jpeg */
#define JPG_numQUNTSize		64		/* length of q tbl */
#define JPG_numYCQUNTSize	(JPG_numQUNTSize*2) /* length of both of c and q tbl */
#define JPG_numOffset_SOFO	0x23e
#else
#define JPGSW_numHeaderSize		0x25f// 334		/* jpeg header size -- Add header */
#define JPGSW_numOffset_YQUNT	0x19	/* Y quantization offset in header of jpeg */
#define JPGSW_numOffset_CQUNT	(JPGSW_numOffset_YQUNT+65)
#define JPGSW_numQUNTSize		64		/* length of q tbl */
#define JPGSW_numYCQUNTSize		(JPGSW_numQUNTSize*2) // (JPGSW_numQUNTSize) /* length of both of c and q tbl */
#define JPGSW_numOffset_SOFO	0x9a // 0x131
#endif

extern int rtspd_client_rtp_jpeg_sw_main(rtsp_client *prcInfo);

#endif
