//***************************************************************************
//!file     si_cdc_heac.c
//!brief    Low level CDC API
//
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

//#include <stdio.h>
//#include <string.h>
//#include "gpio.h"
#include "dev.h"
#include "defs.h"

#include "si_apiCpi.h"
#include "si_cdc_heac.h"
#include "si_apiCEC.h"
#include "si_apiConfigure.h"

#if (IS_CEC == 1)
#include "si_apiCEC.h"
#endif

#if (IS_CDC == 1)

//------------------------------------------------------------------------------
// Global Data
//------------------------------------------------------------------------------

extern CdcInstanceRecord_t   *gpInstRec;    // global pointer to an Instance Record
                                            // It has global scope to minimize
                                            // function stack burden

//------------------------------------------------------------------------------
// Service Functions
//------------------------------------------------------------------------------

// Local function prototypes
static bool_t CdcPhysAddrToAliveListInsert(const uint16_t physAddr);
static bool_t CdcPhysAddrFromAliveListDelete(const uint16_t physAddrToDel, const uint8_t hecIdx);
static bool_t CdcIsLinkedDirectlyInPortGet(const uint16_t sinkPhysAddr, const uint16_t sourcePhysAddr, uint8_t *pinputPortIndex);



//------------------------------------------------------------------------------
// Function:    mRand
// Description: Pseudo-random numbers generator with uniform distribution.
//              This implementation is based on George Marsaglia's MWC algorithm
//
// Parameter:   seed  - non-zero value is initial value that is expected to be
//                      pure random or at least time dependent,
//                      if seed == 0, mRand generates next random output
//
// Returns:     pseudo-random, quasi-uniform number in the range [0;(2^32)-1]
//------------------------------------------------------------------------------

uint32_t mRand(uint32_t seed)
{
    static uint32_t mz = 0x3F218942; // default initial values
    static uint32_t mw = 0x19A46E81;

    if (seed != 0)
    {
        mz = seed >> 16 ;
        mw = seed & 0xFFFF;
    }

    mz = 0x9069 * (mz & 0xFFFF) + (mz >> 16);
    mw = 0x4650 * (mw & 0xFFFF) + (mw >> 16);

    return (mz << 16) + mw;
}


//------------------------------------------------------------------------------
// Function:    CdcHecInstanceRecordReset
// Description: Initializes current Instance Record
//
//------------------------------------------------------------------------------

void CdcHecInstanceRecordReset(void)
{
//    memset(gpInstRec, 0, sizeof(CdcInstanceRecord_t)); // erase all data

    // mark entire array as vacant
//    memset(&gpInstRec->aAliveDevice[0], 0xFF, sizeof(gpInstRec->aAliveDevice));
}

//------------------------------------------------------------------------------
// Function:    CdcHecDescriptorAdd
// Description: Creates new HEC record in the Instance Record
// Parameter:   isActivator - indicates if the device is going to be an activator
//                            for the HEC
// Returns:     index of newly created HEC, 0xFF - error
//------------------------------------------------------------------------------

uint8_t CdcHecDescriptorAdd(const bool_t isActivator)
{
    uint8_t i = 0xFF;
    bool_t isFound;
//    CdcHecDescriptor_t *pNewHec = NULL;
	CdcHecDescriptor_t *pNewHec = 0; 		// JYP

    if (gpInstRec->numberOfHecs < CDC_NUMBER_OF_HECS_MAX)
    {
        // Find first vacant HEC record
        isFound = false;
        for (i = 0; i < CDC_NUMBER_OF_HECS_MAX; i++)
        {
            if (gpInstRec->aHec[i].status.isInUse == false)
            {
                pNewHec = &gpInstRec->aHec[i];
                pNewHec->status.isInUse = true;
                pNewHec->status.isActivator = isActivator;
                gpInstRec->numberOfHecs++;
                isFound = true;
                break;
            }
        }

        if (isFound == false)
        {
            //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHecDescriptorAdd(): All HEC records are in use\n"));
            return 0xFF;
        }
    }
    else
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHecDescriptorAdd(): Maximum number of HECs is exceeded\n"));
        return 0xFF;
    }

    return i;
}


//------------------------------------------------------------------------------
// Function:    CdcHecDescriptorRemove
// Description: Deletes HEC record in the Instance Record;
//              frees memory for activator's HEC path
// Parameters:  hecIdx - index of a HEC to dismiss
// Returns:     false - success, true - error
//------------------------------------------------------------------------------

bool_t CdcHecDescriptorRemove(const uint8_t hecIdx)
{
    if ((gpInstRec->numberOfHecs > 0) && (gpInstRec->aHec[hecIdx].status.isInUse == true))
    {
        // Release the HEC descriptor
        gpInstRec->aHec[hecIdx].status.isInUse = false;
        gpInstRec->numberOfHecs--;

        // Cleanup other fields
        gpInstRec->aHec[hecIdx].status.isActivator  = false;
        gpInstRec->aHec[hecIdx].status.isAdjacent   = false;
        gpInstRec->aHec[hecIdx].status.channelState = CDC_PHEC;
        gpInstRec->aHec[hecIdx].activatorPhysAddr   = 0;
        gpInstRec->aHec[hecIdx].firstDevPhysAddr    = 0;
        gpInstRec->aHec[hecIdx].lastDevPhysAddr     = 0;
    }
    else
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHecDescriptorRemove(): The HEC descriptor is not in use\n"));
        return true;
    }

    return false;
}


//------------------------------------------------------------------------------
// Function:    CdcHecIndexGet
// Description: Searches HEC records in order to find an index of a HEC
//              having given first and last physical addresses
// Parameters:  actPhysAddr   - physical address of HEC's activator
//              firstPhysAddr - first physical address of a HEC
//              lastPhysAddr  - last physical address of a HEC
// Returns:     index of found HEC, 0xFF - if search was unsuccessful
//------------------------------------------------------------------------------

uint8_t CdcHecIndexGet(const uint16_t actPhysAddr, const uint16_t firstPhysAddr, const uint16_t lastPhysAddr)
{
    uint8_t i;
    uint8_t hecIdx = 0xFF; // error code by default

    for (i = 0; i < CDC_NUMBER_OF_HECS_MAX; i++)
    {
        if (gpInstRec->aHec[i].status.isInUse == true)
        {
            // First and Last physical address order makes no difference
            if ((gpInstRec->aHec[i].activatorPhysAddr == actPhysAddr) &&
                (((gpInstRec->aHec[i].firstDevPhysAddr == firstPhysAddr) &&
                (gpInstRec->aHec[i].lastDevPhysAddr == lastPhysAddr)) ||
                ((gpInstRec->aHec[i].firstDevPhysAddr == lastPhysAddr) &&
                (gpInstRec->aHec[i].lastDevPhysAddr == firstPhysAddr))))
            {
                hecIdx = i;
                break; // quit here because the HEC entry must be unique
            }

        }
    }

  if (hecIdx == 0xFF)
  {
    //DEBUG_PRINT(MSG_ALWAYS, ("CdcHecIndexGet(): HEC not found in existing records\n"));
  }

  return hecIdx;
}


//------------------------------------------------------------------------------
// Function:    CdcAliveDeviceListAdd
// Description: Adds new devices from HEC to aAliveDevice device list
//              This function shall be called after CdcHecDescriptorAdd() and
//              initialization of a HEC
//
// Parameters:  hecIdx - index of a new HEC
// Returns:     false - success, true - error
//------------------------------------------------------------------------------

bool_t CdcAliveDeviceListAdd(const uint8_t hecIdx)
{
    uint16_t    physAddr;
    uint16_t    firstPhysAddr;
    uint16_t    lastPhysAddr;
    bool_t      restart = true;
    bool_t      errStatus = false;


    if (gpInstRec->aHec[hecIdx].status.isInUse == true)
    {
        if (gpInstRec->aHec[hecIdx].status.isActivator == true)
        {
            firstPhysAddr = gpInstRec->aHec[hecIdx].firstDevPhysAddr;
            lastPhysAddr  = gpInstRec->aHec[hecIdx].lastDevPhysAddr;
            // add all members of a HEC except the Activator (this device)
            do
            {
                physAddr = CdcHecNextDeviceGet(firstPhysAddr, lastPhysAddr, restart);
                restart = false; // restart == true for the first cycle only
                errStatus = CdcPhysAddrToAliveListInsert(physAddr);
            }
            while ((physAddr != lastPhysAddr) && (errStatus == false));
        }
        else // passive devices
        {
            // add only activator to the list
            physAddr = gpInstRec->aHec[hecIdx].activatorPhysAddr;
            errStatus = CdcPhysAddrToAliveListInsert(physAddr);
        }
    }

    return errStatus;
}


//------------------------------------------------------------------------------
// Function:    CdcPhysAddrToAliveListInsert
// Description: auxiliary function for CdcAliveDeviceListAdd()
//              This function is to be called from within CdcAliveDeviceListAdd()
//
// Parameters:  physAddr - physical address of the device to instert
// Returns:     false - success, true - error
//------------------------------------------------------------------------------

