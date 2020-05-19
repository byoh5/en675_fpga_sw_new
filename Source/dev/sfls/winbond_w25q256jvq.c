#include "dev.h"

#ifdef __SFLS_W25Q256JVQ__
#include "winbond_w25q256jvq.h"

static SFLScontrol sfls_w25q256jvq = {
	.ior_cmd = W25Q256JVQ_IOR_CMD,
	.ior_adr = W25Q256JVQ_IOR_ADR,
	.ior_dat = W25Q256JVQ_IOR_DAT,
	.iow_cmd = W25Q256JVQ_IOW_CMD,
	.iow_adr = W25Q256JVQ_IOW_ADR,
	.iow_dat = W25Q256JVQ_IOW_DAT,
	.iowe_cmd = W25Q256JVQ_IOWE_CMD,
	.iors_cmd = W25Q256JVQ_IORS_CMD,
	.iors_dat = W25Q256JVQ_IORS_DAT,

	.is_qpi = 0,

	.cmd_read = W25Q256JVQ_CMD_FAST_READ,
	.cmd_page_program = W25Q256JVQ_CMD_PAGE_PROGRAM,
	.cmd_write_enable = W25Q256JVQ_CMD_WRITE_ENABLE,
	.cmd_read_status = W25Q256JVQ_CMD_READ_STATUS1,

	.gap = W25Q256JVQ_READ_GAP,
	.rdltc = W25Q256JVQ_RDLTC,
	.size = W25Q256JVQ_SIZE,

	.func_init_1 = SflsW25q256jvq_Init1,
	.func_init_2 = SflsW25q256jvq_Init2,
	.func_write_enable = SflsW25q256jvq_WriteEnable,
	.func_secter_erase = SflsW25q256jvq_SecterErase,
	.func_32k_erase = SflsW25q256jvq_32kErase,
	.func_64k_erase = SflsW25q256jvq_64kErase,
	.func_chip_erase = SflsW25q256jvq_ChipErase,
	.func_read_status = SflsW25q256jvq_ReadStatus1,
	.func_write_status = SflsW25q256jvq_WriteStatus1,
	.func_read_sfdp = SflsW25q256jvq_ReadSFDP,
#if (W25Q256JVQ_ENABLE_4B==1)
	.func_enter_4b = SflsW25q256jvq_Enter4B,
	.func_exit_4b = SflsW25q256jvq_Exit4B,
	.func_is_4b = SflsW25q256jvq_Is4B,
#else
	.func_enter_4b = 0,
	.func_exit_4b = 0,
	.func_is_4b = 0,
#endif
	.func_enter_qpi = 0,
	.func_exit_qpi = 0,
	.func_is_qpi = 0,
#if (W25Q256JVQ_ENABLE_QE==1)
	.func_enter_qe = SflsW25q256jvq_EnterQE,
	.func_exit_qe = SflsW25q256jvq_ExitQE,
	.func_is_qe = SflsW25q256jvq_IsQE,
#else
	.func_enter_qe = 0,
	.func_exit_qe = 0,
	.func_is_qe = 0,
#endif
	.func_enter_protection = SflsW25q256jvq_EnterProtection,
	.func_exit_protection = SflsW25q256jvq_ExitProtection,
	.func_is_protection = SflsW25q256jvq_IsProtection,
};

void SflsW25q256jvq_Init1(void)
{

}

void SflsW25q256jvq_Init2(void)
{

}

