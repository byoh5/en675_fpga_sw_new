
#ifndef __HDMI_HEADER__
#define __HDMI_HEADER__

//*************************************************************************************************
// 4K
//-------------------------------------------------------------------------------------------------

//#include "Constants.h"	// 160118 HSH

enum{
	NO_INPUT = 1	,
	IN_HDSDI		,
	IN_3GSDI		,
	IN_EX270_HD		,
	IN_EX270_3G_4M	,
	IN_EX135_HD		,
	IN_EX1G5_4K
};

extern void HDMI_TwiWriteBlock(BYTE abDevaddr, BYTE abAddr, BYTE * apbBuf, UINT anBufEa);
extern void HDMI_TwiWrite(BYTE abDevaddr,BYTE abAddr,BYTE abData);

extern void HDMI_TwiRead(BYTE abDevaddr,BYTE abAddr,BYTE * apbBuf);
extern void HDMI_TwiReadBlock(BYTE abDevaddr,BYTE abAadr,BYTE * abData,UINT anDataEa);

extern void HDMI_TwiReadSegmentBlock(BYTE abDevaddr,BYTE segment,BYTE abAddr,BYTE * apbBuf,UINT anBufEa);

extern BYTE ReadByteTPI(BYTE RegOffset);
extern void WriteByteTPI(BYTE RegOffset, BYTE Data);

extern BYTE ENX_I2Cr(WORD DevAddr, WORD RegAddr, UINT* RegData);


extern void InitHdmi(void);
extern void Hdmi(BYTE *abDetFrm);




#endif

