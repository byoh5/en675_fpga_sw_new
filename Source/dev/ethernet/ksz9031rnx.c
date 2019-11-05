#include "dev.h"

#ifdef __ETHERNET__
#ifdef __ETHPHY_KSZ9031RNX__
#include "enx_freertos.h"

#include "ethphy.h"
#include "ksz9031rnx.h"

#include "networkif.h"

#if DBG_ETHPHY_LOG
void EthphyVendorRegView(UINT Type, WORD Data)
{
	static char const *strEthphyRegName[16] = {	"REG_0x10", "RLOOPR_ADR", "LKMDR_ADR", "DPPSR_ADR",
												"REG_0x14", "RXERCR_ADR", "REG_0x16", "REG_0x17",
												"REG_0x18", "REG_0x19", "REG_0x1A", "ICSR_ADR",
												"AMDIXR_ADR", "REG_0x1D", "REG_0x1E", "PHYCR_ADR"};
	static char const *strEnDis[2] = {"Enable", "Disable"};
	static char const *strOccur[2] = {"Occurred", "Did not occur"};
	//static char const strNormalOp[] = "Normal operation";

	if (Type < 32) {
		printf("Type:ETHPHY_%s : (0x%04X)\n", strEthphyRegName[Type-16], Data);
	} else {
		printf("Type:Error?(0x%02X) : (0x%04X)\n", Type, Data);
	}
	if (DBG_ETHPHY_LOG & ENX_DBG_STATE) {
		printf("========================================\n");
		switch (Type) {
			case ETHPHY_RLOOPR_ADR:
				break;
			case ETHPHY_LKMDR_ADR:
				break;
			case ETHPHY_DPPSR_ADR:
				break;
			case ETHPHY_RXERCR_ADR:
				break;
			case ETHPHY_ICSR_ADR:
				printf("%41s: %s\n", "Jabber Interrupt En/Dis", Data & ETHPHY_ICSR_JIE ? strEnDis[0] : strEnDis[1]);
				printf("%41s: %s\n", "Receive Error Interrupt En/Dis", Data & ETHPHY_ICSR_REIE ? strEnDis[0] : strEnDis[1]);
				printf("%41s: %s\n", "Received Interrupt En/Dis", Data & ETHPHY_ICSR_PRIE ? strEnDis[0] : strEnDis[1]);
				printf("%41s: %s\n", "Parallel Detect Fault Interrupt En/Dis", Data & ETHPHY_ICSR_PDFIE ? strEnDis[0] : strEnDis[1]);
				printf("%41s: %s\n", "Link Partner Acknowledge Interrupt En/Dis", Data & ETHPHY_ICSR_LPAIE ? strEnDis[0] : strEnDis[1]);
				printf("%41s: %s\n", "Link-Down Interrupt En/Dis", Data & ETHPHY_ICSR_LDIE ? strEnDis[0] : strEnDis[1]);
				printf("%41s: %s\n", "Remote Fault Interrupt En/Dis", Data & ETHPHY_ICSR_RFIE ? strEnDis[0] : strEnDis[1]);
				printf("%41s: %s\n", "Link-Up Interrupt En/Dis", Data & ETHPHY_ICSR_LUIE ? strEnDis[0] : strEnDis[1]);
				printf("%41s: %s\n", "Jabber Interrupt", Data & ETHPHY_ICSR_JI ? strOccur[0] : strOccur[1]);
				printf("%41s: %s\n", "Receive Error Interrupt", Data & ETHPHY_ICSR_REI ? strOccur[0] : strOccur[1]);
				printf("%41s: %s\n", "Received Interrupt", Data & ETHPHY_ICSR_PRI ? strOccur[0] : strOccur[1]);
				printf("%41s: %s\n", "Parallel Detect Fault Interrupt", Data & ETHPHY_ICSR_PDFI ? strOccur[0] : strOccur[1]);
				printf("%41s: %s\n", "Link Partner Acknowledge Interrupt", Data & ETHPHY_ICSR_LPAI ? strOccur[0] : strOccur[1]);
				printf("%41s: %s\n", "Link-Down Interrupt", Data & ETHPHY_ICSR_LDI ? strOccur[0] : strOccur[1]);
				printf("%41s: %s\n", "Remote Fault Interrupt", Data & ETHPHY_ICSR_RFI ? strOccur[0] : strOccur[1]);
				printf("%41s: %s\n", "Link-Up Interrupt", Data & ETHPHY_ICSR_LUI ? strOccur[0] : strOccur[1]);
				break;
			case ETHPHY_AMDIXR_ADR:
				break;
			case ETHPHY_PHYCR_ADR:
				printf("%30s: Interrupt ping active %s\n", "Interrupt Level", Data & ETHPHY_PCR_INT_LEVEL ? "high" : "low");
				printf("%30s: %s jabber counter\n", "Enable Jabber", Data & ETHPHY_PCR_EN_JABBER ? strEnDis[0] : strEnDis[1]);
				printf("%30s: %s\n", "Speed Status 1000Base-T", Data & ETHPHY_PCR_SS_1000M ? "Indicate chip final speed status at 1000Bast-T" : "-");
				printf("%30s: %s\n", "Speed Status 100Base-TX", Data & ETHPHY_PCR_SS_100M ? "Indicate chip final speed status at 100Bast-TX" : "-");
				printf("%30s: %s\n", "Speed Status 10Base-T", Data & ETHPHY_PCR_SS_10M ? "Indicate chip final speed status at 10Bast-T" : "-");
				printf("%30s: Indicate chip final duplex status at %s-duplex\n", "Duplex Status", Data & ETHPHY_PCR_DUPLEX_STATUS ? "full" : "half");
				printf("%30s: Indicate chip master/slave status at 1000Base-T %s mode\n", "1000Base-T Master/Slave Status", Data & ETHPHY_PCR_1000MS_STATUS ? "master" : "slave");
				printf("%30s: Link is %s\n", "Link Status Check Fail", Data & ETHPHY_PCR_LINK_STATUS_CHK_FAIL ? "Fail" : "Not Failing");
				break;
		}
		printf("========================================\n");
	}
}

