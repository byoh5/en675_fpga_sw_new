/* **************************************************************************
 File Name	:	EN675_SENS_IMX291.c
 Description:	EN675 - IMX291 device driver
 Designer	:	Kim, Sunghoon
 Date		:	19. 6. 11
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#define __SENS_LIST__
#include "dev_model.h"				// EN675 - __SENS_LIST__
#define model_Sens	SENS_IMX291

#include "isp_lib_main.h"

//-------------------------------------------------------------------------------------------------

const WORD gwTblIMX291[][2] = {		// sLVDS, Sensor Master
								/*ID2*/
								{0x3005,	0x01},
								{0x3007,	0x00}, // WINMODE  - 190314 0x40 -> 0x00	190702 0x00

								//{0x3009,	0x01}, // FRSEL    - 190314 0x00 -> 0x01	60p
								{0x3009,	0x02}, // FRSEL    - 190314 0x00 -> 0x01	190702 0x02 for 30p

//								{0x300a,	0x04}, // Black Lv - 190314 0x04 -> 0xf0 & ISP Clamp Off-Set 0x00 -> 0x310 ※0x04로 하는 경우 포화영역 경계선 부분에 노이즈 발생
								{0x300a,	0xf0}, // Black Lv - 190314 0x04 -> 0xf0 & ISP Clamp Off-Set 0x00 -> 0x310 ※0x04로 하는 경우 포화영역 경계선 부분에 노이즈 발생

//								{0x300b,	0x00},
								{0x300f,	0x00},
								{0x3010,	0x21},
								{0x3012,	0x64},
								{0x3016,	0x09},
								{0x3018,	0x65},
								{0x3019,	0x04},
//								{0x301a,	0x00},

								//{0x301c,	0x98}, // HMAX - 190314 0x4C -> 0x98	60p
								//{0x301d,	0x08}, // HMAX - 190314 0x04 -> 0x08	60p
								{0x301c,	0x30}, // HMAX - 190314 0x4C -> 0x98	190702 0x30 for 30p
								{0x301d,	0x11}, // HMAX - 190314 0x04 -> 0x08	190702 0x11 for 30p

								{0x3046,	0xe1},
//								{0x3048,	0x00},
//								{0x3049,	0x0a},
								{0x304b,	0x0a}, // Sync Out - 190314 0x00 -> 0x0a
								{0x305c,	0x18},
								{0x305d,	0x00},
								{0x305e,	0x20},
								{0x305f,	0x01},
								{0x3070,	0x02},
								{0x3071,	0x11},
								{0x309b,	0x10},
								{0x309c,	0x22},
								{0x30a2,	0x02},
								{0x30a6,	0x20},
								{0x30a8,	0x20},
								{0x30aa,	0x20},
								{0x30ac,	0x20},
								{0x30b0,	0x43},

								/*ID3*/
								{0x3119,	0x9e},
								{0x311c,	0x1e},
								{0x311e,	0x08},
								{0x3128,	0x05},
								{0x3129,	0x00},
								{0x313d,	0x83},
								{0x3150,	0x03},
								{0x315e,	0x1a},
								{0x3164,	0x1a},
								{0x317c,	0x00},
                             	{0x317e,	0x00},
                                {0x31ec,	0x0e},

								/*ID4*/
								{0x32b8,	0x50},
								{0x32b9,	0x10},
								{0x32ba,	0x00},
								{0x32bb,	0x04},
								{0x32c8,	0x50},
								{0x32c9,	0x10},
								{0x32ca,	0x00},
								{0x32cb,	0x04},

								/*ID5*/
								{0x332c,	0xd3},
								{0x332d,	0x10},
								{0x332e,	0x0d},
								{0x3358,	0x06},
								{0x3359,	0xe1},
								{0x335a,	0x11},
								{0x3360,	0x1e},
								{0x3361,	0x61},
								{0x3362,	0x10},
								{0x33b0,	0x50},
								{0x33b2,	0x1A},
								{0x33b3,	0x04},

								/*ID6*/
								{0x3480,	0x49}
								};

