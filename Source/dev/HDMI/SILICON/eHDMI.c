/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
//#include <stdio.h>
//#include "hdmi/MCU_Regs.h"
#include "dev.h"
#ifdef __HDMI_SILICON__

#include "defs.h"
#include "TypeDefs.h"
#include "si_datatypes.h"
#include "i2c_master_sw.h"
#include "Constants.h"
#include "eHDMI.h"
#include "si_apiCEC.h"


#ifdef DEV_SUPPORT_EHDMI

//static BYTE l_arcRequested = FALSE;


bool_ EHDMI_MODE;

void EHDMI_Init (void)
{
	EHDMI_MODE = 0;
}


//////////////////////////////////////////////////////////////////////////////
// Enable ARC (Audio Return Channel) in Common Mode (ARC CM)
//////////////////////////////////////////////////////////////////////////////
void EHDMI_ARC_Common_Enable(void)
{
	WriteByteEHDMI (EHDMI_BANDGAP, 0xC9); // Bandgap
	WriteByteEHDMI (EHDMI_TERMINATION, 0x0B); // Termination
	WriteByteEHDMI (EHDMI_SPDIF_1, 0x00); //
	WriteByteEHDMI (EHDMI_ARC, 0x00);
	WriteByteEHDMI (EHDMI_ETHERNET_1, 0x00); // Ethernet
	WriteByteEHDMI (EHDMI_ETHERNET_BIAS, 0x04); // Ethernet
	WriteByteEHDMI (EHDMI_ECHO, 0x00); // Echo
	WriteByteEHDMI (EHDMI_ETHERNET_PGA, 0x00); // Ethernet
	WriteByteEHDMI (EHDMI_ETHERNET_2, 0x00); // Ethernet
	WriteByteEHDMI (EHDMI_ETHERNET_3, 0x00); // Ethernet
	WriteByteEHDMI (EHDMI_SPDIF_2, 0x09);
	WriteByteEHDMI (EHDMI_SPDIF_3, 0x04);
	WriteByteEHDMI (EHDMI_CALIBRATION, 0x00);
	WriteByteEHDMI (EHDMI_TEST1, 0x05);
	WriteByteEHDMI (EHDMI_TEST2, 0x80);

	EHDMI_MODE = 2;
}

//////////////////////////////////////////////////////////////////////////////
// Disable ARC (Audio Return Channel) in Common Mode
//////////////////////////////////////////////////////////////////////////////
void EHDMI_Disable(void)
{
	WriteByteEHDMI (EHDMI_BANDGAP, 0x00); // Bandgap
	WriteByteEHDMI (EHDMI_TERMINATION, 0x00); // Termination
	WriteByteEHDMI (EHDMI_SPDIF_1, 0x00); //
	WriteByteEHDMI (EHDMI_ARC, 0x00);
	WriteByteEHDMI (EHDMI_ETHERNET_1, 0x00); // Ethernet
	WriteByteEHDMI (EHDMI_ETHERNET_BIAS, 0x00); // Ethernet
	WriteByteEHDMI (EHDMI_ECHO, 0x00); // Echo
	WriteByteEHDMI (EHDMI_ETHERNET_PGA, 0x00); // Ethernet
	WriteByteEHDMI (EHDMI_ETHERNET_2, 0x00); // Ethernet
	WriteByteEHDMI (EHDMI_ETHERNET_3, 0x00); // Ethernet
	WriteByteEHDMI (EHDMI_SPDIF_2, 0x00);
	WriteByteEHDMI (EHDMI_SPDIF_3, 0x00);
	WriteByteEHDMI (EHDMI_CALIBRATION, 0x00);
	WriteByteEHDMI (EHDMI_TEST1, 0x00);
	WriteByteEHDMI (EHDMI_TEST2, 0x00);

	EHDMI_MODE = 0;
}

//////////////////////////////////////////////////////////////////////////////
// Enable ARC (Audio Return Channel) in Single Mode Only (ARC SM)
//////////////////////////////////////////////////////////////////////////////
void EHDMI_ARC_Single_Enable(void)
{
	WriteByteEHDMI (EHDMI_BANDGAP, 0xC9); // Enable Bandgap and program reference value
	WriteByteEHDMI (EHDMI_TERMINATION, 0x0B); // Disable termination	 and program value
	WriteByteEHDMI (EHDMI_SPDIF_1, 0x00); //
	WriteByteEHDMI (EHDMI_ARC, 0x00); // Set ARC input driver
	WriteByteEHDMI (EHDMI_ETHERNET_1, 0x00); // Disable ethernet TX input and program gain
	WriteByteEHDMI (EHDMI_ETHERNET_BIAS, 0x04); // Disable HEAC differential mode driver
	WriteByteEHDMI (EHDMI_ECHO, 0x00); // Disable full duplex ethernet operation
	WriteByteEHDMI (EHDMI_ETHERNET_PGA, 0x00); // Disable ethernet RX output and program full duplex differential gain
	WriteByteEHDMI (EHDMI_ETHERNET_2, 0x00); // Disable ethernet RX output gain
	WriteByteEHDMI (EHDMI_ETHERNET_3, 0x00); // Disable ethernet RX output
	WriteByteEHDMI (EHDMI_SPDIF_2, 0x39); // Enable SPDIF output and program gain
	WriteByteEHDMI (EHDMI_SPDIF_3, 0x04); // Enable SPDIF output path driver
	WriteByteEHDMI (EHDMI_CALIBRATION, 0x00); // Disable ethernet RX gain boosting
	WriteByteEHDMI (EHDMI_TEST1, 0x04); // Enable ARC and program ethernet driver gain
	WriteByteEHDMI (EHDMI_TEST2, 0x40); // Disable differential resistor and ARC output selection, enable ARC self biasing and slew rate

	EHDMI_MODE = 1;
}


