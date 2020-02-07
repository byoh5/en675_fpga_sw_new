#include "dev.h"

#ifdef __SFLS_EN25QH256__
#include "eon_en25qh256.h"

static SFLScontrol sfls_en25qh256 = {
	.ior_cmd = EN25QH256_IOR_CMD,
	.ior_adr = EN25QH256_IOR_ADR,
	.ior_dat = EN25QH256_IOR_DAT,
	.iow_cmd = EN25QH256_IOW_CMD,
	.iow_adr = EN25QH256_IOW_ADR,
	.iow_dat = EN25QH256_IOW_DAT,
	.iowe_cmd = EN25QH256_IOWE_CMD,
	.iors_cmd = EN25QH256_IORS_CMD,
	.iors_dat = EN25QH256_IORS_DAT,

	.is_qpi = 0,

	.cmd_read = EN25QH256_CMD_FAST_READ,
	.cmd_page_program = EN25QH256_CMD_PAGE_PROGRAM,
	.cmd_write_enable = EN25QH256_CMD_WRITE_ENABLE,
	.cmd_read_status = EN25QH256_CMD_READ_STATUS,

	.gap = EN25QH256_READ_GAP,
	.rdltc = EN25QH256_RDLTC,
	.size = EN25QH256_SIZE,

	.func_init_1 = SflsEn25qh256_Init1,
	.func_init_2 = SflsEn25qh256_Init2,
	.func_write_enable = SflsEn25qh256_WriteEnable,
	.func_secter_erase = SflsEn25qh256_SecterErase,
	.func_32k_erase = SflsEn25qh256_32kErase,
	.func_64k_erase = SflsEn25qh256_64kErase,
	.func_chip_erase = SflsEn25qh256_ChipErase,
	.func_read_status = SflsEn25qh256_ReadStatus,
	.func_write_status = SflsEn25qh256_WriteStatus,
	.func_read_sfdp = SflsEn25qh256_ReadSFDP,
#if (EN25QH256_ENABLE_4B==1)
	.func_enter_4b = SflsEn25qh256_Enter4B,
	.func_exit_4b = SflsEn25qh256_Exit4B,
	.func_is_4b = SflsEn25qh256_Is4B,
#else
	.func_enter_4b = 0,
	.func_exit_4b = 0,
	.func_is_4b = 0,
#endif
#if (EN25QH256_ENABLE_QPI==1)
	.func_enter_qpi = SflsEn25qh256_EnterQPI,
	.func_exit_qpi = SflsEn25qh256_ExitQPI,
	.func_is_qpi = SflsEn25qh256_IsQPI,
#else
	.func_enter_qpi = 0,
	.func_exit_qpi = 0,
	.func_is_qpi = 0,
#endif
#if (EN25QH256_ENABLE_QE==1)
	.func_enter_qe = SflsEn25qh256_EnterQE,
	.func_exit_qe = SflsEn25qh256_ExitQE,
	.func_is_qe = SflsEn25qh256_IsQE,
#else
	.func_enter_qe = 0,
	.func_exit_qe = 0,
	.func_is_qe = 0,
#endif
	.func_enter_protection = SflsEn25qh256_EnterProtection,
	.func_exit_protection = SflsEn25qh256_ExitProtection,
	.func_is_protection = SflsEn25qh256_IsProtection,
};

void SflsEn25qh256_Init1(void)
{

}

void SflsEn25qh256_Init2(void)
{

}

