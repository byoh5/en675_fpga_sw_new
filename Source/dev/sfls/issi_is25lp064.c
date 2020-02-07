#include "dev.h"

#ifdef __SFLS_IS25LP064__
#include "issi_is25lp064.h"

static SFLScontrol sfls_is25lp064 = {
	.ior_cmd = IS25LP064_IOR_CMD,
	.ior_adr = IS25LP064_IOR_ADR,
	.ior_dat = IS25LP064_IOR_DAT,
	.iow_cmd = IS25LP064_IOW_CMD,
	.iow_adr = IS25LP064_IOW_ADR,
	.iow_dat = IS25LP064_IOW_DAT,
	.iowe_cmd = IS25LP064_IOWE_CMD,
	.iors_cmd = IS25LP064_IORS_CMD,
	.iors_dat = IS25LP064_IORS_DAT,

	.is_qpi = 0,

	.cmd_read = IS25LP064_CMD_FAST_READ,
	.cmd_page_program = IS25LP064_CMD_PAGE_PROGRAM,
	.cmd_write_enable = IS25LP064_CMD_WRITE_ENABLE,
	.cmd_read_status = IS25LP064_CMD_READ_STATUS,

	.gap = IS25LP064_READ_GAP,
	.rdltc = IS25LP064_RDLTC,
	.size = IS25LP064_SIZE,

	.func_init_1 = SflsIs25lp064_Init1,
	.func_init_2 = SflsIs25lp064_Init2,
	.func_write_enable = SflsIs25lp064_WriteEnable,
	.func_secter_erase = SflsIs25lp064_SecterErase,
	.func_32k_erase = SflsIs25lp064_32kErase,
	.func_64k_erase = SflsIs25lp064_64kErase,
	.func_chip_erase = SflsIs25lp064_ChipErase,
	.func_read_status = SflsIs25lp064_ReadStatus,
	.func_write_status = SflsIs25lp064_WriteStatus,
	.func_read_sfdp = SflsIs25lp064_ReadSFDP,
	.func_enter_4b = 0,
	.func_exit_4b = 0,
	.func_is_4b = 0,
#if (IS25LP064_ENABLE_QPI==1)
	.func_enter_qpi = SflsIs25lp064_EnterQPI,
	.func_exit_qpi = SflsIs25lp064_ExitQPI,
	.func_is_qpi = SflsIs25lp064_IsQPI,
#else
	.func_enter_qpi = 0,
	.func_exit_qpi = 0,
	.func_is_qpi = 0,
#endif
#if (IS25LP064_ENABLE_QE==1)
	.func_enter_qe = SflsIs25lp064_EnterQE,
	.func_exit_qe = SflsIs25lp064_ExitQE,
	.func_is_qe = SflsIs25lp064_IsQE,
#else
	.func_enter_qe = 0,
	.func_exit_qe = 0,
	.func_is_qe = 0,
#endif
	.func_enter_protection = SflsIs25lp064_EnterProtection,
	.func_exit_protection = SflsIs25lp064_ExitProtection,
	.func_is_protection = SflsIs25lp064_IsProtection,
};

void SflsIs25lp064_Init1(void)
{

}

void SflsIs25lp064_Init2(void)
{

}