//////////////////////////////////////////////////////////////////////////////
// Enable HEC (Home Ethernet Channel) Only (HEC only)
//////////////////////////////////////////////////////////////////////////////
void EHDMI_HEC_Enable(void)
{
	WriteByteEHDMI (EHDMI_BANDGAP, 0xC9); // Enable Bandgap and program reference value
	WriteByteEHDMI (EHDMI_TERMINATION, 0x0B); // Enable termination	 and program value
	WriteByteEHDMI (EHDMI_SPDIF_1, 0x00); //
	WriteByteEHDMI (EHDMI_ARC, 0x00); // Set ARC input driver
	WriteByteEHDMI (EHDMI_ETHERNET_1, 0x21); // Enable ethernet TX input and program gain
	WriteByteEHDMI (EHDMI_ETHERNET_BIAS, 0x04); // Enable HEAC differential mode driver
	WriteByteEHDMI (EHDMI_ECHO, 0x05); // Enable full duplex ethernet operation
	WriteByteEHDMI (EHDMI_ETHERNET_PGA, 0x7F); // Enable ethernet RX output and program full duplex differential gain
	WriteByteEHDMI (EHDMI_ETHERNET_2, 0x7E); // Set ethernet RX output gain
	WriteByteEHDMI (EHDMI_ETHERNET_3, 0x01); // Enable ethernet RX output
	WriteByteEHDMI (EHDMI_SPDIF_2, 0x00); // Enable SPDIF output and program gain
	WriteByteEHDMI (EHDMI_SPDIF_3, 0x00); // Enable SPDIF output path driver
	WriteByteEHDMI (EHDMI_CALIBRATION, 0x00); // Enable ethernet RX gain boosting
	WriteByteEHDMI (EHDMI_TEST1, 0x05); // Enable ARC and program ethernet driver gain
	WriteByteEHDMI (EHDMI_TEST2, 0x80); // Enable differential resistor, ARC output selection and slew rate

	EHDMI_MODE = 3;
}

//////////////////////////////////////////////////////////////////////////////
// Enable ARC Common Mode with HEC	(HEC+ARC)
//////////////////////////////////////////////////////////////////////////////
void EHDMI_ARC_Common_With_HEC_Enable(void)
{
	WriteByteEHDMI (EHDMI_BANDGAP, 0xC9); // Enable Bandgap and program reference value
	WriteByteEHDMI (EHDMI_TERMINATION, 0x0B); // Enable termination	 and program value
	WriteByteEHDMI (EHDMI_SPDIF_1, 0x00); //
	WriteByteEHDMI (EHDMI_ARC, 0x00); // Set ARC input driver
	WriteByteEHDMI (EHDMI_ETHERNET_1, 0x21); // Enable ethernet TX input and program gain
	WriteByteEHDMI (EHDMI_ETHERNET_BIAS, 0x04); // Enable HEAC differential mode driver
	WriteByteEHDMI (EHDMI_ECHO, 0x05); // Enable full duplex ethernet operation
	WriteByteEHDMI (EHDMI_ETHERNET_PGA, 0x7F); // Enable ethernet RX output and program full duplex differential gain
	WriteByteEHDMI (EHDMI_ETHERNET_2, 0x7E); // Set ethernet RX output gain
	WriteByteEHDMI (EHDMI_ETHERNET_3, 0x01); // Enable ethernet RX output
	WriteByteEHDMI (EHDMI_SPDIF_2, 0x09); // Enable SPDIF output and program gain
	WriteByteEHDMI (EHDMI_SPDIF_3, 0x04); // Enable SPDIF output path driver
	WriteByteEHDMI (EHDMI_CALIBRATION, 0x00); // Enable ethernet RX gain boosting
	WriteByteEHDMI (EHDMI_TEST1, 0x05); // Enable ARC and program ethernet driver gain
	WriteByteEHDMI (EHDMI_TEST2, 0x80); // Enable differential resistor, ARC output selection and slew rate

	EHDMI_MODE = 4;
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   ReadByteEHDMI ()
//
// PURPOSE      :   Read one byte from a given offset of the EHDMI interface.
//
// INPUT PARAMS :   Offset of EHDMI register to be read; A pointer to the variable
//                  where the data read will be stored
//
// OUTPUT PARAMS:   Data - Contains the value read from the register value
//                  specified as the first input parameter
//
//////////////////////////////////////////////////////////////////////////////
/*
void ReadByteEHDMI(byte RegOffset, byte *Data)
{
    *Data = I2C_ReadByte(EHDMI_BASE_ADDR, RegOffset);
}
*/

//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION      :  WriteByteEHDMI ()
//
// PURPOSE       :  Write one byte to a given offset of the EHDMI interface.
//
// INPUT PARAMS  :  Offset of EHDMI register to write to; value to be written to
//                  that EHDMI retister
//
// OUTPUT PARAMS :  None
//
//////////////////////////////////////////////////////////////////////////////
void WriteByteEHDMI(byte RegOffset, byte Data)
{
    I2C_WriteByte(EHDMI_BASE_ADDR, RegOffset, Data);
}

#endif
#endif
