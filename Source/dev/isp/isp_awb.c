/* **************************************************************************
 File Name	: 	isp_awb.c
 Description:	EN675 - Auto White Balance module
 Designer	:	Lee, Hoon (14.12.05)
 Modified by:	Kim, Sunghoon (19.08.20)
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"
#include "isp_awb_tbl.h"

//*******************************************************************************
// Reserved define & Variables
//*******************************************************************************
#define	AWB_ODM_CAP_USE		0									// Use for Eyenix engineering
#if AWB_ODM_CAP_USE
	#include "./misc/awb_odmcap.h"
#endif

#define TestMnl	0

extern int giCmat[3][3];
extern const s64 glMtxCf[9][6];

UINT 	gnAwbRGainIIR, gnAwbBGainIIR;
int  	giAwbDbIIR, giAwbDrIIR;

UINT 	gnAwbErr;
BYTE 	gbAwbDebugOn;
UINT	gnAwbOdmCapOn = 0;
int 	giAwbPrstTbl[4] = { (-300)<<4, (-100)<<4, AWB_GAIN_DFLT<<4, AWB_GAIN_DFLT<<4};		// Preset save

#define MTX_CF	gf32MtxCf
float32 gf32MtxCf[9][6];
extern void LibAwbCmat(int, int, float32 [9][6]);


//*************************************************************************************************
// Sub Functions
//*************************************************************************************************
#if 0
int ISRT div4x(const int aiNumer, const int aiDenom, const int aiZeroDenom)	// AWB 8BIT
{
	/*const*/ long lDiv = (aiDenom) ? (((long)aiNumer)<<2) / ((long)aiDenom) : aiZeroDenom;
	//return CLAMP(lDiv, (int)0x80000000, (int)0x7FFFFFFF);
	if(lDiv > (int)0x7FFFFFFF) lDiv = (int)0x7FFFFFFF;			// CLAMP() 보다 코드 감소
	if(lDiv < (int)0x80000000) lDiv = (int)0x80000000;			// "                     , else if(...) 와 코드 동일
	return lDiv;
}
#endif

void HueChroma(void)
{
#if 0
	if(UP(HueChromaMode)) {	// OLD
#endif
		BYHUE_LDw(((int)UP(Yellow_HUE_RedToGreen))-128);
		BYGAIN_LDw(UP(Yellow_GAIN));
		RYHUE_LUw(((int)UP(Red_HUE_YellowToBlue))-128);
		RYGAIN_LUw(UP(Red_GAIN));
		BYHUE_LUw(((int)UP(Blue_HUE_GreenToRed))-128);
		BYGAIN_LUw(UP(Blue_GAIN));
		RYHUE_LDw(((int)UP(Green_HUE_BlueToYellow))-128);
		RYGAIN_LDw(UP(Green_GAIN));
#if 0
	}
	else {					// NEW
		RYHUE_LUw(((int)UP(Yellow_Red_HUE_YellowToBlue))-128);
		BYHUE_LUw(((int)UP(Yellow_Red_HUE_RedToGreen))-128);
		BYGAIN_LUw(UP(Yellow_Red_GAIN_Yellow));
		RYGAIN_LUw(UP(Yellow_Red_GAIN_Red));
		RYHUE_RUw(((int)UP(Red_Blue_HUE_YellowToBlue))-128);
		BYHUE_RUw(((int)UP(Red_Blue_HUE_GreenToRed))-128);
		RYGAIN_RUw(UP(Red_Blue_GAIN_Red));
		BYGAIN_RUw(UP(Red_Blue_GAIN_Blue));
		RYHUE_RDw(((int)UP(Blue_Green_HUE_BlueToYellow))-128);
		BYHUE_RDw(((int)UP(Blue_Green_HUE_GreenToRed))-128);
		BYGAIN_RDw(UP(Blue_Green_GAIN_Blue));
		RYGAIN_RDw(UP(Blue_Green_GAIN_Green));
		RYHUE_LDw(((int)UP(Green_Yellow_HUE_BlueToYellow))-128);
		BYHUE_LDw(((int)UP(Green_Yellow_HUE_RedToGreen))-128);
		RYGAIN_LDw(UP(Green_Yellow_GAIN_Green));
		BYGAIN_LDw(UP(Green_Yellow_GAIN_Yellow));
	}

	HUE_MODw(UP(HueChromaMode));
#else
	HUE_MODw(1);
#endif


}

void AwbCmapSet(void)
{
	int i;
	WORD* pwMapTbl = (WORD*)gwCmap;
	if(UP(Awb)==UP_AWB_AUTOEXT) pwMapTbl++;

	for (i=0; i<64; i++) {
		SetIsp(AWBL_CM_BASE+i, 	*pwMapTbl);
		pwMapTbl += 2;
	}
}

