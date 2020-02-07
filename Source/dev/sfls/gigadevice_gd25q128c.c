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

	.is_qpi = 0,

	.cmd_read = GD25Q128C_CMD_FAST_READ,
	.cmd_page_program = GD25Q128C_CMD_PAGE_PROGRAM,
	.cmd_write_enable = GD25Q128C_CMD_WRITE_ENABLE,
	.cmd_read_status = GD25Q128C_CMD_READ_STATUS1,

	.gap = GD25Q128C_READ_GAP,
	.rdltc = GD25Q128C_RDLTC,
	.size = GD25Q128C_SIZE,

	.func_init_1 = SflsGd25q128c_Init1,
	.func_init_2 = SflsGd25q128c_Init2,
	.func_write_enable = SflsGd25q128c_WriteEnable,
	.func_secter_erase = SflsGd25q128c_SecterErase,
	.func_32k_erase = SflsGd25q128c_32kErase,
	.func_64k_erase = SflsGd25q128c_64kErase,
	.func_chip_erase = SflsGd25q128c_ChipErase,
	.func_read_status = SflsGd25q128c_ReadStatus1,
	.func_write_status = SflsGd25q128c_WriteStatus1,
	.func_read_sfdp = SflsGd25q128c_ReadSFDP,
	.func_enter_4b = 0,
	.func_exit_4b = 0,
	.func_is_4b = 0,
#if (GD25Q128C_ENABLE_QPI==1)
	.func_enter_qpi = SflsGd25q128c_EnterQPI,
	.func_exit_qpi = SflsGd25q128c_ExitQPI,
	.func_is_qpi = SflsGd25q128c_IsQPI,
#else
	.func_enter_qpi = 0,
	.func_exit_qpi = 0,
	.func_is_qpi = 0,
#endif
#if (GD25Q128C_ENABLE_QE==1)
	.func_enter_qe = SflsGd25q128c_EnterQE,
	.func_exit_qe = SflsGd25q128c_ExitQE,
	.func_is_qe = SflsGd25q128c_IsQE,
#else
	.func_enter_qe = 0,
	.func_exit_qe = 0,
	.func_is_qe = 0,
#endif
	.func_enter_protection = SflsGd25q128c_EnterProtection,
	.func_exit_protection = SflsGd25q128c_ExitProtection,
	.func_is_protection = SflsGd25q128c_IsProtection,
};

void SflsGd25q128c_Init1(void)
{

}

void SflsGd25q128c_Init2(void)
{

}

