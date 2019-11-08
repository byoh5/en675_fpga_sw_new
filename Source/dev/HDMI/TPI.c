/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

//#include "../cpu_main.h"

#include "dev.h"
// Needs to be rearranged and minimized
#include "defs.h"
#include "TypeDefs.h"
#include "Constants.h"
//#include "gpio.h"
#include "Externals.h"
#include "Macros.h"
#include "TPI_Regs.h"
#include "TPI_Access.h"
#include "TPI.h"
//#include "util.h"
#include "AV_Config.h"
#include "i2c_master_sw.h"
#include "si_apiConfigure.h"

// Serial Communication Buffer assignments and macros
#define CommandLength				g_CommData[1]
#define Arg(a)                      g_CommData[(2+a)]

// May be empty depending on project definitions
#include "EDID.h"
#include "HDCP.h"

#ifdef F_9136
#include "eHDMI.h"
#endif


#if defined SiI9232_OR_SiI9236
void InitCBusRegs(void);
#endif

#if (IS_CEC == 1)
extern BYTE SI_CecInit (void);
extern byte SI_CecHandler (byte, BYTE);
#endif

static void TxHW_Reset (void);
static BYTE StartTPI (void);

//static void WakeUpFromD3 (void);
static void InitializeStateVariables (void);

static void HotPlugService (void);

static BYTE EnableInterrupts (byte);
static BYTE DisableInterrupts (byte);

static BYTE SetPreliminaryInfoFrames (void);

#ifdef MHD_CABLE_HPD
static void InitForceUsbIdSwitchOpen (void);
static void InitReleaseUsbIdSwitchOpen (void);

static void ForceUsbIdSwitchOpen (void);
static void ReleaseUsbIdSwitchOpen (void);

static void OnMobileHdCableConnected (void);
static void OnMobileHdCableDisconnected (void);
#endif

static void OnHdmiCableConnected (void);
static void OnHdmiCableDisconnected (void);

static void OnDownstreamRxPoweredDown (void);
static void OnDownstreamRxPoweredUp (void);

static void TxPowerStateD0 (void);
static void TxPowerStateD2 (void);

#ifdef RCP_ENABLE
extern void CpCbusInitialize(void);
extern void CpCbusHandler(void);
#endif


#define T_EN_TPI       	10
#define T_HPD_DELAY    	10

byte txPowerState;		// Can be referenced externally by chip-specific TX HAL file, so cannot be static.

static BYTE tmdsPoweredUp;
static BYTE hdmiCableConnected;
static BYTE dsRxPoweredUp;

#ifdef MHD_CABLE_HPD

#define ENABLE_AUTO_SOFT_RESET			0x04
#define DISABLE_AUTO_SOFT_RESET			0x00
#define ASR_VALUE						ENABLE_AUTO_SOFT_RESET

#define DDC_XLTN_TIMEOUT_MAX_VAL		0x30

static BYTE tclkStable;
static BYTE mobileHdCableConnected;
static BYTE checkTclkStable;
static byte cbusRevID;
#endif

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      : TPI_Init()
//
// PURPOSE       : TPI initialization: HW Reset, Interrupt enable.
//
// INPUT PARAMS  : None
//
// OUTPUT PARAMS : void
//
// GLOBALS USED  : None
//
// RETURNS      :   TRUE
//
//////////////////////////////////////////////////////////////////////////////

BYTE TPI_Init(void)
{
//		int i = 0;
	_printf(">>TPI_Init()\r\n");

	txPowerState = TX_POWER_STATE_D0;				// Chip powers up in D2 mode, but init to D0 for testing prupose.

	InitializeStateVariables();

	TxHW_Reset();									// Toggle TX reset pin

	if (StartTPI())									// Enable HW TPI mode, check device ID
	{
		TxPowerStateD0();							// 1-3 Power up transmitter

#ifdef DEV_SUPPORT_HDCP
		g_HDCP_Override = FALSE;
		HDCP_Init();
#endif
		return TRUE;
	}

	return FALSE;
}


