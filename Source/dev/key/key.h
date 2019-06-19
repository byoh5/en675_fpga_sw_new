#ifndef __KEY_H__
#define __KEY_H__


extern void KeyScan(void);
extern void KeyAccLevel(int *aiKeyAdd);
extern BYTE gbUKeyVal;	// Uart for ECM
extern TKey	gtUKey;
extern BYTE gbPKeyVal;	// PELCO
extern TKey	gtPKey;
extern BYTE	gbMKey;		// Menu Key
extern BYTE	gbPKey;
extern BYTE	gbSKeyVal;	// AD Key
extern TKey	gtSKey;
extern int giKeyAdd;
extern UINT gbPTZKeyVal;	// for Onvif PTZ


#endif // __KEY_H__
