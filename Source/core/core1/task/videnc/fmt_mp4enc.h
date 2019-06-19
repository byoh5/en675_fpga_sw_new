/* Copyright (c) 2013-2014 Eyenix Corporation. All rights reserved.
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
* AVR product.
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

#ifndef _MAKEMP4_H_
#define _MAKEMP4_H_

#include "ff.h"

/**************************************************************************
  Simple MP4 Format
  
ftyp ( file type box )
moov ( movie box )
	 mvhd ( movie header box )
	 trak ( trak box )
	 	  tkhd ( track header box )
	 	  edts ( edit box )
	 	  	   elst ( edit list box )
	 	  mdia ( media box )
	 	  	   mdhd ( media header box )
	 	  	   hdlr ( handler reference box )
	 	  	   minf ( media information box )
	 	  	   		hdlr ( handler reference box )
	 	  	   		stbl ( sample table box )
	 	  	   			 stsd ( sample description box )
	 	  	   			 stts ( time-to-sample box )
	 	  	   			 stsz ( sample size box )
	 	  	   			 stsc ( sample to chunk box )
	 	  	   			 stco ( chunk offset box )
	 	  	   			 ....
	 trak
	 trak
	 ....
mdat ( media data box )
....
**************************************************************************/

#define MP4_KEYFRAME	SAVE_NOR_TIME_MAX+1

typedef struct {
	UINT option;							// Video, Audio, Text

	int i_width;
	int i_height;
	int f_fps;

	UINT type;
	UINT un32vidicount;	// video i-frame count
	UINT un32vidscount;	// video total frame count
	UINT un32audscount;	// audio total frame count
	UINT un32txtscount;	// G-sensor, GPS, etc...

	UINT un32vidsmax;
	UINT un32audsmax;
	UINT un32txtsmax;
	UINT un32totalmax;
	UINT un32maxtime;

	uint16 sps_size;
	uint16 pps_size;
	char sps_data[16];
	char pps_data[8];

	UINT mdat_size;
} t_mp4_t;

enum {
	H264_STREAM = 0,
	G711_STREAM = 1,
	TXTS_STREAM = 2,
	IDX1_ARRAY = 9
};

#define FCC(ch1, ch2, ch3, ch4)	((DWORD)((ch1 << 24) | (ch2 << 16) | (ch3 << 8) | (ch4 << 0)))

typedef DWORD FOURCC;

typedef struct {
	DWORD	cb;							// ����ü�� ����� �����Ѵ�. ������ 8 ����Ʈ���� ������ ���� �����Ѵ�.
	FOURCC	fcc;						// FOURCC �ڵ带 �����Ѵ�.	
} __attribute__ ((packed)) FCC_CB_t;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'stco' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwEntryCount;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_CHUNK_OFFSET_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'stsz' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwSampleSize;
		DWORD	dwEntryCount;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SAMPLE_SIZE_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'stss' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwEntryCount;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SYNC_SAMPLE_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'stsc' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwEntryCount;
		DWORD	dwFirstChunk;
		DWORD	dwSamplePerChunk;
		DWORD	dwSampleDescriptionID;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SAMPLE_TO_CHUNK_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'stts' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwEntryCount;
		DWORD	dwSampleCount;
		DWORD	dwSampleDelta;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_DECODING_TIME_TO_SAMPLE_BOX;

/*		 __AVCC Format__		AVC Codec			*
 *		 bits    									*
 *		  8   version ( always 0x01 )				*
 *		  8   avc profile ( sps[0][1] )				*
 *		  8   avc compatibility ( sps[0][2] )		*
 *		  8   avc level ( sps[0][3] )				*
 *		  6   reserved ( all bits on )				*
 *		  2   NALULengthSizeMinusOne				*
 *		  3   reserved ( all bits on )				*
 *		  5   number of SPS NALUs (usually 1)		*
 *		 repeated once per SPS:						*
 *		  16     SPS size							*
 *		 variable   SPS NALU data					*
 *		  8   number of PPS NALUs (usually 1)		*
 *		 repeated once per PPS						*
 *		  16    PPS size							*
 *		  variable PPS NALU data					*/
