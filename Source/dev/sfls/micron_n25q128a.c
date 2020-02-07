#include "dev.h"

#ifdef __SFLS_N25Q128A__
#include "micron_n25q128a.h"

static SFLScontrol sfls_n25q128a = {
	.ior_cmd = N25Q128A_IOR_CMD,
	.ior_adr = N25Q128A_IOR_ADR,
	.ior_dat = N25Q128A_IOR_DAT,
	.iow_cmd = N25Q128A_IOW_CMD,
	.iow_adr = N25Q128A_IOW_ADR,
	.iow_dat = N25Q128A_IOW_DAT,
	.iowe_cmd = N25Q128A_IOWE_CMD,
	.iors_cmd = N25Q128A_IORS_CMD,
	.iors_dat = N25Q128A_IORS_DAT,

	.is_qpi = 0,

	.cmd_read = N25Q128A_CMD_FAST_READ,
	.cmd_page_program = N25Q128A_CMD_PAGE_PROGRAM,
	.cmd_write_enable = N25Q128A_CMD_WRITE_ENABLE,
	.cmd_read_status = N25Q128A_CMD_READ_STATUS,

	.gap = N25Q128A_READ_GAP,
	.rdltc = N25Q128A_RDLTC,
	.size = N25Q128A_SIZE,

	.func_init_1 = SflsN25q128a_Init1,
	.func_init_2 = SflsN25q128a_Init2,
	.func_write_enable = SflsN25q128a_WriteEnable,
	.func_secter_erase = SflsN25q128a_SecterErase,
	.func_32k_erase = SflsN25q128a_32kErase,
	.func_64k_erase = SflsN25q128a_64kErase,
	.func_chip_erase = SflsN25q128a_ChipErase,
	.func_read_status = SflsN25q128a_ReadStatus,
	.func_write_status = SflsN25q128a_WriteStatus,
	.func_read_sfdp = SflsN25q128a_ReadSFDP,
	.func_enter_4b = 0,
	.func_exit_4b = 0,
	.func_is_4b = 0,
	.func_enter_qpi = 0,
	.func_exit_qpi = 0,
	.func_is_qpi = 0,
	.func_enter_qe = 0,
	.func_exit_qe = 0,
	.func_is_qe = 0,
	.func_enter_protection = SflsN25q128a_EnterProtection,
	.func_exit_protection = SflsN25q128a_ExitProtection,
	.func_is_protection = SflsN25q128a_IsProtection,
};

void SflsN25q128a_Init1(void)
{

}

void SflsN25q128a_Init2(void)
{

}

