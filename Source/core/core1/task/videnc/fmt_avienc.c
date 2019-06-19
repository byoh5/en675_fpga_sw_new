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

#if (LOAD_FS_SDCARD==1)
#if (VIDEO_SAVE_AVI==1)
#include "muxer_videnc.h"
#include "fmt_avienc.h"
#include "videnc_info.h"

/*******************************************************************************
AVI File Format
RIFF ('AVI '
      LIST ('hdrl'
            'avih'(<Main AVI Header>)
            LIST ('strl'
                  'strh'(<Stream header>)
                  'strf'(<Stream format>)
                  [ 'strd'(<Additional header data>) ]
                  [ 'strn'(<Stream name>) ]
                  ...
                 )
             ...
           )
      LIST ('movi'
            {SubChunk | LIST ('rec '
                              SubChunk1
                              SubChunk2
                              ...
                             )
               ...
            }
            ...
           )
      ['idx1' (<AVI Index>) ]
     )
*******************************************************************************/

FCC_CB_t avi_write_chunkheader(UINT type, UINT data_size)
{
	FCC_CB_t avi_chunk;
	switch(type)
	{
		case H264_STREAM:
			avi_chunk.fcc = FCC('0','0','d','c');
			break;
#if (FAT_SDSAVE_AUD==1)
		case G711_STREAM:
			avi_chunk.fcc = FCC('0','1','w','b');
			break;
#endif
#if (FAT_SDSAVE_TXT==1)
		case TXTS_STREAM:
			avi_chunk.fcc = FCC('0','2','t','x');
			break;
#endif
		case IDX1_ARRAY:
			avi_chunk.fcc = FCC('i','d','x','1');
			break;
		default:
			printf("%s %d : Not Supported(type:%d)\r\n", type);
			break;
	}
	avi_chunk.cb = NUM32(data_size);
	return avi_chunk;
}

AVIINDEX_t avi_write_index(UINT type, UINT data_pos, UINT data_size, UINT key)
{
	AVIINDEX_t avi_index;
	switch(type)
	{
		case H264_STREAM:
			avi_index.dwChunkId = FCC('0','0','d','c');
			avi_index.dwFlags = NUM32(key ? AVIIF_KEYFRAME : 0);
			break;
#if (FAT_SDSAVE_AUD==1)
		case G711_STREAM:
			avi_index.dwChunkId = FCC('0','1','w','b');
			avi_index.dwFlags = NUM32(AVIIF_KEYFRAME);
			break;
#endif
#if (FAT_SDSAVE_TXT==1)
		case TXTS_STREAM:
			avi_index.dwChunkId = FCC('0','2','t','x');
			avi_index.dwFlags = NUM32(AVIIF_LIST);
			break;
#endif
		default:
			printf("%s %d : Not Supported(type:%d)\r\n", type);
			break;
	}
	avi_index.dwOffset = NUM32(data_pos);
	avi_index.dwSize = NUM32(data_size);
	return avi_index;
}

static BYTE avi_header[AVI_HEADSIZE];
static int avi_header_index = 0;

