#include "dev.h"

#ifdef __ETHERNET__
#ifdef __ETHPHY_KSZ8081RNB__
#include "enx_freertos.h"

#include "ethphy.h"
#include "ksz8081rnb.h"

#include "networkif.h"

#if DBG_ETHPHY_LOG
void EthphyVendorRegView(UINT Type, WORD Data)
{
	static char const *strEthphyRegName[16] = {	"DRCR_ADR", "AFECR_ADR", "REG_0x12", "REG_0x13",
												"REG_0x14", "RXERCR_ADR", "OMSOR_ADR", "OMSSR_ADR",
												"EXPCR_ADR", "REG_0x19", "REG_0x1A", "ICSR_ADR",
												"REG_0x1C", "LKMDR_ADR", "PHYC1R_ADR", "PHYC2R_ADR"};
	static char const *strEnDis[2] = {"Enable", "Disable"};
	static char const *strOccur[2] = {"Occurred", "Did not occur"};
	static char const strNormalOp[] = "Normal operation";

	if (Type < 32) {
		printf("Type:ETHPHY_%s : (0x%04X)\n", strEthphyRegName[Type-16], Data);
	} else {
		printf("Type:Error?(0x%02X) : (0x%04X)\n", Type, Data);
	}
	if (DBG_ETHPHY_LOG & ENX_DBG_STATE) {
		printf("========================================\n");
		switch (Type) {
			case ETHPHY_DRCR_ADR:
				break;
			case ETHPHY_AFECR_ADR:
				break;
			case ETHPHY_RXERCR_ADR:
				break;
			case ETHPHY_OMSOR_ADR:
				break;
			case ETHPHY_OMSSR_ADR:
				break;
			case ETHPHY_EXPCR_ADR:
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
			case ETHPHY_LKMDR_ADR:
				break;
			case ETHPHY_PHYC1R_ADR:
				printf("%26s: %sFlow control capable\n", "Enable Pause(Flow Control)", Data & ETHPHY_PC1R_ENABLE_PAUSE ? "" : "No ");
				printf("%26s: Link is %s\n", "Link Status", Data & ETHPHY_PC1R_LINK_STATUS ? "up" : "down");
				printf("%26s: Polarity is %sreversed\n", "Polarity Status", Data & ETHPHY_PC1R_POLARITY_STATUS ? "" : "not ");
				printf("%26s: MDI%s\n", "MDI/MDI-X State", Data & ETHPHY_PC1R_MDI_STATUS ? "-X" : "");
				printf("%26s: %s on receive differential pair\n", "Energy Detect", Data & ETHPHY_PC1R_ENERGY_DETECT ? "Signal present" : "NO signal detected");
				printf("%26s: PHY in %s\n", "PHY Isolate", Data & ETHPHY_PC1R_ISOLATE ? "isolate mode" : strNormalOp);
				printf("%26s: ", "Operation Mode Indication");
				switch (Data & ETHPHY_PC1R_OPERATION_MI) {
				case 0x0:
					printf("Still in auto-negotiation\n");
					break;
				case 0x1:
					printf("10Base-T half-duplex\n");
					break;
				case 0x2:
					printf("100Base-TX half-duplex\n");
					break;
				case 0x5:
					printf("10Base-T full-duplex\n");
					break;
				case 0x6:
					printf("100Base-TX full-duplex\n");
					break;
				default:
					printf("Reserved(%u)\n", Data & ETHPHY_PC1R_OPERATION_MI);
					break;
				}
				break;
			case ETHPHY_PHYC2R_ADR:
				printf("%26s: %s Auto MDI/MDI-X mode\n", "HP_MDIX", Data & ETHPHY_PC2R_HP_MDI ? "HP" : "Micrel");
				printf("%26s: MDI%s mode\n", "MDI/MDI-X Select", Data & ETHPHY_PC2R_MD_MDI ? "-X" : "");
				printf("%26s: %sable Auto MDI/MDI-X\n", "Pair Swap Disable", Data & ETHPHY_PC2R_PAIR_SWAP_DIS ? "Dis" : "En");
				printf("%26s: %s\n", "Force Link", Data & ETHPHY_PC2R_FOREC_LINK ? "Force link pass" : "Normal link operation");
				printf("%26s: %sable power saving\n", "Power Saving", Data & ETHPHY_PC2R_POWER_SAVING ? "En" : "Dis");
				printf("%26s: Interrupt pin active %s\n", "Interrupt Level", Data & ETHPHY_PC2R_INT_LEVEL ? "high" : "low");
				printf("%26s: %sable jabber counter\n", "Enable Jabber", Data & ETHPHY_PC2R_EN_JABBER ? "En" : "Dis");
				printf("%26s: RMII %uMHz clock mode, clock input to XI is %uMHz\n", "RMII Ref Clock", Data & ETHPHY_PC2R_RMII_CLK_MODE ? 50 : 25, Data & ETHPHY_PC2R_RMII_CLK_MODE ? 50 : 25);
				printf("%26s: %s\n", "LED Mode", (Data & ETHPHY_PC2R_LED_MODE) == 0x10 ? "LED0(Activity) LED1(Link)" : (Data & ETHPHY_PC2R_LED_MODE) == 0x00 ? "LED0(Link/Activity) LED1(Speed)" : "Reserved");
				printf("%26s: %sable transmitter\n", "Disable Transmitter", Data & ETHPHY_PC2R_DIS_TRANSMITTER ? "Dis" : "En");
				printf("%26s: %s\n", "Remote Loopback", Data & ETHPHY_PC2R_REMOTE_LOOP ? "Remote (analog) loopback is enabled" : "Normal mode");
				printf("%26s: %sable SQE test\n", "Enable SQE Test", Data & ETHPHY_PC2R_EN_SEQ_TEST ? "En" : "Dis");
				printf("%26s: %sable scrambler\n", "Disable Data Scrambling", Data & ETHPHY_PC2R_DIS_SCRAMBLER ? "Dis" : "En");
				break;
		}
		printf("========================================\n");
	}
}