void InitAwb(void)
{	// Initial AWB

	HueChroma();

//	ICSELw(0x1);		// matching with OCSEL
//	OZONE_SELw(0x3)				;		// ODM OSD Selection Register

	//ParSet(UpAWB_SPEED_MTRX, MP(MpAwbSpeed));
	//ParSet(UpAWB_AGC_WGT_MIN, MP(MpAwbAgcWgtMin));
	//ParSet(UpLSUP_ON, MP(MpLSUP_ON));
	//ParSet(UpAWB_YSLICE, MP(MpAwbYSliceMax));

	// 배포 X
	// ODM YC matrix	Y  = 0.33R + 0.33G + 0.33B,	Cb = B - Y,	 Cr = R - Y
	ODM_M1w(0x55);
	ODM_M2w(0x55);
	ODM_M3w(0x55);
	ODM_M4w(0x255);
	ODM_M5w(0x255);
	ODM_M6w(0xab);
	ODM_M7w(0xab);
	ODM_M8w(0x255);
	ODM_M9w(0x255);

	AWBLCBCRO_SELw(0x0);	// 0: DbDr, 1: CbCr  OutPut
	AWBL_HTAB_SELw(0x2);	// Size of detection block, LPF 크기(0:8size, 1:4size, 2:2size, 3:1size:No LPF), LPF 후 AWBL_CBDB_SUM_LOCK 와 같은 AWB 누적 출력값 계산
	AWB_LOCK_SELw(0x1);		// Awb data lock at VLOCK
	AWBLYEDGE_THw(0x3ff);	// Y edge pseudo color remove


	AWB_WIN1_HSPw(RP(AWB_HSP));
	AWB_WIN1_HWw(RP(AWB_HW));
	AWB_WIN1_VSPw(RP(AWB_VSP));
	AWB_WIN1_VWw(RP(AWB_VW));

	AWBLNOR_SELw(0x0);		// 1 : Always operate, 0 : Use color map

	AWBLOSD_ONw(0x0);					// AWB OSD On/Off
	AWBL_OSD_SELw(0xf9/*0xfb*/);		// OSD Form
	AWBL_CM_HSPw(0x1c0/*0x1f8*/);		// Color map OSD position	//150422 HSH
	AWBL_CM_VSPw(0x180/*0x138*/);		// "

	//AWBL_CYSLICEw(UP(AWB_YSLICE));	// Y slice
	//AWBL_CYCLIPw(UP(AWB_YCLIP));		// Y clip

	//AWBL0_DBOFFSETw(UP(AWB_DBOFFSET));
	//AWBL0_DROFFSETw(UP(AWB_DROFFSET));

	AWBL_DBGAIN0w(SP(AWB_DBGAIN));
	AWBL_DRGAIN0w(SP(AWB_DRGAIN));

	AWB_CHSELw(0);	// Ch selection 0: Long, 1: Short
	AWB_ONw(1);

	giAwbDbIIR = giDbDrMnl[1][0]<<4;
	giAwbDrIIR = giDbDrMnl[1][1]<<4;
	gnAwbRGainIIR = AWB_GAIN_DFLT<<4;
	gnAwbBGainIIR = AWB_GAIN_DFLT<<4;

	AwbCmapSet();

	UINT i;
	for (i=0; i<4; i++) giAwbPrstTbl[i] = (UD(UdAwbPrst+(i*4)+3)<<24) | (UD(UdAwbPrst+(i*4)+2)<<16) | (UD(UdAwbPrst+(i*4)+1)<<8) | (UD(UdAwbPrst+(i*4)+0)<<0);

#if 1//model_ISP_Save == 1
	const int iDbSum = giAwbPrstTbl[0]>>4;
	const int iDrSum = giAwbPrstTbl[1]>>4;
	const UINT nRGain = giAwbPrstTbl[2]>>4;
	const UINT nBGain = giAwbPrstTbl[3]>>4;

	if( (SP(AWB_DB_MIN) <= iDbSum && iDbSum <= SP(AWB_DB_MAX)) &&
		(SP(AWB_DR_MIN) <= iDrSum && iDrSum <= SP(AWB_DR_MAX)) &&
		(AWB_GAIN_MIN <= nRGain && nRGain <= AWB_GAIN_MAX) &&
		(AWB_GAIN_MIN <= nBGain && nBGain <= AWB_GAIN_MAX) ) {
			giAwbDbIIR = giAwbPrstTbl[0];
			giAwbDrIIR = giAwbPrstTbl[1];
			gnAwbRGainIIR = giAwbPrstTbl[2];
			gnAwbBGainIIR = giAwbPrstTbl[3];

			INIT_STR("AWB : Set to preset values.");
	}
	else {	// Press 모드 Data가 없는 경우 초기값 설정
		giAwbPrstTbl[0] = giAwbDbIIR;
		giAwbPrstTbl[1] = giAwbDrIIR;
		giAwbPrstTbl[2] = (int)gnAwbRGainIIR;
		giAwbPrstTbl[3] = (int)gnAwbBGainIIR;

		INIT_STR("AWB : Set to default values.");
	}

	//TxStrDec("Db:", iDbSum, 4);
	//TxStrDec("Dr:", iDrSum, 4);
	//TxStrDec("Gr:", nRGain, 4);
	//TxStrDec("Gb:", nBGain, 4);
#endif

//#if (model_Save==0)		// EEPROM only
//	TwiRdEep(TWI_EEP_AWBPRST_STT , (BYTE*)giAwbPrstTbl, 16);
//#else
//
//#endif

	UINT j;
	for(j=0; j<9; j++) {
		for(i=0; i<6; i++) {
			//const float a = (float)glMtxCf[j][i];
			//gf32MtxCf[j][i] = TOFLOAT32(a);
			gf32MtxCf[j][i] = (float32)glMtxCf[j][i];
		}
	}

	extern float32 gf32AwbMatGain;
	//const float gfAwbMatGain2 = (double)AWB_MTRX_GAIN * 256. / 100000000000000.;
	//gf32AwbMatGain = TOFLOAT32(gfAwbMatGain2);
	gf32AwbMatGain = (float32)((double)AWB_MTRX_GAIN * 256. / 100000000000000.);
}

