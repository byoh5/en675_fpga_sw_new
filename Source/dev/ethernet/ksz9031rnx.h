#ifndef __KSZ9031RNX_H__
#define __KSZ9031RNX_H__

//*************************************************************************************************
// Register
//-------------------------------------------------------------------------------------------------
// Address (16 bit base) : IEEE-Defined Registers
#define ETHPHY_BCR_ADR					0x0		// Basic Control Register
#define ETHPHY_BSR_ADR					0x1		// Basic Status Register
#define ETHPHY_PI1R_ADR					0x2		// PHY Identifier I Register
#define ETHPHY_PI2R_ADR					0x3		// PHY Identifier II Register
#define ETHPHY_ANAR_ADR					0x4		// Auto-Negotiation Advertisement Register
#define ETHPHY_ANLPAR_ADR				0x5		// Auto-Negotiation Link Partner Ability Register
#define ETHPHY_ANER_ADR					0x6		// Auto-Negotiation Expansion Register
#define ETHPHY_ANNPR_ADR				0x7		// Auto-Negotiation Next Page Register
#define ETHPHY_LPNPAR_ADR				0x8		// Auto-Negotiation Link Partner Next Page Ability Register
#define ETHPHY_1000CR_ADR				0x9		// 1000Base-T Control Register
#define ETHPHY_1000SR_ADR				0xA		// 1000Base-T Status Register
#define ETHPHY_MMDCR_ADR				0xD		// MMD Access - Control
#define ETHPHY_MMDRDR_ADR				0xE		// MMD Access - Register/Data
#define ETHPHY_ESR_ADR					0xF		// Extended Status Register

// Address (16 bit base) : KSZ9031RNX-Defined Registers
#define ETHPHY_RLOOPR_ADR				0x11	// Remote Loopback Register
#define ETHPHY_LKMDR_ADR				0x12	// LinkMD Cable Diagnostic Register
#define ETHPHY_DPPSR_ADR				0x13	// Digital PMA/PCS Status Register
#define ETHPHY_RXERCR_ADR				0x15	// RXER Counter Register
#define ETHPHY_ICSR_ADR					0x1B	// Interrupt Control/Status Register
#define ETHPHY_AMDIXR_ADR				0x1C	// Auto MDI/MDI-X Register
#define ETHPHY_PHYCR_ADR				0x1F	// PHY Control Register

//-------------------------------------------------------------------------------------------------
// Basic Control Register(0x0) Bit field
#define ETHPHY_BCR_RESET				(1<<15) // Reset (1:Software reset 0:Normal operation)
#define ETHPHY_BCR_LOOPBACK				(1<<14) // Loopback (1:Loopback, 0:Normal operation)
#define ETHPHY_BCR_SPEED_L				(1<<13) // Speed Select(LSB) / [0,1]100Mbps [0,0]10Mbps
#define ETHPHY_BCR_AUTONEG_EN			(1<<12) // Auto-Negotiation Enable (1:Enable 0:Disable)
#define ETHPHY_BCR_PWR_DOWN				(1<<11) // Power Down (1:Power-down mode 0:Normal operation)
#define ETHPHY_BCR_ISOLATE				(1<<10) // Isolate (1:Electrical isolation of PHY from MII 0:Normal operation)
#define ETHPHY_BCR_RST_AUTONEG			(1<< 9) // Restart Auto-Negotiation (1:Restart auto-negotiation process 0:Normal operation)
#define ETHPHY_BCR_FULLDPLX				(1<< 8) // Duplex Mode (1:Full-duplex 0:Half-duplex)
#define ETHPHY_BCR_SPEED_M				(1<< 6) // Speed Select(MSB) / [1,1]Reserved [1,0]1000Mbps

