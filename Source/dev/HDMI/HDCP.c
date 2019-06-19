/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

//#include "../cpu_main.h"


//#include <stdio.h>
#include "dev.h"
#include "defs.h"
#include "TypeDefs.h"
#include "Constants.h"
//#include "gpio.h"
#include "TPI_Regs.h"
#include "Externals.h"
#include "Macros.h"
#include "TPI_Access.h"
#include "TPI.h"
#include "HDCP.h"
//#include "util.h"
#include "AV_Config.h"

#ifdef DEV_SUPPORT_HDCP

#define AKSV_SIZE              5
#define NUM_OF_ONES_IN_KSV    20


byte HDCP_TxSupports;
byte HDCP_AksvValid;
byte HDCP_Started;
byte HDCP_LinkProtectionLevel;

static byte AreAKSV_OK(void);
static byte IsHDCP_Supported(void);
static void HDCP_On(void);

#ifdef CHECKREVOCATIONLIST
static byte CheckRevocationList(void);
#endif

#ifdef READKSV
static byte GetKSV(void);
static byte IsRepeater(void);
#endif


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   HDCP_Init()
//
// PURPOSE      :   Tests Tx and Rx support of HDCP. If found, checks if
//                  and attempts to set the security level accordingly.
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :	HDCP_TxSupports - initialized to FALSE, set to TRUE if supported by this device
//					HDCP_AksvValid - initialized to FALSE, set to TRUE if valid AKSVs are read from this device
//					HDCP_Started - initialized to FALSE
//					HDCP_LinkProtectionLevel - initialized to (EXTENDED_LINK_PROTECTION_NONE | LOCAL_LINK_PROTECTION_NONE)
//
// RETURNS      :   None
//
//////////////////////////////////////////////////////////////////////////////

void HDCP_Init (void)
{

	_printf((">>HDCP_Init()\r\n"),0,0);

	HDCP_TxSupports = FALSE;
	HDCP_AksvValid = FALSE;
	HDCP_Started = FALSE;
	HDCP_LinkProtectionLevel = EXTENDED_LINK_PROTECTION_NONE | LOCAL_LINK_PROTECTION_NONE;

	// This is TX-related... need only be done once.
    if (!IsHDCP_Supported())
    {
		// The TX does not support HDCP, so authentication will never be attempted.
		// Video will be shown as soon as TMDS is enabled.
		_printf(("HDCP -> TX does not support HDCP\n"),0,0);
		return;
	}

	HDCP_TxSupports = TRUE;

	// This is TX-related... need only be done once.
    if (!AreAKSV_OK())
    {
		// The TX supports HDCP, but does not have valid AKSVs.
		// Video will not be shown.
        _printf(("HDCP -> Illegal AKSV\n"),0,0);
        return;
    }

	HDCP_AksvValid = TRUE;

#ifdef KSVFORWARD
	// Enable the KSV Forwarding feature and the KSV FIFO Intererrupt
	ReadModifyWriteTPI(TPI_HDCP_CONTROL_DATA_REG, KSV_FORWARD_MASK, KSV_FORWARD_ENABLE);
	ReadModifyWriteTPI(TPI_KSV_FIFO_READY_INT_EN, KSV_FIFO_READY_EN_MASK, KSV_FIFO_READY_ENABLE);
#endif

	_printf(("HDCP -> Supported by TX, AKSVs valid\r\n"),0,0);
}

