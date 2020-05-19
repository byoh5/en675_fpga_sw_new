//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:     dev_debug.h
//	Description:   EN675 Debug definition
//	Author:        Team 2
//
//	Note:          Set the EN675 debugging flag.
//
//	Ver   Date   Author  Changes
//	---  ------  ------  -------
//  0.1  190314  hjlee   support EN675
//------------------------------------------------------------------------------
#ifndef __DEV_DEBUG_H__
#define __DEV_DEBUG_H__

//******************************************************************************
// 0. Define
//------------------------------------------------------------------------------

#define ENX_DEBUG

#define ENX_DBG_ON				0x80	/* Enable that debug message */
#define ENX_DBG_TRACE			0x40	/* Tracing message (to follow program flow) */
#define ENX_DBG_STATE			0x20	/* State debug message (to follow module states) */
#define ENX_DBG_HALT			0x10	/* Halt after printing this debug message */
#define ENX_DBG_OFF				0x00	/* Disable that debug message */

//******************************************************************************
// 1. Function Debug Flag
//------------------------------------------------------------------------------

#define DGB_DMA_MSG				(ENX_DBG_OFF)

#define DBG_SDIO_LOG			(ENX_DBG_OFF)
#define DBG_SDIO_STATUS			(ENX_DBG_ON)

#define DBG_SDIO_CD_CMD			(ENX_DBG_OFF) // (ENX_DBG_ON | ENX_DBG_STATE)
#define DBG_SDIO_CD_DAT			(ENX_DBG_OFF) // (ENX_DBG_ON)
#define DBG_SDIO_CD_ERR			(ENX_DBG_ON)
#define DBG_SDIO_CD_PANIC		(ENX_DBG_ON | ENX_DBG_HALT)

#define DBG_SDIO_WF_CMD			(ENX_DBG_OFF)
#define DBG_SDIO_WF_DAT			(ENX_DBG_OFF)
#define DBG_SDIO_WF_ERR			(ENX_DBG_ON)
#define DBG_SDIO_WF_PANIC		(ENX_DBG_ON | ENX_DBG_HALT)

#define DBG_ETHPHY_MSG			(ENX_DBG_ON)
#define DBG_ETHPHY_LOG			(ENX_DBG_ON)
//#define DBG_ETHPHY_LOG			(ENX_DBG_ON | ENX_DBG_STATE)
//#define DBG_ETHPHY_LOG			(ENX_DBG_OFF)
#define DBG_ETHPHY_ERR			(ENX_DBG_ON)
#define DBG_ETHPHY_PANIC		(ENX_DBG_ON | ENX_DBG_HALT)

#define DBG_MDIO_LOG			(ENX_DBG_OFF)
#define DBG_MDIO_STATUS			(ENX_DBG_ON)

#define DBG_UART_LOG			(ENX_DBG_OFF)
#define DBG_UART_STATUS			(ENX_DBG_ON)

#define DBG_I2C_LOG				(ENX_DBG_OFF)
#define DBG_I2C_STATUS			(ENX_DBG_ON)

#define DBG_SPI_LOG				(ENX_DBG_OFF)
#define DBG_SPI_STATUS			(ENX_DBG_ON)

#define DBG_GPIO_LOG			(ENX_DBG_ON)
#define DGB_GPIO_ERR			(ENX_DBG_ON | ENX_DBG_HALT)

#define DBG_ADC_LOG				(ENX_DBG_ON)

#define DBG_SFLS_LOG			(ENX_DBG_OFF)
#define DBG_SFLS_STATUS			(ENX_DBG_ON)
#define DBG_SFLS_WARNING		(ENX_DBG_ON)
#define DBG_SFLS_ERR			(ENX_DBG_ON | ENX_DBG_HALT)

//******************************************************************************
// 2. Debug Log
//------------------------------------------------------------------------------
#ifdef ENX_DEBUG
#define ENX_DEBUGF(debug, fmt, args...) do { \
											if (((debug) & ENX_DBG_ON)) { \
												printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); \
												if ((debug) & ENX_DBG_HALT) { \
													while(1); \
												} \
											} \
										} while(0)
#define ENX_LOG_START(debug)			do { \
											if (((debug) & ENX_DBG_ON)) { \
												printf("(%04d)%s : START\n", __LINE__, __func__); \
											} \
										} while(0)
#define ENX_LOG_END(debug)				do { \
											if (((debug) & ENX_DBG_ON)) { \
												printf("(%04d)%s : E N D\n", __LINE__, __func__); \
											} \
										} while(0)
#else
#define ENX_DEBUGF(debug, fmt, args...) do { } while(0);
#define ENX_LOG_START(debug)			do { } while(0);
#define ENX_LOG_END(debug)				do { } while(0);
#endif

#define ENX_ASSERT(x)                         if( ( x ) == 0 ) { __asm volatile("csrc mstatus, 8"); printf("(%04d)%s : Assert!\n", __LINE__, __func__); for( ;; ); }

#endif //__DEV_DEBUG_H__
