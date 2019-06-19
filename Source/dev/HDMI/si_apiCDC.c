//***************************************************************************
//!file     si_apiCDC.c
//!brief    Top level CDC API
//
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

//#include "../../cpu_main.h"

#include "dev.h"
#include "si_apiCDC.h"

#ifndef CDC_PC_SIMULATION
//#include <string.h>
//#include <stdio.h>
//#include "gpio.h"
#else
//#include "stdio.h"
//#include "memory.h"
//#include "debug_print.h"
#endif
#include "defs.h"


#if (IS_CDC == 1)

//------------------------------------------------------------------------------
// Global Data
//------------------------------------------------------------------------------
CdcInstanceRecord_t   gInstRec[NUMBER_OF_DEVICES];  // global CDC instance record
CdcInstanceRecord_t   *gpInstRec = &gInstRec[0];    // global pointer to an Instance Record

#ifdef CDC_PC_SIMULATION
uint8_t  g_halMsgLevel = MSG_DBG;
#endif

//------------------------------------------------------------------------------
// Local Service Function Prototypes
//------------------------------------------------------------------------------

static void CdcAliveTimerSet(uint8_t minTimeSec, uint8_t maxTimeSec);
static void CdcLostDeviceDisconnect(uint16_t physAddr);


//------------------------------------------------------------------------------
// Top Level API Functions
//------------------------------------------------------------------------------

#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcDeviceSelect
// Description: Brings device instance record into current context. This function
//              is useful when multiple HDMI outputs are present
//
//------------------------------------------------------------------------------

void SI_CdcDeviceSelect(uint16_t deviceIndex)
{
    if( deviceIndex < NUMBER_OF_DEVICES)
    {
        gpInstRec = &gInstRec[deviceIndex];
    }
}
#endif

//------------------------------------------------------------------------------
// Function:    SI_CdcDeviceInstanceGet
// Description: Returns pointer to current device instance record
//
//------------------------------------------------------------------------------

CdcInstanceRecord_t * SI_CdcDeviceInstanceGet(void)
{
    return gpInstRec;
}


//------------------------------------------------------------------------------
// Function:    SI_CdcConfig
// Description: Initialize the Instance Record and fills its fields related to
//              device capabilities and non-volatile configuration.
//              Alternatively, the Instance Record can be addressed and set
//              directly.
// Parameters:  physAddr - physical address of this device,
//              pIoPorts - pointer to I/O configuration structure,
//              hecCapability  - true if the device supports HEC functionality
//              hostCapability - true if the device can be a special service provider (host)
//              extNetCapability - true if the device has external network connection
//------------------------------------------------------------------------------

//void SI_CdcConfig(uint16_t physAddr, CdcIoConfig_t *pIoPorts,
void SI_CdcConfig(uint16_t physAddr,
               bool_t hecCapability, bool_t hostCapability, bool_t extNetCapability)
{
    CdcHecInstanceRecordReset(); // Initialize all Instance Record fields and free all allocated memory

    gpInstRec->physAddr = physAddr;                             // set PA of this device
//    memcpy(&gpInstRec->ports, pIoPorts, sizeof(CdcIoConfig_t)); // set device I/O configuration

    gpInstRec->deviceStatus.hecFuncState = hecCapability ? CDC_HEC_STATE_INACTIVE : CDC_HEC_FUNC_NOT_SUPPORTED;
    gpInstRec->deviceStatus.hostFuncState= hostCapability ? CDC_HOST_STATE_INACTIVE : CDC_HOST_NOT_SUPPORTED;
    gpInstRec->deviceStatus.encFuncState = extNetCapability ? CDC_ENC_STATE_INACTIVE : CDC_ENC_NOT_SUPPORTED;

    // Broadcact "Capability Notification" message
    CdcHecReportStateSend(0xFFFF, 0);
    CdcDiscoveryDataArrange(); // update discovered and reachable device list
}


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcDeviceHecSupEnable
// Description: Enables or Disables HEC Support function of this Device
//
//------------------------------------------------------------------------------

void SI_CdcDeviceHecSupEnable(const bool_t isEnabled)
{
    uint8_t oldState = gpInstRec->deviceStatus.hecFuncState;

    if (isEnabled && (oldState == CDC_HEC_FUNC_NOT_SUPPORTED))
    {
        gpInstRec->deviceStatus.hecFuncState = CDC_HEC_STATE_INACTIVE;
        // don't change HEC state if already enabled (either Inactive or Active)
    }
    else if (!isEnabled)
    {
        SI_CdcDeviceDisconnect(true,0,0,0); // disconnect all active HECs
        gpInstRec->deviceStatus.hecFuncState = CDC_HEC_FUNC_NOT_SUPPORTED;
    }

    if (gpInstRec->deviceStatus.hecFuncState != oldState)
    {
        // Broadcact "Capability Notification" message
        // only if the state has been changed
        CdcHecReportStateSend(0xFFFF, 0);
        CdcDiscoveryDataArrange(); // update discovered and reachable device list
    }
}
#endif

#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcDeviceHostStateSet
// Description: Sets new Host Capability state of this Device
//
//------------------------------------------------------------------------------

void SI_CdcDeviceHostStateSet(const CdcHostFuncState_t state)
{

    if (gpInstRec->deviceStatus.hostFuncState != state)
    {
        gpInstRec->deviceStatus.hostFuncState = state;
        // Broadcact "Capability Notification" message
        // only if the state has been changed
        CdcHecReportStateSend(0xFFFF, 0);
        CdcDiscoveryDataArrange(); // update discovered and reachable device list
    }
}
#endif


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcDeviceEncStateSet
// Description: Sets new External Network Capability state of this Device
//
//------------------------------------------------------------------------------

void SI_CdcDeviceEncStateSet(const CdcEncFuncState_t state)
{

    if (gpInstRec->deviceStatus.encFuncState != state)
    {
        gpInstRec->deviceStatus.encFuncState = state;
        // Broadcact "Capability Notification" message
        // only if the state has been changed
        CdcHecReportStateSend(0xFFFF, 0);
        CdcDiscoveryDataArrange(); // update discovered and reachable device list
    }
}
#endif


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcPortHecSupEnable
// Description: Enables or Disables HEC Support function on I/O ports
//
// Parameters:  isOutput       - true, if output port is addressed
//              inputPortIndex - input port index (zero-based)
//              isEnabled      - boolean flag
//------------------------------------------------------------------------------

void SI_CdcPortHecSupEnable(const bool_t isOutput, const uint8_t inputPortIndex, const bool_t isEnabled)
{
    bool_t    isChanged = false;
    uint16_t  physAddr;

    if ((isOutput == true) && (gpInstRec->ports.numberOfOutputs != 0))
    {
        if (gpInstRec->ports.output.hecSupport != isEnabled)
        {
            // before disabling output, deactivate affected AHECs
            if (!isEnabled)
            {
                physAddr = CdcAdjacentPhysAddrGet(gpInstRec->physAddr, true, 0);
                if (physAddr != 0xFFFF)
                {
                    CdcLostDeviceDisconnect(physAddr);
                }
            }

            gpInstRec->ports.output.hecSupport = isEnabled;
            isChanged = true;
        }
    }
    else if ((isOutput == false) && (gpInstRec->ports.numberOfInputs > inputPortIndex)) // inputs
    {
        if (gpInstRec->ports.input[inputPortIndex].hecSupport != isEnabled)
        {
            // before disabling input, deactivate affected AHECs
            if (!isEnabled)
            {
                physAddr = CdcAdjacentPhysAddrGet(gpInstRec->physAddr, false, inputPortIndex);
                if (physAddr != 0xFFFF)
                {
                    CdcLostDeviceDisconnect(physAddr);
                }
            }

            gpInstRec->ports.input[inputPortIndex].hecSupport = isEnabled;
            isChanged = true;
        }
    }

    if (isChanged)
    {
        // Broadcact "Capability Notification" message
        // only if the state has been changed
        CdcHecReportStateSend(0xFFFF, 0);
        CdcDiscoveryDataArrange(); // update discovered and reachable device list
    }
}
#endif

//------------------------------------------------------------------------------
// Function:    SI_CdcTaskInit
// Description: Resets all tasks
//
//------------------------------------------------------------------------------

void SI_CdcTaskInit(void)
{
    gpInstRec->cdcTaskFlags = 0;
    gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED; // all tasks are unmasked by default

    // Reset time counters
    CdcTimeCounterReset(&gpInstRec->cdcTaskTimeCount);
    CdcTimeCounterReset(&gpInstRec->aliveNtfDelayCount);
    CdcTimeCounterReset(&gpInstRec->cdcDiscoveryTimeCount);


    // Reset task states
    gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE;

    mRand(gpInstRec->physAddr); // seed the random numbers generator

    gpInstRec->sysTimerCountCurMs = 0;
    gpInstRec->sysTimerCountPrvMs = 0;
}


//------------------------------------------------------------------------------
// Function:    CdcAliveTimerSet
// Description: Sets random initial delay time
//
//------------------------------------------------------------------------------

static void CdcAliveTimerSet(uint8_t minTimeSec, uint8_t maxTimeSec)
{
    if (minTimeSec <= maxTimeSec)
    {
        // Initial delay of AliveNotification messages (Tmin...Tmax randomly chosen)
        CdcTimeCounterSet(CDC_TIME_SEC2TCK(minTimeSec + mRand(0) % (maxTimeSec - minTimeSec + 1)), &gpInstRec->aliveNtfDelayCount);
    }
}


//------------------------------------------------------------------------------
// Function:    CdcHecDisableCheck
// Description: If no HEC devices, disable HEC hardware.
//------------------------------------------------------------------------------

static void CdcHecDisableCheck(void)
{
    if (gpInstRec->numberOfHecs == 0) // not participating in any HEC
    {
         // Deactivate HEC function
         if (CdcEhdmiHwEnable(false))
         {
            if (gpInstRec->deviceStatus.hecFuncState != CDC_HEC_NOT_SUPPORTED)
            {
                gpInstRec->deviceStatus.hecFuncState = CDC_HEC_STATE_INACTIVE;
            }
         }
    }
}


//------------------------------------------------------------------------------
// Function:    CdcHecAddNewDeviceToHostList
// Description: Auxiliary function for Base Service. Adds new device into Found
//              Host List in response to CDCOP_HEC_REPORT_STATE message
//------------------------------------------------------------------------------

static void CdcHecAddNewDeviceToHostList(SI_CpiData_t *pCdcMessage, uint16_t initiatorPhysAddr)
{

    if ((gpInstRec->numberOfAllHosts < CDC_NUMBER_OF_FOUND_HOSTS_MAX) &&        // There is space in the device list
        (((pCdcMessage->args[5] >> 6) & 0x03) != CDC_HEC_FUNC_NOT_SUPPORTED) && // HEC function is available
        ((pCdcMessage->args[5] & 0x03) == CDC_ERRCODE_NO_ERROR)) // the ReportState has no indication of a error
    {
        // add new device to the list
        gpInstRec->cdcFoundHostsList[gpInstRec->numberOfAllHosts].physAddr = initiatorPhysAddr;
        gpInstRec->cdcFoundHostsList[gpInstRec->numberOfAllHosts].hostInfo.portHecCapBits = 0x7FFF & ((pCdcMessage->args[6] << 8) | pCdcMessage->args[7]);
        gpInstRec->cdcFoundHostsList[gpInstRec->numberOfAllHosts].devStat.hecFuncState  = ((pCdcMessage->args[5] >> 6) & 0x03);
        gpInstRec->cdcFoundHostsList[gpInstRec->numberOfAllHosts].devStat.hostFuncState = ((pCdcMessage->args[5] >> 4) & 0x03);
        gpInstRec->cdcFoundHostsList[gpInstRec->numberOfAllHosts].devStat.encFuncState  = ((pCdcMessage->args[5] >> 2) & 0x03);
        gpInstRec->cdcFoundHostsList[gpInstRec->numberOfAllHosts].devStat.cdcErrorState =  (pCdcMessage->args[5] & 0x03);

        TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Device PA = %04x added to found Hosts list\n", initiatorPhysAddr));

        gpInstRec->numberOfAllHosts++;
    }
}


//------------------------------------------------------------------------------
// Function:    CdcTaskDiscover
// Description: Launch discovery process.  When done, create a list
//              of found devices
//
// Returns:     Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

static uint8_t CdcTaskDiscover(void)
{
    uint8_t cdcFeedbackMsg = CDC_FB_MSG_NONE;


    switch(gpInstRec->cdcTaskState)
    {
        case CDC_TASK_EMPTY_STATE_NONE: // all tasks shall start from empty state
                gpInstRec->cdcTaskMask = CDC_TASK_HOSTS_SEARCH; // mask out other tasks until this one is done
                gpInstRec->cdcTaskState = CDC_TASK_FIND_STATE_DISCOVERY_START;

                // set Discovery repetition timer
                CdcTimeCounterSet(CDC_TIME_SEC2TCK(CDC_MIN_DISCOVERY_REP_TIME_SEC), &gpInstRec->cdcDiscoveryTimeCount);
                // don't break here to save time and go ahead to the fist step

        case CDC_TASK_FIND_STATE_DISCOVERY_START: // First stage: Sending out a discovery message
                CdcHecDiscoverSend();
                gpInstRec->numberOfFoundHosts = 0; // reset counter of found devices (ENC or Host capable)
                gpInstRec->numberOfAllHosts = 0;   // reset counter of all found devices
                gpInstRec->cdcTaskState = CDC_TASK_FIND_STATE_GATHER_RESPONSES; // next stage
                TPI_CDC_PRINT(("[CDC_TASK_FIND]: Started gathering responses\n"));
                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);
                CdcTimeCounterSet(CDC_TIME_SEC2TCK(CDC_MAX_MONITORING_TIME_SEC), &gpInstRec->cdcTotalMonTimeCount);
                break;

        case CDC_TASK_FIND_STATE_GATHER_RESPONSES: // Stage: collect responses from all devices
                // wait until all devices respond
                // criteria: timeout since the last response

                // Set recall timer to maximum in order to allow the task counters to set it to a new value
                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);

                CdcTimeCounterUpdate(&gpInstRec->cdcTaskTimeCount);
                CdcTimeCounterUpdate(&gpInstRec->cdcTotalMonTimeCount);
                if (CdcIsTimeCounterExpired(&gpInstRec->cdcTaskTimeCount) || CdcIsTimeCounterExpired(&gpInstRec->cdcTotalMonTimeCount))
                {   // if timer expired
                    gpInstRec->cdcTaskState = CDC_TASK_FIND_STATE_REPORT_RESULTS;  // next stage
                    TPI_CDC_PRINT(("[CDC_TASK_FIND]: Stopped gathering responses\n"));
                }
                break;

        case CDC_TASK_FIND_STATE_REPORT_RESULTS: // Last stage
                // Identify all found HOST or ENC capable devices that aren't reachable from this device
                // then cleanup and rearrange the list to leave only capable & reachable devices
                CdcDiscoveryDataArrange();

                // event message to the caller
                cdcFeedbackMsg = CDC_FB_MSG_HST_SEARCH_DONE;
                gpInstRec->cdcTaskFlags &= ~CDC_TASK_HOSTS_SEARCH; // stop the task
                gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE;  // reset stage
                gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED;  // reset task mask as this task is done
                break;

 		default:
			 	_printf("--------FAIL (CdcTaskDiscover)----------\n",0,0);
 				break;
    }

    return  cdcFeedbackMsg;
}


