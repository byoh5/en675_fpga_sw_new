/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
//#include "../cpu_main.h"

#include "dev.h"

#ifdef __HDMI_SILICON__

#include "defs.h"
#include "TypeDefs.h"
#include "Constants.h"

//------------------------------------------------------------------------------
// Local macros to simplify bit operations
//------------------------------------------------------------------------------
//#define GET_SDA()     ( PinI2CSDA )
//#define GET_SCL()     ( PinI2CSCL )
//#define SET_SDA()     ( PinI2CSDA = 1 )
//#define SET_SCL()     ( PinI2CSCL = 1 )
//#define CLEAR_SDA()   ( PinI2CSDA = 0 )
//#define CLEAR_SCL()   ( PinI2CSCL = 0 )


//------------------------------------------------------------------------------
// Local constants (function parameters)
//------------------------------------------------------------------------------
#define READ   1
#define WRITE  0

#define LAST_BYTE      1
#define NOT_LAST_BYTE  0



//------------------------------------------------------------------------------
// Function: SetSCLHigh
// Description:
//------------------------------------------------------------------------------
//static void SetSCLHigh(void)
//{
//    // set SCL high, and wait for it to go high in case slave is clock stretching
//    SET_SCL();
//    while (!GET_SCL())
//    {
//        /* do nothing - just wait */
//    }
//}


//------------------------------------------------------------------------------
// Function: SendByte
// Description:
//------------------------------------------------------------------------------
//static byte SendByte(byte abyte)
//{
//    byte error = 0;
//    byte i;
//
//    for (i = 0; i < 8; i++)
//    {
//        if (abyte & 0x80)
//            SET_SDA();      //send each bit, MSB first
//        else
//            CLEAR_SDA();
//
//        SetSCLHigh();       //do clock cycle
//        CLEAR_SCL();
//
//        abyte <<= 1;        //shift over to get next bit
//    }
//
//    SET_SDA();              //listen for ACK
//    SetSCLHigh();
//
//    if (GET_SDA())          //error if no ACK
//        error = 1;
//
//    CLEAR_SCL();
//
//    return (error);     //return 0 for no ACK, 1 for ACK received
//}


//------------------------------------------------------------------------------
// Function: SendAddr
// Description:
//------------------------------------------------------------------------------
//static byte SendAddr(byte addr, byte read)
//{
//    volatile byte x = 0;    //delay variable
//
//    //generate START condition
//    SET_SCL();
//    x++;            //short delay to keep setup times in spec
//    CLEAR_SDA();
//    x++;
//    CLEAR_SCL();
//    x++;
//
//    return (SendByte(addr|read));  //send address byte with read/write bit
//}

//------------------------------------------------------------------------------
// Function: GetByte
// Description:
//------------------------------------------------------------------------------
//static byte GetByte(byte lastbyte)
//{
//    byte abyte = 0;
//    byte i;
//
//    for (i = 0; i < 8; i++)  //get each bit, MSB first
//    {
//        SetSCLHigh();
//
//        abyte <<= 1;    //shift result over to make room for next bit
//
//        if (GET_SDA())
//            abyte++;    //same as 'abyte |= 1', only faster
//
//        CLEAR_SCL();
//    }
//
//    if (lastbyte)
//        SET_SDA();      //do not ACK last byte read
//    else
//        CLEAR_SDA();
//
//    SetSCLHigh();
//    CLEAR_SCL();
//    SET_SDA();
//
//    return (abyte);
//}

//------------------------------------------------------------------------------
// Function: SendStop
// Description:
//------------------------------------------------------------------------------
//static void SendStop(void)
//{
//    CLEAR_SDA();
//    SetSCLHigh();
//    SET_SDA();
//}


//------------------------------------------------------------------------------
// Function: I2C_WriteByte
// Description:
//------------------------------------------------------------------------------
void I2C_WriteByte(byte deviceID, byte offset, byte value)
{
//    SendAddr(deviceID, WRITE);
//    SendByte(offset);
//    SendByte(value);
//    SendStop();

	HDMI_TwiWrite(deviceID, offset, value);
}


//------------------------------------------------------------------------------
// Function: I2C_ReadByte
// Description:
//------------------------------------------------------------------------------
byte I2C_ReadByte(byte deviceID, byte offset)
{
    byte abyte;

//    SendAddr(deviceID, WRITE);
//    SendByte(offset);
//    SendAddr(deviceID, READ);
//
//    abyte = GetByte(LAST_BYTE);  //get single byte, same as last byte, no ACK
//    SendStop();
//
//    return (abyte);

	HDMI_TwiRead(deviceID, offset, &abyte);
	return (abyte);

}


//------------------------------------------------------------------------------
// Function: I2C_WriteBlock
// Description:
//------------------------------------------------------------------------------
byte I2C_WriteBlock(byte deviceID, byte offset, byte *buffer, word length)
{
//    byte i;
//
//    SendAddr(deviceID, WRITE);
//    SendByte(offset);
//
//    for (i = 0; i < length; i++)
//    {
//        SendByte(buffer[i]);
//    }
//
//    SendStop();
//
//    return (0);

	HDMI_TwiWriteBlock(deviceID, offset, buffer, length);
	return 0;
}


//------------------------------------------------------------------------------
// Function: I2C_ReadBlock
// Description:
//------------------------------------------------------------------------------
byte I2C_ReadBlock(byte deviceID, byte offset, byte *buffer, word length)
{
//    byte i;
//
//    SendAddr(deviceID, WRITE);
//    SendByte(offset);
//    SendAddr(deviceID, READ);
//
//    length--;   //get all except last byte
//    for (i = 0; i < length; i++)
//    {
//        buffer[i] = GetByte(NOT_LAST_BYTE);
//    }
//
//    buffer[i] = GetByte(LAST_BYTE);  //get last byte, no ACK
//    SendStop();
//
//    return (0);

	HDMI_TwiReadBlock(deviceID, offset, buffer, length);

	return 0;
}


//------------------------------------------------------------------------------
// Function: I2C_ReadSegmentBlock
// Description:
//------------------------------------------------------------------------------
#ifndef CBUS_EDID

byte I2C_ReadSegmentBlock(byte deviceID, byte segment, byte offset, byte *buffer, word length)
{
//    byte i;
//
//	SendAddr(EDID_SEG_ADDR, WRITE);
//	SendByte(segment);
//
//    SendAddr(deviceID, WRITE);
//    SendByte(offset);
//    SendAddr(deviceID, READ);
//
//    length--;   //get all except last byte
//    for (i = 0; i < length; i++)
//    {
//        buffer[i] = GetByte(NOT_LAST_BYTE);
//    }
//
//    buffer[i] = GetByte(LAST_BYTE);  //get last byte, no ACK
//    SendStop();
//
//    return (0);

 	HDMI_TwiReadSegmentBlock(deviceID, segment, offset, buffer, length);

	return 0;

}
#endif

#endif

