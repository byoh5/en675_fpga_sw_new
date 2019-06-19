//***************************************************************************
//!file     si_apiCEC.c
//!brief    Silicon Image mid-level CEC handler
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

//#include <string.h>
//#include <si_api9387.h>
//#if (API_DEBUG_CODE==1)
//    #include <stdio.h>
//#endif
//#include <si_regio.h>
//#include <si_apiHEAC.h>
#include "dev.h"
#include "si_apiCEC.h"
#include "si_apiConfigure.h"
#include "eHDMI.h"
#include "defs.h"


extern	bool_t si_CDCProcess (SI_CpiData_t *);
extern	bool_t CpCdcInit(void);

#if (IS_ARC)
extern	void EHDMI_Disable(void);
#if (IS_HEC)
extern	void EHDMI_ARC_Common_Enable(void);
#else
extern	void EHDMI_ARC_Single_Enable(void);
#endif
#endif

BOOL si_CecRxMsgHandlerARC (SI_CpiData_t *pCpi);  // Warning: Starter Kit Specific
BOOL CpCecRxMsgHandler (SI_CpiData_t *pCpi);      // Warning: Starter Kit Specific

//-------------------------------------------------------------------------------
// CPI Enums, typedefs, and manifest constants
//-------------------------------------------------------------------------------

/* One Touch Play Task internal states    */
enum
{
    TASK_ONETOUCH_START               = 1,
    TASK_ONETOUCH_SENT_IV_ON,
	TASK_ONETOUCH_DELAY,
    TASK_ONETOUCH_SENT_ACTIVE_SOURCE,
};

/* New Source Task internal states    */
enum
{
    NST_START               = 1,
    NST_SENT_PS_REQUEST,
    NST_SENT_PWR_ON,
    NST_SENT_STREAM_REQUEST
};

/* Task CPI states. */
enum
{
    CPI_IDLE            = 1,
    CPI_SENDING,
    CPI_WAIT_ACK,
    CPI_WAIT_RESPONSE,
    CPI_RESPONSE
};

typedef struct
{
    uint8_t     task;       // Current CEC task
    uint8_t     state;      // Internal task state
    uint8_t     cpiState;   // State of CPI transactions
    uint8_t     destLA;     // Logical address of target device
    uint8_t     taskData1;  // BYTE Data unique to task.
    uint16_t    taskData2;  // WORD Data unique to task.

} CEC_TASKSTATE;

//------------------------------------------------------------------------------
// Data
//------------------------------------------------------------------------------

CEC_DEVICE   g_childPortList [SII_NUMBER_OF_PORTS];

//-------------------------------------------------------------------------------
// Module data
//-------------------------------------------------------------------------------

// default Logical Address is as DVD1.  Why?  This is lore that is not documented
// but it seems that all our in-house testing expects a TX to have this logical address.
uint8_t     g_cecAddress            = CEC_LOGADDR_PLAYBACK1;

uint16_t    g_cecPhysical           = 0xFFFF;               // Initialize to Broadcast addr, will get changed
															// when EDID is read.

static uint8_t  g_currentTask       = SI_CECTASK_IDLE;

static BOOL     l_cecEnabled        = false;
static uint8_t  l_powerStatus       = CEC_POWERSTATUS_ON;
static uint8_t  l_portSelect        = 0x00;
static uint8_t  l_sourcePowerStatus = CEC_POWERSTATUS_STANDBY;

static SI_CpiData_t  l_cecFrame;							// CEC Message data frame

/* Active Source Addresses  */
static uint8_t  l_activeSrcLogical  = CEC_LOGADDR_UNREGORBC;    // Logical address of our active source
static uint16_t l_activeSrcPhysical = 0x0000;

// This table is used to covert a CEC logical address into the Device Type.  Note that each
// Devive Type can be at several different Physical Addresses.
//static uint8_t ROM l_devTypes [16] =
//{
//    CEC_LOGADDR_TV,
//    CEC_LOGADDR_RECDEV1,
//    CEC_LOGADDR_RECDEV1,
//    CEC_LOGADDR_TUNER1,
//    CEC_LOGADDR_PLAYBACK1,
//    CEC_LOGADDR_AUDSYS,
//    CEC_LOGADDR_TUNER1,
//    CEC_LOGADDR_TUNER1,
//    CEC_LOGADDR_PLAYBACK1,
//    CEC_LOGADDR_RECDEV1,
//    CEC_LOGADDR_TUNER1,
//    CEC_LOGADDR_PLAYBACK1,
//    0x02,
//    0x02,
//    CEC_LOGADDR_TV,
//    CEC_LOGADDR_TV
//};

static uint8_t l_devTypes [16] =
{
    CEC_LOGADDR_TV,
    CEC_LOGADDR_RECDEV1,
    CEC_LOGADDR_RECDEV1,
    CEC_LOGADDR_TUNER1,
    CEC_LOGADDR_PLAYBACK1,
    CEC_LOGADDR_AUDSYS,
    CEC_LOGADDR_TUNER1,
    CEC_LOGADDR_TUNER1,
    CEC_LOGADDR_PLAYBACK1,
    CEC_LOGADDR_RECDEV1,
    CEC_LOGADDR_TUNER1,
    CEC_LOGADDR_PLAYBACK1,
    0x02,
    0x02,
    CEC_LOGADDR_TV,
    CEC_LOGADDR_TV
};


/* CEC task data    */
static uint8_t l_newTask            = false;
static CEC_TASKSTATE l_cecTaskState = {0, 0, 0, 0, 0, 0};
static CEC_TASKSTATE l_cecTaskStateQueued =
{
    SI_CECTASK_ENUMERATE,           // Current CEC task
    0,                              // Internal task state
    CPI_IDLE,                       // cpi state
    0x00,                           // Logical address of target device
    0x00,                           // BYTE Data unique to task.
    0x0000                          // WORD Data unique to task.
};

//------------------------------------------------------------------------------
// Function:
// Description:
//------------------------------------------------------------------------------

static void PrintLogAddr ( uint8_t bLogAddr )
{

#if (INCLUDE_CEC_NAMES > CEC_NO_NAMES)
    DEBUG_PRINT( MSG_DBG,(  " [%X] %s", (int)bLogAddr, CpCecTranslateLA( bLogAddr ) ));
#else
    bLogAddr = 0;   // Passify the compiler
#endif
}

//------------------------------------------------------------------------------
// Function:    CecSendActiveSource
// Description: Send an ACTIVE SOURCE message.  Does not wait for a reply.
//------------------------------------------------------------------------------

static void CecSendActiveSource(void)
{
    l_cecFrame.opcode        = CECOP_ACTIVE_SOURCE;
    l_cecFrame.srcDestAddr   = MAKE_SRCDEST(g_cecAddress, CEC_LOGADDR_UNREGORBC);
    l_cecFrame.args[0]       = (uint8_t)(SI_CecGetDevicePA() >> 8); // Our Physical Address
    l_cecFrame.args[1]       = (uint8_t)(SI_CecGetDevicePA() && 0xFF);
    l_cecFrame.argCount      = 2;
    SI_CpiWrite(&l_cecFrame);
}


