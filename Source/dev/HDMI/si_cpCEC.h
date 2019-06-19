//***************************************************************************
//!file     si_cpCEC.h
//!brief    CP 9387 Starter Kit CEC handler.
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef _CECHANDLER_H_
#define _CECHANDLER_H_
#include <si_datatypes.h>

//------------------------------------------------------------------------------
// API Function Templates
//------------------------------------------------------------------------------

BOOL CpArcEnable( uint8_t mode );
void CpHecEnable( BOOL enable );

char *CpCecTranslateLA( uint8_t bLogAddr );
char *CpCecTranslateOpcodeName( SI_CpiData_t *pMsg );
BOOL CpCecPrintCommand( SI_CpiData_t *pMsg, BOOL isTX );

extern BOOL CpCecRxMsgHandler( SI_CpiData_t *pCpi );

#endif // _CECHANDLER_H_