void SflsW25q256jvq_WriteEnable(void)
{
	SFLS_USR_CMD_MODE	= sfls_w25q256jvq.iowe_cmd;
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_w25q256jvq.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsW25q256jvq_Erase(SFLS_ERASE erase_type, UINT addr)
{
	SFLS_USR_RDLTC		= sfls_w25q256jvq.rdltc;
	SFLS_USR_CMD_MODE 	= sfls_w25q256jvq.iow_cmd;
	SFLS_USR_ADR_MODE	= sfls_w25q256jvq.iow_adr;
	SFLS_USR_RDREG_CMD_MODE = sfls_w25q256jvq.iors_cmd;
	SFLS_USR_RDREG_DAT_MODE	= sfls_w25q256jvq.iors_dat;

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
		SFLS_USR_CMD	= W25Q256JVQ_CMD_CHIP_ERASE;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= W25Q256JVQ_CMD_SECTOR_ERASE;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= W25Q256JVQ_CMD_32KB_ERASE;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= W25Q256JVQ_CMD_64KB_ERASE;
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsW25q256jvq_SecterErase(UINT addr)
{
	SflsW25q256jvq_Erase(SFLS_E_4K, addr);
}

void SflsW25q256jvq_32kErase(UINT addr)
{
	SflsW25q256jvq_Erase(SFLS_E_32K, addr);
}

void SflsW25q256jvq_64kErase(UINT addr)
{
	SflsW25q256jvq_Erase(SFLS_E_64K, addr);
}

void SflsW25q256jvq_ChipErase(UINT addr)
{
	SflsW25q256jvq_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsW25q256jvq_GetControl(void)
{
	return &sfls_w25q256jvq;
}

void SflsW25q256jvq_ReadSFDP(BYTE *sfdp_bin)
{
	UINT *data = (UINT *)sfdp_bin;

	SFLS_USR_RDLTC		= sfls_w25q256jvq.rdltc;
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
	SFLS_USR_CMD		= W25Q256JVQ_CMD_READ_SFDP;
	SFLS_USR_WRDAT		= 0;

	for (UINT i = 0; i < (256 / 4); i++) {
		SFLS_USR_ADR	= i * 4;
		SFLS_USR_REQ	= 1;
		while (SFLS_USR_REQ);
		data[i] = ENX_HTONL(SFLS_USR_RDDAT);
	}
}

inline static BYTE SflsW25q256jvq_ReadStatus(int cmd)
{
	SFLS_USR_RDLTC		= sfls_w25q256jvq.rdltc;
	SFLS_USR_CMD_MODE	= sfls_w25q256jvq.iors_cmd;
	SFLS_USR_DAT_MODE	= sfls_w25q256jvq.iors_dat;
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

static void SflsW25q256jvq_WriteStatus(int cmd, BYTE dat)
{
	SflsW25q256jvq_WriteEnable();
	while (SFLS_USR_REQ);

	SFLS_USR_RDLTC		= sfls_w25q256jvq.rdltc;
	SFLS_USR_CMD_MODE	= sfls_w25q256jvq.iors_cmd;
	SFLS_USR_DAT_MODE	= sfls_w25q256jvq.iors_dat;
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

BYTE SflsW25q256jvq_ReadStatus1(void)
{
	BYTE rdifr = SflsW25q256jvq_ReadStatus(W25Q256JVQ_CMD_READ_STATUS1);
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (rdifr & 0x1)	printf("- Erase/Write In Progress (BUSY)\n");
	if (rdifr & 0x2)	printf("- Write Enable Latch (WEL)\n");
	if (rdifr & 0x4)	printf("- Block Protect Bits (BP0) \n");
	if (rdifr & 0x8)	printf("- Block Protect Bits (BP1) \n");
	if (rdifr & 0x10)	printf("- Block Protect Bits (BP2) \n");
	if (rdifr & 0x20)	printf("- Block Protect Bits (BP3) \n");
	if (rdifr & 0x40)	printf("- Top/Bottom Block Protect (TB)\n");
	if (rdifr & 0x80)	printf("- Reserved bit\n");
#endif
	return rdifr;
}

inline BYTE SflsW25q256jvq_ReadStatus2(void)
{
	BYTE rdifr = SflsW25q256jvq_ReadStatus(W25Q256JVQ_CMD_READ_STATUS2);
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

BYTE SflsW25q256jvq_ReadStatus3(void)
{
	BYTE rdifr = SflsW25q256jvq_ReadStatus(W25Q256JVQ_CMD_READ_STATUS3);
#if (DBG_SFLS_STATUS==ENX_DBG_ON)
	if (rdifr & 0x1)	printf("- Current Address Mode (ADS)\n");
	if (rdifr & 0x2)	printf("- Power-Up Address Mode (ADP)\n");
	if (rdifr & 0x4)	printf("- Write Protect Selection (WPS)\n");
	if (rdifr & 0x8)	printf("- Reserved bit\n");
	if (rdifr & 0x10)	printf("- Reserved bit\n");
	if (rdifr & 0x20)	printf("- Output Driver Strength (DRV0)\n");
	if (rdifr & 0x40)	printf("- Output Driver Strength (DRV1)\n");
	if (rdifr & 0x80)	printf("- Reserved bit\n");
#endif
	return rdifr;
}

void SflsW25q256jvq_WriteStatus1(BYTE status)
{
	SflsW25q256jvq_WriteStatus(W25Q256JVQ_CMD_WRITE_STATUS1, status);
}

void SflsW25q256jvq_WriteStatus2(BYTE status)
{
	SflsW25q256jvq_WriteStatus(W25Q256JVQ_CMD_WRITE_STATUS2, status);
}

void SflsW25q256jvq_WriteStatus3(BYTE status)
{
	SflsW25q256jvq_WriteStatus(W25Q256JVQ_CMD_WRITE_STATUS3, status);
}

#if (W25Q256JVQ_ENABLE_4B==1)
void SflsW25q256jvq_Enter4B(void)
{
	BYTE status = SflsW25q256jvq_ReadStatus3();
	if (!((status & 0x1) == 0x1)) {
		SflsSingleCommand(W25Q256JVQ_CMD_EN4B, SFLS_BUS_RDREG_CMD_MODE);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter 4B\n");
	}
}

void SflsW25q256jvq_Exit4B(void)
{
	BYTE status = SflsW25q256jvq_ReadStatus3();
	if (status & 0x1) {
		SflsSingleCommand(W25Q256JVQ_CMD_EX4B, SFLS_BUS_RDREG_CMD_MODE);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit 4B\n");
	}
}

UINT SflsW25q256jvq_Is4B(void)
{
	BYTE status = SflsW25q256jvq_ReadStatus3();
	if (status & 0x1) {
		return 1;
	} else {
		return 0;
	}
}
#endif

#if (W25Q256JVQ_ENABLE_QE==1)
void SflsW25q256jvq_EnterQE(void)
{
	BYTE status = SflsW25q256jvq_ReadStatus2();
	if (!((status & 0x02) == 0x02)) {
		status |= 0x02;
		SflsW25q256jvq_WriteStatus2(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QE\n");
	}
}

void SflsW25q256jvq_ExitQE(void)
{
	BYTE status = SflsW25q256jvq_ReadStatus2();
	if (status & 0x02) {
		status &= ~0x02;
		SflsW25q256jvq_WriteStatus2(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QE\n");
	}
}

UINT SflsW25q256jvq_IsQE(void)
{
	BYTE status = SflsW25q256jvq_ReadStatus2();
	if (status & 0x02) {
		return 1;
	} else {
		return 0;
	}
}
#endif

void SflsW25q256jvq_EnterProtection(void)
{
	BYTE status = SflsW25q256jvq_ReadStatus1();
	if (!((status & 0x7C) == 0x7C)) {
		status |= 0x7C;
		SflsW25q256jvq_WriteStatus1(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter protection\n");
	}
}

void SflsW25q256jvq_ExitProtection(void)
{
	BYTE status = SflsW25q256jvq_ReadStatus1();
	if (status & 0x7C) {
		status &= ~0x7C;
		SflsW25q256jvq_WriteStatus1(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit protection\n");
	}
}

UINT SflsW25q256jvq_IsProtection(void)
{
	BYTE status = SflsW25q256jvq_ReadStatus1();
	if (status & 0x7C) {
		return 1;
	} else {
		return 0;
	}
}
#endif
