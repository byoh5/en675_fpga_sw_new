#ifndef __ISP_FONT_H__
#define __ISP_FONT_H__

#define		ISP_FONT_ID_EA			4096	//	Max 4096

#define		ISP_BASE_FONT_IDATTR0	0x1000	//	Attr, ID, Alpha -> Same Address
#define		ISP_BASE_FONT_CHAR0		0x2000

#define		ISP_FONT_LINE_RET		339
#define		ISP_FONT_PAGE_RET		340

#define		ISP_FONT_CHAR_EA		(341-2)	//	Max 341-2(line,page)

#define		MN_GREEN			0x0								// Attribute
#define		MN_WHITE			0x1								// "
#define		MN_GRAY				0x2
#define 	MN_YELLOW			0x3								// "


#define		SPACE_CHAR_ID		0

#define		ISP_FONT_BLACK		0x108080
#define		ISP_FONT_WHITE		0xF08080/*0xb48080*/
#define		ISP_FONT_YELLOW		0xA82C87/*0xa010a0*/
#define		ISP_FONT_RED		0x336CD4
#define		ISP_FONT_GRAY		/*0xB48080*/0x708080
#define		ISP_FONT_CYAN		0x91932C
#define		ISP_FONT_BLUE		0x1BD478
#define		ISP_FONT_MAGENTA	0x3EC0CC
#define		ISP_FONT_GREEN		/*0x853F33*/0xff0000

#define		BORDER_NORMAL		0
#define		BORDER_THICK		1

#define		NO_ALPHA			0
#define		ALPHA_75PER			1
#define		ALPHA_50PER			2
#define		ALPHA_25PER			3

#define		BIG_FONT			0
#define		SMALL_FONT			1

#define		BIG_FONT_AREA		0
#define		SMALL_FONT_AREA_W	1
#define		SMALL_FONT_AREA_H	2


//extern UINT gnFontdummy;
extern UINT gnFontXw;
extern UINT gnFontYw;
extern UINT gnFontXwMax[3];
extern UINT gnFontYwMax[3];
extern char gbStr[33/*16*/+1];	// 33 : for FontBin()

extern void FontInit(UINT HOfs, UINT VOfs, BOOL IsBord, UINT FontHw);
extern void FontSetColor(UINT FontColor0, UINT FontColor1, UINT FontColor2, UINT FontColor3);
extern void FontSetSize(UINT FontSize, UINT FontAreaSet, UINT BordMod);
extern void FontClrAll(BYTE Alpha, BYTE Color);
extern void FontCharInit(const UINT anFont[][12], UINT Ea);
extern void FontEx(UINT anPosY, UINT anPosX, BYTE Alpha, BYTE Color, UINT anLen);
extern void FontCharEx(UINT anPosY, UINT anPosX, BYTE Alpha, BYTE Color, char cChar);
extern UINT FontStrEx(UINT anPosY, UINT anPosX, BYTE Alpha, BYTE Color, const char* cStr, UINT anLen);
extern UINT FontStr(UINT anPosY, UINT anPosX, const char* cStr, UINT anLen);
extern void FontClr(UINT anPosY, UINT anPosX, UINT anLen);
extern void FontClrStr(UINT anPosY, UINT anPosX, const char* cStr, UINT anStrLen, UINT anClrLen);
extern void FontClrLine(UINT anPosY);
extern void FontClrChar(UINT anPosY, UINT anPosX);
extern void FontBin(const UINT anPosY, const UINT anPosX, const UINT anVal, UINT anLen);
extern void FontHex(const UINT anPosY, const UINT anPosX, const UINT anVal, UINT anLen);
extern void FontDec(const UINT anPosY, const UINT anPosX, const int aiVal, UINT anLen, const BYTE abFill);
extern void FontHexEx(const UINT anPosY, const UINT anPosX, const BYTE Alpha, const BYTE Color, const int aiVal, UINT anLen);
extern void FontDecEx(const UINT anPosY, const UINT anPosX, const BYTE Alpha, const BYTE Color, const int aiVal, UINT anLen, const BYTE abFill);
extern void FontStrDec(const BYTE abOn, const UINT anPosY, const UINT anPosX, BYTE StrColor, const char* cStr, const BYTE ValColor, const int aiVal, UINT anValLen, const BYTE abFill);
extern void FontStrHex(const BYTE abOn, const UINT anPosY, const UINT anPosX, BYTE StrColor, const char* cStr, const BYTE ValColor, const int aiVal, UINT anValLen);
extern void FontStrDecUnit(const BYTE abOn, const UINT anPosY, const UINT anPosX, BYTE StrColor, const char* cStr, const BYTE ValColor, const int aiVal, UINT anValLen, const char* cStrUnit);
#if model_CharMax == 1
extern UINT strlenW(const EXCH* apbStr);
extern void FontStrW(UINT anPosY, UINT anPosX, const EXCH* cStr, UINT anLen);
extern void FontClrStrW(UINT anPosY, UINT anPosX, const EXCH* cStr, UINT anStrLen, UINT anClrLen);
#endif

