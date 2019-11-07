#include "dev.h"
#include "sfls_model.h"

SFLScontrol *sfls_master = NULL;

void SflsInit(void)
{
	while (!SFLS_ID_VAL);

	SflsGetinfo();
	if (sfls_master == NULL) {
		return;
	}

	SFLS_IO_CLKDIV = 1;

	SFLS_BUS_RD_CMD_MODE = sfls_master->ior_cmd;
	SFLS_BUS_RD_ADR_MODE = sfls_master->ior_adr;
	SFLS_BUS_RD_DAT_MODE = sfls_master->ior_dat;
	SFLS_BUS_WR_CMD_MODE = sfls_master->iow_cmd;
	SFLS_BUS_WR_ADR_MODE = sfls_master->iow_adr;
	SFLS_BUS_WR_DAT_MODE = sfls_master->iow_dat;
	SFLS_BUS_WREN_CMD_MODE = sfls_master->iowe_cmd;
	SFLS_BUS_RDREG_CMD_MODE = sfls_master->iors_cmd;
	SFLS_BUS_RDREG_DAT_MODE = sfls_master->iors_dat;

	SFLS_BUS_CMD_RD = sfls_master->cmd_read;
	SFLS_BUS_CMD_WR = sfls_master->cmd_page_program;
	SFLS_BUS_CMD_WREN = sfls_master->cmd_write_enable;
	SFLS_BUS_CMD_RDREG = sfls_master->cmd_read_status;

	if (sfls_master->gap) {
		SFLS_BUS_GAP_EN	= 1;
		SFLS_BUS_GAP = sfls_master->gap;
	} else {
		SFLS_BUS_GAP_EN	= 0;
		SFLS_BUS_GAP = 0;
	}
	SFLS_IO_RDLTC = sfls_master->rdltc;

	if (sfls_master->cmd_enter_qpi) {
		SFLS_USR_CMD = sfls_master->cmd_enter_qpi;
		SFLS_USRBUS_REQ = 1;
	} else {
		SFLS_BUS_REQ = 1;
	}
	while (SFLS_BUS_REQ);

	SFLS_USR_CMD_MODE = 0;
	SFLS_USR_ADR_MODE = 0;
	SFLS_USR_DAT_MODE = 0;
	SFLS_USR_RDREG_CMD_MODE = sfls_master->iors_cmd;
	SFLS_USR_RDREG_DAT_MODE = sfls_master->iors_dat;
	SFLS_USR_RDLTC = sfls_master->rdltc;

	if (sfls_master->func_init) {
		sfls_master->func_init();
	}
}

UINT SflsGetinfo(void)
{
	printf("%c: SFLS_ID Get[0x%08X]\n", SFLS_ID_VAL == 1 ? 'O' : 'X', SFLS_ID);
	BYTE u8Manid = SFLS_ID >> 16 & 0xff;
	BYTE u8Type = SFLS_ID >> 8 & 0xff;
	BYTE u8Capa = SFLS_ID & 0xff;
	printf("ID(%02X) Type(%02X) Capa(%02X)\n", u8Manid, u8Type, u8Capa);

	switch (u8Manid) {
	case 0x1C: // EON
		sfls_master = SflsModelEON(u8Type, u8Capa);
		break;
	case 0x20: // Micron
		sfls_master = SflsModelMicron(u8Type, u8Capa);
		break;
	case 0x9D: // ISSI
		sfls_master = SflsModelISSI(u8Type, u8Capa);
		break;
	case 0x9F: // Macronix
		sfls_master = SflsModelMacronix(u8Type, u8Capa);
		break;
	case 0xC8: // GigaDevice
		sfls_master = SflsModelGigaDevice(u8Type, u8Capa);
		break;
	case 0xEF: // Winbond
		sfls_master = SflsModelWinbond(u8Type, u8Capa);
		break;
	case 0xFF:
		printf("SFLS No connection.\n");
		break;
	default:
		printf("SFLS Error.\n");
		//ENX_ASSERT(0);
		break;
	}

	return SFLS_ID;
}

void SflsWriteEnable(void)
{
	if (sfls_master->func_write_enable) {
		sfls_master->func_write_enable();
	}
}

void SflsSectErase(UINT addr, ENX_YN sync)
{
	while (SFLS_USR_REQ);
	SflsWriteEnable();
	while (SFLS_USR_REQ);
	sfls_master->func_secter_erase(addr);
	if (sync == ENX_YES) {
		while (SFLS_USR_REQ);
	}
}

void Sfls32KErase(UINT addr, ENX_YN sync)
{
	while (SFLS_USR_REQ);
	SflsWriteEnable();
	while (SFLS_USR_REQ);
	sfls_master->func_32k_erase(addr);
	if (sync == ENX_YES) {
		while (SFLS_USR_REQ);
	}
}