// Basic Status Register(0x1) Bit field
#define ETHPHY_BSR_100BT4				(1<<15) // 100BASE-T4 Capable
#define ETHPHY_BSR_100BTXFD				(1<<14) // 100BASE-TX Full Duplex
#define ETHPHY_BSR_100BTXHD				(1<<13) // 100BASE-TX Half Duplex
#define ETHPHY_BSR_10BTFD				(1<<12) // 10BASE-T Full Duplex
#define ETHPHY_BSR_10BTHD				(1<<11) // 10BASE-T Half Duplex
#define ETHPHY_BSR_ES					(1<< 8) // Extended Status
#define ETHPHY_BSR_NO_PREMBLE			(1<< 6) // No Premble
#define ETHPHY_BSR_AUTONEG_CMPLT		(1<< 5) // Auto-Negotiation Complete
#define ETHPHY_BSR_REMOTE_FAULT			(1<< 4) // Remote Fault
#define ETHPHY_BSR_AUTONEG_ABLE			(1<< 3) // Auto-Negotiation Ability
#define ETHPHY_BSR_LINK_STATUS			(1<< 2) // Link Status
#define ETHPHY_BSR_JABBER_DETECT		(1<< 1) // Jabber Detect
#define ETHPHY_BSR_EX_CAPABILITY		(1<< 0) // Extended Capability

// PHY Identifier 1 Register(0x2) Bit field
#define ETHPHY_ID1R_ID_NUMBER			(0xFFFF)// PHY ID Number

// PHY Identifier 2 Register(0x3) Bit field
#define ETHPHY_ID2R_ID_NUMBER			(0xFC00)// PHY ID Number
#define ETHPHY_ID2R_MODEL_NUMBER		(0x03F0)// Model Number
#define ETHPHY_ID2R_REVISION_NUMBER		(0x000F)// Revision Number

// Auto-Negotiation Advertisement Register(0x4) Bit field
#define ETHPHY_ANAR_NEXT_PAGE			(1<<15) // Next Page
#define ETHPHY_ANAR_REMOTE_FAULT		(1<<13) // Remote Fault
#define ETHPHY_ANAR_ASY_PAUSE			(1<<11) // Asymmetric pause
#define ETHPHY_ANAR_SY_PAUSE			(1<<10) // Symmetric pause
#define ETHPHY_ANAR_100BASE_T4			(1<< 9) // 100Base-T4
#define ETHPHY_ANAR_100FULL				(1<< 8) // 100Base-T Full-Duplex
#define ETHPHY_ANAR_100HALF				(1<< 7) // 100Base-T Half-Duplex
#define ETHPHY_ANAR_10FULL				(1<< 6) // 10Base-T Full-Duplex
#define ETHPHY_ANAR_10HALF				(1<< 5) // 10Base-T Half-Duplex
#define ETHPHY_ANAR_SELECTOR_FIELD		(0x001F)// Selector Field

// Auto-Negotiation Link Partner Ability Register(0x5) Bit field
#define ETHPHY_ANLPAR_NEXT_PAGE			(1<<15) // Next page
#define ETHPHY_ANLPAR_ACKNOWLEDGE		(1<<14) // Acknowledge
#define ETHPHY_ANLPAR_REMOTE_FAULT		(1<<13) // Remote Fault
#define ETHPHY_ANLPAR_ASY_PAUSE			(1<<11) // Asymmetric pause
#define ETHPHY_ANLPAR_SY_PAUSE			(1<<10) // Symmetric pause
#define ETHPHY_ANLPAR_100BASE4			(1<< 9) // 100BASE-T4
#define ETHPHY_ANLPAR_100FULL			(1<< 8) // 100BASE_TX Full-Duplex
#define ETHPHY_ANLPAR_100HALF			(1<< 7) // 100BASE-TX Half-Duplex
#define ETHPHY_ANLPAR_10FULL			(1<< 6) // 10BASE-T Full-Duplex
#define ETHPHY_ANLPAR_10HALF			(1<< 5) // 10BASE_T Half-Duplex
#define ETHPHY_ANLPAR_SELECTOR_FIELD	(0x001F)// Selector Field

