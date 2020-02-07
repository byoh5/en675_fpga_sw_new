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

#if 1 // bootloader only
	UINT sfls_clk = 20000000;
	SflsSetClk(sfls_clk);

	if (sfls_master->func_init_1) {
		sfls_master->func_init_1();
	}

	if (sfls_master->func_enter_qe) {
		sfls_master->func_enter_qe();
	}

	if (sfls_master->func_enter_4b) {
		sfls_master->func_enter_4b();
		SFLS_BUS_ADR_EXT = 1;
		SFLS_USR_ADR_EXT = 1;
	}

	if (sfls_master->func_enter_qpi) {
		sfls_master->func_enter_qpi();
	}

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
		SFLS_BUS_GAP = sfls_master->gap - 1;
	} else {
		SFLS_BUS_GAP_EN	= 0;
		SFLS_BUS_GAP = 0;
	}
	SFLS_IO_RDLTC = sfls_master->rdltc;

	SFLS_BUS_REQ = 1;
	while (SFLS_BUS_REQ);

	SFLS_USR_CMD_MODE = 0;
	SFLS_USR_ADR_MODE = 0;
	SFLS_USR_DAT_MODE = 0;
	SFLS_USR_RDREG_CMD_MODE = sfls_master->iors_cmd;
	SFLS_USR_RDREG_DAT_MODE = sfls_master->iors_dat;
	SFLS_USR_RDLTC = sfls_master->rdltc;

	if (sfls_master->func_init_2) {
		sfls_master->func_init_2();
	}
#endif
}

void SflsSetClkdiv(UINT Clkdiv)
{
	SFLS_IO_CLKDIV = Clkdiv;
}

UINT SflsGetClkdiv(void)
{
	return SFLS_IO_CLKDIV;
}

void SflsSetClk(UINT Speed_Hz)
{
	UINT u32Cal = (APB_FREQ / (Speed_Hz * 2)) - 1;
	if (u32Cal < 256) {
		SFLS_IO_CLKDIV = u32Cal;
	} else {
		if (u32Cal == 0xFFFFFFFF) {
			SFLS_IO_CLKDIV = 0;
		} else {
			SFLS_IO_CLKDIV = 255;
		}
		ENX_DEBUGF(DBG_SFLS_WARNING, "SFLS Clk %s.(%u/%u)\n", SFLS_IO_CLKDIV ? "Min" : "Max", u32Cal, SFLS_IO_CLKDIV);
	}
}

UINT SflsGetClk(void)
{
	ENX_DEBUGF(DBG_SFLS_LOG, "SFLS Init - %uHz(%u)\n", APB_FREQ / ((SFLS_IO_CLKDIV + 1) * 2), SFLS_IO_CLKDIV);
	return APB_FREQ / ((SFLS_IO_CLKDIV + 1) * 2);
}

