/* **************************************************************************
 File Name	: 	isp_etc.c
 Description:	EN675 - ETC. Functions
 Designer	:	Kim, Sunghoon
 Modified by:	Lee, Wanhee
 Date		:	14. 8. 4 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"

//*************************************************************************************************
// Functions
//-------------------------------------------------------------------------------------------------
int ISRT udiv4x(const UINT anNumer, const UINT anDenom, const UINT anZeroDenom)	// AE 8BIT, AWB 8BIT
{
#if 0
	return (int)( (anDenom) ? (anNumer>0x1FFFFFFF) ? (anDenom>>2) ? anNumer / (anDenom>>2) : (anNumer / anDenom)<<2 : (anNumer<<2) / anDenom : anZeroDenom );	// TestCur10bit()의 주석 ! 에서 오차 발생
#else
	/*const*/ uint64 ulDiv = (anDenom) ? (((uint64)anNumer)<<2) / ((uint64)anDenom) : anZeroDenom;
	//return MIN(ulDiv,0x7FFFFFFF);							// unsign형을 x4 한 후에 나누어서 sign으로 출력하므로 최상위 bit는 0이어야 함
	if(ulDiv > (int)0x7FFFFFFF) ulDiv = (int)0x7FFFFFFF;	// MIN() 보다 코드 감소
	return ulDiv;
#endif
}


//-------------------------------------------------------------------------------------------------
int giIspAgc[3]={0,0,0};
BYTE gbIspAgcSta=0;
BYTE gbIspAgcEnd=0;

void IspAgcSet(void)
{
	extern int AE_GAIN_TOTAL;
	extern int AE_GAIN_TOTAL2;
	const int iAeAgcTVal = AE_GAIN_TOTAL + AE_GAIN_TOTAL2;

	ADNR_AGC_MIN = (UP(ShpAgcLow)  * iAeAgcTVal) / 100;
	ADNR_AGC_MID = (UP(ShpAgcMid)  * iAeAgcTVal) / 100;
	ADNR_AGC_MAX = (UP(ShpAgcHigh) * iAeAgcTVal) / 100;
	if(ADNR_AGC_MID < ADNR_AGC_MIN) ADNR_AGC_MID = ADNR_AGC_MIN;
	if(ADNR_AGC_MAX < ADNR_AGC_MID) ADNR_AGC_MAX = ADNR_AGC_MID;

	//printf("ADNR_AGC: %d, %d, %d\r\n", ADNR_AGC_MIN, ADNR_AGC_MID, ADNR_AGC_MAX);
}

void ISRT IspAgcCurSet(void)
{
	const BYTE iAgcPos = (giCurAgc<=ADNR_AGC_MIN) ? 0 : (giCurAgc<=ADNR_AGC_MID) ? 1 : (giCurAgc<=ADNR_AGC_MAX) ? 2 : 3;

	gbIspAgcSta = (ADNR_AGC_MID==ADNR_AGC_MIN) || (ADNR_AGC_MID==ADNR_AGC_MAX) || (iAgcPos==0) ? 0 : (iAgcPos-1);

	gbIspAgcEnd = (ADNR_AGC_MAX<=ADNR_AGC_MIN) ? 0 : (ADNR_AGC_MID==ADNR_AGC_MIN) || (ADNR_AGC_MID==ADNR_AGC_MAX) || (iAgcPos==3) ? 2 : iAgcPos;
}


//-------------------------------------------------------------------------------------------------
const UINT Adnr2D_Q_Tbl[] = {
	0x0f000f,
	0x120017,
	0x1b0020,
	0x240029,
	0x0f0012,
	0x17001b,
	0x200024,
	0x29002d,
	0x120017,
	0x1b0020,
	0x240029,
	0x2d0032,
	0x17001b,
	0x200024,
	0x29002d,
	0x320036,
	0x1b0020,
	0x240029,
	0x2d0032,
	0x36003b,
	0x200024,
	0x29002d,
	0x320036,
	0x3b003f,
	0x240029,
	0x2d0032,
	0x36003b,
	0x3f0044,
	0x29002d,
	0x320036,
	0x3b003f,
	0x440048
};

void ISRT Adnr_2D_Gain(const UINT anGain)
{
	for(int i=0; i<ARRAY_SIZE(Adnr2D_Q_Tbl); i++)
	{
		const UINT nQHt = (Adnr2D_Q_Tbl[i]>>16)&0x1fff;
		const UINT nQLt = Adnr2D_Q_Tbl[i]&0x1fff;
#if 0
		const UINT nQH = (nQHt*anGain)>>7;
		const UINT nQL = (nQLt*anGain)>>7;
		SetIsp(DNR_Q_BASE+i, (MIN(nQH,0x1fff)<<16) | MIN(nQL,0x1fff) );
#else
		SetIsp(DNR_Q_BASE+i, (((nQHt*anGain)>>7)<<16) | ((nQLt*anGain)>>7) );
#endif
	}
}