void SflsGd25q128c_WriteEnable(void)
{
	if (sfls_gd25q128c.is_qpi) {
		SFLS_USR_CMD_MODE = 2;
	} else {
		SFLS_USR_CMD_MODE = 0;
	}
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
	if (sfls_gd25q128c.is_qpi) {
		SFLS_USR_CMD_MODE = 2;
		SFLS_USR_ADR_MODE = 2;
		SFLS_USR_RDREG_CMD_MODE = 2;
		SFLS_USR_RDREG_DAT_MODE	= 2;
		SFLS_USR_RDLTC	= sfls_gd25q128c.rdltc;
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
	SFLS_USR_ADR_EXT 	= 0;
	SFLS_USR_BUSY_EN 	= 1;
	switch (erase_type) {
	case SFLS_E_ALL:
		SFLS_USR_CMD	= GD25Q128C_CMD_CHIP_ERASE;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= GD25Q128C_CMD_SECTOR_ERASE;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= GD25Q128C_CMD_32KB_ERASE;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= GD25Q128C_CMD_64KB_ERASE;
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

void SflsGd25q128c_ReadSFDP(BYTE *sfdp_bin)
{
	UINT *data = (UINT *)sfdp_bin;

	if (sfls_gd25q128c.is_qpi) {
		SFLS_USR_RDLTC	= sfls_gd25q128c.rdltc;
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
	if (sfls_gd25q128c.is_qpi) {
		SFLS_USR_GAP	= sfls_gd25q128c.gap - 1;
	} else {
		SFLS_USR_GAP	= 7;
	}
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_LEN		= 3;
	SFLS_USR_ADR_EXT	= 0;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= GD25Q128C_CMD_READ_SFDP;
	SFLS_USR_WRDAT		= 0;

	for (UINT i = 0; i < (256 / 4); i++) {
		SFLS_USR_ADR	= i * 4;
		SFLS_USR_REQ	= 1;
		while (SFLS_USR_REQ);
		data[i] = ENX_HTONL(SFLS_USR_RDDAT);
	}
}

inline static BYTE SflsGd25q128c_ReadStatus(int cmd)
{
	if (sfls_gd25q128c.is_qpi) {
		SFLS_USR_RDLTC	= sfls_gd25q128c.rdltc;
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
	SFLS_USR_CMD		= cmd;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
//	printf("CMD%02X: SFLS-RD:0x%08X, MODE(%d,%d)\n", SFLS_USR_CMD, SFLS_USR_RDDAT, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
	return SFLS_USR_RDDAT >> 24;
}

static void SflsGd25q128c_WriteStatus(int cmd, BYTE dat)
{
	SflsGd25q128c_WriteEnable();
	while (SFLS_USR_REQ);

	if (sfls_gd25q128c.is_qpi) {
		SFLS_USR_RDLTC	= sfls_gd25q128c.rdltc;
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
	SFLS_USR_CMD		= cmd;
	SFLS_USR_WRDAT		= dat << 24;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
//	printf("CMD%02X: SFLS-WD:0x%08X, MODE(%d,%d)\n", SFLS_USR_CMD, dat, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
}

BYTE SflsGd25q128c_ReadStatus1(void)
{
	BYTE rdifr = SflsGd25q128c_ReadStatus(GD25Q128C_CMD_READ_STATUS1);
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (rdifr & 0x1)	printf("- Write In Progress (WIP)\n");
	if (rdifr & 0x2)	printf("- Write Enable Latch (WEL)\n");
	if (rdifr & 0x4)	printf("- Block Protect Bits (BP0)\n");
	if (rdifr & 0x8)	printf("- Block Protect Bits (BP1)\n");
	if (rdifr & 0x10)	printf("- Block Protect Bits (BP2)\n");
	if (rdifr & 0x20)	printf("- Block Protect Bits (BP3)\n");
	if (rdifr & 0x40)	printf("- Block Protect Bits (BP4)\n");
	if (rdifr & 0x80)	printf("- Status Register Protect (SRP0)\n");
#endif
	return rdifr;
}

inline BYTE SflsGd25q128c_ReadStatus2(void)
{
	BYTE rdifr = SflsGd25q128c_ReadStatus(GD25Q128C_CMD_READ_STATUS2);
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (rdifr & 0x1)	printf("- Status Register Protect (SRP1)\n");
	if (rdifr & 0x2)	printf("- Quad Enable (QE)\n");
	if (rdifr & 0x4)	printf("- Erase/Program Suspend Status (SUS2)\n");
	if (rdifr & 0x8)	printf("- Security Register Lock Bits (LB1)\n");
	if (rdifr & 0x10)	printf("- Security Register Lock Bits (LB2)\n");
	if (rdifr & 0x20)	printf("- Security Register Lock Bits (LB3)\n");
	if (rdifr & 0x40)	printf("- Complement Protect (CMP)\n");
	if (rdifr & 0x80)	printf("- Erase/Program Suspend Status (SUS1)\n");
#endif
	return rdifr;
}

BYTE SflsGd25q128c_ReadStatus3(void)
{
	BYTE rdifr = SflsGd25q128c_ReadStatus(GD25Q128C_CMD_READ_STATUS3);
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (rdifr & 0x1)	printf("- Reserved bit\n");
	if (rdifr & 0x2)	printf("- Reserved bit\n");
	if (rdifr & 0x4)	printf("- Write Protect Selection (WPS)\n");
	if (rdifr & 0x8)	printf("- Reserved bit\n");
	if (rdifr & 0x10)	printf("- Reserved bit\n");
	if (rdifr & 0x20)	printf("- Output Driver Strength (DRV0)\n");
	if (rdifr & 0x40)	printf("- Output Driver Strength (DRV1)\n");
	if (rdifr & 0x80)	printf("- HOLD/RST\n");
#endif
	return rdifr;
}

void SflsGd25q128c_WriteStatus1(BYTE status)
{
	SflsGd25q128c_WriteStatus(GD25Q128C_CMD_WRITE_STATUS1, status);
}

void SflsGd25q128c_WriteStatus2(BYTE status)
{
	SflsGd25q128c_WriteStatus(GD25Q128C_CMD_WRITE_STATUS2, status);
}

void SflsGd25q128c_WriteStatus3(BYTE status)
{
	SflsGd25q128c_WriteStatus(GD25Q128C_CMD_WRITE_STATUS3, status);
}

#if (GD25Q128C_ENABLE_QPI==1)
void SflsGd25q128c_EnterQPI(void)
{
	if (sfls_gd25q128c.is_qpi == 0) {
		SflsSingleCommand(GD25Q128C_CMD_ENTER_QPI, 0);
		sfls_gd25q128c.is_qpi = 1;
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QPI\n");
	}
}

void SflsGd25q128c_ExitQPI(void)
{
	if (sfls_gd25q128c.is_qpi == 1) {
		SflsSingleCommand(GD25Q128C_CMD_EXIT_QPI, 2);
		sfls_gd25q128c.is_qpi = 0;
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QPI\n");
	}
}

UINT SflsGd25q128c_IsQPI(void)
{
	return sfls_gd25q128c.is_qpi;
}
#endif

#if (GD25Q128C_ENABLE_QE==1)
void SflsGd25q128c_EnterQE(void)
{
	BYTE status = SflsGd25q128c_ReadStatus2();
	if (!((status & 0x02) == 0x02)) {
		status |= 0x02;
		SflsGd25q128c_WriteStatus2(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QE\n");
	}
}

void SflsGd25q128c_ExitQE(void)
{
	BYTE status = SflsGd25q128c_ReadStatus2();
	if (status & 0x02) {
		status &= ~0x02;
		SflsGd25q128c_WriteStatus2(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QE\n");
	}
}

UINT SflsGd25q128c_IsQE(void)
{
	BYTE status = SflsGd25q128c_ReadStatus2();
	if (status & 0x02) {
		return 1;
	} else {
		return 0;
	}
}
#endif

void SflsGd25q128c_EnterProtection(void)
{
	BYTE status = SflsGd25q128c_ReadStatus1();
	if (!((status & 0x7C) == 0x7C)) {
		status |= 0x7C;
		SflsGd25q128c_WriteStatus1(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter protection\n");
	}
}

void SflsGd25q128c_ExitProtection(void)
{
	BYTE status = SflsGd25q128c_ReadStatus1();
	if (status & 0x7C) {
		status &= ~0x7C;
		SflsGd25q128c_WriteStatus1(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit protection\n");
	}
}

UINT SflsGd25q128c_IsProtection(void)
{
	BYTE status = SflsGd25q128c_ReadStatus1();
	if (status & 0x7C) {
		return 1;
	} else {
		return 0;
	}
}
#endif
