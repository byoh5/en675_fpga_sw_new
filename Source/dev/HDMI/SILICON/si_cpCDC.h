//***************************************************************************
//!file     si_cpCDC.h
//!brief    CP 9387 Starter Kit CDC demonstration code.
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef _CDCHANDLER_H_
#define _CDCHANDLER_H_
#include "si_datatypes.h"

//------------------------------------------------------------------------------
// API Function Templates
//------------------------------------------------------------------------------

bool_t CpInitCDC( void );

bool_t si_CDCProcess( SI_CpiData_t *pCdcCmd );

void CpCDCInfoPrint ( void );
void CpCDCInquireStateTest ( void );
void CpCDCSingleDeviceConnectTest ( uint8_t devIndex );



#endif // _CECHANDLER_H_