#if 0
void Adnr_CmdCheck(void) // 180320 LWH
{
	//static BYTE gbAdnr_WaitCnt=3;

	// Exception Condition
#if (model_2M30p)
	#define NOR_MODE	(UP(FrameMode) == UP_FRAMEMODE_NORMAL_25_30)
#else
	#define NOR_MODE	((UP(FrameMode) == UP_FRAMEMODE_NORMAL_25_30) || (UP(FrameMode) == UP_FRAMEMODE_NORMAL_50_60))
#endif

#if (model_Sens == SENS_OV2718)		// OV2718 Parallel 27MHz 동작 시 Sensor 출력의 HLOCK이 흔들림 -> VLOCK이 흔들림 -> DNR 시 간헐적으로 영상이 깨지는 경우 발생 -> HLOCK이 흔들리지 않는 Sensor initial setting이 필요
	const BYTE bMpAdnr = UP_4sOFF;
#else
	const BYTE bMpAdnr = ((UP(BackLight) == UP_BL_WDR) || (gbWdrOn==UP_ON) || (gbWdrOnBuf2==UP_ON) || FPS_HI || NOR_MODE) ? UP_4sOFF : UP(Adnr3D) ;
#endif

	if(bMpAdnr == UP_4sOFF) gbAdnr_WaitCnt=3; // Priority : WDR > DNR
	else if(gbAdnr_WaitCnt) gbAdnr_WaitCnt--; // wait for stable image

	gbAdnr_Excute = (gbAdnr_WaitCnt==0);
}
#endif

	const int Adnr3DFkTbl[3][3]	= 	{//  Low Agc,	Mid Agc,	Hi Agc
										{0x90,		0xc0,		0xe0	},		// Low strength
										{0xa0, 		0xd0, 		0xe0	},		// Mid	"
										{0xb0, 		0xd8, 		0xe0	}		// Hi	"
									};

	const int Adnr3DGainTbl[3][3] =	{//  Low Agc,	Mid Agc,	Hi Agc
										{0x28,		0x10,		0x0c	},		// Low strength
										{0x20, 		0x0c, 		0x08	},		// Mid	"
										{0x18, 		0x08, 		0x04	}		// Hi	"
									};

void ISRT Adnr(void)
{	// Digital noise reduction

	if(UP(Adnr2D)) {	// UP(Adnr2D) : Sharpness=5 이면 2, Sharpness=10 이면 3 권장, Sharpness=10 + Ace=HIGH 이면 8 권장
	#if 1
		#define ADNR_2D_MID 0x80
		#define ADNR_2D_MAX 0x100

		const int iAdnr2D_L = (UP(Adnr2D)<<4);	// UP(Adnr2D)가 4 이면 0x40
		const int iAdnr2D_M = MAX(iAdnr2D_L, ADNR_2D_MID);
		const int iAdnr2D_H = MAX(iAdnr2D_M, ADNR_2D_MAX);

		const int iGain2D_S = (gbIspAgcSta==0) ? iAdnr2D_L : (gbIspAgcSta==1) ? iAdnr2D_M : iAdnr2D_H;
		const int iGain2D_E = (gbIspAgcEnd==0) ? iAdnr2D_L : (gbIspAgcEnd==1) ? iAdnr2D_M : iAdnr2D_H;
		const int iGain2D   = (gbIspAgcSta < gbIspAgcEnd) ? LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, iGain2D_S, iGain2D_E) : iGain2D_S ;
		Adnr_2D_Gain(iGain2D);
	#else
		const int iGain2D_S = (gbIspAgcSta==0) ? UP(Adnr2D) : 0xc0;	// 0xc0 초과 적용 시 포화 경계선 부분에 영상왜곡 발생, UP(Adnr2D)값으로 0x30 또는 0x60 권장
		const int iGain2D_E = (gbIspAgcEnd==0) ? UP(Adnr2D) : 0xc0;
		const int iGain2D = (gbIspAgcSta < gbIspAgcEnd) ? LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, iGain2D_S, iGain2D_E) : iGain2D_S ;
		//DNR2D_GAINw(iGain2D);
	#endif
		DNR2D_ONw(1);
	}
	else {
		DNR2D_ONw(0);
	}

