/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

#ifdef DEV_SUPPORT_EHDMI

#define EHDMI_BASE_ADDR			HDMI_I2C_CA

#define EHDMI_BANDGAP			0x04
#define EHDMI_TERMINATION		0x05
#define EHDMI_SPDIF_1			0x06
#define EHDMI_ARC				0x07
#define EHDMI_ETHERNET_1		0x08
#define EHDMI_ETHERNET_BIAS		0x09
#define EHDMI_ECHO				0x0A
#define EHDMI_ETHERNET_PGA		0x0B
#define EHDMI_ETHERNET_2		0x0C
#define EHDMI_ETHERNET_3		0x0D
#define EHDMI_SPDIF_2			0x0E
#define EHDMI_SPDIF_3			0x0F
#define EHDMI_CALIBRATION		0x10
#define EHDMI_TEST1				0x11
#define EHDMI_TEST2				0x12

// void ReadByteEHDMI(byte RegOffset, byte *Data);
void WriteByteEHDMI(byte RegOffset, byte Data);
void EHDMI_ARC_Common_Enable(void);
void EHDMI_ARC_Single_Enable(void);
void EHDMI_HEC_Enable(void);
void EHDMI_ARC_Common_With_HEC_Enable(void);
void EHDMI_Disable(void);
void EHDMI_Init(void);

//eHDMI Flags
extern byte EHDMI_MODE;

#endif
