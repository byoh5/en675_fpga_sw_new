//***************************************************************************
//!file     si_apiConfigure.h
//!brief    Configure the SiI938x API for a specific system design.
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef __SI_APICONFIGURE_H__
#define __SI_APICONFIGURE_H__
//#include <../CEC/si_datatypes.h>
#include "si_datatypes.h"

//#include <si_regio.h>

//------------------------------------------------------------------------------
//  Compile-time build options
//------------------------------------------------------------------------------

//#define USE_INTERNAL_MUTE       0       // Set to 1 to use SiI9387 muting
//#define DO_EDID_INIT            1       // Set to 1 to perform EDID initialization during boot process
#define API_DEBUG_CODE			0       // Set to 0 to eliminate debug print messages from code
                                        // Set to 1 to allow debug print messages in code
//#define FPGA_DEBUG_CODE         0       // Set to 0 to for normal operation
                                        // Set to 1 to to work with FPGA test board
//#define USE_HW_HDCPGOOD         1       // Set to 0 to use FW to make HDCP GOOD decision
                                        // Set to 1 to use HW to make HDCP GOOD decision
//#define CHIP_REVISION           ES10    // Set according to silicon revision
//#define MHD_WORKAROUND          0       // Set to 1 to enable MHD workaround

//#define DEM_POLLING_DELAY       100     // Main Loop Polling interval (ms)

#define IS_RX                   0       // Set to 1 to if code is for Sink device
#define IS_TX                   1       // Set to 1 to if code is for Source device


    /* The following flags determine what feature set is compiled.  */

#define IS_PACKET               0       // Set to 1 to enable VSIF Packet capture support
#define IS_VIDEO                0       // Set to 1 to enable Video Resolution info capture support
#define IS_CEC                  1       // Set to 1 to enable CEC support
#define IS_ARC                  1       // Set to 1 to enable ARC support
#define IS_HEC                  1       // Set to 1 to enable HEC support
#define IS_CBUS                 0       // Set to 1 to enable MHD CBUS support
#define IS_CDC                  1       // Set to 1 to enable CDC support
#define IS_CDC_REDUCED          0       // Set to 1 to use reduced version of CDC
#define IS_REPEATER             0       // Set to 1 to enable repeater support


#if (IS_CDC_REDUCED == 1)   // Assume that if they asked for CDC reduced, they want CDC
    #undef IS_CDC
    #define IS_CDC  1
#endif

#if (IS_ARC && !IS_CEC)
    #error "ARC requires CEC"
#endif

#ifdef NEVER	// Temp
#if (IS_CDC && ( !IS_CEC || !IS_HEC ))
    #error "CDC requires CEC and HEC"
#endif
#endif

#ifdef NEVER
#if defined FORCE_NO_CDC
    // Defined on command line to override above settings.
    // Used in building small MCU projects

    #undef  IS_CDC
    #undef  IS_CDC_REDUCED
    #define IS_CDC          0
    #define IS_CDC_REDUCED  0
#endif
#endif

//------------------------------------------------------------------------------
//  Device-specific constants
//------------------------------------------------------------------------------

#define ES00            0
#define ES01            1
#define ES10            2

#define SII_MAX_PORTS   5

#ifdef NEVER		// %%% Evita does not use this type of debug trace
//------------------------------------------------------------------------------
// Target System specific
//------------------------------------------------------------------------------

#if (API_DEBUG_CODE==1)
    #include <stdio.h>
#endif

extern char g_strDeviceID [];

    // Debug Macros

#if (API_DEBUG_CODE==1)
    extern  uint8_t     g_halMsgLevel;
    extern  bool_t      g_cecPrint;

    #ifndef DEBUG_PRINT
        #define DEBUG_PRINT(l,x)        if (l<=g_halMsgLevel) printf x
    #endif
    #define _DEBUG_(x)              x

#define CEC_PRINT( x, y )  { if (g_cecPrint) { DEBUG_PRINT( x, y ); }}

#else
    #ifndef DEBUG_PRINT
        #define DEBUG_PRINT(l,x)
    #endif
    #define _DEBUG_(x)
#endif

#endif	//NEVER

    // HDMI port on board that is connected to ARC

#define HEAC_ARC_PORT                   1
#define HEAC_HEC_PORT                   1

    // MSG API   - For Debug purposes

#define MSG_ALWAYS              0x00
#define MSG_STAT                0x01
#define MSG_DBG                 0x02

#ifdef NEVER
    // Timer API - Target system must create these functions
#define ELAPSED_TIMER               0xFF
#define ELAPSED_TIMER1              0xFE

#define TIMER_0                     0   // DO NOT USE - reserved for TimerWait()
#define TIMER_POLLING               1   // Reserved for main polling loop
#define TIMER_2                     2   // Available
#define TIMER_3                     3   // Available
#define TIMER_SERIAL				4
#define TIMER_LINK_CHECK            5
#define TIMER_LINK_RESPONSE         6
#define	TIMER_COUNT		7
#endif


void        HalTimerSet( uint8_t timer, uint16_t m_sec );
void        HalTimerWait( uint16_t m_sec );
uint16_t    HalTimerElapsed( uint8_t index );
uint8_t     HalTimerExpired( uint8_t timer );


//void    *malloc( uint16_t byteCount );
//void    free( void *pData );

//------------------------------------------------------------------------------
// Debug Trace
//------------------------------------------------------------------------------