//------------------------------------------------------------------------------
// Function:    CdcTaskConnect
// Description: Activate one or more HECs.
//              Sends InquireState message to all PHECs and
//              checks if the PHECs have responded, then update PHECs status
//              to VHEC. Initiate activation of all VHECs and check if the
//              VHECs have responded, then update VHECs status to AHEC
//
// Returns:     Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

static uint8_t CdcTaskConnect(void)
{
    uint8_t     cdcFeedbackMsg = CDC_FB_MSG_NONE;
    uint16_t    firstPhysAddr;
    uint16_t    lastPhysAddr;
    bool_t      tmpCond;
    uint8_t     activatorExcludeFactor;
    uint8_t     i;

    switch(gpInstRec->cdcTaskState)
    {
        case CDC_TASK_EMPTY_STATE_NONE: // all tasks shall start from empty state
                gpInstRec->cdcTaskMask = CDC_TASK_CONNECT; // mask out other tasks until this one is done
                gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_INIT;
                // don't break here to save time and go ahead to the fist step

        case CDC_TASK_CONNECT_STATE_INIT:
                gpInstRec->currentHecIdx = 0;   // initialize HEC index
                gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_INQUIRE_START; // next stage
                gpInstRec->connectSettings.hecActDelayed = 0;   // reset number of delayed multi-HEC SetState messages
                gpInstRec->connectSettings.isEndOfList = false; // reset End of VHEC List flag
                //break;

        case CDC_TASK_CONNECT_STATE_INQUIRE_START: // First stage: Sending out InquireState messages for PHECs
                // find first PHEC in the HEC records and start activation of it
                while (gpInstRec->currentHecIdx < CDC_NUMBER_OF_HECS_MAX)
                {
                    if ((gpInstRec->aHec[gpInstRec->currentHecIdx].status.isInUse == true) &&
                        (gpInstRec->aHec[gpInstRec->currentHecIdx].status.channelState == CDC_PHEC))
                    {
                        // Found a PHEC; begin activation process. Increment the current HEC index when done
                        CdcHecInquireStateSend(gpInstRec->currentHecIdx);
                        CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);
                        CdcTimeCounterSet(CDC_TIME_SEC2TCK(CDC_MAX_MONITORING_TIME_SEC), &gpInstRec->cdcTotalMonTimeCount);
                        gpInstRec->verifiedDevNum[0] = 0; // reset counter of verified devices
                        gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_GATHER_INQ_REP; // next stage
                        break;
                    }
                    gpInstRec->currentHecIdx++;
                }

                if (gpInstRec->cdcTaskState == CDC_TASK_CONNECT_STATE_GATHER_INQ_REP) // if found new PHEC
                {
//                    TPI_CDC_PRINT(("[CDC_TASK_CONNECT]: Started gathering responses for PHEC %d\n", (int) gpInstRec->currentHecIdx));
					_printf("[CDC_TASK_CONNECT]: Started gathering responses for PHEC : ", (int) gpInstRec->currentHecIdx, 8);
                }
                else // no new PHEC was found
                {

                    if (gpInstRec->connectSettings.isMultHecMode && (gpInstRec->connectSettings.hecActDelayed > 0))
                    {
 						_printf(("[CDC_TASK_CONNECT]: End of PHEC list found\n"),0,0);
                        gpInstRec->connectSettings.isEndOfList = true;
                        gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_ACTIVATE_START; // send last activation command
                    }
                    else // single HEC activation mode
                    {
                        _printf(("[CDC_TASK_CONNECT]: No new PHEC found. Task finished\n"),0,0);
                        gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_REPORT_RESULTS; // stop the task
                    }
                }
                break;

        case CDC_TASK_CONNECT_STATE_GATHER_INQ_REP:
                // wait until all devices respond
                // criteria: timeout since the last response or arrival of the last verified report

                // Set recall timer to maximum in order to allow the task counters to set it to a new value
                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);

                CdcTimeCounterUpdate(&gpInstRec->cdcTaskTimeCount);
                CdcTimeCounterUpdate(&gpInstRec->cdcTotalMonTimeCount);

                firstPhysAddr = gpInstRec->aHec[gpInstRec->currentHecIdx].firstDevPhysAddr;
                lastPhysAddr  = gpInstRec->aHec[gpInstRec->currentHecIdx].lastDevPhysAddr;

                // Exclude activator (this device) from the HEC lengh formula, if it's a part of the HEC
                activatorExcludeFactor = CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, gpInstRec->physAddr) ? 1 : 0;

                tmpCond = (gpInstRec->verifiedDevNum[0] == (CdcHecLengthGet(firstPhysAddr, lastPhysAddr) - activatorExcludeFactor)); // "-1" means activator exclusion
                if (CdcIsTimeCounterExpired(&gpInstRec->cdcTaskTimeCount) || CdcIsTimeCounterExpired(&gpInstRec->cdcTotalMonTimeCount) || tmpCond)
                {   // if timer expired or all device responses have been collected
                    _printf(("[CDC_TASK_CONNECT]: Stopped gathering InquireState responses\n"),0,0);

                    if (tmpCond)
                    {
                        gpInstRec->aHec[gpInstRec->currentHecIdx].status.channelState = CDC_VHEC;
                        //DEBUG_PRINT(MSG_DBG, ("[CDC_TASK_CONNECT]: HEC %d status has changed to VHEC.\n", (int) gpInstRec->currentHecIdx));

                        gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_ACTIVATE_START; // next stage
                    }
                    else
                    {
//                        TPI_CDC_PRINT(("[CDC_TASK_CONNECT]: PHEC %d wasn't verified. Removed.\n", (int) gpInstRec->currentHecIdx));
						_printf("[CDC_TASK_CONNECT]: PHEC %d wasn't verified. Removed. : ", (int) gpInstRec->currentHecIdx, 8);
                        // Remove the failed PHEC
                        CdcHecDescriptorRemove(gpInstRec->currentHecIdx);
                        gpInstRec->currentHecIdx++; // step to next HEC
                        gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_INQUIRE_START; // move to another PHEC search
                    }

                    // minimize the time of the next call
                    CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MIN_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);
                }
                break;

        case CDC_TASK_CONNECT_STATE_ACTIVATE_START:
                if (gpInstRec->connectSettings.isMultHecMode)
                {
                    // Issue multi-HEC activation command, if there are 4 HECs to activate or the End of HEC list is reached
                    if ((gpInstRec->connectSettings.hecActDelayed >= 3) || (gpInstRec->connectSettings.isEndOfList))
                    {
                        if (!gpInstRec->connectSettings.isEndOfList) // new HEC has to be added
                        {
                            gpInstRec->delayedHecIdx[gpInstRec->connectSettings.hecActDelayed++] = gpInstRec->currentHecIdx; // put current HEC into the list
                        }

                        if (gpInstRec->connectSettings.hecActDelayed > 0)
                        {
                            CdcHecSetStateSend(CDC_HEC_ACTIVE, gpInstRec->connectSettings.hecActDelayed, gpInstRec->delayedHecIdx);
                            _printf(("[CDC_TASK_CONNECT]: Started gathering responses for VHECs in multi-HEC mode\n"),0,0);
                        }
                    }
                    else // delay the activation, memorize delayed HEC index
                    {
                        gpInstRec->delayedHecIdx[gpInstRec->connectSettings.hecActDelayed] = gpInstRec->currentHecIdx++;
                        gpInstRec->connectSettings.hecActDelayed++;
                        gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_INQUIRE_START; // move to another PHEC search
//                        TPI_CDC_PRINT(("[CDC_TASK_CONNECT]: Activation delayed for %d VHECs\n", (int) gpInstRec->connectSettings.hecActDelayed));
						_printf("[CDC_TASK_CONNECT]: Activation delayed for %d VHECs : ", (int) gpInstRec->connectSettings.hecActDelayed, 8);
                        break;
                    }
                }
                else
                {
                    // Issue single-HEC activation command
                    CdcHecSetStateSend(CDC_HEC_ACTIVE, 1, &gpInstRec->currentHecIdx);
                    TPI_CDC_PRINT(("[CDC_TASK_CONNECT]: Started gathering responses for VHEC %d\n", (int) gpInstRec->currentHecIdx));
					_printf("[CDC_TASK_CONNECT]: Started gathering responses for VHEC : ", (int) gpInstRec->currentHecIdx, 8);
                }

                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);
                CdcTimeCounterSet(CDC_TIME_SEC2TCK(CDC_MAX_MONITORING_TIME_SEC), &gpInstRec->cdcTotalMonTimeCount);

                for (i = 0; i < 4; i++)
                {
                    gpInstRec->verifiedDevNum[i] = 0; // reset counters of verified devices
                }
                gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_GATHER_ACT_REP; // next stage
                break;

        case CDC_TASK_CONNECT_STATE_GATHER_ACT_REP:
                // wait until all devices respond
                // criteria: timeout since the last response or arrival of the last verified report

                // Set recall timer to maximum in order to allow the task counters to set it to a new value
                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);

                CdcTimeCounterUpdate(&gpInstRec->cdcTaskTimeCount);
                CdcTimeCounterUpdate(&gpInstRec->cdcTotalMonTimeCount);

                if (gpInstRec->connectSettings.isMultHecMode)
                {
                    tmpCond = false;
                    for (i = 0; i < gpInstRec->connectSettings.hecActDelayed; i++)
                    {
                        // Exclude activator (this device) from the HEC lengh formula, if it's a part of the HEC
                        activatorExcludeFactor = CdcIsDeviceBelongsToHec(gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].firstDevPhysAddr,
                                                                         gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].lastDevPhysAddr,
                                                                         gpInstRec->physAddr) ? 1 : 0;

                        tmpCond &= (gpInstRec->verifiedDevNum[i] ==
                                    (CdcHecLengthGet(gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].firstDevPhysAddr,
                                                     gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].lastDevPhysAddr) - activatorExcludeFactor));// "-1" means activator exclusion
                    }

                    if (CdcIsTimeCounterExpired(&gpInstRec->cdcTaskTimeCount) || CdcIsTimeCounterExpired(&gpInstRec->cdcTotalMonTimeCount) || tmpCond)
                    {
                        _printf(("[CDC_TASK_CONNECT]: Stopped gathering Activation responses in Multi-HEC mode\n"),0,0);

                        // Change curent HEC status to AHEC if all devices in the HEC are verified to be active
                        // do it for all HECs in the list
                        for (i = 0; i < gpInstRec->connectSettings.hecActDelayed; i++)
                        {
                            // Exclude activator (this device) from the HEC lengh formula, if it's a part of the HEC
                            activatorExcludeFactor = CdcIsDeviceBelongsToHec(gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].firstDevPhysAddr,
                                                                             gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].lastDevPhysAddr,
                                                                             gpInstRec->physAddr) ? 1 : 0;

                            tmpCond = (gpInstRec->verifiedDevNum[i] ==
                                       (CdcHecLengthGet(gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].firstDevPhysAddr,
                                                        gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].lastDevPhysAddr) - activatorExcludeFactor));// "-1" means activator exclusion
                            if (tmpCond) // all devices in this HEC have responded positively
                            {
                                bool_t isHwEnabledOk = true;

                                // Activate eHDMI function
                                if (gpInstRec->deviceStatus.hecFuncState < CDC_HEC_STATE_ACTIVE)
                                {
                                    isHwEnabledOk = CdcEhdmiHwEnable(true);
                                    if (isHwEnabledOk)
                                    {
                                        gpInstRec->deviceStatus.hecFuncState = CDC_HEC_STATE_ACTIVE;
                                        CdcAliveTimerSet(10, 50); // start first AliveNotification message in 10-50 seconds
                                    }
                                }

                                if (isHwEnabledOk)
                                {
                                    CdcAliveDeviceListAdd(gpInstRec->delayedHecIdx[i]); // add HEC devices (but Activator) to the Alive List
                                    gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].status.channelState = CDC_AHEC;
                                    TPI_CDC_PRINT(("[CDC_TASK_CONNECT]: HEC %d status has changed to AHEC.\n", (int) gpInstRec->delayedHecIdx[i]));
                                    gpInstRec->delayedHecIdx[i] = 0xFF; // mark as empty
                                }

                            }
                            else
                            {
                                TPI_CDC_PRINT(("[CDC_TASK_CONNECT]: VHEC %d wasn't activated. Will be deactivated.\n", (int) gpInstRec->delayedHecIdx[i]));
                                // Leave values gpInstRec->delayedHecIdx[i] of failed HECs intact
                                // to be used on clean-up stage
                            }
                        }

                        // clean up failed HECs, if any
                        gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_REMOVE_FAILED;

                        // minimize the time of the next call
                        CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MIN_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);
                     }

                }
                else // single HEC connection mode
                {
                    firstPhysAddr = gpInstRec->aHec[gpInstRec->currentHecIdx].firstDevPhysAddr;
                    lastPhysAddr  = gpInstRec->aHec[gpInstRec->currentHecIdx].lastDevPhysAddr;

                    // Exclude activator (this device) from the HEC lengh formula, if it's a part of the HEC
                    activatorExcludeFactor = CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, gpInstRec->physAddr) ? 1 : 0;

                    tmpCond = (gpInstRec->verifiedDevNum[0] == (CdcHecLengthGet(firstPhysAddr, lastPhysAddr) - activatorExcludeFactor)); // "-1" means activator exclusion
                    if (CdcIsTimeCounterExpired(&gpInstRec->cdcTaskTimeCount) || tmpCond)
                    {
                        _printf(("[CDC_TASK_CONNECT]: Stopped gathering Activation responses\n"),0,0);
                        // Change curent HEC status to AHEC if all devices in the HEC are verified to be active
                         if (tmpCond)
                         {
                            bool_t isHwEnabledOk = true;

                            // Activate eHDMI function
                            if (gpInstRec->deviceStatus.hecFuncState < CDC_HEC_STATE_ACTIVE)
                            {
                                isHwEnabledOk = CdcEhdmiHwEnable(true);
                                if (isHwEnabledOk)
                                {
                                    gpInstRec->deviceStatus.hecFuncState = CDC_HEC_STATE_ACTIVE;
                                    CdcAliveTimerSet(10, 50); // start first AliveNotification message in 10-50 seconds
                                }
                            }

                            if (isHwEnabledOk)
                            {
                                CdcAliveDeviceListAdd(gpInstRec->currentHecIdx); // add HEC devices (but Activator) to the Alive List
                                gpInstRec->aHec[gpInstRec->currentHecIdx].status.channelState = CDC_AHEC;
                                TPI_CDC_PRINT(("[CDC_TASK_CONNECT]: HEC %d status has changed to AHEC.\n", (int) gpInstRec->currentHecIdx));
                            }

                         }
                         else
                         {
                             TPI_CDC_PRINT(("[CDC_TASK_CONNECT]: VHEC %d wasn't activated. Will be deactivated.\n", (int) gpInstRec->currentHecIdx));

                             CdcHecSetStateSend(CDC_HEC_INACTIVE, 1, &gpInstRec->currentHecIdx); // deactivation of failed VHEC
                             CdcHecDescriptorRemove(gpInstRec->currentHecIdx); // removal of failed VHEC
                             gpInstRec->currentHecIdx++; // step to next HEC
                         }
                         gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_INQUIRE_START; // move to another PHEC search
                    }
                }
                break;

        case CDC_TASK_CONNECT_STATE_REMOVE_FAILED:
                // find all HECs from the list that failed to activate
                tmpCond = false;
                for (i = 0; i < gpInstRec->connectSettings.hecActDelayed; i++)
                {
                    if (gpInstRec->delayedHecIdx[i] != 0xFF)
                    {
                        CdcHecSetStateSend(CDC_HEC_INACTIVE, 1, &gpInstRec->delayedHecIdx[i]); // deactivation of failed VHEC
                        CdcHecDescriptorRemove(gpInstRec->delayedHecIdx[i]); // removal of failed VHEC
                        gpInstRec->delayedHecIdx[i] = 0xFF;
                        tmpCond = true; // indication of a found failed HEC
                        break;
                    }
                }

                if (!tmpCond) // if all failed HECs have been sanitized
                {
                    gpInstRec->connectSettings.hecActDelayed = 0; // reset number of delayed HECs (all have been processed)
                    if (gpInstRec->connectSettings.isEndOfList)
                    {
                        _printf(("[CDC_TASK_CONNECT]: Task finished (Multi-HEC mode)\n"),0,0);
                        gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_REPORT_RESULTS; // end the task
                    }
                    else
                    {
                        gpInstRec->currentHecIdx++; // step to next HEC
                        gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_INQUIRE_START; // move to another PHEC search
                    }
                }
                else
                {
                    gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_REMOVE_FAILED; // come back to continue
                }
                break;

        case CDC_TASK_CONNECT_STATE_REPORT_RESULTS:
                CdcIoPortsStatusUpdate(); // activate ports involved in the new AHECs
                gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE; // reset stage
                gpInstRec->cdcTaskFlags &= ~CDC_TASK_CONNECT; // stop the task
                gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED; // reset task mask as this task is done
                cdcFeedbackMsg = CDC_FB_MSG_CONNECT_DONE;
                break;

 		default:
			 	_printf("-------FAIL-------\n",0,0);
				break;
    }

    return cdcFeedbackMsg;
}


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcTaskConnectAdjacent
// Description: Establishes connection to adjacent device using
//              simplified activation method: control to adjacent device
//
// Returns:     Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

