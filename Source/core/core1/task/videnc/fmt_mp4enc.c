/* Copyright (c) 2018- Eyenix Corporation. All rights reserved.
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
#if (VIDEO_SAVE_AVI==0)
#include "muxer_videnc.h"
#include "fmt_mp4enc.h"
#include "videnc_info.h"

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

/**************************************************************************
 구조체의 Data align 문제로 값생성은 local variable 으로 처리하고,
 DMA을 통해 data array에 복사하는 형태로 처리됨.

 (이전의 형태는 data array에 바로 pointer로 참조하였으나, 
 영상의 종류에 따라 Data align 문제가 간혹 발생함.)
 (모든 데이터를 1byte단위로 처리하면 기존의 방식으로도 해결 됨)
**************************************************************************/

typedef struct {
	UINT m_dwTimescale;
	UINT v_dwSampleCount;
	UINT v_dwDuration;
	UINT a_dwSampleCount;
	UINT a_dwDuration;

	t_mp4_t *mp4a;
	t_videnc_totalbuf_t *but_t;
	MP4HeaderSTBL *Vstbl;
	MP4HeaderSTBL *Astbl;
} MP4Parameter;

static MP4Parameter mp4para;

void mp4_init_header_build(t_mp4_t *mp4a, t_videnc_totalbuf_t *but_t)
{
	mp4para.mp4a = mp4a;
	mp4para.but_t = but_t;
	mp4para.Vstbl = &(but_t->Vstbl);
	mp4para.Astbl = &(but_t->Astbl);

	mp4para.m_dwTimescale = 1000;
	mp4para.v_dwSampleCount = mp4a->un32vidscount;
	mp4para.a_dwSampleCount = mp4a->un32audscount;
	mp4para.v_dwDuration = mp4para.m_dwTimescale * mp4para.v_dwSampleCount / mp4para.mp4a->f_fps;
	mp4para.a_dwDuration = mp4para.m_dwTimescale * mp4para.a_dwSampleCount / PCM8K_RecordingHz;

	if (mp4para.Vstbl->stsz_len != mp4para.v_dwSampleCount) {
		flprintf("Error sample count(%d/%d)\r\n", mp4para.Vstbl->stsz_len, mp4para.v_dwSampleCount);
	}
}

UINT enx_mp4_header_ftyp(BYTE *data)
{
	MP4_FILE_TYPE_BOX *ftyp = (MP4_FILE_TYPE_BOX *)data;

	ftyp->mhData.fcc1				= FCC('i','s','o','m');
	ftyp->mhData.fcc2				= 0x00000200;
	ftyp->mhData.fcc3				= FCC('i','s','o','m');
	ftyp->mhData.fcc4				= FCC('i','s','o','2');
	ftyp->mhData.fcc5				= FCC('a','v','c','1');
	ftyp->mhData.fcc6				= FCC('m','p','4','1');

	ftyp->fcccb.fcc = FCC('f','t','y','p');
	ftyp->fcccb.cb  = sizeof(MP4_FILE_TYPE_BOX);

	return ftyp->fcccb.cb;
}

