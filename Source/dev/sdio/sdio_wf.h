#ifndef _SDIO_WF_H_
#define	_SDIO_WF_H_

#if defined(__WIFI__)

/* I/O Read/Write Commands */
#define SDCMD_IO_RW_DIRECT_CMD        52 /* [31:16] RCA                 | R5  */
#define SDCMD_IO_RW_EXTENDED_CMD      53 /* [ 0] RD/WR                  | R5  */



#define WIFI_LOG_WSSB 0x80
#define WIFI_LOG_WSPB 0x40
#define WIFI_LOG_RSSB 0x20
#define WIFI_LOG_RSPB 0x10

#define WIFI_LOG_WPB 0x8
#define WIFI_LOG_WSB 0x4
#define WIFI_LOG_RPB 0x2
#define WIFI_LOG_RSB 0x1

typedef ATTR_MALIGN16 struct {
	UINT nCH;
	UINT nErrorCode;
	UINT nActive;
	UINT nVoltageMode;
	UINT nSetClock;
} SDIO_WF;

// CMD5
#define OCR_VOL_WIND		0x00ff8000

// R4
#define R4_C				0x80000000
#define R4_FUNC_NUM			0x70000000
#define R4_MEM_PRESENT		0x08000000
#define R4_OCR				0x00ffff00

// CMD52
#define CMD52_READ  			0x00000000
#define CMD52_WRITE				0x80000000

extern void SdioWfSetHighSpeed(void);
extern void SdioWfSetNormalSpeed(void);
extern SDIO_OKFAIL SdioWfRegWrite(BYTE data, UINT func_num, UINT addr);
extern SDIO_OKFAIL SdioWfRegRead(BYTE *data, UINT func_num, UINT addr);
extern SDIO_OKFAIL SdioWfDatWrite(BYTE *data, UINT func_num, UINT addr, UINT size);
extern SDIO_OKFAIL SdioWfDatRead(BYTE *data, UINT func_num, UINT addr, UINT size);
extern SDIO_OKFAIL SdioWfCmd(BYTE Cmd, UINT Arg, UINT *nResp, eCmdRespType cmdType);
extern ENX_OKFAIL SdioWfInitProcess(void);
extern void SdioWfInit(UINT nCH);
extern void SdioWfIoIrqCallback(irq_fn irqfn, void *arg);
extern void SdioWfSetIoIrqEn(ENX_SWITCH sw);
extern void SdioWfIoIrqClear(void);

#endif // __WIFI__
#endif // _SDIO_WF_H_
