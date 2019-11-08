/* **************************************************************************
 File Name	:	isp_lib_ae_adv.c
 Description:	EN673 - Adv. AE
 Designer	:	Kim, Sunghoon
 Date		:	18. 6. 14
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "isp_lib_main.h"

#define AE_SAT_CLIP_MIN		200
#define AE_SAT_CLIP_MAX		1000

#define IIG					13
#define IKG					7
#define IIRI(Y,X)			Y = (X)<<(IIG+IKG)	// 최대값 = 2047 = 2^11 (11 = 32 - IIG - IKG - 1(sign)), UINT형은 4095
#define IIRR(Y,K,X,YP)		Y = (K)*((X)<<IIG) + ((1<<IKG)-(K))*(YP>>IKG)
#define IIROu(Y)			(((Y)>>(IIG+IKG)) + (((Y)>>(IIG+IKG-1))&1))

extern int giShtMin;
extern int giShtMax;
extern int giAgcMin;
extern int giAgcMax;
extern int giShtDblMax;
extern int giAgcDblMax;

extern int giIrsCent;
extern int giIrsPos;
extern int giErr2d;

#define SHT_PRT(...)	//printf(__VA_ARGS__)
#define AGC_PRT(...)	//printf(__VA_ARGS__)
#define IRS_PRT(...)	//printf(__VA_ARGS__)

#if 0
const WORD	gwShtLut[] = {
	279,280,
	281,282,283,284,285,286,287,288,289,290,
	291,292,293,294,295,296,297,298,299,300,
	301,302,303,304,305,306,307,308,309,310,
	311,312,313,314,315,316,317,318,319,320,
	321,322,323,324,325,326,327,328,329,330,
	331,332,333,334,335,336,337,338,339,340,
	341,342,343,344,345,346,347,348,349,350,
	351,352,353,354,355,356,357,358,359,360,
	361,362,363,364,365,366,367,368,369,370,
	371,372,373,374,375,376,377,378,379,381,
	382,383,384,385,386,387,388,390,391,392,
	393,394,395,397,398,399,400,402,403,404,
	405,407,408,409,410,412,413,414,416,417,
	418,420,421,422,424,425,426,428,429,431,
	432,434,435,436,438,439,441,442,444,445,
	447,448,450,451,453,455,456,458,459,461,
	463,464,466,468,469,471,473,474,476,478,
	480,481,483,485,487,489,490,492,494,496,
	498,500,502,504,506,508,510,511,514,516,
	518,520,522,524,526,528,530,532,534,537,
	539,541,543,546,548,550,553,555,557,560,
	562,564,567,569,572,574,577,579,582,585,
	587,590,593,595,598,601,604,606,609,612,
	615,618,621,624,627,630,633,636,639,642,
	645,648,652,655,658,661,665,668,672,675,
	679,682,686,689,693,697,700,704,708,712,
	716,720,724,728,732,736,740,744,748,753,
	757,762,766,771,775,780,784,789,794,799,
	804,809,814,819,824,829,834,840,845,851,
	856,862,868,873,879,885,891,897,903,910,
	916,923,929,936,942,949,956,963,970,978,
	985,992,1000,1008,1016,1023,1032,1040,1048,1057,
	1065,1074,1083,1092,1101,1110,1120,1129,1139,1149,
	1159,1170,1180,1191,1202,1213,1224,1236,1248,1260,
	1272,1285,1297,1310,1323,1337,1351,1365,1379,1394,
	1409,1424,1440,1456,1472,1489,1506,1524,1542,1560,
	1579,1598,1618,1638,1659,1680,1702,1724,1747,1771,
	1795,1820,1846,1872,1899,1927,1956,1985,2016,2047,
	2080,2114,2148,2184,2221,2259,2299,2340,2383,2427,
	2473,2520,2570,2621,2674,2730,2788,2849,2912,2978,
	3048,3120,3196,3276,3360,3449,3542,3640,3744,3855,
	3971,4095,4228,4369,4519,4681,4854,5041,5242,5461,
	5698,5957,6241,6553,6898,7281,7710,8191,8738,9362,
	10082,10922,11915,13107,14563,16383,18724,21845,26214,32767,
	43690,65535,
};

#define SHT_LUT_DEL		378
#define SHT_LUT_EA		(ARRAY_SIZE(gwShtLut)+SHT_LUT_DEL)
#define SHT_LUT(I)		(((I)>SHT_LUT_DEL) ? gwShtLut[(I)-SHT_LUT_DEL] : (I)+1)

int ISRT LinePos0(const BYTE abCH, const int aiLine)		// 131070 is MAX of LinePos0(...)
{
	static int iLine1d_0 = 0;
	static int iIdx1d_0 = 0;
	static int iLine1d_1 = 0;
	static int iIdx1d_1 = 0;

	int* piLine1d = (abCH) ? &iLine1d_1 : &iLine1d_0 ;
	int* piIdx1d = (abCH) ? &iIdx1d_1 : &iIdx1d_0 ;

	#define iLine1d		(*piLine1d)
	#define iIdx1d		(*piIdx1d)

	if(aiLine<2) return 0;
	if(aiLine==2) return SHT_LUT(SHT_LUT_EA-1)<<1;
	if(aiLine<=381) return SHT_LUT(SHT_LUT_EA-1-aiLine+3);
	if(aiLine>=65537) return 1;

	if(iLine1d==aiLine) return SHT_LUT(SHT_LUT_EA-1-iIdx1d+1);

	int i, iLine;

	if(aiLine>=((1125+897)>>1))		{ iLine=1125; i=707; }
	else if(aiLine>=((897+668)>>1)) { iLine=897;  i=677; }
	else if(aiLine>=((668+532)>>1)) { iLine=668;  i=627; }
	else if(aiLine>=((532+442)>>1)) { iLine=532;  i=577; }
	else if(aiLine>=((442+382)>>1)) { iLine=442;  i=526; }
	else							{ iLine=382;  i=479; }

	const int idiff0 = (aiLine - iLine);
	const int idiff1 = (aiLine - iLine1d);

	if(ABS(idiff0) > ABS(idiff1)) {
		i = iIdx1d;
		iLine = iLine1d;
	}

	int cnt=0;
	const WORD wLine = aiLine - 2;

	if(iLine<aiLine) {
		for(;i<(int)SHT_LUT_EA; i++) {
			cnt++;
			/*if((cnt%50)==0)*/ //SHT_PRT("+%d\t%d\t%d\t%d\n", cnt, SHT_LUT(i), wLine+2, i);
			if(wLine < SHT_LUT(i)) break;
		}
	}
	else if(iLine>aiLine) {
		for(i--;i>=379; i--) {
			cnt++;
			//SHT_PRT("-%d\t%d\t%d\t%d\n", cnt, SHT_LUT(i), wLine, i);
			if(wLine >= SHT_LUT(i)) { i++; break; }
		}
	}

	SHT_PRT("LinePos Cnt: %d\t", cnt);

	iLine1d = aiLine;
	iIdx1d = i;

	return SHT_LUT(SHT_LUT_EA-1-i+1);
}
#endif

// 131070 is MAX of LinePos0(...), (0xffffffff/2) / 131070 = 16384 = 2^14
// 1024(AE error MAX) * 3000 (Gain MAX) = 3072000, (0xffffffff/2) / 3072000 = 699 ≥ 2^9
// giShtPos + iErrPos = 131070*2^9 + 1024*3000*2^9 = 67107840 + 1572864000 = 1639971840 ≤ (0xffffffff/2)
#define SHT_POS_GAB		9
//#define LinePos(CH,V)	(LinePos0(CH,V)<<SHT_POS_GAB)
//#define LinePos(CH,V)	(((65535<<1) / (V-1))<<SHT_POS_GAB)
#define LinePos(CH,V)	((65535<<(1+SHT_POS_GAB)) / (V-1))

BYTE SHT_DLY = 2;
BYTE AGC_DLY = 2;

WORD gwErrSpotSupGab = 0x10/*0x20*/;

const BYTE* gpbVHssAgcTbl = 0;
int giVHssAgcTblEa = 0;

int giShtPos  = 0;
int giShtVal  = 1125/*RP(FR_VTW60)*/;
int giShtPosN = 116<<SHT_POS_GAB;		// = LinePos0(1125)<<SHT_POS_GAB
int giShtPosP = 116<<SHT_POS_GAB;		// = LinePos0(1125+1)<<SHT_POS_GAB
BYTE gbShtDly = 0;
BYTE gbVHssAgc = 0;

int giShtSPos  = 0;
int giShtSVal  = 1125>>2/*RP(FR_VTW60)*/;	// 1125>>2 = 562
int giShtSPosN = 233<<SHT_POS_GAB;		// = LinePos0(1125>>2)<<SHT_POS_GAB
int giShtSPosP = 233<<SHT_POS_GAB;		// = LinePos0((1125>>2)+1)<<SHT_POS_GAB
BYTE gbShtSDly = 0;
BYTE gbVHssSAgc = 0;

#define AGC_VAL_GAB		16

int giAgcVal = 0;
int giAgcPos = 0;		// AGC_VAL_GAB 적용
int giAgcCtrl = 0;		// AGC_VAL_GAB 적용
BYTE gbAgcDly = 0;
//BYTE gbAgcOff = 0/*(AGC_DLY<<1)*/;
int giAgcUpSpd = 20;
int giAgcDnSpd = 200;

void SetAeDevDly(const BYTE aSHT_DLY, const BYTE aAGC_DLY)
{
	SHT_DLY = aSHT_DLY;
	AGC_DLY = aAGC_DLY;
}

void InitAeCtrl(const BYTE aSHT_DLY, const BYTE aAGC_DLY, const BYTE* apbVHssAgcTbl, const UINT aVHssAgcTblEa, const WORD awErrSpotSupGab)
{
	SetAeDevDly(aSHT_DLY, aAGC_DLY);

	gpbVHssAgcTbl = apbVHssAgcTbl;
	giVHssAgcTblEa = (int)aVHssAgcTblEa;
	gwErrSpotSupGab = awErrSpotSupGab;
}

#if 0
int ISRT _fsum3(const int iKaV, const int iKaU, const int iA,
				const int iKbV, const int iKbU, const int iB,
				const int iKcV, const int iKcU, const int iC)
{
	const float32 fhKp = _fdiv(_itof(iKpV), _itof(iKpU));
	const float32 fhKi = _fdiv(_itof(iKiV), _itof(iKiU));
	const float32 fhKd = _fdiv(_itof(iKdV), _itof(iKdU));

	//static float32 fhErrEstAc = 0;
	//fhErrEstAc += _fmul(TOFLOAT32(fKi), _itof(aiErr));
	const float32 fhErrEst = _fadd(_fmul(fhKd, _itof(iErrChg)), _fadd(_fmul(fhKp, _itof(aiErr)), /*fhErrEstAc*/_fmul(fhKi, _itof(iErrEstDly[AGC_DLY])) ));
}
#endif