static uint8_t CdcTaskConnectAdjacent(void)
{
    uint8_t     cdcFeedbackMsg = CDC_FB_MSG_NONE;
    bool_t      tmpCond;

    switch(gpInstRec->cdcTaskState)
    {
        case CDC_TASK_EMPTY_STATE_NONE: // all tasks shall start from empty state
                gpInstRec->cdcTaskMask = CDC_TASK_CONNECT_ADJACENT; // mask out other tasks until this one is done
                gpInstRec->cdcTaskState = CDC_TASK_CONNECT_ADJ_STATE_INIT;
                // don't break here to save time and go ahead to the fist step

        case CDC_TASK_CONNECT_ADJ_STATE_INIT:
                gpInstRec->cdcTaskState = CDC_TASK_CONNECT_ADJ_STATE_ACTIVATE_START; // next stage
                gpInstRec->verifiedDevNum[0] = 0;
                //break;

        case CDC_TASK_CONNECT_ADJ_STATE_ACTIVATE_START:
                // Issue activation command
                CdcHecSetStateAdjacentSend(CDC_HEC_ACTIVE, gpInstRec->currentHecIdx);
                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);
                CdcTimeCounterSet(CDC_TIME_SEC2TCK(CDC_MAX_MONITORING_TIME_SEC), &gpInstRec->cdcTotalMonTimeCount);
                gpInstRec->cdcTaskState = CDC_TASK_CONNECT_ADJ_STATE_WAIT_ACT_REP; // next stage
                TPI_CDC_PRINT(("[CDC_TASK_CONNECT_ADJACENT]: Waiting for response for HEC %d\n", (int) gpInstRec->currentHecIdx));
                break;

        case CDC_TASK_CONNECT_ADJ_STATE_WAIT_ACT_REP:
                // wait until adjacent device respond

                // Set recall timer to maximum in order to allow the task counters to set it to a new value
                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);

                CdcTimeCounterUpdate(&gpInstRec->cdcTaskTimeCount);
                CdcTimeCounterUpdate(&gpInstRec->cdcTotalMonTimeCount);

                tmpCond = (gpInstRec->verifiedDevNum[0] == 1);

                if (CdcIsTimeCounterExpired(&gpInstRec->cdcTaskTimeCount) || CdcIsTimeCounterExpired(&gpInstRec->cdcTotalMonTimeCount) || tmpCond)
                {
                    TPI_CDC_PRINT(("[CDC_TASK_CONNECT_ADJACENT]: Stopped waiting for Activation response\n"));
                     // Change curent HEC status to AHEC if adjacent devices is active
                     if (tmpCond)
                     {
                         // Activate eHDMI function
                         if (CdcEhdmiHwEnable(true))
                         {
                             gpInstRec->aHec[gpInstRec->currentHecIdx].status.channelState = CDC_AHEC;
                             gpInstRec->deviceStatus.hecFuncState = CDC_HEC_STATE_ACTIVE;
                             TPI_CDC_PRINT(("[CDC_TASK_CONNECT_ADJACENT]: HEC %d status has changed to AHEC.\n", (int) gpInstRec->currentHecIdx));
                         }
                     }
                     else
                     {
                         TPI_CDC_PRINT(("[CDC_TASK_CONNECT_ADJACENT]: HEC %d wasn't activated. Removed.\n", (int) gpInstRec->currentHecIdx));
                         CdcHecDescriptorRemove(gpInstRec->currentHecIdx); // removal of failed VHEC
                     }
                     gpInstRec->cdcTaskState = CDC_TASK_CONNECT_ADJ_STATE_REPORT_RESULTS; // move to another PHEC search

                     // minimize the time of the next call
                     CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MIN_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);
                }
                break;

        case CDC_TASK_CONNECT_ADJ_STATE_REPORT_RESULTS:
                CdcIoPortsStatusUpdate(); // activate ports involved in the new AHECs
                gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE; // reset stage
                gpInstRec->cdcTaskFlags &= ~CDC_TASK_CONNECT_ADJACENT; // stop the task
                gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED; // reset task mask as this task is done
                cdcFeedbackMsg = CDC_FB_MSG_CONNECT_ADJ_DONE;
                break;
 		default:
			 	_printf("--------FAIL (CdcTaskConnectAdjacent)----------\n",0,0);
 				break;
    }

    return cdcFeedbackMsg;
}
#endif


//------------------------------------------------------------------------------
// Function:     CdcTaskDisconnect
// Description:  Deactivation of an AHEC by Activator request is done by sending
//               out SetState message (no response gathering)
//               Deactivation of an AHEC by Passive device is done by sending
//               RequestDeactivation message
//               Task approach is needed if more than one AHEC has to be deactivated
//
// Returns:      Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

static uint8_t CdcTaskDisconnect(void)
{
    uint8_t     cdcFeedbackMsg = CDC_FB_MSG_NONE;

    switch(gpInstRec->cdcTaskState)
    {
        case CDC_TASK_EMPTY_STATE_NONE: // all tasks shall start from empty state
                gpInstRec->cdcTaskMask = CDC_TASK_DISCONNECT; // mask out other tasks until this one is done
                gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_STATE_DEACTIVATE;
                // don't break here to save time and go ahead to the fist step

        case CDC_TASK_DISCONNECT_STATE_DEACTIVATE:

                if (gpInstRec->aHec[gpInstRec->currentHecIdx].status.isActivator == true)
                {
                    // Activator deactivates AHEC by sending SetState command
                    CdcHecSetStateSend(CDC_HEC_INACTIVE, 1, &gpInstRec->currentHecIdx); // one HEC at a time
                    CdcAliveDeviceListRemove(gpInstRec->currentHecIdx); // remove devices from Alive List (if possible)
                    CdcHecDescriptorRemove(gpInstRec->currentHecIdx); // release resources for HEC
                    CdcHecDisableCheck(); // disable HEC hardware if no active HECs
                }
                else
                {
                    // Passive device must request deactivation from the Activator
                    CdcHecDeactivationRequestSend(gpInstRec->currentHecIdx);
                }

                gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_STATE_REPORT_RESULTS; // next stage
                break;

        case CDC_TASK_DISCONNECT_STATE_REPORT_RESULTS:
                CdcIoPortsStatusUpdate(); // deactivate ports involved in the deactivated HEC
                gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE; // reset stage
                gpInstRec->cdcTaskFlags &= ~CDC_TASK_DISCONNECT; // stop the task
                gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED; // reset task mask as this task is done
                cdcFeedbackMsg = CDC_FB_MSG_DISCONNECT_DONE;
                break;
		default:
			 	_printf("--------FAIL (CdcTaskDisconnect)----------\n",0,0);
 				break;
    }

    return cdcFeedbackMsg;
}


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    CdcTaskDisconnectAll
// Description: deactivates all curretly open AHECs
//
// Returns:     Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

static uint8_t CdcTaskDisconnectAll(void)
{
    uint8_t     cdcFeedbackMsg = CDC_FB_MSG_NONE;

    switch(gpInstRec->cdcTaskState)
    {
        case CDC_TASK_EMPTY_STATE_NONE: // all tasks shall start from empty state
                gpInstRec->cdcTaskMask = CDC_TASK_DISCONNECT_ALL; // mask out other tasks until this one is done
                gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE_INIT;
                // don't break here to save time and go ahead to the fist step

        case CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE_INIT:
                gpInstRec->currentHecIdx = 0;   // initialize HEC index
                gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE; // next stage
                //break;

        case CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE:

                while (gpInstRec->currentHecIdx < CDC_NUMBER_OF_HECS_MAX)
                {
                    if ((gpInstRec->aHec[gpInstRec->currentHecIdx].status.isInUse == true) &&
                        (gpInstRec->aHec[gpInstRec->currentHecIdx].status.channelState > CDC_PHEC))
                    {
                        // Found AHEC or VHEC; begin deactivation process. Increment the current HEC index when done
                        if (gpInstRec->aHec[gpInstRec->currentHecIdx].status.isActivator == true)
                        {
                            // Activator deactivates AHEC by sending SetState command
                            // Must check if CEC write FIFO isn't full before sending out a new message
                            CdcHecSetStateSend(CDC_HEC_INACTIVE, 1, &gpInstRec->currentHecIdx); // one HEC at a time
                            CdcAliveDeviceListRemove(gpInstRec->currentHecIdx); // remove devices from Alive List (if possible)
                            CdcHecDescriptorRemove(gpInstRec->currentHecIdx); // release resources for the HEC
                            CdcHecDisableCheck(); // disable HEC hardware if no active HECs

                            // Then it's necessary to wait for some time to let the Activator's message to get out (?)
                            CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MSG_OUT_TIME_MS), &gpInstRec->cdcTaskTimeCount);
                            gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE_WAIT;
                        }
                        else
                        {
                            // Passive device must request deactivation from the Activator
                            CdcHecDeactivationRequestSend(gpInstRec->currentHecIdx);
                            // Then it's necessary to wait for MRT to let the Activator respond with Deactivation message
                            CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);
                            gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE_WAIT;
                        }

                        gpInstRec->currentHecIdx++;
                        break; // do one message sending at a time
                    }
                    gpInstRec->currentHecIdx++;
                }

                if (gpInstRec->currentHecIdx == CDC_NUMBER_OF_HECS_MAX) // if all HECs have been processed
                {
                    gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_ALL_STATE_REPORT_RESULTS; // next stage
                }
                break;

        case CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE_WAIT:
                // wait until the Activator respond
                // criteria: timeout since the DeactivationRequest message sending

                // Set recall timer to maximum in order to allow the task counters to set it to a new value
                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);

                CdcTimeCounterUpdate(&gpInstRec->cdcTaskTimeCount);

                if (CdcIsTimeCounterExpired(&gpInstRec->cdcTaskTimeCount))
                {

                    gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE; // return from waiting
                }
                break;

        case CDC_TASK_DISCONNECT_ALL_STATE_REPORT_RESULTS:
                CdcIoPortsStatusUpdate(); // deactivate ports involved in the deactivated HEC
                gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE; // reset stage
                gpInstRec->cdcTaskFlags &= ~CDC_TASK_DISCONNECT_ALL; // stop the task
                gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED; // reset task mask as this task is done
                cdcFeedbackMsg = CDC_FB_MSG_DISCONNECT_ALL_DONE;
                break;
		default:
			 	_printf("--------FAIL (CdcTaskDisconnectAll)----------\n",0,0);
 				break;
    }

    return cdcFeedbackMsg;
}
#endif


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    CdcTaskDisconnectLost
// Description: deactivate all AHECs including a failed device
//
// Returns:     Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

