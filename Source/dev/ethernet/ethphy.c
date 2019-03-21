#include "dev.h"

#ifdef __ETHERNET__
#include "ethphy.h"

#ifdef __ETHPHY_KSZ8081MNX__
#include "ksz8081mnx.h"
#endif

EthphyInfo ethphy_info;

UINT EthphyReset(void)
{
	WORD wBuf, nLoop = 0, nCk;

#if ETHPHY_MDIO_TEST
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
		ethphy_info.addr = i;

		MdioWrite(ethphy_info.addr, ETHPHY_BCR_ADR, ETHPHY_BCR_RESET);		// PHY reset
		do {
			WaitXms(1);
			MdioRead(ethphy_info.addr, ETHPHY_BCR_ADR, &wBuf);
		} while (wBuf & ETHPHY_BCR_RESET && ++nCnt <= 100);
		_printf("Phy address(%02d), wData(0x%04X), nCnt(%d) ", ethphy_info.addr, wBuf, nCnt);

		if (!(wBuf & ETHPHY_BCR_RESET)) {
			_printf(" - ok\n");
			nGetAddr = i;
		} else {
			_printf(" - fail\n");
		}

		WaitXms(1);
	}
	if (nGetAddr != -1) {
		ethphy_info.addr = nGetAddr;
		_printf("Ethernet PHY MDIO Address(%d)\n", ethphy_info.addr);
	} else {
		_printf("Ethernet PHY MDIO Error! Not Ready!\n");
		return DEF_FAIL;
	}
#endif

	MdioWrite(ethphy_info.addr, ETHPHY_BCR_ADR, ETHPHY_BCR_RESET);		// PHY reset
	for (nLoop = 0, nCk = 0; nLoop < 100; nLoop++) {
		MdioRead(ethphy_info.addr, ETHPHY_BCR_ADR, &wBuf);
		if (!(wBuf & ETHPHY_BCR_RESET)) {
			EthphyRegView(ETHPHY_BCR_ADR, wBuf);
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
		MdioRead(ethphy_info.addr, ETHPHY_BSR_ADR, &wBuf);
		EthphyRegView(ETHPHY_BSR_ADR, wBuf);
		if (wBuf & ETHPHY_BSR_LINK_STATUS) {
			return DEF_ON;
		}
		WaitXms(10);
	}
	EthphyReset();	//	fail? sw reset
	return DEF_OFF;
}

UINT EthphyInit(BYTE phy_addr)
{
	GpioOutDir(ETHPHY_GPIO_RST);
	GpioInDir(ETHPHY_GPIO_IRQ);
	GpioIrqCallback(ETHPHY_GPIO_IRQ, EthphyIrq, NULL);
	GpioIrqOn(ETHPHY_GPIO_IRQ);

	GpioSetLo(ETHPHY_GPIO_RST);
	WaitXms(10);
	GpioSetHi(ETHPHY_GPIO_RST);

	ethphy_info.addr = phy_addr & 0x1F;
	ethphy_info.type = ETHPHY_TYPE_UNKNOWN;
	ethphy_info.speed = ETHPHY_SPD_0;
	ethphy_info.duplex = ETHPHY_DUPLEX_UNKNOWN;

	if (EthphyReset() == DEF_FAIL) {
		printf("Ethphy Reset Fail\n");
		return DEF_FAIL;
	}

	return EthphySetting();
}
#endif // __ETHERNET__