void HDCP_CheckStatus (byte InterruptStatusImage)
{
	byte QueryData;
	byte LinkStatus;
	byte RegImage;
	byte NewLinkProtectionLevel;
#ifdef READKSV
	byte RiCnt;
#endif
#ifdef KSVFORWARD
	byte ksv;
#endif

	if ((HDCP_TxSupports == TRUE) && (HDCP_AksvValid == TRUE))
	{
		if ((HDCP_LinkProtectionLevel == (EXTENDED_LINK_PROTECTION_NONE | LOCAL_LINK_PROTECTION_NONE)) && (HDCP_Started == FALSE))
		{
			QueryData = ReadByteTPI(TPI_HDCP_QUERY_DATA_REG);

			if (QueryData & PROTECTION_TYPE_MASK)   // Is HDCP avaialable
			{
				HDCP_On();
			}
		}

		// Check if Link Status has changed:
		if (InterruptStatusImage & SECURITY_CHANGE_EVENT)
		{
			_printf (("HDCP -> "),0,0);

			LinkStatus = ReadByteTPI(TPI_HDCP_QUERY_DATA_REG);
			LinkStatus &= LINK_STATUS_MASK;

			ClearInterrupt(SECURITY_CHANGE_EVENT);

			switch (LinkStatus)
			{
				case LINK_STATUS_NORMAL:
					_printf (("Link = Normal\n"),0,0);
					break;

				case LINK_STATUS_LINK_LOST:
					_printf (("Link = Lost\n"),0,0);
					RestartHDCP();
					break;

				case LINK_STATUS_RENEGOTIATION_REQ:
					_printf (("Link = Renegotiation Required\n"),0,0);
					HDCP_Off();
					HDCP_On();
					break;

				case LINK_STATUS_LINK_SUSPENDED:
					_printf (("Link = Suspended\n"),0,0);
					HDCP_On();
					break;
			}
		}

		// Check if HDCP state has changed:
		if (InterruptStatusImage & HDCP_CHANGE_EVENT)
		{
			RegImage = ReadByteTPI(TPI_HDCP_QUERY_DATA_REG);

			NewLinkProtectionLevel = RegImage & (EXTENDED_LINK_PROTECTION_MASK | LOCAL_LINK_PROTECTION_MASK);
			if (NewLinkProtectionLevel != HDCP_LinkProtectionLevel)
			{
				_printf (("HDCP -> "),0,0);

				HDCP_LinkProtectionLevel = NewLinkProtectionLevel;

				switch (HDCP_LinkProtectionLevel)
				{
					case (EXTENDED_LINK_PROTECTION_NONE | LOCAL_LINK_PROTECTION_NONE):
						_printf (("Protection = None\n"),0,0);
						RestartHDCP();
						break;

					case LOCAL_LINK_PROTECTION_SECURE:

						ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, AV_MUTE_MASK, AV_MUTE_NORMAL);
						_printf (("Protection = Local, Video Unmuted\n"),0,0);
						break;

					case (EXTENDED_LINK_PROTECTION_SECURE | LOCAL_LINK_PROTECTION_SECURE):
						_printf (("Protection = Extended\n"),0,0);
#ifdef READKSV
 						if (IsRepeater())
						{
							RiCnt = ReadIndexedRegister(INDEXED_PAGE_0, 0x25);
							while (RiCnt > 0x70)  // Frame 112
							{
								RiCnt = ReadIndexedRegister(INDEXED_PAGE_0, 0x25);
							}
							ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, 0x06, 0x06);
							GetKSV();
							RiCnt = ReadByteTPI(TPI_SYSTEM_CONTROL_DATA_REG);
							ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, 0x08, 0x00);
						}
#endif
						break;

					default:
						_printf (("Protection = Extended but not Local?\n"),0,0);
						RestartHDCP();
						break;
				}
			}

