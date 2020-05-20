/* **************************************************************************
 File Name  :   isp_par.c
 Description:   EN675 - User Parameter Control
 Designer   :   Kim, Sunghoon
 Date       :   19. 6. 3
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"
#include "isp_font_tbl.h"	// SetFontChg()
#include <string.h>			// strlen()


#define UP_DATA_ID			gDataID
#define UD_DATA_ID			gDataID


#define EEPROM_ADDR_USER	0						// Menu par save pos (1024 byte space)
#define EEPROM_ADDR_DATA	1024					// Application data save pos (1024 byte space)

//*************************************************************************************************
// Variables
//-------------------------------------------------------------------------------------------------
const char *gsBuildDate = "FW Date : "__DATE__" "__TIME__;
WORD gDataID = 0;
BYTE gbUsrParChgOn = 0;	// 0:실행안함, 1:부팅&변경, 2:변경
BYTE gbStylePreviousOn = 0;

// User Parameter --------------------------------------------------
BYTE gbUsrParSaveChk=0;
BYTE gbUsrParReadChk=0;
__attribute__((__aligned__(4))) BYTE gbUsrParTbl[USR_PAR_EA];
__attribute__((__aligned__(4))) BYTE gbUsrParTblSaved[USR_PAR_EA];

// User Data -------------------------------------------------------
BYTE gbUsrDataSaveChk=0;
BYTE gbUsrDataReadChk=0;
BYTE gbUsrDataTbl[USR_DATA_EA];


//*************************************************************************************************
// Functions
//-------------------------------------------------------------------------------------------------
BYTE Month2Hex(const char *Month)
{
	if(*Month=='J') {
		Month++;
		if(*Month=='a') return 0x1;
		Month++;
		if(*Month=='n') return 0x6;
		if(*Month=='l') return 0x7;
		return 0;
	}

	if(*Month=='M') {
		Month++;
		Month++;
		if(*Month=='r') return 0x3;
		if(*Month=='y') return 0x5;
		return 0;
	}

	if(*Month=='A') {
		Month++;
		if(*Month=='p') return 0x4;
		if(*Month=='u') return 0x8;
		return 0;
	}

	if(*Month=='F') return 0x2;
	if(*Month=='S') return 0x9;
	if(*Month=='O') return 0x10;
	if(*Month=='N') return 0x11;
	if(*Month=='D') return 0x12;
	return 0;
}

void InitDataSet(void)
{
	const UINT gsBuildDateLen = strlen(gsBuildDate);
	gDataID  = (*(gsBuildDate + gsBuildDateLen-1) - '0');
	gDataID += (*(gsBuildDate + gsBuildDateLen-2) - '0')<<4;
	gDataID += (*(gsBuildDate + gsBuildDateLen-4) - '0')<<8;
	gDataID += (*(gsBuildDate + gsBuildDateLen-5) - '0')<<12;

	UP_LIB_LIST

	void SensorSetting(BYTE, BYTE, BYTE); SensorSetting(model_Sens_Ctrl, model_Sens_Intf, model_Sens_Fps);

	gbUsrParReadChk = 1;
	gbUsrDataReadChk = 1;

	//_printf_irq("=== %s ===\r\n", gsBuildDate);
}

void UsrParDateTime(void)
{
	const UINT gsBuildDateLen = strlen(gsBuildDate);
	UP(ISP_FW_Build_Year)  = (*(gsBuildDate + gsBuildDateLen-10) - '0');
	UP(ISP_FW_Build_Year) += (*(gsBuildDate + gsBuildDateLen-11) - '0')<<4;
	UP(ISP_FW_Build_Year) += (*(gsBuildDate + gsBuildDateLen-12) - '0')<<8;
	UP(ISP_FW_Build_Year) += (*(gsBuildDate + gsBuildDateLen-13) - '0')<<12;

	UP(ISP_FW_Build_MonthDay)  = (*(gsBuildDate + gsBuildDateLen-15) - '0');
	UP(ISP_FW_Build_MonthDay) += (*(gsBuildDate + gsBuildDateLen-16) - '0')<<4;
	UP(ISP_FW_Build_MonthDay) += Month2Hex(gsBuildDate + gsBuildDateLen-20)<<8;

	UP(ISP_FW_Build_Time)  = (*(gsBuildDate + gsBuildDateLen-4) - '0');
	UP(ISP_FW_Build_Time) += (*(gsBuildDate + gsBuildDateLen-5) - '0')<<4;
	UP(ISP_FW_Build_Time) += (*(gsBuildDate + gsBuildDateLen-7) - '0')<<8;
	UP(ISP_FW_Build_Time) += (*(gsBuildDate + gsBuildDateLen-8) - '0')<<12;
}

void UsrDataReset(void)
{	// User Data Reset
	UD(UD_START)		= (BYTE)((UD_DATA_ID>>8)&0xff);		// for verify
	UD(UD_END)			= (BYTE)((UD_DATA_ID)&0xff);			// "
}

void UsrParValid(const UINT anValid)
{
	if(anValid) {
		gbUsrParTbl[UP_START]		= (BYTE)((UP_DATA_ID>>8)&0xff);		// for verify
		gbUsrParTbl[UP_END]			= (BYTE)((UP_DATA_ID)&0xff);		// "
	}
	else {
		gbUsrParTbl[UP_START]		= (BYTE)(((~UP_DATA_ID)>>8)&0xff);
		gbUsrParTbl[UP_END]			= (BYTE)(((~UP_DATA_ID))&0xff);
	}
}

UINT UsrParBit(const UINT anIdx)
{
	UINT nBit = 0;

	#undef UP_SET
	#define UP_SET(...)
	#undef UP_TITLE
	#define UP_TITLE(N)		if(N##_UP_START < anIdx && anIdx <= (N##_UP_START + sizeof(UP_##N))) {\
								/*_printf("UP %d : "#N" (%d~%d) : %d\r\n", anIdx, N##_UP_START+1, (N##_UP_START + sizeof(UP_##N)), nBit);*/\
								return 1<<nBit;\
							} nBit++;
	USR_PAR_LIST
	#undef UP_TITLE
	#define UP_TITLE(N)

	return 0;
}

