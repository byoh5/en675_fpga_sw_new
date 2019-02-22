#include "dev.h"

#ifdef __ETHERNET__
#ifdef __ETHPHY_KSZ8081MNX__
#include "ethphy.h"
#include "ksz8081mnx.h"

UINT EthphyReset(void)
{
	WORD wBuf, nLoop = 0, nCk;

#if PHY_MDIO_TEST
	UINT nCnt, nGetAddr = -1;
	WaitXms(100);
	_printf("Ethernet PHY MDIO Address Test.. Idle 10sec\n");
	for (UINT i = 0; i < 20; i++) {
		_printf(".");
		WaitXms(50);
	}
	printf("Test Start!\r\n");
	for (UINT i = 0; i < 32; i++) {
		nCnt = 0;
		phy_info.addr = i;

		MdioWrite(phy_info.addr, PHY_BCR_ADR, PHY_BCR_RESET);		// PHY reset
		do {
			WaitXms(1);
			MdioRead(phy_info.addr, PHY_BCR_ADR, &wBuf);
		} while (wBuf & PHY_BCR_RESET && ++nCnt <= 100);
		_printf("Phy address(%02d), wData(0x%04X), nCnt(%d) ", phy_info.addr, wBuf, nCnt);

		if (!(wBuf & PHY_BCR_RESET)) {
			_printf(" - ok\n");
			nGetAddr = i;
		} else {
			_printf(" - fail\n");
		}

		WaitXms(1);
	}
	if (nGetAddr != -1) {
		phy_info.addr = nGetAddr;
		_printf("Ethernet PHY MDIO Address(%d)\n", phy_info.addr);
	} else {
		_printf("Ethernet PHY MDIO Error! Not Ready!\n");
		return DEF_FAIL;
	}
#endif

	MdioWrite(phy_info.addr, PHY_BCR_ADR, PHY_BCR_RESET);		// PHY reset
	for (nLoop = 0, nCk = 0; nLoop < 100; nLoop++) {
		MdioRead(phy_info.addr, PHY_BCR_ADR, &wBuf);
		if (!(wBuf & PHY_BCR_RESET)) {
			nCk = 1;
			break;
		}
		WaitXms(10);
	}

	if (nCk == 1) {
		return DEF_OK;
	} else {
		_printf("Ethernet PHY RESET Fail?\n");
	}

	return DEF_FAIL;
}

UINT EthphyLinkState(void)
{
	WORD wBuf;
	for (UINT i = 0; i < 100; i++) {
		MdioRead(phy_info.addr, PHY_BSR_ADR, &wBuf);
		if (wBuf & PHY_BSR_LS) {
			return DEF_ON;
		}
		WaitXms(10);
	}
	EthphyReset();	//	fail? sw reset
	return DEF_OFF;
}

void EthphyAutoNeg(int onoff)
{
	WORD wBuf = 0;

#if 1 // (ETH_MAC_PAUSE)
	MdioRead(phy_info.addr, PHY_ANAR_ADR, &wBuf);	// Auto-Negotiation Advertisement: Read
	wBuf |= PHY_ANAR_SYPAUSE;						// Auto-Negotiation Advertisement: Enable symmetric pause
	wBuf &= ~PHY_ANAR_ASYPAUSE;						// Auto-Negotiation Advertisement: Disable asymmetric pause
	MdioWrite(phy_info.addr, PHY_ANAR_ADR, wBuf);	// Auto-Negotiation Advertisement: Write
#endif

	MdioRead(phy_info.addr, PHY_BCR_ADR, &wBuf);	// Basic Control: Read1
	MdioRead(phy_info.addr, PHY_BCR_ADR, &wBuf);	// Basic Control: Read2
	if (onoff == DEF_OK) {
		wBuf |= PHY_BCR_AUTONEG_EN; 					// Basic Control: Enable auto-negotiation process
		wBuf |= PHY_BCR_RST_AUTONEG;					// Basic Control: Restart auto-negotiation process
		wBuf |= PHY_BCR_FULLDPLX;						// Basic Control: Temp
		wBuf &= ~PHY_BCR_ISOLATE;						// Basic Control: Normal operation
		MdioWrite(phy_info.addr, PHY_BCR_ADR, wBuf);	// Basic Control: Write
		phy_info.autonego = PHY_AUTONEGO_ING;

		_printf("Phy auto negotiation start.\n");
	} else {
		wBuf &= ~PHY_BCR_AUTONEG_EN; 					// Basic Control: Disable auto-negotiation process
		wBuf &= ~PHY_BCR_RST_AUTONEG; 					// Basic Control: Stop auto-negotiation process
		wBuf |= PHY_BCR_SPEED_L;						// Basic Control: Set 100Mbps
		wBuf |= PHY_BCR_FULLDPLX;						// Basic Control: Set Full-duplex
		MdioWrite(phy_info.addr, PHY_BCR_ADR, wBuf);	// Basic Control: Write

		_printf("Phy manual configuration.\n");
	}
}

void EthphySetting(void)
{
	// Interrupt Control(Enable link-down, link-up interrupt)
	MdioWrite(phy_info.addr, PHY_ICS_ADR, PHY_ICS_LUIE | PHY_ICS_LDIE);

#if 1 // PHY_AUTONEG
	EthphyAutoNeg(DEF_ON);
#endif
}

void EthphyIrq(void *ctx)
{
	WORD getData;
	MdioRead(phy_info.addr, PHY_ICS_ADR, &getData);
	_printf("Phy IRQ Get Data(0x%04X)\n", getData);
	if (getData & PHY_ICS_LUI) { // Link-Up
		_printf("Link-Up\n");
#if 0
		if (PhyLinkInfo() == DEF_OK) {
			_printf("NetNIC Link Up Detect Link Speed(%dMbps) %s Duplex\r\n", phy_info.speed, phy_info.duplex == PHY_DUPLEX_FULL ? "Full" : "Half");
			EthDuplexChange(phy_info.duplex);
		} else {
			_printf("Unknown Speed & Duplex - System reboot\n");
			WdtSysReboot();
		}
#endif
//		EthRxTxinit(phy_info.type, phy_info.speed, phy_info.duplex);
//		netifapi_netif_set_link_up(&gnif_eth);
	} else if (getData & PHY_ICS_LDI) { // Link-Down
		_printf("Link-Down\n");
//		netifapi_netif_set_link_down(&gnif_eth);
	} else {
		_printf("Phy IRQ Get Data(0x%04X)\n", getData);
	}
}
#endif // __ETHPHY_KSZ8081MNX__
#endif // __ETHERNET__