int ISRT ShtCtrl(const BYTE abCH, const int aaiErr, const int aiMargin, const int aaiGain, const int aiShtMin, const int aiShtMax, const BYTE abEstOn, const BYTE abDlyOn, const BYTE abAgcOn, const int aiShtManual)
{
	#define SHT_MAX_ERR		1023
	#define SHT_MAX_GAIN	((0x7fffffff-(131070<<SHT_POS_GAB)) / SHT_MAX_ERR)		// = 2033602  (※131070 = 65535*2)

	const int aiErr = (aaiErr > SHT_MAX_ERR) ? SHT_MAX_ERR :
					  (aaiErr < (-SHT_MAX_ERR)) ? (-SHT_MAX_ERR) : aaiErr ;

	const int aiGain = (aaiGain > SHT_MAX_GAIN) ? SHT_MAX_GAIN :
					   (aaiGain < 1) ? 1 : aaiGain ;

	//--------------------------------------------------------------------------------
	//
	static int iErr1d_0 = 0;
	static int iErr1d_1 = 0;
	static int iErrEstDly_0[8/*SHT_DLY*/+1];
	static int iErrEstDly_1[8/*SHT_DLY*/+1];
	static int iErrEstAc_0 = 0;
	static int iErrEstAc_1 = 0;

	int* piErr1d;
	int* piErrEstDly;
	int* piErrEstAc;
	int* piShtPos;
	int* piShtVal;
	int* piShtPosN;
	int* piShtPosP;
	BYTE* pbShtDly;
	BYTE* pbVHssAgc;

	if(abCH) {
		piErr1d = &iErr1d_1;
		piErrEstDly = iErrEstDly_1;
		piErrEstAc = &iErrEstAc_1;
		piShtPos = &giShtSPos;
		piShtVal = &giShtSVal;
		piShtPosN = &giShtSPosN;
		piShtPosP = &giShtSPosP;
		pbShtDly = &gbShtSDly;
		pbVHssAgc = &gbVHssSAgc;
	}
	else {
		piErr1d = &iErr1d_0;
		piErrEstDly = iErrEstDly_0;
		piErrEstAc = &iErrEstAc_0;
		piShtPos = &giShtPos;
		piShtVal = &giShtVal;
		piShtPosN = &giShtPosN;
		piShtPosP = &giShtPosP;
		pbShtDly = &gbShtDly;
		pbVHssAgc = &gbVHssAgc;
	}

	#define iErr1d		(*piErr1d)
	#define iErrEstDly	piErrEstDly
	#define iErrEstAc	(*piErrEstAc)
	#define giShtPos	(*piShtPos)
	#define giShtVal	(*piShtVal)
	#define giShtPosN	(*piShtPosN)
	#define giShtPosP	(*piShtPosP)
	#define gbShtDly	(*pbShtDly)
	#define gbVHssAgc	(*pbVHssAgc)

	int iErrEstNew = 0;
	int i, iErrEst = 0;
	const int iErrChg = aiErr - iErr1d;
	iErr1d = aiErr;

	//--------------------------------------------------------------------------------
	// Manual & Min & Max
	if(aiShtManual || giShtVal < aiShtMin || giShtVal > aiShtMax) {
		const int iShtValNew = (aiShtManual) ? aiShtManual : (giShtVal < aiShtMin) ? aiShtMin : aiShtMax ;

		if(giShtVal != iShtValNew) {
			gbShtDly = SHT_DLY;
			giShtVal = iShtValNew;
			giShtPosN = LinePos(abCH, giShtVal);
			giShtPosP = LinePos(abCH, giShtVal+1);
			giShtPos = giShtPosP>>1;
		}
		else if(gbShtDly) gbShtDly--;

		SHT_PRT("! Sht:%d\tPos:%d\tLmt:%d\n", giShtVal, giShtPos, giShtPosP);
		iErr1d = 0;
		iErrEstAc = 0;
		for(i=0; i<=SHT_DLY; i++) iErrEstDly[i] = 0;
		if(aiShtManual) goto ShtCtrlEnd_2;
	}

	//--------------------------------------------------------------------------------
	// Agc Min
	extern int AgcCtrl(const int aiErr, const int aiMargin, const int aiGain, const int aiAgcMin, const int aiAgcMax, const BYTE abEstOn, const BYTE abDlyOn, const BYTE abManual, const int aiAgcManual);
	if(giShtVal>giVHssAgcTblEa && giAgcCtrl != (AGC_MIN<<AGC_VAL_GAB) && abAgcOn) AGC_SET(AGC_MIN);
	BYTE bAgcCtrlOn = 0;

	//--------------------------------------------------------------------------------
	// Delay & Margin
	for(i=0; i<SHT_DLY; i++) if(iErrEstDly[i]) break;
	const BYTE bLastDly = i;

	if(abDlyOn && bLastDly<SHT_DLY) gbShtDly = SHT_DLY - bLastDly;

	if(gbShtDly) { SHT_PRT("  Sht Dly:%d\n", gbShtDly); gbShtDly--; goto ShtCtrlEnd; }

	if(-aiMargin <= aiErr && aiErr <= aiMargin) { goto ShtCtrlEnd; }

	//--------------------------------------------------------------------------------
	// Error
	const int iShtVal1d = giShtVal;

#if 0
#if 0
	iErrEst = (bLastDly==SHT_DLY) || (abEstOn==0) ? /*(iErrEstDly[SHT_DLY] ? iErrChg + iErrEstDly[SHT_DLY] : 0) +*/ aiErr : iErrChg + iErrEstDly[SHT_DLY] ;
#else
  #if 0	// for Tuning
	const int iParK = ParAe(PA_IRS_CNT) ? ParAe(PA_IRS_CNT) : 550;

	const int iKpV = iParK;
	const int iKpU = 1000;

	const int iKiV = (iParK>>1);
	const int iKiU = 1000;

	const int iKdV = iParK;
	const int iKdU = 1000;

	const float32 fhKp = _fdiv(_itof(iKpV), _itof(iKpU));
	const float32 fhKi = _fdiv(_itof(iKiV), _itof(iKiU));
	const float32 fhKd = _fdiv(_itof(iKdV), _itof(iKdU));
  #else
	const float fKp = 0.55f;
	const float fKi = 0.275f;
	const float fKd = 0.55f;
	const float32 fhKp = TOFLOAT32(fKp);
	const float32 fhKi = TOFLOAT32(fKi);
	const float32 fhKd = TOFLOAT32(fKd);
  #endif
	//static float32 fhErrEstAc = 0;
	//fhErrEstAc += _fmul(TOFLOAT32(fKi), _itof(aiErr));
	const float32 fhErrEst = _fadd(_fmul(fhKd, _itof(iErrChg)), _fadd(_fmul(fhKp, _itof(aiErr)), /*fhErrEstAc*/_fmul(fhKi, _itof(iErrEstDly[SHT_DLY])) ));

	iErrEst = _ftoi(fhErrEst);
#endif
#else
	iErrEst = (550*aiErr + 275*iErrEstDly[SHT_DLY] + 550*iErrChg) / 1000;
#endif

	//GrpAe(GA_SHT_ERR_EST) = iErrEst;
	//GrpAe(GA_SHT_ERR_CHG) = iErrChg;
	//GrpAe(GA_SHT_ERR_EST_2) = iErrEstDly[2];
	//GrpAe(GA_SHT_ERR_EST_1) = iErrEstDly[1];
	//GrpAe(GA_SHT_ERR_EST_0) = iErrEstDly[0];

#if 1
	#define SHT_GAIN_MIN		(aiGain>>ParAe(PA_AGC_GAIN_MIN_BIT))
	#define SHT_GAIN_MAX		aiGain
	#define SHT_GAIN_MIN_ERR	((int)ParAe(PA_AGC_GAIN_MIN_ERR))
	#define SHT_GAIN_MAX_ERR	((int)ParAe(PA_AGC_GAIN_MAX_ERR))

	const int iErrAbs = ABS(iErrEst/*aiErr*/);
	const int iGain = (iErrAbs <= SHT_GAIN_MIN_ERR) ? SHT_GAIN_MIN :
					  (iErrAbs >= SHT_GAIN_MAX_ERR) ? SHT_GAIN_MAX :
					  SHT_GAIN_MIN + (SHT_GAIN_MAX-SHT_GAIN_MIN) * (iErrAbs - SHT_GAIN_MIN_ERR) / (SHT_GAIN_MAX_ERR - SHT_GAIN_MIN_ERR) ;

	//GrpAe(GA_SHT_ERR) = iErrAbs;
	//GrpAe(GA_SHT_GAIN) = iGain;

	const int iErrPos = (ParAe(PA_ERR_EST_OFF) ? aiErr : iErrEst) * iGain;
#else
	const int iErrPos = (ParAe(PA_ERR_EST_OFF) ? aiErr : iErrEst) * aiGain;
#endif

	//GrpAe(GA_SHT_ERR) = aiErr;

	//--------------------------------------------------------------------------------
	// Position
	if(iErrPos >= 0) {
		if(giShtVal==giVHssAgcTblEa && giAgcCtrl < (AGC_MIN<<AGC_VAL_GAB)) {
			bAgcCtrlOn = 1;
		}
		else if(giShtVal < aiShtMax) {
			giShtPos += iErrPos;	SHT_PRT("NewPos : %d\n", giShtPos);
			while(giShtPos >= giShtPosP) {
				giShtPos -= giShtPosP;
				giShtVal++;
				giShtPosP = LinePos(abCH, giShtVal+1);

				SHT_PRT("! Sht:%d\tPos:%d\tLmt:%d\n", giShtVal, giShtPos, giShtPosP);

				if(giShtVal == aiShtMax || giShtVal == giVHssAgcTblEa) { giShtPos = 0; SHT_PRT("    Pos : %d\n", giShtPos); gbShtDly = SHT_DLY; break; }
			}
			giShtPosN = LinePos(abCH, giShtVal);
		}
	}
	else {
		if(giShtVal==giVHssAgcTblEa && giAgcCtrl > 0) {
			bAgcCtrlOn = 1;
		}
		else if(giShtVal > aiShtMin || (giShtVal == aiShtMin && giShtPos > 0)) {
			giShtPos += iErrPos;	SHT_PRT("NewPos : %d\n", giShtPos);
			while(giShtVal > aiShtMin && giShtPos < 0) {
				giShtPos += giShtPosN;
				giShtVal--;
				giShtPosN = LinePos(abCH, giShtVal);

				SHT_PRT("! Sht:%d\tPos:%d\tLmt:%d\n", giShtVal, giShtPos, giShtPosN);

				//if(giShtVal == aiShtMin && giShtPos < 0) { giShtPos = 0; SHT_PRT("    Pos : %d\n", giShtPos); gbShtDly = SHT_DLY; break; }	// 주석처리 <- while() 조건에서 aiShtMin 확인

				if(giShtVal == giVHssAgcTblEa) { giShtPos = 0; SHT_PRT("    Pos : %d\n", giShtPos); gbShtDly = SHT_DLY; break; }
			}

			if(giShtVal == aiShtMin && giShtPos < 0) { giShtPos = 0; SHT_PRT("    Pos : %d\n", giShtPos); gbShtDly = SHT_DLY; }

			giShtPosP = LinePos(abCH, giShtVal+1);
		}
	}

	//GRP0 = aiErr;
	//GRP1 = iShtVal1d;
	//GRP2 = iErrChg;
	//GRP3 = iErrEstDly[SHT_DLY/*2*/];
	//GRP4 = iErrEstDly[1];
	//GRP5 = iErrEstDly[0];
	GRP6 = iErrEst;
	//GRP7 = iErrPos;

	iErrEstAc += iErrEst;

	if(iShtVal1d != giShtVal) {
		iErrEstNew = iErrEstAc;
		iErrEstAc = 0;
		if(ParAe(PA_ERR_EST_OFF) || abDlyOn) gbShtDly = SHT_DLY;
	}
	else if(giShtVal <= aiShtMin || giShtVal >= aiShtMax) iErrEstAc = 0;
	//else iErrEst = 0;

ShtCtrlEnd:

	for(i=SHT_DLY; i>0; i--) iErrEstDly[i] = (abEstOn) ? iErrEstDly[i-1] : 0 ;
	iErrEstDly[0] = (abEstOn) ? iErrEstNew/*iErrEst*/ : 0 ;

	if(abAgcOn) {
		if(bAgcCtrlOn)	AgcCtrl(aiErr, aiMargin, (aiErr<0) ? (ParAe(PA_AGC_DOWN_SPEED) ? ParAe(PA_AGC_DOWN_SPEED) : 200) : (ParAe(PA_AGC_UP_SPEED) ? ParAe(PA_AGC_UP_SPEED) : 20), 0, AGC_MIN, abEstOn, 0, 0, 0);
		else			AgcCtrl(0, aiMargin, 0, 0, AGC_MIN, abEstOn, 0, 0, 0);	// AGC_DLY 감소용
	}

	//for(i=0; i<=SHT_DLY; i++) SHT_PRT("  >> iErrEstDly[%d] = %d\n", i,  iErrEstDly[i]);

ShtCtrlEnd_2:

	if (giShtVal<giVHssAgcTblEa)	gbVHssAgc = ((giShtPos>>SHT_POS_GAB) * (gpbVHssAgcTbl[giShtVal]+1)) / (giShtPosP>>SHT_POS_GAB);//LibUtlInterp1D(giShtPos, 0, giShtPosP, 0, gbVHssAgcTbl[giShtVal]+1);
	else 							gbVHssAgc = 0;

	//GrpAe(GA_SHT_POS) = giShtPos;
	//GrpAe(GA_SHT_POS_N) = gbVHssAgc/*giShtPosN*/;
	//GrpAe(GA_SHT_POS_P) = giShtPosP;
	//GrpAe(GA_ERR_NIGHT_CNT) = AGC_MIN;

	/*GRP0 = iShtVal1d;
	GRP1 = giShtVal;
	GRP2 = aiShtMax;
	GRP3 = aiShtMin;*/

	GRP4 = iErrEstAc;
	GRP5 = iErrEstNew;
	GRP7 = giShtVal;
	//if(abCH==0) UartTxGrp();

	return giShtVal;
}

#undef iErr1d
#undef iErrEstDly
#undef iErrEstAc
#undef giShtPos
#undef giShtVal
#undef giShtPosN
#undef giShtPosP
#undef gbShtDly
#undef gbVHssAgc

