/* **************************************************************************
 File Name	: 	isp_af.c
 Description:	EN675 - Auto Focus function
 Designer	:	Lee, Wanhee
 Date		:	19. 7. 23
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"


//*******************************************************************************
// Reserved define & tables
//*******************************************************************************
#define AF_CLIP0w(VAL)		AF_CLIPw((VAL)>>2)	// AF 8BIT
#define AF_SLICE0w(VAL)		AF_SLICEw((VAL)>>2)


//*******************************************************************************
// Variables
//*******************************************************************************
BYTE gbAfDebugOn;

// Focus Adj. Color Table - Y,  Cb, Cr
const BYTE gbFocusAdjYCbCrTbl[6][3] = {{235,128,128/*WHT*/},{218,  8,139/*YEL*/},{185,155,  8/*CYN*/},{ 50,101,248/*RED*/},{ 17,248,117/*BLU*/},{  0,128,128/*BLK*/}};


//*******************************************************************************
// Main Functions
//*******************************************************************************
void ISRT Af(void) // 180320 LWH - Focus ADJ / Motorized AF
{
	static BYTE bInitFlag=1, FocusAdj_MBuf[3]={0,};
	const BYTE bMenuChg = (UP(FocusAdj_On)!=FocusAdj_MBuf[0])|(UP(FocusThrs)!=FocusAdj_MBuf[1])|(UP(FocusAdjColorSel)!=FocusAdj_MBuf[2]);

	gbAfDebugOn = (gbMnDebugFnc==2);			// Debug string on condition
	
	// Initialization
	if(bInitFlag) { AF_Init(); bInitFlag=0; }
	
	Af_Debuging(gbAfDebugOn);

#if 1 // Focus ADJ
	if(bMenuChg) {
		if(UP(FocusAdj_On)) {
			EDGE_OSD_ONw(1);
			EDGE_THw(LibUtlInterp1D(UP(FocusThrs), 0, 20, 0x01, 0x2FF));

			EDGE_YLVw((UINT)gbFocusAdjYCbCrTbl[UP(FocusAdjColorSel)][0]<<2);
			//EDGE_CBLVw((UINT)gbFocusAdjYCbCrTbl[UP(FocusAdjColorSel)][1]<<2);
			//EDGE_CRLVw((UINT)gbFocusAdjYCbCrTbl[UP(FocusAdjColorSel)][2]<<2);
			EDGE_CBLVw((UINT)gbFocusAdjYCbCrTbl[UP(FocusAdjColorSel)][2]<<2);
			EDGE_CRLVw((UINT)gbFocusAdjYCbCrTbl[UP(FocusAdjColorSel)][1]<<2);
		}
		else {
			EDGE_OSD_ONw(0);
		}
	}
	FocusAdj_MBuf[0]=UP(FocusAdj_On);
	FocusAdj_MBuf[1]=UP(FocusThrs);
	FocusAdj_MBuf[2]=UP(FocusAdjColorSel);

#endif

#if 0 // Motorized AF
	MAF_CmdCheck();

	if(gbMAF_Excute) {

		if(MP(MpMAF_TestMode)) Lens_AgingTest(); // Lens Aging Test Mode
		else {
			switch (gbAF_MainState) {
				case 1 : { // Lens Initialization
					Lens_Initialization();
					break;
				}
				case 2 : { // Trace Curve Auto Calibration
					TraceCurve_Calibration();
					break;
				}
				default : { // Normal Operation
					MAF_ExternalControl();

					// Motor Control
					MAF_MnlMotorControl((gbMenuState==0)); // Control @Menu Off
					MAF_AutoFocus();
					break;
				}
			}
		}
		MAF_Current_Mng();    // Motor Sleep Mode Managing

		MAF_Lens_StateDisp(); // State Display(Box/Font)
		MAF_DebugingDisp();   // State Display(Font/Grp)

		if(PAR09) MAF_Test_Mode(); // Extern control test enable
	}
#endif
}