static uint8_t CdcTaskDisconnectLost(void)
{
    uint8_t     cdcFeedbackMsg = CDC_FB_MSG_NONE;

    switch(gpInstRec->cdcTaskState)
    {
        case CDC_TASK_EMPTY_STATE_NONE: // all tasks shall start from empty state
                gpInstRec->cdcTaskMask = CDC_TASK_DISCONNECT_LOST; // mask out other tasks until this one is done
                gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE_INIT;
                // don't break here to save time and go ahead to the fist step

        case CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE_INIT:
                 gpInstRec->currentHecIdx = 0;   // initialize HEC index
                 gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE; // next stage
                 //break;

         case CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE:

                 while (gpInstRec->currentHecIdx < CDC_NUMBER_OF_HECS_MAX)
                 {
                     if ((gpInstRec->aHec[gpInstRec->currentHecIdx].status.isInUse == true) &&
                         (gpInstRec->aHec[gpInstRec->currentHecIdx].status.channelState > CDC_PHEC))
                     {
                        if (((gpInstRec->aHec[gpInstRec->currentHecIdx].status.isActivator == true) &&
                             CdcIsDeviceBelongsToHec(gpInstRec->aHec[gpInstRec->currentHecIdx].firstDevPhysAddr,
                                                     gpInstRec->aHec[gpInstRec->currentHecIdx].lastDevPhysAddr,
                                                     gpInstRec->aliveDevPhysAddr)) ||
                            (gpInstRec->aHec[gpInstRec->currentHecIdx].activatorPhysAddr == gpInstRec->aliveDevPhysAddr))
                         {
                             // Found AHEC or VHEC that contain the lost device; begin deactivation process.
                             if (gpInstRec->aHec[gpInstRec->currentHecIdx].status.isActivator == true)
                             {
                                 // Activator deactivates AHEC by sending SetState command
                                 // Must check if CEC write FIFO isn't full before sending out a new message
                                 CdcHecSetStateSend(CDC_HEC_INACTIVE, 1, &gpInstRec->currentHecIdx); // one HEC at a time
                                 // Then it's necessary to wait for some time to let the Activator's message to get out (?)
                                 CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MSG_OUT_TIME_MS), &gpInstRec->cdcTaskTimeCount);
                                 gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE_WAIT;
                             }
                             else
                             {
                                 // Passive device remove HEC containing the lost device without any signalling
                                 // No need for waiting, schedule the next cycle to continue immediatelly
                                 gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE;
                             }
                             CdcAliveDeviceListRemove(gpInstRec->currentHecIdx); // remove devices from Alive List (if possible)
                             CdcHecDescriptorRemove(gpInstRec->currentHecIdx); // release resources for the HEC
                             CdcHecDisableCheck(); // disable HEC hardware if no active HECs

                             gpInstRec->currentHecIdx++;
                             break; // do one message sending at a time
                         }
                     }
                     gpInstRec->currentHecIdx++;
                 }

                 if (gpInstRec->currentHecIdx == CDC_NUMBER_OF_HECS_MAX) // if all HECs have been processed
                 {
                    gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_LOST_STATE_REPORT_RESULTS; // next stage
                 }
                 break;

         case CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE_WAIT:
                 // wait until activator's deactivation message is processed
                 // criteria: timeout since the starting of deactivation

                 // Set recall timer to maximum in order to allow the task counters to set it to a new value
                 CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);

                 CdcTimeCounterUpdate(&gpInstRec->cdcTaskTimeCount);

                 if (CdcIsTimeCounterExpired(&gpInstRec->cdcTaskTimeCount))
                 {
                     gpInstRec->cdcTaskState = CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE; // return from waiting
                 }
                 break;

         case CDC_TASK_DISCONNECT_LOST_STATE_REPORT_RESULTS:
                 CdcIoPortsStatusUpdate(); // deactivate ports involved in the deactivated HEC
                 gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE; // reset stage
                 gpInstRec->cdcTaskFlags &= ~CDC_TASK_DISCONNECT_LOST; // stop the task
                 gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED; // reset task mask as this task is done
                 cdcFeedbackMsg = CDC_FB_MSG_DISCONNECT_LOST_DONE;
                 break;

 		default:
			 	_printf("--------FAIL (CdcTaskDisconnectLost)----------\n",0,0);
			 	break;
    }

    return cdcFeedbackMsg;
}
#endif


#if (IS_RX == 1)
//------------------------------------------------------------------------------
// Function:    CdcTaskHpdSignal
// Description: Send HPD SetState message and wait until the HDMI Source responds
//
// Returns:     Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

uint8_t CdcTaskHpdSignal(void)
{
    uint8_t     cdcFeedbackMsg = CDC_FB_MSG_NONE;

    // Send HPD SetState message and wait until the HDMI Source responds
    switch(gpInstRec->cdcTaskState)
    {
        case CDC_TASK_EMPTY_STATE_NONE: // all tasks shall start from empty state
                gpInstRec->cdcTaskMask = CDC_TASK_HPD_SIGNAL; // mask out other tasks until this one is done
                gpInstRec->cdcTaskState = CDC_TASK_HPD_SIGNAL_STATE_START;
                // don't break here to save time and go ahead to the first step

        case CDC_TASK_HPD_SIGNAL_STATE_START: // First stage: Sending an HPD SetState message
                CdcHpdSetStateSend(gpInstRec->currentPortIdx);
                gpInstRec->cdcTaskState = CDC_TASK_HPD_SIGNAL_STATE_RESPONSE_WAIT; // next stage
                _printf(("[CDC_TASK_HPD_SIGNAL]: Waiting for HPD ReportState\n"),0,0);
                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);
                break;

        case CDC_TASK_HPD_SIGNAL_STATE_RESPONSE_WAIT:
                // wait until Source device responds
                // criteria: MRT timeout

                // Set recall timer to maximum in order to allow the task counters to set it to a new value
                CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);

                CdcTimeCounterUpdate(&gpInstRec->cdcTaskTimeCount);

                if (CdcIsTimeCounterExpired(&gpInstRec->cdcTaskTimeCount))
                {
                    // stop the task and provide error feedback
                    gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE;  // reset stage
                    gpInstRec->cdcTaskFlags &= ~CDC_TASK_HPD_SIGNAL;  // stop the task
                    gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED; // reset task mask as this task is done
                    cdcFeedbackMsg = CDC_FB_MSG_ERR_HPD_SIGNAL;
                    _printf(("[CDC_TASK_HPD_SIGNAL]: HDMI Source haven't responded\n"),0,0);
                }
                break;

        case CDC_TASK_HPD_SIGNAL_STATE_REPORT_RESULTS:
                gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE;  // reset stage
                gpInstRec->cdcTaskFlags &= ~CDC_TASK_HPD_SIGNAL; // stop the task
                gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED; // reset task mask as this task is done
                cdcFeedbackMsg = CDC_FB_MSG_HPD_SIGNAL_DONE; // successful end of the HPD Signalling task
                break;
	 	default:
			 	_printf("--------FAIL (CdcTaskHpdSignal)----------\n",0,0);
 				break;
    }

    return cdcFeedbackMsg;
}
#endif

//------------------------------------------------------------------------------
// Function:    CdcTaskHpdCapabilityVerification
// Description: HPD Capability verification task; sends InqureState Message to
//              an adjacent device and checks the response
//
// Returns:     Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

uint8_t CdcTaskHpdCapabilityVerification(void)
{
    uint8_t     cdcFeedbackMsg = CDC_FB_MSG_NONE;

    // HPD Capability verification task
    // Sends InqureState Message to an adjacent device and checks the response
    switch(gpInstRec->cdcTaskState)
    {
        case CDC_TASK_EMPTY_STATE_NONE: // all tasks shall start from empty state
                 gpInstRec->cdcTaskMask = CDC_TASK_HPD_CAPAB_VERIF; // mask out other tasks until this one is done
                 gpInstRec->cdcTaskState = CDC_TASK_HPD_CAPAB_STATE_START;
                 // don't break here to save time and go ahead to the fist step

         case CDC_TASK_HPD_CAPAB_STATE_START: // First stage: Sending an InquireState message to adjacent source device
                 if (CdcHecInquireStateOfAdjacentSend(false, gpInstRec->currentPortIdx)) // input port mode
                 { // if error
                     _printf(("[CDC_TASK_HPD_CAPAB_VERIF]: The adjecent device doesn't exist\n"),0,0);
                 }
                 gpInstRec->cdcTaskState = CDC_TASK_HPD_CAPAB_STATE_RESPONSE_WAIT; // next stage
                 _printf(("[CDC_TASK_HPD_CAPAB_VERIF]: Waiting for HPD capability verification ReportState\n"),0,0);
                 CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);
                 break;

         case CDC_TASK_HPD_CAPAB_STATE_RESPONSE_WAIT:
                 // wait until Source device responds
                 // criteria: MRT timeout

                 // Set recall timer to maximum in order to allow the task counters to set it to a new value
                 CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);

                 CdcTimeCounterUpdate(&gpInstRec->cdcTaskTimeCount);

                 if (CdcIsTimeCounterExpired(&gpInstRec->cdcTaskTimeCount))
                 {
                     // stop the task and provide error feedback
                     gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE;   // reset stage
                     gpInstRec->cdcTaskFlags &= ~CDC_TASK_HPD_CAPAB_VERIF;  // stop the task
                     gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED;  // reset task mask as this task is done
                     cdcFeedbackMsg = CDC_FB_MSG_ERR_HPD_CAP_FAILED;
                     _printf(("[CDC_TASK_HPD_SIGNAL]: HDMI Source haven't responded to HPD capability verification request\n"),0,0);
                 }
                 break;

         case CDC_TASK_HPD_CAPAB_STATE_REPORT_RESULTS:
                 gpInstRec->cdcTaskState = CDC_TASK_EMPTY_STATE_NONE;  // reset stage
                 gpInstRec->cdcTaskFlags &= ~CDC_TASK_HPD_CAPAB_VERIF; // stop the task
                 gpInstRec->cdcTaskMask = CDC_TASK_MASK_ALL_UNMASKED; // reset task mask as this task is done
                 cdcFeedbackMsg = CDC_FB_MSG_HPD_CAPABILITY_CONF; // HPD capability of the adjacent source is confirmed
                 break;
 		default:
			 	_printf("--------FAIL (CdcTaskHpdCapabilityVerification)----------\n",0,0);
			 	break;
    }

    return cdcFeedbackMsg;
}


//------------------------------------------------------------------------------
// Function:    CdcTaskBase
// Description: Performs CDC message parsing and support basic CDC functionality
//              of passive devices.
//              This task can't be masked and will run in background of the
//              other active task, if an incoming CDC message is pending
//
// Parameters:  pCdcMessage - pointer to a pending CDC message or 0, if there is no
//              message awaiting
//
// Returns:     Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

