#include "dev.h"
#include "exbl.h"

#include <stdio.h> // sprintf

#if 0
_regs_ BF_5(
		UINT _rev0 : 28,
		UINT BITMODE : 1 ,
		UINT IOMODE : 1 ,
		UINT MODE : 1 ,
		UINT EN : 1 ) _rege_ _SDIO_REG0;
_regs_ BF_4(
		UINT _rev0 : 17,
		UINT CLK_EN : 1 ,
		UINT CLK_SELECT : 2 ,
		UINT CLK_DIV : 12 ) _rege_ _SDIO_REG1;
_regs_ BF_1(
		UINT CMD_ARG : 32 ) _rege_ _SDIO_REG2;
_regs_ BF_10(
		UINT _rev0 : 1,
		UINT CMD_IDX : 7 ,
		UINT _rev1 : 14,
		UINT CMD_RESP_TOUT : 1 ,
		UINT CMD_RESP_CRCERR : 1 ,
		UINT _rev2 : 4,
		UINT CMD_RESP_BUSY_EN : 1 ,
		UINT CMD_RESP_TYPE : 1 ,
		UINT CMD_RESP_EN : 1 ,
		UINT CMD_EN : 1 ) _rege_ _SDIO_REG3;
_regs_ BF_4(
		UINT _rev0 : 18,
		UINT CMD_RESP_IDX : 6 ,
		UINT _rev1 : 1,
		UINT CMD_RESP_CRC : 7 ) _rege_ _SDIO_REG4;
_regs_ BF_1(
		UINT CMD_RESP_DAT127_96 : 32 ) _rege_ _SDIO_REG5;
_regs_ BF_1(
		UINT CMD_RESP_DAT95_64 : 32 ) _rege_ _SDIO_REG6;
_regs_ BF_1(
		UINT CMD_RESP_DAT63_32 : 32 ) _rege_ _SDIO_REG7;
_regs_ BF_1(
		UINT CMD_RESP_DAT31_0 : 32 ) _rege_ _SDIO_REG8;
_regs_ BF_1(
		UINT CMD_RESP_TLMT : 32 ) _rege_ _SDIO_REG9;
_regs_ BF_1(
		UINT DAT_ADR : 32 ) _rege_ _SDIO_REG10;
_regs_ BF_3(
		UINT _rev0 : 4,
		UINT DAT_BLKBYTE : 12 ,
		UINT DAT_BLKNUM : 16 ) _rege_ _SDIO_REG11;
_regs_ BF_1(
		UINT DAT_BLKADR : 32 ) _rege_ _SDIO_REG12;
_regs_ BF_13(
		UINT DAT_CRCERR : 1 ,
		UINT _rev0 : 20,
		UINT IO_IRQ : 1 ,
		UINT IO_IRQ_CLR : 1 ,
		UINT IO_IRQ_EN : 1 ,
		UINT CMD_IRQ : 1 ,
		UINT CMD_IRQ_CLR : 1 ,
		UINT CMD_IRQ_EN : 1 ,
		UINT DAT_IRQ : 1 ,
		UINT DAT_IRQ_CLR : 1 ,
		UINT DAT_IRQ_EN : 1 ,
		UINT DAT_WE : 1 ,
		UINT DAT_EN : 1 ) _rege_ _SDIO_REG13;
_regs_ BF_2(
		UINT _rev0 : 16,
		UINT DAT_BLKCNT : 16 ) _rege_ _SDIO_REG14;
_regs_ BF_4(
		UINT _rev0 : 18,
		UINT DAT_STOP_CMD : 6 ,
		UINT _rev1 : 2,
		UINT DAT_IORW_CMD : 6 ) _rege_ _SDIO_REG15;
_regs_ BF_8(
		UINT _rev0 : 2,
		UINT DAT_WRCMD_S : 6 ,
		UINT _rev1 : 2,
		UINT DAT_WRCMD_M : 6 ,
		UINT _rev2 : 2,
		UINT DAT_RDCMD_S : 6 ,
		UINT _rev3 : 2,
		UINT DAT_RDCMD_M : 6 ) _rege_ _SDIO_REG16;
#endif