//*******************************************************************************
// Sub Functions
//*******************************************************************************
void ISRT AF_Init(void)
{

	AF_VLPF_SELw(1); // 0:3x3 LPF, 1:5x5 LPF
	//AF_CHSELw(0);  // None exist Register

	AF1_HSPw(0x009);
	AF1_HEPw(0x3B8);
	AF1_VSPw(0x007);
	AF1_VEPw(0x43D);

	AF2_HSPw(0x120);
	AF2_HEPw(0x2a0);
	AF2_VSPw(0x100);
	AF2_VEPw(0x350);

	AF_SLICEw(0x00);
	AF_CLIPw(0x3C0);

	AF_HONw(1);
	AF_CLIP_ONw(1);

	AF1_HTABw(2);
	AF2_HTABw(6);

	AF_LOCK_SELw(1);

	AF_YSELw(2); // 0 : RGB Interpolator Y, 1 : Aperture Y, 2 : Gamma Y, 3 : ACE Y, 4 : Digital Main Channel 0
	AF_YLPFw(1); // 0 : No LPF Image Source, 1 : (1:1) LPF Image Source

	AF_HONw(1); // Clock Align with HLock of Post side
	AE_HONw(1);

#if 0
	AF_VLPF_SELw(val);
	AF_CHSELw(val);
	AF1_HSPw(val);
	AF1_HEPw(val);
	AF1_VSPw(val);
	AF1_VEPw(val);
	AF2_HSPw(val);
	AF2_HEPw(val);
	AF2_VSPw(val);
	AF2_VEPw(val);
	AF_CLIPw(val);
	AF_SLICEw(val);
	AF_HONw(val);
	AF_CLIP_ONw(val);
	AE_HONw(val);
	AF2_HTABw(val);
	AF1_HTABw(val);
	AF_WIN_SELw(val);
	AF_LOCK_SELw(val);

	AF1_SUM1_LOCKr;
	AF1_SUM2_LOCKr;
	AF2_SUM1_LOCKr;
	AF2_SUM2_LOCKr;
	AF1_CLP_SUM1_LOCKr;
	AF1_CLP_SUM2_LOCKr;
	AF2_CLP_SUM1_LOCKr;
	AF2_CLP_SUM2_LOCKr;
	AF1_YSUM1_LOCKr;
	AF1_YSUM2_LOCKr;
	AF2_YSUM1_LOCKr;
	AF2_YSUM2_LOCKr;
	AF1_CLCNT_LOCKr;
	AF2_CLCNT_LOCKr;
	VAF1_SUM_LOCKr;
	VAF2_SUM_LOCKr;
#endif
}

void Af_Debuging(BYTE bExcute)
{
	static BYTE bExcuteBuf=0;

	if(bExcute) {

		OZONE_SELw(2); // AF ODM ( 0:Off, 1:AE, 2:AF, 3:AWB )
		AF_WIN_SELw(3);

		GRP0 = AF1_SUM1_LOCKr;
		GRP1 = AF1_SUM2_LOCKr;
		GRP2 = AF2_SUM1_LOCKr;
		GRP3 = AF2_SUM2_LOCKr;
		//GRP4 = VAF1_SUM_LOCKr;
		//GRP5 = VAF2_SUM_LOCKr;
		GRP6 = AF1_YSUM1_LOCKr;
		//GRP7 = AF1_CLCNT_LOCKr;

		#if 1
//		UINT nAlpha = LibUtlInterp1D(giCurAgc, 0, AGC_POS_MAX, 0x3E8, 0x4B0);
		UINT nAlpha = LibUtlInterp1D(giCurAgc, 0, 800, 0x3E8, 0x4B0);

		const UINT nAF1_YSUM1_LOCK = AF1_YSUM1_LOCKr;
		#define AF_YSUM_MIN_START	55797761
		#define AF_YSUM_MIN_END		572525

//#if 1
		UINT nAF_SUM1 = MAX(AF_YSUM_MIN_START, nAF1_YSUM1_LOCK);
//#else
		UINT nAF_SUM2 =	(AF_YSUM_MIN_START < nAF1_YSUM1_LOCK) ? nAF1_YSUM1_LOCK :
						(AF_YSUM_MIN_END > nAF1_YSUM1_LOCK) ? AF_YSUM_MIN_END :
						LibUtlInterp1D(nAF1_YSUM1_LOCK, AF_YSUM_MIN_START, AF_YSUM_MIN_END, AF_YSUM_MIN_START, 6675830);
//#endif



		GRP4 = nAlpha;
		GRP5 = (AF1_SUM1_LOCKr/(nAF_SUM1>>0x0F))*nAlpha;
		GRP7 = (AF1_SUM1_LOCKr/(nAF_SUM2>>0x0F))*nAlpha;
		#endif

		UartTxGrp();
	}
	else {
		if(bExcuteBuf) {
			//OZONE_SELw(0); // AF ODM ( 0:Off, 1:AE, 2:AF, 3:AWB )
			AF_WIN_SELw(0);
		}
	}
	bExcuteBuf=bExcute;
}

/* EOF */

