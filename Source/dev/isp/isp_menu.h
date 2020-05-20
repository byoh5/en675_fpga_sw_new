/* **************************************************************************
 File Name	: 	app_menu_new.h
 Description:	New Menu
 Designer	:	Kim Sung-hoon (shkim@eyenix.com)
 Date		:	12.03.30
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

//*************************************************************************************************
// Define
//-------------------------------------------------------------------------------------------------
#define MN_LNG_EA			5								// Language max ea (1 ~ 12) (user fix)


#define	MN_TITLE			1								// title y
#define	MN_YSTART			(MN_TITLE+2)					//


#define	MN_MXSP_DEF			18/*14*//*0x1*/	// 160330 KSH		// main osd x start position
#define MN_MXSP_DBG			(gnFontXw-MN_LINESIZE)//22

extern BYTE MN_MXSP;

#define	MN_SXSP				(MN_MXSP+12)					// sub osd x start position
//#define MN_XBAR				(MN_SXSP+3)						// sub Bar osd start position
#define MN_BAR_SIZE			7								// Bar Size, 7 or 5

#define MN_XCENT        	(MN_MXSP+11)					// Osd center position
#define MN_XTITL			(MN_MXSP+8/*11*/)

#define MN_SUBSIZE      	11/*10*/	// 160330 KSH		// Sub osd ea
#define MN_LINESIZE     	(MN_SXSP+MN_SUBSIZE-MN_MXSP)	// X ea


#define MENU_PK_SETUP		{KEY_VAL_L, KEY_VAL_L, KEY_VAL_R, KEY_VAL_R, KEY_VAL_C}		// Setup Menu pass Key sequence
#define MENU_PK_DEBUG 		{KEY_VAL_U, KEY_VAL_D, KEY_VAL_L, KEY_VAL_R, KEY_VAL_C}     // Debug Menu pass Key sequence

#define	MN_AUTO_OFF			0						// Auto menu off (0:Disable)
#define MN_OFF_TIME			30						// off after (sec)


#define FONT_SEL0											gnFontChar0
#define FONT_SEL1	gbUsrParTbl[UPi(LanguageSel)] == 1  ? gnFontChar1 : FONT_SEL0
#define FONT_SEL2	gbUsrParTbl[UPi(LanguageSel)] == 2  ? gnFontChar2 : FONT_SEL1
#define FONT_SEL3	gbUsrParTbl[UPi(LanguageSel)] == 3  ? gnFontChar3 : FONT_SEL2
#define FONT_SEL4	gbUsrParTbl[UPi(LanguageSel)] == 4  ? gnFontChar4 : FONT_SEL3
#define FONT_SEL5	gbUsrParTbl[UPi(LanguageSel)] == 5  ? gnFontChar5 : FONT_SEL4
#define FONT_SEL6	gbUsrParTbl[UPi(LanguageSel)] == 6  ? gnFontChar6 : FONT_SEL5
#define FONT_SEL7	gbUsrParTbl[UPi(LanguageSel)] == 7  ? gnFontChar7 : FONT_SEL6
#define FONT_SEL8	gbUsrParTbl[UPi(LanguageSel)] == 8  ? gnFontChar8 : FONT_SEL7
#define FONT_SEL9	gbUsrParTbl[UPi(LanguageSel)] == 9  ? gnFontChar9 : FONT_SEL8
#define FONT_SELA	gbUsrParTbl[UPi(LanguageSel)] == 10 ? gnFontCharA : FONT_SEL9
#define FONT_SELB	gbUsrParTbl[UPi(LanguageSel)] == 11 ? gnFontCharB : FONT_SELA

#define FONT_SEL0_S											ARRAY_SIZE(gnFontChar0)
#define FONT_SEL1_S	gbUsrParTbl[UPi(LanguageSel)] == 1  ? ARRAY_SIZE(gnFontChar1) : FONT_SEL0_S
#define FONT_SEL2_S	gbUsrParTbl[UPi(LanguageSel)] == 2  ? ARRAY_SIZE(gnFontChar2) : FONT_SEL1_S
#define FONT_SEL3_S	gbUsrParTbl[UPi(LanguageSel)] == 3  ? ARRAY_SIZE(gnFontChar3) : FONT_SEL2_S
#define FONT_SEL4_S	gbUsrParTbl[UPi(LanguageSel)] == 4  ? ARRAY_SIZE(gnFontChar4) : FONT_SEL3_S
#define FONT_SEL5_S	gbUsrParTbl[UPi(LanguageSel)] == 5  ? ARRAY_SIZE(gnFontChar5) : FONT_SEL4_S
#define FONT_SEL6_S	gbUsrParTbl[UPi(LanguageSel)] == 6  ? ARRAY_SIZE(gnFontChar6) : FONT_SEL5_S
#define FONT_SEL7_S	gbUsrParTbl[UPi(LanguageSel)] == 7  ? ARRAY_SIZE(gnFontChar7) : FONT_SEL6_S
#define FONT_SEL8_S	gbUsrParTbl[UPi(LanguageSel)] == 8  ? ARRAY_SIZE(gnFontChar8) : FONT_SEL7_S
#define FONT_SEL9_S	gbUsrParTbl[UPi(LanguageSel)] == 9  ? ARRAY_SIZE(gnFontChar9) : FONT_SEL8_S
#define FONT_SELA_S	gbUsrParTbl[UPi(LanguageSel)] == 10 ? ARRAY_SIZE(gnFontCharA) : FONT_SEL9_S
#define FONT_SELB_S	gbUsrParTbl[UPi(LanguageSel)] == 11 ? ARRAY_SIZE(gnFontCharB) : FONT_SELA_S

