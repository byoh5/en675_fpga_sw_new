//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:     dev_peri.h
//	Description:   EN675 Registere definition
//	Author:        Team 2
//
//	Note:          This file is created automatically.
//                 Changing this file does not affect your code.
//                If you want to modify it, edit "dev_device.h.cmake.in".
//
//	Ver   Date   Author  Changes
//	---  ------  ------  -------
//  0.1  190214  hjlee   support EN675
//------------------------------------------------------------------------------
#ifndef __DEV_DEVICE_H__
#define __DEV_DEVICE_H__

#if 1
#define __RTC_LOAD__
#define RTC_I2C_CH	7
#if 1
#define __RTC_PCF8563__
#endif // __RTC_PCF8563__
#if 0
#define __RTC_S35390A__
#endif // __RTC_S35390A__
#endif // __RTC_LOAD__

#if 0
#define __NETWORK__
#if 1
#define __ETHERNET__
#define ETHPHY_GPIO_RST	54
#define ETHPHY_GPIO_IRQ	53
#define ETHPHY_MDIO_ADR	1
#if 1
#define __ETHPHY_KSZ8081MNX__
#endif // __ETHPHY_KSZ8081MNX__
#if 0
#define __ETHPHY_KSZ8081RNB__
#endif // __ETHPHY_KSZ8081RNB__
#if 0
#define __ETHPHY_KSZ9031RNX__
#endif // __ETHPHY_KSZ9031RNX__
#endif // __ETHERNET__
#endif // __NETWORK__

#if 0
#define __AUDIO__
#define AUDIO_GPIO_RST	20
#define AUDIO_GPIO_IRQ	21
#define AUDIO_I2C_CH	2
#endif // __AUDIO__

#if 1
#define __USE_LED0__
#define GPIO_LED0	55
#endif // __USE_LED0__

#if 1
#define __USE_LED1__
#define GPIO_LED1	7
#endif // __USE_LED1__

#if 1
#define __USE_LED2__
#define GPIO_LED2	8
#endif // __USE_LED2__

#if 1
#define __USE_SD__
#define SD_GPIO_RST	33
#define SD_GPIO_IRQ	32
#define SD_SDIO_CH	0
#endif // __USE_SD__

#if 1
#define __USE_WF__
#define WF_GPIO_RST	35
#define WF_GPIO_IRQ	34
#define WF_SDIO_CH	1
#endif // __USE_WF__

#if 1
#define __FILESYSTEM__

#if 0
#define LOAD_FS_FLS 1
#else
#define LOAD_FS_FLS 0
#endif // LOAD_FS_FLS
#if 1
#define LOAD_FS_SDCARD 1
#else
#define LOAD_FS_SDCARD 0
#endif // LOAD_FS_SDCARD
#if 0
#define LOAD_FS_USB 1
#else
#define LOAD_FS_USB 0
#endif // LOAD_FS_USB
#if 0
#define LOAD_FS_MEM 1
#else
#define LOAD_FS_MEM 0
#endif // LOAD_FS_MEM
#endif // __FILESYSTEM__

#endif // __DEV_DEVICE_H__