//------------------------------------------------------------------------------
// Function:    CecTaskCpiWaitAck
// Description: Waits for an ACK from the last command sent.
//------------------------------------------------------------------------------

static uint8_t CecTaskCpiWaitAck (SI_CpiStatus_t *pCecStatus)
{
    uint8_t newTask = l_cecTaskState.task;

    if (pCecStatus->txState == SI_TX_SENDFAILED)
    {
//		DEBUG_PRINT(MSG_STAT, ("Task:: NoAck received\n"));
        /* Abort task */
        l_cecTaskState.cpiState = CPI_IDLE;
        newTask                 = SI_CECTASK_IDLE;
    }
    else if (pCecStatus->txState == SI_TX_SENDACKED)
    {
//		DEBUG_PRINT(MSG_STAT, ("Task:: ACK received\n"));
    }
    return(newTask);
}


//------------------------------------------------------------------------------
// Function:    UpdateChildPortList
// Description: Store the PA and LA of the subsystem if it is at the next
//              next level down from us (direct child).
//              Returns the HDMI port index that is hosting this physical address
//------------------------------------------------------------------------------

static void UpdateChildPortList ( uint8_t newLA, uint16_t newPA )
{
    uint8_t     index;
    uint16_t    mask, pa;

    /* Determine Physical Address position (A.B.C.D) of our */
    /* device and generate a mask.                          */
    /* Field D cannot host any child devices.               */

    mask = 0x00F0;
    for ( index = 1; index < 4; index++ )
    {
        if (( g_cecPhysical & mask ) != 0)
            break;
        mask <<= 4;
    }

    /* Re-initialize the child port list with possible physical     */
    /* addresses of our immediate child devices.                    */
    /* If the new physical address is one of our direct children,   */
    /* update the LA for that position in the child port list.      */

    mask    = 0x0001 << ((index - 1) * 4);
    pa      = mask;
    for ( index = 0; index < SII_NUMBER_OF_PORTS; index++)
    {
        g_childPortList[ index].cecPA = pa;
        if ( pa == newPA )
        {
            g_childPortList[ index].cecLA = newLA;
            g_childPortList[ index].deviceType = l_devTypes[newLA];
//            printf("\n***** [%04X][%02D]", (int)newPA, (int)newLA);
        }
        pa += mask;
    }
}

//------------------------------------------------------------------------------
// Function:    CecGetPortIndex
// Description: Return the index within the child port list of the passed port.
//
//              Returns 0xFF if port is not in the child list
//------------------------------------------------------------------------------

//static uint8_t CecGetPortIndex ( uint16_t port )
//{
//    uint16_t    temp;
//    uint8_t     i;
//
//    port++; // need 1-based value.
//
//    // Look for non-zero nibble postion in source port physical address
//
//    temp = 0x000F;
//   // Look for non-zero nibble postion in source port physical address
//    for ( i = 0; i < 4; i++)
//    {
//        if ((g_cecPhysical & temp) != 0)
//                break;
//        temp <<= 4;
//    }
//
//    //If i is equal to zero means no more child (or end of the node)
//    if (i != 0)
//    {
//        port <<= (4 * (i-1));
//        temp = g_cecPhysical | port;        // PA of the current EDID
//    }
//    else
//    {
//        temp = g_cecPhysical;
//    }
//
//    for ( i = 0; i < SII_NUMBER_OF_PORTS; i++)
//    {
//        if ( g_childPortList[ i ].cecPA == temp )
//        {
//            return( g_childPortList[ i ].cecLA );
//        }
//    }
//    return( 0xFF );
//}

//------------------------------------------------------------------------------
// Function:    SI_CecPortToLA
// Description: Return the CEC logical address for the specified device port
// Parameters:  portIndex:  HDMI RX, 0-3
//                          HDMI TX, 0
// Returns:     CEC Logical address for the port, or 0x0F if not a CEC device.
//
// NOTE:    For a HDMI TX, the childPortList array contains the logical address
//          for the HDMI RX that this device is connected to.
//------------------------------------------------------------------------------

uint8_t SI_CecPortToLA (uint8_t portIndex)
{
    return(g_childPortList[portIndex].cecLA);
}

//------------------------------------------------------------------------------
// Function:    SI_CecLaToPort
// Description: Translate the passed logical address into a physical HDMI port
// Parameters:  logicalAddress  - The CEC logical address to translate
// Returns:     HDMI physical port index or oxFF if not found.
//------------------------------------------------------------------------------

uint8_t SI_CecLaToPort (uint8_t logicalAddr)
{
    uint8_t i;

    for (i = 0; i < SII_NUMBER_OF_PORTS; i++)
    {
        if (g_childPortList[i].cecLA == logicalAddr)
            return(i);
    }
    return(0xFF);
}

//------------------------------------------------------------------------------
// Function:    CecSendUserControlPressed
// Description: Local function for sending a "remote control key pressed"
//              command to the specified source.
//------------------------------------------------------------------------------

static void CecSendUserControlPressed (uint8_t keyCode, uint8_t destLA)
{
    SI_CpiData_t cecFrame;

    cecFrame.opcode        = CECOP_USER_CONTROL_PRESSED;
    cecFrame.srcDestAddr   = MAKE_SRCDEST(g_cecAddress, destLA);
    cecFrame.args[0]       = keyCode;
    cecFrame.argCount      = 1;
    SI_CpiWrite(&cecFrame);
}

//------------------------------------------------------------------------------
// Function:    CecSendFeatureAbort
// Description: Send a feature abort as a response to this message unless
//              it was broadcast (illegal).
//------------------------------------------------------------------------------

static void CecSendFeatureAbort ( SI_CpiData_t *pCpi, uint8_t reason )
{
    SI_CpiData_t cecFrame;

    if (( pCpi->srcDestAddr & 0x0F) != CEC_LOGADDR_UNREGORBC )
    {
        cecFrame.opcode        = CECOP_FEATURE_ABORT;
        cecFrame.srcDestAddr   = MAKE_SRCDEST( g_cecAddress, (pCpi->srcDestAddr & 0xF0) >> 4 );
        cecFrame.args[0]       = pCpi->opcode;
        cecFrame.args[1]       = reason;
        cecFrame.argCount      = 2;
        SI_CpiWrite( &cecFrame );
    }
}

//------------------------------------------------------------------------------
// Function:    CecHandleFeatureAbort
// Description: Received a failure response to a previous message.  Print a
//              message and notify the rest of the system
//------------------------------------------------------------------------------

#if (INCLUDE_CEC_NAMES > CEC_NO_TEXT)
static char *ml_abortReason [] =        // (0x00) <Feature Abort> Opcode    (RX)
    {
    "Unrecognized OpCode",              // 0x00
    "Not in correct mode to respond",   // 0x01
    "Cannot provide source",            // 0x02
    "Invalid Operand",                  // 0x03
    "Refused"                           // 0x04
    };
#endif

