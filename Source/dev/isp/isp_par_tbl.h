/* **************************************************************************
 File Name  :   isp_par_tbl.h
 Description:   EN675 - User Parameter table
 Designer   :   Kim, Sunghoon
 Date       :   19. 6. 3
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#ifndef __ISP_PAR_TBL_H__
#define __ISP_PAR_TBL_H__
//*************************************************************************************************
// User Parameter Values
//-------------------------------------------------------------------------------------------------
enum{	// common
   	MN_OFF,
	MN_ON
};

enum {	// common
	MN_3sLOW,
	MN_3sMID,
	MN_3sHI
};

enum {	// common
   	MN_4sOFF,
	MN_4sLOW,
	MN_4sMID,
	MN_4sHI
};

enum {	// UpLanguageSel
	MN_ENG,		// English
	MN_CHN_T,	// Simplified Chinese
	MN_CHN_S,	// Traditional Chinese
	MN_JPN,
	MN_KOR
};

enum {	// UpCamTitleOn
	MN_CT_OFF,
	MN_CT_RIGHT_UP,
	MN_CT_LEFT_DOWN
};

enum {	// UpAwb
	MN_AWB_AUTO,
	MN_AWB_AUTOEXT,
	MN_AWB_PRESET,
	MN_AWB_MNL
};

enum {	// UpAwbMnl
	MN_3000K,
	MN_5000K,
	MN_8000K
};



#endif /* __ISP_PAR_TBL_H__ */

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


#define USR_PAR_STYLE	\
/*                            IPC_INDOOR,      IPC_OUTDOOR,     CAR_REC,         ACTION_REC,      WDR,             LOW_BIT,         CUSTOM        */\
_UPSTYLE_(1, UpFlip,          MN_OFF,          MN_OFF,          MN_ON,           MN_ON,           MN_OFF,          MN_ON,           MN_OFF         )\
_UPSTYLE_(1, UpColorBar,      MN_OFF,          MN_ON,           MN_ON,           MN_OFF,          MN_OFF,          MN_OFF,          MN_ON          )\
_UPSTYLE_(4, UpPAR10,         1000,            2000,            3000,            4000,            5000,            6000,            7000           )\
_UPSTYLE_(4, UpPAR1F,         100000,          200000,          300000,          400000,          500000,          600000,          700000         )


