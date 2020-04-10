/* **************************************************************************
 File Name	:	EN675_SENS_IMX415.c
 Description:	EN675 - IMX415 device driver
 Designer	:	Kim, Sunghoon
 Date		:	20. 3. 31
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#define __SENS_LIST__
#include "dev_model.h"				// EN675 - __SENS_LIST__
#define model_Sens	SENS_IMX415

#include "isp_lib_main.h"

//-------------------------------------------------------------------------------------------------

#if 0
	// Sensor Master Mode,	MIPI 4ch, 12bit,	INCK : 27MHz
	const WORD gwTblIMX415[][2] = 	{
									//0x30**
									{0x3008,    0x5D},
									{0x300A,    0x42},

                                    // Crop
									{0x301C,    0x00},

                                    // Binning
                                    {0x3020,    0x00},
                                    {0x3021,    0x00},
                                    {0x3022,    0x00},

									{0x3024,    0xCA},     // VMAX - 2250 line
									{0x3025,    0x08},     //  "

									{0x3028,    0x98},     // HMAX - 0x898:15P, 0x44c:30P
									{0x3029,    0x08},     //  "

                                    {0x3031,    0x01},		// ADBIT - 0:10bit, 1:12bit
									{0x3033,    0x05},
                                    // Crop
									{0x3040,    0x00},     // PIX_HST
									{0x3041,    0x00},     //   "
									{0x3042,    0x18},		// PIX_HWIDTH
									{0x3043,    0x0F},     //   "
									{0x3044,    0x18},     // PIX_VST
									{0x3045,    0x0F},     //   "
									{0x3046,    0x20},     // PIX_VWIDTH
									{0x3047,    0x11},     //   "

									{0x3050,    0x08},		//	SHR0[7:0]
									{0x30C1,    0x00},

                                    // Binning
									{0x30D9,    0x06},
									{0x30DA,    0x02},

									{0x3116,    0x23},
									{0x3118,    0xC6},
									{0x311A,    0xE7},
									{0x311E,    0x23},
									{0x32D4,    0x21},
									{0x32EC,    0xA1},
									{0x3452,    0x7F},
									{0x3453,    0x03},
									{0x358A,    0x04},
									{0x35A1,    0x02},
									{0x36BC,    0x0C},
									{0x36CC,    0x53},
									{0x36CD,    0x00},
									{0x36CE,    0x3C},
									{0x36D0,    0x8C},
									{0x36D1,    0x00},
									{0x36D2,    0x71},
									{0x36D4,    0x3C},
									{0x36D6,    0x53},
                                    {0x36D7,    0x00},
									{0x36D8,    0x71},
									{0x36DA,    0x8C},
									{0x36DB,    0x00},
									// Binning
                                    {0x3701,    0x03},


									{0x3724,    0x02},
									{0x3726,    0x02},
									{0x3732,    0x02},

									{0x3734,    0x03},
									{0x3736,    0x03},
									{0x3742,    0x03},

									{0x3862,    0xE0},

									{0x38CC,    0x30},
									{0x38CD,    0x2F},

									{0x395C,    0x0C},

									{0x3A42,    0xD1},

									{0x3A4C,    0x77},

									{0x3AE0,    0x02},

									{0x3AEC,    0x0C},

									{0x3B00,    0x2E},

									{0x3B06,    0x29},

									{0x3B98,    0x25},
									{0x3B99,    0x21},

									{0x3B9B,    0x13},
									{0x3B9C,    0x13},
									{0x3B9D,    0x13},
									{0x3B9E,    0x13},

									{0x3BA1,    0x00},
									{0x3BA2,    0x06},
									{0x3BA3,    0x0B},
									{0x3BA4,    0x10},
									{0x3BA5,    0x14},
                                    {0x3BA6,    0x18},
                                    {0x3BA7,    0x1A},
                                    {0x3BA8,    0x1A},
                                    {0x3BA9,    0x1A},

                                    {0x3BAC,    0xED},
                                    {0x3BAD,    0x01},
									{0x3BAE,    0xF6},
									{0x3BAF,    0x02},

									{0x3BB0,    0xA2},
									{0x3BB1,    0x03},
									{0x3BB2,    0xE0},
									{0x3BB3,    0x03},
									{0x3BB4,    0xE0},
									{0x3BB5,    0x03},
									{0x3BB6,    0xE0},
									{0x3BB7,    0x03},
									{0x3BB8,    0xE0},
									{0x3BBA,    0xE0},
                                    {0x3BBC,    0xDA},
									{0x3BBE,    0x88},
									{0x3BC0,    0x44},
									{0x3BC2,    0x7B},
									{0x3BC4,    0xA2},
									{0x3BC8,    0xBD},
                                    {0x3BCA,    0xBD},

									{0x4004,    0xC0},
									{0x4005,    0x06},

									{0x400C,    0x00},
									{0x4018,    0x7F},

									{0x401A,    0x37},
									{0x401C,    0x37},

                                    {0x401E,    0xF7},
                                    {0x401F,    0x00},
                                    {0x4020,    0x3F},
                                    {0x4022,    0x6F},
                                    {0x4024,    0x3F},
                                    {0x4026,    0x5F},
                                    {0x4028,    0x2F},
                                    {0x4074,    0x01}
									};
#else
		const WORD gwTblIMX415[][2] = 	{
									{0x3008,    0x5D},
									{0x3009,    0x00},
									{0x300A,    0x42},
									{0x300B,	0x00},
									{0x301C,    0x00},
									{0x3020,    0x00},
									{0x3021,    0x00},
									{0x3022,    0x00},
									{0x3024,    0xCA},
									{0x3025,    0x08},
									{0x3028,    0x98},
									{0x3029,    0x08},
									{0x302C,    0x00},
									{0x302D,    0x00},
									{0x3031,    0x00},
									{0x3032,    0x01},
									{0x3033,    0x05},
									{0x3040,    0x00},
									{0x3041,    0x00},
									{0x3042,    0x18},
									{0x3043,    0x0F},
									{0x3044,    0x18},
									{0x3045,    0x0F},
									{0x3046,    0x20},
									{0x3047,    0x11},
									{0x3050,    0x08},
									{0x3060,    0x25},
									{0x30C1,    0x00},
									{0x30CF,    0x00},
									{0x30E2,    0x00},
									{0x30E3,    0x00},
									{0x30D9,    0x06},
									{0x30DA,    0x02},
									{0x3115,    0x00},
									{0x3116,    0x23},
									{0x3118,    0xC6},
									{0x3119,    0x00},
									{0x311A,    0xE7},
									{0x311B,    0x00},
									{0x311E,    0x23},
									{0x3260,    0x01},
									{0x32D4,    0x21},
									{0x32EC,    0xA1},
									{0x3452,    0x7F},
									{0x3453,    0x03},
									{0x358A,    0x04},
									{0x35A1,    0x02},
									{0x36BC,    0x0C},
									{0x36CC,    0x53},
									{0x36CD,    0x00},
									{0x36CE,    0x3C},
									{0x36D0,    0x8C},
									{0x36D1,    0x00},
									{0x36D2,    0x71},
									{0x36D4,    0x3C},
									{0x36D6,    0x53},
									{0x36D7,    0x00},
									{0x36D8,    0x71},
									{0x36DA,    0x8C},
									{0x36DB,    0x00},
									{0x3701,    0x00},
									{0x3724,    0x02},
									{0x3726,    0x02},
									{0x3732,    0x02},
									{0x3734,    0x03},
									{0x3736,    0x03},
									{0x3742,    0x03},
									{0x3862,    0xE0},
									{0x38CC,    0x30},
									{0x38CD,    0x2F},
									{0x395C,    0x0C},
									{0x3A42,    0xD1},
									{0x3A4C,    0x77},
									{0x3AE0,    0x02},
									{0x3AEC,    0x0C},
									{0x3B00,    0x2E},
									{0x3B06,    0x29},
									{0x3B98,    0x25},
									{0x3B99,    0x21},
									{0x3B9B,    0x13},
									{0x3B9C,    0x13},
									{0x3B9D,    0x13},
									{0x3B9E,    0x13},
									{0x3BA1,    0x00},
									{0x3BA2,    0x06},
									{0x3BA3,    0x0B},
									{0x3BA4,    0x10},
									{0x3BA5,    0x14},
									{0x3BA6,    0x18},
									{0x3BA7,    0x1A},
									{0x3BA8,    0x1A},
									{0x3BA9,    0x1A},
									{0x3BAC,    0xED},
									{0x3BAD,    0x01},
									{0x3BAE,    0xF6},
									{0x3BAF,    0x02},
									{0x3BB0,    0xA2},
									{0x3BB1,    0x03},
									{0x3BB2,    0xE0},
									{0x3BB3,    0x03},
									{0x3BB4,    0xE0},
									{0x3BB5,    0x03},
									{0x3BB6,    0xE0},
									{0x3BB7,    0x03},
									{0x3BB8,    0xE0},
									{0x3BBA,    0xE0},
									{0x3BBC,    0xDA},
									{0x3BBE,    0x88},
									{0x3BC0,    0x44},
									{0x3BC2,    0x7B},
									{0x3BC4,    0xA2},
									{0x3BC8,    0xBD},
									{0x3BCA,    0xBD},
									{0x4001,    0x03},
									{0x4004,    0xC0},
									{0x4005,    0x06},
									{0x400C,    0x00},
									{0x4018,    0x7F},
									{0x4019,    0x00},
									{0x401A,    0x37},
									{0x401B,    0x00},
									{0x401C,    0x37},
									{0x401D,    0x00},
									{0x401E,    0xF7},
									{0x401F,    0x00},
									{0x4020,    0x3F},
									{0x4021,    0x00},
									{0x4022,    0x6F},
									{0x4023,	0x00},
									{0x4024,    0x3F},
									{0x4025,	0x00},
									{0x4026,    0x5F},
									{0x4027,	0x00},
									{0x4028,    0x2F},
									{0x4029,	0x00},
									{0x4074,    0x01}
								};
#endif

void InitSensRun(void)
{
	SetSens(0x3000, 0x1);		WaitXus(1000);
	SetSens(0x3002, 0x1);		WaitXus(1000);
	SetSens(0x3001, 0x1);

	for(UINT i=0;i<ARRAY_SIZE(gwTblIMX415); i++)
	{
		SetSens(gwTblIMX415[i][0],gwTblIMX415[i][1]);
		//WaitXus(200);
	}

	SetSens(0x3001, 0x0);
	SetSens(0x3000, 0x0);		WaitXus(20000);
	SetSens(0x3002, 0x0);		WaitXus(20000);

	SetSens(0x30e2, 0x0);		WaitXus(20000);
}

BYTE gbBurstBuf[3]={0,0,0};
BYTE gbMirFlpBuf = 0x00;	// 센서마다 초기값이 다름 !!!

void SensMirror(BYTE aOn)
{
	gbMirFlpBuf = (aOn) ? (gbMirFlpBuf|0x01) : (gbMirFlpBuf&0xFE);
	SetSens(0x3030, gbMirFlpBuf);
}

void SensFlip(BYTE aOn)
{
	gbMirFlpBuf = (aOn) ? (gbMirFlpBuf|0x02) : (gbMirFlpBuf&0xFD);
	SetSens(0x3030, gbMirFlpBuf);
}

void ISRT0 AE_SHTw(UINT val)				// SHR0
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x0f;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3050, gbBurstBuf, 3);
}

void ISRT0 AE_DSSw(UINT val)				// VMAX
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x0f;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3024, gbBurstBuf, 3);
}

void ISRT0 AE_AGCw(UINT val)				// GAIN_PGC_0 for LEF (NOR & DOL2 & DOL3)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[1]=(val>> 8)&0x01;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3090, gbBurstBuf, 2);
}

void ISRT0 AE_FREQw(UINT aVtw, UINT aHtw)	// HMAX
{
	CHANGE_VAL(UINT,0xffffffff,aHtw)

	gbBurstBuf[1]=(aHtw>> 8)&0xff;
	gbBurstBuf[0]=(aHtw>> 0)&0xff;
	SetSensBurst(0x3028, gbBurstBuf, 2);
}

void ISRT0 AE_SHR1w(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3054, gbBurstBuf, 3);
}

void ISRT0 AE_SHR2w(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3058, gbBurstBuf, 3);
}

void ISRT0 AE_RHS1w(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3060, gbBurstBuf, 3);
}

void ISRT0 AE_RHS2w(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3064, gbBurstBuf, 3);
}

void ISRT0 AE_GAIN1w(UINT val)				// GAIN_PGC_1 for SEF1 (DOL2 & DOL3)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[1]=(val>> 8)&0x01;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3092, gbBurstBuf, 2);
}

void ISRT0 AE_GAIN2w(UINT val)				// GAIN_PGC_2 for SEF2 (DOL3)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[1]=(val>> 8)&0x01;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3094, gbBurstBuf, 2);
}

void ISRT0 SensUpdate(void)
{
	gbSensUpdate = 1;

	AE_FREQw(0,0);
	AE_DSSw(0);
	AE_SHTw(0);
	AE_AGCw(0);

	AE_SHR1w(0);
	AE_SHR2w(0);
	AE_RHS1w(0);
	AE_RHS2w(0);
	AE_GAIN1w(0);
	AE_GAIN2w(0);

	gbSensUpdate = 0;
}


#undef SetSensBurst		// 아래의 코드(AeSHT(), AeAGC() 등) 에서 SetSensBurst 나 SetSens 의 사용을 방지 -> CHANGE_VAL(...) 적용 불가 -> AeDev() 사용 안됨
#undef SetSens			//		"
//----------------------------------------------------------------------------------------

	#define AE_GAIN_UNIT_MAX		(240)				// Sensor gain step by Unit
	#define AE_GAIN_UNIT_EA			(1)					// Sensor gain Unit EA
	#define AE_GAIN_UNIT_OVR		(0)					// Sensor gain extended step

	const int AGC_POS_MAX = (((AE_GAIN_UNIT_MAX*AE_GAIN_UNIT_EA)+AE_GAIN_UNIT_OVR)*(0.3/0.1));


const BYTE gbSensShtDly = 2;	// ShtCtrl()에서 8이하, 0 : 1Frame 후 AE ODM적용, 1 : 2Frame 후 AE ODM적용, 2 : 3Frame 후 AE ODM적용, 3 : 4Frame 후 AE ODM적용
const BYTE gbSensAgcDly = 2;	// AgcCtrl()에서 8이하, 0 : 1Frame 후 AE ODM적용, 1 : 2Frame 후 AE ODM적용, 2 : 3Frame 후 AE ODM적용, 3 : 4Frame 후 AE ODM적용

const BYTE	gbVHssAgcTbl[]   = {
								 0,	// 0 : dummy
								 0,	// 1
								 0,	// 2
								 0,	// 3
								 6,	// 4 : MIN
								 5,	// 5
								 5,	// 6
								 4,	// 7
								 3,	// 8
								 3,	// 9
								 3,	// 10
								 3,	// 11
								 2,	// 12
								 2,	// 13
								 2,	// 14
								 2,	// 15
								 2,	// 16
								 1,	// 17
								 1,	// 18
								 1,	// 19
								 1,	// 20
								 1,	// 21
								 1,	// 22
								 1,	// 23
								 1,	// 24
								 1,	// 25
								 1,	// 26
								 1,	// 27
								 1,	// 28
								 1,	// 29
								 1,	// 30
								 1};// 31

const UINT gVHssAgcTblEa = ARRAY_SIZE(gbVHssAgcTbl);

int WdrLShtMax(const BYTE abDssRatioLmt);	// VMAX 재설정 때문에 필요

UINT gnVMAX = 2250;

#define VMAX					gnVMAX

#define NOR_SHR0_MAX			(VMAX - 4)							// 2250 - 4 = 2246
#define NOR_SHR0_MIN			8									// 8

#define NOR_SHT_MIN				(VMAX - NOR_SHR0_MAX /*+ 0.12*/)	// 2250 - 2246 = 4     ※30P : 1.79us / (1/2250/30*1000*1000) ≒ 0.12,  AD 10bit : 1.79us, AD 12bit : 2.68us
#define NOR_SHT_MAX				(VMAX - NOR_SHR0_MIN /*+ 0.12*/)	// 2250 - 8    = 2242  ※15P : 1.79us / (1/2250/15*1000*1000) ≒ 0.06

