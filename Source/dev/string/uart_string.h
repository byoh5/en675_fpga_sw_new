#ifndef __UART_STRING_H__
#define __UART_STRING_H__

//extern int UartTxStr(UINT nCH, const char *str);
//extern void UartTxHexUchar(UINT nCH, BYTE data);
//extern void UartTxHexUshort(UINT nCH, USHORT data);
//extern void UartTxHexUint(UINT nCH, UINT data);
//extern void UartTxHexUlong(UINT nCH, ULONG data);
//extern void UartTxDec(UINT nCH, ULONG data);

#define UART_BUF_SIZE		128				// User buffer size
typedef /*volatile*/ struct _UART_QUE{		// Queue structure define
	volatile WORD	wRxLen;					// Number of characters in the Rx ring buffer
	volatile BYTE*	pbRxHead;				// Pointer to where next character will be inserted
	volatile BYTE*	pbRxTail;				// Pointer from where next character will be extracted
	volatile BYTE	bRxQue[UART_BUF_SIZE];	// Ring buffer character storage (Rx)

	volatile WORD	wTxLen;					// Number of characters in the Tx ring buffer
	volatile BYTE*	pbTxHead;				// Pointer to where next character will be inserted
	volatile BYTE*	pbTxTail;				// Pointer from where next character will be extracted
	volatile BYTE	bTxQue[UART_BUF_SIZE];	// Ring buffer character storage (Tx)
} TUartQue;

extern TUartQue gtUartQue;
extern void UartRstRxQue(void);
extern void UartRstTxQue(void);
extern void UartRstQue(void);
extern void UartTxIrq(BYTE abByte);
extern void UartDebugRxIrq(void *ctx);
extern void UartDebugTxIrq(void *ctx);
extern void UartTxGrp(void);
extern void UartTxGrpSet(UINT anNum, UINT anData);
extern void UartTxGrpRun(void);
extern void Comm(void);

//#define UartTxStrHex(STR,VAL,LEN)		printf(STR"%"#LEN"x\r\n", VAL)
//#define UartTxStrDec(STR,VAL,LEN)		printf(STR"%"#LEN"d\r\n", VAL)
//#define UartTxStrHexCh(CH,STR,VAL,LEN)	printf(STR"%"#LEN"x\r\n", VAL)
//#define UartTxStrDecCh(CH,STR,VAL,LEN)	printf(STR"%"#LEN"d\r\n", VAL)
extern void UartTxStrEx(const UINT nCH, const char* apbStr0, const char* apbStr1, UINT anLen, UINT anAddNewLine);
extern void UartTxStrHexCh(const UINT nCH, const char* apbStr0, UINT anVal, UINT anValLen);
extern void UartTxStrDecCh(const UINT nCH, const char* apbStr0, UINT anVal, UINT anValLen);
extern void UartTxStrCh(const UINT nCH, const char* apbStr0);

#define UartTxStr(STR)					UartTxStrCh(INVALID_UART_NUM, STR)
#define UartTxStrNoIRQ(STR)				UartTxStrCh(DEBUG_UART_NUM, STR)
#define UartTxStrHex(STR,VAL,LEN)		UartTxStrHexCh(INVALID_UART_NUM, STR, VAL, LEN)
#define UartTxStrHexNoIRQ(STR,VAL,LEN)	UartTxStrHexCh(DEBUG_UART_NUM, STR, VAL, LEN)
#define UartTxStrDec(STR,VAL,LEN)		UartTxStrDecCh(INVALID_UART_NUM, STR, VAL, LEN)
#define UartTxStrDecNoIRQ(STR,VAL,LEN)	UartTxStrDecCh(DEBUG_UART_NUM, STR, VAL, LEN)

#ifdef __ECM_STRING__
#define INIT_STR			UartTxStrNoIRQ
#else
#define INIT_STR			printf
#endif
#define INIT_STR_DEC		UartTxStrDecNoIRQ
#define INIT_STR_HEX		UartTxStrHexNoIRQ
#define INIT_STR_SENSOR		{void printf_SensorSetting(void); printf_SensorSetting();}


#endif /* __UART_STRING_H__ */