void UsrParChg(const UINT anStrIdx)
{
	if(gbUsrParChgOn == 0) return;

#ifdef __USE_ISP__
	switch(anStrIdx) {
		//#define INIT_NO
		#define INIT_RUN	|| gbUsrParChgOn==1
		#undef UP_SET
		#define UP_SET(S,N,D,I,...)		case UPi(N): if(gbUsrParChgOn==2 I) { __VA_ARGS__ /*printf("UsrParChg("#N"):%d\n", anIdx);*/ } break;
		USR_PAR_LIST
	}
#endif

	if(gbUsrParChgOn==2) {
		if(MpPvcCfgIdx <= anStrIdx && anStrIdx < MpItlWinCfgIdx) {
			PrivacyBox();
		}
		else if(MpItlWinCfgIdx <= anStrIdx && anStrIdx < USR_PAR_EA) {
			MaskIMD();
		}
		else if(anStrIdx < UP_END) {
			UsrParStyle(0, 0, anStrIdx);	// Style 값들 변경여부 체크 -> 값이 변경되었으면 /*CUSTOMIZE*/PREVIOUS 로
		}

#if 0
		static UINT anStrIdx1d = 0;
		if(anStrIdx1d != anStrIdx) {
			anStrIdx1d = anStrIdx;
			UsrParBit(anStrIdx);
		}
#endif

		//void TestInterp1D(void); TestInterp1D();

#if 0	// ddr write test
		IM_GO0w(1);
		_printf_irq("!!! IM_GO !!!\r\n");
#endif
	}
}

