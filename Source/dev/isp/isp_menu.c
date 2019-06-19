/* **************************************************************************
 File Name	: 	menu.c
 Description:	EN675 - Menu function
 Designer	:	Kim, Sunghoon
 Date		:	19. 6. 7
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"
#include "isp_menu.h"
#include "isp_menu_str.h"

#include <string.h>		// strlen()


//*************************************************************************************************
// Global Menu Parameter & variables
//-------------------------------------------------------------------------------------------------
	WORD gwFontTest = 33;

	BYTE gbMenuStyle = 0;

	BYTE gbBlcOsdOn = 0;

	//BYTE gbMenuState = 0;

	BYTE gbMpAwbPrst = MN_OFF;

#ifdef DEBUG_OSD
	BYTE gbMnDebugFnc = DEBUG_OSD;
#else
	BYTE gbMnDebugFnc = 0;
#endif

	BYTE gbMnSetupOn = MN_OFF;

	BYTE gbMpReset = MN_OFF;

	BYTE gbMnDebugBypass = MN_OFF;

	BYTE gbMnParNum = 0;
	BYTE gbMnParType = 0;

	BYTE gbMnExit = 0;

#if(model_Lens==0)

#elif(model_Lens==1)

#else
  #if (model_Iris==4)
	BYTE gbMnIrisCal = 0;
  #endif
	BYTE gbMnPresetAct = 0;
	BYTE gbMnPresetSave = 0;
	BYTE gbMnZoomDirSel = 0;
	BYTE gbMnFocusDirSel = 0;
	BYTE gbMnInitialState = 0;
	BYTE gbMnPuntCorrection = 0;
#endif

#if(model_Iris_P)	//141124 HSH Main
	extern UINT gnAEIrisStep, gnAEIrisMin_Def,gnAEIrisMax_Def,gnAEIrisMin_Pos,gnAEIrisMax_Pos, gnAEIrisMin,gnAEIrisMax;
	BYTE gbMnIrisFlag = 0;
	BYTE gbMnIrisSet = 0;
#endif

#if 1//model_WDR_ROI
	int giKeyAdd = 0;
#endif

//extern void Hue_Chroma_Menu_Set(int MenuState);

//*************************************************************************************************
// variables
//-------------------------------------------------------------------------------------------------
int		giStgPos[6]     = {-1,-1,-1,-1,-1,-1};		// Menu Position, bin 크기가 int형일 작음
int		giLV            = -1;						// Menu Stage Level, bin 크기가 int형일 작음

BYTE	gbPushCnt		= 0;

#define	gbMenuY	  MN_YSTART
//BYTE	gbMenuY	= MN_YSTART;
UINT	DRAW_Y	= MN_YSTART;

BYTE	gbIMDMenuStep	= 0;

#if model_WDR_ROI
	BYTE	gbROIWdrBoxFlag = 0;
#endif

BYTE	MN_MXSP = MN_MXSP_DEF;

//*************************************************************************************************
// Fixed define Functions
//-------------------------------------------------------------------------------------------------
//#define OsdAttrsStgPos(Y, ATTR)         			SetFontAttrs(gbMenuY+Y, MN_MXSP, MN_LINESIZE, ATTR)
//#define OsdAttrsStgPosAr(Y, ATTR)					SETFONTIDATTR(gbMenuY+Y,MN_MXSP-1,((ATTR==MN_YELLOW)?'*':' '),NO_ALPHA,MN_YELLOW)//SetFontAttrs(gbMenuY+Y, MN_MXSP/*-2*/, MN_LINESIZE/*+2*/, ATTR)		// 160330 KSH
#define OsdAttrsStgPosAr(Y, ATTR)					SetFontAttrs(gbMenuY+Y, MN_MXSP/*-2*/, MN_LINESIZE/*+2*/, ATTR)		// 160330 KSH

//*************************************************************************************************
// Sub Functions
//-------------------------------------------------------------------------------------------------
void OsdDispBar(UINT anVal, UINT anPosY, UINT anMin, UINT anMax)
{
	if(anVal > anMin) anVal -= anMin;
	else anVal = 0;

	//if(anMax > anMin) anMax -= anMin;
	//else anMax = anMin;
	anMax -= anMin;                                        // !! Must be 'anMax > anMin'

    UINT Pos1;
	UINT Pos2;

     if(anVal >= anMax) {
		Pos1 = (MN_BAR_SIZE-1);                                       // Set Max Position
		Pos2 = 2;
     }
    else {
		anMax <<= 5;                   // x32 : 32 > 7x3  -->  Increase Resolution
		anVal <<= 5;

		const UINT nDiv1 = (anMax / MN_BAR_SIZE) + 1;
		const UINT nDiv2 = ((nDiv1 - 1) / 3) + 1;

		Pos1 = anVal / nDiv1;
		Pos2 = (anVal % nDiv1) / nDiv2;
    }

    UINT Typ = 0;

	switch(Pos2) {
	case 0 : Typ = (Pos1==(MN_BAR_SIZE-1)) ? _IOI : _IOO; break;
	case 1 : Typ = (Pos1==(MN_BAR_SIZE-1)) ? _OII : (Pos1==0) ? _IIO : _OIO; break;
	case 2 : Typ = (Pos1==0) ? _IOI : _OOI; break;
	}

    UINT i;

	SETFONTID(anPosY,MN_XBAR,_IOO);
	SETFONTID(anPosY,MN_XBAR+(MN_BAR_SIZE-1),_OOI);
	for(i=1; i<(MN_BAR_SIZE-1); i++) SETFONTID(anPosY,MN_XBAR+i,_OOO);
	SETFONTID(anPosY,MN_XBAR+Pos1,Typ);

	for(i=(MN_XBAR+MN_BAR_SIZE); i<(UINT)(MN_SXSP+MN_SUBSIZE); i++) SETFONTID(anPosY,i,' ');
}