static void InitializeStateVariables (void)
{
	tmdsPoweredUp = FALSE;
	hdmiCableConnected = FALSE;
	dsRxPoweredUp = FALSE;
	edidDataValid = FALSE;							// Move this into EDID_Init();
}


#if defined SiI9232_OR_SiI9236
static void ForceUsbIdSwitchOpen (void)
{
	ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x90, BIT_0, 0x00);				// Disable discovery
	ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x95, BIT_6, BIT_6);				// Force USB ID Switch
	WriteIndexedRegister(INDEXED_PAGE_0, 0x92, 0x86);								// Force USB, CBUS discovery
	WriteIndexedRegister(INDEXED_PAGE_0, 0x93, 0x0C);								// Disable CBUS pull-up during RGND measurement
	ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x79, BIT_5 | BIT_4, BIT_4);		// Force HPD to 0 when not in MobileHD mode.
}


static void ReleaseUsbIdSwitchOpen (void)
{
	DelayMS(25);
	ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x95, BIT_6, 0x00);
	ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x90, BIT_0, BIT_0);				// Enable discovery
}


static void InitForceUsbIdSwitchOpen (void)
{
	I2C_WriteByte(0x72, 0x90, 0x26);					// Disable CBUS discovery
	ReadModifyWriteTPI(0x95, BIT_6, BIT_6);				// Force USB ID switch to open
	WriteByteTPI(0x92, 0x86);							//
	ReadModifyWriteTPI(0x79, BIT_5 | BIT_4, BIT_4);		// Force HPD to 0 when not in Mobile HD mode.
	//pinGpio1 = 0;
}


static void InitReleaseUsbIdSwitchOpen (void)
{
	DelayMS(25);
	ReadModifyWriteTPI(0x95, BIT_6, 0x00);				// Release USB ID switch
	ReadModifyWriteTPI(0x90, BIT_0, BIT_0);				// Enable CBUS discovery
}
#endif

#ifdef MHD_CABLE_HPD
static void GoToD3 (void)
{

#if defined SYS_BOARD_STARTERKIT
	//pinWol2MhdRxPwr = 1;
#ifdef SK_X02
	pinM2U_VBusCtrl = 0;
#else
	pinVBusIsolate = 0;
#endif
	pinMhdConn = 1;
	pinUsbConn = 0;
#endif

	TxPowerStateD2();

	mobileHdCableConnected = FALSE;
	hdmiCableConnected = FALSE;
	dsRxPoweredUp = FALSE;
}
#endif


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   HotPlugService()
//
// PURPOSE      :   Implement Hot Plug Service Loop activities
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   void
//
// GLOBALS USED :   LinkProtectionLevel
//
// RETURNS      :   An error code that indicates success or cause of failure
//
//////////////////////////////////////////////////////////////////////////////

void HotPlugService (void)
{
	_printf(">>HotPlugService()\r\n");

	DisableInterrupts(0xFF);

	InitVideo(X1);						// Set PLL Multiplier to x1 upon power up

#ifndef POWER_STATE_D0_AFTER_TMDS_ENABLE
	TxPowerStateD0();
#endif

	if (IsHDMI_Sink())											// Set InfoFrames only if HDMI output mode
	{
		SetPreliminaryInfoFrames();
//#if defined SiI9232_OR_SiI9236
//#else
//			SetBasicAudio();	// set audio interface to basic audio (an external command is needed to set to any other mode	// 160119 HSH
//#endif
	}
	else
	{
		SetAudioMute(AUDIO_MUTE_MUTED);
	}

#ifdef DEV_SUPPORT_HDCP
	if ((HDCP_TxSupports == TRUE) && (VideoModeDescription.HDCPAuthenticated == VMD_HDCP_AUTHENTICATED))
	{
		if (HDCP_AksvValid == TRUE)
		{
			// AV MUTE
			_printf("TMDS -> Enabled (Video Muted)\r\n");
			ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, LINK_INTEGRITY_MODE_MASK | TMDS_OUTPUT_CONTROL_MASK
					| AV_MUTE_MASK, LINK_INTEGRITY_DYNAMIC | TMDS_OUTPUT_CONTROL_ACTIVE | AV_MUTE_MUTED);
			tmdsPoweredUp = TRUE;

			EnableInterrupts(HOT_PLUG_EVENT | RX_SENSE_EVENT | AUDIO_ERROR_EVENT | SECURITY_CHANGE_EVENT | V_READY_EVENT | HDCP_CHANGE_EVENT);
		}

	}
	else
