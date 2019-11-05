#ifndef __DEV_H__
#define __DEV_H__

#include "dev_model.h"				// EN675
#include "dev_peri.h"				// EN675
#include "dev_device.h"				// EN675

//******************************************************************************
// 1. System define
//------------------------------------------------------------------------------
#define BDMA_CNT				4
#define CDMA_CNT				4
#define GPIO_CNT				72
#define SDIO_CNT				2
#define UART_CNT				9
#define SPI_CNT					9
#define I2C_CNT					9
#define TIMER_CNT				39
#define USB_CNT					1
#define ETHERNET_CNT			1
#define I2S_CNT					1
#define ADC_CNT					8


// uart printf out /////////////////////////////////////////////////////////////
#define DEBUG_UART_NUM 			7
#define INVALID_UART_NUM		0xFF


// Tick irq ////////////////////////////////////////////////////////////////////
#define TIME_TICK			100							// 10ms
#define TIME_1MS			((MCK_FREQ / 100) / 1000)
#define TIMECMP_NEXT_VAL	((MCK_FREQ / 100) / TIME_TICK)


////////////////////////////////////////////////////////////////////////////////
#include "isp_clk.h"				// EN675 - ISP define
#include "isp_digital_interface.h"	// EN675 - ISP define
#include "isp_interface.h"			// EN675 - ISP define

#include "dev_types.h"				// EN675
#include "dev_reg_ex.h"				// EN675 - dev_reg.h
#include "dev_debug.h"				// EN675
#include "dev_verify.h"				// EN675

#include "isp_reg.h"				// EN675 - ISP
#include "isp_functions.h"			// EN675 - ISP
#include "isp_font.h"				// EN675 - ISP
#include "hdmi.h"					// EN675 - ISP

#include "cfg_define.h"				// EN675 - Define
#include "cfg_memory.h"				// EN675 - Memory
#include "cfg_network.h"			// EN675 - Network
#include "cfg_filesystem.h"			// EN675 - File system
#include "cfg_media.h"				// EN675 - Media(Video/Audio)

#include "asm.h"					// CPU
#include "bits.h"					// CPU
#include "encoding.h"				// CPU
#include "mtrap.h"					// CPU
#include "rv_utils.h" 				// CPU
#include "pmp.h"					// CPU

#include <stdio.h>					// printf
#include <string.h>					// memset, memcpy
#include <time.h>					// time_t
#include "enx_string.h"				// dev/string
#include "enx_time.h"				//
#include "vsnprintf.h"				// dev/string
#include "boot.h"					// core/shared
#include "msg.h"					// core/shared
#include "user.h"					// user info save/load

#include "exbl.h"					// WaitXus, WaitXms

#include "audio.h"
#include "rtc.h"

#include "uart_string.h"			// dev/string - UART IRQ
#include "eeprom.h"					// dev/eeprom - EEPROM control
#include "key.h"					// dev/key - Key control (ADC,UART,UTC,etc.)

#include "dev_functions.h"			// EN675


#endif // __DEV_H__