#define NOR_SHT_to_SHR0(SHT)	(VMAX - (SHT) /*+ 0.12*/)

#define LWDR_BRL				(VMAX-22)							// = 2228
#define LWDR_2P_FSC				(VMAX<<1)							// = 4500

#define LWDR_2P_RHS1			501									// = 501 (Comment set value is guarantee by Sony Doc) (satisfy formulas : 4n+1)

#define LWDR_2P_SEF1_MAX		(LWDR_2P_RHS1 - 8)					// SEF1 Max at DOL 2P,  501 - 8 = 493
#define LWDR_2P_SEF1_MIN		(               9)					// SEF1 Min at DOL 2P,      + 9 = 9
#define LWDR_2P_LEF_MAX			(LWDR_2P_FSC  - 8)					// LEF  Max at DOL 2P, 4500 - 8 = 4492
#define LWDR_2P_LEF_MIN			(LWDR_2P_RHS1 + 9)					// LEF  Min at DOL 2P,  501 + 9 = 510

#define LWDR_2P_S_MIN			(LWDR_2P_RHS1 - LWDR_2P_SEF1_MAX)	// S Min shutter line at DOL 2P,  501 -  493 = 8
#define LWDR_2P_S_MAX			(LWDR_2P_RHS1 - LWDR_2P_SEF1_MIN)	// S Max shutter line at DOL 2P,  501 -    9 = 492
#define LWDR_2P_L_MIN			(LWDR_2P_FSC  - LWDR_2P_LEF_MAX )	// L Min shutter line at DOL 2P, 4500 - 4492 = 8
#define LWDR_2P_L_MAX			(LWDR_2P_FSC  - LWDR_2P_LEF_MIN )	// L Max shutter line at DOL 2P, 4500 -  510 = 3990