static bool_t CdcPhysAddrToAliveListInsert(const uint16_t physAddr)
{
    uint16_t    listIdx;
    uint16_t    vacantCellIdx=0;
    bool_t      isMatchFound;
    bool_t      isVacantCellFound;
    bool_t      errStatus = false;

    isMatchFound = false;
    isVacantCellFound = false;
    for (listIdx = 0; listIdx < CDC_NUMBER_OF_DEVICES_MAX; listIdx++)
    {
        if (gpInstRec->aAliveDevice[listIdx].physAddr == physAddr)
        {
            isMatchFound = true;
            break;  // all aAliveDevice[] members are unique, so there is no reason to continue
        }
        // simultaneous search for a first empty cell
        else if (!isVacantCellFound && (gpInstRec->aAliveDevice[listIdx].physAddr == 0xFFFF))
        {
            vacantCellIdx = listIdx;
            isVacantCellFound = true;
        }
    }
    // add every device of a HEC except this device
    if ((isMatchFound == false) && (physAddr != gpInstRec->physAddr))
    {
        if (isVacantCellFound == true)
        {
            gpInstRec->aAliveDevice[vacantCellIdx].physAddr = physAddr; // add new device for alive monitoring
            CdcTimeCounterSet(CDC_TIME_SEC2TCK(CDC_MAX_NTF_ALIVE_TIME_SEC), &gpInstRec->aAliveDevice[vacantCellIdx].TimeCount);
            gpInstRec->numberOfAliveDev++;
        }
        else
        {
            //DEBUG_PRINT(MSG_DBG, ("ERROR in CdcPhysAddrToAliveListInsert(): aAliveDevice[] list is full\n"));
            errStatus = true;
        }
    }

    return errStatus;
}


//------------------------------------------------------------------------------
// Function:    CdcAliveDeviceListRemove
// Description: Removes devices that should not be monitored anymore
//              from aAliveDevice device list.
//              This function shall be called before CdcHecDescriptorRemove()
//
// Parameters:  hecIdx - index of the dismissed HEC
// Returns:     false - success, true - error
//------------------------------------------------------------------------------

bool_t CdcAliveDeviceListRemove(const uint8_t hecIdx)
{
    uint16_t    physAddr;
    uint16_t    firstPhysAddr;
    uint16_t    lastPhysAddr;
    bool_t      restart = true;
    bool_t      errStatus = false;


    if (gpInstRec->aHec[hecIdx].status.isInUse == true)
    {
        if (gpInstRec->aHec[hecIdx].status.isActivator == true)
        {
            firstPhysAddr = gpInstRec->aHec[hecIdx].firstDevPhysAddr;
            lastPhysAddr  = gpInstRec->aHec[hecIdx].lastDevPhysAddr;
            // remove all members of a HEC except the Activator (this device)
            do
            {
                physAddr = CdcHecNextDeviceGet(firstPhysAddr, lastPhysAddr, restart);
                restart = false; // restart == true for the first cycle only
                // only delete a physical address from the list
                // if it isn't a subject of alive monitoring in the other active HECs
                errStatus = CdcPhysAddrFromAliveListDelete(physAddr, hecIdx);
            }
            while ((physAddr != lastPhysAddr) && (errStatus == false));
        }
        else // passive devices
        {
            // remove only activator from the list
            // if it isn't monitored as a member of other AHECs
            physAddr = gpInstRec->aHec[hecIdx].activatorPhysAddr;
            errStatus = CdcPhysAddrFromAliveListDelete(physAddr, hecIdx);
        }
    }

    return errStatus;
}


//------------------------------------------------------------------------------
// Function:    CdcPhysAddrFromAliveListDelete
// Description: auxiliary function for CdcAliveDeviceListRemove()
//              This function is to be called from within CdcAliveDeviceListRemove()
//
// Parameters:  physAddr - physical address of the device to delete
//              hecIdx   - index of a HEC that is to be closed
// Returns:     false - success, true - error
//------------------------------------------------------------------------------

static bool_t CdcPhysAddrFromAliveListDelete(const uint16_t physAddrToDel, const uint8_t hecIdx)
{
    uint16_t    listIdx;
    uint8_t     i;
    bool_t      isGoodToDelete = true;
    bool_t      errStatus = false;

    if (physAddrToDel != gpInstRec->physAddr) // this device should never be in the self AliveDevice List
    {
        // Check if the device doesn't belong to other AHECs that need to be monitored hereafter
        for (i = 0; i < CDC_NUMBER_OF_HECS_MAX; i++)
        {
            if ((i != hecIdx) && (gpInstRec->aHec[i].status.isInUse == true) && (gpInstRec->aHec[i].status.channelState == CDC_AHEC))
            {
                if (gpInstRec->aHec[i].status.isActivator)
                {
                    if (CdcIsDeviceBelongsToHec(gpInstRec->aHec[i].firstDevPhysAddr, gpInstRec->aHec[i].lastDevPhysAddr, physAddrToDel))
                    {
                        isGoodToDelete = false;
                        break; // stop immediately, if the device shall stay in the Alive list
                    }
                }
                else // passive device
                {
                    if (gpInstRec->aHec[i].activatorPhysAddr == physAddrToDel)
                    {
                        isGoodToDelete = false;
                        break; // stop immediately, if the device shall stay in the Alive list
                    }
                }
            }
        }

        if (isGoodToDelete)
        {
            // Delete a record in AliveDevice List
            for (listIdx = 0; listIdx < CDC_NUMBER_OF_DEVICES_MAX; listIdx++)
            {
                if (gpInstRec->aAliveDevice[listIdx].physAddr == physAddrToDel)
                {
                    gpInstRec->aAliveDevice[listIdx].physAddr = 0xFFFF; // mark as vacant

                    // reset time counter
                    CdcTimeCounterSet(CDC_TIME_SEC2TCK(CDC_MAX_NTF_ALIVE_TIME_SEC), &gpInstRec->aAliveDevice[listIdx].TimeCount);

                    gpInstRec->numberOfAliveDev--;
                    break;  // all aAliveDevice[] members are unique, so there is no reason to continue
                }
            }
        }
    }

    return errStatus;
}


//------------------------------------------------------------------------------
// Function:    CdcTimeCounterSet
// Description: Sets the time counter to given time in the future relatively
//              to provided system timer reading.
//              CAUTION: this function shall never be followed by the
//              CdcTimeCounterUpdate() for the same timer in one
//              TaskProcess invocation
//
//
// Parameters:  delayTimeTck   - the delay time to set (in ticks)
//              pTimeCounter   - pointer to the time counter to set
//------------------------------------------------------------------------------

void CdcTimeCounterSet(const uint16_t delayTimeTck, CdcTimeCounter_t *pTimeCounter)
{
    uint32_t totalTimeMs;

    totalTimeMs = (uint32_t) gpInstRec->sysTimerCountCurMs + CDC_TIME_TCK_MS * ((uint32_t) delayTimeTck);
    pTimeCounter->tRov = (uint16_t)((totalTimeMs / 0x10000) & 0x3);  // 0x10000 = 2^16
    pTimeCounter->tRes = (uint16_t)((totalTimeMs / CDC_TIME_TCK_MS) & 0x3FFF); // mod (2^16)
}


//------------------------------------------------------------------------------
// Function:    CdcTimeCounterUpdate
// Description: Checks if the time counter has expired and updates the timer counter.
//              Resets the time counter if it is expired
//              CAUTION: this function shall never be called more than once per
//              TaskProcess invocation for the same timer
//
// Parameters:  pTimeCounter   - pointer to the time counter to check
//
// Returns:     true - if timer is expired, false - otherwise
//------------------------------------------------------------------------------

bool_t CdcTimeCounterUpdate(CdcTimeCounter_t *pTimeCounter)
{
    bool_t isExpired = false;

    if ((pTimeCounter->tRov != 0) || (pTimeCounter->tRes != 0)) // if not expired already
    {
        if (gpInstRec->sysTimerCountPrvMs > gpInstRec->sysTimerCountCurMs) // if timer overflow detected
        {
            if (pTimeCounter->tRov > 0)
            {
                pTimeCounter->tRov--;
            }
        }

        if ((pTimeCounter->tRov == 0) && (gpInstRec->sysTimerCountCurMs > (CDC_TIME_TCK_MS * pTimeCounter->tRes)))
        {
            isExpired = true;

            // reset the counter
            pTimeCounter->tRov = 0;
            pTimeCounter->tRes = 0;
        }
    }
    else
    {
        isExpired = true;
    }

    // Update minimum time counter
    if (CdcTimeCounterCompare(pTimeCounter, &gpInstRec->cdcRecallTimeCount) == 0)
    {   // if (*pTimeCounter < cdcRecallTimeCount)
        gpInstRec->cdcRecallTimeCount = *pTimeCounter;
    }

    return isExpired;
}


//------------------------------------------------------------------------------
// Function:    CdcIsTimeCounterExpired
// Description: Checks if the time counter has expired
//
// Parameters:  pTimeCounter   - pointer to the time counter to check
//
// Returns:     true - if timer is expired, false - otherwise
//------------------------------------------------------------------------------

bool_t CdcIsTimeCounterExpired(CdcTimeCounter_t *pTimeCounter)
{
    return ((pTimeCounter->tRov == 0) && (pTimeCounter->tRes == 0));
}