/*
This function builds the following:

RIFF ('AVI '
      LIST ('hdrl'
            'avih'(<Main AVI Header>)

*/
int avi_write_header_build_head(BYTE *data, t_avi_t *avia)
{
	AVI_HEADER_t avi_head;
	UINT byte_per_sec = 0;

	// size : 20byte
	avi_head.head.fcccb.fcc 					 = FCC('R','I','F','F');
	avi_head.head.fcccb.cb						 = NUM32((avia->i_riff > 0 ? (avia->i_riff - 8) : 0xFFFFFFFF));
	avi_head.head.fccListType					 = FCC('A','V','I',' ');
	avi_head.list.fcccb.fcc						 = FCC('L','I','S','T');

  /*
    One or more 'strl' lists follow the main header. A 'strl' list is required 
    for each data stream. Each 'strl' list contains information about one stream 
    in the file, and must contain a stream header chunk ('strh') and a stream 
    format chunk ('strf'). In addition, a 'strl' list might contain a stream-
    header data chunk ('strd') and a stream name chunk ('strn').
  */

	if(avia->option == (VID_VIDEO | VID_AUDIO | VID_TXTS))
		avi_head.list.fcccb.cb 					 = NUM32((4 + sizeof(AVIMAINHEADER) + sizeof(avi_videostream_t) + sizeof(avi_audiostream_t) + sizeof(avi_accscstream_t) + sizeof(FCC_CB_t) + JUNKSIZE_VAT));
	else if(avia->option == (VID_VIDEO | VID_AUDIO))
		avi_head.list.fcccb.cb 					 = NUM32((4 + sizeof(AVIMAINHEADER) + sizeof(avi_videostream_t) + sizeof(avi_audiostream_t) + sizeof(FCC_CB_t) + JUNKSIZE_VA));
	else if(avia->option == (VID_VIDEO | VID_TXTS))
		avi_head.list.fcccb.cb 					 = NUM32((4 + sizeof(AVIMAINHEADER) + sizeof(avi_videostream_t) + sizeof(avi_accscstream_t) + sizeof(FCC_CB_t) + JUNKSIZE_VT));
	else if(avia->option == (VID_VIDEO))
		avi_head.list.fcccb.cb 					 = NUM32((4 + sizeof(AVIMAINHEADER) + sizeof(avi_videostream_t) + sizeof(FCC_CB_t) + JUNKSIZE_V));

	// size : avi_head.main.list.fcccb.cb
	avi_head.list.fccListType					 = FCC('h','d','r','l');

	//build avi main header, 'avih' chunk
	avi_head.data.fcccb.fcc						 = FCC('a','v','i','h');
	avi_head.data.fcccb.cb 						 = NUM32(sizeof(avi_head.data.mhData));
	avi_head.data.mhData.dwMicroSecPerFrame		 = NUM32(((int)(1000000/avia->f_fps)));

	if(avia->un32vidscount)
	{
		byte_per_sec = avia->i_riff / (avia->un32vidscount / avia->f_fps);
	}
	avi_head.data.mhData.dwMaxBytesPerSec		 = NUM32(byte_per_sec);
	
	avi_head.data.mhData.dwPaddingGranularity	 = 0;
	avi_head.data.mhData.dwFlags				 = NUM32((AVIF_HASINDEX | AVIF_ISINTERLEAVED | AVIF_TRUSTCKTYPE));
	avi_head.data.mhData.dwTotalFrames 			 = NUM32(avia->un32vidscount);
	avi_head.data.mhData.dwInitialFrames		 = 0;

	if(avia->option == (VID_VIDEO | VID_AUDIO | VID_TXTS))
		avi_head.data.mhData.dwStreams 			 = NUM32(3);
	else if(avia->option == (VID_VIDEO | VID_AUDIO))
		avi_head.data.mhData.dwStreams 			 = NUM32(2);
	else if(avia->option == (VID_VIDEO | VID_TXTS))
		avi_head.data.mhData.dwStreams 			 = NUM32(2);
	else if(avia->option == (VID_VIDEO))
		avi_head.data.mhData.dwStreams 			 = NUM32(1);

	avi_head.data.mhData.dwSuggestedBufferSize	 = NUM32(1000000);
	avi_head.data.mhData.dwWidth				 = NUM32(avia->i_width);
	avi_head.data.mhData.dwHeight				 = NUM32(avia->i_height);
	avi_head.data.mhData.dwReserved[0] 			 = 0;
	avi_head.data.mhData.dwReserved[1] 			 = 0;
	avi_head.data.mhData.dwReserved[2] 			 = 0;
	avi_head.data.mhData.dwReserved[3] 			 = 0;

	memcpy((uint8 *)&(data[avi_header_index]), (uint8 *)&avi_head, sizeof(AVI_HEADER_t));
	avi_header_index += sizeof(AVI_HEADER_t);
	return sizeof(AVI_HEADER_t);
}

