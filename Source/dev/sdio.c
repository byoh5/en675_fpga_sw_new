#include "dev.h"
#include "exbl.h"

#include <stdio.h> // sprintf

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
#if 1
	arrSDIOReg13[nCH]->IO_IRQ_EN = 0;
	arrSDIOReg13[nCH]->CMD_IRQ_EN = 0;
	arrSDIOReg13[nCH]->DAT_IRQ_EN = 0;
	arrSDIOReg11[nCH]->DAT_BLKBYTE = 0x00000200;
	arrSDIOReg9[nCH]->CMD_RESP_TLMT = 0x00000200;
	arrSDIOReg0[nCH]->BITMODE = 0; // 0:1bit, 1:4bit
	arrSDIOReg0[nCH]->MODE = 1; // 0:Slave, 1:Master
	arrSDIOReg0[nCH]->IOMODE = 0; // 0:Card 1:IO
	arrSDIOReg1[nCH]->CLK_SELECT = 1; // 0:MCK+clock output 1:DIV+clock output 2:SDIO_CLK Input 3:Off
	arrSDIOReg1[nCH]->CLK_DIV = (MCK_FREQ / (2 * Speed_Hz) - 1);
	arrSDIOReg1[nCH]->CLK_EN = 1;
	arrSDIOReg0[nCH]->EN = 1;
#else
	SdioRegShow(nCH, DEF_YES);
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

UINT SdioCmd(UINT nCH, BYTE Cmd, UINT Arg, UINT *nResp, eCmdRespType cmdType)
{
	UINT nRes = DEF_OK;

	while (arrSDIOReg3[nCH]->CMD_EN) {
		delay[nCH](1);
	}

	arrSDIOReg3[nCH]->CMD_IDX = Cmd;
	arrSDIOReg2[nCH]->CMD_ARG = Arg;
	arrSDIOReg3[nCH]->CMD_RESP_BUSY_EN = 0;
	arrSDIOReg3[nCH]->CMD_RESP_TYPE = 0;
	switch (cmdType) {
	case ecrtR1b:
		arrSDIOReg3[nCH]->CMD_RESP_BUSY_EN = 1;
		break;
	case ecrtR2:
		arrSDIOReg3[nCH]->CMD_RESP_TYPE = 1;
		break;
//	case ecrtR1:
//	case ecrtR3:
//	case ecrtR4:
//	case ecrtR5:
//	case ecrtR6:
//	case ecrtR7:
	default:
		break;
	}
	if (nResp != NULL) {
		arrSDIOReg3[nCH]->CMD_RESP_EN = 1;
	} else {
		arrSDIOReg3[nCH]->CMD_RESP_EN = 0;
	}
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
			nRes = DEF_FAIL;
		}
	} else {
		if (arrSDIOReg3[nCH]->CMD_RESP_TOUT || arrSDIOReg3[nCH]->CMD_RESP_CRCERR) {
			printf("CMD(%u) Arg(%u) CRCERR(%u:0x%02X), RESPTOUT(%u) 2\n", Cmd, Arg, arrSDIOReg3[nCH]->CMD_RESP_CRCERR, arrSDIOReg4[nCH]->CMD_RESP_CRC, arrSDIOReg3[nCH]->CMD_RESP_TOUT);
			nRes = DEF_FAIL;
		}
	}

	SdioGetResp(nCH, nResp, cmdType);
	printf("[%2u] res(%c) RESP0(0x%08X)\n", Cmd, nRes == DEF_OK ? 'O' : 'X', nResp[0]);
	if (cmdType == ecrtR2) {
		printf("[%2u] res(%c) RESP1(0x%08X)\n", Cmd, nRes == DEF_OK ? 'O' : 'X', nResp[1]);
		printf("[%2u] res(%c) RESP2(0x%08X)\n", Cmd, nRes == DEF_OK ? 'O' : 'X', nResp[2]);
		printf("[%2u] res(%c) RESP3(0x%08X)\n", Cmd, nRes == DEF_OK ? 'O' : 'X', nResp[3]);
	}

	return nRes;
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

UINT SdioGetBitMode(UINT nCH)
{
	return arrSDIOReg0[nCH]->BITMODE;
}

