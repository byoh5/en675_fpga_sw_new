/* **************************************************************************
 File Name  :   isp_par.c
 Description:   EN675 - User Parameter Control
 Designer   :   Kim, Sunghoon
 Date       :   19. 6. 3
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"
#include "isp_menu.h"		// SetFontChg()
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

// User Parameter --------------------------------------------------
BYTE gbUsrParSaveChk=0;
BYTE gbUsrParReadChk=0;
BYTE gbUsrParTbl[USR_PAR_EA];
BYTE gbUsrParTblSaved[USR_PAR_EA];

// User Data -------------------------------------------------------
BYTE gbUsrDataSaveChk=0;
BYTE gbUsrDataReadChk=0;
__attribute__((__aligned__(4))) BYTE gbUsrDataTbl[USR_DATA_EA];


//*************************************************************************************************
// Functions
//-------------------------------------------------------------------------------------------------
void InitDataID(void)
{
	const UINT gsBuildDateLen = strlen(gsBuildDate);
	gDataID  = (*(gsBuildDate + gsBuildDateLen-1) - '0');
	gDataID += (*(gsBuildDate + gsBuildDateLen-2) - '0')<<4;
	gDataID += (*(gsBuildDate + gsBuildDateLen-4) - '0')<<8;
	gDataID += (*(gsBuildDate + gsBuildDateLen-5) - '0')<<12;
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

void UsrParChg(const UINT anStrIdx)
{
	switch(anStrIdx) {
		#undef _UP_
		#define _UP_(S,N,D,...)		case UPi(N): { __VA_ARGS__ /*printf("UsrParChg("#N"):%d\n", anIdx);*/ } break;
		#undef _UPa_
		#define _UPa_	_UP_
		#include "isp_par_tbl.h"
		USR_PAR_LIST
	}
}

void UsrParChgEndIdx(const UINT anEndIdx)
{
	switch(anEndIdx) {
		#undef _UP_
		#define _UP_(S,N,D,...)		case UPinv(N): UsrParChg(UPi(N)); break;
		#undef _UPa_
		#define _UPa_	_UP_
		#include "isp_par_tbl.h"
		USR_PAR_LIST
	}
}

#if 0
BYTE UsrParSiz(const UINT anIdx)
{
	switch(anIdx) {
		#undef _UP_
		#define _UP_(S,N,D,...)		case UPi(N): return S;
		#undef _UPa_
		#define _UPa_	_UP_
		#include "isp_par_tbl.h"
		USR_PAR_LIST
	}
}
#endif

void UsrParChgAll(void)
{
	#undef _UP_
	#define _UP_(S,N,D,...)		UsrParChg(UPi(N));
	#undef _UPa_
	#define _UPa_	_UP_
	#include "isp_par_tbl.h"
	USR_PAR_LIST
}

void UsrParReset(void)
{	// User Parameter Reset
	UsrParValid(1);

	#undef _UP_
	#define _UP_(S,N,D,...)		UPw(N,D);
	#undef _UPa_
	#define _UPa_	_UP_
	#include "isp_par_tbl.h"
	USR_PAR_LIST

	UsrParStyle(INIT_STYLE);	// UsrParChgAll() 실행
}

