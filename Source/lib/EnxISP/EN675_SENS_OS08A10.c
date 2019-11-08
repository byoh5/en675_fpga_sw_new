/* **************************************************************************
 File Name	:	EN675_SENS_OS08A10.c
 Description:	EN675 - OS08A10 device driver
 Designer	:	Sung, Min-Je
 Date		:	19. 6. 11
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#define __SENS_LIST__
#include "dev_model.h"				// EN675 - __SENS_LIST__
#define model_Sens	SENS_OS08A10

#include "isp_lib_main.h"

//-------------------------------------------------------------------------------------------------

const WORD gTblLVDSOS08A10_15FPS[][2] = {
						{0x0100, 0x00},
						{0x0103, 0x01},

						// PLL
						{0x0303, 0x01},
						{0x0305, 0x5a},

						{0x0306, 0x01},
						{0x0308, 0x03},
						{0x0309, 0x04},
						{0x032a, 0x00},
						{0x300f, 0x00},
						{0x3010, 0x01},
						{0x3011, 0x04},
						{0x3012, 0x41},
						{0x3016, 0xf0},
						{0x301e, 0x98},
						{0x3031, 0xa9},
						{0x3103, 0x92},
						{0x3104, 0x01},
						{0x3106, 0x10},
						{0x340c, 0xff},
						{0x340d, 0xff},
						{0x031e, 0x09},

//						{0x3501, 0x08},				//	Long Exposure High
//						{0x3502, 0xe5},				//	Long Exposure Low
						{0x3501, 0x04},				//	Long Exposure High
						{0x3502, 0x65},				//	Long Exposure Low

						{0x3505, 0x83},
						{0x3508, 0x00},				//	Gain [13:8]
						{0x3509, 0x80},				//
						{0x350a, 0x04},				//	Long Digital Gain [13:8]
						{0x350b, 0x00},				//	Long Digital Gain [ 7:0]
						{0x350c, 0x00},
						{0x350d, 0x80},
						{0x350e, 0x04},
						{0x350f, 0x00},
						{0x3600, 0x00},
						{0x3603, 0x2c},
						{0x3605, 0x50},
						{0x3609, 0xb5},
						{0x3610, 0x69},
						{0x360c, 0x01},
						{0x3628, 0xa4},
						{0x362d, 0x10},
						{0x3660, 0x41},
						{0x3661, 0x06},
						{0x3662, 0x00},
						{0x3663, 0x28},
						{0x3664, 0x0d},
						{0x366a, 0x38},
						{0x366b, 0xa0},
						{0x366d, 0x00},
						{0x366e, 0x00},
						{0x3680, 0x00},
						{0x36c0, 0x00},
						{0x3701, 0x02},
						{0x373b, 0x02},
						{0x373c, 0x02},
						{0x3736, 0x02},
						{0x3737, 0x02},
						{0x3705, 0x00},
						{0x3706, 0x39},
						{0x370a, 0x00},
						{0x370b, 0x98},
						{0x3709, 0x49},
						{0x3714, 0x21},
						{0x371c, 0x00},
						{0x371d, 0x08},
						{0x3740, 0x1b},
						{0x3741, 0x04},
						{0x375e, 0x0b},
						{0x3760, 0x10},
						{0x3776, 0x10},
						{0x3781, 0x02},
						{0x3782, 0x04},
						{0x3783, 0x02},
						{0x3784, 0x08},
						{0x3785, 0x08},
						{0x3788, 0x01},
						{0x3789, 0x01},
						{0x3797, 0x04},
						{0x3800, 0x00},
						{0x3801, 0x00},
						{0x3802, 0x00},
						{0x3803, 0x0c},
						{0x3804, 0x0e},
						{0x3805, 0xff},
						{0x3806, 0x08},
						{0x3807, 0x6f},
						{0x3808, 0x0f},
						{0x3809, 0x20},
						{0x380a, 0x08},
						{0x380b, 0x90},

						{0x380c, 0x11},		// HTS - FREQ_H
						{0x380d, 0x30},		// HTS - FREQ_L

						{0x380e, 0x08},		// VTS - DSS_H
						{0x380f, 0xca},		// VTS - DSS_L

						{0x3813, 0x00},
						{0x3814, 0x01},
						{0x3815, 0x01},
						{0x3816, 0x01},
						{0x3817, 0x01},
						{0x381c, 0x00},
						{0x3820, 0x04},		// FLIP ON
						{0x3821, 0x04},		// MIRROR ON
						{0x3823, 0x08},
						{0x3826, 0x00},
						{0x3827, 0x08},
						{0x382d, 0x08},
						{0x3832, 0x02},
						{0x3833, 0x00},
						{0x383c, 0x00},
						{0x383d, 0xff},
						{0x3d85, 0x0b},
						{0x3d84, 0x40},
						{0x3d8c, 0x63},
						{0x3d8d, 0xd7},
						{0x4000, 0xf8},
						{0x4001, 0x2b},

						{0x4004, 0x00},		// Black Level Target [11:8]
//						{0x4005, 0x40},		// Black Level Target [7:0]
//						{0x4005, 0x10},		// Black Level Target [7:0]
						{0x4005, 0x00},		// Black Level Target [7:0]

						{0x400a, 0x01},
						{0x400f, 0xa0},
						{0x4010, 0x12},
						{0x4018, 0x00},
						{0x4008, 0x02},
						{0x4009, 0x0d},
						{0x401a, 0x58},
						{0x4050, 0x00},
						{0x4051, 0x01},
						{0x4028, 0x2f},
						{0x4052, 0x00},
						{0x4053, 0x80},
						{0x4054, 0x00},
						{0x4055, 0x80},
						{0x4056, 0x00},
						{0x4057, 0x80},
						{0x4058, 0x00},
						{0x4059, 0x80},
						{0x430b, 0x0f},
						{0x430c, 0xfe},
						{0x430d, 0x00},
						{0x430e, 0x01},
						{0x4501, 0x18},
						{0x4502, 0x00},
						{0x4643, 0x00},
						{0x4640, 0x01},
						{0x4641, 0x04},
						{0x4800, 0x04},
						{0x4809, 0x2b},
						{0x4813, 0x90},
						{0x4817, 0x00},
						{0x4833, 0x18},
						{0x4837, 0x0b},
						{0x483b, 0x00},
						{0x484b, 0x03},
						{0x4850, 0x7c},
						{0x4852, 0x06},
						{0x4856, 0x58},
						{0x4857, 0xaa},
						{0x4862, 0x0a},
						{0x4869, 0x18},
						{0x486a, 0xaa},
						{0x486e, 0x03},
						{0x486f, 0x55},
						{0x4875, 0xf0},
						{0x5000, 0x89},
						{0x5001, 0x42},
						{0x5004, 0x40},
						{0x5005, 0x00},
						{0x5180, 0x00},
						{0x5181, 0x10},
						{0x580b, 0x03},
						{0x4d00, 0x03},
						{0x4d01, 0xc9},
						{0x4d02, 0xbc},
						{0x4d03, 0xc6},
						{0x4d04, 0x4a},
						{0x4d05, 0x25},
						{0x4700, 0x2b},
						{0x4e00, 0x2b},

						{0x0303, 0x00},
						{0x0305, 0x21},
						{0x4837, 0x16},
						{0x0323, 0x00},
						{0x0325, 0x21},

						{0x0328, 0x05},
						{0x0329, 0x01},
						{0x032a, 0x00},
						{0x3106, 0x10},
						{0x3808, 0x0f},
						{0x3809, 0x10},
						{0x380a, 0x08},
						{0x380b, 0x80},
						{0x3812, 0x00},
						{0x3813, 0x08},

//						{0x380c, 0x11},
//						{0x380d, 0x30},
//						{0x380e, 0x08},
//						{0x380f, 0xca},

//						{0x3501, 0x08},		// Long Exposure High
//						{0x3502, 0x01},		// Long Exposure Low
//						{0x3501, 0x04},
//						{0x3502, 0x85},

						{0x0100, 0x01},
						{0x0100, 0x01},
						{0x0100, 0x01},
						{0x0100, 0x01},
						{0x0100, 0x01}
						};

void InitSensRun(void)
{
	for(UINT i=0;i<ARRAY_SIZE(gTblLVDSOS08A10_15FPS); i++)
	{
		SetSens(gTblLVDSOS08A10_15FPS[i][0],gTblLVDSOS08A10_15FPS[i][1]);
//		WaitXus(20);
	}
}

void SensFlip(BYTE aOn)
{
	SetSens(0x3820, (aOn>0)<<2);
}

void SensMirror(BYTE aOn)
{
	SetSens(0x3821, (aOn>0)<<2);
}

void ISRT0 AE_SHTw(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	SetSens(0x3501,(val>>8)&0xFF);
	SetSens(0x3502,(val&0xFF));
}

void ISRT0 AE_DSSw(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	SetSens(0x380E,(val>>8)&0xFF);
	SetSens(0x380F,(val&0xFF));
}

void ISRT0 AE_AGCw(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	SetSens(0x3508,(val>>8)&0xFF);
	SetSens(0x3509,(val&0xFF));
}

void ISRT0 AE_FREQw(UINT aVtw, UINT aHtw)
{
	CHANGE_VAL(UINT,0xffffffff,aHtw)

	SetSens(0x380C,(aHtw>>8)&0xFF);
	SetSens(0x380D,(aHtw&0xFF));
}

void ISRT0 AE_ETCw(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	SetSens(0x350A,(val>>8)&0xFF);
	SetSens(0x350B,(val&0xFF));
}


#undef SetSensBurst		// 아래의 코드(AeSHT(), AeAGC() 등) 에서 SetSensBurst 나 SetSens 의 사용을 방지 -> CHANGE_VAL(...) 적용 불가 -> AeDev() 사용 안됨
#undef SetSens			//		"
//----------------------------------------------------------------------------------------

#define AE_GAIN_UNIT_MAX		(60)	    											// Sensor gain step by Unit
#define AE_GAIN_UNIT_EA			(4)														// Sensor gain Unit EA
#define AE_GAIN_UNIT_OVR		(241-3)													// Sensor gain extended step	GetDgcVal()에서 AGC POS가 241일때 DGC VAL이 0x3FFF 이며 AeAGC0()에서 GetDgcVal(aiPos-237)이므로 241+237=478이 됨		TODO KSH> OS08A10 : AE_GAIN_UNIT_OVR 에 -3 추가
#define AE_GAIN_UNIT_OVR2		(0)	    												// ISP gain extended step

const int AE_GAIN_TOTAL = (((AE_GAIN_UNIT_MAX*AE_GAIN_UNIT_EA)+AE_GAIN_UNIT_OVR)*1);	//Modified gain per step		TODO KSH> OS08A10 : giCurAgc의 최대값이 AE_GAIN_TOTAL=478 이므로 800 가까이 되지 않음
const int AE_GAIN_TOTAL2 = 0;


const BYTE gbSensShtDly = 2;	// ShtCtrl()에서 8이하, 0 : 1Frame 후 AE ODM적용, 1 : 2Frame 후 AE ODM적용, 2 : 3Frame 후 AE ODM적용, 3 : 4Frame 후 AE ODM적용
const BYTE gbSensAgcDly = 2;	// AgcCtrl()에서 8이하, 0 : 1Frame 후 AE ODM적용, 1 : 2Frame 후 AE ODM적용, 2 : 3Frame 후 AE ODM적용, 3 : 4Frame 후 AE ODM적용

const BYTE	gbVHssAgcTbl[]   = {
								0 ,	// 0
								0 ,	// 1
								0 ,	// 2
								0 ,	// 3
								0 ,	// 4
								0 ,	// 5
								0 ,	// 6
								0 ,	// 7	EN781에서 OS08A10의 gnAeShtTbl[0][1] 값이 1024 (8 line) 임
								16,	// 8,16 -> 9,0 (SHT,AGC)
								13,	// 9,13 -> 10,0 (SHT,AGC)
								11,
								10,
								9 ,
								8 ,
								7 ,
								7 ,
								6 ,
								6 ,
								5 ,
								5 ,
								5 ,
								4 ,
								4 ,
								4 ,
								4 ,
								3 ,
								3 ,
								3 ,
								3 ,
								3 ,
								3 ,
								3 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 ,
								2 };

const UINT gVHssAgcTblEa = ARRAY_SIZE(gbVHssAgcTbl);

WORD ISRT AeSHT0(UINT anVal, UINT anManual, UINT anManualVal)
{
	UINT nMok, nRem, nShtMax;

	#define AE_SHTMAX	(nMok-8)

	if (anManual) {
		nMok 	= gnAeVtw;
		nShtMax = AE_SHTMAX;
		nRem = CLAMP(anManualVal, 4, nShtMax);
	}
	else if (anVal>gnAeVtw/*gnAeState==3*/) {
		nMok 	= anVal/gnAeVtw;
		nMok 	= ((nMok*gnAeVtw)+gnAeVtw);
		nShtMax = AE_SHTMAX;
		nRem 	= MIN(anVal, nShtMax);
	}
	else {
		nMok 	= gnAeVtw	 ;
		nShtMax = AE_SHTMAX;
		//nRem 	= MIN(anVal, nShtMax);
		nRem = CLAMP(anVal, 8, nShtMax);	// TODO KSH> OS08A10 : Shutter Auto의 최소line이 8미만 테스트 필요
	}

	AE_DSSw(nMok);
	AE_SHTw(nRem);

	giCurDss = MAX((int)(nMok/gnAeVtw)-1, 0);

	DebugDisp2(gbAeDebugOn, Dec, "SHT    ", 0, 24, nRem, 4)
	if(gbAeDebugOn) { DispDec(0, 24+5, giCurDss, 2); }

	return ((WORD)nRem);
}