UINT enx_mp4_header_mvhd(BYTE *data)
{
	MP4_MOVIE_HEADER_BOX *mvhd = (MP4_MOVIE_HEADER_BOX *)data;

	mvhd->mhData.dwVersion_Flags	= 0;
	mvhd->mhData.dwCreationtime		= gptMsgShare.TIME + 0x7C25B080;
	mvhd->mhData.dwModificationtime	= gptMsgShare.TIME + 0x7C25B080;
	mvhd->mhData.dwTimescale		= mp4para.m_dwTimescale;
	mvhd->mhData.dwRate				= 0x00010000;
	mvhd->mhData.wVolume			= 0x0100;
	mvhd->mhData._res1				= 0;
	mvhd->mhData._res2				= 0;
	mvhd->mhData._res3				= 0;
	mvhd->mhData.dwMatrix[0]		= 0x00010000;
	mvhd->mhData.dwMatrix[1]		= 0;
	mvhd->mhData.dwMatrix[2]		= 0;
	mvhd->mhData.dwMatrix[3]		= 0;
	mvhd->mhData.dwMatrix[4]		= 0x00010000;
	mvhd->mhData.dwMatrix[5]		= 0;
	mvhd->mhData.dwMatrix[6]		= 0;
	mvhd->mhData.dwMatrix[7]		= 0;
	mvhd->mhData.dwMatrix[8]		= 0x40000000;
	mvhd->mhData.dwPreDefined[0]	= 0;
	mvhd->mhData.dwPreDefined[1]	= 0;
	mvhd->mhData.dwPreDefined[2]	= 0;
	mvhd->mhData.dwPreDefined[3]	= 0;
	mvhd->mhData.dwPreDefined[4]	= 0;
	mvhd->mhData.dwPreDefined[5]	= 0;
	if (mp4para.mp4a->option & VID_AUDIO) {
		mvhd->mhData.dwDuration		= mp4para.v_dwDuration > mp4para.a_dwDuration ? mp4para.v_dwDuration : mp4para.a_dwDuration;
		mvhd->mhData.dwNexttrakid	= 3;
	} else {
		mvhd->mhData.dwDuration		= mp4para.v_dwDuration;
		mvhd->mhData.dwNexttrakid	= 2;
	}

	mvhd->fcccb.fcc = FCC('m','v','h','d');
	mvhd->fcccb.cb  = sizeof(MP4_MOVIE_HEADER_BOX);

	return mvhd->fcccb.cb;
}

UINT enx_mp4_header_tkhd(BYTE *data, UINT vid_type)
{
	MP4_TRACK_HEADER_BOX *tkhd = (MP4_TRACK_HEADER_BOX *)data;

	tkhd->mhData.dwVersion_Flags	= 0x0000000F;
	tkhd->mhData.dwCreationtime		= gptMsgShare.TIME + 0x7C25B080;
	tkhd->mhData.dwModificationtime	= gptMsgShare.TIME + 0x7C25B080;
	tkhd->mhData._res1				= 0;
	tkhd->mhData._res2				= 0;
	tkhd->mhData._res3				= 0;
	tkhd->mhData.wLayer				= 0;
	tkhd->mhData.wAlternateGroup	= 0;
	tkhd->mhData._res4				= 0;
	tkhd->mhData.dwMatrix[0]		= 0x00010000;
	tkhd->mhData.dwMatrix[1]		= 0x0;
	tkhd->mhData.dwMatrix[2]		= 0x0;
	tkhd->mhData.dwMatrix[3]		= 0x0;
	tkhd->mhData.dwMatrix[4]		= 0x00010000;
	tkhd->mhData.dwMatrix[5]		= 0x0;
	tkhd->mhData.dwMatrix[6]		= 0x0;
	tkhd->mhData.dwMatrix[7]		= 0x0;
	tkhd->mhData.dwMatrix[8]		= 0x40000000;
	if (vid_type == VID_VIDEO) {
		tkhd->mhData.dwTrackId		= 1;
		tkhd->mhData.dwDuration	 	= mp4para.v_dwDuration;
		tkhd->mhData.wVolume		= 0x0;
		tkhd->mhData.dwWidth		= mp4para.mp4a->i_width << 16;
		tkhd->mhData.dwHeight		= mp4para.mp4a->i_height << 16;
	} else if (vid_type == VID_AUDIO) {
		tkhd->mhData.dwTrackId		= 2;
		tkhd->mhData.dwDuration 	= mp4para.a_dwDuration;
		tkhd->mhData.wVolume		= 0x0100;
		tkhd->mhData.dwWidth		= 0;
		tkhd->mhData.dwHeight		= 0;
	}

	tkhd->fcccb.fcc = FCC('t','k','h','d');
	tkhd->fcccb.cb  = sizeof(MP4_TRACK_HEADER_BOX);

	return tkhd->fcccb.cb;
}