static _SDIO_REG0 *arrSDIOReg0[SDIO_CNT];
static _SDIO_REG1 *arrSDIOReg1[SDIO_CNT];
static _SDIO_REG2 *arrSDIOReg2[SDIO_CNT];
static _SDIO_REG3 *arrSDIOReg3[SDIO_CNT];
static _SDIO_REG4 *arrSDIOReg4[SDIO_CNT];
static _SDIO_REG5 *arrSDIOReg5[SDIO_CNT];
static _SDIO_REG6 *arrSDIOReg6[SDIO_CNT];
static _SDIO_REG7 *arrSDIOReg7[SDIO_CNT];
static _SDIO_REG8 *arrSDIOReg8[SDIO_CNT];
static _SDIO_REG9 *arrSDIOReg9[SDIO_CNT];
static _SDIO_REG10 *arrSDIOReg10[SDIO_CNT];
static _SDIO_REG11 *arrSDIOReg11[SDIO_CNT];
static _SDIO_REG12 *arrSDIOReg12[SDIO_CNT];
static _SDIO_REG13 *arrSDIOReg13[SDIO_CNT];
static _SDIO_REG14 *arrSDIOReg14[SDIO_CNT];
static _SDIO_REG15 *arrSDIOReg15[SDIO_CNT];
static _SDIO_REG16 *arrSDIOReg16[SDIO_CNT];
static tIhnd arrSDIOIrq_Io[SDIO_CNT];
static tIhnd arrSDIOIrq_Cmd[SDIO_CNT];
static tIhnd arrSDIOIrq_Dat[SDIO_CNT];
static user_delay_fn delay[SDIO_CNT] = {WaitXus, WaitXus};
static UINT arrSDIODefSpeed[SDIO_CNT];

void SdioInit(UINT nCH, UINT Speed_Hz)
{
	const uint64_t addrSDIO[SDIO_CNT] = {REG_BASE_SDIO0, REG_BASE_SDIO1};

	arrSDIOReg0[nCH] = (_SDIO_REG0 *)(addrSDIO[nCH] + (0 << 3));
	arrSDIOReg1[nCH] = (_SDIO_REG1 *)(addrSDIO[nCH] + (1 << 3));
	arrSDIOReg2[nCH] = (_SDIO_REG2 *)(addrSDIO[nCH] + (2 << 3));
	arrSDIOReg3[nCH] = (_SDIO_REG3 *)(addrSDIO[nCH] + (3 << 3));
	arrSDIOReg4[nCH] = (_SDIO_REG4 *)(addrSDIO[nCH] + (4 << 3));
	arrSDIOReg5[nCH] = (_SDIO_REG5 *)(addrSDIO[nCH] + (5 << 3));
	arrSDIOReg6[nCH] = (_SDIO_REG6 *)(addrSDIO[nCH] + (6 << 3));
	arrSDIOReg7[nCH] = (_SDIO_REG7 *)(addrSDIO[nCH] + (7 << 3));
	arrSDIOReg8[nCH] = (_SDIO_REG8 *)(addrSDIO[nCH] + (8 << 3));
	arrSDIOReg9[nCH] = (_SDIO_REG9 *)(addrSDIO[nCH] + (9 << 3));
	arrSDIOReg10[nCH] = (_SDIO_REG10 *)(addrSDIO[nCH] + (10 << 3));
	arrSDIOReg11[nCH] = (_SDIO_REG11 *)(addrSDIO[nCH] + (11 << 3));
	arrSDIOReg12[nCH] = (_SDIO_REG12 *)(addrSDIO[nCH] + (12 << 3));
	arrSDIOReg13[nCH] = (_SDIO_REG13 *)(addrSDIO[nCH] + (13 << 3));
	arrSDIOReg14[nCH] = (_SDIO_REG14 *)(addrSDIO[nCH] + (14 << 3));
	arrSDIOReg15[nCH] = (_SDIO_REG15 *)(addrSDIO[nCH] + (15 << 3));
	arrSDIOReg16[nCH] = (_SDIO_REG16 *)(addrSDIO[nCH] + (16 << 3));
#if 0
	arrSDIOReg13[nCH]->IO_IRQ_EN = 0;
	arrSDIOReg13[nCH]->CMD_IRQ_EN = 0;
	arrSDIOReg13[nCH]->DAT_IRQ_EN = 0;
	arrSDIOReg11[nCH]->DAT_BLKBYTE = 0x00000200;
	arrSDIOReg9[nCH]->CMD_RESP_TLMT = 0x00000200;
	arrSDIOReg0[nCH]->BITMODE = 1;
	arrSDIOReg0[nCH]->MODE = 1;
	arrSDIOReg0[nCH]->IOMODE = 0; // 0:card 1:io
	arrSDIOReg1[nCH]->CLK_SELECT = 1;
	arrSDIOReg1[nCH]->CLK_DIV = (MCK_FREQ / (2 * Speed_Hz) - 1);
	arrSDIOReg1[nCH]->CLK_EN = 1;
	arrSDIOReg0[nCH]->EN = 1;
#endif
	arrSDIOIrq_Io[nCH].irqfn = NULL;
	arrSDIOIrq_Io[nCH].arg = NULL;
	arrSDIOIrq_Cmd[nCH].irqfn = NULL;
	arrSDIOIrq_Cmd[nCH].arg = NULL;
	arrSDIOIrq_Dat[nCH].irqfn = NULL;
	arrSDIOIrq_Dat[nCH].arg = NULL;

	arrSDIODefSpeed[nCH] = Speed_Hz;

	printf("SDIO%d Init - %uHz\n", nCH, MCK_FREQ / ((arrSDIOReg1[nCH]->CLK_DIV + 1) * 2));
}

