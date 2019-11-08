/* **************************************************************************
 File Name	: 	isp_ae.c
 Description:	EN675 - Auto Exposure function
 Designer	:	Kim, Sunghoon
 Date		:	19. 7. 2
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"

#define AE1_CLIP0w(VAL)		AE1_CLIPw((VAL)>>2)	// AE 8BIT
#define AE2_CLIP0w(VAL)		AE2_CLIPw((VAL)>>2)
#define AE3_CLIP0w(VAL)		AE3_CLIPw((VAL)>>2)
#define AE4_CLIP0w(VAL)		AE4_CLIPw((VAL)>>2)
#define AE5_CLIP0w(VAL)		AE5_CLIPw((VAL)>>2)
#define AE6_CLIP0w(VAL)		AE6_CLIPw((VAL)>>2)
#define AE1_SLICE0w(VAL)	AE1_SLICEw((VAL)>>2)	// AE 8BIT
#define AE2_SLICE0w(VAL)	AE2_SLICEw((VAL)>>2)
#define AE3_SLICE0w(VAL)	AE3_SLICEw((VAL)>>2)
#define AE4_SLICE0w(VAL)	AE4_SLICEw((VAL)>>2)
#define AE5_SLICE0w(VAL)	AE5_SLICEw((VAL)>>2)
#define AE6_SLICE0w(VAL)	AE6_SLICEw((VAL)>>2)

#define WDR_OLD				0

#define AE_FUNC_FPS			FPS_VDI

//*******************************************************************************
// Reserved define & tables
//*******************************************************************************

	#define AE_GAIN_TGT_OFST		0x80

	#define	AE_WDR_SWGT_MAXCNT		(anPxCntShort>>6)

	#define AE_ERR_MGN				0x8
	#define AE_ERR_MIN				0x1


#if (model_Led==1)
  #ifdef __USE_IR_LED_GPIO__
	#define AE_LED_ON				{GpioSetOut(GPIO_IR_LED, GPIO_OUT_HI);}		// IR-LED ON/OFF
	#define AE_LED_OFF				{GpioSetOut(GPIO_IR_LED, GPIO_OUT_LOW);}
  #else
	#error if use GPIO for IR-LED Control, please set "USE_IR_LED_GPIO 1, IR_LED_GPIO_CH #" in "device.cmake"
  #endif
#elif (model_Led==0)
  #ifdef __USE_IR_LED_LPWM__
	#define AE_LED_ON 				IRIS_CLPHw(1375);	//IRIS_ONw(0x1);								// 170209 KSH
	#define AE_LED_OFF				IRIS_CLPHw(0);		//IRIS_ONw(0x0); 	// IR-LED Off 20141103 JYP	// 170209 KSH
  #else
	#error if use LPWM for IR-LED Control, please set "USE_IR_LED_LPWM 1" in "peripheral.cmake"
  #endif
#endif

#if (model_Iris==0)
	#define AE_IRS_OPN_Max		0x0
	#define AE_IRS_CLS_Max		0x0
	#define AE_IRS_STAT_Max		0

#elif (model_Iris==1) //-----------------------------------------------------------
	#define AE_IRS_OPN_Max		0x100
	#define AE_IRS_CLS_Max		0x300
	#define AE_IRS_STAT_Max		1200

#elif (model_Iris==2) //-----------------------------------------------------------
	#define AE_IRS_OPN_Max		0x145/*0x1c5*//*0x180*/
	#define AE_IRS_CLS_Max		0x345/*0x2c5*//*0x380*/
	#define AE_IRS_STAT_Max		/*1800*/(1200/*<<(AE_FUNC_FPS>30)*/)

#endif


#define AE_DEBUG_X		24
#define AE_WDR_DEBUG_Y	7

#ifdef AE_GRP
	#define AeTxGrpFnc()	//UartTxGrp()
#else
	#define AeTxGrpFnc()
#endif

// Lib link ---------------------------------------

extern int AE_GAIN_TOTAL;
extern int AE_GAIN_TOTAL2;

extern const BYTE gbSensShtDly;
extern const BYTE gbSensAgcDly;
extern const BYTE gbVHssAgcTbl[];
extern const UINT gVHssAgcTblEa;

//extern int giIrsVal;
extern BYTE gbIrsDly;

extern int giShtVal;
extern BYTE gbShtDly;
extern BYTE gbVHssAgc;

extern int giShtSVal;
extern BYTE gbVHssSAgc;

extern int giAgcVal;
extern BYTE gbAgcDly;
extern int giAgcUpSpd;
extern int giAgcDnSpd;

extern int iCurBg;
extern int iCurSpot;
extern int iTgtSpotBg;

extern WORD AeSHT0(int aiVal, UINT anManual, UINT anManualVal);
extern void AeAGC0(int aiVal);
extern int AGC2POS(int aiAgc);
extern int POS2AGC(int aiPos);

extern void SetAeDevDly(const BYTE, const BYTE);
extern void InitAeCtrl(const BYTE, const BYTE, const BYTE*, const UINT, const WORD);
extern int ShtCtrl(const BYTE abCH, const int aiErr, const int aiMargin, const int aiGain, const int aiShtMin, const int aiShtMax, const BYTE abEstOn, const BYTE abDlyOn, const BYTE abAgcOn, const int aiShtManual);
extern int AgcCtrl(const int aiErr, const int aiMargin, const int aiGain, const int aiAgcMin, const int aiAgcMax, const BYTE abEstOn, const BYTE abDlyOn, const BYTE abManual, const int aiAgcManual);
extern int IrsCent(const int aiErr, const int aiMargin, const int aiIrisOpenMax, const int aiIrisCloseMax, const int aiIrsPosMax);
extern int IrsCtrl(const int aiErr, const int aiMargin, const int aiGain, const int aiIrsCen, const BYTE abManual, const int aiIrsManual);
extern void IrsPos(const int aiErr, const int aiErrMgn, const int aiGain, const int aiIrsPosMin, const int aiIrsPosMax);
extern int ErrNor(const int aiErr, const int aiCur, const int aiTgt, const BYTE abHalf, const UINT anPosGainBit, const UINT anNegGainBit);
extern int ErrMgn(const int aAE_ERR_MGN_MAX, const int aAE_ERR_MGN_MIN, const int aAE_ERR_MIN, const BYTE abTgtChgCnt);
extern void LtPosThSet(void);
extern int LtPos(const int aiVal0, const int aiVal1);
extern int CurBg(const int aiTgt, const UINT nSlicLvl, const UINT nSlicCnt, const UINT nClipLvl, const UINT nClipCnt, const UINT nSum1, const UINT nPxCnt, const int aiTgtMin);
extern void CurSat(const UINT nClipLvl, const UINT nClipCnt, const UINT nSum2, const int iTgtOfs);
extern void SatCtrl(const BYTE abSatOff, int *apiErr, const int iErrMgn, const int nClipCnt);
extern int WdrAgcWgt(const BYTE abWdrAgcWgtOn, const int aiWdrAgcWgt, const int aiOffCnt, const int aiCtrlSpd);
extern void LibWdrCtrl(const UINT );
extern void AeMon(const BYTE, const BYTE, const int, const int, const int, const BYTE, const BYTE, const BYTE, const BYTE, const BYTE, const BYTE,	const int, const int, const int, const int, const int, const BYTE);


//*******************************************************************************
// Variables
//*******************************************************************************

int		giCurAgc, giCurDss;
int		giCurDssStg;

BYTE	gbWdrOn=0, gbWdrOnBuf=0/*, gbWdrOnBuf2=0*//*, gbWdrOnBuf3=0*/;	// 170530 KSH
WORD 	/*gwWdrCnt=0,*/ gwWdrGmk=0x80<<4;

UINT	gnLSflag;

UINT 	gnAeAgcTVal;

UINT 	gnAeVtw = RP(FR_VTW60);
UINT	gnAeHtw = RP(FR_HTW60);

int 	giErr1d,giErr2d;

BYTE 	gbAeDebugOn;
BYTE 	gbDssRatioLmt = 0;

int 	giIrsCent = /*0x1E5*/(AE_IRS_CLS_Max+AE_IRS_OPN_Max)<<(6-1);	// mean		Enx EV38 AR0230 = 0x1E5
int 	giIrsPos = AE_IRS_STAT_Max/*/2*/;
int		giIrsValBuf;

int iTgtMax = 0;
int iTgtMin = 0;
int iTgt = 0;
int iTgtVal = 0;
//UINT nClipCntIIR = 0;
//int iTgtIIR = 0;

int giShtMin = 0;
int giShtMax = 0;

int giAgcMin = 0;
int giAgcMax = 0;

int giShtDblMax = 0;
int giAgcDblMax = 0;

BYTE gbAeStg = AE_STG_SHT;

BYTE gbTgtChg = 0;

BYTE gbSensUpdate = 0;
//BYTE gbSensUpdateCnt = 0;


//*******************************************************************************
// Sub Functions
//*******************************************************************************
#if 0
void isp_AeTest(void)
{
	#define AE_GET(CH,TYPE)		((CH == 0) ? AE1_##TYPE##r :\
								 (CH == 1) ? AE2_##TYPE##r :\
								 (CH == 2) ? AE3_##TYPE##r :\
								 (CH == 3) ? AE4_##TYPE##r :\
								 (CH == 4) ? AE5_##TYPE##r :\
								 (CH == 5) ? AE6_##TYPE##r : 0)\

	const UINT 	nAe1Sum0    = AE_GET(PAR00, SUM0_LOCK);
	const UINT 	nAe1Sum1    = AE_GET(PAR00, SUM1_LOCK);
	const UINT 	nAe1Sum2    = AE_GET(PAR00, SUM2_LOCK);
	const UINT 	nAe1SlicCnt = AE_GET(PAR00, SLCNT_LOCK);
	const UINT 	nAe1ClipCnt = AE_GET(PAR00, CLCNT_LOCK);
	const UINT	nAe1ClipLvl	= AE_GET(PAR00, CLIP);
	const UINT	nAe1SlicLvl	= AE_GET(PAR00, SLICE);
	const UINT  nAe1Vep     = AE_GET(PAR00, VEP);
	const UINT  nAe1Vsp     = AE_GET(PAR00, VSP);
	const UINT  nAe1Hep     = AE_GET(PAR00, HEP);
	const UINT  nAe1Hsp     = AE_GET(PAR00, HSP);


	const UINT	nAe1PxCnt	= ((nAe1Vep - nAe1Vsp)+1) * ((nAe1Hep - nAe1Hsp)+1);

	const int iAe1Cur = (int)(((nAe1SlicCnt*nAe1SlicLvl) + nAe1Sum1 + (nAe1ClipCnt*nAe1ClipLvl)) / nAe1PxCnt);

	GRP0 = iAe1Cur;
	GRP1 = (nAe1SlicCnt) ? nAe1Sum0 / nAe1SlicCnt : 0;
	GRP2 = (nAe1PxCnt > (nAe1SlicCnt+nAe1ClipCnt)) ? nAe1Sum1 / (nAe1PxCnt - (nAe1SlicCnt+nAe1ClipCnt)) : 0;
	GRP3 = (nAe1ClipCnt) ? nAe1Sum2 / nAe1ClipCnt : 0;

	GRP4 = nAe1Sum0;//nAe1SlicCnt;
	GRP5 = nAe1Sum1;//(nAe1PxCnt > (nAe1SlicCnt+nAe1ClipCnt)) ? (nAe1PxCnt - (nAe1SlicCnt+nAe1ClipCnt)) : 0;
	GRP6 = nAe1Sum2;//nAe1ClipCnt;

	//GRP7 = nAe1ClipCnt;
	GRP7+=10;
	if(GRP7 > 500) GRP7 = 0;


	//nAe1Sum1 / ((AE1_VEPr - AE1_VSPr)+1) * ((AE1_HEPr - AE1_HSPr)+1)

	//GRP7 = nAe1Sum2

	UartTxGrp();
}
#endif

#if 0
void TestCur10bit(void)
{
	#define DivFnc1		udiv4x
	{ const UINT nSum = 0x1FFFFFFF;	const UINT nCnt = 0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc1(nSum, nCnt, 0)); }
	{ const UINT nSum = 0x2FFFFFFF;	const UINT nCnt = 0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc1(nSum, nCnt, 0)); }	// !
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc1(nSum, nCnt, 0)); }	// !
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x2;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc1(nSum, nCnt, 0)); }	// !
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x3;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc1(nSum, nCnt, 0)); }
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x4;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc1(nSum, nCnt, 0)); }
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x5;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc1(nSum, nCnt, 0)); }	// !

	#define DivFnc2		div4x
	extern int DivFnc2(const int aiNumer, const int aiDenom, const int aiZeroDenom);