void InitSensRun(void)
{
	SetSens(0x3000,0x1);		 WaitXus(1000);		// standby
	SetSens(0x3002,0x1);		 WaitXus(1000);		// xmaster stop

	for(UINT i=0;i<ARRAY_SIZE(gwTblIMX291); i++)
	{
		SetSens(gwTblIMX291[i][0], gwTblIMX291[i][1]);
		//WaitXus(200);
	}

	SetSens(0x309c, 0x22);					// Clamp sensitivity adjust	// 150317 LH

	SetSens(0x3000, 0x0);	WaitXus(3000);	// standby cancel
	SetSens(0x3002, 0x0);	WaitXus(2000);	// xmaster

	// SensClk
#if 0
	if(gbSensFps==60) {
		SetSens(0x3009, 0x01);	//FRSEL
		SetSens(0x301c, 0x98);	//HMAX
		SetSens(0x301d, 0x08);	//HMAX
	} else {
		SetSens(0x3009, 0x02);	//FRSEL
		SetSens(0x301c, 0x30);	//HMAX
		SetSens(0x301d, 0x11);	//HMAX
	}
#endif
}

BYTE gbBurstBuf[3]={0,0,0};
BYTE gbMirFlpBuf = 0x00;	// 센서마다 초기값이 다름 !!!

void SensFlip(BYTE aOn)
{
	gbMirFlpBuf = (aOn) ? (gbMirFlpBuf|0x01) : (gbMirFlpBuf&0xFE);
	SetSens(0x3007, gbMirFlpBuf);
}

void SensMirror(BYTE aOn)
{
	gbMirFlpBuf = (aOn) ? (gbMirFlpBuf|0x02) : (gbMirFlpBuf&0xFD);
	SetSens(0x3007, gbMirFlpBuf);
}

void ISRT0 AE_SHTw(UINT val)				// SHS1
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3020, gbBurstBuf, 3);
}

void ISRT0 AE_DSSw(UINT val)				// VMAX
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3018, gbBurstBuf, 3);
}

void ISRT0 AE_AGCw(UINT val)				// GAIN for LEF (NOR & DOL2 & DOL3)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	SetSens(0x3014, val&0xff);
}

void ISRT0 AE_FREQw(UINT aVtw, UINT aHtw)	// HMAX
{
	CHANGE_VAL(UINT,0xffffffff,aHtw)

	gbBurstBuf[1]=(aHtw>> 8)&0xff;
	gbBurstBuf[0]=(aHtw>> 0)&0xff;
	SetSensBurst(0x301c, gbBurstBuf, 2);
}

void ISRT0 AE_ETCw(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	SetSens(0x3009, val);
}

void ISRT0 AE_SHS2w(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3024, gbBurstBuf, 3);
}

void ISRT0 AE_SHS3w(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3028, gbBurstBuf, 3);
}

void ISRT0 AE_RHS1w(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3030, gbBurstBuf, 3);
}

void ISRT0 AE_RHS2w(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[2]=(val>>16)&0x03;
	gbBurstBuf[1]=(val>> 8)&0xff;
	gbBurstBuf[0]=(val>> 0)&0xff;
	SetSensBurst(0x3034, gbBurstBuf, 3);
}

void ISRT0 AE_GAIN1w(UINT val)				// GAIN for SEF1 (DOL2 & DOL3)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	SetSens(0x30f2, val&0xff);
}

void ISRT0 AE_GAIN2w(UINT val)				// GAIN for SEF2 (DOL3)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	SetSens(0x30f6, val&0xff);
}

void ISRT0 SensUpdate(void)
{
	gbSensUpdate = 1;

	AE_ETCw(0);
	AE_FREQw(0,0);
	AE_DSSw(0);
	AE_SHTw(0);
	AE_AGCw(0);

	AE_SHS2w(0);
	AE_SHS3w(0);
	AE_RHS1w(0);
	AE_RHS2w(0);
	AE_GAIN1w(0);
	AE_GAIN2w(0);

	gbSensUpdate = 0;
}


#undef SetSensBurst		// 아래의 코드(AeSHT(), AeAGC() 등) 에서 SetSensBurst 나 SetSens 의 사용을 방지 -> CHANGE_VAL(...) 적용 불가 -> AeDev() 사용 안됨
#undef SetSens			//		"
//----------------------------------------------------------------------------------------

	#define AE_GAIN_UNIT_MAX		(240/*210*/)	    															// Sensor gain step by Unit		// 160608 LH : IMX291
	#define AE_GAIN_UNIT_EA			(1)																				// Sensor gain Unit EA
	#define AE_GAIN_UNIT_OVR		(20/*32*/)																	// Sensor gain extended step

	const int AGC_POS_MAX = (((AE_GAIN_UNIT_MAX*AE_GAIN_UNIT_EA)+AE_GAIN_UNIT_OVR)*(0.3/0.1));


