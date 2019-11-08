/* **************************************************************************
 File Name	: 	isp_af.h
 Description:	EN675 - Auto Focus function
 Designer	:	Lee, Wanhee
 Date		:	19. 7. 23
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#ifndef _ISP_AF_H_
#define	_ISP_AF_H_

#ifdef __ISP_LIB__
#else


#endif//__ISP_LIB__


extern BYTE gbAfDebugOn;

//extern int AE_GAIN_TOTAL;
//extern int AE_GAIN_TOTAL2;

extern void Af(void);
extern void AF_Init(void);
extern void Af_Debuging(BYTE bExcute);


#endif//_ISP_AF_H_