#define EXTERN_FONT_CHAR0						extern const UINT gnFontChar0[][12]
#define EXTERN_FONT_CHAR1	EXTERN_FONT_CHAR0;	extern const UINT gnFontChar1[][12]
#define EXTERN_FONT_CHAR2	EXTERN_FONT_CHAR1;	extern const UINT gnFontChar2[][12]
#define EXTERN_FONT_CHAR3	EXTERN_FONT_CHAR2;	extern const UINT gnFontChar3[][12]
#define EXTERN_FONT_CHAR4	EXTERN_FONT_CHAR3;	extern const UINT gnFontChar4[][12]
#define EXTERN_FONT_CHAR5	EXTERN_FONT_CHAR4;	extern const UINT gnFontChar5[][12]
#define EXTERN_FONT_CHAR6	EXTERN_FONT_CHAR5;	extern const UINT gnFontChar6[][12]
#define EXTERN_FONT_CHAR7	EXTERN_FONT_CHAR6;	extern const UINT gnFontChar7[][12]
#define EXTERN_FONT_CHAR8	EXTERN_FONT_CHAR7;	extern const UINT gnFontChar8[][12]
#define EXTERN_FONT_CHAR9	EXTERN_FONT_CHAR8;	extern const UINT gnFontChar9[][12]
#define EXTERN_FONT_CHARA	EXTERN_FONT_CHAR9;	extern const UINT gnFontCharA[][12]
#define EXTERN_FONT_CHARB	EXTERN_FONT_CHARA;	extern const UINT gnFontCharB[][12]

#if   MN_LNG_EA == 1
	#define SetFontChg() FontCharInit(FONT_SEL0, FONT_SEL0_S)
	EXTERN_FONT_CHAR0;
#elif MN_LNG_EA == 2
	#define SetFontChg() FontCharInit(FONT_SEL1, FONT_SEL1_S)
	EXTERN_FONT_CHAR1;
#elif MN_LNG_EA == 3
	#define SetFontChg() FontCharInit(FONT_SEL2, FONT_SEL2_S)
	EXTERN_FONT_CHAR2;
#elif MN_LNG_EA == 4
	#define SetFontChg() FontCharInit(FONT_SEL3, FONT_SEL3_S)
	EXTERN_FONT_CHAR3;
#elif MN_LNG_EA == 5
	#define SetFontChg() FontCharInit(FONT_SEL4, FONT_SEL4_S)
	EXTERN_FONT_CHAR4;
#elif MN_LNG_EA == 6
	#define SetFontChg() FontCharInit(FONT_SEL5, FONT_SEL5_S)
	EXTERN_FONT_CHAR5;
#elif MN_LNG_EA == 7
	#define SetFontChg() FontCharInit(FONT_SEL6, FONT_SEL6_S)
	EXTERN_FONT_CHAR6;
#elif MN_LNG_EA == 8
	#define SetFontChg() FontCharInit(FONT_SEL7, FONT_SEL7_S)
	EXTERN_FONT_CHAR7;
#elif MN_LNG_EA == 9
	#define SetFontChg() FontCharInit(FONT_SEL8, FONT_SEL8_S)
	EXTERN_FONT_CHAR8;
#elif MN_LNG_EA == 10
	#define SetFontChg() FontCharInit(FONT_SEL9, FONT_SEL9_S)
	EXTERN_FONT_CHAR9;
#elif MN_LNG_EA == 11
	#define SetFontChg() FontCharInit(FONT_SELA, FONT_SELA_S)
	EXTERN_FONT_CHARA;
#elif MN_LNG_EA == 12
	#define SetFontChg() FontCharInit(FONT_SELB, FONT_SELB_S)
	EXTERN_FONT_CHARB;
#endif


//----------------------------------------------------------------------------------------
#define _S(NAME)		s##NAME

#if MN_LNG_EA == 1
  #if model_CharMax == 1
	#define PEXCH		ULONG//UINT//EXCH*
  #else
	#define PEXCH		EXCH
  #endif
#else
	#define PEXCH		ULONG//UINT//EXCH*
#endif

#define MENU_TITLE			((const PEXCH*)gbMenuTitle)
#define MENU_NAME			((const PEXCH*)gbMenuList)
#define MENU_VAL			((const PEXCH*)gbMenuVal)

#define MENU_TITLE_IS(V)	(MENU_TITLE == _S(V))
#define MENU_NAME_IS(V)		(MENU_NAME == _S(V))
#define MENU_VAL_IS(V)		(MENU_VAL == _S(V))

extern PEXCH* gbMenuTitle;
extern PEXCH* gbMenuList;
extern PEXCH* gbMenuVal;

#define EXTERN_STR(STR)		extern const PEXCH _S(STR)[]

EXTERN_STR(NOT_USED);
EXTERN_STR(OFF);
EXTERN_STR(MENU);

extern BYTE gbMnDebugFnc;
extern BYTE gbMnDebugBypass;
extern BYTE gbMpAwbPrst;
extern BYTE gbBlcOsdOn;



//----------------------------------------------------------------------------------------
#define MN_FONT_MAX_PATH	10						// number of String for Debugging (fix)
#define MN_FONT_SPACE		1						// number of Space for Debugging (fix)

// OSD On/Off
#define OSD_ON()		FONT_ON0w(1)
#define OSD_OFF()