void UsrParChgEndIdx(const UINT anEndIdx)
{
	switch(anEndIdx) {
		#undef UP_SET
		#define UP_SET(S,N,D,...)		case UPinv(N): UsrParChg(UPi(N)); break;
		USR_PAR_LIST
	}
}

void UsrParCpy(BYTE *dest, BYTE *src)
{
	if(gbUsrParChgOn==2) {
		#undef UP_SET
		#define UP_SET(S,N,D,...)		if( ((UP_LIST*)dest)->N != ((UP_LIST*)src)->N ) { ((UP_LIST*)dest)->N = ((UP_LIST*)src)->N; UsrParChg(UPi(N)); }
		USR_PAR_LIST

		int i, iChg = 0;
		for(i=MpPvcCfgIdx; i<MpItlWinCfgIdx; i++) {
			if(dest[i] != src[i]) {
				dest[i] = src[i];
				iChg = 1;
			}
		}
		if(iChg) PrivacyBox();

		iChg = 0;
		for(i=MpItlWinCfgIdx; i<USR_PAR_EA; i++) {
			if(dest[i] != src[i]) {
				dest[i] = src[i];
				iChg = 1;
			}
		}
		if(iChg) MaskIMD();
	}
	else {
		for(int i=0; i<USR_PAR_EA; i++) dest[i] = src[i];
	}
}

BYTE UsrParSiz(const UINT anIdx)
{
	switch(anIdx) {
		#undef UP_SET
		#define UP_SET(S,N,D,...)		case UPi(N): return S;
		USR_PAR_LIST
	}

	return 0;
}

void InitUsrParChgAll(void)
{
	gbUsrParChgOn = 1;			// INIT_RUN 설정된 User Parameter 코드 실행

#if 0
	#undef UP_SET
	#define UP_SET(S,N,D,...)		UsrParChg(UPi(N));
	USR_PAR_LIST
#else
	for(UINT i=1; i<UP_END; i++) {
		UsrParChg(i);			// SensFlip(), SensMirror() 실행을 위해 Isp_Sensor_init()이 먼저 설정되어야 함, gbUsrParChgOn = 2 상태에서 UsrParChg()사용하는 경우 PrivacyBox()함수가 중복 호출됨!!!
	}
#endif

	gbUsrParChgOn = 2;			// User Parameter 변경 시 실행
}

void UsrParReset(void)
{	// User Parameter Reset
	int i;

	if(gbUsrParChgOn == 2) {
		for(i=0; i<USR_PAR_EA; i++) gbUsrParTblSaved[i] = gbUsrParTbl[i];	// Reset 실행전 상태 백업
	}

	//----------------------------------------------------------------------
	UsrParValid(1);

	#undef UP_SET
	#define UP_SET(S,N,D,...)		UP(N) = (D);//UPw(N,D);
	USR_PAR_LIST

	for(i=0; i<PVC_EA; i++) {
		gbMnPvcCfg(i)->bAction = (i<4) ? UP_ON : UP_OFF;
		gbMnPvcCfg(i)->bPosX   = 8+((i%6)*4);
		gbMnPvcCfg(i)->bPosY   = 2+((i/6)*4);
		gbMnPvcCfg(i)->bSizX   = 3;
		gbMnPvcCfg(i)->bSizY   = 3;
	}

	gbMnImdCfg(0)->bAction = /*UP_OFF*/UP_ON;
	gbMnImdCfg(0)->bPosX   = 0;
	gbMnImdCfg(0)->bPosY   = 0;
	gbMnImdCfg(0)->bSizX   = IMD_HW;
	gbMnImdCfg(0)->bSizY   = IMD_VW;
	for(i=1; i<MASK_EA; i++)
	{
		gbMnImdCfg(i)->bAction = /*UP_ON*/UP_OFF;
		gbMnImdCfg(i)->bPosX   = (IMD_HW-3)/3 * (i-1) + 1;
		gbMnImdCfg(i)->bPosY   = (IMD_VW-3)/3 * (i-1) + 1;
		gbMnImdCfg(i)->bSizX   = (IMD_HW-3)/3;
		gbMnImdCfg(i)->bSizY   = (IMD_VW-3)/3;
	}
//	gbMnImdCfg(2)->bPosX -= 2;		// TODO KSH ◆ IMD Test Init
//	gbMnImdCfg(2)->bPosY -= 2;
//	gbMnImdCfg(2)->bSizX += 4;
//	gbMnImdCfg(2)->bSizY += 4;
//	gbMnImdCfg(1)->bPosX = 0;
//	gbMnImdCfg(1)->bPosY = 0;
//	gbMnImdCfg(1)->bSizX += 1;
//	gbMnImdCfg(1)->bSizY += 1;
//	gbMnImdCfg(3)->bSizX += (IMD_HW>>1);
//	gbMnImdCfg(3)->bSizY += (IMD_VW>>1);


	UsrParStyle(INIT_STYLE, 1, 1);	// INIT_STYLE 로 Style값들 변경
	//----------------------------------------------------------------------
#if model_TgtBd == 1
	UP(ICSel) = 1;
	UP(OCSel) = 3;
#endif

	menu_val_chack();

	UsrParCpy(gbUsrParTblSaved, gbUsrParTbl);
}

