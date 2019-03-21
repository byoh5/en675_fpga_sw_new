#include "dev.h"

#ifdef __ETHERNET__
void MdioRead(BYTE PhyAdr, BYTE RegAdr, WORD *RdDat)
{
	ETH_MDIO_OPCODE = 2;
	ETH_MDIO_PHYADR = PhyAdr;
	ETH_MDIO_REGADR = RegAdr;
	ETH_MDIO_REQ = 1;
	while (ETH_MDIO_REQ);
	*RdDat = (WORD)ETH_MDIO_RXDAT;
}

void MdioWrite(BYTE PhyAdr, BYTE RegAdr, WORD WrDat)
{
	ETH_MDIO_OPCODE = 1;
	ETH_MDIO_PHYADR = PhyAdr;
	ETH_MDIO_REGADR = RegAdr;
	ETH_MDIO_TXDAT  = WrDat;
	ETH_MDIO_REQ = 1;
	while (ETH_MDIO_REQ);
}

void MdioSetClklmt(UINT Clklmt)
{
	ETH_MDIO_CLKLMT = Clklmt;
}

UINT MdioGetClklmt(void)
{
	return ETH_MDIO_CLKLMT;
}

void MdioSetClk(UINT Speed_Hz)
{
	ETH_MDIO_CLKLMT = (MCK_FREQ / (Speed_Hz * 4)) - 1;
}

UINT MdioGetClk(void)
{
	ENX_DEBUGF(DBG_MDIO_LOG, "MDIO Init - %uHz(%u)\n", MCK_FREQ / ((ETH_MDIO_CLKLMT + 1) * 4), ETH_MDIO_CLKLMT);
	return MCK_FREQ / ((ETH_MDIO_CLKLMT + 1) * 4);
}

void MdioInit(UINT Speed_Hz)
{
	MdioSetClk(Speed_Hz);
	ETH_MDIO_CLKEN	= 1;
	MdioGetClk();
}

void EthInit(void)
{
	ETH_PIN_INIT;

	MdioInit(1000000);

	EthphyInit(ETHPHY_MDIO_ADR);
}
#endif
