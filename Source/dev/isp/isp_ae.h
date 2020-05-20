/* **************************************************************************
 File Name	: 	isp_ae.h
 Description:	EN675 - Auto Exposure function
 Designer	:	Kim, Sunghoon
 Date		:	19. 7. 2
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#ifndef _ISP_AE_H_
#define	_ISP_AE_H_

#ifdef __ISP_LIB__
#else

	#define	UpAE_TGT_GAIN			36/*0x34*//*0x1a*//*0xd*/															// Normal Brightness target gain
	#define	UpAE_TGT_OFST			20/*0x10*//*0x20*/												// Normal Brightness target offset
	#define	UpAE_CLIP_OFST			0x140														// Normal Brightness Clip level (+ Target offset)

	#define	UpAE_SPEED				128															// Tracking Speed : 0 ~ 128 ~ 255 (x0.25 ~ x1 ~ x4)
	#define UpAE_SPEED_DC_VP		19															// IRIS virtual position control Speed for judge IRIS full open (Slow 9 ~ Fast 30)

	#define	UpAE_DEBLUR_GAIN		128/*100*/															// Deblur shutter increasing gradient	 (line)	// 64 ~ 256 -> 0.5 ~ 2.0

	#define	UpAE_WDR_ON_AGC			200/*300*/															// WDR AGC position for maximum weight
	#define AE_WDR_OFF_AGC			(UP(AE_WDR_ON_AGC)+200/*100*/)									// WDR AGC position for minimum weight
	#define AE_WDR_MAX_WGT			0x100														// WDR maximum weight (100% = 0x100)
	#define	UpAE_WDR_MIN_WGT		0/*0x80*/														// WDR minimum weight

	#define	UpAE_WDR_MOTION_SEL		0x0															// Motion mismatch effect selection	(0 or 1)
	#define	UpAE_WDR_STYLE_SEL		1					// 150802								// WDR Style ( 0 : DR first, 1 : Contrast first)

	#define	AE_WDR_LTGT_GAIN		0x3															// WDR Long Brightness target gain
	#define	UpAE_WDR_LTGT_OFST		0x64/*0x8a*/                                                // WDR Long Brightness target offset
	#define	UpAE_WDR_LTGT_OFST2		96/*0x30*/		        // 150802                               // WDR Long Brightness target offset (UpAE_WDR_STYLE_SEL==1)
	#define	AE_WDR_LCLIP_OFST		0x70/*0x50*/                                                        // WDR Long Brightness Clip level (+ Target offset), EN781 WDR 0x50 -> 0x70

	#define	AE_WDR_STGT_GAIN		AE_WDR_LTGT_GAIN											// WDR Short Brightness target gain
	#define	UpAE_WDR_STGT_OFST		0x220/*0x262*/                                              // WDR Short Brightness target offset
	#define	UpAE_WDR_STGT_OFST2		132/*0xa0*/		        // 150802                       // WDR Short Brightness target offset (UpAE_WDR_STYLE_SEL==1), EN781 WDR 0xa0 -> 0x84
	#define	AE_WDR_SCLIP_OFST		(-0x80)                                                     // WDR Short Brightness Clip level (+ Target offset)

	#define UpAE_WDR_SWGT_L			0x80														// WDR weight adjust for menu Low	 (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2)
	#define UpAE_WDR_SWGT_M         0x80                                                        // WDR weight adjust for menu Middle (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2)
	#define UpAE_WDR_SWGT_H         0x80                                                        // WDR weight adjust for menu High   (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2)

	#define AE_LED_PWR(VAL)			IRIS_CLPHw(VAL);											// LED Power Strength (VAL : 1 ~ 1375)


#endif//__ISP_LIB__


enum {		// gnLSflag
	AeSHORT=0,
	AeLONG
};

enum {		// gbWdrOn (+Lib)
	WDR_OFF=0,
	WDR_FRAME,
	WDR_LINE_2P,
	WDR_LINE_3P
};


extern BYTE gbAeDebugOn;
extern BYTE gbSensUpdate;

extern void FreqAdjust(void);
extern void InitAe(void);
extern void Ae(void);
extern void AeDev(void);
extern void InMode(void);

extern int TgtMinGet(const int aiTgt, const BYTE abV);
extern int TgtMaxGet(const BYTE abWdrOn, const UINT anLSflag);

extern int giCurAgc;
extern int giCurDss;
extern int giCurDssStg;

extern int giErr1d;