const BYTE gbSensShtDly = 2;	// ShtCtrl()에서 8이하, 0 : 1Frame 후 AE ODM적용, 1 : 2Frame 후 AE ODM적용, 2 : 3Frame 후 AE ODM적용, 3 : 4Frame 후 AE ODM적용
const BYTE gbSensAgcDly = 2;	// AgcCtrl()에서 8이하, 0 : 1Frame 후 AE ODM적용, 1 : 2Frame 후 AE ODM적용, 2 : 3Frame 후 AE ODM적용, 3 : 4Frame 후 AE ODM적용

const BYTE	gbVHssAgcTbl[]   = {
								0 , // 0 : dummy
								20,	// 1 : 20 초과 시 AE_GAIN_UNIT_OVR 적용 -> LWDR 시 다른 채널에 영향을 줌
								11,	// 2
								8 ,	// 3
								6 ,	// 4,6 -> 5,0 (SHT,AGC)
								5 ,	// 5,5 -> 6,0 (SHT,AGC)
								4 ,
								3 ,
								3 ,
								3 ,
								2 ,
								2 ,
								2 ,
								2 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 ,
								1 };

const UINT gVHssAgcTblEa = ARRAY_SIZE(gbVHssAgcTbl);

int WdrLShtMax(const BYTE abDssRatioLmt);	// VMAX 재설정 때문에 필요

UINT gnVMAX = 1125;

#define VMAX					gnVMAX

#define NOR_SHS1_MAX			(VMAX - 2)							// 1125 - 2 = 1123
#define NOR_SHS1_MIN			1									// 1

#define NOR_SHT_MIN				(VMAX - (NOR_SHS1_MAX + 1))			// 1125 - (1123 + 1) = 1
#define NOR_SHT_MAX				(VMAX - (NOR_SHS1_MIN + 1))			// 1125 - (1    + 1) = 1123

#define NOR_SHT_to_SHS1(SHT)	(VMAX - ((SHT) + 1))

#define LWDR_BRL				(VMAX-16)							// = 1109
#define LWDR_2P_FSC				(VMAX<<1)							// = 2250

#define LWDR_2P_RHS1			(LWDR_2P_FSC - (LWDR_BRL<<1) - 21 + 6)	// 2250 - 1109*2 - 21 + 6 = 17, 6은 guarantee 되지 않은 추가 값

#define LWDR_2P_SEF1_MAX		(LWDR_2P_RHS1 - 2)					// SEF1 Max at DOL 2P,   17 - 2 = 15
#define LWDR_2P_SEF1_MIN		(               2)					// SEF1 Min at DOL 2P,      + 2 = 2
#define LWDR_2P_LEF_MAX			(LWDR_2P_FSC  - 2)					// LEF  Max at DOL 2P, 2250 - 2 = 2248
#define LWDR_2P_LEF_MIN			(LWDR_2P_RHS1 + 2)					// LEF  Min at DOL 2P,   17 + 2 = 19

#define LWDR_2P_S_MIN			(LWDR_2P_RHS1-(LWDR_2P_SEF1_MAX+1))	// S Min shutter line at DOL 2P, 17-(15+1) = 1
#define LWDR_2P_S_MAX			(LWDR_2P_RHS1-(LWDR_2P_SEF1_MIN+1))	// S Max shutter line at DOL 2P, 17-( 2+1) = 14
#define LWDR_2P_L_MIN			(LWDR_2P_FSC -(LWDR_2P_LEF_MAX+1))	// L Min shutter line at DOL 2P, 2250-(2248+1) = 1
#define LWDR_2P_L_MAX			(LWDR_2P_FSC -(LWDR_2P_LEF_MIN+1))	// L Max shutter line at DOL 2P, 2250-(  19+1) = 2230

#define LWDR_2P_S_to_SEF1(S)	(LWDR_2P_RHS1-((S)+1))				// S to SEF1 for Sensor Reg. Setting at DOL 2P
#define LWDR_2P_L_to_LEF(L)		(LWDR_2P_FSC -((L)+1))				// L to LEF  for Sensor Reg. Setting at DOL 2P

