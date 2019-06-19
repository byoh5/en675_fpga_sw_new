//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:     cfg_media.h
//	Description:   EN675 Network configure
//	Author:        Team 2
//
//	Note:          Here you will check the settings.
//
//	Ver   Date   Author  Changes
//	---  ------  ------  -------
//	0.1  190530  hjlee   support EN675
//------------------------------------------------------------------------------
#ifndef __CFG_MEDIA_H__
#define __CFG_MEDIA_H__

////////////////////////////////////////////////////////////////////////////////
// Video
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Audio
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// PCM
#define PCM8K_RecordingHz				(20)

////////////////////////////////////////////////////////////////////////////////
// G.711
#define G711_KBitrate_MAX				(64)
#define G711_KBitrate_MIN				(64)
#define G711_KHzSampleRate_MAX			(8)
#define G711_KHzSampleRate_MIN			(8)
#define G711_RecordingHz				(10)

////////////////////////////////////////////////////////////////////////////////
#define AUD_SAMPLING_RATE 				8000
#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
#define AUD_SAMPLING_BIT  				16
#else
#define AUD_SAMPLING_BIT  				8
#endif

#endif //__CFG_MEDIA_H__