#if 0
	static BYTE bIspResetCnt = 0;
	//GrpAe(GA_ERR_NIGHT_ORI) = bIspResetCnt;
	if(bIspResetCnt) { bIspResetCnt--; return; }

	Adnr_CmdCheck();

	if(gbAdnr_Excute) {

		if(giCurDssStg > 1) return;
		else if(giCurDssStg==1) {
			giCurDssStg = 0;
			isp_reset();
			bIspResetCnt = 1/*PAR00*/;	// isp_reset 후 최소 2frame 후에 DNR3D_ONw(1) 해야 함
			return;
		}
#else
	if(UP(Adnr3D) != UP_4sOFF/*gbAdnr_Excute*/) {
#endif
		int iGain=0,iFk=0,iTh=0;
		const BYTE bTblSel_0 = (UP(Adnr3D) == UP_4sHI) ? UP_3sHI : (UP(Adnr3D) == UP_4sMID) ? UP_3sMID : UP_3sLOW ;

		if((gbIspAgcSta < gbIspAgcEnd) && (UP(DnrIncrease) == UP_ON)) {
			iGain = LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Adnr3DGainTbl[bTblSel_0][gbIspAgcSta], Adnr3DGainTbl[bTblSel_0][gbIspAgcEnd]);
			iTh   = 0;//LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Adnr3DThrsTbl[bTblSel_0][gbIspAgcSta], Adnr3DThrsTbl[bTblSel_0][gbIspAgcEnd]);
			iFk   = LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Adnr3DFkTbl  [bTblSel_0][gbIspAgcSta], Adnr3DFkTbl  [bTblSel_0][gbIspAgcEnd]);
		}
		else {
			const BYTE bTblSel_1 = (UP(DnrIncrease) == UP_ON) ? gbIspAgcSta : 0 ;
			iGain = Adnr3DGainTbl[bTblSel_0][bTblSel_1];
			iTh   = 0;//Adnr3DThrsTbl[bTblSel_0][bTblSel_1];	// 0 초과 적용 시 일반 조도 상황에서 괘적 발생
			iFk   = Adnr3DFkTbl  [bTblSel_0][bTblSel_1];
		}

		// Adding user parameter gain weight
#if 0
		const BYTE b3D_DNR_GAIN = (MP(MpAdnr) == MN_4sLOW) ? UP(3D_DNR_GAIN_LOW)    :
		                          (MP(MpAdnr) == MN_4sMID) ? UP(3D_DNR_GAIN_MIDDLE) :
								  (MP(MpAdnr) == MN_4sHI ) ? UP(3D_DNR_GAIN_HIGH)   : 0 ;

		iGain = MIN((iGain * b3D_DNR_GAIN)>>7, 0x3F);
		iTh   = MIN((iTh   * b3D_DNR_GAIN)>>7, 0xFF);
		iFk   = MIN((iFk   * b3D_DNR_GAIN)>>7, 0xFF);
#endif

		// Post Processing
		DNR3D_ONw(1);
		//DDR_RD_MODw(1); // DDR set to DNR Mode

		DNR3D_GAw((UINT)iGain);		// 0x3F
		DNR3D_THw((UINT)iTh  );		// 0xFF
		DNR3D_FKw((UINT)iFk  );		// 0xFF
	}
	else DNR3D_ONw(0);
}


//-------------------------------------------------------------------------------------------------
const int Apt55ThTbl[3]  =   //  Low Agc,	Mid Agc,	Hi Agc
								{0x10,		0xa0,		0x100	};	// 2015827 - WHL
//								{0x20,		0xa0,		0x100	};	// 2015827 - WHL

#if 0
const int Apt55GainTbl[3]=   //  Low Agc,	Mid Agc,	Hi Agc
								{0x30,		0x20,		0x10	};		// 150526 LH
//								{0x20,		0x18,		0x10	};	// 20170509 - WHL
#endif

const int Apt33ThTbl[3] =   //  Low Agc,	Mid Agc,	Hi Agc
								{0x20,		0x100,		0x100	};	// 2015827 - WHL
//								{0x40,		0x100,		0x100	};	// 20170509 - WHL