void UsrParStyle(const int abStyle, const int abInit, const int abValChg)
{
	static int bStyleBuf = 0;

	#define UP_STYLE(N, ...)	const UPt(N) bStyleVals##N[] = { __VA_ARGS__ };
	USR_PAR_STYLE

	if(abInit) {
		bStyleBuf = abStyle;
		gbStylePreviousOn = (abStyle == 0);
		if(abStyle && abValChg) goto StyleSet;
		else return;
	}

	if(abValChg) {
		int nStyleChg = 0;

		if(bStyleBuf) {
			switch(abValChg) {
				#undef UP_STYLE
				#define UP_STYLE(N, ...)	case UPi(N): if((UINT)(bStyleBuf-1) < ARRAY_SIZE(bStyleVals##N)) { if(UP(N) != bStyleVals##N[bStyleBuf-1]) nStyleChg = 1; } break;
				USR_PAR_STYLE
			}

			if(nStyleChg) {
				//extern BYTE gbMenuStyle;
				//gbMenuStyle = 0;
				UP(Style) = 0;
				bStyleBuf = 0;
				gbStylePreviousOn = 1;
			}
		}
		return;
	}

	if(bStyleBuf == abStyle) return;

	#undef UP_STYLE
	#define UP_STYLE(N, ...)	static UPt(N) bStylePrv##N = -16;
	USR_PAR_STYLE

	if(bStyleBuf == 0) {	// abStyle값이 0에서 다른값으로 변경되는 경우 PREVIOUS값들 업데이트
		#undef UP_STYLE
		#define UP_STYLE(N, ...)	bStylePrv##N = UP(N);
		USR_PAR_STYLE
	}
	bStyleBuf = abStyle;

StyleSet:
	#undef UP_STYLE
	#define UP_STYLE(N, ...)	{\
		if(abStyle==0) { if(UP(N) != bStylePrv##N) { UP(N) = bStylePrv##N; if(abInit==0) UsrParChg(UPi(N)); } }\
		else if((UINT)(abStyle-1) < ARRAY_SIZE(bStyleVals##N)) { if(UP(N) != bStyleVals##N[abStyle-1]) { UP(N) = bStyleVals##N[abStyle-1]; if(abInit==0) UsrParChg(UPi(N)); } }\
	}
	USR_PAR_STYLE
}

void SetByte(BYTE *apAddr, const BYTE abLen, UINT anData)
{
	#define USE_UP_CHG		0

#if USE_UP_CHG==1
	#define goto_SetByteEnd		goto SetByteEnd
	BYTE *const pAddr0 = apAddr;
#else
	#define goto_SetByteEnd		return
#endif

	const BYTE bLen = MIN(abLen,4);

	switch(bLen) {
	case 1 : *apAddr = (BYTE)anData; goto_SetByteEnd;
	case 2 : if(((ULONG)apAddr&1)==0) { *(unsigned short*)apAddr = (unsigned short)anData; goto_SetByteEnd; } break;
	case 4 : if(((ULONG)apAddr&3)==0) { *((unsigned int*)apAddr) = anData; goto_SetByteEnd; } break;
	}

	UINT i;
	for(i=0; i<bLen; i++)
	{
		*apAddr++ = (BYTE)anData;	// Byte Ordering : LSB
		anData >>= 8;
	}

#if USE_UP_CHG==1
SetByteEnd:
	if(gbUsrParTbl < pAddr0 && (pAddr0+bLen) < (gbUsrParTbl+USR_PAR_EA)) UsrParChg(pAddr0-gbUsrParTbl);
#endif
}

UINT GetByte(BYTE *apAddr, const BYTE abLen)
{
	const BYTE bLen = MIN(abLen,4);

	switch(bLen) {
	case 1 : return *apAddr;
	case 2 : if(((ULONG)apAddr&1)==0) { return *((unsigned short*)apAddr); } break;
	case 4 : if(((ULONG)apAddr&3)==0) { return *((unsigned int*)apAddr); } break;
	}

	UINT nData=0, i;
	BYTE *pAddr = (BYTE *)(&nData);
	for(i=0; i<bLen; i++) *pAddr++ = *apAddr++;

	return nData;
}

void UsrParSave(UINT anSaveOn)
{
	int i;

 	if (anSaveOn) {															// Save
 		UsrParValid(1);

		for(i=0; i<USR_PAR_EA; i++) {
			if(gbUsrParTblSaved[i] != gbUsrParTbl[i])
			{
				gbUsrParTblSaved[i] = gbUsrParTbl[i];
				gbUsrParSaveChk = 1;
			}
		}
 	}
	else {																	// No Save & Exit
		UsrParCpy(gbUsrParTbl, gbUsrParTblSaved);	// 복귀
	}

 	UsrParStyle(UP(Style), 1, 0);	// Style의 buffer값 변경 & gbStylePreviousOn 설정
}

void AppSavePar(void)
{	// save parameter to memory
	static BYTE bTwiWrEep2Rdy = 1;	// TwiWrEep2() Ready

	if (bTwiWrEep2Rdy) {

		if (gbUsrParSaveChk) {			// Menu Parameter

#if model_Save == 1
			if(CS_ISPCMD == 0)
#endif
			{
				#if model_Save == 1
					#if 0 // Old
						SfWrite2(gbMnParTbl, MN_PAR_EA, FLASH_MENU_MODE);

					#else
						gptMsgISPTbl.ISPTblState=eIP1_ISP_SAVE_MENU;
//						gbMnParTbl[0]=FLASH_INVALID;
						gptMsgISPTbl.ISPTblAddr =(UINT)&gbMnParTbl;

						CPU_IRQ1 = 1;

					#endif
				#elif model_Save == 2
					TwiWrEep2(EEPROM_ADDR_USER, gbUsrParTbl, USR_PAR_EA);
				#endif

				gbUsrParSaveChk = 0;
				bTwiWrEep2Rdy = 0;

				INIT_STR("MENU PAR Save...");
			}
		}
		else if (gbUsrDataSaveChk) {	// User Data

#if model_Save == 1
			if(CS_ISPCMD == 0)
#endif
			{
				#if model_Save == 1
					#if 0 // Old
						SfWrite2(gbUsrDataTbl, USR_DATA_EA, FLASH_DATA_MODE);

					#else
						gptMsgISPTbl.ISPTblState=eIP1_ISP_SAVE_DATA;
						gptMsgISPTbl.ISPTblAddr =(UINT)&gbUsrDataTbl;
//						INIT_STR_HEX("gnISPTblAddr : ", gnISPTblAddr, 8);

						CPU_IRQ1 = 1;

					#endif
				#elif model_Save == 2
					TwiWrEep2(EEPROM_ADDR_DATA, gbUsrDataTbl, USR_DATA_EA);
				#endif

				gbUsrDataSaveChk = 0;
				bTwiWrEep2Rdy = 0;
			}
		}
	}

#if model_Save == 1
	bTwiWrEep2Rdy = 1;
#elif model_Save == 2
	if(TwiWrEep2(0, 0, 0)) bTwiWrEep2Rdy = 1;	// continuous EEPROM page Write
#endif
}

void AppLoadPar(void)
{	// read parameter from memory
	int i;

	if (gbUsrParReadChk) {			// Menu Parameter

		#if model_Save == 1
			#if 0 // Old
				SfRead2(gbMnParTbl, MN_PAR_EA, FLASH_MENU_MODE);
			#else //New
				BYTE bRes=0;
				while(CS_SFLS);
				bRes = FlsRead((BYTE *)&gbMnParTbl, (SFLS_BASE+FLASH_SECT_MENU_STT*FLASH_SECT_SIZE), MN_PAR_EA);	// 1 : Success, 0 : Fail
				CS_SFLS = 0;
			#endif
		#elif model_Save == 2
		    EepRead(EEPROM_ADDR_USER, gbUsrParTbl, USR_PAR_EA);
		#endif

		if ((WORD)((gbUsrParTbl[UP_START]<<8)|gbUsrParTbl[UP_END])!=(WORD)UP_DATA_ID) {	// Parameter reset condition
			UsrParReset();			// gbUsrParTbl[] 초기화 & UsrParCpy(gbUsrParTblSaved, gbUsrParTbl) 실행
			gbUsrParSaveChk = 1;	// ROM 저장 실행
			INIT_STR("MENU PAR reset");
		}
		else {
			UsrParCpy(gbUsrParTblSaved, gbUsrParTbl);
			UsrParStyle(UP(Style), 1, 0);	// Style의 buffer값 변경 & gbStylePreviousOn 설정
		}

		gbUsrParReadChk = 0;
	}

	if (gbUsrDataReadChk) {			// User Data

		#if model_Save == 1
			#if 0 // Old
				SfRead2(gbUsrDataTbl, USR_DATA_EA, FLASH_DATA_MODE);
			#else //New
				BYTE bRes=0;
				while(CS_SFLS);
				bRes = FlsRead((BYTE *)gbUsrDataTbl, (SFLS_BASE+FLASH_SECT_DATA_STT*FLASH_SECT_SIZE), USR_DATA_EA);	// 1 : Success, 0 : Fail
				CS_SFLS = 0;
			#endif
		#elif model_Save == 2
			EepRead(EEPROM_ADDR_DATA, gbUsrDataTbl, USR_DATA_EA);
		#endif

		if (UD_END_REAL>UD_END)	{ INIT_STR("USER DATA size over"); }			// Size verify
		if ((WORD)((UD(UD_START)<<8)|UD(UD_END))!=(WORD)UD_DATA_ID) {				// reset condition

			UsrDataReset();
			gbUsrDataSaveChk = 1;
			INIT_STR("USER DATA reset");
	    }

		gbUsrDataReadChk = 0;

	}
}

#if 0		// TODO KSH ◆ ParFncTest()
typedef struct {
	BYTE bA;
	WORD wB;
	BYTE bC;
	BYTE bE;
	BYTE bF;
	UINT nD;
} stt;

void ParFncTest(void)
{
	BYTE bArray[12];
	((stt*)bArray)->bA = 0x7;
	((stt*)bArray)->wB = 0x1234;
	((stt*)bArray)->bC = 0x27;
	((stt*)bArray)->bE = 0x28;
	((stt*)bArray)->bF = 0x29;
	((stt*)bArray)->nD = 0x12345678;

	for(int i=0; i<ARRAY_SIZE(bArray); i++)
	{
		printf("[%d] : %x\n", i, bArray[i]);
	}

	printf("size : %d\n", sizeof(((stt*)bArray)->nD) );
	printf("size : %d\n", sizeof(((stt*)bArray)->wB) );

	#define PvcCfg(ADDR,INDEX)	((_PRIVACY*)(ADDR +((INDEX))*sizeof(_PRIVACY)))

	PvcCfg(bArray+1,0)->bAction = 0x11;
	PvcCfg(bArray+1,0)->bPosX = 0x22;
	PvcCfg(bArray+1,0)->bPosY = 0x33;
	PvcCfg(bArray+1,0)->bSizX = 0x44;
	PvcCfg(bArray+1,0)->bSizY = 0x55;

	PvcCfg(bArray+1,1)->bAction = 0x66;
	PvcCfg(bArray+1,1)->bPosX = 0x77;
	PvcCfg(bArray+1,1)->bPosY = 0x88;
	PvcCfg(bArray+1,1)->bSizX = 0x99;
	PvcCfg(bArray+1,1)->bSizY = 0xaa;

	for(int i=0; i<ARRAY_SIZE(bArray); i++)
	{
		printf("[%d] : %x\n", i, bArray[i]);
	}


#if 0
	#define UP_IDX_PRINTF(N)	printf("UPi("#N"):%d\n", UPi(N));
	UP_IDX_PRINTF(Brightness);
	UP_IDX_PRINTF(BrightnessMin);
	UP_IDX_PRINTF(Awb);
	UP_IDX_PRINTF(AwbStyle);
	UP_IDX_PRINTF(ColorBar);
	UP_IDX_PRINTF(Flip);
	UP_IDX_PRINTF(UpPAR00);
	UP_IDX_PRINTF(UpPAR01);
	UP_IDX_PRINTF(UpPAR10);
	UP_IDX_PRINTF(UpPAR1F);

	#define UP_PRINTF(N)	printf("UP("#N"): %d\n", UP(N))
	UP_PRINTF(Brightness);
	UP_PRINTF(BrightnessMin);
	UP_PRINTF(Awb);
	UP_PRINTF(AwbStyle);
	UP_PRINTF(ColorBar);
	UP_PRINTF(Flip);
	UP_PRINTF(UpPAR00);
	UP_PRINTF(UpPAR01);
	UP_PRINTF(UpPAR10);
	UP_PRINTF(UpPAR11);
	UP_PRINTF(UpPAR1F);

	PAR04+=1000000;

	#define PAR_PRINTF(N)	printf("PAR"#N": %d\n", PAR##N)
	PAR_PRINTF(00);
	PAR_PRINTF(01);
	PAR_PRINTF(04);
	PAR_PRINTF(10);
	PAR_PRINTF(11);
	PAR_PRINTF(1F);
#endif

#if 0
	BYTE bTest[8] = {0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0};
	UINT n = 0x12345678;
	WORD w = 0xabcd;
	BYTE b = 0xef;

	SetByte(bTest+4, sizeof(n), n);
	SetByte(bTest  , sizeof(w), w);
	SetByte(bTest+1, sizeof(w), w);
	SetByte(bTest+2, sizeof(w), w);
	SetByte(bTest+7, sizeof(b), b);

	int i;
	for(i=0; i<sizeof(bTest); i++) printf("%x", bTest[i]);
	printf("\n");

	printf(" 0x%x\n", GetByte(bTest+0,4));
	printf(" 0x%x\n", GetByte(bTest+1,4));
	printf(" 0x%x\n", GetByte(bTest+2,4));
	printf(" 0x%x\n", GetByte(bTest+3,4));
	printf(" 0x%x\n", GetByte(bTest+4,4));

	printf(" 0x%x\n", GetByte(bTest+3,2));
	printf(" 0x%x\n", GetByte(bTest+6,2));

	printf(" 0x%x\n", GetByte(bTest+7,1));
#endif
}
#endif