#define LWDR_3P_FSC				(VMAX<<2)							// = 4500
#define LWDR_3P_RHS2			(LWDR_3P_FSC - LWDR_BRL*3 - 25)		// 4500 - 1109*3 - 25 = 1148, 3n+14 조건 맞음
#define LWDR_3P_RHS1			((3*333)*VMAX/gnAeVtw + 7)			// = 1006, 3n+7 조건 맞음

#define LWDR_3P_SEF2_MAX		(LWDR_3P_RHS2 - 2)					// SEF2 Max at DOL 3P, 1148 - 2 = 1146
#define LWDR_3P_SEF2_MIN		(LWDR_3P_RHS1 + 3)					// SEF2 Min at DOL 3P, 1006 + 3 = 1009
#define LWDR_3P_SEF1_MAX		(LWDR_3P_RHS1 - 2)					// SEF1 Max at DOL 3P, 1006 - 2 = 1004
#define LWDR_3P_SEF1_MIN		(               3)					// SEF1 Min at DOL 3P,      + 3 = 3
#define LWDR_3P_LEF_MAX			(LWDR_3P_FSC  - 2)					// LEF  Max at DOL 3P, 4500 - 2 = 4498
#define LWDR_3P_LEF_MIN			(LWDR_3P_RHS2 + 3)					// LEF  Min at DOL 3P, 1148 + 3 = 1151

#define LWDR_3P_S_MIN			(LWDR_3P_RHS2-(LWDR_3P_SEF2_MAX+1))	// S Min shutter line at DOL 3P, 1148-(1146+1) = 1
#define LWDR_3P_S_MAX			(LWDR_3P_RHS2-(LWDR_3P_SEF2_MIN+1))	// S Max shutter line at DOL 3P, 1148-(1009+1) = 138
#define LWDR_3P_M_MIN			(LWDR_3P_RHS1-(LWDR_3P_SEF1_MAX+1))	// M Min shutter line at DOL 3P, 1006-(1004+1) = 1
#define LWDR_3P_M_MAX			(LWDR_3P_RHS1-(LWDR_3P_SEF1_MIN+1))	// M Max shutter line at DOL 3P, 1006-(   3+1) = 1002
#define LWDR_3P_L_MIN			(LWDR_3P_FSC -(LWDR_3P_LEF_MAX+1))	// L Min shutter line at DOL 3P, 4500-(4498+1) = 1
#define LWDR_3P_L_MAX			(LWDR_3P_FSC -(LWDR_3P_LEF_MIN+1))	// L Max shutter line at DOL 3P, 4500-(1151+1) = 3348

#define LWDR_3P_S_to_SEF2(S)	(LWDR_3P_RHS2-((S)+1))				// S to SEF2 for Sensor Reg. Setting at DOL 3P
#define LWDR_3P_M_to_SEF1(M)	(LWDR_3P_RHS1-((M)+1))				// M to SEF1 for Sensor Reg. Setting at DOL 3P
#define LWDR_3P_L_to_LEF(L)		(LWDR_3P_FSC -((L)+1))				// L to LEF  for Sensor Reg. Setting at DOL 3P

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
	AE_SHTw(NOR_SHT_to_SHS1(anVal));

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
	AE_SHTw(NOR_SHT_to_SHS1(anVal));

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

		AE_SHTw(tgt_S);
		AE_SHS2w(tgt_L);

		GRP1 = LWDR_2P_FSC  - (tgt_L + 1);
		GRP3 = LWDR_2P_RHS1 - (tgt_S + 1);
		GRP5 = tgt_M;
	}
	else if (gbWdrOn == WDR_LINE_3P) {		// 3 page

		tgt_L = LWDR_3P_L_to_LEF(tgt_L);
		tgt_S = LWDR_3P_S_to_SEF2(tgt_S);
		tgt_M = LWDR_3P_M_to_SEF1(tgt_M);

		AE_SHTw(tgt_M);
		AE_SHS2w(tgt_S);
		AE_SHS3w(tgt_L);

		GRP1 = LWDR_3P_FSC  - (tgt_L + 1);
		GRP3 = LWDR_3P_RHS2 - (tgt_S + 1);
		GRP5 = LWDR_3P_RHS1 - (tgt_M + 1);
	}

	GRP7 = gbWdrOn;
	UartTxGrp();
}

