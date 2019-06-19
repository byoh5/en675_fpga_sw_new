#ifndef __EEPROM_H__
#define __EEPROM_H__

extern void EepromInit(void);
extern BYTE EepWrite(const WORD awAddr, const BYTE abData);
extern BYTE EepRead(const WORD awAddr, volatile BYTE* abData, const UINT anDataEa);
extern BYTE EepPageWrite(const WORD awWaddr, BYTE* apbBuf, const UINT anBufEa);
extern BYTE TwiWrEep2( WORD awWaddr, volatile BYTE* apbBuf, UINT anBufEa);

#endif // __EEPROM_H__