void Sfls64KErase(UINT addr, ENX_YN sync)
{
	while (SFLS_USR_REQ);
	SflsWriteEnable();
	while (SFLS_USR_REQ);
	sfls_master->func_64k_erase(addr);
	if (sync == ENX_YES) {
		while (SFLS_USR_REQ);
	}
}

void SflsChipErase(ENX_YN sync)
{
	while (SFLS_USR_REQ);
	SflsWriteEnable();
	while (SFLS_USR_REQ);
	sfls_master->func_chip_erase(0);
	if (sync == ENX_YES) {
		while (SFLS_USR_REQ);
	}
}

UINT SflsGetUsrreq(void)
{
	return SFLS_USR_REQ;
}

#if 1
void SflsWriteReg(BYTE wrdata, BYTE len, BYTE iomode)
{
	SflsWriteEnable();

	SFLS_USR_CMD_MODE	= iomode;
//	SFLS_USR_ADR_MODE	= 0;
	SFLS_USR_DAT_MODE	= iomode;
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_RD_EN		= 0;
	SFLS_USR_WR_EN		= 1;
	SFLS_USR_GAP		= 0;
	SFLS_USR_LEN		= len - 1;
	SFLS_USR_ADR_EXT	= 0;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= 0x01;
	SFLS_USR_ADR		= 0;
	SFLS_USR_WRDAT		= wrdata;
	SFLS_USR_RDLTC		= SFLS_IO_RDLTC;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
}

UINT SflsReadReg(BYTE cmd, BYTE iomode, BYTE gap)
{
	SFLS_USR_CMD_MODE	= iomode;
//	SFLS_USR_ADR_MODE	= 0;
	SFLS_USR_DAT_MODE	= iomode;
	SFLS_USR_ADR_EN		= 0;
	if (gap) {
		SFLS_USR_GAP_EN	= 1;
	} else {
		SFLS_USR_GAP_EN	= 0;
	}
	SFLS_USR_RD_EN		= 1;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_GAP		= gap;
	SFLS_USR_LEN		= 2;
	SFLS_USR_ADR_EXT	= 0;
	SFLS_USR_BUSY_EN	= 0;
//	SFLS_USR_BUSY_KEEP	= 0;
	SFLS_USR_CMD		= cmd;
	SFLS_USR_ADR		= 0;
	SFLS_USR_WRDAT		= 0;
	SFLS_USR_RDLTC		= SFLS_IO_RDLTC;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
	return SFLS_USR_RDDAT;
}
#endif

