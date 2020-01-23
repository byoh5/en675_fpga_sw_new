/* **************************************************************************
 File Name  :   isp_sensor_parameter.h
 Description:   EN675 - Sensor Parameter
 Designer   :   Kim, Sunghoon
 Date       :   19. 7. 3
 Copyright �� Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#ifndef __ISP_SENSOR_PARAMETER_H__
#define __ISP_SENSOR_PARAMETER_H__

//------------------------------------------------------------------------------
// Classify according to Sensor.
#if model_Sens ==        SENS_IMX291
	#define SP(a) a##__##SENS_IMX291
#elif model_Sens ==      SENS_IMX327
	#define SP(a) a##__##SENS_IMX327
#elif model_Sens ==      SENS_SPC1
	#define SP(a) a##__##SENS_SPC1
#elif model_Sens ==      SENS_IMX335
	#define SP(a) a##__##SENS_IMX335
#elif model_Sens ==      SENS_OS05A10
	#define SP(a) a##__##SENS_OS05A10
#elif model_Sens ==      SENS_IMX334
	#define SP(a) a##__##SENS_IMX334
#elif model_Sens ==      SENS_IMX274
	#define SP(a) a##__##SENS_IMX274
#elif model_Sens ==      SENS_IMX415
	#define SP(a) a##__##SENS_IMX415
#elif model_Sens ==      SENS_OS08A10
	#define SP(a) a##__##SENS_OS08A10
#elif model_Sens ==      SENS_IMX225
	#define SP(a) a##__##SENS_IMX225
#elif model_Sens ==      SENS_IMX323
	#define SP(a) a##__##SENS_IMX323
#elif model_Sens ==      SENS_OV2718
	#define SP(a) a##__##SENS_OV2718
#elif model_Sens ==      SENS_OV4689
	#define SP(a) a##__##SENS_OV4689
#else
	//#define SP(a) a##__##NOSENS
	//#define SP(a) a
#endif

#if	  model_1M
	#define RP(a) a##__##1M
#elif model_2M || model_2M30p
	#define RP(a) a##__##2M
#elif model_4M
	#define RP(a) a##__##4M
#elif model_8M
	#define RP(a) a##__##8M
#else

#endif

//------------------------------------------------------------------------------
// Sensor Parameter
#define SP_SET(NAME,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10,V11,V12,V13,V14,V15,V16,V17,V18,V19,V20) SENS_SEL(NAME,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10,V11,V12,V13,V14,V15,V16,V17,V18,V19,V20,\
\
					OV2718, IMX291,OS08A10, IMX225, IMX335, IMX274, OV4689, IMX415, IMX323, none10, none11, none12, none13, none14, none15, none16, none17, none18, none19, none20)
SP_SET(LckDly,		     7,      4,      1,      2,      2,      6,      4,      4,      7,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 7,  Lvds/Mipi input clock delay adjust
SP_SET(MipiClkPhase,     0,      0,      0,      2,      2,      0,      2,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Select Mipi 1/4 Clock Phase 0, 1, 2, 3, image align�� ������ �־� ������ ���鼭 �� ���� �����ؾ� ��
SP_SET(MipiBit,		     0,      0,      0,     10,     12,     12,     10,     12,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)

SP_SET(IsSlave,		     1,      0,      1,      1,      1,      1,      1,      1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 : Master Mode, 1 : Slave Mode,  Isp master/slave mode�� ����
SP_SET(IsASync,		     1,      0,      1,      0,      1,      0,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Auto sync generation, omni sensor�� ���� Active �̿��� �������� sync�� ������ �ʴ� sensor���� 1�� ����
SP_SET(IsNSync,		     1,      0,      0,      1,      1,      0,      1,      0,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// �Է� sync�� H/V ������ ��ġ���� �ʴ� ��Ȳ���� 1�� ����
SP_SET(PreHSyncOfs,	  0x16,   0x12,   0xa8,   0xb8,   0xb8,   0x12,   0xb8,   0xa8,   0x8e,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Pre module Horizontal Sync Offset, Hsp ���ڷ� Image ��ġ�� ���� ������ Sync�� ��ġ�� �̵��Ͽ� image ��ġ�� ���ߴµ� ���
SP_SET(PreVSyncOfs,	   0xa,    0xa,    0xa,    0xa,    0xa,    0xa,    0xa,    0xa,    0xf,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Pre module Vertical Sync Offset, Vsp ���ڷ� Image ��ġ�� ���� ������ Sync�� ��ġ�� �̵��Ͽ� image ��ġ�� ���ߴµ� ���
SP_SET(PreHsp,		  0x13,   0x15,   0x15,    0x8,    0x8,   0x15,    0x8,   0x15,    0x7,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Pre module Horizontal image start position
SP_SET(PreVsp,		   0x9,    0x7,    0x3,    0x5,    0x5,    0x7,    0x5,    0x3,    0x7,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Pre module Vertical image start position
SP_SET(PreHSyncPol,	     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 : Falling position, 1 : Rising position, Pre module Horizontal sync input polarity
SP_SET(PreVSyncPol,	     0,      0,      1,      1,      1,      1,      1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 : Falling position, 1 : Rising position, Pre module Vertical sync input polarity
SP_SET(ICSel,		     0,      0,      3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 3,  Pre module color phase selection, RGB interpolator�� ������ ����, AWB ODM ������ �ùٸ��� Ȯ��(AWBLNOR_SEL:1, û������:CRDR- CBDB+, Ȳ������:CRDR+ CBDB-)		TODO KSH �� ICSEL ���� �ʿ�
SP_SET(OCSel,		     0,      2,      1,      3,      3,      2,      2,      0,      2,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 3,  Post module color phase selection, RGB interpolator�� ������ ����, ��´� VLOCKO_IT_POS0w(OutVSyncOfs) & HLOCKO_IT_POS0w(OutHSyncOfs) ������ ���� OCSel���� �߸� ������ �� ������ ����!!!	TODO KSH �� OCSEL ���� �ʿ�, IMX291 ���� �Ϸ�

SP_SET(Dnr2dICSel,	     0,      0,      3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 3,  2D-DNR ICSEL, DNR2D_ON ON/OFF �� Color ��ȭ ����� ��		TODO KSH �� 2D-DNR ICSEL ���� �ʿ�, IMX291 ���� �Ϸ�
SP_SET(Dnr2dOCSel,	     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 3,  2D-DNR OCSEL, �������� Ȯ�� ����, �׻� 0���� ����

//SP_SET(HSUP_TH,	  0x74,   0x70,   0x72,   0x78,   0x72,   0x72,   0x72,   0x72,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// High Light color suppression threshold (Y domain control) (0 ~ 0xFF)
SP_SET(HSUP_TH,		  0xb4,   0xb8,   0xc0,   0xb8,   0xb2,   0xb2,   0xb2,   0xb2,   0xb8,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// High Light color suppression threshold (Y domain control) (0 ~ 0xFF)
//SP_SET(HSUP_TH,	  0xa4,   0xa0,   0xa2,   0xa8,   0xa2,   0xa2,   0xa2,   0xa2,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// High Light color suppression threshold (Y domain control) (0 ~ 0xFF)

SP_SET(AWB_DB_MAX,	    32,     69,    -78,    -78,   -177,   -154,   0x00,   0x00,    -19,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// TODO KSH �� OV4689 �� IMX415 �� AWB_DB_MAX �� Ȯ�� �ʿ�
SP_SET(AWB_DB_MIN,	  -787,   -808,   -720,   -768,   -744,   -745,   0x00,   0x00,   -756,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 		"
SP_SET(AWB_DR_MAX,	   544,    610,    392,    452,    478,    482,   0x00,   0x00,    592,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 		"
SP_SET(AWB_DR_MIN,	  -346,   -405,   -346,   -299,   -145,   -178,   0x00,   0x00,   -269,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 		"

// 191010 KSH OS08A10 OFFSET 0x39,0xed -> 0x60,0xa7
SP_SET(AWB_DBOFFSET,  0x26,   0x51,   0x60,   0x22,   0x10,   0x2e,   0x00,   0x00,   0x46,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 		"
SP_SET(AWB_DROFFSET,  0xc5,   0xa1,   0xa7,   0xc5,  0x102,  0x10e,   0x00,   0x00,  0x118,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 		"
SP_SET(AWB_DBGAIN,	  0x0d,   0x0e,   0x0f,   0x0e,   0x0f,   0x0f,   0x00,   0x00,   0x10,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 		"
SP_SET(AWB_DRGAIN,	  0x0c,   0x0d,   0x0d,   0x0c,   0x0d,   0x0d,   0x00,   0x00,    0xd,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 		"

SP_SET(ShtBrtSpeed,	   300,    350,    350,    350,    350,    350,    350,    350,    350,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// AE SPEED �� SHT. SPEED �� BRIGHTEN : 1 ~ 999 (3E7h), Sensor�� ���� ��Ⱑ Ŭ����(�Ǵ� FPS�� ������ ����) Shutter ���� Speed �������� ������
SP_SET(ShtDrkSpeed,	   160,    350,    300,    350,    350,    350,    350,    350,    350,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// AE SPEED �� SHT. SPEED �� DARKEN : 1 ~ 999 (3E7h), Sensor�� ���� ��Ⱑ Ŭ����(�Ǵ� FPS�� ������ ����) Shutter ���� Speed �������� ������
SP_SET(AgcBrtSpeed,	   384,     42,    150,     42,     42,     42,     42,     42,     42,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// AE SPEED �� AGC SPEED �� BRIGHTEN : 1 ~ 999 (3E7h), AGC�� ���� �ִ� ��Ⱑ ũ�� AGC_MAX���� �������� AGC ���� Speed�� ������, EN675������ = EN673������*0.2
SP_SET(AgcDrkSpeed,	   384,     26,    150,     26,     26,     26,     26,     26,     26,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// AE SPEED �� AGC SPEED �� DARKEN : 1 ~ 999 (3E7h), AGC�� ���� �ִ� ��Ⱑ ũ�� AGC_MAX���� �������� AGC ���� Speed�� ������, EN675������ = EN673������*0.2

SP_SET(LWDR_2P_SGAIN_ALP,0,      0,      0,      0,   0x80,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// EN781�� AE_LWDR_SGAIN_ALP : IMX290 & IMX327 & IMX307 = 0x700 / IMX335 5M = 0x100		TODO KSH �� WDR - LWDR_2P_SGAIN_ALP �� Sensor ���� Ʃ�� �ʿ�

SP_SET(SyncCode0,	 0xfff,  0xfff,  0xfff,      0,      0,      0,      0,      0,  0xfff,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncCode1,	     0,      0,      0,      1,      1,      1,      1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncCode2,	     0,      0,      0,      2,      2,      2,      2,      2,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncCode3_0,	     0,      0,      0,      3,      3,      3,      3,      3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncCode3_1,	 0x800,  0x800,  0x200,   0xb8,   0xb8,   0xb8,   0xb8,   0xb8,  0x800,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncCode3_2,	 0xab0,  0xab0,  0x280,  0xf30,  0xf30,  0xf30,  0xd20, 0x16a4,  0xab0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncS0Code3_0,    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncS0Code3_1,    0,      0,      0, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncS0Code3_2,    0,      0,      0,   0x2b,   0x2c,   0x2c,   0x2b,   0x2c,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncS1Code3_0,    0,      0,      0,      3,      3,      3,      3,      3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncS1Code3_1,    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
SP_SET(SyncS1Code3_2,    0,      0,      0,      3,      3,      3,  0xd20,      3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)

#if	(model_Sens_Fps==60)
SP_SET(SensorClk,	     0,     37,      0,     27,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
SP_SET(PreClk,		     0, C_148M,      0,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#elif (model_Sens_Fps==30)
SP_SET(SensorClk,	    13,     37,      0,      0,     13,      0,     27,      0,     37,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
SP_SET(PreClk,		 C_74M,  C_74M,      0,      0,  C_74M,      0,  C_74M,      0,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#elif (model_Sens_Fps==20)
SP_SET(SensorClk,	     0,      0,      0,      0,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
SP_SET(PreClk,		     0,      0,      0,      0,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#elif (model_Sens_Fps==15)
SP_SET(SensorClk,	     0,      0,     13,      0,      0,     13,     13,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
SP_SET(PreClk,		     0,      0,  C_74M,      0,      0,  C_74M,  C_74M,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#elif (model_Sens_Fps==12)
SP_SET(SensorClk,	     0,      0,      0,      0,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
SP_SET(PreClk,		     0,      0,      0,      0,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#endif

SP_SET(PostClk,		 C_74M,  C_74M,  C_74M,  C_74M,  C_74M,  C_74M,  C_74M,  C_74M,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0


//------------------------------------------------------------------------------
// Resolution Parameter
#define RP_SET(NAME,V1,V2,V3,V4,V5) RP_SEL(NAME,V1,V2,V3,V4,V5,\
\
					    1M,     2M,     4M,     8M,     5M)
RP_SET(FR_HW,		  1280,   1920,   2560,   3840,   2592)	// Digital Output formatter horizontal active width
RP_SET(FR_VW,		   720,   1080,   1440,   2160,   1944)	// Digital Output formatter vertical   active width
RP_SET(PO_HW,		  1288,   1928,   2568,   3848,      0)	// HWI(pre module active image horizontal width) & HWO(Post module horizontal image active width)
RP_SET(PO_VW,		   728,   1088,   1448,   2168,      0)	// VWI(pre module active image vertical   width) & VWO(Post module vertical   image active width)
RP_SET(FR_HTW60,	  1650,   2200,   3300,   4400,      0)	// Total horizontal width for 30P/60P
RP_SET(FR_VTW60,	   750,   1125,   1500,   2250,      0)	// Total vertical   width for 30P/60P
RP_SET(FR_HTW50,	  1980,   2640,   3960,   5280,      0)	// Total horizontal width for 25P/50P
RP_SET(FR_VTW50,	   750,   1125,   1500,   2250,      0)	// Total vertical   width for 25P/60P

RP_SET(OutHSyncOfs,	  0x82,   0x80,   0x82,   0x80,      0)	// 0 ~ 0x1fff,  Digital Output formatter Horizontal sync offset, OCSel�� ������ �ֹǷ� ������ �������� �ʴ� ���� ����, �� ���� �����ص� ������ ȭ�鿡 ä������ ������ pre/post���� Hsync offse�̳� Hsp�� �����ؾ� ��	TODO KSH - OutHSyncOfs : 2M�� ���� �Ϸ�
RP_SET(OutVSyncOfs,	   0x3,  0x449,  0x5bd,  0x8a8,      0)	// 0 ~ 0xfff,   Digital Output formatter Vertical   sync offset, OCSel�� ������ �ֹǷ� ������ �������� �ʴ� ���� ����, �� ���� �����ص� ������ ȭ�鿡 ä������ ������ pre/post���� Vsync offse�̳� Vsp�� �����ؾ� ��	TODO KSH - OutVSyncOfs : 2M�� ���� �Ϸ�

RP_SET(AE_HSP,		   0x8,    0x8,   0x10,   0x10,      0)
RP_SET(AE_HEP,		 0x274,  0x3ba,  0x4f8,  0x77a,      0)
RP_SET(AE_VSP,		   0x8,    0x8,   0x14,   0x14,      0)
RP_SET(AE_VEP,		 0x2c4,  0x434,  0x598,  0x865,      0)

//RP_SET(DF_HSP,		     1,      1,      1,      1,      0)
//RP_SET(DF_VSP,		     5,      5,      5,      5,      0)

RP_SET(AWB_HSP,		   0x3,    0x0,    0xd,    0xf,      0)	// 2M���� 0x8���� 0x0���� ����
RP_SET(AWB_HW,		 0x279,  0x3b0,  0x4ea,  0x770,      0)
RP_SET(AWB_VSP,		   0x9,   0x10,   0x14,   0x14,      0)
RP_SET(AWB_VW,		 0x2c0,  0x420,  0x583,  0x850,      0)

RP_SET(BOX_HSP,		  0x7e,   0x7e,   0x7e,   0x7e,      0)	// TODO KSH - BOX_HSP : 2M�� ���� �Ϸ�,	IMD BG BOX�� ����, OutHSyncOfs ���� �� �����ϰ� �����ؾ� ��
RP_SET(BOX_VSP,		   0xd,    0xd,    0xd,    0xd,      0)	// TODO KSH - BOX_VSP : 2M�� ���� �Ϸ�,	IMD BG BOX�� ����

RP_SET(ACE_HB,		   0xA,    0xF,    0xA,    0xF,      0)	// number of ACE horizontal block
RP_SET(ACE_VB,		   0x5,    0x8,    0x6,    0x8,      0)	// number of ACE vertical   block
RP_SET(DEFOG_HW,	   0xa,    0xa,    0xb,    0xb,      0)	// Defog Area Set Horizontal Width (0x0 ~ 0xf)
RP_SET(DEFOG_HSP,	  0xa0,  0x1c0,  0x10b,  0x3a0,      0)	// Defog Area Set Horizontal Start Position (0x0 ~ 0x790)
RP_SET(DEFOG_VW,	   0x9,    0xa,    0xa,    0xa,      0)	// Defog Area Set Vertical Width (0x0 ~ 0xf)
RP_SET(DEFOG_VSP,	  0x60,   0x14,   0x16,   0x50,      0)	// Defog Area Set Vertical Start Position (0x0 ~ 0x440)

RP_SET(FONT_OFY,	  0x36,    0xf,    0xf,   0x30,      0)	// Font Vertical Position Offset

//------------------------------------------------------------------------------
// Initial value setting
#if model_1M
	enum{	// UP(OutFps)
		UP_1M_720p_25_30,	// IMX225
		UP_1M_720p_50_60	// IMX225
		//UP_1M_720p_100_120,
		//UP_1M_720p_200_240
	};
	#if model_60fps
		#define OUT_FPS_DEF		UP_1M_720p_50_60
	#elif model_30fps
		#define OUT_FPS_DEF		UP_1M_720p_25_30
	#endif
	#define FPS_VDOr			((UP(OutFps)==UP_1M_720p_50_60) ? ((UP(SysFreq) == UP_SYSFREQ_50) ? 50 : 60) : ((UP(SysFreq) == UP_SYSFREQ_50) ? 25 : 30))
#elif model_2M30p
	enum{	// UP(OutFps)
		UP_2M_1080p_25_30,	// IMX291, OV2718
		UP_2M_720p_25_30,
	};
	#define OUT_FPS_DEF			UP_2M_1080p_25_30
	#define FPS_VDOr			((UP(SysFreq) == UP_SYSFREQ_50) ? 25 : 30)
#elif model_2M
	enum{	// UP(OutFps)
		UP_2M_1080p_25_30,	// IMX291, OV2718
		UP_2M_1080p_50_60,	// IMX291
		UP_2M_720p_25_30,
		UP_2M_720p_50_60
		//UP_2M_1080i_50_60,
		//UP_2M_1080p_100_120
	};
	#if model_60fps
		#define OUT_FPS_DEF		UP_2M_1080p_50_60
	#elif model_30fps
		#define OUT_FPS_DEF		UP_2M_1080p_25_30
	#endif
	#define FPS_VDOr			(((UP(OutFps)==UP_2M_1080p_50_60)||(UP(OutFps)==UP_2M_720p_50_60)) ? ((UP(SysFreq) == UP_SYSFREQ_50) ? 50 : 60) : ((UP(SysFreq) == UP_SYSFREQ_50) ? 25 : 30))
#elif model_4M
	enum{	// UP(OutFps)
		UP_4M_1440p_25_30,	// IMX335, OV4689
		UP_4M_1080p_25_30,
		UP_4M_720p_25_30,

		UP_4M_1440p_20,		// IMX335
		UP_4M_1440p_15,		// OV4689
		UP_4M_1440p_12		// IMX335

		//UP_5M_1944p_20,
		//UP_5M_1944p_12
	};
	#if model_30fps
		#define OUT_FPS_DEF		UP_4M_1440p_25_30
	#elif model_20fps
		#define OUT_FPS_DEF		UP_4M_1440p_20
	#elif model_15fps
		#define OUT_FPS_DEF		UP_4M_1440p_15
	#elif model_12fps
		#define OUT_FPS_DEF		UP_4M_1440p_12
	#endif
	#define FPS_VDOr			((UP(OutFps) == UP_4M_1440p_20) ? 20 : (UP(OutFps) == UP_4M_1440p_15) ? 15 : (UP(OutFps) == UP_4M_1440p_12) ? 12 : (UP(SysFreq) == UP_SYSFREQ_50) ? 25 : 30)
#elif model_8M
	enum{	// UP(OutFps)
		UP_8M_2160p_12_15	// IMX274, IMX415, OS08A10
		//UP_8M_2160p_25_30
		//UP_8M_2160p_50_60
	};
	#define OUT_FPS_DEF			UP_8M_2160p_12_15
	#define FPS_VDOr			((UP(SysFreq) == UP_SYSFREQ_50) ? 12 : 15)
#endif

#define FPS_VDIr	FPS_VDOr	// TODO KSH �� FPS_VDI �� FPS_VDO�� �ٸ� ��� ���� �ʿ�


#if model_1M || model_2M
	#define FPS_LOW				((FPS_VDO==25)||(FPS_VDO==30))
#else
	#define FPS_LOW				1
#endif
#define FPS_HI		(!FPS_LOW)


#if model_1M
	#define UpAE_SHT_SLICE_LINE			2
	#define UpAE_OUTDOOR_THRS_VAL		2
	#define UpAE_DEBLUR_THRS_VAL		2
#else
	#define UpAE_SHT_SLICE_LINE			3	// Shutter limit (line)
	#define UpAE_OUTDOOR_THRS_VAL		3	// shutter threshold in OUTDOOR mode (ex. 1125>>3 =  375 => 1/240s)
	#define UpAE_DEBLUR_THRS_VAL		3	// shutter threshold in DEBLUR mode (ex. 1125>>3 =  375 => 1/240s)
#endif


//------------------------------------------------------------------------------
// ISP Function define
#define IspSensorPowerOn()		Isp_SensorPowerOn(FN_ON,SP(SensorClk))

#define IspSDesPowerOn()		Isp_SDesPowerOn(model_Sens_Intf>0, model_Sens_Intf==2, UP(MipiClkPhase))

#if (model_Sens_Intf==0)	// Parallel
	#define IspSDesConfig()		Isp_Parallel_Config(FN_ON/*, ISP_SLAVE*/, PARA_INTER_CLK, PARA_CLK_SDR, 0/*, SYNC_FALL, SYNC_RISE, DIFF_EDGE, NO_USE_AUTOSYNC*/, BIT12TOBit12)
