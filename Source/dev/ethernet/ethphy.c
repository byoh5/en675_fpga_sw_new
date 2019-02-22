#include "dev.h"

#ifdef __ETHERNET__
#include "ethphy.h"

#ifdef __ETHPHY_KSZ8081MNX__
#include "ksz8081mnx.h"
#endif

PhyInfo phy_info;

UINT EthphyInit(BYTE phy_addr)
{
	GpioOutDir(ETHPHY_GPIO_RST);
	GpioInDir(ETHPHY_GPIO_IRQ);
	GpioIrqCallback(ETHPHY_GPIO_IRQ, EthphyIrq, NULL);
	GpioIrqOn(ETHPHY_GPIO_IRQ);

	GpioSetLo(ETHPHY_GPIO_RST);
	WaitXms(10);
	GpioSetHi(ETHPHY_GPIO_RST);

	phy_info.addr = phy_addr & 0x1F;
	phy_info.type = PHY_TYPE_UNKNOWN;
	phy_info.speed = PHY_SPD_0;
	phy_info.duplex = PHY_DUPLEX_UNKNOWN;
	phy_info.autonego = PHY_AUTONEGO_NONE;

	if (EthphyReset() == DEF_FAIL) {
		_printf("Ethphy Reset Fail\n");
		return DEF_FAIL;
	}

	EthphySetting();


	return DEF_OK;
}
#endif // __ETHERNET__