void OsdCamTitle(void)
{
#if 0	// for Test
	FontStrEx(0, gnFontXw-8, NO_ALPHA, MN_GREEN, (const char*)&gbCamTitle(0), 0x8);
	FontStrEx(gnFontYw-2, 0, NO_ALPHA, MN_GREEN, (const char*)&gbCamTitle(0), 0x8);	// TODO KSH gnFontYw-1 이상부터 출력 안됨
#else
//	DispClr(0, ISP_FONT_EAX-9, 0x8);	SetFontAttrs(0, ISP_FONT_EAX-9, 0x8, MN_WHITE);
//	DispClr(19, 0, 0x8);				SetFontAttrs(19, 0, 0x8, MN_WHITE);
//	DispClr(17, 0, 0x8);				SetFontAttrs(17, 0, 0x8, MN_WHITE);

	UINT nUpCamTitleOn = UPr(UpCamTitleOn);
	if(gbMnDebugFnc && nUpCamTitleOn == MN_CT_RIGHT_UP) nUpCamTitleOn = MN_CT_LEFT_DOWN;

	//if(gbMnDebugFnc == 0) {
		switch(nUpCamTitleOn) {
			case MN_CT_RIGHT_UP  :	//DispStr((const char*)&gbCamTitle(0), 0, ISP_FONT_EAX-9, 0x8);
									//SetFontAttrs(0, ISP_FONT_EAX-9, 0x8, MN_BLACK);
									FontClr(gnFontYw-2, 0, 0x8);
									FontStrEx(0, gnFontXw-8, NO_ALPHA, MN_GREEN, (const char*)&gbCamTitle(0), 0x8);
									break;

			case MN_CT_LEFT_DOWN : 
//			  #if (model_2M30p)
//				if (MP(MpOutFps)!=MN_2M_1080p_25_30) { DispStr((const char*)&gbCamTitle(0), 19, 0, 0x8); SetFontAttrs(19, 0, 0x8, MN_BLACK); }
//				else								 { DispStr((const char*)&gbCamTitle(0), 17, 0, 0x8); SetFontAttrs(17, 0, 0x8, MN_BLACK); }
//
//			  #elif (model_2M)
//				if ((MP(MpOutFps)==MN_2M_1080p_25_30)||(MP(MpOutFps)==MN_2M_1080p_50_60))
//					                                 { DispStr((const char*)&gbCamTitle(0), 17, 0, 0x8); SetFontAttrs(17, 0, 0x8, MN_BLACK); }
//				else 								 { DispStr((const char*)&gbCamTitle(0), 19, 0, 0x8); SetFontAttrs(19, 0, 0x8, MN_BLACK); }
//
//			  #else
//				DispStr((const char*)&gbCamTitle(0), 19, 0, 0x8);
//				SetFontAttrs(19, 0, 0x8, MN_BLACK);
//
//			  #endif
				FontClr(0, gnFontXw-8, 0x8);
				FontStrEx(gnFontYw-2, 0, NO_ALPHA, MN_GREEN, (const char*)&gbCamTitle(0), 0x8);
				break;

			default :
				FontClr(gnFontYw-2, 0, 0x8);
				FontClr(0, gnFontXw-8, 0x8);
				break;
		}
	//}
#endif
}

void ChangeMenuSize(void)
{
	UINT nFontSize = BIG_FONT;
	UINT nFontWidx = BIG_FONT_AREA;

	if(gbMnDebugFnc == 0) {
		MN_MXSP = MN_MXSP_DEF;
		if(MENU_TITLE_IS(OFF))	{
			nFontSize = SMALL_FONT;
			nFontWidx = (UPr(UpCamTitleOn) == MN_CT_RIGHT_UP) ? SMALL_FONT_AREA_W : SMALL_FONT_AREA_H;
		}
		else {
			nFontSize = BIG_FONT;
			nFontWidx = BIG_FONT_AREA;
		}
	}
	else {
		MN_MXSP = MN_MXSP_DBG;
		nFontSize = SMALL_FONT;
		nFontWidx = SMALL_FONT_AREA_H;
	}

	FontSetSize(nFontSize, nFontWidx, (gbMnDebugFnc) ? BORDER_NORMAL : BORDER_THICK);

	FontClrAll(NO_ALPHA,MN_WHITE);

	OsdCamTitle();
}

//*************************************************************************************************
// Add Functions
//-------------------------------------------------------------------------------------------------
int  giMenuDispPos	= 0;
int  giMenuDispChg	= 0;
int  giGrayOnly		= 0;
int  giMenuNum		= 0;

PEXCH *gbMenuTitle	= (PEXCH*)_S(OFF);
PEXCH *gbMenuList	= 0;
PEXCH *gbMenuVal	= 0;

PEXCH *gbStgTitle[4] = {(PEXCH*)_S(OFF),};

void menu_redraw(int iChg, int iGrayOnly)
{
	if(giLV < 0) return;

	giMenuDispPos = giStgPos[giLV] + 1;
	giMenuDispChg = iChg;
	giGrayOnly = iGrayOnly;
	giStgPos[giLV] = 0;
}