#endif
	{
		_printf("TMDS -> Enabled\r\n");
		ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, LINK_INTEGRITY_MODE_MASK | TMDS_OUTPUT_CONTROL_MASK | AV_MUTE_MASK, LINK_INTEGRITY_DYNAMIC | TMDS_OUTPUT_CONTROL_ACTIVE | AV_MUTE_NORMAL);
		EnableInterrupts(HOT_PLUG_EVENT | RX_SENSE_EVENT | AUDIO_ERROR_EVENT | V_READY_EVENT);
	}

#ifdef POWER_STATE_D0_AFTER_TMDS_ENABLE
	TxPowerStateD0();
#endif

	//ReadModifyWriteTPI(TPI_END_RIGHT_BAR_MSB, 0x00, 0x00);
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      : HW_Reset()
//
// PURPOSE       : Send a
//
// INPUT PARAMS  : None
//
// OUTPUT PARAMS : void
//
// GLOBALS USED  : None
//
// RETURNS       : Void
//
//////////////////////////////////////////////////////////////////////////////

static void TxHW_Reset(void)
{
	_printf(">>TxHW_Reset()\r\n");

	DelayMS(TX_HW_RESET_PERIOD);

	// Does this need to be done for every chip? Should it be moved into TXHAL_InitPostReset() for each applicable device?
	I2C_WriteByte(0x72, 0x7C, 0x14);					// HW debounce to 64ms (0x14)
#ifdef F_9136
	I2C_WriteByte(0x72, 0x82, 0xA5);
#endif

}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      : StartTPI()
//
// PURPOSE       : Start HW TPI mode by writing 0x00 to TPI address 0xC7. This
//                 will take the Tx out of power down mode.
//
// INPUT PARAMS  : None
//
// OUTPUT PARAMS : void
//
// GLOBALS USED  : None
//
// RETURNS       : TRUE if HW TPI started successfully. FALSE if failed to.
//
//////////////////////////////////////////////////////////////////////////////

