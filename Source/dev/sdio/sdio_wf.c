#include "dev.h"

#if defined(__WIFI__)

#include "sdio_wf.h"
#include "enx_freertos.h"

#ifdef __WIFI_SD8782__
#include "ewl.h"

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

SDIO_OKFAIL SdioWfRegWrite(BYTE data, UINT func_num, UINT addr)
{
	SDIO_OKFAIL bRes = SDIO_FAIL;
 	UINT nResp = 0, nArg = 0;

 	ENX_LOG_START(DBG_SDIO_WF_CMD);

	nArg = CMD52_WRITE | (func_num << 28) | (addr << 9) | data;
	ENX_OKFAIL sdres = SdioCmd(wfinfo.nCH, SDCMD_IO_RW_DIRECT_CMD, nArg, &nResp, ecrtR5);
	if (sdres == ENX_OK) {
		bRes = SDIO_OK;
	} else {
		_Rprintf("Error: CMD52 Write, Resp(0x%08X)\n", nResp);
	}

	ENX_LOG_END(DBG_SDIO_WF_CMD);

	return bRes;
}

SDIO_OKFAIL SdioWfRegRead(BYTE *data, UINT func_num, UINT addr)
{
	SDIO_OKFAIL bRes = SDIO_FAIL;
 	UINT nResp = 0, nArg = 0;

 	ENX_LOG_START(DBG_SDIO_WF_CMD);

	nArg = CMD52_READ | (func_num << 28) | (addr << 9);
	ENX_OKFAIL sdres = SdioCmd(wfinfo.nCH, SDCMD_IO_RW_DIRECT_CMD, nArg, &nResp, ecrtR5);
	if (sdres == ENX_OK) {
		*data = nResp & 0xFF;
		bRes = SDIO_OK;
	} else {
		_Rprintf("Error: CMD52 Read, Resp(0x%08X)\n", nResp);
	}

	ENX_LOG_END(DBG_SDIO_WF_CMD);

	return bRes;
}

SDIO_OKFAIL SdioWfDatWrite(BYTE *data, UINT func_num, UINT addr, UINT size)
{
	SDIO_OKFAIL bRes = SDIO_FAIL;

	ENX_LOG_START(DBG_SDIO_WF_DAT);

	BYTE *buff = data;
	UINT nDBlkSize = SdioGetDataBlockByte(wfinfo.nCH);
	UINT nDBcount = size / nDBlkSize;
	UINT nDBbyte = size % nDBlkSize;

	SdioSetIoFnvalue(wfinfo.nCH, func_num);		// Function Number

	if ((intptr_t)data & 0x3f) {
		_Rprintf("SDIO Write align error: 0x%08X\n", (intptr_t)data);
	}

	if (nDBcount) {
		//_Yprintf("W(Block): 0x%08X %ublk(%ubyte)\n", buff, nDBcount, nDBlkSize * nDBcount);
		SdioSetIoBmMode(wfinfo.nCH, SDIO_BM_BLOCK);		// Block mode
		// nArg = CMD53_WRITE | func->num<<28 | addr<<9 | blocks | CMD53_BMODE;
		ENX_SDIOWF_FLUSH_DCACHE((ULONG)buff, nDBlkSize * nDBcount);
		ENX_OKFAIL sdres = SdioDataIO(wfinfo.nCH, SDIO_DIO_IO_WRITE, (ULONG)buff, addr, nDBcount);
		if (sdres == ENX_OK) {
			bRes = SDIO_OK;
			//hexDump("W(Block)", buff, nDBlkSize * nDBcount);
		}
		buff += (nDBlkSize * nDBcount);
	}

	if (nDBbyte) {
		//_Yprintf("W(Byte): 0x%08X %ubyte\n", buff, nDBbyte);
		SdioSetIoBmMode(wfinfo.nCH, SDIO_BM_BYTE);		// Byte mode
		SdioSetDataBlockByte(wfinfo.nCH, 1);
		ENX_SDIOWF_FLUSH_DCACHE((ULONG)buff, nDBbyte);
		ENX_OKFAIL sdres = SdioDataIO(wfinfo.nCH, SDIO_DIO_IO_WRITE, (ULONG)buff, addr, nDBbyte);
		if (sdres == ENX_OK) {
			bRes = SDIO_OK;
			//hexDump("W(Byte)", buff, nDBbyte);
		}
		SdioSetDataBlockByte(wfinfo.nCH, nDBlkSize);
	}

	ENX_LOG_END(DBG_SDIO_WF_DAT);

	return bRes;
}