void EthphyRegView(UINT Type, WORD Data)
{
	static char const *strEthphyRegName[16] = {	"BCR_ADR", "BSR_ADR", "PI1R_ADR", "PI2R_ADR",
												"ANAR_ADR", "ANLPAR_ADR", "ANER_ADR", "ANNPR_ADR",
												"LPNPAR_ADR", "1000CR_ADR", "1000SR_ADR", "REG_0x0B",
												"REG_0x0C", "MMDCR_ADR", "MMDRDR_ADR", "ESR_ADR"};
	static char const *strEnDis[2] = {"Enable", "Disable"};
	//static char const *strOccur[2] = {"Occurred", "Did not occur"};
	static char const strNormalOp[] = "Normal operation";

	if (Type < 16) {
		printf("Type:ETHPHY_%s : (0x%04X)\n", strEthphyRegName[Type], Data);
	} else if (Type < 32) {
		EthphyVendorRegView(Type, Data);
		return;
	} else {
		printf("Type:Error?(0x%02X) : (0x%04X)\n", Type, Data);
	}
	if (DBG_ETHPHY_LOG & ENX_DBG_STATE) {
		printf("========================================\n");
		switch (Type) {
			case ETHPHY_BCR_ADR:
				printf("%26s: %s\n", "Reset", Data & ETHPHY_BCR_RESET ? "Software reset" : strNormalOp);
				printf("%26s: %s\n", "Power-Down", Data & ETHPHY_BCR_PWR_DOWN ? "Power-down mode" : strNormalOp);
				printf("%26s: %s\n", "Loopback", Data & ETHPHY_BCR_LOOPBACK ? "Loopback mode" : strNormalOp);
				printf("%26s: %s\n", "Isolate", Data & ETHPHY_BCR_PWR_DOWN ? "Electrical isolation of PHY from I/F" : strNormalOp);
				printf("%26s: %s auto-negotiation process\n", "Auto-Negotiation Enable", Data & ETHPHY_BCR_AUTONEG_EN ? strEnDis[0] : strEnDis[1]);
				printf("%26s: %s\n", "Restart Auto-Negotiation", Data & ETHPHY_BCR_RST_AUTONEG ? "Restart auto-negotiation process" : strNormalOp);
				printf("%26s: %s\n", "Speed Select", (Data & ETHPHY_BCR_SPEED_M ? (Data & ETHPHY_BCR_SPEED_L ? "Reserved" : "1000Mbps") : (Data & ETHPHY_BCR_SPEED_L ? "100Mbps" : "10Mbps")));
				printf("%26s: %s\n", "Duplex Mode", Data & ETHPHY_BCR_FULLDPLX ? "Full-duplex" : "Half-duplex");
				break;
			case ETHPHY_BSR_ADR:
				printf("%26s: %sT4 capable\n", "100Base-T4", Data & ETHPHY_BSR_100BT4 ? "" : "Not ");
				printf("%26s: %sapable of 100Mbps full-duplex\n", "100Base-TX Full-Duplex", Data & ETHPHY_BSR_100BTXFD ? "C" : "Not c");
				printf("%26s: %sapable of 100Mbps half-duplex\n", "100Base-TX Half-Duplex", Data & ETHPHY_BSR_100BTXHD ? "C" : "Not c");
				printf("%26s: %sapable of 10Mbps full-duplex\n", "10Base-T Full-Duplex", Data & ETHPHY_BSR_10BTFD ? "C" : "Not c");
				printf("%26s: %sapable of 10Mbps half-duplex\n", "10Base-T Half-Duplex", Data & ETHPHY_BSR_10BTHD ? "C" : "Not c");
				printf("%26s: %s\n", "Extended Status", Data & ETHPHY_BSR_ES ? "Extended status info" : "No extended status info");
				printf("%26s: %s\n", "No Preamble", Data & ETHPHY_BSR_NO_PREMBLE ? "Preamble suppression" : "Normal preamble");
				printf("%26s: Auto-negotiation process %scompleted\n", "Auto-Negotiation Complet", Data & ETHPHY_BSR_AUTONEG_CMPLT ? "" : "not ");
				printf("%26s: %semote fault\n", "Remote Fault", Data & ETHPHY_BSR_REMOTE_FAULT ? "R" : "No r");
				printf("%26s: Can%s perform auto-negotiation\n", "Auto-Negotiation Ability", Data & ETHPHY_BSR_AUTONEG_ABLE ? "" : "not");
				printf("%26s: Link is %s\n", "Link Status", Data & ETHPHY_BSR_LINK_STATUS ? "up" : "down");
				printf("%26s: Jabber %sdetect\n", "Jabber Detect", Data & ETHPHY_BSR_JABBER_DETECT ? "" : "not ");
				printf("%26s: %s\n", "Extended Capability", Data & ETHPHY_BSR_EX_CAPABILITY ? "Supports extended capability registers" : "?");
				break;
			case ETHPHY_PI1R_ADR:
				break;
			case ETHPHY_PI2R_ADR:
				break;
			case ETHPHY_ANAR_ADR:
				printf("%26s: %sNext page capable\n", "Next Page", Data & ETHPHY_ANAR_NEXT_PAGE ? "" : "No ");
				printf("%26s: %s\n", "Remote Fault", Data & ETHPHY_ANAR_REMOTE_FAULT ? "Remote fault supported" : "No remote fault");
				printf("%26s: %s\n", "Pause", (Data & ETHPHY_ANAR_ASY_PAUSE ? (Data & ETHPHY_ANAR_SY_PAUSE ? "Symmetric and asymmetric pause (local device)" : "Asymmetric pause (link partner)") : (Data & ETHPHY_ANAR_SY_PAUSE ? "Symmetric pause" : "No pause")));
				printf("%26s: %sT4 capable\n", "100Base-T4", Data & ETHPHY_ANAR_100BASE_T4 ? "" : "No ");
				printf("%26s: %s100Mbps full-duplex capable\n", "100Base-TX Full-Duplex", Data & ETHPHY_ANAR_100FULL ? "" : "No ");
				printf("%26s: %s100Mbps half-duplex capable\n", "100Base-TX Half-Duplex", Data & ETHPHY_ANAR_100HALF ? "" : "No ");
				printf("%26s: %s10Mbps full-duplex capable\n", "10Base-T Full-Duplex", Data & ETHPHY_ANAR_10FULL ? "" : "No ");
				printf("%26s: %s10Mbps half-duplex capable\n", "10Base-T Half-Duplex", Data & ETHPHY_ANAR_10HALF ? "" : "No ");
				es_printf("%26s: %05b\n", "Selector Field", Data & ETHPHY_ANAR_SELECTOR_FIELD);
				break;
			case ETHPHY_ANLPAR_ADR:
				printf("%26s: %sNext page capable\n", "Next Page", Data & ETHPHY_ANLPAR_NEXT_PAGE ? "" : "No ");
				printf("%26s: Link code word %s\n", "Acknowledge", Data & ETHPHY_ANLPAR_ACKNOWLEDGE ? "received from partner" : "not yet received");
				printf("%26s: %s\n", "Remote Fault", Data & ETHPHY_ANLPAR_REMOTE_FAULT ? "Remote fault detected" : "No remote fault");
				printf("%26s: %s\n", "Pause", (Data & ETHPHY_ANLPAR_ASY_PAUSE ? (Data & ETHPHY_ANLPAR_SY_PAUSE ? "Symmetric and asymmetric pause (local device)" : "Asymmetric pause (link partner)") : (Data & ETHPHY_ANLPAR_SY_PAUSE ? "Symmetric pause" : "No pause")));
				printf("%26s: %sT4 capable\n", "100Base-T4", Data & ETHPHY_ANLPAR_100BASE4 ? "" : "No ");
				printf("%26s: %s100Mbps full-duplex capable\n", "100Base-TX Full-Duplex", Data & ETHPHY_ANLPAR_100FULL ? "" : "No ");
				printf("%26s: %s100Mbps half-duplex capable\n", "100Base-TX Half-Duplex", Data & ETHPHY_ANLPAR_100HALF ? "" : "No ");
				printf("%26s: %s10Mbps full-duplex capable\n", "10Base-T Full-Duplex", Data & ETHPHY_ANLPAR_10FULL ? "" : "No ");
				printf("%26s: %s10Mbps half-duplex capable\n", "10Base-T Half-Duplex", Data & ETHPHY_ANLPAR_10HALF ? "" : "No ");
				es_printf("%26s: %05b\n", "Selector Field", Data & ETHPHY_ANLPAR_SELECTOR_FIELD);
				break;
			case ETHPHY_ANER_ADR:
				break;
			case ETHPHY_ANNPR_ADR:
				break;
			case ETHPHY_LPNPAR_ADR:
				break;
			case ETHPHY_1000CR_ADR:
				es_printf("%26s: %03b\n", "Test Mode Bits", Data & ETHPHY_GIGACR_TMB);
				printf("%26s: %sable master-slave manual configuration value\n", "Master-Slave Manual Configuration Enable", Data & ETHPHY_GIGACR_MSMCE ? "En" : "Dis");
				printf("%26s: Configure PHY as %s during master-slave negotiation\n", "Master-Slave Manual Configuration Value", Data & ETHPHY_GIGACR_MSMCV ? "master" : "slave");
				printf("%26s: Indicate the preference to operate as %s\n", "Port Type", Data & ETHPHY_GIGACR_PT ? "multiport device (master)" : "single-port device (slave)");
				printf("%26s: Advertise PHY is %s1000Base-T full-duplex capable\n", "1000Base-T Full-Duplex", Data & ETHPHY_GIGACR_1000FULL ? "" : "not ");
				printf("%26s: Advertise PHY is %s1000Base-T half-duplex capable\n", "1000Base-T Half-Duplex", Data & ETHPHY_GIGACR_1000HALF ? "" : "not ");
				break;
			case ETHPHY_1000SR_ADR:
				printf("%46s: %saster-slave configuration fault detected\n", "Master-Slave Configuration Fault", Data & ETHPHY_GIGASR_MSCF ? "M" : "No m");
				printf("%46s: Local PHY configuration resolved to %s\n", "Master-Slave Configuration Resolution", Data & ETHPHY_GIGASR_MSCR ? "master" : "slave");
				printf("%46s: Local receiver %sOK\n", "Local Receiver Status", Data & ETHPHY_GIGASR_LRS ? "" : "not ");
				printf("%46s: Remote receiver %sOK\n", "Remote Receiver Status", Data & ETHPHY_GIGASR_RRS ? "" : "not ");
				printf("%46s: Link partner is %scapable of 1000Base-T full-duplex\n", "Link Partner 1000Base-T Full-Duplex Capability", Data & ETHPHY_GIGASR_1000FULLC ? "" : "not ");
				printf("%46s: Link partner is %scapable of 1000Base-T half-duplex\n", "Link Partner 1000Base-T Half-Duplex Capability", Data & ETHPHY_GIGASR_1000HALFC ? "" : "not ");
				printf("%46s: %d\n", "Idle Error Count", Data & ETHPHY_GIGASR_IEC);
				break;
			case ETHPHY_MMDCR_ADR:
				break;
			case ETHPHY_MMDRDR_ADR:
				break;
			case ETHPHY_ESR_ADR:
				printf("%26s: PHY can%s perform 1000Base-X full-duplex\n", "1000Base-X Full-Duplex", Data & ETHPHY_ESR_1000XFULL ? "" : "not");
				printf("%26s: PHY can%s perform 1000Base-X half-duplex\n", "1000Base-X Half-Duplex", Data & ETHPHY_ESR_1000XHALF ? "" : "not");
				printf("%26s: PHY can%s perform 1000Base-T full-duplex\n", "1000Base-T Full-Duplex", Data & ETHPHY_ESR_1000TFULL ? "" : "not");
				printf("%26s: PHY can%s perform 1000Base-T half-duplex\n", "1000Base-T Half-Duplex", Data & ETHPHY_ESR_1000THALF ? "" : "not");
				break;
		}
		printf("========================================\n");
	}
}
#endif

