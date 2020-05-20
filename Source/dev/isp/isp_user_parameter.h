/* **************************************************************************
 File Name  :   isp_user_parameter.h
 Description:   EN675 - User Paramete
 Designer   :   Kim, Sunghoon
 Date       :   19. 6. 3
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#ifndef __ISP_USER_PARAMETER_H__
#define __ISP_USER_PARAMETER_H__
//*************************************************************************************************
// User Parameter Values
//-------------------------------------------------------------------------------------------------
enum{	// common
   	UP_OFF,
	UP_ON
};

enum {	// common
	UP_3sLOW,
	UP_3sMID,
	UP_3sHI
};

enum {	// common
   	UP_4sOFF,
	UP_4sLOW,
	UP_4sMID,
	UP_4sHI
};


#define	UP_LENS_MNL		0	// Iris, no iris
#define	UP_LENS_DC		1	// Iris, DC iris
#define	UP_LENS_AUTO	1	// Iris, pIRIS, AF AUTO


enum {	// UP(Shutter)
	UP_SHUT_AUTO,
	UP_SHUT_MNL,
	UP_SHUT_FLICKER
};

enum{	// UP(ShutSpd)
	UP_MNL_SHUT_DIV1,	// 1125 line, 1/30 sec
	UP_MNL_SHUT_DIV2,	// 562 line, 1/60 sec
	UP_MNL_SHUT_DIV4,	// 281 line, 1/120 sec
	UP_MNL_SHUT_DIV8,	// 140 line, 1/250 sec
	UP_MNL_SHUT_DIV16,	// 70 line, 1/500 sec
	UP_MNL_SHUT_DIV32,	// 35 line, 1/1000 sec
	UP_MNL_SHUT_DIV64,	// 17 line, 1/2000 sec
	UP_MNL_SHUT_DIV128,	// 8 line, 1/4000 sec
	UP_MNL_SHUT_DIV256,	// 4 line, 1/8000 sec
	UP_MNL_SHUT_DIV512,	// 2 line, 1/15000 sec
	UP_MNL_SHUT_DIV1024	// 1 line, 1/30000 sec
};

#define UP_SHT_MIN_DEFAULT	UP_MNL_SHUT_DIV1024		// TODO KSH ◆ Shutter 1 line 을 지원하지 않는 Sensor 주의!!!

enum{	// UP(DcMode)
	UP_DC_IN,
	UP_DC_OUT,
	UP_DC_DEBLUR,
	UP_DC_CUSTOM
};

enum{	// UP(ShtMode)
	UP_SHT_NORMAL,
	UP_SHT_DEBLUR,
	UP_SHT_CUSTOM
};

enum {	// UP(Dss)	// 14.2.13
	UP_DSS_OFF	,
	UP_DSS_X2	,
	UP_DSS_X4	,
	UP_DSS_X8	,
	UP_DSS_X16	,
	UP_DSS_X32	,
	UP_DSS_X64
};

enum {	// UP(SysFreq)
	UP_SYSFREQ_50,
	UP_SYSFREQ_60
};

enum{	// UP(BackLight)
	UP_BL_OFF,
	UP_BL_HLC,
	UP_BL_BLC,
	UP_BL_WDR
};

enum {	// UP(WdrMode)
	UP_WDR_FRAME,
	UP_WDR_LINE
};

enum {	// UP(BlcMode)
	UP_BLC_MODE_CUSTOMIZE,
	UP_BLC_MODE_CENTER,
	UP_BLC_MODE_SPOT
};

enum {	//UP(Tdn)
	UP_TDN_AUTO,
	UP_TDN_DAY,
	UP_TDN_NIGHT,
	UP_TDN_EXTERN
};

enum {	// UP(Awb)
	UP_AWB_AUTO,
	UP_AWB_AUTOEXT,
	UP_AWB_PRESET,
	UP_AWB_MNL
};

enum {
	UP_HUE_GAIN_NEW,
	UP_HUE_GAIN_OLD
};

enum {	// UP(AwbMnl)
	UP_3000K,
	UP_5000K,
	UP_8000K
};

enum {	//MP(MpGamma)
	UP_GAMMA_045,
	UP_GAMMA_050,
	UP_GAMMA_055,
	UP_GAMMA_060,
	UP_GAMMA_065,
	UP_GAMMA_070,
	UP_GAMMA_075,

	UP_GAMMA_AUTO,

	UP_GAMMA_WDR
};

enum{	//UP(DefogMode)
	UP_DEFOG_MANUAL,
	UP_DEFOG_AUTO
};

enum{	//UP(OutFormat)
	UP_FRMT_SD,
	UP_FRMT_HD,
	UP_FRMT_UV
};

enum {	// UP(LanguageSel)
	UP_ENG,		// English
	UP_CHN_T,	// Simplified Chinese
	UP_CHN_S,	// Traditional Chinese
	UP_JPN,
	UP_KOR
};

enum {	// UP(CamTitleOn)
	UP_CT_OFF,
	UP_CT_RIGHT_UP,
	UP_CT_LEFT_DOWN
};



#endif//__ISP_USER_PARAMETER_H__


//*************************************************************************************************
// User Parameter List to be used Library
//-------------------------------------------------------------------------------------------------
#define UP_LIB_LIST/* User Parameter List to be used Library */\
\
UP_LIB(Brightness)\
UP_LIB(ColorBar)\
UP_LIB(IspGain)\
UP_LIB(IspGainAeCur)\
\
UP_LIB(ErrChgStep)\
UP_LIB(SpotSpd)\
UP_LIB(Stabilizing)\
UP_LIB(ErrChtMax)\
UP_LIB(ErrChtMin)\
UP_LIB(LSpotLv)\
UP_LIB(SatBrt)\
UP_LIB(LSpotTh)\
UP_LIB(MinGammaY)\
UP_LIB(SmallSpeed)


//*************************************************************************************************
// User Parameter Style
//-------------------------------------------------------------------------------------------------
#define INIT_STYLE	1	// 1:IPC_INDOOR, 2:IPC_OUTDOOR, 3:CAR_REC, 4:ACTION_REC, 5:WDR, 6:LOW_BIT, 7:CUSTOM