void menu_in(const PEXCH* Title)
{
	gbMenuTitle = (PEXCH*)Title;

	if(giLV >= 0) {		// 이전 메뉴가 있는 경우 (이전 메뉴가 MENU_EXT형식이면 안됨!!!)
		OsdMenuPos(0);
	}
	else {				// 이전 메뉴가 없는 경우
		OSD_ON();
		ChangeMenuSize();
	}

	giLV++;
	giStgPos[giLV] = 0;
	gbStgTitle[giLV] = gbMenuTitle;

	//OsdMenuPos(1)
	giMenuDispPos = giStgPos[giLV] + 1;
	giMenuDispChg = 1;
	giGrayOnly = 0;
}

void menu_close(void)
{ //메뉴 닫기
	giLV = -1;
	gbMenuTitle = (PEXCH*)_S(OFF);

	gbMenuStyle = 0;

	//gbMnSetupOn = 0;

	int i;

	if(giMenuNum == 0) giMenuNum = 16;	// 16 = 메뉴 항목 최대 가능 수

	// UartTxStrHex("giMenuNum ", giMenuNum, 2);

	DispClr(MN_TITLE, MN_MXSP, MN_LINESIZE);
	for(i=0; i<giMenuNum; i++)
	{
		DispClr(gbMenuY+i, MN_MXSP, MN_LINESIZE);
	}
	giMenuNum = 0;
	giMenuDispPos = 1;
	gbMenuVal = 0;
	gbMenuList = 0;


	OSD_OFF();
	ChangeMenuSize();
}

void menu_out(const int aiClearFinger/*const PEXCH* Title*/)
{ //이전단계 메뉴
	//gbMenuTitle = (PEXCH*)Title;

	if(aiClearFinger) { OsdMenuPos(0); }

	giLV--;

	//if(MENU_TITLE_IS(OFF)) giLV = -1;
	//if(((const PEXCH*)Title) == _S(OFF)) { giLV = -1; gbMenuTitle = (PEXCH*)_S(OFF); }

	if(giLV >= 0) {		// 현재 메뉴가 있는 경우
		gbMenuTitle = gbStgTitle[giLV];

		//OsdMenuPos(1);
		giMenuDispPos = giStgPos[giLV] + 1;
		giMenuDispChg = 1;
		giGrayOnly = 0;
		giStgPos[giLV] = 0;
	}
	else
	{				// 현재 메뉴가 없는 경우
		menu_close();
	}
}

void menu_dir(const int aiOn)
{
	if(!aiOn) {
		OsdAttrsStgPosAr(giStgPos[giLV], MN_GRAY);
		gbMenuVal = (PEXCH*)_S(NOT_USED);
	}

	if(giMenuDispPos) {
		if(aiOn) {
			if(giMenuDispPos != giStgPos[giLV] + 1) OsdAttrsStgPosAr(giStgPos[giLV], MN_WHITE);
		}
		else {
			if(giMenuDispPos == giStgPos[giLV] + 1) giMenuDispPos++;
			if(giGrayOnly == 0) DISPCLRSTR(_S(NOT_USED), gbMenuY+giStgPos[giLV], MN_SXSP, MN_SUBSIZE, MN_SUBSIZE);
		}
		giStgPos[giLV]++;
	}
}

void menu_one(const int aiOn, const PEXCH* Str)
{
	gbMenuVal = (PEXCH*)Str;

	if(giMenuDispPos && (aiOn || giGrayOnly)) {
		DISPCLRSTR(Str, gbMenuY+giStgPos[giLV], MN_SXSP, MN_SUBSIZE, MN_SUBSIZE);
	}

	menu_dir(aiOn);
}

BYTE menu_push(const int aiOn, volatile BYTE *Val, const PEXCH* StrNor, const PEXCH* StrPush, const BYTE abDelay)
{
	BYTE bPushOn = 0;

	if(giMenuDispPos && (aiOn || giGrayOnly)) {
		gbPushCnt = 0;
		*Val = 0;
		goto DISPLAY_PUSH_STR;
	}

	if(giMenuDispPos == 0) {
		if(pKEY_C) {
			if(gbPushCnt < 0xFF) gbPushCnt++;
			if(gbPushCnt == abDelay) {
				bPushOn = 1;
				*Val = 1;
				goto DISPLAY_PUSH_STR;
			}
		}
		else {
			if(gbPushCnt) {
				gbPushCnt = 0;
				*Val = 0;
				goto DISPLAY_PUSH_STR;
			}
		}
	}

	gbMenuVal = (PEXCH*)((*Val) ? StrPush : StrNor);
	menu_dir(aiOn);
	return 0;

DISPLAY_PUSH_STR:
	gbMenuVal = (PEXCH*)((*Val) ? StrPush : StrNor);
	DISPCLRSTR(gbMenuVal, gbMenuY+giStgPos[giLV], MN_SXSP, MN_SUBSIZE, MN_SUBSIZE);
	menu_dir(aiOn);
	return bPushOn;
}

