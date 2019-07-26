#ifndef __KSZ8081RNB_H__
#define __KSZ8081RNB_H__

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

// Address (16 bit base) : KSZ8081RNB-Defined Registers
#define ETHPHY_DRCR_ADR					0x10	// Digital Reserved Control Register
#define ETHPHY_AFECR_ADR				0x11	// AFE Control 1 Register
#define ETHPHY_RXERCR_ADR				0x15	// RXER Counter Register
#define ETHPHY_OMSOR_ADR				0x16	// Operation Mode Strap Override Register
#define ETHPHY_OMSSR_ADR				0x17	// Operation Mode Strap Status Register
#define ETHPHY_EXPCR_ADR				0x18	// Expanded Control Register
#define ETHPHY_ICSR_ADR					0x1B	// Interrupt Control/Status Register
#define ETHPHY_LKMDR_ADR				0x1D	// LinkMD Control/Status Register
#define ETHPHY_PHYC1R_ADR				0x1E	// PHY Control 1 Register
#define ETHPHY_PHYC2R_ADR				0x1F	// PHY Control 2 Register

//-------------------------------------------------------------------------------------------------
// Basic Control Register(0x0) Bit field
#define ETHPHY_BCR_RESET				(1<<15) // Reset (1:Software reset 0:Normal operation)
#define ETHPHY_BCR_LOOPBACK				(1<<14) // Loopback (1:Loopback, 0:Normal operation)
#define ETHPHY_BCR_SPEED				(1<<13) // Speed Select (1:100Mbps, 0:10Mbps)
#define ETHPHY_BCR_AUTONEG_EN			(1<<12) // Auto-Negotiation Enable (1:Enable 0:Disable)
#define ETHPHY_BCR_PWR_DOWN				(1<<11) // Power Down (1:Power-down mode 0:Normal operation)
#define ETHPHY_BCR_ISOLATE				(1<<10) // Isolate (1:Electrical isolation of PHY from MII 0:Normal operation)
#define ETHPHY_BCR_RST_AUTONEG			(1<< 9) // Restart Auto-Negotiation (1:Restart auto-negotiation process 0:Normal operation)
#define ETHPHY_BCR_FULLDPLX				(1<< 8) // Duplex Mode (1:Full-duplex 0:Half-duplex)
#define ETHPHY_BCR_COL_TEST				(1<< 7) // Collision Test (1:Enable 0:Disable)

// Basic Status Register(0x1) Bit field
#define ETHPHY_BSR_100BT4				(1<<15) // 100BASE-T4 Capable
#define ETHPHY_BSR_100BTXFD				(1<<14) // 100BASE-TX Full Duplex
#define ETHPHY_BSR_100BTXHD				(1<<13) // 100BASE-TX Half Duplex
#define ETHPHY_BSR_10BTFD				(1<<12) // 10BASE-T Full Duplex
#define ETHPHY_BSR_10BTHD				(1<<11) // 10BASE-T Half Duplex
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

// Digital Reserved Control Register(0x10) Bit field
#define ETHPHY_DRCR_PLL_OFF				(1<< 4)	// PLL Off

// AFE Control 1 Register(0x11) Bit field
#define ETHPHY_AFECR_SLOW_OSCILLATOR	(1<< 5)	// Slow-Oscillator Mode Enable

// RXER Counter Register(0x15) Bit field
#define ETHPHY_RXERCR_COUNTER			(0xFFFF)// RXER Counter

// Operation Mode Strap Override Register(0x16) Bit field
#define ETHPHY_OMSOR_PHYAD				(1<<15)	// Reserved Factory Mode
#define ETHPHY_OMSOR_BCAST				(1<< 9)	// B-CAST_OFF Override
#define ETHPHY_OMSOR_MII_B2B			(1<< 7)	// MII B-to-B Override
#define ETHPHY_OMSOR_RMII_B2B			(1<< 6)	// RMII B-to-B Override
#define ETHPHY_OMSOR_NAND				(1<< 5)	// NAND Tree Override
#define ETHPHY_OMSOR_RMII				(1<< 1)	// RMII Override
#define ETHPHY_OMSOR_MII				(1<< 0)	// MII Override

