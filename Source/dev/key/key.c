/* **************************************************************************
 File Name	: 	isp_key.c
 Description:	EN675 - key scan module
 Designer	:	Kim, Sunghoon
 Date		:	19. 6. 7
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"


//*************************************************************************************************
// Define & Variable
//-------------------------------------------------------------------------------------------------
// Key filtering (Chattering, Repeated key)
BYTE	gbUKeyVal = 0;				// Key UART(Input)
TKey	gtUKey = {0};				// Key UART(Chattering or Holding)

BYTE	gbPKeyVal = 0;				// Key UART(Input) - Pelco
TKey	gtPKey = {0};				// Key UART(Chattering or Holding)

BYTE	gbSKeyVal = 0;				// Key UART(Input)
TKey	gtSKey = {0};				// Key UART(Chattering or Holding)

BYTE	gbMKey = 0;					// Key MENU(Repeated)
BYTE	gbPKey = 0;					// Key MENU(Pushing)

UINT	gbPTZKeyVal = 0;			// Key for Onvif PTZ(Input)
TKey	gtPTZKey = {0};				// Key for Onvif PTZ(Chattering or Holding)


//*************************************************************************************************
// Key
//-------------------------------------------------------------------------------------------------
// Chattering
void KeyCht(const BYTE abIn, TKey* aptKey, const BYTE abCHT_BIT)
{
	aptKey->v.bSft = (aptKey->v.bSft<<1) | (abIn!=0);
	aptKey->v.bOn = ((aptKey->v.bSft & abCHT_BIT) == abCHT_BIT) ? abIn : 0 ;
}

void KeyHld(const BYTE abIn, TKey* aptKey, const BYTE abHoldTime)
{
	if(abIn) {
		aptKey->v.bSft = abHoldTime;
		aptKey->v.bOn = abIn;
	}
	
	if(aptKey->v.bSft) aptKey->v.bSft--;
	else aptKey->v.bOn = 0;
}

void KeyPush(const BYTE abIn, TKey* aptKey, BYTE* apbMenuPushState, BYTE* apRepeatKEY, BYTE* apPushKEY)
{
	aptKey->v.bRpt = abIn;
	if(aptKey->v.bRpt) *apRepeatKEY = aptKey->v.bRpt;

	if(*apbMenuPushState == 1) {
		if(aptKey->b.Cr) aptKey->b.C = (!aptKey->b.C);
		else if(*apRepeatKEY || *apPushKEY) {
			if(aptKey->b.C)	*apbMenuPushState = 2;
			else			*apbMenuPushState = 0;
			aptKey->v.bOn = 0;
		}
	}
	else {
		aptKey->v.bOn = 0;
	}

	if(aptKey->v.bOn) *apPushKEY = aptKey->v.bOn;
}

//-------------------------------------------------------------------------------------------------
// Repeat key
void KeyRpt(TKey* aptKey, const BYTE abRptStart, const BYTE abRptTime)
{
	if(aptKey->v.bOn)
	{
		if(aptKey->v.bCnt == 0) {								// First
			aptKey->v.bCnt++;
			aptKey->v.bRpt = aptKey->v.bOn;
		}
		else if(aptKey->v.bCnt == abRptStart) {					// Second
			aptKey->v.bCnt++;
			aptKey->v.bRpt = aptKey->v.bOn;
		}
		else if(aptKey->v.bCnt >= (abRptStart + abRptTime)) {	// Every
			aptKey->v.bCnt = abRptStart;
			aptKey->v.bRpt = 0;
		}
		else{
			aptKey->v.bCnt++;
			aptKey->v.bRpt = 0;
		}
	}
	else {
		aptKey->v.bCnt = 0;
		aptKey->v.bRpt = 0;
	}
}

void KeyScan(void)
{
#if (model_Key==1)
	const UINT nADC_KEY = ADC_KEYr;
	DebugDisp(((gbMnDebugFnc==5)), , "ADC_Key_", 2, 24, nADC_KEY)

	DebugDisp(((gbMnDebugFnc==5)), , "ADC0____", 5, 24, ADC0_RDATr)
	DebugDisp(((gbMnDebugFnc==5)), , "ADC1____", 6, 24, ADC1_RDATr)
	DebugDisp(((gbMnDebugFnc==5)), , "ADC2____", 7, 24, ADC2_RDATr)
	
	DebugDisp(((gbMnDebugFnc==5)), , "gbMKey__", 10, 24, gbMKey)
	DebugDisp(((gbMnDebugFnc==5)), , "gbPKey__", 11, 24, gbPKey)
	DebugDisp(((gbMnDebugFnc==5)), , "SHARED0_", 12, 24, CPU_SHARED0)
	DebugDisp(((gbMnDebugFnc==5)), , "PTPKey_V", 13, 24, gbPTZKeyVal)
	DebugDisp(((gbMnDebugFnc==5)), , "pKEY    ", 14, 24, pKEY)

	gbSKeyVal =	(nADC_KEY<(((UINT)UP(ADC_LEVEL_C)+(UINT)UP(ADC_LEVEL_U))>>1)) ? KEY_VAL_C :		// Center
				(nADC_KEY<(((UINT)UP(ADC_LEVEL_U)+(UINT)UP(ADC_LEVEL_D))>>1)) ? KEY_VAL_U :		// Up
				(nADC_KEY<(((UINT)UP(ADC_LEVEL_D)+(UINT)UP(ADC_LEVEL_L))>>1)) ? KEY_VAL_D :		// Down
				(nADC_KEY<(((UINT)UP(ADC_LEVEL_L)+(UINT)UP(ADC_LEVEL_R))>>1)) ? KEY_VAL_L : 	// Left
//					(nADC_KEY<(((UINT)UP(ADC_LEVEL_R)+             0x000001ff)>>1)) ? KEY_VAL_R : 0;	// Right
				(nADC_KEY<(((UINT)UP(ADC_LEVEL_R)+             0x000003ff)>>1)) ? KEY_VAL_R : 0;	// Right

	static BYTE bSKeyVal1d = 0;
	if(bSKeyVal1d != gbSKeyVal) {
		bSKeyVal1d = gbSKeyVal;
		gbSKeyVal = 0;
	}

	KeyCht(gbSKeyVal, &gtSKey, (1<<UP(AD_CHT_BIT))-1);
	KeyRpt(&gtSKey, AD_KEY_RPTF, AD_KEY_RPTT);
#endif

	//----------------------------------------------------------------------------------------
	// UART Key - ECM(PC)
	KeyHld(gbUKeyVal, &gtUKey, UART_HOLD_CNT);
//		KeyRpt(&gtUKey, UART_KEY_RPTF, UART_KEY_RPTT);
	KeyRpt(&gtUKey, UART_KEY_RPTF, UART_KEY_RPTT2);

#if 0
	//----------------------------------------------------------------------------------------
	// UART Key - Pelco
	KeyHld(gbPKeyVal, &gtPKey, /*1*/PELCO_HOLD_CNT);
	KeyRpt(&gtPKey, PELCO_KEY_RPTF, PELCO_KEY_RPTT);

	//----------------------------------------------------------------------------------------
	// PTZ Key - Onvif
	KeyHld(gbPTZKeyVal, &gtPTZKey, /*1*/PTZ_HOLD_CNT);
	KeyRpt(&gtPTZKey, PTZ_KEY_RPTF, PTZ_KEY_RPTT);