void EthphyRegView(UINT Type, WORD Data)
{
	static char const *strEthphyRegName[16] = {	"BCR_ADR", "BSR_ADR", "PI1R_ADR", "PI2R_ADR",
												"ANAR_ADR", "ANLPAR_ADR", "ANER_ADR", "ANNPR_ADR",
												"LPNPAR_ADR", "REG_0x09", "REG_0x0A", "REG_0x0B",
												"REG_0x0C", "REG_0x0D", "REG_0x0E", "REG_0x0F"};
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
				printf("%26s: %s\n", "Speed Select", Data & ETHPHY_BCR_SPEED ? "100Mbps" : "10Mbps");
				printf("%26s: %s\n", "Duplex Mode", Data & ETHPHY_BCR_FULLDPLX ? "Full-duplex" : "Half-duplex");
				break;
			case ETHPHY_BSR_ADR:
				printf("%26s: %sT4 capable\n", "100Base-T4", Data & ETHPHY_BSR_100BT4 ? "" : "Not ");
				printf("%26s: %sapable of 100Mbps full-duplex\n", "100Base-TX Full-Duplex", Data & ETHPHY_BSR_100BTXFD ? "C" : "Not c");
				printf("%26s: %sapable of 100Mbps half-duplex\n", "100Base-TX Half-Duplex", Data & ETHPHY_BSR_100BTXHD ? "C" : "Not c");
				printf("%26s: %sapable of 10Mbps full-duplex\n", "10Base-T Full-Duplex", Data & ETHPHY_BSR_10BTFD ? "C" : "Not c");
				printf("%26s: %sapable of 10Mbps half-duplex\n", "10Base-T Half-Duplex", Data & ETHPHY_BSR_10BTHD ? "C" : "Not c");
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
		}
		printf("========================================\n");
	}
}
#endif

void EthphyAutoNeg(ENX_SWITCH onoff)
{
	WORD wANAR = 0, wBCR = 0;

	if (onoff == ENX_ON) {
#if 1 // (ETH_MAC_PAUSE)
		MdioRead(ethphy_info.addr, ETHPHY_ANAR_ADR, &wANAR);	// Auto-Negotiation Advertisement: Read
		wANAR |= ETHPHY_ANAR_SY_PAUSE;							// Auto-Negotiation Advertisement: Enable symmetric pause
		wANAR &= ~ETHPHY_ANAR_ASY_PAUSE;						// Auto-Negotiation Advertisement: Disable asymmetric pause
		EthphyRegView(ETHPHY_ANAR_ADR, wANAR);
		MdioWrite(ethphy_info.addr, ETHPHY_ANAR_ADR, wANAR);	// Auto-Negotiation Advertisement: Write
#endif

		wBCR |= ETHPHY_BCR_AUTONEG_EN; 							// Basic Control: Enable auto-negotiation process
		wBCR |= ETHPHY_BCR_RST_AUTONEG;							// Basic Control: Restart auto-negotiation process
		ENX_DEBUGF(DBG_ETHPHY_MSG, "Auto-Negotiation start.\n");
	} else {
		wBCR |= ETHPHY_BCR_SPEED;								// Basic Control: Set 100Mbps
		wBCR |= ETHPHY_BCR_FULLDPLX;							// Basic Control: Set Full-duplex
		ENX_DEBUGF(DBG_ETHPHY_MSG, "Manual configuration.\n");
	}
	EthphyRegView(ETHPHY_BCR_ADR, wBCR);
	MdioWrite(ethphy_info.addr, ETHPHY_BCR_ADR, wBCR);			// Basic Control: Write
}