const BYTE bAGain_Step_OS08A10[16] = {0,	// AGC STEP -> AGC POS
									  5,
									  10,
									  15,
									  19,
									  24,
									  28,
									  32,
									  35,
									  39,
									  42,
									  45,
									  49,
									  52,
									  55,
									  57
									};

#define AGC_STEP_TBL_EA		ARRAY_SIZE(bAGain_Step_OS08A10)

int ISRT GetAgcStep(int aiAgcPos)	// AGC POS -> AGC STEP
{
	int i;

	const int aiAgcPosDivUnit = aiAgcPos%AE_GAIN_UNIT_MAX;

	if(aiAgcPosDivUnit > bAGain_Step_OS08A10[AGC_STEP_TBL_EA-1]) return AGC_STEP_TBL_EA;

	for(i=1; i<AGC_STEP_TBL_EA; i++) {
	   if(aiAgcPosDivUnit <= bAGain_Step_OS08A10[i])  return i;
	}

	return AGC_STEP_TBL_EA;
}

const UINT gnAeGainTbl[][2] =   {			// AGC POS -> DGC VAL
								{0  , 1024},
								{14 , 1203},
								{28 , 1413},
								{42 , 1660},
								{56 , 1951},
								{70 , 2292},
								{84 , 2693},
								{98 , 3164},
								{112, 3717},
								{126, 4368},
								{140, 5132},
								{154, 6029},
								{168, 7084},
								{182, 8323},
								{196, 9779},
								{210, 11489},
								{224, 13498},
								{238, 15859},
								{243, 16905}
								};

