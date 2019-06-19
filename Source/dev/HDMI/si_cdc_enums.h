//***************************************************************************
//!file     si_cdc_enums.h
//!brief    HDMI/HEAC Spec v1.4 command and operand codes.
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/
#ifndef __SI_CDC_ENUMS_H__
#define __SI_CDC_ENUMS_H__


typedef enum                    // CDC opcodes
{
    CDCOP_HEC_INQUIRE_STATE         = 0x00,
    CDCOP_HEC_REPORT_STATE          = 0x01,
    CDCOP_HEC_SET_STATE_ADJACENT    = 0x02,
    CDCOP_HEC_SET_STATE             = 0x03,
    CDCOP_HEC_REQUEST_DEACTIVATION  = 0x04,
    CDCOP_HEC_NOTIFY_ALIVE          = 0x05,
    CDCOP_HEC_DISCOVER              = 0x06,
    CDCOP_HPD_SET_STATE             = 0x10,
    CDCOP_HPD_REPORT_STATE          = 0x11
        
} CdcOpcode_t;


typedef enum                    // HEC channel state
{
    CDC_PHEC    = 0x00,
    CDC_VHEC    = 0x01,
    CDC_AHEC    = 0x02
    
} CdcHecChannelState_t;

typedef enum                    // HEC activation status
{
    CDC_HEC_INACTIVE    = 0x00,
    CDC_HEC_ACTIVE      = 0x01
    
} CdcHecActivation_t;


typedef enum                    // HEC support
{
    CDC_HEC_NOT_SUPPORTED   = 0x00,
    CDC_HEC_SUPPORTED       = 0x01
    
} CdcHecSupport_t;


typedef enum                    // ARC support
{
    CDC_ARC_NOT_SUPPORTED   = 0x00,
    CDC_ARC_SUPPORTED       = 0x01
    
} CdcArcSupport_t;


typedef enum                    // HEC functionality state
{
    CDC_HEC_FUNC_NOT_SUPPORTED      = 0x00,
    CDC_HEC_STATE_INACTIVE          = 0x01,
    CDC_HEC_STATE_ACTIVE            = 0x02,
    CDC_HEC_ACTIVATION_FLD          = 0x03
    
} CdcHecFuncState_t;


typedef enum                    // Host functionality state
{
    CDC_HOST_NOT_SUPPORTED  = 0x00,
    CDC_HOST_STATE_INACTIVE = 0x01,
    CDC_HOST_STATE_ACTIVE   = 0x02
    
} CdcHostFuncState_t;


typedef enum                    // External Network Connection (ENC) functionality state
{
    CDC_ENC_NOT_SUPPORTED   = 0x00,
    CDC_ENC_STATE_INACTIVE  = 0x01,
    CDC_ENC_STATE_ACTIVE    = 0x02
    
} CdcEncFuncState_t;


typedef enum                    // HPD states
{
    CDC_HPD_CP_AND_EDID_DISABLE             = 0x00,
    CDC_HPD_CP_AND_EDID_ENABLE              = 0x01,
    CDC_HPD_CP_AND_EDID_DISABLE_ENABLE      = 0x02,
    CDC_HPD_EDID_DISABLE                    = 0x03,
    CDC_HPD_EDID_ENABLE                     = 0x04,
    CDC_HPD_EDID_DISABLE_ENABLE             = 0x05,
    CDC_HPD_UNKNOWN_STATE                   = 0x07
    
} CdcHpdState_t;


typedef enum                    // CDC Error codes
{
    CDC_ERRCODE_NO_ERROR                    = 0x00,
    CDC_ERRCODE_NOT_SUPPORTED_CAPABILITY    = 0x01,
    CDC_ERRCODE_INCOMPATIBLE_STATE          = 0x02,
    CDC_ERRCODE_OTHER_ERROR                 = 0x03
    
} CdcErrCode_t;

typedef enum                    // CDC_HPD Error codes
{
    CDC_HPD_ERRCODE_NO_ERROR                    = 0x00,
    CDC_HPD_ERRCODE_NOT_SUPPORTED_CAPABILITY    = 0x01,
    CDC_HPD_ERRCODE_INCOMPATIBLE_STATE          = 0x02,
    CDC_HPD_ERRCODE_OTHER_ERROR                 = 0x03,
    CDC_HPD_ERRCODE_NO_ERROR_NO_VIDEO           = 0x04
    
} CdcHpdErrCode_t;


typedef enum                    // CDC Report State mode
{
    CDC_REPORT_STATE_SHORT_MODE         = 0x00,
    CDC_REPORT_STATE_DISCOVER_MODE      = 0x01,    
    CDC_REPORT_STATE_MULTIVHEC_MODE     = 0x02
    
} CdcReportStateMode_t;



typedef enum                    // CDC tasks FSM states
{
    CDC_TASK_EMPTY_STATE_NONE                       = 0x00,

    CDC_TASK_FIND_STATE_DISCOVERY_START             = 0x01,
    CDC_TASK_FIND_STATE_GATHER_RESPONSES            = 0x02,    
    CDC_TASK_FIND_STATE_REPORT_RESULTS              = 0x03,

    CDC_TASK_CONNECT_STATE_INIT                     = 0x10,
    CDC_TASK_CONNECT_STATE_INQUIRE_START            = 0x11,
    CDC_TASK_CONNECT_STATE_GATHER_INQ_REP           = 0x12,
    CDC_TASK_CONNECT_STATE_ACTIVATE_START           = 0x13,
    CDC_TASK_CONNECT_STATE_GATHER_ACT_REP           = 0x14,
    CDC_TASK_CONNECT_STATE_REMOVE_FAILED            = 0x15,
    CDC_TASK_CONNECT_STATE_REPORT_RESULTS           = 0x16,

    CDC_TASK_CONNECT_ADJ_STATE_INIT                 = 0x20,
    CDC_TASK_CONNECT_ADJ_STATE_ACTIVATE_START       = 0x21,
    CDC_TASK_CONNECT_ADJ_STATE_WAIT_ACT_REP         = 0x22,
    CDC_TASK_CONNECT_ADJ_STATE_REPORT_RESULTS       = 0x23,

    CDC_TASK_DISCONNECT_STATE_DEACTIVATE            = 0x30,
    CDC_TASK_DISCONNECT_STATE_REPORT_RESULTS        = 0x31,

    CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE_INIT   = 0x40,
    CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE        = 0x41,
    CDC_TASK_DISCONNECT_ALL_STATE_DEACTIVATE_WAIT   = 0x42,
    CDC_TASK_DISCONNECT_ALL_STATE_REPORT_RESULTS    = 0x43,
    
    CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE_INIT  = 0x50,
    CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE       = 0x51,
    CDC_TASK_DISCONNECT_LOST_STATE_DEACTIVATE_WAIT  = 0x52,
    CDC_TASK_DISCONNECT_LOST_STATE_REPORT_RESULTS   = 0x53,
    
    CDC_TASK_HPD_SIGNAL_STATE_START                 = 0x60,
    CDC_TASK_HPD_SIGNAL_STATE_RESPONSE_WAIT         = 0x61,    
    CDC_TASK_HPD_SIGNAL_STATE_REPORT_RESULTS        = 0x62,

    CDC_TASK_HPD_CAPAB_STATE_START                  = 0x70,
    CDC_TASK_HPD_CAPAB_STATE_RESPONSE_WAIT          = 0x71,    
    CDC_TASK_HPD_CAPAB_STATE_REPORT_RESULTS         = 0x72    
    
} CdcTaskState_t;


#endif //__SI_CDC_ENUMS_H__