extern void uint2strb(char* apbStr, UINT anVal, UINT anLen);
extern void uint2strh(char* apbStr, UINT anVal, UINT anLen);
extern UINT uint2str(char* apbStr, UINT anVal, UINT anLen);
extern void int2str(char* apbStr, int aiVal, UINT anLen, const BYTE abFill);
extern UINT hex2dec(const UINT hex);
extern UINT dec2hex(UINT dec);


#define SetFontChar(anAddr,anData)	{_wr32(REG_BASE_ISP+((ISP_BASE_FONT_CHAR0 +(UINT)(anAddr))<<2),  (UINT)(anData));\
									/*gnFontdummy = *(volatile unsigned int*)(unsigned long)(REG_BASE_ISP+((ISP_BASE_FONT_CHAR0 +(UINT)(anAddr))<<2));*/ }

#define SetFontID(anAddr,anID)  	{_wr32(REG_BASE_ISP+((ISP_BASE_FONT_IDATTR0 +(UINT)(anAddr))<<2), (0x80000000|((UINT)(anID)&0x1FF)));\
									/*gnFontdummy = *(volatile unsigned int*)(unsigned long)(REG_BASE_ISP+((ISP_BASE_FONT_IDATTR0 +(UINT)(anAddr))<<2));*/ }

#define SetFontAttr(anAddr,anAttr)  {_wr32(REG_BASE_ISP+((ISP_BASE_FONT_IDATTR0 +(UINT)(anAddr))<<2), (0x40000000|((UINT)(anAttr<<9)&0x600)));\
									/*gnFontdummy = *(volatile unsigned int*)(unsigned long)(REG_BASE_ISP+((ISP_BASE_FONT_IDATTR0 +(UINT)(anAddr))<<2));*/ }

#define SetFontAlpha(anAddr,anAlpha)  {_wr32(REG_BASE_ISP+((ISP_BASE_FONT_IDATTR0 +(UINT)(anAddr))<<2), (0x20000000|((UINT)(anAlpha<<11)&0x1800)));\
									/*gnFontdummy = *(volatile unsigned int*)(unsigned long)(REG_BASE_ISP+((ISP_BASE_FONT_IDATTR0 +(UINT)(anAddr))<<2));*/ }

extern int giMenuChkOn;

#define SETFONTID(Y,X,ID)			{ if(X<gnFontXw && !giMenuChkOn) SetFontID((((Y)*(gnFontXw+1))+(X)), ID); }			//	Line Return -> (gnFontXw+1)
#define SETFONTATTR(Y,X,ATTR)		{ if(X<gnFontXw && !giMenuChkOn) SetFontAttr((((Y)*(gnFontXw+1))+(X)), ATTR); }		//	Line Return -> (gnFontXw+1)
#define SETFONTALPHA(Y,X,ALPHA)		{ if(X<gnFontXw && !giMenuChkOn) SetFontAlpha((((Y)*(gnFontXw+1))+(X)), ALPHA); }		//	Line Return -> (gnFontXw+1)


#endif /* __ISP_FONT_H__ */