UINT enx_mp4_header_elst(BYTE *data, UINT vid_type)
{
	MP4_EDIT_LIST_BOX *elst = (MP4_EDIT_LIST_BOX *)data;

	elst->mhData.dwVersion_Flags	= 0;
	elst->mhData.dwEntries			= 1;
	if (vid_type == VID_VIDEO) {
		elst->mhData.dwTrackduration= mp4para.v_dwDuration;
	} else if (vid_type == VID_AUDIO) {
		elst->mhData.dwTrackduration= mp4para.a_dwDuration;
	}
	elst->mhData.dwMediatime		= 0;
	elst->mhData.dwMediarate		= 0x00010000;

	elst->fcccb.fcc = FCC('e','l','s','t');
	elst->fcccb.cb  = sizeof(MP4_EDIT_LIST_BOX);

	return elst->fcccb.cb;
}

UINT enx_mp4_header_edts(BYTE *data, UINT vid_type)
{
	FCC_CB_t *edts = (FCC_CB_t *)data;

	edts->fcc = FCC('e','d','t','s');
	edts->cb  = sizeof(FCC_CB_t);
	edts->cb += enx_mp4_header_elst(data + edts->cb, vid_type);

	return edts->cb;
}

UINT enx_mp4_header_mdhd(BYTE *data, UINT vid_type)
{
	MP4_MEDIA_HEADER_BOX *mdhd = (MP4_MEDIA_HEADER_BOX *)data;

	mdhd->mhData.dwVersion_Flags	= 0;
	mdhd->mhData.dwCreationtime		= gptMsgShare.TIME + 0x7C25B080;
	mdhd->mhData.dwModificationtime	= gptMsgShare.TIME + 0x7C25B080;
	if (vid_type == VID_VIDEO) {
		mdhd->mhData.dwTimescale	= mp4para.m_dwTimescale * mp4para.mp4a->f_fps;
		mdhd->mhData.dwDuration		= mp4para.m_dwTimescale * mp4para.v_dwSampleCount;
	} else if (vid_type == VID_AUDIO) {
		mdhd->mhData.dwTimescale	= AUD_SAMPLING_RATE;
		mdhd->mhData.dwDuration		= mp4para.a_dwSampleCount * ((AUD_SAMPLING_RATE * (AUD_SAMPLING_BIT / 8)) / PCM8K_RecordingHz) / (AUD_SAMPLING_BIT / 8);
	}
	mdhd->mhData.wLanguage			= 0x55C4; // Pad(1) + Language(15)
	mdhd->mhData.wPreDefined		= 0;

	mdhd->fcccb.fcc = FCC('m','d','h','d');
	mdhd->fcccb.cb  = sizeof(MP4_MEDIA_HEADER_BOX);

	return mdhd->fcccb.cb;
}

UINT enx_mp4_header_hdlr(BYTE *data, UINT vid_type)
{
	MP4_HANDLER_REFERENCE_BOX *hdlr = (MP4_HANDLER_REFERENCE_BOX *)data;

	hdlr->mhData.dwVersion_Flags	= 0;
	hdlr->mhData.dwPreDefined		= 0;
	hdlr->mhData._res1 				= 0;
	hdlr->mhData._res2				= 0;
	hdlr->mhData._res3				= 0;
	if (vid_type == VID_VIDEO) {
		hdlr->mhData.dwSubtype		= FCC('v','i','d','e');
		hdlr->mhData.dwName[0]		= FCC('V','i','d','e');
		hdlr->mhData.dwName[1]		= FCC('o','H','a','n');
		hdlr->mhData.dwName[2]		= FCC('d','l','e','r');
	} else if (vid_type == VID_AUDIO) {
		hdlr->mhData.dwSubtype		= FCC('s','o','u','n');
		hdlr->mhData.dwName[0]		= FCC('S','o','u','n');
		hdlr->mhData.dwName[1]		= FCC('d','H','a','n');
		hdlr->mhData.dwName[2]		= FCC('d','l','e','r');
	}
	hdlr->mhData.bNull				= 0;

	hdlr->fcccb.fcc = FCC('h','d','l','r');
	hdlr->fcccb.cb  = sizeof(MP4_HANDLER_REFERENCE_BOX);

	return hdlr->fcccb.cb;
}

