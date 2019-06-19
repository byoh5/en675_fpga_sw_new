/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

extern VideoModeDescription_t VideoModeDescription;
extern int VModeTblIndex;

// Patches
//========
extern byte EmbeddedSynPATCH;

//UART
//====
extern byte TXBusy;

extern byte NumOfArgs;
extern byte MessageType;
extern bool_ F_SBUF_DataReady;
extern bool_ F_CollectingData;

extern bool_ USRX_OutputChange;

// Simulation Configuration
extern bool_ g_EDID_Simulation;
extern bool_ g_Video_Mode_Simulation;

extern byte g_audio_Checksum;	// Audio checksum

// Communication
extern byte g_CommData[];
     
// HDCP Override
extern bool_ g_HDCP_Override;  