ENX_OKFAIL EthphySetting(void)
{
	// Interrupt Control(Enable link-down, link-up interrupt)
	MdioWrite(ethphy_info.addr, ETHPHY_ICSR_ADR, ETHPHY_ICSR_LUIE | ETHPHY_ICSR_LDIE);

	EthphyAutoNeg(ETHPHY_AUTONEG);

	return ENX_OK;
}

ENX_OKFAIL EthphyLinkInfo(void)
{
	WORD wPHYC1R = 0;
	MdioRead(ethphy_info.addr, ETHPHY_PHYC1R_ADR, &wPHYC1R); // Read the PHY Control 1 Register.
	EthphyRegView(ETHPHY_PHYC1R_ADR, wPHYC1R);

	ethphy_info.type = ETHPHY_TYPE_VAL;

	if (!(wPHYC1R & ETHPHY_PC1R_LINK_STATUS)) {
		ENX_DEBUGF(DBG_ETHPHY_ERR, "Not linked.\n");
		ethphy_info.speed = ETHPHY_SPD_0;
		ethphy_info.duplex = ETHPHY_DUPLEX_UNKNOWN;
		return ENX_FAIL;
	}

	switch (wPHYC1R & ETHPHY_PC1R_OPERATION_MI) {
	case 0x0:
		ENX_DEBUGF(DBG_ETHPHY_ERR, "Still in auto-negotiation\n");
		ethphy_info.speed = ETHPHY_SPD_0;
		ethphy_info.duplex = ETHPHY_DUPLEX_UNKNOWN;
		return ENX_FAIL;
	case 0x1:
		ethphy_info.speed = ETHPHY_SPD_10;
		ethphy_info.duplex = ETHPHY_DUPLEX_HALF;
		break;
	case 0x2:
		ethphy_info.speed = ETHPHY_SPD_100;
		ethphy_info.duplex = ETHPHY_DUPLEX_HALF;
		break;
	case 0x5:
		ethphy_info.speed = ETHPHY_SPD_10;
		ethphy_info.duplex = ETHPHY_DUPLEX_FULL;
		break;
	case 0x6:
		ethphy_info.speed = ETHPHY_SPD_100;
		ethphy_info.duplex = ETHPHY_DUPLEX_FULL;
		break;
	default:
		ENX_DEBUGF(DBG_ETHPHY_ERR, "Reserved(%u)\n", wPHYC1R & ETHPHY_PC1R_OPERATION_MI);
		ethphy_info.speed = ETHPHY_SPD_0;
		ethphy_info.duplex = ETHPHY_DUPLEX_UNKNOWN;
		return ENX_FAIL;
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

#if (ETHPHY_LOOPBACK_TEST==1)
void EthphyLoopbackMode(void)
{
	printf("Ethernet PHY Loopback Mode\n");

	// Set ethernet PHY : disable interrupt
	MdioWrite(ethphy_info.addr, ETHPHY_ICSR_ADR, 0);

	// Set ethernet PHY : loopback mode, Speed(100Mbps), Full-duplex
	MdioWrite(ethphy_info.addr, ETHPHY_BCR_ADR, ETHPHY_BCR_LOOPBACK | ETHPHY_BCR_SPEED | ETHPHY_BCR_FULLDPLX);
	ethphy_info.type = ETHPHY_TYPE_VAL;
	ethphy_info.speed = ETHPHY_SPD_VAL;
	ethphy_info.duplex = ETHPHY_DUPLEX_FULL;

	printf("  Type(%d), Speed(%d), Duplex(%d)\n", ethphy_info.type, ethphy_info.speed, ethphy_info.duplex);

	EthRxTxInit(ethphy_info.type, ethphy_info.speed, ethphy_info.duplex);

	// Disable filter
	EthSetRxFilterEn(ENX_OFF);
}
#endif
#endif // __ETHPHY_KSZ8081RNB__
#endif // __ETHERNET__