//*************************************************************************************************
// User Parameter List
//-------------------------------------------------------------------------------------------------
#define USR_PAR_LIST/* User Parameter List */\
\
/*--　MENU:　EXPOSURE　--*/\
_UP_(1, UpBrightness, 10, )/* BRIGHTNESS → DAY : 0 ~ 20 */\
_UP_(1, UpBrightnessMin, 10, )/* BRIGHTNESS → NIGHT : 0 ~ 20 */\
/*--　MENU:　COLOR　--*/\
_UP_(1, UpAwb, MN_AWB_AUTO, )/* AWB : 0 = AUTO, 1 = AUTOext, 2 = PRESET, 3 = MANUAL */\
_UP_(1, UpAwbStyle, 0, )/* AWB:STYLE */\
_UP_(1, UpAwbMnl, MN_5000K, )/* AWB:MANUAL → C-TEMP : 0 = 3000K, 1 = 5000K, 2 = 8000K */\
_UP_(1, UpRgain, 10, )/* AWB:MANUAL → R-GAIN : 0 ~ 20 */\
_UP_(1, UpBgain, 10, )/* AWB:MANUAL → B-GAIN : 0 ~ 20 */\
_UP_(1, UpSaturation,  20, )/* G-GAIN : 0 ~ 40 */\
_UP_(1, UpSaturationR, 20, )/* R-GAIN : 0 ~ 40 */\
_UP_(1, UpSaturationB, 20, )/* B-GAIN : 0 ~ 40 */\
/*--　SYSTEM　--*/\
_UP_(1, UpCamTitleOn, MN_CT_RIGHT_UP, OsdCamTitle(); )/* CAM TITLE : 0 = OFF, 1 = RIGHT UP, 2 = LEFT DOWN */\
_UP_(1, UpCamTitle0, 'E', OsdCamTitle(); )/* The first letter of the CAM TITLE, Use ASCII code. */\
_UP_(1, UpCamTitle1, 'N', OsdCamTitle(); )/* The second letter of the CAM TITLE, Use ASCII code. */\
_UP_(1, UpCamTitle2, '6', OsdCamTitle(); )/* The third letter of the CAM TITLE, Use ASCII code. */\
_UP_(1, UpCamTitle3, '7', OsdCamTitle(); )/* The fourth letter of the CAM TITLE, Use ASCII code. */\
_UP_(1, UpCamTitle4, '5', OsdCamTitle(); )/* The fifth letter of the CAM TITLE, Use ASCII code. */\
_UP_(1, UpCamTitle5, '_', OsdCamTitle(); )/* The sixth letter of the CAM TITLE, Use ASCII code. */\
_UP_(1, UpCamTitle6, '0', OsdCamTitle(); )/* The seventh letter of the CAM TITLE, Use ASCII code. */\
_UP_(1, UpCamTitle7, '1', OsdCamTitle(); )/* The eighth letter of the CAM TITLE, Use ASCII code. */\
_UP_(1, UpLanguageSel, MN_ENG, SetFontChg(); )/* LANGUAGE : 0 = ENG, 1 = CHN, 1 = CHN(S), 1 = JPN, 1 = KOR */\
_UP_(1, UpColorBar, MN_OFF, if(UPr(UpColorBar)<2) { DCBAR_ON0w(UPr(UpColorBar)); DCBAR_ST0w(0x40); } )/* COLOR BAR : 0 = OFF, 1 = ON */\
/*--　IMAGE　--*/\
_UP_(1, UpFlip, MN_OFF, if(UPr(UpFlip)<2) { FLP_ONw(UPr(UpFlip)); FLP_OFSw(0x1e); } )/* FLIP : 0 = OFF, 1 = ON */\
/*--　USER　Custom　--*/\
_UPa_(4, UpPAR00, 0/*gnFontXwMax[1]*/, /*gnFontXwMax[1] = UPr(UpPAR00); gnFontYwMax[1] = ISP_FONT_ID_EA / (gnFontXwMax[1]+1);   if(gnFontYwMax[1] > gnFontYwMax[2]) gnFontYwMax[1] = gnFontYwMax[2]; ChangeMenuSize(); menu_redraw(1,0);*/ )\
_UP_(4, UpPAR01, 0/*gnFontYwMax[2]*/,  /*gnFontYwMax[2] = UPr(UpPAR01); gnFontXwMax[2] = (ISP_FONT_ID_EA / gnFontYwMax[2]) - 1; if(gnFontXwMax[2] > gnFontXwMax[1]) gnFontXwMax[2] = gnFontXwMax[1]; ChangeMenuSize(); menu_redraw(1,0);*/ )\
_UP_(4, UpPAR02, 0, )\
_UP_(4, UpPAR03, 0, )\
_UP_(4, UpPAR04, 0, )\
_UP_(4, UpPAR05, 0, )\
_UP_(4, UpPAR06, 0, )\
_UP_(4, UpPAR07, 0, )\
_UP_(4, UpPAR08, 0, )\
_UP_(4, UpPAR09, 0, )\
_UP_(4, UpPAR0A, 0, )\
_UP_(4, UpPAR0B, 0, )\
_UP_(4, UpPAR0C, 0, )\
_UP_(4, UpPAR0D, 0, )\
_UP_(4, UpPAR0E, 0, )\
_UP_(4, UpPAR0F, 0, )\
_UP_(4, UpPAR10, 0, )\
_UP_(4, UpPAR11, 750, )\
_UP_(4, UpPAR12, 0, )\
_UP_(4, UpPAR13, 0, )\
_UP_(4, UpPAR14, 0, )\
_UP_(4, UpPAR15, 0, )\
_UP_(4, UpPAR16, 0, )\
_UP_(4, UpPAR17, 0, )\
_UP_(4, UpPAR18, 0, )\
_UP_(4, UpPAR19, 0, )\
_UP_(4, UpPAR1A, 0, )\
_UP_(4, UpPAR1B, 0, )\
_UP_(4, UpPAR1C, 0, )\
_UP_(4, UpPAR1D, 0, )\
_UP_(4, UpPAR1E, 0, )\
_UP_(4, UpPAR1F, 999, )\
/*--　END　--*/\

