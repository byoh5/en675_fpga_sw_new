#ifndef _VIDEOCHANGE_H_
#define _VIDEOCHANGE_H_

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//
enum {
//								= 0x00000001,
//								= 0x00000002,
	UIN_VIDEO_JPEG_SIZE			= 0x00000004,
	UIN_VIDEO_JPEG_QUALITY		= 0x00000008,
	UIN_VIDEO_JPEG_FPS			= 0x00000010,
	UIN_VIDEO_JPEG_BRMODE		= 0x00000020,
	UIN_VIDEO_JPEG_BRATE		= 0x00000040,
//								= 0x00000080,
	UIN_VIDEO_H264_1CH_PROFILE	= 0x00000100,
	UIN_VIDEO_H264_1CH_SIZE		= 0x00000200,
	UIN_VIDEO_H264_1CH_QUALITY	= 0x00000400,
	UIN_VIDEO_H264_1CH_IDRFRAME	= 0x00000800,
	UIN_VIDEO_H264_1CH_FPS		= 0x00001000,
	UIN_VIDEO_H264_1CH_BRMODE	= 0x00002000,
	UIN_VIDEO_H264_1CH_BRATE	= 0x00004000,
//								= 0x00008000,
	UIN_VIDEO_H264_2CH_PROFILE	= 0x00010000,
	UIN_VIDEO_H264_2CH_SIZE		= 0x00020000,
	UIN_VIDEO_H264_2CH_QUALITY	= 0x00040000,
	UIN_VIDEO_H264_2CH_IDRFRAME	= 0x00080000,
	UIN_VIDEO_H264_2CH_FPS		= 0x00100000,
	UIN_VIDEO_H264_2CH_BRMODE	= 0x00200000,
	UIN_VIDEO_H264_2CH_BRATE	= 0x00400000,
//								= 0x00800000,
	UIN_STREAM_PORT				= 0x01000000,
	UIN_STREAM_USER_ID			= 0x02000000,
	UIN_STREAM_USER_PW			= 0x04000000,
	UIN_STREAM_AUDIO			= 0x08000000,
//								= 0x10000000,
//								= 0x20000000,
//								= 0x40000000,
//								= 0x80000000,
};

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern void VideoChangeTask(void *pvParameters);


//-------------------------------------------------------------------------------------------------
// Variable
extern UINT unVideoChangeFlag;


#endif // _VIDEOCHANGE_H_