#define DispStr(S,Y,X,L)			FontStr(Y,X,S,L)	// TODO KSH> Font Fnc 나중에 제거
#define DispClr(Y,X,L)				FontClr(Y,X,L)
#define DispClrStr(S,Y,X,SL,CL)		FontClrStr(Y,X,S,SL,CL)
#define DispClrDec(V,Y,X,L)			FontDec(Y,X,V,L,1)
#define SetFontAttrs(Y,X,L,A)		FontEx(Y,X,NO_ALPHA,A,L)
#define DispStrEx(S,Y,X,L)			FontStrW(Y,X,S,L)
#define DispClrStrEx(S,Y,X,SL,CL)	FontClrStrW(Y,X,S,SL,CL)

//----------------------------------------------------------------------------------------
// String
//#define _S(NAME)		s##NAME

#define STR_LIST1(NAME)						(PEXCH)_S(1##NAME)
#define STR_LIST2(NAME)		STR_LIST1(NAME),(PEXCH)_S(2##NAME)
#define STR_LIST3(NAME)		STR_LIST2(NAME),(PEXCH)_S(3##NAME)
#define STR_LIST4(NAME)		STR_LIST3(NAME),(PEXCH)_S(4##NAME)
#define STR_LIST5(NAME)		STR_LIST4(NAME),(PEXCH)_S(5##NAME)
#define STR_LIST6(NAME)		STR_LIST5(NAME),(PEXCH)_S(6##NAME)
#define STR_LIST7(NAME)		STR_LIST6(NAME),(PEXCH)_S(7##NAME)
#define STR_LIST8(NAME)		STR_LIST7(NAME),(PEXCH)_S(8##NAME)
#define STR_LIST9(NAME)		STR_LIST8(NAME),(PEXCH)_S(9##NAME)
#define STR_LISTA(NAME)		STR_LIST9(NAME),(PEXCH)_S(A##NAME)
#define STR_LISTB(NAME)		STR_LISTA(NAME),(PEXCH)_S(B##NAME)
#define STR_LISTC(NAME)		STR_LISTB(NAME),(PEXCH)_S(C##NAME)

#if MN_LNG_EA == 1
  #if model_CharMax == 1
	#define STRLEN(L,STR)		{ if(*((BYTE*)STR[1]) == 2) L = strlenW((EXCH*)STR[0]); else L = strlen((char*)STR[0]); }
	#define DISPSTR(STR,...)	{ if(*((BYTE*)STR[1]) == 2) DispStrEx((EXCH*)STR[0],__VA_ARGS__); else DispStr((char*)STR[0],__VA_ARGS__); }
	#define DISPCLRSTR(STR,...)	{ if(*((BYTE*)STR[1]) == 2) DispClrStrEx((EXCH*)STR[0],__VA_ARGS__); else DispClrStr((char*)STR[0],__VA_ARGS__); }
	//#define PEXCH				UINT//EXCH*
	#define STRING1(NAME,C1,STR1)		const C1 _S(1##NAME)[] = STR1; const BYTE _S(0##NAME) = sizeof(C1); const PEXCH _S(NAME)[] = { (PEXCH)_S(1##NAME), (PEXCH)&_S(0##NAME) }
  #else
	#define STRLEN(L,STR)		L = strlen((const char*)STR)
	#define DISPSTR(STR,...)	DispStr((const char*)STR,__VA_ARGS__)
	#define DISPCLRSTR(STR,...)	DispClrStr((const char*)STR,__VA_ARGS__)
	//#define PEXCH				EXCH
	#define STRING1(NAME,C1,STR1)		const PEXCH _S(NAME)[] = STR1;
  #endif
	#define STRING2(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING3(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING4(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING5(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING6(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING7(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING8(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING9(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRINGA(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRINGB(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRINGC(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
