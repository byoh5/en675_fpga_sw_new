#ifndef __ISP_LIB_MAIN_H__
#define __ISP_LIB_MAIN_H__


#define __ISP_LIB__

#include "cfg_memory.h"				// EN675 - Memory

#include "dev_types.h"				// EN675
#include "dev_reg_ex.h"				// EN675 - dev_reg.h

#include "isp_reg_ex.h"				// EN675 - ISP - isp_reg.h
#include "isp_app.h"				// EN675 - ISP
#include "isp_ae.h"					// EN675 - ISP
#include "isp_af.h"					// EN675 - ISP
#include "isp_imd.h"				// EN675 - ISP
#include "isp_font.h"				// EN675 - ISP
#include "isp_menu.h"				// EN675 - ISP

#include "bits.h"					// CPU

#include "vsnprintf.h"				// dev/string

#include "exbl.h"					// WaitXus, WaitXms

#include "uart_string.h"			// dev/string - UART IRQ
#include "key.h"					// dev/key - Key control (ADC,UART,UTC,etc.)


extern BYTE gbSensCtrl;
extern BYTE gbSensIntf;
extern BYTE gbSensFps;



#undef __ISP_LIB__

#endif // __ISP_LIB_MAIN_H__