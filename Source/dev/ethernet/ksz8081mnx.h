#ifndef __KSZ8081MNX_H__
#define __KSZ8081MNX_H__

//*************************************************************************************************
// Register
//-------------------------------------------------------------------------------------------------
// Address (32 bit base) : IEEE-Defined Registers
#define PHY_BCR_ADR				0x0		// Basic Control Register
#define PHY_BSR_ADR				0x1		// Basic Status Register
#define PHY_PI1_ADR				0x2		// PHY Identifier I
#define PHY_PI2_ADR				0x3		// PHY Identifier II
#define PHY_ANAR_ADR			0x4		// Auto-Negotiation Advertisement Register
#define PHY_ANLPAR_ADR			0x5		// Auto-Negotiation Link Partner Ability Register
#define PHY_ANER_ADR			0x6		// Auto-Negotiation Expansion Register
#define PHY_ANNP_ADR			0x7		// Auto-Negotiation Next Page Register
#define PHY_ANLPNPA_ADR			0x8		// Auto-Negotiation Link Partner Next Page Abillty Register
#define PHY_1000C_ADR			0x9		// 1000Base-T Control
#define PHY_1000S_ADR			0xA		// 1000Base-T Status
#define PHY_ESR_ADR				0xF		// Exteded Status Register

// Address (32 bit base) : KSZ8081MNX-Defined Registers
#define PHY_ICS_ADR				0x1B	// Interrupt Control/Status
#define PHY_PHYC1_ADR			0x1E	// PHY Control 1
#define PHY_PHYC2_ADR			0x1F	// PHY Control 2

//-------------------------------------------------------------------------------------------------
// Basic Control Register(0x0) Bit field
#define PHY_BCR_SPEED_M         (1<< 6) // Speed Select(MSB)
#define PHY_BCR_FULLDPLX        (1<< 8) // Duplex Mode
#define PHY_BCR_RST_AUTONEG     (1<< 9) // Restart Auto-Negotiation
#define PHY_BCR_ISOLATE         (1<<10) // Isolate
#define PHY_BCR_PWR_DOWN        (1<<11) // Power Down
#define PHY_BCR_AUTONEG_EN      (1<<12) // Auto-Negotiation Enable
#define PHY_BCR_SPEED_L         (1<<13) // Speed Select(LSB)
#define PHY_BCR_LOOP_BACK       (1<<14) // Loop Back
#define PHY_BCR_RESET           (1<<15) // Reset

// Basic Status Register(0x1) Bit field
#define PHY_BSR_EC              (1<< 0) // Extended Capability
#define PHY_BSR_JD              (1<< 1) // Jabber Detect
#define PHY_BSR_LS              (1<< 2) // Link Status
#define PHY_BSR_AUTONEG_ABLE    (1<< 3) // Auto-Negotiation Ability
#define PHY_BSR_RF              (1<< 4) // Remote Fault
#define PHY_BSR_AUTONEG_CMPLT   (1<< 5) // Auto-Negotiation Complete
#define PHY_BSR_NP              (1<< 6) // No Premble
#define PHY_BSR_ES              (1<< 8) // Extended Status
#define PHY_BSR_10BTHD          (1<<11) // 10BASE-T Half Duplex
#define PHY_BSR_10BTFD          (1<<12) // 10BASE-T Full Duplex
#define PHY_BSR_100BTXHD        (1<<13) // 100BASE-TX Half Duplex
#define PHY_BSR_100BTXFD        (1<<14) // 100BASE-TX Full Duplex
#define PHY_BSR_100BT4          (1<<15) // 100BASE-T4 Capable

// Auto-Negotiation Advertisement Register(0x4) Bit field
#define PHY_ANAR_SF				(0x001F)// Selector Field
#define PHY_ANAR_10BTHD			(1<< 5) // 10Base-T Half-Duplex
#define PHY_ANAR_10BTFD			(1<< 6) // 10Base-T Full-Duplex
#define PHY_ANAR_100BTXHD		(1<< 7) // 100Base-T Half-Duplex
#define PHY_ANAR_100BTXFD		(1<< 8) // 100Base-T Full-Duplex
#define PHY_ANAR_100BT4			(1<< 9) // 100Base-T4
#define PHY_ANAR_SYPAUSE		(1<<10) // Symmetric pause
#define PHY_ANAR_ASYPAUSE		(1<<11) // Asymmetric pause
#define PHY_ANAR_RF				(1<<13) // Remote Fault
#define PHY_ANAR_NP				(1<<15) // Next Page

// Auto-Negotiation Link Partner Ability Register(0x5) Bit field
#define PHY_ANLPAR_SLCT         (0x001F)// Same as advertise selector
#define PHY_ANLPAR_10HALF       (1<< 5) // Can do 10mbps half-duplex
#define PHY_ANLPAR_10FULL       (1<< 6) // Can do 10mbps full-duplex
#define PHY_ANLPAR_100HALF      (1<< 7) // Can do 100mbps half-duplex
#define PHY_ANLPAR_100FULL      (1<< 8) // Can do 100mbps full-duplex
#define PHY_ANLPAR_100BASE4     (1<< 9) // Can do 100mbps 4k packets
#define PHY_ANLPAR_SYPAUSE		(1<<10) // Symmetric pause
#define PHY_ANLPAR_ASYPAUSE		(1<<11) // Asymmetric pause
#define PHY_ANLPAR_RFAULT       (1<<13) // Link partner faulted
#define PHY_ANLPAR_LPACK        (1<<14) // Link partner acked us
#define PHY_ANLPAR_NPAGE        (1<<15) // Next page bit