void EthphyAutoNeg(ENX_SWITCH onoff)
{
	WORD wANAR = 0, wBCR = 0, w1000CR;

	if (onoff == ENX_ON) {
		MdioRead(ethphy_info.addr, ETHPHY_ANAR_ADR, &wANAR);	// Auto-Negotiation Advertisement: Read
#if 1 // (ETH_MAC_PAUSE)
		wANAR |= ETHPHY_ANAR_SY_PAUSE;							// Auto-Negotiation Advertisement: Enable symmetric pause
		wANAR &= ~ETHPHY_ANAR_ASY_PAUSE;						// Auto-Negotiation Advertisement: Disable asymmetric pause
#endif
		wANAR |= ETHPHY_ANAR_100FULL;
		wANAR |= ETHPHY_ANAR_100HALF;
		wANAR |= ETHPHY_ANAR_10FULL;
		wANAR |= ETHPHY_ANAR_10HALF;
		EthphyRegView(ETHPHY_ANAR_ADR, wANAR);
		MdioWrite(ethphy_info.addr, ETHPHY_ANAR_ADR, wANAR);	// Auto-Negotiation Advertisement: Write
#if 1
		MdioRead(ethphy_info.addr, ETHPHY_1000CR_ADR, &w1000CR);// 1000Base-T Control Register: Read
		w1000CR |= ETHPHY_GIGACR_1000FULL;
		w1000CR |= ETHPHY_GIGACR_1000HALF;
		EthphyRegView(ETHPHY_1000CR_ADR, w1000CR);
		MdioWrite(ethphy_info.addr, ETHPHY_1000CR_ADR, w1000CR);// 1000Base-T Control Register: Write
#endif
		wBCR |= ETHPHY_BCR_AUTONEG_EN; 							// Basic Control: Enable auto-negotiation process
		wBCR |= ETHPHY_BCR_RST_AUTONEG;							// Basic Control: Restart auto-negotiation process
		ENX_DEBUGF(DBG_ETHPHY_MSG, "Auto-Negotiation start.\n");
	} else {
		wBCR |= ETHPHY_BCR_SPEED_M;								// Basic Control: Set 1000Mbps
		wBCR |= ETHPHY_BCR_FULLDPLX;							// Basic Control: Set Full-duplex
		ENX_DEBUGF(DBG_ETHPHY_MSG, "Manual configuration.\n");
	}
	EthphyRegView(ETHPHY_BCR_ADR, wBCR);
	MdioWrite(ethphy_info.addr, ETHPHY_BCR_ADR, wBCR);			// Basic Control: Write
}