//------------------------------------------------------------------------------
// Function:    CdcTimeCounterReset
// Description: Resets the time counter to zero
//
// Parameters:  pTimeCounter   - pointer to the time counter to check
//
//------------------------------------------------------------------------------

void CdcTimeCounterReset(CdcTimeCounter_t *pTimeCounter)
{
    pTimeCounter->tRov = 0;
    pTimeCounter->tRes = 0;
}


//------------------------------------------------------------------------------
// Function:    CdcTimeCounterCompare
// Description: Compares two time counters
//              CAUTION: time counters shall be updated before comparison
//
// Parameters:  pTimeCounterA   - pointer to the time counter A to compare
//              pTimeCounterB   - pointer to the time counter B to compare
//
// Returns:     0 - if time A < B,
//              1 - if time A == B
//              2 - if time A > B
//------------------------------------------------------------------------------

uint8_t CdcTimeCounterCompare(CdcTimeCounter_t *pTimeCounterA, CdcTimeCounter_t *pTimeCounterB)
{
    uint8_t cmpRet;

    if (pTimeCounterA->tRov > pTimeCounterB->tRov)
    {
        cmpRet = 2;
    }
    else if (pTimeCounterA->tRov < pTimeCounterB->tRov)
    {
        cmpRet = 0;
    }
    else if (pTimeCounterA->tRes > pTimeCounterB->tRes)
    {
        cmpRet = 2;
    }
    else if (pTimeCounterA->tRes < pTimeCounterB->tRes)
    {
        cmpRet = 0;
    }
    else
    {
        cmpRet = 1;
    }

    return cmpRet;

}


//------------------------------------------------------------------------------
// Function:    CdcTimeCounterSecGet
// Description: Returns time left until counter expires in seconds
//
// Parameters:  pTimeCounter   - pointer to the time counter
//
// Returns:     time in seconds
//------------------------------------------------------------------------------

uint16_t CdcTimeCounterLeftSecGet(CdcTimeCounter_t *pTimeCounter)
{
    return ((pTimeCounter->tRov * (0x10000 / CDC_TIME_TCK_MS) + pTimeCounter->tRes -
            (gpInstRec->sysTimerCountCurMs / CDC_TIME_TCK_MS)) / (1000 / CDC_TIME_TCK_MS));
}


//------------------------------------------------------------------------------
// Function:    CdcAliveDeviceListTimeReset
// Description: Resets the watchdog's timer for the device that responded
//              with NotifyAlive message
//
// Parameters:  physAddr - physical address of alive device
// Returns:     false    - success, true - error (device not found)
//------------------------------------------------------------------------------

bool_t CdcAliveDeviceListTimeReset(const uint16_t physAddr)
{
    uint16_t    listIdx;
    bool_t      isFound = false;

    for (listIdx = 0; listIdx < CDC_NUMBER_OF_DEVICES_MAX; listIdx++)
    {
         if (gpInstRec->aAliveDevice[listIdx].physAddr == physAddr)
         {
            isFound = true;
            CdcTimeCounterSet(CDC_TIME_SEC2TCK(CDC_MAX_NTF_ALIVE_TIME_SEC), &gpInstRec->aAliveDevice[listIdx].TimeCount);
            break; // because physAddr must be present uniquely
         }
    }

    return isFound;
}


//------------------------------------------------------------------------------
// Function:    CdcAliveDeviceListTimeUpdate
// Description: Set AliveDevice List times up-to-date
//
// Returns:     0xFFFF            - no timeouts have been detected,
//              other numbers     - index of a device failed to notify it's
//                                  alive status. If more than one failed device
//                                  is detected, the fuction reports the last
//                                  one in the list. Other timeouts will be
//                                  pending and reported in subsequent calls of
//                                  the function
//------------------------------------------------------------------------------

uint16_t CdcAliveDeviceListTimeUpdate(void)
{
    uint16_t    listIdx;
    uint16_t    timeoutedDeviceDetected = 0xFFFF;


    for (listIdx = 0; listIdx < CDC_NUMBER_OF_DEVICES_MAX; listIdx++)
    {
        if (gpInstRec->aAliveDevice[listIdx].physAddr != 0xFFFF) // for all nonempty records
        {
            if (CdcTimeCounterUpdate(&gpInstRec->aAliveDevice[listIdx].TimeCount) == true)
            {   // if counter has expired
                timeoutedDeviceDetected = listIdx;
                //DEBUG_PRINT(MSG_DBG, ("CdcAliveDeviceListTimeUpdate(): Timeout detected for PA = %04x\n", gpInstRec->aAliveDevice[listIdx].physAddr));
            }

        }
    }

    return timeoutedDeviceDetected;
}


//------------------------------------------------------------------------------
// Function:    CdcIsDeviceBelongsToHec
// Description: Figures out whether a device is in the path of the eHDMI channel
//
// Parameters:  firstPhysAddr - PA of the HEC's beginning device,
//              lastPhysAddr  - PA of the HEC's end device,
//              testPhysAddr  - PA of the device to examine
//
// Returns:     true, if the device with testPhysAddr belongs to the HEC,
//              false - otherwise
//------------------------------------------------------------------------------

bool_t CdcIsDeviceBelongsToHec(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr, const uint16_t testPhysAddr)
{
    uint16_t mask = 0xF000;

    if (firstPhysAddr == lastPhysAddr) // prohibited combination that should never happen. This "if" prevents deadlock
    {
        return (firstPhysAddr == testPhysAddr);
    }

    while ((firstPhysAddr & mask) == (lastPhysAddr & mask)) // Find root level of the HEC tree
    {
        mask = (mask >> 4) | 0xF000;
    }

    mask <<= 4;
    if (testPhysAddr == (firstPhysAddr & mask)) // check if there is a match with the HEC tree root
    {
        return true;
    }

    // step down the tree (left and right from root) and see if there is a match
    while(mask != 0xFFFF)
    {
        mask = (mask >> 4) | 0xF000;

        if ((testPhysAddr == (firstPhysAddr & mask)) || (testPhysAddr == (lastPhysAddr & mask)))
        {
            return true;
        }
    }

    return false;
}



//------------------------------------------------------------------------------
// Function:    CdcHecNextDeviceGet
// Description: Consecutively calculates physical addresses of HEC devices
//              starting from firstPhysAddr and ending with lastPhysAddr.
//              Calling the function after reaching the lastPhysAddr without
//              the parameter "restart" set to true will result in starting over
//              again from the firstPhysAddr.
//
// Parameters:  firstPhysAddr - PA of the HEC's beginning device,
//              lastPhysAddr  - PA of the HEC's end device,
//              restart       - forces the function to start over from the beginning
//
// Returns:     physical address of the next device in the HEC's path
//------------------------------------------------------------------------------

uint16_t CdcHecNextDeviceGet(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr, const bool_t restart)
{
    static uint16_t mask = 0xFFF0;
    static bool_t   reverse = false;
    uint16_t physAddr;
    uint16_t pa;

    if (restart)
    {
        mask = 0x0000;
        reverse = false;
        // skip absent levels (right zeros)
        pa = firstPhysAddr;
        while (pa != 0)
        {
            pa <<= 4;
            mask = (mask >> 4) | 0xF000;
        }
    }

    if (reverse == false)  // go up from first PA to the top of HEC's tree
    {
        physAddr = firstPhysAddr & mask;
        if (physAddr == (lastPhysAddr & mask))
        {
            reverse = true;
            mask = (mask >> 4) | 0xF000;
        }
        else
        {
            mask <<= 4;
        }
    }
    else    // go down from the HEC's tree top to the last PA
    {
        physAddr = lastPhysAddr & mask;
        if ((lastPhysAddr & mask) == lastPhysAddr) // start over when the end of HEC is reached
        {
            reverse = false;
            mask = 0x0000;
            // skip absent levels (right zeros)
            pa = firstPhysAddr;
            while (pa != 0)
            {
                pa <<= 4;
                mask = (mask >> 4) | 0xF000;
            }
        }
        else
        {
            mask = (mask >> 4) | 0xF000;
        }
    }

    return physAddr;
}


//------------------------------------------------------------------------------
// Function:    CdcHecLengthGet
// Description: Calculates length (number of devices) of a HEC
//
// Parameters:  firstPhysAddr - PA of the HEC's beginning device,
//              lastPhysAddr  - PA of the HEC's end device,
//
// Returns:     number of devices in the HEC
//------------------------------------------------------------------------------

uint8_t CdcHecLengthGet(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr)
{
    uint8_t     deviceCount = 1; // minimal length is 2 (fist & last devices only)
    uint16_t    physAddr;

    physAddr = CdcHecNextDeviceGet(firstPhysAddr, lastPhysAddr, true);
    while (physAddr != lastPhysAddr)
    {
        physAddr = CdcHecNextDeviceGet(firstPhysAddr, lastPhysAddr, false);
        deviceCount++;
    }
    return deviceCount;
}


//------------------------------------------------------------------------------
// Function:    CdcIsLinkedDirectly
// Description: Figures out whether two devices have relationship
//              of a Sink and a Source. This function is used in
//              HPD signalling protocol implementation
//
// Parameters:  sinkPhysAddr    - PA of the possible Sink device,
//              sourcePhysAddr  - PA of the possible Source device,
//              inputPortIndex  - index of an input port of the Sink
//
// Returns:     true, if devices are linked over the given input port
//              false - otherwise
//------------------------------------------------------------------------------

