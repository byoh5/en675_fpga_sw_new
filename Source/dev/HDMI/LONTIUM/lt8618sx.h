/******************************************************************************
  * @project: LT8618SX
  * @file: lt8618sx.c
  * @author: xhguo
  * @company: LONTIUM COPYRIGHT and CONFIDENTIAL
  * @date: 2017.07.25
******************************************************************************/

#ifndef		_LT8618SX_H
#define		_LT8618SX_H

typedef	unsigned char	uint8_t;
typedef	unsigned int	uint32_t;
typedef	unsigned short	uint16_t;
typedef	unsigned char	u8;

//#define   LT8618SX_ADR			0x76		//CI2CA HIGH : 0x76
#define   LT8618SX_ADR			0x72		//CI2CA LOW : 0x72
#define   LT8618SX_ADR_last		0x7e

//-------------------------------------------------------------------------------------------------
// Video timing information
//-------------------------------------------------------------------------------------------------
typedef struct video_timing{
	uint16_t hfp;
	uint16_t hs;
	uint16_t hbp;
	uint16_t hact;
	uint16_t htotal;
	uint16_t vfp;
	uint16_t vs;
	uint16_t vbp;
	uint16_t vact;
	uint16_t vtotal;
} video_timing;

//-------------------------------------------------------------------------------------------------
// Audio mode
//-------------------------------------------------------------------------------------------------
#define	I2S_2CH		0
#define	I2S_8CH		1
#define	SPDIF		2

#define Audio_Input_Mode	I2S_2CH

//-------------------------------------------------------------------------------------------------
// Video input mode
//-------------------------------------------------------------------------------------------------
#define Input_RGB888			0
#define Input_RGB565			1
#define Input_YCbCr444			2
#define Input_YCbCr422_16BIT	3
#define Input_YCbCr422_20BIT	4
#define Input_YCbCr422_24BIT	5
#define Input_BT1120_16BIT		6
#define Input_BT1120_20BIT		7
#define Input_BT1120_24BIT		8
#define Input_BT656_8BIT		9
#define Input_BT656_10BIT		10
#define Input_BT656_12BIT		11

#define Video_Input_Mode 		Input_BT1120_16BIT

//-------------------------------------------------------------------------------------------------
// Video output mode
//-------------------------------------------------------------------------------------------------
#define Output_RGB888			0
#define Output_YCbCr444			1
#define Output_YCbCr422_16BIT	2
#define Output_YCbCr422_20BIT	3
#define Output_YCbCr422_24BIT	4

#define Video_Output_Mode  		Output_RGB888

//-------------------------------------------------------------------------------------------------
// extern function
//-------------------------------------------------------------------------------------------------
extern video_timing const* in_video_timing;
extern void LT8618SX_Init(uint8_t VideoFmt);

#endif	// #ifndef _LT8618SX_H