int ISRT AgcCtrl(const int aaiErr, const int aiMargin, const int aaiGain, const int aiAgcMin, const int aiAgcMax, const BYTE abEstOn, const BYTE abDlyOn, const BYTE abManual, const int aiAgcManual)
{
	#define AGC_MAX_ERR		1023
	#define AGC_MAX_GAIN	((0x7fffffff-((0x400+0x2E00)<<AGC_VAL_GAB)) / AGC_MAX_ERR)	// = 1279201	!!! aiAgcMax 의 최대값은 (0x400+0x2E00) !!!

	const int aiErr = (aaiErr > AGC_MAX_ERR) ? AGC_MAX_ERR :
					  (aaiErr < (-AGC_MAX_ERR)) ? (-AGC_MAX_ERR) : aaiErr ;

	const int aiGain = (aaiGain > AGC_MAX_GAIN) ? AGC_MAX_GAIN :
					   (aaiGain < 1) ? 1 : aaiGain ;

	/*GRP0 = aiErr;
	GRP1 = aiMargin;
	GRP2 = aiGain;
	GRP3 = aiAgcMin;
	GRP4 = aiAgcMax;
	GRP5 = abEstOn;
	GRP6 = abDlyOn;
	GRP7 = abManual;
	UartTxGrp();*/
	//--------------------------------------------------------------------------------
	//
	static int iErr1d = 0;
	static int iErrEstDly[8/*AGC_DLY*/+1];
	static int iErrEstAc = 0;

	int iErrEstNew = 0;
	int i, iErrEst = 0;
	const int iErrChg = aiErr - iErr1d;
	iErr1d = aiErr;

	//--------------------------------------------------------------------------------
	// Manual
	if(abManual) {
		giAgcCtrl = (aiAgcManual<<AGC_VAL_GAB) /*+ (1<<(AGC_VAL_GAB-1))*/;

		if(giAgcVal != aiAgcManual) gbAgcDly = AGC_DLY;
		else if(gbAgcDly) gbAgcDly--;

		iErr1d = 0;
		iErrEstAc = 0;
		for(i=0; i<=AGC_DLY; i++) iErrEstDly[i] = 0;
		goto AgcCtrlEnd_2;
	}

	//--------------------------------------------------------------------------------
	// Delay & Margin
	for(i=0; i<AGC_DLY; i++) if(iErrEstDly[i]) break;
	const BYTE bLastDly = i;

	if(abDlyOn && bLastDly<AGC_DLY) gbAgcDly = AGC_DLY - bLastDly;

	if(gbAgcDly) { AGC_PRT("  Agc Dly:%d", gbAgcDly); gbAgcDly--; goto AgcCtrlEnd; }

	if(-aiMargin <= aiErr && aiErr <= aiMargin) { goto AgcCtrlEnd; }

	//--------------------------------------------------------------------------------
	// Error
	//if(giAgcVal == 0) gbAgcOff = ParAe(PA_AGC_OFF_CNT);// ? ParAe(PA_AGC_OFF_CNT) : (AGC_DLY<<1) ;
	//else if(gbAgcOff) gbAgcOff--;
	//GrpAe(GA_AGC_OFF) = gbAgcOff;

	//GrpAe(GA_AGC_OFF) = iErrEstDly[AGC_DLY];

#if 0
#if 0
	iErrEst = (bLastDly==AGC_DLY) || (abEstOn==0) ? /*(iErrEstDly[AGC_DLY] ? iErrChg + iErrEstDly[AGC_DLY] : 0) +*/ aiErr : iErrChg + iErrEstDly[AGC_DLY] ;
#else
  #if 0	// for Tuning
	const int iParK = ParAe(PA_IRS_CNT) ? ParAe(PA_IRS_CNT) : 270;

	const int iKpV = iParK;
	const int iKpU = 1000;

	const int iKiV = (iParK>>1);
	const int iKiU = 1000;

	const int iKdV = iParK;
	const int iKdU = 1000;

	const float32 fhKp = _fdiv(_itof(iKpV), _itof(iKpU));
	const float32 fhKi = _fdiv(_itof(iKiV), _itof(iKiU));
	const float32 fhKd = _fdiv(_itof(iKdV), _itof(iKdU));
  #else
	const float fKp = 0.27f;
	const float fKi = 0.135f;
	const float fKd = 0.27f;
	const float32 fhKp = TOFLOAT32(fKp);
	const float32 fhKi = TOFLOAT32(fKi);
	const float32 fhKd = TOFLOAT32(fKd);
  #endif
	//static float32 fhErrEstAc = 0;
	//fhErrEstAc += _fmul(TOFLOAT32(fKi), _itof(aiErr));
	const float32 fhErrEst = _fadd(_fmul(fhKd, _itof(iErrChg)), _fadd(_fmul(fhKp, _itof(aiErr)), /*fhErrEstAc*/_fmul(fhKi, _itof(iErrEstDly[AGC_DLY])) ));

	//const float fKp = (float)iKpV/(float)iKpU;
	//const float fKi = (float)iKiV/(float)iKiU;
	//const float fKd = (float)iKdV/(float)iKdU;
	//const float fErrEst = (fKd*(float)iErrChg) + ((fKp*(float)aiErr) + (fKi*(float)iErrEstDly[AGC_DLY]));
	//printf("%f = %f\n", TOFLOAT(fhErrEst), fErrEst);

	iErrEst = _ftoi(fhErrEst);
#endif
#else
	iErrEst = (270*aiErr + 135*iErrEstDly[AGC_DLY] + 270*iErrChg) / 1000;
#endif

	//GrpAe(GA_AGC_ERR_CHG) = iErrChg;
	//GrpAe(GA_AGC_ERR_EST_2) = iErrEstDly[2];

	#define AGC_CTRL_EST_OFF	(ParAe(PA_ERR_EST_OFF) || (0/*1*//*UP(BackLight)!=UP_BL_BLC*/))

#if 1
	#define AGC_GAIN_MIN		(aiGain>>ParAe(PA_AGC_GAIN_MIN_BIT))
	#define AGC_GAIN_MAX		aiGain
	#define AGC_GAIN_MIN_ERR	((int)ParAe(PA_AGC_GAIN_MIN_ERR))
	#define AGC_GAIN_MAX_ERR	((int)ParAe(PA_AGC_GAIN_MAX_ERR))

	const int iErrAbs = ABS(iErrEst/*aiErr*/);
	const int iGain = (iErrAbs <= AGC_GAIN_MIN_ERR) ? AGC_GAIN_MIN :
					  (iErrAbs >= AGC_GAIN_MAX_ERR) ? AGC_GAIN_MAX :
					  AGC_GAIN_MIN + (AGC_GAIN_MAX-AGC_GAIN_MIN) * (iErrAbs - AGC_GAIN_MIN_ERR) / (AGC_GAIN_MAX_ERR - AGC_GAIN_MIN_ERR) ;

	//GrpAe(GA_AGC_ERR) = iErrAbs;
	//GrpAe(GA_AGC_GAIN) = iGain;

	const int iErrPos = ((AGC_CTRL_EST_OFF ? aiErr : iErrEst) * iGain) /*>> (gbAgcOff>0)*/;
#else
	const int iErrPos = ((AGC_CTRL_EST_OFF ? aiErr : iErrEst) * aiGain) /*>> (gbAgcOff>0)*/;
#endif

	//--------------------------------------------------------------------------------
	// Position
	//if(iErrPos < 0) iErrPos = CLAMP(iErrPos, MIN(-(1<<AGC_VAL_GAB), -(giAgcCtrl>>1)), -1);
	//else			iErrPos = CLAMP(iErrPos, 1, MAX(1<<AGC_VAL_GAB, giAgcCtrl>>1));

	giAgcCtrl += iErrPos;

	GRP0 = aiErr;
	GRP1 = giAgcCtrl;
	GRP2 = iErrChg;
	GRP3 = iErrEstDly[AGC_DLY/*2*/];
	//GRP4 = iErrEstDly[1];
	//GRP5 = iErrEstDly[0];
	GRP6 = iErrEst;
	//GRP7 = iErrPos;

AgcCtrlEnd:

	//GrpAe(GA_ERR_DAY) = aiAgcMin;

	giAgcCtrl = CLAMP(giAgcCtrl, (aiAgcMin<<AGC_VAL_GAB), (aiAgcMax<<AGC_VAL_GAB));

	const int iAgcValNow = giAgcCtrl>>AGC_VAL_GAB;

	iErrEstAc += iErrEst;

	if(giAgcVal != iAgcValNow) {
		iErrEstNew = iErrEstAc;
		iErrEstAc = 0;
		if(AGC_CTRL_EST_OFF || abDlyOn || iAgcValNow==aiAgcMin || iAgcValNow==aiAgcMax) gbAgcDly = AGC_DLY;
	}
	else if(iAgcValNow <= aiAgcMin || iAgcValNow >= aiAgcMax) iErrEstAc = 0;
	//else iErrEst = 0;

	GRP0 = giAgcVal;
	GRP1 = iAgcValNow;
	GRP2 = aiAgcMax;
	GRP3 = aiAgcMin;

	for(i=AGC_DLY; i>0; i--) iErrEstDly[i] = (abEstOn) ? iErrEstDly[i-1] : 0 ;
	iErrEstDly[0] = (abEstOn) ? iErrEstNew/*iErrEst*/ : 0 ;

AgcCtrlEnd_2:

	giAgcPos = giAgcCtrl - (AGC_MIN<<AGC_VAL_GAB);
	if(giAgcPos < 0) giAgcPos = 0;

	giAgcVal = giAgcCtrl>>AGC_VAL_GAB;

	//GrpAe(GA_AGC_OFF) = gbAgcDly;

	//GRP0 = aiErr;
	//GRP5 = giAgcCtrl;
	//GRP6 = giAgcVal;
	//GRP7 = aiGain/*giAgcPos*/;
	GRP4 = iErrEstAc;
	GRP5 = iErrEstNew;
	GRP7 = giAgcVal;
	//UartTxGrp();

	AGC_PRT("! Agc:%d\tPos:%d\n", giAgcVal, giAgcCtrl);
	return giAgcVal;
}

#define IRS_VAL_GAB		7

#define IRS_DLY			2	// 0 : 1Frame 후 AE ODM적용, 1 : 2Frame 후 AE ODM적용, 2 : 3Frame 후 AE ODM적용, 3 : 4Frame 후 AE ODM적용
#define IRS_MIN			0
#define IRS_MAX			0x3fe	// 1022

#define IRS_DLY_CTRL	0

int giIrsVal = 0;
BYTE gbIrsDly = 0;

int ISRT IrsCent(const int aiErr, const int aiMargin, const int aiIrisOpenMax, const int aiIrisCloseMax, const int aiIrsPosMax)
{
	const int iErrOn   = ABS(aiErr)>aiMargin;
	const int iErrSign = (aiErr < 0) ? -1 : 1 ;//aiErr/ABS(aiErr);
	const int iErrGain = (aiErr < 0) ? 3 :													// Close
						 (giIrsPos <= (aiIrsPosMax>>1)) ? 3 :							// Open Start
						 (giIrsPos <= (aiIrsPosMax>>1)+(aiIrsPosMax>>2)) ? 2 : 1 ;	// Open End

	static UINT gnIrsInitCnt;
	#define	IrsInitCntMax	500
	gnIrsInitCnt++;
	gnIrsInitCnt = MIN(gnIrsInitCnt, IrsInitCntMax);

	if (iErrOn) {
		// Center control -------------
		if (gnIrsInitCnt<IrsInitCntMax)	giIrsCent -= (iErrSign*(50-(gnIrsInitCnt/10)));				// initialize center
		else							giIrsCent -= (iErrSign*iErrGain);									// normal center
										giIrsCent = CLAMP(giIrsCent, aiIrisOpenMax<<6, aiIrisCloseMax<<6);

		//giIrsCent = 0x260;	// Center of Stop = PS4100:0x260, MN34227:0x254(0x242~0x248)
	}
	else {

	}

	return giIrsCent>>6;
}

int ISRT IrsCtrl(const int aiErr, const int aiMargin, const int aiGain, const int aiIrsCen, const BYTE abManual, const int aiIrsManual)
{
	static int giIrsCtrl = 0;		// IRS_VAL_GAB 적용
	static int giIrsErr = 0;
	//static int giIrsManual1d = -1;

	//--------------------------------------------------------------------------------
	// Manual
	if(abManual) {
#if IRS_DLY_CTRL
		if(giIrsVal != aiIrsManual) gbIrsDly = IRS_DLY;
		else if(gbIrsDly) gbIrsDly--;
#endif

	#if 0	// 사용하지 않는것을 권장
		if(aiIrsManual == 0) {
			if(giIrsManual1d != aiIrsManual) giIrsCtrl = (aiIrsCen-6)<<IRS_VAL_GAB;
			else							 giIrsCtrl -= (1<<(IRS_VAL_GAB-4));	// 천천히 16frame에 1씩 Close

			if(giIrsCtrl < 0) giIrsCtrl = 0;
		}
		else giIrsCtrl = (aiIrsManual<<IRS_VAL_GAB);

		giIrsManual1d = aiIrsManual;
	#else
		giIrsCtrl = (aiIrsManual<<IRS_VAL_GAB);
	#endif

		giIrsErr = 0;
		goto IrsCtrlEnd_2;
	}

	//--------------------------------------------------------------------------------
	// Delay & Margin
	const int iIrsCen = (aiIrsCen<<IRS_VAL_GAB);

#if IRS_DLY_CTRL
	if(gbIrsDly && ParAe(PA_AGC_OFF_CNT)) { IRS_PRT("  Irs Dly:%d", gbIrsDly); gbIrsDly--; giIrsCtrl = iIrsCen; goto IrsCtrlEnd; }
#endif

	if(-aiMargin <= aiErr && aiErr <= aiMargin && giIrsErr==0) { giIrsCtrl = iIrsCen; goto IrsCtrlEnd; }

	//--------------------------------------------------------------------------------
	// Error
	int iIrsCtrl = 0;
	int iIrsCtrlMin = 0;

#if IRS_DLY_CTRL
	//if(ParAe(PA_AGC_OFF_CNT)) {
	if(giIrsErr == 0) giIrsErr = (aiErr<<IRS_VAL_GAB);

	if(giIrsErr<0) {	// Close
		iIrsCtrl = ((aiIrsCen - IRS_MAX)<<IRS_VAL_GAB)/aiGain;
		iIrsCtrl = MAX(giIrsErr, iIrsCtrl);
		iIrsCtrlMin = (-3<<IRS_VAL_GAB);
	}
	else {				// Open
		iIrsCtrl = ((aiIrsCen - IRS_MIN)<<IRS_VAL_GAB)/aiGain;
		iIrsCtrl = MIN(giIrsErr, iIrsCtrl);
		iIrsCtrlMin = (3<<IRS_VAL_GAB);
	}
#else
	//} else {
	giIrsErr = aiErr;
	iIrsCtrl = giIrsErr;
	iIrsCtrlMin = ((giIrsErr<0) ? -3 : 3)<<IRS_VAL_GAB;
	//}
#endif

	giIrsErr -= iIrsCtrl;

#if IRS_DLY_CTRL
	if(giIrsErr==0 && ParAe(PA_AGC_OFF_CNT)) gbIrsDly = IRS_DLY;
#endif

#if 1
	#define IRS_GAIN_MIN		(aiGain>>ParAe(PA_IRS_GAIN_MIN_BIT))
	#define IRS_GAIN_MAX		aiGain
	#define IRS_GAIN_MIN_ERR	((int)ParAe(PA_IRS_GAIN_MIN_ERR))
	#define IRS_GAIN_MAX_ERR	((int)ParAe(PA_IRS_GAIN_MAX_ERR))

	const int iErrAbs = ABS(/*iErrEst*/aiErr);
	const int iGain = /*(iErrAbs <= 30) ? (aiGain>>(ParAe(PA_IRS_GAIN_MIN_BIT)+2)) :*/
					  (iErrAbs <= IRS_GAIN_MIN_ERR) ? IRS_GAIN_MIN :
					  (iErrAbs >= IRS_GAIN_MAX_ERR) ? IRS_GAIN_MAX :
					  IRS_GAIN_MIN + (IRS_GAIN_MAX-IRS_GAIN_MIN) * (iErrAbs - IRS_GAIN_MIN_ERR) / (IRS_GAIN_MAX_ERR - IRS_GAIN_MIN_ERR) ;

	//GrpAe(GA_IRS_ERR) = iErrAbs;
	//GrpAe(GA_IRS_GAIN) = iGain;

	iIrsCtrl *= iGain;
#else
	iIrsCtrl *= aiGain;
#endif

	giIrsCtrl = iIrsCen - iIrsCtrlMin - iIrsCtrl;

IrsCtrlEnd:

	giIrsCtrl = CLAMP(giIrsCtrl, (IRS_MIN<<IRS_VAL_GAB), (IRS_MAX<<IRS_VAL_GAB));

	//GrpAe(GA_IRS_ERR) = giIrsErr;
	//GrpAe(GA_IRS_CTRL) = (giIrsCtrl - iIrsCen)>>IRS_VAL_GAB;

	//GrpAe(GA_IRS_VAL0) = gbIrsDly;
	//GrpAe(GA_IRS_ERR) = giIrsErr;

IrsCtrlEnd_2:

	giIrsVal = giIrsCtrl>>IRS_VAL_GAB;

	//GrpAe(GA_IRS_VAL0) = giIrsVal;

	IRS_PRT("! Irs:%d\tErr:%d\tCtrl:%d\n", giIrsCtrl, giIrsErr, iIrsCtrl);
	return giIrsVal;
}