#define LWDR_2P_S_to_SEF1(S)	(LWDR_2P_RHS1 - (S))				// S to SEF1 for Sensor Reg. Setting at DOL 2P
#define LWDR_2P_L_to_LEF(L)		(LWDR_2P_FSC  - (L))				// L to LEF  for Sensor Reg. Setting at DOL 2P

#define LWDR_3P_FSC				(VMAX<<2)							// = 9000
#define LWDR_3P_RHS2			((6*384)*VMAX/gnAeVtw + 2)			// = 2306, 6n+2 조건 맞음
#define LWDR_3P_RHS1			((6*336)*VMAX/gnAeVtw + 1)			// = 2017, 6n+2 조건 맞음

#define LWDR_3P_SEF2_MAX		(LWDR_3P_RHS2 - 12)					// SEF2 Max at DOL 3P, 2306 - 12 = 2294
#define LWDR_3P_SEF2_MIN		(LWDR_3P_RHS1 + 13)					// SEF2 Min at DOL 3P, 2017 + 13 = 2030
#define LWDR_3P_SEF1_MAX		(LWDR_3P_RHS1 - 12)					// SEF1 Max at DOL 3P, 2017 - 12 = 2005
#define LWDR_3P_SEF1_MIN		(               13)					// SEF1 Min at DOL 3P,      + 13 = 13
#define LWDR_3P_LEF_MAX			(LWDR_3P_FSC  - 12)					// LEF  Max at DOL 3P, 9000 - 12 = 8988
#define LWDR_3P_LEF_MIN			(LWDR_3P_RHS2 + 13)					// LEF  Min at DOL 3P, 2306 + 13 = 2319

