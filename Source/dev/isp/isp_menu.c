/* **************************************************************************
 File Name	: 	menu.c
 Description:	EN675 - Menu function
 Designer	:	Kim, Sunghoon
 Date		:	19. 6. 7
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"
#include "isp_menu_str.h"

#include <string.h>		// strlen()


//*************************************************************************************************
// Global Menu Parameter & variables
//-------------------------------------------------------------------------------------------------
	WORD gwFontTest = 33;

	//BYTE gbMenuStyle = 0;

	BYTE gbBlcOsdOn = 0;

	BYTE gbMnPvcZone = 0;

	BYTE gbMnImdZone = 0;
	BYTE gbMnImdDetWin = 0;

	//BYTE gbMenuState = 0;

	BYTE gbMpAwbPrst = UP_OFF;

	BYTE gbMnDebugFnc = /*0*/1/*3*//*6*//*4*/;		// TODO KSH ◆ AE Debug OSD

	BYTE gbMnSetupOn = UP_ON/*UP_OFF*/;
	#define DEV_ON		(gbMnSetupOn==UP_ON)
	#define DEV_OFF		(gbMnSetupOn==UP_OFF)

	BYTE gbMpReset = UP_OFF;

	BYTE gbMnDebugBypass = UP_OFF;

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
#define MENU_STAGE_LV	6

int		giStgPos[MENU_STAGE_LV] = {-1,-1,-1,-1,-1,-1};		// Menu Position, bin 크기가 int형일 작음
int		giLV            = -1;								// Menu Stage Level, bin 크기가 int형일 작음

BYTE	gbPushCnt		= 0;

#define	gbMenuY	  MN_YSTART
//BYTE	gbMenuY	= MN_YSTART;
UINT	DRAW_Y	= MN_YSTART;

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
	FontStrEx(gnFontYw-1, 0, NO_ALPHA, MN_GREEN, (const char*)&gbCamTitle(0), 0x8);
#else
//	DispClr(0, ISP_FONT_EAX-9, 0x8);	SetFontAttrs(0, ISP_FONT_EAX-9, 0x8, MN_WHITE);
//	DispClr(19, 0, 0x8);				SetFontAttrs(19, 0, 0x8, MN_WHITE);
//	DispClr(17, 0, 0x8);				SetFontAttrs(17, 0, 0x8, MN_WHITE);

	UINT nUpCamTitleOn = UP(CamTitleOn);
	if(gbMnDebugFnc && nUpCamTitleOn == UP_CT_RIGHT_UP) nUpCamTitleOn = UP_CT_LEFT_DOWN;

	//if(gbMnDebugFnc == 0) {
		switch(nUpCamTitleOn) {
			case UP_CT_RIGHT_UP  :	//DispStr((const char*)&gbCamTitle(0), 0, ISP_FONT_EAX-9, 0x8);
									//SetFontAttrs(0, ISP_FONT_EAX-9, 0x8, MN_BLACK);
									FontClr(gnFontYw-1, 0, 0x8);
									FontStrEx(0, gnFontXw-8, NO_ALPHA, MN_GREEN, (const char*)&gbCamTitle(0), 0x8);
									break;

			case UP_CT_LEFT_DOWN :
//			  #if (model_2M30p)
//				if (UP(OutFps)!=UP_2M_1080p_25_30) { DispStr((const char*)&gbCamTitle(0), 19, 0, 0x8); SetFontAttrs(19, 0, 0x8, MN_BLACK); }
//				else								 { DispStr((const char*)&gbCamTitle(0), 17, 0, 0x8); SetFontAttrs(17, 0, 0x8, MN_BLACK); }
//
//			  #elif (model_2M)
//				if ((UP(OutFps)==UP_2M_1080p_25_30)||(UP(OutFps)==UP_2M_1080p_50_60))
//					                                 { DispStr((const char*)&gbCamTitle(0), 17, 0, 0x8); SetFontAttrs(17, 0, 0x8, MN_BLACK); }
//				else 								 { DispStr((const char*)&gbCamTitle(0), 19, 0, 0x8); SetFontAttrs(19, 0, 0x8, MN_BLACK); }
//
//			  #else
//				DispStr((const char*)&gbCamTitle(0), 19, 0, 0x8);
//				SetFontAttrs(19, 0, 0x8, MN_BLACK);
//
//			  #endif
				FontClr(0, gnFontXw-8, 0x8);
				FontStrEx(gnFontYw-1, 0, NO_ALPHA, MN_GREEN, (const char*)&gbCamTitle(0), 0x8);
				break;

			default :
				FontClr(gnFontYw-1, 0, 0x8);
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
		if(MENU_TITLE_IS(OFF))	{
			nFontSize = SMALL_FONT;
			nFontWidx = (UP(CamTitleOn) == UP_CT_RIGHT_UP) ? SMALL_FONT_AREA_W : SMALL_FONT_AREA_H;
		}
		else {
			nFontSize = BIG_FONT;
			nFontWidx = BIG_FONT_AREA;
		}
	}
	else {
		nFontSize = SMALL_FONT;
		nFontWidx = SMALL_FONT_AREA_H;
	}

	FontSetSize(nFontSize, nFontWidx, (gbMnDebugFnc) ? BORDER_NORMAL : BORDER_THICK);

	MN_MXSP = (gbMnDebugFnc) ? MN_MXSP_DBG : MN_MXSP_DEF;

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