UINT SdioCmd(UINT nCH, BYTE Cmd, UINT Arg, BYTE RespEn, BYTE RespType, BYTE RespBusyEn)
{
	while (arrSDIOReg3[nCH]->CMD_EN) {
		delay[nCH](1);
	}
	flprintf("arrSDIOReg11[%u]->DAT_BLKBYTE => %u, %u\n", nCH, arrSDIOReg11[nCH]->DAT_BLKBYTE, SDIO0_DAT_BLKBYTE);
	//arrSDIOReg3[nCH]->CMD_IDX = Cmd;
	SDIO0_CMD_IDX = Cmd;
	flprintf("arrSDIOReg11[%u]->DAT_BLKBYTE => %u, %u\n", nCH, arrSDIOReg11[nCH]->DAT_BLKBYTE, SDIO0_DAT_BLKBYTE);
	arrSDIOReg2[nCH]->CMD_ARG = Arg;
	arrSDIOReg3[nCH]->CMD_RESP_BUSY_EN = RespBusyEn;
	arrSDIOReg3[nCH]->CMD_RESP_TYPE = RespType;
	arrSDIOReg3[nCH]->CMD_RESP_EN = RespEn;
	arrSDIOReg3[nCH]->CMD_EN = 1;
	while (arrSDIOReg3[nCH]->CMD_EN) {
		//printf(".");
		//if (arrSDIOReg3[nCH]->CMD_RESP_TOUT || arrSDIOReg3[nCH]->CMD_RESP_CRCERR) {
			//printf("1 CMD(%u) Arg(%u) CRCERR(%u:0x%02X), RESPTOUT(%u)\n", Cmd, Arg, arrSDIOReg3[nCH]->CMD_RESP_CRCERR, arrSDIOReg4[nCH]->CMD_RESP_CRC, arrSDIOReg3[nCH]->CMD_RESP_TOUT);
			//break;
		//}
		delay[nCH](1);
	}

	if (Cmd == 2 || Cmd == 9 || Cmd == 10 || Cmd == 41) {
		if (arrSDIOReg3[nCH]->CMD_RESP_TOUT) {
			printf("CMD(%u) Arg(%u) CRCERR(%u:0x%02X), RESPTOUT(%u) 1\n", Cmd, Arg, arrSDIOReg3[nCH]->CMD_RESP_CRCERR, arrSDIOReg4[nCH]->CMD_RESP_CRC, arrSDIOReg3[nCH]->CMD_RESP_TOUT);
			return DEF_FAIL;
		}
	} else {
		if (arrSDIOReg3[nCH]->CMD_RESP_TOUT || arrSDIOReg3[nCH]->CMD_RESP_CRCERR) {
			printf("CMD(%u) Arg(%u) CRCERR(%u:0x%02X), RESPTOUT(%u) 2\n", Cmd, Arg, arrSDIOReg3[nCH]->CMD_RESP_CRCERR, arrSDIOReg4[nCH]->CMD_RESP_CRC, arrSDIOReg3[nCH]->CMD_RESP_TOUT);
			return DEF_FAIL;
		}
	}

	return DEF_OK;
}

void SdioSetDelayfn(UINT nCH, user_delay_fn user_delay)
{
	delay[nCH] = user_delay;
}

void SdioGetResp(UINT nCH, UINT *nResp, eCmdRespType cmdType)
{
	nResp[0] = arrSDIOReg5[nCH]->CMD_RESP_DAT127_96;
	if (cmdType == ecrtR2) {
		nResp[1] = arrSDIOReg6[nCH]->CMD_RESP_DAT95_64;
		nResp[2] = arrSDIOReg7[nCH]->CMD_RESP_DAT63_32;
		nResp[3] = arrSDIOReg8[nCH]->CMD_RESP_DAT31_0;
	}
}