static uint8_t CdcTaskBase(SI_CpiData_t *pCdcMessage)
{
//		uint8_t     cdcFeedbackMsg = CDC_FB_MSG_NONE;
    static uint8_t     cdcFeedbackMsg = CDC_FB_MSG_NONE;	//Beacuse of Memory issue	// 160118 HSH
    uint16_t    initiatorPhysAddr;
    uint16_t    targetPhysAddr;
    uint16_t    firstPhysAddr;
    uint16_t    lastPhysAddr;
    uint8_t     inputPortIndex;
    uint8_t     aInputPortRtIdx[2];
    bool_t      outputPortRtFlg;
    uint8_t     hecIdx;
    uint8_t     hecCount;
    uint8_t     numberOfHecs;
    bool_t      tmpCond;
//		uint8_t     errCode = CDC_ERRCODE_NO_ERROR;
    static uint8_t     errCode = CDC_ERRCODE_NO_ERROR;	//Beacuse of Memory issue	// 160118 HSH
    uint8_t     i;
    uint8_t     j;

	cdcFeedbackMsg = CDC_FB_MSG_NONE;	//Beacuse of Memory issue	// 160118 HSH
	errCode = CDC_ERRCODE_NO_ERROR;		//Beacuse of Memory issue	// 160118 HSH

    if (pCdcMessage != 0) //decode a message, update status
    {
        initiatorPhysAddr = (pCdcMessage->args[0] << 8) + pCdcMessage->args[1];

        // Specifically ignore, but consume, messages that belong to CDC group and have direct addressing
        if ((pCdcMessage->opcode == CDC_CEC_OPCODE) && ((pCdcMessage->srcDestAddr & 0xF) != 0xF))
        {
            cdcFeedbackMsg = CDC_FB_MSG_NONE;
        }
        // check if the message belongs to CDC group and has broadcast destination
        else if ((pCdcMessage->opcode == CDC_CEC_OPCODE) && ((pCdcMessage->srcDestAddr & 0xF) == 0xF))
        {
            switch (pCdcMessage->args[2]) // CDC operation code
            {
                case CDCOP_HEC_INQUIRE_STATE:

                    if (pCdcMessage->argCount != 7)
					{
                        _printf(("[CDC_TASK_PASSIVE_SERVICE]: ERROR in InquireState message: wrong length!\n"),0,0);
                        break;
                    }

                    firstPhysAddr = (pCdcMessage->args[3] << 8) + pCdcMessage->args[4];
                    lastPhysAddr  = (pCdcMessage->args[5] << 8) + pCdcMessage->args[6];

                    // reply with ReportState message only if the device belongs to the inquired HEC
                    if (CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, gpInstRec->physAddr))
                    {
                        // report without error if all relevant ports are HEC-capable
                        if(CdcCapablePortGet(firstPhysAddr, lastPhysAddr, aInputPortRtIdx, &outputPortRtFlg))
                        {
                            TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: received InquireState message from PA = %04x\n", initiatorPhysAddr));
                            if (outputPortRtFlg)
                            {
                                _printf(("* HEC capable Output port is involved\n"),0,0);
                            }

                            for (i = 0; i < 2; i++)
                            {
                                if (aInputPortRtIdx[i] != 0xFF)
                                {
                                    TPI_CDC_PRINT(("* HEC capable Input port %d is involved\n", (int) aInputPortRtIdx[i]));
                                }
                            }
                        }
                        else
                        {
                            // report an error when required ports are HEC incapable
                            gpInstRec->deviceStatus.cdcErrorState = CDC_ERRCODE_NOT_SUPPORTED_CAPABILITY;
                        }

                        CdcHecReportStateSend(initiatorPhysAddr, CDC_REPORT_STATE_SHORT_MODE);
                    }
                    break;

                case CDCOP_HEC_REPORT_STATE:

                    errCode = pCdcMessage->args[5] & 0x03;

                    if ((pCdcMessage->argCount != 6) && (pCdcMessage->argCount != 8))
                    {
                        _printf(("[CDC_TASK_PASSIVE_SERVICE]: ERROR in ReportState message: wrong length!\n"),0,0);
                        break;
                    }
                    else if (errCode != CDC_ERRCODE_NO_ERROR)
                    {
                        TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Device PA = %04x reported an error: ", initiatorPhysAddr));
                        switch (errCode)
                        {
                            case CDC_ERRCODE_NOT_SUPPORTED_CAPABILITY:
                                _printf(("capability not supported.\n"),0,0);
                                break;
                            case CDC_ERRCODE_INCOMPATIBLE_STATE:
                                _printf(("incompatible state.\n"),0,0);
                                break;
                            case CDC_ERRCODE_OTHER_ERROR:
                                _printf(("other error.\n"),0,0);
								break;
 							default:
								_printf(("other error. JYP\n"),0,0);
                                break;
                        }

                        CdcTimeCounterReset(&gpInstRec->cdcTaskTimeCount); // stop waiting to react immediatelly
                        break;
                    }


                    targetPhysAddr = (pCdcMessage->args[3] << 8) + pCdcMessage->args[4];

                    if ((targetPhysAddr == 0xFFFF) && (pCdcMessage->argCount == 8)) // Capability notification broadcast
                    {
                        TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: received Capability Notification message from PA = %04x\n", initiatorPhysAddr));
                        // Check if some of AHECs have to be deactivated if the initiating device reports
                        // its HEC capability change and takes part in those AHECs

                        // -- Normally, all devices shall disconnect all their open HECs
                        //    before changing HEC support state to not supported

                        // Update found device list taking into account the new capability information
                        tmpCond = false; // not found by default
                        for (i = 0; i < gpInstRec->numberOfAllHosts; i++)
                        {
                            if (gpInstRec->cdcFoundHostsList[i].physAddr == initiatorPhysAddr)
                            {
                                if (((pCdcMessage->args[5] >> 6) & 0x03) != CDC_HEC_FUNC_NOT_SUPPORTED)
                                {  // update port capability bits and device status
                                    gpInstRec->cdcFoundHostsList[i].hostInfo.portHecCapBits = 0x7FFF & ((pCdcMessage->args[6] << 8) | pCdcMessage->args[7]);
                                    gpInstRec->cdcFoundHostsList[i].devStat.hecFuncState  = ((pCdcMessage->args[5] >> 6) & 0x03);
                                    gpInstRec->cdcFoundHostsList[i].devStat.hostFuncState = ((pCdcMessage->args[5] >> 4) & 0x03);
                                    gpInstRec->cdcFoundHostsList[i].devStat.encFuncState  = ((pCdcMessage->args[5] >> 2) & 0x03);
                                    gpInstRec->cdcFoundHostsList[i].devStat.cdcErrorState =  (pCdcMessage->args[5] & 0x03);
                                }
                                else
                                {
                                    // remove incapable device from the list
                                    for (j = i; j < (gpInstRec->numberOfAllHosts - 1); j++)
                                    {
                                        gpInstRec->cdcFoundHostsList[j] = gpInstRec->cdcFoundHostsList[j+1];
                                    }
                                    gpInstRec->numberOfAllHosts--;
                                }
                                tmpCond = true; // set found flag
                                break;
                            }
                        }

                        if (!tmpCond) // if initiator haven't been found in the list
                        {
                            CdcHecAddNewDeviceToHostList(pCdcMessage, initiatorPhysAddr);
                        }

                        CdcDiscoveryDataArrange();

                        // check if initiating device has been excluded from the list of
                        // potentially reachable hosts
                        tmpCond = true; // will flip to false, if the device have been found in the list
                        for (i = 0; i < gpInstRec->numberOfFoundHosts; i++)
                        {
                            if (gpInstRec->cdcFoundHostsList[i].physAddr == initiatorPhysAddr)
                            {
                                tmpCond = false; // device is still in the list
                                break;
                            }
                        }
                        // if so, disconnect all HECs that include the failed device
                        // Make sure other tasks are inactive, leave the affected HECs to be
                        // deactivated by timeout otherwise
                        if (tmpCond)
                        {
                            CdcLostDeviceDisconnect(initiatorPhysAddr);
                        }

                        cdcFeedbackMsg = CDC_FB_MSG_CAPABILITY_CHANGED;
                        break;
                    }
                    else if (targetPhysAddr != gpInstRec->physAddr)
                    {
                        break; // don't process message addressed to another device
                    }

                    TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: received ReportState message from PA = %04x\n", initiatorPhysAddr));

                    // Process ReportState messages for response gathering stage of ENC search task
                    if (gpInstRec->cdcTaskState == CDC_TASK_FIND_STATE_GATHER_RESPONSES)
                    {
                        if (gpInstRec->cdcTaskFlags & CDC_TASK_HOSTS_SEARCH)
                        {
                            // reset the time counter if new ReportState message arrived
                            CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);

                            // Collect HEC & Ext NW capable device PAs if ...
                            if (pCdcMessage->argCount == 8) // Discovery mode of ReportState
                            {
                                CdcHecAddNewDeviceToHostList(pCdcMessage, initiatorPhysAddr);
                            }
                        }
                    }
                    // Process ReportState messages for InquireState response gathering stage of Connect Task
                    else if (gpInstRec->cdcTaskState == CDC_TASK_CONNECT_STATE_GATHER_INQ_REP)
                    {
                        // reset the time counter if new ReportState message arrived
                        CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);

                        // Make sure all devices in tested PHEC report HEC support
                        // if not, the HEC can't obtain VHEC status
                        if (((pCdcMessage->args[5] >> 6) & 0x03) == CDC_HEC_FUNC_NOT_SUPPORTED) // HEC function isn't available
                        {
                            TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Device PA = %04x doesn't support HEC function. PHEC %d can't be verified.\n", initiatorPhysAddr, (int) gpInstRec->currentHecIdx));
                            CdcHecDescriptorRemove(gpInstRec->currentHecIdx); // Remove failed PHEC
                            gpInstRec->currentHecIdx++; // step to next HEC
                            gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_INQUIRE_START; // move to another PHEC search
                            CdcTimeCounterReset(&gpInstRec->cdcTaskTimeCount); // stop waiting to react immediatelly
                        }
                        else if (errCode == CDC_ERRCODE_NO_ERROR)
                        {   // add to the number of verified devices, if the device also belongs to HEC
                            firstPhysAddr = gpInstRec->aHec[gpInstRec->currentHecIdx].firstDevPhysAddr;
                            lastPhysAddr  = gpInstRec->aHec[gpInstRec->currentHecIdx].lastDevPhysAddr;
                            if (CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, initiatorPhysAddr))
                            {
                                gpInstRec->verifiedDevNum[0]++;
                                TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Device PA = %04x supports HEC function.\n", initiatorPhysAddr));
                            }
                        }
                    }
                    else if (gpInstRec->cdcTaskState == CDC_TASK_CONNECT_STATE_GATHER_ACT_REP)
                    {
                        // reset the time counter if new ReportState message arrived
                        CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RESPONSE_TIME_MS), &gpInstRec->cdcTaskTimeCount);

                        if ((!gpInstRec->connectSettings.isMultHecMode) && (pCdcMessage->argCount == 6)) // short mode of ReportState
                        {
                            // Make sure all devices in tested VHEC report activated status
                            // if not, the HEC can't obtain AHEC status
                            if (((pCdcMessage->args[5] >> 6) & 0x03) != CDC_HEC_STATE_ACTIVE) // HEC isn't active
                            {
                                TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Device PA = %04x isn't active. AHEC %d can't be verified.\n", initiatorPhysAddr, (int) gpInstRec->currentHecIdx));
                                CdcHecDescriptorRemove(gpInstRec->currentHecIdx); // Remove failed PHEC
                                gpInstRec->currentHecIdx++; // step to next HEC
                                gpInstRec->cdcTaskState = CDC_TASK_CONNECT_STATE_INQUIRE_START; // move to another PHEC search
                                CdcTimeCounterReset(&gpInstRec->cdcTaskTimeCount); // stop waiting to react immediatelly
                            }
                            else if (errCode == CDC_ERRCODE_NO_ERROR)
                            {   // add to the number of verified devices, if the device also belongs to HEC
                                firstPhysAddr = gpInstRec->aHec[gpInstRec->currentHecIdx].firstDevPhysAddr;
                                lastPhysAddr  = gpInstRec->aHec[gpInstRec->currentHecIdx].lastDevPhysAddr;
                                if (CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, initiatorPhysAddr))
                                {
                                    gpInstRec->verifiedDevNum[0]++;
                                    TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Device PA = %04x is HEC-active.\n", initiatorPhysAddr));
                                }
                            }

                        }
                        else if (gpInstRec->connectSettings.isMultHecMode) // Multi-HEC activation mode
                        {
                            uint8_t hecFuncState = (pCdcMessage->args[5] >> 6) & 0x03;

                            // collect positive responses for every VHEC in multi-VHEC activation command
                            if (((hecFuncState == CDC_HEC_STATE_ACTIVE) && (pCdcMessage->argCount == 6)) ||
                                ((hecFuncState == CDC_HEC_ACTIVATION_FLD) && (pCdcMessage->argCount == 8)))
                            {   // add to the number of verified device
                                for (i = 0; i < gpInstRec->connectSettings.hecActDelayed; i++)
                                {
                                    if (CdcIsDeviceBelongsToHec(gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].firstDevPhysAddr,
                                                                gpInstRec->aHec[gpInstRec->delayedHecIdx[i]].lastDevPhysAddr,
                                                                initiatorPhysAddr))
                                    {
                                        if (pCdcMessage->argCount == 8) // multi-HEC mode of ReportState (device is part of more than one HECs)
                                        {   // check if ports are active to support this HEC
                                            if (!CdcIsHecEnabledInDev(gpInstRec->delayedHecIdx[i], initiatorPhysAddr,
                                                                 (pCdcMessage->args[6] << 8) | pCdcMessage->args[7]))
                                            {
                                                TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Device PA = %04x ports can't support HEC %d.\n", initiatorPhysAddr, (int) i));
                                                continue; // jump to the next HEC, if failed to confirm port activation
                                            }
                                        }

                                        gpInstRec->verifiedDevNum[i]++;
                                        TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Device PA = %04x is active for HEC %d.\n", initiatorPhysAddr, (int) i));
                                    }
                                }
                            }
                            else
                            {
                                TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Device PA = %04x isn't active. AHECs can't be verified.\n", initiatorPhysAddr));
                                CdcTimeCounterReset(&gpInstRec->cdcTaskTimeCount); // stop waiting to react immediatelly
                            }
                        }

                    }
                    else if (gpInstRec->cdcTaskState == CDC_TASK_CONNECT_ADJ_STATE_WAIT_ACT_REP)
                    {

                        if (pCdcMessage->argCount == 6) // short mode of ReportState
                        {
                            // Make sure adjacent device reports activated status
                            // if not, the HEC can't obtain AHEC status
                            if (((pCdcMessage->args[5] >> 6) & 0x03) == CDC_HEC_STATE_ACTIVE) // HEC is active
                            {
                                firstPhysAddr = gpInstRec->aHec[gpInstRec->currentHecIdx].firstDevPhysAddr;
                                lastPhysAddr  = gpInstRec->aHec[gpInstRec->currentHecIdx].lastDevPhysAddr;
                                if (CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, initiatorPhysAddr))
                                {
                                    gpInstRec->verifiedDevNum[0] = 1;
                                    TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: Adjacent device PA = %04x is HEC-active.\n", initiatorPhysAddr));
                                }
                            }
                        }
                    }
                    else if (gpInstRec->cdcTaskState == CDC_TASK_HPD_CAPAB_STATE_RESPONSE_WAIT)
                    {
                        // Make sure the response comes from an adjacent HDMI source
                        // and the device doesn't report an error status
                        if (CdcIsLinkedDirectly(gpInstRec->physAddr, initiatorPhysAddr, gpInstRec->currentPortIdx) &&
                            ((pCdcMessage->args[5] & 0x03) == CDC_ERRCODE_NO_ERROR))
                        {
                            TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: PA = %04x confirms HPD-over-CDC capability\n", initiatorPhysAddr));
                            gpInstRec->cdcTaskState = CDC_TASK_HPD_CAPAB_STATE_REPORT_RESULTS; // break waiting
                        }
                    }
                    break;

                case CDCOP_HEC_SET_STATE_ADJACENT:

