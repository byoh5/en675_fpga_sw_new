#include "dev.h"

static tIhnd arrSDIOIrq[SDIO_CNT];

void SdioInit(UINT nCH)
{
	arrSDIOIrq[nCH].irqfn = NULL;
	arrSDIOIrq[nCH].arg = NULL;
}

void SdioIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrSDIOIrq[nCH].irqfn = irqfn;
	arrSDIOIrq[nCH].arg = arg;
}

void SdioIrqOn(UINT nCH)
{

}

void SdioIrqOff(UINT nCH)
{

}

void SdioIrqClear(UINT nCH)
{

}

UINT SdioIsIrq(UINT nCH)
{
	return 0;
}

void IrqSdio(UINT nCH)
{
	if (SdioIsIrq(nCH)) {
		_printf("SDIO IRQ Get [%d]\n", nCH);
		if (arrSDIOIrq[nCH].irqfn) {
			arrSDIOIrq[nCH].irqfn(arrSDIOIrq[nCH].arg);
		}
		SdioIrqClear(nCH);
	}
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