void SdioSetClockDef(UINT nCH)
{
	arrSDIOReg1[nCH]->CLK_DIV = (MCK_FREQ / (2 * arrSDIODefSpeed[nCH]) - 1);
}

void SdioSetClock(UINT nCH, UINT Speed_Hz)
{
	arrSDIOReg1[nCH]->CLK_DIV = (MCK_FREQ / (2 * Speed_Hz) - 1);
}

void SdioSetClockDiv(UINT nCH, UINT nClkDiv)
{
	arrSDIOReg1[nCH]->CLK_DIV = nClkDiv;
}

UINT SdioGetClockDiv(UINT nCH)
{
	return arrSDIOReg1[nCH]->CLK_DIV;
}

void SdioClockEnable(UINT nCH)
{
	arrSDIOReg1[nCH]->CLK_EN = 1;
}

void SdioClockDisable(UINT nCH)
{
	arrSDIOReg1[nCH]->CLK_EN = 0;
}

void SdioClockDivPrint(UINT nCH, char *strBuffer)
{
	sprintf(strBuffer, "%uHz/CLK_DIV:%u", MCK_FREQ / ((arrSDIOReg1[nCH]->CLK_DIV + 1) * 2), arrSDIOReg1[nCH]->CLK_DIV);
}

UINT SdioGetDataBlockByte(UINT nCH)
{
	return arrSDIOReg11[nCH]->DAT_BLKBYTE;
}

void SdioSetDataBlockByte(UINT nCH, UINT BlkByte)
{
	arrSDIOReg11[nCH]->DAT_BLKBYTE = BlkByte;
}

UINT SdioIsDataEn(UINT nCH)
{
	return arrSDIOReg13[nCH]->DAT_EN;
}

UINT SdioDataIO(UINT nCH, eSDIO_DAT_IO_TYPE DatWe, ULONG MemDst, UINT BlkAdr, UINT BlkCnt)
{
	arrSDIOReg10[nCH]->DAT_ADR = MemDst;
	arrSDIOReg11[nCH]->DAT_BLKNUM = BlkCnt;
	arrSDIOReg12[nCH]->DAT_BLKADR = BlkAdr;
	arrSDIOReg13[nCH]->DAT_WE = DatWe;
	arrSDIOReg13[nCH]->DAT_EN = 1;
	uint64_t a = rdcycle();
	while (arrSDIOReg13[nCH]->DAT_EN);
	uint64_t b = rdcycle();
	printf("DAT cycle %lu\n", b - a);

	if (arrSDIOReg3[nCH]->CMD_RESP_TOUT || arrSDIOReg3[nCH]->CMD_RESP_CRCERR || arrSDIOReg13[nCH]->DAT_CRCERR) {
		printf("DRS CMD(%u) Arg(%u) CMDCRC(%u:0x%02X), RESPTOUT(%u), DATCRC(%u)\n", arrSDIOReg16[nCH]->DAT_RDCMD_S, BlkAdr, arrSDIOReg3[nCH]->CMD_RESP_CRCERR, arrSDIOReg4[nCH]->CMD_RESP_CRC, arrSDIOReg3[nCH]->CMD_RESP_TOUT, arrSDIOReg13[nCH]->DAT_CRCERR);
		return DEF_FAIL;
	}

	return DEF_OK;
}

void SdioSetCmdDataWriteS(UINT nCH, UINT nCmd)
{
	arrSDIOReg16[nCH]->DAT_WRCMD_S = nCmd;
}

void SdioSetCmdDataWriteM(UINT nCH, UINT nCmd)
{
	arrSDIOReg16[nCH]->DAT_WRCMD_M = nCmd;
}

void SdioSetCmdDataReadS(UINT nCH, UINT nCmd)
{
	arrSDIOReg16[nCH]->DAT_RDCMD_S = nCmd;
}

void SdioSetCmdDataReadM(UINT nCH, UINT nCmd)
{
	arrSDIOReg16[nCH]->DAT_RDCMD_M = nCmd;
}

void SdioIrqCallback_Io(UINT nCH, irq_fn irqfn, void *arg)
{
	arrSDIOIrq_Io[nCH].irqfn = irqfn;
	arrSDIOIrq_Io[nCH].arg = arg;
}

void SdioIrqCallback_Cmd(UINT nCH, irq_fn irqfn, void *arg)
{
	arrSDIOIrq_Cmd[nCH].irqfn = irqfn;
	arrSDIOIrq_Cmd[nCH].arg = arg;
}

void SdioIrqCallback_Dat(UINT nCH, irq_fn irqfn, void *arg)
{
	arrSDIOIrq_Dat[nCH].irqfn = irqfn;
	arrSDIOIrq_Dat[nCH].arg = arg;
}