#ifdef __ISP_MENU_STR__
STRING1(IPC_INDOOR,	_B,"IPC INDOOR");
STRING1(IPC_OUTDOOR,_B,"IPC OUTDOOR");
STRING1(CAR_REC,	_B,"CAR REC");
STRING1(ACTION_REC,	_B,"ACTION REC");
//STRING1(WDR,		_B,"WDR");			// 이미 isp_menu_str.h에 있음
STRING1(LOW_BIT,	_B,"LOW BIT");
STRING1(CUSTOM,		_B,"CUSTOM");
#endif


#define USR_PAR_STYLE \
/*                      IPC_INDOOR,      IPC_OUTDOOR,     CAR_REC,         ACTION_REC,      WDR,             LOW_BIT,         CUSTOM        */\
UP_STYLE(Brightness,    4,               6,               4,               4,               4,               4,               4              )\
UP_STYLE(Adnr2D,        8,               4,               8,               8,               8,               8,               8              )\
UP_STYLE(ShpBigEdge,    0x80,            0x40,            0x80,            0x80,            0x80,            0x80,            0x80           )\
UP_STYLE(Gamma,         UP_GAMMA_075,    UP_GAMMA_055,    UP_GAMMA_075,    UP_GAMMA_075,    UP_GAMMA_075,    UP_GAMMA_075,    UP_GAMMA_075   )\
UP_STYLE(Ace,           UP_4sHI,         UP_4sOFF,        UP_4sHI,         UP_4sHI,         UP_4sHI,         UP_4sHI,         UP_4sHI        )
// ※IMX323의 경우 ACE HIGH 가 오히려 Color가 안좋음

