#include "dev.h"
#include "sdio_cd.h"
#include "enx_freertos.h"

#include <string.h> // memset

static SDIO_SD sdinfo;

#define ENX_SDIOCD_CMD_DEBUG
#define ENX_SDIOCD_DAT_DEBUG
#define ENX_SDIOCD_ERR_DEBUG

#define ENX_SDIOCD_DELAY vTaskDelay
#define DELAY_MS(ms) ENX_SDIOCD_DELAY(ms)

#ifdef ENX_SDIOCD_CMD_DEBUG
	#define gprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
	#define SDIO_CMD_LOG_START	{	printf("(%04d)%s : START\r\n", __LINE__, __func__);	}
	#define SDIO_CMD_LOG_END	{	printf("(%04d)%s : E N D\r\n", __LINE__, __func__);	}
#else
	#define gprintf(fmt, args...) do {} while(0);
	#define SDIO_CMD_LOG_START	{ }
	#define SDIO_CMD_LOG_END	{ }
#endif

#ifdef ENX_SDIOCD_DAT_DEBUG
	#define tprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
	#define SDIO_DAT_LOG_START	{	printf("(%04d)%s : START\r\n", __LINE__, __func__);	}
	#define SDIO_DAT_LOG_END	{	printf("(%04d)%s : E N D\r\n", __LINE__, __func__);	}
#else
	#define tprintf(fmt, args...) do {} while(0);
	#define SDIO_DAT_LOG_START	{ }
	#define SDIO_DAT_LOG_END	{ }
#endif

#ifdef ENX_SDIOCD_ERR_DEBUG
	#define eprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define eprintf(fmt, args...) do {} while(0);
#endif

//-------------------------------------------------------------------------------------------------
// CMD8
#define CMD8_VHS			0x00000100		// [11: 8] : 2.7-3.6V
#define CMD8_PATTERN		0x000000aa		// [ 7: 0] : Just "0xaa"

// ACMD41
#define	ACMD41_HCS			0x40000000		// [   30] : SDHC or SDXC Supported
#define ACMD41_XPC			0x10000000		// [   28] : SDXC Maximum Performance
#define ACMD41_S18R			0x01000000		// [   24] : Switching to 1.8V Request
#define	ACMD41_VOLT			0x00ff8000		// [23:15] : Voltage window OCR


//-------------------------------------------------------------------------------------------------
// SDIO-CD Command

// CMD0, CMD8
UINT SdioCdReset(void)
{
	UINT nResp, nRes;

	nRes = SdioCmd(sdinfo.nCH, 0, 0x00000000, 0, 0, 0);
	gprintf("[ 0] res(%d)\r\n", nRes);
	DELAY_MS(1);

	nRes = SdioCmd(sdinfo.nCH, 0, 0x00000000, 0, 0, 0);
	gprintf("[ 0] res(%d)\r\n", nRes);
	DELAY_MS(1);

	nRes = SdioCmd(sdinfo.nCH, 8, CMD8_VHS|CMD8_PATTERN, 1, 0, 0);	//	Send IF Condition
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR7);
	gprintf("[ 8] res(%d) RESP(0x%08X)\r\n", nRes, nResp);

	if (nResp & 0x300) {
		SDIO_CMD_LOG_END;
		return DEF_OK;
	} else {
		eprintf("End(FAIL) : RESP(0x%08X)\r\n", nResp);
		SDIO_CMD_LOG_END;
		return DEF_FAIL;
	}
}

// ACMD41
UINT SdioCdInitialization(void)
{
	return DEF_FAIL;
}

// CMD11
UINT SdioCdSwitchVoltage(void)
{
	return DEF_FAIL;
}

// CMD2, CMD10
UINT SdioCdGetCID(int nType)
{
	return DEF_FAIL;
}

// CMD3
UINT SdioCdGetRCA(void)
{
	return DEF_FAIL;
}

// CMD9
UINT SdioCdGetCSD(void)
{
	return DEF_FAIL;
}

// CMD7, CMD42, ACMD6
UINT SdioCdBusWidthChange(void)
{
	return DEF_FAIL;
}

// ACMD13
UINT SdioCdGetSSR(void)
{
	return DEF_FAIL;
}

// ACMD51
UINT SdioCdGetSCR(void)
{
	return DEF_FAIL;
}

// CMD6
UINT SdioCdSetClock(void)
{
	return DEF_FAIL;
}