bool_t CdcIsLinkedDirectly(const uint16_t sinkPhysAddr, const uint16_t sourcePhysAddr, const uint8_t inputPortIndex)
{
    uint8_t     actualInputPortIndex;
    bool_t      isLinked;

    isLinked = CdcIsLinkedDirectlyInPortGet(sinkPhysAddr, sourcePhysAddr, &actualInputPortIndex);

    if (actualInputPortIndex != inputPortIndex)
    {
        isLinked = false;
    }

    return isLinked;
}


//------------------------------------------------------------------------------
// Function:    CdcAdjacentPhysAddrGet
// Description: Calculates physical address of a device, potential or real, that
//              is adjacent to the reference device.
//
// Parameters:  basePhysAddr   - PA of the reference device,
//              isAtOutput     - if true, returns a device's PA adjacent to output port,
//                               if false, returns a device's PA adjacent PA to
//                               specified input port
//              inputPortIndex - zero-based index of the input port of the adjacent device
//                             (ignored, if isAtOutput == true)
//
// Returns:     Physical Address of the Adjacent device,
//              0xFFFF - if the Adjacent device PA doesn't exist
//                       (sink of a root or source of 5th level)
//------------------------------------------------------------------------------

uint16_t CdcAdjacentPhysAddrGet(const uint16_t basePhysAddr, const bool_t isAtOutput, const uint8_t inputPortIndex)
{
    uint16_t mask = 0xFFF0;
    uint16_t adjPhysAddr = 0xFFFF;
    uint8_t  n = 0;

    if (inputPortIndex >= 0xF) // illegal port index
    {
        return 0xFFFF;
    }

    // find the last non-zero digit in basePhysAddr
    while ((n < 4) && ((basePhysAddr & mask) == basePhysAddr))
    {
        n++;
        mask <<= 4;
    }

    if (isAtOutput)
    {
        if (basePhysAddr != 0x0000)
        {
            adjPhysAddr = basePhysAddr & mask;
        }
    }
    else // adjacent to input
    {
        if (n != 0) // if last digit is 0
        {
           adjPhysAddr = basePhysAddr | ((inputPortIndex + 1) << (4 * (n - 1)));
        }
    }

    return adjPhysAddr;
}


//------------------------------------------------------------------------------
// Function:    CdcIsCapableToAdjacent
// Description: Figures out whether two devices have relationship
//              of Sink and Source (i.e. they are adjacent) and the adjacent
//              device has HEC capable input or output port that connects
//              it to the initiator.
//              This function is used in control for Adjacent devices.
//
// Parameters:  initiatorPhysAddr - PA of the initiating device,
//              adjacentPhysAddr  - PA of the adjacent device,
//              isAdjacent        - (return value) pointer to adjacent flag
//                                  that is true, if 2 devices are adjacent
//
// Returns:     true, if devices are linked over HEC capable port
//              false - otherwise
//------------------------------------------------------------------------------

bool_t CdcIsCapableToAdjacent(const uint16_t initiatorPhysAddr, const uint16_t adjacentPhysAddr, bool_t *isAdjacent)
{
    uint8_t inputPortIndex;
    bool_t  isLinkedToSink = false;
    bool_t  isLinkedToSource = false;

    isLinkedToSink = CdcIsLinkedDirectlyInPortGet(initiatorPhysAddr, adjacentPhysAddr, &inputPortIndex);
    isLinkedToSource = CdcIsLinkedDirectlyInPortGet(adjacentPhysAddr, initiatorPhysAddr, &inputPortIndex);
    *isAdjacent = isLinkedToSink || isLinkedToSource;

    if (/* if initiator is a Sink, need capable output to establish a connection */
        ( isLinkedToSink &&
         (gpInstRec->ports.numberOfOutputs > 0) && (gpInstRec->ports.output.hecSupport == CDC_HEC_SUPPORTED)) ||
        /* if initiator is a Source, need capable input to establish a connection */
        ( isLinkedToSource &&
         (gpInstRec->ports.numberOfInputs > inputPortIndex) && (gpInstRec->ports.input[inputPortIndex].hecSupport == CDC_HEC_SUPPORTED)))
    {
        return true;
    }
    else
    {
        return false;
    }
}


//------------------------------------------------------------------------------
// Function:    CdcIsLinkedDirectlyInPortGet
// Description: Figures out whether two devices have relationship
//              of a Sink and a Source. If so, returns index of the Sink's
//              input port that is connected to the Source output port
//
// Parameters:  sinkPhysAddr      - PA of the possible Sink device,
//              sourcePhysAddr    - PA of the possible Source device,
//              pInputPortIndex   - (return value) pointer to an index of an input
//                                  port of the Sink that must be used for the HEC
//                                  connection.
//                                  0xFF - in the case when Sink-Source relationship
//                                  hasn't been verified
//
// Returns:     true, if devices are linked directly over some of the Sink's inputs
//              false - otherwise
//------------------------------------------------------------------------------

static bool_t CdcIsLinkedDirectlyInPortGet(const uint16_t sinkPhysAddr, const uint16_t sourcePhysAddr, uint8_t *pInputPortIndex)
{
    // criteria: 1. Sink must be one level above the Source
    //           2. The input port number (= inputPortIdx+1) must be equal to
    //              the Source's physical address last nonzero digit (p. 8.7.2. of HDMI 1.4 spec)
    uint8_t     i;
    uint8_t     n = 0;
    uint16_t    mask = 0x000F;
    uint16_t    sourceMasked;
    uint16_t    sinkMasked;
    bool_t      isLinked = false;

    // crit.1 meets if and only if sink and source have one of the following patterns:
    // {0000, n000}, {x000, xn00}, {xy00, xyn0}, {xyz0, xyzn}
    for (i = 0; i < 4; i++)
    {
        sourceMasked = sourcePhysAddr & mask;
        sinkMasked   = sinkPhysAddr & mask;

        if (n == 0) // n point not found yet
        {
             // skip zero-zero pairs
             if ((sinkMasked == 0) && (sourceMasked !=0)) // first zero-nonzero pair found
             {
                 n = sourceMasked >> (4 * i); // input port number (nonzero)
                 *pInputPortIndex = n - 1;      // input port index to return
                 isLinked = true;             // potential detection
             }
             else if ((sinkMasked != 0) && (sourceMasked !=0))
             {
                break;
             }
             else if ((sinkMasked != 0) && (sourceMasked ==0))
             {
                break;
             }
        }
        else // all other digit pairs must be non-zero & equal
        {
            if ((sinkMasked == 0) || (sinkMasked != sourceMasked))
            {
                isLinked = false; // reset linked flag if wrong MSB pattern found
                *pInputPortIndex = 0xFF;
                break;
            }
        }

        mask <<= 4; // scan all 4 digits
    }


    return isLinked;
}


//------------------------------------------------------------------------------
// Function:    CdcCapablePortGet
// Description: Finds out what input and/or output ports are utilized
//              for a HEC that includes this device. Verifies if those ports
//              are capable to maintain the HEC. This function shall be called
//              after verifying that the HEC includes this device
//
// Parameters:  firstPhysAddr   - first device PA of the HEC,
//              lastPhysAddr    - last device PA of the HEC,
//              aInPortIdx[2]   - pointer to input port index array (return value),
//                                containing 2 elements: ports of the HEC
//                                coming-in and coming-out.
//                                0xFF value indicates that the HEC route
//                                doesn't come through the input
//              pOutPortFlg     - pointer to output port flag (return value)
//                                0 - indicates that the HEC route
//                                doesn't come through the output
//                                1 - indicates thar the HEC route
//                                come through the output
//
// Returns:     true, if routing is verified for the HEC
//              false - otherwise
//------------------------------------------------------------------------------

bool_t  CdcCapablePortGet(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr,
                           uint8_t aInPortIdx[2], bool_t *pOutPortFlg)
{
    uint8_t  i;
    uint8_t  inpCnt = 0;
    bool_t   restart = true;
    uint16_t physAddr;
    bool_t   isVerified = true;


    *pOutPortFlg    = false;    // output port isn't in use by default
    aInPortIdx[0]   = 0xFF;     // no input port is in use by default
    aInPortIdx[1]   = 0xFF;

    do // for all devices in the HEC
    {
        physAddr = CdcHecNextDeviceGet(firstPhysAddr, lastPhysAddr, restart);
        restart = false; // restart == true for the first cycle only

        // check if a device from the HEC is a Sink for this device, so the output shall be in use
        if (*pOutPortFlg == false) // do it only if output usage has not been determined yet
        {
            *pOutPortFlg = CdcIsLinkedDirectlyInPortGet(physAddr, gpInstRec->physAddr, &i);
        }

        // check if a device from the HEC is a Source for this device
        // and find out the input ports that are connected to the Source (maximum 2)
        if ((inpCnt == 0) || ((inpCnt == 1) && (!*pOutPortFlg))) // do it only if input usage has not been determined yet
        {
            if (CdcIsLinkedDirectlyInPortGet(gpInstRec->physAddr, physAddr, &i))
            {
                aInPortIdx[inpCnt++] = i;
            }

        }
    }
    while ((physAddr != lastPhysAddr) && ((*pOutPortFlg == false) || (inpCnt == 0)));

    // verify HEC capability of the affected ports
    if (*pOutPortFlg == true) // output
    {
        if ((gpInstRec->ports.numberOfOutputs == 0) || (!gpInstRec->ports.output.hecSupport))
        {
            isVerified = false;
        }
    }

    for (i = 0; i < inpCnt; i++) // inputs (up to 2)
    {

        if ((gpInstRec->ports.numberOfInputs <= aInPortIdx[i]) || (!gpInstRec->ports.input[aInPortIdx[i]].hecSupport))
        {
            isVerified = false;
            break;
        }

    }

    if ((inpCnt == 0) && (*pOutPortFlg == false)) // not verified, if no ports participate in the HEC
    {
        isVerified = false;
    }

    return isVerified;
}


