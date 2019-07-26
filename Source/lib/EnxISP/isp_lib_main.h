#ifndef __ISP_LIB_MAIN_H__
#define __ISP_LIB_MAIN_H__


#define __ISP_LIB__
#include "dev_types.h"				// EN675
#include "dev_reg_ex.h"				// EN675 - dev_reg.h

#include "isp_reg.h"				// EN675 - ISP
#include "isp_functions.h"			// EN675 - ISP
#include "isp_font.h"				// EN675 - ISP

#include "vsnprintf.h"				// dev/string

#include "exbl.h"					// WaitXus, WaitXms

#include "key.h"					// dev/key - Key control (ADC,UART,UTC,etc.)


extern BYTE gbSensCtrl;
extern BYTE gbSensIntf;
extern BYTE gbSensFps;



#undef __ISP_LIB__

#endif // __ISP_LIB_MAIN_H__