PEXCH *gbStgTitle[MENU_STAGE_LV] = {(PEXCH*)_S(OFF),};

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
	if((giLV+1) >= MENU_STAGE_LV) {
		UartTxStr("Menu Stage Level is Max !!!");
		return;
	}

	gbMenuTitle = (PEXCH*)Title;

	if(giLV >= 0) {		// 이전 메뉴가 있는 경우 (이전 메뉴가 MENU_EXT 형식이면 안됨!!!)
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

	//gbMenuStyle = 0;

	//gbMnSetupOn = 0;

	gbMnImdDetWin = 0;

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
		if(gbUsrParTbl < Val && (Val+abValSize) < (gbUsrParTbl+USR_PAR_EA)) UsrParChg(Val-gbUsrParTbl);
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
		if(gbUsrParTbl < (BYTE*)Val && ((BYTE*)Val+abValSize) < (gbUsrParTbl+USR_PAR_EA)) return 1;//UsrParChg((BYTE*)Val-gbUsrParTbl);
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
		if(UP(Itl)==UP_ON && UP(ItlAlarm)==UP_ON && (gbMenuY+giMenuNum)<=gbIMDFontY) DispClr(gbIMDFontY, gbIMDFontX, 18);	// 170725 KSH
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
void InitMenu(void)
{	// Initiate Menu
	UartTxStrNoIRQ("Menu Init... ");

	UINT i;
	UINT nResetKey = 0;

	#if (model_Key==0)		// AD Key
		nResetKey = (ADC_KEYr<(((UINT)UP(ADC_LEVEL_U)+(UINT)UP(ADC_LEVEL_D))>>1));           // 141031 LH

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
		UsrParReset();			// gbUsrParTbl[] 초기화 & UsrParCpy(gbUsrParTblSaved, gbUsrParTbl) 실행
		gbUsrParSaveChk = 1;	// ROM 저장 실행

		UartTxStrNoIRQ("MENU PAR reset ");
    }
}

