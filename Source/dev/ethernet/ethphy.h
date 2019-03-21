#ifndef __ETHPHY_H__
#define __ETHPHY_H__

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
#define ETHPHY_TYPE_UNKNOWN							0		// Type unknown
#define ETHPHY_TYPE_10_100							1		// Type = 10/100Mbps Phy
#define ETHPHY_TYPE_1000							2		// Type = 1000Mbps Phy

#define ETHPHY_SPD_0								0		// Link speed unknown, or link down
#define ETHPHY_SPD_10								10		// Link speed = 10mbps
#define ETHPHY_SPD_100								100		// Link speed = 100mbps
#define ETHPHY_SPD_1000								1000	// Link speed = 1000mbps

#define ETHPHY_DUPLEX_UNKNOWN						0		// Duplex unknown or auto-neg incomplete
#define ETHPHY_DUPLEX_HALF							1		// Duplex = Half Duplex
#define ETHPHY_DUPLEX_FULL							2		// Duplex = Full Duplex

typedef struct {
	UINT addr;		// MDIO Address(0~31)
	UINT type;		// 1:10/100, 2:1000
	UINT speed;		// 10, 100, 1000
	UINT duplex;	// 1:half, 2:full
} EthphyInfo;

#if (DBG_ETHPHY_LOG == ENX_DBG_OFF)
#define EthphyRegView(Type, Data)
#else
extern void EthphyRegView(UINT Type, WORD Data); // submodule
#endif

//-------------------------------------------------------------------------------------------------
// Function
extern UINT EthphySetting(void);		// submodule
extern void EthphyIrq(void *ctx);		// submodule
extern void EthphyAutoNeg(int onoff);	// submodule

extern UINT EthphyReset(void);			// ethphy.c
extern UINT EthphyLinkState(void);		// ethphy.c
extern UINT EthphyInit(BYTE phy_addr);	// ethphy.c

//-------------------------------------------------------------------------------------------------
// Variable
extern EthphyInfo ethphy_info;

#endif // __ETHPHY_H__
