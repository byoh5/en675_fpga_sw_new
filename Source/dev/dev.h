#ifndef __DEV_H__
#define __DEV_H__

#include "dev_model.h"				// EN675
#include "dev_peri.h"				// EN675
#include "dev_device.h"				// EN675

//******************************************************************************
// 1. Memory Size & Base Address Definition
//------------------------------------------------------------------------------
// Mem size
#if defined(__SFLS_8MB__)
#define SFLS_SIZE				(  8*1024*1024)	//	 8MB
#elif defined(__SFLS_16MB__)
#define SFLS_SIZE				( 16*1024*1024)	//	 16MB
#elif defined(__SFLS_32MB__)
#define SFLS_SIZE				( 32*1024*1024)	//	 32MB
#else
#error "Sfls size error."
#endif
#define DDR0_SIZE				( 64*1024*1024)	//	 64MB	-	AP Memory AD251232 , Winbond W979H2KK , Etron EM6KA32HGDA
#define DDR1_SIZE				( 64*1024*1024)	//	 64MB	-	AP Memory AD251232 , Winbond W979H2KK , Etron EM6KA32HGDA
//#define DDR_SIZE				(128*1024*1024)	//	128MB	-	AP Memory 1Gb , Fidelix 1Gb, Winbond W97AH2KK
//#define DDR_SIZE				(256*1024*1024)	//	256MB	-	AP Memory AD220032
//#define SRAM_SIZE				(     256*1024)	//	256kB   -
#define SRAM_SIZE				(  1*1024*1024)	//	  1MB   -   TEST

// Base address
#define SFLS_BASE				0xC0000000
#define DDR0_BASE				0x80000000
#define DDR1_BASE				0x90000000
#define SRAM_BASE				0xA0000000

#define DDR_BASE_ISP 			(DDR1_BASE>>4)

// RegBase address
#define REG_BASE_ISP			0x40000000
#define REG_BASE_H265			0x40100000
#define REG_BASE_H264       	0x40200000
#define REG_BASE_DDR0        	0x40300000
#define REG_BASE_DDR1       	0x40400000
#define REG_BASE_SFLS       	0x40500000
#define REG_BASE_SDIO0      	0x40600000
#define REG_BASE_SDIO1      	0x40700000
#define REG_BASE_ETH        	0x40800000
#define REG_BASE_AUD        	0x40900000
#define REG_BASE_UTIL       	0x40A00000
#define REG_BASE_GPIO       	0x40B00000
#define REG_BASE_SPI0       	0x40C00000
#define REG_BASE_SPI1       	0x40D00000
#define REG_BASE_SPI2       	0x40E00000
#define REG_BASE_SPI3       	0x40F00000
#define REG_BASE_SPI4       	0x41000000
#define REG_BASE_SPI5       	0x41100000
#define REG_BASE_SPI6       	0x41200000
#define REG_BASE_SPI7       	0x41300000
#define REG_BASE_SPI8       	0x41400000
#define REG_BASE_I2C0       	0x41500000
#define REG_BASE_I2C1       	0x41600000
#define REG_BASE_I2C2       	0x41700000
#define REG_BASE_I2C3       	0x41800000
#define REG_BASE_I2C4       	0x41900000
#define REG_BASE_I2C5       	0x41A00000
#define REG_BASE_I2C6       	0x41B00000
#define REG_BASE_I2C7       	0x41C00000
#define REG_BASE_I2C8       	0x41D00000
#define REG_BASE_TIMER0     	0x41E00000
#define REG_BASE_TIMER1     	0x41F00000
#define REG_BASE_TIMER2     	0x42000000
#define REG_BASE_TIMER3     	0x42100000
#define REG_BASE_TIMER4     	0x42200000
#define REG_BASE_TIMER5     	0x42300000
#define REG_BASE_TIMER6     	0x42400000
#define REG_BASE_TIMER7     	0x42500000
#define REG_BASE_TIMER8     	0x42600000
#define REG_BASE_TIMER9     	0x42700000
#define REG_BASE_TIMER10    	0x42800000
#define REG_BASE_TIMER11    	0x42900000
#define REG_BASE_TIMER12    	0x42A00000
#define REG_BASE_TIMER13    	0x42B00000
#define REG_BASE_TIMER14    	0x42C00000
#define REG_BASE_TIMER15    	0x42D00000
#define REG_BASE_TIMER16    	0x42E00000
#define REG_BASE_TIMER17    	0x42F00000
#define REG_BASE_TIMER18    	0x43000000
#define REG_BASE_TIMER19    	0x43100000
#define REG_BASE_TIMER20    	0x43200000
#define REG_BASE_TIMER21    	0x43300000
#define REG_BASE_TIMER22    	0x43400000
#define REG_BASE_TIMER23    	0x43500000
#define REG_BASE_TIMER24    	0x43600000
#define REG_BASE_TIMER25    	0x43700000
#define REG_BASE_TIMER26    	0x43800000
#define REG_BASE_TIMER27    	0x43900000
#define REG_BASE_TIMER28    	0x43A00000
#define REG_BASE_TIMER29    	0x43B00000
#define REG_BASE_TIMER30    	0x43C00000
#define REG_BASE_TIMER31    	0x43D00000
#define REG_BASE_TIMER32    	0x43E00000
#define REG_BASE_TIMER33    	0x43F00000
#define REG_BASE_TIMER34    	0x44000000
#define REG_BASE_TIMER35    	0x44100000
#define REG_BASE_TIMER36    	0x44200000
#define REG_BASE_TIMER37    	0x44300000
#define REG_BASE_TIMER38    	0x44400000
#define REG_BASE_UART0      	0x44500000
#define REG_BASE_UART1      	0x44600000
#define REG_BASE_UART2      	0x44700000
#define REG_BASE_UART3      	0x44800000
#define REG_BASE_UART4      	0x44900000
#define REG_BASE_UART5      	0x44A00000
#define REG_BASE_UART6      	0x44B00000
#define REG_BASE_UART7      	0x44C00000
#define REG_BASE_UART8      	0x44D00000
#define REG_BASE_AES			0x44E00000
#define REG_BASE_SHA			0x44F00000
#define REG_BASE_CHKSUM			0x45000000
#define REG_BASE_SYS			0x45100000
#define REG_BASE_BDMA0			0x45200000
#define REG_BASE_BDMA1			0x45200020
#define REG_BASE_BDMA2			0x45200040
#define REG_BASE_BDMA3			0x45200060
#define REG_BASE_CDMA0			0x45300000
#define REG_BASE_CDMA1			0x45300020
#define REG_BASE_CDMA2			0x45300040
#define REG_BASE_CDMA3			0x45300060
#define REG_BASE_I2S			0x45400000
#define REG_BASE_IRQ			0x45500000
#define REG_BASE_ADC			0x45600000
#define REG_BASE_BUS			0x45700000


//******************************************************************************
// 2. System define
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

#include "cfg_network.h"			// EN675 - Network

#include "asm.h"					// CPU
#include "bits.h"					// CPU
#include "encoding.h"				// CPU
#include "mtrap.h"					// CPU
#include "rv_utils.h" 				// CPU

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
