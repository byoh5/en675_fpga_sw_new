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

/**************************************************************************
 Big/Little Endian 처리를 추가함
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
#if (FAT_SDSAVE_AUD==1)
	mp4para.a_dwDuration = mp4para.m_dwTimescale * mp4para.a_dwSampleCount / PCM_RecordingHz;
#endif
	if (mp4para.Vstbl->stsz_len != mp4para.v_dwSampleCount) {
		flprintf("Error sample count(%d/%d)\r\n", mp4para.Vstbl->stsz_len, mp4para.v_dwSampleCount);
	}
}

UINT enx_mp4_header_ftyp(BYTE *data)
{
	MP4_FILE_TYPE_BOX *ftyp = (MP4_FILE_TYPE_BOX *)data;

	ftyp->mhData.fcc1               = FCC('i','s','o','m');
	ftyp->mhData.fcc2               = vidswapSB32(0x00000200);
	ftyp->mhData.fcc3               = FCC('i','s','o','m');
	ftyp->mhData.fcc4               = FCC('i','s','o','2');
	ftyp->mhData.fcc5               = FCC('a','v','c','1');
	ftyp->mhData.fcc6               = FCC('m','p','4','1');

	ftyp->fcccb.fcc                 = FCC('f','t','y','p');
	ftyp->fcccb.cb                  = vidswapSB32(sizeof(MP4_FILE_TYPE_BOX));

	return sizeof(MP4_FILE_TYPE_BOX);
}

UINT enx_mp4_header_mvhd(BYTE *data)
{
	MP4_MOVIE_HEADER_BOX *mvhd = (MP4_MOVIE_HEADER_BOX *)data;

	mvhd->mhData.dwVersion_Flags    = vidswapSB32(0);
	mvhd->mhData.dwCreationtime     = vidswapSB32(gptMsgShare.TIME + 0x7C25B080);
	mvhd->mhData.dwModificationtime = vidswapSB32(gptMsgShare.TIME + 0x7C25B080);
	mvhd->mhData.dwTimescale        = vidswapSB32(mp4para.m_dwTimescale);
	mvhd->mhData.dwRate	            = vidswapSB32(0x00010000);
	mvhd->mhData.wVolume            = vidswapSB16(0x0100);
	mvhd->mhData._res1              = vidswapSB16(0);
	mvhd->mhData._res2              = vidswapSB32(0);
	mvhd->mhData._res3              = vidswapSB32(0);
	mvhd->mhData.dwMatrix[0]        = vidswapSB32(0x00010000);
	mvhd->mhData.dwMatrix[1]        = vidswapSB32(0);
	mvhd->mhData.dwMatrix[2]        = vidswapSB32(0);
	mvhd->mhData.dwMatrix[3]        = vidswapSB32(0);
	mvhd->mhData.dwMatrix[4]        = vidswapSB32(0x00010000);
	mvhd->mhData.dwMatrix[5]        = vidswapSB32(0);
	mvhd->mhData.dwMatrix[6]        = vidswapSB32(0);
	mvhd->mhData.dwMatrix[7]        = vidswapSB32(0);
	mvhd->mhData.dwMatrix[8]        = vidswapSB32(0x40000000);
	mvhd->mhData.dwPreDefined[0]    = vidswapSB32(0);
	mvhd->mhData.dwPreDefined[1]    = vidswapSB32(0);
	mvhd->mhData.dwPreDefined[2]    = vidswapSB32(0);
	mvhd->mhData.dwPreDefined[3]    = vidswapSB32(0);
	mvhd->mhData.dwPreDefined[4]    = vidswapSB32(0);
	mvhd->mhData.dwPreDefined[5]    = vidswapSB32(0);
#if (FAT_SDSAVE_AUD==1)
	if (mp4para.mp4a->option & VID_AUDIO) {
		mvhd->mhData.dwDuration     = vidswapSB32(mp4para.v_dwDuration > mp4para.a_dwDuration ? mp4para.v_dwDuration : mp4para.a_dwDuration);
		mvhd->mhData.dwNexttrakid   = vidswapSB32(3);
	} else
#endif
	{
		mvhd->mhData.dwDuration     = vidswapSB32(mp4para.v_dwDuration);
		mvhd->mhData.dwNexttrakid   = vidswapSB32(2);
	}

	mvhd->fcccb.fcc                 = FCC('m','v','h','d');
	mvhd->fcccb.cb                  = vidswapSB32(sizeof(MP4_MOVIE_HEADER_BOX));

	return sizeof(MP4_MOVIE_HEADER_BOX);
}

UINT enx_mp4_header_tkhd(BYTE *data, UINT vid_type)
{
	MP4_TRACK_HEADER_BOX *tkhd = (MP4_TRACK_HEADER_BOX *)data;

	tkhd->mhData.dwVersion_Flags    = vidswapSB32(0x0000000F);
	tkhd->mhData.dwCreationtime     = vidswapSB32(gptMsgShare.TIME + 0x7C25B080);
	tkhd->mhData.dwModificationtime = vidswapSB32(gptMsgShare.TIME + 0x7C25B080);
	tkhd->mhData._res1              = vidswapSB32(0);
	tkhd->mhData._res2              = vidswapSB32(0);
	tkhd->mhData._res3              = vidswapSB32(0);
	tkhd->mhData.wLayer             = vidswapSB16(0);
	tkhd->mhData.wAlternateGroup    = vidswapSB16(0);
	tkhd->mhData._res4              = vidswapSB16(0);
	tkhd->mhData.dwMatrix[0]        = vidswapSB32(0x00010000);
	tkhd->mhData.dwMatrix[1]        = vidswapSB32(0x0);
	tkhd->mhData.dwMatrix[2]        = vidswapSB32(0x0);
	tkhd->mhData.dwMatrix[3]        = vidswapSB32(0x0);
	tkhd->mhData.dwMatrix[4]        = vidswapSB32(0x00010000);
	tkhd->mhData.dwMatrix[5]        = vidswapSB32(0x0);
	tkhd->mhData.dwMatrix[6]        = vidswapSB32(0x0);
	tkhd->mhData.dwMatrix[7]        = vidswapSB32(0x0);
	tkhd->mhData.dwMatrix[8]        = vidswapSB32(0x40000000);
	if (vid_type == VID_VIDEO) {
		tkhd->mhData.dwTrackId      = vidswapSB32(1);
		tkhd->mhData.dwDuration     = vidswapSB32(mp4para.v_dwDuration);
		tkhd->mhData.wVolume        = vidswapSB16(0x0);
		tkhd->mhData.dwWidth        = vidswapSB32(mp4para.mp4a->i_width << 16);
		tkhd->mhData.dwHeight       = vidswapSB32(mp4para.mp4a->i_height << 16);
	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		tkhd->mhData.dwTrackId      = vidswapSB32(2);
		tkhd->mhData.dwDuration     = vidswapSB32(mp4para.a_dwDuration);
		tkhd->mhData.wVolume        = vidswapSB16(0x0100);
		tkhd->mhData.dwWidth        = vidswapSB32(0);
		tkhd->mhData.dwHeight       = vidswapSB32(0);
	}
#endif

	tkhd->fcccb.fcc                 = FCC('t','k','h','d');
	tkhd->fcccb.cb                  = vidswapSB32(sizeof(MP4_TRACK_HEADER_BOX));

	return sizeof(MP4_TRACK_HEADER_BOX);
}

UINT enx_mp4_header_elst(BYTE *data, UINT vid_type)
{
	MP4_EDIT_LIST_BOX *elst = (MP4_EDIT_LIST_BOX *)data;

	elst->mhData.dwVersion_Flags    = vidswapSB32(0);
	elst->mhData.dwEntries          = vidswapSB32(1);
	if (vid_type == VID_VIDEO) {
		elst->mhData.dwTrackduration= vidswapSB32(mp4para.v_dwDuration);
	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		elst->mhData.dwTrackduration= vidswapSB32(mp4para.a_dwDuration);
	}
#endif
	elst->mhData.dwMediatime        = vidswapSB32(0);
	elst->mhData.dwMediarate        = vidswapSB32(0x00010000);

	elst->fcccb.fcc                 = FCC('e','l','s','t');
	elst->fcccb.cb                  = vidswapSB32(sizeof(MP4_EDIT_LIST_BOX));

	return sizeof(MP4_EDIT_LIST_BOX);
}

UINT enx_mp4_header_edts(BYTE *data, UINT vid_type)
{
	FCC_CB_t *edts = (FCC_CB_t *)data;

	UINT size                       = sizeof(FCC_CB_t);
	size                           += enx_mp4_header_elst(data + sizeof(FCC_CB_t), vid_type);

	edts->fcc                       = FCC('e','d','t','s');
	edts->cb                        = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_mdhd(BYTE *data, UINT vid_type)
{
	MP4_MEDIA_HEADER_BOX *mdhd = (MP4_MEDIA_HEADER_BOX *)data;

	mdhd->mhData.dwVersion_Flags    = vidswapSB32(0);
	mdhd->mhData.dwCreationtime     = vidswapSB32(gptMsgShare.TIME + 0x7C25B080);
	mdhd->mhData.dwModificationtime = vidswapSB32(gptMsgShare.TIME + 0x7C25B080);
	if (vid_type == VID_VIDEO) {
		mdhd->mhData.dwTimescale    = vidswapSB32(mp4para.m_dwTimescale * mp4para.mp4a->f_fps);
		mdhd->mhData.dwDuration     = vidswapSB32(mp4para.m_dwTimescale * mp4para.v_dwSampleCount);
	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		mdhd->mhData.dwTimescale    = vidswapSB32(AUD_SAMPLING_RATE);
		mdhd->mhData.dwDuration     = vidswapSB32(mp4para.a_dwSampleCount * ((AUD_SAMPLING_RATE * (AUD_SAMPLING_BIT / 8)) / PCM_RecordingHz) / (AUD_SAMPLING_BIT / 8));
	}
#endif
	WORD lan = (('k' & 0x1F) << 10) | (('o' & 0x1F) << 5) | ('r' & 0x1F);
	mdhd->mhData.wLanguage          = vidswapSB16(lan); // Pad(1) + Language(15)
	//mdhd->mhData.wLanguage          = vidswapSB16(0x55C4); // Pad(1) + Language(15)
	mdhd->mhData.wPreDefined        = vidswapSB16(0);

	mdhd->fcccb.fcc                 = FCC('m','d','h','d');
	mdhd->fcccb.cb                  = vidswapSB32(sizeof(MP4_MEDIA_HEADER_BOX));

	return sizeof(MP4_MEDIA_HEADER_BOX);
}

UINT enx_mp4_header_hdlr(BYTE *data, UINT vid_type)
{
	MP4_HANDLER_REFERENCE_BOX *hdlr = (MP4_HANDLER_REFERENCE_BOX *)data;

	hdlr->mhData.dwVersion_Flags    = vidswapSB32(0);
	hdlr->mhData.dwPreDefined       = vidswapSB32(0);
	hdlr->mhData._res1              = vidswapSB32(0);
	hdlr->mhData._res2              = vidswapSB32(0);
	hdlr->mhData._res3              = vidswapSB32(0);
	if (vid_type == VID_VIDEO) {
		hdlr->mhData.dwSubtype      = FCC('v','i','d','e');
		hdlr->mhData.dwName[0]      = FCC('V','i','d','e');
		hdlr->mhData.dwName[1]      = FCC('o','H','a','n');
		hdlr->mhData.dwName[2]      = FCC('d','l','e','r');
	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		hdlr->mhData.dwSubtype      = FCC('s','o','u','n');
		hdlr->mhData.dwName[0]      = FCC('S','o','u','n');
		hdlr->mhData.dwName[1]      = FCC('d','H','a','n');
		hdlr->mhData.dwName[2]      = FCC('d','l','e','r');
	}
#endif
	hdlr->mhData.bNull              = 0;

	hdlr->fcccb.fcc                 = FCC('h','d','l','r');
	hdlr->fcccb.cb                  = vidswapSB32(sizeof(MP4_HANDLER_REFERENCE_BOX));

	return sizeof(MP4_HANDLER_REFERENCE_BOX);
}

UINT enx_mp4_header_vmhd(BYTE *data)
{
	MP4_VIDEO_MEDIA_HEADER_BOX *vmhd = (MP4_VIDEO_MEDIA_HEADER_BOX *)data;

	vmhd->mhData.dwVersion_Flags    = vidswapSB32(1);
	vmhd->mhData.wGraphicsMode      = vidswapSB16(0);
	vmhd->mhData.wOpColor[0]        = vidswapSB16(0);
	vmhd->mhData.wOpColor[1]        = vidswapSB16(0);
	vmhd->mhData.wOpColor[2]        = vidswapSB16(0);

	vmhd->fcccb.fcc                 = FCC('v','m','h','d');
	vmhd->fcccb.cb                  = vidswapSB32(sizeof(MP4_VIDEO_MEDIA_HEADER_BOX));

	return sizeof(MP4_VIDEO_MEDIA_HEADER_BOX);
}

#if (FAT_SDSAVE_AUD==1)
UINT enx_mp4_header_smhd(BYTE *data)
{
	MP4_SOUND_MEDIA_HEADER_BOX *smhd = (MP4_SOUND_MEDIA_HEADER_BOX *)data;

	smhd->mhData.dwVersion_Flags    = vidswapSB32(0);
	smhd->mhData.wBalance           = vidswapSB16(0);
	smhd->mhData._res1              = vidswapSB16(0);

	smhd->fcccb.fcc                 = FCC('s','m','h','d');
	smhd->fcccb.cb                  = vidswapSB32(sizeof(MP4_SOUND_MEDIA_HEADER_BOX));

	return sizeof(MP4_SOUND_MEDIA_HEADER_BOX);
}
#endif

UINT enx_mp4_header_url(BYTE *data, UINT vid_type)
{
	MP4_DATA_ENTRY_URL_BOX *url = (MP4_DATA_ENTRY_URL_BOX *)data;

	url->mhData.dwVersion_Flags     = vidswapSB32(1);

	url->fcccb.fcc                  = FCC('u','r','l',' ');
	url->fcccb.cb                   = vidswapSB32(sizeof(MP4_DATA_ENTRY_URL_BOX));

	return sizeof(MP4_DATA_ENTRY_URL_BOX);
	UNUSED(vid_type);
}

UINT enx_mp4_header_dref(BYTE *data, UINT vid_type)
{
	MP4_DATA_REFERENCE_BOX *dref = (MP4_DATA_REFERENCE_BOX *)data;

	UINT size                       = sizeof(MP4_DATA_REFERENCE_BOX);
	size                           += enx_mp4_header_url(data + sizeof(MP4_DATA_REFERENCE_BOX), vid_type);

	dref->mhData.dwVersion_Flags    = vidswapSB32(0);
	dref->mhData.dwEntry            = vidswapSB32(1);

	dref->fcccb.fcc                 = FCC('d','r','e','f');
	dref->fcccb.cb                  = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_dinf(BYTE *data, UINT vid_type)
{
	FCC_CB_t *dinf = (FCC_CB_t *)data;

	UINT size                       = sizeof(FCC_CB_t);
	size                           += enx_mp4_header_dref(data + sizeof(FCC_CB_t), vid_type);

	dinf->fcc                       = FCC('d','i','n','f');
	dinf->cb                        = vidswapSB32(size);

	return size;
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

	UINT size                       = sizeof(MP4_AVC_CONFIGURATION_BOX) + i;

	avcc->fcccb.fcc                 = FCC('a','v','c','C');
	avcc->fcccb.cb                  = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_avc1(BYTE *data)
{
	MP4_AVC1_BOX *avc1 = (MP4_AVC1_BOX *)data;

	avc1->mhData.bReserved[0]       = 0;
	avc1->mhData.bReserved[1]       = 0;
	avc1->mhData.bReserved[2]       = 0;
	avc1->mhData.bReserved[3]       = 0;
	avc1->mhData.bReserved[4]       = 0;
	avc1->mhData.bReserved[5]       = 0;
	avc1->mhData.wDataRefIndex      = vidswapSB16(1);
	avc1->mhData.dwPreDefined[0]    = vidswapSB32(0);
	avc1->mhData.dwPreDefined[1]    = vidswapSB32(0);
	avc1->mhData.dwPreDefined[2]    = vidswapSB32(0);
	avc1->mhData.dwPreDefined[3]    = vidswapSB32(0);
	avc1->mhData.wWidth             = vidswapSB16((WORD)mp4para.mp4a->i_width);
	avc1->mhData.wHeight            = vidswapSB16((WORD)mp4para.mp4a->i_height);
	avc1->mhData.dwHorizResolution  = vidswapSB32(0x00480000);
	avc1->mhData.dwVertiResolution  = vidswapSB32(0x00480000);
	avc1->mhData.dwDatasize         = vidswapSB32(0);
	avc1->mhData.wFrameCount        = vidswapSB16(1);
	avc1->mhData.dwCompressorName[0]= vidswapSB32(0);
	avc1->mhData.dwCompressorName[1]= vidswapSB32(0);
	avc1->mhData.dwCompressorName[2]= vidswapSB32(0);
	avc1->mhData.dwCompressorName[3]= vidswapSB32(0);
	avc1->mhData.dwCompressorName[4]= vidswapSB32(0);
	avc1->mhData.dwCompressorName[5]= vidswapSB32(0);
	avc1->mhData.dwCompressorName[6]= vidswapSB32(0);
	avc1->mhData.dwCompressorName[7]= vidswapSB32(0);
	avc1->mhData.wDepth             = vidswapSB16(24);
	avc1->mhData.wPreDefined2       = vidswapSB16(-1);

	UINT size                       = sizeof(MP4_AVC1_BOX);
	size                           += enx_mp4_header_avcc(data + sizeof(MP4_AVC1_BOX));

	avc1->fcccb.fcc = FCC('a','v','c','1');
	avc1->fcccb.cb  = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_hvcc(BYTE *data)
{
	MP4_HEVC_CONFIGURATION_BOX *hvcc = (MP4_HEVC_CONFIGURATION_BOX *)data;
	UINT i = 0, j = 0, zerocount = 0;
	BYTE *array = data + sizeof(MP4_HEVC_CONFIGURATION_BOX);

	i = 6;

#define helper_3byte_zero_nalunit() if (mp4para.mp4a->vps_data[i++] == 0) { zerocount++; if (zerocount == 2) { i++; zerocount = 0; } }

	hvcc->mhData.bConfigureationVersion = 0x01;
	hvcc->mhData.bGeneralProfile = mp4para.mp4a->vps_data[i];		// profile_space(2), tier_flag(1), profile_idc(1)
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralProfileCompatibilityFlags[0] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralProfileCompatibilityFlags[1] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralProfileCompatibilityFlags[2] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralProfileCompatibilityFlags[3] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralConstraintIndicatorFlags[0] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralConstraintIndicatorFlags[1] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralConstraintIndicatorFlags[2] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralConstraintIndicatorFlags[3] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralConstraintIndicatorFlags[4] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralConstraintIndicatorFlags[5] = mp4para.mp4a->vps_data[i];
	helper_3byte_zero_nalunit();
	hvcc->mhData.bGeneralLevelIdc   = 0;
	hvcc->mhData.wMinSpatialSegmentationIdc = vidswapSB16(0xF000 | 0x0);	// 0xF000 | min_spatial_segmentation_idc
	hvcc->mhData.bParallelismType   = 0xFC | 0x0;			// 0xFC   | parallelismType
	hvcc->mhData.bChromaFormatIdc   = 0xFC | 0x1;			// 0xFC   | chroma_format_idc
	hvcc->mhData.bBitDepthLumaMinus8= 0xF8 | 0x0;			// 0xF8   | bit_depth_luma_minus8
	hvcc->mhData.bBitDepthChromaMinus8 = 0xF8 | 0x0;		// 0xF8   | bit_depth_chroma_minus8
	hvcc->mhData.wAvgFrameRate      = vidswapSB16(0x0);
	hvcc->mhData.bTemporal          = (((0x0) & 0x3) << 6) | (((0x1) & 0x7) << 3) | (((0x1) & 0x1) << 2) | ((0x3) & 0x3);
	hvcc->mhData.bNumOfArrays       = 3;

	i = 0;

	// VPS NAL Info
	array[i++]                      = 0x20;		// 1byte: Array completeness(1b), Reserved(1b), NAL unit type(6b)
	array[i++]                      = 0x00;		// 2byte: Num nal us
	array[i++]                      = 0x01;		// 2byte: Num nal us

	// VPS Length(16bit)
	array[i++]                      = (mp4para.mp4a->vps_size >> 8) & 0xFF;
	array[i++]                      = (mp4para.mp4a->vps_size     ) & 0xFF;

	// VPS NAL
	for (j = 0; j < mp4para.mp4a->vps_size; j++) {
		array[i++]                  = mp4para.mp4a->vps_data[j];
	}

	// SPS NAL Info
	array[i++]                      = 0x21;		// 1byte: Array completeness(1b), Reserved(1b), NAL unit type(6b)
	array[i++]                      = 0x00;		// 2byte: Num nal us
	array[i++]                      = 0x01;		// 2byte: Num nal us

	// SPS Length(16bit)
	array[i++]                      = (mp4para.mp4a->sps_size >> 8) & 0xFF;
	array[i++]                      = (mp4para.mp4a->sps_size     ) & 0xFF;

	// SPS NAL
	for (j = 0; j < mp4para.mp4a->sps_size; j++) {
		array[i++]                  = mp4para.mp4a->sps_data[j];
	}

	// PPS NAL Info
	array[i++]                      = 0x22;		// 1byte: Array completeness(1b), Reserved(1b), NAL unit type(6b)
	array[i++]                      = 0x00;		// 2byte: Num nal us
	array[i++]                      = 0x01;		// 2byte: Num nal us

	// PPS Length(16bit)
	array[i++]                      = (mp4para.mp4a->pps_size >> 8) & 0xFF;
	array[i++]                      = (mp4para.mp4a->pps_size     ) & 0xFF;

	// PPS NAL
	for (j = 0; j < mp4para.mp4a->pps_size; j++) {
		array[i++]                  = mp4para.mp4a->pps_data[j];
	}

	UINT size                       = sizeof(MP4_HEVC_CONFIGURATION_BOX) + i;

	hvcc->fcccb.fcc                 = FCC('h','v','c','C');
	hvcc->fcccb.cb                  = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_hvc1(BYTE *data)
{
	MP4_HVC1_BOX *hvc1 = (MP4_HVC1_BOX *)data;

	hvc1->mhData.bReserved[0]       = 0;
	hvc1->mhData.bReserved[1]       = 0;
	hvc1->mhData.bReserved[2]       = 0;
	hvc1->mhData.bReserved[3]       = 0;
	hvc1->mhData.bReserved[4]       = 0;
	hvc1->mhData.bReserved[5]       = 0;
	hvc1->mhData.wDataRefIndex      = vidswapSB16(1);
	hvc1->mhData.dwPreDefined[0]    = vidswapSB32(0);
	hvc1->mhData.dwPreDefined[1]    = vidswapSB32(0);
	hvc1->mhData.dwPreDefined[2]    = vidswapSB32(0);
	hvc1->mhData.dwPreDefined[3]    = vidswapSB32(0);
	hvc1->mhData.wWidth             = vidswapSB16((WORD)mp4para.mp4a->i_width);
	hvc1->mhData.wHeight            = vidswapSB16((WORD)mp4para.mp4a->i_height);
	hvc1->mhData.dwHorizResolution  = vidswapSB32(0x00480000);
	hvc1->mhData.dwVertiResolution  = vidswapSB32(0x00480000);
	hvc1->mhData.dwDatasize         = vidswapSB32(0);
	hvc1->mhData.wFrameCount        = vidswapSB16(1);
	hvc1->mhData.dwCompressorName[0]= vidswapSB32(0);
	hvc1->mhData.dwCompressorName[1]= vidswapSB32(0);
	hvc1->mhData.dwCompressorName[2]= vidswapSB32(0);
	hvc1->mhData.dwCompressorName[3]= vidswapSB32(0);
	hvc1->mhData.dwCompressorName[4]= vidswapSB32(0);
	hvc1->mhData.dwCompressorName[5]= vidswapSB32(0);
	hvc1->mhData.dwCompressorName[6]= vidswapSB32(0);
	hvc1->mhData.dwCompressorName[7]= vidswapSB32(0);
	hvc1->mhData.wDepth             = vidswapSB16(24);
	hvc1->mhData.wPreDefined2       = vidswapSB16(-1);

	UINT size                       = sizeof(MP4_HVC1_BOX);
	size                           += enx_mp4_header_hvcc(data + sizeof(MP4_HVC1_BOX));

	hvc1->fcccb.fcc = FCC('h','e','v','1');
	hvc1->fcccb.cb  = vidswapSB32(size);

	return size;
}

#if (FAT_SDSAVE_AUD==1)
UINT enx_mp4_header_twos(BYTE *data)
{
	MP4_TWOS_BOX *twos = (MP4_TWOS_BOX *)data;

	twos->mhData.dwReserved[0]      = vidswapSB32(0);
	twos->mhData.dwReserved[1]      = vidswapSB32(1);							// ?????
	twos->mhData.dwReserved[2]      = vidswapSB32(0);
	twos->mhData.dwReserved[3]      = vidswapSB32(0);
	twos->mhData.wNumberOfChannels  = vidswapSB16(1);
	twos->mhData.wSampleSize        = vidswapSB16(AUD_SAMPLING_BIT);
	twos->mhData.wCompressionID     = vidswapSB16(0);
	twos->mhData.wPacketSize        = vidswapSB16(0);
	twos->mhData.dwSampleRate       = vidswapSB32((AUD_SAMPLING_RATE << 16));

	twos->fcccb.fcc                 = FCC('t','w','o','s');
	twos->fcccb.cb                  = vidswapSB32(sizeof(MP4_TWOS_BOX));

	return sizeof(MP4_TWOS_BOX);
}
#endif

UINT enx_mp4_header_stsd(BYTE *data, UINT vid_type)
{
	MP4_SAMPLE_DESCRIPTION_BOX *stsd = (MP4_SAMPLE_DESCRIPTION_BOX *)data;

	UINT size                       = sizeof(MP4_SAMPLE_DESCRIPTION_BOX);
	if (vid_type == VID_VIDEO) {
		if (gtUser.vcVideo[mp4para.mp4a->v_type].eCodec == e_vcodecH264) {
			size                   += enx_mp4_header_avc1(data + sizeof(MP4_SAMPLE_DESCRIPTION_BOX));
		} else if (gtUser.vcVideo[mp4para.mp4a->v_type].eCodec == e_vcodecH265) {
			size                   += enx_mp4_header_hvc1(data + sizeof(MP4_SAMPLE_DESCRIPTION_BOX));
		} else {
			_Rprintf("enx_mp4_header_stsd Error!\n");
		}
	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		size                       += enx_mp4_header_twos(data + sizeof(MP4_SAMPLE_DESCRIPTION_BOX));
	}
#endif

	stsd->mhData.dwVersion_Flags    = vidswapSB32(0);
	stsd->mhData.dwEntryCount       = vidswapSB32(1);

	stsd->fcccb.fcc                 = FCC('s','t','s','d');
	stsd->fcccb.cb                  = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_stts(BYTE *data, UINT vid_type)
{
	MP4_DECODING_TIME_TO_SAMPLE_BOX *stts = (MP4_DECODING_TIME_TO_SAMPLE_BOX *)data;

	stts->mhData.dwVersion_Flags    = vidswapSB32(0);
	stts->mhData.dwEntryCount       = vidswapSB32(1);
	if (vid_type == VID_VIDEO) {
		stts->mhData.dwSampleCount  = vidswapSB32(mp4para.v_dwSampleCount);
		stts->mhData.dwSampleDelta  = vidswapSB32(mp4para.m_dwTimescale);
	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		stts->mhData.dwSampleCount  = vidswapSB32(mp4para.a_dwSampleCount * ((AUD_SAMPLING_RATE * (AUD_SAMPLING_BIT / 8)) / PCM_RecordingHz) / (AUD_SAMPLING_BIT / 8));
		stts->mhData.dwSampleDelta  = vidswapSB32(1);
	}
#endif

	stts->fcccb.fcc                 = FCC('s','t','t','s');
	stts->fcccb.cb                  = vidswapSB32(sizeof(MP4_DECODING_TIME_TO_SAMPLE_BOX));

	return sizeof(MP4_DECODING_TIME_TO_SAMPLE_BOX);
}

UINT enx_mp4_header_stsc(BYTE *data, UINT vid_type)
{
	MP4_SAMPLE_TO_CHUNK_BOX *stsc = (MP4_SAMPLE_TO_CHUNK_BOX *)data;

	stsc->mhData.dwVersion_Flags    = vidswapSB32(0);
	stsc->mhData.dwEntryCount       = vidswapSB32(1);
	stsc->mhData.dwFirstChunk       = vidswapSB32(1);
	if (vid_type == VID_VIDEO) {
		stsc->mhData.dwSamplePerChunk= vidswapSB32(1);
	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		stsc->mhData.dwSamplePerChunk= vidswapSB32(((AUD_SAMPLING_RATE * (AUD_SAMPLING_BIT / 8)) / PCM_RecordingHz) / (AUD_SAMPLING_BIT / 8)); // 400
	}
#endif
	stsc->mhData.dwSampleDescriptionID= vidswapSB32(1);

	stsc->fcccb.fcc                 = FCC('s','t','s','c');
	stsc->fcccb.cb                  = vidswapSB32(sizeof(MP4_SAMPLE_TO_CHUNK_BOX));

	return sizeof(MP4_SAMPLE_TO_CHUNK_BOX);
}

UINT enx_mp4_header_stss(BYTE *data)
{
	MP4_SYNC_SAMPLE_BOX *stss = (MP4_SYNC_SAMPLE_BOX *)data;

	UINT size                       = sizeof(MP4_SYNC_SAMPLE_BOX);
	if (mp4para.Vstbl->stss_len) {
		BYTE *array = (BYTE *)(data + sizeof(MP4_SYNC_SAMPLE_BOX));
		BDmaMemCpy_rtos_flush(0, array, (BYTE *)mp4para.Vstbl->stss, mp4para.Vstbl->stss_len * sizeof(UINT));
		size                       += (mp4para.Vstbl->stss_len * sizeof(UINT));
	}

	stss->mhData.dwVersion_Flags    = vidswapSB32(0);
	stss->mhData.dwEntryCount       = vidswapSB32(mp4para.Vstbl->stss_len);

	stss->fcccb.fcc                 = FCC('s','t','s','s');
	stss->fcccb.cb                  = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_stsz(BYTE *data, UINT vid_type)
{
	MP4_SAMPLE_SIZE_BOX *stsz = (MP4_SAMPLE_SIZE_BOX *)data;

	UINT size = sizeof(MP4_SAMPLE_SIZE_BOX);

	stsz->mhData.dwVersion_Flags    = vidswapSB32(0);
	if (vid_type == VID_VIDEO) {
		stsz->mhData.dwSampleSize   = vidswapSB32(0);
		stsz->mhData.dwEntryCount   = vidswapSB32(mp4para.Vstbl->stsz_len);

		if (mp4para.Vstbl->stsz_len) {
			BYTE *array = (BYTE *)(data + sizeof(MP4_SAMPLE_SIZE_BOX));
			BDmaMemCpy_rtos_flush(0, array, (BYTE *)mp4para.Vstbl->stsz, mp4para.Vstbl->stsz_len * sizeof(UINT));
			size                   += (mp4para.Vstbl->stsz_len * sizeof(UINT));
		}
 	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		stsz->mhData.dwSampleSize   = vidswapSB32((AUD_SAMPLING_BIT / 8));
		stsz->mhData.dwEntryCount   = vidswapSB32(mp4para.Astbl->stsz_len * ((AUD_SAMPLING_RATE * (AUD_SAMPLING_BIT / 8)) / PCM_RecordingHz) / (AUD_SAMPLING_BIT / 8));
	}
#endif

	stsz->fcccb.fcc                 = FCC('s','t','s','z');
	stsz->fcccb.cb                  = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_stco(BYTE *data, UINT vid_type)
{
	MP4_CHUNK_OFFSET_BOX *stco = (MP4_CHUNK_OFFSET_BOX *)data;
	BYTE *array = (BYTE *)(data + sizeof(MP4_CHUNK_OFFSET_BOX));
	BYTE *soucre = NULL;
	UINT stcl_len = 0;

	UINT size                       = sizeof(MP4_CHUNK_OFFSET_BOX);

	if (vid_type == VID_VIDEO) {
		stcl_len                    = mp4para.Vstbl->stco_len;
		soucre                      = (BYTE *)mp4para.Vstbl->stco;
	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		stcl_len                    = mp4para.Astbl->stco_len;
		soucre                      = (BYTE *)mp4para.Astbl->stco;
	}
#endif
	if (stcl_len) {
		BDmaMemCpy_rtos_flush(0, array, soucre, stcl_len * sizeof(UINT));
		size                       += (stcl_len * sizeof(UINT));
	}

	stco->mhData.dwVersion_Flags    = vidswapSB32(0);
	stco->mhData.dwEntryCount       = vidswapSB32(stcl_len);

	stco->fcccb.fcc                 = FCC('s','t','c','o');
	stco->fcccb.cb	                = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_stbl(BYTE *data, UINT vid_type)
{
	FCC_CB_t *stbl = (FCC_CB_t *)data;

	UINT size                       = sizeof(FCC_CB_t);
	size                           += enx_mp4_header_stsd(data + size, vid_type);
	size                           += enx_mp4_header_stts(data + size, vid_type);
	size                           += enx_mp4_header_stsc(data + size, vid_type);
	if (vid_type == VID_VIDEO) {
		size                       += enx_mp4_header_stss(data + size);
	}
	size                           += enx_mp4_header_stsz(data + size, vid_type);
	size                           += enx_mp4_header_stco(data + size, vid_type);

	stbl->fcc                       = FCC('s','t','b','l');
	stbl->cb                        = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_minf(BYTE *data, UINT vid_type)
{
	FCC_CB_t *minf = (FCC_CB_t *)data;

	UINT size                       = sizeof(FCC_CB_t);
	if (vid_type == VID_VIDEO) {
		size                       += enx_mp4_header_vmhd(data + size);
	}
#if (FAT_SDSAVE_AUD==1)
	else if (vid_type == VID_AUDIO) {
		size                       += enx_mp4_header_smhd(data + size);
	}
#endif
	size                           += enx_mp4_header_dinf(data + size, vid_type);
	size                           += enx_mp4_header_stbl(data + size, vid_type);

	minf->fcc                       = FCC('m','i','n','f');
	minf->cb                        = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_mdia(BYTE *data, UINT vid_type)
{
	FCC_CB_t *mdia = (FCC_CB_t *)data;

	UINT size                       = sizeof(FCC_CB_t);
	size                           += enx_mp4_header_mdhd(data + size, vid_type);
	size                           += enx_mp4_header_hdlr(data + size, vid_type);
	size                           += enx_mp4_header_minf(data + size, vid_type);

	mdia->fcc                       = FCC('m','d','i','a');
	mdia->cb                        = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_trak(BYTE *data, UINT vid_type)
{
	FCC_CB_t *trak = (FCC_CB_t *)data;

	UINT size                       = sizeof(FCC_CB_t);
	size                           += enx_mp4_header_tkhd(data + size, vid_type);
	size                           += enx_mp4_header_edts(data + size, vid_type);
	size                           += enx_mp4_header_mdia(data + size, vid_type);

	trak->fcc                       = FCC('t','r','a','k');
	trak->cb                        = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_free(BYTE *data, UINT free_size)
{
	FCC_CB_t *free = (FCC_CB_t *)data;

	UINT size                       = sizeof(FCC_CB_t) + free_size;

	free->fcc                       = FCC('f','r','e','e');
	free->cb                        = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_moov(BYTE *data)
{
	FCC_CB_t *moov = (FCC_CB_t *)data;

	UINT size                       = sizeof(FCC_CB_t);
	size                           += enx_mp4_header_mvhd(data + size);
	if (mp4para.mp4a->option & VID_VIDEO) {
		size                       += enx_mp4_header_trak(data + size, VID_VIDEO);
	}
#if (FAT_SDSAVE_AUD==1)
	if (mp4para.mp4a->option & VID_AUDIO) {
		size                       += enx_mp4_header_trak(data + size, VID_AUDIO);
	}
#endif
	int hdr_free = mp4para.but_t->hdr_size - mp4para.but_t->hdr_len - size - 16;
	if (hdr_free < 0) {
		flprintf("MP4 Header overflow(%d/%d/%d)\r\n", size, mp4para.but_t->hdr_len, mp4para.but_t->hdr_size);
	}
	size                           += enx_mp4_header_free(data + size, hdr_free);

	moov->fcc                       = FCC('m','o','o','v');
	moov->cb                        = vidswapSB32(size);

	return size;
}

UINT enx_mp4_header_mdat(BYTE *data)
{
	FCC_CB_t *mdat = (FCC_CB_t *)data;

	UINT size = sizeof(FCC_CB_t) + mp4para.mp4a->mdat_size;

	mdat->fcc                       = FCC('m','d','a','t');
	mdat->cb                        = vidswapSB32(size);

	return size;
}

BYTE *mp4_write_header(t_mp4_t *mp4a, void *buffer)
{
	t_videnc_totalbuf_t *buf_t = (t_videnc_totalbuf_t *)buffer; 
	BYTE *data = buf_t->hdr_info;
	ULONG a = BenchTimeStart();
	mp4_init_header_build(mp4a, buf_t);

	buf_t->hdr_len = 0;
	buf_t->hdr_len += enx_mp4_header_ftyp(data);
	buf_t->hdr_len += enx_mp4_header_moov(data + buf_t->hdr_len);
	buf_t->hdr_len += enx_mp4_header_mdat(data + buf_t->hdr_len);
	UINT b = BenchTimeStop(a);
	printf("make mp4 header: %uus\n", b);
	return data;
}
#endif
#endif