//*************************************************************************************************
// Menu
//-------------------------------------------------------------------------------------------------
void Menu(void)
{
	//if (UP(DefDet)==1)	return;

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
	MENU_SET( 11, MENU, UP_ON,
			EXPOSURE,		MENU_ONEi(UP_ON, e, UP_ON, EXPOSURE, ),

			BACKLIGHT, 		MENU_STRi(UP_ON, UP(BackLight)!=UP_BL_OFF, BACKLIGHT,
								if (MENU_VAL_IS(HLCe)) 		{MENU_TITLE_CHANGE(HLC)}
								else if (MENU_VAL_IS(BLCe)) {MENU_TITLE_CHANGE(BLC)}
								else if (MENU_VAL_IS(WDRe)) {MENU_TITLE_CHANGE(WDR)}, , UP(BackLight), 4, OFF, HLCe, BLCe, WDRe),
		#if(model_Lens==0)
			FOCUS_ADJ, 		MENU_STRi(UP_ON, MENU_VAL_IS(ONe) ,FOCUS_ADJ, MENU_OFF_GRAY_ONLY();, if_KEY_LR(MENU_REDRAW_GRAY_ONLY()), UP(FocusAdj_On), 2, OFF, ONe), // 150325 WHL
			//MENU_ONEn(UP_OFF, OFF),
		#elif(model_Lens==1)
			MOTORIZED,		if(DEV_ON){ MENU_ONEi(UP_ON, e, UP_ON, MOTOR_SETUP, )	MENU_NAME_REDRAW(MOTOR_SETUP) }
							else      { MENU_ONEi(UP_ON, e, UP_ON, MOTORIZED, )		MENU_NAME_REDRAW(MOTORIZED) },
		#endif
			DAYnNIGHT,		MENU_STRi(UP_ON, (MENU_VAL != _S(COLOR)), DAYnNIGHT, , ,UP(Tdn), 4, AUTOe, COLOR, BnWe, EXTERNe),		// 141120 LH
			COLORm,			MENU_ONEi(UP_ON, e, UP_ON, COLORm, ),

			DNR,			MENU_ONEi(UP_ON, e, UP_ON, DNR, ),
							//MENU_STRi(UP_ON, (MENU_VAL != _S(OFF)), DNR, , ,UP(Adnr3D), 4, OFF, LOWe, MIDDLEe, HIGHe),

			IMAGE,			MENU_ONEi(UP_ON, e, UP_ON, IMAGE, ),

			MOTION, 		MENU_STRi(UP_ON, MENU_VAL_IS(ONe) ,MOTION, MENU_OFF_GRAY_ONLY();, , UP(Itl), 2, OFF, ONe),				//lgh

			OUTPUTSET,      MENU_ONEi(UP_ON, e, UP_ON, OUTPUTSET, ),
			SYSTEM, 		if(DEV_ON){ MENU_ONEi(UP_ON, e, UP_ON, SETUP, )  MENU_NAME_REDRAW(SETUP) }
							else      { MENU_ONEi(UP_ON, e, !iSetMnOn, SYSTEM, /*MENU_OFF_GRAY_ONLY();*/ )  MENU_NAME_REDRAW(SYSTEM) },
			EXIT,			MENU_STRo(UP_ON, UP_ON, UsrParSave(gbMnExit==0); gbMnExit = 0;, , gbMnExit, 2, SAVEe, CANCELe))


// MENU - EXPOSURE ----------------------------------------------------------------------------------------------------
#if(model_Iris==0)						// no Iris
	#define MENU_NUM_EXPOSURE	2
#else
	#define MENU_NUM_EXPOSURE	3
#endif

	// MENU - EXPOSURE
	MENU_SET( MENU_NUM_EXPOSURE, EXPOSURE, UP_ON,
			BRIGHTNESS, 	if(DEV_ON){ MENU_ONEi(UP_ON, e, UP_ON, BRIGHTNESS, ) }
							else      { MENU_BARn(UP_ON, , UP(Brightness), 0, 20, 1) },
		#if(model_Iris_DC)		// DC Iris
			nIRIS,			MENU_STRn(UP_ON, , UP(Iris), 2, ELC, ALC),
		#elif(model_Iris_P)		// pIris, AF Iris
			nIRIS,			MENU_STRi(UP_ON, UP(Iris)==UP_LENS_MNL, IRIS, , , UP(Iris), 2, MANUALe, AUTO),
		#endif
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - EXPOSURE - BRIGHTNESS
	MENU_SET( 3, BRIGHTNESS, UP_ON,
			DAY,			MENU_BARn(UP_ON, , UP(Brightness), 0, 20, 1),
							/*({ menu_bar(UP_ON, &UP(Brightness), 0, 20, 1, _S());
							   SETFONTID(DRAW_Y, MN_SXSP+3, ':');
							   DispClrDec(TgtMaxGet(0,0), DRAW_Y, MN_SXSP+5, 3);
							   MENU_CODE(if_KEY_LR(MENU_REDRAW())) }),*/
			NIGHT,			MENU_BARn(UP_ON, , UP(BrightnessMin), 0, 20, 1),
							/*({ menu_bar(UP_ON, &UP(BrightnessMin), 0, 20, 1, _S());
							   SETFONTID(DRAW_Y, MN_SXSP+3, ':');
							   DispClrDec(TgtMinGet(TgtMaxGet(0,0), UP(BrightnessMin)), DRAW_Y, MN_SXSP+5, 3);
							   MENU_CODE(if_KEY_LR(MENU_REDRAW())) }),*/
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

// MENU - BACKLIGHT ----------------------------------------------------------------------------------------------------
	// MENU - BACKLIGHT:HLC
	MENU_SET( 3, HLC, UP(BackLight) == UP_BL_HLC,
			LEVEL,			MENU_BARn(UP_ON, , UP(HlMaskThrs), 0, 20, 1),
			COLOR,			MENU_STRn(UP_ON, , UP(HlMaskColorSel), 9, WHT, YEL, CYN, GRN, MAG, RED, BLU, BLK, CUSTOMIZE),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - BACKLIGHT:BLC
	MENU_SET( 6, BLC, UP(BackLight) == UP_BL_BLC,
			MODE,			MENU_STRn(UP_ON, if_KEY_LR(MENU_REDRAW()), UP(BlcMode), 3, CUSTOMIZE, CENTER, SPOT),
			HPOS,			MENU_DECn(UP(BlcMode)==UP_BLC_MODE_CUSTOMIZE, , UP(BlcPosX), 0, 20, 1, ),
			VPOS,			MENU_DECn(UP(BlcMode)==UP_BLC_MODE_CUSTOMIZE, , UP(BlcPosY), 0, 20, 1, ),
			HSIZE,			MENU_DECn(UP(BlcMode)==UP_BLC_MODE_CUSTOMIZE, , UP(BlcSizX), 0, 20, 1, ),
			VSIZE,			MENU_DECn(UP(BlcMode)==UP_BLC_MODE_CUSTOMIZE, , UP(BlcSizY), 0, 20, 1, ),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - BACKLIGHT:WDR
	MENU_SET( 7, WDR, UP(BackLight) == UP_BL_WDR,
			WEIGHT,			MENU_STRn(UP_ON, , UP(WdrWgt), 3, LOW, MIDDLE, HIGH),
			Ap_BRIGHT_L,	MENU_DECn(UP_ON, , UP(AE_WDR_LTGT_OFST2), 0, 255, 1, ),
			Ap_BRIGHT_S,	MENU_DECn(UP_ON, , UP(AE_WDR_STGT_OFST2), 0, 255, 1, X4 ),
			Dp_BRIGHT,		MENU_BARn(UP_ON, , UP(ACEWDR1_TH), 0, 255, 1 ),
			Dp_CONTRAST,	MENU_BARn(UP_ON, , UP(ACEWDR2_TH), 0, 255, 1 ),
			GAMMA,			MENU_STRi(UP_ON, MENU_VAL_IS(AUTOe), AUTO_GAMMA, , , UP(GammaWdr), 9, GAMMA_045, GAMMA_050, GAMMA_055, GAMMA_060, GAMMA_065, GAMMA_070, GAMMA_075, AUTOe, DEFAULT),
		 	RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - Focus ----------------------------------------------------------------------------------------------------
#if (model_Lens==0)
	MENU_SET( 3, FOCUS_ADJ, UP_ON,
			LEVEL,			MENU_BARn(UP_ON, , UP(FocusThrs), 0, 20, 1),
			COLOR,			MENU_STRn(UP_ON, , UP(FocusAdjColorSel), 6, WHT, YEL, CYN, RED, BLU, BLK),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))
#endif

// MENU - COLOR ----------------------------------------------------------------------------------------------------
	MENU_SET( 7, COLORm, UP_ON/*DEV_OFF*/,
			AWB,			MENU_STRi(UP_ON, MENU_VAL_IS(MANUALe), AWB, ,
										MENU_VAL_PUSH(PRESETp, PUSHING, PUSH_DELAY_NOR, gbMpAwbPrst = UP_OFF, gbMpAwbPrst = UP_ON),
										UP(Awb), 4, AUTO, AUTOext, PRESETp, MANUALe),
			STYLE,			MENU_STRn(UP_ON, /*if_KEY_LR(Hue_Chroma_Menu_Set(0))*/, UP(AwbStyle), 6, A_TYPE, B_TYPE, C_TYPE, D_TYPE, E_TYPE, F_TYPE),
			R_GAIN,			MENU_BARn(UP_ON, , UP(SaturationR), 0, 64, 1 ),	// 2017419 - WHL : CBB TEST
			G_GAIN,			MENU_BARn(UP_ON, , UP(Saturation), 0, 64, 1 ),
			B_GAIN,			MENU_BARn(UP_ON, , UP(SaturationB), 0, 64, 1 ),
			HUEnCHROMA,		MENU_ONEi(UP_ON, e, UP_ON, HUEnCHROMA, ),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - COLOR - AWB:MANUAL
	MENU_SET( 4, AWB, UP(Awb) == UP_AWB_MNL,
			C_TEMP,			MENU_STRn(UP_ON, , UP(AwbMnl), 3, TEMP1, TEMP2, TEMP3),
			RGAIN,			MENU_BARn(UP_ON, , UP(Rgain), 0, 20, 1),
			BGAIN,			MENU_BARn(UP_ON, , UP(Bgain), 0, 20, 1),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - COLOR - HUE & CHROMA
	MENU_SET( 10, HUEnCHROMA, UP(HueChromaMode) == UP_HUE_GAIN_OLD,
			MODE,			MENU_STRn(UP_ON, if_KEY_LR(MENU_CHANGE()), UP(HueChromaMode), 2, NEW, OLD),
			YEL_REDdGRN,	MENU_BARn(UP_ON, , UP(Yellow_HUE_RedToGreen), 1, 255, 1),
			YEL_GAIN,		MENU_BARn(UP_ON, , UP(Yellow_GAIN), 0, 255, 1),
			RED_YELdBLU,	MENU_BARn(UP_ON, , UP(Red_HUE_YellowToBlue), 1, 255, 1),
			RED_GAIN,		MENU_BARn(UP_ON, , UP(Red_GAIN), 0, 255, 1),
			BLU_GRNdRED,	MENU_BARn(UP_ON, , UP(Blue_HUE_GreenToRed), 1, 255, 1),
			BLU_GAIN,		MENU_BARn(UP_ON, , UP(Blue_GAIN), 0, 255, 1),
			GRN_BLUdYEL,	MENU_BARn(UP_ON, , UP(Green_HUE_BlueToYellow), 1, 255, 1),
			GRN_GAIN,		MENU_BARn(UP_ON, , UP(Green_GAIN), 0, 255, 1),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	MENU_SET( 6, HUEnCHROMA, UP(HueChromaMode) == UP_HUE_GAIN_NEW,
			MODE,			MENU_STRn(UP_ON, if_KEY_LR(MENU_CHANGE()), UP(HueChromaMode), 2, NEW, OLD),
			YELLOWnRED,		MENU_ONEi(UP_ON, e, UP_ON, YELLOWnRED, ),
			REDnBLUE,		MENU_ONEi(UP_ON, e, UP_ON, REDnBLUE, ),
			BLUEnGREEN,		MENU_ONEi(UP_ON, e, UP_ON, BLUEnGREEN, ),
			GREENnYELLOW,	MENU_ONEi(UP_ON, e, UP_ON, GREENnYELLOW, ),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - COLOR - HUE & CHROMA - YELLOW&RED
	MENU_SET( 5, YELLOWnRED, UP_ON,
			YELLOWdBLUE,	MENU_BARn(UP_ON, , UP(Yellow_Red_HUE_YellowToBlue), 1, 255, 1),
			REDdGREEN,		MENU_BARn(UP_ON, , UP(Yellow_Red_HUE_RedToGreen), 1, 255, 1),
			GAIN_YELLOW,	MENU_BARn(UP_ON, , UP(Yellow_Red_GAIN_Yellow), 0, 255, 1),
			GAIN_RED,		MENU_BARn(UP_ON, , UP(Yellow_Red_GAIN_Red), 0, 255, 1),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - COLOR - HUE & CHROMA - RED&BLUE
	MENU_SET( 5, REDnBLUE, UP_ON,
			YELLOWdBLUE,	MENU_BARn(UP_ON, , UP(Red_Blue_HUE_YellowToBlue), 1, 255, 1),
			GREENdRED,		MENU_BARn(UP_ON, , UP(Red_Blue_HUE_GreenToRed), 1, 255, 1),
			GAIN_RED,		MENU_BARn(UP_ON, , UP(Red_Blue_GAIN_Red), 0, 255, 1),
			GAIN_BLUE,		MENU_BARn(UP_ON, , UP(Red_Blue_GAIN_Blue), 0, 255, 1),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - COLOR - HUE & CHROMA - BLUE&GREEN
	MENU_SET( 5, BLUEnGREEN, UP_ON,
			BLUEdYELLOW,	MENU_BARn(UP_ON, , UP(Blue_Green_HUE_BlueToYellow), 1, 255, 1),
			GREENdRED,		MENU_BARn(UP_ON, , UP(Blue_Green_HUE_GreenToRed), 1, 255, 1),
			GAIN_BLUE,		MENU_BARn(UP_ON, , UP(Blue_Green_GAIN_Blue), 0, 255, 1),
			GAIN_GREEN,		MENU_BARn(UP_ON, , UP(Blue_Green_GAIN_Green), 0, 255, 1),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - COLOR - HUE & CHROMA - GREEN&YELLOW
	MENU_SET( 5, GREENnYELLOW, UP_ON,
			BLUEdYELLOW,	MENU_BARn(UP_ON, , UP(Green_Yellow_HUE_BlueToYellow), 1, 255, 1),
			REDdGREEN,		MENU_BARn(UP_ON, , UP(Green_Yellow_HUE_RedToGreen), 1, 255, 1),
			GAIN_GREEN,		MENU_BARn(UP_ON, , UP(Green_Yellow_GAIN_Green), 0, 255, 1),
			GAIN_YELLOW,	MENU_BARn(UP_ON, , UP(Green_Yellow_GAIN_Yellow), 0, 255, 1),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

// MENU - DNR ----------------------------------------------------------------------------------------------------
	MENU_SET( 3, DNR, UP_ON,
			DNR_3D,			MENU_STRn(UP_ON, , UP(Adnr3D), 4, OFF, LOW, MIDDLE, HIGH),
			DNR_2D,			MENU_BARn(UP_ON, , UP(Adnr2D), 0, 8, 1),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

// MENU - IMAGE ----------------------------------------------------------------------------------------------------
	MENU_SET( 5, IMAGE, UP_ON,
			GAMMA,			MENU_STRi(UP_ON, MENU_VAL_IS(AUTOe), AUTO_GAMMA, , , UP(Gamma), 8, GAMMA_045, GAMMA_050, GAMMA_055, GAMMA_060, GAMMA_065, GAMMA_070, GAMMA_075, AUTOe),
			FLIP,			MENU_STRn(UP_ON, , UP(Flip), 2, OFF, ON),
			MIRROR,			MENU_STRn(UP_ON, , UP(Mirror), 2, OFF, ON),
			PRIVACY,		MENU_STRi(UP_ON, MENU_VAL_IS(ONe), PRIVACY, MENU_OFF_GRAY_ONLY(), , UP(PvcOn), 2, OFF, ONe),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - IMAGE - GAMMA
	MENU_SET( 3, AUTO_GAMMA, UP_ON,
			DAY,			MENU_STRn(UP_ON, , UP(GammaDay), 7, GAMMA_045, GAMMA_050, GAMMA_055, GAMMA_060, GAMMA_065, GAMMA_070, GAMMA_075),
			NIGHT,			MENU_STRn(UP_ON, , UP(GammaNgt), 7, GAMMA_045, GAMMA_050, GAMMA_055, GAMMA_060, GAMMA_065, GAMMA_070, GAMMA_075),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - IMAGE - PRIVACY
	MENU_SET( 12, PRIVACY, UP_ON,
			ZONENUM,		MENU_DECn(UP_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), gbMnPvcZone, 0, PVC_EA-1, 1, ),
			ZONEDISP,		MENU_STRn(UP_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), gbMnPvcCfg(gbMnPvcZone)->bAction, 2, OFF, ON),
			HPOS,			MENU_DECn(gbMnPvcCfg(gbMnPvcZone)->bAction, , gbMnPvcCfg(gbMnPvcZone)->bPosX, 0, BOX_XMAX, 0, ),
			VPOS,			MENU_DECn(gbMnPvcCfg(gbMnPvcZone)->bAction, , gbMnPvcCfg(gbMnPvcZone)->bPosY, 0, BOX_YMAX, 0, ),
			HSIZE,			MENU_DECn(gbMnPvcCfg(gbMnPvcZone)->bAction, , gbMnPvcCfg(gbMnPvcZone)->bSizX, 1, BOX_XMAX, 0, ),
			VSIZE,			MENU_DECn(gbMnPvcCfg(gbMnPvcZone)->bAction, , gbMnPvcCfg(gbMnPvcZone)->bSizY, 1, BOX_YMAX, 0, ),
			PRV_MASKMOD,	MENU_STRn(UP_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), UP(PvcFormat), 2, PRV_FILL, PRV_FORMAT),
			YLEVEL, 		MENU_BARn(UP_ON, , UP(PvcYLevel), 0, UpBOX_COLOR_STEP, 1),
			CBLEVEL,		MENU_BARn(UP_ON, , UP(PvcCBLevel),0, UpBOX_COLOR_STEP, 1),
			CRLEVEL,		MENU_BARn(UP_ON, , UP(PvcCRLevel),0, UpBOX_COLOR_STEP, 1),
			TRANS,			MENU_DECn(UP_ON, , UP(PvcTrans), 0, 3, 1, ),
			RETURN, 		MENU_ONEo(UP_ON, e, UP_ON, ))

// MENU - MOTION ----------------------------------------------------------------------------------------------------
	MENU_SET( 8, MOTION, UP_ON,
			DET_WINDOW,		MENU_ONEi(UP_ON, e, UP_ON, DET_WINDOW, MENU_OFF_GRAY_ONLY(); gbMnImdDetWin = 1;),
			DET_TONE,		MENU_DECn(UP_ON, , UP(ItlDettone), 0, 4, 1, ),
			MDRECT_FILL,	MENU_STRn(UP_ON, , UP(ItlRectFill), 2, OFF, ON),
			SENSITIVITY, 	MENU_BARn(UP_ON, , UP(ItlSens),0, 10, 1)
							if(DEV_ON){
								MENU_IN(UP_ON, SENSITIVITY, )
								SETFONTID(DRAW_Y, MN_SXSP+10, 0xa6);
							},
			MOTION_OSD, 	MENU_STRn(UP_ON, , UP(ItlMaskOsd), 2, OFF, ON),
			TEXT_ALARM, 	MENU_STRn(UP_ON, , UP(ItlAlarm), 2, OFF, ON),
			SIGNAL_OUT, 	MENU_STRn(UP_ON, , UP(ItlSigno), 2, OFF, ON),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON,))

	MENU_SET( 7, DET_WINDOW, UP_ON,
			WINDOWZONE,		MENU_DECn(UP_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), gbMnImdZone, 0, MASK_EA-1, 1,),
			WINDOWUSE,		MENU_STRn(UP_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), gbMnImdCfg(gbMnImdZone)->bAction, 2, OFF, ON),
			DET_H_POS, 		MENU_DECn(gbMnImdCfg(gbMnImdZone)->bAction, , gbMnImdCfg(gbMnImdZone)->bPosX, 0, IMD_HW, 0, ),
			DET_V_POS, 		MENU_DECn(gbMnImdCfg(gbMnImdZone)->bAction, , gbMnImdCfg(gbMnImdZone)->bPosY, 0, IMD_VW, 0, ),
			DET_H_SIZE, 	MENU_DECn(gbMnImdCfg(gbMnImdZone)->bAction, , gbMnImdCfg(gbMnImdZone)->bSizX, 1, IMD_HW, 0, ),
			DET_V_SIZE, 	MENU_DECn(gbMnImdCfg(gbMnImdZone)->bAction, , gbMnImdCfg(gbMnImdZone)->bSizY, 1, IMD_VW, 0, ),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, gbMnImdDetWin = 0; ))

	MENU_SET( 4, SENSITIVITY, UP_ON,
			DETp_MOTION,	MENU_BARn(UP_ON, , UP(ItlSens),0, IMD_SENS_MAX, 1),
			CAMp_MOVING, 	MENU_BARn(UP_ON, , UP(ItlCamMovingTH),0, 20, 1),
			BRIGHT_CHGp, 	MENU_BARn(UP_ON, , UP(ItlBrightChgTH),0, 200, 1),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

// MENU - OUTPUTSET ----------------------------------------------------------------------------------------------------
	MENU_SET( 2, OUTPUTSET, UP_ON,
			FREQ,			MENU_STRn(UP_ON, if_KEY_LR(MENU_REDRAW()), UP(SysFreq), 2, 50HZ, 60HZ),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))


// MENU - SYSTEM ----------------------------------------------------------------------------------------------------
	MENU_SET( 5, SYSTEM, UP_ON,
			CAM_TITLE,		MENU_STRi(UP_ON, UP(CamTitleOn)!=UP_CT_OFF, CAM_TITLE, , , UP(CamTitleOn), 3, OFF, RIGHT_UPe, LEFT_DOWNe),
			LANGUAGE,		MENU_STRn(UP_ON, if_KEY_LR(MENU_CHANGE()), UP(LanguageSel), 5, ENG, CHN, CHNs, JPN, KOR),
			COLOR_BAR,		MENU_STRn(UP_ON, , UP(ColorBar), 2, OFF, ON),
			RESET,			MENU_PUSH(UP_ON, gbMpReset, ONp, PUSHING, PUSH_DELAY_NOR, UsrParReset(); MENU_CHANGE(); ),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))

	// MENU - SYSTEM - CAM_TITLE
	MENU_EXT( CAM_TITLE, UP_ON, giMenuNum = 8;/*나가기 전에 SubMenu 7번째 라인까지 지움*/,

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
	MENU_SET(8, SETUP, UP_ON,
			DEVELOPER,		MENU_STRn(UP_ON, , gbMnSetupOn, 2, OFF, ON),
			STYLE,			MENU_STRn(UP_ON, if_KEY_LR(/*UsrParStyle(gbMenuStyle, 0, 0);*/ MENU_CHANGE()), UP(Style)/*gbMenuStyle*/, 8, /*CUSTOMIZE*/PREVIOUS, IPC_INDOOR, IPC_OUTDOOR, CAR_REC, ACTION_REC, WDR, LOW_BIT, CUSTOM),

			CAM_TITLE,		MENU_STRi(UP_ON, UP(CamTitleOn)!=UP_CT_OFF, CAM_TITLE, , , UP(CamTitleOn), 3, OFF, RIGHT_UPe, LEFT_DOWNe),
			LANGUAGE,		MENU_STRn(UP_ON, if_KEY_LR(MENU_CHANGE()), UP(LanguageSel), 5, ENG, CHN, CHNs, JPN, KOR),
			COLOR_BAR,		MENU_STRn(UP_ON, , UP(ColorBar), 2, OFF, ON),
			CAM_VERSION,	MENU_DIRn(UP_ON
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
			RESET,			MENU_PUSH(UP_ON, gbMpReset, ONp, PUSHING, PUSH_DELAY_NOR, UsrParReset(); MENU_CHANGE(); ),
			RETURN,			MENU_ONEo(UP_ON, e, UP_ON, ))


// Debug MENU ----------------------------------------------------------------------------------------------------
	MENU_SET(7, nDEBUG, UP_ON,
			FUNCTION,		MENU_STRn(UP_ON, if_KEY_LR(ChangeMenuSize(); MENU_CHANGE()) , gbMnDebugFnc, 9, OFF, AE, AF, AWB, DBG_4, DBG_5, IMD_DBG, TDN_DBG, STATUS),
			BYPASS,			MENU_STRn(UP_ON, , gbMnDebugBypass, 2, OFF, ON),
			PAR_NUM,		MENU_DECn(UP_ON, if_KEY_LR(MENU_REDRAW()), gbMnParNum, 0, 31, 1, ),
		#if USE_UP_PAR == 0
			PAR_VAL,		menu_bar(UP_ON, gnRxPar+gbMnParNum, 4, 0, 0xFFFFFFFF, 1, _S(), gbMnParType);  MENU_DISABLE(UP_ON)  MENU_CODE( ),
		#else
			PAR_VAL,		menu_bar(UP_ON, (gbUsrParTbl+UPi(UpPAR00)+(gbMnParNum<<2)), 4, 0, 0xFFFFFFFF, 1, _S(), gbMnParType);  MENU_DISABLE(UP_ON)  MENU_CODE( ),
		#endif
			PAR_TYPE,		MENU_STRn(UP_ON, if_KEY_LR(MENU_REDRAW()), gbMnParType, 2, DEC, HEX),
			FONT_TEST,		menu_bar(UP_ON, &gwFontTest, sizeof(gwFontTest), 0, ISP_FONT_CHAR_EA-1, 1, _S(), 0); SETFONTID(DRAW_Y, MN_SXSP+4, ':'); SETFONTID(DRAW_Y, MN_SXSP+6, '\''); SETFONTID(DRAW_Y, MN_SXSP+7, gwFontTest); SETFONTID(DRAW_Y, MN_SXSP+8, '\''); ,	// TODO KSH> Font Test Menu
			nEXIT,			MENU_STRo(UP_ON, UP_ON, UsrParSave(gbMnExit==0); gbMnExit = 0;, , gbMnExit, 2, SAVEe, CANCELe))


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
			if ( gnMnTimeCnt >= ( MN_OFF_TIME * IF_FUNC_FPS ) )
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
			if(KEY_U) gbCamTitle(giStgPos[giLV]) = (gbCamTitle(giStgPos[giLV]) >= 'Z') ? ' ' : gbCamTitle(giStgPos[giLV]) + 1;	// TODO KSH> Cam Title 외국어 다른 ID로 변경 필요
			if(KEY_D) gbCamTitle(giStgPos[giLV]) = (gbCamTitle(giStgPos[giLV]) <= ' ') ? 'Z' : gbCamTitle(giStgPos[giLV]) - 1;
		#endif
			DispStr((const char*)&gbCamTitle(0), gbMenuY+2, MN_XTITL, 0x8);
			OsdCamTitle();
		}
	}
#endif

	gbBlcOsdOn = ((MENU_TITLE_IS(BLC)&&((UP(BackLight)==UP_BL_BLC)/*||(UP(WdrBlc)!=UP_OFF)*/))/*||UP(BlcOsdOn)*/) ? 1 : 0;


menu_end:

	//gbMenuState = !MENU_TITLE_IS(OFF);	// 0:Menu Close 1:Menu Open

	return;
}



/* EOF */