void menu_str(const int aiOn, BYTE *Val, const BYTE abValSize, int OpNum, const PEXCH** Str)
{
	const UINT nOri = GetByte(Val,abValSize);
	UINT nVal = nOri;

	if(giMenuDispPos && aiOn) {		// 170530 KSH
		if(nVal >= OpNum) nVal = 0;
		const UINT nOld = nVal;
		gbMenuVal = (PEXCH*)Str[nVal];

		while(gbMenuVal == _S()) {
			nVal++;
			if(nVal >= OpNum) nVal = 0;
			gbMenuVal = (PEXCH*)Str[nVal];
			if(nVal == nOld) goto DISPLAY_STR;
		}
	}

	if(giMenuDispPos == 0 && aiOn) {
		do {
			if(KEY_L)	nVal = (nVal == 0) ? OpNum - 1 : nVal - 1;
			if(KEY_R)	nVal = (nVal >= OpNum - 1) ? 0 : nVal + 1;
			gbMenuVal = (PEXCH*)Str[nVal];
		} while(gbMenuVal == _S());
	}

DISPLAY_STR:
	if((giMenuDispPos || KEY_R || KEY_L) && (aiOn || giGrayOnly)) {
		DISPCLRSTR(Str[nVal], gbMenuY+giStgPos[giLV], MN_SXSP, MN_SUBSIZE, MN_SUBSIZE);
	}

	menu_dir(aiOn);

	if(nOri != nVal) {
		SetByte(Val,abValSize,nVal);
		if(gbUsrParTbl <= Val && (Val+abValSize) <= (gbUsrParTbl+USR_PAR_EA)) UsrParChg(Val-gbUsrParTbl);
	}
}

UINT menu_val(void *Val, const BYTE abValSize, UINT anMin, UINT anMax, UINT anRotation)
{
	const UINT nOri = GetByte(Val,abValSize);
	UINT nVal = nOri;

	if(giMenuDispPos == 0) {
		UINT nA, nB;

		if(anRotation) {
			nA = anMax;
			nB = anMin;
		}
		else {
			nA = anMin;
			nB = anMax;
		}

		if(anMax > (anMin+20)) {
			KeyAccLevel(&giKeyAdd);
			if(KEY_L) {
				const UINT nVn = nVal - giKeyAdd;
				nVal = (nVn > nVal) ? nA :
					   (nVn < anMin) ? nA : nVn;
			}
			if(KEY_R) {
				const UINT nVp = nVal + giKeyAdd;
				nVal = (nVp < nVal) ? nB :
					   (nVp > anMax) ? nB : nVp;
			}
		}
		else {
			if(KEY_L) nVal = (nVal <= anMin) ? nA : nVal - 1;
			if(KEY_R) nVal = (nVal >= anMax) ? nB : nVal + 1;
		}
	}
	else {
		if(nVal < anMin) nVal = anMin;
		if(nVal > anMax) nVal = anMax;
	}

	if(nOri != nVal) {
		SetByte(Val,abValSize,nVal);
		if(gbUsrParTbl <= (BYTE*)Val && ((BYTE*)Val+abValSize) <= (gbUsrParTbl+USR_PAR_EA)) return 1;//UsrParChg((BYTE*)Val-gbUsrParTbl);
	}

	return 0;
}

void menu_bar(const int aiOn, void *Val, const BYTE abValSize, const UINT anMin, const UINT anMax, const UINT anRotation, const PEXCH *Unit, const BYTE abHex)
{
	UINT nUpChgOn = 0;
	if(aiOn) nUpChgOn = menu_val(Val, abValSize, anMin, anMax, anRotation);

	const UINT nVal = GetByte(Val,abValSize);

	if((giMenuDispPos || KEY_R || KEY_L) && (aiOn || giGrayOnly)) {
		const UINT nPosY = gbMenuY+giStgPos[giLV];
		UINT nLen;

		//DispClrDec(nVal, nPosY, MN_SXSP, 3);
		if(abHex) {
			gbStr[0] = '0'; gbStr[1] = 'x';
			uint2strh(gbStr+2, nVal, (abValSize<<1));
			nLen = (abValSize<<1) + 2;
		}
		else {
			nLen = uint2str(gbStr, nVal, 10);
		}
		FontClrStr(nPosY, MN_SXSP, gbStr, nLen, MN_SUBSIZE);

		if(Unit) {
			if(MN_SUBSIZE > nLen) {
				DISPCLRSTR(Unit, nPosY, MN_SXSP+nLen, MN_SUBSIZE-nLen, MN_SUBSIZE-nLen);
			}
		}
		else {
			OsdDispBar(nVal, nPosY, anMin, anMax);
		}
	}

	//if(gbMenuList) UartTxStr((BYTE*)MENU_NAME);

	gbMenuVal = 0;

	menu_dir(aiOn);

	if(nUpChgOn) UsrParChg((BYTE*)Val-gbUsrParTbl);
}

#if model_WDR_ROI
void menu_val_ex(const int aiOn, BYTE *apbValL, BYTE *apbValH, const WORD awMin, const WORD awMax)
{
	int iVal = ((*apbValH)<<8) | (*apbValL);

	if(giMenuDispPos == 0 && aiOn)
	{
		KeyAccLevel(&giKeyAdd);

		if(KEY_L) {
			const int iValTmp = iVal - giKeyAdd;
			iVal = MAX(awMin, iValTmp);
		}
		if(KEY_R) {
			const int iValTmp = iVal + giKeyAdd;
			iVal = MIN(awMax, iValTmp);
		}

		*apbValH = (iVal>>8)&0xff;
		*apbValL = iVal&0xff;

		gbMenuVal = 0;
	}

	if((giMenuDispPos || KEY_R || KEY_L) && (aiOn || giGrayOnly)) {
		int2str(gbStr, iVal, (iVal < 10) ? 1 : (iVal < 100) ? 2 : (iVal < 1000) ? 3 : 4);
		DispClrStr(gbStr, DRAW_Y, MN_SXSP, 4, MN_SUBSIZE);
	}
	
	menu_dir(aiOn);
}
#endif

