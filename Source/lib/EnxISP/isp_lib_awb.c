/* **************************************************************************
 File Name	:	isp_lib_awb.c
 Description:	EN675 - Auto White Balance function
 Designer	:	Lee, Hoon (13.8.4)
 Modified by:	Kim, Sunghoon (19.08.20)
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "isp_lib_main.h"

int giCmat[3][3];

float32 gf32AwbMatGain;

void ISRT LibAwbCmat(int aiDb, int aiDr, float32 alMtxCf[9][6])
{
	UINT		i,j;

	float32		fDb, fDr, fDbDr, fDb2, fDr2;
	float32* 	p = (float32*)alMtxCf;
	float32		fBuf;

	fDb			= _itof(aiDb);
	fDr			= _itof(aiDr);
	fDbDr		= _fmul(fDb, fDr);
	fDb2 		= _fmul(fDb, fDb);
	fDr2 		= _fmul(fDr, fDr);


	for(j=0; j<3; j++) {
		for(i=0; i<3; i++) {
			fBuf = *p++;
			fBuf = _fadd(fBuf, _fmul(*p++, fDb	));
			fBuf = _fadd(fBuf, _fmul(*p++, fDr	));
			fBuf = _fadd(fBuf, _fmul(*p++, fDbDr));
			fBuf = _fadd(fBuf, _fmul(*p++, fDb2	));
			fBuf = _fadd(fBuf, _fmul(*p++, fDr2	));

			giCmat[j][i] = _ftoi(_fmul(fBuf, gf32AwbMatGain));
		}
	}

}

int RR_GAIN;
int RG_GAIN;
int RB_GAIN;
int GR_GAIN;
int GG_GAIN;
int GB_GAIN;
int BR_GAIN;
int BG_GAIN;
int BB_GAIN;

void ISRT LibAwbWdrRGB (int aiOn, int aiNight, int aiMon)
{
//	float 	fWdrClipR;
//	float 	fWdrClipG;
//	float 	fWdrClipB;

	float32 	fWdrClipR;		// 171207 LH : HW floating
	float32 	fWdrClipG;
	float32 	fWdrClipB;

//	s64 	fWdrClipR;		// 171211 LH : int floating -> size is too big !
//	s64 	fWdrClipG;
//	s64 	fWdrClipB;

	if (aiOn) {
		// inverse matrix
		const int DET = (RR_GAIN*GG_GAIN*BB_GAIN) + (GR_GAIN*BG_GAIN*RB_GAIN) + (BR_GAIN*RG_GAIN*GB_GAIN) - (RR_GAIN*BG_GAIN*GB_GAIN) - (BR_GAIN*GG_GAIN*RB_GAIN) - (GR_GAIN*RG_GAIN*BB_GAIN);
		const int RR_INV = (GG_GAIN*BB_GAIN)-(GB_GAIN*BG_GAIN);
		const int RG_INV = (RB_GAIN*BG_GAIN)-(RG_GAIN*BB_GAIN);
		const int RB_INV = (RG_GAIN*GB_GAIN)-(RB_GAIN*GG_GAIN);
		const int GR_INV = (GB_GAIN*BR_GAIN)-(GR_GAIN*BB_GAIN);
		const int GG_INV = (RR_GAIN*BB_GAIN)-(RB_GAIN*BR_GAIN);
		const int GB_INV = (RB_GAIN*GR_GAIN)-(RR_GAIN*GB_GAIN);
		const int BR_INV = (GR_GAIN*BG_GAIN)-(GG_GAIN*BR_GAIN);
		const int BG_INV = (RG_GAIN*BR_GAIN)-(RR_GAIN*BG_GAIN);
		const int BB_INV = (RR_GAIN*GG_GAIN)-(RG_GAIN*GR_GAIN);

//		fWdrClipR = (float)(RR_INV+RG_INV+RB_INV)/(float)DET;
//		fWdrClipG = (float)(GR_INV+GG_INV+GB_INV)/(float)DET;
//		fWdrClipB = (float)(BR_INV+BG_INV+BB_INV)/(float)DET;
//		// normalization to 1023
//		const float fWdrClipMax = MAX(MAX(fWdrClipR, fWdrClipG),fWdrClipB);
//		fWdrClipR = aiNight? 0x3ff : ((fWdrClipR/fWdrClipMax)*1023.f);
//		fWdrClipG = aiNight? 0x3ff : ((fWdrClipG/fWdrClipMax)*1023.f);
//		fWdrClipB = aiNight? 0x3ff : ((fWdrClipB/fWdrClipMax)*1023.f);
//
//		WDR_LCLIPRw((int)fWdrClipR);
//		WDR_LCLIPGw((int)fWdrClipG);
//		WDR_LCLIPBw((int)fWdrClipB);
//		WDR_SCLIPRw((int)fWdrClipR);
//		WDR_SCLIPGw((int)fWdrClipG);
//		WDR_SCLIPBw((int)fWdrClipB);

	// 171207 LH : HW floating -------------------------------------------------
		fWdrClipR = _fdiv(_itof(RR_INV+RG_INV+RB_INV), _itof(DET));
		fWdrClipG = _fdiv(_itof(GR_INV+GG_INV+GB_INV), _itof(DET));
		fWdrClipB = _fdiv(_itof(BR_INV+BG_INV+BB_INV), _itof(DET));
		// normalization to 1023
		const float32 fWdrClipMax = MAX(MAX(fWdrClipR, fWdrClipG),fWdrClipB);
		fWdrClipR = aiNight ? _itof(0x3ff) : _fmul(_fdiv(fWdrClipR,fWdrClipMax),_itof(1023));
		fWdrClipG = aiNight ? _itof(0x3ff) : _fmul(_fdiv(fWdrClipG,fWdrClipMax),_itof(1023));
		fWdrClipB = aiNight ? _itof(0x3ff) : _fmul(_fdiv(fWdrClipB,fWdrClipMax),_itof(1023));

		WDR_LCLIPRw(_ftoi(fWdrClipR));
		WDR_LCLIPGw(_ftoi(fWdrClipG));
		WDR_LCLIPBw(_ftoi(fWdrClipB));
		WDR_SCLIPRw(_ftoi(fWdrClipR));
		WDR_SCLIPGw(_ftoi(fWdrClipG));
		WDR_SCLIPBw(_ftoi(fWdrClipB));
	// ------------------------------------------------------------------------

//  // 171211 LH : int floating -> size is too big !
////		DebugDisp(aiMon,	Dec, "WdrClipR"	, 17, 24, (int)(RR_INV+RG_INV+RB_INV))
////		DebugDisp(aiMon,	Dec, "WdrClipG"	, 18, 24, (int)(GR_INV+GG_INV+GB_INV))
////		DebugDisp(aiMon,	Dec, "WdrClipB"	, 19, 24, (int)(BR_INV+BG_INV+BB_INV))
////		DebugDisp(aiMon,	Dec, "det_____"	, 20, 24, (int)DET)
//
//		fWdrClipR = ((s64)RR_INV+(s64)RG_INV+(s64)RB_INV)<<16/(s64)DET;
//		fWdrClipG = ((s64)GR_INV+(s64)GG_INV+(s64)GB_INV)<<16/(s64)DET;
//		fWdrClipB = ((s64)BR_INV+(s64)BG_INV+(s64)BB_INV)<<16/(s64)DET;
//		DebugDisp(aiMon,	Dec, "WdrClipR"	, 17, 24, (int)fWdrClipR)
//		DebugDisp(aiMon,	Dec, "WdrClipG"	, 18, 24, (int)fWdrClipG)
//		DebugDisp(aiMon,	Dec, "WdrClipB"	, 19, 24, (int)fWdrClipB)
//
//		// normalization to 1023
//		const s64 fWdrClipMax = MAX(MAX(fWdrClipR, fWdrClipG),fWdrClipB);
//		fWdrClipR = aiNight? 0x3ff : (ifmul8(fWdrClipR)/fWdrClipMax)*(s64)1023;
//		fWdrClipG = aiNight? 0x3ff : (ifmul8(fWdrClipG)/fWdrClipMax)*(s64)1023;
//		fWdrClipB = aiNight? 0x3ff : (ifmul8(fWdrClipB)/fWdrClipMax)*(s64)1023;
//
//		WDR_LCLIPRw((int)ifdiv8(fWdrClipR));
//		WDR_LCLIPGw((int)ifdiv8(fWdrClipG));
//		WDR_LCLIPBw((int)ifdiv8(fWdrClipB));
//		WDR_SCLIPRw((int)ifdiv8(fWdrClipR));
//		WDR_SCLIPGw((int)ifdiv8(fWdrClipG));
//		WDR_SCLIPBw((int)ifdiv8(fWdrClipB));
//	// --------------------------------------------------------------------------

		extern BYTE gbAwbDebugOn;
		DebugDisp2(gbAwbDebugOn, Dec, "WCLP R", 20, 24, (int)fWdrClipR, 4)
		DebugDisp2(gbAwbDebugOn, Dec, "WCLP G", 21, 24, (int)fWdrClipG, 4)
		DebugDisp2(gbAwbDebugOn, Dec, "WCLP B", 22, 24, (int)fWdrClipB, 4)
	}
}