#ifdef KSVFORWARD
			// Check if KSV FIFO is ready and forward - Bug# 17892
			// If interrupt never goes off:
		 	//   a) KSV formwarding is not enabled
			//   b) not a repeater
			//   c) a repeater with device count == 0
			// and therefore no KSV list to forward
			if ((ReadByteTPI(TPI_KSV_FIFO_READY_INT) & KSV_FIFO_READY_MASK) == KSV_FIFO_READY_YES)
			{
				ReadModifyWriteTPI(TPI_KSV_FIFO_READY_INT, KSV_FIFO_READY_MASK, KSV_FIFO_READY_YES);
				TPI_DEBUG_PRINT (("KSV Fwd: KSV FIFO has data...\n"));
				{
					// While !(last byte has been read from KSV FIFO)
					// if (count = 0) then a byte is not in the KSV FIFO yet, do not read
					//   else read a byte from the KSV FIFO and forward it or keep it for revocation check
					do
					{
						ksv = ReadByteTPI(TPI_KSV_FIFO_STATUS_REG);
						if (ksv & KSV_FIFO_COUNT_MASK)
						{
							TPI_DEBUG_PRINT (("KSV Fwd: KSV FIFO Count = %d, ", (int)(ksv & KSV_FIFO_COUNT_MASK)));
							ksv = ReadByteTPI(TPI_KSV_FIFO_VALUE_REG);	// Forward or store for revocation check
							TPI_DEBUG_PRINT (("Value = %d\n", (int)ksv));
						}
					} while ((ksv & KSV_FIFO_LAST_MASK) == KSV_FIFO_LAST_NO);
					TPI_DEBUG_PRINT (("KSV Fwd: Last KSV FIFO forward complete\n"));
				}
			}
#endif
			ClearInterrupt(HDCP_CHANGE_EVENT);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   IsHDCP_Supported()
//
// PURPOSE      :   Check Tx revision number to find if this Tx supports HDCP
//                  by reading the HDCP revision number from TPI register 0x30.
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   None
//
// RETURNS      :   TRUE if Tx supports HDCP. FALSE if not.
//
//////////////////////////////////////////////////////////////////////////////

byte IsHDCP_Supported(void)
{
    byte HDCP_Rev;
	byte HDCP_Supported;

	_printf((">>IsHDCP_Supported()\r\n"),0,0);

	HDCP_Supported = TRUE;

	// Check Device ID
    HDCP_Rev = ReadByteTPI(TPI_HDCP_REVISION_DATA_REG);

    if (HDCP_Rev != (HDCP_MAJOR_REVISION_VALUE | HDCP_MINOR_REVISION_VALUE))
	{
    	HDCP_Supported = FALSE;
	}

#ifdef SiI_9022AYBT_DEVICEID_CHECK
	// Even if HDCP is supported check for incorrect Device ID
	HDCP_Rev = ReadByteTPI(TPI_AKSV_1_REG);
	if (HDCP_Rev == 0x90)
	{
		HDCP_Rev = ReadByteTPI(TPI_AKSV_2_REG);
		if (HDCP_Rev == 0x22)
		{
			HDCP_Rev = ReadByteTPI(TPI_AKSV_3_REG);
			if (HDCP_Rev == 0xA0)
			{
				HDCP_Rev = ReadByteTPI(TPI_AKSV_4_REG);
				if (HDCP_Rev == 0x00)
				{
					HDCP_Rev = ReadByteTPI(TPI_AKSV_5_REG);
					if (HDCP_Rev == 0x00)
					{
						HDCP_Supported = FALSE;
					}
				}
			}
		}
	}
#endif
	return HDCP_Supported;
}

