/* **************************************************************************
 File Name	: isp_lib_defog.c
 Description: Defog
 Designer	: GH Lee,
 Date		: 14. 8. 6
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "isp_lib_main.h"


#define ENABLE_HISTOGRAM	0
#define DWDR_INIT_CNT		4

#define HistoX				100 	// OSD x pos
#define HistoY				0
#define HistoW				640 	// width
#define HistoH				200 	// height

#define DefogVmax			0x80 	// Defog weight max.
#define DefogV0wt			0x30 	// Defog V0 weight
#define DefogV0min			40 		// Defog V0 min.

#define DEFOG_HISTO_NUM 	11 		// The number of histogram.
									// It is Fixed because of the defog algorithm

//------------------------------------------------------------------------------
//
unsigned int 	gnHistoM[DEFOG_HISTO_NUM];
//unsigned int 	gnDefogV;
//unsigned int 	gnDwdrV;

unsigned char 	gbContrastMode;
unsigned char 	gbDefogMode;
unsigned char 	gbDefogLevel;

unsigned int 	gnHistoV[DEFOG_HISTO_NUM];

extern BYTE gbContDebugOn;

//------------------------------------------------------------------------------
//
void ISRT LibDefog(unsigned int * anDefogThres, unsigned int * anDefogWeight, unsigned int *anDefogTbl, const BYTE abDssOn)
{	// De-fog
	int i;

	//----------------------------------------------------------------------------------------
	gnHistoV[10] = 0;
	unsigned int nMax1 = 10;
	unsigned int nMax2 = 10;
	unsigned int nHistoVsum = 0;
	unsigned int nHistoVsumR = 0;

	for(i=0; i<10; i++)
	{
		//#define STR_HISTO_Y		10
		//#define STR_HISTO_X		24
		//FontHex(STR_HISTO_Y+i, STR_HISTO_X, gnHistoM[i], 2);
		//SETFONTID(STR_HISTO_Y+i, STR_HISTO_X+2, ':');
		//DispDec(STR_HISTO_Y+i, STR_HISTO_X+3, gnHistoV[i], 10);

		nHistoVsum += gnHistoV[i];

		if(i == 0)	gnHistoV[i] = (gnHistoV[i] * 0x10) / gnHistoM[i];

		else {
			gnHistoV[i] = (gnHistoV[i] * 0x10) / (gnHistoM[i] - gnHistoM[i-1]);
			nHistoVsumR += gnHistoV[i];
		}

		if(gnHistoV[nMax1] < gnHistoV[i]) {	nMax2 = nMax1;	nMax1 = i;	}
		else if(gnHistoV[nMax2] < gnHistoV[i]) {	nMax2 = i;	}

	}

	gnHistoV[i] = gnAe1Size - nHistoVsum;
	gnHistoV[i] = (gnHistoV[i] * 0x10) / (gnHistoM[i] - gnHistoM[i-1]);

	if(gnHistoV[nMax1] < gnHistoV[i]) {
		nMax2 = nMax1;
		nMax1 = i;
	}
	else if(gnHistoV[nMax2] < gnHistoV[i]) {
		nMax2 = i;
	}

	//----------------------------------------------------------------------------------------
	// Set Color

	const unsigned int nMaxR1 = (nMax1 == 0)	? nMax2 : nMax1;
	const unsigned int nMaxR2 =	(nMaxR1 == 1)	? 2 :
								(nMaxR1 == 10)	? 9 :
								(gnHistoV[nMaxR1-1] > gnHistoV[nMaxR1+1]) ? nMaxR1-1 : nMaxR1+1 ;

	const unsigned int nHistoA = (gnHistoV[nMaxR1] + gnHistoV[nMaxR2]);
	const unsigned int nHistoB = nHistoVsumR - nHistoA;
	const unsigned int nHistoR = (nHistoA > nHistoB) ?	((HistoH>>1) * (nHistoA - nHistoB)) / nHistoA : 0 ;

	const unsigned int nHistoD = (gnHistoV[nMaxR1] <= (gnHistoV[nMaxR2]<<1)) ? 0 :
								 ((HistoH>>1) * (gnHistoV[nMaxR1] - (gnHistoV[nMaxR2]<<1))) / gnHistoV[nMaxR1] ;

	const unsigned int nHistoVmax = gnHistoV[nMax1];


	//DebugDisp(gbContDebugOn, , "nMaxR1", 10, 24, nMaxR1	);
	//DebugDisp(gbContDebugOn, , "nMaxR2", 11, 24, nMaxR2	);
	//DebugDisp(gbContDebugOn, , "nHistoA", 12, 24, nHistoA	);
	//DebugDisp(gbContDebugOn, , "nHistoB", 13, 24, nHistoB	);
	//DebugDisp(gbContDebugOn, , "nHistoR", 14, 24, nHistoR	);
	//DebugDisp(gbContDebugOn, , "nHistoD", 15, 24, nHistoD	);


	for(i=0; i<DEFOG_HISTO_NUM; i++)
	{
		gnHistoV[i] *= HistoH;
		gnHistoV[i] /= nHistoVmax;
	}

	const unsigned int nHistoV0 = (gnHistoV[0] > DefogV0min) ? 0 : ((DefogV0min - gnHistoV[0]) * DefogV0wt) / DefogV0min ;
	const unsigned int nHistoRD = (nHistoR > nHistoD) ? ((nHistoR - nHistoD) * (DefogVmax - DefogV0wt)) / (HistoH>>1) : 0;

	const unsigned int gnDefogV = (gnHistoV[0] > DefogV0min) ? 0 : nHistoV0 + nHistoRD;

	//const unsigned int nHistoV123 = (gnHistoV[1] + gnHistoV[2] + gnHistoV[3]);

	//const unsigned int gnDwdrV = (nMax1 == 0 && gnHistoV[0] > nHistoV123) ? gnHistoV[0] - nHistoV123 : 0;

	//----------------------------------------------------------------------------------------
	//
	static unsigned int nDefogVprv = 0;

	static unsigned int nDwdrInitCnt = 0;
	//static unsigned int nDwdrVprv = 0; 					// 0 when menu is changed. Andthen, increases to 0x80

	if(nDwdrInitCnt < DWDR_INIT_CNT){
		nDwdrInitCnt++;
	}

	if(nDwdrInitCnt >= DWDR_INIT_CNT) {

		if(nDefogVprv > gnDefogV)		nDefogVprv--;
		else if(nDefogVprv < gnDefogV)	nDefogVprv++;


		//if(nDwdrVprv < 0x80) nDwdrVprv++;
	}

	static int iContrastMode1d = 0;
	if(iContrastMode1d != gbContrastMode)
	{
		iContrastMode1d = gbContrastMode;
		nDefogVprv = 0;
		//nDwdrVprv = 0;
		nDwdrInitCnt = 0;
	}

	#define nDefogVprvOut	nDefogVprv

	if(nDwdrInitCnt < DWDR_INIT_CNT || abDssOn) { 				// DSS
		if(gbDefogMode == UP_DEFOG_AUTO) {								// AUTO		// 160201 KSH
//			HEQ_TH_EN773Aw(0x0); 										// Off
			*anDefogThres = 0; 											// Off
		}
		else {															// MANUAL	// 160201 KSH
//			HEQ_TH_EN773Aw(anDefogTbl[gbDefogLevel]);
			*anDefogThres = anDefogTbl[gbDefogLevel];								// 160201 KSH
		}
	}
	else if((gbContrastMode==UP_ON) && (!abDssOn)) { 				// Not DSS

		if(gbDefogMode == UP_DEFOG_AUTO) {								// AUTO
		 	const unsigned int nDefogTh = ((nDefogVprvOut*0x20) > anDefogTbl[gbDefogLevel]) ? anDefogTbl[gbDefogLevel] : (nDefogVprvOut*0x20);
//			HEQ_TH_EN773Aw(nDefogTh);
			*anDefogThres = nDefogTh;
		}
		else {															// MANUAL
//			HEQ_TH_EN773Aw(anDefogTbl[gbDefogLevel]);
			*anDefogThres = anDefogTbl[gbDefogLevel];
		}
	}

	//----------------------------------------------------------------------------------------
	//
#if ENABLE_HISTOGRAM == 1
 	BOSD_ONw(BOSD_ONr | 0x0000ffff);

	for (i = 0; i < 30; i++) { 				// without 31, 30th box of menu
		SetIsp(0x278 + i, 0xe3a52c); 		// LATT
		SetIsp(0x1C0 + i, 0x0c415cff); 		// AATT
	}

	// Set Main Line
	LINEH(0, HistoX, HistoY+HistoH, HistoX+HistoW);

	OZONE_SELw(0x1);
	AE_WIN_SELw(0x1);

	UINT sx = HistoX;
	UINT ex = sx + (gnHistoM[0]<<2);
	UINT sy = HistoY + HistoH;
	UINT ey = HistoY + HistoH + gnHistoV[0];

	BOX(1,sx,sy,ex,ey);

	sx = ex;	ex = sx + ((gnHistoM[1] - gnHistoM[0])<<2);		ey = (HistoY + HistoH) + gnHistoV[1];	BOX(2,sx,sy,ex,ey);
	sx = ex;	ex = sx + ((gnHistoM[2] - gnHistoM[1])<<2);		ey = (HistoY + HistoH) + gnHistoV[2];	BOX(3,sx,sy,ex,ey);
	sx = ex;	ex = sx + ((gnHistoM[3] - gnHistoM[2])<<2);		ey = (HistoY + HistoH) + gnHistoV[3];	BOX(4,sx,sy,ex,ey);
	sx = ex;	ex = sx + ((gnHistoM[4] - gnHistoM[3])<<1);		ey = (HistoY + HistoH) + gnHistoV[4];	BOX(5,sx,sy,ex,ey);
	sx = ex;	ex = sx + ((gnHistoM[5] - gnHistoM[4])<<1);		ey = (HistoY + HistoH) + gnHistoV[5];	BOX(6,sx,sy,ex,ey);
	sx = ex;	ex = sx + ((gnHistoM[6] - gnHistoM[5])<<1);		ey = (HistoY + HistoH) + gnHistoV[6];	BOX(7,sx,sy,ex,ey);
	sx = ex;	ex = sx + ((gnHistoM[7] - gnHistoM[6])<<1);		ey = (HistoY + HistoH) + gnHistoV[7];	BOX(8,sx,sy,ex,ey);
	sx = ex;	ex = sx + ((gnHistoM[8] - gnHistoM[7])<<1);		ey = (HistoY + HistoH) + gnHistoV[8];	BOX(9,sx,sy,ex,ey);
	sx = ex;	ex = sx + ((gnHistoM[9] - gnHistoM[8])<<1);		ey = (HistoY + HistoH) + gnHistoV[9];	BOX(10,sx,sy,ex+1,ey);

#endif

	if ((gbContrastMode == UP_OFF)){
//		HEQ_WG_EN773Aw(0x1); 			// Image 25%, ACE 75%
//		HEQ_TH_EN773Aw(0x0); 			// Defog off

//			*anDefogWeight = 1; 				// Image 25%, ACE 75%
		*anDefogWeight = 64; 				// Image 25%, ACE 75%
		*anDefogThres = 0; 				// Defog off
		return;
	}
}