#define DGC_VAL_TBL_EA		ARRAY_SIZE(gnAeGainTbl)

UINT ISRT GetDgcVal(int aiAgcPos)
{
	UINT 	i;
	UINT    nGain=0;

    if(aiAgcPos >= (int)gnAeGainTbl[DGC_VAL_TBL_EA-1][0]) return gnAeGainTbl[DGC_VAL_TBL_EA-1][1];		// TODO KSH> OS08A10 : 부호를 "==" 에서 ">="로 변경

    for(i=1; i<DGC_VAL_TBL_EA; i++) {
    	if (aiAgcPos < (int)gnAeGainTbl[i][0]) {
    		return LibUtlInterp1D(aiAgcPos, gnAeGainTbl[i-1][0], gnAeGainTbl[i][0], gnAeGainTbl[i-1][1], gnAeGainTbl[i][1]);
    	}
    }

    return gnAeGainTbl[0][1];
}

void ISRT AeAGC0(int aiPos)
{	// AGC driver

	static UINT	gnDgc=0, gnAgc=0;

	if(aiPos < 237) {
		const int iAgcStep = GetAgcStep(aiPos);
		const int iAgcStepPos = bAGain_Step_OS08A10[iAgcStep-1];

	    if(aiPos<60)		{ gnAgc = 120+(iAgcStep<<3);    gnDgc = GetDgcVal(aiPos-0  -iAgcStepPos); }
		else if(aiPos<120 )	{ gnAgc = 240+(iAgcStep<<4);	gnDgc = GetDgcVal(aiPos-60 -iAgcStepPos); }
		else if(aiPos<180 ) { gnAgc = 480+(iAgcStep<<5);	gnDgc = GetDgcVal(aiPos-120-iAgcStepPos); }
		else if(aiPos<237 ) { gnAgc = 960+(iAgcStep<<6);	gnDgc = GetDgcVal(aiPos-180-iAgcStepPos); }
		gnAgc = CLAMP(gnAgc, 0x80, 0x7C0);
	}
	else {
		gnDgc = GetDgcVal(aiPos-237);
		gnDgc = CLAMP(gnDgc, 0x400, 0x3FFF);
		gnAgc = 0x7C0;
	}

	AE_AGCw(gnAgc);
	AE_ETCw(gnDgc);

	if(gbAeDebugOn) {
		FontStrEx(1, 24, NO_ALPHA, MN_GREEN, "AGC", 3);
		FontHexEx(1, 28, NO_ALPHA, MN_WHITE, gnAgc, 3);
		FontHexEx(1, 32, NO_ALPHA, MN_WHITE, gnDgc, 4);
	}
}