// 1000Base-T Control Register(0x9) Bit field
#define PHY_GIGACR_1000HALF     (1<< 8) // 1000Base-T Half-Duplex
#define PHY_GIGACR_1000FULL     (1<< 9) // 1000Base-T Full-Duplex
#define PHY_GIGACR_PT           (1<<10) // Port Type
#define PHY_GIGACR_MSMCV        (1<<11) // Master-Slave Manual Configuration Value
#define PHY_GIGACR_MSMCE        (1<<12) // Master-Slave Manual Configuration Enable
#define PHY_GIGACR_TMB          (0xE000)// Test Mode Bits

// 1000Base-T Status Register(0xA) Bit field
#define PHY_GIGASR_IEC          (0x00FF)// Idle Error Count
#define PHY_GIGASR_1000HALFC    (1<<10) // Link Partner 1000Base-T Half-Duplex Capability
#define PHY_GIGASR_1000FULLC    (1<<11) // Link Partner 1000Base-T Full-Duplex Capability
#define PHY_GIGASR_RRS          (1<<12) // Remote Recevier Status
#define PHY_GIGASR_LRS          (1<<13) // Local Receiver Status
#define PHY_GIGASR_MSCR         (1<<14) // Master-Slave Configuration Resolution
#define PHY_GIGASR_MSCF         (1<<15) // Master-Slave Configuration Fault

// Extended Status Register(0xF) Bit field
#define PHY_ESR_1000THALF		(1<<12) // PHY can perform 1000Base-T half-duplex
#define PHY_ESR_1000TFULL		(1<<13) // PHY can perform 1000Base-T full-duplex
#define PHY_ESR_1000XHALF		(1<<14) // PHY can perform 1000Base-X half-duplex
#define PHY_ESR_1000XFULL		(1<<15) // PHY can perform 1000Base-X full-duplex

// Interrupt Control/Status Register(0x1B) Bit field
#define PHY_ICS_LUI				(1<< 0)	// Link-Up Interrupt
#define PHY_ICS_RFI				(1<< 1)	// Remote Fault Interrupt
#define PHY_ICS_LDI				(1<< 2)	// Link-Down Interrupt
#define PHY_ICS_LPAI			(1<< 3)	// Link Partner Acknowledge Interrupt
#define PHY_ICS_PDFI			(1<< 4)	// Parallel Detect Fault Interrupt
#define PHY_ICS_PRI				(1<< 5)	// Page Receive Interrupt
#define PHY_ICS_REI				(1<< 6)	// Receive Error Interrupt
#define PHY_ICS_JI				(1<< 7)	// Jabber Interrupt
#define PHY_ICS_LUIE			(1<< 8)	// Link-Up Interrupt Enable
#define PHY_ICS_RFIE			(1<< 9)	// Remote Fault Interrupt Enable
#define PHY_ICS_LDIE			(1<<10)	// Link-Down Interrupt Enable
#define PHY_ICS_LPAIE			(1<<11)	// Link Partner Acknowledge Interrupt Enable
#define PHY_ICS_PDFIE			(1<<12)	// Parallel Detect Fault Interrupt Enable
#define PHY_ICS_PRIE			(1<<13)	// Page Receive Interrupt Enable
#define PHY_ICS_REIE			(1<<14)	// Receive Error Interrupt Enable
#define PHY_ICS_JIE				(1<<15)	// Jabber Interrupt Enable

// PHY Control 1 Register(0x1E) Bit field
#define PHY_PC1_OMI				(0x0007)// Operation Mode Indication
#define PHY_PC1_ISO				(1<< 3)	// PHY Isolate
#define PHY_PC1_ED				(1<< 4)	// Energy Detect
#define PHY_PC1_MDI				(1<< 5)	// MDI/MDI-X State
#define PHY_PC1_PS				(1<< 7)	// Polarity Status
#define PHY_PC1_LS				(1<< 8)	// Link Status
#define PHY_PC1_EP				(1<< 9)	// Enable Pause

// PHY Control 2 Register(0x1F) Bit field
#define PHY_PC2_DDS				(1<< 0)	// Disable scrambler
#define PHY_PC2_SEQ				(1<< 1)	// Enable SQE test
#define PHY_PC2_RLOOP			(1<< 2)	// Remote Loopback
#define PHY_PC2_DIS_TRANS		(1<< 3)	// Disable Transmitter
#define PHY_PC2_LED				(0x0030)// LED Mode
#define PHY_PC2_EJ				(1<< 8)	// Enable Jabber
#define PHY_PC2_IL				(1<< 9)	// Interrupt Level
#define PHY_PC2_PS				(1<<10)	// Power Saving
#define PHY_PC2_FL				(1<<11)	// Force Link
#define PHY_PC2_PSD				(1<<13)	// Pair Swap Disable
#define PHY_PC2_MD_MDI			(1<<14)	// MDI/MDI-X Select
#define PHY_PC2_HP_MDI			(1<<15)	// HP_MDIX

#endif // __KSZ8081MNX_H__