// Operation Mode Strap Status Register(0x17) Bit field
#define ETHPHY_OMSSR_PHYAD				(0xE000)// PHYAD[2:0] Strap-In Status
#define ETHPHY_OMSSR_BCAST				(1<< 9)	// B-CAST_OFF Strap-In Status
#define ETHPHY_OMSSR_MII_B2B			(1<< 7)	// MII B-to-B Strap-In Status
#define ETHPHY_OMSSR_RMII_B2B			(1<< 6)	// RMII B-to-B Strap-In Status
#define ETHPHY_OMSSR_NAND				(1<< 5)	// NAND Tree Strap-In Status
#define ETHPHY_OMSSR_RMII				(1<< 1)	// RMII Strap-In Status
#define ETHPHY_OMSSR_MII				(1<< 0)	// MII Strap-In Status

// Expanded Control Register(0x18) Bit field
#define ETHPHY_EXPCR_EDPD_DIS			(1<<11)	// EDPD Disabled
#define ETHPHY_EXPCR_100TX_LATENCY		(1<<10)	// 100BASE-TX Latency
#define ETHPHY_EXPCR_10T_PRE_RESTORE	(1<< 6)	// 10BASE-T Preamble Restore

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

// LinkMD Control/Status Register(0x1D) Bit field
#define ETHPHY_LKMDR_CABLE_TEST_EN		(1<<15)	// Cable Diagnostic Test Enable
#define ETHPHY_LKMDR_CABLE_TEST_RES		(0x6000)// Cable Diagnostic Test Result
#define ETHPHY_LKMDR_SHORT_CABLE		(1<<12)	// Short Cable Indicator
#define ETHPHY_LKMDR_CABLE_FAULT_CNT	(0x01FF)// Cable Fault Counter

// PHY Control 1 Register(0x1E) Bit field
#define ETHPHY_PC1R_ENABLE_PAUSE		(1<< 9)	// Enable Pause
#define ETHPHY_PC1R_LINK_STATUS			(1<< 8)	// Link Status
#define ETHPHY_PC1R_POLARITY_STATUS		(1<< 7)	// Polarity Status
#define ETHPHY_PC1R_MDI_STATUS			(1<< 5)	// MDI/MDI-X State
#define ETHPHY_PC1R_ENERGY_DETECT		(1<< 4)	// Energy Detect
#define ETHPHY_PC1R_ISOLATE				(1<< 3)	// PHY Isolate
#define ETHPHY_PC1R_OPERATION_MI		(0x0007)// Operation Mode Indication

// PHY Control 2 Register(0x1F) Bit field
#define ETHPHY_PC2R_HP_MDI				(1<<15)	// HP_MDIX
#define ETHPHY_PC2R_MD_MDI				(1<<14)	// MDI/MDI-X Select
#define ETHPHY_PC2R_PAIR_SWAP_DIS		(1<<13)	// Pair Swap Disable
#define ETHPHY_PC2R_FOREC_LINK			(1<<11)	// Force Link
#define ETHPHY_PC2R_POWER_SAVING		(1<<10)	// Power Saving
#define ETHPHY_PC2R_INT_LEVEL			(1<< 9)	// Interrupt Level
#define ETHPHY_PC2R_EN_JABBER			(1<< 8)	// Enable Jabber
#define ETHPHY_PC2R_RMII_CLK_MODE		(1<< 7) // RMII Reference Clock Select
#define ETHPHY_PC2R_LED_MODE			(0x0030)// LED Mode
#define ETHPHY_PC2R_DIS_TRANSMITTER		(1<< 3)	// Disable Transmitter
#define ETHPHY_PC2R_REMOTE_LOOP			(1<< 2)	// Remote Loopback
#define ETHPHY_PC2R_EN_SEQ_TEST			(1<< 1)	// Enable SQE test
#define ETHPHY_PC2R_DIS_SCRAMBLER		(1<< 0)	// Disable scrambler

#endif // __KSZ8081RNB_H__