void SdioIrqOn_Io(UINT nCH)
{
	arrSDIOReg13[nCH]->IO_IRQ_EN = 1;
}

void SdioIrqOn_Cmd(UINT nCH)
{
	arrSDIOReg13[nCH]->CMD_IRQ_EN = 1;
}

void SdioIrqOn_Dat(UINT nCH)
{
	arrSDIOReg13[nCH]->DAT_IRQ_EN = 1;
}

void SdioIrqOff_Io(UINT nCH)
{
	arrSDIOReg13[nCH]->IO_IRQ_EN = 0;
}

void SdioIrqOff_Cmd(UINT nCH)
{
	arrSDIOReg13[nCH]->CMD_IRQ_EN = 0;
}

void SdioIrqOff_Dat(UINT nCH)
{
	arrSDIOReg13[nCH]->DAT_IRQ_EN = 0;
}

void SdioIrqClear_Io(UINT nCH)
{
	arrSDIOReg13[nCH]->IO_IRQ_CLR = 1;
}

void SdioIrqClear_Cmd(UINT nCH)
{
	arrSDIOReg13[nCH]->CMD_IRQ_CLR = 1;
}

void SdioIrqClear_Dat(UINT nCH)
{
	arrSDIOReg13[nCH]->DAT_IRQ_CLR = 1;
}

UINT SdioIsIrq_Io(UINT nCH)
{
	return arrSDIOReg13[nCH]->IO_IRQ;
}

UINT SdioIsIrq_Cmd(UINT nCH)
{
	return arrSDIOReg13[nCH]->CMD_IRQ;
}

UINT SdioIsIrq_Dat(UINT nCH)
{
	return arrSDIOReg13[nCH]->DAT_IRQ;
}

void IrqSdio(UINT nCH)
{
	if (SdioIsIrq_Io(nCH)) {
		_printf("SDIO%d-IO IRQ Get\n", nCH);
		if (arrSDIOIrq_Io[nCH].irqfn) {
			arrSDIOIrq_Io[nCH].irqfn(arrSDIOIrq_Io[nCH].arg);
		}
		SdioIrqClear_Io(nCH);
	}

	if (SdioIsIrq_Cmd(nCH)) {
		_printf("SDIO%d-CMD IRQ Get\n", nCH);
		if (arrSDIOIrq_Cmd[nCH].irqfn) {
			arrSDIOIrq_Cmd[nCH].irqfn(arrSDIOIrq_Cmd[nCH].arg);
		}
		SdioIrqClear_Cmd(nCH);
	}

	if (SdioIsIrq_Dat(nCH)) {
		_printf("SDIO%d-DAT IRQ Get\n", nCH);
		if (arrSDIOIrq_Dat[nCH].irqfn) {
			arrSDIOIrq_Dat[nCH].irqfn(arrSDIOIrq_Dat[nCH].arg);
		}
		SdioIrqClear_Dat(nCH);
	}
}