void ISRT Sharpness(void)
{	// Aperture

	//if(gbMnDebugFnc==8) return;

	int i55Th, i55Gain, i33Th, i33Gain;

	const int i55Gain_S = (gbIspAgcSta==0) ? UP(ShpBigEdge) : (gbIspAgcSta==1) ? (UP(ShpBigEdge)>>1) : 0;
	const int i55Gain_E = (gbIspAgcEnd==0) ? UP(ShpBigEdge) : (gbIspAgcEnd==1) ? (UP(ShpBigEdge)>>1) : 0;

	const int i33Gain_S = (gbIspAgcSta==0) ? UP(ShpSmallEdge) : 0;
	const int i33Gain_E = (gbIspAgcEnd==0) ? UP(ShpSmallEdge) : 0;

//	APT_ONw(1);
//	APT_GAIN_ONw(1);

	if(gbIspAgcSta < gbIspAgcEnd) {
		i55Th 	= LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Apt55ThTbl[gbIspAgcSta], Apt55ThTbl[gbIspAgcEnd]);
		i55Gain = LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, i55Gain_S, i55Gain_E);
		i33Th 	= LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Apt33ThTbl[gbIspAgcSta], Apt33ThTbl[gbIspAgcEnd]);
		i33Gain = LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, i33Gain_S, i33Gain_E);
	}
	else {
		i55Th  	= Apt55ThTbl[gbIspAgcSta];
		i55Gain	= i55Gain_S;
		i33Th	= Apt33ThTbl[gbIspAgcSta];
		i33Gain	= i33Gain_S;
	}

	// APT 5x5 Filter control
	i55Gain = MIN((i55Gain * UP(Sharpness) / 5/*UP(SHARPNESS_GAIN)*/), 0xff);

	APT_GAIN1_POSw(i55Gain);
	APT_GAIN2_POSw(i55Gain);
	APT_GAIN1_NEGw(i55Gain);
	APT_GAIN2_NEGw(i55Gain);
	APT_TH1w(i55Th);
//	APT_TH3w(0x80);

	// APT 3x3 Filter control
	i33Gain = MIN((i33Gain * UP(Sharpness) / 5/*UP(SHARPNESS_GAIN)*/), 0xff);

	APT_GAIN3w(i33Gain);
	APT_SLIC3w(i33Th);
//	APT_CLIP3w(0x80);

	/*GRP0 = giCurAgc;
	GRP1 = i55Th;
	GRP2 = i55Gain;
	GRP3 = i33Th;
	GRP4 = i33Gain;
	TxGrpFnc();*/
}

//-------------------------------------------------------------------------------------------------
const int DefectDifGaTbl[3]  = {0x20, 0x30, 0x40};		//  AUTO Defect gain table (Low Agc, Mid Agc, Hi Agc)
const int DefectSDifGaTbl[3] = {0x10, 0x08, 0x00};		// AUTO Defect spot gain table (Low Agc, Mid Agc, Hi Agc)

void ISRT DefectAuto(void)	// TODO KSH ◆ DefectAuto() - DOL 시 Long과 Short이 따로 설정되도록 해야 함
{	// Auto Defect correction
	int iGain, iSGain;
	if(gbIspAgcSta < gbIspAgcEnd) {
		iGain  = LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, DefectDifGaTbl[gbIspAgcSta], DefectDifGaTbl[gbIspAgcEnd]);
		iSGain = LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, DefectSDifGaTbl[gbIspAgcSta], DefectSDifGaTbl[gbIspAgcEnd]);
	}
	else {
		iGain  = DefectDifGaTbl[gbIspAgcSta];
		iSGain = DefectSDifGaTbl[gbIspAgcSta];
	}

	iGain = MIN(((iGain * UP(DEFECT_GA))>>7), 0x3F);	// 2015615 HSH
	iSGain = MIN(((iSGain * UP(DEFECT_SPOT_GA))>>7), 0x3F);	// 2015615 HSH

	DF_DIFGA_LEw(iGain);
	DF_DIFGA_SE0w(iGain);

	DFS_DIFGA_LEw(iSGain);
	DFS_DIFGA_SE0w(iSGain);
}

//-------------------------------------------------------------------------------------------------
#if(model_Sens==SENS_IMX291)||(model_Sens==SENS_IMX327)||(model_Sens==SENS_IMX307)
							//Low Agc,	Mid Agc,	Hi Agc
const int LowCSupGaTbl[3] = {0x38,		0x24,		0x10	}; // 14.2.10
#else
const int LowCSupGaTbl[3] = {0x20,		0x18,		0x10	}; // 14.2.10
#endif

