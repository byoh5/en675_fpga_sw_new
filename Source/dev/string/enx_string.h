#ifndef __ENX_STRING_H__
#define __ENX_STRING_H__

extern int IPtoStr(unsigned int addr, char* buf, int buflen, int type);
extern char* enx_strcat(char* dest, char* src);
extern BYTE* enx_memstr(BYTE* src, int srcSize, BYTE* search, int searchSize);
extern void Hex2Str32B(unsigned char* digest, char* rchar);
extern void Dec2Str64B(char *str, ULONG val);

#endif /* __ENX_STRING_H__ */