void ISRT Awb(void)
{
	int		iDrSum=0, iDgSum=0, iDbSum=0;

	UINT	nRsum,  nGsum,  nBsum,  nRGain, nBGain, nAwbRgain, nAwbGgain, nAwbBgain;

	int		iRcm0=0x100,iRcm1=0x0,iRcm2=0x0,iGcm0=0x0,iGcm1=0x100,iGcm2=0x0,iBcm0=0x0,iBcm1=0x0,iBcm2=0x100;
	UINT	nRcm1,nRcm2,nGcm0,nGcm2,nBcm0,nBcm1;

//	double	oIntpRgain=1,oIntpBgain=1;

	int 	iAgcPos,iWgt;
	static BYTE	gbMnAwbBuf;

	gbAwbDebugOn = (gbMnDebugFnc==3);
//--------------------------------------------------------------------------------------------------
// Exception

	//if (WDR_ONr&&((LS_FLGr&0x00000001)==AeLONG))	return;		// WDR (process at Short and apply to Long)
	//if (gbModeWdrOn_F&&((LS_FLGr&0x00000001)==AeLONG))	return;		// LH TMP
	if((gbWdrOn==WDR_FRAME)&&(LS_FLGr==AeLONG))	return;

#if AWB_CAPTURE_MODE
	else if (gnAwbOdmCapOn) {
		if (gnAwbOdmCapOn==1)	{ InitAwbOdmCap();	gnAwbOdmCapOn++; }
		else					{ AwbOdmCap();						 }
		return;
	}
#endif

// -------------------------------------------------------------------------------
// ODM Area

	if(gbAwbDebugOn&&(!gbBlcOsdOn)) {
		AWBLOSD_ONw(1);
		OZONE_SELw (3);
	}
	else {
		AWBLOSD_ONw(0);
	}

	if ((!((gbMnDebugFnc>=1)&&(gbMnDebugFnc<=3)))&&(!gbBlcOsdOn)/*&&(UP(ShdDet)!=1)*//*&&(gbOzone_Hold!=1)*/)	OZONE_SELw(0);		// final OSD Off condition // 150325 WHL

//--------------------------------------------------------------------------------------------------
// ColorMap Set

	if (UP(Awb)!=gbMnAwbBuf) {
		AwbCmapSet();
		gbMnAwbBuf = UP(Awb);
	}

//--------------------------------------------------------------------------------------------------
// Color Matrix control

	const UINT nTcnt = AWBL_CCT_SUM_LOCKr;											// Total block cnt, 실제 최대 크기 : AWB_WIN1_HW * AWB_WIN1_VW / 2
//	iDrSum		= AWBL_CRDR_SUM_LOCKr	;     										// CR/DR(32b sign), 실제 최대 크기 : AWB_WIN1_HW * AWB_WIN1_VW / 2 * [-1024 ~ +1023] = 0x770 * 0x850 / 2 * [-1024 ~ +1023] = -2074476544 ~ 2072450688 (32bit)
//	iDgSum		= AWBL_CY_SUM_LOCKr		;											// CY	(31b)
//	iDbSum		= AWBL_CBDB_SUM_LOCKr	;	     									// CB/DB(32b sign)

#if 1//model_ISP_Save == 1
	static BYTE bInitAwb = /*(model_ISP_Save == 1) ? 3 :*/ 1;
	if(bInitAwb) {
		//INIT_STR_DEC("Awb X:", iDgSum / (int)nTcnt, 4);
	}
	else {
		static BYTE bDispCnt = 5;
		if(bDispCnt) {
			//INIT_STR_DEC("Awb O:", iDgSum / (int)nTcnt, 4);
			bDispCnt--;
		}
	}
#else
	#define bInitAwb 0
#endif

	UINT nHold = (nTcnt<(UINT)UP(AWB_TCNT_LMT))? 1 : 0;												// AWB Hold flag by block cnt.

	if (UP(Awb)==UP_AWB_MNL){																	// Manual Matrix
		if(UP(AwbMnl) < ARRAY_SIZE(giDbDrMnl)) {
			iDbSum	= giDbDrMnl[UP(AwbMnl)][0];													// UP(AwbMnl) = UP_3000K, UP_5000K, UP_8000K
			iDrSum  = giDbDrMnl[UP(AwbMnl)][1];
		}
	}
	else if (bInitAwb || ((UP(Awb)==UP_AWB_PRESET)&&(gbMpAwbPrst==UP_OFF)) || (nHold&0x1) ) {	// Hold Matrix
		if (bInitAwb || (UP(Awb)==UP_AWB_PRESET)) {
			iDbSum = giAwbPrstTbl[0]>>4;
			iDrSum = giAwbPrstTbl[1]>>4;
		}
		else {
			iDbSum	= giAwbDbIIR >> 4;
			iDrSum	= giAwbDrIIR >> 4;
			nHold  += 0x10;
		}
	}
	else if (UP(Awb)==UP_AWB_AUTOEXT) {
		iDbSum	= giDbDrMnl[UP_5000K][0];
		iDrSum	= giDbDrMnl[UP_5000K][1];
	}
	else{
#if 0
		iDbSum	= div4x(AWBL_CBDB_SUM_LOCKr, (int)nTcnt, 0);							// Pixel mean, CB/DB(32b sign), AWB 8BIT,
		iDrSum	= div4x(AWBL_CRDR_SUM_LOCKr, (int)nTcnt, 0);							// "         , CR/DR(32b sign), "       ,
#else
		iDbSum	= (int)AWBL_CBDB_SUM_LOCKr / (int)nTcnt;								// Pixel mean, CB/DB(32b sign), 3840 / 2(AWB ODM CLK이 1/2 이므로) / 2(Fixed) x 2160 x 1023 = 960 x 2160 x 1023 = 0x7E705C00
		iDrSum	= (int)AWBL_CRDR_SUM_LOCKr / (int)nTcnt;								// "         , CR/DR(32b sign), "
#endif
//		iDgSum	= div4x(AWBL_CY_SUM_LOCKr,   (int)nTcnt, 0);							// Pixel mean, CY	(31b)     , AWB 8BIT, 3840 x 2160 x 255 = 0x7E117000
		iDgSum	= udiv4x(AWBL_CY_SUM_LOCKr, nTcnt, 0);									// Pixel mean, CY	(31b)     , AWB 8BIT, 3840 x 2160 x 255 = 0x7E117000
	}

	iDbSum 		= CLAMP(iDbSum, SP(AWB_DB_MIN), SP(AWB_DB_MAX));						// ODM data limitaion
	iDrSum 		= CLAMP(iDrSum, SP(AWB_DR_MIN), SP(AWB_DR_MAX));           				// "

	giAwbDbIIR 	= ( ((int)UP(AWB_SPEED_MTRX)*giAwbDbIIR) + ((int)(0x100-UP(AWB_SPEED_MTRX))*(iDbSum<<4)) ) >> 8;
	giAwbDrIIR 	= ( ((int)UP(AWB_SPEED_MTRX)*giAwbDrIIR) + ((int)(0x100-UP(AWB_SPEED_MTRX))*(iDrSum<<4)) ) >> 8;

	gnAwbErr	= MAX((UINT)ABS((giAwbDbIIR>>4) - iDbSum), (UINT)ABS((giAwbDrIIR>>4) - iDrSum));

//	DispDatDec("AWB_P1 :", 20, 24, giAwbDrIIR);

	LibAwbCmat ((giAwbDbIIR>>4), (giAwbDrIIR>>4), MTX_CF);
//	DispDatDec("AWB_P2 :", 20, 24, giAwbDrIIR);

// ------------------------------------------------------------------
// Apply Pre ODM Matrix  (Sign[11], Integer[10:8], Float[7:0])
//
#if (TestMnl==0)
	iRcm0 = CLAMP(giCmat[0][0],     0, 2047);
	iRcm1 = CLAMP(giCmat[0][1], -2047, 2047);
	iRcm2 = CLAMP(giCmat[0][2], -2047, 2047);
	iGcm0 = CLAMP(giCmat[1][0], -2047, 2047);
	iGcm1 = CLAMP(giCmat[1][1],     0, 2047);
	iGcm2 = CLAMP(giCmat[1][2], -2047, 2047);
	iBcm0 = CLAMP(giCmat[2][0], -2047, 2047);
	iBcm1 = CLAMP(giCmat[2][1], -2047, 2047);
	iBcm2 = /*PAR00 ? PAR00 : */CLAMP(giCmat[2][2],     0, 2047);
#else	// Manual control (affect to Post Matrix)
	iRcm0 = ((PRR_GAINr)>>11) ? -(PRR_GAINr&BitMask_11) : (PRR_GAINr&BitMask_11);
	iRcm1 = ((PRG_GAINr)>>11) ? -(PRG_GAINr&BitMask_11) : (PRG_GAINr&BitMask_11);
	iRcm2 = ((PRB_GAINr)>>11) ? -(PRB_GAINr&BitMask_11) : (PRB_GAINr&BitMask_11);
	iGcm0 = ((PGR_GAINr)>>11) ? -(PGR_GAINr&BitMask_11) : (PGR_GAINr&BitMask_11);
	iGcm1 = ((PGG_GAINr)>>11) ? -(PGG_GAINr&BitMask_11) : (PGG_GAINr&BitMask_11);
	iGcm2 = ((PGB_GAINr)>>11) ? -(PGB_GAINr&BitMask_11) : (PGB_GAINr&BitMask_11);
	iBcm0 = ((PBR_GAINr)>>11) ? -(PBR_GAINr&BitMask_11) : (PBR_GAINr&BitMask_11);
	iBcm1 = ((PBG_GAINr)>>11) ? -(PBG_GAINr&BitMask_11) : (PBG_GAINr&BitMask_11);
	iBcm2 = ((PBB_GAINr)>>11) ? -(PBB_GAINr&BitMask_11) : (PBB_GAINr&BitMask_11);
#endif

	#define SIGN_ABS(SIGN_BIT,VAL)		(((VAL)<0) ? ((1<<(SIGN_BIT)) | (-(VAL))) : (VAL))
	nRcm1 = SIGN_ABS(11,iRcm1);
	nRcm2 = SIGN_ABS(11,iRcm2);
	nGcm0 = SIGN_ABS(11,iGcm0);
	nGcm2 = SIGN_ABS(11,iGcm2);
	nBcm0 = SIGN_ABS(11,iBcm0);
	nBcm1 = SIGN_ABS(11,iBcm1);

	// Pre mat (temp)
	PRR_GAINw(iRcm0);
	PRG_GAINw(nRcm1);	// 변수 nRcm1을 사용하지 않고 바로 PRG_GAINw(SIGN_ABS(11,iRcm1)); 하면 코드 증가함
	PRB_GAINw(nRcm2);
	PGR_GAINw(nGcm0);
	PGG_GAINw(iGcm1);
	PGB_GAINw(nGcm2);
	PBR_GAINw(nBcm0);
	PBG_GAINw(nBcm1);
	PBB_GAINw(iBcm2);

//-------------------------------------------------------------------------------------------------
// RGB gain control

//	nRsum 	= AWBL_CCR_SUM_LOCKr	;											// R from Pre ODM Mat
//	nGsum 	= AWBL_CCG_SUM_LOCKr	;       									// G "
//	nBsum 	= AWBL_CCB_SUM_LOCKr	;       									// B "

	const UINT nAwbGainMax = AWB_GAIN_MAX;										// RGB Gain max
	const UINT nAwbGainMin = AWB_GAIN_MIN;										// RGB Gain min

	if (UP(Awb)==UP_AWB_MNL){													// Manual Gain
		nRsum 	= 0;
		nGsum 	= 0;
		nBsum 	= 0;

		#define AWB_GAIN_STEP_H		((nAwbGainMax-AWB_GAIN_DFLT)/10)
		#define AWB_GAIN_STEP_L		((AWB_GAIN_DFLT-nAwbGainMin)/10)

		nRGain  = (UP(Rgain)>=10) ? (UINT)((UP(Rgain)-10)*AWB_GAIN_STEP_H)+AWB_GAIN_DFLT:
									(UINT)((UP(Rgain)   )*AWB_GAIN_STEP_L)+nAwbGainMin  ;
		nBGain  = (UP(Bgain)>=10) ? (UINT)((UP(Bgain)-10)*AWB_GAIN_STEP_H)+AWB_GAIN_DFLT:
									(UINT)((UP(Bgain)   )*AWB_GAIN_STEP_L)+nAwbGainMin  ;

	}
	else if (bInitAwb || ((UP(Awb)==UP_AWB_PRESET)&&(gbMpAwbPrst==UP_OFF)) || (nHold&0x1) ) {	// Hold Gain
		if (bInitAwb || (UP(Awb)==UP_AWB_PRESET)) {					// Preset save example
			nRGain = giAwbPrstTbl[2]>>4;
			nBGain = giAwbPrstTbl[3]>>4;
		}
		else {
			nRGain = gnAwbRGainIIR>>4;
			nBGain = gnAwbBGainIIR>>4;
			nHold += 0x100;
		}
		nRsum 	= 0;
		nGsum 	= 0;
		nBsum 	= 0;
	}
	else {																						// Auto Gain
		nRsum 	= udiv4x(AWBL_CCR_SUM_LOCKr, nTcnt, 0);	// R from Pre ODM Mat, AWB 8BIT, 3840 x 2160 x 255 = 0x7E117000
		nGsum 	= udiv4x(AWBL_CCG_SUM_LOCKr, nTcnt, 0);	// G "               , "       , "
		nBsum 	= udiv4x(AWBL_CCB_SUM_LOCKr, nTcnt, 0);	// B "               , "       , "

		nRsum 	= MAX(nRsum, 1);
		nGsum 	= MAX(nGsum, 1);
		nBsum 	= /*PAR01 ? PAR01 : */MAX(nBsum, 1);

//		nRGain  = (nGsum*AWB_GAIN_DFLT) / ((nRsum*AWB_GAIN_DFLT)/(gnAwbRGainIIR>>4));				// gain with default offset		// new
//		nBGain  = (nGsum*AWB_GAIN_DFLT) / ((nBsum*AWB_GAIN_DFLT)/(gnAwbBGainIIR>>4));				// "							// new

		nRGain = (nGsum*AWB_GAIN_DFLT)/nRsum;								// gain with default offset
		nBGain = (nGsum*AWB_GAIN_DFLT)/nBsum;
	}

//	DispDatDec("AWB_P3 :", 20, 24, giAwbDrIIR);

	nRGain = CLAMP(nRGain, nAwbGainMin, nAwbGainMax);						// Gain limitation
	nBGain = CLAMP(nBGain, nAwbGainMin, nAwbGainMax);						//	"

	gnAwbRGainIIR = ( (AWB_SPEED_GAIN*gnAwbRGainIIR) + ((0x100-AWB_SPEED_GAIN)*(nRGain<<4)) ) >> 8;
	gnAwbBGainIIR = ( (AWB_SPEED_GAIN*gnAwbBGainIIR) + ((0x100-AWB_SPEED_GAIN)*(nBGain<<4)) ) >> 8;

	gnAwbErr = MAX(gnAwbErr, (UINT)ABS((int)(gnAwbRGainIIR>>4) - (int)nRGain));
	gnAwbErr = MAX(gnAwbErr, (UINT)ABS((int)(gnAwbBGainIIR>>4) - (int)nBGain));

//-------------------------------------------------------------------------------------------------
// Final Gain with weights

#if (TestMnl==0)

		iAgcPos = (giCurAgc<=UP(AWB_AGC_MIN)) ? 0 :
				  (giCurAgc<=AWB_AGC_MAX) ? 1 : 2;

		switch (iAgcPos) {
			case 0 :	iWgt 	= AWB_AGC_WGT_MAX;	break;
			case 1 :	iWgt 	= LibUtlInterp1D(giCurAgc, UP(AWB_AGC_MIN), AWB_AGC_MAX, AWB_AGC_WGT_MAX, UP(AWB_AGC_WGT_MIN)); break;
			case 2 :	iWgt 	= UP(AWB_AGC_WGT_MIN);	break;
		}

		AWBL_CYSLICEw(AE2_SLICEr); // follow AE Slice 2019-4-2 KDH

		iWgt = (gnTdnChkOut==TDN_NIT)? 0 : iWgt;			// Day or B&W

		// ---------------------
	#if 0
		int iDrCur = iDrSum, iWgt2;
		iDrCur = MAX(iDrCur, giDbDrMnl[1][1]);
		iDrCur = MIN(iDrCur, AWB_DR_MAX);
		iWgt2  = LibUtlInterp1D(iDrCur, giDbDrMnl[1][1], AWB_DR_MAX, AWB_AGC_WGT_MAX, (((AWB_AGC_WGT_MAX-UP(AWB_AGC_WGT_MIN))<<3)>>4)+UP(AWB_AGC_WGT_MIN));
//		DispDat("WGT2____"	, 13, 24, iWgt2);

		iWgt   = MIN(iWgt, iWgt2);
	#endif

//		nAwbRgain 	= ((WORD)((float)(gnAwbRGainIIR>>4) * (0.5f+((float)MP(MpSaturation)*0.05f))) * iWgt) >> 8;		// divide by iWgt scale
//		nAwbGgain 	= ((WORD)((float)AWB_GAIN_DFLT   	* (0.5f+((float)MP(MpSaturation)*0.05f))) * iWgt) >> 8;
//		nAwbBgain 	= ((WORD)((float)(gnAwbBGainIIR>>4) * (0.5f+((float)MP(MpSaturation)*0.05f))) * iWgt) >> 8;

	#if 0
		const BYTE bMpSaturationR = (gbWdrOn!=WDR_ OFF) ? ((40-10)>UP(SaturationR)) ? (UP(SaturationR)+10) : 40 : UP(SaturationR) ;
		const BYTE bMpSaturationG = (gbWdrOn!=WDR_ OFF) ? ((40-10)>UP(SaturationG)) ? (UP(SaturationG)+10) : 40 : UP(SaturationG) ;
		const BYTE bMpSaturationB = (gbWdrOn!=WDR_ OFF) ? ((40-10)>UP(SaturationB)) ? (UP(SaturationB)+10) : 40 : UP(SaturationB) ;

		nAwbRgain 	= ((WORD)((float)(gnAwbRGainIIR>>4) * (0.5f+((float)bMpSaturationR*0.025f))) * iWgt) >> 8;		// divide by iWgt scale
		nAwbGgain 	= ((WORD)((float)AWB_GAIN_DFLT   	* (0.5f+((float)bMpSaturationG*0.025f))) * iWgt) >> 8;
		nAwbBgain 	= ((WORD)((float)(gnAwbBGainIIR>>4) * (0.5f+((float)bMpSaturationB*0.025f))) * iWgt) >> 8;
	#else
		// Color Gamma를 낮추고 Color Gain을 올리는 것이 Color Noise가 덜하고 더 자연스러움
		const BYTE bMpSaturationR = (gbWdrOn!=WDR_OFF) ? ((64-16)>UP(SaturationR)) ? (UP(SaturationR)+16) : 64 : UP(SaturationR) ;	// TODO KSH ◆ WDR - Awb()에서 "R-GAIN"의 Line WDR 설정을 Frame WDR로 하는게 더 좋은지 확인 필요
		const BYTE bMpSaturationG = (gbWdrOn!=WDR_OFF) ? ((64-16)>UP(SaturationG)) ? (UP(SaturationG)+16) : 64 : UP(SaturationG) ;	//	"
		const BYTE bMpSaturationB = (gbWdrOn!=WDR_OFF) ? ((64-16)>UP(SaturationB)) ? (UP(SaturationB)+16) : 64 : UP(SaturationB) ;	//	"

		nAwbRgain = ((gnAwbRGainIIR>>4) * iWgt * (32+bMpSaturationR)) >> (8+6);		// 8:iWgt, 6:bMpSaturationX		190902 KSH
		nAwbGgain = (AWB_GAIN_DFLT      * iWgt * (32+bMpSaturationG)) >> (8+6);		//		"
		nAwbBgain = ((gnAwbBGainIIR>>4) * iWgt * (32+bMpSaturationB)) >> (8+6);		//		"
	#endif

#else
		nAwbRgain 	= AWB_GAIN_DFLT;
		nAwbGgain 	= AWB_GAIN_DFLT;
		nAwbBgain 	= AWB_GAIN_DFLT;
#endif

////-------------------------------------------------------------------------------------------------
//// Post Matrix (final with R,B Gain)(Sign[12], Integer[10:8], Float[7:0])
//
	iRcm0 = (iRcm0 * (int)nAwbRgain) >> 8;				// divide by Gain scale
	iRcm1 = (iRcm1 * (int)nAwbRgain) >> 8;
	iRcm2 = (iRcm2 * (int)nAwbRgain) >> 8;
	iGcm0 = (iGcm0 * (int)nAwbGgain) >> 8;
	iGcm1 = (iGcm1 * (int)nAwbGgain) >> 8;
	iGcm2 = (iGcm2 * (int)nAwbGgain) >> 8;
	iBcm0 = (iBcm0 * (int)nAwbBgain) >> 8;
	iBcm1 = (iBcm1 * (int)nAwbBgain) >> 8;
	iBcm2 = (iBcm2 * (int)nAwbBgain) >> 8;

	nRcm1 = SIGN_ABS(12,iRcm1);
	nRcm2 = SIGN_ABS(12,iRcm2);
	nGcm0 = SIGN_ABS(12,iGcm0);
	nGcm2 = SIGN_ABS(12,iGcm2);
	nBcm0 = SIGN_ABS(12,iBcm0);
	nBcm1 = SIGN_ABS(12,iBcm1);

	WPRR_GAINw(iRcm0);
	WPRG_GAINw(nRcm1);	// 변수 nRcm1을 사용하지 않고 바로 WPRG_GAINw(SIGN_ABS(12,iRcm1)); 하면 코드 증가함
	WPRB_GAINw(nRcm2);
	WPGR_GAINw(nGcm0);
	WPGG_GAINw(iGcm1);
	WPGB_GAINw(nGcm2);
	WPBR_GAINw(nBcm0);
	WPBG_GAINw(nBcm1);
	WPBB_GAINw(iBcm2);


//	static UINT nGrpCnt = 0;
//	nGrpCnt = (nGrpCnt==FPS_VDI) ? 1 : nGrpCnt+1;
//	GRP0 = iWgt;
//	GRP1 = AWBL_CCB_SUM_LOCKr;//(gnAwbRGainIIR>>4);
//	GRP2 = PBB_GAINr;//AWB_GAIN_DFLT;
//	GRP3 = WPBB_GAINr;//(gnAwbBGainIIR>>4);
//	GRP4 = nAwbRgain;
//	GRP5 = nAwbGgain;
//	GRP6 = nAwbBgain;
//	GRP7 = nGrpCnt;
//	if(gbMnDebugFnc==1) UartTxGrp();

//-------------------------------------------------------------------------------------------------
// Preset save example	13.2.22
	static UINT	gnAwbPrstCnt,gnAwbPrstDone=0;

	if((UP(Awb)==UP_AWB_PRESET)&&(gbMpAwbPrst==UP_ON)) {	// preset key push

		if ((gnAwbPrstCnt>=0x40)&&(gnAwbErr<0x18)&&(!gnAwbPrstDone)) {		// save on condition
			gnAwbPrstCnt = 0;
			giAwbPrstTbl[0] = giAwbDbIIR;
			giAwbPrstTbl[1] = giAwbDrIIR;
			giAwbPrstTbl[2] = (int)gnAwbRGainIIR;
			giAwbPrstTbl[3] = (int)gnAwbBGainIIR;

			gnAwbPrstDone = 1;

			UINT i;
			for (i=0; i<4; i++)	 UD(UdAwbPrst+ 0+i)	= (giAwbPrstTbl[0]>>(i*8)) & 0xff;
			for (i=0; i<4; i++)	 UD(UdAwbPrst+ 4+i)	= (giAwbPrstTbl[1]>>(i*8)) & 0xff;
            for (i=0; i<4; i++)	 UD(UdAwbPrst+ 8+i)	= (giAwbPrstTbl[2]>>(i*8)) & 0xff;
            for (i=0; i<4; i++)	 UD(UdAwbPrst+12+i)	= (giAwbPrstTbl[3]>>(i*8)) & 0xff;
            gbUsrDataSaveChk = 1;

//			DebugDisp(1, Hex, "Verify", 17, 24, giAwbPrstTbl[0])
//			DebugDisp(1, Hex, "Verify", 18, 24, giAwbPrstTbl[1])
//			DebugDisp(1, Hex, "Verify", 19, 24, giAwbPrstTbl[2])
//			DebugDisp(1, Hex, "Verify", 20, 24, giAwbPrstTbl[3])

		}
		else gnAwbPrstCnt ++;

		gnAwbPrstCnt = MIN(gnAwbPrstCnt, 0x100);

	}
	else {
		gnAwbPrstCnt = 0;
		gnAwbPrstDone = 0;
	}

// -------------------------------------------------------------------------------
// AWB State Auto Save
	if(bInitAwb) bInitAwb--;


	extern int RR_GAIN;
	extern int RG_GAIN;
	extern int RB_GAIN;
	extern int GR_GAIN;
	extern int GG_GAIN;
	extern int GB_GAIN;
	extern int BR_GAIN;
	extern int BG_GAIN;
	extern int BB_GAIN;
	extern  void LibAwbWdrRGB (int, int, int);

	RR_GAIN = iRcm0;
	RG_GAIN = iRcm1;
	RB_GAIN = iRcm2;
	GR_GAIN = iGcm0;
	GG_GAIN = iGcm1;
	GB_GAIN = iGcm2;
	BR_GAIN = iBcm0;
	BG_GAIN = iBcm1;
	BB_GAIN = iBcm2;
	LibAwbWdrRGB (1, (gnTdnChkOut==TDN_NIT), 0);

//-------------------------------------------------------------------------------------------------
// Monitoring
#if 0
//	DebugDisp(1,Dec ,"AWB_Db_MAX", 32,10, AWB_DB_MAX);
//	DebugDisp(1,Dec ,"AWB_Db_MIN", 33,10, AWB_DB_MIN);
//
//	DebugDisp(1,Dec ,"AWB_Dr_MAX", 34,10, AWB_DR_MAX);
//	DebugDisp(1,Dec ,"AWB_Dr_MIN", 35,10, AWB_DR_MIN);

	DebugDisp(gbAwbDebugOn, Dec, "TCNT___d"	,  1, 24, nTcnt 	)
	DebugDisp(gbAwbDebugOn, Dec, "CRDR___d"	,  2, 24, iDrSum	)
	DebugDisp(gbAwbDebugOn, Dec, "CY_____d"	,  3, 24, iDgSum	)
	DebugDisp(gbAwbDebugOn, Dec, "CBDB___d"	,  4, 24, iDbSum	)
	DebugDisp(gbAwbDebugOn, Dec, "R______d"	,  6, 24, nRsum		)
	DebugDisp(gbAwbDebugOn, Dec, "G______d"	,  7, 24, nGsum		)
	DebugDisp(gbAwbDebugOn, Dec, "B______d"	,  8, 24, nBsum		)
	DebugDisp(gbAwbDebugOn,    , "R_Gain__"	, 10, 24, nAwbRgain	)
	DebugDisp(gbAwbDebugOn,    , "B_Gain__"	, 11, 24, nAwbBgain	)
	DebugDisp(gbAwbDebugOn,    , "WGT_____"	, 12, 24, iWgt		)
	DebugDisp(gbAwbDebugOn,    , "Hold____"	, 14, 24, nHold		)
	DebugDisp(gbAwbDebugOn,    , "Done____"	, 15, 24, gnAwbErr	)
//	DebugDisp(gbAwbDebugOn,    , "nRGain"	, 15, 24, nRGain	)
//	DebugDisp(gbAwbDebugOn,    , "nBGain"	, 16, 24, nBGain	)
#else
	#define  AWB_DEBUG_X	24
	DebugDisp2(gbAwbDebugOn, Dec, "TCNT"  ,  1, AWB_DEBUG_X, nTcnt,  8)
	DebugDisp2(gbAwbDebugOn, Dec, "CRDR  ",  2, AWB_DEBUG_X, iDrSum, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "CY    ",  3, AWB_DEBUG_X, iDgSum, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "CBDB  ",  4, AWB_DEBUG_X, iDbSum, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "R     ",  6, AWB_DEBUG_X, nRsum, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "G     ",  7, AWB_DEBUG_X, nGsum, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "B     ",  8, AWB_DEBUG_X, nBsum, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "R_GAIN", 10, AWB_DEBUG_X, nAwbRgain, 4)	// IIR(nRGain) + MENU(R-GAIN)
	DebugDisp2(gbAwbDebugOn, Dec, "B_GAIN", 11, AWB_DEBUG_X, nAwbBgain, 4)	// IIR(nBGain) + MENU(B-GAIN)
	DebugDisp2(gbAwbDebugOn, Dec, "WGT   ", 13, AWB_DEBUG_X, iWgt, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "HOLD  ", 14, AWB_DEBUG_X, nHold, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "DONE  ", 15, AWB_DEBUG_X, gnAwbErr, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "RGAIN0", 17, AWB_DEBUG_X, nRGain, 4)
	DebugDisp2(gbAwbDebugOn, Dec, "BGAIN0", 18, AWB_DEBUG_X, nBGain, 4)
#endif

}	// end of AWB

