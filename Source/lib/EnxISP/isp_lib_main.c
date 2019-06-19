/* **************************************************************************
 File Name	:	isp_lib_main.c
 Description:	EN675 - ISP Library
 Designer	:	Kim, Sunghoon
 Date		:	19. 6. 11
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#define __ISP_LIB__
#include "dev_types.h"				// EN675
#include "dev_reg_ex.h"				// EN675 - dev_reg.h

#include "isp_reg.h"				// EN675 - ISP
#include "isp_functions.h"			// EN675 - ISP

#include "key.h"					// dev/key - Key control (ADC,UART,UTC,etc.)

//#include "isp_lib_main.h"

//-------------------------------------------------------------------------------------------------
int ISRT0 LibUtlInterp1D(int aiXCur, int aiXPos1, int aiXPos2, int aiYPos1, int aiYPos2)
{	// interpolation val

	int	iResult=0;

	if (aiXPos1==aiXPos2)			return 0;										// Divide by 0 " error "
	else if (aiYPos1==aiYPos2)		return aiYPos1;									// Position equal
	else if (aiYPos1<aiYPos2) {														// Increase wgt

		iResult = (((aiYPos2-aiYPos1) * (aiXCur-aiXPos1)) / (aiXPos2-aiXPos1)) + aiYPos1;

	}
	else if (aiYPos1>aiYPos2) {													// Decrease wgt

		iResult = (((aiYPos1-aiYPos2) * (aiXPos2 - aiXCur)) / (aiXPos2-aiXPos1)) + aiYPos2;
	}

	iResult = (iResult<0) ? 0 : iResult;

	return iResult;
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
