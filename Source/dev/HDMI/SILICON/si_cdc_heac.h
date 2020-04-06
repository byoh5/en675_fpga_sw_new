//***************************************************************************
//!file     si_cdc_heac.h
//!brief    Low level CDC operation API
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/
#ifndef __SI_CDC_HEAC_H__
#define __SI_CDC_HEAC_H__

#include "si_apiConfigure.h"
#include "si_cdc_enums.h"

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

#define CDC_NUMBER_OF_HECS_PAS_MAX      6   // Maximum number of HECs the device can participate passively
#define CDC_NUMBER_OF_HECS_ACT_MAX      3   // Maximum number of HECs where the device plays as an activator
#define CDC_NUMBER_OF_HECS_MAX          (CDC_NUMBER_OF_HECS_PAS_MAX + CDC_NUMBER_OF_HECS_ACT_MAX) // Maximum number of HECs (<256)

#define CDC_NUMBER_OF_NODES_MAX         3   // Maximum number of path nodes in an HEC not including terminators
#define CDC_EHDMI_INPUTS_MAX            5   // Maximum number of HDMI inputs per device

// Maximum number of different physical addresses in all HECs
// to be monitored by this device (as activator and passive node)
#define CDC_NUMBER_OF_DEVICES_MAX       (CDC_NUMBER_OF_HECS_PAS_MAX +\
                                         CDC_NUMBER_OF_HECS_ACT_MAX * (CDC_NUMBER_OF_NODES_MAX + 2))

#define CDC_NUMBER_OF_FOUND_HOSTS_MAX   CDC_NUMBER_OF_DEVICES_MAX   // Maximum number of devices to be found as a result of a search

#define CDC_CEC_OPCODE                  0xF8
#define CDC_CEC_SRCDEST_ADDR_DEFAULT    0xFF

// Time related definitions
#define CDC_TIME_TCK_MS                 4                               // number of milliseconds in one tick (definition of the "tick")
#define CDC_TIME_MS2TCK(t)              ((t) / CDC_TIME_TCK_MS)         // Converts time in ms to number of counter ticks
#define CDC_TIME_SEC2TCK(t)             ((t) * (1000 / CDC_TIME_TCK_MS))// Converts time in seconds to number of counter ticks
#define CDC_TIME_TCK2SEC(t)             ((t) / (1000 / CDC_TIME_TCK_MS))// Converts time in number of counter ticks to seconds

#define CDC_MAX_NTF_ALIVE_TIME_SEC      140  // threshold time for a Watchdog (devices shall keep sending NotifyAlive messages within this time)


//------------------------------------------------------------------------------
// Data type definitions
//------------------------------------------------------------------------------

typedef struct          // Time counter structure
{
    uint16_t  tRov : 2;    // number system timer overflow events ( = log_2(CDC_TIME_TCK_MS))
    uint16_t  tRes : 14;   // residual time count (in ticks)      ( = 16 - log_2(CDC_TIME_TCK_MS))

} CdcTimeCounter_t;


typedef struct          // HEC record status
{
    uint8_t             isInUse      : 1;      // true, if the HEC record is not vacant
    uint8_t             isActivator  : 1;      // true, if the device is Activator of this HEC
    uint8_t             isAdjacent   : 1;      // true, if AHEC is established by control to adjacent device
    uint8_t             channelState : 2;      // channel state of the HEC (PHEC,AHEC,VHEC)  [CdcHecChannelState_t]

} CdcHecRecStatus_t;


typedef struct          // HEC descriptor
{
    CdcHecRecStatus_t   status;               // HEC record status flags
    uint16_t            activatorPhysAddr;    // PA of the device activated this HEC
    uint16_t            firstDevPhysAddr;     // PA of first terminating device
    uint16_t            lastDevPhysAddr;      // PA of last terminating device

} CdcHecDescriptor_t;


typedef struct          // Port (input/output) capability/status
{
    uint8_t             hecSupport      : 1; // HEC support by the port flag                        [CdcHecSupport_t]
    uint8_t             hecActive       : 1; // AHEC operation over the port flag                   [CdcHecActivation_t]
    uint8_t             hpdState        : 3; // latest sent/received HPD state                      [CdcHpdState_t]
    uint8_t             hpdErrorState   : 3; // CDC_HPD error code in case of failed HPD operation  [CdcHpdErrCode_t]

} CdcPort_t;