#define LWDR_3P_S_MIN			(LWDR_3P_RHS2 - LWDR_3P_SEF2_MAX)	// S Min shutter line at DOL 3P, 2306 - 2294 = 12
#define LWDR_3P_S_MAX			(LWDR_3P_RHS2 - LWDR_3P_SEF2_MIN)	// S Max shutter line at DOL 3P, 2306 - 2030 = 276
#define LWDR_3P_M_MIN			(LWDR_3P_RHS1 - LWDR_3P_SEF1_MAX)	// M Min shutter line at DOL 3P, 2017 - 2005 = 12
#define LWDR_3P_M_MAX			(LWDR_3P_RHS1 - LWDR_3P_SEF1_MIN)	// M Max shutter line at DOL 3P, 2017 - 13   = 1002
#define LWDR_3P_L_MIN			(LWDR_3P_FSC  - LWDR_3P_LEF_MAX )	// L Min shutter line at DOL 3P, 9000 - 8988 = 12
#define LWDR_3P_L_MAX			(LWDR_3P_FSC  - LWDR_3P_LEF_MIN )	// L Max shutter line at DOL 3P, 9000 - 2319 = 6681

#define LWDR_3P_S_to_SEF2(S)	(LWDR_3P_RHS2 - (S))				// S to SEF2 for Sensor Reg. Setting at DOL 3P
#define LWDR_3P_M_to_SEF1(M)	(LWDR_3P_RHS1 - (M))				// M to SEF1 for Sensor Reg. Setting at DOL 3P
#define LWDR_3P_L_to_LEF(L)		(LWDR_3P_FSC  - (L))				// L to LEF  for Sensor Reg. Setting at DOL 3P

