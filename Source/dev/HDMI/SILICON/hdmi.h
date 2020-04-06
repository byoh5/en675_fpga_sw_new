
#ifndef __HDMI_HEADER__
#define __HDMI_HEADER__


#ifdef __HDMI_SILICON__
extern void HDMI_TwiWriteBlock(BYTE abDevaddr, BYTE abAddr, BYTE * apbBuf, UINT anBufEa);
extern void HDMI_TwiReadBlock(BYTE abDevaddr,BYTE abAadr,BYTE * abData,UINT anDataEa);
extern void HDMI_TwiReadSegmentBlock(BYTE abDevaddr,BYTE segment,BYTE abAddr,BYTE * apbBuf,UINT anBufEa);

extern BYTE ReadByteTPI(BYTE RegOffset);
extern void WriteByteTPI(BYTE RegOffset, BYTE Data);

//extern BYTE ENX_I2Cr(WORD DevAddr, WORD RegAddr, UINT* RegData);

extern void InitHdmi(void);
extern void Hdmi(BYTE *abDetFrm);
#endif


#ifdef __HDMI_LONTIUM__
extern void LT8618SX_Init(uint8_t VideoFmt);
#endif


#ifdef HDMI_I2C_CH
extern void HDMI_TwiWrite(BYTE abDevaddr,BYTE abAddr,BYTE abData);
extern void HDMI_TwiRead(BYTE abDevaddr,BYTE abAddr,BYTE * apbBuf);
#endif


#endif

