/* **************************************************************************
 File Name	:	EN675_SENS_IMX323.c
 Description:	EN675 - IMX323 device driver
 Designer	:	Lee, Wanhee
 Date		:	20. 1. 14
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#define __SENS_LIST__
#include "dev_model.h"				// EN675 - __SENS_LIST__
#define model_Sens	SENS_IMX323

#include "isp_lib_main.h"

//-------------------------------------------------------------------------------------------------

const WORD gwTblIMX323[][2] = { // for TWI
								{0x3000, 0x31},	// Valid register write / Standby
								{0x0100, 0x00},	// Standby control (0:Standby, 1:Normal)

								{0x3002, 0x0F},	// Readout mode designation (0x1:720P,0xF:1080P,Others:Invalid)
								{0x3011, 0x00},	// FRSEL / Output data rate designation (10,12bit 15P:0x01,30P:0x00)
								{0x3012, 0x82},	//1101Sony
								{0x3016, 0x3C},	// 1080P:0x3C,720P:0xF0

								{0x301f, 0x73},	// Fixed to 0x73
								{0x3021, 0x00},	// 10BIA (10bit:1, 12bit:0)
								{0x3027, 0x20},	// Fixed to 0x20

								{0x302c, 0x00},	// XMSTA (0:Sensor master, 1:Trig Standby)

								{0x303f, 0x0A},	// Fixed to 0x0A

								{0x307a, 0x00},	// 10BIA (10bit:0x40, 12bit:0)
								{0x307b, 0x00},	// 10BIA (10bit:0x02, 12bit:0)

								{0x309a, 0x26},	// 12bit1080P[11:4] Adjustment registers for each operation mode
								{0x309b, 0x02},	// 12bit1080P[ 3:0] Adjustment registers for each operation mode

								// TWI Only Register
								{0x0008, 0x00},	// Black Level[8]
								{0x0009, 0x3C},	// Black Level[7:0] It's recommended to be set 10bit:0x3c, 12bit:0xf0

								{0x0101, 0x00},	// H[0]/V[1] Scanning direction control (0:Normal, 1:Inverted)

								{0x0104, 0x00},	// Register reflection timing hold (0:Normal, 1:Register setting hold)

								{0x0112, 0x0C},	// TWI ADRES1 (0x0A:10bits, 0x0C:12bits)
								{0x0113, 0x0C},	// TWI ADRES2 (0x0A:10bits, 0x0C:12bits)

								{0x0202, 0x00},	// Integration time[15:8] adjustment direction line number designation
								{0x0203, 0x00},	// Integration time[ 7:0] adjustment direction line number designation

								{0x0340, 0x04},	// In master mode. Vertical(V) direction line number[15:8] designation
								{0x0341, 0x65},	// In master mode. Vertical(V) direction line number[ 7:0] designation

								//{0x0342, 0x08},	// In master mode. Horizontal(H) direction clock number[15:8] designation
								//{0x0343, 0x98},	// In master mode. Horizontal(H) direction clock number[ 7:0] designation

								{0x0342, 0x04},	// In master mode. Horizontal(H) direction clock number[15:8] designation
								{0x0343, 0x4C} //1101Sony,	// In master mode. Horizontal(H) direction clock number[ 7:0] designation
								//{0x0342, 0x05},	// In master mode. Horizontal(H) direction clock number[15:8] designation
								//{0x0343, 0xa0} //1101Sony,	// In master mode. Horizontal(H) direction clock number[ 7:0] designation

								//{0x3000, 0x30}	// Valid register write / Normal Operation
								};

void InitSensRun(void)
{
	for(UINT i=0;i<ARRAY_SIZE(gwTblIMX323); i++) SetSens(gwTblIMX323[i][0], gwTblIMX323[i][1]);
	WaitXus(1000);
	SetSens(0x3117, 0x0D);		// 추가 (IMX322 & IMX323)

	WaitXus(1000);
	WaitXus(1000);

	SetSens(0x0100, 1); // Standby control (0:Standby, 1:Normal) - i2c only
	WaitXus(1000);
}

BYTE gbBurstBuf[2]={0,0};
BYTE gbMirFlpBuf = 0x00;	// 센서마다 초기값이 다름 !!!

void SensFlip(BYTE aOn)
{
	gbMirFlpBuf = (aOn) ? (gbMirFlpBuf|0x02) : (gbMirFlpBuf&0xFD);
	SetSens(0x0101, gbMirFlpBuf);
}

void SensMirror(BYTE aOn)
{
	gbMirFlpBuf = (aOn) ? (gbMirFlpBuf|0x01) : (gbMirFlpBuf&0xFE);
	SetSens(0x0101, gbMirFlpBuf);
}

void ISRT0 AE_SHTw(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[0]=(val>> 8)&0xff;
	gbBurstBuf[1]=(val>> 0)&0xff;
	SetSensBurst(0x0202, gbBurstBuf, 2);
}

void ISRT0 AE_DSSw(UINT val)				// VMAX
{
	CHANGE_VAL(UINT,0xffffffff,val)

	gbBurstBuf[0]=(val>> 8)&0xff;
	gbBurstBuf[1]=(val>> 0)&0xff;
	SetSensBurst(0x0340, gbBurstBuf, 2);
}

void ISRT0 AE_AGCw(UINT val)
{
	CHANGE_VAL(UINT,0xffffffff,val)

	SetSens(0x301e, val&0xff);
}

void ISRT0 AE_FREQw(UINT aVtw, UINT aHtw)	// HMAX
{
	CHANGE_VAL(UINT,0xffffffff,aHtw)

	gbBurstBuf[0]=(aHtw>> 8)&0xff;
	gbBurstBuf[1]=(aHtw>> 0)&0xff;
	SetSensBurst(0x0342, gbBurstBuf, 2);
}

void ISRT0 SensUpdate(void)
{
	gbSensUpdate = 1;

	AE_FREQw(0,0);
	AE_DSSw(0);
	AE_SHTw(0);
	AE_AGCw(0);

	gbSensUpdate = 0;
}


#undef SetSensBurst		// 아래의 코드(AeSHT(), AeAGC() 등) 에서 SetSensBurst 나 SetSens 의 사용을 방지 -> CHANGE_VAL(...) 적용 불가 -> AeDev() 사용 안됨
#undef SetSens			//		"
//----------------------------------------------------------------------------------------

	#define AE_GAIN_UNIT_MAX		150/*140*/	// CAUTION !!! - 변경 시 AE_GAIN_NORMAL_MAX도 수정 필요				// Sensor gain step by Unit
	#define AE_GAIN_UNIT_EA			1																				// Sensor gain Unit EA

	#define AE_GAIN_NORMAL_MAX		480/*448*/					// = AE_GAIN_UNIT_MAX / 0.3125f

	const int AGC_POS_MAX = AE_GAIN_NORMAL_MAX;					// TODO KSH> IMX323 : giCurAgc의 최대값이 AGC_POS_MAX=480 이므로 800 가까이 되지 않음