UINT enx_mp4_header_vmhd(BYTE *data)
{
	MP4_VIDEO_MEDIA_HEADER_BOX *vmhd = (MP4_VIDEO_MEDIA_HEADER_BOX *)data;

	vmhd->mhData.dwVersion_Flags	= 1;
	vmhd->mhData.wGraphicsMode		= 0;
	vmhd->mhData.wOpColor[0]		= 0;
	vmhd->mhData.wOpColor[1]		= 0;
	vmhd->mhData.wOpColor[2]		= 0;

	vmhd->fcccb.fcc = FCC('v','m','h','d');
	vmhd->fcccb.cb  = sizeof(MP4_VIDEO_MEDIA_HEADER_BOX);

	return vmhd->fcccb.cb;
}

UINT enx_mp4_header_smhd(BYTE *data)
{
	MP4_SOUND_MEDIA_HEADER_BOX *smhd = (MP4_SOUND_MEDIA_HEADER_BOX *)data;

	smhd->mhData.dwVersion_Flags	= 0;
	smhd->mhData.wBalance			= 0;
	smhd->mhData._res1				= 0;

	smhd->fcccb.fcc = FCC('s','m','h','d');
	smhd->fcccb.cb  = sizeof(MP4_SOUND_MEDIA_HEADER_BOX);

	return smhd->fcccb.cb;
}

UINT enx_mp4_header_url(BYTE *data, UINT vid_type)
{
	MP4_DATA_ENTRY_URL_BOX *url = (MP4_DATA_ENTRY_URL_BOX *)data;

	url->mhData.dwVersion_Flags		= 1;

	url->fcccb.fcc = FCC('u','r','l',' ');
	url->fcccb.cb  = sizeof(MP4_DATA_ENTRY_URL_BOX);

	return url->fcccb.cb;
	UNUSED(vid_type);
}

UINT enx_mp4_header_dref(BYTE *data, UINT vid_type)
{
	MP4_DATA_REFERENCE_BOX *dref = (MP4_DATA_REFERENCE_BOX *)data;

	dref->mhData.dwVersion_Flags	= 0;
	dref->mhData.dwEntry			= 1;

	dref->fcccb.fcc = FCC('d','r','e','f');
	dref->fcccb.cb  = sizeof(MP4_DATA_REFERENCE_BOX);
	dref->fcccb.cb += enx_mp4_header_url(data + dref->fcccb.cb, vid_type);

	return dref->fcccb.cb;
}

UINT enx_mp4_header_dinf(BYTE *data, UINT vid_type)
{
	FCC_CB_t *dinf = (FCC_CB_t *)data;

	dinf->fcc = FCC('d','i','n','f');
	dinf->cb  = sizeof(FCC_CB_t);
	dinf->cb += enx_mp4_header_dref(data + dinf->cb, vid_type);

	return dinf->cb;
}