static void CecHandleFeatureAbort( SI_CpiData_t *pCpi )
{
    SI_CpiData_t cecFrame;

    cecFrame.opcode = pCpi->args[0];
    cecFrame.argCount = 0;
#if (INCLUDE_CEC_NAMES > CEC_NO_NAMES)
//    DEBUG_PRINT(
//        MSG_STAT, (
//        "\nMessage %s(%02X) was %s by %s (%d)\n",
//        CpCecTranslateOpcodeName( &cecFrame ), (int)pCpi->args[0],
//        ml_abortReason[ (pCpi->args[1] <= CECAR_REFUSED) ? pCpi->args[1] : 0],
//        CpCecTranslateLA( pCpi->srcDestAddr >> 4 ), (int)(pCpi->srcDestAddr >> 4)
//        ));
#elif (INCLUDE_CEC_NAMES > CEC_NO_TEXT)
//    DEBUG_PRINT(
//        MSG_STAT, (
//        "\nMessage %02X was %s by logical address %d\n",
//        (int)pCpi->args[0],
//        ml_abortReason[ (pCpi->args[1] <= CECAR_REFUSED) ? pCpi->args[1] : 0],
//        (int)(pCpi->srcDestAddr >> 4)
//        ));
#endif
}

//------------------------------------------------------------------------------
// Function:    CecHandleActiveSource
// Description: Process the CEC Active Source command by switching to the
//              broadcast port.
//------------------------------------------------------------------------------

static void CecHandleActiveSource ( SI_CpiData_t *pCpi )
{
    uint8_t     i;
    uint16_t    newPA;

    /* Extract the logical and physical addresses of the new active source. */

    l_activeSrcLogical  = (pCpi->srcDestAddr >> 4) & 0x0F;
    l_activeSrcPhysical = ((uint16_t)pCpi->args[0] << 8 ) | pCpi->args[1];

    UpdateChildPortList( l_activeSrcLogical, l_activeSrcPhysical );

    /* Determine the index of the HDMI port that    */
    /* is handling this physical address.           */

    newPA = l_activeSrcPhysical;
    for ( i = 0; i < 4; i++ )
    {
        if (( newPA & 0x000F ) != 0)
            break;
        newPA >>= 4;
    }

    /* Port address (1-based) is left in the lowest nybble. */
    /* Convert to 0-based and use it.                       */
    /* Signal main process of new port.  The main process   */
    /* will perform the physical port switch.               */

    l_portSelect = (( newPA & 0x000F ) - 1 );
//    DEBUG_PRINT( MSG_DBG,( "\nACTIVE_SOURCE: %02X (%04X) (port %02X)\n", (int)l_activeSrcLogical, l_activeSrcPhysical, (int)l_portSelect ));
}

//------------------------------------------------------------------------------
// Function:    CecHandleInactiveSource
// Description: Process the CEC Inactive Source command
//------------------------------------------------------------------------------

static void CecHandleInactiveSource ( SI_CpiData_t *pCpi )
{
    uint8_t la;

    la = (pCpi->srcDestAddr >> 4) & 0x0F;
    if ( la == l_activeSrcLogical )    // The active source has deserted us!
    {
        l_activeSrcLogical  = CEC_LOGADDR_TV;
        l_activeSrcPhysical = 0x0000;
    }

    l_portSelect = 0xFF;    // Tell main process to choose another port.
}

//------------------------------------------------------------------------------
// Function:    CecHandleReportPhysicalAddress
// Description: Store the PA and LA of the subsystem.
//              This routine is called when a physical address was broadcast.
//              usually this routine is used for a system which configure as TV or Repeater.
//------------------------------------------------------------------------------

static void CecHandleReportPhysicalAddress ( SI_CpiData_t *pCpi )
{
    UpdateChildPortList(
        (pCpi->srcDestAddr >> 4) & 0xF,         // broadcast logical address
        (pCpi->args[0] << 8) | pCpi->args[1]    // broadcast physical address
        );
}

//------------------------------------------------------------------------------
// Function:    CecTaskEnumerate
// Description: Ping every logical address on the CEC bus to see if anything
//              is attached.  Code can be added to store information about an
//              attached device, but we currently do nothing with it.
//
//              As a side effect, we determines our Initiator address as
//              the first available address of 0x00, 0x0E or 0x0F.
//
// l_cecTaskState.taskData1 == Not Used.
// l_cecTaskState.taskData2 == Not used
//------------------------------------------------------------------------------

static uint8_t CecTaskEnumerate ( SI_CpiStatus_t *pCecStatus )
{
    uint8_t newTask = l_cecTaskState.task;

    g_cecAddress = CEC_LOGADDR_UNREGORBC;

    if (l_cecTaskState.cpiState == CPI_IDLE)
    {
        if (l_cecTaskState.destLA < CEC_LOGADDR_UNREGORBC)   // Don't ping broadcast address
        {
            SI_CpiSendPing(l_cecTaskState.destLA);
//            DEBUG_PRINT(MSG_DBG, ("...Ping...\n"));
            PrintLogAddr(l_cecTaskState.destLA);
            l_cecTaskState.cpiState = CPI_WAIT_ACK;
        }
        else    // We're done
        {
            SI_CpiSetLogicalAddr(g_cecAddress);
//            DEBUG_PRINT(MSG_DBG,("ENUM DONE: Initiator address is "));
            PrintLogAddr(g_cecAddress);
//            DEBUG_PRINT( MSG_DBG,("\n"));

            /* Go to idle task, we're done. */
            l_cecTaskState.cpiState = CPI_IDLE;
            newTask = SI_CECTASK_IDLE;
        }
    }
    else if (l_cecTaskState.cpiState == CPI_WAIT_ACK)
    {
        if (pCecStatus->txState == SI_TX_SENDFAILED)
        {
//            DEBUG_PRINT(MSG_DBG,( "\nEnum NoAck"));
            PrintLogAddr(l_cecTaskState.destLA);

            /* If a TV address, grab it for our use. */
            if ((g_cecAddress == CEC_LOGADDR_UNREGORBC) &&
                ((l_cecTaskState.destLA == CEC_LOGADDR_TV) ||
                (l_cecTaskState.destLA == CEC_LOGADDR_FREEUSE)))
            {
                g_cecAddress = l_cecTaskState.destLA;
            }

            /* Restore Tx State to IDLE to try next address.    */
            l_cecTaskState.cpiState = CPI_IDLE;
            l_cecTaskState.destLA++;
        }
        else if ( pCecStatus->txState == SI_TX_SENDACKED )
        {
//            DEBUG_PRINT(MSG_DBG,("\n-----------------------------------------------> Enum Ack\n"));
            PrintLogAddr( l_cecTaskState.destLA );

            /* TODO: Add code here to store info about this device if needed.   */

            /* Restore Tx State to IDLE to try next address. */
            l_cecTaskState.cpiState = CPI_IDLE;
            l_cecTaskState.destLA++;
        }
    }

    return( newTask );
}


//------------------------------------------------------------------------------
// Function:    CecTaskStartNewSource
// Description: Request power status from a device.  If in standby, tell it
//              to turn on and wait until it does, then start the device
//              streaming to us.
//
// l_cecTaskState.taskData1 == Number of POWER_ON messages sent.
// l_cecTaskState.taskData2 == Physical address of target device.
//------------------------------------------------------------------------------