void ISRT AeDSS(UINT anVal)
{
	UINT i;

	for(i=1; i<=64; i++) {
		if(WdrLShtMax(i) >= anVal) break;
	}

	if(giCurDss != (i-1)) {
		giCurDss = i-1;
		VMAX = i * gnAeVtw;
		AE_DSSw(VMAX);

		if(gbWdrOn == WDR_LINE_2P) {
			AE_RHS1w(LWDR_2P_RHS1);
		}
		else if (gbWdrOn == WDR_LINE_3P) {
			AE_RHS1w(LWDR_3P_RHS1);
			AE_RHS2w(LWDR_3P_RHS2);
		}
	}
}

WORD ISRT AeSHT1(UINT anVal, UINT anManual, UINT anManualVal)
{
#if 1	// 1 이면 DSS x2 시 30/3=10fps, 0이면 30/4=7.5fps
	AeDSS(anVal);
#endif
	AE_SHTw(NOR_SHT_to_SHR0(anVal));

	if(gbAeDebugOn) { DispDec(0, 37, anVal, 4); /*DispDec(0, 42, tgt_M, 4);*/ }

#if 0	// Sensor Shutter 제어
	UartTxGrpSet(6, anVal);
	UartTxGrpSet(7, VMAX);
	extern int giUartTxGrpOn; giUartTxGrpOn = 0;
#endif
	return ((WORD)anVal);
}

