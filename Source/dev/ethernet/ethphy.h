#ifndef __ETHPHY_H__
#define __ETHPHY_H__

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
#define PHY_TYPE_UNKNOWN							0		// Type unknown
#define PHY_TYPE_10_100								1		// Type = 10/100Mbps Phy
#define PHY_TYPE_1000								2		// Type = 1000Mbps Phy

#define PHY_SPD_0									0		// Link speed unknown, or link down
#define PHY_SPD_10									10		// Link speed = 10mbps
#define PHY_SPD_100									100		// Link speed = 100mbps
#define PHY_SPD_1000								1000	// Link speed = 1000mbps

#define PHY_DUPLEX_UNKNOWN							0		// Duplex unknown or auto-neg incomplete
#define PHY_DUPLEX_HALF								1		// Duplex = Half Duplex
#define PHY_DUPLEX_FULL								2		// Duplex = Full Duplex

#define PHY_AUTONEGO_NONE							0
#define PHY_AUTONEGO_ENABLE							1
#define PHY_AUTONEGO_ING							2
#define PHY_AUTONEGO_OK								3
#define PHY_AUTONEGO_ERROR							4

typedef struct {
	UINT addr;		// MDIO addr(0~31)
	UINT type;		// 1:10/100, 2:1000
	UINT speed;		// 10, 100, 1000
	UINT duplex;	// 1:half, 2:full
	UINT autonego;	// 1:none, 2:en, 3:ing, 4:ok, 5:err
} PhyInfo;

//-------------------------------------------------------------------------------------------------
// Function
extern UINT EthphyInit(BYTE phy_addr);
extern void EthphySetting(void);
extern void EthphyIrq(void *ctx);

//-------------------------------------------------------------------------------------------------
// Variable
extern PhyInfo phy_info;

#endif // __ETHPHY_H__