void SflsRegShow(ENX_YN isDetail)
{
	_Cprintf("SFLS Register View\n");
	printf("========================================\n");
	if (isDetail == ENX_YES) {
		_Yprintf(" 0:0x%08X\n", _cm(REG_BASE_SFLS, 0));
		printf("   %-20s: 0x%06X, %u\n", "ID", SFLS_ID, SFLS_ID);
		printf("   %-20s: %u\n", "IF_RST", SFLS_IF_RST);
		printf("   %-20s: %u\n", "ID_VAL", SFLS_ID_VAL);
		_Yprintf(" 1:0x%08X\n", _cm(REG_BASE_SFLS, 1));
		printf("   %-20s: 0x%02X, %u\n", "BUS_CMD_RD", SFLS_BUS_CMD_RD, SFLS_BUS_CMD_RD);
		printf("   %-20s: 0x%02X, %u\n", "BUS_CMD_WR", SFLS_BUS_CMD_WR, SFLS_BUS_CMD_WR);
		printf("   %-20s: 0x%02X, %u\n", "BUS_CMD_WREN", SFLS_BUS_CMD_WREN, SFLS_BUS_CMD_WREN);
		printf("   %-20s: 0x%02X, %u\n", "BUS_CMD_RDREG", SFLS_BUS_CMD_RDREG, SFLS_BUS_CMD_RDREG);
		_Yprintf(" 2:0x%08X\n", _cm(REG_BASE_SFLS, 2));
		printf("   %-20s: %u\n", "BUS_RD_CMD_MODE", SFLS_BUS_RD_CMD_MODE);
		printf("   %-20s: %u\n", "BUS_RD_ADR_MODE", SFLS_BUS_RD_ADR_MODE);
		printf("   %-20s: %u\n", "BUS_RD_DAT_MODE", SFLS_BUS_RD_DAT_MODE);
		printf("   %-20s: %u\n", "BUS_WR_CMD_MODE", SFLS_BUS_WR_CMD_MODE);
		printf("   %-20s: %u\n", "BUS_WR_ADR_MODE", SFLS_BUS_WR_ADR_MODE);
		printf("   %-20s: %u\n", "BUS_WR_DAT_MODE", SFLS_BUS_WR_DAT_MODE);
		printf("   %-20s: %u\n", "BUS_WREN_CMD_MODE", SFLS_BUS_WREN_CMD_MODE);
		printf("   %-20s: %u\n", "BUS_RDREG_CMD_MODE", SFLS_BUS_RDREG_CMD_MODE);
		printf("   %-20s: %u\n", "BUS_RDREG_DAT_MODE", SFLS_BUS_RDREG_DAT_MODE);
		printf("   %-20s: %u\n", "BUS_ADR_EXT", SFLS_BUS_ADR_EXT);
		printf("   %-20s: %u\n", "BUS_GAP_EN", SFLS_BUS_GAP_EN);
		printf("   %-20s: %u\n", "BUS_GAP", SFLS_BUS_GAP);
		_Yprintf(" 3:0x%08X\n", _cm(REG_BASE_SFLS, 3));
		printf("   %-20s: 0x%02X, %u\n", "USR_CMD_RDREG", SFLS_USR_CMD_RDREG, SFLS_USR_CMD_RDREG);
		printf("   %-20s: %u\n", "USR_RDREG_CMD_MODE", SFLS_USR_RDREG_CMD_MODE);
		printf("   %-20s: %u\n", "USR_RDREG_DAT_MODE", SFLS_USR_RDREG_DAT_MODE);
		_Yprintf(" 4:0x%08X\n", _cm(REG_BASE_SFLS, 4));
		printf("   %-20s: %u\n", "USR_CMD_MODE", SFLS_USR_CMD_MODE);
		printf("   %-20s: %u\n", "USR_ADR_MODE", SFLS_USR_ADR_MODE);
		printf("   %-20s: %u\n", "USR_DAT_MODE", SFLS_USR_DAT_MODE);
		printf("   %-20s: %u\n", "USR_ADR_EN", SFLS_USR_ADR_EN);
		printf("   %-20s: %u\n", "USR_GAP_EN", SFLS_USR_GAP_EN);
		printf("   %-20s: %u\n", "USR_RD_EN", SFLS_USR_RD_EN);
		printf("   %-20s: %u\n", "USR_WR_EN", SFLS_USR_WR_EN);
		printf("   %-20s: %u\n", "USR_GAP", SFLS_USR_GAP);
		printf("   %-20s: %u\n", "USR_LEN", SFLS_USR_LEN);
		printf("   %-20s: %u\n", "USR_ADR_EXT", SFLS_USR_ADR_EXT);
		printf("   %-20s: %u\n", "USR_BUSY_EN", SFLS_USR_BUSY_EN);
		printf("   %-20s: %u\n", "USR_RDLTC", SFLS_USR_RDLTC);
		printf("   %-20s: 0x%02X, %u\n", "USR_CMD", SFLS_USR_CMD, SFLS_USR_CMD);
		_Yprintf(" 5:0x%08X\n", _cm(REG_BASE_SFLS, 5));
		printf("   %-20s: 0x%08X, %u\n", "USR_ADR", SFLS_USR_ADR, SFLS_USR_ADR);
		_Yprintf(" 6:0x%08X\n", _cm(REG_BASE_SFLS, 6));
		printf("   %-20s: 0x%08X, %u\n", "USR_RDDAT", SFLS_USR_RDDAT, SFLS_USR_RDDAT);
		_Yprintf(" 7:0x%08X\n", _cm(REG_BASE_SFLS, 7));
		printf("   %-20s: 0x%08X, %u\n", "USR_WRDAT", SFLS_USR_WRDAT, SFLS_USR_WRDAT);
		_Yprintf(" 8:0x%08X\n", _cm(REG_BASE_SFLS, 8));
		printf("   %-20s: %u\n", "IO_RXEDGE", SFLS_IO_RXEDGE);
		printf("   %-20s: %u\n", "IO_TXEDGE", SFLS_IO_TXEDGE);
		printf("   %-20s: %u\n", "IO_RDLTC", SFLS_IO_RDLTC);
		printf("   %-20s: %u\n", "IO_CLKDIV", SFLS_IO_CLKDIV);
		printf("   %-20s: %u\n", "IO_CS_GAP", SFLS_IO_CS_GAP);
		printf("   %-20s: %u\n", "USRBUS_REQ", SFLS_USRBUS_REQ);
		printf("   %-20s: %u\n", "BUS_REQ", SFLS_BUS_REQ);
		printf("   %-20s: %u\n", "USR_REQ", SFLS_USR_REQ);
	} else {
		printf("SFLS  0:0x%08X  1:0x%08X  2:0x%08X  3:0x%08X  4:0x%08X\n", _cm(REG_BASE_SFLS, 0), _cm(REG_BASE_SFLS, 1), _cm(REG_BASE_SFLS, 2), _cm(REG_BASE_SFLS, 3), _cm(REG_BASE_SFLS, 4));
		printf("SFLS  6:0x%08X  7:0x%08X  8:0x%08X\n", _cm(REG_BASE_SFLS, 5), _cm(REG_BASE_SFLS, 6), _cm(REG_BASE_SFLS, 7), _cm(REG_BASE_SFLS, 8));
	}
	printf("========================================\n");
}
