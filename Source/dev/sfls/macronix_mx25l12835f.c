#include "dev.h"

#ifdef __SFLS_MX25L12835F__
#include "macronix_mx25l12835f.h"

static SFLScontrol sfls_mx25l12835f = {
	.ior_cmd = MX25L12835F_IOR_CMD,
	.ior_adr = MX25L12835F_IOR_ADR,
	.ior_dat = MX25L12835F_IOR_DAT,
	.iow_cmd = MX25L12835F_IOW_CMD,
	.iow_adr = MX25L12835F_IOW_ADR,
	.iow_dat = MX25L12835F_IOW_DAT,
	.iowe_cmd = MX25L12835F_IOWE_CMD,
	.iors_cmd = MX25L12835F_IORS_CMD,
	.iors_dat = MX25L12835F_IORS_DAT,

	.is_qpi = 0,

	.cmd_read = MX25L12835F_CMD_FAST_READ,
	.cmd_page_program = MX25L12835F_CMD_PAGE_PROGRAM,
	.cmd_write_enable = MX25L12835F_CMD_WRITE_ENABLE,
	.cmd_read_status = MX25L12835F_CMD_READ_STATUS,

	.gap = MX25L12835F_READ_GAP,
	.rdltc = MX25L12835F_RDLTC,
	.size = MX25L12835F_SIZE,

	.func_init_1 = SflsMx25l12835f_Init1,
	.func_init_2 = SflsMx25l12835f_Init2,
	.func_write_enable = SflsMx25l12835f_WriteEnable,
	.func_secter_erase = SflsMx25l12835f_SecterErase,
	.func_32k_erase = SflsMx25l12835f_32kErase,
	.func_64k_erase = SflsMx25l12835f_64kErase,
	.func_chip_erase = SflsMx25l12835f_ChipErase,
	.func_read_status = SflsMx25l12835f_ReadStatus,
	.func_write_status = SflsMx25l12835f_WriteStatus,
	.func_read_sfdp = SflsMx25l12835f_ReadSFDP,
	.func_enter_4b = 0,
	.func_exit_4b = 0,
	.func_is_4b = 0,
#if (MX25L12835F_ENABLE_QPI==1)
	.func_enter_qpi = SflsMx25l12835f_EnterQPI,
	.func_exit_qpi = SflsMx25l12835f_ExitQPI,
	.func_is_qpi = SflsMx25l12835f_IsQPI,
#else
	.func_enter_qpi = 0,
	.func_exit_qpi = 0,
	.func_is_qpi = 0,
#endif
#if (MX25L12835F_ENABLE_QE==1)
	.func_enter_qe = SflsMx25l12835f_EnterQE,
	.func_exit_qe = SflsMx25l12835f_ExitQE,
	.func_is_qe = SflsMx25l12835f_IsQE,
#else
	.func_enter_qe = 0,
	.func_exit_qe = 0,
	.func_is_qe = 0,
#endif
	.func_enter_protection = SflsMx25l12835f_EnterProtection,
	.func_exit_protection = SflsMx25l12835f_ExitProtection,
	.func_is_protection = SflsMx25l12835f_IsProtection,
};

void SflsMx25l12835f_Init1(void)
{

}

void SflsMx25l12835f_Init2(void)
{

}

