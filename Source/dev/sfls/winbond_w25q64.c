#include "dev.h"

#ifdef __SFLS_W25Q64__
#include "winbond_w25q64.h"

static SFLScontrol sfls_w25q64 = {
	.ior_cmd = W25Q64_IOR_CMD,
	.ior_adr = W25Q64_IOR_ADR,
	.ior_dat = W25Q64_IOR_DAT,
	.iow_cmd = W25Q64_IOW_CMD,
	.iow_adr = W25Q64_IOW_ADR,
	.iow_dat = W25Q64_IOW_DAT,
	.iowe_cmd = W25Q64_IOWE_CMD,
	.iors_cmd = W25Q64_IORS_CMD,
	.iors_dat = W25Q64_IORS_DAT,

#if (W25Q64_IOMODE==SFLS_SINGLE)
	.cmd_read = W25Q64_CMD_READ,
#elif (W25Q64_IOMODE==SFLS_DUAL)
	.cmd_read = W25Q64_CMD_DUAL_READ,
#elif (W25Q64_IOMODE==SFLS_QUAD)
	.cmd_read = W25Q64_CMD_QUAD_READ,
#endif
	.cmd_page_program = W25Q64_CMD_PAGE_PROGRAM,
	.cmd_write_enable = W25Q64_CMD_WRITE_ENABLE,
	.cmd_read_status = W25Q64_CMD_READ_STATUS,
#if (W25Q64_ENABLE_QPI==1)
	.cmd_enter_qpi = W25Q64_CMD_ENTER_QPI,
#else
	.cmd_enter_qpi = 0,
#endif
	.cmd_enter_4b = 0,
	.cmd_exit_4b = 0,
	.cmd_sector_erase = W25Q64_CMD_SECTOR_ERASE,
	.cmd_32k_erase = W25Q64_CMD_32KB_ERASE,
	.cmd_64k_erase = W25Q64_CMD_64KB_ERASE,
	.cmd_chip_erase = W25Q64_CMD_CHIP_ERASE,

	.gap = W25Q64_READ_GAP - 1,
	.rdltc = W25Q64_RDLTC,

	.func_write_enable = SflsW25q64_WriteEnable,
	.func_secter_erase = SflsW25q64_SecterErase,
	.func_32k_erase = SflsW25q64_32kErase,
	.func_64k_erase = SflsW25q64_64kErase,
	.func_chip_erase = SflsW25q64_ChipErase,
};

void SflsW25q64_WriteEnable(void)
{
	SFLS_USR_CMD_MODE	= sfls_w25q64.iow_cmd;
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_w25q64.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsW25q64_Erase(SFLS_ERASE erase_type, UINT addr)
{
	SFLS_USR_CMD_MODE 	= sfls_w25q64.iow_cmd;
	if (erase_type == SFLS_E_ALL) {
		SFLS_USR_ADR_MODE= 0;
		SFLS_USR_ADR_EN	= 0;
		SFLS_USR_ADR	= 0;
	} else {
		SFLS_USR_ADR_MODE= sfls_w25q64.iow_adr;
		SFLS_USR_ADR_EN	= 1;
		SFLS_USR_ADR	= addr;
	}
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_ADR_EXT 	= 0;
	SFLS_USR_BUSY_EN 	= 1;
	SFLS_USR_RDREG_CMD_MODE = sfls_w25q64.iors_cmd;
	SFLS_USR_RDREG_DAT_MODE	= sfls_w25q64.iors_dat;
	switch (erase_type) {
	case SFLS_E_ALL:
		SFLS_USR_CMD	= sfls_w25q64.cmd_chip_erase;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= sfls_w25q64.cmd_sector_erase;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= sfls_w25q64.cmd_32k_erase;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= sfls_w25q64.cmd_64k_erase;
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsW25q64_SecterErase(UINT addr)
{
	SflsW25q64_Erase(SFLS_E_4K, addr);
}

void SflsW25q64_32kErase(UINT addr)
{
	SflsW25q64_Erase(SFLS_E_32K, addr);
}

void SflsW25q64_64kErase(UINT addr)
{
	SflsW25q64_Erase(SFLS_E_64K, addr);
}

void SflsW25q64_ChipErase(UINT addr)
{
	SflsW25q64_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsW25q64_GetControl(void)
{
	return &sfls_w25q64;
}
#endif