// CMD19
UINT SdioCdTuningCommand(void)
{
	return DEF_FAIL;
}

// CMD16
UINT SdioCdSetBlockLength(void)
{
	return DEF_FAIL;
}

////////////////////////////////////////////////////////////////////////////////
// SDIO-CD Process
UINT SdioCdInitProcess(void)
{
	SDIO_CMD_LOG_START;

	// CMD 0, 8
	if (SdioCdReset() == DEF_FAIL) {
		sdinfo.nErrorCode = 1;
		goto done_fail;
	}

	// ACMD 41
	if (SdioCdInitialization() == DEF_FAIL) {
		sdinfo.nErrorCode = 2;
		goto done_fail;
	}

	sdinfo.nVoltageMode = 0;
	if (sdinfo.ocr.VDD18 && sdinfo.ocr.CCS) {
		// CMD 11
		if (SdioCdSwitchVoltage() == DEF_FAIL) {
			sdinfo.nErrorCode = 3;
		} else {
			sdinfo.nVoltageMode = 1;
		}
	}

	// CMD 2
	if (SdioCdGetCID(0) == DEF_FAIL) {
		sdinfo.nErrorCode = 4;
		goto done_fail;
	}

	// CMD 3
	if (SdioCdGetRCA() == DEF_FAIL) {
		sdinfo.nErrorCode = 5;
		goto done_fail;
	}

	// CMD 9
	if (SdioCdGetCSD() == DEF_FAIL) {
		sdinfo.nErrorCode = 6;
		goto done_fail;
	}

	// CMD 10
	if (SdioCdGetCID(1) == DEF_FAIL) {
		sdinfo.nErrorCode = 7;
		goto done_fail;
	}

	// CMD 7, 42, ACMD 6
	if (SdioCdBusWidthChange() == DEF_FAIL) {
		sdinfo.nErrorCode = 8;
		goto done_fail;
	}

	// ACMD 13
	if (SdioCdGetSSR() == DEF_FAIL) {
		sdinfo.nErrorCode = 9;
		goto done_fail;
	}

	// ACMD 51
	if (SdioCdGetSCR() == DEF_FAIL) {
		sdinfo.nErrorCode = 10;
		goto done_fail;
	}

	// CMD 6
	if (SdioCdSetClock() == DEF_FAIL) {
		sdinfo.nErrorCode = 11;
		goto done_fail;
	}

	// CMD 19
	if(sdinfo.nVoltageMode == 1) {	// UHS-1 1.8v
		if (SdioCdTuningCommand() == DEF_FAIL) {
			sdinfo.nErrorCode = 12;
			goto done_fail;
		}
	}

	// CMD 16
	if (SdioCdSetBlockLength() == DEF_FAIL) {
		sdinfo.nErrorCode = 13;
		goto done_fail;
	}

	char strClockPrint[40] = {0};
	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Init OK(%s)\n", sdinfo.nCH, strClockPrint);

	SDIO_CMD_LOG_END;
	return DEF_OK;
done_fail:
	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Init Fail(err:%u)(%s)\n", sdinfo.nCH, sdinfo.nErrorCode, strClockPrint);

	SDIO_CMD_LOG_END;
	return DEF_FAIL;
}

void SdioCdClockDown(void)
{
	UINT Clkdiv = SdioGetClockDiv(sdinfo.nCH);
	SdioSetClockDiv(sdinfo.nCH, Clkdiv + 1);

	char strClockPrint[40] = {0};
	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Change Clock(%s)\n", sdinfo.nCH, strClockPrint);
}

UINT SdioCdDet(void)
{
	return GpioGetPin(SD_GPIO_IRQ);
}

// 기본적인 CH, IRQ, 최초주파수, Pull-up 등을 설정한다.
void SdioCdInit(UINT nCH)
{
	memset(&sdinfo, 0, sizeof(sdinfo));
	sdinfo.nCH = nCH;
	sdinfo.nErrorCode = 0;

	GpioSetHi(SD_GPIO_RST);
	SdioSetDelayfn(sdinfo.nCH, (user_delay_fn)ENX_SDIOCD_DELAY);

//	GpioInDir(SD_GPIO_IRQ);
//	GpioRiseEdge(SD_GPIO_IRQ);
//	GpioIrqCallback(SD_GPIO_IRQ, SdioCdDetectIrq, NULL);
//	GpioIrqOn(SD_GPIO_IRQ);
}