static BYTE StartTPI(void)
{
 	byte devID = 0x00;
	byte devRID = 0x00;
 	word wID = 0x0000;

 	_printf(">>StartTPI()\r\n");

	WriteByteTPI(TPI_ENABLE, 0x00);           			 // Write "0" to 72:C7 to start HW TPI mode 1-1
    DelayMS(100);

    devID = ReadIndexedRegister(INDEXED_PAGE_0, 0x03);
    wID = devID;
    wID <<= 8;
    devID = ReadIndexedRegister(INDEXED_PAGE_0, 0x02);
    wID |= devID;

    devID = ReadByteTPI(TPI_DEVICE_ID);					// 1-2

 	_printf("ID : %d\r\n", wID);

	_printf("Device ID : %d\r\n",devID);

	devRID = ReadByteTPI(TPI_DEVICE_REV_ID);
	_printf("Dev Pro Rev ID : %d\r\n",devRID);

    if (devID == SiI_DEVICE_ID)
	{
		_printf(SiI_DEVICE_STRING);
		return TRUE;
	}

    _printf("Unsupported TX\r\n");
    return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  EnableInterrupts()
//
// PURPOSE       :  Enable the interrupts specified in the input parameter
//
// INPUT PARAMS  :  A bit pattern with "1" for each interrupt that needs to be
//                  set in the Interrupt Enable Register (TPI offset 0x3C)
//
// OUTPUT PARAMS :  void
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE
//
//////////////////////////////////////////////////////////////////////////////
BYTE EnableInterrupts(byte Interrupt_Pattern)
{
    TPI_TRACE_PRINT(">>EnableInterrupts()\r\n");
    ReadSetWriteTPI(TPI_INTERRUPT_ENABLE_REG, Interrupt_Pattern);

#if defined SiI9232_OR_SiI9236
	WriteIndexedRegister(INDEXED_PAGE_0, 0x75, BIT_5);
#endif

    return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  DisableInterrupts()
//
// PURPOSE       :  Enable the interrupts specified in the input parameter
//
// INPUT PARAMS  :  A bit pattern with "1" for each interrupt that needs to be
//                  cleared in the Interrupt Enable Register (TPI offset 0x3C)
//
// OUTPUT PARAMS :  void
//
// GLOBALS USED  :  None
//
// RETURNS       :  TRUE
//
//////////////////////////////////////////////////////////////////////////////
static BYTE DisableInterrupts(byte Interrupt_Pattern)
{
 	_printf(">>DisableInterrupts()\r\n");
 	ReadClearWriteTPI(TPI_INTERRUPT_ENABLE_REG, Interrupt_Pattern);

 	return TRUE;
}


#if defined HAS_CTRL_BUS
static BYTE CBUS_Discovery (void)
{
    byte i;

    for (i = 0; i < 20; i++)
    {
        WriteByteTPI (TPI_DEVICE_POWER_STATE_CTRL_REG, CTRL_PIN_DRIVEN_TX_BRIDGE | TX_POWER_STATE_D0);                  // Start CBUS self-discovery
        DelayMS (T_CBUSDISCOVERY_DELAY);

        if (ReadByteCBUS(0x0A) & 0x01)
        {
            TPI_DEBUG_PRINT (("CBUS discovered in %d attempt(s).\r\n", (int)(i + 1)));
             return TRUE;
        }

        WriteByteTPI (TPI_DEVICE_POWER_STATE_CTRL_REG, CTRL_PIN_TRISTATE | TX_POWER_STATE_D0);
        DelayMS (T_CBUSDISCOVERY_DELAY);
    }

    TPI_DEBUG_PRINT (("CBUS downstream device not detected.\r\n0xC8:0x0A = %02X\r\n", (int)ReadByteCBUS(0x0A)));
    return FALSE;
}
#endif


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      : SetPreliminaryInfoFrames()
//
// PURPOSE       : Set InfoFrames for default (initial) setup only
//
// INPUT PARAMS  : VIC, output mode,
//
// OUTPUT PARAMS : void
//
// GLOBALS USED  : None
//
// RETURNS       : TRUE
//
//////////////////////////////////////////////////////////////////////////////
static BYTE SetPreliminaryInfoFrames(void)
{
    byte i;
    //API_Cmd Command;        // to allow using function SetAVI_InfoFrames()

        TPI_TRACE_PRINT((">>SetPreliminaryInfoFrames()\r\n"));

    for (i = 0; i < MAX_COMMAND_ARGUMENTS; i++)
        Arg(i) = 0;

        CommandLength = 0;      // fixes SetAVI_InfoFrames() from going into an infinite loop

    Arg(0) = VideoModeDescription.VIC;

#ifdef DEV_SUPPORT_EDID
        if (EDID_Data.YCbCr_4_4_4)
        {
                Arg(3) = 0x01;
        }
        else
        {
            if (EDID_Data.YCbCr_4_2_2)
            {
                Arg(3) = 0x02;
            }
        }
#else
        Arg(3) = 0x00;
#endif

    SetAVI_InfoFrames();
    return TRUE;
}


static void TxPowerStateD0 (void)
{

	ReadModifyWriteTPI(TPI_DEVICE_POWER_STATE_CTRL_REG, TX_POWER_STATE_MASK, TX_POWER_STATE_D0);
	_printf("TX Power State D0\r\n");
	txPowerState = TX_POWER_STATE_D0;
}


static void TxPowerStateD2 (void)
{

#ifdef F_9136
		ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_0, ENABLE);
		DelayMS(10);
		ReadModifyWriteTPI(TPI_DEEP_COLOR_GCP, BIT_0, DISABLE);

		ReadModifyWriteTPI(TPI_DEVICE_POWER_STATE_CTRL_REG, TX_POWER_STATE_MASK, TX_POWER_STATE_D2);
#endif

#ifdef SiI9232_OR_SiI9236
	ReadModifyWriteIndexedRegister(INDEXED_PAGE_1, 0x3D, BIT_0, 0x00);
#endif

	_printf("TX Power State D2\r\n");
	txPowerState = TX_POWER_STATE_D0;
}


void EnableTMDS (void)
{

    _printf("TMDS -> Enabled\r\n");
    ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, TMDS_OUTPUT_CONTROL_MASK, TMDS_OUTPUT_CONTROL_ACTIVE);
    tmdsPoweredUp = TRUE;
}