typedef struct          // Decriptor of I/O ports configuration
{
    uint8_t             numberOfInputs;
    uint8_t             numberOfOutputs; // 0 (root) or 1
    CdcPort_t           input[CDC_EHDMI_INPUTS_MAX]; // Input ports capability
    CdcPort_t           output; // output port capability

} CdcIoConfig_t;


typedef struct          // Bitfield reflecting states and capability of device
{
    uint8_t             hecFuncState    : 2; // HEC functional capability/state                 [CdcHecFuncState_t]
    uint8_t             hostFuncState   : 2; // host capability/state                           [CdcHostFuncState_t]
    uint8_t             encFuncState    : 2; // external network connection capability/state    [CdcEncFuncState_t]
    uint8_t             cdcErrorState   : 2; // CDC error code in case of failed operation      [CdcErrCode_t]

} CdcDeviceStatus_t;


typedef struct          // Table entry for HEC Watchdog to monitor Alive status of devices
{
    uint16_t            physAddr;       // Physical Address of a Device
    CdcTimeCounter_t    TimeCount;      // Time counter to generate timeouts

} CdcPhysAddrTime_t;


typedef struct
{
    uint8_t             isMultHecMode : 1; // Multi HEC Activation mode flag
    uint8_t             isEndOfList   : 1; // End of VHEC list to activate flag
    uint8_t             hecActDelayed : 3; // number of delayed activation events (0-4)

} CdcMultHecActData_t;


typedef struct          // cdcFoundHostsList[] entry for Discovery process
{
    uint16_t            physAddr;       // Physical Address of a Device
    CdcDeviceStatus_t   devStat;        // State and capabilities of the Device

    struct
    {
        uint16_t        isSelected     :  1;// flag indicating that the Device has been selected (for sorting)
        uint16_t        portHecCapBits : 15;// HEC capability flags of I/O ports

    } hostInfo;

} CdcFoundListEntry_t;



typedef struct          // CDC Instance Record
{
    // -------------------- Public area -----------------------
    uint16_t            physAddr;       // current Physical Address of the device
    CdcIoConfig_t       ports;          // IO configuration and capability
    CdcHecDescriptor_t  aHec[CDC_NUMBER_OF_HECS_MAX]; // HEC records
    uint8_t             numberOfHecs;   // number of HECs in the record
    CdcDeviceStatus_t   deviceStatus;   // status & capability of this device

    // List of Device Physical Addresses found as a result of search tasks
    CdcFoundListEntry_t cdcFoundHostsList[CDC_NUMBER_OF_FOUND_HOSTS_MAX];
    uint8_t             numberOfFoundHosts;

    // -------------------- Private area -----------------------
    // Storage for time since last Alive message from participating
    // devices has been received:
    // Activators monitor all members of their AHECs except themselves.
    // Passive devices monitor Activators alive status.
    // Special PA value 0xFFFF indicates vacant cells
    uint8_t             numberOfAllHosts; // number of all found devices including ENC or Host incapable

    uint16_t            sysTimerCountCurMs;// current reading of the system timer
    uint16_t            sysTimerCountPrvMs;// previous reading of the system timer
    CdcTimeCounter_t    cdcTotalMonTimeCount; // Time counter for total monitoring time metering
    CdcTimeCounter_t    cdcDiscoveryTimeCount;// Time counter for minimal Discovery repetition time rule enforcement
    CdcTimeCounter_t    cdcRecallTimeCount;   // estimeted time point of the task handler next recall

    CdcPhysAddrTime_t   aAliveDevice[CDC_NUMBER_OF_DEVICES_MAX];
    uint8_t             numberOfAliveDev; // number of nonempty records in aAliveDevice[] list
    uint16_t            aliveDevPhysAddr; // PA of a current Device from AliveDevice List the task works with
    CdcTimeCounter_t    aliveNtfDelayCount; // Time delay before sending NotifyAlive msg

    // CDC Task states
    uint16_t            cdcTaskFlags;     // Bit fields indicating active CDC tasks
    uint16_t            cdcTaskMask;      // Bitmask selecting currently active CDC task
    CdcTimeCounter_t    cdcTaskTimeCount; // Time counter for waiting within tasks
    CdcTaskState_t      cdcTaskState;     // task current state (combined)

    // HEC status assignment states
    uint8_t             currentHecIdx;    // index of a current HEC the task works with
    uint8_t             verifiedDevNum[4];// number of verified devices for HEC status change
    uint8_t             currentPortIdx;   // index of a current input port for HPD signalling task
    CdcMultHecActData_t connectSettings;  // Multi HEC activation mode settings
    uint8_t             delayedHecIdx[4]; // Up to 3 delayed and 1 current HEC (for Multi-HEC activation)

} CdcInstanceRecord_t;