//*************************************************************************************************
// User Parameter List
//-------------------------------------------------------------------------------------------------
#define USR_PAR_LIST/* User Parameter List */\
\
UP_TITLE(SENSOR)/*--　SENSOR　--*/\
UP_SET(1, LckDly, SP(LckDly), ,  /*LCK_DLYw(UP(LckDly));*/ )/* 0 ~ 7,  Lvds/Mipi input clock delay adjust */\
UP_SET(1, MipiClkPhase, SP(MipiClkPhase), , /*LSYNCM_SELw(UP(MipiClkPhase));*/ )/* 0 ~ 3,  Select Mipi 1/4 Clock Phase 0, 1, 2, 3, image align에 영향을 주어 영상을 보면서 이 값을 조절해야 함 */\
UP_SET(2, PreHSyncOfs, SP(PreHSyncOfs), , HLOCKI_POSw(UP(PreHSyncOfs)); )/* 0 ~ 0x1fff,  Pre module Horizontal Sync Offset, Hsp 인자로 Image 위치가 맞지 않을때 Sync의 위치를 이동하여 image 위치를 맞추는데 사용 */\
UP_SET(2, PreVSyncOfs, SP(PreVSyncOfs), , VLOCKI_POSw(UP(PreVSyncOfs)); )/* 0 ~ 0xfff,  Pre module Vertical Sync Offset, Vsp 인자로 Image 위치가 맞지 않을때 Sync의 위치를 이동하여 image 위치를 맞추는데 사용 */\
UP_SET(1, PreHSyncPol, SP(PreHSyncPol), , POL_HSIw(UP(PreHSyncPol)); )/* 0 ~ 1,  0 : Falling position, 1 : Rising position, Pre module Horizontal sync input polarity */\
UP_SET(1, PreVSyncPol, SP(PreVSyncPol), , POL_VSIw(UP(PreVSyncPol)); )/* 0 ~ 1,  0 : Falling position, 1 : Rising position, Pre module Vertical sync input polarity */\
UP_SET(2, PreHsp, SP(PreHsp), , HSPIw(UP(PreHsp)); )/* 0 ~ 0x1fff,  Pre module Horizontal image start position */\
UP_SET(2, PreVsp, SP(PreVsp), , VSPIw(UP(PreVsp)); )/* 0 ~ 0xfff,  Pre module Vertical image start position */\
UP_SET(1, ICSel, SP(ICSel), , Flip(); )/* 0 ~ 3,  Pre module color phase selection, RGB interpolator의 위상을 조절, AWB ODM 값으로 올바른지 확인(AWBLNOR_SEL:1, 청색필터:CRDR- CBDB+, 황색필터:CRDR+ CBDB-) */\
UP_SET(1, OCSel, SP(OCSel), , Flip(); )/* 0 ~ 3,  Post module color phase selection, RGB interpolator의 위상을 조절, 출력단 VLOCKO_IT_POS0w(UP(OutVSyncOfs)) & HLOCKO_IT_POS0w(UP(OutHSyncOfs)) 설정에 따라 OCSel값을 잘못 설정할 수 있으니 주의!!! */\
UP_SET(2, OutHSyncOfs, RP(OutHSyncOfs), , HLOCKO_IT_POS0w(UP(OutHSyncOfs)); )/* 0 ~ 0x1fff,  Digital Output formatter Horizontal sync offset, OCSel에 영향을 주므로 가능한 변경하지 않는 것을 권장, 이 값을 조절해도 영상이 화면에 채워지지 않으면 pre/post단의 Hsync offse이나 Hsp를 조절해야 함 */\
UP_SET(2, OutVSyncOfs, RP(OutVSyncOfs), , VLOCKO_IT_POS0w(UP(OutVSyncOfs)); )/* 0 ~ 0xfff,  Digital Output formatter Vertical sync offset, OCSel에 영향을 주므로 가능한 변경하지 않는 것을 권장, 이 값을 조절해도 영상이 화면에 채워지지 않으면 pre/post단의 Vsync offse이나 Vsp를 조절해야 함 */\
UP_TITLE(AE)/*--　AE　--*/\
UP_SET(1, LSpotLv, 20, , )/* ANTI_SAT:ON → LEVEL : 0 ~ 20 */\
UP_SET(1, SpotSpd, UP_SPOTSPD, , )/* ANTI_SAT:ON → SPOT SPEED : 0 ~ 20 */\
UP_SET(1, ErrChgStep, UP_ERRCHGSTEP, , )/* ANTI_SAT:ON → LEVEL : 0 ~ 8 */\
UP_SET(1, ErrChtMax, ERR_CHT_SIZ, , )/* ANTI_SAT:ON → ERR CHT MAX : 1 ~ 60 */\
UP_SET(1, ErrChtMin, (ERR_CHT_SIZ<<1)/3, , )/* ANTI_SAT:ON → ERR CHT MIN : 1 ~ 60 */\
UP_SET(1, MinGammaY, UP_4sOFF, , )/* ANTI_SAT:ON → BACKLIGHT : 0 = OFF, 1 = LOW, 2 = MIDDLE, 3 = HIGH */\
UP_SET(1, LSpotTh, 0, , )/* 0 only, Brightness threshold to turn on the ANTI_SAT, If the brightness is darker than the threshold, the ANTI_SAT is turned on. */\
UP_SET(1, ShtMin, UP_SHT_MIN_DEFAULT, , gbUpShtMin = UP(ShtMin); ShutterMenuSet(); )/* SHUTTER:AUTO → SHUTTER MIN (Depends on 'FREQ' & 'OUTPUT SET→MODE') : 0 = 1/30, 1 = 1/60, 2 = 1/120, 3 = 1/250, 4 = 1/500, 5 = 1/1000, 6 = 1/2000, 7 = 1/4000, 8 = 1/8000, 9 = 1/15000, 10 = 1/30000 */\
UP_SET(1, DeblurMin, UP_MNL_SHUT_DIV1/*UP_MNL_SHUT_DIV8*/, , gbUpDeblurMin = UP(DeblurMin); ShutterMenuSet(); )/* SHUTTER:AUTO → DEBLUR MIN (Depends on 'FREQ' & 'OUTPUT SET→MODE') : 0 = 1/30, 1 = 1/60, 2 = 1/120, 3 = 1/250, 4 = 1/500, 5 = 1/1000, 6 = 1/2000, 7 = 1/4000, 8 = 1/8000, 9 = 1/15000, 10 = 1/30000 */\
UP_SET(1, ShtMax, UP_MNL_SHUT_DIV1, , gbUpShtMax = UP(ShtMax); ShutterMenuSet(); )/* SHUTTER:AUTO → SHUTTER MAX (Depends on 'FREQ' & 'OUTPUT SET→MODE') : 0 = 1/30, 1 = 1/60, 2 = 1/120, 3 = 1/250, 4 = 1/500, 5 = 1/1000, 6 = 1/2000, 7 = 1/4000, 8 = 1/8000, 9 = 1/15000, 10 = 1/30000 */\
UP_SET(1, SmallSpeed, 2, , )/* AE SPEED → STABILIZING : 0 = LOW, 1 = MIDDLE, 2 = HIGH */\
UP_SET(2, ShtBrtSpeed, SP(ShtBrtSpeed), , )/* AE SPEED → SHT. SPEED → BRIGHTEN : 1 ~ 999 (3E7h) */\
UP_SET(2, ShtDrkSpeed, SP(ShtDrkSpeed), , )/* AE SPEED → SHT. SPEED → DARKEN : 1 ~ 999 (3E7h) */\
UP_SET(2, AgcBrtSpeed, SP(AgcBrtSpeed), , )/* AE SPEED → AGC SPEED → BRIGHTEN : 1 ~ 999 (3E7h) */\
UP_SET(2, AgcDrkSpeed, SP(AgcDrkSpeed), , )/* AE SPEED → AGC SPEED → DARKEN : 1 ~ 999 (3E7h) */\
UP_SET(1, AgcMin, 0, , )/* AGC MIN : 0 ~ 255 */\
UP_SET(1, IspGainAeCur, 112, , )/* AE CURRENT BY ISP GAIN : 0 ~ 255, 32 = x1, 96 = x3, 112 = x3.5, 128 = x4 */\
UP_SET(1, IspGainAePos, 0, , )/* AE POSITION BY ISP GAIN : 0 ~ 128 */\
UP_SET(1, AE_DEBLUR_GAIN, UpAE_DEBLUR_GAIN, , )/* Deblur shutter increasing gradient (0x40 ~ 0xFF -> 0.5 ~ 2.0) */\
UP_SET(1, AE_SPEED_DC_VP, UpAE_SPEED_DC_VP, , )/* IRIS Virtual Position control Speed for judge IRIS full open (Slow 9 ~ Fast 30) */\
UP_SET(2, AE_TGT_GAIN, UpAE_TGT_GAIN, , )/* Normal Brightness target gain ((UpAE_TGT_GAIN * BRIGHTNESS in Menu) + UpAE_TGT_OFST < 0x3FF) */\
UP_SET(2, AE_TGT_OFST, UpAE_TGT_OFST, , )/* Normal Brightness target offset ((UpAE_TGT_GAIN * BRIGHTNESS in Menu) + UpAE_TGT_OFST < 0x3FF) */\
UP_SET(2, AE_CLIP_OFST, UpAE_CLIP_OFST, , )/* Normal Brightness Clip level (AE Target +  UpAE_CLIP_OFST < 0x3FF) */\
UP_SET(2, AE_WDR_ON_AGC, UpAE_WDR_ON_AGC, , )/* WDR AGC position for maximum weight (default +- value) */\
UP_SET(1, AE_WDR_MIN_WGT, UpAE_WDR_MIN_WGT, , )/* WDR minimum weight (under 0x100(100%)) */\
UP_SET(1, AE_WDR_SWGT_L, UpAE_WDR_SWGT_L, , )/* WDR weight adjust for menu Low (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, AE_WDR_SWGT_M, UpAE_WDR_SWGT_M, , )/* WDR weight adjust for menu Middle (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, AE_WDR_SWGT_H, UpAE_WDR_SWGT_H, , )/* WDR weight adjust for menu High (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_TITLE(AWB)/*--　AWB　--*/\
UP_SET(2, AWB_TCNT_LMT, UpAWB_TCNT_LMT, , )/* White pixel cnt limit to minimum (under 0x10000) */\
UP_SET(2, AWB_DBOFFSET, SP(AWB_DBOFFSET), INIT_RUN, AWBL0_DBOFFSETw(UP(AWB_DBOFFSET)/*>>2*/);/* AWB 8BIT */ )/* Color map coordinate x offset (default +- value) */\
UP_SET(2, AWB_DROFFSET, SP(AWB_DROFFSET), INIT_RUN, AWBL0_DROFFSETw(UP(AWB_DROFFSET)/*>>2*/);/* AWB 8BIT */ )/* Color map coordinate y offset (default +- value) */\
UP_SET(2, AWB_AGC_MIN, UpAWB_AGC_MIN, , )/* Chroma AGC position for maximum weight (default +- value) */\
UP_SET(1, AWB_AGC_WGT_MIN, UpAWB_AGC_WGT_MIN, , /* MP(MpAwbAgcWgtMin) */ )/* Chroma minimum weight (under 0x100(100%)) */\
UP_SET(1, AWB_SPEED_MTRX, UpAWB_SPEED_MTRX, ,   /* MP(MpAwbSpeed)     */ )/* Matrix tracking speed (Low is high speed, max=0xff) */\
UP_SET(2, AWB_YSLICE, UpAWB_YSLICE, INIT_RUN,   /* MP(MpAwbYSliceMax) */ /*AWBL_CYSLICEw(UP(AWB_YSLICE)>>2);*/ /* AWB 8BIT */ )/* White Y slice (under 0x3ff) */\
UP_SET(2, AWB_YCLIP, UpAWB_YCLIP, INIT_RUN, AWBL_CYCLIPw(UP(AWB_YCLIP)>>2); /* AWB 8BIT */ )/* White Y clip (under 0x3ff, more than UpAWB_YSLICE) */\
UP_SET(1, AWB_GAIN_TIMES, UpAWB_GAIN_TIMES, , )/* RGB Gain limit (X times, under 4) */\
UP_TITLE(COLOR_SUPPRESSION)/*--　COLOR　SUPPRESSION　--*/\
UP_SET(1, HSUP_GA, 0xff/*0x80*/, INIT_RUN, HSUP_GAw(MIN((0x20/*0x10*/ * UP(HSUP_GA))>>7, 0x3F)); )/* High Light color suppression gain (Y domain control) (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, CES_GAIN, 0x10, , )/* Edge color suppression normal gain (0 ~ 0x3F) */\
UP_SET(1, CES_WGAIN, 0x20, , )/* Edge color suppression WDR gain (0 ~ 0x3F) */\
UP_SET(1, LSUP_GA, /*0xff*/0x80, , )/* Low Light color suppression gain (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, LSUP_TH, /*0*/1, INIT_RUN, LSUP_THw(UP(LSUP_TH)); )/* Low Light color suppression threshold (0 ~ 0xFF) */\
UP_TITLE(DEFOG)/*--　DEFOG　--*/\
UP_SET(1, DEFOG_OSDON, 0, INIT_RUN, HEQ_OSDw(UP(DEFOG_OSDON)); )/* Defog Area Set OSD On/Off Function (0x0 or 0x1) */\
UP_SET(1, DEFOG_HW,  RP(DEFOG_HW),  INIT_RUN, HEQ_HWw(UP(DEFOG_HW)); )/* Defog Area Set Horizontal Width (0x0 ~ 0xf) */\
UP_SET(2, DEFOG_HSP, RP(DEFOG_HSP), INIT_RUN, HEQ_HSPw(UP(DEFOG_HSP)); )/* Defog Area Set Horizontal Start Position in 720 mode (0x0 ~ 0x790) */\
UP_SET(1, DEFOG_VW,  RP(DEFOG_VW),  INIT_RUN, HEQ_VWw(UP(DEFOG_VW)); )/* Defog Area Set Vertical Width in 720 mode (0x0 ~ 0xf) */\
UP_SET(2, DEFOG_VSP, RP(DEFOG_VSP), INIT_RUN, HEQ_VSPw(UP(DEFOG_VSP)); )/* Defog Area Set Vertical Start Position in 720 mode (0x0 ~ 0x440) */\
UP_SET(2, DEFOG_TH_LOW, UpDEFOG_TH_LOW, , )/* Defog Maximum Strength for menu LOW (0x0 ~ 0xffff) */\
UP_SET(2, DEFOG_TH_MID, UpDEFOG_TH_MID, , )/* Defog Maximum Strength for menu MIDDLE (LOW ~ 0xffff) */\
UP_SET(2, DEFOG_TH_HIGH, UpDEFOG_TH_HIGH, , )/* Defog Maximum Strength for menu HIGH (MIDDLE ~ 0xffff) */\
UP_TITLE(ACE)/*--　ACE　--*/\
UP_SET(1, ACETBL1_LOW, UpACETBL1_LOW, , )/* ACE Dark Area Strength for menu LOW (0x0 ~ 0xff) */\
UP_SET(1, ACETBL1_MID, UpACETBL1_MID, , )/* ACE Dark Area Strength for menu MIDDLE (LOW ~ 0xff) */\
UP_SET(1, ACETBL1_HIGH, UpACETBL1_HIGH, , )/* ACE Dark Area Strength for menu HIGH (MIDDLE ~ 0xff) */\
UP_SET(1, ACETBL2_LOW, UpACETBL2_LOW, , )/* ACE Bright Area Strength for menu LOW (0x0 ~ 0xff) */\
UP_SET(1, ACETBL2_MID, UpACETBL2_MID, , )/* ACE Bright Area Strength for menu MIDDLE (LOW ~ 0xff) */\
UP_SET(1, ACETBL2_HIGH, UpACETBL2_HIGH, , )/* ACE Bright Area Strength for menu HIGH (MIDDLE ~ 0xff) */\
UP_TITLE(IMAGE)/*--　IMAGE　--*/\
UP_SET(1, ShpAgcLow, 8, , IspAgcSet(); )/* SHARPNESS → AGC LOW : 0 ~ 99 (%) */\
UP_SET(1, ShpAgcMid, 48, , IspAgcSet(); )/* SHARPNESS → AGC MID : 0 ~ 99 (%) */\
UP_SET(1, ShpAgcHigh, 64, , IspAgcSet(); )/* SHARPNESS → AGC HIGH : 0 ~ 99 (%) */\
UP_SET(1, HlMaskColorCY, 100, , HlMask(); )/* HLMASK CY of Color for Customize Color Menu (0 ~ 0xFF) */\
UP_SET(1, HlMaskColorCB, 100, , HlMask(); )/* HLMASK CB of Color for Customize Color Menu (0 ~ 0xFF) */\
UP_SET(1, HlMaskColorCR, 100, , HlMask(); )/* HLMASK CR of Color for Customize Color Menu (0 ~ 0xFF) */\
UP_TITLE(DEFECT_CORRECTION)/*--　DEFECT　CORRECTION　--*/\
UP_SET(1, DEFECT_DIFTH, 0, INIT_RUN, DF_DIFTH_LEw(UP(DEFECT_DIFTH)); DF_DIFTH_SE0w(UP(DEFECT_DIFTH)); )/* Within the pattern, this register uses to detect directional defect. Difference threshold for isolated point determination. (0x0 ~ 0xff) */\
UP_SET(1, DEFECT_GA, 0x80, , )/* Within the pattern, this parameter uses to detect directional defect. Difference gain for isolated point determination. (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, DEFECT_SPOT_GA, 0x80, , )/* Spot pattern area gain. (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, DEFECT_TSEL, 0, INIT_RUN, DF_TSEL_LEw(UP(DEFECT_TSEL)); DF_TSEL_SE0w(UP(DEFECT_TSEL)); )/* Select test image in core for directional defect correction. (VLOCKI)  "0" : Normal operation  "1" : Difference values for isolated points  "2" : Image before correction  "3" : Complete correction image (100% correction image except weight).  "4" : Defect pattern detection value */\
UP_TITLE(ISP_BINARY_INFO)/*--　ISP　BINARY　INFO.　--*/\
UP_SET(2, ISP_FW_Ver, model_ver, INIT_RUN, UP(ISP_FW_Ver) = model_ver; )/* ISP Firmware Ver. (Read Only) */\
UP_SET(2, ISP_FW_Build_Year, 0, , UsrParDateTime(); )/* Year of Build ISP Firmware (Read Only) */\
UP_SET(2, ISP_FW_Build_MonthDay, 0, , UsrParDateTime(); )/* Month & Day of Build ISP Firmware (Read Only) */\
UP_SET(2, ISP_FW_Build_Time, 0, INIT_RUN, UsrParDateTime(); )/* Time when Build ISP Firmware (Read Only) */\
UP_SET(1, Sensor_ID, model_Sens, INIT_RUN, UP(Sensor_ID) = model_Sens; )/* Sensor ID (Read Only) */\
UP_SET(1, Sensor_Ctrl, model_Sens_Ctrl, INIT_RUN, UP(Sensor_Ctrl) = model_Sens_Ctrl; )/* Sensor Control Interface, 0 : SPI, 1 : TWI (Read Only) */\
UP_SET(1, Sensor_FPS, model_Sens_Fps, INIT_RUN, UP(Sensor_FPS) = model_Sens_Fps; )/* Sensor Initial FPS (Read Only) */\
UP_TITLE(MENU_OUTPUT_SET)/*--　MENU:　OUTPUT　SET　--*/\
UP_SET(1, OutFps, OUT_FPS_DEF, , OutMode(); )/* SYSTEM → OUTPUT → FRAMERATE : */\
UP_SET(1, SysFreq, UP_SYSFREQ_60, , OutMode(); )/* FREQ : 0 = 25Hz, 1 = 30Hz */\
UP_SET(1, Cvbs, UP_OFF, , Isp_Cvbs_Config(UP(Cvbs), NTSC, FREQ_27M, ISP_74M, DS_ISP_FONT_PATH, NO_VLCBIT, 0x7a, 0xe); )/* CVBS : 0 = OFF, 1 = ON */\
UP_TITLE(MENU_EXPOSURE)/*--　MENU:　EXPOSURE　--*/\
UP_SET(1, Brightness, 5/*10*/, , )/* BRIGHTNESS → DAY : 0 ~ 20 */\
UP_SET(1, BrightnessMin, 20/*17*/, , )/* BRIGHTNESS → NIGHT : 0 ~ 20 */\
UP_SET(1, AntiSatOn, UP_ANTISAT_ON, , )/* ANTI_SAT : 0 = OFF, 1 = ON */\
UP_SET(1, SatBrt, 10, , )/* ANTI_SAT:ON → WEIGHT : 0 ~ 20 */\
UP_SET(1, Stabilizing, UP_STABILIZING, INIT_RUN, AntiSatStabilizingSet(); )/* ANTI_SAT:ON → STABILIZING : 0 = OFF, 1 = LOW, 2 = MIDDLE, 3 = HIGH */\
UP_SET(1, Iris, UP_LENS_MNL, INIT_RUN, ShutterMenuGet(); )/* IRIS (DC iris) : 0 = ELC, 1 = ALC,  (piris & AF iris) : 0 = MANUAL, 1 = AUTO */\
UP_SET(1, Shutter, UP_SHUT_AUTO, , )/* SHUTTER : 0 = AUTO, 1 = MANUAL, 2 = FLICKER */\
UP_SET(1, ShutSpd, UP_MNL_SHUT_DIV1, , )/* SHUTTER:MANUAL → SPEED (Depends on 'FREQ' & 'OUTPUT SET→MODE') : 0 = 1/30, 1 = 1/60, 2 = 1/120, 3 = 1/250, 4 = 1/500, 5 = 1/1000, 6 = 1/2000, 7 = 1/4000, 8 = 1/8000, 9 = 1/15000, 10 = 1/30000 */\
UP_SET(1, DcMode, UP_DC_IN, , ShutterMenuGet(); )/* IRIS:ALC & SHUTTER:AUTO → MODE : 0 = INDOOR, 1 = OUTDOOR, 2 = DEBLUR */\
UP_SET(1, ShtMode, UP_SHT_NORMAL, , ShutterMenuGet(); )/* IRIS:ELC/MANUAL/AUTO & SHUTTER:AUTO → MODE : 0 = NORMAL, 1 = DEBLUR */\
UP_SET(1, Dss, UP_DSS_OFF, , )/* SEMS-UP : 0 = OFF, 1 = X2, 2 = X4, 3 = X8, 4 = X16, 5 = X32, 6 = X64(Must be set OUTPUT SET→MODE:60P_NOMAL｜30P_WDR｜60P_DNR) */\
UP_SET(1, Agc, 91, , )/* SENSOR GAIN MAX : 0 ~ 255 */\
UP_SET(1, IspGain, 150, , )/* ISP GAIN MAX : 0 ~ 255 */\
UP_SET(1, ExtraGain, 164, , )/* EXTRA GAIN MAX : 0 ~ (255-AGC_MAX) */\
UP_SET(1, IrsSpeed, 10, , )/* AE SPEED → IRIS SPEED : 0 ~ 20 */\
UP_SET(1, ShtSpeed, 10, , )/* AE SPEED → SHT. SPEED : 1 ~ 20 */\
UP_SET(1, AgcSpeed, 10, , )/* AE SPEED → AGC SPEED : 1 ~ 20 */\
UP_TITLE(MENU_BACKLIGHT)/*--　MENU:　BACKLIGHT　--*/\
UP_SET(1, BackLight, UP_BL_OFF, , HlMask(); InMode(); )/* BACKLIGHT : 0 = OFF, 1 = HLC, 2 = BLC, 3 = WDR  */\
UP_SET(1, HlMaskThrs, 18, , HlMask(); )/* BACKLIGHT:HLC → LEVEL : 0 ~ 20 */\
UP_SET(1, HlMaskColorSel, 7, , HlMask(); )/* BACKLIGHT:HLC → COLOR : 0 = WHT, 1 = YEL, 2 = CYN, 3 = GRN, 4 = MAG, 5 = RED, 6 = BLU, 7 = BLK, 8 = CUSTOMIZE */\
UP_SET(1, BlcMode, UP_BLC_MODE_CUSTOMIZE, , )/* BACKLIGHT:BLC → H-POS : 0 ~ 20 */\
UP_SET(1, BlcPosX, /*3*/8, , )/* BACKLIGHT:BLC → H-POS : 0 ~ 20 */\
UP_SET(1, BlcPosY, /*10*/7, , )/* BACKLIGHT:BLC → V-POS : 0 ~ 20 */\
UP_SET(1, BlcSizX, /*14*/3, , )/* BACKLIGHT:BLC → H-SIZE : 0 ~ 20 */\
UP_SET(1, BlcSizY, /*4*/3, , )/* BACKLIGHT:BLC → V-SIZE : 0 ~ 20 */\
UP_SET(1, WdrWgt, UP_3sHI, , )/* BACKLIGHT:WDR → WEIGHT : 0 = LOW, 1 = MIDDLE, 2 = HIGH */\
UP_SET(1, WdrMode, UP_WDR_FRAME, , InMode(); )/* BACKLIGHT:WDR → MODE : 0 = FRAME, 1 = LINE */\
UP_SET(2, AE_WDR_LTGT_OFST2, UpAE_WDR_LTGT_OFST2, , )/* BACKLIGHT:WDR → A. BRIGHT L : Brightness target of WDR Long (default +- value) */\
UP_SET(2, AE_WDR_STGT_OFST2, UpAE_WDR_STGT_OFST2, , )/* BACKLIGHT:WDR → A. BRIGHT S : A quarter of Brightness target of WDR Short (default +- value) */\
UP_SET(1, ACEWDR1_TH, UpACEWDR1_TH, , )/* BACKLIGHT:WDR → D. BRIGHT : ACE Dark Area Strength of WDR (0x0 ~ 0xff) */\
UP_SET(1, ACEWDR2_TH, UpACEWDR2_TH, , )/* BACKLIGHT:WDR → D. CONTRAST : ACE Bright Area Strength of WDR (0x0 ~ 0xff) */\
UP_SET(1, GammaWdr, UP_GAMMA_060/*UP_GAMMA_045*/, , )/* BACKLIGHT:WDR → GAMMA : 0 = 0.45, 1 = 0.55, 2 = 0.65, 3 = 0.75 */\
UP_TITLE(MENU_FOCUS)/*--　MENU:　FOCUS　--*/\
UP_SET(1, FocusAdj_On, 0, , )/* FOCUS ADJ : 0 = OFF, 1 ON */\
UP_SET(1, FocusThrs, 10, , )/* Edge Level : 0 ~ 20 */\
UP_SET(1, FocusAdjColorSel, 1, , )/* FOCUS ADJ → COLOR : 0 = WHT, 1 = YEL, 2 = CYN, 3 = GRN, 4 = MAG, 5 = RED, 6 = BLU, 7 = BLK, 8 = CUSTOMIZE */\
UP_TITLE(MENU_DAYnNIGHT)/*--　MENU:　DAY　&　NIGHT　--*/\
UP_SET(1, Tdn, UP_TDN_DAY, , )/* DAY&NIGHT : 0 = AUTO, 1 = COLOR, 2 = B&W, 3 = EXTERN */\
UP_SET(1, TdnThrs, 10, , )/* DAY&NIGHT:AUTO → AGC THRES : 0 ~ 20 */\
UP_SET(1, TdnMarg, 10, , )/* DAY&NIGHT:AUTO → AGC MARGIN : 0 ~ 20 */\
UP_SET(1, TdnDly, UP_3sLOW, , )/*  DAY&NIGHT:AUTO → DELAY : 0 = LOW, 1 = MIDDLE, 2 = HIGH */\
UP_SET(1, LedOn, UP_OFF, , )/* DAY&NIGHT → IR LED : 0 = OFF, 1 = ON */\
UP_SET(1, TdnSw, 0, , )/* DAY&NIGHT:EXTERN → EXTERN SW : 0 = HIGH, 1 = LOW */\
UP_SET(1, TdnDNThres, 13, , )/* DAY&NIGHT:EXTERN → D>N THRES : 0 ~ 20 */\
UP_SET(1, TdnNDThres, 7, , )/* DAY&NIGHT:EXTERN → N>D THRES : 0 ~ 20 */\
UP_TITLE(MENU_COLOR)/*--　MENU:　COLOR　--*/\
UP_SET(1, Awb, /*UP_AWB_MNL*/UP_AWB_AUTO, , )/* AWB : 0 = AUTO, 1 = AUTOext, 2 = PRESET, 3 = MANUAL */\
UP_SET(1, AwbStyle, 0, , )/* AWB:STYLE */\
UP_SET(1, AwbMnl, UP_5000K, , )/* AWB:MANUAL → C-TEMP : 0 = 3000K, 1 = 5000K, 2 = 8000K */\
UP_SET(1, Rgain, 10, , )/* AWB:MANUAL → R-GAIN : 0 ~ 20, 10:Default */\
UP_SET(1, Bgain, 10, , )/* AWB:MANUAL → B-GAIN : 0 ~ 20, 10:Default */\
UP_SET(1, SaturationR, SP(SaturationR), , )/* R-GAIN : 0 ~ 64, 32:Default */\
UP_SET(1, SaturationG, SP(SaturationG), , )/* G-GAIN : 0 ~ 64, 32:Default */\
UP_SET(1, SaturationB, SP(SaturationB), , )/* B-GAIN : 0 ~ 64, 32:Default */\
UP_TITLE(MENU_COLOR_SUPPRESSION)/*--　MENU:　COLOR　SUPPRESSION　--*/\
UP_SET(1, HSUP_ON, 1, INIT_RUN, HSUP_ONw(UP(HSUP_ON)); )/* High Light color suppression on/off (Y domain control) (0: off, 1: on) */\
UP_SET(1, HSUP_TH, SP(HSUP_TH), INIT_RUN, HSUP_THw(UP(HSUP_TH)); )/* High Light color suppression threshold (Y domain control) (0 ~ 0xFF) */\
UP_SET(1, CES_NOR, 2/* TODO KSH ◆ Edge Color Sup. */, , )/* Edge color suppression weight in normal(0 ~ 40) */\
UP_SET(1, CES_WDR, 20, , )/* Edge color suppression weight in WDR(0 ~ 40) */\
UP_SET(1, LSUP_ON, 1, , )/* Low Light color suppression on/off (0: off, 1: on) */\
UP_TITLE(MENU_HUEnCHROMA)/*--　MENU:　HUE　&　CHROMA　--*/\
UP_SET(1, Yellow_HUE_RedToGreen,  SP(Yel_RedToGrn), , HueChroma(); )/* Adjust Yellow Hue from Red to Green (0x1:Red, 0x80:Default, 0xFF:Green) */\
UP_SET(1, Yellow_GAIN,            SP(Yel_Gain)    , , HueChroma(); )/* Adjust Yellow Gain (0 ~ 0xFF, Default:0x60) */\
UP_SET(1, Red_HUE_YellowToBlue,   SP(Red_YelToBlu), , HueChroma(); )/* Adjust Red Hue from Yellow to Blue (0x1:Yellow, 0x80:Default, 0xFF:Blue) */\
UP_SET(1, Red_GAIN,               SP(Red_Gain)    , , HueChroma(); )/* Adjust Red Gain (0 ~ 0xFF, Default:0x60) */\
UP_SET(1, Blue_HUE_GreenToRed,    SP(Blu_GrnToRed), , HueChroma(); )/* Adjust Blue Hue from Green to Red (0x1:Green, 0x80:Default, 0xFF:Red) */\
UP_SET(1, Blue_GAIN,              SP(Blu_Gain)    , , HueChroma(); )/* Adjust Blue Gain (0 ~ 0xFF, Default:0x60) */\
UP_SET(1, Green_HUE_BlueToYellow, SP(Grn_BluToYel), , HueChroma(); )/* Adjust Green Hue from Blue to Yellow (0x1:Blue, 0x80:Default, 0xFF:Yellow) */\
UP_SET(1, Green_GAIN,             SP(Grn_Gain)    , , HueChroma(); )/* Adjust Green Gain (0 ~ 0xFF, Default:0x60) */\
UP_TITLE(MENU_DEFOG)/*--　MENU:　DEFOG　--*/\
UP_SET(1, Defog, UP_OFF, , )/* DEFOG : 0 = OFF, 1 = ON */\
UP_SET(1, DefogMode, UP_DEFOG_MANUAL, , )/* DEFOG:ON → MODE : 0 = MANUAL, 1 = AUTO */\
UP_SET(1, DefogLevel, UP_3sHI/*UP_3sLOW*/, , )/* DEFOG:ON → LEVEL : 0 = LOW, 1 = MIDDLE, 2 = HIGH */\
UP_TITLE(MENU_ACE)/*--　MENU:　ACE　--*/\
UP_SET(1, Ace, /*UP_4sLOW*//*UP_4sMID*/UP_4sHI/*UP_4sOFF*/, , )/* ACE : 0 = OFF, 1 = LOW, 2 = MIDDLE, 3 = HIGH (Must be set DEFOG:OFF) */\
UP_SET(1, AceGmgn, 0x20/*63*/, , )/* ACE → WEIGHT : 0 ~ 255 */\
UP_SET(1, AceBrt, 0/*32*/, , )/* ACE & DEFOG → DIGITAL BRIGHTNESS : 0 ~ 64 */\
UP_SET(1, AceCont, 0/*32*/, , )/* ACE & DEFOG → DIGITAL CONTRAST : 0 ~ 64 */\
UP_TITLE(MENU_IMAGE)/*--　MENU:　IMAGE　--*/\
UP_SET(1, Adnr2D, 4/*8*/, , )/* 2D DNR : 0 ~ 16, 0 = OFF, 2 = LOW, 4 = MIDDLE, 8 = HIGH */\
UP_SET(1, Adnr3D, UP_4sHI/*UP_4sOFF*//*UP_4sMID*/, , )/* 3D DNR : 0 = OFF, 1 = LOW, 2 = MIDDLE, 3 = HIGH */\
UP_SET(1, DnrIncrease, UP_OFF, , )/* 3D DNR  → INCR.BY AGC : 0 = OFF, 1 = ON */\
UP_SET(1, Sharpness, 10/*5*/, , )/* SHARPNESS : 0 ~ 10 */\
UP_SET(1, ShpSmallEdge, 128, , )/* SHARPNESS → SMALL EDGE : 0 ~ 255 */\
UP_SET(1, ShpBigEdge, 128/*10*//*0x30*/, , )/* SHARPNESS → BIG EDGE : 0 ~ 255 */\
UP_SET(1, Gamma, UP_GAMMA_075/*UP_GAMMA_055*/, , )/* GAMMA : 0 = 0.45, 1 = 0.55, 2 = 0.65, 3 = 0.75 */\
UP_SET(1, GammaDay, UP_GAMMA_055, , )/* GAMMA:AUTO → DAY : 0 = 0.45, 1 = 0.55, 2 = 0.65, 3 = 0.75 */\
UP_SET(1, GammaNgt, UP_GAMMA_045, , )/* GAMMA:AUTO → NIGHT : 0 = 0.45, 1 = 0.55, 2 = 0.65, 3 = 0.75 */\
UP_SET(1, Flip, UP_OFF, INIT_RUN, Flip(); )/* FLIP : 0 = OFF, 1 = ON */\
UP_SET(1, Mirror, UP_OFF, INIT_RUN, if(UP(Mirror)<2) { SensMirror(UP(Mirror)); } )/* MIRROR : 0 = OFF, 1 = ON */\
UP_SET(2, DZoom, 10, , DZoom(); )/* Digital Zoom : 10 ~ 640 = x1.0 ~ x64.0 */\
UP_SET(2, DZoomPosH, DZ_HW>>1, , DZoom(); )/* Horizontal Position of Digital Zoom : 30 ~ 1890 (2M) */\
UP_SET(2, DZoomPosV, DZ_VW>>1, , DZoom(); )/* Vertical Position of Digital Zoom : 17 ~ 1063 (2M) */\
UP_TITLE(MENU_PRIVACY)/*--　MENU:　PRIVACY　--*/\
UP_SET(1, PvcOn, UP_OFF, , PrivacyBox(); )/* PRIVACY : 0 = OFF, 1 = ON */\
UP_SET(1, PvcFormat, 1, , PrivacyBox(); )/* PRIVACY:ON → MASK MODE : 0 = FILL, 1 = FORMAT */\
UP_SET(1, PvcTrans, 0, , PrivacyBox(); )/* PRIVACY:ON → TRANS : 0 ~ 3 */\
UP_SET(1, PvcYLevel, 18, , PrivacyBox(); )/* PRIVACY:ON → Y LEVEL : 0 ~ 20 (Must be set MASK MODE:FILL) */\
UP_SET(1, PvcCRLevel, 11, , PrivacyBox(); )/* PRIVACY:ON → CB LEVEL : 0 ~ 20 (Must be set MASK MODE:FILL) */\
UP_SET(1, PvcCBLevel, 20, , PrivacyBox(); )/* PRIVACY:ON → CR LEVEL : 0 ~ 20 (Must be set MASK MODE:FILL) */\
UP_TITLE(MENU_MOTION)/*--　MENU:　MOTION　--*/\
UP_SET(1, Itl, UP_OFF, , IMD_CK_PDw(UP(Itl)); )/* MOTION : 0 = OFF, 1 = ON */\
UP_SET(1, ItlSens, 5, , )/* MOTION:ON → SENSITIVITY : 0 ~ 10 */\
UP_SET(1, ItlDettone, 4/*2*/, , )/* MOTION:ON → DET TONE : 0 ~ 4 */\
UP_SET(1, ItlRectFill, UP_OFF/*UP_ON*/, , )/* MOTION:ON → MDRECT FILL : 0 = OFF, 1 = ON */\
UP_SET(1, ItlMaskOsd, UP_ON, , )/* MOTION:ON → MOTION OSD : 0 = OFF, 1 = ON */\
UP_SET(1, ItlAlarm, UP_ON, , )/* MOTION:ON → TEXT ALARM : 0 = OFF, 1 = ON */\
UP_SET(1, ItlSigno, UP_OFF, , )/* MOTION:ON → SIGNAL OUT : 0 = OFF, 1 = ON */\
UP_SET(1, ItlCamMovingTH, 13/*16*/, INIT_RUN, CamMovTh(24-UP(ItlCamMovingTH)); )/* MOTION:ON → SENSITIVITY → CAM. MOVING : 0 ~ 20 */\
UP_SET(1, ItlBrightChgTH, 175, INIT_RUN, const int iItlBrtChgTH = 255-UP(ItlBrightChgTH); BrtChgTh(iItlBrtChgTH, -(iItlBrtChgTH>>1), ((iItlBrtChgTH*3)>>2), -((iItlBrtChgTH*3)>>3)); )/* MOTION:ON → SENSITIVITY → BRIGHT CHG. : 0 ~ 200 */\
UP_TITLE(MENU_SYSTEM)/*--　MENU:　SYSTEM　--*/\
UP_SET(1, CamTitleOn, UP_CT_RIGHT_UP, , OsdCamTitle(); )/* CAM TITLE : 0 = OFF, 1 = RIGHT UP, 2 = LEFT DOWN */\
UP_SET(1, CamTitle0, 'E', , OsdCamTitle(); )/* The first letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, CamTitle1, 'N', , OsdCamTitle(); )/* The second letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, CamTitle2, '6', , OsdCamTitle(); )/* The third letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, CamTitle3, '7', , OsdCamTitle(); )/* The fourth letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, CamTitle4, '5', , OsdCamTitle(); )/* The fifth letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, CamTitle5, '_', , OsdCamTitle(); )/* The sixth letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, CamTitle6, '0', , OsdCamTitle(); )/* The seventh letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, CamTitle7, '1', , OsdCamTitle(); )/* The eighth letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, LanguageSel, UP_ENG, INIT_RUN, SetFontChg(); )/* LANGUAGE : 0 = ENG, 1 = CHN, 2 = CHN(S), 3 = JPN, 4 = KOR */\
UP_SET(1, ColorBar, UP_OFF, INIT_RUN, if(UP(ColorBar)<2) { DCBAR_ON0w(UP(ColorBar)); DCBAR_ST0w(0x40); } )/* COLOR BAR : 0 = OFF, 1 = ON */\
UP_SET(1, OutFormat, UP_FRMT_HD, INIT_RUN, ColorSpace(); )/* Color space : 0 = SD, 1 = HD, 2 = UV */\
UP_SET(1, Style, INIT_STYLE, INIT_RUN, UsrParStyle(UP(Style), 0, 0); )/* STYLE : 0 = PREVIOUS, 1 = IPC_INDOOR, 2 = IPC_OUTDOOR, 3 = CAR_REC, 4 = ACTION_REC, 5 = WDR, 6 = LOW_BIT, 7 = CUSTOM */\
/*--　END　--*/\