SDIO_OKFAIL SdioWfDatRead(BYTE *data, UINT func_num, UINT addr, UINT size)
{
	SDIO_OKFAIL bRes = SDIO_FAIL;

	if ((intptr_t)data & 0x3f) {
		_Rprintf("SDIO Read align error: 0x%08X\n", (intptr_t)data);
	}

	ENX_LOG_START(DBG_SDIO_WF_DAT);

	BYTE *buff = data;
	UINT nDBlkSize = SdioGetDataBlockByte(wfinfo.nCH);
	UINT nDBcount = size / nDBlkSize;
	UINT nDBbyte = size % nDBlkSize;

	SdioSetIoFnvalue(wfinfo.nCH, func_num);		// Function Number

	if (nDBcount) {
		//_Yprintf("R(Block): 0x%08X %ublk(%ubyte)\n", buff, nDBcount, nDBlkSize * nDBcount);
		SdioSetIoBmMode(wfinfo.nCH, SDIO_BM_BLOCK);		// Block mode
		// nArg = CMD53_WRITE | func->num<<28 | addr<<9 | blocks | CMD53_BMODE;
		ENX_SDIOWF_FLUSH_DCACHE((ULONG)buff, nDBlkSize * nDBcount);
		ENX_OKFAIL sdres = SdioDataIO(wfinfo.nCH, SDIO_DIO_IO_READ, (ULONG)buff, addr, nDBcount);
		if (sdres == ENX_OK) {
			bRes = SDIO_OK;
			//hexDump("R(Block)", buff, nDBlkSize * nDBcount);
		}
		buff += (nDBlkSize * nDBcount);
	}

	if (nDBbyte) {
		//_Yprintf("R(Byte): 0x%08X %ubyte\n", buff, nDBbyte);
		SdioSetIoBmMode(wfinfo.nCH, SDIO_BM_BYTE);		// Byte mode
		SdioSetDataBlockByte(wfinfo.nCH, nDBbyte);
		ENX_SDIOWF_FLUSH_DCACHE((ULONG)buff, nDBbyte);
		ENX_OKFAIL sdres = SdioDataIO(wfinfo.nCH, SDIO_DIO_IO_READ, (ULONG)buff, addr, 1);
		if (sdres == ENX_OK) {
			bRes = SDIO_OK;
			//hexDump("R(Byte)", buff, nDBbyte);
		}
		SdioSetDataBlockByte(wfinfo.nCH, nDBlkSize);
	}

	ENX_LOG_END(DBG_SDIO_WF_DAT);

	return bRes;
}

SDIO_OKFAIL SdioWfCmd(BYTE Cmd, UINT Arg, UINT *nResp, eCmdRespType cmdType)
{
	SDIO_OKFAIL bRes = SDIO_FAIL;

	ENX_LOG_START(DBG_SDIO_WF_CMD);

	ENX_OKFAIL sdres = SdioCmd(wfinfo.nCH, Cmd, Arg, nResp, cmdType);
	if (sdres == ENX_OK) {
		bRes = SDIO_OK;
	}

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
	SdioSetDataBlockByte(wfinfo.nCH, 256);

	GpioSetOut(WF_GPIO_RST, GPIO_OUT_HI); // LO:ON HI:OFF
	SdioSetDelayfn(wfinfo.nCH, (user_delay_fn)ENX_SDIOWF_DELAY);

	//SdioIoIrqCallback(wfinfo.nCH, ewl_sdio_ext_irq_handler, NULL);
	//SdioSetIoIrqEn(wfinfo.nCH, ENX_ON);
}

void SdioWfIoIrqCallback(irq_fn irqfn, void *arg)
{
	SdioIoIrqCallback(wfinfo.nCH, irqfn, arg);
}

void SdioWfSetIoIrqEn(ENX_SWITCH sw)
{
	SdioSetIoIrqEn(wfinfo.nCH, sw);
}

void SdioWfIoIrqClear(void)
{
	SdioIoIrqClear(wfinfo.nCH);
}
#endif
#endif
