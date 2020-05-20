/* **************************************************************************
 File Name	:	isp_cont.c
 Description:	EN675 - Gamma, ACE & Defog function
 Designer	:	Kim, Sunghoon
 Modified by:	Lee, Wanhee
 Date		:	14. 8. 4 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"

//=================================================================================================
//	reserved define & tables
//=================================================================================================
// Gamma table
//#if (model_Contrast==1)	// New

const UINT	gnTblGamma_NY0[9] = {0x0000003d,0x00540073,0x009d00d7,0x01260191,0x01e20224,0x025e0292,0x02ed033d,0x038403c4,0x03ff0000}; // Y gamma-0 (0.45)
const UINT	gnTblGamma_NY1[9] = {0x00000009,0x0010001a,0x002d004c,0x008000d7,0x0123016a,0x01ac01eb,0x026102d0,0x0339039f,0x03ff0000}; // Y gamma-1 (0.75)
const UINT	gnTblGamma_NC0[9] = {0x0000009d,0x00c200ee,0x0126016a,0x01bd0224,0x026b02a3,0x02d202fb,0x033f0379,0x03ab03d8,0x03ff0000}; // c gamma-0 (0.3)
const UINT	gnTblGamma_NC1[9] = {0x00000018,0x00240037,0x00540080,0x00c20126,0x017701bd,0x01fd0238,0x02a30304,0x035e03b1,0x03ff0000}; // c gamma-1 (0.6)

//#else // Old		// 180426 KSH

const UINT	gnTblGamma_OY0[9] = {0x12      ,0x28005d  ,0xaf0110  ,0x183021b ,0x28a02e7 ,0x337037e ,0x3fc03ff ,0x3ff03ff ,0x3ff0000};
//const UINT	gnTblGamma_OY1[9] = {0x9       ,0x140026  ,0x46007a  ,0xd20166  ,0x1e8025f ,0x2cc0337 ,0x3fe03ff ,0x3ff03ff ,0x3ff0000};
const UINT	gnTblGamma_OC0[9] = {0x18      ,0x320080  ,0xf00168  ,0x1e0026e ,0x2d0031f ,0x360039a ,0x3fc03ff ,0x3ff03ff ,0x3ff0000};
//const UINT	gnTblGamma_OC1[9] = {0xd       ,0x190030  ,0x560096  ,0xf80192  ,0x2120282 ,0x2ea0348 ,0x3fc03ff ,0x3ff03ff ,0x3ff0000};

//#endif

const UINT	gnTblGamma_WY0[9] = {0x6       ,0x11002f  ,0x64009f  ,0xd9011d  ,0x1520182 ,0x1b901f9 ,0x2760302 ,0x38603de ,0x3ff0000};	// 14.09.04	0.7_1023 base
const UINT	gnTblGamma_WY1[9] = {0x0000003c,0x005b0082,0x00be0122,0x018f01e3,0x02130244,0x028502d4,0x035e03bb,0x03e803ff,0x03ff0000};	// 150802
const UINT	gnTblGamma_WC1[9] = {0x00000006,0x00120046,0x00960104,0x016801c7,0x0203023f,0x028002d0,0x035c03b6,0x03e803ff,0x03ff0000};	// 151118


#define		gnTblGamma_75		gnTblGamma_NY1
const UINT	gnTblGamma_70[9]  = {0x0000000d,0x00150022,0x0037005a,0x009300ef,0x013d0184,0x01c50203,0x027602e1,0x034503a5,0x03ff0000};
const UINT	gnTblGamma_65[9]  = {0x00000011,0x001b002b,0x0044006b,0x00a90109,0x015901a0,0x01e1021d,0x028c02f2,0x035103ab,0x03ff0000};
#define		gnTblGamma_60		gnTblGamma_NC1
const UINT	gnTblGamma_55[9]  = {0x00000021,0x00300047,0x00680098,0x00de0146,0x019801dd,0x021c0255,0x02bb0317,0x036a03b8,0x03ff0000};
const UINT	gnTblGamma_50[9]  = {0x0000002d,0x0040005a,0x008000b5,0x0100016a,0x01bb0200,0x023c0273,0x02d40329,0x037703be,0x03ff0000};
#define		gnTblGamma_45		gnTblGamma_NY0
const UINT	gnTblGamma_40[9]  = {0x00000054,0x006f0093,0x00c20100,0x015101bd,0x020c024c,0x028302b3,0x03080350,0x039103cb,0x03ff0000};
const UINT	gnTblGamma_35[9]  = {0x00000073,0x009300bb,0x00ee0130,0x018401ee,0x023a0276,0x02a902d6,0x03230364,0x039e03d1,0x03ff0000};
#define		gnTblGamma_30		gnTblGamma_NC0
//const UINT	gnTblGamma_25[9]  = {0x000000d7,0x01000130,0x016a01ae,0x02000261,0x02a102d4,0x02fd0321,0x035d038e,0x03b903de,0x03ff0000};
//const UINT	gnTblGamma_20[9]  = {0x00000126,0x01510184,0x01bd0200,0x024c02a3,0x02dc0308,0x032b0349,0x037b03a4,0x03c603e5,0x03ff0000};
//const UINT	gnTblGamma_15[9]  = {0x00000191,0x01bd01ee,0x02240260,0x02a302ed,0x031c033f,0x035c0374,0x039b03ba,0x03d403eb,0x03ff0000};

const UINT*	gpTblGamma[] = { gnTblGamma_40/*gnTblGamma_30*/, gnTblGamma_40/*gnTblGamma_35*/, gnTblGamma_40, gnTblGamma_45, gnTblGamma_50, gnTblGamma_55, gnTblGamma_60, gnTblGamma_65, gnTblGamma_70, gnTblGamma_75 };