void ISRT CSup(void)
{
	// Low light color suppression ----------------------------
	int iGain = (gbIspAgcSta < gbIspAgcEnd) ? LibUtlInterp1D(giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, LowCSupGaTbl[gbIspAgcSta], LowCSupGaTbl[gbIspAgcEnd]) : LowCSupGaTbl[gbIspAgcSta];
	iGain = MIN((iGain * UP(LSUP_GA))>>7, 0x3F);	// 15.06.13 KSH

	LSUP_ONw(!(!UP(LSUP_ON)||gbWdrOn));		// 141209 (gbWdrOn&&UpLSUP_ON) ? 0: 1	//HTY 20150613
	LSUP_GAw(iGain);

	// Edge color suppression -------------------------------
	const BYTE bMpCES_ON = (gbWdrOn==1) ? UP(CES_WDR) : UP(CES_NOR);
	const BYTE bMpCES_ONrv = (40 < bMpCES_ON) ? 40 : 40 - bMpCES_ON;
	const BYTE bCES_LCLIP = (bMpCES_ONrv > 20) ? bMpCES_ONrv + (((bMpCES_ONrv-19)*(bMpCES_ONrv-20))>>1) : bMpCES_ONrv;

	CES_ONw(bMpCES_ON>0);				//WHL 20180807
	CES_GAINw((gbWdrOn==1) ? UP(CES_WGAIN) : UP(CES_GAIN));
	CES_LCLIPw((gbWdrOn==1) ? (bCES_LCLIP>>1)/*UP(CES_WLCLP)*/ : bCES_LCLIP/*UP(CES_LCLP)*/);
}

//-------------------------------------------------------------------------------------------------
// Privacy Color Table -            Y,  Cb, Cr
const BYTE gbPrvcYCbCrTbl[8][3] = {{235,128,128/*WHT*/},{218,  8,139/*YEL*/},{185,155,  8/*CYN*/},{168, 35, 19/*GRN*/},
								   { 67,221,237/*MAG*/},{ 50,101,248/*RED*/},{ 17,248,117/*BLU*/},{  0,128,128/*BLK*/}};

void HlMask(void)
{	// High Light Mask

#if 1
	HLMASK_ONw((UP(BackLight)==UP_BL_HLC));
	HLMASK_THw(54+(UP(HlMaskThrs)*0xA/*UP(HLMASK_THRS_STEP)*/));

	if(UP(HlMaskColorSel)<8) {	// Fixed Menu Color Table

		HLMASK_YLVw((UINT)gbPrvcYCbCrTbl[UP(HlMaskColorSel)][0]);
		HLMASK_BLVw((UINT)gbPrvcYCbCrTbl[UP(HlMaskColorSel)][1]);
		HLMASK_RLVw((UINT)gbPrvcYCbCrTbl[UP(HlMaskColorSel)][2]);
	}
	else {	// CUSTOMIZE

		HLMASK_YLVw((UINT)UP(HlMaskColorCY));
		HLMASK_BLVw((UINT)UP(HlMaskColorCB));
		HLMASK_RLVw((UINT)UP(HlMaskColorCR));
	}
#endif
}

//-------------------------------------------------------------------------------------------------
void ColorSpace(void)
{ // Color space setup

	switch(UP(OutFormat)) {
		case UP_FRMT_UV :	RMYRw(0x276);RMYGw(0x20F);RMYBw(0x66);BMYRw(0x97);BMYGw(0x128);BMYBw(0x1BE); break;
		case UP_FRMT_SD :	RMYRw(0x20B);RMYGw(0x1B6);RMYBw(0x55);BMYRw(0xB0);BMYGw(0x15B);BMYBw(0x20B); break;
		case UP_FRMT_HD :	RMYRw(0x20B);RMYGw(0x1DB);RMYBw(0x30);BMYRw(0x78);BMYGw(0x193);BMYBw(0x20B); break;
	}
}

void Flip(void)
{
	if(UP(Flip) == UP_ON || UP(Flip) == UP_OFF) {
		//FLP_ONw(UP(Flip)); FLP_OFSw(0x1e);
		SensFlip(UP(Flip));
	}

	ICSELw(/*(UP(Flip)==UP_ON) ? UP(ICSelFlip) :*/ UP(ICSel));
	OCSELw(/*(UP(Flip)==UP_ON) ? UP(OCSelFlip) :*/ UP(OCSel));

	//DNR_CSELw(UP(ICSel));	// TODO KSH + ICSEL 과 DNR_CSEL 테스트
}


//-------------------------------------------------------------------------------------------------
UINT gnBoxPosPVC[PVC_EA*2];
UINT gnBoxPltPVC/*[PVC_EA]*/;
UINT gnBoxTonePVC[2];
UINT gnBoxFillPVC;
UINT gnBoxOnPVC;
UINT gnBoxFmtPVC;
UINT gnBBoxPltPVC;
UINT gnBoxNumPVC;

