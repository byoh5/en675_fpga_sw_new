/***************************************************************************
 File Name	: 	font.c
 Description:	EN675 - Font Control driver
 Designer	:
 Date		:
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include "dev.h"


//UINT gnFontdummy=0;
UINT gnFontXw = 60;
UINT gnFontYw = 23;
UINT gnFontXwMax[3] = { 60,60,60 };
UINT gnFontYwMax[3] = { 23,23,23 };

char gbStr[16+1];


//	HOfs, VOfs		->	Font H/V Position Offset
//	IsBord			->	0 : No Border, 1: Use Border
//	FontHw			->	Image Resolution
void FontInit(UINT HOfs, UINT VOfs, BOOL IsBord, UINT FontHw)
{
	FONT_ON0w(1);
	FONT_OFY0w(VOfs);
	FONT_OFX0w(HOfs);
//	FONT_VBLK0w(0x81);
//	FONT_HBLK0w(0x80);
	//FONT_VBLK0w(VBlk);
	//FONT_HBLK0w(HBlk);
	//FONT_VSIZ0w(VSize);
	//FONT_HSIZ0w(HSize);
	BOAD_ON0w(IsBord);
	//BOAD_MODE0w(BordMod);
	FONT_HW0w(FontHw);
	FONT_MUTE0w(0);
}

void FontSetColor(UINT FontColor0, UINT FontColor1, UINT FontColor2, UINT FontColor3)
{
//	FONT_LV0_0w(0x108080);
//	FONT_LV1_0w(0xF08080);
//	FONT_LV2_0w(0xA82C87);
//	FONT_LV3_0w(0x336CD4);
	FONT_LV0_0w(FontColor0);
	FONT_LV1_0w(FontColor1);
	FONT_LV2_0w(FontColor2);
	FONT_LV3_0w(FontColor3);
}

//	FontSize		->	0 : Big Font, 1 : Small Font
//	FontAreaSet		->	0 : Big Font, 1 : Small Font for Debug and Right-Up Title, 2 : Small Font for Debug and Left-Down Title
//	BordMod			->	0 : Normal,	1 : thick
void FontSetSize(UINT FontSize, UINT FontAreaSet, UINT BordMod)
{
	gnFontXw = gnFontXwMax[FontAreaSet];
	gnFontYw = gnFontYwMax[FontAreaSet];

	if(FontSize == SMALL_FONT) {
		FONT_VBLK0w(0x1);	// Font V Blank : Normally 0x1
		FONT_HBLK0w(0x80);	// Font H Blank : Normally 0x80
		FONT_VSIZ0w(1);		// Font V Size  : Normally '2'
		FONT_HSIZ0w(1);		// Font H Size  : Normally '2'
	}
	else {
		FONT_VBLK0w(0x1);	// Font V Blank : Normally 0x1
		FONT_HBLK0w(0x80);	// Font H Blank : Normally 0x80
		FONT_VSIZ0w(2);		// Font V Size  : Normally '2'
		FONT_HSIZ0w(2);		// Font H Size  : Normally '2'
	}

	BOAD_MODE0w(BordMod);
}

void FontSetArea(UINT BigFontMaxW, UINT BigFontMaxH, UINT SmallFontMaxW, UINT SmallFontMaxH)
{
	// Big Font for MENU
	gnFontXwMax[0] = BigFontMaxW;
	gnFontYwMax[0] = BigFontMaxH;

	// Small Font for Debug and Right-Up Title
	gnFontXwMax[1] = SmallFontMaxW;
	gnFontYwMax[1] = ISP_FONT_ID_EA / (SmallFontMaxW+1);	// do +1 because of Line returnID
	if(gnFontYwMax[1] > SmallFontMaxH) gnFontYwMax[1] = SmallFontMaxH;

	// Small Font for Debug and Left-Down Title
	gnFontXwMax[2] = (ISP_FONT_ID_EA / SmallFontMaxH) - 1;	// do -1 because of Line returnID
	gnFontYwMax[2] = SmallFontMaxH;
	if(gnFontXwMax[2] > SmallFontMaxW) gnFontXwMax[2] = SmallFontMaxW;
}

void FontClrAll(BYTE Alpha, BYTE Color)
{	// init ID
    UINT i;
    UINT j=1;
    UINT n=0;

	//FONT_ON0w(1);
	//FONT_MUTE0w(1);

	for(j=0; j<gnFontYw; j++) {															// Initial Font ID
		for(i=0; i<(gnFontXw+1); i++,n++) {
			if(i<gnFontXw)			{SetFontID(n, SPACE_CHAR_ID);}		// fill space
			else if((j+1)<gnFontYw)	{SetFontID(n, ISP_FONT_LINE_RET);}	// Line returnID
			else					{SetFontID(n, ISP_FONT_PAGE_RET);}	// Page returnID

			SetFontAttr(n, Color);
			SetFontAlpha(n, Alpha);
		}
	}

	//FONT_MUTE0w(0);
	//FONT_ON0w(0);
}

void FontCharInit(const UINT anFont[][12], UINT Ea)
{
	UINT i,j;

	if(Ea > ISP_FONT_CHAR_EA) Ea = ISP_FONT_CHAR_EA;

	//FONT_ON0w(1);
	FONT_MUTE0w(1);
	CHAR_WMOD0w(1);

	for(j=0; j<Ea; j++){
		for(i=0; i<12; i++){
			SetFontChar( (j*12)+i, ((UINT)(anFont[j][i])));
		}
	}

	CHAR_WMOD0w(0);
	FONT_MUTE0w(0);
	//FONT_ON0w(0);
}

void FontEx(UINT anPosY, UINT anPosX, BYTE Alpha, BYTE Color, UINT anLen)
{	// Font attributes setting
	UINT i;
	for(i=0; i<anLen; i++) {
		SETFONTATTR(anPosY, anPosX+i, Color);
		SETFONTALPHA(anPosY, anPosX+i, Alpha);
	}
}

void FontStr(UINT anPosY, UINT anPosX, const char* cStr, UINT anLen)
{
	UINT i;
	for(i=0; i<anLen && (*(cStr+i)); i++) {
		SETFONTID(anPosY, anPosX+i, (BYTE)cStr[i]);
	}
}

void FontStrEx(UINT anPosY, UINT anPosX, BYTE Alpha, BYTE Color, const char* cStr, UINT anLen)
{
	UINT i;
	for(i=0; i<anLen && (*(cStr+i)); i++) {
		SETFONTID(anPosY, anPosX+i, (BYTE)cStr[i]);
		SETFONTATTR(anPosY, anPosX+i, Color);
		SETFONTALPHA(anPosY, anPosX+i, Alpha);
	}
}

void FontClr(UINT anPosY, UINT anPosX, UINT anLen)
{
	UINT i;
	for(i=0; i<anLen; i++) {
		SETFONTID(anPosY, anPosX+i, SPACE_CHAR_ID);
	}
}

void FontClrStr(UINT anPosY, UINT anPosX, const char* cStr, UINT anStrLen, UINT anClrLen)
{
	UINT i;
	for(i=0; i<anStrLen && (*(cStr+i)); i++) {
		SETFONTID(anPosY, anPosX+i, (BYTE)cStr[i]);
	}

	for(; i<anClrLen; i++) {
		SETFONTID(anPosY, anPosX+i, SPACE_CHAR_ID);
	}
}

#if model_CharMax == 1
UINT strlenW(const EXCH* apbStr)
{	// String length
	UINT	nLen = 0;
    while(*apbStr++) nLen++;
	return nLen;
}

void FontStrW(UINT anPosY, UINT anPosX, const EXCH* cStr, UINT anLen)
{
	UINT i;
	for(i=0; i<anLen && (*(cStr+i)); i++) {
		SETFONTID(anPosY, anPosX+i, (EXCH)(*(cStr+i)));
	}
}

void FontClrStrW(UINT anPosY, UINT anPosX, const EXCH* cStr, UINT anStrLen, UINT anClrLen)
{
	UINT i;
	for(i=0; i<anStrLen && (*(cStr+i)); i++) {
		SETFONTID(anPosY, anPosX+i, (EXCH)(*(cStr+i)));
	}

	for(; i<anClrLen; i++) {
		SETFONTID(anPosY, anPosX+i, SPACE_CHAR_ID);
	}
}
#endif

void FontClrLine(UINT anPosY)
{
	UINT i;
	for(i=0; i<gnFontXw; i++) {
		SETFONTID(anPosY, i, SPACE_CHAR_ID);
	}
}

void FontClrChar(UINT anPosY, UINT anPosX)
{
	SETFONTID(anPosY, anPosX, SPACE_CHAR_ID);
}

void uint2strh(char* apbStr, UINT anVal, UINT anLen)
{	// Value -> String Hex
	UINT	i;
	apbStr[anLen] = '\0';	// CAUTION!
	apbStr += anLen-1;
	for(i=0; i<anLen; i++){
		const BYTE bBuf = anVal & 0xf;
		*apbStr-- = (bBuf >= 0xA) ? bBuf - 0xA + 'A' : bBuf + '0' ;
		anVal >>= 4;
    }
}

UINT uint2str(char* apbStr, UINT anVal, UINT anLen)	// Left Align
{
	UINT	i=0;
	UINT	t;
	char	tmp[11];

	while(anVal >= 10 && i<anLen) {
		t = anVal / 10;
		tmp[i++] = '0' + (anVal - (t * 10));
		anVal = t;
	}

	if(i<anLen) tmp[i++] = '0' + anVal;

	*(apbStr+i) = '\0';
	t = i;

	while(i) *apbStr++ = tmp[--i];

	return t;
}

void int2str(char* apbStr, int aiVal, UINT anLen, const BYTE abFill)	// Right Align
{	// Value -> String Dec
	UINT	i;
	UINT	nMod;

	apbStr[anLen] = '\0';	// CAUTION!

	int neg;

	if(aiVal < 0) {
		neg = 1;
		aiVal = -aiVal;
	}
	else {
		neg = 0;
	}

	for(i=0; i<anLen-neg; i++){
		if(aiVal >= 10) {
			nMod = aiVal / 10;
			apbStr[anLen-i-1] = '0' + (aiVal - (nMod * 10));
			aiVal = nMod;
		}
		else {
			break;
		}
	}

	if(i<anLen && aiVal >= 0) {
		apbStr[anLen-i-1] = '0' + aiVal;
		i++;
	}

	if(i<anLen && neg) {
		apbStr[anLen-i-1] = '-';
		i++;
	}

	const char c = (abFill) ? '0' : ' ';

	for(; i<anLen; i++) {
		apbStr[anLen-i-1] = c;
	}
}

UINT hex2dec(const UINT hex)
{
	UINT i, d=1, dec=0;

	for(i=0; i<32; i+=4) {
		const UINT digit = (hex>>i)&0xf;
		//dec += ((digit < 0xa) ? digit : 9) * d;
		if(digit < 0xa) dec += digit * d;
		else			dec = d * 10 - 1;
		d *= 10;
	}

	return dec;
}

UINT dec2hex(UINT dec)
{
	UINT i, h=1, hex=0;

	for(i=0; i<8; i++) {
		const UINT nMod = dec / 10;
		hex += (dec - (nMod * 10)) * h;
		dec = nMod;
		h *= 16;
	}

	return hex;
}

void FontDec(const UINT anPosY, const UINT anPosX, const int aiVal, UINT anLen, const BYTE abFill)
{
	//if(sizeof(gbStr) <= anLen) anLen = sizeof(gbStr) - 1;
	if(12 <= anLen) anLen = 12 - 1;	// 12 = sign + 10 + '\0'
	int2str(gbStr, aiVal, anLen, abFill);
	FontStr(anPosY, anPosX, gbStr, anLen);
}

void FontHex(const UINT anPosY, const UINT anPosX, const UINT anVal, UINT anLen)
{
	if(9 <= anLen) anLen = 9 - 1;	// 9 = 8 + '\0'
	uint2strh(gbStr, anVal, anLen);
	FontStr(anPosY, anPosX, gbStr, anLen);
}

void FontDecEx(const UINT anPosY, const UINT anPosX, const BYTE Alpha, const BYTE Color, const int aiVal, UINT anLen, const BYTE abFill)
{
	FontDec(anPosY, anPosX, aiVal, anLen, abFill);
	FontEx(anPosY, anPosX, Alpha, Color, anLen);
}

