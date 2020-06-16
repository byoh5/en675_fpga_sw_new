#ifndef __ETHPHY_H__
#define __ETHPHY_H__

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
#define ETHPHY_TYPE_UNKNOWN							0		// Type unknown
#define ETHPHY_TYPE_RGMII							1		// Type = 1000Mbps Phy
#define ETHPHY_TYPE_MII								2		// Type = 10/100Mbps Phy
#define ETHPHY_TYPE_RMII							3		// Type = 10/100Mbps Phy

#define ETHPHY_SPD_0								0		// Link speed unknown, or link down
#define ETHPHY_SPD_10								10		// Link speed = 10mbps
#define ETHPHY_SPD_100								100		// Link speed = 100mbps
#define ETHPHY_SPD_1000								1000	// Link speed = 1000mbps

#define ETHPHY_DUPLEX_UNKNOWN						0		// Duplex unknown or auto-neg incomplete
#define ETHPHY_DUPLEX_HALF							1		// Duplex = Half Duplex
#define ETHPHY_DUPLEX_FULL							2		// Duplex = Full Duplex

#define ETHPHY_LINKSTATUS_NONE						0		//
#define ETHPHY_LINKSTATUS_UP						1		//
#define ETHPHY_LINKSTATUS_DOWN						2		//
#define ETHPHY_LINKSTATUS_ERROR						3		//

typedef struct {
	UINT addr;		// MDIO Address(0~31)
	UINT type;		// 1:RGMII 2:MII 3:RMII
	UINT speed;		// 10, 100, 1000
	UINT duplex;	// 1:half, 2:full
} EthphyInfo;

//-------------------------------------------------------------------------------------------------
// Function
extern UINT EthphySetting(void);				// submodule
extern UINT EthphyLinkCheck(void);				// submodule
extern void EthphyLinkView(void);				// submodule
extern ENX_OKFAIL EthphyLinkInfo(int log);		// submodule
extern void EthphyAutoNeg(ETHERNETIF_AUTONEGO autonego);// submodule

extern UINT EthphyReset(void);					// ethphy.c
extern UINT EthphyLinkState(void);				// ethphy.c
extern void EthphyInit(BYTE phy_addr, irq_fn irqfn);	// ethphy.c
extern UINT EthphyGetPHYID(void);				// ethphy.c
extern void EthphyPrintPHYName(void);			// ethphy.c

#if (DBG_ETHPHY_LOG == ENX_DBG_OFF)
#define EthphyRegView(Type, Data)
#else
extern void EthphyRegView(UINT Type, WORD Data); // submodule
#endif

//-------------------------------------------------------------------------------------------------
// Variable
extern EthphyInfo ethphy_info;



//-------------------------------------------------------------------------------------------------
// Ethernet PHY Loopback

#if (ETHPHY_LOOPBACK_TEST==1)
#define ETHPHY_LOOPBACK_PACKET_SIZE					1514
#define ETHPHY_LOOPBACK_PACKET_CNT					4

typedef enum {
	ePlb_idle,		// 0:none, idle
	ePlb_ok,		// 1:test oK
	ePlb_data,		// 2:data error, compare fail
	ePlb_zero,		// 3:zero data
	ePlb_outside,	// 4:data length underflow or overflow
	ePlb_timeout,	// 5:timeout
} eEthphyLoopbackRes;

typedef enum {
	ePlk_off,		// 0:off status
	ePlk_stop,		// 1:close command
	ePlk_init,		// 2:init
	ePlk_ready,		// 3:ready
	ePlk_single,	// 4:single
	ePlk_auto,		// 5:auto
} eEthphyLoopbackMode;

typedef struct {
	BYTE *arrSendBuffer;
	BYTE *arrBuffer[ETHPHY_LOOPBACK_PACKET_CNT];
	BYTE u8Index;
	BYTE u8Option;
	UINT u32Loop;
	eEthphyLoopbackMode eRunMode;
	eEthphyLoopbackRes eRes;

	UINT u32Speed;
	UINT u32Duplex;
} EthLoopbackGp;

extern void EthphyLoopbackMode(UINT speed, UINT duplex);
#endif


#endif // __ETHPHY_H__
