//***************************************************************************
//!file     si_cpCDC.c
//!brief    CP 9387 Starter Kit CDC demonstration code.
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

//#include <stdio.h>

//#include <si_cp9387.h>:1260
#include "dev.h"
#include "si_apiCEC.h"
#include "si_cdc_enums.h"
#include "si_cdc_heac.h"
#include "si_apiCDC.h"
#include "si_cpCDC.h"
#include "defs.h"

#if (IS_CDC == 1)

//------------------------------------------------------------------------------
// Module variables
//------------------------------------------------------------------------------

CdcInstanceRecord_t* pDeviceInstRec = 0x0;
CdcIoConfig_t cdcIo;

static int l_devPhysAddr    = 0x0000;

static uint8_t lastFbMsg    = CDC_FB_MSG_NONE;

//------------------------------------------------------------------------------
// Function:    CpCdcInit
// Description:
//------------------------------------------------------------------------------

bool_t CpCdcInit ( void )
{
    int i;

    pDeviceInstRec = SI_CdcDeviceInstanceGet(); // initialize pointer to current CDC Instance Record

    l_devPhysAddr = SI_CecGetDevicePA();

    cdcIo.numberOfInputs = SII_MAX_PORTS;
    for (i = 0; i < cdcIo.numberOfInputs; i++)
    {
        cdcIo.input[i].hecSupport = CDC_HEC_NOT_SUPPORTED;
        cdcIo.input[i].hecActive = CDC_HEC_INACTIVE;
        cdcIo.input[i].hpdState = CDC_HPD_CP_AND_EDID_ENABLE;
    }

    /* The starter kit uses port 1 for HEC. */

    cdcIo.input[HEAC_HEC_PORT].hecSupport = CDC_HEC_SUPPORTED;

    cdcIo.numberOfOutputs = (l_devPhysAddr == 0x0000) ? 0 : 1;
    cdcIo.output.hecSupport = CDC_HEC_SUPPORTED;                // If TX has HEC: CDC_HEC_SUPPORTED;
    cdcIo.output.hecActive = CDC_HEC_INACTIVE;
    cdcIo.output.hpdState = CDC_HPD_CP_AND_EDID_ENABLE;

//    SI_CdcConfig(l_devPhysAddr, &cdcIo, true, true, false);    // Device is HEC, Host capable and External network incapable
    SI_CdcConfig(l_devPhysAddr, true, true, false);    // Device is HEC, Host capable and External network incapable

	SI_CdcTaskInit();
	SI_CdcSrvStart();

//    HalTimerSet(ELAPSED_TIMER1, 1);	// sets CDC timer's granularity and kicks the timer off to free running

    return( true );
}

//------------------------------------------------------------------------------
// Function:    si_CDCProcess
// Description: Monitor CDC sub-system.  Called from the CEC handler
//------------------------------------------------------------------------------

bool_t si_CDCProcess ( SI_CpiData_t *pCdcCmd )
{
    bool_t  usedMessage;
    uint8_t fbMsg;
    int i;
    uint16_t timeTickMs=0;

    /* Call the main CDC handler. We pass it the system tick timer's reading    */
    /* so that it can update its internal timers.                               */

//    timeTickMs = HalTimerElapsed(ELAPSED_TIMER1);
    fbMsg = SI_CdcTaskProcess(timeTickMs, pCdcCmd);

    /* Process feedback messages.       */
    usedMessage = true;
    switch ( fbMsg )
    {
        default:
        case CDC_FB_MSG_NONE:
            // nothing to do
            break;
        case CDC_FB_MSG_HST_SEARCH_DONE:    // Host search done, print list of found devices.
            for (i = 0; i < pDeviceInstRec->numberOfFoundHosts; i++)
            {
                TPI_CDC_PRINT(( "%04x", pDeviceInstRec->cdcFoundHostsList[ i].physAddr ));
                if ( pDeviceInstRec->cdcFoundHostsList[ i].hostInfo.isSelected )
                {
                    TPI_CDC_PRINT(( " +" ));
                }
                TPI_CDC_PRINT(( "\n" ));
            }
            lastFbMsg = fbMsg;
            break;
        case CDC_FB_MSG_CONNECT_DONE:       // Update lists
            TPI_CDC_PRINT(( "Connection task finished\n" ));
            lastFbMsg = fbMsg;
            break;
        case CDC_FB_MSG_DISCONNECT_DONE:
        case CDC_FB_MSG_DISCONNECT_ALL_DONE:
        case CDC_FB_MSG_DISCONNECT_LOST_DONE:
            TPI_CDC_PRINT(( "Disconnect task finished\n" ));
            break;
        case CDC_FB_MSG_HPD_SIGNAL_DONE:
            TPI_CDC_PRINT(( "HPD Signal task finished\n" ));
            break;
        case CDC_FB_MSG_ERR_HPD_SIGNAL:
            TPI_CDC_PRINT(( "HDMI Source not responded to HPD command!" ));
            break;
        case CDC_FB_MSG_HPD_STATE_CHANGED: // message from Sink to Source
            break;
        case CDC_FB_MSG_ERR_NONCDC_CMD:
            usedMessage = false;
            break;
    }

    return( usedMessage );
}


#ifdef	NEVER	// Implement when we do Bug#18424 so these actions can be commanded via IForms
//------------------------------------------------------------------------------
// Function:    CpCDCInfoPrint
// Description: Show current CDC status
//------------------------------------------------------------------------------

