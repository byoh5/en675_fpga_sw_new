//***************************************************************************
//!file     si_apiCDC.h
//!brief    Top level CDC operation API
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/
#ifndef __SI_API_CDC_H__
#define __SI_API_CDC_H__

#include "si_apiConfigure.h"
#include "si_cdc_heac.h"
#include "si_apiCpi.h"


//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------
#define NUMBER_OF_DEVICES                    1  // Can be more than 1 for HDMI repeater with
                                                // multiple HDMI outputs

// CDC Tasks
#define CDC_TASK_BASE_SERVICE           0x0001  // sends/checks Alive messages, responds to commands and requests
#define CDC_TASK_HOSTS_SEARCH           0x0002  // searches for all HEC capable devices
#define CDC_TASK_CONNECT                0x0004  // establishes a connection to HEC capable devices
#define CDC_TASK_CONNECT_ADJACENT       0x0008  // establishes a connection to an ajacent HEC capable devices
#define CDC_TASK_DISCONNECT             0x0010  // dismisses connections to HEC capable devices
#define CDC_TASK_DISCONNECT_ALL         0x0020  // dismisses all open HECs
#define CDC_TASK_DISCONNECT_LOST        0x0040  // dismisses all HECs that include a lost device
#define CDC_TASK_HPD_SIGNAL             0x0080  // delivers HPD status from Sink to Source device
#define CDC_TASK_HPD_CAPAB_VERIF        0x0100  // verifies HPD-over-CDC capability

// CDC Feedback Messages
#define CDC_FB_MSG_NONE                 0x0000  // default value: no messages
#define CDC_FB_MSG_HST_SEARCH_DONE      0x0001  // Hosts search task finished
#define CDC_FB_MSG_CONNECT_DONE         0x0002  // Connect task finished
#define CDC_FB_MSG_CONNECT_ADJ_DONE     0x0003  // Connect to Adjacent device task finished
#define CDC_FB_MSG_DISCONNECT_DONE      0x0004  // Disonnect task finished
#define CDC_FB_MSG_DISCONNECT_ALL_DONE  0x0005  // Disonnect All task finished
#define CDC_FB_MSG_DISCONNECT_LOST_DONE 0x0006  // Disonnect Lost Device task finished
#define CDC_FB_MSG_HPD_SIGNAL_DONE      0x0007  // HPD signalling task finished
#define CDC_FB_MSG_HPD_STATE_CHANGED    0x0008  // HPD state of a Source has been changed by Sink device
#define CDC_FB_MSG_HPD_CAPABILITY_CONF  0x0009  // HPD capability has been confirmed
#define CDC_FB_MSG_CAPABILITY_CHANGED   0x000a  // One of Devices in the HDMI network has changed its capability state
#define CDC_FB_MSG_ERROR                0x00E0  // General error occured
#define CDC_FB_MSG_ERR_NONCDC_CMD       0x00E1  // Non CDC command received
#define CDC_FB_MSG_ERR_HPD_SIGNAL       0x00E2  // HDMI Source not responded
#define CDC_FB_MSG_ERR_HPD_CAP_FAILED   0x00E3  // Failed to verify HDP-over-CDC capability

// CDC Tasks related time constants
#define CDC_MAX_RESPONSE_TIME_MS          1500  // milliseconds to wait before ending of each response gathering (required maximum = 1s, exception may apply)
#define CDC_MAX_MONITORING_TIME_SEC          5  // seconds to timeout event while collecting all responses (total maximum waiting time)
#define CDC_MIN_DISCOVERY_REP_TIME_SEC     180  // seconds (3 minutes) before repetitive discovery command can be issued
#define CDC_MSG_OUT_TIME_MS                300  // milliseconds to wait until outgoing message presumably is sent
#define CDC_MIN_RECALL_TIME_MS             100  // shortest CDC task handler recall time in milliseconds
#define CDC_MAX_RECALL_TIME_MS           20000  // longest CDC task handler recall time in milliseconds


// CDC Task mask
#define CDC_TASK_MASK_ALL_UNMASKED      0xFFFF  // default value of the Task mask allowing any tasks to start
#define CDC_TASK_MASK_ALL_BUT_BASE      0xFFFE  // mask for all tasks except the Base service task

//------------------------------------------------------------------------------
// Data type definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Top level function prototypes
//------------------------------------------------------------------------------
void     SI_CdcDeviceSelect(uint16_t deviceIndex);
CdcInstanceRecord_t *SI_CdcDeviceInstanceGet(void);

//void     SI_CdcConfig(uint16_t physAddr, CdcIoConfig_t *pIoPorts,
void     SI_CdcConfig(uint16_t physAddr,
                      bool_t hecCapability, bool_t hostCapability, bool_t extNetCapability);
void     SI_CdcDeviceHecSupEnable(const bool_t isEnabled);
void     SI_CdcDeviceHostStateSet(const CdcHostFuncState_t state);
void     SI_CdcDeviceEncStateSet(const CdcEncFuncState_t state);
void     SI_CdcPortHecSupEnable(const bool_t isOutput, const uint8_t inputPortIndex, const bool_t isEnabled);


uint8_t  SI_CdcSrvHostsFind(void);
bool_t   SI_CdcDeviceConnect(bool_t isConnectEntireList, uint16_t firstPhysAddr, uint16_t lastPhysAddr);
bool_t   SI_CdcDeviceAdjacentConnect(uint16_t adjacentPhysAddr);
bool_t   SI_CdcDeviceDisconnect(bool_t isDisconnectAll, uint16_t actPhysAddr, uint16_t firstPhysAddr, uint16_t lastPhysAddr);
bool_t   SI_CdcDeviceAdjacentDisconnect(uint16_t adjacentPhysAddr);


bool_t   SI_CdcSrvStart(void);
void     SI_CdcSrvStop(void);

bool_t   SI_CdcPortIsActive(const bool_t isOutput, const uint8_t portIndex);
bool_t   SI_CdcHpdSetState(uint8_t inputPortIndex, CdcHpdState_t hpdState);
bool_t   SI_CdcHpdCapabilityInquire(uint8_t inputPortIndex);
CdcHpdState_t SI_CdcHpdStateGet(void);

void     SI_CdcTaskInit(void);
uint8_t  SI_CdcTaskProcess(uint16_t sysTimerCountMs, SI_CpiData_t *pCdcMessage);
uint16_t SI_CdcNextInvocationTimeGet(uint16_t sysTimerCountMs);


#endif //__SI_API_CDC_H__