UINT enx_mp4_header_avcc(BYTE *data)
{
	MP4_AVC_CONFIGURATION_BOX *avcc = (MP4_AVC_CONFIGURATION_BOX *)data;
	UINT i = 0, j = 0;
	BYTE *array = data + sizeof(MP4_AVC_CONFIGURATION_BOX);

	avcc->mhData.bConfigureationVersion	= 0x01;
	avcc->mhData.bAVCProfile			= mp4para.mp4a->sps_data[1];
	avcc->mhData.bProfileCompatibility	= mp4para.mp4a->sps_data[2];
	avcc->mhData.bAVCLevelIndication	= mp4para.mp4a->sps_data[3];
	avcc->mhData.bLengthSizeMinusOne	= 0xFF;

	// Reserved(3bit/111B) NumOfSPS(5bit/00001B)
	array[i++] = 0xE1;

	// SPS Length(16bit)
	array[i++] = (mp4para.mp4a->sps_size >> 8) & 0xFF;
	array[i++] = (mp4para.mp4a->sps_size     ) & 0xFF;

	// SPS NAL
	for (j = 0; j < mp4para.mp4a->sps_size; j++) {
		array[i++] = mp4para.mp4a->sps_data[j];
	}

	// NumOfPPS(8bit/00000001B)
	array[i++] = 0x01;

	// PPS Length(16bit)
	array[i++] = (mp4para.mp4a->pps_size >> 8) & 0xFF;
	array[i++] = (mp4para.mp4a->pps_size     ) & 0xFF;

	// PPS NAL
	for (j = 0; j < mp4para.mp4a->pps_size; j++) {
		array[i++] = mp4para.mp4a->pps_data[j];
	}

	avcc->fcccb.fcc = FCC('a','v','c','C');
	avcc->fcccb.cb  = sizeof(MP4_AVC_CONFIGURATION_BOX) + i;

	return avcc->fcccb.cb;
}

UINT enx_mp4_header_avc1(BYTE *data)
{
	MP4_AVC1_BOX *avc1 = (MP4_AVC1_BOX *)data;

	avc1->mhData.bReserved[0]			= 0;
	avc1->mhData.bReserved[1]			= 0;
	avc1->mhData.bReserved[2]			= 0;
	avc1->mhData.bReserved[3]			= 0;
	avc1->mhData.bReserved[4]			= 0;
	avc1->mhData.bReserved[5]			= 0;
	avc1->mhData.wDataRefIndex			= 1;
	avc1->mhData.dwPreDefined[0]		= 0;
	avc1->mhData.dwPreDefined[1]		= 0;
	avc1->mhData.dwPreDefined[2]		= 0;
	avc1->mhData.dwPreDefined[3]		= 0;
	avc1->mhData.wWidth					= (WORD)mp4para.mp4a->i_width;
	avc1->mhData.wHeight				= (WORD)mp4para.mp4a->i_height;
	avc1->mhData.dwHorizResolution		= 0x00480000;
	avc1->mhData.dwVertiResolution		= 0x00480000;
	avc1->mhData.dwDatasize				= 0;
	avc1->mhData.wFrameCount			= 1;
	avc1->mhData.dwCompressorName[0]	= 0;
	avc1->mhData.dwCompressorName[1]	= 0;
	avc1->mhData.dwCompressorName[2]	= 0;
	avc1->mhData.dwCompressorName[3]	= 0;
	avc1->mhData.dwCompressorName[4]	= 0;
	avc1->mhData.dwCompressorName[5]	= 0;
	avc1->mhData.dwCompressorName[6]	= 0;
	avc1->mhData.dwCompressorName[7]	= 0;
	avc1->mhData.wDepth					= 24;
	avc1->mhData.wPreDefined2			= -1;

	avc1->fcccb.fcc = FCC('a','v','c','1');
	avc1->fcccb.cb  = sizeof(MP4_AVC1_BOX);
	avc1->fcccb.cb += enx_mp4_header_avcc(data + avc1->fcccb.cb);

	return avc1->fcccb.cb;
}

UINT enx_mp4_header_twos(BYTE *data)
{
	MP4_TWOS_BOX *twos = (MP4_TWOS_BOX *)data;

	twos->mhData.dwReserved[0]		= 0;
	twos->mhData.dwReserved[1]		= 1;							// ?????
	twos->mhData.dwReserved[2]		= 0;
	twos->mhData.dwReserved[3]		= 0;
	twos->mhData.wNumberOfChannels	= 1;
	twos->mhData.wSampleSize		= AUD_SAMPLING_BIT;
	twos->mhData.wCompressionID		= 0;
	twos->mhData.wPacketSize		= 0;
	twos->mhData.dwSampleRate		= (AUD_SAMPLING_RATE << 16);

	twos->fcccb.fcc = FCC('t','w','o','s');
	twos->fcccb.cb  = sizeof(MP4_TWOS_BOX);

	return twos->fcccb.cb;
}

