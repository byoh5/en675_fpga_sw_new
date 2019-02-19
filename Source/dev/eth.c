#include "dev.h"

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

void MdioInit(UINT Speed_Hz)
{
	ETH_MDIO_CLKLMT = (MCK_FREQ / (Speed_Hz << 2)) - 1;
	ETH_MDIO_CLKEN	= 1;
}

void EthInit(void)
{
	ETH_PIN_INIT;

	MdioInit(1000000); // 1MHz



}