#if 0
void Test_AeSHT_LWDR(void)
{
	extern int giSht_L;
	extern int giSht_S;
	//extern int giSht_M;

	int iSht_L, iSht_S, iSht_M;

	const BYTE bWdrOnTest = WDR_LINE_3P/*gbWdrOn*/;

	if (bWdrOnTest == WDR_LINE_2P) {
		iSht_L = (giSht_L*LWDR_2P_FSC) / VMAX;
		iSht_L = MIN(iSht_L, LWDR_2P_L_MAX);

		iSht_S = (giSht_S*LWDR_2P_FSC) / VMAX;
		iSht_S = MIN(iSht_S, LWDR_2P_S_MAX);

		iSht_M = 0;
	}
	else {
		iSht_L = (giSht_L*LWDR_3P_FSC) / VMAX;
		iSht_L = MIN(iSht_L, LWDR_3P_L_MAX);

		iSht_S = (giSht_S*LWDR_3P_FSC) / VMAX;
		iSht_S = MIN(iSht_S, LWDR_3P_S_MAX);

		iSht_M = (iSht_L - iSht_S)/3 + iSht_S;
		iSht_M = MIN(iSht_M, LWDR_3P_M_MAX);
	}

	const BYTE bWdrOn = gbWdrOn;
	gbWdrOn = bWdrOnTest;
	AeSHT_LWDR(iSht_L, iSht_S, iSht_M);
	gbWdrOn = bWdrOn;
}
#endif

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

void ISRT AeAGC0(int aiVal)
{	// AGC driver
	aiVal = AeETC(aiVal);
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
	aiVal = AeETC(aiVal);
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

// HTY 20150119
//		const s64 glMtxCf[9][6] =		{	// RT, Y 350
//										{	     -15775956550642LL,	       -158246467330LL,	       -280699043464LL,	           -79647158LL,	            20654898LL,	           241815413LL},
//										{	     -18286397476155LL,	        -31993028271LL,	        -28312828189LL,	          -118990038LL,	           -62473310LL,	          -111081511LL},
//										{	      19698210332374LL,	          8399529077LL,	         39481472411LL,	           100336703LL,	            90274541LL,	            91585700LL},
//										{	     -21110340241324LL,	          7599893414LL,	        -35514860345LL,	            61137927LL,	            72129955LL,	            65741426LL},
//										{	      56175730833417LL,	         67665048921LL,	         87540521876LL,	           141817493LL,	            38489173LL,	           113850759LL},
//										{	      -6176727406929LL,	       -123290324218LL,	        -81738923762LL,	          -279159505LL,	           -41370416LL,	          -125152757LL},
//										{	      47279267550011LL,	        199166227496LL,	        230803110466LL,	           605139021LL,	           280747804LL,	           426324684LL},
//										{	    -114508654817055LL,	       -353178934980LL,	       -505826363059LL,	          -987462638LL,	          -436421985LL,	          -711378965LL},
//										{	     401345314813783LL,	       1028337428513LL,	       1722833337092LL,	          2954136869LL,	          1419558362LL,	          2290512638LL}
//										};
// HTY 201060912
	const s64 glMtxCf[9][6] =		{	// RT, Y 350
									{	      39950736911249LL,	       -216126363082LL,	       -330684110705LL,	          -575405698LL,	          -260232927LL,	          -251496258LL},
									{	     -13830187163178LL,	         21231910614LL,	         37325701651LL,	            42442091LL,	             9135167LL,	            13782432LL},
									{	      42702575503157LL,	        146558369098LL,	        120679471381LL,	           377523174LL,	           311075430LL,	           201935641LL},
									{	     -26786098001963LL,	         -8006291482LL,	        -25736027991LL,	            87309140LL,	            51701338LL,	            72089627LL},
									{	      81456997952612LL,	         35781880379LL,	         24816425665LL,	          -165446922LL,	           -85337069LL,	          -138603207LL},
									{	      36444139512488LL,	        171339995318LL,	        181086642132LL,	           371973593LL,	           361028330LL,	           294931053LL},
									{	      29121303766713LL,	        104357143175LL,	        109938059158LL,	           381167268LL,	           187919639LL,	           229554171LL},
									{	     -93628939388145LL,	       -359549711943LL,	       -443453299222LL,	         -1172991878LL,	          -575562466LL,	          -757659571LL},
									{	     389681162987275LL,	       1497506884270LL,	       1803005166094LL,	          4290521070LL,	          2498802266LL,	          2936061999LL}
									};