void menu_pos(const PEXCH* Title, int MenuNum, const PEXCH** Str)
{
	if(giMenuDispChg)
	{
		if(MenuNum) giMenuDispChg = 0;

		int i;

		STRLEN(i, Title);
		//DispClr(MN_TITLE, h760(0x8,MN_MXSP), LINESIZE - h760(0x8,MN_MXSP) - 3);
		DispClr(MN_TITLE, MN_MXSP, MN_LINESIZE);
		DISPSTR(Title, MN_TITLE, MN_XCENT - (i>>1) - (giLV==1), i);		// 160330 KSH

		//gbMenuY = MN_YSTART;

		for(i=0; i<MenuNum; i++)
		{
			DISPCLRSTR(Str[i], gbMenuY+i, MN_MXSP, (MN_SXSP-MN_MXSP)/*MN_LINESIZE*/, (MN_SXSP-MN_MXSP)/*MN_LINESIZE*/);
		}

		for(; i<giMenuNum; i++)
		{
			DispClr(gbMenuY+i, MN_MXSP, MN_LINESIZE);
		}
		giMenuNum = MenuNum;

#if 0
		extern BYTE gbIMDFontY, gbIMDFontX;
		if(MP(MpItl)==MN_ON && MP(MpItlAlarm)==MN_ON && (gbMenuY+giMenuNum)<=gbIMDFontY) DispClr(gbIMDFontY, gbIMDFontX, 18);	// 170725 KSH
#endif
	}
	else if(!giMenuDispPos){		// 13.11.8 LJH 동시 키 오류  방지
		if((KEY_U || KEY_D) && MenuNum) {
			if((const PEXCH*)gbMenuVal != _S(NOT_USED)) OsdMenuPos(0);	// !! if문은 MN_WHITE에만 영향, finger clear는 항상 적용

			if(KEY_U)	giStgPos[giLV] = (giStgPos[giLV] == 0) ? MenuNum - 1 : giStgPos[giLV] - 1;
			if(KEY_D)	giStgPos[giLV] = (giStgPos[giLV] == MenuNum - 1) ? 0 : giStgPos[giLV] + 1;

			//OsdMenuPos(1);
		}
	}

	DRAW_Y = gbMenuY+giStgPos[giLV];

	if(MenuNum && Str) gbMenuList = (PEXCH*)Str[giStgPos[giLV]];
}

//*************************************************************************************************
// Initial
//-------------------------------------------------------------------------------------------------
void InitMenuSet(void)
{
#if 0
	MENU_LIB_LIST	// TODO KSH MENU를 Lib에서도 쓸 수 있도록 설정 필요
#endif
}

void InitMenu(void)
{	// Initiate Menu
	UartTxStrNoIRQ("Menu Init... ");

	UINT i;
	UINT nResetKey = 0;

	InitMenuSet();

	#if (model_Key==0)		// AD Key
		nResetKey = (ADC_KEYr<(((UINT)UPr(UpADC_LEVEL_U)+(UINT)UPr(UpADC_LEVEL_D))>>1));           // 141031 LH

	#elif (model_Key==1)	// GPIO Key
		nResetKey = !GpioGetPin(GPIO_KEY_U);									// if up key is pressed

	#else
//		nResetKey = 0;

	#endif


#if model_MenuDefSet == 1
	if (((WORD)((MSP(MP_START)<<8)|MSP(MP_END))!=(WORD)MN_DATA_ID)||nResetKey) {	// Parameter reset condition
		MenuResetSet(1);
		gbMnParSaveChk = 1;
		UartTxStrNoIRQ("MENU SET PAR reset ");
	}
#endif

	if (nResetKey) {	// Parameter reset condition
		UsrParReset();	// gbUsrParTbl[] 초기화 & UsrParChgAll() 실행
		UsrParSave(1);	// gbUsrParTblSaved[] 업데이트 & ROM 저장 실행
		UartTxStrNoIRQ("MENU PAR reset ");
    }

	ChangeMenuSize();
}