void SdioSet1BitMode(UINT nCH)
{
	arrSDIOReg0[nCH]->BITMODE = 0;
}

void SdioSet4BitMode(UINT nCH)
{
	arrSDIOReg0[nCH]->BITMODE = 1;
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
	//uint64_t a = rdcycle();
	while (arrSDIOReg13[nCH]->DAT_EN);
	//uint64_t b = rdcycle();
	//printf("DAT cycle %lu\n", b - a);

	if (arrSDIOReg3[nCH]->CMD_RESP_TOUT || arrSDIOReg3[nCH]->CMD_RESP_CRCERR || arrSDIOReg13[nCH]->DAT_CRCERR) {
		printf("DRS CMD(%u) Arg(%u) CMDCRC(%u:0x%02X), RESPTOUT(%u), DATCRC(%u)\n", arrSDIOReg16[nCH]->DAT_RDCMD_S, BlkAdr, arrSDIOReg3[nCH]->CMD_RESP_CRCERR, arrSDIOReg4[nCH]->CMD_RESP_CRC, arrSDIOReg3[nCH]->CMD_RESP_TOUT, arrSDIOReg13[nCH]->DAT_CRCERR);
		return DEF_FAIL;
	}

	return DEF_OK;
}

void SdioSetCmdDataS(UINT nCH, eSDIO_DAT_IO_TYPE esditType, UINT nCmd)
{
	switch (esditType) {
	case esditREAD:
		arrSDIOReg16[nCH]->DAT_RDCMD_S = nCmd;
		break;
	case esditWRITE:
		arrSDIOReg16[nCH]->DAT_WRCMD_S = nCmd;
		break;
	}
}

UINT SdioGetCmdDataS(UINT nCH, eSDIO_DAT_IO_TYPE esditType)
{
	switch (esditType) {
	case esditREAD:
		return arrSDIOReg16[nCH]->DAT_RDCMD_S;
	case esditWRITE:
		return arrSDIOReg16[nCH]->DAT_WRCMD_S;
	}
	return 0xFFFFFFFF;
}

void SdioSetCmdDataM(UINT nCH, eSDIO_DAT_IO_TYPE esditType, UINT nCmd)
{
	switch (esditType) {
	case esditREAD:
		arrSDIOReg16[nCH]->DAT_RDCMD_M = nCmd;
		break;
	case esditWRITE:
		arrSDIOReg16[nCH]->DAT_WRCMD_M = nCmd;
		break;
	}
}