WORD ISRT AeSHT0(UINT anVal, UINT anManual, UINT anManualVal)
{
	AeDSS(anVal);
	AE_SHTw(NOR_SHT_to_SHR0(anVal));

	DebugDisp2(gbAeDebugOn, Dec, "SHT    ", 0, 24, anVal, 4)
	if(gbAeDebugOn) { DispDec(0, 24+5, giCurDss, 2); }

#if 0	// Sensor Shutter 제어
	/*GRP0 = anVal;
	GRP1 = VMAX;
	GRP2 = (VMAX-(anVal+1));
	GRP3 = giCurDss;
	GRP5 = AE2_SUM2_LOCKr;
	UartTxGrp();*/
	UartTxGrpSet(0, anVal);
	UartTxGrpSet(1, VMAX);
	UartTxGrpSet(5, AE2_SUM2_LOCKr);
#endif
	return ((WORD)anVal);
}

void ISRT AeSHT_LWDR(int tgt_L, int tgt_S, int tgt_M)
{	// Shutter driver for Line WDR
	AeDSS(tgt_L);

	DebugDisp2(gbAeDebugOn, Dec, "SHT    ", 0, 24, tgt_L, 4)
	if(gbAeDebugOn) { DispDec(0, 37, tgt_S, 4); DispDec(0, 42, tgt_M, 4); }

	GRP0 = tgt_L;
	GRP2 = tgt_S;
	GRP4 = tgt_M;

	if(gbWdrOn == WDR_LINE_2P) {			// 2 page
		tgt_L = LWDR_2P_L_to_LEF(tgt_L);
		tgt_S = LWDR_2P_S_to_SEF1(tgt_S);

		AE_SHTw(tgt_L);
		AE_SHR1w(tgt_S);

		GRP1 = LWDR_2P_FSC  - (tgt_L + 1);
		GRP3 = LWDR_2P_RHS1 - (tgt_S + 1);
		GRP5 = tgt_M;
	}
	else if (gbWdrOn == WDR_LINE_3P) {		// 3 page

		tgt_L = LWDR_3P_L_to_LEF(tgt_L);
		tgt_S = LWDR_3P_S_to_SEF2(tgt_S);
		tgt_M = LWDR_3P_M_to_SEF1(tgt_M);

		AE_SHTw(tgt_L);
		AE_SHR1w(tgt_M);
		AE_SHR2w(tgt_S);

		GRP1 = LWDR_3P_FSC  - (tgt_L + 1);
		GRP3 = LWDR_3P_RHS2 - (tgt_S + 1);
		GRP5 = LWDR_3P_RHS1 - (tgt_M + 1);
	}

	GRP7 = gbWdrOn;
	UartTxGrp();
}