void SflsN25q128a_WriteEnable(void)
{
	SFLS_USR_CMD_MODE	= sfls_n25q128a.iowe_cmd;
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_n25q128a.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsN25q128a_Erase(SFLS_ERASE erase_type, UINT addr)
{
	SFLS_USR_RDLTC		= 1;
	SFLS_USR_CMD_MODE 	= sfls_n25q128a.iow_cmd;
	SFLS_USR_ADR_MODE	= sfls_n25q128a.iow_adr;
	SFLS_USR_RDREG_CMD_MODE = sfls_n25q128a.iors_cmd;
	SFLS_USR_RDREG_DAT_MODE	= sfls_n25q128a.iors_dat;

	if (erase_type == SFLS_E_ALL) {
		SFLS_USR_ADR_EN	= 0;
		SFLS_USR_ADR	= 0;
	} else {
		SFLS_USR_ADR_EN	= 1;
		SFLS_USR_ADR	= addr;
	}
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_ADR_EXT 	= 0;
	SFLS_USR_BUSY_EN 	= 1;
	switch (erase_type) {
	case SFLS_E_ALL:
		SFLS_USR_CMD	= N25Q128A_CMD_CHIP_ERASE;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= N25Q128A_CMD_SECTOR_ERASE;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= N25Q128A_CMD_64KB_ERASE;
		break;
	case SFLS_E_32K:
		ENX_ASSERT(0);
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsN25q128a_SecterErase(UINT addr)
{
	SflsN25q128a_Erase(SFLS_E_4K, addr);
}

void SflsN25q128a_32kErase(UINT addr)
{
	for (int i = 0; i < 8; i++) {
		SflsN25q128a_Erase(SFLS_E_4K, addr + (i * 4096));
		while(SFLS_USR_REQ);
	}
}

void SflsN25q128a_64kErase(UINT addr)
{
	SflsN25q128a_Erase(SFLS_E_64K, addr);
}

void SflsN25q128a_ChipErase(UINT addr)
{
	SflsN25q128a_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsN25q128a_GetControl(void)
{
	return &sfls_n25q128a;
}















void SflsN25q128a_ReadSFDP(BYTE *sfdp_bin)
{
	UINT *data = (UINT *)sfdp_bin;

	SFLS_USR_RDLTC		= 1;
	SFLS_USR_CMD_MODE	= 0;
	SFLS_USR_ADR_MODE	= 0;
	SFLS_USR_DAT_MODE	= 0;
	SFLS_USR_ADR_EN		= 1;
	SFLS_USR_GAP_EN		= 1;
	SFLS_USR_GAP		= 7;
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_LEN		= 3;
	SFLS_USR_ADR_EXT	= 0;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= N25Q128A_CMD_READ_SFDP;
	SFLS_USR_WRDAT		= 0;

	for (UINT i = 0; i < (256 / 4); i++) {
		SFLS_USR_ADR	= i * 4;
		SFLS_USR_REQ	= 1;
		while (SFLS_USR_REQ);
		data[i] = ENX_HTONL(SFLS_USR_RDDAT);
	}
}

BYTE SflsN25q128a_ReadStatus(void)
{
	SFLS_USR_RDLTC		= sfls_n25q128a.rdltc;
	SFLS_USR_CMD_MODE	= sfls_n25q128a.iors_cmd;
	SFLS_USR_DAT_MODE	= sfls_n25q128a.iors_dat;
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_LEN		= 0;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= N25Q128A_CMD_READ_STATUS;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
//	printf("CMD%02X: SFLS-RD:0x%08X(0x%02X), MODE(%d,%d)\n", SFLS_USR_CMD, SFLS_USR_RDDAT, SFLS_USR_RDDAT >> 24, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
	BYTE status = SFLS_USR_RDDAT >> 24;
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (status & 0x1)	printf("- Write In Progress (WIP)\n");
	if (status & 0x2)	printf("- Write Enable Latch (WEL)\n");
	if (status & 0x4)	printf("- Block Protect Bits (BP0) \n");
	if (status & 0x8)	printf("- Block Protect Bits (BP1) \n");
	if (status & 0x10)	printf("- Block Protect Bits (BP2) \n");
	if (status & 0x20)	printf("- Block Protect Bits (BP3) \n");
	if (status & 0x40)	printf("- Top/Bottom Protect Bits\n");
	if (status & 0x80)	printf("- Status Register Write Disable (SRWD)\n");
#endif
	return status;
}

void SflsN25q128a_WriteStatus(BYTE status)
{
	SflsN25q128a_WriteEnable();
	while (SFLS_USR_REQ);

	SFLS_USR_RDLTC		= sfls_n25q128a.rdltc;
	SFLS_USR_CMD_MODE	= sfls_n25q128a.iors_cmd;
	SFLS_USR_DAT_MODE	= sfls_n25q128a.iors_dat;
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_RD_EN		= 0;
	SFLS_USR_WR_EN		= 1;
	SFLS_USR_LEN		= 0;
	SFLS_USR_BUSY_EN	= 1;
	SFLS_USR_CMD		= N25Q128A_CMD_WRITE_STATUS;
	SFLS_USR_WRDAT		= status << 24;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
//	printf("CMD%02X: SFLS-WD:0x%08X(0x%02X), MODE(%d,%d)\n", SFLS_USR_CMD, SFLS_USR_WRDAT, dat, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
}

void SflsN25q128a_EnterProtection(void)
{
	BYTE status = SflsN25q128a_ReadStatus();
	if (!((status & 0x7C) == 0x7C)) {
		status |= 0x7C;
		SflsN25q128a_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter protection\n");
	}
}

void SflsN25q128a_ExitProtection(void)
{
	BYTE status = SflsN25q128a_ReadStatus();
	if (status & 0x7C) {
		status &= ~0x7C;
		SflsN25q128a_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit protection\n");
	}
}

UINT SflsN25q128a_IsProtection(void)
{
	BYTE status = SflsN25q128a_ReadStatus();
	if (status & 0x7C) {
		return 1;
	} else {
		return 0;
	}
}
#endif
