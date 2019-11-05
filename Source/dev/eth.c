#include "dev.h"

#if USE_ETH
#include "ethphy.h"

#define ETH_RXFILTER_CNT 31 // 0~30

static _ETH_DSTMAC1 * const arrDSTMAC_H[ETH_RXFILTER_CNT+1] = {(_ETH_DSTMAC1 *)(REG_BASE_ETH+(33<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(35<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(37<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(39<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(41<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(43<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(45<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(47<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(49<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(51<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(53<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(55<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(57<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(59<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(61<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(63<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(65<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(67<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(69<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(71<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(73<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(75<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(77<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(79<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(81<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(83<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(85<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(87<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(89<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(91<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(93<<3)), (_ETH_DSTMAC1 *)(REG_BASE_ETH+(95<<3))};
static _ETH_DSTMAC2 * const arrDSTMAC_L[ETH_RXFILTER_CNT+1] = {(_ETH_DSTMAC2 *)(REG_BASE_ETH+(34<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(36<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(38<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(40<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(42<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(44<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(46<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(48<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(50<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(52<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(54<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(56<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(58<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(60<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(62<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(64<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(66<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(68<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(70<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(72<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(74<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(76<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(78<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(80<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(82<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(84<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(86<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(88<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(90<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(92<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(94<<3)), (_ETH_DSTMAC2 *)(REG_BASE_ETH+(96<<3))};
ISRD static tIhnd arrETHERNETTXIrq;
ISRD static tIhnd arrETHERNETRXIrq;

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
	UINT u32Cal = (MCK_FREQ / (Speed_Hz * 2)) - 1;
	if (u32Cal > 255) {
		ETH_MDIO_CLKLMT = 255;
		ENX_DEBUGF(DBG_MDIO_LOG, "MDIO Clk Min.(%u/%u)\n", u32Cal, ETH_MDIO_CLKLMT);
	} else {
		ETH_MDIO_CLKLMT = u32Cal;
	}
}

UINT MdioGetClk(void)
{
	ENX_DEBUGF(DBG_MDIO_LOG, "MDIO Init - %uHz(%u)\n", MCK_FREQ / ((ETH_MDIO_CLKLMT + 1) * 2), ETH_MDIO_CLKLMT);
	return MCK_FREQ / ((ETH_MDIO_CLKLMT + 1) * 2);
}

void MdioInit(UINT Speed_Hz)
{
	ETH_MDIO_RXEDGE = 1;
	ETH_MDIO_TXEDGE = 0;
	ETH_MDIO_RDLTC = 1;

	MdioSetClk(Speed_Hz);
	ETH_MDIO_CLKEN	= 1;
	MdioGetClk();
}

void EthInit(void)
{
	ETH_PIN_INIT;

	EthRxTxInit(ETHPHY_TYPE_VAL, ETHPHY_SPD_VAL, ETHPHY_DUPLEX_VAL);

	ETH_RX_CRC_EN = 1;
	ETH_RX_ERR_EN = 1;
	ETH_DSTMAC_BYP = 0;

	arrETHERNETTXIrq.irqfn = NULL;
	arrETHERNETTXIrq.arg = NULL;

	arrETHERNETRXIrq.irqfn = NULL;
	arrETHERNETRXIrq.arg = NULL;
}

void EthRxFilterMacAdr(BYTE *addr)
{
	ETH_DSTMAC_EN31 = 1;
	ETH_DSTMAC_ADR31_47_32 = (addr[0] << 8) | (addr[1] << 0);
	ETH_DSTMAC_ADR31_31_0 = (addr[2] << 24) | (addr[3] << 16) |  (addr[4] << 8) | (addr[5] << 0);
}

ENX_OKFAIL EthRxFilterInsert(BYTE *addr)
{
	for (int i = 0 ; i < ETH_RXFILTER_CNT; i++) {
		if (arrDSTMAC_H[i]->DSTMAC_EN == 0) {
			arrDSTMAC_H[i]->DSTMAC_EN = 1;
			arrDSTMAC_H[i]->DSTMAC_ADR_47_32 = (addr[0] << 8) | (addr[1] << 0);
			arrDSTMAC_L[i]->DSTMAC_ADR_31_0 = (addr[2] << 24) | (addr[3] << 16) |  (addr[4] << 8) | (addr[5] << 0);
			return ENX_OK;
		}
	}
	return ENX_FAIL;
}

ENX_OKFAIL EthRxFilterDelete(BYTE *addr)
{
	UINT adr_47_32 = (addr[0] << 8) | (addr[1] << 0);
	UINT adr_31_00 = (addr[2] << 24) | (addr[3] << 16) |  (addr[4] << 8) | (addr[5] << 0);
	for (int i = 0 ; i < ETH_RXFILTER_CNT; i++) {
		if (arrDSTMAC_H[i]->DSTMAC_EN == 1) {
			if ((arrDSTMAC_H[i]->DSTMAC_ADR_47_32 == adr_47_32) && (arrDSTMAC_L[i]->DSTMAC_ADR_31_0 == adr_31_00)) {
				arrDSTMAC_H[i]->DSTMAC_ADR_47_32 = 0;
				arrDSTMAC_L[i]->DSTMAC_ADR_31_0 = 0;
				arrDSTMAC_H[i]->DSTMAC_EN = 0;
				return ENX_OK;
			}
		}
	}
	return ENX_FAIL;
}

void EthRxFilterList(void)
{
	BYTE arrMac[6];
	for (int i = 0 ; i < ETH_RXFILTER_CNT + 1; i++) {
		arrMac[0] = (arrDSTMAC_H[i]->DSTMAC_ADR_47_32 >> 8) & 0xFF;
		arrMac[1] = (arrDSTMAC_H[i]->DSTMAC_ADR_47_32 >> 0) & 0xFF;
		arrMac[2] = (arrDSTMAC_L[i]->DSTMAC_ADR_31_0 >> 24) & 0xFF;
		arrMac[3] = (arrDSTMAC_L[i]->DSTMAC_ADR_31_0 >> 16) & 0xFF;
		arrMac[4] = (arrDSTMAC_L[i]->DSTMAC_ADR_31_0 >> 8) & 0xFF;
		arrMac[5] = (arrDSTMAC_L[i]->DSTMAC_ADR_31_0 >> 0) & 0xFF;
		printf("[%2u] %3s %02X:%02X:%02X:%02X:%02X:%02X\n", i, arrDSTMAC_H[i]->DSTMAC_EN ? "ON" : "OFF",
				arrMac[0], arrMac[1], arrMac[2], arrMac[3], arrMac[4], arrMac[5]);
	}
}

void EthRxSetBuffer(BYTE *addr, UINT rx_lmt)
{
	--rx_lmt;
	ENX_ASSERT(rx_lmt <= 255);
	ETH_RX_LMT = rx_lmt; // Max 256 Packet * 1536 = 393,216 Byte = 0x60000
	ETH_RX_ADR = (intptr_t)addr;
}

void EthRxSetAddrOffset(UINT rx_adrofs)
{
	ETH_RX_ADROFS = rx_adrofs;
}

void EthTxPacket(BYTE *addr, UINT Len)
{
	ETH_TX_ADR = (intptr_t)addr;
	ETH_TX_LEN = Len;
	while (ETH_TX_FULL) {
#if 0
		if (ETH_TX_PAUSE) {
			printf("p");
		}
#endif
#if 0
#if 0
#if 0
#if 0
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
#endif
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
#endif
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
#endif
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
		asm volatile("NOP");
#endif
//		UartTx(7, '_');
		//printf("ETH_TX_FULL!!!!!!\n");
	}
//	UartTx(7, 'O');
	ETH_TX_VAL = 1;
}

void EthRxTxClockDly(BYTE u8TXe, BYTE u8TXd, BYTE u8RXe, BYTE u8RXd)
{
	ETH_TX_CLKEDGE = u8TXe;
	ETH_TX_TCKDLY = u8TXd;
	ETH_RX_RCKEDGE = u8RXe;
	ETH_RX_RCKDLY = u8RXd;
}

void EthRxTxInit(UINT type, UINT speed, UINT duplex)
{
	switch (type) {
	case ETHPHY_TYPE_RGMII:
		ETH_TX_COLEN		= 0;
		switch (duplex) {
		case ETHPHY_DUPLEX_HALF:
			ETH_TX_CRSCHK	= 1;
			ETH_TX_COLCHK	= 1;
			ETH_TX_COLSEL	= 1;
			break;
		case ETHPHY_DUPLEX_FULL:
			ETH_TX_CRSCHK	= 0;
			ETH_TX_COLCHK	= 0;
			ETH_TX_COLSEL	= 0;
			break;
		}
		ETH_RX_RMII10		= 0;
		ETH_TX_RMII10		= 0;
		ETH_RX_ERTYPE		= 1;
		if (speed == ETHPHY_SPD_1000 && duplex == ETHPHY_DUPLEX_FULL) {
			ETH_RX_DATTYPE	= 1;
			ETH_TX_DATTYPE	= 1;
		} else {
			ETH_RX_DATTYPE	= 0;
			ETH_TX_DATTYPE	= 0;
		}
		ETH_TX_DATBIT		= 1;
		ETH_TX_CLKOE		= 1;
		ETH_TX_CLKSEL		= 1;
		ETH_TX_IFGGAP		= 10;

		// Edge & Delay
		// eth lbm
		// eth lbt 1000
#if 1 // 191018(J)
		switch (duplex) {
		case ETHPHY_DUPLEX_HALF:
			ETH_TX_CLKEDGE = 1;		// no test
			ETH_TX_TCKDLY = 0x7;	//
			ETH_RX_RCKEDGE = 0;		//
			ETH_RX_RCKDLY = 0xB;	//
			break;
		case ETHPHY_DUPLEX_FULL:
			ETH_TX_CLKEDGE = 1;
			ETH_TX_TCKDLY = 0x1;
			ETH_RX_RCKEDGE = 0;
			ETH_RX_RCKDLY = 0x0;
			break;
		}
#elif 1 // 191010(J)
		switch (duplex) {
		case ETHPHY_DUPLEX_HALF:
			ETH_TX_CLKEDGE = 1;
			ETH_TX_TCKDLY = 0x7;
			ETH_RX_RCKEDGE = 0;
			ETH_RX_RCKDLY = 0xB;
			break;
		case ETHPHY_DUPLEX_FULL:
			ETH_TX_CLKEDGE = 1;
			ETH_TX_TCKDLY = 0x6;
			ETH_RX_RCKEDGE = 0;
			ETH_RX_RCKDLY = 0xB;
			break;
		}
#elif 0 // 190917(J)
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x1;
		ETH_RX_RCKEDGE = 0;
		ETH_RX_RCKDLY = 0xC;
#elif 0 // 190829(C)
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x5;
		ETH_RX_RCKEDGE = 0;
		ETH_RX_RCKDLY = 0x1;
#elif 1 // 190820(J)
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x1;
		ETH_RX_RCKEDGE = 0;
		ETH_RX_RCKDLY = 0xC;
#elif 0 // 190708
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x6;
		ETH_RX_RCKEDGE = 1;
		ETH_RX_RCKDLY = 0x1;
#elif 0 // 190701
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x9;
		ETH_RX_RCKEDGE = 0;
		ETH_RX_RCKDLY = 0x6;
#elif 0 // 190626
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x7;
		ETH_RX_RCKEDGE = 1;
		ETH_RX_RCKDLY = 0x2;
#elif 0 // 190624
		ETH_TX_CLKEDGE = 0;
		ETH_TX_TCKDLY = 0xC;
		ETH_RX_RCKEDGE = 1;
		ETH_RX_RCKDLY = 0x2;
#elif 0 // 190621
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x5;
		ETH_RX_RCKEDGE = 1;
		ETH_RX_RCKDLY = 0x2;
#elif 0 // 190620
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x1;
		ETH_RX_RCKEDGE = 1;
		ETH_RX_RCKDLY = 0xE;
#elif 0 // 19.06.03 오전
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x7;
		ETH_RX_RCKEDGE = 0;
		ETH_RX_RCKDLY = 0x7;
#elif 1 // 19.06.03 오후
		ETH_TX_CLKEDGE = 1;
		ETH_TX_TCKDLY = 0x5;
		ETH_RX_RCKEDGE = 1;
		ETH_RX_RCKDLY = 0xB;
#endif

		ETH_TX_TXENDLY		= 0;
		ETH_TX_TXD0DLY		= 0;
		ETH_TX_TXD1DLY		= 0;
		ETH_TX_TXD2DLY		= 0;
		ETH_TX_TXD3DLY		= 0;
		break;

	case ETHPHY_TYPE_MII:
		ETH_TX_COLEN		= 0;
		switch (duplex) {
		case ETHPHY_DUPLEX_HALF:
			ETH_TX_CRSCHK	= 1;
			ETH_TX_COLCHK	= 1;
			break;
		case ETHPHY_DUPLEX_FULL:
			ETH_TX_CRSCHK	= 0;
			ETH_TX_COLCHK	= 0;
			break;
		}
		ETH_TX_COLSEL		= 0;
		ETH_RX_RMII10		= 0;
		ETH_TX_RMII10		= 0;
		ETH_RX_ERTYPE		= 0;
		ETH_RX_DATTYPE		= 0;
		ETH_TX_DATTYPE		= 0;
		ETH_TX_DATBIT		= 1;
		ETH_TX_CLKOE		= 0;
		ETH_TX_CLKSEL		= 0;
		ETH_TX_IFGGAP		= 10;

		// Edge & Delay
		ETH_TX_CLKEDGE		= 1;
		ETH_TX_TCKDLY		= 0x0;
		ETH_RX_RCKEDGE		= 0;
		ETH_RX_RCKDLY		= 0x0;

		ETH_TX_TXENDLY		= 0;
		ETH_TX_TXD0DLY		= 0;
		ETH_TX_TXD1DLY		= 0;
		ETH_TX_TXD2DLY		= 0;
		ETH_TX_TXD3DLY		= 0;
		break;

	case ETHPHY_TYPE_RMII:
		ETH_TX_COLEN		= 0;
		switch (duplex) {
		case ETHPHY_DUPLEX_HALF:
			ETH_TX_CRSCHK	= 1;
			ETH_TX_COLCHK	= 1;
			ETH_TX_COLSEL	= 1;
			break;
		case ETHPHY_DUPLEX_FULL:
			ETH_TX_CRSCHK	= 0;
			ETH_TX_COLCHK	= 0;
			ETH_TX_COLSEL	= 0;
			break;
		}
		switch (speed) {
		case ETHPHY_SPD_10:
			ETH_RX_RMII10	= 1;
			ETH_TX_RMII10	= 1;
			break;
		case ETHPHY_SPD_100:
			ETH_RX_RMII10	= 0;
			ETH_TX_RMII10	= 0;
			break;
		}
		ETH_RX_ERTYPE		= 2;
		ETH_RX_DATTYPE		= 2;
		ETH_TX_DATTYPE		= 0;
		ETH_TX_DATBIT		= 0;
		ETH_TX_CLKOE		= 0;
		ETH_TX_CLKSEL		= 1;
		ETH_TX_IFGGAP		= 20;

		// Edge & Delay
		ETH_TX_CLKEDGE		= 0;
		ETH_TX_TCKDLY		= 0x0;
		ETH_RX_RCKEDGE		= 0;
		ETH_RX_RCKDLY		= 0x0;

		ETH_TX_TXENDLY		= 0;
		ETH_TX_TXD0DLY		= 0;
		ETH_TX_TXD1DLY		= 0;
		ETH_TX_TXD2DLY		= 0;
		ETH_TX_TXD3DLY		= 0;
		break;
	}
}

void EthSetRxFilterEn(ENX_SWITCH sw)
{
	ETH_DSTMAC_BYP = (sw == ENX_ON ? 0 : 1);
}

ENX_SWITCH EthGetRxFilterEn(void)
{
	return ETH_DSTMAC_BYP == 0 ? ENX_ON : ENX_OFF;
}

void EthSetRxEn(ENX_SWITCH sw)
{
	ETH_RX_EN = sw;
}

ENX_SWITCH EthGetRxEn(void)
{
	return ETH_RX_EN;
}

void EthTxIrqCallback(irq_fn irqfn, void *arg)
{
	arrETHERNETTXIrq.irqfn = irqfn;
	arrETHERNETTXIrq.arg = arg;
}

void EthRxIrqCallback(irq_fn irqfn, void *arg)
{
	arrETHERNETRXIrq.irqfn = irqfn;
	arrETHERNETRXIrq.arg = arg;
}

void EthSetTxIrqEn(ENX_SWITCH sw)
{
	ETH_IRQ_TX_EN = sw;
}

void EthSetRxIrqEn(ENX_SWITCH sw)
{
	ETH_IRQ_RX_EN = sw;
}

ENX_SWITCH EthGetTxIrqEn(void)
{
	return ETH_IRQ_TX_EN;
}

ENX_SWITCH EthGetRxIrqEn(void)
{
	return ETH_IRQ_RX_EN;
}

void EthTxIrqClear(void)
{
	ETH_IRQ_TX_CLR = 1;
}

void EthRxIrqClear(void)
{
	ETH_IRQ_RX_CLR = 1;
}

UINT EthIsTxIrq(void)
{
	return ETH_IRQ_TX;
}

UINT EthIsRxIrq(void)
{
	return ETH_IRQ_RX;
}

void IrqEthTx(void)
{
	if (EthIsTxIrq()) {
		printf("EthTx IRQ Get\n");
		if (arrETHERNETTXIrq.irqfn) {
			arrETHERNETTXIrq.irqfn(arrETHERNETTXIrq.arg);
		}
		EthTxIrqClear();
	}
}

void IrqEthRx(void)
{
	if (EthIsRxIrq()) {
//		printf("EthRx IRQ Get\n");
		if (arrETHERNETRXIrq.irqfn) {
			arrETHERNETRXIrq.irqfn(arrETHERNETRXIrq.arg);
		}
		EthRxIrqClear();
	}
}

void EthMacAddrCheck(BYTE *addr)
{
	if (gtSystem.arr8MacAddress[0] & 1) {
		while (1) {
			printf("Wrong MAC Address - Group Address!\n");
			printf("                    New Mac Address Input!\n");
//			WdtReset(); // CPU WDT reset
			WaitXms(2000);
		}
	} else if (gtSystem.arr8MacAddress[0] == 0 && gtSystem.arr8MacAddress[1] == 0 && gtSystem.arr8MacAddress[2] == 0 &&
		gtSystem.arr8MacAddress[3] == 0 && gtSystem.arr8MacAddress[4] == 0 && gtSystem.arr8MacAddress[5] == 0) {
		while (1) {
			printf("Wrong MAC Address - Zero Address!\n");
			printf("                    New Mac Address Input!\n");
//			WdtReset(); // CPU WDT reset
			WaitXms(2000);
		}
	} else if (gtSystem.arr8MacAddress[3] == 0xff && gtSystem.arr8MacAddress[4] == 0xff && gtSystem.arr8MacAddress[5] == 0xff) {
		while (1) {
			printf("Wrong MAC Address - Broadcast Address!\n");
			printf("                    New Mac Address Input!\n");
//			WdtReset(); // CPU WDT reset
			WaitXms(2000);
		}
	}
}
#else
void IrqEthTx(void)
{
	_Rprintf("Ethernet TX IRQ Get! Ethernet is inactive.\n");
	ENX_ASSERT(0);
}

void IrqEthRx(void)
{
	_Rprintf("Ethernet RX IRQ Get! Ethernet is inactive.\n");
	ENX_ASSERT(0);
}
#endif