#undef VMAX
#define VMAX	nVMAX

int FrameSetCount(const BYTE abDssRatioLmt)
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	(gbWdrOn == WDR_LINE_2P) ? LWDR_2P_FSC :
			(gbWdrOn == WDR_LINE_3P) ? LWDR_3P_FSC :
									   VMAX;
}

int WdrSShtMax(const BYTE abDssRatioLmt)	// 0:FWDR, 1:LWDR 2P, 2:LWDR 3P
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	(gbWdrOn == WDR_LINE_2P) ? LWDR_2P_S_MAX :
			(gbWdrOn == WDR_LINE_3P) ? LWDR_3P_S_MAX :
									   NOR_SHT_MAX;
}

int WdrLShtMax(const BYTE abDssRatioLmt)		// 0:FWDR, 1:LWDR 2P, 2:LWDR 3P
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	(gbWdrOn == WDR_LINE_2P) ? LWDR_2P_L_MAX :
			(gbWdrOn == WDR_LINE_3P) ? LWDR_3P_L_MAX :
									   NOR_SHT_MAX;
}

int WdrMShtMax(const BYTE abDssRatioLmt)		// 0:FWDR, 1:LWDR 2P, 2:LWDR 3P
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	(gbWdrOn == WDR_LINE_3P) ? LWDR_3P_M_MAX :
									   NOR_SHT_MAX;
}

int WdrSShtMin(const BYTE abDssRatioLmt)	// 0:FWDR, 1:LWDR 2P, 2:LWDR 3P
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	(gbWdrOn == WDR_LINE_2P) ? LWDR_2P_S_MIN :
			(gbWdrOn == WDR_LINE_3P) ? LWDR_3P_S_MIN :
									   NOR_SHT_MIN;
}

int WdrLShtMin(const BYTE abDssRatioLmt)		// 0:FWDR, 1:LWDR 2P, 2:LWDR 3P
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	(gbWdrOn == WDR_LINE_2P) ? LWDR_2P_L_MIN :
			(gbWdrOn == WDR_LINE_3P) ? LWDR_3P_L_MIN :
									   NOR_SHT_MIN;
}

int WdrMShtMin(const BYTE abDssRatioLmt)		// 0:FWDR, 1:LWDR 2P, 2:LWDR 3P
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	(gbWdrOn == WDR_LINE_3P) ? LWDR_3P_M_MIN :
									   NOR_SHT_MIN;
}