#if (IS_CDC_REDUCED != 1)
                    if (pCdcMessage->argCount != 6)
                    {
                        _printf(("[CDC_TASK_PASSIVE_SERVICE]: ERROR in SetStateAdjacent message: wrong length!\n"),0,0);
                        break;
                    }

                    lastPhysAddr  = (pCdcMessage->args[3] << 8) + pCdcMessage->args[4]; // shall be equal to PA of this device ???
                    if (lastPhysAddr != gpInstRec->physAddr)
                    {
                        break;
                    }

                    // Make sure the request comes from an adjacent device, either HDMI Sink or Source,
                    // and receiving (this) device has capable input or output port that connects it to the initiator
                    tmpCond = false; // adjacency flag
                    if (CdcIsCapableToAdjacent(initiatorPhysAddr, lastPhysAddr, &tmpCond) && (gpInstRec->deviceStatus.hecFuncState != CDC_HEC_FUNC_NOT_SUPPORTED))
                    {
                        // Immediately enable the AHEC without verification of the channel and Alive Notifications
                        // Activation command
                        if (pCdcMessage->args[5] == CDC_HEC_ACTIVE)
                        {
                            TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: received SetStateAdjacent::Activate message from PA = %04x\n", initiatorPhysAddr));

                            // Add new AHEC to the Instance Record
                            hecIdx = CdcHecDescriptorAdd(false);
                            if (hecIdx == 0xFF)
                            {
                                _printf(("[CDC_TASK_PASSIVE_SERVICE]: Can't add new HEC descriptor.\n"),0,0);
                                gpInstRec->deviceStatus.cdcErrorState = CDC_ERRCODE_OTHER_ERROR; // memory limitation error
                            }
                            else
                            {
                                gpInstRec->aHec[hecIdx].activatorPhysAddr = initiatorPhysAddr;
                                gpInstRec->aHec[hecIdx].firstDevPhysAddr = initiatorPhysAddr;
                                gpInstRec->aHec[hecIdx].lastDevPhysAddr = lastPhysAddr;
                                gpInstRec->aHec[hecIdx].status.channelState = CDC_AHEC; // Passive device registers AHECs only
                                gpInstRec->aHec[hecIdx].status.isAdjacent = true;

                                // Activate eHDMI function
                                if (CdcEhdmiHwEnable(true))
                                {
                                    gpInstRec->deviceStatus.hecFuncState = CDC_HEC_STATE_ACTIVE;
                                    CdcIoPortsStatusUpdate();// activate involved ports
                                }
                            }

                        }
                        else // Deactivation command
                        if (pCdcMessage->args[5] == CDC_HEC_INACTIVE)
                        {
                            TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: received SetStateAdjacent::Deactivate message from PA = %04x\n", initiatorPhysAddr));

                            // Remove the AHEC from the Instance Record
                            hecIdx = CdcHecIndexGet(initiatorPhysAddr, initiatorPhysAddr, lastPhysAddr);
                            if (hecIdx == 0xFF)
                            {
                                _printf(("[CDC_TASK_PASSIVE_SERVICE]: Can't find existing HEC descriptor to delete.\n"),0,0);
                            }
                            else
                            {
                                CdcHecDescriptorRemove(hecIdx);
                                CdcHecDisableCheck(); // disable HEC hardware if no active HECs

                                // Update I/O ports status
                                // Deactivate ports that no longer participate in active HECs
                                CdcIoPortsStatusUpdate();
                            }

                        }

                        CdcHecReportStateSend(initiatorPhysAddr, CDC_REPORT_STATE_SHORT_MODE);
                    }
                    else if (tmpCond == true) // if two devices are adjacent but not capable for connection
                    {
                        // report an error when required ports are HEC incapable
                        gpInstRec->deviceStatus.cdcErrorState = CDC_ERRCODE_NOT_SUPPORTED_CAPABILITY;
                        CdcHecReportStateSend(initiatorPhysAddr, CDC_REPORT_STATE_SHORT_MODE);
                    }
#endif
                    break;

                case CDCOP_HEC_SET_STATE:
                    numberOfHecs = (pCdcMessage->argCount - 6) / 2; // total number of HECs in the SetState command
                    if ((numberOfHecs < 1) || (numberOfHecs > 4))
                    {
                        _printf(("[CDC_TASK_PASSIVE_SERVICE]: ERROR in SetState message format!\n"),0,0);
                        break;
                    }

                    hecCount = 0; // number of HECs where this device participate (=0 by default)
                    firstPhysAddr = (pCdcMessage->args[3] << 8) + pCdcMessage->args[4];
                    lastPhysAddr  = (pCdcMessage->args[5] << 8) + pCdcMessage->args[6];

                    // Activation command
                    if (pCdcMessage->args[7] == CDC_HEC_ACTIVE)
                    {
                        bool_t isAtLeastOneDeviceBelongsToHec = false;

                        // It can be from 1 to 4 HECs that need to be activated or omited
                        for (i = 0; i < numberOfHecs; i++) // can be up to 4 terminator's PAs
                        {
                            // reply with ReportState message only if the device belongs to the inquired HEC
                            if (CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, gpInstRec->physAddr))
                            {
                                isAtLeastOneDeviceBelongsToHec = true;

                                // report no error if all relevant ports are HEC capable
                                if (CdcCapablePortGet(firstPhysAddr, lastPhysAddr, aInputPortRtIdx, &outputPortRtFlg))
                                {
                                        TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: received SetState::Activate message from PA = %04x, (HEC %d)\n", initiatorPhysAddr, (int) i));

                                        // Make sure the HEC is new and hasn't been activated already
                                        hecIdx = CdcHecIndexGet(initiatorPhysAddr, firstPhysAddr, lastPhysAddr);
                                        if (hecIdx == 0xFF) // existing HEC not found
                                        {
                                            hecCount++; // count this HEC, if this device is a part of it and capable to handle the HEC

                                            // Add new AHEC to the Instance Record
                                            hecIdx = CdcHecDescriptorAdd(false);
                                            if (hecIdx == 0xFF)
                                            {
                                                _printf(("[CDC_TASK_PASSIVE_SERVICE]: Can't add new HEC descriptor.\n"),0,0);
                                                gpInstRec->deviceStatus.cdcErrorState = CDC_ERRCODE_OTHER_ERROR; // memory limitation error
                                            }
                                            else
                                            {
                                                gpInstRec->aHec[hecIdx].activatorPhysAddr = initiatorPhysAddr;
                                                gpInstRec->aHec[hecIdx].firstDevPhysAddr = firstPhysAddr;
                                                gpInstRec->aHec[hecIdx].lastDevPhysAddr = lastPhysAddr;
                                                gpInstRec->aHec[hecIdx].status.channelState = CDC_AHEC; // Passive device registers AHECs only

                                                CdcAliveDeviceListAdd(hecIdx); // add HEC's activator to Alive List
                                                CdcIoPortsActivate(aInputPortRtIdx, outputPortRtFlg); // activate involved ports
                                            }
                                        }
                                        else if ((gpInstRec->aHec[hecIdx].status.channelState == CDC_AHEC) && (gpInstRec->deviceStatus.cdcErrorState == CDC_ERRCODE_NO_ERROR))
                                        {
                                            hecCount++; // count this compatible AHEC without deactivation
                                        }
                                        else
                                        {
                                            _printf(("[CDC_TASK_PASSIVE_SERVICE]: HEC already exists and it is incompatible state. The HEC must be removed first.\n"),0,0);
                                            gpInstRec->deviceStatus.cdcErrorState = CDC_ERRCODE_INCOMPATIBLE_STATE;
                                        }

                                }
                                else
                                {
                                    // report an error when required ports are HEC incapable
                                    gpInstRec->deviceStatus.cdcErrorState = CDC_ERRCODE_NOT_SUPPORTED_CAPABILITY;
                                }
                            }

                            lastPhysAddr  = (pCdcMessage->args[8 + 2 * i] << 8) + pCdcMessage->args[9 + 2 * i];
                        }


                        // Report activation state
                        // Only one report is sent for any number of HECs in one SetState command
                        if (isAtLeastOneDeviceBelongsToHec) // don't report, if not in declared HECs
                        {
                            // Activate eHDMI function
                            if (hecCount > 0)
                            {
                                if (gpInstRec->deviceStatus.hecFuncState < CDC_HEC_STATE_ACTIVE)
                                {
                                    if (CdcEhdmiHwEnable(true))
                                    {
                                        gpInstRec->deviceStatus.hecFuncState = CDC_HEC_STATE_ACTIVE;
                                        CdcAliveTimerSet(10, 50); // set random delay for NotifyAlive messages (10 - 50 sec)
                                    }
                                }
                            }

                            if (numberOfHecs == 1)
                            {
                                uint8_t hecFuncState = gpInstRec->deviceStatus.hecFuncState;
                                // simplified forms of report is used for single HEC activation command
                                if (gpInstRec->deviceStatus.cdcErrorState == CDC_ERRCODE_NOT_SUPPORTED_CAPABILITY)
                                {   // temporary change the device status in order to report "HEC not supported"
                                    // at one or two of the ports
                                    gpInstRec->deviceStatus.hecFuncState = CDC_HEC_NOT_SUPPORTED;
                                }
                                CdcHecReportStateSend(initiatorPhysAddr, CDC_REPORT_STATE_SHORT_MODE);
                                gpInstRec->deviceStatus.hecFuncState = hecFuncState; // restore the device status
                            }
                            else
                            {   // HEC activation fields must be sent for multi-HEC activation command
                                CdcHecReportStateSend(initiatorPhysAddr, CDC_REPORT_STATE_MULTIVHEC_MODE);
                            }
                        }

                    }

                    // Deactivation command
                    if (CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, gpInstRec->physAddr) &&
                        (pCdcMessage->args[7] == CDC_HEC_INACTIVE) && (numberOfHecs == 1))
                    {
                        TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: received SetState::Deactivate message from PA = %04x\n", initiatorPhysAddr));

                        // Remove the AHEC from the Instance Record
                        hecIdx = CdcHecIndexGet(initiatorPhysAddr, firstPhysAddr, lastPhysAddr);
                        if (hecIdx == 0xFF)
                        {
                            _printf(("[CDC_TASK_PASSIVE_SERVICE]: Can't find existing HEC descriptor to delete.\n"),0,0);
                        }
                        else
                        {
                            CdcAliveDeviceListRemove(hecIdx); // remove devices from Alive List (if possible)
                            CdcHecDescriptorRemove(hecIdx);
                            CdcHecDisableCheck(); // disable HEC hardware if no active HECs

                            // Update I/O ports status
                            // Deactivate ports that no longer participate in active HECs
                            CdcIoPortsStatusUpdate();
                        }
                        // No report is needed for deactivation command
                    }

                    break;

                case CDCOP_HEC_REQUEST_DEACTIVATION:

                    if (pCdcMessage->argCount != 9)
                    {
                        _printf(("[CDC_TASK_PASSIVE_SERVICE]: ERROR in DeactivationRequest message: wrong length!\n"),0,0);
                        break;
                    }

                    // Only Activator of deactivated AHEC must respond to the message
                    targetPhysAddr = (pCdcMessage->args[3] << 8) + pCdcMessage->args[4]; // activator's physical address
                    firstPhysAddr = (pCdcMessage->args[5] << 8) + pCdcMessage->args[6];
                    lastPhysAddr  = (pCdcMessage->args[7] << 8) + pCdcMessage->args[8];

                    // reply with SetState message only if the device is activator of the registered AHEC
                    // and initiating device belongs to the HEC it requests to deactivate
                    hecIdx = CdcHecIndexGet(targetPhysAddr, firstPhysAddr, lastPhysAddr);
                    if ((hecIdx != 0xFF) && (gpInstRec->aHec[hecIdx].status.channelState == CDC_AHEC) &&
                         CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, initiatorPhysAddr))
                    {
                        TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: received RequestDeactivation message from PA = %04x\n", initiatorPhysAddr));
                        SI_CdcDeviceDisconnect(false, targetPhysAddr, firstPhysAddr, lastPhysAddr);
                    }
                    break;

                case CDCOP_HEC_NOTIFY_ALIVE:
                    // find out if the initiator is in the list of monitored devices
                    // reset Alive Time counter to 140 sec for the initiator
                    if(CdcAliveDeviceListTimeReset(initiatorPhysAddr) == true) // device has been found in AliveDevice List
                    {
                        TPI_CDC_PRINT( ("[CDC_TASK_PASSIVE_SERVICE]: received NotifyAlive message from PA = %04x\n", initiatorPhysAddr));
                    }
                    break;

                case CDCOP_HEC_DISCOVER:
                    // reply with ReportState message
                    if (gpInstRec->deviceStatus.hecFuncState != CDC_HEC_FUNC_NOT_SUPPORTED)
                    {
                        TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: received Discover message from PA = %04x\n", initiatorPhysAddr));
                        CdcHecReportStateSend(initiatorPhysAddr, CDC_REPORT_STATE_DISCOVER_MODE);
                    }
                    break;

                case CDCOP_HPD_SET_STATE:

                    if (pCdcMessage->argCount != 4)
                    {
                        _printf(("[CDC_TASK_PASSIVE_SERVICE]: ERROR in HpdSetState message: wrong length!\n"),0,0);
                        break;
                    }

                    // Source replies with HPD ReportState message if the message came from the port of adjacent Sink
                    // that is connected to this Source
                    inputPortIndex = (pCdcMessage->args[3] >> 4) & 0x0F; // index of an input port that sent the HPD SetState command
                    if (CdcIsLinkedDirectly(initiatorPhysAddr, gpInstRec->physAddr, inputPortIndex))
                    {
                        TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: received HPD SetState message from PA = %04x\n", initiatorPhysAddr));
                        // set HPD state of the output port
                        gpInstRec->ports.output.hpdState = pCdcMessage->args[3] & 0x0F;
                        CdcHpdReportStateSend();
                        cdcFeedbackMsg = CDC_FB_MSG_HPD_STATE_CHANGED;
                    }
                    break;

                case CDCOP_HPD_REPORT_STATE:
                    // Make sure the HPD ReportState comes from adjacent Source over currently selected input port
                    // during HPD Signalling Task execution on the waiting stage
                    if ((gpInstRec->cdcTaskFlags & CDC_TASK_HPD_SIGNAL) && (gpInstRec->cdcTaskState == CDC_TASK_HPD_SIGNAL_STATE_RESPONSE_WAIT))
                    {
                        if (CdcIsLinkedDirectly(gpInstRec->physAddr, initiatorPhysAddr, gpInstRec->currentPortIdx))
                        {
                            TPI_CDC_PRINT(("[CDC_TASK_PASSIVE_SERVICE]: received HPD ReportState message from PA = %04x\n", initiatorPhysAddr));
                            gpInstRec->cdcTaskState = CDC_TASK_HPD_SIGNAL_STATE_REPORT_RESULTS; // break waiting
                        }
                    }
                    break;
            }
        }
        else
        {
            _printf(("[CDC_TASK_PASSIVE_SERVICE]: received a message with non-CDC CEC opcode or with non-broadcast destination\n"),0,0);
            cdcFeedbackMsg = CDC_FB_MSG_ERR_NONCDC_CMD;
        }

    }
    else
    {
        if ((gpInstRec->numberOfAliveDev > 0) && (gpInstRec->deviceStatus.hecFuncState >= CDC_HEC_STATE_ACTIVE))
        {
            // if it's time to send alive notification, do it also
            // if participating in AHECs (either as activator or passive device),
            // and if Alive Device list has at least one entry
            if (CdcIsTimeCounterExpired(&gpInstRec->aliveNtfDelayCount)) // if timer expired
            {
                CdcHecNotifyAliveSend();
                CdcAliveTimerSet(60, 62); // send consecutive notification in 60-65 seconds
                _printf(("[CDC_TASK_PASSIVE_SERVICE]: sent AliveNotify message.\n"),0,0);
            }
        }

    }

    return cdcFeedbackMsg;
}