#elif (model_Sens_Intf==1)	// LVDS
	#define IspSDesConfig()		Isp_Lvds_Config(SP(LvdsBit), LVDS_4LANE, 0, UP(LvdsPNSel), SP(LvdsSofNo))
#elif (model_Sens_Intf==2)	// MIPI
	#define IspSDesConfig()		Isp_Mipi_Config(SP(MipiBit), MIPI_4LANE, 0, 1, UP(MipiHSyncOfs), USE_ECC_CHECK, SP(MipiUseWcl), NO_USE_CHECK, NO_USE_CHECK, 3)
#endif

#define IspSDesDelay()			Isp_SDesDelay(UP(LckDly))

#define IspSYNC_CODE()			Isp_SYNC_CODE(SP(SyncCode0), SP(SyncCode1), SP(SyncCode2), SP(SyncCode3_0), SP(SyncCode3_1), SP(SyncCode3_2),\
											  SP(SyncS0Code3_0), SP(SyncS0Code3_1), SP(SyncS0Code3_2), SP(SyncS1Code3_0), SP(SyncS1Code3_1), SP(SyncS1Code3_2))

#define IspPreClkConfig()		Isp_PreClk_Config(SP(PreClk), USE_FRC)
#define IspPostClkConfig()		Isp_PostClk_Config(SP(PostClk))