int avi_write_header_build_video(BYTE *data, t_avi_t *avia)
{
	avi_videostream_t video;

	video.list.fcccb.fcc						 = FCC('L','I','S','T');
	video.list.fcccb.cb							 = NUM32((4 + sizeof(video.stream) + sizeof(video.info)));
	video.list.fccListType						 = FCC('s','t','r','l');

	video.stream.fcccb.fcc						 = FCC('s','t','r','h');
	video.stream.fcccb.cb 						 = NUM32(sizeof(video.stream.shData));
	video.stream.shData.fccType					 = FCC('v','i','d','s');
	video.stream.shData.fccHandler				 = FCC('H','2','6','4');
	video.stream.shData.dwFlags					 = 0;
	video.stream.shData.wPriority 				 = 0;
	video.stream.shData.wLanguage 				 = 0;
	video.stream.shData.dwInitialFrames			 = 0;
	video.stream.shData.dwScale					 = NUM32(1);
	video.stream.shData.dwRate					 = NUM32(avia->f_fps);
	video.stream.shData.dwStart					 = 0;
	video.stream.shData.dwLength				 = NUM32(avia->un32vidscount);
	video.stream.shData.dwSuggestedBufferSize 	 = NUM32(0x80000);
	video.stream.shData.dwQuality 				 = NUM32(0xFFFFFFFF);
	video.stream.shData.dwSampleSize			 = 0;
	video.stream.shData.rcFrame.left			 = 0;
	video.stream.shData.rcFrame.top				 = 0;
	video.stream.shData.rcFrame.right 			 = NUM16(avia->i_width);
	video.stream.shData.rcFrame.bottom			 = NUM16(avia->i_height);

	video.info.fcccb.fcc						 = FCC('s','t','r','f');
	video.info.fcccb.cb							 = NUM32(sizeof(video.info.bmData));
	video.info.bmData.biSize					 = NUM32(sizeof(video.info.bmData));
	video.info.bmData.biWidth 					 = NUM32(avia->i_width);
	video.info.bmData.biHeight					 = NUM32(avia->i_height);
	video.info.bmData.biPlanes					 = NUM16(1);
	video.info.bmData.biBitCount				 = NUM16(24);
	video.info.bmData.biCompression				 = FCC('H','2','6','4');
	video.info.bmData.biSizeImage 				 = NUM32((avia->i_width * avia->i_height));
	video.info.bmData.biXPelsPerMeter 			 = 0;
	video.info.bmData.biYPelsPerMeter 			 = 0;
	video.info.bmData.biClrUsed					 = 0;
	video.info.bmData.biClrImportant			 = 0;

	memcpy((uint8 *)&(data[avi_header_index]), (uint8 *)&video, sizeof(avi_videostream_t));
	avi_header_index += sizeof(avi_videostream_t);
	return sizeof(avi_videostream_t);
}