void ISRT IrsPos(const int aiErr, const int aiErrMgn, const int aiGain, const int aiIrsPosMin, const int aiIrsPosMax)
{
	const int iErrOn   = ABS(aiErr)>aiErrMgn;
	const int iErrDif  = aiErr-giErr2d;

	if(iErrOn)
	{
		// Position control (judge full open) -------------
		if (aiErr>0) {
			if (aiErr<=giErr2d)	giIrsPos += (aiGain / ((iErrDif*iErrDif)+1))+1;		// open status
			else 				giIrsPos += 1;
		}
		else if (aiErr<0) {
			if (aiErr>=giErr2d)	giIrsPos -= (aiGain / ((iErrDif*iErrDif)+1))+1;		// close status
			else 				giIrsPos -= 1;
		}
	}
	else {
		giIrsPos = ((giIrsPos * 240) + ((aiIrsPosMax>>1) * (256-240))) >> 8;
	}

	giIrsPos = CLAMP(giIrsPos, aiIrsPosMin, aiIrsPosMax);
}


#define ERR_CHT_MAX		UP(ErrChtMax)
#define ERR_CHT_CUT		UP(ErrChtMin)

char gcErrSign[ERR_CHT_SIZ];
BYTE gbErrSingPosIn = 0;
char gcErrSignAccu = 0;
BYTE gbErrChtOff = 0;

void ISRT ErrChtInit(const char acInit)
{
	int j;
	for(j=0; j<ERR_CHT_SIZ; j++)
	{
		gcErrSign[j] = acInit;
	}

	gbErrSingPosIn = 0;
	gcErrSignAccu = acInit*ERR_CHT_MAX;
	//gbErrChtOff = 0;
}

void ISRT ErrChtChg(void/*const BYTE abChtSiz*/)
{
	int j;
	//ERR_CHT_MAX = abChtSiz;
	const BYTE bErrSignPosOut = ((gbErrSingPosIn < ERR_CHT_MAX) ? ERR_CHT_SIZ + gbErrSingPosIn : gbErrSingPosIn) - ERR_CHT_MAX;
	gcErrSignAccu = 0;

	for(j=0; j<ERR_CHT_MAX; j++)
	{
		BYTE bErrSignPos = bErrSignPosOut + j;
		if(bErrSignPos >= ERR_CHT_SIZ) bErrSignPos -= ERR_CHT_SIZ;

		gcErrSignAccu += gcErrSign[bErrSignPos];
	}
}

int ISRT ErrChtGet(const int aiErr)
{
	if((aiErr>>31)^(gcErrSignAccu>>7)) return 0;

	const char cErrSignAccuAbs = ABS(gcErrSignAccu);

	if(cErrSignAccuAbs < ERR_CHT_CUT) return 0;

	//const char cErrSignAccuAbsTh = cErrSignAccuAbs ;
	//const char cErrSignAccuAbsTh = cErrSignAccuAbs - ERR_CHT_CUT + 1 ;

	//printf("Err:%3d, Sgn:%2d, Acu:%2d, Abs:%2d, Th:%2d", aiErr, cErrSignNew, gcErrSignAccu, cErrSignAccuAbs, cErrSignAccuAbsTh);

	//return (aiErr * cErrSignAccuAbsTh) / ERR_CHT_MAX;
	return (aiErr * 1/*cErrSignAccuAbsTh*/) / (ERR_CHT_MAX-ERR_CHT_CUT+1);
}

void ISRT ErrChtSet(const int aiErr, const int aiMargin)
{
	static BYTE bErrChtSiz = 0xff;
	if(bErrChtSiz != ERR_CHT_MAX) {
		bErrChtSiz = ERR_CHT_MAX;
		ErrChtChg();
	}

	const BYTE bErrSignPosOut = ((gbErrSingPosIn < ERR_CHT_MAX) ? ERR_CHT_SIZ + gbErrSingPosIn : gbErrSingPosIn) - ERR_CHT_MAX;

	//printf("ErrSign[%2d] = %2d ", bErrSignPosOut, gcErrSign[bErrSignPosOut]);

	const char cErrSignNew = (aiErr < -aiMargin) ? -1 : (aiErr > aiMargin) ? 1 : 0;
	gcErrSignAccu += cErrSignNew - gcErrSign[bErrSignPosOut];

	gcErrSign[gbErrSingPosIn++] = cErrSignNew;
	if(gbErrSingPosIn == ERR_CHT_SIZ) gbErrSingPosIn = 0;
}

int ISRT ErrNor(const int aiErr, const int aiCur, const int aiTgt, const BYTE abHalf, const UINT anPosGainBit, const UINT anNegGainBit)
{
	int iErr = aiErr;

	//iErr = (iErr>0) ? iErr<<1 : iErr;	// Speed equalizer
	//if(ParAe(PA_ERR_UP_GAIN)   && (iErr>0)) iErr *= ParAe(PA_ERR_UP_GAIN);
	//if(ParAe(PA_ERR_DOWN_GAIN) && (iErr<0)) iErr *= ParAe(PA_ERR_DOWN_GAIN);

	if(iErr > 0) {
		iErr <<= anPosGainBit;

		if(abHalf) {
			iErr = (iErr + (iErr<<8)/MAX(aiCur,16))>>1;
		}
		else {
			iErr = (iErr<<8)/MAX(aiCur,16);
		}

		iErr = MIN(iErr, ParAe(PA_ERR_UP_MAX) ? (int)ParAe(PA_ERR_UP_MAX) : 1045);
	}
	else {
		iErr <<= anNegGainBit;
	}

	return iErr;
}

//int AE_ERR_MGN = 0x8;
int AE_ERR_MIN = 0x1;

int ISRT ErrMgn(const int aAE_ERR_MGN_MAX, const int aAE_ERR_MGN_MIN, const int aAE_ERR_MIN, const BYTE abTgtChgCnt)
{
	//AE_ERR_MGN = aAE_ERR_MGN;
	AE_ERR_MIN = aAE_ERR_MIN;

	static BYTE bMpLSpotLv1d = 0xff;
	static BYTE bMpSatBrt1d = 0xff;

	static BYTE bMpLSpotLvChgCnt = 0;

	extern BYTE gbTgtChg;
	if(gbTgtChg || (bMpSatBrt1d != UP(SatBrt))) bMpLSpotLvChgCnt = abTgtChgCnt;
	else if(bMpLSpotLv1d < UP(LSpotLv)) {
		bMpLSpotLvChgCnt = abTgtChgCnt;
		//bErrNightCnt = ERR_CHG_CNT_MAX;
		//bErrOfsCnt = ERR_CHG_CNT_MAX;
	}
	else if(bMpLSpotLv1d > UP(LSpotLv)) {
		bMpLSpotLvChgCnt = abTgtChgCnt;
		//bErrNightCnt = 0;
		//bErrOfsCnt = 0;
	}
	else if(bMpLSpotLvChgCnt) bMpLSpotLvChgCnt--;

	bMpLSpotLv1d = UP(LSpotLv);
	bMpSatBrt1d = UP(SatBrt);

	extern int iCurBgIIR;
	const int iCurBgIIRout = iCurBgIIR>>(IIG+IKG);
	const int AE_ERR_MGN = (iCurBgIIRout >= 150) ? aAE_ERR_MGN_MAX :
						   (iCurBgIIRout <=  50) ? aAE_ERR_MGN_MIN :
						   ((aAE_ERR_MGN_MAX-aAE_ERR_MGN_MIN)*(iCurBgIIRout-50)) / (150-50) + aAE_ERR_MGN_MIN ;

	return (bMpLSpotLvChgCnt) ? AE_ERR_MIN : ParAe(PA_ERR_MGN) ? (int)ParAe(PA_ERR_MGN) : AE_ERR_MGN ;
}

WORD gwLSPOT_TH[6] = {0, 0, 0, 0, 0, 0};

void ISRT LtPosThSet(void)
{
	static BYTE bMpLSpotTh = 0xff;
	if(bMpLSpotTh == UP(LSpotTh)) return;

	#define LT_SHT0		gwLSPOT_TH[0]
	#define LT_SHT1		gwLSPOT_TH[1]
	#define LT_SHT2		gwLSPOT_TH[2]
	#define LT_AGC0		gwLSPOT_TH[3]
	#define LT_AGC1		gwLSPOT_TH[4]
	#define LT_AGC2		gwLSPOT_TH[5]

	#define LT_STA0		(LT_SHT0 + (LT_AGC0<<3))
	#define LT_END0		(LT_SHT1 + (LT_AGC1<<3))
	#define LT_END1		(LT_SHT2 + (LT_AGC2<<3))

	if(UP(LSpotTh)==0) {
		LT_SHT0 = 0;
		LT_SHT1 = 0;
		LT_SHT2 = 0;
		LT_AGC0 = 0;
		LT_AGC1 = 0;
		LT_AGC2 = 0;
	}
	else {
		const WORD wThSta = (UP(LSpotTh)-1) * 80 + 5;
		LT_SHT0 = MIN(wThSta, SHT_MAX);
		LT_SHT1 = MIN(LT_SHT0+400, SHT_MAX);
		LT_SHT2 = MIN(LT_SHT1+400, SHT_MAX);
		LT_AGC0 = MAX((((int)wThSta-SHT_MAX)>>3), 0);
		LT_AGC1 = LT_AGC0 + 50 - 50*(LT_SHT1-LT_SHT0)/400;
		LT_AGC2 = LT_AGC1 + 50 - 50*(LT_SHT2-LT_SHT1)/400;
	}

	bMpLSpotTh = UP(LSpotTh);
}