typedef struct {
	FCC_CB_t fcccb;						// ���� 'avcC' �� �ƴϸ� �ȵȴ�.
	struct {
		uint8 bConfigureationVersion;
		uint8 bAVCProfile;
		uint8 bProfileCompatibility;
		uint8 bAVCLevelIndication;
		uint8 bLengthSizeMinusOne;
//		uint8 bNumOfSPS;
//		uint16 wSPSL;
//		uint8 bSPS[11];		// sps 11bytes
//		uint8 bNumOfPPS;
//		uint16 wPPSL;
//		uint8 bPPS[5];		// pps 5bytes
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_AVC_CONFIGURATION_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'avc1' �� �ƴϸ� �ȵȴ�.
	struct {
		BYTE	bReserved[6];
		WORD	wDataRefIndex;
		DWORD	dwPreDefined[4];
		WORD	wWidth;
		WORD	wHeight;
		DWORD	dwHorizResolution;
		DWORD	dwVertiResolution;
		DWORD	dwDatasize;
		WORD	wFrameCount;
		DWORD	dwCompressorName[8];
		WORD	wDepth;
		WORD	wPreDefined2;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_AVC1_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'twos' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD dwReserved[4];
//		DWORD dwVersion_Flags;
//		DWORD dwVendor;
		WORD wNumberOfChannels;
		WORD wSampleSize;
		WORD wCompressionID;
		WORD wPacketSize;
		DWORD dwSampleRate;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_TWOS_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'stsd' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD dwVersion_Flags;
		DWORD dwEntryCount;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SAMPLE_DESCRIPTION_BOX;

typedef struct {
	FCC_CB_t fcccb; 					// ���� 'url ' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD dwVersion_Flags;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_DATA_ENTRY_URL_BOX;

typedef struct {
	FCC_CB_t fcccb; 					// ���� 'dref' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD dwVersion_Flags;
		DWORD dwEntry;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_DATA_REFERENCE_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'vmhd' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		WORD	wGraphicsMode;
		WORD	wOpColor[3];
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_VIDEO_MEDIA_HEADER_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'smhd' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		WORD	wBalance;
		WORD	_res1;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SOUND_MEDIA_HEADER_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'hdlr' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwPreDefined;
		DWORD	dwSubtype;
		DWORD	_res1;
		DWORD	_res2;
		DWORD	_res3;
		DWORD	dwName[3];
		uint8	bNull;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_HANDLER_REFERENCE_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'mdhd' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwCreationtime;
		DWORD	dwModificationtime;
		DWORD	dwTimescale;
		DWORD	dwDuration;
		WORD	wLanguage;
		WORD	wPreDefined;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_MEDIA_HEADER_BOX;

typedef struct {
	FCC_CB_t fcccb; 					// ���� 'elst' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD dwVersion_Flags;
		DWORD dwEntries;
		DWORD dwTrackduration;
		DWORD dwMediatime;
		DWORD dwMediarate;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_EDIT_LIST_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'tkhd' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwCreationtime;
		DWORD	dwModificationtime;
		DWORD	dwTrackId;				 // 1: Video, 2: Audio
		DWORD	_res1;
		DWORD	dwDuration;
		DWORD	_res2;
		DWORD	_res3;
		WORD	wLayer;
		WORD	wAlternateGroup;
		WORD	wVolume;				// Video: 0x0000, Audio: 0x0100
		WORD	_res4;
		DWORD	dwMatrix[9];
		DWORD	dwWidth;				// Video: ex:1920, Audio: 0
		DWORD	dwHeight;				// Video: ex:1080, Audio: 0
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_TRACK_HEADER_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'mvhd' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwCreationtime;
		DWORD	dwModificationtime;
		DWORD	dwTimescale;
		DWORD	dwDuration;				// TimeScale * Play Time
		DWORD	dwRate;
		WORD	wVolume;
		WORD	_res1;
		DWORD	_res2;
		DWORD	_res3;
		DWORD	dwMatrix[9];
		DWORD	dwPreDefined[6];
		DWORD	dwNexttrakid;			// Only Video:2 A+V:3
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_MOVIE_HEADER_BOX;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'ftyp' �� �ƴϸ� �ȵȴ�.
	struct {
		FOURCC fcc1;
		FOURCC fcc2;
		FOURCC fcc3;
		FOURCC fcc4;
		FOURCC fcc5;
		FOURCC fcc6;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_FILE_TYPE_BOX;

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern BYTE *mp4_write_header(t_mp4_t *mp4a, void *buffer);

//-------------------------------------------------------------------------------------------------
// Variable

#define MIX_FREE_SD_4CENT		5

#endif
