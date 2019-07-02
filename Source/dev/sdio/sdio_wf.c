#include "dev.h"
#include "sdio_wf.h"

#if defined(__WIFI__)

static SDIO_WF wfinfo;

#define ENX_SDIOWF_DELAY vTaskDelay
#define DELAY_MS(ms) ENX_SDIOWF_DELAY(ms)

#define ENX_SDIOWF_IRQ_LOCK portENTER_CRITICAL
#define ENX_SDIOWF_IRQ_UNLOCK portEXIT_CRITICAL

#define ENX_SDIOWF_FLUSH_DCACHE hwflush_dcache_range

//-------------------------------------------------------------------------------------------------

void SdioWfSetHighSpeed(void)
{
	SdioSetClockDiv(wfinfo.nCH, 0);
}

void SdioWfSetNormalSpeed(void)
{
	SdioSetClockDiv(wfinfo.nCH, 1);
}

ENX_OKFAIL SdioWfRegWrite(BYTE data, UINT func, UINT addr)
{
	ENX_OKFAIL bRes = ENX_FAIL;
 	UINT *pResp;
 	UINT nArg = 0;

 	ENX_LOG_START(DBG_SDIO_WF_CMD);

	nArg = CMD52_WRITE | (func << 28) | (addr << 9) | data;
	SdioSetIoOpMode(wfinfo.nCH, SDIO_WRITE_OP);
	bRes = SdioCmd(wfinfo.nCH, SDCMD_IO_RW_DIRECT_CMD, nArg, pResp, ecrtR5);
	if (bRes == ENX_FAIL) {
		_Rprintf("Error: CMD52 Write, Resp(0x%08X)\r\n", *pResp);
	}

	ENX_LOG_END(DBG_SDIO_WF_CMD);

	return bRes;
}

ENX_OKFAIL SdioWfRegRead(BYTE *data, UINT func, UINT addr)
{
	ENX_OKFAIL bRes = ENX_FAIL;
 	UINT *pResp;
 	UINT nArg = 0;

 	ENX_LOG_START(DBG_SDIO_WF_CMD);

	nArg = CMD52_READ | (func << 28) | (addr << 9);
	SdioSetIoOpMode(wfinfo.nCH, SDIO_READ_OP);
	bRes = SdioCmd(wfinfo.nCH, SDCMD_IO_RW_DIRECT_CMD, nArg, pResp, ecrtR5);
	if (bRes == ENX_FAIL) {
		_Rprintf("Error: CMD52 Read, Resp(0x%08X)\r\n", *pResp);
	} else {
		*data = *pResp & 0xFF;
	}

	ENX_LOG_END(DBG_SDIO_WF_CMD);

	return bRes;
}

ENX_OKFAIL SdioWfDatWrite(BYTE *buf, UINT addr, UINT size)
{
	ENX_OKFAIL bRes = ENX_FAIL;

	ENX_LOG_START(DBG_SDIO_WF_DAT);

	UINT nDBlkSize = SdioGetDataBlockByte(wfinfo.nCH);
	UINT nDBcount = size / nDBlkSize;


	SdioSetIoFnvalue(wfinfo.nCH, func->num);		// Function Number
	SdioSetIoBmMode(wfinfo.nCH, SDIO_BM_BLOCK);		// Block mode
	//SdioSetIoOpCode(wfinfo.nCH, SDIO_OP_FIXED);	// OP Code
//	ENX_OKFAIL SdioDataIO(UINT nCH, eSDIO_DIO_TYPE iotype, ULONG MemDst, UINT BlkAdr, UINT BlkCnt)
	// nArg = CMD53_WRITE | func->num<<28 | addr<<9 | blocks | CMD53_BMODE;
	bRes = SdioDataIO(wfinfo.nCH, SDIO_DIO_IO_WRITE, (ULONG)buf, addr, nDBcount);













	ENX_LOG_END(DBG_SDIO_WF_CMD);

	return bRes;
}

ENX_OKFAIL SdioWfDatRead(BYTE *data, UINT func, UINT addr)
{
	ENX_OKFAIL bRes = ENX_FAIL;

	ENX_LOG_START(DBG_SDIO_WF_DAT);

	SdioSetIoOpMode(wfinfo.nCH, SDIO_READ_OP);

	ENX_LOG_END(DBG_SDIO_WF_CMD);

	return bRes;
}



ENX_OKFAIL SdioWfInitProcess(void)
{
	ENX_LOG_START(DBG_SDIO_WF_CMD);

	UINT nCH = wfinfo.nCH;
	memset(&wfinfo, 0, sizeof(wfinfo));
	wfinfo.nCH = nCH;

	SdioSetClockDef(wfinfo.nCH);
	char strClockPrint[40] = {0};
#if (DBG_SDIO_WF_CMD & ENX_DBG_STATE)
	SdioClockDivPrint(wfinfo.nCH, strClockPrint);
	printf("SDIO%u(Wi-Fi) Init Start(%s)\n", wfinfo.nCH, strClockPrint);
#endif

	// process....
	// process....
	// process....
	// process....
	// process....
	// process....
	// process....
	// process....
	// process....
	// process....

	wfinfo.nActive = 1;

	SdioClockDivPrint(wfinfo.nCH, strClockPrint);
	_Gprintf("SDIO%u(Wi-Fi) Init OK(%s)\n", wfinfo.nCH, strClockPrint);

	ENX_LOG_END(DBG_SDIO_WF_CMD);
	return ENX_OK;
done_fail:
	SdioClockDivPrint(wfinfo.nCH, strClockPrint);
	_Rprintf("SDIO%u(Wi-Fi) Init Fail(err:%u)(%s)\n", wfinfo.nCH, wfinfo.nErrorCode, strClockPrint);

	ENX_LOG_END(DBG_SDIO_WF_CMD);
	return ENX_FAIL;
}

// 기본적인 CH, IRQ, 최초주파수, Pull-up 등을 설정한다.
void SdioWfInit(UINT nCH)
{
	memset(&wfinfo, 0, sizeof(wfinfo));
	wfinfo.nCH = nCH;
	wfinfo.nErrorCode = 0;

	SdioSetIoMode(wfinfo.nCH, SDIO_IO_MODE);

	GpioSetOut(WF_GPIO_RST, GPIO_OUT_LOW); // LO:ON HI:OFF
	SdioSetDelayfn(wfinfo.nCH, (user_delay_fn)ENX_SDIOWF_DELAY);
}
#endif