// Auto-Negotiation Expansion Register(0x6) Bit field
#define ETHPHY_ANER_PARALLEL_FAULT		(1<< 4)	// Parallel Detection Fault
#define ETHPHY_ANER_PARTNER_NEXT_PAGE	(1<< 3)	// Link Partner Next Page Able
#define ETHPHY_ANER_NEXT_PAGE			(1<< 2)	// Next Page Able
#define ETHPHY_ANER_PAGE_RECEIVED		(1<< 1)	// Page Received
#define ETHPHY_ANER_PARTNER_AUTONEG		(1<< 0)// Link Partner Auto-Negotiation Able

// Auto-Negotiation Next Page Register(0x7) Bit field
#define ETHPHY_ANNPR_NEXT_PAGE			(1<<15)	// Next Page
#define ETHPHY_ANNPR_MESSAGE_PAGE		(1<<13)	// Message Page
#define ETHPHY_ANNPR_ACKNOWLEDGAE2		(1<<12)	// Acknowledge2
#define ETHPHY_ANNPR_TOGGLE				(1<<11)	// Toggle
#define ETHPHY_ANNPR_MESSAGE_FIELD		(0x07FF)// Message Field

// Link Partner Next Page Ability Register(0x8) Bit field
#define ETHPHY_LPNPAR_NEXT_PAGE			(1<<15)	// Next Page
#define ETHPHY_LPNPAR_ACKNOWLEDGE		(1<<14)	// Acknowledge
#define ETHPHY_LPNPAR_MESSAGE_PAGE		(1<<13)	// Message Page
#define ETHPHY_LPNPAR_ACKNOWLEDGAE2		(1<<12)	// Acknowledge2
#define ETHPHY_LPNPAR_TOGGLE			(1<<11)	// Toggle
#define ETHPHY_LPNPAR_MESSAGE_FIELD		(0x07FF)// Message Field

// 1000Base-T Control Register(0x9) Bit field
#define ETHPHY_GIGACR_TMB				(0xE000)// Test Mode Bits
#define ETHPHY_GIGACR_MSMCE				(1<<12) // Master-Slave Manual Configuration Enable
#define ETHPHY_GIGACR_MSMCV				(1<<11) // Master-Slave Manual Configuration Value
#define ETHPHY_GIGACR_PT				(1<<10) // Port Type
#define ETHPHY_GIGACR_1000FULL			(1<< 9) // 1000Base-T Full-Duplex
#define ETHPHY_GIGACR_1000HALF			(1<< 8) // 1000Base-T Half-Duplex

// 1000Base-T Status Register(0xA) Bit field
#define ETHPHY_GIGASR_MSCF				(1<<15) // Master-Slave Configuration Fault
#define ETHPHY_GIGASR_MSCR				(1<<14) // Master-Slave Configuration Resolution
#define ETHPHY_GIGASR_LRS				(1<<13) // Local Receiver Status
#define ETHPHY_GIGASR_RRS				(1<<12) // Remote Receiver Status
#define ETHPHY_GIGASR_1000FULLC			(1<<11) // Link Partner 1000Base-T Full-Duplex Capability
#define ETHPHY_GIGASR_1000HALFC			(1<<10) // Link Partner 1000Base-T Half-Duplex Capability
#define ETHPHY_GIGASR_IEC				(0x00FF)// Idle Error Count

// MMD Access - Control Register(0xD) Bit field
#define ETHPHY_MMDCR_OPMODE				(0xC000) // MMD - Operation Modee
#define ETHPHY_MMDCR_DEVADR				(0x001C) // MMD - Device Address

// MMD Access - Register/Data Register(0xE) Bit field
#define ETHPHY_MMDRDR_REG_DAT			(0xFFFF) // MMD - Register/Data

// Extended Status Register(0xF) Bit field
#define ETHPHY_ESR_1000XFULL			(1<<15) // 1000Base-X Full-Duplex
#define ETHPHY_ESR_1000XHALF			(1<<14) // 1000Base-X Half-Duplex
#define ETHPHY_ESR_1000TFULL			(1<<13) // 1000Base-T Full-Duplex
#define ETHPHY_ESR_1000THALF			(1<<12) // 1000Base-T Half-Duplex

