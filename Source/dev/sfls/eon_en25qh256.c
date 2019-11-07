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

#if (EN25QH256_IOMODE==SFLS_SINGLE)
	.cmd_read = EN25QH256_CMD_READ,
#elif (EN25QH256_IOMODE==SFLS_DUAL)
	.cmd_read = EN25QH256_CMD_DUAL_READ,
#elif (EN25QH256_IOMODE==SFLS_QUAD)
	.cmd_read = EN25QH256_CMD_QUAD_READ,
#endif
	.cmd_page_program = EN25QH256_CMD_PAGE_PROGRAM,
	.cmd_write_enable = EN25QH256_CMD_WRITE_ENABLE,
	.cmd_read_status = EN25QH256_CMD_READ_STATUS,
#if (EN25QH256_ENABLE_QPI==1)
	.cmd_enter_qpi = EN25QH256_CMD_ENTER_QPI,
#else
	.cmd_enter_qpi = 0,
#endif
	.cmd_sector_erase = EN25QH256_CMD_SECTOR_ERASE,
	.cmd_32k_erase = EN25QH256_CMD_32KB_ERASE,
	.cmd_64k_erase = EN25QH256_CMD_64KB_ERASE,
	.cmd_chip_erase = EN25QH256_CMD_CHIP_ERASE,

	.gap = EN25QH256_READ_GAP - 1,
	.rdltc = EN25QH256_RDLTC,

	.func_write_enable = SflsEn25qh256_WriteEnable,
	.func_secter_erase = SflsEn25qh256_SecterErase,
	.func_32k_erase = SflsEn25qh256_32kErase,
	.func_64k_erase = SflsEn25qh256_64kErase,
	.func_chip_erase = SflsEn25qh256_ChipErase,
};

void SflsEn25qh256_WriteEnable(void)
{
	SFLS_USR_CMD_MODE	= sfls_en25qh256.iow_cmd;
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
	SFLS_USR_CMD_MODE 	= sfls_en25qh256.iow_cmd;
	if (erase_type == SFLS_E_ALL) {
		SFLS_USR_ADR_MODE= 0;
		SFLS_USR_ADR_EN	= 0;
		SFLS_USR_ADR	= 0;
	} else {
		SFLS_USR_ADR_MODE= sfls_en25qh256.iow_adr;
		SFLS_USR_ADR_EN	= 1;
		SFLS_USR_ADR	= addr;
	}
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_ADR_EXT 	= 0;
	SFLS_USR_BUSY_EN 	= 1;
	SFLS_USR_RDREG_CMD_MODE = sfls_en25qh256.iors_cmd;
	SFLS_USR_RDREG_DAT_MODE	= sfls_en25qh256.iors_dat;
	switch (erase_type) {
	case SFLS_E_ALL:
		SFLS_USR_CMD	= sfls_en25qh256.cmd_chip_erase;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= sfls_en25qh256.cmd_sector_erase;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= sfls_en25qh256.cmd_32k_erase;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= sfls_en25qh256.cmd_64k_erase;
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
#endif
