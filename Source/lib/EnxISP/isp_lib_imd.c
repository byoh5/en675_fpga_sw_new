/* **************************************************************************
 File Name	: 	isp_lib_imd.c
 Description:	EN675 - Motion detector
 Designer	:	Kim, Sunghoon
 Date		:	19. 7. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "isp_lib_main.h"

extern UINT	gnBoxMskPos[MASK_EA];
extern UINT	gnBoxMskPosNum;
extern BYTE	gbBoxMskDet[MASK_EA];

extern int	gbIMD_Bright;
extern int	gbIMD_Moving;
extern int	gbIMD_Update;

int giIMD_MOVING_TH = 8;		// Threshold for Camera Moving, The unit is number of labeling (카메라 움직임 검출을 위한 라벨링 개수 증가 값)
int giIMD_BRIGHT_MAX_TH1 = 80;	// Threshold for Bright Change, The unit is error of AE
int giIMD_BRIGHT_MIN_TH1 = -40;	//		"
int giIMD_BRIGHT_MAX_TH2 = 60;	// Threshold for Bright Change when the Camera Moving, The unit is error of AE
int giIMD_BRIGHT_MIN_TH2 = -30;	//		"

void CamMovTh(const int aiIMD_MOVING_TH)
{
	giIMD_MOVING_TH = aiIMD_MOVING_TH;

#if IMD_TEST
	printf("IMD_MOVING_TH = %d\r\n", giIMD_MOVING_TH);
#endif
}

void BrtChgTh(const int aiIMD_BRIGHT_MAX_TH1, const int aiIMD_BRIGHT_MIN_TH1, const int aiIMD_BRIGHT_MAX_TH2, const int aiIMD_BRIGHT_MIN_TH2)
{
	giIMD_BRIGHT_MAX_TH1 = aiIMD_BRIGHT_MAX_TH1;
	giIMD_BRIGHT_MIN_TH1 = aiIMD_BRIGHT_MIN_TH1;
	giIMD_BRIGHT_MAX_TH2 = aiIMD_BRIGHT_MAX_TH2;
	giIMD_BRIGHT_MIN_TH2 = aiIMD_BRIGHT_MIN_TH2;

#if IMD_TEST
	printf("IMD_BRIGHT_TH = %d, %d, %d, %d\r\n", giIMD_BRIGHT_MAX_TH1, giIMD_BRIGHT_MIN_TH1, giIMD_BRIGHT_MAX_TH2, giIMD_BRIGHT_MIN_TH2);
#endif
}

int ISRT CombineRect(bRECT* rtSrc, const int aiNum)
{
	// Partition the Rect into the Mask
	#define IEC			>= 						// Include edge : >=	> 가 맞지만, >= 가 BOX개수가 감소하는 효율이 좋음
	#define MAO			->						// member access operator

	#define ISOLT(T,R1,R2) {\
		o##T = 0;\
		if(R1 MAO s##T < R2 MAO s##T) {\
			if	(R1 MAO e##T IEC R2 MAO s##T)	o##T = 1;\
		}\
		else {\
			if	(R2 MAO e##T IEC R1 MAO s##T)	o##T = 1;\
		}\
	}

	#define ISOL(OS,R1,R2)	{\
		ISOLT(x,R1,R2);\
		ISOLT(y,R1,R2);\
		OS = ox & oy;\
	}

	int i, j, k;
	int ox, oy, isOL;
	BYTE EQU[aiNum];	// TODO KSH> 동적 할당 - CombineRect()에서 EQU
	//GrpIMD(7,sizeof(EQU))

	// Combine Rect
	for (i=0; i<aiNum; i++) EQU[i] = i;	// Make lookup table

	int eqMax, eqMin;

	for(i=0; i<(aiNum/*-1*/); i++) {
		for(j=0/*+i*/; j<aiNum; j++)
		{
			if(i == j || EQU[i] == EQU[j]) continue;

			ISOL(isOL, (rtSrc+EQU[i]), (rtSrc+EQU[j]) );

			if(isOL) {
				if(EQU[i] > EQU[j]) {
					eqMax = EQU[i];
					eqMin = EQU[j];
				}
				else {
					eqMax = EQU[j];
					eqMin = EQU[i];
				}

				for(k=0/*i*/; k<aiNum; k++)
				{
					if( *(EQU+k) == eqMax ) {
						*(EQU+k) = eqMin;

						if((rtSrc+eqMin)->sx > (rtSrc+eqMax)->sx) (rtSrc+eqMin)->sx = (rtSrc+eqMax)->sx;
						if((rtSrc+eqMin)->ex < (rtSrc+eqMax)->ex) (rtSrc+eqMin)->ex = (rtSrc+eqMax)->ex;
						if((rtSrc+eqMin)->sy > (rtSrc+eqMax)->sy) (rtSrc+eqMin)->sy = (rtSrc+eqMax)->sy;
						if((rtSrc+eqMin)->ey < (rtSrc+eqMax)->ey) (rtSrc+eqMin)->ey = (rtSrc+eqMax)->ey;
					}
				}
			}
		}
	}

	for(i=0,k=0; k<aiNum; k++){
		if( *(EQU+k)==k ) {
			if(k != i) {
				//*bRectIMD(i) = *bRectIMD(k);
				*(rtSrc+i) = *(rtSrc+k);
			}
			i++;
			//if(i>IMD_OUT_EA) break;
		}
	}

	return i;
}