//------------------------------------------------------------------------------
// Function:    CdcIsCapableForHec
// Description: Figures out if this device has all capabilities to become a
//              part of a HEC, i.e. the device is HEC capable and all ports
//              that must be activated to create the HEC are HEC capable too.
//
// Parameters:  firstPhysAddr   - first device PA of the HEC,
//              lastPhysAddr    - last device PA of the HEC,
//              isStrict        - false will allow to verify this device
//                                positively, if it's outside of the HEC
//
//
// Returns:     true, if routing is verified for the HEC
//              false - otherwise
//------------------------------------------------------------------------------

bool_t  CdcIsCapableForHec(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr, const bool_t isStrict)
{
    uint8_t aInPortIdx[2];
    bool_t  outPortFlg;
    bool_t  isVerified;

    if (firstPhysAddr != lastPhysAddr)
    {
        if (CdcIsDeviceBelongsToHec(firstPhysAddr, lastPhysAddr, gpInstRec->physAddr))
        {
            isVerified = CdcCapablePortGet(firstPhysAddr, lastPhysAddr, aInPortIdx, &outPortFlg);
        }
        else
        {
            isVerified = !isStrict;
        }
    }
    else
    {
        isVerified = false; // Situation when (firstPhysAddr == lastPhysAddr) is prohibited
    }

    return isVerified;
}

//------------------------------------------------------------------------------
// Function:    CdcIoPortsActivate
// Description: Activates I/O ports of this device when new AHEC is created.
//              This service function shall be called after calling of
//              CdcIsDeviceBelongsToHec() and CdcCapablePortGet(), both
//              has to return true, as it lacks a few condition checks
//              in order to improve performance
//
// Parameters:  aInPortIdx[2]   - pointer to input port index array,
//                                containing 2 elements: ports of the HEC
//                                coming-in and coming-out.
//                                0xFF value indicates that the HEC route
//                                doesn't come through the input
//              pOutPortFlg     - pointer to output port flag
//                                0 - indicates that the HEC route
//                                doesn't come through the output
//                                1 - indicates thar the HEC route
//                                come through the output
//------------------------------------------------------------------------------

void CdcIoPortsActivate(const uint8_t aInPortIdx[2], const bool_t outPortFlg)
{
    uint8_t i;

    // update output port status
    if (outPortFlg && (gpInstRec->ports.output.hecActive == CDC_HEC_INACTIVE))
    {
        // Activate output's hardware
        if (CdcEhdmiIoPortHwEnable(true, 0, true))
        {
            gpInstRec->ports.output.hecActive = CDC_HEC_ACTIVE;
        }
    }
    // update input ports status
    for (i = 0; i < 2; i++)
    {
        if ((aInPortIdx[i] != 0xFF) && (gpInstRec->ports.input[aInPortIdx[i]].hecActive == CDC_HEC_INACTIVE))
        {
            // Activate input's hardware
            if (CdcEhdmiIoPortHwEnable(false, aInPortIdx[i], true))
            {
                gpInstRec->ports.input[aInPortIdx[i]].hecActive = CDC_HEC_ACTIVE;
            }
        }
    }

}


//------------------------------------------------------------------------------
// Function:    CdcIoPortsStatusUpdate
// Description: Updates I/O ports status of this device to reflect current state
//              and participation in active HECs. This function shall be called
//              whenever HECs are activated or deactivated
//------------------------------------------------------------------------------

void CdcIoPortsStatusUpdate(void)
{
    bool_t  isInputInUse[CDC_EHDMI_INPUTS_MAX];
    bool_t  isOutputInUse = false;
    uint8_t aInPortIdx[2];
    bool_t  outPortFlg;
    uint8_t i;

    // initialize isInputInUse[] flags
    for (i = 0; i < gpInstRec->ports.numberOfInputs; i++)
    {
        isInputInUse[i] = false;
    }

    // set flags for ports that support at least one AHEC
    for (i = 0; i < CDC_NUMBER_OF_HECS_MAX; i++)
    {
        if ((gpInstRec->aHec[i].status.isInUse == true) && (gpInstRec->aHec[i].status.channelState == CDC_AHEC))
        {
            if (CdcCapablePortGet(gpInstRec->aHec[i].firstDevPhysAddr,
                                  gpInstRec->aHec[i].lastDevPhysAddr, aInPortIdx, &outPortFlg))
            {
                if (aInPortIdx[0] != 0xFF)
                {
                    isInputInUse[aInPortIdx[0]] = true;
                }
                if (aInPortIdx[1] != 0xFF)
                {
                    isInputInUse[aInPortIdx[1]] = true;
                }
                if (outPortFlg)
                {
                    isOutputInUse = true;
                }

            }
        }
    }

    // set input ports status
    for (i = 0; i < gpInstRec->ports.numberOfInputs; i++)
    {
        if (gpInstRec->ports.input[i].hecSupport == CDC_HEC_SUPPORTED)
        {
            if(isInputInUse[i])
            {
                // Activate input's hardware
                if (CdcEhdmiIoPortHwEnable(false, i, true))
                {
                    gpInstRec->ports.input[i].hecActive = CDC_HEC_ACTIVE;
                }
            }
            else
            {
                // Deactivate input's hardware
                if (CdcEhdmiIoPortHwEnable(false, i, false))
                {
                    gpInstRec->ports.input[i].hecActive = CDC_HEC_INACTIVE;
                }

            }
        }
    }

    // set output port status
    if (gpInstRec->ports.numberOfOutputs > 0)
    {
        if (gpInstRec->ports.output.hecSupport == CDC_HEC_SUPPORTED)
        {
            if(isOutputInUse)
            {
                // Activate output's hardware
                if (CdcEhdmiIoPortHwEnable(true, 0, true))
                {
                    gpInstRec->ports.output.hecActive = CDC_HEC_ACTIVE;
                }
            }
            else
            {
                // deactivate output's hardware
                if (CdcEhdmiIoPortHwEnable(true, 0, false))
                {
                    gpInstRec->ports.output.hecActive = CDC_HEC_INACTIVE;
                }

            }
        }
    }

}


//------------------------------------------------------------------------------
// Function:    CdcIsHecEnabledInDev
// Description: Finds out if a HEC is enabled or possible in the given Device.
//              This function should be used when analysing the Device ReportState
//              extended message, including the bit field stating the ports status
//
// Parameters:  hecIdx          - Index of existing HEC to check,
//              devPhysAddr     - the Device (an initiator of ReportState msg) physical address,
//              portBitField    - bit field indicating the I/O ports status (see CdcHecReportStateSend())
//
// Returns:     true, if support/activation is verified for the HEC
//              false - otherwise
//------------------------------------------------------------------------------

bool_t  CdcIsHecEnabledInDev(const uint8_t hecIdx, const uint16_t devPhysAddr, const uint16_t portBitField)
{
    uint8_t  i;
    uint8_t  inpCnt = 0;
    bool_t   outPortFlg;
    uint8_t  aInPortIdx[2];
    bool_t   restart = true;
    uint16_t physAddr;
    bool_t   isVerified = false;


    outPortFlg      = false;    // output port isn't in use by default
    aInPortIdx[0]   = 0xFF;     // no input port is in use by default
    aInPortIdx[1]   = 0xFF;

    do // for all devices in the HEC
    {
        physAddr = CdcHecNextDeviceGet(gpInstRec->aHec[hecIdx].firstDevPhysAddr,
                                       gpInstRec->aHec[hecIdx].lastDevPhysAddr, restart);
        restart = false; // restart == true for the first cycle only

        // check if a device from the HEC is a Sink for this device, so the output shall be in use
        if (outPortFlg == false) // do it only if output usage has not been determined yet
        {
            outPortFlg = CdcIsLinkedDirectlyInPortGet(physAddr, devPhysAddr, &i);
        }

        // check if a device from the HEC is a Source for this device
        // and find out the input ports that are connected to the Source (maximum 2)
        if ((inpCnt == 0) || ((inpCnt == 1) && (!outPortFlg))) // do it only if input usage has not been determined yet
        {
            if (CdcIsLinkedDirectlyInPortGet(devPhysAddr, physAddr, &i))
            {
                aInPortIdx[inpCnt++] = i;
            }

        }
    }
    while ((physAddr != gpInstRec->aHec[hecIdx].lastDevPhysAddr) && ((outPortFlg == false) || (inpCnt == 0)));

    // Verify that required ports for this HEC are HEC-capable or active
    // (unused connections can have any state)
    if ((!outPortFlg || (((portBitField >> 14) & 0x01) == 1)) &&
        ((aInPortIdx[0] == 0xFF) || (((portBitField >> (13 - aInPortIdx[0])) & 0x01) == 1)) &&
        ((aInPortIdx[1] == 0xFF) || (((portBitField >> (13 - aInPortIdx[1])) & 0x01) == 1)))
    {
        isVerified = true;
    }

    return isVerified;
}