UINT SflsGetinfo(void)
{
	printf("%c: SFLS_ID Get[0x%08X]\n", SFLS_ID_VAL == 1 ? 'O' : 'X', SFLS_ID);
	BYTE u8Manid = SFLS_ID >> 16 & 0xff;
	BYTE u8Type = SFLS_ID >> 8 & 0xff;
	BYTE u8Capa = SFLS_ID & 0xff;
	printf("ID(%02X) Type(%02X) Capa(%02X) - ", u8Manid, u8Type, u8Capa);

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
	case 0x9F: // Macronix (3.3v)
	case 0xC2: // Macronix (1.8v)
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

UINT SflsGetSize(void)
{
	if (sfls_master) {
		return sfls_master->size;
	}
	return 0;
}

void SflsWriteEnable(void)
{
	if (sfls_master && sfls_master->func_write_enable) {
		sfls_master->func_write_enable();
	}
}

void SflsSectErase(UINT addr, ENX_YN sync)
{
	while (SFLS_USR_REQ);
	SflsWriteEnable();
	while (SFLS_USR_REQ);
	if (sfls_master && sfls_master->func_secter_erase) {
		sfls_master->func_secter_erase(addr);
	}
	if (sync == ENX_YES) {
		while (SFLS_USR_REQ);
	}
}

void Sfls32KErase(UINT addr, ENX_YN sync)
{
	while (SFLS_USR_REQ);
	SflsWriteEnable();
	while (SFLS_USR_REQ);
	if (sfls_master && sfls_master->func_32k_erase) {
		sfls_master->func_32k_erase(addr);
	}
	if (sync == ENX_YES) {
		while (SFLS_USR_REQ);
	}
}

void Sfls64KErase(UINT addr, ENX_YN sync)
{
	while (SFLS_USR_REQ);
	SflsWriteEnable();
	while (SFLS_USR_REQ);
	if (sfls_master && sfls_master->func_64k_erase) {
		sfls_master->func_64k_erase(addr);
	}
	if (sync == ENX_YES) {
		while (SFLS_USR_REQ);
	}
}

void SflsChipErase(ENX_YN sync)
{
	while (SFLS_USR_REQ);
	SflsWriteEnable();
	while (SFLS_USR_REQ);
	if (sfls_master && sfls_master->func_chip_erase) {
		sfls_master->func_chip_erase(0);
	}
	if (sync == ENX_YES) {
		while (SFLS_USR_REQ);
	}
}

UINT SflsGetUsrreq(void)
{
	return SFLS_USR_REQ;
}

void SflsSingleCommand(BYTE cmd, UINT CMD_MODE)
{
	SFLS_USR_CMD_MODE	= CMD_MODE;
	SFLS_USR_ADR_EN		= 0;
	SFLS_USR_GAP_EN		= 0;
	SFLS_USR_RD_EN		= 0;
	SFLS_USR_WR_EN		= 0;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= cmd;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
}

void SflsWriteStatus(BYTE status)
{
	if (sfls_master && sfls_master->func_write_status) {
		sfls_master->func_write_status(status);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "WriteStatus is not defined.\n");
	}
}

BYTE SflsReadStatus(void)
{
	BYTE res = 0;
	if (sfls_master && sfls_master->func_read_status) {
		res = sfls_master->func_read_status();
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "ReadStatus is not defined.\n");
	}
	return res;
}

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
	SFLS_USR_ADR_EXT	= SFLS_BUS_ADR_EXT;
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
	SFLS_USR_ADR_EXT	= SFLS_BUS_ADR_EXT;
	SFLS_USR_BUSY_EN	= 0;
	SFLS_USR_CMD		= cmd;
	SFLS_USR_ADR		= 0;
	SFLS_USR_WRDAT		= 0;
	SFLS_USR_RDLTC		= SFLS_IO_RDLTC;
	SFLS_USR_REQ		= 1;
	while (SFLS_USR_REQ);
	return SFLS_USR_RDDAT;
}

void SflsEnter4B(void)
{
	if (sfls_master && sfls_master->func_enter_4b) {
		sfls_master->func_enter_4b();
		SFLS_USR_ADR_EXT = 1;
		SFLS_BUS_ADR_EXT = 1;
		SFLS_BUS_REQ = 1;
		while (SFLS_BUS_REQ);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "Enter4B is not defined.\n");
	}
}

void SflsExit4B(void)
{
	if (sfls_master && sfls_master->func_exit_4b) {
		sfls_master->func_exit_4b();
		SFLS_USR_ADR_EXT = 0;
		SFLS_BUS_ADR_EXT = 0;
		SFLS_BUS_REQ = 1;
		while (SFLS_BUS_REQ);
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "Exit4B is not defined.\n");
	}
}

void SflsEnterQPI(void)
{
	if (sfls_master && sfls_master->func_enter_qpi) {
		sfls_master->func_enter_qpi();
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "EnterQPI is not defined.\n");
	}
}

void SflsExitQPI(void)
{
	if (sfls_master && sfls_master->func_exit_qpi) {
		sfls_master->func_exit_qpi();
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "ExitQPI is not defined.\n");
	}
}

void SflsEnterQE(void)
{
	if (sfls_master && sfls_master->func_enter_qe) {
		sfls_master->func_enter_qe();
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "EnterQE is not defined.\n");
	}
}