void UsrParStyle(const BYTE abStyle)
{
	#define _UPSTYLE_1(N, ...) {\
		const BYTE bStyle##N[] = { __VA_ARGS__ };\
		if(abStyle==0) gbUsrParTbl[UPi(N)] = gbUsrParTblSaved[UPi(N)];\
		else if((UINT)(abStyle-1) < ARRAY_SIZE(bStyle##N)) gbUsrParTbl[UPi(N)] = bStyle##N[abStyle-1]; }

	#define _UPSTYLE_2(N, ...) {\
		const WORD wStyle##N[] = { __VA_ARGS__ };\
		if(abStyle==0) {\
			gbUsrParTbl[UPi(N)]   = gbUsrParTblSaved[UPi(N)];\
			gbUsrParTbl[UPi(N)+1] = gbUsrParTblSaved[UPi(N)+1];\
		}\
		else if((UINT)(abStyle-1) < ARRAY_SIZE(wStyle##N)) UPw(N, wStyle##N[abStyle-1]); }

	#define _UPSTYLE_3(N, ...) {\
		const UINT nStyle##N[] = { __VA_ARGS__ };\
		if(abStyle==0) {\
			gbUsrParTbl[UPi(N)]   = gbUsrParTblSaved[UPi(N)];\
			gbUsrParTbl[UPi(N)+1] = gbUsrParTblSaved[UPi(N)+1];\
			gbUsrParTbl[UPi(N)+2] = gbUsrParTblSaved[UPi(N)+2];\
		}\
		else if((UINT)(abStyle-1) < ARRAY_SIZE(nStyle##N)) UPw(N, nStyle##N[abStyle-1]); }

	#define _UPSTYLE_4(N, ...) {\
		const UINT nStyle##N[] = { __VA_ARGS__ };\
		if(abStyle==0) {\
			gbUsrParTbl[UPi(N)]   = gbUsrParTblSaved[UPi(N)];\
			gbUsrParTbl[UPi(N)+1] = gbUsrParTblSaved[UPi(N)+1];\
			gbUsrParTbl[UPi(N)+2] = gbUsrParTblSaved[UPi(N)+2];\
			gbUsrParTbl[UPi(N)+3] = gbUsrParTblSaved[UPi(N)+3];\
		}\
		else if((UINT)(abStyle-1) < ARRAY_SIZE(nStyle##N)) UPw(N, nStyle##N[abStyle-1]); }

	#define _UPSTYLE_(S,N, ...)		_UPSTYLE_##S(N, __VA_ARGS__)

	USR_PAR_STYLE

	UsrParChgAll();
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
	if(gbUsrParTbl <= pAddr0 && (pAddr0+bLen) <= (gbUsrParTbl+USR_PAR_EA)) UsrParChg(pAddr0-gbUsrParTbl);
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
		for(i=0; i<USR_PAR_EA; i++) gbUsrParTbl[i] = gbUsrParTblSaved[i];	// 복귀
		UsrParChgAll();
	}
}

void AppSavePar(void)
{	// save parameter to memory
	static BYTE bTwiWrEep2Rdy = 1;	// TwiWrEep2() Ready

	if (bTwiWrEep2Rdy) {

		if (gbUsrParSaveChk) {			// Menu Parameter

#if model_Save
			if(CS_ISPCMD == 0)
#endif
			{
				#if model_Save
					#if 0 // Old
						SfWrite2(gbMnParTbl, MN_PAR_EA, FLASH_MENU_MODE);

					#else
						gptMsgISPTbl.ISPTblState=eIP1_ISP_SAVE_MENU;
//						gbMnParTbl[0]=FLASH_INVALID;
						gptMsgISPTbl.ISPTblAddr =(UINT)&gbMnParTbl;

						CPU_IRQ1 = 1;

					#endif
				#else
					TwiWrEep2(EEPROM_ADDR_USER, gbUsrParTbl, USR_PAR_EA);
				#endif

				gbUsrParSaveChk = 0;
				bTwiWrEep2Rdy = 0;

				UartTxStr("MENU PAR Save...");
			}
		}
		else if (gbUsrDataSaveChk) {	// User Data

#if model_Save
			if(CS_ISPCMD == 0)
#endif
			{
				#if model_Save
					#if 0 // Old
						SfWrite2(gbUsrDataTbl, USR_DATA_EA, FLASH_DATA_MODE);

					#else
						gptMsgISPTbl.ISPTblState=eIP1_ISP_SAVE_DATA;
						gptMsgISPTbl.ISPTblAddr =(UINT)&gbUsrDataTbl;
//						UartTxStrHexNoIRQ("gnISPTblAddr : ", gnISPTblAddr, 8);

						CPU_IRQ1 = 1;

					#endif
				#else
					TwiWrEep2(EEPROM_ADDR_DATA, gbUsrDataTbl, USR_DATA_EA);
				#endif

				gbUsrDataSaveChk = 0;
				bTwiWrEep2Rdy = 0;
			}
		}
	}

#if model_Save
	bTwiWrEep2Rdy = 1;
#else
	if(TwiWrEep2(0, 0, 0)) bTwiWrEep2Rdy = 1;	// continuous EEPROM page Write
#endif
}

void AppLoadPar(void)
{	// read parameter from memory
	int i;

	if (gbUsrParReadChk) {			// Menu Parameter

		#if model_Save
			#if 0 // Old
				SfRead2(gbMnParTbl, MN_PAR_EA, FLASH_MENU_MODE);
			#else //New
				BYTE bRes=0;
				while(CS_SFLS);
				bRes = FlsRead((BYTE *)&gbMnParTbl, (SFLS_BASE+FLASH_SECT_MENU_STT*FLASH_SECT_SIZE), MN_PAR_EA);	// 1 : Success, 0 : Fail
				CS_SFLS = 0;
			#endif
		#else
		    EepRead(EEPROM_ADDR_USER, gbUsrParTbl, USR_PAR_EA);
		#endif

		if ((WORD)((gbUsrParTbl[UP_START]<<8)|gbUsrParTbl[UP_END])!=(WORD)UP_DATA_ID) {	// Parameter reset condition
			UsrParReset();			// gbUsrParTbl[] 초기화 & UsrParChgAll() 실행
			gbUsrParSaveChk = 1;
			UartTxStrNoIRQ("MENU PAR reset ");
		}
		else {
			UsrParChgAll();
		}

		for(i=0; i<USR_PAR_EA; i++) gbUsrParTblSaved[i] = gbUsrParTbl[i];

		gbUsrParReadChk = 0;
	}

	if (gbUsrDataReadChk) {			// User Data

		#if model_Save
			#if 0 // Old
				SfRead2(gbUsrDataTbl, USR_DATA_EA, FLASH_DATA_MODE);
			#else //New
				BYTE bRes=0;
				while(CS_SFLS);
				bRes = FlsRead((BYTE *)gbUsrDataTbl, (SFLS_BASE+FLASH_SECT_DATA_STT*FLASH_SECT_SIZE), USR_DATA_EA);	// 1 : Success, 0 : Fail
				CS_SFLS = 0;
			#endif
		#else
			EepRead(EEPROM_ADDR_DATA, gbUsrDataTbl, USR_DATA_EA);
		#endif

		if (UD_END_REAL>UD_END)	UartTxStrNoIRQ("USER DATA size over ");			// Size verify
		if ((WORD)((UD(UD_START)<<8)|UD(UD_END))!=(WORD)UD_DATA_ID) {				// reset condition

			UsrDataReset();
			gbUsrDataSaveChk = 1;
			UartTxStrNoIRQ("USER DATA reset ");
	    }

		gbUsrDataReadChk = 0;

	}
}

#if 1
void ParFncTest(void)
{
	#define UP_IDX_PRINTF(N)	printf("UPi("#N"):%d\n", UPi(N));
	UP_IDX_PRINTF(UpColorBar);
	UP_IDX_PRINTF(UpFlip);
	UP_IDX_PRINTF(UpPAR00);
	UP_IDX_PRINTF(UpPAR01);
	UP_IDX_PRINTF(UpPAR10);
	UP_IDX_PRINTF(UpPAR1F);

	#define UP_PRINTF(N)	printf("UPr("#N"): %d\n", UPr(N))
	UP_PRINTF(UpColorBar);
	UP_PRINTF(UpFlip);
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