void SflsMx25l12835f_WriteEnable(void)
{
	if (sfls_mx25l12835f.is_qpi) {
		SFLS_USR_CMD_MODE = 2;
	} else {
		SFLS_USR_CMD_MODE = 0;
	}
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_mx25l12835f.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsMx25l12835f_Erase(SFLS_ERASE erase_type, UINT addr)
{
	if (sfls_mx25l12835f.is_qpi) {
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
		SFLS_USR_CMD	= MX25L12835F_CMD_CHIP_ERASE;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= MX25L12835F_CMD_SECTOR_ERASE;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= MX25L12835F_CMD_32KB_ERASE;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= MX25L12835F_CMD_64KB_ERASE;
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsMx25l12835f_SecterErase(UINT addr)
{
	SflsMx25l12835f_Erase(SFLS_E_4K, addr);
}

void SflsMx25l12835f_32kErase(UINT addr)
{
	SflsMx25l12835f_Erase(SFLS_E_32K, addr);
}

void SflsMx25l12835f_64kErase(UINT addr)
{
	SflsMx25l12835f_Erase(SFLS_E_64K, addr);
}

void SflsMx25l12835f_ChipErase(UINT addr)
{
	SflsMx25l12835f_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsMx25l12835f_GetControl(void)
{
	return &sfls_mx25l12835f;
}

void SflsMx25l12835f_ReadSFDP(BYTE *sfdp_bin)
{
	UINT *data = (UINT *)sfdp_bin;

	if (sfls_mx25l12835f.is_qpi) {
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
	if (sfls_mx25l12835f.is_qpi) {
		SFLS_USR_GAP	= SFLS_BUS_GAP;
	} else {
		SFLS_USR_GAP	= 7;
	}
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_LEN		= 3;
	SFLS_USR_ADR_EXT	= 0;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= MX25L12835F_CMD_READ_SFDP;
	SFLS_USR_WRDAT		= 0;

	for (UINT i = 0; i < (256 / 4); i++) {
		SFLS_USR_ADR	= i * 4;
		SFLS_USR_REQ	= 1;
		while (SFLS_USR_REQ);
		data[i] = ENX_HTONL(SFLS_USR_RDDAT);
	}
}

BYTE SflsMx25l12835f_ReadStatus(void)
{
	if (sfls_mx25l12835f.is_qpi) {
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
	SFLS_USR_CMD		= MX25L12835F_CMD_READ_STATUS;
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

void SflsMx25l12835f_WriteStatus(BYTE status)
{
	SflsMx25l12835f_WriteEnable();
	while (SFLS_USR_REQ);

	if (sfls_mx25l12835f.is_qpi) {
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
	SFLS_USR_CMD		= MX25L12835F_CMD_WRITE_STATUS;
	SFLS_USR_WRDAT		= status << 24;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
//	printf("CMD%02X: SFLS-WD:0x%08X, MODE(%d,%d)\n", SFLS_USR_CMD, status, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
}

#if (MX25L12835F_ENABLE_QPI==1)
void SflsMx25l12835f_EnterQPI(void)
{
	if (sfls_mx25l12835f.is_qpi == 0) {
		SflsSingleCommand(MX25L12835F_CMD_ENTER_QPI, 0);
		sfls_mx25l12835f.is_qpi = 1;
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QPI\n");
	}
}

void SflsMx25l12835f_ExitQPI(void)
{
	if (sfls_mx25l12835f.is_qpi == 1) {
		SflsSingleCommand(MX25L12835F_CMD_EXIT_QPI, 2);
		sfls_mx25l12835f.is_qpi = 0;
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QPI\n");
	}
}

UINT SflsMx25l12835f_IsQPI(void)
{
	return sfls_mx25l12835f.is_qpi;
}
#endif

#if (MX25L12835F_ENABLE_QE==1)
void SflsMx25l12835f_EnterQE(void)
{
	BYTE status = SflsMx25l12835f_ReadStatus();
	if (!((status & 0x40) == 0x40)) {
		status |= 0x40;
		SflsMx25l12835f_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QE\n");
	}
}

void SflsMx25l12835f_ExitQE(void)
{
	BYTE status = SflsMx25l12835f_ReadStatus();
	if (status & 0x40) {
		status &= ~0x40;
		SflsMx25l12835f_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QE\n");
	}
}

UINT SflsMx25l12835f_IsQE(void)
{
	BYTE status = SflsMx25l12835f_ReadStatus();
	if (status & 0x40) {
		return 1;
	} else {
		return 0;
	}
}
#endif

void SflsMx25l12835f_EnterProtection(void)
{
	BYTE status = SflsMx25l12835f_ReadStatus();
	if (!((status & 0x3C) == 0x3C)) {
		status |= 0x3C;
		SflsMx25l12835f_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter protection\n");
	}
}

void SflsMx25l12835f_ExitProtection(void)
{
	BYTE status = SflsMx25l12835f_ReadStatus();
	if (status & 0x3C) {
		status &= ~0x3C;
		SflsMx25l12835f_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit protection\n");
	}
}

UINT SflsMx25l12835f_IsProtection(void)
{
	BYTE status = SflsMx25l12835f_ReadStatus();
	if (status & 0x3C) {
		return 1;
	} else {
		return 0;
	}
}
#endif