#if 0
int ISRT AeETC(int aiVal)
{
	static int  iChk = 0;

	if(FPS_HI) AE_ETCw(0x1|(iChk<<4)); // 60P
	else       AE_ETCw(0x2|(iChk<<4)); // 30P

	aiVal = CLAMP(aiVal, 0, (AE_GAIN_UNIT_MAX*AE_GAIN_UNIT_EA)+AE_GAIN_UNIT_OVR);

	if (aiVal > AE_GAIN_UNIT_OVR) {
		aiVal -= AE_GAIN_UNIT_OVR;
		iChk = 1;
	}
	else iChk = 0;

	if(gbAeDebugOn) { FontCharEx(1, 24+6, NO_ALPHA, MN_WHITE, '0'+iChk); }

	return aiVal;
}
#endif

void ISRT AeAGC0(int aiVal)
{	// AGC driver
	//aiVal = AeETC(aiVal);
	AE_AGCw(aiVal);

	//DebugDisp2(gbAeDebugOn, Dec, "AGC    ", 1, 24, aiVal, 4)
	if(gbAeDebugOn) {
		FontStrEx(1, 24, NO_ALPHA, MN_GREEN, "AGC", 3);
		DispDec(1, 32, aiVal, 4);
	}

	//UartTxGrpSet(4, giAgc);	// Sensor Shutter 제어
}

void ISRT AeAGC_LWDR(int aiVal, int aiVhssL, int aiVhssS, int aiVhssM)
{
	//aiVal = AeETC(aiVal);
	AE_AGCw(aiVal+aiVhssL);

	if(gbWdrOn == WDR_LINE_2P) {			// 2 page
		AE_GAIN1w(aiVal+aiVhssS);
	}
	else if (gbWdrOn == WDR_LINE_3P) {		// 3 page
		AE_GAIN1w(aiVal+aiVhssM);
		AE_GAIN2w(aiVal+aiVhssS);
	}

	if(gbAeDebugOn) {
		FontStrEx(1, 24, NO_ALPHA, MN_GREEN, "AGC", 3);
		DispDec(1, 32, aiVal+aiVhssL, 4);
		DispDec(1, 37, aiVal+aiVhssS, 4);
		DispDec(1, 42, aiVal+aiVhssM, 4);
	}
}

int ISRT AGC2POS(int aiAgc)
{
	return (int)((float)aiAgc / 0.34f) + 1;
}

int ISRT POS2AGC(int aiPos)
{
	return aiPos/3;
}


// -----------------------------------------------------------------------------------------
// AWB
// -----------------------------------------------------------------------------------------

	//Designer 	: Park, Chan Bin
	//Date		: 18.04.05
	const s64 glMtxCf[9][6] =		{	// RT, Y 370
                                    {        -12259188624606LL,        -360495353736LL,        -501190275327LL,           -584386496LL,           -314087105LL,           -120413307LL},
                                    {        165350672655222LL,         495066206146LL,         496338604705LL,            844096349LL,            438722107LL,            432161539LL},
                                    {       -117939205598235LL,        -421596448868LL,        -911103419264LL,          -1815733573LL,           -225348386LL,          -1160011682LL},
                                    {       -319365995668088LL,        -866954845347LL,        -701030721869LL,          -1316412307LL,           -837051309LL,           -630853876LL},
                                    {        700162824100484LL,        2011114390501LL,        1754121159083LL,           3371559595LL,           1948762287LL,           1684020327LL},
                                    {       -730241424529638LL,       -2919553714795LL,       -2959725968345LL,          -6613210634LL,          -3143611040LL,          -3407050266LL},
                                    {        -38186043478705LL,          16497489423LL,          29217014506LL,            307693249LL,            119851162LL,            161608706LL},
                                    {        -65823028453394LL,        -615590729403LL,        -795796910693LL,          -1898358198LL,           -899249406LL,          -1238906695LL},
                                    {       1432139160530024LL,        5848279583688LL,        6795254715416LL,          13639668961LL,           7023080476LL,           9157972274LL}
                                    };