void SdioRegShow(UINT nCH)
{
#if 0
	_regs_ BF_5(UINT _rev0 : 28, UINT BITMODE : 1 ,UINT IOMODE : 1 ,UINT MODE : 1 ,UINT EN : 1 ) _rege_ _SDIO_REG0;
	_regs_ BF_4(UINT _rev0 : 17, UINT CLK_EN : 1 ,UINT CLK_SELECT : 2 ,UINT CLK_DIV : 12 ) _rege_ _SDIO_REG1;
	_regs_ BF_1(UINT CMD_ARG : 32 ) _rege_ _SDIO_REG2;
	_regs_ BF_10(UINT _rev0 : 1, UINT CMD_IDX : 7 ,UINT _rev1 : 14, UINT CMD_RESP_TOUT : 1 ,UINT CMD_RESP_CRCERR : 1 ,UINT _rev2 : 4, UINT CMD_RESP_BUSY_EN : 1 ,UINT CMD_RESP_TYPE : 1 ,UINT CMD_RESP_EN : 1 ,UINT CMD_EN : 1 ) _rege_ _SDIO_REG3;
	_regs_ BF_4(UINT _rev0 : 18, UINT CMD_RESP_IDX : 6 ,UINT _rev1 : 1, UINT CMD_RESP_CRC : 7 ) _rege_ _SDIO_REG4;
	_regs_ BF_1(UINT CMD_RESP_DAT127_96 : 32 ) _rege_ _SDIO_REG5;
	_regs_ BF_1(UINT CMD_RESP_DAT95_64 : 32 ) _rege_ _SDIO_REG6;
	_regs_ BF_1(UINT CMD_RESP_DAT63_32 : 32 ) _rege_ _SDIO_REG7;
	_regs_ BF_1(UINT CMD_RESP_DAT31_0 : 32 ) _rege_ _SDIO_REG8;
	_regs_ BF_1(UINT CMD_RESP_TLMT : 32 ) _rege_ _SDIO_REG9;
	_regs_ BF_1(UINT DAT_ADR : 32 ) _rege_ _SDIO_REG10;
	_regs_ BF_3(UINT _rev0 : 4, UINT DAT_BLKBYTE : 12 ,UINT DAT_BLKNUM : 16 ) _rege_ _SDIO_REG11;
	_regs_ BF_1(UINT DAT_BLKADR : 32 ) _rege_ _SDIO_REG12;
	_regs_ BF_13(UINT DAT_CRCERR : 1 ,UINT _rev0 : 20, UINT IO_IRQ : 1 ,UINT IO_IRQ_CLR : 1 ,UINT IO_IRQ_EN : 1 ,UINT CMD_IRQ : 1 ,UINT CMD_IRQ_CLR : 1 ,UINT CMD_IRQ_EN : 1 ,UINT DAT_IRQ : 1 ,UINT DAT_IRQ_CLR : 1 ,UINT DAT_IRQ_EN : 1 ,UINT DAT_WE : 1 ,UINT DAT_EN : 1 ) _rege_ _SDIO_REG13;
	_regs_ BF_2(UINT _rev0 : 16, UINT DAT_BLKCNT : 16 ) _rege_ _SDIO_REG14;
	_regs_ BF_4(UINT _rev0 : 18, UINT DAT_STOP_CMD : 6 ,UINT _rev1 : 2, UINT DAT_IORW_CMD : 6 ) _rege_ _SDIO_REG15;
	_regs_ BF_8(UINT _rev0 : 2, UINT DAT_WRCMD_S : 6 ,UINT _rev1 : 2, UINT DAT_WRCMD_M : 6 ,UINT _rev2 : 2, UINT DAT_RDCMD_S : 6 ,UINT _rev3 : 2, UINT DAT_RDCMD_M : 6 ) _rege_ _SDIO_REG16;
#endif
	printf("SDIO%u 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X\n", nCH, arrSDIOReg0[nCH]->a, arrSDIOReg1[nCH]->a, arrSDIOReg2[nCH]->a, arrSDIOReg3[nCH]->a, arrSDIOReg4[nCH]->a);
	printf("SDIO%u 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X\n", nCH, arrSDIOReg5[nCH]->a, arrSDIOReg6[nCH]->a, arrSDIOReg7[nCH]->a, arrSDIOReg8[nCH]->a, arrSDIOReg9[nCH]->a);
	printf("SDIO%u 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X\n", nCH, arrSDIOReg10[nCH]->a, arrSDIOReg11[nCH]->a, arrSDIOReg12[nCH]->a, arrSDIOReg13[nCH]->a, arrSDIOReg14[nCH]->a);
	printf("SDIO%u 0x%08X 0x%08X\n", nCH, arrSDIOReg15[nCH]->a, arrSDIOReg16[nCH]->a);
}

