//***************************************************************************
//!file     si_datatypes.h
//!brief    Silicon Image data type header (conforms to C99).
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef __SI_DATATYPES_H__
#define __SI_DATATYPES_H__

    /* C99 defined data types.  */

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
//typedef unsigned long  uint32_t;

typedef signed char    int8_t;
typedef signed short   int16_t;
//typedef signed long    int32_t;

    /* Emulate C99/C++ bool type    */

#ifdef __cplusplus
typedef bool bool_t;
#else
typedef enum
{
    false   = 0,
    true    = !(false)
} bool_t;

//typedef char BOOL;
#endif // __cplusplus




#define ROM     code        // 8051 type of ROM memory
#define XDATA   xdata       // 8051 type of external memory

//------------------------------------------------------------------------------
// Configuration defines used by hal_config.h
//------------------------------------------------------------------------------

//#define ENABLE      (0xFF)
//#define DISABLE     (0x00)

//#define BIT0                    0x01 		// JYP
//#define BIT1                    0x02
//#define BIT2                    0x04
//#define BIT3                    0x08
//#define BIT4                    0x10
//#define BIT5                    0x20
//#define BIT6                    0x40
#define BIT7                    0x80

#define CEC_NO_TEXT         0
#define CEC_NO_NAMES        1
#define CEC_ALL_TEXT        2
#define INCLUDE_CEC_NAMES   CEC_NO_TEXT

#define MSG_ALWAYS              0x00
#define MSG_STAT                0x01
#define MSG_DBG                 0x02
//#define DEBUG_PRINT(l,x)      if (l<=5) printf x
#define DEBUG_PRINT(l,x)


#define CBUS_SLAVE_ADDR 0xC8
#define CEC_SLAVE_ADDR 0xC0

#define SET_BITS    0xFF
#define CLEAR_BITS  0x00

uint8_t SiIRegioRead ( uint16_t regAddr );
void SiIRegioWrite ( uint16_t regAddr, uint8_t value );
void SiIRegioModify ( uint16_t regAddr, uint8_t mask, uint8_t value);
void SiIRegioWriteBlock ( uint16_t regAddr, uint8_t* buffer, uint16_t length);

#endif  // __SI_DATATYPES_H__