UINT enx_mp4_header_stsd(BYTE *data, UINT vid_type)
{
	MP4_SAMPLE_DESCRIPTION_BOX *stsd = (MP4_SAMPLE_DESCRIPTION_BOX *)data;

	stsd->mhData.dwVersion_Flags	= 0;
	stsd->mhData.dwEntryCount		= 1;

	stsd->fcccb.fcc = FCC('s','t','s','d');
	stsd->fcccb.cb  = sizeof(MP4_SAMPLE_DESCRIPTION_BOX);
	if (vid_type == VID_VIDEO) {
		stsd->fcccb.cb += enx_mp4_header_avc1(data + stsd->fcccb.cb);
	} else if (vid_type == VID_AUDIO) {
		stsd->fcccb.cb += enx_mp4_header_twos(data + stsd->fcccb.cb);
	}

	return stsd->fcccb.cb;
}

UINT enx_mp4_header_stts(BYTE *data, UINT vid_type)
{
	MP4_DECODING_TIME_TO_SAMPLE_BOX *stts = (MP4_DECODING_TIME_TO_SAMPLE_BOX *)data;

	stts->mhData.dwVersion_Flags	= 0;
	stts->mhData.dwEntryCount		= 1;
	if (vid_type == VID_VIDEO) {
		stts->mhData.dwSampleCount	= mp4para.v_dwSampleCount;
		stts->mhData.dwSampleDelta	= mp4para.m_dwTimescale;
	} else if (vid_type == VID_AUDIO) {
		stts->mhData.dwSampleCount	= mp4para.a_dwSampleCount * ((AUD_SAMPLING_RATE * (AUD_SAMPLING_BIT / 8)) / PCM8K_RecordingHz) / (AUD_SAMPLING_BIT / 8);
		stts->mhData.dwSampleDelta	= 1;
	}

	stts->fcccb.fcc = FCC('s','t','t','s');
	stts->fcccb.cb  = sizeof(MP4_DECODING_TIME_TO_SAMPLE_BOX);

	return stts->fcccb.cb;
}

UINT enx_mp4_header_stsc(BYTE *data, UINT vid_type)
{
	MP4_SAMPLE_TO_CHUNK_BOX *stsc = (MP4_SAMPLE_TO_CHUNK_BOX *)data;

	stsc->mhData.dwVersion_Flags	= 0;
	stsc->mhData.dwEntryCount 		= 1;
	stsc->mhData.dwFirstChunk 		= 1;
	if (vid_type == VID_VIDEO) {
		stsc->mhData.dwSamplePerChunk 	= 1;
	} else if (vid_type == VID_AUDIO) {
		stsc->mhData.dwSamplePerChunk 	= ((AUD_SAMPLING_RATE * (AUD_SAMPLING_BIT / 8)) / PCM8K_RecordingHz) / (AUD_SAMPLING_BIT / 8); // 400
	}
	stsc->mhData.dwSampleDescriptionID	= 1;

	stsc->fcccb.fcc = FCC('s','t','s','c');
	stsc->fcccb.cb  = sizeof(MP4_SAMPLE_TO_CHUNK_BOX);

	return stsc->fcccb.cb;
}

UINT enx_mp4_header_stss(BYTE *data)
{
	MP4_SYNC_SAMPLE_BOX *stss = (MP4_SYNC_SAMPLE_BOX *)data;
	BYTE *array = (BYTE *)(data + sizeof(MP4_SYNC_SAMPLE_BOX));

	stss->mhData.dwVersion_Flags	= 0;
	stss->mhData.dwEntryCount		= mp4para.Vstbl->stss_len;
	BDmaMemCpy_rtos(0, array, (BYTE *)mp4para.Vstbl->stss, mp4para.Vstbl->stss_len * sizeof(UINT));

	stss->fcccb.fcc = FCC('s','t','s','s');
	stss->fcccb.cb  = sizeof(MP4_SYNC_SAMPLE_BOX) + (mp4para.Vstbl->stss_len * sizeof(UINT));

	return stss->fcccb.cb;
}