extern BYTE gbWdrOn/*,gbWdrOnBuf,gbWdrOnBuf2*/;	// WDR on/off status,  Ae() 에서만 gbWdrOn!=gbWdrOnBuf 으로 변경 여부 체크 가능
extern WORD gwWdrGmk;						// WDR Gamma weight

#define gbWdrOnBuf2		gbWdrOn

enum{
	AE_STG_SHT,	// 0
	AE_STG_IRS,	// 1
	AE_STG_AGC,	// 2
	AE_STG_DSS,	// 3
	AE_STG_DBL,	// 4
	AE_STG_TGT	// 5
};

extern BYTE gbAeStg;
extern void AntiSatStabilizingSet(void);

#define SHT_DBL_MAX		giShtDblMax
#define AGC_DBL_MAX		giAgcDblMax

#define SHT_MIN			giShtMin
#define SHT_MAX			giShtMax

#define AGC_MAX			giAgcMax
#define AGC_MIN			giAgcMin

#define AGC_SET(V)		AgcCtrl(0,0,0,0,0,0,0,1,(V))
#define SHT_SET(V)		ShtCtrl(0, 0, AE_ERR_MGN, 1100, SHT_MIN, SHT_MAX, 0, 0, 0, (V))
#define SHT_SET1(V)		ShtCtrl(1, 0, AE_ERR_MGN, 1100, SHT_MIN, SHT_MAX, 0, 0, 0, (V))
//#define IRS_SET(V)		IrsCtrl(0, 0, 0, 0, 1, (V))

#if 1
#define IRS_RUN(E,M,G)	{ IrsCent((gnMnLensChg ? 0 : E), M, AE_IRS_OPN_Max, AE_IRS_CLS_Max, AE_IRS_STAT_Max);\
						  giIrsValOut = IrsCtrl(E, M, G, giIrsCent>>6, 0, 0);\
						  IrsPos( E, M, ParAe(PA_IRS_POS_SPEED), 0, AE_IRS_STAT_Max);\
						  /*if(gbIrsDly && giIrsPos>=AE_IRS_STAT_Max) giIrsPos = AE_IRS_STAT_Max - 1;*/ }
#else
#define IRS_RUN(E,M,G)	{ giIrsValOut/*iIrsVal*/ = LibAeSpeedDc(E, M, AE_IRS_OPN_Max, AE_IRS_CLS_Max, UP(AE_SPEED_DC), 0);\
						  IrsPos(E, M, ParAe(PA_IRS_POS_SPEED)/*9*//*12*/, 0, AE_IRS_STAT_Max);\
						  /*giIrsValOut = iIrsVal;*/ }
#endif

#define AE_CTRL_ADV			1//((model_ADV_AE==1)&&(model_Pana==0)&&(model_Sens!=SENS_IMX225))
#define AE_CTRL_ORIGINAL	((AE_CTRL_ADV==0) || ParAe(PA_CTRL_ORIGINAL))
#define AE_SAT_OFF			(/*((gbAeStg!=AE_STG_AGC)&&(gbAeStg!=AE_STG_DSS)) ||*/ (UP(BackLight)==UP_BL_WDR) || (gbWdrOn!=WDR_OFF) || (gbWdrOnBuf2!=WDR_OFF) || ParAe(PA_SAT_OFF) || (UP(AntiSatOn)==UP_OFF)/*(UP(LSpotLv)==0)*/ /*|| UP(ClipMax)*/)

#define AE_DEV_ON			1//((model_Pana==0)&&(model_Sens!=SENS_IMX225))

#define ERR_CHT_SIZ			60		// (+Lib)
#define ERR_SPOT_SUP_GAB	UP(AE_TGT_OFST)

#define AE_TGT_MIN_BIT		2
//#define NO_EST_SPD_DOWN_BIT	3

extern BYTE gbUpShtMin;
extern BYTE gbUpDeblurMin;
extern BYTE gbUpShtMax;
extern void ShutterMenuGet(void);
extern void ShutterMenuSet(void);


