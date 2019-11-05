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
	eVideoChannel v_type;		// H.264/5

	UINT un32vidicount;	// video i-frame count
	UINT un32vidscount;	// video total frame count
	UINT un32audscount;	// audio total frame count
	UINT un32txtscount;	// G-sensor, GPS, etc...

	UINT un32vidsmax;
	UINT un32audsmax;
	UINT un32txtsmax;
	UINT un32totalmax;
	UINT un32maxtime;

	uint16 vps_size;
	uint16 sps_size;
	uint16 pps_size;
	char vps_data[32];
	char sps_data[64];
	char pps_data[8];

	UINT mdat_size;
} t_mp4_t;

#if 1
#define vidswapSB32(x)	((((x) & (DWORD)0x000000ffUL) << 24) | \
						 (((x) & (DWORD)0x0000ff00UL) <<  8) | \
						 (((x) & (DWORD)0x00ff0000UL) >>  8) | \
						 (((x) & (DWORD)0xff000000UL) >> 24))
#define vidswapSB16(x)	((WORD)((((x) & (WORD)0x00ffU) << 8) | (((x) & (WORD)0xff00U) >> 8)))
#define FCC(ch1, ch2, ch3, ch4)	((DWORD)((ch4 << 24) | (ch3 << 16) | (ch2 << 8) | (ch1 << 0)))
#else
#define vidswapSB32(x)	x
#define vidswapSB16(x)	x
#define FCC(ch1, ch2, ch3, ch4)	((DWORD)((ch1 << 24) | (ch2 << 16) | (ch3 << 8) | (ch4 << 0)))
#endif

typedef DWORD FOURCC;

typedef struct {
	DWORD	cb;							// 구조체의 사이즈를 지정한다. 최초의 8 바이트분을 공제한 값을 지정한다.
	FOURCC	fcc;						// FOURCC 코드를 지정한다.	
} __attribute__ ((packed)) FCC_CB_t;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'stco' 가 아니면 안된다.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwEntryCount;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_CHUNK_OFFSET_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'stsz' 가 아니면 안된다.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwSampleSize;
		DWORD	dwEntryCount;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SAMPLE_SIZE_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'stss' 가 아니면 안된다.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwEntryCount;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SYNC_SAMPLE_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'stsc' 가 아니면 안된다.
	struct {
		DWORD	dwVersion_Flags;
		DWORD	dwEntryCount;
		DWORD	dwFirstChunk;
		DWORD	dwSamplePerChunk;
		DWORD	dwSampleDescriptionID;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SAMPLE_TO_CHUNK_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'stts' 가 아니면 안된다.
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
	FCC_CB_t fcccb;						// 값은 'avcC' 가 아니면 안된다.
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
	FCC_CB_t fcccb;						// 값은 'avc1' 가 아니면 안된다.
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

#if 0
	unsigned int(8) configurationVersion = 1;
	unsigned int(2) general_profile_space;
	unsigned int(1) general_tier_flag;
	unsigned int(5) general_profile_idc;
	unsigned int(32) general_profile_compatibility_flags;
	unsigned int(48) general_constraint_indicator_flags;
	unsigned int(8) general_level_idc;
	bit(4) reserved = ‘1111’b;
	unsigned int(12) min_spatial_segmentation_idc;
	bit(6) reserved = ‘111111’b;
	unsigned int(2) parallelismType;
	bit(6) reserved = ‘111111’b;
	unsigned int(2) chroma_format_idc;
	bit(5) reserved = ‘11111’b;
	unsigned int(3) bit_depth_luma_minus8;
	bit(5) reserved = ‘11111’b;
	unsigned int(3) bit_depth_chroma_minus8;
	bit(16) avgFrameRate;
	bit(2) constantFrameRate;
	bit(3) numTemporalLayers;
	bit(1) temporalIdNested;
	unsigned int(2) lengthSizeMinusOne;
	unsigned int(8) numOfArrays;
	for (j=0; j < numOfArrays; j++) {
		bit(1) array_completeness;
		unsigned int(1) reserved = 0;
		unsigned int(6) NAL_unit_type;
		unsigned int(16) numNalus;
		for (i=0; i< numNalus; i++) {
			unsigned int(16) nalUnitLength;
			bit(8*nalUnitLength) nalUnit;
		}
	}
#endif
typedef struct {
	FCC_CB_t fcccb;						// 값은 'hvcC' 가 아니면 안된다.
	struct {
		BYTE	bConfigureationVersion;
		BYTE	bGeneralProfile;		// profile_space(2), tier_flag(1), profile_idc(1)
		BYTE	bGeneralProfileCompatibilityFlags[4];
		BYTE	bGeneralConstraintIndicatorFlags[6];
		BYTE	bGeneralLevelIdc;
		WORD	wMinSpatialSegmentationIdc;	// 0xF000 | min_spatial_segmentation_idc
		BYTE	bParallelismType;			// 0xFC   | parallelismType
		BYTE	bChromaFormatIdc;			// 0xFC   | chroma_format_idc
		BYTE	bBitDepthLumaMinus8;		// 0xF8   | bit_depth_luma_minus8
		BYTE	bBitDepthChromaMinus8;		// 0xF8   | bit_depth_chroma_minus8
		WORD	wAvgFrameRate;
		BYTE	bTemporal;					// constantFrameRate(2), numTemporalLayers(3), temporalIdNested(1), lengthSizeMinusOne(2)
		BYTE	bNumOfArrays;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_HEVC_CONFIGURATION_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'hev1' 가 아니면 안된다. avc1과 동일한 구조로 보인다(추정)
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
} __attribute__ ((packed)) MP4_HVC1_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'twos' 가 아니면 안된다.
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
	FCC_CB_t fcccb;						// 값은 'stsd' 가 아니면 안된다.
	struct {
		DWORD dwVersion_Flags;
		DWORD dwEntryCount;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SAMPLE_DESCRIPTION_BOX;

typedef struct {
	FCC_CB_t fcccb; 					// 값은 'url ' 가 아니면 안된다.
	struct {
		DWORD dwVersion_Flags;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_DATA_ENTRY_URL_BOX;

typedef struct {
	FCC_CB_t fcccb; 					// 값은 'dref' 가 아니면 안된다.
	struct {
		DWORD dwVersion_Flags;
		DWORD dwEntry;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_DATA_REFERENCE_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'vmhd' 가 아니면 안된다.
	struct {
		DWORD	dwVersion_Flags;
		WORD	wGraphicsMode;
		WORD	wOpColor[3];
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_VIDEO_MEDIA_HEADER_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'smhd' 가 아니면 안된다.
	struct {
		DWORD	dwVersion_Flags;
		WORD	wBalance;
		WORD	_res1;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_SOUND_MEDIA_HEADER_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'hdlr' 가 아니면 안된다.
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
	FCC_CB_t fcccb;						// 값은 'mdhd' 가 아니면 안된다.
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
	FCC_CB_t fcccb; 					// 값은 'elst' 가 아니면 안된다.
	struct {
		DWORD dwVersion_Flags;
		DWORD dwEntries;
		DWORD dwTrackduration;
		DWORD dwMediatime;
		DWORD dwMediarate;
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) MP4_EDIT_LIST_BOX;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'tkhd' 가 아니면 안된다.
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
	FCC_CB_t fcccb;						// 값은 'mvhd' 가 아니면 안된다.
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
	FCC_CB_t fcccb;						// 값은 'ftyp' 가 아니면 안된다.
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