const BYTE gbSensShtDly = 2;	// ShtCtrl()에서 8이하, 0 : 1Frame 후 AE ODM적용, 1 : 2Frame 후 AE ODM적용, 2 : 3Frame 후 AE ODM적용, 3 : 4Frame 후 AE ODM적용
const BYTE gbSensAgcDly = 2;	// AgcCtrl()에서 8이하, 0 : 1Frame 후 AE ODM적용, 1 : 2Frame 후 AE ODM적용, 2 : 3Frame 후 AE ODM적용, 3 : 4Frame 후 AE ODM적용

const BYTE	gbVHssAgcTbl[]   = {
								0 , // 0 : dummy
								25,	// 1
								13,	// 2
								9 ,	// 3
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
								1 };

const UINT gVHssAgcTblEa = ARRAY_SIZE(gbVHssAgcTbl);

int WdrLShtMax(const BYTE abDssRatioLmt);	// VMAX 재설정 때문에 필요

UINT gnVMAX = 1125;

#define VMAX					gnVMAX

#define NOR_SHS1_MAX			(VMAX - 1)						// 1125 - 1 = 1124
#define NOR_SHS1_MIN			0								// 0

#define NOR_SHT_MIN				(VMAX - NOR_SHS1_MAX /*- 0.3*/)	// 1125 - 1124 - 0.3 = 0.7    ≒ 1
#define NOR_SHT_MAX				(VMAX - NOR_SHS1_MIN /*- 0.3*/)	// 1125 - 0    - 0.3 = 1124.7 ≒ 1125

#define NOR_SHT_to_SHS1(SHT)	(VMAX - (SHT) /*- 0.3*/)		// 0.3은 생략


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

}

#undef VMAX
#define VMAX	nVMAX

int FrameSetCount(const BYTE abDssRatioLmt)
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	VMAX;
}

int WdrSShtMax(const BYTE abDssRatioLmt)	// 0:FWDR, 1:LWDR 2P, 2:LWDR 3P
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	NOR_SHT_MAX;
}

int WdrLShtMax(const BYTE abDssRatioLmt)		// 0:FWDR, 1:LWDR 2P, 2:LWDR 3P
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	NOR_SHT_MAX;
}

int WdrMShtMax(const BYTE abDssRatioLmt)		// 0:FWDR, 1:LWDR 2P, 2:LWDR 3P
{
	const UINT nVMAX = MAX(abDssRatioLmt, 1) * gnAeVtw;
	return	NOR_SHT_MAX;
}

void ISRT AeAGC0(int aiVal)
{	// AGC driver
	aiVal = CLAMP(aiVal, 0, (AE_GAIN_UNIT_EA*AE_GAIN_UNIT_MAX));
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

}

int ISRT AGC2POS(int aiAgc)
{
	return (int)((float)aiAgc * 3.2f) + 1;
}

int ISRT POS2AGC(int aiPos)
{
	return (int)((float)aiPos * 0.3125f);
}


// -----------------------------------------------------------------------------------------
// AWB
// -----------------------------------------------------------------------------------------

// 15.03.31 WHL
	const s64 glMtxCf[9][6] =		{	// RT, Y 350
									{	     -80211516700095LL,	       -635034814914LL,	       -789435616830LL,	         -1021433329LL,	          -487493235LL,	          -180101999LL},
									{	     -37671441578610LL,	       -195150890297LL,	       -155681982320LL,	          -696021281LL,	          -405873911LL,	          -439989538LL},
									{	       2958176489446LL,	       -255405965224LL,	        -40080406405LL,	          -130187230LL,	             1928619LL,	           281379472LL},
									{	     -38146294520683LL,	         88115763983LL,	         50387282750LL,	           443064402LL,	           269992747LL,	           253566202LL},
									{	     119730648081741LL,	         31399416306LL,	          3035351530LL,	          -217982043LL,	          -149127897LL,	          -127591442LL},
									{	    -229124356201391LL,	      -1305413933910LL,	       -917564820630LL,	         -2684026872LL,	         -1410735769LL,	          -867651332LL},
									{	      58506871759534LL,	        395398285557LL,	        337389747497LL,	          1245462803LL,	           672431539LL,	           685558474LL},
									{	    -141082927534720LL,	       -764049109633LL,	       -813747199389LL,	         -2172352636LL,	         -1171875379LL,	         -1214427246LL},
									{	     220945824781521LL,	        306126728452LL,	       1035655332088LL,	          1847717110LL,	          1004600089LL,	          1961532051LL}
									};