//------------------------------------------------------------------------------
// Function:    CdcDiscoveryDataArrange
// Description: Identifies all found HOST or ENC capable devices that aren't
//              reachable from this device, and then cleans up and rearranges
//              the cdcFoundHostsList to leave only capable & reachable devices.
//              This function shall be used after collection of all responses
//              in the Discovery process
//
//------------------------------------------------------------------------------

void CdcDiscoveryDataArrange(void)
{
    uint8_t  i;
    uint8_t  j;
    uint8_t  inpIdx;
    bool_t   isVerified;
    uint16_t prevPhysAddr;
    uint16_t curPhysAddr;
    uint8_t  prevIdx = 0; // index of the prevPhysAddr in cdcFoundHostsList[]
    uint8_t  curIdx = 0;  // index of the curPhysAddr in cdcFoundHostsList[]
    bool_t   isPrevPortCapable=0;
    bool_t   outPortFlg;
    CdcFoundListEntry_t tempListEntry;

    // remove unreachable devices
    for (i = 0; i < gpInstRec->numberOfAllHosts; i++)
    {
        if ( ((gpInstRec->cdcFoundHostsList[i].devStat.hecFuncState != CDC_HOST_NOT_SUPPORTED) ||
              (gpInstRec->cdcFoundHostsList[i].devStat.encFuncState != CDC_ENC_NOT_SUPPORTED)) &&
             (gpInstRec->cdcFoundHostsList[i].devStat.cdcErrorState == CDC_ERRCODE_NO_ERROR) )
        {
             gpInstRec->cdcFoundHostsList[i].hostInfo.isSelected = true; // select all potential connections
            // check that all HEC@port capable devices in the PHEC are present
            // in the cdcFoundHostsList
            prevPhysAddr = CdcHecNextDeviceGet(gpInstRec->physAddr, gpInstRec->cdcFoundHostsList[i].physAddr, 1); // skip this device

            do // for the rest of the device chain in the tested HEC
            {
                curPhysAddr = CdcHecNextDeviceGet(gpInstRec->physAddr, gpInstRec->cdcFoundHostsList[i].physAddr, 0);

                // find curPhysAddr in the cdcFoundHostsList[], it must be present to allow the HEC possibility
                isVerified = false; // initial value; it will be true if HEC is verified to be possible
                for (j = 0; j < gpInstRec->numberOfAllHosts; j++)
                {
                    if (gpInstRec->cdcFoundHostsList[j].physAddr == curPhysAddr)
                    {
                        isVerified = true;
                        curIdx = j;
                        break; // quit this search cycle, if the match has been found
                    }
                }

                if (isVerified)
                { // Verify that connecting ports of the prevPhysAddr & curPhysAddr devices are HEC capable

                    // check if the curPhysAddr is a Sink for prevPhysAddr, so the output of prevPhysAddr shall be in use
                    outPortFlg = CdcIsLinkedDirectlyInPortGet(curPhysAddr, prevPhysAddr, &inpIdx);

                    if (outPortFlg) // curPhysAddr is a Sink for prevPhysAddr
                    {
                        // verify that connected output and input are HEC capable
                        if (prevPhysAddr != gpInstRec->physAddr) // for devices from list
                        {
                            isPrevPortCapable = (gpInstRec->cdcFoundHostsList[prevIdx].hostInfo.portHecCapBits & 0x4000) != 0;
                        }
                        else if (gpInstRec->ports.numberOfOutputs != 0) // for this device, that isn't in list (by definition)
                        {
                            isPrevPortCapable = gpInstRec->ports.output.hecSupport;
                        }
                        else // this device is incapable
                        {
                            isVerified = false;
                        }

                        if (!((gpInstRec->cdcFoundHostsList[curIdx].hostInfo.portHecCapBits & (0x2000 >> inpIdx)) && isPrevPortCapable))
                        {
                            isVerified = false;
                        }

                    }
                    else // If not, the opposite has to be true: curPhysAddr is a Source for prevPhysAddr
                    {
                        outPortFlg = CdcIsLinkedDirectlyInPortGet(prevPhysAddr, curPhysAddr, &inpIdx);

                        if (outPortFlg) //curPhysAddr is a Source for prevPhysAddr
                        {
                            // verify that connected output and input are HEC capable
                            if (prevPhysAddr != gpInstRec->physAddr) // for devices from list
                            {
                                isPrevPortCapable = (gpInstRec->cdcFoundHostsList[prevIdx].hostInfo.portHecCapBits & (0x2000 >> inpIdx)) != 0;
                            }
                            else if (gpInstRec->ports.numberOfInputs > inpIdx) // for this device, that isn't in list (by definition)
                            {
                                isPrevPortCapable = gpInstRec->ports.input[inpIdx].hecSupport;
                            }
                            else // this device is incapable
                            {
                                isVerified = false;
                            }

                            if (!(((gpInstRec->cdcFoundHostsList[curIdx].hostInfo.portHecCapBits & 0x4000) != 0) && isPrevPortCapable))
                            {
                                isVerified = false;
                            }

                        }
                        else // curPhysAddr and prevPhysAddr aren't connected directly (should be logically impossible)
                        {
                            //DEBUG_PRINT(MSG_DBG, ("(IMPROBABLE) ERROR in CdcDiscoveryDataArrange(): devices must be connected directly\n"));
                        }
                    }

                    prevPhysAddr = curPhysAddr;
                    prevIdx = curIdx;
                }
            }
            while (isVerified && (curPhysAddr != gpInstRec->cdcFoundHostsList[i].physAddr));


            // clear "Host or Enc" flag, if the device is unreachable
            if (!isVerified)
            {
                gpInstRec->cdcFoundHostsList[i].hostInfo.isSelected = 0;
                //DEBUG_PRINT(MSG_DBG, ("CdcDiscoveryDataArrange(): unreachable device (PA = %04x) found. Omitted from list.\n", gpInstRec->cdcFoundHostsList[i].physAddr));
            }

        }
    }

    // rearrange the verified list of found Host/ENC capable devices:
    // put all "intermediate" devices not labeled as Host or EN capable after all labeled ones
    j = 0; // counts a number of "intermediate" devices
    for (i = 0; i < gpInstRec->numberOfAllHosts; i++)
    {
        if (gpInstRec->cdcFoundHostsList[i].hostInfo.isSelected)
        {
            // swap entries
            tempListEntry = gpInstRec->cdcFoundHostsList[i - j];
            gpInstRec->cdcFoundHostsList[i - j] = gpInstRec->cdcFoundHostsList[i];
            gpInstRec->cdcFoundHostsList[i] = tempListEntry;
        }
        else
        {
            j++;
        }
    }

    gpInstRec->numberOfFoundHosts = gpInstRec->numberOfAllHosts - j; // shorten the list length to the number of Host/ENC capable devices

}


//------------------------------------------------------------------------------
// Function:    CdcEhdmiHwEnable
// Description: Turns on or off the eHDMI hardware function when it is required
//              by CDC protocol.
//              NOTE: this function must return success if HW already was
//                    in the requested state.
//
// Parameters:  isEnabled  - true,  if eHDMI should be turned on,
//                           false, if eHDMI should be turned off
//
// Returns:     true, if eHDMI hardware enabled or disabled successfully
//              false - otherwise
//------------------------------------------------------------------------------

bool_t CdcEhdmiHwEnable(const bool_t isEnabled)
{
    bool_t isSuccess = isEnabled || true; // Suppress compiler warning

    /* For the SiI9387, eHDMI hardware is enabled on a port-by-port */
    /* basis, so we just return success.                            */

    if (!isSuccess)
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcEhdmiHwEnable(): Could not access HEC hardware.\n"));
        gpInstRec->deviceStatus.cdcErrorState = CDC_ERRCODE_OTHER_ERROR;
    }

    return isSuccess;
}

//------------------------------------------------------------------------------
// Function:    CdcEhdmiIoPortHwEnable
// Description: Turns on or off the eHDMI I/O ports hardware, if it has to be
//              done independently from enabling the eHDMI function as a whole.
//              NOTE: this function must return success if HW already was
//                    in the requested state.
//
// Parameters:  isOutput   - true, if output state has to be changed
//                           false,if input state has to be changed
//              inPortIndex- 0-based index of input port, if (isOutput == false)
//              isEnabled  - true,  if eHDMI should be turned on,
//                           false, if eHDMI should be turned off
// Returns:     true, if eHDMI I/O hardware enabled or disabled successfully
//              false - otherwise
//------------------------------------------------------------------------------