void SflsExitQE(void)
{
	if (sfls_master && sfls_master->func_exit_qe) {
		sfls_master->func_exit_qe();
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "ExitQE is not defined.\n");
	}
}

void SflsEnterProtection(void)
{
	if (sfls_master && sfls_master->func_enter_protection) {
		sfls_master->func_enter_protection();
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "EnterProtection is not defined.\n");
	}
}

void SflsExitProtection(void)
{
	if (sfls_master && sfls_master->func_exit_protection) {
		sfls_master->func_exit_protection();
	} else {
		ENX_DEBUGF(DBG_SFLS_WARNING, "ExitProtection is not defined.\n");
	}
}

void SflsReadSFDP(BYTE *sfdp_bin)
{
	if (sfls_master && sfls_master->func_read_sfdp) {
		sfls_master->func_read_sfdp(sfdp_bin);
	} else {
		UINT *data = (UINT *)sfdp_bin;

		SFLS_USR_CMD_MODE	= SFLS_BUS_RD_CMD_MODE;
		SFLS_USR_ADR_MODE	= SFLS_BUS_RD_ADR_MODE;
		SFLS_USR_DAT_MODE	= SFLS_BUS_RD_DAT_MODE;
		SFLS_USR_ADR_EN		= 1;
		SFLS_USR_GAP_EN		= SFLS_BUS_GAP_EN;
		SFLS_USR_GAP		= SFLS_BUS_GAP;
		SFLS_USR_RD_EN		= 1;
		SFLS_USR_WR_EN		= 0;
		SFLS_USR_LEN		= 3;
		SFLS_USR_ADR_EXT	= SFLS_BUS_ADR_EXT;
		SFLS_USR_BUSY_EN	= 0;
		SFLS_USR_CMD		= 0x5A;
		SFLS_USR_WRDAT		= 0;
		SFLS_USR_RDLTC		= SFLS_IO_RDLTC;

		for (UINT i = 0; i < (256 / 4); i++) {
			SFLS_USR_ADR	= i * 4;
			SFLS_USR_REQ	= 1;
			while (SFLS_USR_REQ);
			data[i] = ENX_HTONL(SFLS_USR_RDDAT);
		}
	}
}