void SdioRegTest(UINT nCH)
{
	SdioRegShow(nCH);
#if 0
	arrSDIOReg0[nCH]->a = 0;
	arrSDIOReg1[nCH]->a = 0;
	arrSDIOReg2[nCH]->a = 0;
	arrSDIOReg3[nCH]->a = 0;
	arrSDIOReg4[nCH]->a = 0;
	arrSDIOReg5[nCH]->a = 0;
	arrSDIOReg6[nCH]->a = 0;
	arrSDIOReg7[nCH]->a = 0;
	arrSDIOReg8[nCH]->a = 0;
	arrSDIOReg9[nCH]->a = 0;
	arrSDIOReg10[nCH]->a = 0;
	arrSDIOReg11[nCH]->a = 0;
	arrSDIOReg12[nCH]->a = 0;
	arrSDIOReg13[nCH]->a = 0;
	arrSDIOReg14[nCH]->a = 0;
	arrSDIOReg15[nCH]->a = 0;
	arrSDIOReg16[nCH]->a = 0;
#else
	//arrSDIOReg1[nCH]->a = 0;
#endif
	SdioRegShow(nCH);
	//arrSDIOReg0[nCH]->EN = 1;
	SDIO0_EN = 1;
	SdioRegShow(nCH);
	//arrSDIOReg1[nCH]->CLK_EN = 1;
	SDIO0_CLK_EN = 1;
	//arrSDIOReg13[nCH]->IO_IRQ_CLR = 1;
	SDIO0_IO_IRQ_CLR = 1;
	//arrSDIOReg1[nCH]->CLK_SELECT = 0;
	SDIO0_CLK_SELECT = 3;
	printf("CLK_SEL(%d)\n", arrSDIOReg1[nCH]->CLK_SELECT);
	arrSDIOReg13[nCH]->IO_IRQ_CLR = 1;
	arrSDIOReg1[nCH]->CLK_SELECT = 3;
	printf("CLK_SEL(%d)\n", arrSDIOReg1[nCH]->CLK_SELECT);
	arrSDIOReg13[nCH]->IO_IRQ_CLR = 1;
	SdioRegShow(nCH);
	arrSDIOReg13[nCH]->IO_IRQ_CLR = 1;
	SdioRegShow(nCH);
	arrSDIOReg0[nCH]->BITMODE = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg0[nCH]->IOMODE = 0;
	SdioRegShow(nCH);
	arrSDIOReg0[nCH]->MODE = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg1[nCH]->CLK_SELECT = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg1[nCH]->CLK_DIV = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg2[nCH]->CMD_ARG = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg3[nCH]->CMD_IDX = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg3[nCH]->CMD_RESP_TOUT = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg3[nCH]->CMD_RESP_CRCERR = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg3[nCH]->CMD_RESP_BUSY_EN = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg3[nCH]->CMD_RESP_TYPE = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg3[nCH]->CMD_RESP_EN = 0xffffffff;
	SdioRegShow(nCH);
	//arrSDIOReg3[nCH]->CMD_EN = 0xffffffff;
	//SdioRegShow(nCH);
	arrSDIOReg4[nCH]->CMD_RESP_IDX = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg4[nCH]->CMD_RESP_CRC = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg5[nCH]->CMD_RESP_DAT127_96 = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg6[nCH]->CMD_RESP_DAT95_64 = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg7[nCH]->CMD_RESP_DAT63_32 = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg8[nCH]->CMD_RESP_DAT31_0 = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg9[nCH]->CMD_RESP_TLMT = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg10[nCH]->DAT_ADR = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg11[nCH]->DAT_BLKBYTE = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg11[nCH]->DAT_BLKNUM = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg12[nCH]->DAT_BLKADR = 0xffffffff;
	SdioRegShow(nCH);
	//arrSDIOReg13[nCH]->DAT_CRCERR = 0xffffffff;
	//SdioRegShow(nCH);
	//arrSDIOReg13[nCH]->IO_IRQ = 0xffffffff;
	//SdioRegShow(nCH);
	arrSDIOReg13[nCH]->IO_IRQ_CLR = 1;
	SdioRegShow(nCH);
	arrSDIOReg13[nCH]->IO_IRQ_EN = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg13[nCH]->CMD_IRQ = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg13[nCH]->CMD_IRQ_CLR = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg13[nCH]->CMD_IRQ_EN = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg13[nCH]->DAT_IRQ = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg13[nCH]->DAT_IRQ_CLR = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg13[nCH]->DAT_IRQ_EN = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg13[nCH]->DAT_WE = 0xffffffff;
	SdioRegShow(nCH);
	//arrSDIOReg13[nCH]->DAT_EN = 0xffffffff;
	//SdioRegShow(nCH);
	arrSDIOReg14[nCH]->DAT_BLKCNT = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg15[nCH]->DAT_STOP_CMD = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg15[nCH]->DAT_IORW_CMD = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg16[nCH]->DAT_WRCMD_S = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg16[nCH]->DAT_WRCMD_M = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg16[nCH]->DAT_RDCMD_S = 0xffffffff;
	SdioRegShow(nCH);
	arrSDIOReg16[nCH]->DAT_RDCMD_M = 0xffffffff;
	SdioRegShow(nCH);
}

#if 0

void sdcd0_init()
{
	SDIO0_CMD_RESP_TLMT = 0x00000200;
	SDIO0_DAT_IRQ_EN = 1;
	SDIO0_BITMODE 	= 1;
	SDIO0_MODE 		= 1;
	SDIO0_IOMODE 	= 0;
	SDIO0_CLK_SELECT= 0;
	SDIO0_CLK_EN	= 1;
	SDIO0_EN		= 1;
}
void sdcd1_init()
{
	SDIO1_CMD_RESP_TLMT = 0x00000200;
	SDIO1_DAT_IRQ_EN = 1;
	SDIO1_BITMODE 	= 1;
	SDIO1_MODE 		= 1;
	SDIO1_IOMODE 	= 0;
	SDIO1_CLK_SELECT= 0;
	SDIO1_CLK_EN	= 1;
	SDIO1_EN		= 1;
}