//------------------------------------------------------------------------------
// Function:    SI_CdcTaskProcess
// Description: Task dispatching function. Shall be called periodically.
//              Using of SI_CdcNextInvocationTimeGet() helps to figure out
//              exact recall time that can vary depending on current task status.
//              Alternatively, simple periodic polling would work as well;
//              in the case of polling, CDC_MIN_RECALL_TIME_MS is a suggested
//              recall time.
//
// Parameters:  pCdcMessage   - pointer to a pending CDC message or 0,
//                              if there is no message awaiting
//              sysTimerCount - current value of the system time counter
//                              (in milliseconds)
//
// Returns:     Feedback message to the caller (Task status or error reports)
//------------------------------------------------------------------------------

uint8_t SI_CdcTaskProcess(uint16_t sysTimerCountMs, SI_CpiData_t *pCdcMessage)
{
    uint8_t  cdcFeedbackMsg = CDC_FB_MSG_NONE;
    uint16_t cdcTaskFlagsMasked;
    uint16_t devIdx;


    // Keep all tasks frozen if the Base task is off
    if (gpInstRec->cdcTaskFlags & CDC_TASK_BASE_SERVICE)
    {
        // Update time reading variables
        gpInstRec->sysTimerCountPrvMs = gpInstRec->sysTimerCountCurMs; // memorise previous timer reading
        gpInstRec->sysTimerCountCurMs = sysTimerCountMs; // set current timer reading

        if (((gpInstRec->cdcTaskFlags & CDC_TASK_MASK_ALL_BUT_BASE) == 0) && (pCdcMessage == 0))
        {
            // Initialize the recall time counter by maximum possible value
            // in order to ensure rare recalls of the task handler when tasks are inactive
            CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MAX_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);
        }
        else
        {   // Ensure fast recall if there are active tasks or pending messages as they could
            // require immediate subsequent action (e.g., launching of active tasks)
            // Actual recall time will be corrected by task internal timer updates, if any
            CdcTimeCounterSet(CDC_TIME_MS2TCK(CDC_MIN_RECALL_TIME_MS), &gpInstRec->cdcRecallTimeCount);
        }

        if ((gpInstRec->numberOfAliveDev > 0) && (gpInstRec->deviceStatus.hecFuncState >= CDC_HEC_STATE_ACTIVE))
        {
            CdcTimeCounterUpdate(&gpInstRec->aliveNtfDelayCount);
        }

        // Update AliveDevice List times and react on timeouts
        if ((devIdx = CdcAliveDeviceListTimeUpdate()) != 0xFFFF) // if timeout detected for some device
        {
            // Disconnect all HECs that include the failed device
            // Make sure other tasks are inactive, leave the timout pending otherwise
            CdcLostDeviceDisconnect(gpInstRec->aAliveDevice[devIdx].physAddr);
        }

        // Update the Discovery repetition timer
        CdcTimeCounterUpdate(&gpInstRec->cdcDiscoveryTimeCount);

        // Do one task at a time. Tasks shall never be running concurrently except the Base Service that
        // can run in background of another task, not corrupting its context.
        // Base Service has highest priority if a CDC message is pending and lowest priority otherwise

        cdcTaskFlagsMasked = (gpInstRec->cdcTaskFlags & gpInstRec->cdcTaskMask);

        if (pCdcMessage != 0) // CDC message is pending
        {
            cdcFeedbackMsg = CdcTaskBase(pCdcMessage);
        }
        else
        {
            if (cdcTaskFlagsMasked & CDC_TASK_HOSTS_SEARCH)
            {
                cdcFeedbackMsg = CdcTaskDiscover();
            }
            else if (cdcTaskFlagsMasked & CDC_TASK_CONNECT)
            {
                cdcFeedbackMsg = CdcTaskConnect();
            }
            else if (cdcTaskFlagsMasked & CDC_TASK_DISCONNECT)
            {
                cdcFeedbackMsg = CdcTaskDisconnect();
            }

#if (IS_CDC_REDUCED != 1)
            else if (cdcTaskFlagsMasked & CDC_TASK_CONNECT_ADJACENT)
            {
                cdcFeedbackMsg = CdcTaskConnectAdjacent();
            }
            else if (cdcTaskFlagsMasked & CDC_TASK_DISCONNECT_ALL)
            {
                cdcFeedbackMsg = CdcTaskDisconnectAll();
            }
            else if (cdcTaskFlagsMasked & CDC_TASK_DISCONNECT_LOST)
            {
                cdcFeedbackMsg = CdcTaskDisconnectLost();
            }
#else
            else if (cdcTaskFlagsMasked & (CDC_TASK_DISCONNECT_ALL | CDC_TASK_DISCONNECT_LOST | CDC_TASK_CONNECT_ADJACENT))
            {
                cdcFeedbackMsg = CDC_FB_MSG_ERROR;
            }
#endif
            else if (cdcTaskFlagsMasked & CDC_TASK_HPD_SIGNAL)
            {
#if (IS_RX == 1)
                cdcFeedbackMsg = CdcTaskHpdSignal();
#else
                cdcFeedbackMsg = CDC_FB_MSG_ERROR;
#endif
            }
            else if (cdcTaskFlagsMasked & CDC_TASK_HPD_CAPAB_VERIF)
            {
                cdcFeedbackMsg = CdcTaskHpdCapabilityVerification();
            }
            // Base Task can't be masked and can run in background of the other active task if incoming CDC message is pending
            else
            {
                cdcFeedbackMsg = CdcTaskBase(0);
            }
        }

    }

    return cdcFeedbackMsg;
}


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcNextInvocationTimeGet
// Description: Returns longest possible time delay before the next invocation
//              of the SI_CdcTaskProcess() function.
//              This function can be used to set a countdown timer and call
//              the SI_CdcTaskProcess() on the timer's expiration event unless
//              new CDC message is pending
//
// Parameters:  sysTimerCount - current value of the system time counter
//                              (in milliseconds)
//
// Returns:     time in milliseconds
//------------------------------------------------------------------------------

uint16_t SI_CdcNextInvocationTimeGet(uint16_t sysTimerCountMs)
{
    uint32_t totalDelayMs;
    uint16_t invDelayMs;

    if (CdcIsTimeCounterExpired(&gpInstRec->cdcRecallTimeCount))
    {
        totalDelayMs = 0;
    }
    else
    {
        totalDelayMs =  (uint32_t) gpInstRec->cdcRecallTimeCount.tRov * 0x10000 /* 2^16 */ +
                        (uint32_t) gpInstRec->cdcRecallTimeCount.tRes * CDC_TIME_TCK_MS - (uint32_t) sysTimerCountMs;
    }

    // Apply limits to recall delay
    if (totalDelayMs < CDC_MIN_RECALL_TIME_MS)
    {
        invDelayMs = CDC_MIN_RECALL_TIME_MS;
    }
    else if (totalDelayMs > CDC_MAX_RECALL_TIME_MS)
    {
        invDelayMs = CDC_MAX_RECALL_TIME_MS;
    }
    else
    {
        invDelayMs = (uint16_t) totalDelayMs;
    }

    TPI_CDC_PRINT(("SI_CdcNextInvocationTimeGet(): -- recall in -- %d ms\n", invDelayMs));

    return invDelayMs;
}
#endif


//------------------------------------------------------------------------------
// Function:    SI_CdcSrvStart
// Description: Launches CDC Active and Passive Services.
//              Active CDC service maintains operation of all activated by this
//              device HECs. Passive CDC Service maintains responsiveness to
//              CDC requests and sending Alive messages (when participating in
//              AHECs)
//
// Returns:     true, if the task can't be scheduled, false - otherwise
//------------------------------------------------------------------------------

bool_t SI_CdcSrvStart(void)
{
    bool_t errStatus = false;

    // check if Passive or Active services can be started
    if (gpInstRec->deviceStatus.hecFuncState != CDC_HEC_FUNC_NOT_SUPPORTED)
    {
        gpInstRec->cdcTaskFlags |= CDC_TASK_BASE_SERVICE;
    }
    else
    {
        errStatus = true;
        _printf(("WARNING in SI_CdcSrvStart(): Services can't be started.\n"),0,0);
    }

    return errStatus;
}


//------------------------------------------------------------------------------
// Function:    SI_CdcSrvStop
// Description: Stops CDC Active and Passive Services
//
//------------------------------------------------------------------------------

void SI_CdcSrvStop(void)
{
    gpInstRec->cdcTaskFlags &= ~CDC_TASK_BASE_SERVICE;
}

#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    CdcTaskStatusGet
// Description: Retrieves status of a task
//
// Returns:     true if the task is active, false - otherwise
//------------------------------------------------------------------------------

bool_t CdcTaskStatusGet(uint16_t cdcTaskId)
{
    return (gpInstRec->cdcTaskFlags & cdcTaskId);
}
#endif

//------------------------------------------------------------------------------
// Function:    SI_CdcSrvHostsFind
// Description: Discovery of all devices having HEC capability and supports Host
//              or External network function throughout entire HDMI network.
//              Discovery process shall not be conducted more frequently than
//              once in 3 minutes. This rule is enforced in this function; it
//              will return non-zero, if called before 3 minutes since the last call.
//
// Returns:     0        - if the search task is scheduled successfully,
//              [1-0xFE] - the number of seconds before next permitted Discovery,
//              0xFF     - other error
//
//------------------------------------------------------------------------------

uint8_t SI_CdcSrvHostsFind(void)
{
    uint16_t timeLeftSec = 0;

    if (CdcIsTimeCounterExpired(&gpInstRec->cdcDiscoveryTimeCount))
    {
        // schedule immediate start of the search
        gpInstRec->cdcTaskFlags |= CDC_TASK_HOSTS_SEARCH;
    }
    else
    {
        timeLeftSec = CdcTimeCounterLeftSecGet(&gpInstRec->cdcDiscoveryTimeCount);
        TPI_CDC_PRINT(("WARNING: Discovery function will be available after %d seconds\n", timeLeftSec));
    }

    if (timeLeftSec < 0xFF)
    {
        return (uint8_t) timeLeftSec;
    }
    else
    {
        return 0xFF; // error code
    }
}


//------------------------------------------------------------------------------
// Function:    SI_CdcDeviceConnect
// Description: Establishes connection to a specified device or group of devices.
//              This function should be used after discovery process, so that the
//              cdcFoundHostsList[] list isn't empty. It must be used by Activator
//              of the HEC
//
// Parameters:  isConnectEntireList - if true, will connect all devices from the
//                                    cdcFoundHostsList[] that have selection bit
//                                    (hostInfo.isSelected) asserted. The lastPhysAddr
//                                    parameter is ignored in this case.
//                                    if false, only one device having specified
//                                    physical address will be connected.
//              firstPhysAddr       - physical address of a first HEC device to be
//                                    connected to the last device or devices in
//                                    the cdcFoundHostsList[]. Typically,
//                                    the first device is an activator of the HEC
//              lastPhysAddr        - physical address of a second device to be
//                                    connected to the first device
//
// Returns:     true, if the connect task can't be scheduled, false - otherwise
//------------------------------------------------------------------------------

bool_t SI_CdcDeviceConnect(bool_t isConnectEntireList, uint16_t firstPhysAddr, uint16_t lastPhysAddr)
{
    uint8_t i;
    uint8_t idx;
    bool_t  errStatus = false;

    // check if this device is HEC capable
    if (gpInstRec->deviceStatus.hecFuncState == CDC_HEC_FUNC_NOT_SUPPORTED)
    {
        _printf(("ERROR in SI_CdcDeviceConnect(): Device is HEC incapable and can't initiate HEC communications\n"),0,0);
        errStatus = true;
    }
    else
    {
        // check if the found device list has the device entry(ies)
        if ((isConnectEntireList && gpInstRec->numberOfFoundHosts > 0) || (!isConnectEntireList))
        {
            // create new HEC descriptor(s) and label the HEC as PHEC
            if (isConnectEntireList)
            {
                gpInstRec->connectSettings.isMultHecMode = true;  // use multi-HEC activation mode for more than 1 device

                for (i = 0; i < gpInstRec->numberOfFoundHosts; i++)
                {
                    // connect only selected devices having unequal PAs
                    if (gpInstRec->cdcFoundHostsList[i].hostInfo.isSelected && (firstPhysAddr != gpInstRec->cdcFoundHostsList[i].physAddr))
                    {
                        // check if activator itself can be a part of the new HEC, if it belongs to the HEC
                        if (CdcIsCapableForHec(gpInstRec->physAddr, gpInstRec->cdcFoundHostsList[i].physAddr, false))
                        {
                            if (CdcHecIndexGet(gpInstRec->physAddr, firstPhysAddr, gpInstRec->cdcFoundHostsList[i].physAddr) == 0xFF) // make sure that new HEC is unique
                            {
                                if ((idx = CdcHecDescriptorAdd(true)) != 0xFF)
                                {
                                    gpInstRec->aHec[idx].activatorPhysAddr   = gpInstRec->physAddr;
                                    gpInstRec->aHec[idx].firstDevPhysAddr    = firstPhysAddr;
                                    gpInstRec->aHec[idx].lastDevPhysAddr     = gpInstRec->cdcFoundHostsList[i].physAddr;
                                    gpInstRec->aHec[idx].status.channelState = CDC_PHEC;
                                }
                                else
                                {
                                    errStatus = true;
                                    break;
                                }
                            }
                            // if HEC isn't unique, don't rise error flag because connection should be already established
                        }
                        else
                        {
                            _printf(("WARNING in SI_CdcDeviceConnect(): Device is incapable to be an activator for a HEC\n"),0,0);
                        }
                    }
                }
            }
            else // one HEC
            {
                gpInstRec->connectSettings.isMultHecMode = false;  // use single-HEC activation mode for 1 device case

                // check if activator itself can be a part of the new HEC, if it belongs to the HEC
                if (CdcIsCapableForHec(firstPhysAddr, lastPhysAddr, false))
                {
                    if (CdcHecIndexGet(gpInstRec->physAddr, firstPhysAddr, lastPhysAddr) == 0xFF) // make sure that new HEC is unique
                    {
                        if ((idx = CdcHecDescriptorAdd(true)) != 0xFF)
                        {
                            gpInstRec->aHec[idx].activatorPhysAddr   = gpInstRec->physAddr;
                            gpInstRec->aHec[idx].firstDevPhysAddr    = firstPhysAddr;
                            gpInstRec->aHec[idx].lastDevPhysAddr     = lastPhysAddr;
                            gpInstRec->aHec[idx].status.channelState = CDC_PHEC;
                        }
                        else
                        {
                            errStatus = true;
                        }
                    }
                }
                else
                {
                    _printf(("WARNING in SI_CdcDeviceConnect(): Device is incapable to be an activator for a HEC\n"),0,0);
                }

            }

            // schedule immediate start of connection task
            if (errStatus == false)
            {
                gpInstRec->cdcTaskFlags |= CDC_TASK_CONNECT;
            }
        }
        else
        {
            errStatus = true; // can't run the task
        }
    }

    return errStatus;
}


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcDeviceAdjacentConnect
// Description: Establishes connection to a specified device using "HEC control
//              to Adjacent Device" method.
//
// Parameters:  adjacentPhysAddr  - physical address of an adjacent device
//
// Returns:     true, if the connect task can't be scheduled, false - otherwise
//------------------------------------------------------------------------------