static void EthphyMMDWrite(BYTE u8Mode, WORD u16DevAddr, WORD u16Reg, WORD u16Data)
{
	MdioWrite(ethphy_info.addr, ETHPHY_MMDCR_ADR, u16DevAddr);
	MdioWrite(ethphy_info.addr, ETHPHY_MMDRDR_ADR, u16Reg);
	MdioWrite(ethphy_info.addr, ETHPHY_MMDCR_ADR, (u8Mode << 14) | u16DevAddr);
	MdioWrite(ethphy_info.addr, ETHPHY_MMDRDR_ADR, u16Data);
}

static void EthphyMMDRead(BYTE u8Mode, WORD u16DevAddr, WORD u16Reg, WORD *u16Data)
{
	MdioWrite(ethphy_info.addr, ETHPHY_MMDCR_ADR, u16DevAddr);
	MdioWrite(ethphy_info.addr, ETHPHY_MMDRDR_ADR, u16Reg);
	MdioWrite(ethphy_info.addr, ETHPHY_MMDCR_ADR, (u8Mode << 14) | u16DevAddr);
	MdioRead(ethphy_info.addr, ETHPHY_MMDRDR_ADR, u16Data);
}

ENX_OKFAIL EthphySetting(void)
{
	// Interrupt Control(Enable link-down, link-up interrupt)
	MdioWrite(ethphy_info.addr, ETHPHY_ICSR_ADR, ETHPHY_ICSR_LUIE | ETHPHY_ICSR_LDIE);

/* KSZ9031RNX Revision 2.2 page 72.(May 14, 2015)
The KSZ9031RNX fast link pulse(FLP) burst-toburst transmit timing for Auto-Negotiation defaults to 8ms.
IEEE 802.3 standard specifies this timing to be 16ms +/-8ms.
Some PHY link partners need to receive the FLP with 16ms centered timing; otherwise, there can be intermittent link failures and long link-up times.
after KSZ9031RNX power-up/reset, program the following register sequence to set the FLP timing to 16ms.
the above setting for 16ms FLP transmit timing is compatible with all PHY link partners. */
	EthphyMMDWrite(1, 0, 0x0004, 0x0006);
	EthphyMMDWrite(1, 0, 0x0003, 0x1A80);

	EthphyAutoNeg(ETHPHY_AUTONEG);

	return ENX_OK;
}

