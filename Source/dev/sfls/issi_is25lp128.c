#include "dev.h"

#ifdef __SFLS_IS25LP128__
#include "issi_is25lp128.h"

static SFLScontrol sfls_is25lp128 = {
	.ior_cmd = IS25LP128_IOR_CMD,
	.ior_adr = IS25LP128_IOR_ADR,
	.ior_dat = IS25LP128_IOR_DAT,
	.iow_cmd = IS25LP128_IOW_CMD,
	.iow_adr = IS25LP128_IOW_ADR,
	.iow_dat = IS25LP128_IOW_DAT,
	.iowe_cmd = IS25LP128_IOWE_CMD,
	.iors_cmd = IS25LP128_IORS_CMD,
	.iors_dat = IS25LP128_IORS_DAT,

#if (IS25LP128_IOMODE==SFLS_SINGLE)
	.cmd_read = IS25LP128_CMD_READ,
#elif (IS25LP128_IOMODE==SFLS_DUAL)
	.cmd_read = IS25LP128_CMD_DUAL_READ,
#elif (IS25LP128_IOMODE==SFLS_QUAD)
	.cmd_read = IS25LP128_CMD_QUAD_READ,
#endif
	.cmd_page_program = IS25LP128_CMD_PAGE_PROGRAM,
	.cmd_write_enable = IS25LP128_CMD_WRITE_ENABLE,
	.cmd_read_status = IS25LP128_CMD_READ_STATUS,
#if (IS25LP128_ENABLE_QPI==1)
	.cmd_enter_qpi = IS25LP128_CMD_ENTER_QPI,
#else
	.cmd_enter_qpi = 0,
#endif
	.cmd_enter_4b = 0,
	.cmd_exit_4b = 0,
	.cmd_sector_erase = IS25LP128_CMD_SECTOR_ERASE,
	.cmd_32k_erase = IS25LP128_CMD_32KB_ERASE,
	.cmd_64k_erase = IS25LP128_CMD_64KB_ERASE,
	.cmd_chip_erase = IS25LP128_CMD_CHIP_ERASE,

	.gap = IS25LP128_READ_GAP - 1,
	.rdltc = IS25LP128_RDLTC,

	.func_init = SflsIs25lp128_Init,
	.func_write_enable = SflsIs25lp128_WriteEnable,
	.func_secter_erase = SflsIs25lp128_SecterErase,
	.func_32k_erase = SflsIs25lp128_32kErase,
	.func_64k_erase = SflsIs25lp128_64kErase,
	.func_chip_erase = SflsIs25lp128_ChipErase,
};

void SflsIs25lp128_Init(void)
{
#if (IS25LP128_IOMODE==SFLS_DUAL)

#elif (IS25LP128_IOMODE==SFLS_QUAD)

#endif
}

void SflsIs25lp128_WriteEnable(void)
{
	SFLS_USR_CMD_MODE	= sfls_is25lp128.iow_cmd;
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_is25lp128.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsIs25lp128_Erase(SFLS_ERASE erase_type, UINT addr)
{
	SFLS_USR_CMD_MODE 	= sfls_is25lp128.iow_cmd;
	if (erase_type == SFLS_E_ALL) {
		SFLS_USR_ADR_MODE= 0;
		SFLS_USR_ADR_EN	= 0;
		SFLS_USR_ADR	= 0;
	} else {
		SFLS_USR_ADR_MODE= sfls_is25lp128.iow_adr;
		SFLS_USR_ADR_EN	= 1;
		SFLS_USR_ADR	= addr;
	}
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_ADR_EXT 	= 0;
	SFLS_USR_BUSY_EN 	= 1;
	SFLS_USR_RDREG_CMD_MODE = sfls_is25lp128.iors_cmd;
	SFLS_USR_RDREG_DAT_MODE	= sfls_is25lp128.iors_dat;
	switch (erase_type) {
	case SFLS_E_ALL:
		SFLS_USR_CMD	= sfls_is25lp128.cmd_chip_erase;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= sfls_is25lp128.cmd_sector_erase;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= sfls_is25lp128.cmd_32k_erase;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= sfls_is25lp128.cmd_64k_erase;
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsIs25lp128_SecterErase(UINT addr)
{
	SflsIs25lp128_Erase(SFLS_E_4K, addr);
}

void SflsIs25lp128_32kErase(UINT addr)
{
	SflsIs25lp128_Erase(SFLS_E_32K, addr);
}

void SflsIs25lp128_64kErase(UINT addr)
{
	SflsIs25lp128_Erase(SFLS_E_64K, addr);
}

void SflsIs25lp128_ChipErase(UINT addr)
{
	SflsIs25lp128_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsIs25lp128_GetControl(void)
{
	return &sfls_is25lp128;
}
#endif
