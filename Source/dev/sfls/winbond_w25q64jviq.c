#include "dev.h"

#ifdef __SFLS_W25Q64JVIQ__
#include "winbond_w25q64jviq.h"

static SFLScontrol sfls_w25q64jviq = {
	.ior_cmd = W25Q64JVIQ_IOR_CMD,
	.ior_adr = W25Q64JVIQ_IOR_ADR,
	.ior_dat = W25Q64JVIQ_IOR_DAT,
	.iow_cmd = W25Q64JVIQ_IOW_CMD,
	.iow_adr = W25Q64JVIQ_IOW_ADR,
	.iow_dat = W25Q64JVIQ_IOW_DAT,
	.iowe_cmd = W25Q64JVIQ_IOWE_CMD,
	.iors_cmd = W25Q64JVIQ_IORS_CMD,
	.iors_dat = W25Q64JVIQ_IORS_DAT,

	.is_qpi = 0,

	.cmd_read = W25Q64JVIQ_CMD_FAST_READ,
	.cmd_page_program = W25Q64JVIQ_CMD_PAGE_PROGRAM,
	.cmd_write_enable = W25Q64JVIQ_CMD_WRITE_ENABLE,
	.cmd_read_status = W25Q64JVIQ_CMD_READ_STATUS1,

	.gap = W25Q64JVIQ_READ_GAP,
	.rdltc = W25Q64JVIQ_RDLTC,
	.size = W25Q64JVIQ_SIZE,

	.func_init_1 = SflsW25q64jviq_Init1,
	.func_init_2 = SflsW25q64jviq_Init2,
	.func_write_enable = SflsW25q64jviq_WriteEnable,
	.func_secter_erase = SflsW25q64jviq_SecterErase,
	.func_32k_erase = SflsW25q64jviq_32kErase,
	.func_64k_erase = SflsW25q64jviq_64kErase,
	.func_chip_erase = SflsW25q64jviq_ChipErase,
	.func_read_status = SflsW25q64jviq_ReadStatus1,
	.func_write_status = SflsW25q64jviq_WriteStatus1,
	.func_read_sfdp = SflsW25q64jviq_ReadSFDP,
	.func_enter_4b = 0,
	.func_exit_4b = 0,
	.func_is_4b = 0,
	.func_enter_qpi = 0,
	.func_exit_qpi = 0,
	.func_is_qpi = 0,
#if (W25Q64JVIQ_ENABLE_QE==1)
	.func_enter_qe = SflsW25q64jviq_EnterQE,
	.func_exit_qe = SflsW25q64jviq_ExitQE,
	.func_is_qe = SflsW25q64jviq_IsQE,
#else
	.func_enter_qe = 0,
	.func_exit_qe = 0,
	.func_is_qe = 0,
#endif
	.func_enter_protection = SflsW25q64jviq_EnterProtection,
	.func_exit_protection = SflsW25q64jviq_ExitProtection,
	.func_is_protection = SflsW25q64jviq_IsProtection,
};

void SflsW25q64jviq_Init1(void)
{

}

void SflsW25q64jviq_Init2(void)
{

}