ENX_OKFAIL EthphyLinkInfo(void)
{
	UINT u32Loop;
	WORD wPHYCR = 0, w1000SR = 0;

	MdioRead(ethphy_info.addr, ETHPHY_1000SR_ADR, &w1000SR); // Get 1000Base-T Status from PHY status reg.
	EthphyRegView(ETHPHY_1000SR_ADR, w1000SR);
	if ((w1000SR & ETHPHY_GIGASR_IEC) == 0xFF) {
		ENX_DEBUGF(DBG_ETHPHY_ERR, "Idle Error(0xFF).\n");
		return ENX_FAIL;
	}

	ethphy_info.type = ETHPHY_TYPE_VAL;

	for (u32Loop = 0; u32Loop < 50; u32Loop++) {
		MdioRead(ethphy_info.addr, ETHPHY_PHYCR_ADR, &wPHYCR); // Read the PHY Control Register.
		EthphyRegView(ETHPHY_PHYCR_ADR, wPHYCR);

		if (!(wPHYCR & ETHPHY_PCR_LINK_STATUS_CHK_FAIL)) { // Link is not failing
			u32Loop = 0;
			break;
		}

		vTaskDelay(10);
	}

	if (u32Loop) {
		ENX_DEBUGF(DBG_ETHPHY_ERR, "Not linked.\n");
		ethphy_info.speed = ETHPHY_SPD_0;
		ethphy_info.duplex = ETHPHY_DUPLEX_UNKNOWN;
		return ENX_FAIL;
	}

	if (wPHYCR & ETHPHY_PCR_SS_1000M) {
		ethphy_info.speed = ETHPHY_SPD_1000;
	} else if (wPHYCR & ETHPHY_PCR_SS_100M) {
		ethphy_info.speed = ETHPHY_SPD_100;
	} else if (wPHYCR & ETHPHY_PCR_SS_10M) {
		ethphy_info.speed = ETHPHY_SPD_10;
	} else {
		ethphy_info.speed = ETHPHY_SPD_0;
	}

	if (wPHYCR & ETHPHY_PCR_DUPLEX_STATUS) {
		ethphy_info.duplex = ETHPHY_DUPLEX_FULL;
	} else {
		ethphy_info.duplex = ETHPHY_DUPLEX_HALF;
	}

	return ENX_OK;
}