#if (FAT_SDSAVE_AUD==1)
int avi_write_header_build_audio(BYTE *data, t_avi_t *avia)
{
	avi_audiostream_t audio;

	audio.list.fcccb.fcc					 = FCC('L','I','S','T');
	audio.list.fcccb.cb 					 = NUM32((4 + sizeof(audio.stream) + sizeof(audio.info)));
	audio.list.fccListType					 = FCC('s','t','r','l');

	audio.stream.fcccb.fcc					 = FCC('s','t','r','h');
	audio.stream.fcccb.cb					 = NUM32(sizeof(audio.stream.shData));
	audio.stream.shData.fccType 			 = FCC('a','u','d','s');
	audio.stream.shData.fccHandler			 = NUM32(0x00000001);
	audio.stream.shData.dwFlags 			 = 0;
	audio.stream.shData.wPriority			 = 0;
	audio.stream.shData.wLanguage			 = 0;
	audio.stream.shData.dwInitialFrames 	 = 0;
	audio.stream.shData.dwScale 			 = NUM32(1);
	audio.stream.shData.dwRate				 = NUM32(AUD_SAMPLING_RATE);
	audio.stream.shData.dwStart 			 = 0;
#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
	audio.stream.shData.dwLength			 = NUM32((avia->un32audscount*800)>>1);
#else
	audio.stream.shData.dwLength			 = NUM32((AUD_SAMPLING_RATE*(avia->un32audscount/G711_RecordingHz)));
#endif
	audio.stream.shData.dwSuggestedBufferSize= NUM32(AUD_SAMPLING_RATE*(AUD_SAMPLING_BIT/8));
	audio.stream.shData.dwQuality			 = NUM32(0xFFFFFFFF);
	audio.stream.shData.dwSampleSize		 = NUM32((AUD_SAMPLING_BIT/8));
	audio.stream.shData.rcFrame.left		 = 0;
	audio.stream.shData.rcFrame.top 		 = 0;
	audio.stream.shData.rcFrame.right		 = 0;
	audio.stream.shData.rcFrame.bottom		 = 0;

	audio.info.fcccb.fcc					 = FCC('s','t','r','f');
	audio.info.fcccb.cb 					 = NUM32(sizeof(audio.info.wmData));
#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
	audio.info.wmData.wFormatTag			 = NUM16(WAVE_FORMAT_PCM);
#elif (AUDIO_CODEC==AUDIO_CODEC_G711A)
	audio.info.wmData.wFormatTag			 = NUM16(WAVE_FORMAT_ALAW);
#elif (AUDIO_CODEC==AUDIO_CODEC_G711U)
	audio.info.wmData.wFormatTag			 = NUM16(WAVE_FORMAT_MULAW);
#endif
	audio.info.wmData.nChannels 			 = NUM16(1);
	audio.info.wmData.nSamplesPerSec		 = NUM32(AUD_SAMPLING_RATE);
	audio.info.wmData.nAvgBytesPerSec		 = NUM32(AUD_SAMPLING_RATE*(AUD_SAMPLING_BIT/8));
	audio.info.wmData.nBlockAlign			 = NUM16((AUD_SAMPLING_BIT/8));
	audio.info.wmData.wBitsPerSample		 = NUM16(AUD_SAMPLING_BIT);
	audio.info.wmData.cbSize				 = 0;

	memcpy((uint8 *)&(data[avi_header_index]), (uint8 *)&audio, sizeof(avi_audiostream_t));
	avi_header_index += sizeof(avi_audiostream_t);
	return sizeof(avi_audiostream_t);
}
#endif

#if (FAT_SDSAVE_TXT==1)
int avi_write_header_build_txts(BYTE *data, t_avi_t *avia)
{
	avi_accscstream_t txts;
	
	txts.list.fcccb.fcc 					 = FCC('L','I','S','T');
	txts.list.fcccb.cb						 = NUM32((4 + sizeof(txts.stream) + sizeof(txts.info)));
	txts.list.fccListType					 = FCC('s','t','r','l');
	
	txts.stream.fcccb.fcc					 = FCC('s','t','r','h');
	txts.stream.fcccb.cb					 = NUM32(sizeof(txts.stream.shData));
	txts.stream.shData.fccType				 = FCC('t','x','t','s');
	txts.stream.shData.fccHandler			 = 0;				// or FCC('XBLB');
	txts.stream.shData.dwFlags				 = 0;
	txts.stream.shData.wPriority			 = 0;
	txts.stream.shData.wLanguage			 = 0;
	txts.stream.shData.dwInitialFrames		 = 0;
	txts.stream.shData.dwScale				 = NUM32(0x64); // NUM32(0x3E8);	// 1000 	// DEC : 1000	// or 100
	txts.stream.shData.dwRate				 = 0;			//NUM32(0x2710);	// 10000	// DEC : 9605	// or 0
	txts.stream.shData.dwStart				 = 0;
	txts.stream.shData.dwLength 			 = NUM32(avia->un32txtscount);	// 데이터의 갯수(movi에 포함된 02tx의 갯수)
	txts.stream.shData.dwSuggestedBufferSize = NUM32(0x70);				// 데이터의 길이(02tx와 size를 제외한 최대크기)
	txts.stream.shData.dwQuality			 = NUM32(0xFFFFFFFF);
	txts.stream.shData.dwSampleSize 		 = 0;
	txts.stream.shData.rcFrame.left 		 = 0;
	txts.stream.shData.rcFrame.top			 = 0;
	txts.stream.shData.rcFrame.right		 = 0;
	txts.stream.shData.rcFrame.bottom		 = 0;
	
	txts.info.fcccb.fcc 					 = FCC('s','t','r','f');
	txts.info.fcccb.cb						 = 0;

	memcpy((uint8 *)&(data[avi_header_index]), (uint8 *)&txts, sizeof(avi_accscstream_t));
	avi_header_index += sizeof(avi_accscstream_t);
	return sizeof(avi_accscstream_t);
}
#endif