void PrivacyBox(void)
{	// Box Privacy 32ea

	const UINT nPixelStep = CLAMP(UpBOX_MV_PXL, BOX_MVPXL_MIN, BOX_MVPXL_MAX);		//141126 LGH
	const BYTE nColorStep = CLAMP(UpBOX_COLOR_STEP, 0x01, 0xFF);					//141126 LGH

	gnBoxOnPVC = 0;		// init	14.2.20
	gnBoxFmtPVC = 0;
	gnBoxNumPVC = 0;
	gnBoxTonePVC[0] = 0;
	gnBoxTonePVC[1] = 0;
	gnBoxFillPVC = 0;

	if(UP(PvcOn)==UP_ON)
	{
		const UINT nPRVC_CY = UP(PvcYLevel)  * 255 / nColorStep;
		const UINT nPRVC_CB = UP(PvcCBLevel) * 255 / nColorStep;
		const UINT nPRVC_CR = UP(PvcCRLevel) * 255 / nColorStep;

		gnBBoxPltPVC = (nPRVC_CB<<16)|(nPRVC_CR<<8)|(nPRVC_CY);
		gnBoxPltPVC  = (nPRVC_CY<<16)|(nPRVC_CB<<8)|(nPRVC_CR);			// box attr

		for(UINT i=0; i<PVC_EA; i++)
		{
			if(gbMnPvcCfg(i)->bAction)
			{
				const int iPosX = (gbMnPvcCfg(i)->bPosX) * nPixelStep;
				const int iSizX = (gbMnPvcCfg(i)->bSizX) * nPixelStep;

				const int iPosY = (gbMnPvcCfg(i)->bPosY) * nPixelStep;
				const int iSizY = (gbMnPvcCfg(i)->bSizY) * nPixelStep;

				gnBoxPosPVC[(gnBoxNumPVC<<1)]   = (MAX((iPosY+iSizY), 0)) | (MAX(iPosY, 0)<<16);	// y pos
				gnBoxPosPVC[(gnBoxNumPVC<<1)+1] = (iPosX+iSizX) | (iPosX<<16);						// x pos

//				gnBoxPltPVC[gnBoxNumPVC] = (nPRVC_CY<<16)|(nPRVC_CB<<8)|(nPRVC_CR);			// box attr

				if (gnBoxNumPVC<16)	gnBoxTonePVC[0] |= UP(PvcTrans)<<(gnBoxNumPVC<<1);
				else				gnBoxTonePVC[1] |= UP(PvcTrans)<<((gnBoxNumPVC-16)<<1);

				gnBoxFillPVC |= (1<<gnBoxNumPVC);
				gnBoxOnPVC |= (1<<gnBoxNumPVC);

				if(UP(PvcFormat)==1) {	// 2015112 - WHL : Add Bit Box Mode
					gnBoxFmtPVC |= (1<<gnBoxNumPVC);
				}

				gnBoxNumPVC++;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
//                             0x0  0x1  0x2  0x3  0x4   0x5   0x6   0x7   0x8   0x9   0xA   0xB   0xC   0xD   0xE   0xF
const BYTE gbTwinBitLUT[] = { 0x0, 0x3, 0xC, 0xF, 0x30, 0x33, 0x3C, 0x3F, 0xC0, 0xC3, 0xCC, 0xCF, 0xF0, 0xF3, 0xFC, 0xFF };
#define CHG_TWIN_BIT(M)	gbTwinBitLUT[(M)&0xf]

void ISRT BoxLast(void)
{	// Last Box apply (priority, sharing control)

// LGH 20150410
#if 0
  #if (model_1M)
	BOX_HSP0w(0x127);
	BOX_VSP0w(0x22);

  #else				//2M
		if((UP(OutFps)==UP_2M_720p_25_30)||(UP(OutFps)==UP_2M_720p_50_60)){		//MD, Privacy Box Start Position
			BOX_HSP0w(0x4F);	// 20161111 - WHL
			BOX_VSP0w(0x1a);
		}
		else{
			#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3)) // BT656 or BT1120 Input Setting
				BOX_HSP0w(0x00);
				BOX_VSP0w(0x00);
			#else
				BOX_HSP0w(0x4F);	// 20161111 - WHL
				BOX_VSP0w(0x09);
			#endif
		}
  #endif
#endif

	//const int DefogAutoOn = (UP(Defog) == UP_ON) && (UP(DefogMode) == UP_DEFOG_AUTO);
	//const int HistoOn = (UP(PvcOn) == UP_OFF) && (UP(Itl) == UP_OFF) && (DefogAutoOn == 0) && (gbMnDebugFnc == 1);

#if 1
	const int AeMonOn = (gbMnDebugFnc == 1);
	static BYTE AeMonOn1d = 0;
	if(AeMonOn1d != AeMonOn) {
		AeMonOn1d = AeMonOn;
		if(AeMonOn) { extern BYTE grr; grr = 1; }
		else        { extern BYTE grr; grr = 0; }
	}
	if(AeMonOn) { void AeMonOn(const int); AeMonOn(173829904); }
	else        { void AeMonOff(void); AeMonOff(); }
#endif

	//----------------------------------------------------------------------------------------
	int i;

#if 0//(model_Lens==0)
	if(MP(MpFad_On)) {
		for(i=0; i<FADJ_EA; i++) { // Focus Assist
			SetIsp(BOXPOS_BASE+(i*2)  , gnBoxPosFADJ[(i*2)]);			// box y
			SetIsp(BOXPOS_BASE+(i*2+1), gnBoxPosFADJ[(i*2)+1]);		// box x
			SetIsp(BOXPLT_BASE+i      , gnBoxPltFADJ[i]);				// box attr
		}

		SetIsp(BOXTON_BASE  , gnBoxToneFADJ[0]);						// box tone msb
		SetIsp(BOXTON_BASE+1, gnBoxToneFADJ[1]);						// box tone lsb
		SetIsp(BOXFIL_BASE  , gnBox_BFL_FADJ);							// box fill tone

		BOSD_ON0w(0x7ffff); // USE 19 Box
	}
	else {
#endif

		const int iStaPVC = ISP_BOX_EA - gnBoxNumIMD - gnBoxNumPVC;
		for (i=iStaPVC; i<(ISP_BOX_EA-gnBoxNumIMD); i++) {
			SetIsp(BOXPOS_BASE+(i<<1),     gnBoxPosPVC[((i-iStaPVC)<<1)]);		// box y
			SetIsp(BOXPOS_BASE+((i<<1)+1), gnBoxPosPVC[((i-iStaPVC)<<1)+1]);	// box x
			SetIsp(BOXPLT_BASE+i,          gnBoxPltPVC/*[i]*/);					// box attr
		}

		const int iStaIMD = ISP_BOX_EA - gnBoxNumIMD;
		for (i=iStaIMD; i<ISP_BOX_EA; i++) {
			SetIsp(BOXPOS_BASE+(i<<1),     gnBoxPosIMD[((i-iStaIMD)<<1)]);		// box y
			SetIsp(BOXPOS_BASE+((i<<1)+1), gnBoxPosIMD[((i-iStaIMD)<<1)+1]);	// box x

			extern BYTE gbMnImdDetWin;
			//const int iItlDettone = (gbMnImdDetWin && (UP(ItlDettone)==3)) ? 2 : UP(ItlDettone);	// DET TONE이 3일 때 DET WINDOW 메뉴 사용 시 DET TONE를 2로 설정
			const int iItlDettone = (gbMnImdDetWin) ? 4 : UP(ItlDettone);							// DET WINDOW 메뉴 사용 시 DET TONE를 4로 설정

			const UINT nColorIMD = (i < (ISP_BOX_EA-1-gnBoxMskNum) || iItlDettone==3) ? (0x51<<16 | 0x5a<<8 | 0xef) :	// Motion Box
								   (i < (ISP_BOX_EA-1            ))	? (gnBoxMskSel&&(i==(ISP_BOX_EA-1-gnBoxMskNum+(gnBoxMskSel-1)))) ? 0xd21092 : 0x903522 : // Area Box
										   	   	   	   	  	  	  	  (235<<16 | 128<<8 | 128) ;	// Background Box
			SetIsp(BOXPLT_BASE+i, nColorIMD);									// box attr
		}

#if (model_Lens==1)
		if(gbMAFBoxFlag) {
			for (i=MAF_BOX_START; i<ISP_BOX_EA; i++) {
				SetIsp(BOXPOS_BASE+(i<<1),     gnBoxPosMAF_Y[(i-MAF_BOX_START)]);	// box y
				SetIsp(BOXPOS_BASE+((i<<1)+1), gnBoxPosMAF_X[(i-MAF_BOX_START)]);	// box x
				SetIsp(BOXPLT_BASE+i,          gnBoxPltMAF[(i-MAF_BOX_START)]);	// box attr
			}
		}
#endif


#if (model_Lens==1)
		SetIsp(BOXTON_BASE  , (gnBoxToneIMD[0]|gnBoxToneMAF)&(~(UINT)(gbBoxToneROIWdr<<((AE_ROI_WDR_BOX_NO-16)*2))));		// box tone msb
		SetIsp(BOXTON_BASE+1, gnBoxTonePVC[0]);								// box tone lsb
		SetIsp(BOXFIL_BASE  , ((gnBoxFillIMD<<16)|gnBoxFillPVC|gnBoxFillMAF)&(~(UINT)(gbBoxFillROIWdr<<AE_ROI_WDR_BOX_NO)));	// box fill tone

		BOSD_ON0w(gnBoxOnIMD<<16|gnBoxOnPVC|(gbBoxOnROIWdr<<AE_ROI_WDR_BOX_NO)|gnBoxOnMAF);	// box on/off
#else

		const UINT nBoxBitMask = ~(gnBoxOnIMD<<iStaIMD | gnBoxOnPVC<<iStaPVC);
		const UINT nBoxBitMask2L = CHG_TWIN_BIT(nBoxBitMask)     | (CHG_TWIN_BIT(nBoxBitMask>>4)<<8)  | (CHG_TWIN_BIT(nBoxBitMask>>8)<<16)  | (CHG_TWIN_BIT(nBoxBitMask>>12)<<24);
		const UINT nBoxBitMask2H = CHG_TWIN_BIT(nBoxBitMask>>16) | (CHG_TWIN_BIT(nBoxBitMask>>20)<<8) | (CHG_TWIN_BIT(nBoxBitMask>>24)<<16) | (CHG_TWIN_BIT(nBoxBitMask>>28)<<24);

		const ULONG gnBoxTonePVC_U = ((((ULONG)gnBoxTonePVC[1])<<32) | gnBoxTonePVC[0])<<(iStaPVC<<1);
		const UINT gnBoxTonePVC_L = gnBoxTonePVC_U;
		const UINT gnBoxTonePVC_H = gnBoxTonePVC_U>>32;

		//const UINT gnBoxTonePVC_L = (gnBoxTonePVC[0]<<(iStaPVC<<1));
		//const UINT gnBoxTonePVC_H = (gnBoxTonePVC[1]<<(iStaPVC<<1)) | (gnBoxTonePVC[0]>>((16-iStaPVC)<<1));

		//SetIsp(BOXTON_BASE  , gnBoxToneIMD[0]&(~(UINT)(gbBoxToneROIWdr<<((AE_ROI_WDR_BOX_NO-16)*2))));		// box tone msb
		SetIsp(BOXTON_BASE  , (GetIsp(BOXTON_BASE  )&nBoxBitMask2H) | gnBoxTonePVC_H | gnBoxToneIMD<<((iStaIMD-16)<<1));			// box tone lsb
		SetIsp(BOXTON_BASE+1, (GetIsp(BOXTON_BASE+1)&nBoxBitMask2L) | gnBoxTonePVC_L);				// box tone lsb
		//SetIsp(BOXFIL_BASE  , ((gnBoxFillIMD<<16)|gnBoxFillPVC)&(~(UINT)(gbBoxFillROIWdr<<AE_ROI_WDR_BOX_NO)));	// box fill tone
		SetIsp(BOXFIL_BASE  , (GetIsp(BOXFIL_BASE)&nBoxBitMask) | gnBoxFillIMD<<iStaIMD | gnBoxFillPVC<<iStaPVC);	// box fill tone

		//BOSD_ON0w(gnBoxOnIMD<<16|gnBoxOnPVC|(gbBoxOnROIWdr<<AE_ROI_WDR_BOX_NO));	// box on/off
		BOSD_ON0w((BOSD_ON0r&nBoxBitMask) | gnBoxOnIMD<<iStaIMD | gnBoxOnPVC<<iStaPVC);	// box on/off
#endif
		// 2015112 - WHL : Add Bit Box Mode
		SetIsp(BOXFMT_BASE , gnBoxFmtPVC<<iStaPVC);						// box format
		SetIsp(BBOXFMT_BASE, gnBBoxPltPVC);								// bit box fill format

		/*GRP0 = nBoxBitMask;
		GRP1 = nBoxBitMask2L;
		GRP2 = nBoxBitMask2H;
		GRP3 = gnBoxTonePVC_L;
		GRP4 = gnBoxTonePVC_H;
		GRP5 = iStaPVC;
		UartTxGrp();*/

#if 0//(model_Lens==0)
	}
#endif
}




/* EOF */