UINT enx_mp4_header_stsz(BYTE *data, UINT vid_type)
{
	MP4_SAMPLE_SIZE_BOX *stsz = (MP4_SAMPLE_SIZE_BOX *)data;

	stsz->mhData.dwVersion_Flags	= 0;
	if (vid_type == VID_VIDEO) {
		stsz->mhData.dwSampleSize	= 0;
		stsz->mhData.dwEntryCount	= mp4para.Vstbl->stsz_len;

		BYTE *array = (BYTE *)(data + sizeof(MP4_SAMPLE_SIZE_BOX));
		BDmaMemCpy_rtos(0, array, (BYTE *)mp4para.Vstbl->stsz, stsz->mhData.dwEntryCount * sizeof(UINT));
		stsz->fcccb.cb				= sizeof(MP4_SAMPLE_SIZE_BOX) + (stsz->mhData.dwEntryCount * sizeof(UINT));
 	} else if (vid_type == VID_AUDIO) {
		stsz->mhData.dwSampleSize	= (AUD_SAMPLING_BIT / 8);
		stsz->mhData.dwEntryCount	= mp4para.Astbl->stsz_len * ((AUD_SAMPLING_RATE * (AUD_SAMPLING_BIT / 8)) / PCM8K_RecordingHz) / (AUD_SAMPLING_BIT / 8);
		stsz->fcccb.cb				= sizeof(MP4_SAMPLE_SIZE_BOX);
	}

	stsz->fcccb.fcc = FCC('s','t','s','z');

	return stsz->fcccb.cb;
}

UINT enx_mp4_header_stco(BYTE *data, UINT vid_type)
{
	MP4_CHUNK_OFFSET_BOX *stco = (MP4_CHUNK_OFFSET_BOX *)data;
	BYTE *array = (BYTE *)(data + sizeof(MP4_CHUNK_OFFSET_BOX));
	BYTE *soucre = NULL;

	stco->mhData.dwVersion_Flags	= 0;
	if (vid_type == VID_VIDEO) {
		stco->mhData.dwEntryCount	= mp4para.Vstbl->stco_len;
		soucre = (BYTE *)mp4para.Vstbl->stco;
	} else if (vid_type == VID_AUDIO) {
		stco->mhData.dwEntryCount	= mp4para.Astbl->stco_len;
		soucre = (BYTE *)mp4para.Astbl->stco;
	}
	BDmaMemCpy_rtos(0, array, soucre, stco->mhData.dwEntryCount * sizeof(UINT));

	stco->fcccb.fcc = FCC('s','t','c','o');
	stco->fcccb.cb  = sizeof(MP4_CHUNK_OFFSET_BOX) + (stco->mhData.dwEntryCount * sizeof(UINT));

	return stco->fcccb.cb;
}

UINT enx_mp4_header_stbl(BYTE *data, UINT vid_type)
{
	FCC_CB_t *stbl = (FCC_CB_t *)data;

	stbl->fcc = FCC('s','t','b','l');
	stbl->cb  = sizeof(FCC_CB_t);

	stbl->cb += enx_mp4_header_stsd(data + stbl->cb, vid_type);
	stbl->cb += enx_mp4_header_stts(data + stbl->cb, vid_type);
	stbl->cb += enx_mp4_header_stsc(data + stbl->cb, vid_type);
	if (vid_type == VID_VIDEO) {
		stbl->cb += enx_mp4_header_stss(data + stbl->cb);
	}
	stbl->cb += enx_mp4_header_stsz(data + stbl->cb, vid_type);
	stbl->cb += enx_mp4_header_stco(data + stbl->cb, vid_type);

	return stbl->cb;
}