UINT SdioGetCmdDataM(UINT nCH, eSDIO_DAT_IO_TYPE esditType)
{
	switch (esditType) {
	case esditREAD:
		return arrSDIOReg16[nCH]->DAT_RDCMD_M;
	case esditWRITE:
		return arrSDIOReg16[nCH]->DAT_WRCMD_M;
	}
	return 0xFFFFFFFF;
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

void SdioRegShow(UINT nCH, UINT isDetail)
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
	printf("SDIO%u Register View\n", nCH);
	printf("========================================\n");
	if (isDetail == DEF_OK) {
		printf(" 0:0x%08X\n", arrSDIOReg0[nCH]->a);
		printf("   %-20s: %u\n", "BITMODE", SDIO0_BITMODE);
		printf("   %-20s: %u\n", "IOMODE", SDIO0_IOMODE);
		printf("   %-20s: %u\n", "MODE", SDIO0_MODE);
		printf("   %-20s: %u\n", "EN", SDIO0_EN);
		printf(" 1:0x%08X\n", arrSDIOReg1[nCH]->a);
		printf("   %-20s: %u\n", "CLK_EN", SDIO0_CLK_EN);
		printf("   %-20s: %u\n", "CLK_SELECT", SDIO0_CLK_SELECT);
		printf("   %-20s: %u\n", "CLK_DIV", SDIO0_CLK_DIV);
		printf(" 2:0x%08X\n", arrSDIOReg2[nCH]->a);
		printf("   %-20s: %u\n", "CMD_ARG", SDIO0_CMD_ARG);
		printf(" 3:0x%08X\n", arrSDIOReg3[nCH]->a);
		printf("   %-20s: %u\n", "CMD_IDX", SDIO0_CMD_IDX);
		printf("   %-20s: %u\n", "CMD_RESP_TOUT", SDIO0_CMD_RESP_TOUT);
		printf("   %-20s: %u\n", "CMD_RESP_CRCERR", SDIO0_CMD_RESP_CRCERR);
		printf("   %-20s: %u\n", "CMD_RESP_BUSY_EN", SDIO0_CMD_RESP_BUSY_EN);
		printf("   %-20s: %u\n", "CMD_RESP_TYPE", SDIO0_CMD_RESP_TYPE);
		printf("   %-20s: %u\n", "CMD_RESP_EN", SDIO0_CMD_RESP_EN);
		printf("   %-20s: %u\n", "CMD_EN", SDIO0_CMD_EN);
		printf(" 4:0x%08X\n", arrSDIOReg4[nCH]->a);
		printf("   %-20s: %u\n", "CMD_RESP_IDX", SDIO0_CMD_RESP_IDX);
		printf("   %-20s: %u\n", "CMD_RESP_CRC", SDIO0_CMD_RESP_CRC);
		printf(" 5:0x%08X\n", arrSDIOReg5[nCH]->a);
		printf("   %-20s: %u, 0x%08X\n", "CMD_RESP_DAT127_96", SDIO0_CMD_RESP_DAT127_96, SDIO0_CMD_RESP_DAT127_96);
		printf(" 6:0x%08X\n", arrSDIOReg6[nCH]->a);
		printf("   %-20s: %u, 0x%08X\n", "CMD_RESP_DAT95_64", SDIO0_CMD_RESP_DAT95_64, SDIO0_CMD_RESP_DAT95_64);
		printf(" 7:0x%08X\n", arrSDIOReg7[nCH]->a);
		printf("   %-20s: %u, 0x%08X\n", "CMD_RESP_DAT63_32", SDIO0_CMD_RESP_DAT63_32, SDIO0_CMD_RESP_DAT63_32);
		printf(" 8:0x%08X\n", arrSDIOReg8[nCH]->a);
		printf("   %-20s: %u, 0x%08X\n", "CMD_RESP_DAT31_0", SDIO0_CMD_RESP_DAT31_0, SDIO0_CMD_RESP_DAT31_0);
		printf(" 9:0x%08X\n", arrSDIOReg9[nCH]->a);
		printf("   %-20s: %u\n", "CMD_RESP_TLMT", SDIO0_CMD_RESP_TLMT);
		printf("10:0x%08X\n", arrSDIOReg10[nCH]->a);
		printf("   %-20s: %u, 0x%08X\n", "DAT_ADR", SDIO0_DAT_ADR, SDIO0_DAT_ADR);
		printf("11:0x%08X\n", arrSDIOReg10[nCH]->a);
		printf("   %-20s: %u\n", "DAT_BLKBYTE", SDIO0_DAT_BLKBYTE);
		printf("   %-20s: %u\n", "DAT_BLKNUM", SDIO0_DAT_BLKNUM);
		printf("12:0x%08X\n", arrSDIOReg12[nCH]->a);
		printf("   %-20s: %u\n", "DAT_BLKADR", SDIO0_DAT_BLKADR);
		printf("13:0x%08X\n", arrSDIOReg13[nCH]->a);
		printf("   %-20s: %u\n", "DAT_CRCERR", SDIO0_DAT_CRCERR);
		printf("   %-20s: %u\n", "IO_IRQ", SDIO0_IO_IRQ);
		printf("   %-20s: %u\n", "IO_IRQ_CLR", SDIO0_IO_IRQ_CLR);
		printf("   %-20s: %u\n", "IO_IRQ_EN", SDIO0_IO_IRQ_EN);
		printf("   %-20s: %u\n", "CMD_IRQ", SDIO0_CMD_IRQ);
		printf("   %-20s: %u\n", "CMD_IRQ_CLR", SDIO0_CMD_IRQ_CLR);
		printf("   %-20s: %u\n", "CMD_IRQ_EN", SDIO0_CMD_IRQ_EN);
		printf("   %-20s: %u\n", "DAT_IRQ", SDIO0_DAT_IRQ);
		printf("   %-20s: %u\n", "DAT_IRQ_CLR", SDIO0_DAT_IRQ_CLR);
		printf("   %-20s: %u\n", "DAT_IRQ_EN", SDIO0_DAT_IRQ_EN);
		printf("   %-20s: %u\n", "CMD53_OP", SDIO0_CMD53_OP);
		printf("   %-20s: %u\n", "DAT_WE", SDIO0_DAT_WE);
		printf("   %-20s: %u\n", "DAT_EN", SDIO0_DAT_EN);
		printf("14:0x%08X\n", arrSDIOReg14[nCH]->a);
		printf("   %-20s: %u\n", "DAT_BLKCNT", SDIO0_DAT_BLKCNT);
		printf("15:0x%08X\n", arrSDIOReg15[nCH]->a);
		printf("   %-20s: %u\n", "DAT_STOP_CMD", SDIO0_DAT_STOP_CMD);
		printf("   %-20s: %u\n", "DAT_IORW_CMD", SDIO0_DAT_IORW_CMD);
		printf("16:0x%08X\n", arrSDIOReg16[nCH]->a);
		printf("   %-20s: %u\n", "DAT_WRCMD_S", SDIO0_DAT_WRCMD_S);
		printf("   %-20s: %u\n", "DAT_WRCMD_M", SDIO0_DAT_WRCMD_M);
		printf("   %-20s: %u\n", "DAT_RDCMD_S", SDIO0_DAT_RDCMD_S);
		printf("   %-20s: %u\n", "DAT_RDCMD_M", SDIO0_DAT_RDCMD_M);
	} else {
		printf("SDIO%u  0:0x%08X  1:0x%08X  2:0x%08X  3:0x%08X  4:0x%08X  5:0x%08X\n", nCH, arrSDIOReg0[nCH]->a, arrSDIOReg1[nCH]->a, arrSDIOReg2[nCH]->a, arrSDIOReg3[nCH]->a, arrSDIOReg4[nCH]->a, arrSDIOReg5[nCH]->a);
		printf("SDIO%u  6:0x%08X  7:0x%08X  8:0x%08X  9:0x%08X 10:0x%08X 11:0x%08X\n", nCH, arrSDIOReg6[nCH]->a, arrSDIOReg7[nCH]->a, arrSDIOReg8[nCH]->a, arrSDIOReg9[nCH]->a, arrSDIOReg10[nCH]->a, arrSDIOReg11[nCH]->a);
		printf("SDIO%u 12:0x%08X 13:0x%08X 14:0x%08X 15:0x%08X 16:0x%08X\n",           nCH, arrSDIOReg12[nCH]->a, arrSDIOReg13[nCH]->a, arrSDIOReg14[nCH]->a, arrSDIOReg15[nCH]->a, arrSDIOReg16[nCH]->a);
	}
	printf("========================================\n");
}