void SflsW25q64jviq_WriteEnable(void)
{
	SFLS_USR_CMD_MODE	= sfls_w25q64jviq.iowe_cmd;
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_w25q64jviq.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsW25q64jviq_Erase(SFLS_ERASE erase_type, UINT addr)
{
	SFLS_USR_RDLTC		= 1;
	SFLS_USR_CMD_MODE 	= sfls_w25q64jviq.iow_cmd;
	SFLS_USR_ADR_MODE	= sfls_w25q64jviq.iow_adr;
	SFLS_USR_RDREG_CMD_MODE = sfls_w25q64jviq.iors_cmd;
	SFLS_USR_RDREG_DAT_MODE	= sfls_w25q64jviq.iors_dat;

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
		SFLS_USR_CMD	= W25Q64JVIQ_CMD_CHIP_ERASE;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= W25Q64JVIQ_CMD_SECTOR_ERASE;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= W25Q64JVIQ_CMD_32KB_ERASE;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= W25Q64JVIQ_CMD_64KB_ERASE;
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsW25q64jviq_SecterErase(UINT addr)
{
	SflsW25q64jviq_Erase(SFLS_E_4K, addr);
}

void SflsW25q64jviq_32kErase(UINT addr)
{
	SflsW25q64jviq_Erase(SFLS_E_32K, addr);
}

void SflsW25q64jviq_64kErase(UINT addr)
{
	SflsW25q64jviq_Erase(SFLS_E_64K, addr);
}

void SflsW25q64jviq_ChipErase(UINT addr)
{
	SflsW25q64jviq_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsW25q64jviq_GetControl(void)
{
	return &sfls_w25q64jviq;
}

void SflsW25q64jviq_ReadSFDP(BYTE *sfdp_bin)
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
	SFLS_USR_CMD		= W25Q64JVIQ_CMD_READ_SFDP;
	SFLS_USR_WRDAT		= 0;

	for (UINT i = 0; i < (256 / 4); i++) {
		SFLS_USR_ADR	= i * 4;
		SFLS_USR_REQ	= 1;
		while (SFLS_USR_REQ);
		data[i] = ENX_HTONL(SFLS_USR_RDDAT);
	}
}

inline static BYTE SflsW25q64jviq_ReadStatus(int cmd)
{
	SFLS_USR_RDLTC		= sfls_w25q64jviq.rdltc;
	SFLS_USR_CMD_MODE	= sfls_w25q64jviq.iors_cmd;
	SFLS_USR_DAT_MODE	= sfls_w25q64jviq.iors_dat;
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
//	printf("CMD%02X: SFLS-RD:0x%08X(0x%02X), MODE(%d,%d)\n", SFLS_USR_CMD, SFLS_USR_RDDAT, SFLS_USR_RDDAT >> 24, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
	return SFLS_USR_RDDAT >> 24;
}

static void SflsW25q64jviq_WriteStatus(int cmd, BYTE dat)
{
	SflsW25q64jviq_WriteEnable();
	while (SFLS_USR_REQ);

	SFLS_USR_RDLTC		= sfls_w25q64jviq.rdltc;
	SFLS_USR_CMD_MODE	= sfls_w25q64jviq.iors_cmd;
	SFLS_USR_DAT_MODE	= sfls_w25q64jviq.iors_dat;
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
//	printf("CMD%02X: SFLS-WD:0x%08X(0x%02X), MODE(%d,%d)\n", SFLS_USR_CMD, SFLS_USR_WRDAT, dat, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
}

BYTE SflsW25q64jviq_ReadStatus1(void)
{
	BYTE rdifr = SflsW25q64jviq_ReadStatus(W25Q64JVIQ_CMD_READ_STATUS1);
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (rdifr & 0x1)	printf("- Erase/Write In Progress (BUSY)\n");
	if (rdifr & 0x2)	printf("- Write Enable Latch (WEL)\n");
	if (rdifr & 0x4)	printf("- Block Protect Bits (BP0)\n");
	if (rdifr & 0x8)	printf("- Block Protect Bits (BP1)\n");
	if (rdifr & 0x10)	printf("- Block Protect Bits (BP2)\n");
	if (rdifr & 0x20)	printf("- Top/Bottom Block Protect (TB)\n");
	if (rdifr & 0x40)	printf("- Sector/Block Protect Bit (SEC)\n");
	if (rdifr & 0x80)	printf("- Complement Protect (CMP)\n");
#endif
	return rdifr;
}

inline BYTE SflsW25q64jviq_ReadStatus2(void)
{
	BYTE rdifr = SflsW25q64jviq_ReadStatus(W25Q64JVIQ_CMD_READ_STATUS2);
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (rdifr & 0x1)	printf("- Status Register Protect (SRL)\n");
	if (rdifr & 0x2)	printf("- Quad Enable (QE)\n");
	if (rdifr & 0x4)	printf("- Reserved bit\n");
	if (rdifr & 0x8)	printf("- Security Register Lock Bits (LB1)\n");
	if (rdifr & 0x10)	printf("- Security Register Lock Bits (LB2)\n");
	if (rdifr & 0x20)	printf("- Security Register Lock Bits (LB3)\n");
	if (rdifr & 0x40)	printf("- Complement Protect (CMP)\n");
	if (rdifr & 0x80)	printf("- Erase/Program Suspend Status (SUS)\n");
#endif
	return rdifr;
}

BYTE SflsW25q64jviq_ReadStatus3(void)
{
	BYTE rdifr = SflsW25q64jviq_ReadStatus(W25Q64JVIQ_CMD_READ_STATUS3);
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (rdifr & 0x1)	printf("- Reserved bit\n");
	if (rdifr & 0x2)	printf("- Reserved bit\n");
	if (rdifr & 0x4)	printf("- Write Protect Selection (WPS)\n");
	if (rdifr & 0x8)	printf("- Reserved bit\n");
	if (rdifr & 0x10)	printf("- Reserved bit\n");
	if (rdifr & 0x20)	printf("- Output Driver Strength (DRV0)\n");
	if (rdifr & 0x40)	printf("- Output Driver Strength (DRV1)\n");
	if (rdifr & 0x80)	printf("- Reserved bit\n");
#endif
	return rdifr;
}

void SflsW25q64jviq_WriteStatus1(BYTE status)
{
	SflsW25q64jviq_WriteStatus(W25Q64JVIQ_CMD_WRITE_STATUS1, status);
}

void SflsW25q64jviq_WriteStatus2(BYTE status)
{
	SflsW25q64jviq_WriteStatus(W25Q64JVIQ_CMD_WRITE_STATUS2, status);
}

void SflsW25q64jviq_WriteStatus3(BYTE status)
{
	SflsW25q64jviq_WriteStatus(W25Q64JVIQ_CMD_WRITE_STATUS3, status);
}

#if (W25Q64JVIQ_ENABLE_QE==1)
void SflsW25q64jviq_EnterQE(void)
{
	BYTE status = SflsW25q64jviq_ReadStatus2();
	if (!((status & 0x02) == 0x02)) {
		status |= 0x02;
		SflsW25q64jviq_WriteStatus2(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QE\n");
	}
}

void SflsW25q64jviq_ExitQE(void)
{
	BYTE status = SflsW25q64jviq_ReadStatus2();
	if (status & 0x02) {
		status &= ~0x02;
		SflsW25q64jviq_WriteStatus2(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QE\n");
	}
}

UINT SflsW25q64jviq_IsQE(void)
{
	BYTE status = SflsW25q64jviq_ReadStatus2();
	if (status & 0x02) {
		return 1;
	} else {
		return 0;
	}
}
#endif

void SflsW25q64jviq_EnterProtection(void)
{
	BYTE status = SflsW25q64jviq_ReadStatus1();
	if (!((status & 0x1C) == 0x1C)) {
		status |= 0x1C;
		SflsW25q64jviq_WriteStatus1(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter protection\n");
	}
}

void SflsW25q64jviq_ExitProtection(void)
{
	BYTE status = SflsW25q64jviq_ReadStatus1();
	if (status & 0x1C) {
		status &= ~0x1C;
		SflsW25q64jviq_WriteStatus1(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit protection\n");
	}
}

UINT SflsW25q64jviq_IsProtection(void)
{
	BYTE status = SflsW25q64jviq_ReadStatus1();
	if (status & 0x1C) {
		return 1;
	} else {
		return 0;
	}
}
#endif