void ISRT LibIMD(const int aIMD_HW, const int aIMD_VW, const int aIMD_DET_EA, const int aIMD_FPS)
{
	#define IMD_HW_REG_NUM	4		// 만약 고정이 아닌 경우 iHPOS가 120일 때마다 iVPOS를 증가하는 방식으로 수정 필요
	#define IMD_LUT_SIZE 	255		// IMD_IMG[0]의 최대값

	int i,j,k,x;
	int iHPOS = 0;

	__attribute__((__aligned__(4))) BYTE IMD_IMG[aIMD_HW*aIMD_VW];	// 4K MAX = 8040 byte (2M = 1980 byte, 1M = 880 byte)	// TODO KSH> 동적 할당 - LibIMD()에서 IMD_IMG
	BYTE *pC = IMD_IMG;
	BYTE *pL = IMD_IMG;
	BYTE *pU = IMD_IMG;

	int	iLabelValue = 1;
	int MaxEq, MinEq;

	BYTE EQU[IMD_LUT_SIZE];			// MAX = 255 byte
	for (i=0; i<IMD_LUT_SIZE; i++) EQU[i] = i;	// Make lookup table

	//---------------------------------------------------------------------------------------------
	// Labeling
	const UINT nBuf = GetIsp(0x600);		// Reg. Read Start command

#if 0
	for(i=0; i<(aIMD_VW*IMD_HW_REG_NUM); i+=IMD_HW_REG_NUM)
	{
		iHPOS = 0;
		for(k=0; k<IMD_HW_REG_NUM; k++)
		{
			const UINT nIMD_OUT = GetIsp(0x600+1+i+k);	// 무조건 순서대로 읽어야 함 !!!

			for(j=31; j>=0 && iHPOS<aIMD_HW; j--)
			{
				*pC++ = (nIMD_OUT>>j)&0x1;
				iHPOS++;
			}
		}
	}

	pC = IMD_IMG;

	// Calculate labeling of first point.
	if(*pC)	*pC = iLabelValue++;
	else	*pC = 0;
	pC++;

	// Calculate labeling of first line.
	for(j=1; j<aIMD_HW; j++)
	{
		const int iBinL = (*pL > 0);

		if(*pC == 0)		  *pC = 0;
		else if(*pC != iBinL) *pC = iLabelValue++;	// Give new ID (label)
		else				  *pC = *pL;				// Give left label

		pC++;
		pL++;
	}

	for(i=1; i<aIMD_VW; i++)
	{
		for(j=0; j<aIMD_HW; j++)
		{
			const int iBinL = (j) ? (*pL > 0) : 0;
			const int iBinU = (*pU > 0);

			const int CeqU = (*pC == iBinU);
			const int CeqL = (*pC == iBinL);

			if ((*pC == 0) || (iLabelValue == IMD_LUT_SIZE))	*pC = 0;
			else if ((!CeqU) && (!CeqL))	*pC = iLabelValue++; 	// Add new label
			else if ((!CeqU) &&   CeqL )	*pC = *pL;				// Give left label
			else if (  CeqU  && (!CeqL))	*pC = *pU;				// Give up label
			else {	// if ( CeqU && CeqL)

				*pC = *pU;

				const int v = EQU[*pU];		// Look up table value of up label
				const int w = EQU[*pL];		// Look up table value of left label

				if (v != w) {
					if(v > w) { MaxEq = v; MinEq = w; }
					else	  { MaxEq = w; MinEq = v; }

					for(x=0; x<iLabelValue; x++){
						if(EQU[x] == MaxEq) EQU[x] = MinEq;
					}
				}
			}

			pC++;
			pL++;
			pU++;
		}
	}

#else

  #if 0
	// Calculate labeling of first line.
	for(k=0; k<IMD_HW_REG_NUM; k++)
	{
		const UINT nIMD = GetIsp(0x600+1+k);	// 무조건 순서대로 읽어야 함 !!!

		for(j=31; j>=0 && iHPOS<aIMD_HW; j--)
		{
			const int iBinC = (nIMD>>j)&0x1;
			const int iBinL = (iHPOS) ? (*pL > 0) : 0;

			if(iBinC == 0)			*pC = 0;
			else if(iBinC != iBinL) *pC = iLabelValue++;	// Give new ID (label)
			else					*pC = *pL;				// Give left label

			pC++;
			if(iHPOS) pL++;
			iHPOS++;
		}
	}
  #else// 코드는 길지만 처리 속도가 좀더 빠름
	const UINT nIMD0 = GetIsp(0x600+1);		// 무조건 순서대로 읽어야 함 !!!

	// Calculate labeling of first point.
	if((nIMD0>>31)&0x1)	*pC = iLabelValue++;
	else				*pC = 0;
	pC++;
	iHPOS++;

	// Calculate labeling of first line.
	for(j=30; j>=0 && iHPOS<aIMD_HW; j--)
	{
		const int iBinC = (nIMD0>>j)&0x1;
		const int iBinL = (*pL > 0);

		if(iBinC == 0)			*pC = 0;
		else if(iBinC != iBinL) *pC = iLabelValue++;	// Give new ID (label)
		else					*pC = *pL;				// Give left label

		pC++;
		pL++;
		iHPOS++;
	}

	for(k=1; k<IMD_HW_REG_NUM; k++)
	{
		const UINT nIMD = GetIsp(0x600+1+k);	// 무조건 순서대로 읽어야 함 !!!

		for(j=31; j>=0 && iHPOS<aIMD_HW; j--)
		{
			const int iBinC = (nIMD>>j)&0x1;
			const int iBinL = (*pL > 0);

			if(iBinC == 0)			*pC = 0;
			else if(iBinC != iBinL) *pC = iLabelValue++;	// Give new ID (label)
			else					*pC = *pL;				// Give left label

			pC++;
			pL++;
			iHPOS++;
		}
	}
  #endif

	// Calculate labeling from second line.
	for(i=IMD_HW_REG_NUM; i<(aIMD_VW*IMD_HW_REG_NUM); i+=IMD_HW_REG_NUM)
	{
		iHPOS = 0;

		for(k=0; k<IMD_HW_REG_NUM; k++)
		{
			const UINT nIMD = GetIsp(0x600+1+i+k);	// 무조건 순서대로 읽어야 함 !!!

			for(j=31; j>=0 && iHPOS<aIMD_HW; j--)
			{
				const int iBinC = (nIMD>>j)&0x1;
				const int iBinL = (iHPOS) ? (*pL > 0) : 0;
				const int iBinU = (*pU > 0);

				const int CeqU = (iBinC == iBinU);
				const int CeqL = (iBinC == iBinL);

				if ((iBinC == 0) || (iLabelValue == IMD_LUT_SIZE))	*pC = 0;
				else if ((!CeqU) && (!CeqL))	*pC = iLabelValue++; 	// Add new label
				else if ((!CeqU) &&   CeqL )	*pC = *pL;				// Give left label
				else if (  CeqU  && (!CeqL))	*pC = *pU;				// Give up label
				else {	// if ( CeqU && CeqL)

					*pC = *pU;

					const int v = EQU[*pU];		// Look up table value of up label
					const int w = EQU[*pL];		// Look up table value of left label

					if (v != w) {
						if(v > w) { MaxEq = v; MinEq = w; }
						else	  { MaxEq = w; MinEq = v; }

						for(x=0; x<iLabelValue; x++){
							if(EQU[x] == MaxEq) EQU[x] = MinEq;
						}
					}
				}

				pC++;
				pL++;
				pU++;
				iHPOS++;
			}
		}
	}
#endif

	// Calculate the number of label.
	k = 0;
	for(x=0; x<iLabelValue; x++) {
		if (EQU[x] == x) EQU[x] = k++;
		else			 EQU[x] = EQU[EQU[x]];
	}
	k--;
	//GrpIMD(0,k)		// 라벨링 개수

	//---------------------------------------------------------------------------------------------
	// Check the Camera Moving & Bright Change
#if 1	// TODO KSH ◆ 4K 에서 Camera Moving & Bright Change 테스트 필요
	static BYTE bLabelNum[2] = { 0, 0 };
	gbIMD_Moving = (k >= (giIMD_MOVING_TH + bLabelNum[0])) || (k >= (giIMD_MOVING_TH + bLabelNum[1]));
	bLabelNum[1] = bLabelNum[0];
	bLabelNum[0] = k;

	gbIMD_Bright = (gbIMD_Moving) ? (giErr1d < giIMD_BRIGHT_MIN_TH2) || (giIMD_BRIGHT_MAX_TH2 < giErr1d) :
									(giErr1d < giIMD_BRIGHT_MIN_TH1) || (giIMD_BRIGHT_MAX_TH1 < giErr1d) ;

	if(gbIMD_Moving || gbIMD_Bright) gbIMD_Update = aIMD_FPS;			// Camera Moving or Bright Change (1s)
	else if(gbIMD_Update) gbIMD_Update--;

	GrpIMD2(0,k)	// 검출된 영역 개수 (MAX=IMD_LUT_SIZE)
	GrpIMD2(1,IMD_WCNT1r)
	GrpIMD2(2,IMD_WCNT2r)
	GrpIMD2(3,IMD_WCNT3r)
	GrpIMD2(4,IMD_WCNT4r)
	GrpIMD2(5,giErr1d)
	GrpIMD2(6,gbIMD_Moving)
	GrpIMD2(7,gbIMD_Bright)

	if(k==0 || gbIMD_Update) { gnBoxNumIMD = 0; return; }
#endif
	//---------------------------------------------------------------------------------------------
	// Initialize the Rect
	x = MIN(k, aIMD_DET_EA);

	for(i=0; i<x; i++) {
		bRectIMD(i)->sx = aIMD_HW+1;
		bRectIMD(i)->ex = 0;
		bRectIMD(i)->sy = aIMD_VW+1;
		bRectIMD(i)->ey = 0;
	}

	GrpIMD(0,x)		// 검출된 영역 개수 (MAX=aIMD_DET_EA)

	// Calculate rect position.
	pC = IMD_IMG;

	for(i=0; i<aIMD_VW; i++)
	{
		for(j=0; j<aIMD_HW; j++, pC++)
		{
			//FontHex(i,20+j, EQU[*pC], 1);		// Display Label Values

			if (*pC == 0) continue;

			const UINT nLabel = EQU[*pC] - 1;

			if(nLabel >= aIMD_DET_EA) continue;

			if(bRectIMD(nLabel)->sx > j    ) bRectIMD(nLabel)->sx = j;
			if(bRectIMD(nLabel)->ex < (j+1)) bRectIMD(nLabel)->ex = j+1;
			if(bRectIMD(nLabel)->sy > i    ) bRectIMD(nLabel)->sy = i;
			if(bRectIMD(nLabel)->ey < (i+1)) bRectIMD(nLabel)->ey = i+1;
		}
	}

#if 0
	for(i=0; i<aIMD_VW; i++)
	{
		for(j=0; j<aIMD_HW; j++)
		{
			FontBin(i,20+j, IMD_IMG[i*aIMD_HW+j], 1);
		}
	}
#endif

	//---------------------------------------------------------------------------------------------
	// Masking & Combine
	GrpIMD(1,0)
	GrpIMD(2,0)
	GrpIMD(3,0)
	GrpIMD(4,0)
	GrpIMD(5,0)
	GrpIMD(6,0)

#if 0
	#define IEC2		>

	#define ISOLT2(T,R1,R2) {\
		o##T = 0;\
		if(R1 MAO s##T < R2 MAO s##T) {\
			if	(R1 MAO e##T IEC2 R2 MAO s##T)	o##T = 1;\
		}\
		else {\
			if	(R2 MAO e##T IEC2 R1 MAO s##T)	o##T = 1;\
		}\
	}

	#define ISOL2(OS,R1,R2)	{\
		ISOLT2(x,R1,R2);\
		ISOLT2(y,R1,R2);\
		OS = ox & oy;\
	}

	int isOL;
	int ox, oy;
#endif

	if(x > 0 && gnBoxMskPosNum > 1)
	{
		UINT* pnIMD_IMG = (UINT*)IMD_IMG;

		for(i=0; i<x; i++)
		{
			//*bRectOri(i) = *bRectIMD(i);
			*(pnIMD_IMG + i) = *(gnBoxPosIMD + i);	// IMD_IMG가 gnBoxPosIMD보다 크므로 4의 배수가 아니어도 됨
		}
#if 1
		j = 0;
		for(k=0; k<gnBoxMskPosNum && j<aIMD_DET_EA; k++)
		{
			const int iStaj = j;
			for(i=0; i<x && j<aIMD_DET_EA; i++)
			{
				//ISOL2(isOL, bRectOri(i), bRectMsk(k));
				//if(isOL) {
					bRectIMD(j)->sx = MAX(bRectOri(i)->sx, bRectMsk(k)->sx);
					bRectIMD(j)->ex = MIN(bRectOri(i)->ex, bRectMsk(k)->ex);
					bRectIMD(j)->sy = MAX(bRectOri(i)->sy, bRectMsk(k)->sy);
					bRectIMD(j)->ey = MIN(bRectOri(i)->ey, bRectMsk(k)->ey);
					//j++;
				//}

				if(bRectIMD(j)->sx < bRectIMD(j)->ex && bRectIMD(j)->sy < bRectIMD(j)->ey) j++;
			}

			GrpIMD(k+1, j - iStaj, if(k<3))	// MASK 처리 후 (MAX=aIMD_DET_EA)
			j = iStaj + CombineRect(bRectIMD(iStaj), j - iStaj);
			GrpIMD(k+4, j - iStaj, if(k<3))	// 결합 후 최종 검출 개수 (MAX=IMD_OUT_EA <- 현재 적용 안함)

			gbBoxMskDet[k] = j - iStaj;
		}
#else
		j = x;
		for(i=0; i<x && j<aIMD_DET_EA; i++)
		{
			for(k=0; k<gnBoxMskPosNum && j<aIMD_DET_EA; k++)
			{
				ISOL2(isOL, bRectIMD(i), bRectMsk(k));

				if(isOL) {
					bRectIMD(j)->sx = MAX(bRectIMD(i)->sx, bRectMsk(k)->sx);
					bRectIMD(j)->ex = MIN(bRectIMD(i)->ex, bRectMsk(k)->ex);
					bRectIMD(j)->sy = MAX(bRectIMD(i)->sy, bRectMsk(k)->sy);
					bRectIMD(j)->ey = MIN(bRectIMD(i)->ey, bRectMsk(k)->ey);
					j++;
				}
			}
			j--;
			*bRectIMD(i) = *bRectIMD(j);
		}
#endif
		gnBoxNumIMD = j;
	}
	else {
		GrpIMD(1,x)
		gnBoxNumIMD = CombineRect(bRectIMD(0), x);
		GrpIMD(4,gnBoxNumIMD)

		gbBoxMskDet[0] = gnBoxNumIMD;
	}
}



/* EOF */

