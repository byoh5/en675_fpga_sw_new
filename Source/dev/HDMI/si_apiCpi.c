//***************************************************************************
//!file     si_apiCpi.c
//!brief    CP 9387 Starter Kit CPI functions.
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
//#include "../../cpu_main.h"
#include "dev.h"
#include "si_apiCpi.h"
#include "si_cpi_regs.h"
#include "si_cec_enums.h"
#include "TypeDefs.h"
#include "defs.h"
#include "i2c_master_sw.h"

//------------------------------------------------------------------------------
// Function:    SiIRegioReadBlock
// Description: Read a block of registers starting with the specified register.
//              The register address parameter is translated into an I2C
//              slave address and offset.
//              The block of data bytes is read from the I2C slave address
//              and offset.
//------------------------------------------------------------------------------

void SiIRegioReadBlock ( uint16_t regAddr, uint8_t* buffer, uint16_t length)
{
    I2C_ReadBlock(CEC_SLAVE_ADDR, (uint8_t)regAddr, buffer, length);
//    HalI2cBus0ReadBlock( l_regioDecodePage[ regAddr >> 8], (uint8_t)regAddr, buffer, length);
}
//------------------------------------------------------------------------------
// Function:    SiIRegioWriteBlock
// Description: Write a block of registers starting with the specified register.
//              The register address parameter is translated into an I2C slave
//              address and offset.
//              The block of data bytes is written to the I2C slave address
//              and offset.
//------------------------------------------------------------------------------

void SiIRegioWriteBlock ( uint16_t regAddr, uint8_t* buffer, uint16_t length)
{
    I2C_WriteBlock(CEC_SLAVE_ADDR, (uint8_t)regAddr, buffer, length);
    //HalI2cBus0WriteBlock( l_regioDecodePage[ regAddr >> 8], (uint8_t)regAddr, buffer, length);
}

//------------------------------------------------------------------------------
// Function:    SiIRegioRead
// Description: Read a one byte register.
//              The register address parameter is translated into an I2C slave
//              address and offset. The I2C slave address and offset are used
//              to perform an I2C read operation.
//------------------------------------------------------------------------------

uint8_t SiIRegioRead ( uint16_t regAddr )
{
    return (I2C_ReadByte(CEC_SLAVE_ADDR, (uint8_t)regAddr));
}

//------------------------------------------------------------------------------
// Function:    SiIRegioWrite
// Description: Write a one byte register.
//              The register address parameter is translated into an I2C
//              slave address and offset. The I2C slave address and offset
//              are used to perform an I2C write operation.
//------------------------------------------------------------------------------

void SiIRegioWrite ( uint16_t regAddr, uint8_t value )
{
    I2C_WriteByte(CEC_SLAVE_ADDR, (uint8_t)regAddr, value);
}


//------------------------------------------------------------------------------
// Function:    SiIRegioModify
// Description: Modify a one byte register under mask.
//              The register address parameter is translated into an I2C
//              slave address and offset. The I2C slave address and offset are
//              used to perform I2C read and write operations.
//
//              All bits specified in the mask are set in the register
//              according to the value specified.
//              A mask of 0x00 does not change any bits.
//              A mask of 0xFF is the same a writing a byte - all bits
//              are set to the value given.
//              When only some bits in the mask are set, only those bits are
//              changed to the values given.
//------------------------------------------------------------------------------

void SiIRegioModify ( uint16_t regAddr, uint8_t mask, uint8_t value)
{
    uint8_t abyte;

    abyte = I2C_ReadByte( CEC_SLAVE_ADDR, (uint8_t)regAddr );
    abyte &= (~mask);                                       //first clear all bits in mask
    abyte |= (mask & value);                                //then set bits from value
    I2C_WriteByte( CEC_SLAVE_ADDR, (uint8_t)regAddr, abyte);
}

//------------------------------------------------------------------------------
// Function:    SI_CpiSetLogicalAddr
// Description: Configure the CPI subsystem to respond to a specific CEC
//              logical address.
//------------------------------------------------------------------------------