int avi_write_header_build_junk(BYTE *data, t_avi_t *avia)
{
	uint32 size = 0;
	FCC_CB_t junk;

	junk.fcc								 = FCC('J','U','N','K');
	if(avia->option == (VID_VIDEO | VID_AUDIO | VID_TXTS)) {
		junk.cb								 = NUM32(JUNKSIZE_VAT);
		size = JUNKSIZE_VAT;
	} else if(avia->option == (VID_VIDEO | VID_AUDIO)) {
		junk.cb								 = NUM32(JUNKSIZE_VA);
		size = JUNKSIZE_VA;
	} else if(avia->option == (VID_VIDEO | VID_TXTS)) {
		junk.cb								 = NUM32(JUNKSIZE_VT);
		size = JUNKSIZE_VT;
	} else if(avia->option == (VID_VIDEO)) {
		junk.cb								 = NUM32(JUNKSIZE_V);
		size = JUNKSIZE_V;
	}

	memcpy((uint8 *)&(data[avi_header_index]), (uint8 *)&junk, sizeof(FCC_CB_t));
	avi_header_index += (sizeof(FCC_CB_t) + size);
	return (sizeof(FCC_CB_t) + size);
}

int avi_write_header_build_tail(BYTE *data, t_avi_t *avia)
{
	RIFFLIST tail;

	tail.fcccb.fcc							 = FCC('L','I','S','T');
	tail.fcccb.cb							 = NUM32(avia->i_movi_end > 0 ? (avia->i_movi_end - avia->i_header_end) : 0xFFFFFFFF);
	tail.fccListType						 = FCC('m','o','v','i');

	memcpy((uint8 *)&(data[avi_header_index]), (uint8 *)&tail, sizeof(tail));
	avi_header_index += sizeof(tail);
	return sizeof(tail);
}

/*
This function builds the followings:

RIFF ('AVI '
      LIST ('hdrl'
            'avih'(<Main AVI Header>)
            LIST ('strl'
                  'strh'(<Stream header>)
                  'strf'(<Stream format>)
                  [ 'strd'(<Additional header data>) ]
                  [ 'strn'(<Stream name>) ]
                  ...
                 )
             ...
           )
      LIST ('movi'
*/
BYTE *avi_write_header(t_avi_t *avia)
{
	DmaMemSet_ip(avi_header, 0, sizeof(avi_header));
	invalidate_dcache_range((uint)avi_header, (uint)(avi_header+sizeof(avi_header)));
	avi_header_index = 0;

	avi_write_header_build_head(avi_header, avia);
	if(avia->option & VID_VIDEO)
		avi_write_header_build_video(avi_header, avia);
#if (FAT_SDSAVE_AUD==1)
	if(avia->option & VID_AUDIO)
		avi_write_header_build_audio(avi_header, avia);
#endif
#if (FAT_SDSAVE_TXT==1)
	if(avia->option & VID_TXTS)
		avi_write_header_build_txts(avi_header, avia);
#endif
	avi_write_header_build_junk(avi_header, avia);
	avi_write_header_build_tail(avi_header, avia);

	return avi_header;
}
#endif
#endif