#ifdef READKSV
//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   IsRepeater()
//
// PURPOSE      :   Test if sink is a repeater
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   None
//
// RETURNS      :   TRUE if sink is a repeater. FALSE if not.
//
//////////////////////////////////////////////////////////////////////////////
byte IsRepeater(void)
{
    byte RegImage;

	_printf((">>IsRepeater()\n"),0,0);

    RegImage = ReadByteTPI(TPI_HDCP_QUERY_DATA_REG);

    if (RegImage & HDCP_REPEATER_MASK)
        return TRUE;

    return FALSE;           // not a repeater
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   HDCP_On()
//
// PURPOSE      :   Switch hdcp on.
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   HDCP_Started set to TRUE
//
// RETURNS      :   None
//
//////////////////////////////////////////////////////////////////////////////

void HDCP_On(void)
{
	if (g_HDCP_Override == FALSE)
	{
		_printf(("HDCP Started\n"),0,0);

		WriteByteTPI(TPI_HDCP_CONTROL_DATA_REG, PROTECTION_LEVEL_MAX);

		HDCP_Started = TRUE;
	}
	else
	{
		HDCP_Started = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   HDCP_Off()
//
// PURPOSE      :   Switch hdcp off.
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   HDCP_Started set to FALSE
//
// RETURNS      :   None
//
//////////////////////////////////////////////////////////////////////////////

void HDCP_Off(void)
{
	_printf((">>HDCP_Off()\n"),0,0);

	// AV MUTE
	ReadModifyWriteTPI(TPI_SYSTEM_CONTROL_DATA_REG, AV_MUTE_MASK, AV_MUTE_MUTED);
	WriteByteTPI(TPI_HDCP_CONTROL_DATA_REG, PROTECTION_LEVEL_MIN);

	HDCP_Started = FALSE;
	HDCP_LinkProtectionLevel = EXTENDED_LINK_PROTECTION_NONE | LOCAL_LINK_PROTECTION_NONE;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   GetKSV()
//
// PURPOSE      :   Collect all downstrean KSV for verification
//
// INPUT PARAMS :   None
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   KSV_Array[]
//
// RETURNS      :   TRUE if KSVs collected successfully. False if not.
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The buffer is limited to KSV_ARRAY_SIZE due to the 8051 implementation.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//////////////////////////////////////////////////////////////////////////////
#ifdef READKSV
static byte GetKSV(void)
{
	byte i;
    word KeyCount;
    byte KSV_Array[KSV_ARRAY_SIZE];

	TPI_TRACE_PRINT((">>GetKSV()\n"));
    ReadBlockHDCP(DDC_BSTATUS_ADDR_L, 1, &i);
    KeyCount = (i & DEVICE_COUNT_MASK) * 5;
	if (KeyCount != 0)
	{
		ReadBlockHDCP(DDC_KSV_FIFO_ADDR, KeyCount, KSV_Array);
	}

	/*
	TPI_TRACE_PRINT(("KeyCount = %d\n", (int) KeyCount));
	for (i=0; i<KeyCount; i++)
	{
		TPI_TRACE_PRINT(("KSV[%2d] = %2.2X\n", (int) i, (int) KSV_Array[i]));
	}
	*/

	 return TRUE;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  AreAKSV_OK()
//
// PURPOSE       :  Check if AKSVs contain 20 '0' and 20 '1'
//
// INPUT PARAMS  :  None
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  TBD
//
// RETURNS       :  TRUE if 20 zeros and 20 ones found in AKSV. FALSE OTHERWISE
//
//////////////////////////////////////////////////////////////////////////////
static byte AreAKSV_OK(void)
{
    byte B_Data[AKSV_SIZE];
    byte NumOfOnes = 0;

    byte i;
    byte j;

	TPI_TRACE_PRINT((">>AreAKSV_OK()\n"));

    ReadBlockTPI(TPI_AKSV_1_REG, AKSV_SIZE, B_Data);

    for (i=0; i < AKSV_SIZE; i++)
    {
        for (j=0; j < BYTE_SIZE; j++)
        {
            if (B_Data[i] & 0x01)
            {
                NumOfOnes++;
            }
            B_Data[i] >>= 1;
        }
     }
     if (NumOfOnes != NUM_OF_ONES_IN_KSV)
        return FALSE;

    return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  CheckRevocationList()
//
// PURPOSE       :  Compare KSVs to those included in a revocation list
//
// INPUT PARAMS  :  None
//
// OUTPUT PARAMS :  None
//
// GLOBALS USED  :  TBD
//
// RETURNS       :  TRUE if no illegal KSV found in BKSV list
//
// NOTE			 :	Currently this function is implemented as a place holder only
//
//////////////////////////////////////////////////////////////////////////////
#ifdef CHECKREVOCATIONLIST
static byte CheckRevocationList(void)
{
	_printf((">>CheckRevocationList()\n"),0,0);
    return TRUE;
}
#endif

#endif