void SflsEn25qh256_WriteEnable(void)
{
	if (sfls_en25qh256.is_qpi) {
		SFLS_USR_CMD_MODE = 2;
	} else {
		SFLS_USR_CMD_MODE = 0;
	}
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_en25qh256.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsEn25qh256_Erase(SFLS_ERASE erase_type, UINT addr)
{
	if (sfls_en25qh256.is_qpi) {
		SFLS_USR_CMD_MODE = 2;
		SFLS_USR_ADR_MODE = 2;
		SFLS_USR_RDREG_CMD_MODE = 2;
		SFLS_USR_RDREG_DAT_MODE	= 2;
		SFLS_USR_RDLTC	= sfls_en25qh256.rdltc;
	} else {
		SFLS_USR_CMD_MODE = 0;
		SFLS_USR_ADR_MODE = 0;
		SFLS_USR_RDREG_CMD_MODE = 0;
		SFLS_USR_RDREG_DAT_MODE	= 0;
		SFLS_USR_RDLTC	= 1;
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
	SFLS_USR_ADR_EXT 	= SFLS_BUS_ADR_EXT;
	SFLS_USR_BUSY_EN 	= 1;
	switch (erase_type) {
	case SFLS_E_ALL:
		SFLS_USR_CMD	= EN25QH256_CMD_CHIP_ERASE;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= EN25QH256_CMD_SECTOR_ERASE;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= EN25QH256_CMD_32KB_ERASE;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= EN25QH256_CMD_64KB_ERASE;
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsEn25qh256_SecterErase(UINT addr)
{
	SflsEn25qh256_Erase(SFLS_E_4K, addr);
}

void SflsEn25qh256_32kErase(UINT addr)
{
	SflsEn25qh256_Erase(SFLS_E_32K, addr);
}

void SflsEn25qh256_64kErase(UINT addr)
{
	SflsEn25qh256_Erase(SFLS_E_64K, addr);
}

void SflsEn25qh256_ChipErase(UINT addr)
{
	SflsEn25qh256_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsEn25qh256_GetControl(void)
{
	return &sfls_en25qh256;
}

BYTE SflsEn25qh256_ReadIFR(void)
{
	if (sfls_en25qh256.is_qpi) {
		SFLS_USR_RDLTC	= sfls_en25qh256.rdltc;
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
	SFLS_USR_CMD		= EN25QH256_CMD_READ_IRF;
	SFLS_USR_REQ		= 1;
 	while (SFLS_USR_REQ);

	BYTE rdifr = SFLS_USR_RDDAT >> 24;
	if (rdifr & 0x1)	printf("- Reserved bit\n");
	if (rdifr & 0x2)	printf("- OTP sector is protected\n");
	if (rdifr & 0x4)	printf("- 4byte address mode\n");
	if (rdifr & 0x8)	printf("- Reserved bit\n");
	if (rdifr & 0x10)	printf("- Reserved bit\n");
	if (rdifr & 0x20)	printf("- Indicate program failed\n");
	if (rdifr & 0x40)	printf("- Indicate erase failed\n");
	if (rdifr & 0x80)	printf("- access larger than 128Mb\n");

	return rdifr;
}

void SflsEn25qh256_ReadSFDP(BYTE *sfdp_bin)
{
	UINT *data = (UINT *)sfdp_bin;

	if (sfls_en25qh256.is_qpi) {
		SFLS_USR_RDLTC	= sfls_en25qh256.rdltc;
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
	if (sfls_en25qh256.is_qpi) {
		SFLS_USR_GAP	= sfls_en25qh256.gap - 1;
	} else {
		SFLS_USR_GAP	= 7;
	}
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_LEN		= 3;
	SFLS_USR_ADR_EXT	= SFLS_BUS_ADR_EXT;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= EN25QH256_CMD_READ_SFDP;
	SFLS_USR_WRDAT		= 0;

	for (UINT i = 0; i < (256 / 4); i++) {
		SFLS_USR_ADR	= i * 4;
		SFLS_USR_REQ	= 1;
		while (SFLS_USR_REQ);
		data[i] = ENX_HTONL(SFLS_USR_RDDAT);
	}
}

BYTE SflsEn25qh256_ReadStatus(void)
{
	if (sfls_en25qh256.is_qpi) {
		SFLS_USR_RDLTC	= sfls_en25qh256.rdltc;
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
	SFLS_USR_CMD		= EN25QH256_CMD_READ_STATUS;
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

void SflsEn25qh256_WriteStatus(BYTE status)
{
	SflsEn25qh256_WriteEnable();
	while (SFLS_USR_REQ);

	if (sfls_en25qh256.is_qpi) {
		SFLS_USR_RDLTC	= sfls_en25qh256.rdltc;
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
	SFLS_USR_CMD		= EN25QH256_CMD_WRITE_STATUS;
	SFLS_USR_WRDAT		= status << 24;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
//	printf("CMD%02X: SFLS-WD:0x%08X, MODE(%d,%d)\n", SFLS_USR_CMD, status, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
}

#if (EN25QH256_ENABLE_4B==1)
void SflsEn25qh256_Enter4B(void)
{
	BYTE status = SflsEn25qh256_ReadIFR();
	if (!((status & 0x04) == 0x04)) {
		SflsSingleCommand(EN25QH256_CMD_EN4B, SFLS_BUS_RDREG_CMD_MODE);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter 4B\n");
	}
}

void SflsEn25qh256_Exit4B(void)
{
	BYTE status = SflsEn25qh256_ReadIFR();
	if (status & 0x04) {
		SflsSingleCommand(EN25QH256_CMD_EX4B, SFLS_BUS_RDREG_CMD_MODE);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit 4B\n");
	}
}

UINT SflsEn25qh256_Is4B(void)
{
	BYTE status = SflsEn25qh256_ReadIFR();
	if (status & 0x04) {
		return 1;
	} else {
		return 0;
	}
}
#endif

#if (EN25QH256_ENABLE_QPI==1)
void SflsEn25qh256_EnterQPI(void)
{
	if (sfls_en25qh256.is_qpi == 0) {
		SflsSingleCommand(EN25QH256_CMD_ENTER_QPI, 0);
		sfls_en25qh256.is_qpi = 1;
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QPI\n");
	}
}

void SflsEn25qh256_ExitQPI(void)
{
	if (sfls_en25qh256.is_qpi == 1) {
		SflsSingleCommand(EN25QH256_CMD_EXIT_QPI, 2);
		sfls_en25qh256.is_qpi = 0;
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QPI\n");
	}
}

UINT SflsEn25qh256_IsQPI(void)
{
	return sfls_en25qh256.is_qpi;
}
#endif

#if (EN25QH256_ENABLE_QE==1)
void SflsEn25qh256_EnterQE(void)
{
	BYTE status = SflsEn25qh256_ReadStatus();
	if (!((status & 0x40) == 0x40)) {
		status |= 0x40;
		SflsEn25qh256_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QE\n");
	}
}

void SflsEn25qh256_ExitQE(void)
{
	BYTE status = SflsEn25qh256_ReadStatus();
	if (status & 0x40) {
		status &= ~0x40;
		SflsEn25qh256_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QE\n");
	}
}

UINT SflsEn25qh256_IsQE(void)
{
	BYTE status = SflsEn25qh256_ReadStatus();
	if (status & 0x40) {
		return 1;
	} else {
		return 0;
	}
}
#endif

void SflsEn25qh256_EnterProtection(void)
{
	BYTE status = SflsEn25qh256_ReadStatus();
	if (!((status & 0x3C) == 0x3C)) {
		status |= 0x3C;
		SflsEn25qh256_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter protection\n");
	}
}

void SflsEn25qh256_ExitProtection(void)
{
	BYTE status = SflsEn25qh256_ReadStatus();
	if (status & 0x3C) {
		status &= ~0x3C;
		SflsEn25qh256_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit protection\n");
	}
}

UINT SflsEn25qh256_IsProtection(void)
{
	BYTE status = SflsEn25qh256_ReadStatus();
	if (status & 0x3C) {
		return 1;
	} else {
		return 0;
	}
}

void SflsEn25qh256_Reset(void)
{
	SflsSingleCommand(EN25QH256_CMD_EXIT_QPI, 2);
	WaitXms(1);
	SflsSingleCommand(0x66, 0);
	WaitXms(1);
	SflsSingleCommand(0x99, 0);
}
#endif
