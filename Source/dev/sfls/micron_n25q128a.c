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

#if (N25Q128A_IOMODE==SFLS_SINGLE)
	.cmd_read = N25Q128A_CMD_READ,
#elif (N25Q128A_IOMODE==SFLS_DUAL)
	.cmd_read = N25Q128A_CMD_DUAL_READ,
#elif (N25Q128A_IOMODE==SFLS_QUAD)
	.cmd_read = N25Q128A_CMD_QUAD_READ,
#endif
	.cmd_page_program = N25Q128A_CMD_PAGE_PROGRAM,
	.cmd_write_enable = N25Q128A_CMD_WRITE_ENABLE,
	.cmd_read_status = N25Q128A_CMD_READ_STATUS,
#if (N25Q128A_ENABLE_QPI==1)
	.cmd_enter_qpi = N25Q128A_CMD_ENTER_QPI,
#else
	.cmd_enter_qpi = 0,
#endif
	.cmd_sector_erase = N25Q128A_CMD_SECTOR_ERASE,
	.cmd_64k_erase = N25Q128A_CMD_64KB_ERASE,
	.cmd_chip_erase = N25Q128A_CMD_CHIP_ERASE,

	.gap = N25Q128A_READ_GAP - 1,
	.rdltc = N25Q128A_RDLTC,

	.func_init = SflsN25q128a_Init,
	.func_write_enable = SflsN25q128a_WriteEnable,
	.func_secter_erase = SflsN25q128a_SecterErase,
	.func_32k_erase = SflsN25q128a_32kErase,
	.func_64k_erase = SflsN25q128a_64kErase,
	.func_chip_erase = SflsN25q128a_ChipErase,
};

void SflsN25q128a_Init(void)
{
#if (N25Q128A_IOMODE==SFLS_DUAL)
	SFLS_USR_CMD_MODE	= 0;
	SFLS_USR_ADR_MODE	= 0;
	SFLS_USR_DAT_MODE	= 0;
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_GAP		= 0;
	SFLS_USR_LEN		= 0;
	SFLS_USR_ADR_EXT	= 0;
	SFLS_USR_BUSY_EN	= 0;
//	SFLS_USR_BUSY_KEEP	= 0;
	SFLS_USR_CMD		= N25Q128A_CMD_READ_ENHANCED_VOLATILE_CONFIGURATION;
	SFLS_USR_ADR		= 0;
	SFLS_USR_WRDAT		= 0;
	SFLS_USR_RDLTC		= SFLS_IO_RDLTC;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);

	UINT getData = SFLS_USR_RDDAT;
	getData &= ~0x40000000;
	printf("SFLS(0x65) => [0x%02X->0x%02X]\n", SFLS_USR_RDDAT, getData);

	getData = 0x9f9f9f9f;

	// 왜 안될까?

	SflsN25q128a_WriteEnable();

	SFLS_USR_CMD_MODE	= 0;
	SFLS_USR_ADR_MODE	= 0;
	SFLS_USR_DAT_MODE	= 0;
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_RD_EN		= 0;
	SFLS_USR_WR_EN		= 1;
	SFLS_USR_GAP		= 0;
	SFLS_USR_LEN		= 1;
	SFLS_USR_ADR_EXT	= 0;
	SFLS_USR_BUSY_EN	= 0;
//	SFLS_USR_BUSY_KEEP	= 0;
	SFLS_USR_CMD		= N25Q128A_CMD_WRITE_ENHANCED_VOLATILE_CONFIGURATION;
	SFLS_USR_ADR		= 0;
	SFLS_USR_RDLTC		= SFLS_IO_RDLTC;
	SFLS_USR_WRDAT		= getData;
	SFLS_USRBUS_REQ		= 1;
	while (SFLS_BUS_REQ);


	SFLS_USR_CMD_MODE	= 0;
	SFLS_USR_ADR_MODE	= 0;
	SFLS_USR_DAT_MODE	= 0;
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_GAP		= 0;
	SFLS_USR_LEN		= 0;
	SFLS_USR_ADR_EXT	= 0;
	SFLS_USR_BUSY_EN	= 0;
//	SFLS_USR_BUSY_KEEP	= 0;
	SFLS_USR_CMD		= N25Q128A_CMD_READ_ENHANCED_VOLATILE_CONFIGURATION;
	SFLS_USR_ADR		= 0;
	SFLS_USR_WRDAT		= 0;
	SFLS_USR_RDLTC		= SFLS_IO_RDLTC;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);

	getData = SFLS_USR_RDDAT;
	printf("SFLS(0x65) => [0x%02X]\n", getData);


#elif (N25Q128A_IOMODE==SFLS_QUAD)


#endif
}

void SflsN25q128a_WriteEnable(void)
{
	SFLS_USR_CMD_MODE	= sfls_n25q128a.iow_cmd;
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
	SFLS_USR_CMD_MODE 	= sfls_n25q128a.iow_cmd;
	if (erase_type == SFLS_E_ALL) {
		SFLS_USR_ADR_MODE= 0;
		SFLS_USR_ADR_EN	= 0;
		SFLS_USR_ADR	= 0;
	} else {
		SFLS_USR_ADR_MODE= sfls_n25q128a.iow_adr;
		SFLS_USR_ADR_EN	= 1;
		SFLS_USR_ADR	= addr;
	}
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_ADR_EXT 	= 0;
	SFLS_USR_BUSY_EN 	= 1;
	SFLS_USR_RDREG_CMD_MODE = sfls_n25q128a.iors_cmd;
	SFLS_USR_RDREG_DAT_MODE	= sfls_n25q128a.iors_dat;
	switch (erase_type) {
	case SFLS_E_ALL:
		SFLS_USR_CMD	= sfls_n25q128a.cmd_chip_erase;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= sfls_n25q128a.cmd_sector_erase;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= sfls_n25q128a.cmd_64k_erase;
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

SFLScontrol *SflsGetControl(void)
{
	return &sfls_n25q128a;
}
#endif