static uint8_t CecTaskStartNewSource (SI_CpiStatus_t *pCecStatus)
{
    SI_CpiData_t    cecFrame;
    uint8_t         newTask = l_cecTaskState.task;

    if (l_cecTaskState.cpiState == CPI_IDLE)
    {
        if (l_cecTaskState.state == NST_START)
        {
            /* First time through here, TIMER_2 will not have   */
            /* been started yet, and therefore should return    */
            /* expired (0 == 0).                                */
            //if (HalTimerExpired(TIMER_2))
            {
//                DEBUG_PRINT( MSG_DBG,( "\nNST_START\n" ));
                si_CecSendMessage(CECOP_GIVE_DEVICE_POWER_STATUS, l_cecTaskState.destLA);
                l_cecTaskState.cpiState = CPI_WAIT_ACK;
                l_cecTaskState.state    = NST_SENT_PS_REQUEST;
            }
        }
    }

    /* Wait for acknowledgement of message sent.    */
    else if (l_cecTaskState.cpiState == CPI_WAIT_ACK)
    {
        if (pCecStatus->txState == SI_TX_SENDFAILED)
        {
//            DEBUG_PRINT(MSG_DBG,("New Source Task NoAck\n"));

            /* Abort task */
            l_cecTaskState.cpiState = CPI_IDLE;
            newTask                 = SI_CECTASK_IDLE;
        }
        else if (pCecStatus->txState == SI_TX_SENDACKED)
        {
//            DEBUG_PRINT(MSG_DBG,("New Source Task ACK\n"));

            if (l_cecTaskState.state == NST_SENT_PS_REQUEST)
            {
                l_cecTaskState.cpiState  = CPI_WAIT_RESPONSE;
            }
            else if (l_cecTaskState.state == NST_SENT_PWR_ON)
            {
                /* This will force us to start over with status request.    */
                l_cecTaskState.cpiState = CPI_IDLE;
                l_cecTaskState.state    = NST_START;
                l_cecTaskState.taskData1++;
            }
            else if (l_cecTaskState.state == NST_SENT_STREAM_REQUEST)
            {
                /* Done.    */
                l_cecTaskState.cpiState = CPI_IDLE;
                newTask                 = SI_CECTASK_IDLE;
            }
        }
    }

    /* Looking for a response to our message. During this time, we do nothing.  */
    /* The various message handlers will set the cpi state to CPI_RESPONSE when */
    /* a device has responded with an appropriate message.                      */
    else if (l_cecTaskState.cpiState == CPI_WAIT_RESPONSE)
    {
        //Lee: Need to have a timeout here.
    }

    /* We have an answer. Go to next step.  */
    else if (l_cecTaskState.cpiState == CPI_RESPONSE)
    {
        if (l_cecTaskState.state == NST_SENT_PS_REQUEST)    // Acking GIVE_POWER_STATUS
        {
            /* l_sourcePowerStatus is updated by the REPORT_POWER_STATUS message.   */
            switch (l_sourcePowerStatus)
            {
                case CEC_POWERSTATUS_ON:
                    {
                        /* Device power is on, tell device to send us some video.   */
                        cecFrame.opcode         = CECOP_SET_STREAM_PATH;
                        cecFrame.srcDestAddr    = MAKE_SRCDEST( g_cecAddress, CEC_LOGADDR_UNREGORBC );
                        cecFrame.args[0]        = (uint8_t)(l_cecTaskState.taskData2 >> 8);
                        cecFrame.args[1]        = (uint8_t)l_cecTaskState.taskData2;
                        cecFrame.argCount       = 2;
                        SI_CpiWrite(&cecFrame);
                        l_cecTaskState.cpiState = CPI_WAIT_ACK;
                        l_cecTaskState.state    = NST_SENT_STREAM_REQUEST;
                    }
                    break;

                case CEC_POWERSTATUS_STANDBY:
                case CEC_POWERSTATUS_ON_TO_STANDBY:
                    {
                        /* Device is in standby, tell it to turn on via remote control. */
                        if (l_cecTaskState.taskData1 == 0)
                        {
                            CecSendUserControlPressed(CEC_RC_POWER, l_cecTaskState.destLA);
                            l_cecTaskState.cpiState = CPI_WAIT_ACK;
                            l_cecTaskState.state    = NST_SENT_PWR_ON;
                            //HalTimerSet( TIMER_2, 500 );
                        }
                        else    // Abort task if already tried to turn it on device without success
                        {
                            l_cecTaskState.cpiState = CPI_IDLE;
                            newTask                 = SI_CECTASK_IDLE;
                        }
                    }
                    break;

                case CEC_POWERSTATUS_STANDBY_TO_ON:
                    {
                        l_cecTaskState.cpiState = CPI_IDLE;
                        l_cecTaskState.state    = NST_START;
                        //HalTimerSet(TIMER_2, 500);   // Wait .5 seconds between requests for status
                    }
                    break;
            }
        }
    }

    return(newTask);
}


#if (IS_CEC == 1)
//------------------------------------------------------------------------------
// Function:    CecTaskOneTouchPlay
// Description: Implement One Touch Play message sequence.  The discription can
// 				be found in CEC 13.1.
//------------------------------------------------------------------------------

static uint8_t CecTaskOneTouchPlay (SI_CpiStatus_t *pCecStatus)
{
    uint8_t         newTask = l_cecTaskState.task;

    switch (l_cecTaskState.cpiState)
    {
        case CPI_IDLE:
            switch (l_cecTaskState.state)
            {
                case TASK_ONETOUCH_START:
                    // Send an Image View On command to wake up the TV
                    l_cecFrame.opcode           = CECOP_IMAGE_VIEW_ON;
                    l_cecFrame.srcDestAddr      = MAKE_SRCDEST(g_cecAddress, CEC_LOGADDR_TV);
                    l_cecFrame.argCount         = 0;
                    SI_CpiWrite(&l_cecFrame);
					DEBUG_PRINT(MSG_DBG, ("CecTaskOneTouchPlay: Sent <Image View On>\n")); //%%% chnge to DEBUG later
					// Goto Next State
//                    		HalTimerSet(TIMER_2, 30);	// We need at least a 16.8ms delay.  Just to be safe, let's do 30ms.
                    l_cecTaskState.state     = TASK_ONETOUCH_DELAY;
                    break;

				case TASK_ONETOUCH_DELAY:
//            		if (HalTimerExpired(TIMER_2))
//					{
//                    	l_cecTaskState.state = TASK_ONETOUCH_SENT_IV_ON;
//					}
					break;

                case TASK_ONETOUCH_SENT_IV_ON:
                    // Now send an ACTIVE SOURCE message to tell them who we are
                    CecSendActiveSource();
					DEBUG_PRINT(MSG_DBG, ("CecTaskOneTouchPlay: Sent <Active Source>\n")); //%%% chnge to DEBUG later
					// Goto Next State
                    l_cecTaskState.state        = TASK_ONETOUCH_SENT_ACTIVE_SOURCE;
                    l_cecTaskState.cpiState     = CPI_WAIT_ACK;
                    break;

                case TASK_ONETOUCH_SENT_ACTIVE_SOURCE:
                    newTask = SI_CECTASK_IDLE;
                    break;
            }
            break;

        case CPI_WAIT_ACK:
            l_cecTaskState.cpiState = CPI_IDLE;
			//DEBUG_PRINT(MSG_DBG, ("CecTaskOneTouchPlay: Waiting for ACK\n"));
            newTask = CecTaskCpiWaitAck(pCecStatus);  // Will go to idle task if no ack received
            break;
    }

    return(newTask);
}
#endif	// IS_CEC