#if 0
void SdioRegTest2(UINT nCH)
{
	printf("OK(%d)\n", nCH);
}
#endif

void SdioRegTest(UINT nCH)
{
#if 0
	//SdioRegShow(nCH);
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
	//arrSDIOReg13[nCH]->a = 0;
#endif

#if 1 // TEST
//	SdioRegShow(nCH, DEF_NO);

//	SdioRegTest2(nCH);
//	printf("SDIO%u IRQ(%u/%u)(%u/%u)(%u/%u)\n", nCH, SDIO0_IO_IRQ, SDIO0_IO_IRQ_EN, SDIO0_DAT_IRQ, SDIO0_DAT_IRQ_EN, SDIO0_CMD_IRQ, SDIO0_CMD_IRQ_EN);
	SDIO0_DAT_IRQ_EN = 0;
	SDIO0_EN = 1;
//	printf("SDIO%u IRQ(%u/%u)(%u/%u)(%u/%u)\n", nCH, SDIO0_IO_IRQ, SDIO0_IO_IRQ_EN, SDIO0_DAT_IRQ, SDIO0_DAT_IRQ_EN, SDIO0_CMD_IRQ, SDIO0_CMD_IRQ_EN);
	SDIO0_CLK_EN = 1;
//	printf("SDIO%u IRQ(%u/%u)(%u/%u)(%u/%u)\n", nCH, SDIO0_IO_IRQ, SDIO0_IO_IRQ_EN, SDIO0_DAT_IRQ, SDIO0_DAT_IRQ_EN, SDIO0_CMD_IRQ, SDIO0_CMD_IRQ_EN);
	SDIO0_CLK_SELECT = 0;
//	printf("SDIO%u IRQ(%u/%u)(%u/%u)(%u/%u)\n", nCH, SDIO0_IO_IRQ, SDIO0_IO_IRQ_EN, SDIO0_DAT_IRQ, SDIO0_DAT_IRQ_EN, SDIO0_CMD_IRQ, SDIO0_CMD_IRQ_EN);
	SDIO0_IO_IRQ_CLR = 1;
//	printf("SDIO%u IRQ(%u/%u)(%u/%u)(%u/%u)\n", nCH, SDIO0_IO_IRQ, SDIO0_IO_IRQ_EN, SDIO0_DAT_IRQ, SDIO0_DAT_IRQ_EN, SDIO0_CMD_IRQ, SDIO0_CMD_IRQ_EN);
	SdioRegTest2(nCH);
#endif
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg0[nCH]->EN = 0xffffffff;
	arrSDIOReg1[nCH]->CLK_EN = 0xffffffff;
	arrSDIOReg1[nCH]->CLK_SELECT = 0xffffffff;
	//SdioRegShow(nCH);
	arrSDIOReg13[nCH]->IO_IRQ_CLR = 0xffffffff;
//	arrSDIOReg13[nCH]->CMD_IRQ_CLR = 0xffffffff;
//	arrSDIOReg13[nCH]->DAT_IRQ_CLR = 0xffffffff;
//	SdioRegShow(nCH);

	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg0[nCH]->BITMODE = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg0[nCH]->IOMODE = 0;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg0[nCH]->MODE = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg1[nCH]->CLK_SELECT = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg1[nCH]->CLK_DIV = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg2[nCH]->CMD_ARG = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg3[nCH]->CMD_IDX = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg3[nCH]->CMD_RESP_TOUT = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg3[nCH]->CMD_RESP_CRCERR = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg3[nCH]->CMD_RESP_BUSY_EN = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg3[nCH]->CMD_RESP_TYPE = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg3[nCH]->CMD_RESP_EN = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	//arrSDIOReg3[nCH]->CMD_EN = 0xffffffff;
	//SdioRegShow(nCH, DEF_NO);
	arrSDIOReg4[nCH]->CMD_RESP_IDX = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg4[nCH]->CMD_RESP_CRC = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg5[nCH]->CMD_RESP_DAT127_96 = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg6[nCH]->CMD_RESP_DAT95_64 = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg7[nCH]->CMD_RESP_DAT63_32 = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg8[nCH]->CMD_RESP_DAT31_0 = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg9[nCH]->CMD_RESP_TLMT = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg10[nCH]->DAT_ADR = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg11[nCH]->DAT_BLKBYTE = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg11[nCH]->DAT_BLKNUM = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg12[nCH]->DAT_BLKADR = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	//arrSDIOReg13[nCH]->DAT_CRCERR = 0xffffffff;
	//SdioRegShow(nCH, DEF_NO);
	arrSDIOReg13[nCH]->IO_IRQ = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	//arrSDIOReg13[nCH]->IO_IRQ_CLR = 0xffffffff;
	//SdioRegShow(nCH, DEF_NO);
	arrSDIOReg13[nCH]->IO_IRQ_EN = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg13[nCH]->CMD_IRQ = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg13[nCH]->CMD_IRQ_CLR = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg13[nCH]->CMD_IRQ_EN = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg13[nCH]->DAT_IRQ = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg13[nCH]->DAT_IRQ_CLR = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg13[nCH]->DAT_IRQ_EN = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg13[nCH]->DAT_WE = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	//arrSDIOReg13[nCH]->DAT_EN = 0xffffffff;
	//SdioRegShow(nCH, DEF_NO);
	arrSDIOReg14[nCH]->DAT_BLKCNT = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg15[nCH]->DAT_STOP_CMD = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg15[nCH]->DAT_IORW_CMD = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg16[nCH]->DAT_WRCMD_S = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg16[nCH]->DAT_WRCMD_M = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg16[nCH]->DAT_RDCMD_S = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
	arrSDIOReg16[nCH]->DAT_RDCMD_M = 0xffffffff;
	SdioRegShow(nCH, DEF_NO);
#endif
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