#if 0
	printf("\r\n");
	{ const UINT nSum = 0x1FFFFFFF;	const UINT nCnt = 0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc2(nSum, nCnt, 0)); }
	{ const UINT nSum = 0x2FFFFFFF;	const UINT nCnt = 0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc2(nSum, nCnt, 0)); }	// !
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc2(nSum, nCnt, 0)); }	// !
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x2;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc2(nSum, nCnt, 0)); }	// !
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x3;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc2(nSum, nCnt, 0)); }
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x4;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc2(nSum, nCnt, 0)); }
	{ const UINT nSum = 0x3FFFFFFF;	const UINT nCnt = 0x5;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, nCnt, DivFnc2(nSum, nCnt, 0)); }	// !
#elif 0
	printf("\r\n");
	{ const UINT nSum = 0x1FFFFFFF;	const int iCnt = -0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, iCnt, DivFnc2(nSum, iCnt, 0)); }
	{ const UINT nSum = 0x2FFFFFFF;	const int iCnt = -0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, iCnt, DivFnc2(nSum, iCnt, 0)); }	// !
	{ const UINT nSum = 0x3FFFFFFF;	const int iCnt = -0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, iCnt, DivFnc2(nSum, iCnt, 0)); }	// !
	{ const UINT nSum = 0x3FFFFFFF;	const int iCnt = -0x2;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, iCnt, DivFnc2(nSum, iCnt, 0)); }	// !
	{ const UINT nSum = 0x3FFFFFFF;	const int iCnt = -0x3;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, iCnt, DivFnc2(nSum, iCnt, 0)); }
	{ const UINT nSum = 0x3FFFFFFF;	const int iCnt = -0x4;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, iCnt, DivFnc2(nSum, iCnt, 0)); }
	{ const UINT nSum = 0x3FFFFFFF;	const int iCnt = -0x5;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", nSum, iCnt, DivFnc2(nSum, iCnt, 0)); }	// !
#else
	printf("\r\n");
	{ const int iSum = -0x1FFFFFFF;	const int iCnt = -0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", iSum, iCnt, DivFnc2(iSum, iCnt, 0)); }
	{ const int iSum = -0x2FFFFFFF;	const int iCnt = -0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", iSum, iCnt, DivFnc2(iSum, iCnt, 0)); }	// !
	{ const int iSum = -0x3FFFFFFF;	const int iCnt = -0x1;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", iSum, iCnt, DivFnc2(iSum, iCnt, 0)); }	// !
	{ const int iSum = -0x3FFFFFFF;	const int iCnt = -0x2;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", iSum, iCnt, DivFnc2(iSum, iCnt, 0)); }	// !
	{ const int iSum = -0x3FFFFFFF;	const int iCnt = -0x3;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", iSum, iCnt, DivFnc2(iSum, iCnt, 0)); }
	{ const int iSum = -0x3FFFFFFF;	const int iCnt = -0x4;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", iSum, iCnt, DivFnc2(iSum, iCnt, 0)); }
	{ const int iSum = -0x3FFFFFFF;	const int iCnt = -0x5;	printf("** (4 x 0x%x) / 0x%x = %d\r\n", iSum, iCnt, DivFnc2(iSum, iCnt, 0)); }	// !
#endif
}
#endif


void ISRT AeIRS(const int aiVal)	// Fixed
{	// IRIS driver

		//PwmSetDty(aiVal);
#ifdef __USE_IRIS_PWM__
		TimerSetTrig(TIMER_PWM_IRIS, aiVal);
#endif

		//DebugDisp(gbAeDebugOn, Hex, "IRS_CEN_", 3, 24, giIrsCent>>6	)
		//DebugDisp(gbAeDebugOn, Hex, "IRS_VAL_", 4, 24, aiVal			)
		//DebugDisp2(gbAeDebugOn, Hex, "_IrsCen", 10, 24, giIrsCent>>6, 4)
		//DebugDisp2(gbAeDebugOn, Hex, "_IrsVal", 11, 24, aiVal, 4)
		if(gbAeDebugOn) {
			DispStr("IRS", 2, 24, 3);	FontEx(2, 24, NO_ALPHA, MN_GREEN, 3);
			DispDec(2, 24+4, giIrsCent>>6, 3);
			DispDec(2, 24+4+3+1, aiVal, 4);
		}

#if IRIS_DBG
		GRP0 = giIrsCent>>6;
		GRP1 = nDTY;
		TxGrp();
#endif
}

void ISRT AeDev(void)
{
#if AE_DEV_ON
	gbSensUpdate = 1;

//	FUNC_TIME_OSD(gbMnDebugFnc==1, AeDev, 16, 24,
	const ULONG AeDevTimeSta = BenchTimeStart();

		AE_ETCw(0);
		AE_FREQw(0,0);
		AE_DSSw(0);
		AE_SHTw(0);
		AE_AGCw(0);
		//AE_DGCw(0);
//	)
	FontBenchTime(gbMnDebugFnc==1, 16, 0, "AeDev", AeDevTimeSta, 6);

	gbSensUpdate = 0;
#endif

	//extern void SensorIn(const BYTE abOn);
	//SensorIn(1);
}

void ISRT FreqAdjust(void)
{
	FPS_VDO = FPS_VDOr;
	FPS_VDI = FPS_VDIr;

	if (UP(SysFreq)==UP_SYSFREQ_60) {
		gnAeVtw = RP(FR_VTW60);
		gnAeHtw = RP(FR_HTW60);
	}
	else {
		gnAeVtw = RP(FR_VTW50);
		gnAeHtw = RP(FR_HTW50);
	}

#if (model_Sens == SENS_AR0237CSH) // 180329 KSH - AR0237 Frame Control
	if(FPS_HI) { AE_ETCw(1); }
	else	   { AE_ETCw(2); }
	AE_FREQw(gnAeVtw, gnAeHtw);
#elif model_2M
	AE_FREQw(gnAeVtw, (FPS_HI) ? gnAeHtw : (gnAeHtw<<1));
#else
	AE_FREQw(gnAeVtw, gnAeHtw);
#endif
}

void InitAe(void)
{
	AE_LOCK_SELw(1);
	AE_WIN_SELw(7);

	// 2015826 - WHL - AE1 area set for Histogram window (Same with AE Window2)
	#define AE_AREA_INIT(NUM)	{ AE##NUM##_HSPw(RP(AE_HSP)); AE##NUM##_HEPw(RP(AE_HEP)); AE##NUM##_VSPw(RP(AE_VSP)); AE##NUM##_VEPw(RP(AE_VEP)); }
	AE_AREA_INIT(1)

	AE1_SLICE0w(0x0);			// DEFOG
	AE1_CLIP0w(0x3ff);			// "
	AE2_SLICE0w(0x0);			// Main
	AE2_CLIP0w(0x3ff);			// "
	AE3_SLICE0w(0x0);			// BLC
	AE3_CLIP0w(0x3ff);			// "
	AE4_SLICE0w(0x0);
	AE4_CLIP0w(0x3ff);

#if WDR_OLD == 1
	WDR_MOTION_ONw(0/*UP(AE_WDR_MOTION_SEL)*/);		// 150615 LH
#endif

#if (model_Iris==4)

	TwiRdEep(TWI_EEP_IRIS_STTZ , (BYTE*)giIrisLmtVal, 8);

	if (((UINT)giIrisLmtVal[0]>AE_PIRS_POS_MAX)||((UINT)giIrisLmtVal[1]>AE_PIRS_POS_MAX)||((UINT)giIrisLmtVal[0]>(UINT)giIrisLmtVal[1])) {
		giIrisLmtVal[0] = AE_PIRS_BOT_DFLT;
		giIrisLmtVal[1] = AE_PIRS_TOP_DFLT;
	}
#endif

	// sync allign at system init
	//FreqAdjust();

#if (model_Sens == SENS_OV2718)
	AE_DSSw(gnAeVtw);
#endif

	AeSHT0(0, 1, gnAeVtw);

	//AeDev();

#if (model_Led == 0)							// 141120 IR-LED Logic PWM
	IRIS_ONw(0x1/*0x0*/);						// 170209 KSH
	IRIS_SWw(0x0);
	IRIS_MODw(0x1);
	IRIS_HSPw(0x0);
	IRIS_HEPw(1375);
	IRIS_VSPw(0x0);
	IRIS_VEPw(0xfff);

	IRIS_CLPLw(0x0);
	IRIS_CLPHw(1375);							// Pulse Duty control : 1 ~ 1375 (full)
#endif

#if AE_CTRL_ADV
	InitAeCtrl(gbSensShtDly, gbSensAgcDly, gbVHssAgcTbl, gVHssAgcTblEa, ERR_SPOT_SUP_GAB);
#endif

#ifdef ENABLE_PA
	ParAe(PA_IRS_GAIN_MIN_ERR)	= 100/*20*//*80*/;
	ParAe(PA_IRS_GAIN_MAX_ERR)	= 300/*200*/;

	ParAe(PA_AGC_GAIN_MIN_ERR)	= 20/*10*//*20*//*80*/;
	ParAe(PA_AGC_GAIN_MAX_ERR)	= 100/*30*//*200*/;

	ParAe(PA_ERR_MGN)			= 8/*4*/;	// BRIGHTNESS 20 에서 8 이상이어야 AGC헌팅 없음
	//ParAe(PA_ERR_UP_GAIN)		= 2;
	//ParAe(PA_ERR_DOWN_GAIN)		= 1;

#if 1	// NORMAL
	ParAe(PA_IRS_GAIN_MIN_BIT)	= 2;
	//ParAe(PA_AGC_GAIN_MIN_BIT)	= 2;
	//ParAe(PA_SHT_UP_SPEED)		= 200;
	//ParAe(PA_SHT_DOWN_SPEED)	= 2000;
	ParAe(PA_AGC_UP_SPEED)		= 20/*40*/;
	ParAe(PA_AGC_DOWN_SPEED)	= 200/*400*/;
	ParAe(PA_IRS_OPEN_SPEED)	= 1024;
	ParAe(PA_IRS_CLOSE_SPEED)	= 1024;
#else	// BLC
	ParAe(PA_IRS_GAIN_MIN_BIT)	= 2;
	//ParAe(PA_AGC_GAIN_MIN_BIT)	= 2;
	//ParAe(PA_SHT_UP_SPEED)		= 200;
	//ParAe(PA_SHT_DOWN_SPEED)	= 2000;
	ParAe(PA_AGC_UP_SPEED)		= 40;
	ParAe(PA_AGC_DOWN_SPEED)	= 400;
	ParAe(PA_IRS_OPEN_SPEED)	= 512;
	ParAe(PA_IRS_CLOSE_SPEED)	= 512;
#endif

	ParAe(PA_TGT_NIGHT_OFS)		= 64;

	ParAe(PA_IRS_POS_SPEED)		= UpAE_SPEED_DC_VP;

	ParAe(PA_ERR_CHT_TH)		= 60;

	ParAe(PA_WDR_SHORT_MIN1)	= 8/*1*/;

	ParAe(PA_H264_SKIP_CNT)		= 6;

	//ParAe(PA_ERR_EST_OFF)		= 1;

	//ParAe(PA_SHT_MANUAL)		= 1125;

	//ParAe(PA_CTRL_ORIGINAL)		= 1;
	//ParAe(PA_SAT_OFF)			= 1;
#endif

	//ShutterMenuGet();

#if model_ISP_Save == 1
	if(gUdAeStat->bErrNightCnt <= (1<<5)) bErrNightCnt = gUdAeStat->bErrNightCnt;
	if(gUdAeStat->bErrOfsCnt <= (1<<5)) bErrOfsCnt = gUdAeStat->bErrOfsCnt;
	if(gUdAeStat->bAeStg <= AE_STG_TGT) gbAeStg = gUdAeStat->bAeStg;

	const int iIrsCent = cat2(gUdAeStat->bIrsCent);
	if(AE_IRS_OPN_Max <= iIrsCent && iIrsCent <= AE_IRS_CLS_Max) giIrsCent = iIrsCent;

	const int iIrsVal = cat2(gUdAeStat->bIrsValBuf);
	if(0 <= iIrsVal && iIrsVal <= 0x3fe) giIrsValBuf = iIrsVal;


	if(SHT_MIN==0) SHT_MIN = 1;				// TODO KSH> 초기값 설정 어디서?
	if(SHT_MAX==0) SHT_MAX = (gnAeVtw<<6);	// <<6 = x64

	const int iShtVal = cat2(gUdAeStat->bShtVal);
	if(SHT_MIN <= iShtVal && iShtVal <= SHT_MAX) {
		SHT_SET(iShtVal);//giShtVal = iShtVal;
		AeSHT0(ParAe(PA_SHT_MANUAL) ? (int)ParAe(PA_SHT_MANUAL) : giShtVal, (UP(Shutter)==UP_SHUT_MNL), (gnAeVtw>>UP(ShutSpd)) );
	}

	const int iTgtVal0 = cat2(gUdAeStat->bTgtVal);
	if(0 <= iTgtVal0 && iTgtVal0 <= TgtMaxGet(0,0)) iTgtVal = iTgtVal0;

	const int iAgcVal = cat2(gUdAeStat->bAgcVal);
	if(1 <= iAgcVal && iAgcVal <= (int)(gnAeVtw<<6)) {
		AGC_SET(iAgcVal);//giAgcCtrl = iAgcVal<<AGC_VAL_GAB
		giCurAgc = AGC2POS(giAgcVal);
		AeAGC0(ParAe(PA_AGC_MANUAL) ? (int)ParAe(PA_AGC_MANUAL) : ((gbAeStg==AE_STG_SHT) ? gbVHssAgc : 0) + giAgcVal);
	}

	const int iShtSVal = cat2(gUdAeStat->bShtSVal);
	if(SHT_MIN <= iShtSVal && iShtSVal <= SHT_MAX) SHT_SET1(iShtSVal);//giShtSVal = iShtSVal;		// <<6 = x64

	//TxStrDec("Sht:", giShtVal, 4);
	//TxStrDec("Agc:", giAgcVal, 4);
	//TxStrDec("ErrNightCnt:", bErrNightCnt, 2);
	//TxStrDec("ErrOfsCnt:", bErrOfsCnt, 2);
#endif

	//SHT_SET(4);
	//AeSHT0(giShtVal, 0, 0);

	AeDev();

	//TestCur10bit();
}

void ISRT AntiSatStabilizingSet(void)
{
	UP(ErrChtMax) =	(UP(Stabilizing)==UP_4sOFF) ? 1 :
					(UP(Stabilizing)==UP_4sLOW) ? (ERR_CHT_SIZ>>1) :
					(UP(Stabilizing)==UP_4sMID) ? (ERR_CHT_SIZ>>1)+(ERR_CHT_SIZ>>2) :
												   ERR_CHT_SIZ ;

	UP(ErrChtMin) =	(UP(Stabilizing)==UP_4sOFF) ? 1 :
					(UP(Stabilizing)==UP_4sLOW) ? (ERR_CHT_SIZ>>2) :
					(UP(Stabilizing)==UP_4sMID) ? (ERR_CHT_SIZ>>1) :
												  (ERR_CHT_SIZ<<1)/3 ;

	//TxStrDec("MpErrChtMax: ", UP(ErrChtMax), 3);
	//TxStrDec("MpErrChtMin: ", UP(ErrChtMin), 3);

	//static BYTE bMpErrChtMax1d = -1;	// AE Test
	//static BYTE bMpErrChtMin1d = -1;
	//if(bMpErrChtMax1d != UP(ErrChtMax) || bMpErrChtMin1d != UP(ErrChtMin)) {
	//	TxStrDec("*ERR_CHT_MAX:", UP(ErrChtMax), 3);
	//	TxStrDec("*ERR_CHT_CUT:", UP(ErrChtMin), 3);
	//	bMpErrChtMax1d = UP(ErrChtMax);
	//	bMpErrChtMin1d = UP(ErrChtMin);
	//}
}

void ISRT AeODM(void)
{
	UINT	nBlcPosX, nBlcPosY, nBlcSizX, nBlcSizY;


	if(gbAeDebugOn||(gbBlcOsdOn/*&&(UP(BlcMode)==UP_BLC_MODE_CUSTOMIZE)*/)) {
		OZONE_SELw(1);
		//AE_WIN_SELw((UP(BackLight) == UP_BL_BLC) ? 6 : 2);
	}

	if(UP(Defog) == UP_OFF || UP(DefogMode) == UP_DEFOG_MANUAL) {	// Defog-Auto일 경우 AceDefog()함수에서 AE1 ODM 설정함
		AE_AREA_INIT(1)
	}

	if((UP(BackLight)==UP_BL_BLC) /*|| UP(WdrBlc)*/) {

	 	const UINT nBlcHw = /*((UP(OutFps)==UP_2M_720p_25_30)||(UP(OutFps)==UP_2M_720p_50_60)) ? FR_HW_720 :*/ RP(FR_HW);	// 1M Sensor는 FR_HW_720 와 FR_HW 가 동일
		const UINT nBlcVw = /*((UP(OutFps)==UP_2M_720p_25_30)||(UP(OutFps)==UP_2M_720p_50_60)) ? FR_VW_720 :*/ RP(FR_VW);	// 1M Sensor는 FR_VW_720 와 FR_VW 가 동일

		const BYTE bBlcPosX = (UP(BlcMode)==UP_BLC_MODE_CENTER) ?  6 : (UP(BlcMode)==UP_BLC_MODE_SPOT) ? 8 : UP(BlcPosX) ;
		const BYTE bBlcPosY = (UP(BlcMode)==UP_BLC_MODE_CENTER) ?  5 : (UP(BlcMode)==UP_BLC_MODE_SPOT) ? 8 : UP(BlcPosY) ;
		const BYTE bBlcSizX = (UP(BlcMode)==UP_BLC_MODE_CENTER) ?  8 : (UP(BlcMode)==UP_BLC_MODE_SPOT) ? 4 : UP(BlcSizX) ;
		const BYTE bBlcSizY = (UP(BlcMode)==UP_BLC_MODE_CENTER) ? 10 : (UP(BlcMode)==UP_BLC_MODE_SPOT) ? 4 : UP(BlcSizY) ;

	 	nBlcPosX = RP(AE_HSP) + (nBlcHw*bBlcPosX/40);
		nBlcPosY = RP(AE_VSP) + (nBlcVw*bBlcPosY/20);
		nBlcSizX = nBlcPosX + (nBlcHw*bBlcSizX/40);
		nBlcSizY = nBlcPosY + (nBlcVw*bBlcSizY/20);

		nBlcSizX = MIN(nBlcSizX, RP(AE_HEP));
		nBlcSizY = MIN(nBlcSizY, RP(AE_VEP));
		nBlcPosX = MIN(nBlcPosX, RP(AE_HEP));
		nBlcPosY = MIN(nBlcPosY, RP(AE_VEP));

		AE2_HSPw(nBlcPosX);
		AE2_HEPw(nBlcSizX);
		AE2_VSPw(nBlcPosY);
		AE2_VEPw(nBlcSizY);

	} else {
		AE_AREA_INIT(2)
	}

	if(UP(BackLight) == UP_BL_WDR/*gbWdrOn*/) {		// for Short in WDR
		LBUFS0_ONw(1);
		AE3_WIN_CHw(1);
		AE3_HSPw(AE2_HSPr);
		AE3_HEPw(AE2_HEPr);
		AE3_VSPw(AE2_VSPr);
		AE3_VEPw(AE2_VEPr);
	}
	else {											// for All Area & Original Clip
		LBUFS0_ONw(0);
		AE3_WIN_CHw(0);
		AE_AREA_INIT(3)
	}

#if model_LED_ASAT
	if(AE_LED_ASAT_COND) {
		const UINT AE2_HW_Unit = RP(AE_HEP) - RP(AE_HSP);
		const UINT AE2_VW_Unit = RP(AE_VEP) - RP(AE_VSP);
		const UINT AE2_HW_Gab = (UP(LedSatCtrl) > 10) ? (AE2_HW_Unit>>2) + ((AE2_HW_Unit/10) * (UP(LedSatCtrl) - 10) + 10) / 20 :
														((AE2_HW_Unit>>1) * UP(LedSatCtrl) + 10) / 20;
		const UINT AE2_VW_Gab = (UP(LedSatCtrl) > 10) ? (AE2_VW_Unit>>2) + ((AE2_VW_Unit/10) * (UP(LedSatCtrl) - 10) + 10) / 20 :
														((AE2_VW_Unit>>1) * UP(LedSatCtrl) + 10) / 20;
		AE2_HSPw(RP(AE_HSP)+AE2_HW_Gab);
		AE2_HEPw(RP(AE_HEP)-AE2_HW_Gab);
		AE2_VSPw(RP(AE_VSP)+AE2_VW_Gab);
		AE2_VEPw(RP(AE_VEP)-AE2_VW_Gab);
	}
#endif

}

int ISRT TgtMaxGet(const BYTE abWdrOn, const UINT anLSflag)
{
	int iTgtOut;

	if(abWdrOn) {
		if(anLSflag==AeSHORT) {
//			iTgtOut = (AE_WDR_LTGT_GAIN * UP(Brightness)) + UP(AE_WDR_LTGT_OFST);
			iTgtOut = (AE_WDR_LTGT_GAIN * UP(Brightness)) + ((UP(AE_WDR_STYLE_SEL)==0)? UP(AE_WDR_LTGT_OFST) : UP(AE_WDR_LTGT_OFST2)) ; // 150802
//			iTgtOut = (AE_WDR_LTGT_GAIN * UP(Brightness)) + UP(AE_WDR_LTGT_OFST2) + (int)(0x10*((int)UP(WdrWgt)-3));	// 150802
		}
		else {
//			iTgtOut = (AE_WDR_STGT_GAIN * UP(Brightness)) + UP(AE_WDR_STGT_OFST);
			iTgtOut = (AE_WDR_STGT_GAIN * UP(Brightness)) + ((UP(AE_WDR_STYLE_SEL)==0)? UP(AE_WDR_STGT_OFST) : (UP(AE_WDR_STGT_OFST2)<<2)) ; // 150802 : if want preserve Short DR
//			iTgtOut = (AE_WDR_STGT_GAIN * UP(Brightness)) + UP(AE_WDR_STGT_OFST) + (int)(0x30*((int)UP(WdrWgt)-3));	// 150802 : if want preserve Short contrast
		}
	}
	else {
		iTgtOut = (UP(AE_TGT_GAIN) * UP(Brightness)) + UP(AE_TGT_OFST);
	}

	return iTgtOut;
}

void ISRT CurWDR(int *apiCur, int *apiTgt, int *apiErr,	const UINT anSum1, const UINT anClipLvLong, const UINT anClipCntLong, const UINT anPxCntLong,
		   int *apiCurS1, int *apiTgtS1, int *apiErrS1, const UINT anSum2, const UINT anClipLvShort, const UINT anClipCntShort, const UINT anPxCntShort)
{
	int iCur, iTgt, iErr;

	if(gbWdrOn&&(gnLSflag==AeLONG)) {
		*apiCur = 0;
		*apiTgt = 0;
		*apiErr = 0;
		*apiCurS1 = 0;
		*apiTgtS1 = 0;
		*apiErrS1 = 0;
		return;
	}

	// Long proc ----------------------------------------------------------------------------------
	iCur = udiv4x(anSum1 + (anClipCntLong*anClipLvLong), anPxCntLong, 0);

	iTgt = TgtMaxGet(UP_ON, AeSHORT);
	iTgt = MAX(iTgt-(giCurAgc/2), (iTgt/2));	// 13. 8. 4		// EN781 diff

	iErr = iTgt - iCur;

	//if(AE_CTRL_ORIGINAL&&(UP(Shutter)==UP_SHUT_MNL)&&(iErr<0)&&(gnAeState==0)) iErr = 0;	// 170331 KSH

	AE2_SLICE0w(iTgt/3);		// 150802		// EN781 diff
	AE2_CLIP0w(iTgt + AE_WDR_LCLIP_OFST);							// next long

	//GrpAe(GA_WDR_LONG_CLIP) = iTgt + AE_WDR_LCLIP_OFST;

	*apiCur = iCur;
	*apiTgt = iTgt;
	*apiErr = iErr;


	// Short proc ---------------------------------------------------------------------------------
#if 0
	iCur = LibUtlInterp1D(MIN(anClipCntShort, AE_WDR_SWGT_MAXCNT), 0, AE_WDR_SWGT_MAXCNT, anClipLvShort, (anClipCntShort==0) ? 0 : anSum2 / anClipCntShort );
#else
	const int iSClipWgt = LibUtlInterp1D(MIN(anClipCntShort, AE_WDR_SWGT_MAXCNT), 0, AE_WDR_SWGT_MAXCNT, 0, 0x100);
	iCur = ( (iSClipWgt * udiv4x(anSum2, anClipCntShort, anClipLvShort<<2)) + ((0x100-iSClipWgt) * (anClipLvShort<<2)) ) >> 8;

	DebugDisp2(gbAeDebugOn, Dec, "WdrSCpW", AE_WDR_DEBUG_Y+6/*23*/, AE_DEBUG_X, iSClipWgt, 4)
#endif

	//GrpAe(GA_WDR_SHORT_CLIP_1D) = anClipLvShort;

	iTgt = TgtMaxGet(UP_ON, AeLONG);
	iTgt = MAX(iTgt-(giCurAgc/2), (iTgt/2));	// 13. 8. 4		// EN781 diff

	iErr = iTgt - iCur;

	AE3_SLICE0w(0);
	AE3_CLIP0w(iTgt + AE_WDR_SCLIP_OFST);							// next short

	//GrpAe(GA_WDR_SHORT_CLIP) = iTgt + AE_WDR_SCLIP_OFST;
	//GrpAe(GA_WDR_SHORT_CUR) = iCur;

	*apiCurS1 = iCur;
	*apiTgtS1 = iTgt;
	*apiErrS1 = iErr;
}

int ISRT WdrCtrl(WORD *apwShtInt)
{
	#define AE_WDR_ON_COND	((gnLSflag==AeLONG)&&(UP(BackLight)==UP_BL_WDR)/*&&(UP(Shutter)==UP_SHUT_AUTO)*/)
	#define AE_WDR_OFF_COND	(((gnLSflag==AeLONG/*AeSHORT*/)&&(UP(BackLight)!=UP_BL_WDR))/*||(UP(Shutter)!=UP_SHUT_AUTO)*/)

	//--------------------------------------------------------------------------------
	// DNR OFF -> 3 Delay -> WDR ON
#if 0
	static BYTE bWaitRd_Cnt = 4;
	//if(((UP(BackLight)==UP_BL_WDR) != (gbWdrOn==UP_ON)) && (bWaitRd_Cnt == 0)) { bWaitRd_Cnt = 3; return ; }	// Detecting WDR ON/OFF Change
	if(((UP(BackLight)==UP_BL_WDR) && (gbWdrOn==UP_OFF)) && (bWaitRd_Cnt == 0)) { bWaitRd_Cnt = 3; return 0; }		// Detecting WDR ON only
	if(bWaitRd_Cnt > 1) { bWaitRd_Cnt--; return 0; }

	if(giCurDssStg > 1) return 0;

	if((bWaitRd_Cnt==1) && (gnLSflag==AeLONG)) bWaitRd_Cnt = 0;

	if(AE_WDR_ON_COND && (giCurDssStg==1)) {
		giCurDssStg = 0;

		//TxStr("ISP reset.", 0, 0);
		//UINT nIsp0x5 = GetIsp(0x5);
		//UINT nIsp0x6 = GetIsp(0x6);
		//TxStrNoIRQ(" 5h:", nIsp0x5, 8);
		//TxStrNoIRQ(" 6h:", nIsp0x6, 8);

		isp_reset();

	#if 0//(model_Sens == SENS_OV2718)
		extern void InitSens(void);
		extern void InitSensCstm(void);
		InitSens();
		InitSensCstm();
		AE_FREQw(gnAeVtw, gnAeHtw);
		AE_DSSw(gnAeVtw);
		AeDev();
	#endif

		//nIsp0x5 = GetIsp(0x5);
		//nIsp0x6 = GetIsp(0x6);
		//TxStrNoIRQ(" 5h:", nIsp0x5, 8);
		//TxStrNoIRQ(" 6h:", nIsp0x6, 8);

		//return 0;
	}
#endif
	//--------------------------------------------------------------------------------
	//gbWdrOnBuf3 = gbWdrOnBuf2;	// 170530 KSH
	//gbWdrOnBuf2 = gbWdrOnBuf;
	gbWdrOnBuf  = gbWdrOn;


	if(AE_WDR_ON_COND)			gbWdrOn = UP_ON;	// 170331 KSH
	else if(AE_WDR_OFF_COND) 	gbWdrOn = UP_OFF;	// 150729 LH : Out timing	170331 KSH

	if ((gbWdrOn==UP_ON)||(gbWdrOnBuf2==UP_ON)) {

		WDR_ONw(1);

#if WDR_OLD == 1
//		DDR_RD_MODw(0); // WDR DDR Priority  EN675에서는 필요없음

		//gwWdrCnt = (gwWdrCnt>=32) ? 32 : gwWdrCnt+1;

		//if(PAR00) {
//			if(!gbLssChgCnt) {
//				if (gwWdrCnt<=1)		STLL_ONw(1);	// 150729 LH : img crack at ON/OFF
//				else if (gwWdrCnt>=2)	STLL_ONw(0);
//			}
		//}

		int iSwgt = ((0x800>>2)+((UP(WdrWgt)*2)<<6));							// 150615 LH
		iSwgt = (UP(WdrWgt)==UP_3sLOW) ? ((iSwgt*UP(AE_WDR_SWGT_L))>>7) :
				(UP(WdrWgt)==UP_3sMID) ? ((iSwgt*UP(AE_WDR_SWGT_M))>>7) :
										 ((iSwgt*UP(AE_WDR_SWGT_H))>>7) ;
		WDR_SWGTw(MIN(iSwgt,0x3ff));

//		WDR_SWGTw((0x800>>2)+((UP(WdrWgt)*2)<<6));
#else
		TMG_ONw(/*aiModeROI?0:*/1);			// TMG OFF at ROI
#endif
		static WORD gwShtIntBuf;
		WORD wShtInt = *apwShtInt;

		if (gnLSflag==AeSHORT)	{
#if WDR_OLD == 1
			wShtInt = (wShtInt>=gwShtIntBuf) ? gwShtIntBuf-1 : wShtInt;			// under long

			WDR_SSHUTw(/*ParAe(PA_STA) ? (ParAe(PA_STA)&0xFFF) : */wShtInt);												// Short weight
#else

			const UINT anWeight = (UP(WdrWgt) == 0) ? 0x100 :
								  (UP(WdrWgt) == 1) ? 0x200 :
										  	  	  	  0x400 ;

//			iWdrGainS1 = (gwShtIntBuf*256)/wShtInt;			// WDR S1 gain = (Long * 256)/Short
//			iWdrGain   = ((0x3fffff*256)/((0x3ff*gwShtIntBuf)/wShtInt));		// WDR last gain

			const int iWdrGainS1 = (gwShtIntBuf/*anLef*/*256)/wShtInt/*anSef1*/;							// WDR S1 gain = (Long * 256)/Short
			const int iWdrGainS2 = 0;												// WDR S2 gain = dummy
			UINT SMAX = (0x3ff*gwShtIntBuf/*anLef*/)/wShtInt/*anSef1*/;								// Short max = 1023 * S gain

			SMAX = (SMAX * anWeight)>>8 ;									// 170213 LH : Brightness weight between Dark and Bright area
//			SMAX = (SMAX * 0x100)>>8 ;										// Brightness weight between Dark and Bright area default
			const int iWdrGain   = ((0x3fffff*256)/(SMAX));							// WDR last gain

			WDR_3MODEw(0x0);

//			WDR_LGAINw(0x1000);												// WDR_LGAIN  : 12b float offset
//			WDR_LGAINw(0x100); 												// 180321 LGH : Need to check LH  (WDR_LGAIN  : 12b float offset)
			WDR_SGAINw(iWdrGainS1);         								// WDR_SGAIN  :  8b "
			WDR_SGAIN2w(iWdrGainS2);										// WDR_SGAIN2 :  4b "

			WDR_GAINw(iWdrGain>>4);         								// WDR_GAIN	  :  8b "	TMP
//			WDR_STEPw(4);					// TMP

			WDR_SATVLw(0x3c6);
			WDR_SATVWw(0x1);
			TMG_YHLPFw(0);

			LibWdrCtrl(SMAX);
#endif
		}
		else {
#if WDR_OLD == 1
			wShtInt = MAX(wShtInt, 1);
			WDR_LSHUTw(/*ParAe(PA_STA) ? ((ParAe(PA_STA)>>16)&0xFFF) : */wShtInt);												// Long weight

			WORD wWdrGmkNew = ((256<<4) * gwShtIntBuf) / wShtInt;				// Shutter Rate : 0 ~ 256

			#if (model_Aptn)													// 151020
			#define SHT_RT_WG_ON	96
			#define SHT_RT_WG_OFF	128
			#else
			#define SHT_RT_WG_ON	128											// slce 50%		Shutter Rate of WDR_GAMMA ON
			#define SHT_RT_WG_OFF	192											// clip 75%		Shutter Rate of WDR_GAMMA OFF
			#endif
			wWdrGmkNew = LibUtlInterp1D(CLAMP(wWdrGmkNew, SHT_RT_WG_ON<<4, SHT_RT_WG_OFF<<4), SHT_RT_WG_ON<<4, SHT_RT_WG_OFF<<4, 0x80<<4, 0x0);
			gwWdrGmk = ((0xf0 * gwWdrGmk) + ((0x100-0xf0) * wWdrGmkNew)) >> 8;

			//GrpAe(GA_WDR_GAMMA_WGT) = gwWdrGmk;
#else
			gwWdrGmk = 0x80<<4;
#endif
		}

		gwShtIntBuf = wShtInt;
		*apwShtInt = wShtInt;

		return 0;
	}
	else {
		return 1;
	}
}


//*******************************************************************************
// ADV. AE
//*******************************************************************************
BYTE gbUpShtMin    = UP_MNL_SHUT_DIV512;	// 2 line, 1/15000 sec
BYTE gbUpDeblurMin = UP_MNL_SHUT_DIV1;		// 1125 line, 1/30 sec
BYTE gbUpShtMax    = UP_MNL_SHUT_DIV1;		// 1125 line, 1/30 sec

void ShutterMenuGet(void)
{
	if(UP(Iris)==UP_LENS_MNL) {
		switch(UP(ShtMode)) {
			case UP_SHT_NORMAL:	gbUpShtMin		= UP_MNL_SHUT_DIV512;	// 2 line, 1/15000 sec
								gbUpDeblurMin	= UP_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								gbUpShtMax		= UP_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			case UP_SHT_DEBLUR: gbUpShtMin		= UP_MNL_SHUT_DIV512;	// 2 line, 1/15000 sec
								gbUpDeblurMin	= UP_MNL_SHUT_DIV8/*UP(AE_DEBLUR_THRS_VAL)*/;	// 140 line, 1/250 sec
								gbUpShtMax		= UP_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			default:// UP_SHT_CUSTOM
								gbUpShtMin		= UP(ShtMin);
								gbUpDeblurMin	= UP(DeblurMin);
								gbUpShtMax		= UP(ShtMax);
		}
	}
	else {// UP(Iris) == UP_LENS_DC
		switch(UP(DcMode)) {
			case UP_DC_IN:		gbUpShtMin		= UP_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								gbUpDeblurMin	= UP_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								gbUpShtMax		= UP_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			case UP_DC_OUT:		gbUpShtMin		= UP_MNL_SHUT_DIV8/*UP(AE_OUTDOOR_THRS_VAL)*/;	// 140 line, 1/250 sec
								gbUpDeblurMin	= UP_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								gbUpShtMax		= UP_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			case UP_DC_DEBLUR:	gbUpShtMin		= UP_MNL_SHUT_DIV8/*UP(AE_DEBLUR_THRS_VAL)*/;	// 140 line, 1/250 sec
								gbUpDeblurMin	= UP_MNL_SHUT_DIV8/*UP(AE_DEBLUR_THRS_VAL)*/;	// 140 line, 1/250 sec
								gbUpShtMax		= UP_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			default:// UP_DC_CUSTOM
								gbUpShtMin		= UP(ShtMin);
								gbUpDeblurMin	= UP(DeblurMin);
								gbUpShtMax		= UP(ShtMax);
		}
	}
}

void ShutterMenuSet(void)
{
	if(gbUpShtMin < gbUpShtMax) {
		ShutterMenuGet();
		return;
	}

	if(gbUpDeblurMin > gbUpShtMin) gbUpDeblurMin = gbUpShtMin;
	if(gbUpDeblurMin < gbUpShtMax) gbUpDeblurMin = gbUpShtMax;

	UP(ShtMin) = gbUpShtMin;
	UP(DeblurMin) = gbUpDeblurMin;
	UP(ShtMax) = gbUpShtMax;

#if(model_Iris_DC)
	if(UP(Iris)==UP_LENS_MNL) UP(ShtMode) = UP_SHT_CUSTOM;
	else UP(DcMode) = UP_DC_CUSTOM;
#else
	UP(ShtMode) = UP_SHT_CUSTOM;
#endif
}

int ISRT ClipGet(const int iClipLvD)
{
	const int iClipLvN = (AE_SAT_OFF) ? iClipLvD :
		(ParAe(PA_TGT_NIGHT) ? (int)ParAe(PA_TGT_NIGHT) : 1024 - (UP(LSpotLv)<<4)) - (1 + (ParAe(PA_TGT_NIGHT_OFS) ? (int)ParAe(PA_TGT_NIGHT_OFS) : 64)) ;

	return LtPos(iClipLvD, iClipLvN);
}

void ISRT TgtChg(void)
{
	static BYTE bUpBrightness = 0xff;
	static BYTE bUpBrightnessMin = 0xff;
	//static BYTE bUpAdptAe = 0xff;
	gbTgtChg = bUpBrightness != UP(Brightness) || bUpBrightnessMin != UP(BrightnessMin) /*|| bUpAdptAe != UP(AdptAe)*/;
	bUpBrightness = UP(Brightness);
	bUpBrightnessMin = UP(BrightnessMin);
	//bUpAdptAe = UP(AdptAe);
}

int ISRT TgtGet(const int aiTgtMax, const int aiTgtMin, const BYTE abAeStg)
{
	int iTgt = aiTgtMax;

	#define VAR_TGT_CLIP	(ParAe(PA_SLICE)==0 && ParAe(PA_CLIP)==0)

	if(VAR_TGT_CLIP) {
		/*if(UP(AdptAe) == UP_ADPT_AE_AGC) {
			iTgt = MAX(iTgt-(giCurAgc>>1), (iTgt>>1));
		}
		else if(UP(AdptAe) == UP_ADPT_AE_BR)*/
		{
			if(gbTgtChg) {
				switch(abAeStg) {
					case AE_STG_IRS: iTgtVal = aiTgtMax; break;
					case AE_STG_SHT: iTgtVal = aiTgtMax; break;
					case AE_STG_DBL: iTgtVal = aiTgtMin; break;
					case AE_STG_AGC: iTgtVal = aiTgtMin; break;
					case AE_STG_DSS: iTgtVal = aiTgtMin; break;
					case AE_STG_TGT: if(iTgtVal < aiTgtMin)		 iTgtVal = aiTgtMin;
									 else if(iTgtVal > aiTgtMax) iTgtVal = aiTgtMax;
									 break;
				}
			}

			/*iTgt = LibUtlInterp1D(giShtVal, SHT_MIN, SHT_MAX, iTgt, iTgt>>2);
			*aiTgtDly = (*aiTgtDly < iTgt) ? *aiTgtDly+1 :
					  (*aiTgtDly > iTgt) ? *aiTgtDly-1 :
										 *aiTgtDly   ;
			iTgt = *aiTgtDly;*/

			//if(bMpBrightness != UP(Brightness) || bMpAdptAe != UP(AdptAe)) {
			//	*iTgtVal = iTgt;
			//	//IIRI(iTgtIIR, iTgt);
			//}
			iTgt = iTgtVal;

			//iTgt = LibUtlInterp1D(MAX(giShtVal,SHT_MIN+10), SHT_MIN+10, SHT_MAX, iTgt, iTgt>>2);
		}
	}

	return iTgt;
}

int ISRT TgtMinGet(const int aiTgt, const BYTE abV)
{
	return (abV < 10) ? ((aiTgt*12*abV)>>9) + 4 :
						((aiTgt * (38*(abV-10)+128))>>9);//(iTgtMax>>AE_TGT_MIN_BIT);
}

int ISRT TgtSpotBg(const int aiTgt, const BYTE abV, const int aiTgtMin)
{
#if 1
	UNUSED(aiTgtMin);
	const int iTgtSpotBg0 = ((aiTgt*aiTgt)>>11) + (aiTgt>>1) - 6;
	return (abV<10) ? aiTgt - (aiTgt-iTgtSpotBg0)*abV/10 : (iTgtSpotBg0*(20-abV))/10 ;
#else
	const int iTgtDiff = aiTgt - aiTgtMin;
	int iTgtSpotBg0 = abV * iTgtDiff;
	iTgtSpotBg0 = iTgtDiff - (((iTgtSpotBg0<<2)-iTgtSpotBg0+32)>>6);
	if(iTgtSpotBg0 < 0) iTgtSpotBg0 = 0;
	return iTgtSpotBg0;
#endif
}


//*******************************************************************************
// Main Functions
//*******************************************************************************
void ISRT AeAdv(void)
{
	const UINT 	nAe2Sum1    = AE2_SUM1_LOCKr;		// AE 8BIT, 3840 x 2160 x 255 = 0x7E117000
	const UINT 	nAe2Sum2    = AE2_SUM2_LOCKr;		// "      , "
	const UINT 	nAe3Sum1    = AE3_SUM1_LOCKr;		// "      , "
	const UINT 	nAe3Sum2    = AE3_SUM2_LOCKr;		// "      , "
	const UINT 	nAe2ClipCnt = AE2_CLCNT_LOCKr;
	const UINT 	nAe2SlicCnt = AE2_SLCNT_LOCKr;
	const UINT 	nAe3ClipCnt = AE3_CLCNT_LOCKr;
	const UINT 	nAe3SlicCnt = AE3_SLCNT_LOCKr;

	const UINT	nAe2PxCnt	= ((AE2_VEPr - AE2_VSPr)+1) * ((AE2_HEPr - AE2_HSPr)+1);
	const UINT	nAe3PxCnt	= ((AE3_VEPr - AE3_VSPr)+1) * ((AE3_HEPr - AE3_HSPr)+1);
	const UINT	nAe2ClipLvl	= AE2_CLIPr;			// AE 8BIT
	const UINT	nAe2SlicLvl	= AE2_SLICEr;			// "
	const UINT	nAe3ClipLvl	= AE3_CLIPr;			// "
	const UINT	nAe3SlicLvl	= AE3_SLICEr;			// "

	int		/*iTgt=0,*/ iCur=0, iErr=0;
	int		iTgtS1=0, iCurS1=0, iErrS1=0;

	WORD	wShtInt=0;		// SHT

	static int iShtSVal=0;

	static BYTE gbMpIris=0xff;
	static BYTE gbMpDcMode=0xff;
	static UINT gnMnLensChg=0;

	if((gbMpIris!=UP(Iris))||(gbMpDcMode!=UP(DcMode))) gnMnLensChg = (AE_FUNC_FPS<<2);		// IRIS 설정 메뉴 변경 시 4초 동안 IRIS Center 업데이트 하지 않음
	else if(gnMnLensChg) gnMnLensChg--;
	gbMpIris = UP(Iris);
	gbMpDcMode = UP(DcMode);

	TgtChg();	// ErrMgn() & TgtGet() 호출전에 먼저 실행되야 함

	//GrpAe(GA_ERR_NIGHT_CNT) = giCurAgc;

	AeODM();

	gnLSflag = LS_FLGr;

	//--------------------------------------------------------------------------------
	// Freq adjust
	//FreqAdjust();

	// LED ON/OFF
#if (model_Led!=2)
	if (((UP(Tdn)==UP_TDN_NIGHT)||(UP(Tdn)==UP_TDN_EXTERN)) &&	UP(LedOn) && (gnTdnChkOut==TDN_NIT))	AE_LED_ON
	else 																								AE_LED_OFF
#endif

	//HLMASK_ONw(!ParAe(PA_SAT_OFF));
	//HLMASK_THw(AE2_CLIPr>>2);

	const BYTE bWdrOn = (gbWdrOn||gbWdrOnBuf2);

	//static BYTE bWdrOffCnt = 0;
	//if((gbWdrOnBuf==UP_OFF) && (gbWdrOnBuf2==UP_ON)) bWdrOffCnt = AE_FUNC_FPS>>1;
	//else if(bWdrOffCnt) bWdrOffCnt--;

	static BYTE bWdrOnCnt = 0;
	if((bWdrOn==UP_ON) && (gbWdrOnBuf==UP_OFF)) bWdrOnCnt = MAX(gbSensShtDly, gbSensAgcDly)+1;
	else if(bWdrOnCnt) bWdrOnCnt--;

	//extern BYTE gbAdnr_WaitCnt;
	#define gbAdnr_WaitCnt	3		// EN673에서 DNR ON 시 3 frame 후에 실행
	if(UP(BackLight) == UP_BL_WDR || gbWdrOn || gbWdrOnBuf2 || (gbAdnr_WaitCnt<3)/*gbAdnr_Excute*//*DNR3D_ONr*/) {
		gbDssRatioLmt = 1;
	}
	else {
		switch (UP(Dss)) {
			case UP_DSS_OFF	 :	gbDssRatioLmt = 1; break;	// x1 (fix)
			case UP_DSS_X2	 :	gbDssRatioLmt = 2; break;	// x2
			case UP_DSS_X4	 :	gbDssRatioLmt = 4; break;	// x4
			case UP_DSS_X8	 :	gbDssRatioLmt = 8; break;	// x8
			case UP_DSS_X16	 :	gbDssRatioLmt =16; break;	// x16
			case UP_DSS_X32  :	gbDssRatioLmt =32; break;	// x32
			case UP_DSS_X64  :	gbDssRatioLmt = FPS_HI ? 64 : 32; break;	// x64
		}
	}

	/*const int*/ iTgtMax = TgtMaxGet(0,0);

	/*const int*/ iTgtMin = (bWdrOn) ? iTgtMax : TgtMinGet(iTgtMax, UP(BrightnessMin)) ;


	const int iErrMgn = ErrMgn(AE_ERR_MGN, AE_ERR_MGN>>1, AE_ERR_MIN, 16);

#ifdef ENABLE_PA
	ParAe(PA_AGC_GAIN_MIN_BIT) = UP(SmallSpeed);
#else
	int iShtUpSpd, iShtDnSpd, iIrsUpSpd, iIrsDnSpd;
#endif


	/*clear_csr(mstatus, MSTATUS_MIE);
	const ULONG sta_time = rdcycle();*/

#if 1	// 0인 경우 실제 Code Size 20 byte 증가, 실행 시간 50 tick(cucle) 증가, add+shift 연산보다 정수 곱 연산이 더 빠름
	const int ShtBrtSpdMin = UP(ShtBrtSpeed) * 10;
	const int ShtDrkSpdMin = UP(ShtDrkSpeed) * 100;
	const int AgcBrtSpdMin = UP(AgcBrtSpeed) * 5;
	const int AgcDrkSpdMin = UP(AgcDrkSpeed) * 50;
#else
	const int ShtBrtSpdMin = (UP(ShtBrtSpeed)<<3) + (UP(ShtBrtSpeed)<<1);							// 8a + 2a = 10a
	const int ShtDrkSpdMin = (UP(ShtDrkSpeed)<<6) + (UP(ShtDrkSpeed)<<5) + (UP(ShtDrkSpeed)<<2);	// 64a + 32a + 4a = 100a
	const int AgcBrtSpdMin = (UP(AgcBrtSpeed)<<2) + (UP(AgcBrtSpeed));								// 4a + a = 5a
	const int AgcDrkSpdMin = (UP(AgcDrkSpeed)<<5) + (UP(AgcDrkSpeed)<<4) + (UP(AgcDrkSpeed)<<1);	// 32a + 16a + 2a = 50a
#endif

	ParAe(PA_SHT_UP_SPEED)     = UP(ShtSpeed)*ShtBrtSpdMin;
	ParAe(PA_SHT_DOWN_SPEED)   = UP(ShtSpeed)*ShtDrkSpdMin;
	ParAe(PA_AGC_UP_SPEED)     = UP(AgcSpeed)*AgcBrtSpdMin;
	ParAe(PA_AGC_DOWN_SPEED)   = UP(AgcSpeed)*AgcDrkSpdMin;

	ParAe(PA_IRS_OPEN_SPEED)   = UP(IrsSpeed)*40/*100*/+24;
	ParAe(PA_IRS_CLOSE_SPEED)  = UP(IrsSpeed)*40/*100*/+24;

	/*const ULONG end_time = rdcycle();
	set_csr(mstatus, MSTATUS_MIE);
	const ULONG prc_time = (sta_time < end_time) ? end_time - sta_time : (0xFFFFFFFFFFFFFFFF - sta_time) + end_time + 1;

	#define IIG					13
	#define IKG					7
	#define IIRI(Y,X)			Y = (X)<<(IIG+IKG)	// 최대값 = 2047 = 2^11 (11 = 32 - IIG - IKG - 1(sign)), UINT형은 4095
	#define IIRR(Y,K,X,YP)		Y = (K)*((X)<<IIG) + ((1<<IKG)-(K))*(YP>>IKG)
	#define IIROu(Y)			(((Y)>>(IIG+IKG)) + (((Y)>>(IIG+IKG-1))&1))
	GRP0 = MIN(4095, prc_time);
	static UINT nPrcTimeIIR = 0;
	IIRR(nPrcTimeIIR,2,GRP0,nPrcTimeIIR);
	GRP2 = IIROu(nPrcTimeIIR);
	GRP1 = PAR00;
	UartTxGrp();*/


	LtPosThSet();

// -------------------------------------------------------------------------------
// Input condition adj

	if(gbWdrOn){	// WDR mode --------------------------------------------------

		// CurWDR() 실행전에 gnLSflag 설정되어야 함
		CurWDR(	&iCur, &iTgt, &iErr, nAe2Sum1, nAe2ClipLvl, nAe2ClipCnt, nAe2PxCnt,
				&iCurS1, &iTgtS1, &iErrS1, nAe3Sum2, nAe3ClipLvl, nAe3ClipCnt, nAe3PxCnt);

#if model_WDR_ROI
		ROI_WDR();
#endif

	}
	else {		// Normal Mode -------------------------------------------------------

		const int iAe2ClipCnt = (UP(BackLight)==UP_BL_HLC) ? LibUtlInterp1D(UP(HlMaskThrs)+8, 0, 28, 0, nAe2ClipCnt) : (int)nAe2ClipCnt ;

		iCur = udiv4x((nAe2SlicCnt*nAe2SlicLvl) + nAe2Sum1 + (iAe2ClipCnt*nAe2ClipLvl), nAe2PxCnt, 0);

		iTgt = TgtGet(iTgtMax, iTgtMin, gbAeStg);

		iErr = iTgt - iCur;

		// Set Slice
		const int iSliceLv = CurBg(iTgt, nAe3SlicLvl, nAe3SlicCnt, nAe3ClipLvl, nAe3ClipCnt, nAe3Sum1, nAe3PxCnt, UP(AE_TGT_OFST));

		AE2_SLICE0w(ParAe(PA_SLICE) ? (int)ParAe(PA_SLICE) : iSliceLv);
		AE3_SLICE0w(ParAe(PA_SLICE) ? (int)ParAe(PA_SLICE) : iSliceLv);

		// Set Clip
		const int iClipLvD = MIN(iTgt + UP(AE_CLIP_OFST), 0x3ff);
		const int iClipLv = ClipGet(iClipLvD);

		CurSat(nAe2ClipLvl, nAe2ClipCnt, nAe2Sum2, (ParAe(PA_TGT_NIGHT_OFS) ? (int)ParAe(PA_TGT_NIGHT_OFS) : 64));

		AE2_CLIP0w(ParAe(PA_CLIP) ? (int)ParAe(PA_CLIP) : iClipLv);
		AE3_CLIP0w(ParAe(PA_CLIP) ? (int)ParAe(PA_CLIP) : iClipLvD);
	}

	//GrpAe(GA_CUR) = iCur;
	//GrpAe(GA_CLIP_CNT_OFF) = iClipCntOff;
	//GrpAe(GA_CLIP_CNT_ON) = iClipCntON;
	//GrpAe(GA_CLIP_CNT) = nAe2ClipCnt;
	//GrpAe(GA_CUR_NIGHT) = iCurSpot;
	//GrpAe(GA_ERR_NIGHT) = iErrSpot;


#if 1//model_ISP_Save == 1
	static BYTE bInitAe = /*(model_ISP_Save == 1) ? 3 :*/ 2;
	if(bWdrOnCnt) bInitAe = bWdrOnCnt;
	if(bInitAe) {
		UartTxStrDec("Ae X:", iCur, 4);
		//bInitAe--;
		//giCurAgc = AGC2POS(giAgcVal);
		//AeSHT0(ParAe(PA_SHT_MANUAL) ? (int)ParAe(PA_SHT_MANUAL) : giShtVal, (UP(Shutter)==UP_SHUT_MNL), (gnAeVtw>>UP(ShutSpd)) );
		//AeAGC0(ParAe(PA_AGC_MANUAL) ? (int)ParAe(PA_AGC_MANUAL) : ((gbAeStg==AE_STG_SHT) ? gbVHssAgc : 0) + giAgcVal);
		//goto AeMonitoring;
	}
	else {
		static BYTE bDispCnt = 5/*10*/;
		if(bDispCnt) {
			UartTxStrDec("Ae O:", iCur, 4);
			bDispCnt--;
		}
	}
#else
	#define bInitAe 0
#endif

// -------------------------------------------------------------------------------
// Status mode
	if(gbWdrOn&&(gnLSflag==AeLONG)){	// ============================================ // Long time, Short proc, Long apply

		// apply control val -------------- // Long
		wShtInt = AeSHT0(ParAe(PA_SHT_MANUAL) ? (int)ParAe(PA_SHT_MANUAL) : giShtVal, (UP(Shutter)==UP_SHUT_MNL), (gnAeVtw>>UP(ShutSpd)) );
		AeAGC0(ParAe(PA_AGC_MANUAL) ? (int)ParAe(PA_AGC_MANUAL) : ((gbAeStg==AE_STG_SHT) ? gbVHssAgc : 0) + giAgcVal);
		//AeIRS(UP(DefDet) ? AE_IRS_CLS_Max : ((model_Iris_DC) ? 0 : AE_IRS_STAT_Max));

		//GrpAeF(GA_WDR_SHORT_POS) = gfAePosWdrflt;
		//GrpAe(GA_WDR_LONG_POS) = giShtValBuf;
		//GrpAe(GA_WDR_HSS_T_POS) = gnAeHssTVal;
		//GrpAe(GA_WDR_SHORT_LMT_POS) = iWdrSLmt;
		//GrpAe(GA_WDR_AGC_WGT) = iWdrAgcWgt;
	}
	else {	// ======================================================================== // Short time, Long proc, Short apply

		if(ParAe(PA_SHT_MANUAL)) {
			SHT_MAX = (int)ParAe(PA_SHT_MANUAL);
			SHT_MIN = SHT_MAX;
			SHT_DBL_MAX = SHT_MAX;
		}
		else if(UP(Shutter)==UP_SHUT_MNL) {
			SHT_MAX = (int)(gnAeVtw>>UP(ShutSpd));
			SHT_MIN = SHT_MAX;
			SHT_DBL_MAX = SHT_MAX;
		}
		else if(UP(Shutter)==UP_SHUT_FLICKER) {
			SHT_MAX = (UP(SysFreq)==UP_SYSFREQ_60) ? (gnAeVtw*30)/50 : (gnAeVtw*25)/60;
			SHT_MIN = SHT_MAX;
			SHT_DBL_MAX = SHT_MAX;
		}
		else {	// UP(Shutter)==UP_SHUT_AUTO
			SHT_MIN = gnAeVtw>>gbUpShtMin;
			SHT_MAX = gnAeVtw>>gbUpDeblurMin;
			SHT_DBL_MAX = gnAeVtw>>gbUpShtMax;
		}

		if(ParAe(PA_AGC_MANUAL)) {
			AGC_MAX = ParAe(PA_AGC_MANUAL);
			AGC_MIN = ParAe(PA_AGC_MANUAL);
			AGC_DBL_MAX = AGC_MIN;
		}
		else {
			gnAeAgcTVal = AE_GAIN_TOTAL + AE_GAIN_TOTAL2;	// gnAeAgcTVal -> gnAgcLim -> TDN에서 사용

			const int iAgcLmt = (UP(Agc)>=255) ? (int)gnAeAgcTVal :
												 (int)((((gnAeAgcTVal-AE_GAIN_TGT_OFST)*UP(Agc))/255)+AE_GAIN_TGT_OFST);
			AGC_MAX = POS2AGC(iAgcLmt);

			//AGC_MIN = (bWdrOn) ? 0 : ((AGC_MAX*UP(AgcMin))>>5);
			const int iAgcMin = (UP(AgcMin)>=255) ? (int)(gnAeAgcTVal>>2) :
													(int)(((gnAeAgcTVal)*UP(AgcMin))/(255<<2));
			AGC_MIN = (bWdrOn) ? 0 : POS2AGC(iAgcMin);
			AGC_MIN = MIN(AGC_MIN, AGC_MAX);


			if(SHT_MAX < SHT_DBL_MAX) {
				AGC_DBL_MAX = POS2AGC(((SHT_DBL_MAX-SHT_MAX)<<(7-2))/UP(AE_DEBLUR_GAIN));	// 7:UP(AE_DEBLUR_GAIN)=128 -> x1 , 2:4SHT=1AGCPOS,		Sensor AGC
				if(AGC_DBL_MAX > AGC_MAX) AGC_DBL_MAX = AGC_MAX;
			}
			else {
				AGC_DBL_MAX = AGC_MIN;
			}
		}


		const BYTE bDeblurOn = (SHT_MAX < SHT_DBL_MAX) && (AGC_MIN < AGC_DBL_MAX);
		const int iShtMax_Agc = (bDeblurOn) ? SHT_DBL_MAX : SHT_MAX ;
		const int iAgcMin_Agc = (bDeblurOn) ? AGC_DBL_MAX : AGC_MIN ;
		const int iShtMax_Dss = (gbDssRatioLmt<2) ? gnAeVtw : (gnAeVtw*gbDssRatioLmt)-1;
		const BYTE bAgcOn = (iAgcMin_Agc < AGC_MAX);
		const BYTE bShtOn = (SHT_MIN < SHT_MAX);
		const BYTE bIrsOn = (UP(Iris)!=UP_LENS_MNL) && (ParAe(PA_IRS_MANUAL)==0);
		const BYTE bDssOn = (UP(Shutter)!=UP_SHUT_MNL) && (iShtMax_Agc < iShtMax_Dss);
		const BYTE bTgtOn = (bWdrOn==UP_OFF);
		//const BYTE bEstOn = (bWdrOn==UP_OFF) && (bWdrOffCnt==0);
		const BYTE bEstOn = (bWdrOnCnt==0);

		if(gbWdrOnBuf2==UP_ON) SetAeDevDly(gbSensShtDly>>1, gbSensAgcDly>>1);
		else				   SetAeDevDly(gbSensShtDly, gbSensAgcDly);

		if(bInitAe) goto AeCtrl;

	// Status manage ----------------------------------------------------------------------------------------------------------------------------
		//if(gbWdrOn==0) GrpAe(GA_ERR_DAY_ORI) = iErr;

		if(gbAeStg==AE_STG_IRS) {
			if(bIrsOn) {
				iErr = ErrNor(iErr, iCur, 0, 1, 0, 0);
			}
			else {
				iErr = ErrNor(iErr, iCur, 0, 0, 1, 0);
				giIrsPos = AE_IRS_STAT_Max;
				gbAeStg = AE_STG_SHT;
			}
		}
		else {
			iErr = ErrNor(iErr, iCur, 0, 0, 1, 0);
		}

		SatCtrl(AE_SAT_OFF, &iErr, iErrMgn, nAe2ClipCnt);

		const int iIrsSpd = (iErr<0) ? (ParAe(PA_IRS_CLOSE_SPEED) ? ParAe(PA_IRS_CLOSE_SPEED) : 1024) : (ParAe(PA_IRS_OPEN_SPEED) ? ParAe(PA_IRS_OPEN_SPEED) : 1024);
		const int iShtSpd = (iErr<0) ? (ParAe(PA_SHT_DOWN_SPEED)  ? ParAe(PA_SHT_DOWN_SPEED)  : 1200) : (ParAe(PA_SHT_UP_SPEED)   ? ParAe(PA_SHT_UP_SPEED)   : 200 );
		const int iAgcSpd = (iErr<0) ? (ParAe(PA_AGC_DOWN_SPEED)  ? ParAe(PA_AGC_DOWN_SPEED)  : 200 ) : (ParAe(PA_AGC_UP_SPEED)   ? ParAe(PA_AGC_UP_SPEED)   : 20  );

#ifdef AE_EST_AUTO
	#define AE_EST_AUTO_ON_VAL	AE_EST_AUTO
#else
	#define AE_EST_AUTO_ON_VAL	0
#endif

		//extern BYTE bErrNightCnt;
		//extern BYTE bErrOfsCnt;
		//static BYTE bErrNightCnt1d = 0xff;
		//static BYTE bErrOfsCnt1d   = 0xff;
		const BYTE bDlyOn = 0;//bWdrOffCnt/*0*/;//(bErrNightCnt1d != bErrNightCnt) || (bErrOfsCnt1d != bErrOfsCnt);
		//if((ParAe(PA_STA)==(!AE_EST_AUTO_ON_VAL)) && bEstOn && ((bErrNightCnt1d != bErrNightCnt) || (bErrOfsCnt1d != bErrOfsCnt))) bEstOn = 0;
		//bErrNightCnt1d = bErrNightCnt;
		//bErrOfsCnt1d = bErrOfsCnt;

		//static BYTE bPA_STA = -1;	// AE Test
		//if(bPA_STA != ParAe(PA_STA)) {
		//	if(ParAe(PA_STA)==(!AE_EST_AUTO_ON_VAL)) TxStr("*Est Auto ON", 0, 0);
		//	else TxStr("*Est Auto OFF", 0, 0);
		//	bPA_STA = ParAe(PA_STA);
		//}

		//if (/*bWdrOn ||*/ (bEstOn==0)) {
		//	iShtSpd >>= NO_EST_SPD_DOWN_BIT;
		//	iAgcSpd >>= NO_EST_SPD_DOWN_BIT;
		//}

		const BYTE BRIGHT_ON = iErr > iErrMgn;
		const BYTE DARKEN_ON = iErr < -iErrMgn;

		if(gbWdrOn==0) GrpAe(GA_ERR_DAY_ORI) = 0;

		switch(gbAeStg) {
			case AE_STG_IRS :	// IRIS
				IRS_RUN(iErr, iErrMgn, iIrsSpd);
				SHT_SET(SHT_MIN);
				AGC_SET(AGC_MIN);

				if(BRIGHT_ON && giIrsPos>=AE_IRS_STAT_Max && gbIrsDly==0) {
					if(bShtOn) gbAeStg = AE_STG_SHT;
					else	   gbAeStg = AE_STG_TGT;
				}
				break;
			case AE_STG_SHT :	// SHT
				ShtCtrl(0, iErr, iErrMgn, iShtSpd, SHT_MIN, SHT_MAX, bEstOn, bDlyOn, 1, 0/*iShtManual*/);
				//AGC_SET(AGC_MIN);		// ShtCtrl()의 abAgcOn=1로써 AGC까지 제어하므로 주석처리
				AeTxGrpFnc();

				if(DARKEN_ON && giShtVal<=SHT_MIN && gbShtDly==0) {
					if(bIrsOn) gbAeStg = AE_STG_IRS;
				}
				if(BRIGHT_ON && giShtVal>=SHT_MAX && gbShtDly==0) {
					gbAeStg = AE_STG_TGT;
				}
				break;
			case AE_STG_TGT :
				SHT_SET(SHT_MAX);
				AGC_SET(AGC_MIN);

				if(DARKEN_ON) {	// iErr < 0 : Close Ctrl : AGC -> SHT : iTgt -> iTgtMax
					//iTgtVal++;
					iTgtVal -= MIN(iErr>>1,-1);
					if((bTgtOn==0) || (iTgtVal >= iTgtMax)) {
						iTgtVal = iTgtMax;
						if(bShtOn)		gbAeStg = AE_STG_SHT;
						else if(bIrsOn) gbAeStg = AE_STG_IRS;
					}
				}
				if(BRIGHT_ON) {	// iErr > 0 : Open Ctrl : SHT -> AGC : iTgt -> iTgtMin
					//iTgtVal--;
					iTgtVal -= MAX(iErr>>1,1);
					if((bTgtOn==0) || (iTgtVal <= iTgtMin)) {
						iTgtVal = iTgtMin;
						if(bDeblurOn)	gbAeStg = AE_STG_DBL;
						else if(bAgcOn) gbAeStg = AE_STG_AGC;
						else if(bDssOn) gbAeStg = AE_STG_DSS;
					}
				}
				break;
			case AE_STG_DBL :
				ShtCtrl(0, iErr, iErrMgn, ((iShtSpd * (11 + UP(AgcSpeed)/*10 + bMpAgcSpeed*/) / 10)>>2)+1, SHT_MAX, SHT_DBL_MAX, bEstOn, bDlyOn, 0, 0/*iShtManual*/);
				const int iDeblurAgc = ((bDeblurOn) ? (AGC_DBL_MAX - AGC_MIN) * (giShtVal - SHT_MAX) / (SHT_DBL_MAX - SHT_MAX) : 0) + AGC_MIN;
				AGC_SET(CLAMP(iDeblurAgc, AGC_MIN, AGC_DBL_MAX));

				if(DARKEN_ON && giShtVal<=SHT_MAX && gbShtDly==0 && gbAgcDly==0) {
					gbAeStg = AE_STG_TGT;
				}
				if(BRIGHT_ON && giShtVal>=SHT_DBL_MAX && gbShtDly==0 && gbAgcDly==0) {
					if(bAgcOn)		gbAeStg = AE_STG_AGC;
					else if(bDssOn)	gbAeStg = AE_STG_DSS;
				}
				break;
			case AE_STG_AGC :	// AGC

				if(gbWdrOn==0) GrpAe(GA_ERR_DAY_ORI) = iErr;

				SHT_SET(iShtMax_Agc);
				AgcCtrl(iErr, iErrMgn, iAgcSpd, iAgcMin_Agc, AGC_MAX, /*0*/bEstOn, bDlyOn, 0/*ParAe(PA_AGC_MANUAL)>0*/, 0/*ParAe(PA_AGC_MANUAL)*/);
				AeTxGrpFnc();

				if(DARKEN_ON && giAgcVal <= iAgcMin_Agc && gbAgcDly==0) {
					if(bDeblurOn) gbAeStg = AE_STG_DBL;
					else		  gbAeStg = AE_STG_TGT;
				}
				if(BRIGHT_ON && giAgcVal >= AGC_MAX && gbAgcDly==0) {
					if(bDssOn) gbAeStg = AE_STG_DSS;
				}
				break;
			case AE_STG_DSS :	// AGC
				if(bDssOn) ShtCtrl(0, iErr, iErrMgn, iShtSpd, iShtMax_Agc, iShtMax_Dss, bEstOn, bDlyOn, 0, 0/*iShtManual*/);
				AGC_SET(AGC_MAX);

				if((bDssOn==0) || (DARKEN_ON && giShtVal <= iShtMax_Agc && gbShtDly==0)) {
					if(bAgcOn)		   gbAeStg = AE_STG_AGC;
					else if(bDeblurOn) gbAeStg = AE_STG_DBL;
					else			   gbAeStg = AE_STG_TGT;
				}
				break;
		}

		//GrpAe(GA_ERR_DAY) = AGC_DBL_MAX;
		//if(gbWdrOn==0) GrpAe(GA_ERR_DAY_ORI) = iErr;
		//GrpAe(GA_ERR) = iErr;
		GrpAe(GA_ERR) = (AE_SAT_OFF) ? iCur : iCurBg ;
		//GrpAe(GA_TGT_MAX) = iTgt;
		//GrpAe(GA_TGT_MIN) = iTgtSpotBg;
		//GrpAe(GA_SHT_DLY) = gbShtDly;
		//GrpAe(GA_SHT_VAL) = giShtVal;
		//GrpAe(GA_AGC_VAL) = giAgcVal/*iErr*//*giShtPos*/;
		//GrpAe(GA_ERR_CHT_PRE) = iCurSpotEst>>IIG;
		//GrpAe(GA_ERR_CHT_PRE) = giIrsPos;
		//GrpAe(GA_ERR_NIGHT) = gbAeStg*100 + gbIrsDly*1000 + gbAgcDly*10 + gbShtDly;	// AE Test
		//GrpAe(GA_ERR_NIGHT) = nAe2ClipCnt;

		// ------------------------------------------------------------------------------------------------------------ //
AeCtrl:
		giCurAgc = AGC2POS(giAgcVal);

		if (bWdrOn) {												// WDR Short /or fill Short to Long because turn off hunting

			const int iShtSpd = (iErrS1<0) ? (ParAe(PA_SHT_DOWN_SPEED)  ? ParAe(PA_SHT_DOWN_SPEED)  : 1200) : (ParAe(PA_SHT_UP_SPEED)   ? ParAe(PA_SHT_UP_SPEED)   : 200 );

			//iShtSpd = LibUtlInterp1D( CLAMP(giCurAgc, UP(AE_WDR_ON_AGC), AE_WDR_OFF_AGC), UP(AE_WDR_ON_AGC), AE_WDR_OFF_AGC, iShtSpd, iShtSpd>>3);	// Sensor AGC
			//if(iErrS1 < (-iErrMgn-1)) iErrS1 = -iErrMgn-1 - LibUtlInterp1D( CLAMP(giCurAgc, UP(AE_WDR_ON_AGC), AE_WDR_OFF_AGC), UP(AE_WDR_ON_AGC), AE_WDR_OFF_AGC, (iErrS1+iErrMgn+1)>>0, (iErrS1+iErrMgn+1)>>10);	// Sensor AGC
			GrpAe(GA_ERR_DAY_ORI) = iErrS1;

#if 1
		#if (model_Sens==SENS_IMX327) || (model_Sens==SENS_OV2718)
			#define WDR_SHORT_MIN1	/*8*/ParAe(PA_WDR_SHORT_MIN1)	// 너무 빠른 Shutter에서는 AGC를 써도 SUM2가 크게 변화하여 헌팅이 발생 -> 1에서 8로 변경
			#define WDR_SHORT_MIN2	40
			const int iShtSValMax = (giShtVal)>>1;					// WDR Short limit 50% of Long
		#else
			#define WDR_SHORT_MIN1	20
			#define WDR_SHORT_MIN2	40
			const int iShtSValMax = ((giShtVal<<1) + giShtVal)>>2;	// WDR Short limit 75% of Long
		#endif

			const int iWdrSLmt = LibUtlInterp1D( CLAMP(giShtVal, (int)(gnAeVtw>>1)/*250*/, (int)(gnAeVtw-1)), gnAeVtw>>1/*250*/, gnAeVtw-1, WDR_SHORT_MIN1, WDR_SHORT_MIN2);	// Sht limit

			const int iShtSMin = MIN(iShtSValMax, iWdrSLmt);

			//if(iShtSValMax<iWdrSLmt && iErrS1 < (-iErrMgn-1)) iErrS1 = (-iErrMgn-1) - ((iErrS1+iErrMgn+1)>>3);

			// ShtCtrl(1, ...)에서 abAgcOn를 1로 사용하기 위해서는 AgcCtrl()의 giAgcVal도 2개가 있어야 함 -> 현재는 gbVHssSAgc + giAgcVal(Long Shutter) 로 동작 -> WDR 사용 시 AGC_MIN 적용하지 않음
			// 너무 빠른 Shutter에서는 AGC를 써도 SUM2가 크게 변화하여 헌팅이 발생 -> iErrMgn에 +4 적용
			if(bInitAe==0) ShtCtrl(1, iErrS1, iErrMgn+4, (iShtSpd/*>>NO_EST_SPD_DOWN_BIT*/), MAX(iShtSMin,1), giShtVal-1, 0, 0, 0, 0);	// over than short limit, under than long

			const int iWdrAgcWgt = LibUtlInterp1D( CLAMP(giCurAgc, UP(AE_WDR_ON_AGC), AE_WDR_OFF_AGC), UP(AE_WDR_ON_AGC), AE_WDR_OFF_AGC, AE_WDR_MAX_WGT, UP(AE_WDR_MIN_WGT));	// Sensor AGC
			iShtSVal = WdrAgcWgt((bInitAe==0) && (UP(AE_WDR_ON_AGC) < giCurAgc), iWdrAgcWgt, 30, 18);	// CAUTION ! -> iShtSVal 와 gbVHssSAgc 는 비동기

			if(iShtSVal > iShtSValMax) iShtSVal = iShtSValMax;//SHT_SET1(iShtSValMax);
#else
			iShtSVal = SHT_SET1((giShtVal>1) ? giShtVal-1 : 1);
#endif

			wShtInt = AeSHT0(ParAe(PA_SHT_MANUAL) ? (int)ParAe(PA_SHT_MANUAL) : iShtSVal, 0/*(UP(Shutter)==UP_SHUT_MNL)*/, 0/*(gnAeVtw>>UP(ShutSpd))*/ );
			AeAGC0(ParAe(PA_AGC_MANUAL) ? (int)ParAe(PA_AGC_MANUAL) : (((gbAeStg==AE_STG_SHT)&&(giShtSVal==iShtSVal)) ? gbVHssSAgc : 0) + giAgcVal);
			//AeIRS(UP(DefDet) ? AE_IRS_CLS_Max : ((model_Iris_DC) ? 0 : AE_IRS_STAT_Max));

			AeMon(AE_SAT_OFF, AeLONG, iErrMgn+4, iCurS1, iTgtS1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		}
		else {
			wShtInt = AeSHT0(ParAe(PA_SHT_MANUAL) ? (int)ParAe(PA_SHT_MANUAL) : giShtVal, (UP(Shutter)==UP_SHUT_MNL), (gnAeVtw>>UP(ShutSpd)) );
			AeAGC0(ParAe(PA_AGC_MANUAL) ? (int)ParAe(PA_AGC_MANUAL) : ((gbAeStg==AE_STG_SHT) ? gbVHssAgc : 0) + giAgcVal);
		}


			// ------------------------------------------------------------------------------------------------------------ //
			static int iShtValDly = 0;
			if(iShtValDly < giShtVal) iShtValDly++;
			else					  iShtValDly = giShtVal;

			static int iTgtValDly = 0;
			if(iTgtValDly > iTgtVal) iTgtValDly--;
			else					 iTgtValDly = iTgtVal;


			int iIrsVal = 0;
		#ifdef ENABLE_PA
			if(ParAe(PA_IRS_MANUAL)) {	// for IRIS test
				static BYTE bIrsCnt = 0;
				iIrsVal = (giIrsCent>>6)/*0x270*/ + (int)ParAe(PA_IRS_MANUAL) - 1000;
				if(ParAe(PA_IRS_MANUAL) != 1000 && bIrsCnt==0) bIrsCnt = ParAe(PA_IRS_CNT)+1;

				if(bIrsCnt) bIrsCnt--;
				if(bIrsCnt==0) ParAe(PA_IRS_MANUAL) = 1000;
			} else
		#endif
			{
				// increse open step
				static BYTE iIncreseOpenCnt = 0;
				const int iIncreseOpen = ( ((iTgtMax-iTgtValDly)>>1) + ((bShtOn || bDeblurOn) ? (iShtValDly - SHT_MIN) : 0) + ((gbAeStg==AE_STG_AGC) ? ((giAgcVal - iAgcMin_Agc)<<2) : 0) ) ;
				iIrsVal = (UP(Iris)==UP_LENS_MNL) || (gbAeStg==AE_STG_DSS) ? 0 :
						  giIrsValBuf - iIncreseOpen;	// gbAeStg==AE_STG_IRS 일 때 IRIS를 제어하고, 다른 Stage일 때 IRIS를 0(Open)으로 제어하면 IRIS 다시 제어 시 IRIS가 튀는(?) 현상 발생하므로 increse open step 필요

				if(giIrsValBuf && (gbAeStg!=AE_STG_IRS))
				{
					if(iIncreseOpen) iIncreseOpenCnt++;
					if(iIncreseOpenCnt==(AE_FUNC_FPS>>2)) {	// 초당 4씩 giIrsValBuf 감소 - 너무 빠르게 감소하면 다른 Stage일 때 IRIS를 0(Open)으로 제어하는 것과 동일하게 IRIS가 튀는 현상 발생하므로 천천히 감소해야 함
						iIncreseOpenCnt = 0;
						giIrsValBuf--;
					}
				}
			}

			iIrsVal = CLAMP(iIrsVal, 0, 0x3fe);
			AeIRS(/*UP(DefDet) ? AE_IRS_CLS_Max :*/ iIrsVal);
		//}

		AeMon(AE_SAT_OFF, AeSHORT, iErrMgn, iCur, iTgt, bIrsOn, bShtOn, bTgtOn, bDeblurOn, bAgcOn, bDssOn, AE_IRS_STAT_Max, iShtMax_Agc, iAgcMin_Agc, POS2AGC(gnAeAgcTVal), iShtMax_Dss, UP(Dss));

		// -------------------------------------------------------------------------------
		// Ae State Auto Save
		if(bInitAe) bInitAe--;
#if model_ISP_Save == 1
		//if(bInitAe) bInitAe--;
		else {
			static int iStableCnt = 0;
			static BYTE bSaveDone = 1;

			if(-iErrMgn <= iErr && iErr <= iErrMgn) {
				if(iStableCnt < 0) iStableCnt = 1;
				else if(iStableCnt < 36000) iStableCnt++;		// 36000 = 60FPS * 600sec(=10min)
			}
			else {
				if(iStableCnt > 0) iStableCnt = -1;
				else if(iStableCnt > -36000) iStableCnt--;		// 36000 = 60FPS * 600sec(=10min)
			}

			const UINT nAE_FUNC_FPS = gbWdrOn ? AE_FUNC_FPS>>1 : AE_FUNC_FPS;

			if((iStableCnt < -(int)(nAE_FUNC_FPS>>2)) && (bSaveDone == 1)) {	// >>2 = 0.25sec
				bSaveDone = 0;
				TxStr("AE error occurred", 0, 0);
			}
			if((iStableCnt > (int)(nAE_FUNC_FPS<<1)) && (bSaveDone == 0)) {	// <<1 = 2sec
				bSaveDone = 1;

				if(ABSDIFF(gUdAeStat->bErrNightCnt, bErrNightCnt) >= 2) gbUsrDataSaveChk = 1;
				if(ABSDIFF(gUdAeStat->bErrOfsCnt, bErrOfsCnt) >= 2) gbUsrDataSaveChk = 1;
				if(ABSDIFF(gUdAeStat->bAeStg, gbAeStg) >= 1) gbUsrDataSaveChk = 1;

				const int iIrsCent = cat2(gUdAeStat->bIrsCent);
				if(ABSDIFF(iIrsCent, giIrsCent) >= 4) gbUsrDataSaveChk = 1;

				const int iIrsVal = cat2(gUdAeStat->bIrsValBuf);
				if(ABSDIFF(iIrsVal, giIrsValBuf) >= 4) gbUsrDataSaveChk = 1;

				const int iShtVal = cat2(gUdAeStat->bShtVal);
				const int iShtDif = (iShtVal <= 20) ? 1 : (iShtVal>>4);			// >>4 => 1/16 => 6.25%
				if(ABSDIFF(iShtVal, giShtVal) >= iShtDif) gbUsrDataSaveChk = 1;

				//const int iTgtVal0 = cat2(gUdAeStat->bTgtVal);
				//if(ABSDIFF(iTgtVal0, iTgtVal) >= 4) gbUsrDataSaveChk = 1;

				const int iAgcVal = cat2(gUdAeStat->bAgcVal);
				if(ABSDIFF(iAgcVal, giAgcVal) >= 4) gbUsrDataSaveChk = 1;		// 4 : need normalization ?

				const int iShtSVal = cat2(gUdAeStat->bShtSVal);
				const int iShtSDif = (iShtSVal <= 20) ? 1 : (iShtSVal>>4);		// >>4 => 1/16 => 6.25%
				if(ABSDIFF(iShtSVal, giShtSVal) >= iShtSDif) gbUsrDataSaveChk = 1;

				// AWB 변화에 대한 SAVE CHK필요 !!

				if(gbUsrDataSaveChk) {
					gUdAeStat->bIrsCentL = giIrsCent&0xff;
					gUdAeStat->bIrsCentH = (giIrsCent>>8)&0xff;
					gUdAeStat->bIrsValBufL = giIrsValBuf&0xff;
					gUdAeStat->bIrsValBufH = (giIrsValBuf>>8)&0xff;
					gUdAeStat->bShtValL = giShtVal&0xff;
					gUdAeStat->bShtValH = (giShtVal>>8)&0xff;
					gUdAeStat->bTgtValL = iTgtVal&0xff;
					gUdAeStat->bTgtValH = (iTgtVal>>8)&0xff;
					gUdAeStat->bAgcValL = giAgcVal&0xff;
					gUdAeStat->bAgcValH = (giAgcVal>>8)&0xff;
					gUdAeStat->bShtSValL = giShtSVal&0xff;
					gUdAeStat->bShtSValH = (giShtSVal>>8)&0xff;

					gUdAeStat->bErrNightCnt = bErrNightCnt;
					gUdAeStat->bErrOfsCnt = bErrOfsCnt;
					gUdAeStat->bAeStg = gbAeStg;

					//TxStrDec("ErrNightCnt:", gUdAeStat->bErrNightCnt, 2);
					//TxStrDec("ErrOfsCnt:", gUdAeStat->bErrOfsCnt, 2);

					TxStr("AE state:Save done", 0, 0);
				}
				else TxStr("AE state:No need to save", 0, 0);
			}

			GrpAe(GA_ERR) = iStableCnt + (gbUsrDataSaveChk<<8);
		}
#endif

	}

	//GrpAe(GA_ERR_DAY) = iTgtVal/*giShtVal*/;
	//GrpAe(GA_ERR_NIGHT_ORI) = iTgtMin/*iTgtValDly*//*iShtValDly*/;
	//GrpAe(GA_IRS_VAL) = iIrsVal;
	//GrpAe(GA_ERR_CHT_PRE) = /*iIrsVal*/giShtVal;
	//GrpAe(GA_AGC_VAL_PRE) = /*giIrsPos*/iShtSVal/*giAgcVal*/;// + giShtVal - 125;

// -------------------------------------------------------------------------------
// WDR Control
	if(WdrCtrl(&wShtInt))
	{
		//if (gbWdrOnBuf2!=gbWdrOnBuf3) {SYNC_UPw(1); /*TxStr("sync_up_", 1,  "\n");*/}	// 141031 SMJ : 1080p30 WDR OFF image crack

		WDR_ONw(0);

		WDR_GAINw(0x100);

		TMG_ONw(0);

		//gwWdrCnt = (gwWdrCnt==0 ) ? 0  : gwWdrCnt-1;

		iShtSVal = SHT_SET1((giShtVal>1) ? giShtVal-1 : 1);
	}

// -------------------------------------------------------------------------------
	giErr2d = giErr1d;
	giErr1d = iErr;

// -------------------------------------------------------------------------------
// Monitoring
//AeMonitoring:
	if(gbWdrOn){
		DebugDisp2(gbAeDebugOn, Dec, "Tgt    ",  3, AE_DEBUG_X, iTgt, 4)
		DebugDisp2(gbAeDebugOn, Dec, "Cur    ",  4, AE_DEBUG_X, iCur, 4)

		DebugDisp2(gbAeDebugOn, Dec, "SliceLv",  5, AE_DEBUG_X, nAe2SlicLvl, 4)
		DebugDisp2(gbAeDebugOn, Dec, "ClipLv ",  6, AE_DEBUG_X, nAe2ClipLvl, 4)

		if (gnLSflag==AeSHORT) {
				DebugDisp2(gbAeDebugOn, Dec, "WdrLtgt", AE_WDR_DEBUG_Y+0/*17*/, AE_DEBUG_X, iTgt, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrLcur", AE_WDR_DEBUG_Y+1/*18*/, AE_DEBUG_X, iCur, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrSsht", AE_WDR_DEBUG_Y+5/*22*/, AE_DEBUG_X, wShtInt, 4)

				DebugDisp2(gbAeDebugOn, Dec, "WdrStgt", AE_WDR_DEBUG_Y+2/*19*/, AE_DEBUG_X, iTgtS1, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrScur", AE_WDR_DEBUG_Y+3/*20*/, AE_DEBUG_X, iCurS1, 4)
		} else {
				//DebugDisp2(gbAeDebugOn, Dec, "WdrStgt", AE_WDR_DEBUG_Y+2/*19*/, AE_DEBUG_X, iTgt, 4)
				//DebugDisp2(gbAeDebugOn, Dec, "WdrScur", AE_WDR_DEBUG_Y+3/*20*/, AE_DEBUG_X, iCur, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrLsht", AE_WDR_DEBUG_Y+4/*21*/, AE_DEBUG_X, wShtInt, 4)

		}
	}
	else if ((gbWdrOn==UP_OFF)&&(gbWdrOnBuf/*gbWdrOnBuf2*/==UP_ON)) {	// 191104 KSH gbWdrOnBuf2 -> gbWdrOnBuf
		DispClr(AE_WDR_DEBUG_Y+4, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+5, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+6, AE_DEBUG_X, 7+1+4);
	}
	else {
		DebugDisp2(gbAeDebugOn, Dec, "TgtMax ",  3, AE_DEBUG_X, iTgt, 4)
		DebugDisp2(gbAeDebugOn, Dec, "TgtMin ",  4, AE_DEBUG_X, iTgtSpotBg, 4)
		DebugDisp2(gbAeDebugOn, Dec, "Cur    ",  5, AE_DEBUG_X, (AE_SAT_OFF) ? iCur : iCurBg, 4)
		DebugDisp2(gbAeDebugOn, Dec, "CurSpot",  6, AE_DEBUG_X, iCurSpot, 4)
		DebugDisp2(gbAeDebugOn, Dec, "ClipCnt",  7, AE_DEBUG_X, MIN(9999,nAe2ClipCnt), 4)
		DebugDisp2(gbAeDebugOn, Dec, "ClipLv ",  8, AE_DEBUG_X, nAe2ClipLvl<<2, 4)	// AE_DISP 8BIT
		DebugDisp2(gbAeDebugOn, Dec, "SliceLv",  9, AE_DEBUG_X, nAe2SlicLvl<<2, 4)	// AE_DISP 8BIT
		DebugDisp2(gbAeDebugOn, Dec, "SlicCnt", 10, AE_DEBUG_X, (nAe3SlicCnt<<10)/nAe3PxCnt, 4)
	}

	/*GRP0 = ParAe(PA_SHT_MANUAL);
	GRP1 = iCur;
	GRP2 = iCurBg;
	UartTxGrp();*/

#if 0	// TODO KSH + AE ANTI-SAT.정보 OSD 출력
	if(!AE_SAT_OFF) {
		const UINT OSD_AE_Y = (gbMnDebugFnc) ? 20 : 4;
		DispClr(OSD_AE_Y, 0, 10);
		DispDec(OSD_AE_Y, 0, MIN(9999,nAe2ClipCnt)/101, 2);
		DispDec(OSD_AE_Y, 2, (nAe2ClipLvl<<2) + 1 + (ParAe(PA_TGT_NIGHT_OFS) ? (int)ParAe(PA_TGT_NIGHT_OFS) : 64) - iCurSpot, 4);	// AE_DISP 8BIT

		const int iCurPer = (iCurBg < iTgtSpotBg) ?	(iCurBg - iTgtSpotBg) * 100 / iTgtSpotBg :
													(iCurBg - iTgtSpotBg) * 100 / (iTgt - iTgtSpotBg);
		if(iCurBg > iTgt) {
			DispDec(OSD_AE_Y, 7, iCurPer - 100, 3);
			SETFONTID(OSD_AE_Y, 7, '+');
		} else { DispDec(OSD_AE_Y, 7, iCurPer, 3); }
	}
#endif
}

void ISRT Ae(void)
{
	gbAeDebugOn = (gbMnDebugFnc==1);			// Debug string on condition

	//isp_AeTest(); // TODO KSH + AE test

	//----------------------------------------------------------------------------------------
	const ULONG AeTimeSta = BenchTimeStart();

	AeAdv();

	FontBenchTime(gbMnDebugFnc==1, 15, 0, "Ae   ", AeTimeSta, 6);
	//----------------------------------------------------------------------------------------

	IspAgcCurSet();
}







/* EOF */