#if (API_DEBUG_CODE==1)
    #define DF1_SW_HDCPGOOD         0x01
    #define DF1_HW_HDCPGOOD         0x02
    #define DF1_SCDT_INT            0x04
    #define DF1_SCDT_HI             0x08
    #define DF1_SCDT_STABLE         0x10
    #define DF1_HDCP_STABLE         0x20
    #define DF1_NON_HDCP_STABLE     0x40
    #define DF1_RI_CLEARED          0x80

    #define DF2_MP_AUTH             0x01
    #define DF2_MP_DECRYPT          0x02
    #define DF2_HPD                 0x04
    #define DF2_HDMI_MODE           0x08
    #define DF2_MUTE_ON             0x10
    #define DF2_PORT_SWITCH_REQ     0x20
    #define DF2_PIPE_PWR            0x40
    #define DF2_PORT_PWR_CHG        0x80

typedef struct
{
    char    debugTraceVerStr[5];    // Changed each time the debug labels are
                                    // changed to keep track of what values
                                    // a specific version is monitoring.
    uint8_t demFlags1;
    uint8_t demFlags2;
} DEBUG_DATA_t;

extern DEBUG_DATA_t g_dbg;

#endif

#ifdef NEVER		// dup on what is in si_datatypes.h
//------------------------------------------------------------------------------
//  CEC
//------------------------------------------------------------------------------

/* Control for amount of text printed by CEC commands.  */

#define CEC_NO_TEXT         0
#define CEC_NO_NAMES        1
#define CEC_ALL_TEXT        2           // Not compatible with IS_CDC==1 for 8051
#define INCLUDE_CEC_NAMES   CEC_NO_NAMES

#if (INCLUDE_CEC_NAMES > CEC_NO_TEXT)
    #include "si_apiCpi.h"
    #include "si_cpCEC.h"
#endif
#endif

//------------------------------------------------------------------------------
//  MobileHD / CBUS
//------------------------------------------------------------------------------

#define RCP_DEBUG                       0

#define MHD_MAX_CHANNELS                2   // Number of MDHI channels

#define CBUS_CMD_RESPONSE_TIMEOUT       10      // In 100ms increments
#define CBUS_CONNECTION_CHECK_INTERVAL  40      // In 100ms increments

#define CBUS_FW_COMMAND_TIMEOUT_SECS    1       // No response for 1 second means we hold off and move
#define CBUS_FW_INTR_POLL_MILLISECS     50      // wait this to issue next status poll i2c read
#define CBUS_FW_HOLD_OFF_SECS           2       // Allow bus to quieten down when ABORTs are received.

//------------------------------------------------------------------------------
//  API_SWITCH definitions
//------------------------------------------------------------------------------

#define DELAY_MATCH_SETTLE_TIME (100  / DEM_POLLING_DELAY   )

#define DEM_MAX_POWERUP_HOTPLUG (1200 / DEM_POLLING_DELAY )     // Length of full hotplug performed after an AC power loss and restore
#define DEM_HPD_TOGGLE_TIME     (1200 / DEM_POLLING_DELAY   )   // Leave HPD low for ~200ms. Has to be larger than DEM_DVI_DET.
                                                                // Otherwise, DVI settle down after HPD comes back
#define DEM_RXTERM_TOGGLE_TIME  (DEM_HPD_TOGGLE_TIME * 3 / 4)   // Leave RXTerm disabled for shorter time.

#define DEM_SCDT_INT_DELAY      (300  / DEM_POLLING_DELAY   )   // Don't check DM range until this time after a SCDT interrupt
#define DEM_SCDT_STABLE         (500  / DEM_POLLING_DELAY   )   // Time that SCDT must remain high to be considered stable.
#define DEM_NONAUTH_STABLE      (800  / DEM_POLLING_DELAY   )   // Time that SCDT must remain high and no AUTH to be considered stable non-hdcp.
#define DEM_DVI_DET             (200  / DEM_POLLING_DELAY   )   // Time to decide DVI mode and turn off auto reset
#define DEM_POLL_PERIOD         (2500 / DEM_POLLING_DELAY   )   // Length of polling period to count DM value changes
#define DEM_ENOUGH_CHGS         (800  / DEM_POLLING_DELAY   )   // Number of DM changes in polling period to be
                                                                // considered an oscillating change.
#define DEM_AUTH_TIME           (1000 / DEM_POLLING_DELAY )     // Do not perform HPD on a port switch after this time

#define DEM_DECRYPT_ECC_THRESHOLD   (800 / DEM_POLLING_DELAY )  // Turn on ECC checking after decrypting for this period.

#define DEM_MUTE_LENGTH         (300  / DEM_POLLING_DELAY   )   // Duration of Muteduring PWR/SCDT event
#define DEM_MUTE_LEN_PORTCHANGE (300  / DEM_POLLING_DELAY   )   // Duration of Mute during switching
                                                                // 0 means no Muting while switching
#define DEM_MULTIPLIER_FOR_NONHDCP  8                           // Multiply each DEM_TMDS_OFF_xxx delay step by this
                                                                // value (except the last step, which is always 100ms).
                                                                // This is used because HDCP devices have been known to
                                                                // take a long time to start authentication, which can
                                                                // make them look like NON-HDCP sources, resulting in snow
#define DEM_EXTENDED_MUTE       (800 / DEM_POLLING_DELAY )      // Extend the MUTE delay for sources that have a long
                                                                // delay between R0 and the first CTL3, or do not send
                                                                // the CTL3 at all (one STB does this)

#endif  // __SI_APICONFIGURE_H__