BOOL SI_CpiSetLogicalAddr ( uint8_t logicalAddress )
{
    uint8_t capture_address[2];
    uint8_t capture_addr_sel = 0x01;

    capture_address[0] = 0;
    capture_address[1] = 0;
    if( logicalAddress < 8 )
    {
        capture_addr_sel = capture_addr_sel << logicalAddress;
        capture_address[0] = capture_addr_sel;
    }
    else
    {
        capture_addr_sel   = capture_addr_sel << ( logicalAddress - 8 );
        capture_address[1] = capture_addr_sel;
    }

        // Set Capture Address

    SiIRegioWriteBlock(REG_CEC_CAPTURE_ID0, capture_address, 2 );
    SiIRegioWrite( REG_CEC_TX_INIT, logicalAddress );
	DEBUG_PRINT(MSG_STAT,("CEC: logicalAddress: 0x%x\n", (int)logicalAddress));

    return( true );
}

//------------------------------------------------------------------------------
// Function:    SI_CpiSendPing
// Description: Initiate sending a ping, and used for checking available
//                       CEC devices
//------------------------------------------------------------------------------

void SI_CpiSendPing ( uint8_t bCECLogAddr )
{
    SiIRegioWrite( REG_CEC_TX_DEST, BIT_SEND_POLL | bCECLogAddr );
}

//------------------------------------------------------------------------------
// Function:    SI_CpiWrite
// Description: Send CEC command via CPI register set
//------------------------------------------------------------------------------

BOOL SI_CpiWrite( SI_CpiData_t *pCpi )
{
    uint8_t cec_int_status_reg[2];

#if (INCLUDE_CEC_NAMES > CEC_NO_TEXT)
    CpCecPrintCommand( pCpi, true );
#endif
    SiIRegioModify( REG_CEC_DEBUG_3, BIT_FLUSH_TX_FIFO, BIT_FLUSH_TX_FIFO );  // Clear Tx Buffer

        /* Clear Tx-related buffers; write 1 to bits to be cleared. */

    cec_int_status_reg[0] = 0x64 ; // Clear Tx Transmit Buffer Full Bit, Tx msg Sent Event Bit, and Tx FIFO Empty Event Bit
    cec_int_status_reg[1] = 0x02 ; // Clear Tx Frame Retranmit Count Exceeded Bit.
    SiIRegioWriteBlock( REG_CEC_INT_STATUS_0, cec_int_status_reg, 2 );

        /* Send the command */

    SiIRegioWrite( REG_CEC_TX_DEST, pCpi->srcDestAddr & 0x0F );           // Destination
    SiIRegioWrite( REG_CEC_TX_COMMAND, pCpi->opcode );
    SiIRegioWriteBlock( REG_CEC_TX_OPERAND_0, pCpi->args, pCpi->argCount );
    SiIRegioWrite( REG_CEC_TRANSMIT_DATA, BIT_TRANSMIT_CMD | pCpi->argCount );

    return( true );
}

//------------------------------------------------------------------------------
// Function:    SI_CpiRead
// Description: Reads a CEC message from the CPI read FIFO, if present.
//------------------------------------------------------------------------------

BOOL SI_CpiRead( SI_CpiData_t *pCpi )
{
    BOOL    error = false;
    uint8_t argCount;

    argCount = SiIRegioRead( REG_CEC_RX_COUNT );

    if ( argCount & BIT_MSG_ERROR )
    {
        error = true;
    }
    else
    {
        pCpi->argCount = argCount & 0x0F;
        pCpi->srcDestAddr = SiIRegioRead( REG_CEC_RX_CMD_HEADER );
        pCpi->opcode = SiIRegioRead( REG_CEC_RX_OPCODE );
        if ( pCpi->argCount )
        {
            SiIRegioReadBlock( REG_CEC_RX_OPERAND_0, pCpi->args, pCpi->argCount );
        }
    }

        // Clear CLR_RX_FIFO_CUR;
        // Clear current frame from Rx FIFO

    SiIRegioModify( REG_CEC_RX_CONTROL, BIT_CLR_RX_FIFO_CUR, BIT_CLR_RX_FIFO_CUR );

#if (INCLUDE_CEC_NAMES > CEC_NO_TEXT)
    if ( !error )
    {
        CpCecPrintCommand( pCpi, false );
    }
#endif
    return( error );
}