void sdcd0_test()
{
	sdcd0_init();
	sdcd0_send_cmd(0, 0, 0, 0, 0);
	sdcd0_send_cmd(16, 0x200, 0, 0, 1);
	sdcd0_dat_write(RAM_BASE, 0x00000000, 10, 0x200);
	sdcd0_dat_read (RAM_BASE, 0x00000000, 10, 0x200);
}

void sdcd1_test()
{
	sdcd1_init();
	sdcd1_send_cmd(0, 0, 0, 0, 0);
	sdcd1_send_cmd(16, 0x200, 0, 0, 1);
	sdcd1_dat_write(RAM_BASE, 0x00000000, 10, 0x200);
	sdcd1_dat_read (RAM_BASE, 0x00000000, 10, 0x200);
}

void sdcd0_dat_write(UINT MemDst, UINT BlkAdr, UINT BlkCnt, UINT BlkLen)
{
	SDIO0_DAT_ADR		=	MemDst;
	SDIO0_DAT_BLKBYTE 	=	BlkLen;
	SDIO0_DAT_BLKNUM 	=	BlkCnt;
	SDIO0_DAT_BLKADR	=	BlkAdr;
	SDIO0_DAT_WE		=	1;
	SDIO0_DAT_EN		=	1;
	while(SDIO0_DAT_EN);
}

void sdcd0_dat_read(UINT MemSrc, UINT BlkAdr, UINT BlkCnt, UINT BlkLen)
{
	SDIO0_DAT_ADR		=	MemSrc;
	SDIO0_DAT_BLKBYTE 	=	BlkLen;
	SDIO0_DAT_BLKNUM 	=	BlkCnt;
	SDIO0_DAT_BLKADR	=	BlkAdr;
	SDIO0_DAT_WE		=	0;
	SDIO0_DAT_EN		=	1;
	while(SDIO0_DAT_EN);
}

void sdcd0_send_cmd(BYTE Idx, UINT Arg, BYTE RespBusyEn, BYTE RespType, BYTE RespEn)
{
	SDIO0_CMD_IDX			=	Idx;
	SDIO0_CMD_ARG			=	Arg;
	SDIO0_CMD_RESP_BUSY_EN	=	RespBusyEn;
	SDIO0_CMD_RESP_TYPE		=	RespType;
	SDIO0_CMD_RESP_EN		=	RespEn;
	SDIO0_CMD_EN			=	1;
	while(SDIO0_CMD_EN);
	if(SDIO0_CMD_RESP_TOUT|SDIO0_CMD_RESP_CRCERR);
}

void sdcd1_dat_write(UINT MemDst, UINT BlkAdr, UINT BlkCnt, UINT BlkLen)
{
	SDIO1_DAT_ADR		=	MemDst;
	SDIO1_DAT_BLKBYTE 	=	BlkLen;
	SDIO1_DAT_BLKNUM 	=	BlkCnt;
	SDIO1_DAT_BLKADR	=	BlkAdr;
	SDIO1_DAT_WE		=	1;
	SDIO1_DAT_EN		=	1;
	while(SDIO1_DAT_EN);
}

void sdcd1_dat_read(UINT MemSrc, UINT BlkAdr, UINT BlkCnt, UINT BlkLen)
{
	SDIO1_DAT_ADR		=	MemSrc;
	SDIO1_DAT_BLKBYTE 	=	BlkLen;
	SDIO1_DAT_BLKNUM 	=	BlkCnt;
	SDIO1_DAT_BLKADR	=	BlkAdr;
	SDIO1_DAT_WE		=	0;
	SDIO1_DAT_EN		=	1;
	while(SDIO1_DAT_EN);
}

void sdcd1_send_cmd(BYTE Idx, UINT Arg, BYTE RespBusyEn, BYTE RespType, BYTE RespEn)
{
	SDIO1_CMD_IDX			=	Idx;
	SDIO1_CMD_ARG			=	Arg;
	SDIO1_CMD_RESP_BUSY_EN	=	RespBusyEn;
	SDIO1_CMD_RESP_TYPE		=	RespType;
	SDIO1_CMD_RESP_EN		=	RespEn;
	SDIO1_CMD_EN			=	1;
	while(SDIO1_CMD_EN);
	if(SDIO1_CMD_RESP_TOUT|SDIO1_CMD_RESP_CRCERR);
}
#endif