#define IspPreSyncConfig()		Isp_PreSync_Config(SP(IsSlave), gnAeHtw, gnAeVtw, UP(PreHSyncOfs), UP(PreVSyncOfs), UP(PreHsp), UP(PreVsp), RP(PO_HW), RP(PO_VW), SP(IsASync), SP(IsNSync), UP(PreHSyncPol), UP(PreVSyncPol))

#define IspPostSyncConfig()		Isp_PostSync_Config(1, 0, gnAeHtw, gnAeVtw, 0x3a, 0x2, 0x6, 0x4, RP(PO_HW), RP(PO_VW), UP(OCSel))

#define IspDout0SyncConfig()	Isp_Dout0_Sync_Config(gnAeHtw, UP(OutHSyncOfs), UP(OutVSyncOfs), 0, 0x2a, RP(FR_HW), RP(FR_VW))
#define IspDout1SyncConfig()	Isp_Dout1_Sync_Config(gnAeHtw, UP(OutHSyncOfs), UP(OutVSyncOfs), 0, 0x2a, RP(FR_HW), RP(FR_VW))

#define FONT_INIT()				FontInit(0x3a, RP(FONT_OFY), 1, RP(FR_HW))


//------------------------------------------------------------------------------
// Library Link
#define InitSensRun			SP(InitSensRun)
#define SensFlip			SP(SensFlip)
#define SensMirror			SP(SensMirror)

