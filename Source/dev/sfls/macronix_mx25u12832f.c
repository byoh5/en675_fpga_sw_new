#include "dev.h"

#ifdef __SFLS_MX25U12832F__
#include "macronix_mx25u12832f.h"

static SFLScontrol sfls_mx25u12832f = {
	.ior_cmd = MX25U12832F_IOR_CMD,
	.ior_adr = MX25U12832F_IOR_ADR,
	.ior_dat = MX25U12832F_IOR_DAT,
	.iow_cmd = MX25U12832F_IOW_CMD,
	.iow_adr = MX25U12832F_IOW_ADR,
	.iow_dat = MX25U12832F_IOW_DAT,
	.iowe_cmd = MX25U12832F_IOWE_CMD,
	.iors_cmd = MX25U12832F_IORS_CMD,
	.iors_dat = MX25U12832F_IORS_DAT,

	.cmd_read = MX25U12832F_CMD_FAST_READ,
	.cmd_page_program = MX25U12832F_CMD_PAGE_PROGRAM,
	.cmd_write_enable = MX25U12832F_CMD_WRITE_ENABLE,
	.cmd_read_status = MX25U12832F_CMD_READ_STATUS,

	.gap = MX25U12832F_READ_GAP,
	.rdltc = MX25U12832F_RDLTC,

	.func_init_1 = SflsMx25u12832f_Init1,
	.func_init_2 = SflsMx25u12832f_Init2,
	.func_write_enable = SflsMx25u12832f_WriteEnable,
	.func_secter_erase = SflsMx25u12832f_SecterErase,
	.func_32k_erase = SflsMx25u12832f_32kErase,
	.func_64k_erase = SflsMx25u12832f_64kErase,
	.func_chip_erase = SflsMx25u12832f_ChipErase,
	.func_read_status = SflsMx25u12832f_ReadStatus,
	.func_write_status = SflsMx25u12832f_WriteStatus,
	.func_read_sfdp = SflsMx25u12832f_ReadSFDP,
	.func_enter_4b = 0,
	.func_exit_4b = 0,
#if (MX25U12832F_ENABLE_QPI==1)
	.func_enter_qpi = SflsMx25u12832f_EnterQPI,
	.func_exit_qpi = SflsMx25u12832f_ExitQPI,
#else
	.func_enter_qpi = 0,
	.func_exit_qpi = 0,
#endif
#if (MX25U12832F_ENABLE_QE==1)
	.func_enter_qe = SflsMx25u12832f_EnterQE,
	.func_exit_qe = SflsMx25u12832f_ExitQE,
#else
	.func_enter_qe = 0,
	.func_exit_qe = 0,
#endif
	.func_enter_protection = SflsMx25u12832f_EnterProtection,
	.func_exit_protection = SflsMx25u12832f_ExitProtection,
};

void SflsMx25u12832f_Init1(void)
{

}

void SflsMx25u12832f_Init2(void)
{

}

void SflsMx25u12832f_WriteEnable(void)
{
	SFLS_USR_CMD_MODE	= sfls_mx25u12832f.iowe_cmd;
	SFLS_USR_ADR_EN 	= 0;
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_BUSY_EN 	= 0;
	SFLS_USR_CMD 		= sfls_mx25u12832f.cmd_write_enable;
	SFLS_USR_REQ 		= 1;
}

static void SflsMx25u12832f_Erase(SFLS_ERASE erase_type, UINT addr)
{
	SFLS_USR_CMD_MODE 	= sfls_mx25u12832f.iow_cmd;
	if (erase_type == SFLS_E_ALL) {
		SFLS_USR_ADR_MODE= 0;
		SFLS_USR_ADR_EN	= 0;
		SFLS_USR_ADR	= 0;
	} else {
		SFLS_USR_ADR_MODE= sfls_mx25u12832f.iow_adr;
		SFLS_USR_ADR_EN	= 1;
		SFLS_USR_ADR	= addr;
	}
	SFLS_USR_GAP_EN 	= 0;
	SFLS_USR_RD_EN 		= 0;
	SFLS_USR_WR_EN 		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_ADR_EXT 	= 0;
	SFLS_USR_BUSY_EN 	= 1;
	SFLS_USR_RDREG_CMD_MODE = sfls_mx25u12832f.iors_cmd;
	SFLS_USR_RDREG_DAT_MODE	= sfls_mx25u12832f.iors_dat;
	switch (erase_type) {
	case SFLS_E_ALL:
		SFLS_USR_CMD	= MX25U12832F_CMD_CHIP_ERASE;
		break;
	case SFLS_E_4K:
		SFLS_USR_CMD	= MX25U12832F_CMD_SECTOR_ERASE;
		break;
	case SFLS_E_32K:
		SFLS_USR_CMD	= MX25U12832F_CMD_32KB_ERASE;
		break;
	case SFLS_E_64K:
		SFLS_USR_CMD	= MX25U12832F_CMD_64KB_ERASE;
		break;
	}
	SFLS_USR_REQ 		= 1;
}