bool_t CdcEhdmiIoPortHwEnable( const bool_t isOutput, const uint8_t portIndex, const bool_t isEnabled)
{
    bool_t isSuccess = isOutput || portIndex || true;

    /* The SiI9387 is input only, so we can ignore the isOutput flag.   */
    /* This should be changed if the CDC code is used in a system that  */
    /* incorporates both input and out HEC ports.                       */

    if (isOutput)
    {
        isSuccess = false;  // HEC function at output isn't available
    }
    else
    {
		// %%% Commented out because for now we are not including HEC in the Evita build
        //isSuccess = SI_HecSetState( portIndex, isEnabled ? SI_HEC_ENABLE : SI_HEC_DISABLE );
	if(isEnabled){}// this is just to make the compiler happed 'cause we are not using isEnabled.
    }

    if (!isSuccess)
    {
        TPI_CDC_PRINT(("ERROR in CdcEhdmiHwEnable(): couldn't %s HEC function of %s port [%d].\n", isEnabled ? "enable" : "disable", isOutput ? "output" : "input", (int) portIndex));
        gpInstRec->deviceStatus.cdcErrorState = CDC_ERRCODE_NOT_SUPPORTED_CAPABILITY;
    }

    return( isSuccess );
}

//------------------------------------------------------------------------------
// Function:    CdcCpiWrite
// Description: Sends a CDC message over the CEC bus using CPI
//------------------------------------------------------------------------------

static void CdcCpiWrite (uint8_t opCode, uint8_t argCount, SI_CpiData_t *cdcMsg)
{
    uint16_t physAddr = gpInstRec->physAddr;

    /* Add standard CDC stuff to CEC message */

#if (IS_CEC == 1)
    cdcMsg->srcDestAddr =  MAKE_SRCDEST( SI_CpiGetLogicalAddr(), 0xF); // if CEC & CDC coexist
#else
    cdcMsg->srcDestAddr    = CDC_CEC_SRCDEST_ADDR_DEFAULT;          // CDC only implementation
#endif

    cdcMsg->opcode         = CDC_CEC_OPCODE;
    cdcMsg->args[0]        = (physAddr >> 8) & 0xFF;   // Our Address (MSB)
    cdcMsg->args[1]        = physAddr & 0xFF;          // Our Physical Address (LSB)
    cdcMsg->args[2]        = opCode;
    cdcMsg->argCount       = argCount;

    SI_CpiWrite(cdcMsg);                               // Send CDC message over CPI
}

//------------------------------------------------------------------------------
// Low level API Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function:    CdcHecDiscoverSend
// Description: Sends <CDC HEC Discover> in order to collect HEC capability of
//              all devices in the network.
//              This function is to be used by Activator.
//------------------------------------------------------------------------------

void CdcHecDiscoverSend(void)
{
    SI_CpiData_t cdcDiscoveryMsg;

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HEC_DISCOVER, 3, &cdcDiscoveryMsg);

}



//------------------------------------------------------------------------------
// Function:    CdcHecInquireStateSend
// Description: Sends <CDC HEC InquireState> in order to inquire HEC capability of
//              all devices in the potential channel.
//              This function is to be used by Activator.
// Parameters:  hecIdx - index of a HEC in the Instance Record
//------------------------------------------------------------------------------

void CdcHecInquireStateSend(const uint8_t hecIdx)
{
    SI_CpiData_t cdcInquireStateMsg;
    uint16_t firstPhysAddr, lastPhysAddr;

    // Check for necessary conditions required to carry out this operation
    if(gpInstRec->aHec[hecIdx].status.isActivator == false)
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHecInquireStateSend(): device must be an Activator\n"));
        return;
    }

    firstPhysAddr = gpInstRec->aHec[hecIdx].firstDevPhysAddr;
    lastPhysAddr  = gpInstRec->aHec[hecIdx].lastDevPhysAddr;

    cdcInquireStateMsg.args[3]      = (firstPhysAddr >> 8) & 0xFF;
    cdcInquireStateMsg.args[4]      = firstPhysAddr & 0xFF;
    cdcInquireStateMsg.args[5]      = (lastPhysAddr >> 8) & 0xFF;
    cdcInquireStateMsg.args[6]      = lastPhysAddr & 0xFF;

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HEC_INQUIRE_STATE, 7, &cdcInquireStateMsg);

}


//------------------------------------------------------------------------------
// Function:    CdcHecInquireStateOfAdjacentSend
// Description: Sends <CDC HEC InquireState> in order to inquire HEC capability of
//              an adjacent device. This function is useful for HPD
//              capability verification
//
// Parameters:  isAtOutput   - if true, the message is sent to a device
//                             adjacent to output port;
//                             if false, the message is sent to a device adjacent
//                             to the specified input port
//
//              inputPortIndex - zero-based index of the input port of the adjacent device
//                             (ignored, if isAtOutput == true)
//
// Returns:     true  - if the adjacent device doesn't exist,
//              false - otherwise
//------------------------------------------------------------------------------

bool_t CdcHecInquireStateOfAdjacentSend(const bool_t isAtOutput, const uint8_t inputPortIndex)
{
    SI_CpiData_t cdcInquireStateMsg;
    uint16_t firstPhysAddr, lastPhysAddr;

    firstPhysAddr = gpInstRec->physAddr;
    lastPhysAddr  = CdcAdjacentPhysAddrGet(gpInstRec->physAddr, isAtOutput, inputPortIndex);

    if (lastPhysAddr == 0xFFFF)
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHecInquireStateOfAdjacentSend(): adjacent device doesn't exist\n"));
        return true; // error
    }

    cdcInquireStateMsg.args[3]      = (firstPhysAddr >> 8) & 0xFF;
    cdcInquireStateMsg.args[4]      = firstPhysAddr & 0xFF;
    cdcInquireStateMsg.args[5]      = (lastPhysAddr >> 8) & 0xFF;
    cdcInquireStateMsg.args[6]      = lastPhysAddr & 0xFF;

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HEC_INQUIRE_STATE, 7, &cdcInquireStateMsg);

    return false; // no error

}

//------------------------------------------------------------------------------
// Function:    CdcHecReportStateSend
// Description: Sends <CDC HEC Report State> in response for Discover,
//              IquireState or SetState message.
//              This function is to be used by all devices.
// Parameters:  targetPhysAddr  - physical address of the device to send the report.
//                                If the physical address is 0xFFFF, the message
//                                will be send out as broadcast notification
//                                with "HEC State" and "HEC Support Field included"
//              reportStateMode - selection of short, discover or multiVHEC report
//------------------------------------------------------------------------------

void CdcHecReportStateSend(const uint16_t targetPhysAddr, const CdcReportStateMode_t reportStateMode)
{
    uint8_t  i;
    SI_CpiData_t cdcReportStateMsg;
    uint8_t  hecState;
    uint8_t  argCount;
    uint16_t hecExtraField;

    // Target Physical Address - where this report is going.
    cdcReportStateMsg.args[3]      = (targetPhysAddr >> 8) & 0xFF;
    cdcReportStateMsg.args[4]      = targetPhysAddr & 0xFF;

    if (reportStateMode == CDC_REPORT_STATE_DISCOVER_MODE || targetPhysAddr == 0xFFFF) // HEC support field
    {
        hecExtraField = 0;
        if (gpInstRec->ports.numberOfOutputs > 0)
        {
            hecExtraField = gpInstRec->ports.output.hecSupport << 14; // HEC (eHDMI) support for the output
        }

        for (i = 0; i < gpInstRec->ports.numberOfInputs; i++)
        {
            hecExtraField |= gpInstRec->ports.input[i].hecSupport << (13 - i); // HEC support bits for inputs
        }

        cdcReportStateMsg.args[6] = (hecExtraField >> 8) & 0xFF;
        cdcReportStateMsg.args[7] = hecExtraField & 0xFF;
        argCount = 8;

    }
    else if (reportStateMode == CDC_REPORT_STATE_MULTIVHEC_MODE) // HEC activation field
    {
        gpInstRec->deviceStatus.hecFuncState = CDC_HEC_ACTIVATION_FLD;

        hecExtraField = 0;
        if (gpInstRec->ports.numberOfOutputs > 0)
        {
            hecExtraField = gpInstRec->ports.output.hecActive << 14; // HEC activation bit for the output
        }

        for (i = 0; i < gpInstRec->ports.numberOfInputs; i++)
        {
            hecExtraField |= gpInstRec->ports.input[i].hecActive << (13 - i); // HEC activation bit for inputs
        }

        cdcReportStateMsg.args[6] = (hecExtraField >> 8) & 0xFF;
        cdcReportStateMsg.args[7] = hecExtraField & 0xFF;
        argCount = 8;
    }
    else // short mode
    {
        argCount = 6;
    }

    hecState = (gpInstRec->deviceStatus.hecFuncState  << 6) |
               (gpInstRec->deviceStatus.hostFuncState << 4) |
               (gpInstRec->deviceStatus.encFuncState  << 2) |
                gpInstRec->deviceStatus.cdcErrorState;

    cdcReportStateMsg.args[5]      = hecState;

    gpInstRec->deviceStatus.cdcErrorState = CDC_ERRCODE_NO_ERROR; // clear error status after report

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HEC_REPORT_STATE, argCount, &cdcReportStateMsg);

}