//------------------------------------------------------------------------------
// Function:    ValidateCecMessage
// Description: Validate parameter count of passed CEC message
//              Returns FALSE if not enough operands for the message
//              Returns TRUE if the correct amount or more of operands (if more
//              the message processor willl just ignore them).
//------------------------------------------------------------------------------

static BOOL ValidateCecMessage (SI_CpiData_t *pCpi)
{
    uint8_t parameterCount = 0;
    BOOL    countOK = true;

    /* Determine required parameter count   */

    switch (pCpi->opcode)
    {
        case CECOP_IMAGE_VIEW_ON:
        case CECOP_TEXT_VIEW_ON:
        case CECOP_STANDBY:
        case CECOP_GIVE_PHYSICAL_ADDRESS:
        case CECOP_GIVE_DEVICE_POWER_STATUS:
        case CECOP_GET_MENU_LANGUAGE:
        case CECOP_GET_CEC_VERSION:
        case CECOP_ARC_INITIATE:
        case CECOP_ARC_REPORT_INITIATED:
        case CECOP_ARC_REPORT_TERMINATED:
        case CECOP_ARC_REQUEST_INITIATION:
        case CECOP_ARC_REQUEST_TERMINATION:
        case CECOP_ARC_TERMINATE:
            parameterCount = 0;
            break;
        case CECOP_REPORT_POWER_STATUS:         // power status
        case CECOP_CEC_VERSION:                 // cec version
            parameterCount = 1;
            break;
        case CECOP_INACTIVE_SOURCE:             // physical address
        case CECOP_FEATURE_ABORT:               // feature opcode / abort reason
        case CECOP_ACTIVE_SOURCE:               // physical address
            parameterCount = 2;
            break;
        case CECOP_REPORT_PHYSICAL_ADDRESS:     // physical address / device type
        case CECOP_DEVICE_VENDOR_ID:            // vendor id
            parameterCount = 3;
            break;
        case CECOP_SET_OSD_NAME:                // osd name (1-14 bytes)
        case CECOP_SET_OSD_STRING:              // 1 + x   display control / osd string (1-13 bytes)
            parameterCount = 1;                 // must have a minimum of 1 operands
            break;
        case CECOP_ABORT:
            break;

        default:
            break;
    }

    /* Test for correct parameter count.    */
    if ( pCpi->argCount < parameterCount )
    {
        countOK = false;
    }

    return(countOK);
}

//------------------------------------------------------------------------------
// Function:    si_CecRxMsgHandlerFirst
// Description: This is the first message handler called in the chain.
//              It parses messages we don't want to pass on to outside handlers
//              and those that we need to look at, but not exclusively
//------------------------------------------------------------------------------

static BOOL si_CecRxMsgHandlerFirst (SI_CpiData_t *pCpi)
{
    BOOL            msgHandled, isDirectAddressed;

    // Check messages we handle for correct number of operands and abort if incorrect.
    msgHandled = true;
    if (ValidateCecMessage(pCpi))    // If invalid message, ignore it, but treat it as handled
    {
        isDirectAddressed = !((pCpi->srcDestAddr & 0x0F ) == CEC_LOGADDR_UNREGORBC );
        if (isDirectAddressed)
        {
            switch (pCpi->opcode)
            {
                case CECOP_INACTIVE_SOURCE:
                    CecHandleInactiveSource(pCpi);
                    msgHandled = false;             // Let user also handle it if they want.
                    break;
                default:
                    msgHandled = false;
                    break;
            }
        }
        else
        {
            switch (pCpi->opcode)
            {
                case CECOP_ACTIVE_SOURCE:
                    CecHandleActiveSource(pCpi);
                    msgHandled = false;             // Let user also handle it if they want.
                    break;
                default:
                    msgHandled = false;
                    break;
            }
        }
    }
	else
	{
		DEBUG_PRINT(MSG_DBG,("Invalid Message\n"));
	}

    return(msgHandled);
}

//------------------------------------------------------------------------------
// Function:    si_CecRxMsgHandlerLast
// Description: This is the last message handler called in the chain, and
//              parses any messages left untouched by the previous handlers.
//------------------------------------------------------------------------------