void DisableTMDS (void)
{

    _printf("TMDS -> Disabled\r\n");

    // AV MUTE
    ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, TMDS_OUTPUT_CONTROL_MASK |
		    	AV_MUTE_MASK, TMDS_OUTPUT_CONTROL_POWER_DOWN | AV_MUTE_MUTED);
    tmdsPoweredUp = FALSE;
}


#ifdef DEV_SUPPORT_HDCP
void RestartHDCP (void)
{
	_printf("HDCP -> Restart\r\n");

	DisableTMDS();
	HDCP_Off();
	EnableTMDS();
}
#endif

void SetAudioMute (byte audioMute)
{
	ReadModifyWriteTPI(TPI_AUDIO_INTERFACE_REG, AUDIO_MUTE_MASK, audioMute);
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   TPI_Poll ()
//
// PURPOSE      :   Poll Interrupt Status register for new interrupts
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   LinkProtectionLevel
//
// RETURNS      :   None
//
//////////////////////////////////////////////////////////////////////////////

extern BYTE gbVideoChg;

void TPI_Poll (void)
{
	byte InterruptStatusImage;

//	_printf (("TPI_POLL START \r\n"),0,0);


	if (txPowerState == TX_POWER_STATE_D0)
	{
//		_printf (("TX_POWER_STATE_D0 INSERT \r\n"),0,0);
		InterruptStatusImage = ReadByteTPI(TPI_INTERRUPT_STATUS_REG);

		if (InterruptStatusImage & HOT_PLUG_EVENT)
		{

			_printf("HPD  ->\r\n");

			ReadSetWriteTPI(TPI_INTERRUPT_ENABLE_REG, HOT_PLUG_EVENT); 				// Enable HPD interrupt bit

			// Repeat this loop while cable is bouncing:
			do
			{
				WriteByteTPI(TPI_INTERRUPT_STATUS_REG, HOT_PLUG_EVENT);
				DelayMS(T_HPD_DELAY); // Delay for metastability protection and to help filter out connection bouncing
				InterruptStatusImage = ReadByteTPI(TPI_INTERRUPT_STATUS_REG); 		// Read Interrupt status register
			} while (InterruptStatusImage & HOT_PLUG_EVENT); 						// loop as long as HP interrupts recur

			if (((InterruptStatusImage & HOT_PLUG_STATE) >> 2) != hdmiCableConnected)
			{
				if (hdmiCableConnected == TRUE)
				{
					OnHdmiCableDisconnected();
				}
				else
				{
					OnHdmiCableConnected();
					ReadModifyWriteIndexedRegister(INDEXED_PAGE_0, 0x0A, 0x08, 0x08);
				}

				if (hdmiCableConnected == FALSE)
				{
					return;
				}
			}
		}

		// Check rx power
		if (((InterruptStatusImage & RX_SENSE_STATE) >> 3) != dsRxPoweredUp)
		{
			if (hdmiCableConnected == TRUE)
			{
				if (dsRxPoweredUp == TRUE)	OnDownstreamRxPoweredDown();
				else						OnDownstreamRxPoweredUp();
			}
			ClearInterrupt(RX_SENSE_EVENT);
		}

		// Check if Audio Error event has occurred:
		if (InterruptStatusImage & AUDIO_ERROR_EVENT)
		{
			//TPI_DEBUG_PRINT (("TP -> Audio Error Event\r\n"));
			//  The hardware handles the event without need for host intervention (PR, p. 31)
			ClearInterrupt(AUDIO_ERROR_EVENT);
		}

#ifdef DEV_SUPPORT_HDCP
		if ((hdmiCableConnected == TRUE)
				&& (dsRxPoweredUp == TRUE)
				&& (VideoModeDescription.HDCPAuthenticated == VMD_HDCP_AUTHENTICATED))
		{
			HDCP_CheckStatus(InterruptStatusImage);
		}
#endif

		if (/*(tmdsPoweredUp == TRUE) && */(gbVideoChg == TRUE))
		{
			_printf("TP -> Video Mode...\r\n");
			DisableTMDS();

#ifdef DEV_SUPPORT_HDCP
		if ((HDCP_TxSupports == TRUE)
						&& (HDCP_AksvValid == TRUE)
						&& (VideoModeDescription.HDCPAuthenticated == VMD_HDCP_AUTHENTICATED))
		{
			RestartHDCP();
		}
#endif
			HotPlugService();
			gbVideoChg = FALSE;
		}
	}

}

static void OnHdmiCableConnected(void)
{
	_printf("HDMI Connected\r\n");

	// No need to call TPI_Init here unless TX has been powered down on cable removal.
	TPI_Init();

	hdmiCableConnected = TRUE;

#ifdef HDCP_DONT_CLEAR_BSTATUS
#else
	if ((HDCP_TxSupports == TRUE) && (HDCP_AksvValid == TRUE) && (VideoModeDescription.HDCPAuthenticated == VMD_HDCP_AUTHENTICATED)) {
		WriteIndexedRegister(INDEXED_PAGE_0, 0xCE, 0x00); // Clear BStatus
		WriteIndexedRegister(INDEXED_PAGE_0, 0xCF, 0x00);
	}
#endif

#ifdef DEV_SUPPORT_EDID
		DoEdidRead();
#endif

#ifdef READKSV
	ReadModifyWriteTPI(0xBB, 0x08, 0x08);
#endif

	if (IsHDMI_Sink())              // select output mode (HDMI/DVI) according to sink capabilty
	{
		_printf("HDMI Sink Detected\r\n");
		ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, OUTPUT_MODE_MASK, OUTPUT_MODE_HDMI);
	}
	else
	{
		_printf("DVI Sink Detected\r\n");
		ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, OUTPUT_MODE_MASK, OUTPUT_MODE_DVI);
	}
}


static void OnHdmiCableDisconnected(void)
{

	_printf("HDMI Disconnected\r\n");

	hdmiCableConnected = FALSE;

#ifdef DEV_SUPPORT_EDID
	edidDataValid = FALSE;
#endif
	OnDownstreamRxPoweredDown();

#ifdef F_9136
	TxPowerStateD2();
#endif

}


static void OnDownstreamRxPoweredDown(void)
{

	_printf("DSRX -> Powered Down\r\n");
	dsRxPoweredUp = FALSE;

#ifdef DEV_SUPPORT_HDCP
	if ((HDCP_TxSupports == TRUE)
					&& (HDCP_AksvValid == TRUE)
					&& (VideoModeDescription.HDCPAuthenticated == VMD_HDCP_AUTHENTICATED))
	{
		HDCP_Off();
	}
#endif

	DisableTMDS();
}


static void OnDownstreamRxPoweredUp(void)
{
	_printf("DSRX -> Powered Up\r\n");
	dsRxPoweredUp = TRUE;

	HotPlugService();

	gbVideoChg = FALSE;
}


