/* **************************************************************************
 File Name	:	isp_lib_main.c
 Description:	EN675 - ISP Library
 Designer	:	Kim, Sunghoon
 Date		:	19. 6. 11
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#define UP_LIB_LINK
#include "isp_lib_main.h"

BYTE gbSensCtrl = 0;
BYTE gbSensIntf = 0;
BYTE gbSensFps = 0;

//-------------------------------------------------------------------------------------------------
int ISRT0 LibUtlInterp1D(int aiXCur, int aiXPos1, int aiXPos2, int aiYPos1, int aiYPos2)
{	// interpolation val
	int	iResult=0;

	if (aiXPos1==aiXPos2)			return 0;										// Divide by 0 " error "
	else if (aiYPos1==aiYPos2)		return aiYPos1;									// Position equal
	else if (aiYPos1<aiYPos2) {														// Increase wgt
		iResult = (((aiYPos2-aiYPos1) * (aiXCur-aiXPos1)) / (aiXPos2-aiXPos1)) + aiYPos1;
	}
	else if (aiYPos1>aiYPos2) {														// Decrease wgt, 1.9 => 1, -1.9 => -2가 아닌 -1 이므로 필요
		iResult = (((aiYPos1-aiYPos2) * (aiXPos2 - aiXCur)) / (aiXPos2-aiXPos1)) + aiYPos2;
	}

	return (iResult<0) ? 0 : iResult;
}

int ISRT0 LibUtlInterp1D_CLAMP(int aiXCur, int aiXPos1, int aiXPos2, int aiYPos1, int aiYPos2)
{	// interpolation val
	int	iResult=0;

	if(aiXPos1 <= aiXPos2) {
		if(aiXCur<aiXPos1) aiXCur = aiXPos1;
		if(aiXCur>aiXPos2) aiXCur = aiXPos2;
	}
	else {
		if(aiXCur>aiXPos1) aiXCur = aiXPos1;
		if(aiXCur<aiXPos2) aiXCur = aiXPos2;
	}

	if (aiXPos1==aiXPos2)			return 0;										// Divide by 0 " error "
	else if (aiYPos1==aiYPos2)		return aiYPos1;									// Position equal
	else if (aiYPos1<aiYPos2) {														// Increase wgt
		iResult = (((aiYPos2-aiYPos1) * (aiXCur-aiXPos1)) / (aiXPos2-aiXPos1)) + aiYPos1;
	}
	else if (aiYPos1>aiYPos2) {														// Decrease wgt, 1.9 => 1, -1.9 => -2가 아닌 -1 이므로 필요
		iResult = (((aiYPos1-aiYPos2) * (aiXPos2 - aiXCur)) / (aiXPos2-aiXPos1)) + aiYPos2;
	}

	return (iResult<0) ? 0 : iResult;
}

//-------------------------------------------------------------------------------------------------
BYTE ISRT0 LibUtlKeyPass(const BYTE* abpPassKeys, BYTE abKeySize, BYTE* abpPassCnt)
{	// Key pass function
	if(KEY == KEY_VAL_S) return	0;

	if(*abpPassCnt < abKeySize) {
		if (KEY == abpPassKeys[*abpPassCnt]) 	(*abpPassCnt) ++;
		else if (KEY)	 						(*abpPassCnt) = 0;
	}
	else {
		*abpPassCnt = 0;
	}

	if(*abpPassCnt == abKeySize) {		// OK pass code
		return	1;
	}
	else return	0;

//		// original
//		switch (gbMnPassCnt2) {															// Debug MENU start
//			case 0 : if (KEY_U) gbMnPassCnt2 ++; else if (KEY) gbMnPassCnt2 = 0; break;
//			case 1 : if (KEY_D) gbMnPassCnt2 ++; else if (KEY) gbMnPassCnt2 = 0; break;
//			case 2 : if (KEY_L) gbMnPassCnt2 ++; else if (KEY) gbMnPassCnt2 = 0; break;
//			case 3 : if (KEY_R) gbMnPassCnt2 ++; else if (KEY) gbMnPassCnt2 = 0; break;
//			case 4 : if (KEY_C) {gbMnPassCnt2 ++;
//								MENU_OPEN(DEBUG);
//								}				 else if (KEY) gbMnPassCnt2 = 0; break;
//			default : 	gbMnPassCnt2 = 0; break;
//		}
}

//-------------------------------------------------------------------------------------------------
#if 0
void LibUpTest(void)
{
	#define DISP_TIME_Y		10
	#define DISP_TIME_X		0

	UP(Brightness)++;
	PAR1F++;

	FontStrEx(DISP_TIME_Y, DISP_TIME_X, NO_ALPHA, MN_GREEN, "UpBrt", 5);
	FontHex(DISP_TIME_Y, DISP_TIME_X+6, UP(Brightness), sizeof(UP(Brightness))<<1);
	FontStrEx(DISP_TIME_Y+1, DISP_TIME_X, NO_ALPHA, MN_GREEN, "UpCBa", 5);
	FontHex(DISP_TIME_Y+1, DISP_TIME_X+6, UP(ColorBar), sizeof(UP(ColorBar))<<1);
	FontStrEx(DISP_TIME_Y+2, DISP_TIME_X, NO_ALPHA, MN_GREEN, "UpP1F", 5);
	FontHex(DISP_TIME_Y+2, DISP_TIME_X+6, PAR1F, sizeof(PAR1F)<<1);
}
#endif

void SensorSetting(BYTE abSensCtrl, BYTE abSensIntf, BYTE abSensFps)
{
	gbSensCtrl = abSensCtrl;
	gbSensIntf = abSensIntf;
	gbSensFps = abSensFps;
}

void printf_SensorSetting(void)
{
	_printf("Sensor Ctrl : %s\r\n", (gbSensCtrl==1) ? "TWI" : "SPI");
	_printf("Sensor Intf : %s\r\n", (gbSensIntf==2) ? "MIPI" : (gbSensIntf==1) ? "LVDS" : "Parallel");
	_printf("Sensor Fps  : %d\r\n", gbSensFps);
}