UINT EthphyLinkCheck(void)
{
	WORD getData;
	MdioRead(ethphy_info.addr, ETHPHY_ICSR_ADR, &getData);
	EthphyRegView(ETHPHY_ICSR_ADR, getData);
	ENX_DEBUGF(DBG_ETHPHY_LOG, "Get Data (0x%04X)\n", getData);

	if (getData & ETHPHY_ICSR_LUI) { // Link-Up
		ENX_DEBUGF(DBG_ETHPHY_LOG, "Link-Up\n");
		if (EthphyLinkInfo() == ENX_OK) {
			ENX_DEBUGF(DBG_ETHPHY_MSG, "NetNIC Link Up Detect Link Speed(%uMbps) %s Duplex\n", ethphy_info.speed, ethphy_info.duplex == ETHPHY_DUPLEX_FULL ? "Full" : "Half");
			EthRxTxInit(ethphy_info.type, ethphy_info.speed, ethphy_info.duplex);
			return ETHPHY_LINKSTATUS_UP;
		} else {
			ENX_DEBUGF(DBG_ETHPHY_ERR, "Unknown Speed & Duplex\n");
			return ETHPHY_LINKSTATUS_ERROR;
		}
	} else if (getData & ETHPHY_ICSR_LDI) { // Link-Down
		ENX_DEBUGF(DBG_ETHPHY_LOG, "Link-Down\n");
		return ETHPHY_LINKSTATUS_DOWN;
	} else {
		ENX_DEBUGF(DBG_ETHPHY_ERR, "Get Data (0x%04X)\n", getData);
		return ETHPHY_LINKSTATUS_ERROR;
	}
}