void SflsMx25u12832f_SecterErase(UINT addr)
{
	SflsMx25u12832f_Erase(SFLS_E_4K, addr);
}

void SflsMx25u12832f_32kErase(UINT addr)
{
	SflsMx25u12832f_Erase(SFLS_E_32K, addr);
}

void SflsMx25u12832f_64kErase(UINT addr)
{
	SflsMx25u12832f_Erase(SFLS_E_64K, addr);
}

void SflsMx25u12832f_ChipErase(UINT addr)
{
	SflsMx25u12832f_Erase(SFLS_E_ALL, addr);
}

SFLScontrol *SflsMx25u12832f_GetControl(void)
{
	return &sfls_mx25u12832f;
}

void SflsMx25u12832f_ReadSFDP(BYTE *sfdp_bin)
{
	UINT *data = (UINT *)sfdp_bin;

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
	SFLS_USR_CMD		= 0x5A;
	SFLS_USR_WRDAT		= 0;
	SFLS_USR_RDLTC		= 1;

	for (UINT i = 0; i < (256 / 4); i++) {
		SFLS_USR_ADR	= i * 4;
		SFLS_USR_REQ	= 1;
		while (SFLS_USR_REQ);
		data[i] = ENX_HTONL(SFLS_USR_RDDAT);
	}
}

BYTE SflsMx25u12832f_ReadStatus(void)
{
	SFLS_USR_CMD_MODE	= sfls_mx25u12832f.iors_cmd;
	SFLS_USR_DAT_MODE	= sfls_mx25u12832f.iors_dat;
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_LEN		= 0;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= MX25U12832F_CMD_READ_STATUS;
	SFLS_USR_RDLTC		= SFLS_IO_RDLTC;
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

void SflsMx25u12832f_WriteStatus(BYTE status)
{
	SflsMx25u12832f_WriteEnable();
	while (SFLS_USR_REQ);

	SFLS_USR_CMD_MODE	= sfls_mx25u12832f.iors_cmd;
	SFLS_USR_DAT_MODE	= sfls_mx25u12832f.iors_dat;
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_GAP 		= 0;
	SFLS_USR_RD_EN		= 0;
	SFLS_USR_WR_EN		= 1;
	SFLS_USR_LEN		= 0;
	SFLS_USR_BUSY_EN	= 1;
	SFLS_USR_CMD		= MX25U12832F_CMD_WRITE_STATUS;
	SFLS_USR_WRDAT		= status << 24;
	SFLS_USR_RDLTC		= SFLS_IO_RDLTC;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
//	printf("CMD%02X: SFLS-WD:0x%08X, MODE(%d,%d)\n", SFLS_USR_CMD, status, SFLS_USR_CMD_MODE, SFLS_USR_DAT_MODE);
}

#if (MX25U12832F_ENABLE_QPI==1)
void SflsMx25u12832f_EnterQPI(void)
{
	SflsSingleCommand(MX25U12832F_CMD_ENTER_QPI);
}

void SflsMx25u12832f_ExitQPI(void)
{
	SflsSingleCommand(MX25U12832F_CMD_EXIT_QPI);
}
#endif

#if (MX25U12832F_ENABLE_QE==1)
void SflsMx25u12832f_EnterQE(void)
{
	BYTE status = SflsMx25u12832f_ReadStatus();
	if (!((status & 0x40) == 0x40)) {
		status |= 0x40;
		SflsMx25u12832f_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter QE\n");
	}
}

void SflsMx25u12832f_ExitQE(void)
{
	BYTE status = SflsMx25u12832f_ReadStatus();
	if (status & 0x40) {
		status &= ~0x40;
		SflsMx25u12832f_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit QE\n");
	}
}
#endif

void SflsMx25u12832f_EnterProtection(void)
{
	BYTE status = SflsMx25u12832f_ReadStatus();
	if (!((status & 0x3C) == 0x3C)) {
		status |= 0x3C;
		SflsMx25u12832f_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already enter protection\n");
	}
}

void SflsMx25u12832f_ExitProtection(void)
{
	BYTE status = SflsMx25u12832f_ReadStatus();
	if (status & 0x3C) {
		status &= ~0x3C;
		SflsMx25u12832f_WriteStatus(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "already exit protection\n");
	}
}
#endif
