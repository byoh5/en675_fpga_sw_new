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