void SflsIs25lp064_WriteEnable(void)
{
	if (sfls_is25lp064.is_qpi) {
		SFLS_USR_CMD_MODE = 2;
	} else {
		SFLS_USR_CMD_MODE = 0;
	}
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_is25lp064.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsIs25lp064_Erase(SFLS_ERASE erase_type, UINT addr)
{
	if (sfls_is25lp064.is_qpi) {
		SFLS_USR_RDLTC	= SFLS_IO_RDLTC;
		SFLS_USR_CMD_MODE = 2;
		SFLS_USR_ADR_MODE = 2;
		SFLS_USR_RDREG_CMD_MODE = 2;
		SFLS_USR_RDREG_DAT_MODE	= 2;
	} else {
		SFLS_USR_RDLTC	= 1;
		SFLS_USR_CMD_MODE = 0;
		SFLS_USR_ADR_MODE = 0;
		SFLS_USR_RDREG_CMD_MODE = 0;
		SFLS_USR_RDREG_DAT_MODE	= 0;
	}

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
		SFLS_USR_CMD	= IS25LP064_CMD_CHIP_ERASE;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= IS25LP064_CMD_SECTOR_ERASE;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= IS25LP064_CMD_32KB_ERASE;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= IS25LP064_CMD_64KB_ERASE;
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsIs25lp064_SecterErase(UINT addr)
{
	SflsIs25lp064_Erase(SFLS_E_4K, addr);
}

void SflsIs25lp064_32kErase(UINT addr)
{
	SflsIs25lp064_Erase(SFLS_E_32K, addr);
}

void SflsIs25lp064_64kErase(UINT addr)
{
	SflsIs25lp064_Erase(SFLS_E_64K, addr);
}

void SflsIs25lp064_ChipErase(UINT addr)
{
	SflsIs25lp064_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsIs25lp064_GetControl(void)
{
	return &sfls_is25lp064;
}

void SflsIs25lp064_ReadSFDP(BYTE *sfdp_bin)
{
	UINT *data = (UINT *)sfdp_bin;

	if (sfls_is25lp064.is_qpi) {
		SFLS_USR_RDLTC	= SFLS_IO_RDLTC;
		SFLS_USR_CMD_MODE = 2;
		SFLS_USR_ADR_MODE = 2;
		SFLS_USR_DAT_MODE = 2;
	} else {
		SFLS_USR_RDLTC	= 1;
		SFLS_USR_CMD_MODE = 0;
		SFLS_USR_ADR_MODE = 0;
		SFLS_USR_DAT_MODE = 0;
	}
	SFLS_USR_ADR_EN		= 1;
	SFLS_USR_GAP_EN		= 1;
	if (sfls_is25lp064.is_qpi) {
		SFLS_USR_GAP	= SFLS_BUS_GAP;
	} else {
		SFLS_USR_GAP	= 7;
	}
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_LEN		= 3;
	SFLS_USR_ADR_EXT	= 0;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= IS25LP064_CMD_READ_SFDP;
	SFLS_USR_WRDAT		= 0;

	for (UINT i = 0; i < (064 / 4); i++) {
		SFLS_USR_ADR	= i * 4;
		SFLS_USR_REQ	= 1;
		while (SFLS_USR_REQ);
		data[i] = ENX_HTONL(SFLS_USR_RDDAT);
	}
}

BYTE SflsIs25lp064_ReadStatus(void)
{
	if (sfls_is25lp064.is_qpi) {
		SFLS_USR_RDLTC	= SFLS_IO_RDLTC;
		SFLS_USR_CMD_MODE = 2;
		SFLS_USR_DAT_MODE = 2;
	} else {
		SFLS_USR_RDLTC	= 1;
		SFLS_USR_CMD_MODE = 0;
		SFLS_USR_DAT_MODE = 0;
	}
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_LEN		= 0;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= IS25LP064_CMD_READ_STATUS;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
//	printf("CMD%02X: SFLS-RD:0x%08X, MODE(%d,%d)\n", SFLS_USR_CMD, SFLS_USR_RDDAT, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
	BYTE status = SFLS_USR_RDDAT >> 24;
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (status & 0x1)	printf("- Write In Progress (WIP)\n");
	if (status & 0x2)	printf("- Write Enable Latch (WEL)\n");
	if (status & 0x4)	printf("- Block Protect Bits (BP0) \n");
	if (status & 0x8)	printf("- Block Protect Bits (BP1) \n");
	if (status & 0x10)	printf("- Block Protect Bits (BP2) \n");
	if (status & 0x20)	printf("- Block Protect Bits (BP3) \n");
	if (status & 0x40)	printf("- Quad Enable (QE)\n");
	if (status & 0x80)	printf("- Status Register Write Disable (SRWD)\n");
#endif
	return status;
}

void SflsIs25lp064_WriteStatus(BYTE status)
{
	SflsIs25lp064_WriteEnable();
	while (SFLS_USR_REQ);

	if (sfls_is25lp064.is_qpi) {
		SFLS_USR_RDLTC	= SFLS_IO_RDLTC;
		SFLS_USR_CMD_MODE = 2;
		SFLS_USR_DAT_MODE = 2;
	} else {
		SFLS_USR_RDLTC	= 1;
		SFLS_USR_CMD_MODE = 0;
		SFLS_USR_DAT_MODE = 0;
	}
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_RD_EN		= 0;
	SFLS_USR_WR_EN		= 1;
	SFLS_USR_LEN		= 0;
	SFLS_USR_BUSY_EN	= 1;
	SFLS_USR_CMD		= IS25LP064_CMD_WRITE_STATUS;
	SFLS_USR_WRDAT		= status << 24;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
//	printf("CMD%02X: SFLS-WD:0x%08X, MODE(%d,%d)\n", SFLS_USR_CMD, status, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
}

#if (IS25LP064_ENABLE_QPI==1)
void SflsIs25lp064_EnterQPI(void)
{
	if (sfls_is25lp064.is_qpi == 0) {
		SflsSingleCommand(IS25LP064_CMD_ENTER_QPI, 0);
		sfls_is25lp064.is_qpi = 1;
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QPI\n");
	}
}

void SflsIs25lp064_ExitQPI(void)
{
	if (sfls_is25lp064.is_qpi == 1) {
		SflsSingleCommand(IS25LP064_CMD_EXIT_QPI, 2);
		sfls_is25lp064.is_qpi = 0;
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QPI\n");
	}
}

UINT SflsIs25lp064_IsQPI(void)
{
	return sfls_is25lp064.is_qpi;
}
#endif

#if (IS25LP064_ENABLE_QE==1)
void SflsIs25lp064_EnterQE(void)
{
	BYTE status = SflsIs25lp064_ReadStatus();
	if (!((status & 0x40) == 0x40)) {
		status |= 0x40;
		SflsIs25lp064_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QE\n");
	}
}

void SflsIs25lp064_ExitQE(void)
{
	BYTE status = SflsIs25lp064_ReadStatus();
	if (status & 0x40) {
		status &= ~0x40;
		SflsIs25lp064_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QE\n");
	}
}

UINT SflsIs25lp064_IsQE(void)
{
	BYTE status = SflsIs25lp064_ReadStatus();
	if (status & 0x40) {
		return 1;
	} else {
		return 0;
	}
}
#endif

void SflsIs25lp064_EnterProtection(void)
{
	BYTE status = SflsIs25lp064_ReadStatus();
	if (!((status & 0x3C) == 0x3C)) {
		status |= 0x3C;
		SflsIs25lp064_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter protection\n");
	}
}

void SflsIs25lp064_ExitProtection(void)
{
	BYTE status = SflsIs25lp064_ReadStatus();
	if (status & 0x3C) {
		status &= ~0x3C;
		SflsIs25lp064_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit protection\n");
	}
}

UINT SflsIs25lp064_IsProtection(void)
{
	BYTE status = SflsIs25lp064_ReadStatus();
	if (status & 0x3C) {
		return 1;
	} else {
		return 0;
	}
}
#endif