// Remote Loopback Register(0x11) Bit field
#define ETHPHY_RLOOPR_EN				(1<< 8) // Remote Loopback

// LinkMD Cable Diagnostic Register(0x12) Bit field
#define ETHPHY_LKMDR_TEST_EN			(1<<15)	// Cable Diagnostic Test Enable
#define ETHPHY_LKMDR_TEST_PAIR			(0x3000)// Cable Diagnostic Test Pair
#define ETHPHY_LKMDR_STATUS				(0x0300)// Cable Diagnostic Status
#define ETHPHY_LKMDR_FAULT_DATA			(0x00FF)// Cable Diagnostic Fault Data

// Digital PMA/PCS Status Register(0x13) Bit field
#define ETHPHY_DPPSR_1000M_LINK_STATUS	(1<< 2)	// 1000Base-T Link Status
#define ETHPHY_DPPSR_100M_LINK_STATUS	(1<< 1) // 100Base-TX Link Status

// RXER Counter Register(0x15) Bit field
#define ETHPHY_RXERCR_RXER_COUNT		(0XFFFF)// RXER Counter

// Interrupt Control/Status Register(0x1B) Bit field
#define ETHPHY_ICSR_JIE					(1<<15)	// Jabber Interrupt Enable
#define ETHPHY_ICSR_REIE				(1<<14)	// Receive Error Interrupt Enable
#define ETHPHY_ICSR_PRIE				(1<<13)	// Page Receive Interrupt Enable
#define ETHPHY_ICSR_PDFIE				(1<<12)	// Parallel Detect Fault Interrupt Enable
#define ETHPHY_ICSR_LPAIE				(1<<11)	// Link Partner Acknowledge Interrupt Enable
#define ETHPHY_ICSR_LDIE				(1<<10)	// Link-Down Interrupt Enable
#define ETHPHY_ICSR_RFIE				(1<< 9)	// Remote Fault Interrupt Enable
#define ETHPHY_ICSR_LUIE				(1<< 8)	// Link-Up Interrupt Enable
#define ETHPHY_ICSR_JI					(1<< 7)	// Jabber Interrupt
#define ETHPHY_ICSR_REI					(1<< 6)	// Receive Error Interrupt
#define ETHPHY_ICSR_PRI					(1<< 5)	// Page Receive Interrupt
#define ETHPHY_ICSR_PDFI				(1<< 4)	// Parallel Detect Fault Interrupt
#define ETHPHY_ICSR_LPAI				(1<< 3)	// Link Partner Acknowledge Interrupt
#define ETHPHY_ICSR_LDI					(1<< 2)	// Link-Down Interrupt
#define ETHPHY_ICSR_RFI					(1<< 1)	// Remote Fault Interrupt
#define ETHPHY_ICSR_LUI					(1<< 0)	// Link-Up Interrupt

// Auto MDI/MDI-X Register(0x1C) Bit field
#define ETHPHY_AMDIXR_MDISET			(1<< 7) // MDI Set
#define ETHPHY_AMDIXR_SWAP_OFF			(1<< 6) // Swap-Off

// PHY Control Register(0x1F) Bit field
#define ETHPHY_PCR_INT_LEVEL			(1<<14)	// Interrupt Level
#define ETHPHY_PCR_EN_JABBER			(1<< 9)	// Enable Jabber
#define ETHPHY_PCR_SS_1000M				(1<< 6)	// Speed Status 1000Base-T
#define ETHPHY_PCR_SS_100M				(1<< 5)	// Speed Status 100Base-TX
#define ETHPHY_PCR_SS_10M				(1<< 4)	// Speed Status 10Base-T
#define ETHPHY_PCR_DUPLEX_STATUS		(1<< 3)	// Duplex Status
#define ETHPHY_PCR_1000MS_STATUS		(1<< 2)	// 1000Base-T Master/Slave Status
#define ETHPHY_PCR_LINK_STATUS_CHK_FAIL	(1<< 0)	// Link Status Check Fail

#endif // __KSZ9031RNX_H__