#else
	//#define PEXCH		UINT//EXCH*

  #if model_CharMax == 1
	#define LNG_INDEX(STR)		(gbUsrParTbl[UPi(LanguageSel)]>=((BYTE*)STR[0])[0] ? 1 : gbUsrParTbl[UPi(LanguageSel)]+1)
	#define LS(ARRAY)			ARRAY[LNG_INDEX(ARRAY)]
	#define STRLEN(L,STR)		{ if( ((BYTE*)STR[0])[LNG_INDEX(STR)] == 2) L = strlenW((EXCH*)LS(STR)); else L = strlen((char*)LS(STR)); }
	#define DISPSTR(STR,...)	{ if( ((BYTE*)STR[0])[LNG_INDEX(STR)] == 2) DispStrEx((EXCH*)LS(STR),__VA_ARGS__); else DispStr((char*)LS(STR),__VA_ARGS__); }
	#define DISPCLRSTR(STR,...)	{ if( ((BYTE*)STR[0])[LNG_INDEX(STR)] == 2) DispClrStrEx((EXCH*)LS(STR),__VA_ARGS__); else DispClrStr((char*)LS(STR),__VA_ARGS__); }
	#define STR_LIST(NUM,NAME)	const PEXCH	_S(NAME)[] = { (PEXCH)_S(0##NAME), STR_LIST##NUM(NAME) }

	#define SIZE_LIST1(C1)			sizeof(C1)
	#define SIZE_LIST2(C1,...)		sizeof(C1),SIZE_LIST1(__VA_ARGS__)
	#define SIZE_LIST3(C1,...)		sizeof(C1),SIZE_LIST2(__VA_ARGS__)
	#define SIZE_LIST4(C1,...)		sizeof(C1),SIZE_LIST3(__VA_ARGS__)
	#define SIZE_LIST5(C1,...)		sizeof(C1),SIZE_LIST4(__VA_ARGS__)
	#define SIZE_LIST6(C1,...)		sizeof(C1),SIZE_LIST5(__VA_ARGS__)
	#define SIZE_LIST7(C1,...)		sizeof(C1),SIZE_LIST6(__VA_ARGS__)
	#define SIZE_LIST8(C1,...)		sizeof(C1),SIZE_LIST7(__VA_ARGS__)
	#define SIZE_LIST9(C1,...)		sizeof(C1),SIZE_LIST8(__VA_ARGS__)
	#define SIZE_LISTA(C1,...)		sizeof(C1),SIZE_LIST9(__VA_ARGS__)
	#define SIZE_LISTB(C1,...)		sizeof(C1),SIZE_LISTA(__VA_ARGS__)
	#define SIZE_LISTC(C1,...)		sizeof(C1),SIZE_LISTB(__VA_ARGS__)
	#define SIZE_LIST(NUM,NAME,...)	const BYTE	_S(0##NAME)[] = { NUM, SIZE_LIST##NUM(__VA_ARGS__) };

	// STR1,STR2,...의 선언은 _P(...)사용으로 인해 MACRO불가능
	#define STRING1(NAME,C1,STR1)\
			SIZE_LIST(1,NAME,C1)\
			const C1	_S(1##NAME)[] = STR1;\
			STR_LIST(1,NAME)

	#define STRING2(NAME,C1,STR1,C2,STR2)\
			SIZE_LIST(2,NAME,C1,C2)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			STR_LIST(2,NAME)

	#define STRING3(NAME,C1,STR1,C2,STR2,C3,STR3)\
			SIZE_LIST(3,NAME,C1,C2,C3)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			STR_LIST(3,NAME)

	#define STRING4(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4)\
			SIZE_LIST(4,NAME,C1,C2,C3,C4)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			STR_LIST(4,NAME)

	#define STRING5(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5)\
			SIZE_LIST(5,NAME,C1,C2,C3,C4,C5)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			STR_LIST(5,NAME)

	#define STRING6(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6)\
			SIZE_LIST(6,NAME,C1,C2,C3,C4,C5,C6)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			STR_LIST(6,NAME)

	#define STRING7(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7)\
			SIZE_LIST(7,NAME,C1,C2,C3,C4,C5,C6,C7)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			STR_LIST(7,NAME)

	#define STRING8(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8)\
			SIZE_LIST(8,NAME,C1,C2,C3,C4,C5,C6,C7,C8)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			STR_LIST(8,NAME)
	
	#define STRING9(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9)\
			SIZE_LIST(9,NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			STR_LIST(9,NAME)
	
	#define STRINGA(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA)\
			SIZE_LIST(A,NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9,CA)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			STR_LIST(A,NAME)
	
	#define STRINGB(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA,CB,STRB)\
			SIZE_LIST(B,NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9,CA,CB)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			const CB	_S(B##NAME)[] = STRB;\
			STR_LIST(B,NAME)
	
	#define STRINGC(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA,CB,STRB,CC,STRC)\
			SIZE_LIST(C,NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9,CA,CB,CC)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			const CB	_S(B##NAME)[] = STRB;\
			const CC	_S(C##NAME)[] = STRC;\
			STR_LIST(C,NAME)
	
  #else
	#define LNG_INDEX(STR)		(gbUsrParTbl[UPi(LanguageSel)]>=STR[0] ? 1 : gbUsrParTbl[UPi(LanguageSel)]+1)
	#define STRLEN(L,STR)		L = strlen((EXCH*)STR[LNG_INDEX(STR)])
	#define DISPSTR(STR,...)	DispStr((EXCH*)STR[LNG_INDEX(STR)],__VA_ARGS__)
	#define DISPCLRSTR(STR,...)	DispClrStr((EXCH*)STR[LNG_INDEX(STR)],__VA_ARGS__)
	#define STR_LIST(NUM,NAME)	const PEXCH	_S(NAME)[] = { NUM, STR_LIST##NUM(NAME) }

	// STR1,STR2,...의 선언은 _P(...)사용으로 인해 MACRO불가능
	#define STRING1(NAME,C1,STR1)\
			const C1	_S(1##NAME)[] = STR1;\
			STR_LIST(1, NAME)

	#define STRING2(NAME,C1,STR1,C2,STR2)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			STR_LIST(2, NAME)

	#define STRING3(NAME,C1,STR1,C2,STR2,C3,STR3)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			STR_LIST(3, NAME)

	#define STRING4(NAME,C1,STR1,C2,STR2,C3,STR3, C4,STR4)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			STR_LIST(4, NAME)

	#define STRING5(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			STR_LIST(5, NAME)

	#define STRING6(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			STR_LIST(6, NAME)

	#define STRING7(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			STR_LIST(7, NAME)
	
	#define STRING8(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			STR_LIST(8, NAME)
	
	#define STRING9(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			STR_LIST(9, NAME)
	
	#define STRINGA(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			STR_LIST(A, NAME)
	
	#define STRINGB(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA,CB,STRB)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			const CB	_S(B##NAME)[] = STRB;\
			STR_LIST(B, NAME)
	
	#define STRINGC(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA,CB,STRB,CC,STRC)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			const CB	_S(B##NAME)[] = STRB;\
			const CC	_S(C##NAME)[] = STRC;\
			STR_LIST(C, NAME)
  #endif
#endif

#define DrawStr(STR,...)	{ DISPSTR(_S(STR),__VA_ARGS__); }

#define DrawBar(VAL,MIN,MAX) {\
	DispClrDec(VAL, DRAW_Y, MN_SXSP, 3);\
	OsdDispBar(VAL, DRAW_Y, MN_SXSP+3, MIN, MAX);}


#define OsdMenuPos(V)		OsdMenuPos##V()

// Clear : Index finger & color on line
#define OsdMenuPos0()	if(giMenuNum) {\
						OsdAttrsStgPosAr(giStgPosLine[giStgPos[giLV]], MN_WHITE);\
						/*SetFont(gbMenuY+giStgPos[giLV], MXSP-2, 0);*/	\
						}

// Display : Index finger & color on line
#define OsdMenuPos1()	if(giMenuNum) {\
						OsdAttrsStgPosAr(giStgPosLine[giStgPos[giLV]], MN_YELLOW);\
						/*SetFont(gbMenuY+giStgPos[giLV], MXSP-2, 0)*/	\
						}

//----------------------------------------------------------------------------------------
// Menu
#define S_LIST1(O1)				_S(O1)
#define S_LIST2(O1, ...)		_S(O1),S_LIST1(__VA_ARGS__)
#define S_LIST3(O1, ...)		_S(O1),S_LIST2(__VA_ARGS__)
#define S_LIST4(O1, ...)		_S(O1),S_LIST3(__VA_ARGS__)
#define S_LIST5(O1, ...)		_S(O1),S_LIST4(__VA_ARGS__)
#define S_LIST6(O1, ...)		_S(O1),S_LIST5(__VA_ARGS__)
#define S_LIST7(O1, ...)		_S(O1),S_LIST6(__VA_ARGS__)
#define S_LIST8(O1, ...)		_S(O1),S_LIST7(__VA_ARGS__)
#define S_LIST9(O1, ...)		_S(O1),S_LIST8(__VA_ARGS__)
#define S_LIST10(O1, ...)		_S(O1),S_LIST9(__VA_ARGS__)
#define S_LIST11(O1, ...)		_S(O1),S_LIST10(__VA_ARGS__)
#define S_LIST12(O1, ...)		_S(O1),S_LIST11(__VA_ARGS__)
#define S_LIST13(O1, ...)		_S(O1),S_LIST12(__VA_ARGS__)
#define S_LIST14(O1, ...)		_S(O1),S_LIST13(__VA_ARGS__)
#define S_LIST15(O1, ...)		_S(O1),S_LIST14(__VA_ARGS__)
#define S_LIST16(O1, ...)		_S(O1),S_LIST15(__VA_ARGS__)

#define S_LISTm1(M1,C1)			_S(M1)
#define S_LISTm2(M1,C1, ...)	_S(M1),S_LISTm1(__VA_ARGS__)
#define S_LISTm3(M1,C1, ...)	_S(M1),S_LISTm2(__VA_ARGS__)
#define S_LISTm4(M1,C1, ...)	_S(M1),S_LISTm3(__VA_ARGS__)
#define S_LISTm5(M1,C1, ...)	_S(M1),S_LISTm4(__VA_ARGS__)
#define S_LISTm6(M1,C1, ...)	_S(M1),S_LISTm5(__VA_ARGS__)
#define S_LISTm7(M1,C1, ...)	_S(M1),S_LISTm6(__VA_ARGS__)
#define S_LISTm8(M1,C1, ...)	_S(M1),S_LISTm7(__VA_ARGS__)
#define S_LISTm9(M1,C1, ...)	_S(M1),S_LISTm8(__VA_ARGS__)
#define S_LISTm10(M1,C1, ...)	_S(M1),S_LISTm9(__VA_ARGS__)
#define S_LISTm11(M1,C1, ...)	_S(M1),S_LISTm10(__VA_ARGS__)
#define S_LISTm12(M1,C1, ...)	_S(M1),S_LISTm11(__VA_ARGS__)
#define S_LISTm13(M1,C1, ...)	_S(M1),S_LISTm12(__VA_ARGS__)
#define S_LISTm14(M1,C1, ...)	_S(M1),S_LISTm13(__VA_ARGS__)
#define S_LISTm15(M1,C1, ...)	_S(M1),S_LISTm14(__VA_ARGS__)
#define S_LISTm16(M1,C1, ...)	_S(M1),S_LISTm15(__VA_ARGS__)

#define M_CODE1(NUM,M1,C1)			case NUM-1 : C1; M_CODE_OUT;
#define M_CODE2(NUM,M1,C1, ...)		case NUM-2 : C1; M_CODE_OUT;	M_CODE1(NUM,__VA_ARGS__)
#define M_CODE3(NUM,M1,C1, ...)		case NUM-3 : C1; M_CODE_OUT;	M_CODE2(NUM,__VA_ARGS__)
#define M_CODE4(NUM,M1,C1, ...)		case NUM-4 : C1; M_CODE_OUT;	M_CODE3(NUM,__VA_ARGS__)
#define M_CODE5(NUM,M1,C1, ...)		case NUM-5 : C1; M_CODE_OUT;	M_CODE4(NUM,__VA_ARGS__)
#define M_CODE6(NUM,M1,C1, ...)		case NUM-6 : C1; M_CODE_OUT;	M_CODE5(NUM,__VA_ARGS__)
#define M_CODE7(NUM,M1,C1, ...)		case NUM-7 : C1; M_CODE_OUT;	M_CODE6(NUM,__VA_ARGS__)
#define M_CODE8(NUM,M1,C1, ...)		case NUM-8 : C1; M_CODE_OUT;	M_CODE7(NUM,__VA_ARGS__)
#define M_CODE9(NUM,M1,C1, ...)		case NUM-9 : C1; M_CODE_OUT;	M_CODE8(NUM,__VA_ARGS__)
#define M_CODE10(NUM,M1,C1, ...)	case NUM-10 : C1; M_CODE_OUT;	M_CODE9(NUM,__VA_ARGS__)
#define M_CODE11(NUM,M1,C1, ...)	case NUM-11 : C1; M_CODE_OUT;	M_CODE10(NUM,__VA_ARGS__)
#define M_CODE12(NUM,M1,C1, ...)	case NUM-12 : C1; M_CODE_OUT;	M_CODE11(NUM,__VA_ARGS__)
#define M_CODE13(NUM,M1,C1, ...)	case NUM-13 : C1; M_CODE_OUT;	M_CODE12(NUM,__VA_ARGS__)
#define M_CODE14(NUM,M1,C1, ...)	case NUM-14 : C1; M_CODE_OUT;	M_CODE13(NUM,__VA_ARGS__)
#define M_CODE15(NUM,M1,C1, ...)	case NUM-15 : C1; M_CODE_OUT;	M_CODE14(NUM,__VA_ARGS__)
#define M_CODE16(NUM,M1,C1, ...)	case NUM-16 : C1; M_CODE_OUT;	M_CODE15(NUM,__VA_ARGS__)

#define MENU_SET_NUM(TITLE,NUM, ...)		MS_PRE(TITLE)\
		S_LISTm##NUM(__VA_ARGS__) };		MS_SUF(TITLE,NUM)\
		M_CODE##NUM(NUM,__VA_ARGS__) }

#define M_CODE_OUT			/*goto menu_display_end*/break
		
#define MS_PRE(TITLE)		const PEXCH* TITLE##LIST[] = {
#define MS_SUF(TITLE,NUM)	menu_pos(_S(TITLE), NUM, TITLE##LIST);	switch(giStgPos[giLV]) {

#define MENU_START\
	/*int i;*/	\
	UINT DRAW_ON;\
	/*UINT DRAW_Y;*/	\
menu_start:\
	DRAW_ON = (KEY_L || KEY_R || giMenuDispPos);\
	/*DRAW_Y = gbMenuY+giStgPos[giLV];*/

#define MENU_DISPLAY_END(...)\
	if(MENU_TITLE != _S(OFF)) { menu_pos(MENU_TITLE, 0, 0); if(giMenuDispChg==1) { giMenuDispChg = 0;\
								DrawStr(MENU_IS_NOT_AVAILABLE, gbMenuY+1, MN_MXSP+1, 21);\
								DrawStr(ENTER_d_RETURN,	gbMenuY+4, MN_MXSP+4, 14); }\
		MENU_OUT(/*OUT_ON*/1, 0, )\
	}\
menu_display_end:\
	if(giMenuChkOn) {\
		if(giStgPos[giLV] < 15) {/* 15 : 현재M_CODE16까지지원 */\
			giStgPos[giLV]++;\
			goto menu_start;\
		} else {\
			giLV = giMenuChkLV;\
			if(giLV >= 0) giStgPos[giLV] = giMenuChkPos;\
			giMenuChkOn = 0;\
			return;\
		}\
	}\
	else if(giMenuDispPos) {\
		gbMenuList	= 0;\
		gbMenuVal	= 0;\
		if(giLV >= 0) {\
			if(giStgPos[giLV] != giMenuNum) goto menu_start;\
			giStgPos[giLV] = giMenuDispPos - 1;\
			OsdMenuPos(1);\
		}\
		/*UartTxStgLine();*/\
		giMenuDispPos = 0;\
		giMenuDispChg = 0;\
		giGrayOnly = 0;\
		__VA_ARGS__\
		goto menu_end;\
	} else {\
		if(KEY_U || KEY_D) OsdMenuPos(1);\
	}

#define MENU_DISPLAY_STAT\
	DispClr(15,MN_MXSP,MN_LINESIZE);		if(MENU_TITLE)	{ DISPSTR(MENU_TITLE,	15, MN_MXSP, MN_LINESIZE); }\
	DispClr(16,MN_MXSP,MN_SXSP-MN_MXSP);	if(MENU_NAME)	{ DISPSTR(MENU_NAME,	16, MN_MXSP, MN_SXSP-MN_MXSP); }\
	DispClr(16,MN_SXSP,MN_SUBSIZE);			if(MENU_VAL)	{ DISPSTR(MENU_VAL,		16, MN_SXSP, MN_SUBSIZE); }

#define MENU_DISABLE(MENU_ON)			if(!(MENU_ON)) { if(KEY_U || KEY_D) goto menu_start; }
#define MENU_IN(IN_ON,IN_NAME, ...)		if(KEY_C && (giMenuDispPos == 0) && (IN_ON)) { menu_in(_S(IN_NAME)); __VA_ARGS__ goto menu_start; }
#define MENU_OUT(OUT_ON, CF, ...)		if(KEY_C && (giMenuDispPos == 0) && (OUT_ON)) { __VA_ARGS__  menu_out(CF); goto menu_start; }
#define MENU_CODE(...)					if(giMenuDispPos == 0) { __VA_ARGS__ }
#if 0
#define MENU_STR(MENU_ON,VAL,VALSIZE,NUM, ...)		const PEXCH* __LIST[] = { S_LIST##NUM(__VA_ARGS__) }; menu_str(MENU_ON, &VAL, VALSIZE, NUM, __LIST);	MENU_DISABLE(MENU_ON)
#define MENU_BAR(MENU_ON,VAL,VALSIZE,MIN,MAX,ROT, ...)			menu_bar(MENU_ON, &VAL, VALSIZE, MIN,MAX,ROT, 0, 0);		MENU_DISABLE(MENU_ON)
#define MENU_DEC(MENU_ON,VAL,VALSIZE,MIN,MAX,ROT,UNIT, ...)		menu_bar(MENU_ON, &VAL, VALSIZE, MIN,MAX,ROT, _S(UNIT), 0);	MENU_DISABLE(MENU_ON)
#define MENU_HEX(MENU_ON,VAL,VALSIZE,MIN,MAX,ROT,UNIT, ...)		menu_bar(MENU_ON, &VAL, VALSIZE, MIN,MAX,ROT, _S(UNIT), 1);	MENU_DISABLE(MENU_ON)
#else
#define MENU_STR(MENU_ON,VAL,NUM, ...)		const PEXCH* __LIST[] = { S_LIST##NUM(__VA_ARGS__) }; menu_str(MENU_ON, &VAL, sizeof(VAL), NUM, __LIST);	MENU_DISABLE(MENU_ON)
#define MENU_BAR(MENU_ON,VAL,MIN,MAX,ROT, ...)			menu_bar(MENU_ON, &VAL, sizeof(VAL), MIN,MAX,ROT, 0, 0);		MENU_DISABLE(MENU_ON)
#define MENU_DEC(MENU_ON,VAL,MIN,MAX,ROT,UNIT, ...)		menu_bar(MENU_ON, &VAL, sizeof(VAL), MIN,MAX,ROT, _S(UNIT), 0);	MENU_DISABLE(MENU_ON)
#define MENU_HEX(MENU_ON,VAL,MIN,MAX,ROT,UNIT, ...)		menu_bar(MENU_ON, &VAL, sizeof(VAL), MIN,MAX,ROT, _S(UNIT), 1);	MENU_DISABLE(MENU_ON)
#endif
//-------------------------------------------------------------------------------------------------
// Define etc Font ID (ASCII)
#define	_IIO		133
#define	_IOI		134
#define	_OII		135
#define	_OOO		154
#define	_IOO		155
#define	_OIO		156
#define	_OOI		157

//*************************************************************************************************
// Menu Top
//-------------------------------------------------------------------------------------------------
#define _B			BYTE
#define _W			EXCH
#define _P(...)		{ __VA_ARGS__, 0 }

#define MENU_EXT(TITLE,MENU_ON/*,OUT_ON*/,OUT_CODE, ...)\
	if((MENU_TITLE == _S(TITLE)) && (MENU_ON)) { menu_pos(_S(TITLE), 0, 0); if(giMenuDispChg==1) { giMenuDispChg = 0; __VA_ARGS__ }\
		MENU_OUT(/*OUT_ON*/1, 0, OUT_CODE;)\
		if(giMenuChkOn == 0) goto menu_display_end;\
	}
#define MENU_SET0(NUM,TITLE,MENU_ON/*,OUT_ON*/, ...)\
	if(((MENU_TITLE == _S(TITLE)) && (MENU_ON)) || giMenuChkOn) { MENU_SET_NUM(TITLE, NUM, __VA_ARGS__)\
		/*if(OUT_ON) {\
			if(giMenuDispPos) { DrawStr(ENTER_d_RETURN, MN_YSTART+NUM+1, MN_MXSP+2, 14); }\
			MENU_OUT(UP_ON, 1, DispClr(MN_YSTART+NUM+1, MN_MXSP+2, 14);)\
		}*/\
		if(giMenuChkOn == 0) goto menu_display_end;\
	}
#define MENU_SET(NUM,TITLE,MENU_ON/*,OUT_ON*/, ...)	MENU_SET0(NUM,TITLE,MENU_ON/*,OUT_ON*/, __VA_ARGS__)

// n : nothing
// i : in
// o : out
// ONE : enter
// STR : many menu	
#define MENU_ONEn(MENU_ON,STR)					menu_one(MENU_ON, _S(STR));	MENU_DISABLE(MENU_ON)
#define MENU_ONEi(MENU_ON,STR,IN_ON,IN,IN_CODE)	MENU_ONEn(MENU_ON,STR)	MENU_IN(IN_ON,IN,IN_CODE)
#define MENU_ONEo(MENU_ON,STR,OUT_ON,OUT_CODE)	MENU_ONEn(MENU_ON,STR)	MENU_OUT(OUT_ON,1,OUT_CODE)

#define MENU_PUSH(MENU_ON,VAL,STR_NOR,STR_PUSH,PUSH_DELAY,PUSH_CODE)	if(menu_push(MENU_ON,&VAL,_S(STR_NOR),_S(STR_PUSH),PUSH_DELAY)) {PUSH_CODE;}	MENU_DISABLE(MENU_ON)

#if 0
#define MENU_STRn(MENU_ON,CODE,VAL,VALSIZE,NUM, ...)					({ MENU_STR(MENU_ON,VAL,VALSIZE,NUM, __VA_ARGS__)	MENU_CODE(CODE) })
#define MENU_STRi(MENU_ON,IN_ON,IN,IN_CODE,CODE,VAL,VALSIZE,NUM, ...)	({ MENU_STR(MENU_ON,VAL,VALSIZE,NUM, __VA_ARGS__)	MENU_IN(IN_ON,IN,IN_CODE)	MENU_CODE(CODE) })
#define MENU_STRo(MENU_ON,OUT_ON,OUT_CODE,CODE,VAL,VALSIZE,NUM, ...)	({ MENU_STR(MENU_ON,VAL,VALSIZE,NUM, __VA_ARGS__)	MENU_OUT(OUT_ON,1,OUT_CODE)	MENU_CODE(CODE) })

#define MENU_BARn(MENU_ON,CODE,VAL,VALSIZE,MIN,MAX, ...)		{ MENU_BAR(MENU_ON,VAL,VALSIZE,MIN,MAX, __VA_ARGS__)		MENU_CODE(CODE) }
#define MENU_DECn(MENU_ON,CODE,VAL,VALSIZE,MIN,MAX,ROT, ...)	{ MENU_DEC(MENU_ON,VAL,VALSIZE,MIN,MAX,ROT, __VA_ARGS__)	MENU_CODE(CODE) }
#define MENU_HEXn(MENU_ON,CODE,VAL,VALSIZE,MIN,MAX,ROT, ...)	{ MENU_HEX(MENU_ON,VAL,VALSIZE,MIN,MAX,ROT, __VA_ARGS__)	MENU_CODE(CODE) }
#else
#define MENU_STRn(MENU_ON,CODE,VAL,NUM, ...)					({ MENU_STR(MENU_ON,VAL,NUM, __VA_ARGS__)	MENU_CODE(CODE) })
#define MENU_STRi(MENU_ON,IN_ON,IN,IN_CODE,CODE,VAL,NUM, ...)	({ MENU_STR(MENU_ON,VAL,NUM, __VA_ARGS__)	MENU_IN(IN_ON,IN,IN_CODE)	MENU_CODE(CODE) })
#define MENU_STRo(MENU_ON,OUT_ON,OUT_CODE,CODE,VAL,NUM, ...)	({ MENU_STR(MENU_ON,VAL,NUM, __VA_ARGS__)	MENU_OUT(OUT_ON,1,OUT_CODE)	MENU_CODE(CODE) })

#define MENU_BARn(MENU_ON,CODE,VAL,MIN,MAX, ...)		{ MENU_BAR(MENU_ON,VAL,MIN,MAX, __VA_ARGS__)		MENU_CODE(CODE) }
#define MENU_DECn(MENU_ON,CODE,VAL,MIN,MAX,ROT, ...)	{ MENU_DEC(MENU_ON,VAL,MIN,MAX,ROT, __VA_ARGS__)	MENU_CODE(CODE) }
#define MENU_HEXn(MENU_ON,CODE,VAL,MIN,MAX,ROT, ...)	{ MENU_HEX(MENU_ON,VAL,MIN,MAX,ROT, __VA_ARGS__)	MENU_CODE(CODE) }
#endif

//#define MENU_VARe(MENU_ON,CODE,VAL_L,VAL_H,MIN,MAX)		menu_val_ex(MENU_ON, &VAL_L, &VAL_H, MIN, MAX);		MENU_DISABLE(MENU_ON)	MENU_CODE(CODE)
#define MENU_DIRn(MENU_ON,STA,VAL,DRAW,END) ({\
		menu_sta(MENU_ON);\
		STA\
		if((giMenuDispPos == 0) && MENU_ON) { VAL }\
		if(DRAW_ON && (MENU_ON || giGrayOnly)) { DRAW }\
		END\
		gbMenuVal = 0;	menu_dir(MENU_ON);	MENU_DISABLE(MENU_ON) })

#define MENU_CHANGE()			{ menu_redraw(1,0); goto menu_start; } // NAME, VAL 변경
#define MENU_REDRAW()			{ menu_redraw(0,0); goto menu_start; } // 다시 그리는데, GRAY는 NOT_USED
#define MENU_REDRAW_GRAY_ONLY()	{ menu_redraw(0,1); goto menu_start; } // 다시 그리는데, GRAY에 NOT_USED 안함
#define MENU_OFF_GRAY_ONLY()	  giGrayOnly = 1;	// MENU 들어가기 전 IN_CODE에서 호출, 'NOT USED' 출력을 사용하지 않음.
#define MENU_VAL_REDRAW(VAL)	{ /*gbMenuVal = (PEXCH*)_S(VAL);*/ DISPCLRSTR(_S(VAL), DRAW_Y/*gbMenuY+giStgPos[giLV]*/, MN_SXSP, MN_SUBSIZE, MN_SUBSIZE); } // VAR 영역에 글자 표기
#define MENU_NAME_REDRAW(NAME)	{ /*gbMenuList = (PEXCH*)_S(NAME);*/ DISPCLRSTR(_S(NAME), DRAW_Y/*gbMenuY+giStgPos[giLV]*/, MN_MXSP, (MN_SXSP-MN_MXSP)/*MN_LINESIZE*/, (MN_SXSP-MN_MXSP)/*MN_LINESIZE*/); }
#define MENU_OPEN(MENU)			menu_in(_S(MENU))
#define MENU_CLOSE()			{ OsdMenuPos(0); menu_close(); } // MENU 영역 어디서든 이 함수를 부르면 메뉴가 닫힘
#define if_KEY_LR(CODE)			if(KEY_L || KEY_R) { CODE; }

#define MENU_VAL_PUSH(STR_NOR,STR_PUSH,PUSH_DELAY,/*PUSH_ON,*/CODE_NOR,CODE_PUSH) {\
			if(MENU_VAL_IS(STR_NOR)) {\
				if(pKEY_C/*PUSH_ON*/) {\
					if(gbPushCnt < 0xFF) gbPushCnt++;\
					if(gbPushCnt == PUSH_DELAY) {\
						CODE_PUSH;\
						MENU_VAL_REDRAW(STR_PUSH);\
					}\
				}\
				else {\
					if(gbPushCnt) {\
						gbPushCnt = 0;\
						CODE_NOR;\
						MENU_VAL_REDRAW(STR_NOR);\
					}\
				}\
			}}

#define MENU_TITLE_CHANGE(TITLE) { gbMenuTitle = (PEXCH*)_S(TITLE); gbStgTitle[giLV] = gbMenuTitle; }