#endif

	//----------------------------------------------------------------------------------------
	// Key MUX
	KEY =	(gtUKey.v.bRpt) ? gtUKey.v.bRpt : (gtPKey.v.bRpt) ? gtPKey.v.bRpt : gtSKey.v.bRpt;

//	pKEY =	(gtUKey.v.bOn) ? gtUKey.v.bOn : (gtPKey.v.bOn) ? gtPKey.v.bOn : gtSKey.v.bOn;
//	pKEY |= gbPTZKeyVal;
	pKEY =	(gtUKey.v.bOn) ? gtUKey.v.bOn : (gtPKey.v.bOn) ? gtPKey.v.bOn : (gtPTZKey.v.bOn) ? gtPTZKey.v.bOn : gtSKey.v.bOn;
							
	//----------------------------------------------------------------------------------------
	// Debug

	
#if (SDI_UCC_MODE==1) // for UCC Key Control

//	UINT nRxBuff=0;
	static BYTE bInitFlag=1, bTranComplete=0;

	if(bInitFlag) {
		EN331_Write(0x306, 0x80000000); // UCC Dummy Transmit for init time
		bInitFlag=0;
	}

	if(KEY) {
		if     (KEY_U) { EN331_Write(0x3e1, 0x080001ff); EN331_Write(0x3e2, 0x090000); printf("\r\n UCC Up");     }
		else if(KEY_D) { EN331_Write(0x3e1, 0x100001ff); EN331_Write(0x3e2, 0x110000); printf("\r\n UCC Down");   }
		else if(KEY_L) { EN331_Write(0x3e1, 0x040001ff); EN331_Write(0x3e2, 0x050000); printf("\r\n UCC Left");   }
		else if(KEY_R) { EN331_Write(0x3e1, 0x020001ff); EN331_Write(0x3e2, 0x030000); printf("\r\n UCC Right");  }
//		else if(KEY_C) { EN331_Write(0x3e1, 0x1e0001ff); EN331_Write(0x3e2, 0x1f0000); printf("\r\n UCC Center"); }
		else if(KEY_C) { EN331_Write(0x3e1, 0x030001ff); EN331_Write(0x3e2, 0x040000); printf("\r\n UCC Center"); } // Old

		bTranComplete=1;

		#if 0 // UCC Ready Check
		EN331_Read(0x306, &nRxBuff);
		if(!nRxBuff) printf("\r\n UCC Ready [0x%08x]", nRxBuff);
		else         printf("\r\n UCC Not Ready [0x%08x]", nRxBuff);
		#endif

//		EN331_Write(0x306, 0x80000000); // UCC Transmit
		if(!KEY_E) EN331_Write(0x306, 0x80000000); // UCC Transmit (Exception Exit Key)
	}
	else {
		if(bTranComplete) {
			UINT nTransBuf=0;
			
			EN331_Read(0x306,&nTransBuf);			
			EN331_Write(0x306, nTransBuf&0x7FFFFFFF); // UCC Transmit Complete
			bTranComplete=0;
		}
	}
