//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:     cfg_filesystem.h
//	Description:   EN675 Filesystem configure
//	Author:        Team 2
//
//	Note:          Here you will check the settings.
//
//	Ver   Date   Author  Changes
//	---  ------  ------  -------
//	0.1  190530  hjlee   support EN675
//------------------------------------------------------------------------------
#ifndef _CFG_FILESYSTEM_H_
#define _CFG_FILESYSTEM_H_

////////////////////////////////////////////////////////////////////////////////
// File System
////////////////////////////////////////////////////////////////////////////////
#if defined(__FILESYSTEM__)
#if (LOAD_FS_SDCARD==1)

	#define FAT_SDSAVE					DEF_OFF		// Start recording after booting.

	#define FAT_SDSAVE_FREE_ALLOC		1			// 0 : unuse
													//*1 : Use f_expand when recording
													
	#define VIDEO_SAVE_AVI				0			// 0 : Save MP4 File
													//*1 : Save AVI File
#if (FAT_SDSAVE_FREE_ALLOC==1)
	#define FAT_SDSAVE_FREE_ALLOC_SIZE	(48*1024*1024)
													// Set a multiple of 4MB, (* 1.2) is a buffer(margin).
													// 10Mbps 30sec = 37.5MB * 1.2 => 48MB
													// 10Mbps 60sec = 75.0MB * 1.2 => 92MB
													// 16Mbps 30sec = 60.0MB * 1.2 => 72MB
													// 16Mbps 60sec =120.0MB * 1.2 =>144MB
#endif

	#define FAT_SDSAVE_AUD				0			// 0 : unuse
													//*1 : Save audio to an AVI/MP4 file

	#define FAT_SDSAVE_TXT				0			//*0 : unuse
													// 1 : Save text(Gsensor, GPS...) to an AVI file(Test...)

	#define JPEG_SDSAVE					1			// 0 : unuse
													//*1 : Save JPEG still shots to SD card

	#define FAT_SDSAVE_EVENT			0			// 0 : unuse
													// 1 : Event(motion, manual, etc..) recording

	#define VID_BUFFERING				5			// Caution, This value should fixed. = 2s
	#define NUM_STREAM_VIDEO 			(VID_BUFFERING*30)
#if (FAT_SDSAVE_AUD==1)
	#define NUM_STREAM_AUDIO			(VID_BUFFERING*G711_RecordingHz)	// 1f = 100ms => 5s = 50f
#else
	#define NUM_STREAM_AUDIO			(0)
#endif
#if (FAT_SDSAVE_TXT==1)
	#define NUM_STREAM_TXTS				(VID_BUFFERING*GSS_RecordingHz)
#else
	#define NUM_STREAM_TXTS				(0)
#endif
	#define NUM_STREAM_TOTAL			(NUM_STREAM_VIDEO+NUM_STREAM_AUDIO+NUM_STREAM_TXTS)
#endif
#endif

	#define SDIO0_SAFE_MODE		0					//*0: 50MHz - High speed mode(default)
													// 1: 33MHz - High speed mode(low speed)

#endif //_CFG_FILESYSTEM_H_