//------------------------------------------------------------------------------
// Function:    SI_CpiStatus
// Description: Check CPI registers for a CEC event
//------------------------------------------------------------------------------

BOOL SI_CpiStatus( SI_CpiStatus_t *pStatus )
{
    uint8_t cecStatus[2];

    pStatus->txState    = 0;
    pStatus->cecError   = 0;
    pStatus->rxState    = 0;

    SiIRegioReadBlock( REG_CEC_INT_STATUS_0, cecStatus, 2);

    if ( (cecStatus[0] & 0x7F) || cecStatus[1] )
    {
        DEBUG_PRINT(MSG_STAT,("\nCEC Status: %02X %02X\n", (int) cecStatus[0], (int) cecStatus[1]));

            // Clear interrupts

        if ( cecStatus[1] & BIT_FRAME_RETRANSM_OV )
        {
            DEBUG_PRINT(MSG_DBG,("\n!TX retry count exceeded! [%02X][%02X]\n",(int) cecStatus[0], (int) cecStatus[1]));

                /* Flush TX, otherwise after clearing the BIT_FRAME_RETRANSM_OV */
                /* interrupt, the TX command will be re-sent.                   */

            SiIRegioModify( REG_CEC_DEBUG_3,BIT_FLUSH_TX_FIFO, BIT_FLUSH_TX_FIFO );
        }

            // Clear set bits that are set

        SiIRegioWriteBlock( REG_CEC_INT_STATUS_0, cecStatus, 2 );

            // RX Processing

        if ( cecStatus[0] & BIT_RX_MSG_RECEIVED )
        {
            pStatus->rxState = 1;   // Flag caller that CEC frames are present in RX FIFO
        }

            // RX Errors processing

        if ( cecStatus[1] & BIT_SHORT_PULSE_DET )
        {
            pStatus->cecError |= SI_CEC_SHORTPULSE;
        }

        if ( cecStatus[1] & BIT_START_IRREGULAR )
        {
            pStatus->cecError |= SI_CEC_BADSTART;
        }

        if ( cecStatus[1] & BIT_RX_FIFO_OVERRUN )
        {
            pStatus->cecError |= SI_CEC_RXOVERFLOW;
        }

            // TX Processing

        if ( cecStatus[0] & BIT_TX_FIFO_EMPTY )
        {
            pStatus->txState = SI_TX_WAITCMD;
        }
        if ( cecStatus[0] & BIT_TX_MESSAGE_SENT )
        {
            pStatus->txState = SI_TX_SENDACKED;
        }
        if ( cecStatus[1] & BIT_FRAME_RETRANSM_OV )
        {
            pStatus->txState = SI_TX_SENDFAILED;
        }
    }

    return( true );
}

//------------------------------------------------------------------------------
// Function:    SI_CpiGetLogicalAddr
// Description: Get Logical Address
//------------------------------------------------------------------------------

uint8_t SI_CpiGetLogicalAddr( void )
{
    return SiIRegioRead( REG_CEC_TX_INIT);
}

//------------------------------------------------------------------------------
// Function:    SI_CpiInit
// Description: Initialize the CPI subsystem for communicating via CEC
//------------------------------------------------------------------------------

BOOL SI_CpiInit( void )
{

#ifdef DEV_SUPPORT_CEC_FEATURE_ABORT
    // Turn on CEC auto response to <Abort> command.
    SiIRegioWrite( CEC_OP_ABORT_31, BIT7 );
#else
    // Turn off CEC auto response to <Abort> command.
    SiIRegioWrite( CEC_OP_ABORT_31, CLEAR_BITS );
#endif

#ifdef DEV_SUPPORT_CEC_CONFIG_CPI_0
    // Bit 4 of the CC Config reister must be cleared to enable CEC
	SiIRegioModify (REG_CEC_CONFIG_CPI, 0x10, 0x00);
#endif

    // initialized he CEC CEC_LOGADDR_TV logical address
    if ( !SI_CpiSetLogicalAddr( CEC_LOGADDR_PLAYBACK1 ))
    {
        DEBUG_PRINT( MSG_ALWAYS, ("\n Cannot init CPI/CEC"));
        return( false );
    }

    return( true );
}

