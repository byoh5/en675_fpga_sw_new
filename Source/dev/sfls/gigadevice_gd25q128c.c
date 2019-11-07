#include "dev.h"

#ifdef __SFLS_GD25Q128C__
#include "gigadevice_gd25q128c.h"

static SFLScontrol sfls_gd25q128c = {
	.ior_cmd = GD25Q128C_IOR_CMD,
	.ior_adr = GD25Q128C_IOR_ADR,
	.ior_dat = GD25Q128C_IOR_DAT,
	.iow_cmd = GD25Q128C_IOW_CMD,
	.iow_adr = GD25Q128C_IOW_ADR,
	.iow_dat = GD25Q128C_IOW_DAT,
	.iowe_cmd = GD25Q128C_IOWE_CMD,
	.iors_cmd = GD25Q128C_IORS_CMD,
	.iors_dat = GD25Q128C_IORS_DAT,

#if (GD25Q128C_IOMODE==SFLS_SINGLE)
	.cmd_read = GD25Q128C_CMD_READ,
#elif (GD25Q128C_IOMODE==SFLS_DUAL)
	.cmd_read = GD25Q128C_CMD_DUAL_READ,
#elif (GD25Q128C_IOMODE==SFLS_QUAD)
	.cmd_read = GD25Q128C_CMD_QUAD_READ,
#endif
	.cmd_page_program = GD25Q128C_CMD_PAGE_PROGRAM,
	.cmd_write_enable = GD25Q128C_CMD_WRITE_ENABLE,
	.cmd_read_status = GD25Q128C_CMD_READ_STATUS,
#if (GD25Q128C_ENABLE_QPI==1)
	.cmd_enter_qpi = GD25Q128C_CMD_ENTER_QPI,
#else
	.cmd_enter_qpi = 0,
#endif
	.cmd_sector_erase = GD25Q128C_CMD_SECTOR_ERASE,
	.cmd_32k_erase = GD25Q128C_CMD_32KB_ERASE,
	.cmd_64k_erase = GD25Q128C_CMD_64KB_ERASE,
	.cmd_chip_erase = GD25Q128C_CMD_CHIP_ERASE,

	.gap = GD25Q128C_READ_GAP - 1,
	.rdltc = GD25Q128C_RDLTC,

	.func_write_enable = SflsGd25q128c_WriteEnable,
	.func_secter_erase = SflsGd25q128c_SecterErase,
	.func_32k_erase = SflsGd25q128c_32kErase,
	.func_64k_erase = SflsGd25q128c_64kErase,
	.func_chip_erase = SflsGd25q128c_ChipErase,
};

void SflsGd25q128c_WriteEnable(void)
{
	SFLS_USR_CMD_MODE	= sfls_gd25q128c.iow_cmd;
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_gd25q128c.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsGd25q128c_Erase(SFLS_ERASE erase_type, UINT addr)
{
	SFLS_USR_CMD_MODE 	= sfls_gd25q128c.iow_cmd;
	if (erase_type == SFLS_E_ALL) {
		SFLS_USR_ADR_MODE= 0;
		SFLS_USR_ADR_EN	= 0;
		SFLS_USR_ADR	= 0;
	} else {
		SFLS_USR_ADR_MODE= sfls_gd25q128c.iow_adr;
		SFLS_USR_ADR_EN	= 1;
		SFLS_USR_ADR	= addr;
	}
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_ADR_EXT 	= 0;
	SFLS_USR_BUSY_EN 	= 1;
	SFLS_USR_RDREG_CMD_MODE = sfls_gd25q128c.iors_cmd;
	SFLS_USR_RDREG_DAT_MODE	= sfls_gd25q128c.iors_dat;
	switch (erase_type) {
	case SFLS_E_ALL:
		SFLS_USR_CMD	= sfls_gd25q128c.cmd_chip_erase;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= sfls_gd25q128c.cmd_sector_erase;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= sfls_gd25q128c.cmd_32k_erase;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= sfls_gd25q128c.cmd_64k_erase;
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsGd25q128c_SecterErase(UINT addr)
{
	SflsGd25q128c_Erase(SFLS_E_4K, addr);
}

void SflsGd25q128c_32kErase(UINT addr)
{
	SflsGd25q128c_Erase(SFLS_E_32K, addr);
}

void SflsGd25q128c_64kErase(UINT addr)
{
	SflsGd25q128c_Erase(SFLS_E_64K, addr);
}

void SflsGd25q128c_ChipErase(UINT addr)
{
	SflsGd25q128c_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsGd25q128c_GetControl(void)
{
	return &sfls_gd25q128c;
}
#endif