int ISRT AGC2POS(int aiAgc)
{
	return aiAgc;
}

int ISRT POS2AGC(int aiPos)
{
	return aiPos;
}


// -----------------------------------------------------------------------------------------
// AWB
// -----------------------------------------------------------------------------------------

	const s64 glMtxCf[9][6] =	{
								{	      97953021357281LL,	        162028717740LL,	        -43887842879LL,	           406730122LL,	           311064465LL,	           307293487LL},
								{	      -1111645494022LL,	          5168283084LL,	         45576060778LL,	            -7078654LL,	           -39081060LL,	             3469475LL},
								{	      -1745971655516LL,	        -52347281406LL,	        -83427488481LL,	          -160619669LL,	            10185073LL,	           -97565133LL},
								{	     -55612408256212LL,	       -148319334517LL,	       -157175475516LL,	          -268298733LL,	          -139502731LL,	          -126330365LL},
								{	     170943944030429LL,	        469294534043LL,	        392694234902LL,	           863262298LL,	           502003396LL,	           413583254LL},
								{	      -7061097672787LL,	       -111593306979LL,	         87444654891LL,	          -104910819LL,	          -110023367LL,	           187266672LL},
								{	      53643169106058LL,	        262740275895LL,	        207723671190LL,	           684234979LL,	           409141282LL,	           349228520LL},
								{	    -197617420305327LL,	       -900072960784LL,	       -920131273752LL,	         -2333690378LL,	         -1297467190LL,	         -1366759973LL},
								{	     851945285224989LL,	       3689636736591LL,	       4353864097586LL,	         10008549236LL,	          5267193034LL,	          6525837721LL}
								};