bool_t SI_CdcDeviceAdjacentConnect(uint16_t adjacentPhysAddr)
{
    uint8_t idx;
    bool_t  errStatus = false;

    // check if this device is HEC capable
    if (gpInstRec->deviceStatus.hecFuncState == CDC_HEC_FUNC_NOT_SUPPORTED)
    {
        _printf(("ERROR in SI_CdcDeviceAdjacentConnect(): Device is HEC incapable and can't initiate HEC communications\n"),0,0);
        errStatus = true;
    }
    else
    {
        // check if activator itself can be a part of the new HEC (strict check)
        if (CdcIsCapableForHec(gpInstRec->physAddr, adjacentPhysAddr, true))
        {
            if (CdcHecIndexGet(gpInstRec->physAddr, gpInstRec->physAddr, adjacentPhysAddr) == 0xFF) // make sure that new HEC is unique
            {
                if ((idx = CdcHecDescriptorAdd(true)) != 0xFF)
                {
                    gpInstRec->aHec[idx].activatorPhysAddr   = gpInstRec->physAddr;
                    gpInstRec->aHec[idx].firstDevPhysAddr    = gpInstRec->physAddr;
                    gpInstRec->aHec[idx].lastDevPhysAddr     = adjacentPhysAddr;
                    gpInstRec->aHec[idx].status.channelState = CDC_PHEC;
                    gpInstRec->aHec[idx].status.isAdjacent   = true;

                    gpInstRec->currentHecIdx = idx;
                    gpInstRec->cdcTaskFlags |= CDC_TASK_CONNECT_ADJACENT; // schedule immediate start of connection task
                }
                else
                {
                    errStatus = true;
                }
            }
            else
            {
                _printf(("WARNING in SI_CdcDeviceAdjacentConnect(): HEC already exist.\n"),0,0);

            }
        }
        else
        {
             _printf(("WARNING in SI_CdcDeviceAdjacentConnect(): Device is incapable to be an activator for a HEC\n"),0,0);
        }

     }


    return errStatus;
}
#endif


//------------------------------------------------------------------------------
// Function:    SI_CdcDeviceDisconnect
// Description: Terminates connection to a specified device or all connected devices.
//              This function can be used by Activator of the HEC
//
// Parameters:  isDisconnectAll    - if true, will close all active HECs.
//                                   The second & third
//                                   parameters are ignored in this case.
//                                   if false, only one device having specified
//                                   physical address will be disconnected.
//              actPhysAddr        - physical address of HEC's activator
//              firstPhysAddr      - physical address of a first HEC device
//              lastPhysAddr       - physical address of a second device in HEC
//
// Returns:     true, if the disconnect task can't be scheduled, false - otherwise
//------------------------------------------------------------------------------

bool_t SI_CdcDeviceDisconnect(bool_t isDisconnectAll, uint16_t actPhysAddr, uint16_t firstPhysAddr, uint16_t lastPhysAddr)
{
    bool_t  errStatus = false;

    if (isDisconnectAll)
    {
#if (IS_CDC_REDUCED != 1)
        // schedule immediate start of disconnection task
        gpInstRec->cdcTaskFlags |= CDC_TASK_DISCONNECT_ALL;
#else
        errStatus = true;
#endif
    }
    else // disconnect only one device
    {
        gpInstRec->currentHecIdx = CdcHecIndexGet(actPhysAddr, firstPhysAddr, lastPhysAddr);

        if (gpInstRec->currentHecIdx < 0xFF)
        {
            if (gpInstRec->aHec[gpInstRec->currentHecIdx].status.channelState > CDC_PHEC)
            {
                // schedule immediate start of disconnection task
                gpInstRec->cdcTaskFlags |= CDC_TASK_DISCONNECT;
            }
            else
            {
                errStatus = true; // HEC isn't active
            }
        }
        else
        {
            errStatus = true; // the AHEC wasn't found
        }
    }

    return errStatus;
}


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcDeviceAdjacentDisconnect
// Description: Terminates connection to a specified adjacent device.
//              This function should be used by Activator of the HEC, however,
//              passive device can reset it's connection to the adjacent
//              device (activator) when it has been physically disconnected
//
// Parameters:  adjacentPhysAddr - physical address of an adjacent device
//
// Returns:     true, if the disconnect task can't be scheduled, false - otherwise
//------------------------------------------------------------------------------

bool_t SI_CdcDeviceAdjacentDisconnect(uint16_t adjacentPhysAddr)
{
    bool_t  errStatus = false;

    gpInstRec->currentHecIdx = CdcHecIndexGet(gpInstRec->physAddr, gpInstRec->physAddr, adjacentPhysAddr);

    if (gpInstRec->currentHecIdx < 0xFF)
    {
        if ((gpInstRec->aHec[gpInstRec->currentHecIdx].status.isAdjacent == true) &&
            (gpInstRec->aHec[gpInstRec->currentHecIdx].status.channelState > CDC_PHEC))
        {
            // disconnection of adjacent device doesn't require state reporting,
            // therefore, non-task approach is relevant. Just send disconnection CDC command
            CdcHecSetStateAdjacentSend(CDC_HEC_INACTIVE, gpInstRec->currentHecIdx);

            CdcHecDescriptorRemove(gpInstRec->currentHecIdx); // release resources for HEC
            CdcHecDisableCheck(); // disable HEC hardware if no active HECs
            CdcIoPortsStatusUpdate(); // deactivate ports involved in the deactivated HEC
        }
        else
        {
            errStatus = true; // HEC isn't active or activated by control to adjacent device
        }
    }
    else
    {   // check if Adjacent Device is the activator of a HEC
        gpInstRec->currentHecIdx = CdcHecIndexGet(adjacentPhysAddr, gpInstRec->physAddr, adjacentPhysAddr);

        if (gpInstRec->currentHecIdx < 0xFF)
        {
            if (gpInstRec->aHec[gpInstRec->currentHecIdx].status.isAdjacent == true)
            {
                // force the HEC disconnection
                CdcHecDescriptorRemove(gpInstRec->currentHecIdx); // release resources for HEC
                CdcHecDisableCheck(); // disable HEC hardware if no active HECs
                CdcIoPortsStatusUpdate(); // deactivate ports involved in the deactivated HEC
            }
            else
            {
                errStatus = true; // AHEC isn't activated by control to adjacent device
            }
        }
        else
        {
            errStatus = true; // the AHEC wasn't found
        }
    }

    return errStatus;
}
#endif


#if (IS_CDC_REDUCED != 1)
//------------------------------------------------------------------------------
// Function:    CdcLostDeviceDisconnect
// Description: Deactivates all AHECs that include a device having
//              given physical address.
//
// Parameters:  physAddr - physical address of the failed device
//
//------------------------------------------------------------------------------

static void CdcLostDeviceDisconnect(uint16_t physAddr)
{
    if ((gpInstRec->cdcTaskFlags & CDC_TASK_MASK_ALL_BUT_BASE) == 0)
    {
            // disconnect only one device (other timeouts will be pending)
            gpInstRec->aliveDevPhysAddr = physAddr;
            // schedule immediate start of disconnection task
            gpInstRec->cdcTaskFlags |= CDC_TASK_DISCONNECT_LOST;
    }
}
#else
static void CdcLostDeviceDisconnect(uint16_t physAddr) { physAddr = 0;}
#endif


//------------------------------------------------------------------------------
// Function:    SI_CdcPortIsActive
// Description: Determines if the specified port is an active CDC port.
//
// Parameters:  isOutput       - true, if output port is addressed
//              inputPortIndex - input port index (zero-based)
//              isEnabled      - boolean flag
//
// Returns:     true if a CDC channel is attached to the specified HDMI port
//              and is active.
//------------------------------------------------------------------------------

bool_t SI_CdcPortIsActive(const bool_t isOutput, const uint8_t portIndex)
{
    bool_t    isActive = false;

    if (( isOutput == true) && (gpInstRec->ports.numberOfOutputs != 0))
    {
        isActive = (gpInstRec->ports.output.hecSupport == true ) &&
                   (gpInstRec->ports.output.hecActive == true );
    }
    else if ((isOutput == false) && (gpInstRec->ports.numberOfInputs > portIndex)) // inputs
    {
        isActive = (gpInstRec->ports.input[portIndex].hecSupport == true ) &&
                   (gpInstRec->ports.input[portIndex].hecActive == true );
    }

    return isActive;
}


#if (IS_RX == 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcHpdSetState
// Description: Sets new HPD state to selected port and sends an HPD State
//              message to HDMI Source device.
//              This function shall be used by an HDMI Sink device
//
// Parameters:  inputPortIndex - zero based input port number
//              hpdState       - HPD state of the input port
//
// Returns:     true, if the HPD signalling task can't be scheduled, false - otherwise
//------------------------------------------------------------------------------

bool_t SI_CdcHpdSetState(uint8_t inputPortIndex, CdcHpdState_t hpdState)
{
    bool_t  errStatus = true;

    if (inputPortIndex < gpInstRec->ports.numberOfInputs)
    {
        if (gpInstRec->ports.input[inputPortIndex].hecSupport != CDC_HEC_NOT_SUPPORTED)
        {
            gpInstRec->currentPortIdx = inputPortIndex;
            gpInstRec->ports.input[inputPortIndex].hpdState = hpdState;

            // schedule immediate start of HPD signalling task
            gpInstRec->cdcTaskFlags |= CDC_TASK_HPD_SIGNAL;
            errStatus = false;
        }
    }

    return errStatus;
}
#endif


#if (IS_TX == 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcHpdStateGet
// Description: Gets current HPD state of a Source device.
//              This function shall be used by an HDMI Source device
//
// Returns:     HPD state code or CDC_HPD_UNKNOWN_STATE, if output is in
//              error state or doesn't exist (root device)
//------------------------------------------------------------------------------

CdcHpdState_t SI_CdcHpdStateGet(void)
{
    CdcHpdState_t hpdState = CDC_HPD_UNKNOWN_STATE;

    if ((gpInstRec->ports.numberOfOutputs == 1) &&
        ( (gpInstRec->ports.output.hpdErrorState == CDC_HPD_ERRCODE_NO_ERROR) ||
          (gpInstRec->ports.output.hpdErrorState == CDC_HPD_ERRCODE_NO_ERROR_NO_VIDEO)  ))
    {
        hpdState = gpInstRec->ports.output.hpdState;
    }

    return hpdState;
}
#endif


#if (IS_RX == 1)
//------------------------------------------------------------------------------
// Function:    SI_CdcHpdCapabilityInquire
// Description: Launches verification task that checks if selected adjecent
//              device supports HPD-over-CDC messaging.
//              This function shall be used by an HDMI Sink device
//
// Parameters:  inputPortIndex - zero based input port number
//
// Returns:     true, if the HPD capability verification task can't be scheduled,
//              false - otherwise
//------------------------------------------------------------------------------

bool_t SI_CdcHpdCapabilityInquire(uint8_t inputPortIndex)
{
    bool_t  errStatus = true;

    if (inputPortIndex < gpInstRec->ports.numberOfInputs)
    {
        if (gpInstRec->ports.input[inputPortIndex].hecSupport != CDC_HEC_NOT_SUPPORTED)
        {
            gpInstRec->currentPortIdx = inputPortIndex;

            // schedule immediate start of HPD capability verification task
            gpInstRec->cdcTaskFlags |= CDC_TASK_HPD_CAPAB_VERIF;
            errStatus = false;
        }
    }

    return errStatus;
}

#endif

#else

//void     SI_CdcDeviceSelect(uint16_t deviceIndex);
//CdcInstanceRecord_t *SI_CdcDeviceInstanceGet(void);
//
//void     SI_CdcConfig(uint16_t physAddr, CdcIoConfig_t *pIoPorts,
//                      bool_t hecCapability, bool_t hostCapability, bool_t extNetCapability);
//void     SI_CdcDeviceHecSupEnable(const bool_t isEnabled);
//void     SI_CdcDeviceHostStateSet(const CdcHostFuncState_t state);
//void     SI_CdcDeviceEncStateSet(const CdcEncFuncState_t state);
//void SI_CdcPortHecSupEnable(const bool_t isOutput, const uint8_t inputPortIndex, const bool_t isEnabled)

//
//
//bool_t   SI_CdcSrvHostsFind(void);
//bool_t   SI_CdcDeviceConnect(bool_t isConnectEntireList, uint16_t firstPhysAddr, uint16_t lastPhysAddr);
//bool_t   SI_CdcDeviceAdjacentConnect(uint16_t adjacentPhysAddr);
//bool_t   SI_CdcDeviceDisconnect(bool_t isDisconnectAll, uint16_t actPhysAddr, uint16_t firstPhysAddr, uint16_t lastPhysAddr);
//bool_t   SI_CdcDeviceAdjacentDisconnect(uint16_t adjacentPhysAddr);
//
//
//bool_t   SI_CdcSrvStart(void);
//void     SI_CdcSrvStop(void);
//
bool_t   SI_CdcHpdSetState( uint8_t port, CdcHpdState_t hpdState ) { port = hpdState; return( false ); }
//bool_t   SI_CdcHpdCapabilityInquire(uint8_t inputPortIndex);
//CdcHpdState_t SI_CdcHpdStateGet(void);
//
void     SI_CdcTaskInit( void ) {}
//uint8_t  SI_CdcTaskProcess(uint16_t sysTimerCountMs, SI_CpiData_t *pCdcMessage);
//uint16_t SI_CdcNextInvocationTimeGet(uint16_t sysTimerCountMs);
//
bool_t SI_CdcPortIsActive(const bool_t isOutput, const uint8_t portIndex) { return( portIndex != portIndex & isOutput ); }

#endif