void SflsViewerSFDP(BYTE *sfdp_bin)
{
	hexDump("SFDP", (BYTE *)sfdp_bin, 256);

	SFLSsfdp_header *sfdphead = (SFLSsfdp_header *)sfdp_bin;
	if (sfdphead->SFDPSignature[0] == 'S' && sfdphead->SFDPSignature[1] == 'F' && sfdphead->SFDPSignature[2] == 'D' && sfdphead->SFDPSignature[3] == 'P') {
		SFLSsfdp *sfdp = (SFLSsfdp *)(sfdp_bin + sfdphead->ParameterTablePointer);

		printf("== %c%c%c%c ==\n", sfdphead->SFDPSignature[0], sfdphead->SFDPSignature[1], sfdphead->SFDPSignature[2], sfdphead->SFDPSignature[3]);
		printf("Parameter Table Length(%u) Pointer(0x%02X)\n", sfdphead->ParameterTableLength, sfdphead->ParameterTablePointer);

		printf("=====================\n");

		printf("Block / Sector Erase sizes : [%u]\n", sfdp->BlockSectorEreaseSize);
		printf("Write Granularity : [%u]\n", sfdp->WriteGranularity);
		printf("Write Enable Instruction Required for Writing to Volatile Status Register : [%u]\n", sfdp->WEIR_WVSR);
		printf("Write Enable Opcode Select for Writing to Volatile Status Register : [%u]\n", sfdp->WEOS_WVSR);
		printf("4 Kilo-Byte Erase Opcode : [0x%02X]\n", sfdp->FourKBEraseOpcode);
		printf("Address Byte : [%u]\n", sfdp->AddrByte);
		printf("Supports Double Transfer Rate (DTR) Clocking : [%u]\n", sfdp->SupDTRClocking);
		printf("SFLS size: %uByte (raw:0x%08X)\n", (sfdp->FlashMemoryDensity + 1) / 8, sfdp->FlashMemoryDensity);

		printf("=====================\n");

		printf("Supports (1-4-4) Fast Read : [%u]\n", sfdp->Sup144FastRead);
		printf("(1-4-4) Fast Read Number of Wait states (dummy clocks) needed before valid output : [%u]\n", sfdp->DummyClock144FR);
		printf("(1-4-4) Fast Read Number of Mode Bits : [%u]\n", sfdp->Modebit144FR);
		printf("(1-4-4) Fast Read Opcode : [0x%02X]\n", sfdp->Op144FR);

		printf("Supports (1-1-4) Fast Read : [%u]\n", sfdp->Sup114FastRead);
		printf("(1-1-4) Fast Read Number of Wait states (dummy clocks) needed before valid output : [%u]\n", sfdp->DummyClock114FR);
		printf("(1-1-4) Fast Read Number of Mode Bits : [%u]\n", sfdp->Modebit114FR);
		printf("(1-1-4) Fast Read Opcode : [0x%02X]\n", sfdp->Op114FR);

		printf("Supports (1-1-2) Fast Read : [%u]\n", sfdp->Sup112FastRead);
		printf("(1-1-2) Fast Read Number of Wait states (dummy clocks) needed before valid output : [%u]\n", sfdp->DummyClock112FR);
		printf("(1-1-2) Fast Read Number of Mode Bits : [%u]\n", sfdp->Modebit112FR);
		printf("(1-1-2) Fast Read Opcode : [0x%02X]\n", sfdp->Op112FR);

		printf("Supports (1-2-2) Fast Read : [%u]\n", sfdp->Sup122FastRead);
		printf("(1-2-2) Fast Read Number of Wait states (dummy clocks) needed before valid output : [%u]\n", sfdp->DummyClock122FR);
		printf("(1-2-2) Fast Read Number of Mode Bits : [%u]\n", sfdp->Modebit122FR);
		printf("(1-2-2) Fast Read Opcode : [0x%02X]\n", sfdp->Op122FR);

		printf("Supports (2-2-2) Fast Read : [%u]\n", sfdp->Sup222FastRead);
		printf("(2-2-2) Fast Read Number of Wait states (dummy clocks) needed before valid output : [%u]\n", sfdp->DummyClock222FR);
		printf("(2-2-2) Fast Read Number of Mode Bits : [%u]\n", sfdp->Modebit222FR);
		printf("(2-2-2) Fast Read Opcode : [0x%02X]\n", sfdp->Op222FR);

		printf("Supports (4-4-4) Fast Read : [%u]\n", sfdp->Sup444FastRead);
		printf("(4-4-4) Fast Read Number of Wait states (dummy clocks) needed before valid output : [%u]\n", sfdp->DummyClock444FR);
		printf("(4-4-4) Fast Read Number of Mode Bits : [%u]\n", sfdp->Modebit444FR);
		printf("(4-4-4) Fast Read Opcode : [0x%02X]\n", sfdp->Op444FR);

		printf("Sector Type 1 Size   : [0x%02X]\n", sfdp->SectorType1Size);
		printf("Sector Type 1 Opcode : [0x%02X]\n", sfdp->SectorType1Op);
		printf("Sector Type 2 Size   : [0x%02X]\n", sfdp->SectorType2Size);
		printf("Sector Type 2 Opcode : [0x%02X]\n", sfdp->SectorType2Op);
		printf("Sector Type 3 Size   : [0x%02X]\n", sfdp->SectorType3Size);
		printf("Sector Type 3 Opcode : [0x%02X]\n", sfdp->SectorType3Op);
		printf("Sector Type 4 Size   : [0x%02X]\n", sfdp->SectorType4Size);
		printf("Sector Type 4 Opcode : [0x%02X]\n", sfdp->SectorType4Op);

		printf("================================\n");
	} else {
		_Rprintf("SFDP Data Error\n");
	}
}

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