static BOOL si_CecRxMsgHandlerLast ( SI_CpiData_t *pCpi )
{
    BOOL            isDirectAddressed;
    SI_CpiData_t    cecFrame;

    isDirectAddressed = !((pCpi->srcDestAddr & 0x0F ) == CEC_LOGADDR_UNREGORBC );

    if (ValidateCecMessage(pCpi))            // If invalid message, ignore it, but treat it as handled
    {
        if (isDirectAddressed)
        {
            switch (pCpi->opcode)
            {
                case CECOP_FEATURE_ABORT:
                    CecHandleFeatureAbort(pCpi);
                    break;

                case CECOP_IMAGE_VIEW_ON:       // In our case, respond the same to both these messages
                case CECOP_TEXT_VIEW_ON:
                    break;

                case CECOP_STANDBY:             // Direct and Broadcast
					/* Setting this here will let the main task know    */
					/* (via SI_CecGetPowerState) and at the same time   */
					/* prevent us from broadcasting a STANDBY message   */
					/* of our own when the main task responds by        */
					/* calling SI_CecSetPowerState( STANDBY );          */
                    l_powerStatus = CEC_POWERSTATUS_STANDBY;
                    break;

                case CECOP_INACTIVE_SOURCE:
                    CecHandleInactiveSource(pCpi);
                    break;

                case CECOP_GIVE_PHYSICAL_ADDRESS:
                    cecFrame.opcode        = CECOP_REPORT_PHYSICAL_ADDRESS;
                    cecFrame.srcDestAddr   = MAKE_SRCDEST(g_cecAddress, CEC_LOGADDR_UNREGORBC);
                    cecFrame.args[0]       = (uint8_t)(SI_CecGetDevicePA() >> 8); // Physical Address
                    cecFrame.args[1]       = (uint8_t)(SI_CecGetDevicePA() && 0xFF);
		    		// Get the Device Type via look-up table indexed by our Logical Address
                    cecFrame.args[2]       = l_devTypes[SI_CpiGetLogicalAddr()];
                    cecFrame.argCount      = 3;
                    SI_CpiWrite(&cecFrame);
                    break;

                case CECOP_GIVE_DEVICE_POWER_STATUS:
                    /* TV responds with power status.   */
                    cecFrame.opcode        = CECOP_REPORT_POWER_STATUS;
                    cecFrame.srcDestAddr   = MAKE_SRCDEST( g_cecAddress, (pCpi->srcDestAddr & 0xF0) >> 4 );
                    cecFrame.args[0]       = l_powerStatus;
                    cecFrame.argCount      = 1;
                    SI_CpiWrite( &cecFrame );
                    break;

                case CECOP_GET_MENU_LANGUAGE:
                    /* Respond with a Set Menu language command.    */
                    cecFrame.opcode         = CECOP_SET_MENU_LANGUAGE;
                    cecFrame.srcDestAddr    = CEC_LOGADDR_UNREGORBC;
                    cecFrame.args[0]        = 'e';     // [language code see iso/fdis 639-2]
                    cecFrame.args[1]        = 'n';     // [language code see iso/fdis 639-2]
                    cecFrame.args[2]        = 'g';     // [language code see iso/fdis 639-2]
                    cecFrame.argCount       = 3;
                    SI_CpiWrite( &cecFrame );
                    break;

                case CECOP_GET_CEC_VERSION:
                    /* Responds to this request with version support.   */
                    cecFrame.srcDestAddr   = MAKE_SRCDEST( g_cecAddress, (pCpi->srcDestAddr & 0xF0) >> 4 );
                    cecFrame.opcode        = CECOP_CEC_VERSION;
                    cecFrame.args[0]       = 0x04;       // Report CEC1.3a
                    cecFrame.argCount      = 1;
                    SI_CpiWrite( &cecFrame );
                    break;

                case CECOP_REPORT_POWER_STATUS:         // Someone sent us their power state.
                    l_sourcePowerStatus = pCpi->args[0];
					/* Let NEW SOURCE task know about it.   */
                    if (l_cecTaskState.task == SI_CECTASK_NEWSOURCE)
                    {
                        l_cecTaskState.cpiState = CPI_RESPONSE;
                    }
                    break;

#if (IS_TX == 1)
                case CECOP_REQUEST_ACTIVE_SOURCE:       // If we are active source, we need to respond
					if (1)	// Test to see if we are active would go here
					{
						cecFrame.opcode        = CECOP_ACTIVE_SOURCE;
						cecFrame.srcDestAddr   = MAKE_SRCDEST(g_cecAddress, CEC_LOGADDR_UNREGORBC);
						cecFrame.args[0]       = (uint8_t)(SI_CecGetDevicePA() >> 8);
						cecFrame.args[1]       = (uint8_t)SI_CecGetDevicePA();
						cecFrame.argCount      = 2;
						SI_CpiWrite(&cecFrame);
					}
                    break;
#endif

#if (IS_ARC)
				//
				// Handling for ARC (Audio Return Channel commands from the Sink
				// NOTE:  Since this is a Return Channel the data flow is upstream rather than
				// downstream.  So a ARC TX device will be what we normally
				// consider an HDMI RX devise and a ARC RX device is what we normally consider
				// an HDMI TX device.
				//
				case CECOP_ARC_REQUEST_INITIATION:
					// The ARC Transmitter wants to send us Return Audio, so we need to
					// Enable ARC.
#if (IS_HEC)
					// HEC also, so us Common Mode API to turn on ARC
					EHDMI_ARC_Common_Enable();
					DEBUG_PRINT(MSG_STAT, ("ARC Initiate Request from ARC TX: Common Mode\n"));
#else
					// Only ARC, no HEC.  So use ARC-Single Mode API
					EHDMI_ARC_Single_Enable();
					DEBUG_PRINT(MSG_STAT, ("ARC Initiate Request from ARC TX: Single Mode\n"));
#endif	// IS_HEC
					// Send the Initiate message back to tell them we are ready to receive ARC.
                    cecFrame.opcode        = CECOP_ARC_INITIATE;
                    cecFrame.srcDestAddr   = MAKE_SRCDEST(g_cecAddress, CEC_LOGADDR_UNREGORBC);
                    cecFrame.argCount      = 0;
                    SI_CpiWrite(&cecFrame);
					break;

				case CECOP_ARC_REPORT_INITIATED:
					// The ARC Transmitter ACKed the initiation.  We don't do anything with this.
					DEBUG_PRINT(MSG_STAT, ("ARC Initiate Report from ARC TX\n"));
					break;

				case CECOP_ARC_REPORT_TERMINATED:
					// The ARC Transmitter ACKed the termination.  We don't do anything with this.
					DEBUG_PRINT(MSG_STAT, ("ARC Terminte Report from ARC TX\n"));
					break;

				case CECOP_ARC_REQUEST_TERMINATION:
					// The ARC Transmitter wants to stop sending Return Audio, so we need to
					// Disable ARC.
#ifdef DEV_SUPPORT_EHDMI
					EHDMI_Disable();
#endif
					// Tell them we have shut off receiving.
                    cecFrame.opcode        = CECOP_ARC_TERMINATE;
                    cecFrame.srcDestAddr   = MAKE_SRCDEST(g_cecAddress, CEC_LOGADDR_UNREGORBC);
                    cecFrame.argCount      = 0;
                    SI_CpiWrite(&cecFrame);
					DEBUG_PRINT( MSG_STAT, ("ARC Terminate Request from ARC TX\n"));
					break;
#endif //IS_ARC

                /* Do not reply to directly addressed 'Broadcast' msgs.  */
                case CECOP_ACTIVE_SOURCE:
                case CECOP_REPORT_PHYSICAL_ADDRESS:     // A physical address was broadcast -- ignore it.
                case CECOP_ROUTING_CHANGE:              // We are not a downstream switch, so ignore this one.
                case CECOP_ROUTING_INFORMATION:         // We are not a downstream switch, so ignore this one.
                case CECOP_SET_STREAM_PATH:             // We are not a source, so ignore this one.
                case CECOP_SET_MENU_LANGUAGE:           // As a TV, we can ignore this message
                case CECOP_DEVICE_VENDOR_ID:
                    break;

                case CECOP_ABORT:       // Send Feature Abort for all unsupported features.
                default:
                    CecSendFeatureAbort(pCpi, CECAR_UNRECOG_OPCODE);
                    break;
            }
        }

        /* Respond to broadcast messages.   */

        else
        {
            switch ( pCpi->opcode )
            {
                case CECOP_STANDBY:
                    /* Setting this here will let the main task know    */
                    /* (via SI_CecGetPowerState) and at the same time   */
                    /* prevent us from broadcasting a STANDBY message   */
                    /* of our own when the main task responds by        */
                    /* calling SI_CecSetPowerState( STANDBY );          */
                    l_powerStatus = CEC_POWERSTATUS_STANDBY;
                    break;

                case CECOP_ACTIVE_SOURCE:
                    CecHandleActiveSource(pCpi);
                    break;

                case CECOP_REPORT_PHYSICAL_ADDRESS:
                    CecHandleReportPhysicalAddress( pCpi );
                    break;

                /* Do not reply to 'Broadcast' msgs that we don't need.  */
                case CECOP_REQUEST_ACTIVE_SOURCE:       // We are not a source, so ignore this one.
                case CECOP_ROUTING_CHANGE:              // We are not a downstream switch, so ignore this one.
                case CECOP_ROUTING_INFORMATION:         // We are not a downstream switch, so ignore this one.
                case CECOP_SET_STREAM_PATH:             // We are not a source, so ignore this one.
                case CECOP_SET_MENU_LANGUAGE:           // As a TV, we can ignore this message
                    break;
            }
        }
    }

    return( true );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// All public API functions are below
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function:    si_CecSendMessage
// Description: Send a single byte (no parameter) message on the CEC bus.  Does
//              no wait for a reply.
//------------------------------------------------------------------------------

void si_CecSendMessage ( uint8_t opCode, uint8_t dest )
{
    SI_CpiData_t cecFrame;

    cecFrame.opcode        = opCode;
    cecFrame.srcDestAddr   = MAKE_SRCDEST( g_cecAddress, dest );
    cecFrame.argCount      = 0;
    SI_CpiWrite( &cecFrame );
}

//------------------------------------------------------------------------------
// Function:    SI_CecSendUserControlPressed
// Description: Send a remote control key pressed command to the
//              current active source.
//------------------------------------------------------------------------------

void SI_CecSendUserControlPressed ( uint8_t keyCode )
{
    if ( !l_cecEnabled )
        return;

    if ( l_activeSrcLogical != CEC_LOGADDR_UNREGORBC )
    {
        CecSendUserControlPressed( keyCode, l_activeSrcLogical );
    }
}

//------------------------------------------------------------------------------
// Function:    SI_CecSendUserControlReleased
// Description: Send a remote control key released command to the
//              current active source.
//------------------------------------------------------------------------------

void SI_CecSendUserControlReleased ( void )
{
    if ( !l_cecEnabled )
        return;

    if ( l_activeSrcLogical != CEC_LOGADDR_UNREGORBC )
    {
        si_CecSendMessage( CECOP_USER_CONTROL_RELEASED, l_activeSrcLogical );
    }
}

//------------------------------------------------------------------------------
// Function:    SI_CecGetPowerState
// Description: Returns the CEC local power state.
//              Should be called after every call to SI_CecHandler since
//              a CEC device may have issued a standby or view message.
//
// Valid values:    CEC_POWERSTATUS_ON
//                  CEC_POWERSTATUS_STANDBY
//                  CEC_POWERSTATUS_STANDBY_TO_ON
//                  CEC_POWERSTATUS_ON_TO_STANDBY
//------------------------------------------------------------------------------

uint8_t SI_CecGetPowerState ( void )
{

    return( l_powerStatus );
}

//------------------------------------------------------------------------------
// Function:    SI_CecSetPowerState
// Description: Set the CEC local power state.
//
// Valid values:    CEC_POWERSTATUS_ON
//                  CEC_POWERSTATUS_STANDBY
//                  CEC_POWERSTATUS_STANDBY_TO_ON
//                  CEC_POWERSTATUS_ON_TO_STANDBY
//------------------------------------------------------------------------------

void SI_CecSetPowerState ( uint8_t newPowerState )
{
    if ( !l_cecEnabled )
        return;

    if ( l_powerStatus != newPowerState )
    {
        switch ( newPowerState )
        {
            case CEC_POWERSTATUS_ON:

                /* Find out who is the active source. Let the   */
                /* ACTIVE_SOURCE handler do the rest.           */

                si_CecSendMessage( CECOP_REQUEST_ACTIVE_SOURCE, CEC_LOGADDR_UNREGORBC );
                break;

            case CEC_POWERSTATUS_STANDBY:

                /* If we are shutting down, tell every one else to also.   */

                si_CecSendMessage( CECOP_STANDBY, CEC_LOGADDR_UNREGORBC );
                break;

            case CEC_POWERSTATUS_STANDBY_TO_ON:
            case CEC_POWERSTATUS_ON_TO_STANDBY:
            default:
                break;
        }

    l_powerStatus = newPowerState;
    }
}

//------------------------------------------------------------------------------
// Function:    SI_CecSourceRemoved
// Description: The hardware detected an HDMI connector removal, so clear the
//              position in our device list.
//
// TODO:    The source that was removed may or may not have had time to send out
//          an INACTIVE_SOURCE message, so we may want to emulate that action.
//
//------------------------------------------------------------------------------

void SI_CecSourceRemoved ( uint8_t portIndex )
{
    if ( !l_cecEnabled )
        return;

    DEBUG_PRINT(
        MSG_DBG,
        ( "CEC Source removed: Port: %d, LA:%02X \n",
        (int)portIndex, (int)g_childPortList[ portIndex].cecLA
        ));

	_printf("CEC Source removed : ", portIndex, "\n");
 	portIndex = 0;
}

//------------------------------------------------------------------------------
// Function:    SI_CecGetDevicePA
// Description: Return the physical address for this Host device
//
//------------------------------------------------------------------------------

uint16_t SI_CecGetDevicePA ( void )
{
    return( g_cecPhysical );
}

//------------------------------------------------------------------------------
// Function:    SI_CecSetDevicePA
// Description: Set the host device physical address (initiator physical address)
//------------------------------------------------------------------------------

void SI_CecSetDevicePA ( uint16_t devPa )
{
    g_cecPhysical = devPa;
//    DEBUG_PRINT( MSG_STAT, ("CEC: Device PA [%04X]\n", (int)devPa));
	_printf("CEC: Device PA : \r\n", devPa);
}

//------------------------------------------------------------------------------
// Function:    SI_CecInit
// Description: Initialize the CEC subsystem.
//------------------------------------------------------------------------------

BOOL SI_CecInit ( void )
{
    uint16_t i;

    SI_CpiInit();

    for ( i = 0; i < SII_NUMBER_OF_PORTS; i++)
    {
        //if this device is the TV then assign the PA to
        //the childPortList
        if (SI_CpiGetLogicalAddr() == CEC_LOGADDR_TV)
            g_childPortList[i].cecPA = (i+1) << 12;
        else
            g_childPortList[i].cecPA = 0xFFFF;
        g_childPortList[i].cecLA = 0xFF;
    }

#ifdef F_9022A_9334
    // Initialize the CDC
    CpCdcInit();
#endif

    l_cecEnabled = true;
    return( true );
}

//------------------------------------------------------------------------------
// Function:    si_TaskServer
// Description: Calls the current task handler.  A task is used to handle cases
//              where we must send and receive a specific set of CEC commands.
//------------------------------------------------------------------------------

static void si_TaskServer (SI_CpiStatus_t *pCecStatus)
{
    switch (g_currentTask)
    {
        case SI_CECTASK_IDLE:
            if (l_newTask)
            {
                /*
				 * A new task has been activated. Copy the queued task block into the
				 * operating task block.
				 */
//                memcpy(&l_cecTaskState, &l_cecTaskStateQueued, sizeof(CEC_TASKSTATE));
                l_newTask = false;
                g_currentTask = l_cecTaskState.task;
            }
            break;

        case SI_CECTASK_ENUMERATE:
            g_currentTask = CecTaskEnumerate(pCecStatus);
            break;

        case SI_CECTASK_NEWSOURCE:
            g_currentTask = CecTaskStartNewSource(pCecStatus);
            break;

#if (IS_CEC == 1)
        case SI_CECTASK_ONETOUCH:
            g_currentTask = CecTaskOneTouchPlay(pCecStatus);
            break;
#endif

        default:
            break;
    }

}

//------------------------------------------------------------------------------
// Function:    SI_CecSwitchSources
// Description: Send the appropriate CEC commands for switching from the current
//              active source to the requested source.
//              This is called after the main process has ALREADY performed the
//              actual port switch on the RX, so the physical address is changed
//              even if the input is NOT in the CEC list, either because it has
//              not reported itself as an active source, or it is not CEC
//              capable.
//------------------------------------------------------------------------------

BOOL SI_CecSwitchSources (uint8_t portIndex)
{
    uint8_t         newLogical;
    uint16_t        newPhysical = g_cecPhysical;
    SI_CpiData_t    cecFrame;
    BOOL            success = false;

    while (l_cecEnabled)
    {
        newPhysical = g_childPortList[portIndex].cecPA;
        if (newPhysical != 0xFFFF)
        {
            newLogical = g_childPortList[portIndex].cecLA;

            /* Broadcast a routing change message to tell switches what's going on.     */
            /* This happens even if this is a non-CEC device (CEC_LOGADDR_UNREGORBC).   */
            cecFrame.opcode         = CECOP_ROUTING_CHANGE;
            cecFrame.srcDestAddr    = MAKE_SRCDEST( g_cecAddress, CEC_LOGADDR_UNREGORBC );
            cecFrame.args[0]        = (uint8_t)(l_activeSrcPhysical >> 8);
            cecFrame.args[1]        = (uint8_t)l_activeSrcPhysical;
            cecFrame.args[2]        = (uint8_t)(newPhysical >> 8);
            cecFrame.args[3]        = (uint8_t)newPhysical;
            cecFrame.argCount       = 4;
            l_activeSrcLogical  = newLogical;
            SI_CpiWrite( &cecFrame );
        }
        else
        {
            DEBUG_PRINT(MSG_DBG, ("\nCecSwitchSources: Port %d not found in CEC list\n", (int)portIndex));
            /* If not in the CEC list, we still have to change the physical */
            /* address, and we change the logical address to 'UNKNOWN'.     */
            l_activeSrcLogical  = CEC_LOGADDR_UNREGORBC;
            break;
        }

        l_activeSrcPhysical = newPhysical;

        DEBUG_PRINT( MSG_DBG,( "\nCecSwitchSources: %02X to %02X (%04X)\n", (int)l_activeSrcLogical, (int)newLogical, newPhysical ));

        /* Start a task to handshake with the new source about power    */
        /* status and playback.                                         */
        l_cecTaskStateQueued.task       = SI_CECTASK_NEWSOURCE;
        l_cecTaskStateQueued.state      = NST_START;
        l_cecTaskStateQueued.destLA     = newLogical;
        l_cecTaskStateQueued.cpiState   = CPI_IDLE;
        l_cecTaskStateQueued.taskData1  = 0;
        l_cecTaskStateQueued.taskData2  = newPhysical;
        l_newTask = true;
        success = true;
        break;
    }

    return( success );
}

//------------------------------------------------------------------------------
// Function:    SI_CecEnumerate
// Description: Send the appropriate CEC commands to enumerate all the logical
//              devices on the CEC bus.
//------------------------------------------------------------------------------

BOOL SI_CecEnumerate (void)
{
    BOOL            success = false;

    /* If the task handler queue is not full, add the enumerate task.   */
    if (!l_newTask)
    {
        l_cecTaskStateQueued.task       = SI_CECTASK_ENUMERATE;
        l_cecTaskStateQueued.state      = 0;
        l_cecTaskStateQueued.destLA     = 0;
        l_cecTaskStateQueued.cpiState   = CPI_IDLE;
        l_cecTaskStateQueued.taskData1  = 0;
        l_cecTaskStateQueued.taskData2  = 0;
        l_newTask   = true;
        success     = true;
        DEBUG_PRINT(MSG_STAT, ("CEC: Queued Enumerate Task\n"));
    }

    return(success);
}


//------------------------------------------------------------------------------
// Function:    SI_CecOneTouchPlay
// Description: Send the appropriate CEC commands to the specified logical
//              device on the CEC bus.
//------------------------------------------------------------------------------

BOOL SI_CecOneTouchPlay (void)
{
    BOOL            success = false;

    /* If the Tx task handler queue is not full, add the One Touch Play task.   */
    if (!l_newTask)
    {
        l_cecTaskStateQueued.task       = SI_CECTASK_ONETOUCH;
        l_cecTaskStateQueued.state      = TASK_ONETOUCH_START;
        l_cecTaskStateQueued.destLA     = 0;
        l_cecTaskStateQueued.cpiState   = CPI_IDLE;
        l_cecTaskStateQueued.taskData1  = 0;
        l_cecTaskStateQueued.taskData2  = 0;
        l_newTask   = true;
        success     = true;
        DEBUG_PRINT(MSG_STAT, ("CEC: Queued OneTouch Task\n"));
    }

    return(success);
}


//------------------------------------------------------------------------------
// Function:    SI_CecHandler
// Description: Polls the send/receive state of the CPI hardware and runs the
//              current task, if any.
//
//              A task is used to handle cases where we must send and receive
//              a specific set of CEC commands.
//------------------------------------------------------------------------------

uint8_t SI_CecHandler (uint8_t currentPort, BOOL returnTask)
{
    bool_t cdcCalled  = false;
    SI_CpiStatus_t  cecStatus;

    l_portSelect = currentPort;

    /* Get the CEC transceiver status and pass it to the    */
    /* current task.  The task will send any required       */
    /* CEC commands.                                        */
    SI_CpiStatus(&cecStatus);
    si_TaskServer(&cecStatus);

    /* Now look to see if any CEC commands were received.   */
    if (cecStatus.rxState)
    {
        uint8_t         frameCount;
        SI_CpiData_t    cecFrame;

        /* Get CEC frames until no more are present.    */
        cecStatus.rxState = 0;      // Clear activity flag

        frameCount = ((SiIRegioRead(REG_CEC_RX_COUNT) & 0xF0) >> 4);
        while (frameCount)
        {
            DEBUG_PRINT(MSG_DBG, ("\n%d frames in Rx Fifo\n", (int)frameCount ));

            if (SI_CpiRead(&cecFrame))
            {
                DEBUG_PRINT(MSG_STAT, ("Error in Rx Fsi_CecRxMsgHandlerifo \n"));
                break;
            }

            /* Send the frame through the RX message Handler chain. */
            for (;;)
            {
                if (si_CecRxMsgHandlerFirst( &cecFrame))	// We get a chance at it before the end user App
                    break;
                if (CpCecRxMsgHandler(&cecFrame))			// Let the end-user app have a shot at it.
                    break;
#if (IS_CDC == 1)
                if (si_CDCProcess(&cecFrame))				// Check CDC handler, ignore any error it returns
					cdcCalled = true;
#endif
                si_CecRxMsgHandlerLast(&cecFrame);			// We get a chance at it afer the end user App to cleanup
                break;
            }
        }
    }

#if (IS_CDC == 1)
    /* If CDC handler was not called in the RX loop at least once, we must call it now. */
    if (!cdcCalled)
    {
//        si_CDCProcess( );  // No message associated with this call. 		// JYP
    }
#endif

    if (l_portSelect != currentPort)
    {
        DEBUG_PRINT(MSG_DBG, ("\nNew port value returned: %02X (Return Task: %d)\n", (int)l_portSelect, (int)returnTask));
    }
    return(returnTask ? g_currentTask : l_portSelect);
}