//------------------------------------------------------------------------------
// Low level function prototypes
//------------------------------------------------------------------------------



// Low level API functions

void     CdcHecDiscoverSend(void);
void     CdcHecInquireStateSend(const uint8_t hecIdx);
bool_t   CdcHecInquireStateOfAdjacentSend(const bool_t isAtOutput, const uint8_t inputPortIndex);
void     CdcHecReportStateSend(const uint16_t targetPhysAddr, const CdcReportStateMode_t reportStateMode);
void     CdcHecSetStateSend(const CdcHecActivation_t hecActivState, const uint8_t numberOfHecs, const uint8_t aHecIdx[]);
void     CdcHecSetStateAdjacentSend(const CdcHecActivation_t hecActivState, const uint8_t adjHecIdx);
void     CdcHecDeactivationRequestSend(const uint8_t hecIdx);
void     CdcHecNotifyAliveSend(void);

void     CdcHpdSetStateSend(const uint8_t inputPortIndex);
void     CdcHpdReportStateSend(void);


// Internal service functions

void     CdcHecInstanceRecordReset(void);
uint8_t  CdcHecDescriptorAdd(const bool_t isActivator);
bool_t   CdcHecDescriptorRemove(const uint8_t hecIdx);
uint8_t  CdcHecIndexGet(const uint16_t actPhysAddr, const uint16_t firstPhysAddr, const uint16_t lastPhysAddr);

bool_t   CdcAliveDeviceListAdd(const uint8_t hecIdx);
bool_t   CdcAliveDeviceListRemove(const uint8_t hecIdx);
bool_t   CdcAliveDeviceListTimeReset(const uint16_t physAddr);
uint16_t CdcAliveDeviceListTimeUpdate(void);

void     CdcTimeCounterReset(CdcTimeCounter_t *pTimeCounter);
void     CdcTimeCounterSet(const uint16_t delayTimeTck, CdcTimeCounter_t *pTimeCounter);
bool_t   CdcTimeCounterUpdate(CdcTimeCounter_t *pTimeCounter);
bool_t   CdcIsTimeCounterExpired(CdcTimeCounter_t *pTimeCounter);
uint16_t CdcTimeCounterLeftSecGet(CdcTimeCounter_t *pTimeCounter);
uint8_t  CdcTimeCounterCompare(CdcTimeCounter_t *pTimeCounterA, CdcTimeCounter_t *pTimeCounterB);


bool_t   CdcIsDeviceBelongsToHec(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr, const uint16_t testPhysAddr);
uint16_t CdcHecNextDeviceGet(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr, const bool_t restart);
uint8_t  CdcHecLengthGet(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr);
bool_t   CdcIsLinkedDirectly(const uint16_t sinkPhysAddr, const uint16_t sourcePhysAddr, const uint8_t inputPortIndex);
uint16_t CdcAdjacentPhysAddrGet(const uint16_t basePhysAddr, const bool_t isAtOutput, const uint8_t inputPortIndex);
bool_t   CdcIsCapableToAdjacent(const uint16_t initiatorPhysAddr, const uint16_t adjacentPhysAddr, bool_t *isAdjacent);
bool_t   CdcCapablePortGet(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr, uint8_t aInPortIdx[2], bool_t *pOutPortFlg);
bool_t   CdcIsCapableForHec(const uint16_t firstPhysAddr, const uint16_t lastPhysAddr, const bool_t isStrict);
void     CdcIoPortsActivate(const uint8_t aInPortIdx[2], const bool_t outPortFlg);
void     CdcIoPortsStatusUpdate(void);
bool_t   CdcIsHecEnabledInDev(const uint8_t hecIdx, const uint16_t devPhysAddr, const uint16_t portBitField);
void     CdcDiscoveryDataArrange(void);

bool_t   CdcEhdmiHwEnable(const bool_t isEnabled);
bool_t   CdcEhdmiIoPortHwEnable(const bool_t isOutput, const uint8_t inPortIdx, const bool_t isEnabled);

uint32_t mRand(uint32_t seed);



#endif //__SI_CDC_HEAC_H__