#endif

#if 0
	if(gbMnDebugFnc==5) {
		GRP0 = ADC0_RDATr;
		GRP1 = ADC1_RDATr;
		GRP2 = ADC2_RDATr;
		UartTxGrp();
	}
#endif

#if (model_Lens==1)
	if(gbUKeyVal) 		gbMAF_KeyBuf = gbUKeyVal;
	else if(gbSKeyVal)	gbMAF_KeyBuf = gbSKeyVal;
	else if(gbPTZKeyVal)	gbMAF_KeyBuf = gbPTZKeyVal;
#endif

	gbUKeyVal = 0;
	gbPKeyVal = 0;
	gbPTZKeyVal = 0;
}

#if 1//model_WDR_ROI
void KeyAccLevel(int *aiKeyAdd)
{
	#define FR			IF_FUNC_FPS/*30*/		// Frame pre sec

	//KEY Accumulator Level
	#define KEY_ACC_LV0		1
	#define KEY_ACC_LV1		2//9
	#define KEY_ACC_LV2		3//14
	#define KEY_ACC_LV3		4//18
	#define KEY_ACC_LV4		5//22
	#define KEY_ACC_LV5		6//25
	#define KEY_ACC_LV6		7//28
	#define KEY_ACC_LV7		8//30
	
	//KEY Accumulator Level Count
	#define KEY_ACC_LV0_CNT	(FR/2)			// 30
	#define KEY_ACC_LV1_CNT	(FR/3)			// 15
	#define KEY_ACC_LV2_CNT	(FR/4)			// 10
	#define KEY_ACC_LV3_CNT	(FR/5)			// 7
	#define KEY_ACC_LV4_CNT	(FR/6)			// 6
	#define KEY_ACC_LV5_CNT	(FR/7)			// 5
	#define KEY_ACC_LV6_CNT	(FR/8)			// 4
	#define KEY_ACC_LV7_CNT	(FR/11)			// 3
	
	//#define KEY_NONE_CNT	2

	static int iKeyAccLv = 0;
	static int iKeyAccLvCnt = 0;
	//static int iKeyAccLvDn = 0;
	static BYTE pKEY1d = 0;
	
	if(pKEY == 0 /*|| KEY != pKEY*/ || ((pKEY1d!=pKEY)/*&&pKEY1d*/)) {
		//iKeyAccLvDn++;
		//if(iKeyAccLvDn == KEY_NONE_CNT || ((pKEY1d!=pKEY)&&pKEY1d))
		{
			//iKeyAccLvDn = 0;
			iKeyAccLvCnt = 0;
			iKeyAccLv = 0;
		}
	}
	pKEY1d = pKEY;

	if(pKEY) {
		//iKeyAccLvDn = 0;
		iKeyAccLvCnt++;

		switch(iKeyAccLv) {
			case 0 : 
				if(iKeyAccLvCnt == KEY_ACC_LV0_CNT) { iKeyAccLvCnt = 0;	iKeyAccLv++; }		
				*aiKeyAdd = LibUtlInterp1D(iKeyAccLvCnt,0,KEY_ACC_LV0_CNT,KEY_ACC_LV0,KEY_ACC_LV1); break;
			case 1 : 
				if(iKeyAccLvCnt == KEY_ACC_LV1_CNT) { iKeyAccLvCnt = 0;	iKeyAccLv++; }		
				*aiKeyAdd = LibUtlInterp1D(iKeyAccLvCnt,0,KEY_ACC_LV1_CNT,KEY_ACC_LV1,KEY_ACC_LV2); break;
			case 2 : 
				if(iKeyAccLvCnt == KEY_ACC_LV2_CNT) { iKeyAccLvCnt = 0;	iKeyAccLv++; }		
				*aiKeyAdd = LibUtlInterp1D(iKeyAccLvCnt,0,KEY_ACC_LV2_CNT,KEY_ACC_LV2,KEY_ACC_LV3); break;
			case 3 : 
				if(iKeyAccLvCnt == KEY_ACC_LV3_CNT) { iKeyAccLvCnt = 0;	iKeyAccLv++; }		
				*aiKeyAdd = LibUtlInterp1D(iKeyAccLvCnt,0,KEY_ACC_LV3_CNT,KEY_ACC_LV3,KEY_ACC_LV4); break;
			case 4 : 
				if(iKeyAccLvCnt == KEY_ACC_LV4_CNT) { iKeyAccLvCnt = 0;	iKeyAccLv++; }		
				*aiKeyAdd = LibUtlInterp1D(iKeyAccLvCnt,0,KEY_ACC_LV4_CNT,KEY_ACC_LV4,KEY_ACC_LV5); break;
			case 5 : 
				if(iKeyAccLvCnt == KEY_ACC_LV5_CNT) { iKeyAccLvCnt = 0;	iKeyAccLv++; }		
				*aiKeyAdd = LibUtlInterp1D(iKeyAccLvCnt,0,KEY_ACC_LV5_CNT,KEY_ACC_LV5,KEY_ACC_LV6); break;
			case 6 : 
				if(iKeyAccLvCnt == KEY_ACC_LV6_CNT) { iKeyAccLvCnt = 0;	iKeyAccLv++; }		
				*aiKeyAdd = LibUtlInterp1D(iKeyAccLvCnt,0,KEY_ACC_LV6_CNT,KEY_ACC_LV6,KEY_ACC_LV7); break;
			default: 
				if(iKeyAccLvCnt == KEY_ACC_LV7_CNT) { iKeyAccLvCnt = KEY_ACC_LV7_CNT - 1; }	
				*aiKeyAdd = KEY_ACC_LV7; break;
		}
	}
	
}
#endif

/* EOF */