#define AGC_POS_MAX			SP(AGC_POS_MAX)

#define gbSensShtDly		SP(gbSensShtDly)
#define gbSensAgcDly		SP(gbSensAgcDly)
#define gbVHssAgcTbl		SP(gbVHssAgcTbl)
#define gVHssAgcTblEa		SP(gVHssAgcTblEa)
#define AeSHT0				SP(AeSHT0)
#define AeSHT1				SP(AeSHT1)
#define AeAGC0				SP(AeAGC0)
#define AGC2POS				SP(AGC2POS)
#define POS2AGC				SP(POS2AGC)

#define AeSHT_LWDR			SP(AeSHT_LWDR)
#define FrameSetCount		SP(FrameSetCount)
#define WdrSShtMax			SP(WdrSShtMax)
#define WdrLShtMax			SP(WdrLShtMax)
#define WdrMShtMax			SP(WdrMShtMax)
#define AeAGC_LWDR			SP(AeAGC_LWDR)

#define AE_SHTw				SP(AE_SHTw)
#define AE_DSSw				SP(AE_DSSw)
#define AE_AGCw				SP(AE_AGCw)
#define AE_FREQw			SP(AE_FREQw)
#define AE_ETCw				SP(AE_ETCw)
#define SensUpdate			SP(SensUpdate)

#define glMtxCf				SP(glMtxCf)


#endif//__ISP_SENSOR_PARAMETER_H__