#define CHANGE_VAL2(TYPE,DEF,VAL,TEMP,...) \
			static TYPE d0_##TEMP = DEF;\
			if(gbSensUpdate==0) { d0_##TEMP = VAL; return; }\
			static TYPE d1_##TEMP = DEF;\
			if(/*VAL*/d0_##TEMP == d1_##TEMP) return;\
			/*gbSensUpdateCnt++;*/\
			d1_##TEMP = /*VAL*/d0_##TEMP;\
			VAL = d0_##TEMP;

#define CHANGE_VALt(TYPE,DEF,VAL,TEMP,...)	CHANGE_VAL2(TYPE,DEF,VAL,TEMP,__VA_ARGS__)
#define CHANGE_VAL(TYPE,DEF,VAL,...)		CHANGE_VALt(TYPE,DEF,VAL,__LINE__,__VA_ARGS__)


  #if 0/* AE TEST ON */		// TODO KSH + ENABLE_PA
	#define ENABLE_PA		// (+Lib)
	#define AE_GRP
	//#define DEBUG_OSD		1
	#define UP_ANTISAT_ON	UP_OFF/*UP_ON*/		// isp_app.c
  #else
	//#define ENABLE_PA		// (+Lib)
	#define UP_ANTISAT_ON	UP_OFF				// isp_app.c
  #endif

  #if 1/* AE TYPE 1 */
	#define UP_STABILIZING	UP_4sHI		// isp_app.c
	#define UP_SPOTSPD		10/*20*/			// isp_app_menu.c
	#define UP_ERRCHGSTEP	1/*5*/			// isp_app_menu.c
	#define AE_EST_AUTO		0			// isp_app_ae.c
	#define	AE_SPOT_DECR	1			// Lib_app_ae_adv.c
  #else	/* AE TYPE 2 */
	#define UP_STABILIZING	UP_4sOFF	// isp_app.c
	#define UP_SPOTSPD		1			// isp_app_menu.c
	#define UP_ERRCHGSTEP	4			// isp_app_menu.c
	#define AE_EST_AUTO		1			// isp_app_ae.c
	#define	AE_SPOT_DECR	0			// Lib_app_ae_adv.c
  #endif


#define GrpAe(ID)	gnTxGrp[ID].m_UINT		// GRP 사용
#define GrpAeF(ID)	gnTxGrp[ID].m_float

#if 1
enum{// GA = Graph of AE
	// Use List		// Scale,	Offset

	//GA_AE_POS,

	//GA_WDR_SHORT_POS,
	//GA_WDR_LONG_POS,

	//GA_AGC_GAIN,
	//GA_AGC_ERR,

	//GA_SHT_GAIN,
	//GA_SHT_ERR,

	//GA_SHT_POS,
	//GA_SHT_POS_N,
	//GA_SHT_POS_P,

	//GA_IRS_CTRL_ORI,

	//GA_IRS_VAL0,
	//GA_IRS_ERR,
	//GA_AGC_OFF,
	//GA_IRS_CTRL,

	//GA_SHT_VAL,

	GA_ERR_NIGHT_CNT,		// gcErrSignAccu	YGMY_01r	iCurSpotChg>>IIG	iTgtVal
	//GA_TGT_NIGHT,
	//GA_ERR_DAY_POS_N,
	//GA_ERR_DAY_OFS_CNT,
	GA_ERR_DAY_ORI,			// iErrBgChg>>IIG	nAe2ClipCnt		iTgtVal

	//GA_WDR_SHT_S_VAL,
	//GA_WDR_SHT_S_VAL_CNT,

	//GA_ERR_BG_CHG,
	//GA_CUR_SPOT_CHG,

	//GA_AGC_ERR_CHG,
	//GA_AGC_ERR_EST_2,
	GA_TGT_MAX,
	GA_TGT_MIN,

	//GA_SHT_ERR_EST,
	//GA_SHT_ERR_CHG,
	//GA_SHT_ERR_EST_2,
	//GA_SHT_ERR_EST_1,
	//GA_SHT_ERR_EST_0,

	//GA_CUR_DSS_STG,

	//GA_CLIP_CNT,
	//GA_SUM2_AVG,
	//GA_CLIP_CNT_OFF,
	//GA_CLIP_CNT_ON,
	//GA_ERR_DAY,				// iErrOfs		YGMY_01r
	//GA_ERR_NIGHT_ORI,		// iErrNightRaw
	//GA_CUR_NIGHT,
	GA_ERR_NIGHT,			// GA_ERR_DAY와 같은 위치	GA_CUR_NIGHT와 같은 위치	iErrNightRaw	gbAgcDly*10 + gbShtDly


	//GA_IRS_VAL,

	//GA_CUR,
	//GA_SHT_ERR,

	//GA_AGCw,// 5 Fix

	//GA_SHT_VAL,
	//GA_AGC_VAL,

	//GA_ERR_DAY_OFS,
	GA_ERR_CHT_PRE,			// iCurSpotEst GA_ERR 와 같은 위치	iErrSpot

	GA_AGC_VAL_PRE,			// giAgcVal + giShtVal - 125		iCurSpotChg>>IIG
	GA_ERR,					// iErrBg


	__GA_END	// !! CAUTION - GA_END must be less than 9.
};
#endif


#ifdef ENABLE_PA
  #if USE_UP_PAR == 0
	#define ParAe(ID)	gnRxPar[ID]
  #else
	#define ParAe(ID)	(*(PAR_BASE+ID))
  #endif
enum{// PA = Parameter of AE
	PA_STA=0x0,

	PA_H264_SKIP_CNT,
	PA_WDR_SHORT_MIN1,
	PA_ERR_CHT_TH,
	PA_SLICE,
	PA_CLIP,
	PA_ERR_NIGHT_ON_AGC,
	PA_TGT_NIGHT_OFS,
	PA_TGT_NIGHT,
	PA_SAT_OFF,
	PA_ERR_UP_MAX,
	PA_AGC_OFF_CNT,
	PA_IRS_POS_SPEED,
	PA_IRS_OPEN_SPEED,
	PA_IRS_CLOSE_SPEED,
	PA_ERR_EST_OFF,
	PA_IRS_GAIN_MIN_BIT,
	PA_IRS_GAIN_MIN_ERR,
	PA_IRS_GAIN_MAX_ERR,
	PA_AGC_GAIN_MIN_BIT,
	PA_AGC_GAIN_MIN_ERR,
	PA_AGC_GAIN_MAX_ERR,
	PA_CTRL_ORIGINAL,
	PA_ERR_MGN,
	PA_SHT_MANUAL,
	PA_SHT_UP_SPEED,
	PA_SHT_DOWN_SPEED,
	PA_AGC_MANUAL,
	PA_AGC_UP_SPEED,
	PA_AGC_DOWN_SPEED,
	PA_IRS_MANUAL,
	PA_IRS_CNT,

	PA_END	// !! CAUTION - PA_END must be less than 33.
};
#else
	#define ParAe(V)			V
	#define PA_STA				0					// Default is 0, ClipCnt에 따라 Err보정 기능
	#define PA_H264_SKIP_CNT	6					// Init
	#define PA_WDR_SHORT_MIN1	8					// Init
	#define	PA_ERR_CHT_TH		60					// Init
	#define	PA_SLICE			0					// Test for Realtime Change
	#define	PA_CLIP				0					// Test for Realtime Change
	#define	PA_ERR_NIGHT_ON_AGC	0					// Default is 0
	#define	PA_TGT_NIGHT_OFS	64					// Init
	#define	PA_TGT_NIGHT		0					// Test for Realtime Change, Clip Lv
	#define	PA_SAT_OFF			0					// Default is 0
	#define	PA_ERR_UP_MAX		0					// Test for Realtime Change
	#define	PA_AGC_OFF_CNT		0					// IRS_DLY_CTRL 에서 사용(실제 사용 안함)
	#define	PA_IRS_POS_SPEED	UP(AE_SPEED_DC_VP)	// Init
	#define	PA_IRS_OPEN_SPEED	iIrsUpSpd			// Always, Init?
	#define	PA_IRS_CLOSE_SPEED	iIrsDnSpd			// Always, Init?
	#define	PA_ERR_EST_OFF		0					// Default is 0
	#define	PA_IRS_GAIN_MIN_BIT	2					// Init
	#define	PA_IRS_GAIN_MIN_ERR	100					// Init
	#define	PA_IRS_GAIN_MAX_ERR	300					// Init
	#define	PA_AGC_GAIN_MIN_BIT	UP(SmallSpeed)	// OSD MENU로 대체
	#define	PA_AGC_GAIN_MIN_ERR	20					// Init
	#define	PA_AGC_GAIN_MAX_ERR	100					// Init
	#define	PA_CTRL_ORIGINAL	0//UP(AE_ORIGINAL_CTRL)	// Default is 0
	#define	PA_ERR_MGN			8					// Init
	#define	PA_SHT_MANUAL		0					// Default is 0
	#define	PA_SHT_UP_SPEED		iShtUpSpd			// Always
	#define	PA_SHT_DOWN_SPEED	iShtDnSpd			// Always
	#define	PA_AGC_MANUAL		0					// Default is 0
	#define	PA_AGC_UP_SPEED		giAgcUpSpd			// Always, Init?
	#define	PA_AGC_DOWN_SPEED	giAgcDnSpd			// Always, Init?
	#define	PA_IRS_MANUAL		0					// Default is 0
	#define	PA_IRS_CNT			0					// Default is 0
#endif

#define AddDbgData(DATA,LEN)



#endif//_ISP_AE_H_