//------------------------------------------------------------------------------
// Function:    CdcHecSetStateSend
// Description: Sends <CDC HEC SetState> in order to activate from 1 up to 4 VHECs
//              or deactivate 1 AHEC
//              This function is to be used by Activator.
// Parameters:  hecActivState - type of a state to set (Activate or Deactivate),
//              numberOfHecs  - minimum 1 and maximum 4 HECs to activate,
//                              it must be 1 for deactivation,
//              aHecIdx       - array of indexes of HECs in the Instance Record
//------------------------------------------------------------------------------

void CdcHecSetStateSend(const CdcHecActivation_t hecActivState,
                        const uint8_t numberOfHecs,
                        const uint8_t aHecIdx[])
{
    uint8_t i,j;
    SI_CpiData_t cdcSetStateMsg;
    uint16_t firstPhysAddr, lastPhysAddr;
    bool_t errFlag = false;

    // Check for necessary conditions required to carry out this operation

    if (numberOfHecs < 1 || numberOfHecs > 4 || (numberOfHecs > 1 && hecActivState != CDC_HEC_ACTIVE))
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHecSetStateSend(): invalid number of HECs\n"));
        return;
    }

    // Check appropriateness of HEC channel state for the operation
    if (hecActivState == CDC_HEC_ACTIVE)
    {
        for (i = 0; i < numberOfHecs; i++)
        {
            if (gpInstRec->aHec[aHecIdx[i]].status.channelState != CDC_VHEC) // activation is only allowed for VHEC
            {
                errFlag = true;
                break;
            }
        }
    }
    else
    {
        for (i = 0; i < numberOfHecs; i++)
        {
            if (gpInstRec->aHec[aHecIdx[i]].status.channelState == CDC_PHEC) // deactivation is allowed for AHEC & VHEC
            {
                errFlag = true;
                break;
            }
        }
    }

    if (errFlag)
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHecSetStateSend(): invalid HEC channel state\n"));
        return;
    }


    firstPhysAddr = gpInstRec->aHec[aHecIdx[0]].firstDevPhysAddr;
    lastPhysAddr  = gpInstRec->aHec[aHecIdx[0]].lastDevPhysAddr;

    cdcSetStateMsg.args[3] = (firstPhysAddr >> 8) & 0xFF;
    cdcSetStateMsg.args[4] = firstPhysAddr & 0xFF;
    cdcSetStateMsg.args[5] = (lastPhysAddr >> 8) & 0xFF;
    cdcSetStateMsg.args[6] = lastPhysAddr & 0xFF;

    cdcSetStateMsg.args[7] = hecActivState; // HEC set state

    j = 7;
    for (i = 1; i < numberOfHecs; i++)  // Physical Addresses of additional terminators of the HECs
    {
        lastPhysAddr  = gpInstRec->aHec[aHecIdx[i]].lastDevPhysAddr;

        cdcSetStateMsg.args[++j] = (lastPhysAddr >> 8) & 0xFF;
        cdcSetStateMsg.args[++j] = lastPhysAddr & 0xFF;
    }

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HEC_SET_STATE, j + 1, &cdcSetStateMsg);

}



//------------------------------------------------------------------------------
// Function:    CdcHecSetStateAdjacentSend
// Description: Sends <CDC HEC SetState> in order to activate from 1 up to 4 VHECs
//              or deactivate 1 AHEC
//              This function is to be used by Activator.
// Parameters:  hecActivState - type of a state to set (Activate or Deactivate),
//              adjHecIdx     - index of a HEC to be activated or deactivated
//------------------------------------------------------------------------------

void CdcHecSetStateAdjacentSend(const CdcHecActivation_t hecActivState, const uint8_t adjHecIdx)
{
    SI_CpiData_t cdcSetStateAdjacentMsg;
    uint16_t lastPhysAddr;

    lastPhysAddr  = gpInstRec->aHec[adjHecIdx].lastDevPhysAddr;


    cdcSetStateAdjacentMsg.args[3] = (lastPhysAddr >> 8) & 0xFF;
    cdcSetStateAdjacentMsg.args[4] = lastPhysAddr & 0xFF;
    cdcSetStateAdjacentMsg.args[5] = hecActivState; // HEC set state

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HEC_SET_STATE_ADJACENT, 6, &cdcSetStateAdjacentMsg);

}



//------------------------------------------------------------------------------
// Function:    CdcHecDeactivationRequestSend
// Description: Sends <CDC HEC RequestDeactivation> to ask Activator of an AHEC
//              to deactivate it.
//              This function is to be used by an AHEC member addressing to
//              the HEC's Activator.
// Parameters:  HecIdx - index of an AHEC to be deactivated
//------------------------------------------------------------------------------

void CdcHecDeactivationRequestSend(const uint8_t hecIdx)
{
    SI_CpiData_t cdcRequestDeactivationMsg;
    uint16_t actPhysAddr;
    uint16_t firstPhysAddr;
    uint16_t lastPhysAddr;

    // Check for necessary conditions required to carry out this operation

    if (gpInstRec->aHec[hecIdx].status.channelState != CDC_AHEC) // Channel must be in active state
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHecDeactivationRequestSend(): state of HEC must be active\n"));
        return;
    }

    if(gpInstRec->aHec[hecIdx].status.isActivator == true)
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHecDeactivationRequestSend(): device must not be an Activator\n"));
        return;
    }

    actPhysAddr    = gpInstRec->aHec[hecIdx].activatorPhysAddr;
    firstPhysAddr  = gpInstRec->aHec[hecIdx].firstDevPhysAddr;
    lastPhysAddr   = gpInstRec->aHec[hecIdx].lastDevPhysAddr;

    cdcRequestDeactivationMsg.args[3] = (actPhysAddr >> 8) & 0xFF;   // PA of Activator
    cdcRequestDeactivationMsg.args[4] =  actPhysAddr & 0xFF;
    cdcRequestDeactivationMsg.args[5] = (firstPhysAddr >> 8) & 0xFF; // PA of first terminator
    cdcRequestDeactivationMsg.args[6] =  firstPhysAddr & 0xFF;
    cdcRequestDeactivationMsg.args[7] = (lastPhysAddr >> 8) & 0xFF;  // PA of last terminator in the HEC
    cdcRequestDeactivationMsg.args[8] =  lastPhysAddr & 0xFF;

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HEC_REQUEST_DEACTIVATION, 9, &cdcRequestDeactivationMsg);

}



//------------------------------------------------------------------------------
// Function:    CdcHecNotifyAliveSend
// Description: Sends <CDC HEC NotifyAlive> to keep AHECs active.
//              This function is to be used by an AHEC members
//------------------------------------------------------------------------------

void CdcHecNotifyAliveSend(void)
{
    SI_CpiData_t cdcNotifyAliveMsg;

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HEC_NOTIFY_ALIVE, 3, &cdcNotifyAliveMsg);

}



//------------------------------------------------------------------------------
// Function:    CdcHpdSetStateSend
// Description: Sends <CDC HPD SetState> over input of a sink device
//              This function substitutes physical HPD pin signalling.
//              To be used by a Sink device.
// Parameters:  inputPortIndex  - zero based index of an input port that
//                              state will be sent
//------------------------------------------------------------------------------

void CdcHpdSetStateSend(const uint8_t inputPortIndex)
{
    SI_CpiData_t cdcHpdSetStateMsg;
    CdcHpdState_t hpdState;

    // Check for necessary conditions required to carry out this operation

    if (inputPortIndex >= gpInstRec->ports.numberOfInputs)
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHpdSetStateSend(): invalid input port index\n"));
        return;
    }

    hpdState = gpInstRec->ports.input[inputPortIndex].hpdState;
    cdcHpdSetStateMsg.args[3] = ((inputPortIndex << 4) & 0xF0) | (hpdState & 0x0F);

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HPD_SET_STATE, 4, &cdcHpdSetStateMsg);

}



//------------------------------------------------------------------------------
// Function:    CdcHpdReportStateSend
// Description: Sends <CDC HPD ReportState> over input of a sink device
//              This function substitutes physical HPD pin signaling
//              To be used by a Source device.
//------------------------------------------------------------------------------

void CdcHpdReportStateSend(void)
{
    SI_CpiData_t cdcHpdReportStateMsg;
    CdcHpdState_t hpdState;
    CdcHpdErrCode_t hpdErrCode;

    // Check for necessary conditions required to carry out this operation

    if (gpInstRec->ports.numberOfOutputs == 0)
    {
        //DEBUG_PRINT(MSG_ALWAYS, ("ERROR in CdcHpdReportStateSend(): root device can't report HPD state\n"));
        return;
    }

    hpdState = gpInstRec->ports.output.hpdState;
    hpdErrCode = gpInstRec->ports.output.hpdErrorState;

    gpInstRec->ports.output.hpdErrorState = CDC_HPD_ERRCODE_NO_ERROR; // clear error status after report

    cdcHpdReportStateMsg.args[3] = ((hpdState << 4) & 0xF0) | (hpdErrCode & 0x0F);

    //Send CDC message over CPI
    CdcCpiWrite(CDCOP_HPD_REPORT_STATE, 4, &cdcHpdReportStateMsg);

}

#endif
