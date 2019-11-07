#ifndef __SFLS_MODEL_H__
#define __SFLS_MODEL_H__

extern SFLScontrol* SflsModelEON(BYTE u8Type, BYTE u8Capa);
extern SFLScontrol* SflsModelMicron(BYTE u8Type, BYTE u8Capa);
extern SFLScontrol* SflsModelISSI(BYTE u8Type, BYTE u8Capa);
extern SFLScontrol* SflsModelMacronix(BYTE u8Type, BYTE u8Capa);
extern SFLScontrol* SflsModelGigaDevice(BYTE u8Type, BYTE u8Capa);
extern SFLScontrol* SflsModelWinbond(BYTE u8Type, BYTE u8Capa);

#endif // __SFLS_MODEL_H__