void EthphyLinkView(void)
{
	WORD getData;
	MdioRead(ethphy_info.addr, ETHPHY_1000SR_ADR, &getData); // Get 1000Base-T Status from PHY status reg.
	EthphyRegView(ETHPHY_1000SR_ADR, getData);
	if ((getData & ETHPHY_GIGASR_IEC) == 0xFF) {
		printf("Idle Error(0xFF).\n");
		return;
	}

	MdioRead(ethphy_info.addr, ETHPHY_PHYCR_ADR, &getData); // Read the PHY Control Register.

	printf("NetNIC Link ");
	if (!(getData & ETHPHY_PCR_LINK_STATUS_CHK_FAIL)) { // Link-Up
		int speed, duplex;
		if (getData & ETHPHY_PCR_SS_1000M) {
			speed = ETHPHY_SPD_1000;
		} else if (getData & ETHPHY_PCR_SS_100M) {
			speed = ETHPHY_SPD_100;
		} else if (getData & ETHPHY_PCR_SS_10M) {
			speed = ETHPHY_SPD_10;
		} else {
			speed = ETHPHY_SPD_0;
		}
		if (getData & ETHPHY_PCR_DUPLEX_STATUS) {
			duplex = ETHPHY_DUPLEX_FULL;
		} else {
			duplex = ETHPHY_DUPLEX_HALF;
		}
		printf("Up Detect Link Speed(%uMbps) %s Duplex\n", speed, duplex == ETHPHY_DUPLEX_FULL ? "Full" : "Half");
	} else { // Link-Down
		printf("Down\n");
	}
}

#if (ETHPHY_LOOPBACK_TEST==1)
void EthphyLoopbackMode(UINT speed, UINT duplex)
{
	const char *strErr = "You entered an incorrect \"%s\".(speed:%u, duplex:%u)\n";

	printf("Ethernet PHY Loopback Mode\n");

	// Set ethernet PHY : disable interrupt
	MdioWrite(ethphy_info.addr, ETHPHY_ICSR_ADR, 0);

	// Set ethernet PHY : loopback mode, Speed(10~1000Mbps), Full/Half-duplex
	WORD wBCR = ETHPHY_BCR_LOOPBACK;
	switch (speed) {
	case ETHPHY_SPD_1000:
		wBCR |= ETHPHY_BCR_SPEED_M;
		break;
	case ETHPHY_SPD_100:
		wBCR |= ETHPHY_BCR_SPEED_L;
		break;
	case ETHPHY_SPD_10:
		break;
	default:
		printf(strErr, "speed", speed, duplex);
		return;
	}

	switch (duplex) {
	case ETHPHY_DUPLEX_HALF:
		break;
	case ETHPHY_DUPLEX_FULL:
		wBCR |= ETHPHY_BCR_FULLDPLX;
		break;
	default:
		printf(strErr, "duplex mode", speed, duplex);
		return;
	}

	MdioWrite(ethphy_info.addr, ETHPHY_BCR_ADR, wBCR);
	ethphy_info.type = ETHPHY_TYPE_VAL;
	ethphy_info.speed = speed;
	ethphy_info.duplex = duplex;

	printf("  Type(%d), Speed(%d), Duplex(%d)\n", ethphy_info.type, ethphy_info.speed, ethphy_info.duplex);

	EthRxTxInit(ethphy_info.type, ethphy_info.speed, ethphy_info.duplex);

	// Disable filter
	EthSetRxFilterEn(ENX_OFF);
}
#endif
#endif // __ETHPHY_KSZ9031RNX__
#endif // __ETHERNET__