void CpCDCInfoPrint ( void )
{
    int i;
    char *pStateStr;

    // show HEC list
    TPI_CDC_PRINT(("\n-------- HEC List --------\n"));
    for (i = 0; i < CDC_NUMBER_OF_HECS_MAX; i++)
    {
        if (pDeviceInstRec->aHec[i].status.isInUse == true)
        {
            switch (pDeviceInstRec->aHec[i].status.channelState)
            {
                default:
                case CDC_PHEC:
                    pStateStr = "PHEC";
                    break;
                case CDC_VHEC:
                    pStateStr = "VHEC";
                    break;
                case CDC_AHEC:
                    pStateStr = "AHEC";
                    break;
            }

            TPI_CDC_PRINT(("%04x >> %04x, act: %04x, typ: %s, state: %s\n",
                pDeviceInstRec->aHec[i].firstDevPhysAddr,
                pDeviceInstRec->aHec[i].lastDevPhysAddr,
                pDeviceInstRec->aHec[i].activatorPhysAddr,
                pDeviceInstRec->aHec[i].status.isAdjacent ? "adjacent" : "normal",
                pStateStr));
        }
    }

    // show alive list
    TPI_CDC_PRINT(("-------- Alive List --------\n"));
    for (i = 0; i < CDC_NUMBER_OF_DEVICES_MAX; i++)
    {
        if (pDeviceInstRec->aAliveDevice[i].physAddr != 0xFFFF)
        {
            TPI_CDC_PRINT(("%04x, left: %d s\n",
                pDeviceInstRec->aAliveDevice[i].physAddr,
                CdcTimeCounterLeftSecGet(&pDeviceInstRec->aAliveDevice[i].TimeCount)));
        }

    }

    // show found devices list
    TPI_CDC_PRINT(("-------- Found Devices List --------\n"));

    for (i = 0; i < pDeviceInstRec->numberOfFoundHosts; i++)
    {
        TPI_CDC_PRINT(( "%04x [h = %d, e = %d, sel = %d]\n",
            pDeviceInstRec->cdcFoundHostsList[i].physAddr,
            (int) pDeviceInstRec->cdcFoundHostsList[i].devStat.hostFuncState,
            (int) pDeviceInstRec->cdcFoundHostsList[i].devStat.encFuncState,
            (int) pDeviceInstRec->cdcFoundHostsList[i].hostInfo.isSelected));
    }

}


//------------------------------------------------------------------------------
// Function:    CpCDCInquireStateTest
// Description: Sends InquireState CDC message
//------------------------------------------------------------------------------

void CpCDCInquireStateTest ( void )
{
    uint8_t hecIdx;

    hecIdx = CdcHecDescriptorAdd(true);

    pDeviceInstRec->aHec[hecIdx].activatorPhysAddr = pDeviceInstRec->physAddr;
    pDeviceInstRec->aHec[hecIdx].firstDevPhysAddr  = pDeviceInstRec->physAddr;
    pDeviceInstRec->aHec[hecIdx].lastDevPhysAddr  = 0x2000; // TE ID1 (HEACT Table 7-1)

    CdcHecInquireStateSend(hecIdx);

    TPI_CDC_PRINT(( "InquireState sent to HEC %d [%04x,%04x]\n",
        (int)hecIdx,
        pDeviceInstRec->aHec[hecIdx].firstDevPhysAddr,
        pDeviceInstRec->aHec[hecIdx].lastDevPhysAddr));

    CdcHecDescriptorRemove(hecIdx);
}


//------------------------------------------------------------------------------
// Function:    CpCDCSingleDeviceConnectTest
// Description: Activates and Deactivates HECs to the devices from
//              the device found list
//------------------------------------------------------------------------------

void CpCDCSingleDeviceConnectTest ( uint8_t devIndex )
{
    if (CdcHecIndexGet(pDeviceInstRec->physAddr, pDeviceInstRec->physAddr, pDeviceInstRec->cdcFoundHostsList[devIndex].physAddr) == 0xFF)
    {
        SI_CdcDeviceConnect( false, pDeviceInstRec->physAddr, pDeviceInstRec->cdcFoundHostsList[devIndex].physAddr);
        TPI_CDC_PRINT(("TEST: Activation of a HEC %d [%04x,%04x]\n",
            (int)devIndex,
            pDeviceInstRec->physAddr,
            pDeviceInstRec->cdcFoundHostsList[devIndex].physAddr));
    }
    else
    {
        SI_CdcDeviceDisconnect( false, pDeviceInstRec->physAddr, pDeviceInstRec->physAddr, pDeviceInstRec->cdcFoundHostsList[devIndex].physAddr);
        TPI_CDC_PRINT(("TEST: Deactivation of a HEC %d [%04x,%04x]\n",
            (int) devIndex,
            pDeviceInstRec->physAddr,
            pDeviceInstRec->cdcFoundHostsList[devIndex].physAddr));
    }
}
#endif		//NEVER

#else

//-------------------------------------------------------------------------------
// Dummy functions for use when this module is disabled
//-------------------------------------------------------------------------------

bool_t CpCdcInit ( void ) { return( true ); }
bool_t si_CDCProcess ( SI_CpiData_t *pCdcCmd ) { pCdcCmd = 0; return( false ); }

#endif