//*************************************************************************************************
// Menu
//-------------------------------------------------------------------------------------------------
void Menu(void)
{
	//if (MP(MpDefDet)==1)	return;

	static BYTE gbPassCnt1=0;
	static BYTE gbPassCnt2=0;
	static BYTE gbPassCnt3=0;
	static BYTE gbPassCnt4=0;
	static BYTE gbPassCnt5=0;
	static BYTE gbPassCnt6=0;

	const BYTE gbPassKeys1[] = MENU_PK_SETUP;
	const BYTE gbPassKeys2[] = MENU_PK_DEBUG;

	const int iSetMnOn = (!MENU_TITLE_IS(OFF)) ? (LibUtlKeyPass(gbPassKeys1, sizeof(gbPassKeys1), &gbPassCnt1) == 1) : 0 ;

// Menu Exit ----------------------------------------------------------------------------------------------------
	if(MENU_NAME_IS(RETURN)) {
		if(KEY_L || KEY_R) {
			menu_val(&gbMnExit, sizeof(gbMnExit), 0, 2, 1);
			switch(gbMnExit) {
				case 0 : MENU_NAME_REDRAW(RETURN);	MENU_VAL_REDRAW(e);			break;
				case 1 : MENU_NAME_REDRAW(nEXIT);	MENU_VAL_REDRAW(SAVEe);		break;
				case 2 : MENU_NAME_REDRAW(nEXIT);	MENU_VAL_REDRAW(CANCELe);	break;
			}
		}
		else if(KEY_C && gbMnExit) {
			//const BYTE bSetFontChgOn = (gbMnParTbl[MpLanguageSel] != gbMnParTblOld[MpLanguageSel]) && (gbMnExit == 2);	// 160223 KSH
			UsrParSave(gbMnExit == 1);
			//if(bSetFontChgOn) { SetFontChg(); }																			// 160223 KSH
			gbMnExit = 0;
			MENU_CLOSE();
		}
	}


	MENU_START

// Main MENU --------------------------------------------------------------------------------
	MENU_SET( 5, MENU, MN_ON,
			EXPOSURE,		MENU_ONEi(MN_ON, e, MN_ON, EXPOSURE, ),
			COLORm,			MENU_ONEi(MN_ON, e, MN_ON, COLORm, ),
			IMAGE,			MENU_ONEi(MN_ON, e, MN_ON, IMAGE, ),
			SYSTEM, 		MENU_ONEi(MN_ON, e, !iSetMnOn, SYSTEM, /*MENU_OFF_GRAY_ONLY();*/ ),		// if not Setup menu
			EXIT,			MENU_STRo(MN_ON, MN_ON, UsrParSave(gbMnExit==0); gbMnExit = 0;, , UPx(gbMnExit), 2, SAVEe, CANCELe))


// MENU - EXPOSURE ----------------------------------------------------------------------------------------------------
	MENU_SET( 2, EXPOSURE, MN_ON,
			BRIGHTNESS, 	if(gbMnSetupOn==1){
								MENU_ONEi(MN_ON, e, MN_ON, BRIGHTNESS, )
							}else{
								MENU_BARn(MN_ON, , UPo(UpBrightness), 0, 20, 1)
							},
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - EXPOSURE - BRIGHTNESS
	MENU_SET( 3, BRIGHTNESS, MN_ON,
			DAY,			MENU_BARn(MN_ON, , UPo(UpBrightness), 0, 20, 1),
							/*({ menu_bar(MN_ON, &MP(MpBrightness), 0, 20, 1, _S());
							   SETFONTID(DRAW_Y, MN_SXSP+3, ':');
							   DispClrDec(TgtMaxGet(0,0), DRAW_Y, MN_SXSP+5, 3);
							   MENU_CODE(if_KEY_LR(MENU_REDRAW())) }),*/
			NIGHT,			MENU_BARn(MN_ON, , UPo(UpBrightnessMin), 0, 20, 1),
							/*({ menu_bar(MN_ON, &MP(MpBrightnessMin), 0, 20, 1, _S());
							   SETFONTID(DRAW_Y, MN_SXSP+3, ':');
							   DispClrDec(TgtMinGet(TgtMaxGet(0,0), MP(MpBrightnessMin)), DRAW_Y, MN_SXSP+5, 3);
							   MENU_CODE(if_KEY_LR(MENU_REDRAW())) }),*/
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

// MENU - COLOR ----------------------------------------------------------------------------------------------------
	MENU_SET( 6, COLORm, MN_ON/*(gbMnSetupOn==0)*/,
			AWB,			MENU_STRi(MN_ON, MENU_VAL_IS(MANUALe), AWB, ,
										MENU_VAL_PUSH(PRESETp, PUSHING, PUSH_DELAY_NOR, gbMpAwbPrst = MN_OFF, gbMpAwbPrst = MN_ON),
										UPo(UpAwb), 4, AUTO, AUTOext, PRESETp, MANUALe),
			STYLE,			MENU_STRn(MN_ON, /*if_KEY_LR(Hue_Chroma_Menu_Set(0))*/, UPo(UpAwbStyle), 6, A_TYPE, B_TYPE, C_TYPE, D_TYPE, E_TYPE, F_TYPE),
			R_GAIN,			MENU_BARn(MN_ON, , UPo(UpSaturationR), 0, 40, 1 ),	// 2017419 - WHL : CBB TEST
			G_GAIN,			MENU_BARn(MN_ON, , UPo(UpSaturation), 0, 40, 1 ),
			B_GAIN,			MENU_BARn(MN_ON, , UPo(UpSaturationB), 0, 40, 1 ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - AWB:MANUAL
	MENU_SET( 4, AWB, UPr(UpAwb) == MN_AWB_MNL,
			C_TEMP,			MENU_STRn(MN_ON, , UPo(UpAwbMnl), 3, TEMP1, TEMP2, TEMP3),
			RGAIN,			MENU_BARn(MN_ON, , UPo(UpRgain), 0, 20, 1),
			BGAIN,			MENU_BARn(MN_ON, , UPo(UpBgain), 0, 20, 1),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))


// MENU - IMAGE ----------------------------------------------------------------------------------------------------
	MENU_SET( 2, IMAGE, MN_ON,
			FLIP,			MENU_STRn(MN_ON, , UPo(UpFlip), 2, OFF, ON),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))


// MENU - SYSTEM ----------------------------------------------------------------------------------------------------
	MENU_SET( 5, SYSTEM, MN_ON,
			CAM_TITLE,		MENU_STRi(MN_ON, UPr(UpCamTitleOn)!=MN_CT_OFF, CAM_TITLE, , , UPo(UpCamTitleOn), 3, OFF, RIGHT_UPe, LEFT_DOWNe),
			LANGUAGE,		MENU_STRn(MN_ON, if_KEY_LR(MENU_CHANGE()), UPo(UpLanguageSel), 5, ENG, CHN, CHNs, JPN, KOR),
			COLOR_BAR,		MENU_STRn(MN_ON, , UPo(UpColorBar), 2, OFF, ON),
			RESET,			MENU_PUSH(MN_ON, gbMpReset, ONp, PUSHING, PUSH_DELAY_NOR, UsrParReset(); MENU_CHANGE(); ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - SYSTEM - CAM_TITLE
	MENU_EXT( CAM_TITLE, MN_ON, giMenuNum = 8;/*나가기 전에 SubMenu 7번째 라인까지 지움*/,

			OsdAttrsStgPosAr(1, MN_WHITE);
			OsdAttrsStgPosAr(2, MN_WHITE);
			OsdAttrsStgPosAr(3, MN_WHITE);

			SETFONTID(gbMenuY+1, MN_XTITL, 162);						// Display Arrow-down

			SETFONTATTR(gbMenuY+2, MN_XTITL, MN_YELLOW);				// Display Cam-title
			DispStr((const char*)&gbCamTitle(0), gbMenuY+2, MN_XTITL, 0x8);			//		"

			SETFONTATTR(gbMenuY+1, MN_XTITL, MN_YELLOW);				// Display line
			DrawStr(TLx8, gbMenuY+3, MN_XTITL, 8);

			//---------------------------------------------------------
			OsdAttrsStgPosAr(5, MN_WHITE);
			OsdAttrsStgPosAr(6, MN_WHITE);
			OsdAttrsStgPosAr(7, MN_WHITE);

			DrawStr(UPsDN_d_CHAR_SELECT,gbMenuY+5, MN_MXSP+3, 19);		// Display UP/DN Instruction
			DrawStr(LEsRI_d_POSITION,	gbMenuY+6, MN_MXSP+3, 16);		// Display LE/RI Instruction
			DrawStr(ENTER_d_RETURN,		gbMenuY+7, MN_MXSP+3, 14);		// Display ENTER Instruction
	)


// Setup MENU ----------------------------------------------------------------------------------------------------
	MENU_SET(4, SETUP, MN_ON,
			DEVELOPER,		MENU_STRn(MN_ON, , UPx(gbMnSetupOn), 2, OFF, ON),
			STYLE,			MENU_STRn(MN_ON, if_KEY_LR(UsrParStyle(gbMenuStyle); MENU_CHANGE()), UPx(gbMenuStyle), 8, PREVIOUS, IPC_INDOOR, IPC_OUTDOOR, CAR_REC, ACTION_REC, WDR, LOW_BIT, CUSTOM),
#if 0
  			SHADINGDET,		MENU_ONEo(MN_ON, ONp, MN_ON,
											MP(MpShdDet) = 1;
											MP(MpShading) = 0;
											MENU_CLOSE();
											goto menu_start;
											),
#endif
			CAM_VERSION,	MENU_DIRn(MN_ON
							/* MENU_DIRn:Pre Code */,
							/* MENU_DIRn:Value Change */,
							/* MENU_DIRn:Display Code */,
							extern WORD gDataID;
							uint2strh(gbStr, model_ver, 4);
							uint2strh(gbStr+5, gDataID, 4);
							gbStr[4] = '(';
							gbStr[9] = ')';
							gbStr[10] = 0;
							DispStr(gbStr, DRAW_Y, MN_SXSP, 10);
							/* MENU_DIRn:End Code */,),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))


// Debug MENU ----------------------------------------------------------------------------------------------------
	MENU_SET(7, nDEBUG, MN_ON,
			FUNCTION,		MENU_STRn(MN_ON, if_KEY_LR(ChangeMenuSize(); MENU_CHANGE()) , UPx(gbMnDebugFnc), 9, OFF, AE, AF, AWB, DBG_4, DBG_5, IMD_DBG, TDN_DBG, STATUS),
			BYPASS,			MENU_STRn(MN_ON, , UPx(gbMnDebugBypass), 2, OFF, ON),
			PAR_NUM,		MENU_DECn(MN_ON, if_KEY_LR(MENU_REDRAW()), UPx(gbMnParNum), 0, 31, 1, ),

			PAR_VAL,		menu_bar(MN_ON, (gbUsrParTbl+UPi(UpPAR00)+(gbMnParNum<<2)), 4, 0, 0xFFFFFFFF, 1, _S(), gbMnParType);  MENU_DISABLE(MN_ON)  MENU_CODE( ),

			PAR_TYPE,		MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW()), UPx(gbMnParType), 2, DEC, HEX),
			FONT_TEST,		menu_bar(MN_ON, &gwFontTest, sizeof(gwFontTest), 0, ISP_FONT_CHAR_EA-1, 1, _S(), 0); SETFONTID(DRAW_Y, MN_SXSP+4, ':'); SETFONTID(DRAW_Y, MN_SXSP+6, '\''); SETFONTID(DRAW_Y, MN_SXSP+7, gwFontTest); SETFONTID(DRAW_Y, MN_SXSP+8, '\''); ,	// TODO KSH Font Test Menu
			nEXIT,			MENU_STRo(MN_ON, MN_ON, UsrParSave(gbMnExit==0); gbMnExit = 0;, , UPx(gbMnExit), 2, SAVEe, CANCELe))


	MENU_DISPLAY_END()
	//MENU_DISPLAY_STAT


// Condition for Menu call --------------------------------------------------------------------
	if(iSetMnOn&&MENU_NAME_IS(SYSTEM)) {										// Setup MENU start
		//gbMnSetupOn = 1;
		MENU_OPEN(SETUP);
		goto menu_start;
	}

	if(MENU_TITLE_IS(OFF)) {	// if not Main Menu

		if(LibUtlKeyPass(gbPassKeys2, sizeof(gbPassKeys2), &gbPassCnt2) == 1) {	// Debug MENU start
			//MN_MXSP = 0;
			//MN_POS_SIZ_SMALL
			MENU_OPEN(nDEBUG);
			goto menu_start;
		}

//		if (KEY_C && (!gbPassCnt1) && (!gbPassCnt2)) {						// Main  MENU start
		if (KEY_C &&
			(!gbPassCnt2) && (!gbPassCnt3) && (!gbPassCnt4) &&
			(!gbPassCnt5) && (!gbPassCnt6)						) {
			//DispClr(gbIMDFontY,gbIMDFontX,18);
			MENU_OPEN(MENU);
			goto menu_start;
		}
	}
 	else if(KEY_E) 	MENU_CLOSE();		// Menu out by ECM KEY_E


// Menu Exit by Time out  -------------------------------------------------------------------------

#if (MN_AUTO_OFF)
	static UINT gnMnTimeCnt = 0;

 	if ( (!MENU_TITLE_IS(OFF))&&(gbMnDebugFnc==0) ) {

		if (KEY) gnMnTimeCnt = 0;
		else {
			 gnMnTimeCnt++;
			if ( gnMnTimeCnt >= ( MN_OFF_TIME * (MN_FPS_LOW ? (FR>>1) : FR) ) )
			{
				gnMnTimeCnt = 0;
				UsrParSave(0);
				MENU_CLOSE();
			}

		}
  	}
  	else 	gnMnTimeCnt = 0;

	/* Debug
		FONT_ONw(1);
		DispDatDec("giLV____",	1, 24, giLV);
		DispDatDec("MenuCnt_",  2, 24, gnMnTimeCnt);
	*/
#endif

// Exceptional Menu --------------------------------------------------------------------------------

// CAM_TITLE
#if 1
	if(MENU_TITLE_IS(CAM_TITLE)) {
		if(KEY_L || KEY_R) {
			SETFONTATTR(gbMenuY+2, MN_XTITL+giStgPos[giLV], MN_WHITE);
			SETFONTATTR(gbMenuY+1, MN_XTITL+giStgPos[giLV], MN_WHITE);
			SETFONTID(gbMenuY+1, MN_XTITL+giStgPos[giLV], 0);

			menu_val(giStgPos+giLV, 1, 0, 7, 1);
			SETFONTATTR(gbMenuY+2, MN_XTITL+giStgPos[giLV], MN_YELLOW);
			SETFONTATTR(gbMenuY+1, MN_XTITL+giStgPos[giLV], MN_YELLOW);
			SETFONTID(gbMenuY+1, MN_XTITL+giStgPos[giLV], 162);
		}
		/*
		DispStr("gbCamTitle", 0, 30, 10);
		DispClrDec(gbCamTitle(giStgPos[giLV]), 0, 42, 10);
		*/
		if(KEY_D || KEY_U) {
		#if 0
			switch( gbCamTitle(giStgPos[giLV]) ) {
				case ' ' :	if(KEY_U) gbCamTitle(giStgPos[giLV]) = '0';
							if(KEY_D) gbCamTitle(giStgPos[giLV]) = 'Z';	break;
				case '0' :	if(KEY_U) gbCamTitle(giStgPos[giLV]) = '1';
							if(KEY_D) gbCamTitle(giStgPos[giLV]) = ' ';	break;
				case '9' :	if(KEY_U) gbCamTitle(giStgPos[giLV]) = 'A';
							if(KEY_D) gbCamTitle(giStgPos[giLV]) = '8';	break;
				case 'A' :	if(KEY_U) gbCamTitle(giStgPos[giLV]) = 'B';
							if(KEY_D) gbCamTitle(giStgPos[giLV]) = '9';	break;
				case 'Z' :	if(KEY_U) gbCamTitle(giStgPos[giLV]) = ' ';
							if(KEY_D) gbCamTitle(giStgPos[giLV]) = 'Y';	break;
				default  :	if(KEY_U) gbCamTitle(giStgPos[giLV])++;
							if(KEY_D) gbCamTitle(giStgPos[giLV])--;		break;
			}
		#else
			if(KEY_U) gbCamTitle(giStgPos[giLV]) = (gbCamTitle(giStgPos[giLV]) >= 'Z') ? ' ' : gbCamTitle(giStgPos[giLV]) + 1;	// TODO KSH Cam Title 외국어 다른 ID로 변경 필요
			if(KEY_D) gbCamTitle(giStgPos[giLV]) = (gbCamTitle(giStgPos[giLV]) <= ' ') ? 'Z' : gbCamTitle(giStgPos[giLV]) - 1;
		#endif
			DispStr((const char*)&gbCamTitle(0), gbMenuY+2, MN_XTITL, 0x8);
			OsdCamTitle();
		}
	}
#endif

#if 0
//	if (MENU_TITLE_IS(BACKLIGHT)&&(MP(MpBackLight)==MN_BL_BLC))	gbBlcOsdOn = 1;
	if ((MENU_TITLE_IS(BLC)&&((MP(MpBackLight)==MN_BL_BLC)||(MP(MpWdrBlc)!=MN_OFF)))||MP(MpBlcOsdOn))	gbBlcOsdOn = 1;
	else 																								gbBlcOsdOn = 0;
#endif


menu_end:

	//gbMenuState = !MENU_TITLE_IS(OFF);	// 0:Menu Close 1:Menu Open

	return;
}



/* EOF */