int ISRT LtPos(const int aiVal0, const int aiVal1)
{
	int iVal;

	#define AGC_TO_SHT_BIT		5		// 5bit = 32 -> AGC:SHT=1:32
	//#define ERR_DAY_SUB_SHT		gwLSPOT_TH[0]//800		// SHT	// This must be less than SHT_MAX.
	//#define ERR_DAY_SUB_SHT_E	gwLSPOT_TH[1]//1125		// SHT	// This must be less than SHT_MAX.
	//#define ERR_NIGHT_OFF_AGC	gwLSPOT_TH[2]//0		// AGC	// This must be greater than AGC_MIN.
	//#define ERR_NIGHT_ON_AGC	gwLSPOT_TH[3]//(ParAe(PA_ERR_NIGHT_ON_AGC) ? (int)ParAe(PA_ERR_NIGHT_ON_AGC) : /*10*//*25*/50)		// AGC	// This must be greater than ERR_NIGHT_OFF_AGC.

	const int iLtPos = giShtVal + (giAgcPos>>(AGC_VAL_GAB-3));

	if(iLtPos > LT_END0) iVal = aiVal1;
	else if(iLtPos < LT_STA0) iVal = aiVal0;
	else {
		//const int iPosT = ((SHT_MAX  - ERR_DAY_SUB_SHT)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ((ERR_NIGHT_OFF_AGC - AGC_MIN)<<AGC_VAL_GAB);
		//const int iPosN = ((giShtVal - ERR_DAY_SUB_SHT)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ( giAgcPos          - (AGC_MIN<<AGC_VAL_GAB));
		const int iPosT = ((LT_SHT1  - LT_SHT0)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ((LT_AGC1 - LT_AGC0)<<AGC_VAL_GAB);
		const int iPosN = ((giShtVal - LT_SHT0)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ( giAgcPos          - (LT_AGC0<<AGC_VAL_GAB));

		iVal = (aiVal1 - aiVal0) * iPosN / iPosT + aiVal0;
	}

	return iVal;
}

int iCurBg=0, iErrBg=0;
int iTgtSpotBg = 0;
int iCurBgSum1=0, iErrBgSum1=0;
int iCurSpot=0, iTgtSpot=0;

int ISRT CurBg(const int aiTgt, const UINT nSlicLvl, const UINT nSlicCnt, const UINT nClipLvl, const UINT nClipCnt, const UINT nSum1, const UINT nPxCnt, const int aiTgtMin)
{
	// BLC
	//const UINT nClipCntBgCut = nAe2PxCnt - MAX(nAe2PxCnt>>5, 1);
	//const UINT nAe2ClipCntBg = (nAe2ClipCnt > nClipCntBgCut) ? nAe2ClipCnt - nClipCntBgCut : 0 ;
	//iCurBg = (int)((((nAe2SlicCnt-nAe3SlicCnt)*nAe2SlicLvl) + (nAe2Sum1-nAe3Sum1) + ((nAe2ClipCnt-nAe3ClipCnt)*nAe2ClipLvl)) / (nAe2PxCnt-nAe3PxCnt));
	//iCurBg = (int)(((nAe2SlicCnt*nAe2SlicLvl) + nAe2Sum1 + (nAe2ClipCntBg*nAe2ClipLvl)) / (nAe2PxCnt-nAe2ClipCnt+nAe2ClipCntBg));

	// NOR
	//const UINT nClipCntBgCut = nAe3PxCnt - MAX(nAe3PxCnt>>5, 1);
	//const UINT nAe3ClipCntBg = (nAe3ClipCnt > nClipCntBgCut) ? nAe3ClipCnt - nClipCntBgCut : 0 ;
	//iCurBg = iCur;
	//iErrBg = iErr;
	//iCurBg = (int)(((nAe3SlicCnt*nAe3SlicLvl) + nAe3Sum1 + (nAe3ClipCntBg*nAe3ClipLvl)) / (nAe3PxCnt-nAe3ClipCnt+nAe3ClipCntBg));

	iCurBg = udiv4x((nSlicCnt*nSlicLvl) + nSum1 + (nClipCnt*nClipLvl), nPxCnt, 0);
	iErrBg = aiTgt - iCurBg;

	iCurBgSum1 = udiv4x((nSlicCnt*nSlicLvl) + nSum1, (nPxCnt-nClipCnt), 0);
	iErrBgSum1 = aiTgt - (iCurBgSum1);

#if 1
	extern int TgtSpotBg(const int aiTgt, const BYTE abV, const int aiTgtMin);
	iTgtSpotBg = TgtSpotBg(aiTgt, UP(SatBrt), aiTgtMin);
#else
	iTgtSpotBg = UP(SatBrt) * (aiTgt - aiTgtMin);
	iTgtSpotBg = (aiTgt - aiTgtMin) - (((iTgtSpotBg<<2)-iTgtSpotBg+32)>>6);
	if(iTgtSpotBg < 0) iTgtSpotBg = 0;
#endif

	return MIN(0xC,iTgtSpotBg>>1);
}

void ISRT CurSat(const UINT anClipLvl, const UINT nClipCnt, const UINT nSum2, const int iTgtOfs)
{
	const UINT nClipLvl = anClipLvl<<2;	// AE 8BIT	nClipLvl는 10bit 이므로 8bit인 nSum2와 가감연산 시 주의!!!
	const int iSum2Avg = udiv4x(nSum2, nClipCnt, nClipLvl);

	const int iClipCntOff = AE_SAT_CLIP_MIN/*nAe3PxCnt>>7*//*AE_LED_MAX_SHIFT_BIT*/;
	const int iClipCntON  = AE_SAT_CLIP_MAX/*nAe3PxCnt>>5*//*4*//*AE_LED_MIN_SHIFT_BIT*/;
	//GrpAe(GA_ERR_DAY_ORI) = nClipCnt;

	const int iClipCntCut = CLAMP((int)nClipCnt,iClipCntOff,iClipCntON);
	iCurSpot = LibUtlInterp1D(iClipCntCut, iClipCntOff, iClipCntON, nClipLvl, iSum2Avg);
	iTgtSpot = (int)nClipLvl + 1 + iTgtOfs;
	//GrpAe(GA_TGT_NIGHT) = *apiTgtNight;

	//if(*apiTgtNight < *apiCurNight) *apiCurNight = LibUtlInterp1D(iClipCntCut, iClipCntOff, iClipCntON, iTgtSpot, iCurSpot);

	//const int iErrNightMax = iTgtSpot - (int)nAe3ClipLvl;
}

int iErrBg1dIIR = 0;	// Cur가 낮은 상태에서의 Err /= Cur 된 Err가 Cur보다 분별력이 좋음
int iCurBgIIR = 0;

//int iCurBgSum1IIR = 0;
//int iCurSpotIIR = 0;

#define ERR_CHG_CNT_BIT		UP(ErrChgStep)/*3*//*4*//*5*/
#define ERR_CHG_CNT_MAX		(1<<ERR_CHG_CNT_BIT)
BYTE bErrNightCnt = 0;//(1<<5)/*ERR_CHG_CNT_MAX*/>>1;
BYTE bErrOfsCnt   = 0;//(1<<5)/*ERR_CHG_CNT_MAX*/>>1;

void ISRT SatCtrl(const BYTE abSatOff, int *apiErr, const int iErrMgn, const int nClipCnt)
{
	//static BYTE bErrChgCnt = 0;
	static int iCurSpotEst = 0;
	//static int iErrBg1dIIR = 0;	// Cur가 낮은 상태에서의 Err /= Cur 된 Err가 Cur보다 분별력이 좋음
	//static int iCurBgIIR = 0;

	//static BYTE bErrNightCnt = ERR_CHG_CNT_MAX>>1;
	//static BYTE bErrOfsCnt   = ERR_CHG_CNT_MAX>>1;

	IIRR(iCurBgIIR,2/*1*//*10*/,iCurBg,iCurBgIIR);	// 10:0.5sec, 6:1sec, 4:2sec

	if(abSatOff) {
		//bErrNightCnt = 0;
		//bErrOfsCnt = 0;
		return;
	}

	// -------------------------------------------------------------------------------
	iErrBg = ErrNor(iErrBg, iCurBg, 0, 0, 1, 0);

	int iErrSpotBg = iTgtSpotBg - iCurBg;
	iErrSpotBg = ErrNor(iErrSpotBg, iCurBg, 0, 0, 0, 0);

	iErrBgSum1 = ErrNor(iErrBgSum1, iCurBgSum1, 0, 0, 1, 0);

	const int iErrBg1dOut = iErrBg1dIIR>>(IIG+IKG);
	IIRR(iErrBg1dIIR,/*34*/18,iErrBgSum1/*iErrBg*/,iErrBg1dIIR);	// 10:15frame, 6:30frame, 4:60frame
	//IIRI(iErrBg1dIIR,iErrBg);

	//IIRR(iCurBgIIR,2/*1*//*10*/,iCurBg,iCurBgIIR);	// 10:0.5sec, 6:1sec, 4:2sec
	//IIRI(iCurBgIIR,iCurBg);
	//const int iCurIIRout = IIROu(iCurBgIIR);

	//IIRR(iCurBgSum1IIR,10,iCurBgSum1,iCurBgSum1IIR);	// 10:0.5sec, 6:1sec, 4:2sec

	const int iErrBgChg = (iErrBgSum1/*iErrBg*/ - iErrBg1dOut)<<IIG;

	// -------------------------------------------------------------------------------
	//IIRR(iCurSpotIIR,18,iCurSpot,iCurSpotIIR);	// 10:0.5sec, 6:1sec, 4:2sec
	//iCurSpot = IIROu(iCurSpotIIR);

	//const int iErrSpotRaw = iTgtSpot - iCurSpot;
	const int iCurSpotDiff = (iCurSpot<<IIG) - iCurSpotEst;
	const int iSpotDiffMrgn = /*(UP(ClipMax)) ? (iErrMgn<<IIG) :*/ (iErrMgn<<IIG)/*<<2*/ ;		// CAUTION ! -> 8 x 4 = 32	, iCurSpotIIR 사용 시 <<2 필요 없음
	const BYTE bCurSpotNoDiff = (ABS(iCurSpotDiff) <= iSpotDiffMrgn);

	ErrChtSet(iCurSpotDiff, iSpotDiffMrgn);

	//if(ABS(gcErrSignAccu) >= ERR_CHT_CUT && bCurSpotNoDiff) gbErrChtOff = 1;
	if(ABS(gcErrSignAccu) >= ERR_CHT_CUT) {
		if(bCurSpotNoDiff || ((iCurSpotDiff>>31)^(gcErrSignAccu>>7))) gbErrChtOff = 1;	// bCurSpotNoDiff : 만나는 경우, ((iCurSpotDiff>>31)^(gcErrSignAccu>>7)) : 방향이 바뀌는 경우
	}

	BYTE bCurSpotEstInitOn = (bCurSpotNoDiff && (gbErrChtOff == 0)) ? 1 :
							 //(ABS(iErrSpotRaw) <= iErrMgn)		  ? 2 :		// 극악조건에서 헌팅방지를 위해 사용
							 (UP(Stabilizing)==UP_4sOFF)		  ? 7 :
							 (iErrMgn==AE_ERR_MIN)				  ? 3 : 0 ;

	//GrpAe(GA_ERR_DAY_ORI) = bCurSpotEstInitOn;
	GRP2 = bCurSpotEstInitOn ;

	static BYTE bMpStabilizing = -1;	// AE Test
	if(bMpStabilizing != UP(Stabilizing)) {
		if(UP(Stabilizing)==UP_4sOFF && bCurSpotEstInitOn) UartTxStr("*Stabilizing OFF");
		else UartTxStr("*Stabilizing ON");
		bMpStabilizing = UP(Stabilizing);
	}

	if(bCurSpotEstInitOn) {
		gbErrChtOff = 0;
		//const char cErrSignInit = (iCurSpotDiff < 0) ? -1 : (iCurSpotDiff > 0) ? 1 : 0;
		ErrChtInit(0/*cErrSignInit*/);
		iCurSpotEst = (iCurSpot<<IIG);
	}
	else if(gbErrChtOff) {
		const char cErrSignInit = (iCurSpotDiff < 0) ? -1 : (iCurSpotDiff > 0) ? 1 : 0;
		ErrChtInit(cErrSignInit);

		//if(bCurSpotNoDiff)	iCurSpotEst = (iCurSpot<<IIG);
		//else				iCurSpotEst += ErrChtGet(iCurSpotDiff);
		iCurSpotEst = (iCurSpot<<IIG);
	}
	else {
		const int iErrChtGet = ErrChtGet(iCurSpotDiff);
		iCurSpotEst += iErrChtGet;

		//GrpAe(GA_TGT_MAX) = iErrChtGet;
		GrpAe(GA_TGT_MIN) = gcErrSignAccu;
	}

	//iErrSpot = iTgtSpot - (iCurSpotEst>>IIG);

	const int iCurSpotChg = iCurSpotEst - (iCurSpot<<IIG);

	//nAeClipCnt1d = nAe2ClipCnt;

//				if(iErrBg > (ERR_NIGHT_OFF_END-iErrMgn)) {
////					if(   ((iCurSpotEst > ((iCurSpot+iErrMgn)<<IIG)) && (iErrBg > (iErrBg1dOut+iErrMgn)))
//					if(   ((iCurSpotEst > ((iCurSpot+iErrMgn)<<IIG)) && (iErrBgChg<<1) >= iCurSpotChg)
//						|| (iCurSpotEst < ((iCurSpot-iErrMgn)<<IIG))
//					) iCurSpotEst = (iCurSpot<<IIG);
//				}

#if 0
	if(UP(BackLight)==UP_BL_BLC && UP(ClipMax)) {
		//iErrSpot = iErr;
		iErrSpot = ErrNor(iErrSpot, (iCurSpotEst>>IIG), 0, 0, 0, 0);
	}

	//iErrSpot = (iErrSpot<<7)/MAX(iCur,16);
	//iErrSpot = (iErrSpot + (iErrSpot<<8)/MAX(iCur,16))>>1;
#endif

	//const int iErrChtTh = ParAe(PA_ERR_CHT_TH) ? ParAe(PA_ERR_CHT_TH) : 60 ;

	bCurSpotEstInitOn = ((iCurSpotEst > ((iCurSpot+iErrMgn)<<IIG)) && (/*(iErrBgChg >=  (iErrChtTh<<(IIG+0))) ||*/ ((iErrBgChg /*+ (iErrBgChg>>1)*/) >= iCurSpotChg))) ? 4 :	// + : iErrBgChg >= iCurSpotChg
						((iCurSpotEst < ((iCurSpot-iErrMgn)<<IIG)) && (/*(iErrBgChg <= -(iErrChtTh<<(IIG+0))) ||*/ ((iErrBgChg /*+ (iErrBgChg>>1)*/) <= iCurSpotChg))) ? 5 : 0 ;// - : iErrBgChg <= iCurSpotChg*0.66 (=iCurSpotChg/1.5)

	//GrpAe(GA_ERR_DAY_ORI) = (GrpAe(GA_ERR_DAY_ORI)) ? GrpAe(GA_ERR_DAY_ORI) : bCurSpotEstInitOn ;
	GRP2 = (GRP2) ? GRP2 : bCurSpotEstInitOn ;

	if(bCurSpotEstInitOn) {
	#if 0
		gbErrChtOff = 0;
		ErrChtInit(0);
		/*if(nAe2ClipCnt > (AE_SAT_CLIP_MAX<<1))*/ iCurSpotEst = (iCurSpot<<IIG);
	#else
		gbErrChtOff = 1;

		const char cErrSignInit = (iCurSpotDiff < 0) ? -1 : (iCurSpotDiff > 0) ? 1 : 0;
		ErrChtInit(cErrSignInit);

		//if(bCurSpotNoDiff)	iCurSpotEst = (iCurSpot<<IIG);
		//else				iCurSpotEst += ErrChtGet(iCurSpotDiff);
		iCurSpotEst = (iCurSpot<<IIG);
	#endif
	}

	int iErrSpot = iTgtSpot - (iCurSpotEst>>IIG);	// iErrSpotRaw와 delay가 있으면 안되므로 iCurSpotEst 관련 설정이 모두 완료된 후 iErrSpot 설정

	AddDbgData(iErrBg,3);
	AddDbgData(iErrSpotBg,3);
	AddDbgData(iCurSpot,3);
	AddDbgData(iCurSpotEst>>IIG,3);
	AddDbgData(bCurSpotEstInitOn,1);

	//iErrBg1dIIR = iErrBg;

	//const int iErrNightOri0 = iErrSpot;
	//ErrChtSet(iErrSpot, iErrMgn);
	//ErrChtSet(IIROu(iCurSpotEst) - iCurSpot, iErrMgn);
	//iErrSpot = ErrChtGet(iErrSpot);

	GrpAe(GA_ERR_NIGHT) = (iCurSpotEst>>IIG);
	GrpAe(GA_AGC_VAL_PRE) = iCurSpot;
	//GrpAe(GA_ERR) = iErrBg/*iCurBg*/;
	//GrpAe(GA_ERR_NIGHT_ORI) = iErrSpot;
	//GrpAe(GA_CUR_SPOT_CHG) = iCurSpotChg>>IIG/*iErrBg1dOut*//*iCurIIRout*/;
	//GrpAe(GA_ERR_BG_CHG) = iErrBgChg>>IIG/*nClipCnt - nAeClipCnt1d*//*iErrBgSum1*//*iCurIIRout - iCurBg*//*iErrBgChg>>IIG*/;
	//GrpAe(GA_ERR_DAY_ORI) = iErrSpot;
	//GrpAe(GA_AGC_VAL_PRE) = iCurSpotChg>>IIG;
	//GrpAe(GA_ERR_CHT_PRE) = gcErrSignAccu/*iErrSpot*/;
	//GrpAe(GA_ERR_NIGHT_CNT) = iCurSpotChg>>IIG;
	//GrpAe(GA_ERR_NIGHT_CNT) = gcErrSignAccu;
	//GrpAe(GA_ERR_CHT_PRE) = iErrSpot;
	//GrpAe(GA_ERR_NIGHT_ORI) = iErrSpotRaw;
	//GrpAe(GA_ERR_NIGHT) = iTgtSpot - iCurSpot/*iErrSpotRaw*/;

	//GrpAe(GA_AGC_VAL_PRE) = giAgcVal + giShtVal - 125;
	//GrpAe(GA_ERR_NIGHT_CNT) = giAgcVal;
	//GrpAe(GA_ERR_NIGHT) = gbAgcDly*10 + gbShtDly;

	GRP4 = (iCurSpotEst>>IIG);
	GRP5 = iCurSpot;
	GRP6 = gcErrSignAccu;

	//----------------------------------------------------------------------------------------------------
	// Spot Ctrl Speed
	const int iErrSpotMgn = iErrMgn + 1 /*+ ParAe(PA_ERR_NIGHT_ON_AGC)*/;
	if(iErrSpot <= -iErrSpotMgn) {
		const BYTE bMpErrSpotSpd = UP(SpotSpd);//ParAe(PA_ERR_NIGHT_ON_AGC) ? ParAe(PA_ERR_NIGHT_ON_AGC) : 10 ;
		iErrSpot = ((((iErrSpot+iErrSpotMgn)*bMpErrSpotSpd)+10)/20) - iErrSpotMgn;	// 10 is used for rounding
	}

	AddDbgData(iErrSpot,3);

	//----------------------------------------------------------------------------------------------------
	const int iErrNightOri = iErrSpot;

	//const int iAgcPre = giAgcPos>>AGC_VAL_GAB;
	const int iLtPos = giShtVal + (giAgcPos>>(AGC_VAL_GAB-3));
	//GrpAe(GA_ERR_DAY_POS_N) = iLtPos;
	//GrpAe(GA_TGT_NIGHT) = LT_END0;

	const int iErrNeg = ((iErrBg < 0) ? iErrBg : 0) ;// - 1;
	const int iErrOfs = iErrSpotBg;//(iErrBg > ERR_NIGHT_OFF_END) ? (iErrBg - ERR_NIGHT_OFF_END)>>1 : (iErrBg < 0) ? iErrBg : 0 ;

	//const int iErrSpotSub = iErrNeg - (((iErrNeg - iErrSpot) * (iAgcPre - ERR_NIGHT_OFF_AGC)) / (ERR_NIGHT_ON_AGC - ERR_NIGHT_OFF_AGC));
	const int iErrSpotSub = iErrNeg - (((iErrNeg - iErrSpot) * (iLtPos - LT_END0)) / (LT_END1 - LT_END0));

	//----------------------------------------------------------------------------------------------------
	// Change 'iErrSpot',  Set 'bErrOfsCnt' & 'bErrNightCnt'
	if(UP(LSpotTh) && iErrSpot <= iErrNeg) {
		if     (iLtPos <  LT_END0) iErrSpot = iErrNeg;
		else if(iLtPos <= LT_END1) iErrSpot = iErrSpotSub;
	}

#ifdef AE_SPOT_DECR
	#define AE_SPOT_DECR_ON_VAL	AE_SPOT_DECR
#else
	#define AE_SPOT_DECR_ON_VAL	1
#endif

	extern int iTgt;
	const WORD ERR_NIGHT_OFF_STA = iTgt/*iTgtSpotBg+gwErrSpotSupGab*/;
	const WORD ERR_NIGHT_OFF_END = iTgtSpotBg;

	if(iErrSpot < 0 && 0 < iErrBg) {
		if(ParAe(PA_ERR_NIGHT_ON_AGC)==(!AE_SPOT_DECR_ON_VAL)) {
			//if(iCurBg<=ERR_NIGHT_OFF_END)		iErrSpot = -1;
			//else if(iCurBg<ERR_NIGHT_OFF_STA)	iErrSpot = (iErrSpot+1) * (iCurBg-ERR_NIGHT_OFF_END) / (ERR_NIGHT_OFF_STA-ERR_NIGHT_OFF_END) - 1;

			const int iErrSpotMin = iErrSpot < (-iErrMgn) ? (-iErrMgn) : iErrSpot;
			if(iCurBg<=ERR_NIGHT_OFF_END)		iErrSpot = iErrSpotMin;
			else if(iCurBg<ERR_NIGHT_OFF_STA)	iErrSpot = (iErrSpot-iErrSpotMin) * (iCurBg-ERR_NIGHT_OFF_END) / (ERR_NIGHT_OFF_STA-ERR_NIGHT_OFF_END) + iErrSpotMin;
		}
		//GrpAe(GA_CUR_NIGHT) = 1;
	}

	static BYTE b1d = -1;	// AE Test
	if(b1d != ParAe(PA_ERR_NIGHT_ON_AGC)) {
		if(ParAe(PA_ERR_NIGHT_ON_AGC)==(!AE_SPOT_DECR_ON_VAL)) UartTxStr("*ErrSpot Decrease ON");
		else UartTxStr("*ErrSpot Decrease OFF");
		//UartTxStrHex("PA_ERR_NIGHT_ON_AGC ", ParAe(PA_ERR_NIGHT_ON_AGC), 2);
		b1d = ParAe(PA_ERR_NIGHT_ON_AGC);
	}


	// (iErrMgn<<1) = 16, iErrSpot = CLIP+1+PA_TGT_NIGHT_OFS-1023 ~ CLIP+1+PA_TGT_NIGHT_OFS-CLIP = -159 ~ 65 이므로 (iErrMgn<<1)는 PA_TGT_NIGHT_OFS보다 작아야 함.
	const BYTE bErrNightOn = (iErrSpot < iErrBg) && (iErrSpot <= (iErrMgn<<1)/*< 0*/) && (iErrSpotBg <= -iErrMgn);	// 0일 경우 iErrSpot 광원이 안정적이지 않는 경우 0보다 커지면서 iErrBg값이 크면 헌팅 발생 함

	if(((iErrSpot < 0) && (iErrSpot < iErrBg)) || ((iErrSpot < (iErrMgn<<1)) && (iErrSpotBg > 0))) { if(bErrOfsCnt<ERR_CHG_CNT_MAX) bErrOfsCnt++; }
	else                                                                                           { if(bErrOfsCnt)                 bErrOfsCnt--; }

	if(bErrNightOn) { if(bErrNightCnt<ERR_CHG_CNT_MAX) bErrNightCnt++; }
	else            { if(bErrNightCnt)                 bErrNightCnt--; }


	AddDbgData(bErrNightOn,1);
	AddDbgData(iErrSpot,3);
	AddDbgData(bErrNightCnt,2);
	AddDbgData(bErrOfsCnt,2);

	//----------------------------------------------------------------------------------------------------
	// When the 'iErrSpot' is small, reduce the 'iErrBg'.
#if 0
	int iErrDis = 0;
	const int iErrNightTgtOfs = 1 + (ParAe(PA_TGT_NIGHT_OFS) ? (int)ParAe(PA_TGT_NIGHT_OFS) : 64);
	const int iErrNightAbs = ABS(iErrSpot);
	//iErrDis = (iErrNightAbs < iErrNightTgtOfs && iAgcPre > ((ERR_NIGHT_OFF_AGC+ERR_NIGHT_ON_AGC)>>1)) ? (iErrBg + (iErrBg * iErrNightAbs) / iErrNightTgtOfs)>>1 : iErrBg ;
	iErrDis = (iErrNightAbs < iErrNightTgtOfs && iLtPos > LT_END0) ? (iErrBg + (iErrBg * iErrNightAbs) / iErrNightTgtOfs)>>1 : iErrBg ;
	//if(iErrNightAbs < iErrNightTgtOfs && iErrBg < (ERR_NIGHT_OFF_END-iErrMgn)) iErrBg = (iErrBg * iErrNightAbs) / iErrNightTgtOfs;
#endif

	//----------------------------------------------------------------------------------------------------
	// Prevents 'iErrBg' value from becoming too large before Anti-saturation function.
	if(UP(LSpotTh))
	{
		const int iErrSub = (iErrNightOri < -128) ? (iErrBg>>1) : (iErrBg*(256+iErrNightOri))>>8 ;

		//if(ERR_DAY_SUB_SHT <= giShtVal && iAgcPre <= ERR_NIGHT_OFF_AGC) {
		if(LT_STA0 <= iLtPos && iLtPos <= LT_END0) {
			//const int iPosT = ((SHT_MAX  - ERR_DAY_SUB_SHT)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ((ERR_NIGHT_OFF_AGC - AGC_MIN)<<AGC_VAL_GAB);
			//const int iPosN = ((giShtVal - ERR_DAY_SUB_SHT)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ( giAgcPos          - (AGC_MIN<<AGC_VAL_GAB));
			const int iPosT = ((LT_SHT1  - LT_SHT0)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ((LT_AGC1 - LT_AGC0)<<AGC_VAL_GAB);
			const int iPosN = ((giShtVal - LT_SHT0)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ( giAgcPos          - (LT_AGC0<<AGC_VAL_GAB));
			//GrpAe(GA_ERR_DAY_POS_N) = iPosN;

			if(iErrNightOri < 0 && iErrBg > 0) iErrBg = iErrBg + (((iErrSub-iErrBg)*iPosN)/iPosT);
		}
		else if(LT_END0 <= iLtPos && iLtPos <= LT_END1) {
			//const int iPosT = (ERR_NIGHT_ON_AGC - ERR_NIGHT_OFF_AGC)<<AGC_VAL_GAB;
			//const int iPosN =  giAgcPos         - (ERR_NIGHT_OFF_AGC<<AGC_VAL_GAB);
			const int iPosT = ((LT_SHT2  - LT_SHT1)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ((LT_AGC2 - LT_AGC1)<<AGC_VAL_GAB);
			const int iPosN = ((giShtVal - LT_SHT1)<<(AGC_VAL_GAB-AGC_TO_SHT_BIT)) + ( giAgcPos          - (LT_AGC1<<AGC_VAL_GAB));
			//GrpAe(GA_ERR_DAY_POS_N) = iPosN;

			if(iErrNightOri < 0 && iErrBg > 0) iErrBg = iErrSub + (((iErrBg-iErrSub)*iPosN)/iPosT);
		}

		//if(iErrBg > iErrDis) iErrBg = iErrDis;
	}

	//----------------------------------------------------------------------------------------------------
	//GrpAe(GA_ERR_DAY_OFS_CNT) = /*iErrDis*/bErrOfsCnt;
	//GrpAe(GA_ERR_DAY_OFS) = iErrOfs;
	//GrpAe(GA_ERR_DAY) = iErrBg;
	//GrpAe(GA_ERR_DAY) = iErrOfs;
	//GrpAe(GA_ERR_NIGHT) = iErrSpot;

	int iErr;

	if(iLtPos >= LT_END0)
	{
		//ErrChtSet((bErrNightOn) ? iErrSpot : (iErrSpot < 0) ? iErrOfs : /*iErrSpot*/iErrBg, iErrMgn);
		//iErrSpot = ErrChtGet(iErrSpot);
		//GrpAe(GA_ERR_CHT_PRE) = iErrSpot;

		iErrBg = ((ERR_CHG_CNT_MAX-bErrOfsCnt)*iErrBg + bErrOfsCnt*iErrOfs)>>ERR_CHG_CNT_BIT;

		iErr = ((ERR_CHG_CNT_MAX-bErrNightCnt)*iErrBg + bErrNightCnt*iErrSpot)>>ERR_CHG_CNT_BIT;

		//ErrChtSet((-iErrMgn <= iErr && iErr <= iErrMgn) ? 0 : iErrNightOri0, 1/*iErrMgn*/);
		//ErrChtSet(bErrNightOn ? 0 : iErrNightOri0, 1/*iErrMgn*/);
	}
	else {
		if(bErrNightCnt < (ERR_CHG_CNT_MAX>>1)) bErrNightCnt = ERR_CHG_CNT_MAX>>1;

		iErr = iErrBg;
	}

	AddDbgData(iErr,3);

	static int iClipCntCut1d = 0;
	const int iSatClipMax = ParAe(PA_STA);// ? ParAe(PA_STA) : 5000;
	const int iClipCntCut = CLAMP((int)nClipCnt, 0, iSatClipMax);
	const int iClipCntCutDiff = iClipCntCut - iClipCntCut1d;
	const int iClipCntCutDiffAbs = ABS(iClipCntCutDiff);
	static int iClipCntCutDiffAbsBuf[3] = {0,0,0};

	GRP0 = iClipCntCut;
	GRP1 = iClipCntCutDiffAbs;
	//GRP2 = iErr;

	const int iErrAbs = ABS(iErr);
	if(ParAe(PA_STA)/*iErrAbs>iErrMgn*/) {
	#if 0
		const int iErrAbsMin = MAX(iErrAbs>>1, iErrMgn+1);
		const int iSatClipMax = AE_SAT_CLIP_MAX/*<<1*/;
		const int iClipCntCut = CLAMP((int)nClipCnt, AE_SAT_CLIP_MIN, iSatClipMax);
		const int iErrAbsNew = LibUtlInterp1D(iClipCntCut, AE_SAT_CLIP_MIN, iSatClipMax, iErrAbsMin, iErrAbs);
	#else
		const int iErrAbsNew = LibUtlInterp1D(iClipCntCutDiffAbsBuf[2], 0, iSatClipMax, iErrAbs, iErrMgn+1);
	#endif
		iErr = (iErr < 0) ? -iErrAbsNew : iErrAbsNew;
	}

	iClipCntCut1d = iClipCntCut;
	iClipCntCutDiffAbsBuf[2] = iClipCntCutDiffAbsBuf[1];
	iClipCntCutDiffAbsBuf[1] = iClipCntCutDiffAbsBuf[0];
	iClipCntCutDiffAbsBuf[0] = iClipCntCutDiffAbs;


	if(gbErrChtOff && iErrAbs<=iErrMgn) {
		gbErrChtOff = 0;
		//ErrChtInit(0);
	}


	GRP3 = iErr;
	UartTxGrp();

	AddDbgData(iErr,3);
	//TxDbgData();

	*apiErr = iErr;

	//GrpAe(GA_ERR_CHT_PRE) = iErrSpot/*iErr*/;
	//GrpAe(GA_ERR_DAY_ORI) = gcErrSignAccu;

	//ErrChtSet((bErrNightOn) ? iErr : 0, iErrMgn);

	GrpAe(GA_ERR_NIGHT_CNT) = (bErrNightCnt>>((ERR_CHG_CNT_BIT>3) ? ERR_CHG_CNT_BIT-3 : ERR_CHG_CNT_BIT))*100 + bErrOfsCnt;
}

int ISRT WdrAgcWgt(const BYTE abWdrAgcWgtOn, const int aiWdrAgcWgt, const int aiOffCnt, const int aiCtrlSpd)
{
	const int iShtSVal = ((giShtSVal*aiWdrAgcWgt)+((0x100-aiWdrAgcWgt)*(giShtVal-1)))>>8;	// AGC wgt
	//SHT_SET1(((giShtSVal*iWdrAgcWgt)+((0x100-iWdrAgcWgt)*(giShtVal-1)))>>8);

	static BYTE bShtSValCnt = 0;
	if(abWdrAgcWgtOn) { if(bShtSValCnt<aiOffCnt) bShtSValCnt++; }
	else { if(bShtSValCnt) bShtSValCnt--; }

	//GrpAe(GA_WDR_SHT_S_VAL) = iShtSVal;
	//GrpAe(GA_WDR_SHT_S_VAL_CNT) = bShtSValCnt;

	static int IIRI(iShtSValDly,500);
	if(bShtSValCnt==0) IIRI(iShtSValDly,iShtSVal);
	else			   IIRR(iShtSValDly,aiCtrlSpd,iShtSVal,iShtSValDly);

	return iShtSValDly>>(IIG+IKG);
}

const UINT gnTblToneMapBase[][2] =  {// X    , Y 0.01	Y 0.1
									{0       , 0x0  	/*0x0  	*/	/*0x00  */},     // 00
									{1       , 0x0  	/*0x7  	*/	/*0x1C  */},     // 01
									{2       , 0x1  	/*0xE  	*/	/*0x30  */},     // 02
									{4       , 0x3  	/*0x1A 	*/	/*0x4D  */},     // 03
									{8       , 0x7  	/*0x2E 	*/	/*0x71  */},     // 04
									{16      , 0xE  	/*0x4B 	*/	/*0x9A  */},     // 05
									{32      , 0x1A 	/*0x71 	*/	/*0xC7  */},     // 06
									{64      , 0x2F 	/*0x9E 	*/	/*0xF5  */},     // 07
									{128     , 0x4F 	/*0xCF 	*/	/*0x125 */},     // 08
									{256     , 0x7A 	/*0x103	*/	/*0x155 */},     // 09
									{512     , 0xAE 	/*0x138	*/	/*0x185 */},     // 10
									{768     , 0xCF 	/*0x158	*/	/*0x1A2 */},     // 11
									{1024    , 0xE8 	/*0x16E	*/	/*0x1B6 */},     // 12
									{1536    , 0x10C	/*0x18E	*/	/*0x1D3 */},     // 13
									{2048    , 0x126	/*0x1A4	*/	/*0x1E7 */},     // 14
									{3072    , 0x14C	/*0x1C4	*/	/*0x203 */},     // 15
									{4096    , 0x167	/*0x1DB	*/	/*0x217 */},     // 16
									{6144    , 0x18D	/*0x1FB	*/	/*0x234 */},     // 17
									{8192    , 0x1A8	/*0x212	*/	/*0x248 */},     // 18
									{12288   , 0x1CF	/*0x232	*/	/*0x265 */},     // 19
									{16384   , 0x1EA	/*0x248	*/	/*0x279 */},     // 20
									{24576   , 0x211	/*0x268	*/	/*0x295 */},     // 21
									{32768   , 0x22C	/*0x27F	*/	/*0x2AA */},     // 22
									{40960   , 0x242	/*0x291	*/	/*0x2B9 */},     // 23
									{49152   , 0x253	/*0x29F	*/	/*0x2C6 */},     // 24
									{57344   , 0x262	/*0x2AB	*/	/*0x2D1 */},     // 25
									{65536   , 0x26F	/*0x2B6	*/	/*0x2DA */},     // 26
									{73728   , 0x27A	/*0x2BF	*/	/*0x2E2 */},     // 27
									{81920   , 0x284	/*0x2C8	*/	/*0x2EA */},     // 28
									{90112   , 0x28E	/*0x2CF	*/	/*0x2F1 */},     // 29
									{98304   , 0x296	/*0x2D6	*/	/*0x2F7 */},     // 30
									{114688  , 0x2A5	/*0x2E2	*/	/*0x302 */},     // 31
									{131072  , 0x2B1	/*0x2ED	*/	/*0x30B */},     // 32
									{147456  , 0x2BD	/*0x2F6	*/	/*0x313 */},     // 33
									{163840  , 0x2C7	/*0x2FE	*/	/*0x31B */},     // 34
									{180224  , 0x2D0	/*0x306	*/	/*0x321 */},     // 35
									{196608  , 0x2D8	/*0x30D	*/	/*0x327 */},     // 36
									{212992  , 0x2E0	/*0x313	*/	/*0x32D */},     // 37
									{229376  , 0x2E7	/*0x319	*/	/*0x332 */},     // 38
									{245760  , 0x2EE	/*0x31E	*/	/*0x337 */},     // 39
									{262144  , 0x2F4	/*0x323	*/	/*0x33C */},     // 40
									{393216  , 0x31B	/*0x343	*/	/*0x358 */},     // 41
									{524288  , 0x337	/*0x35A	*/	/*0x36C */},     // 42
									{655360  , 0x34C	/*0x36C	*/	/*0x37C */},     // 43
									{786432  , 0x35E	/*0x37A	*/	/*0x389 */},     // 44
									{917504  , 0x36C	/*0x386	*/	/*0x394 */},     // 45
									{1048576 , 0x379	/*0x391	*/	/*0x39D */},     // 46
									{1179648 , 0x385	/*0x39A	*/	/*0x3A5 */},     // 47
									{1310720 , 0x38F	/*0x3A3	*/	/*0x3AD */},     // 48
									{1441792 , 0x398	/*0x3AA	*/	/*0x3B3 */},     // 49
									{1572864 , 0x3A0	/*0x3B1	*/	/*0x3BA */},     // 50
									{1835008 , 0x3AF	/*0x3BD	*/	/*0x3C4 */},     // 51
									{2097152 , 0x3BC	/*0x3C8	*/	/*0x3CE */},     // 52
									{2359296 , 0x3C7	/*0x3D1	*/	/*0x3D6 */},     // 53
									{2621440 , 0x3D1	/*0x3D9	*/	/*0x3DD */},     // 54
									{2883584 , 0x3DA	/*0x3E1	*/	/*0x3E4 */},     // 55
									{3145728 , 0x3E3	/*0x3E8	*/	/*0x3EA */},     // 56
									{3407872 , 0x3EB	/*0x3EE	*/	/*0x3F0 */},     // 57
									{3670016 , 0x3F2	/*0x3F4	*/	/*0x3F5 */},     // 58
									{3932160 , 0x3F8	/*0x3F9	*/	/*0x3FA */},     // 59
									{4194304 , 0x3FF	/*0x3FF	*/	/*0x3FF */}      // 60
									};

const UINT gnTblToneMapCurX[41]= { // X
									0      ,   // 00
									16     ,   // 01
									32     ,   // 02
									64     ,   // 03
									128    ,   // 04
									256    ,   // 05
									512    ,   // 06
									1024   ,   // 07
									2048   ,   // 08
									4096   ,   // 09
									8192   ,   // 10
									12288  ,   // 11
									16384  ,   // 12
									24576  ,   // 13
									32768  ,   // 14
									49152  ,   // 15
									65536  ,   // 16
									98304  ,   // 17
									131072 ,   // 18
									196608 ,   // 19
									262144 ,   // 20
									393216 ,   // 21
									524288 ,   // 22
									655360 ,   // 23
									786432 ,   // 24
									917504 ,   // 25
									1048576,   // 26
									1179648,   // 27
									1310720,   // 28
									1441792,   // 29
									1572864,   // 30
									1835008,   // 31
									2097152,   // 32
									2359296,   // 33
									2621440,   // 34
									2883584,   // 35
									3145728,   // 36
									3407872,   // 37
									3670016,   // 38
									3932160,   // 39
									4194304    // 40
								};

void ISRT LibWdrCtrl(const UINT SMAX)
{
	int i, j;

	UINT MAXX = gnTblToneMapBase[60][0];
	UINT MAXY = gnTblToneMapBase[60][1];
	UINT MAXX_P = gnTblToneMapBase[60-1][0];
	UINT MAXY_P = gnTblToneMapBase[60-1][1];

	for(i=1; i<=60; i++) {
		if(gnTblToneMapBase[i][0]>=SMAX){
			MAXX = gnTblToneMapBase[i][0];
			MAXY = gnTblToneMapBase[i][1];
			MAXX_P = gnTblToneMapBase[i-1][0];
			MAXY_P = gnTblToneMapBase[i-1][1];
			break;
		}
	}

	MAXY = (MAXY*(SMAX-MAXX_P)+MAXY_P*(MAXX-SMAX))/(MAXX-MAXX_P);

	UINT MADDR = 0;
	UINT nTblToneMapCurY[41];

	nTblToneMapCurY[0] = 0;

	for(i=1; i<=40; i++) {
//			NTBLX = gnTblToneMapCurX[i]*(SMAX/0x3fffff);
		const u64 NTBLX = (gnTblToneMapCurX[i]*(u64)SMAX)/(u64)0x3fffff;
//			DebugDisp(1, Hex, "NTBLX ", 6+i, 24, (UINT)NTBLX)

//			if (gnTblToneMapBase[MADDR][0]<=NTBLX)	MADDR++;
		while (gnTblToneMapBase[MADDR][0]<=(UINT)NTBLX) {MADDR++;}

//			WGT = ((float)(NTBLX-gnTblToneMapBase[MADDR-1][0])/(float)(gnTblToneMapBase[MADDR][0]-gnTblToneMapBase[MADDR-1][0]));
//			nTblToneMapCurY[i] = (UINT)(((WGT*(float)gnTblToneMapBase[MADDR][1])+((1.f-WGT)*(float)gnTblToneMapBase[MADDR-1][1]))/(float)MAXY);
		const int WGT = (((UINT)NTBLX-gnTblToneMapBase[MADDR-1][0])*256)/(gnTblToneMapBase[MADDR][0]-gnTblToneMapBase[MADDR-1][0]);
		nTblToneMapCurY[i] = (((WGT*gnTblToneMapBase[MADDR][1])+((256-WGT)*gnTblToneMapBase[MADDR-1][1]))*1023)/(MAXY*256);

//			DebugDisp(1, Dec, "WGT ", 1, 24, WGT  	)
//			DebugDisp(1, Dec, "WGT ", 1, 24, WGT  	)
	}

	TMG_TBL00w(nTblToneMapCurY[ 0]);
	TMG_TBL01w(nTblToneMapCurY[ 1]);

	for(i=1,j=2; i<=13; i++,j+=3) {

		SetIsp(TMG_BASE+i, (nTblToneMapCurY[j]<<20)|(nTblToneMapCurY[j+1]<<10)|(nTblToneMapCurY[j+2]));
	}
}

//                     Gamma: 0.45  0.5   0.55  0.6   0.65  0.7   0.75
const WORD gwGammaAeCur[] = { 228,  263,  293,  320,  346,  370,  392 };

UINT ISRT AutoGamma(/*const int aiCurIIR,*/ const int aiTgtMax, const int aiTgtMin, const BYTE abGammaIdxMax, const BYTE abGammaIdxMin, BYTE *apbGammaIdx)
{
	int i;

	if(abGammaIdxMax/*UP(Gamma)*/ == 0) {
		*apbGammaIdx = 0;
		return 128;
	}

	if(abGammaIdxMax/*UP(Gamma)*/ <= abGammaIdxMin/*UP(GammaNgt)*/) {
		*apbGammaIdx = abGammaIdxMax/*(UP(Gamma)<<1)*/-1;
		return 0;
	}

	const int iGmAeMaxIdx = abGammaIdxMax/*UP(Gamma)<<1*/;			// Menu -> Index
	const int iGmAeMinIdx = abGammaIdxMin/*UP(GammaNgt)<<1*/;		//		"

	const int iGmAeMax = gwGammaAeCur[iGmAeMaxIdx];
	const int iGmAeMin = gwGammaAeCur[iGmAeMinIdx];
	const int iGmAeSiz = iGmAeMax - iGmAeMin;

	const int iTgtSiz = aiTgtMax - aiTgtMin;

	const int iTgtMin = (iGmAeSiz < iTgtSiz) ? aiTgtMin : aiTgtMax - iGmAeSiz;

	const int iTgtMaxIIR = aiTgtMax<<(IIG+IKG);
	const int iTgtMinIIR = iTgtMin<<(IIG+IKG);

	const int iCurLmt = CLAMP(iCurBgIIR/*aiCurIIR*/, iTgtMinIIR, iTgtMaxIIR);
	const int iCurNor = ((iCurLmt - iTgtMinIIR) / (aiTgtMax - iTgtMin)) * iGmAeSiz + (iGmAeMin<<(IIG+IKG));

	for(i=iGmAeMinIdx; i<iGmAeMaxIdx-1; i++) {
		//printf("i:%d, CurNor:%d\n", i, iCurNor>>(IIG+IKG));
		if(iCurNor <= (gwGammaAeCur[i+1]<<(IIG+IKG))) break;
	}

	*apbGammaIdx = i;
	return ((gwGammaAeCur[i+1]<<(IIG+IKG)) - iCurNor) / ((gwGammaAeCur[i+1] - gwGammaAeCur[i])<<(IIG+IKG-7));
}

UINT ISRT SatGamma(const int iTgtOri, const int iTgtSpotBgOri, BYTE *apbGammaIdx0)
{
	//const int iErrBg = MAX(iErrBg1dIIR>>(IIG+IKG-7),0);
	//#define BG_GAMMA_0	328/*218*/
	//#define BG_GAMMA_1	546/*436*/
	//#define BG_GAMMA_2	670/*655*/

	#define BG_GAMMA_0_UNIT	3
	#define BG_GAMMA_1_UNIT	5
	#define BG_GAMMA_2_UNIT	6

	const int iCurBg = (iCurBgIIR*BG_GAMMA_2_UNIT)>>(IIG+IKG-7);
	//const int iCurIIRout = IIROu(iCurBgIIR);
	const int iErrBg = MAX(((iTgtOri*BG_GAMMA_2_UNIT)<<7) - iCurBg, 0);
	const int BG_GAMMA = (iTgtOri-iTgtSpotBgOri-8);	// 8 = AE_ERR_MGN

	const int BG_GAMMA_0 = BG_GAMMA*BG_GAMMA_0_UNIT;
	const int BG_GAMMA_1 = BG_GAMMA*BG_GAMMA_1_UNIT;
	const int BG_GAMMA_2 = BG_GAMMA*BG_GAMMA_2_UNIT;

	//GrpAe(GA_ERR_DAY_ORI) = iErrBg>>7;

	//extern const UINT* gpTblGamma[];
	//extern UINT GetGamma(const UINT nG1, const UINT nG2, const UINT nGK);

	UINT nSatGmkY/*, nWdrGmkC*/;
	//UINT const *pnTblGmaC0;
	//UINT const *pnTblGmaC1;

	if(  (UP(MinGammaY) == 1) ||
		((UP(MinGammaY) == 2) && (iErrBg <= (BG_GAMMA_1<<7))) ||
		((UP(MinGammaY) == 3) && (iErrBg <= (BG_GAMMA_0<<7))) )
	{
		nSatGmkY = (UP(MinGammaY) == 1) ? MIN(iErrBg/BG_GAMMA_2, 0x80) :
				   (UP(MinGammaY) == 2) ? iErrBg/BG_GAMMA_1 :
											iErrBg/BG_GAMMA_0 ;
		//pnTblGmaY0 = gpTblGamma[(UP(Gamma)<<1)+3];
		//pnTblGmaY1 = gpTblGamma[(UP(Gamma)<<1)+2];
		*apbGammaIdx0 = 2;
	}
	else if( (UP(MinGammaY) == 2) ||
			((UP(MinGammaY) == 3) && (iErrBg <= (BG_GAMMA_1<<7))) )
	{
		nSatGmkY = (UP(MinGammaY) == 2) ? MIN((iErrBg-(BG_GAMMA_1<<7))/(BG_GAMMA_2-BG_GAMMA_1), 0x80) :
												(iErrBg-(BG_GAMMA_0<<7))/(BG_GAMMA_1-BG_GAMMA_0) ;
		//pnTblGmaY0 = gpTblGamma[(UP(Gamma)<<1)+2];
		//pnTblGmaY1 = gpTblGamma[(UP(Gamma)<<1)+1];
		*apbGammaIdx0 = 1;
	}
	else {
		nSatGmkY = MIN((iErrBg-(BG_GAMMA_1<<7))/(BG_GAMMA_2-BG_GAMMA_1), 0x80);
		//pnTblGmaY0 = gpTblGamma[(UP(Gamma)<<1)+1];
		//pnTblGmaY1 = gpTblGamma[(UP(Gamma)<<1)+0];
		*apbGammaIdx0 = 0;
	}

	return nSatGmkY;

#if 0
	if(UP(MinGammaC) == 0) {
		nWdrGmkC = 0x80;
		pnTblGmaC0 = gnTblGamma_30;
		pnTblGmaC1 = gnTblGamma_30;
	}
	else if( (UP(MinGammaC) == 1) ||
			((UP(MinGammaC) == 2) && (iErrBg <= (BG_GAMMA_1<<7))) ||
			((UP(MinGammaC) == 3) && (iErrBg <= (BG_GAMMA_0<<7))) )
	{
		nWdrGmkC = (UP(MinGammaC) == 1) ? MIN(iErrBg/BG_GAMMA_2, 0x80) :
				   (UP(MinGammaC) == 2) ? iErrBg/BG_GAMMA_1 :
											iErrBg/BG_GAMMA_0 ;
		pnTblGmaC0 = gnTblGamma_30;
		pnTblGmaC1 = gnTblGamma_25;
	}
	else if( (UP(MinGammaC) == 2) ||
			((UP(MinGammaC) == 3) && (iErrBg <= (BG_GAMMA_1<<7))) )
	{
		nWdrGmkC = (UP(MinGammaC) == 2) ? MIN((iErrBg-(BG_GAMMA_1<<7))/(BG_GAMMA_2-BG_GAMMA_1), 0x80) :
												(iErrBg-(BG_GAMMA_0<<7))/(BG_GAMMA_1-BG_GAMMA_0) ;
		pnTblGmaC0 = gnTblGamma_25;
		pnTblGmaC1 = gnTblGamma_20;
	}
	else {
		nWdrGmkC = MIN((iErrBg-(BG_GAMMA_1<<7))/(BG_GAMMA_2-BG_GAMMA_1), 0x80);
		pnTblGmaC0 = gnTblGamma_20;
		pnTblGmaC1 = gnTblGamma_15;
	}

	for (i=0; i<9; i++) SetIsp(CGAMMA_BASE+i, GetGamma(*(pnTblGmaC0+i), *(pnTblGmaC1+i), nWdrGmkC));	// c gamma exchange
#endif

}

//-------------------------------------------------------------------------------------------------
//UINT gnBoxOnAeMon;
int gpw = 0;
BYTE grr = 0;

void AeMonOn(const int ai)
{
	#define BOX_ID_AE_POS	0//20	// AE Control Position : White
	#define BOX_ID_SPOT_CNT	1//21	// TgtMax or TgtSpot : Green
	#define BOX_ID_OFS_CNT	2//22	// TgtMax or TgtMin : Green
	#define BOX_ID_CUR_BG	3//23
	#define BOX_ID_CUR_SPOT	4//24
	#define BOX_ID_TGT_BG	5//25	// TgtMax ~ TgtMin : Yellow
	#define BOX_ID_TGT_SPOT	6//26

	#define BOX_ID_SHT		7//27
	#define BOX_ID_AGC		8//28
	#define BOX_ID_DEBLUR	9//29
	#define BOX_ID_IRIS		10//30
	#define BOX_ID_DSS		11//31

	BOX_ATT(BOX_ID_AE_POS,	1,BOX_TONE_100,0xff,0x80,0x80/*WHT*/);
	BOX_ATT(BOX_ID_CUR_BG,	1,BOX_TONE_100,0xff,0x80,0x80/*WHT*/);
	BOX_ATT(BOX_ID_CUR_SPOT,1,BOX_TONE_100,0xff,0x80,0x80/*WHT*/);
	BOX_ATT(BOX_ID_SPOT_CNT,1,BOX_TONE_100,168,35,19/*GRN*/);
	BOX_ATT(BOX_ID_OFS_CNT,	1,BOX_TONE_100,168,35,19/*GRN*/);
	BOX_ATT(BOX_ID_TGT_BG,	1,BOX_TONE_25,218,8,139/*YEL*/);
	BOX_ATT(BOX_ID_TGT_SPOT,1,BOX_TONE_25,218,8,139/*YEL*/);
	BOX_ATT(BOX_ID_IRIS,	1,BOX_TONE_25,185,155,8/*CYN*/);
	BOX_ATT(BOX_ID_SHT,		1,BOX_TONE_25,17,248,117/*BLU*/);
	BOX_ATT(BOX_ID_DEBLUR,	1,BOX_TONE_25,67,221,237/*MAG*/);
	BOX_ATT(BOX_ID_AGC,		1,BOX_TONE_25,50,101,248/*RED*/);
	BOX_ATT(BOX_ID_DSS,		1,BOX_TONE_25,168,35,19/*GRN*/);

	gpw = ai;
}

void AeMonOff(void)
{
//	gnBoxOnAeMon = 0;
	BOSD_ON0w(0);
}

void ISRT AeMon(const BYTE bSatOff, const BYTE bLSflag, const int iErrMgn, const int iCur, const int iTgt,
				const BYTE bIrsOn, const BYTE bShtOn, const BYTE bTgtOn, const BYTE bDeblurOn, const BYTE bAgcOn, const BYTE bDssOn,
				const int AE_IRS_STAT_Max, const int iShtMax_Agc, const int iAgcMin_Agc, const int iAgcDevMax, const int iShtMax_Dss, const BYTE bMpDss
				)
{
	if(gpw!=173829904 || grr==0) return;

	extern UINT gnAeVtw;
	extern BYTE gbDssRatioLmt;

	#define AE_GRAPH_SX		1200
	#define AE_GRAPH_SY		40
	#define AE_GRAPH_H_BIT	8	// 2^8 = 256
	#define AE_GRAPH_EY		(AE_GRAPH_SY+(1<<AE_GRAPH_H_BIT))

	#define AE_GRAPH_IRS_W	100
	#define AE_GRAPH_SHT_W	200
	#define AE_GRAPH_AGC_W	200
	#define AE_GRAPH_DSS_W	40

	#define BOX_H(ID,X,W)	gnBoxOnAeMon |= (1<<ID); BOX(ID, X, AE_GRAPH_SY, X+W, AE_GRAPH_EY)
	#define BOX_W(ID,SY,EY)	gnBoxOnAeMon |= (1<<ID); BOX(ID, AE_GRAPH_SX, SY, wX, EY)

	static WORD wX = AE_GRAPH_SX;

	if(gbWdrOn&&(bLSflag==AeLONG)) {
		const WORD wCurY = AE_GRAPH_EY - (iCur>>(10-AE_GRAPH_H_BIT));
		BOX(BOX_ID_CUR_SPOT, AE_GRAPH_SX, wCurY, wX, wCurY)

		const WORD wTgtYS = AE_GRAPH_EY - ((iTgt+iErrMgn)>>(10-AE_GRAPH_H_BIT));
		const WORD wTgtYE = AE_GRAPH_EY - ((iTgt-iErrMgn)>>(10-AE_GRAPH_H_BIT));
		BOX(BOX_ID_TGT_SPOT, AE_GRAPH_SX, wTgtYS, wX, wTgtYE)
	}
	else {
		static WORD wPos = AE_GRAPH_SX;
		UINT gnBoxOnAeMon = 0;

		wX = AE_GRAPH_SX;

		if(bIrsOn) {
			if(gbAeStg == AE_STG_IRS) wPos = wX + giIrsPos * AE_GRAPH_IRS_W / AE_IRS_STAT_Max;
			BOX_H(BOX_ID_IRIS, wX, AE_GRAPH_IRS_W);
			wX += AE_GRAPH_IRS_W;
		}
		if(bShtOn) {
			const WORD wShtW = ((SHT_MAX - SHT_MIN) * AE_GRAPH_SHT_W) / gnAeVtw + 1;
			if(gbAeStg == AE_STG_SHT) wPos = wX + ((giShtVal - SHT_MIN) * wShtW) / (SHT_MAX - SHT_MIN);
			BOX_H(BOX_ID_SHT, wX, wShtW);
			wX += wShtW;
		}
		if(bTgtOn) {

		}
		if(bDeblurOn) {
			const WORD wDblW = ((SHT_DBL_MAX - SHT_MAX) * AE_GRAPH_SHT_W) / gnAeVtw + 1;
			if(gbAeStg == AE_STG_DBL) wPos = wX + ((giShtVal - SHT_MAX) * wDblW) / (SHT_DBL_MAX - SHT_MAX);
			BOX_H(BOX_ID_DEBLUR, wX, wDblW);
			wX += wDblW;
		}
		if(bAgcOn) {
			const WORD wAgcW = ((AGC_MAX - iAgcMin_Agc) * AE_GRAPH_AGC_W) / iAgcDevMax/*POS2AGC(gnAeAgcTVal+1)*/ + 1;
			if(gbAeStg == AE_STG_AGC) wPos = wX + ((giAgcVal - iAgcMin_Agc) * wAgcW) / (AGC_MAX - iAgcMin_Agc);
			BOX_H(BOX_ID_AGC, wX, wAgcW);
			wX += wAgcW;
		}
		if(bDssOn) {
			const WORD wShtW = ((gnAeVtw - iShtMax_Agc) * AE_GRAPH_SHT_W) / gnAeVtw;
			const WORD wDssW = (gbDssRatioLmt > 1) ? bMpDss/*UP(Dss)*/ * AE_GRAPH_DSS_W : 0 ;

			if(gbAeStg == AE_STG_DSS) wPos = wX + ((giShtVal>(int)gnAeVtw) ? wShtW + ((giShtVal - gnAeVtw) * wDssW) / (iShtMax_Dss - gnAeVtw) :
																			 ((giShtVal - iShtMax_Agc) * wShtW) / (gnAeVtw - iShtMax_Agc)) ;
			BOX_H(BOX_ID_DSS, wX, wShtW+wDssW);
			wX += wShtW+wDssW;
		}

		/*if(wPos)*/ BOX_H(BOX_ID_AE_POS, wPos, 0);

		const int iCurGrp = (bSatOff) ? iCur : iCurBg ;
		const WORD wCurY = AE_GRAPH_EY - (iCurGrp>>(10-AE_GRAPH_H_BIT));
		BOX_W(BOX_ID_CUR_BG,wCurY,wCurY);

		const int iTgtGrp = (bSatOff) ? iTgt : iTgtSpotBg ;
		const short TgtYS = AE_GRAPH_EY - ((iTgt+iErrMgn)>>(10-AE_GRAPH_H_BIT));
		const short TgtYE = AE_GRAPH_EY - ((iTgtGrp-iErrMgn)>>(10-AE_GRAPH_H_BIT));
		const WORD wTgtYS = MIN(TgtYS, AE_GRAPH_EY);
		const WORD wTgtYE = MIN(TgtYE, AE_GRAPH_EY);
		BOX_W(BOX_ID_TGT_BG,wTgtYS,wTgtYE);

		if(bSatOff==0) {
			const WORD wCurSpotY = AE_GRAPH_EY - (iCurSpot>>(10-AE_GRAPH_H_BIT));
			BOX_W(BOX_ID_CUR_SPOT,wCurSpotY,wCurSpotY);

			const WORD wTgtSpotYS = AE_GRAPH_EY - ((iTgtSpot+iErrMgn)>>(10-AE_GRAPH_H_BIT));
			const WORD wTgtSpotYE = AE_GRAPH_EY - ((iTgtSpot-iErrMgn)>>(10-AE_GRAPH_H_BIT));
			BOX_W(BOX_ID_TGT_SPOT,wTgtSpotYS,wTgtSpotYE);

			const WORD wErrSpotCntY = bErrNightCnt * (wTgtSpotYE - wTgtYS) / ERR_CHG_CNT_MAX + wTgtYS;
			const WORD wErrOfsCntY = (ERR_CHG_CNT_MAX - bErrOfsCnt) * (wTgtYS - wTgtYE) / ERR_CHG_CNT_MAX + wTgtYE;

			BOX_W(BOX_ID_SPOT_CNT,wErrSpotCntY,wErrSpotCntY);
			BOX_W(BOX_ID_OFS_CNT,wErrOfsCntY,wErrOfsCntY);
		}

		if(gbWdrOn) gnBoxOnAeMon |= (1<<BOX_ID_CUR_SPOT)|(1<<BOX_ID_TGT_SPOT);

		BOSD_ON0w(gnBoxOnAeMon);
	}
}