//*************************************************************************************************
// Variables
//-------------------------------------------------------------------------------------------------
const UINT gnDefog_HS = RP(AE_HSP) + DEFOG_X_OFF;	// Histogram detection area H start
const UINT gnDefog_HE = RP(AE_HEP) - DEFOG_X_OFF;	// "						H end
const UINT gnDefog_VS = RP(AE_VSP);				// "						V start
const UINT gnDefog_VE = (RP(AE_VEP)<<1)/3;			// "						V end
const UINT gnAe1Size = (gnDefog_HE - gnDefog_HS + 1) * (gnDefog_VE - gnDefog_VS + 1);

BYTE gbContDebugOn=0, gbGamma_Excute=0;


//=================================================================================================
//	Functions
//=================================================================================================
// Init
void InitHisto(void)
{
#if 0
	//OSD_MUTEw(0);
	//BOX_HSPw(0);
	//BOX_VSPw(0);

	BOSD_ONw(/*0x7ffff*/0x1ffff);
	BOX_ATT( 0,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT( 8,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 1,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT( 9,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 2,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(10,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 3,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(11,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 4,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(12,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 5,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(13,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 6,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(14,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 7,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(15,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT(16,1,BOX_TONE_25,0xff,0x80,0x80);

	gbHistoM[0] = HISTO0_MAXr;
	gbHistoM[1] = HISTO1_MAXr;
	gbHistoM[2] = HISTO2_MAXr;
	gbHistoM[3] = HISTO3_MAXr;
	gbHistoM[4] = HISTO4_MAXr;
	gbHistoM[5] = HISTO5_MAXr;
	gbHistoM[6] = HISTO6_MAXr;
	gbHistoM[7] = HISTO7_MAXr;
	gbHistoM[8] = HISTO8_MAXr;
	gbHistoM[9] = HISTO9_MAXr;
	gbHistoM[10] = HISTO10_MAXr;
	gbHistoM[11] = HISTO11_MAXr;
	gbHistoM[12] = HISTO12_MAXr;
	gbHistoM[13] = HISTO13_MAXr;
	gbHistoM[14] = HISTO14_MAXr;
	gbHistoM[15] = (1<<HISTO_MAX_BIT);
#endif
}

void InitContrast(void)
{
	ACE_ONw(1);
	HEQ_ONw(1);
	HEQ_IIRKw(0xd0);	// IIR Speed

	ACE_VSPw(0x1c);		// TODO KSH - ACE_VSP : EN673버전만 0x1c, EN781버전은 확인 필요, 튜닝 시 ACE_OSD 로 확인
	ACE_HBw(RP(ACE_HB));
	ACE_VBw(RP(ACE_VB));
	ACE_HBSw(((model_4M)||(model_8M))?(0x3):(0x2));				// ACE block size
	ACE_VBSw(((model_4M)||(model_8M))?(0x3):(0x2));
	ACE_IIRw(0x2f);												// 161121 LGH

#if 0
	gnDefog_HS = RP(AE_HSP) + DEFOG_X_OFF;						// Histogram detection area H start
	gnDefog_HE = RP(AE_HEP) - DEFOG_X_OFF;						// "						H end
	gnDefog_VS = RP(AE_VSP);									// "						V start
	gnDefog_VE = RP(AE_VEP);									// "						V end

	const UINT nHW = gnDefog_HE - gnDefog_HS + 1;
	const UINT nVW = ((gnDefog_VE<<1)/3) - gnDefog_VS + 1;

	gnAe1Size = nHW * nVW;											// total size
#endif

	// Histogram X domain read
	#define HISTO_MAX_VAL0	0x10203040
	#define HISTO_MAX_VAL1	0x6080a0c0
	#define HISTO_MAX_VAL2	0xe0ff

	gnHistoM[10] = 0x100;

	UINT nHistoMax = HISTO_MAX_VAL0;
	gnHistoM[3] = nHistoMax&0xff;
	gnHistoM[2] = (nHistoMax>>8)&0xff;
	gnHistoM[1] = (nHistoMax>>16)&0xff;
	gnHistoM[0] = (nHistoMax>>24)&0xff;

	nHistoMax = HISTO_MAX_VAL1;
	gnHistoM[7] = nHistoMax&0xff;
	gnHistoM[6] = (nHistoMax>>8)&0xff;
	gnHistoM[5] = (nHistoMax>>16)&0xff;
	gnHistoM[4] = (nHistoMax>>24)&0xff;

	nHistoMax = HISTO_MAX_VAL2;
	gnHistoM[9] = nHistoMax&0xff;
	gnHistoM[8] = (nHistoMax>>8)&0xff;

	//gnDefogV = 0;
	//gnDwdrV = 0;
}

//-------------------------------------------------------------------------------------------------
// Gamma
UINT ISRT GetGamma(const UINT nG1, const UINT nG2, const UINT nGK)
{
	const UINT nG1L = nG1&0xffff;
	const UINT nG2L = nG2&0xffff;
	const UINT nG1H = nG1>>16;
	const UINT nG2H = nG2>>16;

	const UINT nGL = ((nG1L * (0x80-nGK)) + (nG2L * nGK))>>7;
	const UINT nGH = ((nG1H * (0x80-nGK)) + (nG2H * nGK))>>7;

	return (nGH<<16) | nGL;
}

void ISRT Gamma(void) // 180320 LWH
{
	gbContDebugOn = (gbMnDebugFnc==4);

	int i;
	static BYTE bWdrOnBuf = 0xAA, bMpGammaBuf = 0xAA, bMpMinGammaYBuf = 0xAA;
	static BYTE bMpGammaDayBuf = 0xAA, bMpGammaNgtBuf = 0xAA, bMpGammaWdrBuf = 0xAA;
	static WORD wWdrGmkBuf = 0xFFFF;
	//static BYTE bUpAE_WDR_STYLE_SEL = 0xAA;

	const BYTE bMpGamma = (gbWdrOn!=WDR_OFF) ? UP(GammaWdr) : UP(Gamma);	// TODO KSH ◆ WDR - Gamma()에서 Line WDR 설정을 Frame WDR로 하는게 더 좋은지 확인 필요

	const BYTE bAutoGammaOff = (UP(GammaDay) <= UP(GammaNgt)) || (UP(GammaDay) == 0) || (bMpGamma != UP_GAMMA_AUTO);

	gbGamma_Excute = /*(bUpAE_WDR_STYLE_SEL != UP(AE_WDR_STYLE_SEL)) ||*/ (bMpGammaWdrBuf != UP(GammaWdr)) || (bMpGammaDayBuf != UP(GammaDay)) || (bMpGammaNgtBuf != UP(GammaNgt)) || (!bAutoGammaOff) || (bWdrOnBuf != gbWdrOn) || (bMpGammaBuf != UP(Gamma)) || (wWdrGmkBuf != gwWdrGmk) || (bMpMinGammaYBuf != UP(MinGammaY)) || ((!AE_SAT_OFF)&&UP(MinGammaY)) ; // When the WDR or Gamma setting changed

	if(gbGamma_Excute)
	{
		const BYTE bYGMA_ON = YGMA_ONr;
		const BYTE bCGMA_ON = CGMA_ONr;

		if((gbWdrOn!=WDR_OFF) && (bMpGamma >= UP_GAMMA_WDR)) { // WDR Gamma
			const UINT nWdrGmk = gwWdrGmk>>4;

#if 0
			if (UP(AE_WDR_STYLE_SEL)==0) {	// 141209
				for (i=0; i<9; i++) SetIsp(YGAMMA_BASE+i, GetGamma(*(gnTblGamma_OY0+i), *(gnTblGamma_WY0+i), nWdrGmk));	// y gamma exchange
				for (i=0; i<9; i++) SetIsp(CGAMMA_BASE+i, GetGamma(*(gnTblGamma_OC0+i), *(gnTblGamma_WY0+i), nWdrGmk));	// c gamma exchange
			} else
#endif
			{
				for (i=0; i<9; i++) SetIsp(YGAMMA_BASE+i, GetGamma(*(gnTblGamma_OY0+i), *(gnTblGamma_WY1+i), nWdrGmk));	// y gamma exchange		// 150802
				for (i=0; i<9; i++) SetIsp(CGAMMA_BASE+i, GetGamma(*(gnTblGamma_OC0+i), *(gnTblGamma_WC1+i), nWdrGmk));	// c gamma exchange		// 151118
			}

		}
		else { // Normal Gamma
	#if 1
			if(bAutoGammaOff) {
				const BYTE bMpGammaO = (bMpGamma < UP_GAMMA_AUTO) ? bMpGamma : UP(GammaDay) ;	// GAMMA가 AUTO로 설정되어 있지만 bAutoGammaOff 조건인 경우 UP(GammaDay) 사용

				const UINT *pnTblGmaY = gpTblGamma[bMpGammaO+3];
				const UINT *pnTblGmaC = (gbWdrOn!=WDR_OFF) ? pnTblGmaY : gpTblGamma[bMpGammaO+0];		// Color Gamma를 낮추고 Color Gain을 올리는 것이 Color Noise가 덜하고 더 자연스러움

				for (i=0; i<9; i++) SetIsp(YGAMMA_BASE+i, *(pnTblGmaY+i));	// y gamma exchange
				for (i=0; i<9; i++) SetIsp(CGAMMA_BASE+i, *(pnTblGmaC+i));	// c gamma exchange
			}
			else {
				extern int iTgtMax;
				//extern int TgtMinGet(const int aiTgt, const BYTE abV);
				const int iTgtOri = iTgtMax;//TgtMaxGet(0,0);
				const int iTgtOriMin = TgtMinGet(iTgtOri, UP(BrightnessMin));					// UP(BrightnessMin) : BRIGHTNESS NIGHT

				extern int TgtSpotBg(const int aiTgt, const BYTE abV, const int aiTgtMin);
				const int iTgtSpotBgOri = TgtSpotBg(iTgtOri, UP(SatBrt), UP(AE_TGT_OFST));	// UP(SatBrt) : ANTI-SAT. WEIGHT

				BYTE bGammaIdx;
				extern UINT AutoGamma(const int, const int, const BYTE, const BYTE, BYTE*);
				UINT nAutoGmK = AutoGamma(iTgtSpotBgOri, iTgtOriMin, UP(GammaDay), UP(GammaNgt), &bGammaIdx);
				const UINT *pnTblGmaY0 = gpTblGamma[bGammaIdx+4];
				const UINT *pnTblGmaY1 = gpTblGamma[bGammaIdx+3];
				const UINT *pnTblGmaC0 = (gbWdrOn!=WDR_OFF) ? pnTblGmaY0 : gpTblGamma[bGammaIdx+1];		// Color Gamma를 낮추고 Color Gain을 올리는 것이 Color Noise가 덜하고 더 자연스러움
				const UINT *pnTblGmaC1 = (gbWdrOn!=WDR_OFF) ? pnTblGmaY1 : gpTblGamma[bGammaIdx+0];		// "

				for (i=0; i<9; i++) {
					const UINT nGmaY = GetGamma(*(pnTblGmaY0+i), *(pnTblGmaY1+i), nAutoGmK);
					const UINT nGmaC = GetGamma(*(pnTblGmaC0+i), *(pnTblGmaC1+i), nAutoGmK);
					SetIsp(YGAMMA_BASE+i, nGmaY);	// y gamma exchange
					SetIsp(CGAMMA_BASE+i, nGmaC);	// c gamma exchange
				}
			}
	#else
			if(AE_SAT_OFF || (UP(MinGammaY)==0)) {
				for (i=0; i<9; i++) SetIsp(YGAMMA_BASE+i, *(pnTblGmaY+i));	// y gamma exchange
			}
		#if AE_CTRL_ADV
			else {
				extern int iTgtMax;
				//extern int TgtMinGet(const int aiTgt, const BYTE abV);
				const int iTgtOri = iTgtMax;//TgtMaxGet(0,0);
				const int iTgtOriMin = TgtMinGet(iTgtOri, 0/*UP(BrightnessMin)*/);					// 0:최소가능밝기(BRIGHTNESS NIGHT)

			#if 1
				extern int TgtSpotBg(const int aiTgt, const BYTE abV, const int aiTgtMin);
				const int iTgtSpotBgOri = TgtSpotBg(iTgtOriMin, 20/*UP(SatBrt)*/, UP(AE_TGT_OFST));	// 20:최소가능밝기(ANTI-SAT. WEIGHT)
			#else
				int iTgtSpotBgOri = 20/*UP(SatBrt)*/ * (iTgtOriMin - UP(AE_TGT_OFST));	// 20:최소가능밝기(ANTI-SAT. WEIGHT)
				iTgtSpotBgOri = (iTgtOriMin - UP(AE_TGT_OFST)) - (((iTgtSpotBgOri<<2)-iTgtSpotBgOri+32)>>6);
				if(iTgtSpotBgOri < 0) iTgtSpotBgOri = 0;
			#endif

				BYTE bGammaIdx0;
				extern UINT SatGamma(const int, const int, BYTE*);
				const UINT nSatGmkY = SatGamma(iTgtOri, iTgtSpotBgOri, &bGammaIdx0);
				const UINT *pnTblGmaY0 = gpTblGamma[(UP(Gamma)<<1)+bGammaIdx0+1];
				const UINT *pnTblGmaY1 = gpTblGamma[(UP(Gamma)<<1)+bGammaIdx0+0];

				extern int iTgt;
				const UINT nSatGmkOff = ((iTgt-iTgtOriMin)<<7) / (iTgtOri-iTgtOriMin);	// iTgt==iTgtOri -> 0x80, iTgt==iTgtOriMin -> 0x0

				for (i=0; i<9; i++) {
					UINT nGmaY = GetGamma(*(pnTblGmaY0+i), *(pnTblGmaY1+i), nSatGmkY);

					if(UP(MinGammaMode)==UP_SAT_BACKLIGHT_DAY_ONLY) {
						nGmaY = GetGamma(*(pnTblGmaY+i), nGmaY, nSatGmkOff);
					}

					SetIsp(YGAMMA_BASE+i, nGmaY);	// y gamma exchange

				}
			}
		#endif
	#endif
		}
		YGMA_ONw(bYGMA_ON/*UP(GAMMA_Y_ON)*/);
		CGMA_ONw(bCGMA_ON/*UP(GAMMA_C_ON)*/);
	}
	bWdrOnBuf = gbWdrOn; bMpGammaBuf = UP(Gamma); wWdrGmkBuf = gwWdrGmk; // Setting value buffering
	bMpMinGammaYBuf = UP(MinGammaY);
	bMpGammaDayBuf = UP(GammaDay);
	bMpGammaNgtBuf = UP(GammaNgt);
	bMpGammaWdrBuf = UP(GammaWdr);
	//bUpAE_WDR_STYLE_SEL = UP(AE_WDR_STYLE_SEL);

	//GrpAe(GA_ERR_DAY) = YGMY_01r;

	//DebugDisp(gbContDebugOn, , "WDR_CNT_", 1, 24, gwWdrCnt 		);
	DebugDisp(gbContDebugOn, , "WDR_GMK_", 2, 24, gwWdrGmk>>4	);
}

//-------------------------------------------------------------------------------------------------
// ACE & Defog
void ISRT AceDefog(void)
{
	//if(gbContDebugOn) return;

	// Ace ---------------------------------------------------------------------------------
	const int iAceAgcPos = CLAMP(giCurAgc, ADNR_AGC_MIN, ADNR_AGC_MID);
	int 	iAceTh1=0, iAceTh2=0, iAceDth=3;
	int		iAceTh1S, iAceTh2S, iAceTh1E, iAceTh2E;
	BYTE 	bAceTblSel=0;
	BYTE	bAceGmgnMin=0;

#if 0
	#if (model_1M)
		ACE_HBw(0xa);
		ACE_VBw(0x5);

	#else
		if ((UP(OutFps)==UP_2M_720p_25_30)||(UP(OutFps)==UP_2M_720p_50_60)) { 		// Crop mode
			ACE_HBw(0xa);
			ACE_VBw(0x5);
		}
		else { 											// 2M
			// HBS,VBS=2 (Block Size = 128)
			ACE_HBw(0xf); // 15*128 = 1920
			ACE_VBw(0x8); //  8*128 = 1024 (Detection area should not exceed!)
		}
	#endif
#endif


	if (gbWdrOn!=WDR_OFF) {									// WDR on	// TODO KSH ◆ WDR - AceDefog()에서 Line WDR 설정을 Frame WDR로 하는게 더 좋은지 확인 필요

		ACE_BPw(0);			// Block bypass OFF

		bAceGmgnMin = (UP(Ace) != UP_4sOFF) ? UP(AceGmgn) : 0;
//		ACE_DTHw(0x4);

		iAceDth = 0x7;
		iAceTh1 = UP(ACEWDR1_TH);
		iAceTh2 = UP(ACEWDR2_TH);
	}
	else {													// WDR off
		if ((UP(Defog)==UP_ON) || (UP(Ace) != UP_4sOFF)) {	// ACE, Defog condition

			ACE_BPw(0);		// Block bypass OFF

			if (UP(Defog)==UP_ON && UP(Ace) == UP_4sOFF)	{
				bAceGmgnMin = 0;
				bAceTblSel = UP_4sMID;
			}
			else {
				bAceGmgnMin = UP(AceGmgn);
				bAceTblSel = UP(Ace);
			}

			switch (bAceTblSel) {
				case UP_4sLOW	:	iAceTh1S = 0;					iAceTh1 = UP(ACETBL1_LOW);	iAceTh1E = UP(ACETBL1_MID);
									iAceTh2S = 0;					iAceTh2 = UP(ACETBL2_LOW);	iAceTh2E = UP(ACETBL2_MID);
									break;
				default			:
				case UP_4sMID	:	iAceTh1S = UP(ACETBL1_LOW);	iAceTh1 = UP(ACETBL1_MID);	iAceTh1E = UP(ACETBL1_HIGH);
									iAceTh2S = UP(ACETBL2_LOW);	iAceTh2 = UP(ACETBL2_MID);	iAceTh2E = UP(ACETBL2_HIGH);
									break;
				case UP_4sHI	:	iAceTh1S = UP(ACETBL1_MID);	iAceTh1 = UP(ACETBL1_HIGH);	iAceTh1E = (iAceTh1<<1)-iAceTh1S;
									iAceTh2S = UP(ACETBL2_MID);	iAceTh2 = UP(ACETBL2_HIGH);	iAceTh2E = (iAceTh2<<1)-iAceTh2S;
									break;
			}

			iAceTh1S = MAX(0,iAceTh1S);
			iAceTh1  = MAX(iAceTh1S,iAceTh1);
			iAceTh1E = MAX(iAceTh1,iAceTh1E);

			iAceTh1 = (UP(AceBrt)>32)  ? (((iAceTh1E-iAceTh1) * (UP(AceBrt) -32))>>5) + iAceTh1 : (((iAceTh1-iAceTh1S) * UP(AceBrt))>>5)  + iAceTh1S ;
			iAceTh2 = (UP(AceCont)>32) ? (((iAceTh2E-iAceTh2) * (UP(AceCont)-32))>>5) + iAceTh2 : (((iAceTh2-iAceTh2S) * UP(AceCont))>>5) + iAceTh2S ;

//			ACE_DTHw(0x3);
			iAceDth = 0x3;
		}
		else {														// all off condition

			ACE_BPw(UP(IspGain)==0/*1*/);		// Block bypass ON

			bAceGmgnMin = 0;

			iAceDth = 0x3;
			iAceTh1 = 0;
			iAceTh2 = 0;
		}
	}

	iAceTh1 = LibUtlInterp1D(iAceAgcPos, ADNR_AGC_MIN, ADNR_AGC_MID, iAceTh1, 0x0);	// Decrease by AGC
	iAceTh2 = LibUtlInterp1D(iAceAgcPos, ADNR_AGC_MIN, ADNR_AGC_MID, iAceTh2, 0x0);	// "

	ACE_DTHw(iAceDth);
	ACE_TH1w(iAceTh1);
	ACE_TH2w(iAceTh2);

	DebugDisp(gbContDebugOn, , "ACE_TH1_", 4, 24, ACE_TH1r);
	DebugDisp(gbContDebugOn, , "ACE_TH2_", 5, 24, ACE_TH2r);

	void AceGmgnSet(const BYTE);	AceGmgnSet(bAceGmgnMin);

	// Defog -------------------------------------------------------------------------------
	//if (UP(ShdDet))		return;							// because AE Window

	// Defog table
	UINT	DefogTbl[3];	// Low, Middle, High
	//Defog Table UserParameter
	DefogTbl[0] = UP(DEFOG_TH_LOW);	DefogTbl[1] = MAX(DefogTbl[0],(UINT)UP(DEFOG_TH_MID));	DefogTbl[2] = MAX(DefogTbl[1],(UINT)UP(DEFOG_TH_HIGH));

  	UINT 	nDefogThres = 0;
//	UINT 	nDefogWeight = 3;		//Defog off  (Weight 0 : Defog 0%, 1 : Defog 25%, 2 : Defog 50%, 3 : Defog 75%, else Defog 100% coverage)
	UINT 	nDefogWeight = 0xC0;	//Defog off  (Weight 0 : Defog 0%, 0x40 : Defog 25%, 0x80 : Defog 50%, 0xC0 : Defog 75%, else Defog 100% coverage)

//	UINT 	nHS = 0;
//	UINT 	nHE = 0;
//	UINT 	nVS = 0;
//	UINT 	nVE = 0;

	gbContrastMode 	= UP(Defog);							// menu parameter   OFF/ON
	gbDefogMode 	= UP(DefogMode);						// "				MANUAL/AUTO
	gbDefogLevel 	= UP(DefogLevel);						// "				STRENGTH LOW/MID/HIGH
	//gbDefogLevel 	= 1/*(AE_SAT_OFF)*/ ? UP(DefogLevel) : UP_3sHI ;	// "				STRENGTH LOW/MID/HIGH

	if (gbContrastMode==UP_ON) {	// Defog on

		if(gbDefogMode == UP_DEFOG_AUTO) {
#if 0
	#if 0
			UINT 	nV_OFF;

//			if (UP(Flip)==UP_ON) 	nV_OFF = VWIr - ((gnDefog_VE<<1)/3) - 24;	// Flip condition
//			else 					nV_OFF = 0;
 			nV_OFF = 0;

			// Must be Same 'gnAe1Size' !!!
//			nHS = gnDefog_HS + DEFOG_X_OFF;
//	 		nHE = gnDefog_HE - DEFOG_X_OFF;
//			nVS = gnDefog_VS + nV_OFF;
//			nVE = (gnDefog_VE<<1)/3 + nV_OFF;

 			#if (model_1M)
					gnDefog_HS = UP(AE2_HSP) + DEFOG_X_OFF;
					gnDefog_HE = UP(AE2_HEP) - DEFOG_X_OFF;
					gnDefog_VS = UP(AE2_VSP);
					gnDefog_VE = UP(AE2_VEP);
					HEQ_HWw(UP(DEFOG_720_HW));	// Defog window setup
					HEQ_VWw(UP(DEFOG_720_VW));	// "
					HEQ_HSPw(UP(DEFOG_720_HSP));	// "
					HEQ_VSPw(UP(DEFOG_720_VSP));  // "
			#else
				if ((UP(OutFps)==UP_2M_720p_25_30)||(UP(OutFps)==UP_2M_720p_50_60)) { 		// Crop mode
					gnDefog_HS = UP(AE2_HSP) + DEFOG_X_OFF + 0x1e;
					gnDefog_HE = UP(AE2_HEP) - DEFOG_X_OFF - 0x15a; //
					gnDefog_VS = UP(AE2_VSP);
					gnDefog_VE = UP(AE2_VEP) - 0x164; 				// 2d0
					HEQ_HWw(UP(DEFOG_720_HW));	// Defog window setup
					HEQ_VWw(UP(DEFOG_720_VW));	// "
					HEQ_HSPw(UP(DEFOG_720_HSP));	// "
					HEQ_VSPw(UP(DEFOG_720_VSP));  // "
				}
				else { 											// 2M
					gnDefog_HS = UP(AE2_HSP) + DEFOG_X_OFF;
					gnDefog_HE = UP(AE2_HEP) - DEFOG_X_OFF;
					gnDefog_VS = UP(AE2_VSP);
					gnDefog_VE = UP(AE2_VEP);
					HEQ_HWw(UP(DEFOG_1080_HW));	// Defog window setup
					HEQ_VWw(UP(DEFOG_1080_VW));	// "
					HEQ_HSPw(UP(DEFOG_1080_HSP));	// "
					HEQ_VSPw(UP(DEFOG_1080_VSP)); // "
				}
			#endif
	#else
			gnDefog_HS = RP(AE_HSP)/*UP(AE2_HSP)*/ + DEFOG_X_OFF;
			gnDefog_HE = RP(AE_HEP)/*UP(AE2_HEP)*/ - DEFOG_X_OFF;
			gnDefog_VS = RP(AE_VSP)/*UP(AE2_VSP)*/;
			gnDefog_VE = RP(AE_VEP)/*UP(AE2_VEP)*/;
	#endif

			UINT 	nHW = gnDefog_HE - gnDefog_HS + 1;
			UINT 	nVW = ((gnDefog_VE<<1)/3) - gnDefog_VS + 1;

			gnAe1Size = nHW*nVW;

			AE1_HSPw(gnDefog_HS);								// AE1 area H (Histogram window)
			AE1_HEPw(gnDefog_HE);								// "

			AE1_VSPw(gnDefog_VS);								// AE1 area V (Histogram window)
			AE1_VEPw((gnDefog_VE<<1)/3 /*+ nV_OFF*/);				//
#endif
			AE1_HSPw(gnDefog_HS);								// AE1 area H (Histogram window)
			AE1_HEPw(gnDefog_HE);								// "

			AE1_VSPw(gnDefog_VS);								// AE1 area V (Histogram window)
			AE1_VEPw(gnDefog_VE);								//

			//															0  1  2  3     4     5     6     7     8  9
			// HISTO10_MAX : 10203040 6080a0c0 0000e0ff				-> 10 20 30 40    60    80    a0    c0    e0 ff 100
			// HISTO15_MAX : 08101820 28303840 48505860 68707f00	->  8 10 18 20 28 30 38 40 48 50 58 60 68 70 7f  80
			//															0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
			gnHistoV[0] = HISTO0_LOCKr;
			gnHistoV[1] = HISTO1_LOCKr;
			gnHistoV[2] = HISTO2_LOCKr;
			gnHistoV[3] = HISTO3_LOCKr;
			gnHistoV[4] = HISTO4_LOCKr + HISTO5_LOCKr;
			gnHistoV[5] = HISTO6_LOCKr + HISTO7_LOCKr;
			gnHistoV[6] = HISTO8_LOCKr + HISTO9_LOCKr;
			gnHistoV[7] = HISTO10_LOCKr + HISTO11_LOCKr;
			gnHistoV[8] = HISTO12_LOCKr + HISTO13_LOCKr;;
			gnHistoV[9] = HISTO14_LOCKr;
		}

		LibDefog(&nDefogThres, &nDefogWeight, DefogTbl, /*(AE_CTRL_ORIGINAL) ? (gnAeState==3) :*/ (gbAeStg==AE_STG_DSS)); 				// Defog library
	}
//	else nDefogWeight = 1;	// 2015921 - WHL : 25% at EN771								// Defog off
	else nDefogWeight = 64;	// 2015921 - WHL : 25% at EN673								// Defog off

	if (gbWdrOn!=WDR_OFF && gbContrastMode!=UP_ON) nDefogWeight = 1;	// 180426 KSH

	DebugDisp(gbContDebugOn, , "DFG_THRS", 7, 24, nDefogThres);

	nDefogThres = LibUtlInterp1D(iAceAgcPos, ADNR_AGC_MIN, ADNR_AGC_MID, nDefogThres, 0x0);	// Decrease by AGC
	nDefogWeight = LibUtlInterp1D(iAceAgcPos, ADNR_AGC_MIN, ADNR_AGC_MID, nDefogWeight, 0x0);	// "

	HEQ_THw(nDefogThres);								// apply to HEQ
	HEQ_WGw(nDefogWeight);									// Defog Intensity


	// Mon
	DebugDisp(gbContDebugOn, , "DFG_THRS", 8, 24, HEQ_THr 	);
	DebugDisp(gbContDebugOn, , "DFG_WG__", 9, 24, HEQ_WGr	);
}




/* EOF */