UINT enx_mp4_header_minf(BYTE *data, UINT vid_type)
{
	FCC_CB_t *minf = (FCC_CB_t *)data;

	minf->fcc = FCC('m','i','n','f');
	minf->cb  = sizeof(FCC_CB_t);

	if (vid_type == VID_VIDEO) {
		minf->cb += enx_mp4_header_vmhd(data + minf->cb);
	} else if (vid_type == VID_AUDIO) {
		minf->cb += enx_mp4_header_smhd(data + minf->cb);
	}
	minf->cb += enx_mp4_header_dinf(data + minf->cb, vid_type);
	minf->cb += enx_mp4_header_stbl(data + minf->cb, vid_type);

	return minf->cb;
}

UINT enx_mp4_header_mdia(BYTE *data, UINT vid_type)
{
	FCC_CB_t *mdia = (FCC_CB_t *)data;

	mdia->fcc = FCC('m','d','i','a');
	mdia->cb  = sizeof(FCC_CB_t);

	mdia->cb += enx_mp4_header_mdhd(data + mdia->cb, vid_type);
	mdia->cb += enx_mp4_header_hdlr(data + mdia->cb, vid_type);
	mdia->cb += enx_mp4_header_minf(data + mdia->cb, vid_type);

	return mdia->cb;
}

UINT enx_mp4_header_trak(BYTE *data, UINT vid_type)
{
	FCC_CB_t *trak = (FCC_CB_t *)data;

	trak->fcc = FCC('t','r','a','k');
	trak->cb  = sizeof(FCC_CB_t);

	trak->cb += enx_mp4_header_tkhd(data + trak->cb, vid_type);
	trak->cb += enx_mp4_header_edts(data + trak->cb, vid_type);
	trak->cb += enx_mp4_header_mdia(data + trak->cb, vid_type);

	return trak->cb;
}

UINT enx_mp4_header_free(BYTE *data, UINT free_size)
{
	FCC_CB_t *free = (FCC_CB_t *)data;

	free->fcc = FCC('f','r','e','e');
	free->cb  = sizeof(FCC_CB_t) + free_size;

	return free->cb;
}

UINT enx_mp4_header_moov(BYTE *data)
{
	FCC_CB_t *moov = (FCC_CB_t *)data;

	moov->fcc = FCC('m','o','o','v');
	moov->cb  = sizeof(FCC_CB_t);

	moov->cb += enx_mp4_header_mvhd(data + moov->cb);
	if (mp4para.mp4a->option & VID_VIDEO) {
		moov->cb += enx_mp4_header_trak(data + moov->cb, VID_VIDEO);
	}
#if (FAT_SDSAVE_AUD==1)
	if (mp4para.mp4a->option & VID_AUDIO) {
		moov->cb += enx_mp4_header_trak(data + moov->cb, VID_AUDIO);
	}
#endif
	int hdr_free = mp4para.but_t->hdr_size - mp4para.but_t->hdr_len - moov->cb - 16;
	if (hdr_free < 0) {
		flprintf("MP4 Header overflow(%d/%d/%d)\r\n", moov->cb, mp4para.but_t->hdr_len, mp4para.but_t->hdr_size);
	}
	moov->cb += enx_mp4_header_free(data + moov->cb, hdr_free);

	return moov->cb;
}

UINT enx_mp4_header_mdat(BYTE *data)
{
	FCC_CB_t *mdat = (FCC_CB_t *)data;

	mdat->cb  = sizeof(FCC_CB_t) + mp4para.mp4a->mdat_size;
	mdat->fcc = FCC('m','d','a','t');

	return mdat->cb;
}

BYTE *mp4_write_header(t_mp4_t *mp4a, void *buffer)
{
	t_videnc_totalbuf_t *buf_t = (t_videnc_totalbuf_t *)buffer; 
	BYTE *data = buf_t->hdr_info;

	mp4_init_header_build(mp4a, buf_t);

	buf_t->hdr_len = 0;
	buf_t->hdr_len += enx_mp4_header_ftyp(data);
	buf_t->hdr_len += enx_mp4_header_moov(data + buf_t->hdr_len);
	buf_t->hdr_len += enx_mp4_header_mdat(data + buf_t->hdr_len);

	return data;
}
#endif
#endif
