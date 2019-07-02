//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:     dev_device.h
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

#define OSC_FREQ	25000000
#define MCK_FREQ	50000000
#define CPU_FREQ	75000000

#if 0
#define __SFLS_EN25QH128A__
#define __SFLS_EON__
#define __SFLS_16MB__
#endif
#if 0
#define __SFLS_EN25QH256__
#define __SFLS_EON__
#define __SFLS_32MB__
#endif
#if 0
#define __SFLS_W25Q64__
#define __SFLS_WINBOND__
#define __SFLS_8MB__
#endif
#if 0
#define __SFLS_MX25L12835F__
#define __SFLS_MACRONIX__
#define __SFLS_16MB__
#endif
#if 0
#define __SFLS_N25Q128A__
#define __SFLS_MICRON__
#define __SFLS_16MB__
#endif
#if 0
#define __SFLS_GD25Q128C__
#define __SFLS_GIGADEVICE__
#define __SFLS_16MB__
#endif
#if 0
#define __SFLS_IS25LP064__
#define __SFLS_ISSI__
#define __SFLS_8MB__
#endif
#if 1
#define __SFLS_IS25LP128__
#define __SFLS_ISSI__
#define __SFLS_16MB__
#endif

#if 1
#define __FREERTOS__
#endif // __FREERTOS__

#if 0
#define __ECM_STRING__
#endif // __ECM_STRING__

#if 0
#define __SENSOR__
#define SENSOR_RESET_GPIO_CH	1
#define SENSOR_I2C_CH	0
#define SENSOR_SPI_CH	0
#endif // __SENSOR__

#if 0
#define __HDMI__
#define HDMI_I2C_CA	0x90
#define HDMI_I2C_CH	7
#endif // __HDMI__

#if 1
#define __RTC_LOAD__
#define RTC_I2C_CH	7
#if 1
#define __RTC_PCF8563__
#define RTC_PCF8563_I2C_CA	0xa2
#endif // __RTC_PCF8563__
#if 0
#define __RTC_S35390A__
#define RTC_S35390A_I2C_CA	0x60
#endif // __RTC_S35390A__
#endif // __RTC_LOAD__

#if 1
#define __EEPROM__
#define EEPROM_I2C_CA	0xa0
#define EEPROM_I2C_CH	7
#endif // __EEPROM__

#if 1
#define __NETWORK__
#if 1
#define __ETHERNET__
#define ETHPHY_GPIO_RST	54
#define ETHPHY_GPIO_IRQ	53
#define ETHPHY_MDIO_ADR	1
#if 1
#define __ETHPHY_KSZ8081MNX__
#define ETHPHY_TYPE_VAL 2
#define ETHPHY_SPD_VAL 100
#define ETHPHY_DUPLEX_VAL 2
#endif // __ETHPHY_KSZ8081MNX__
#if 0
#define __ETHPHY_KSZ8081RNB__
#define ETHPHY_TYPE_VAL 3
#define ETHPHY_SPD_VAL 100
#define ETHPHY_DUPLEX_VAL 2
#endif // __ETHPHY_KSZ8081RNB__
#if 0
#define __ETHPHY_KSZ9031RNX__
#define ETHPHY_TYPE_VAL 1
#define ETHPHY_SPD_VAL 1000
#define ETHPHY_DUPLEX_VAL 2
#endif // __ETHPHY_KSZ9031RNX__
#endif // __ETHERNET__
#endif // __NETWORK__

#if 1
#define __AUDIO__
#define AUDIO_GPIO_RST	20
#define AUDIO_GPIO_IRQ	21
#define AUDIO_I2C_CH	2
#if 1
#define __AUDIO_TLV320AIC3206__
#define AUDIO_TLV320AIC3206_I2C_CA 0x30
#endif
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
#define __USE_SDIOCD__
#define SD_GPIO_RST	33
#define SD_GPIO_IRQ	32
#define SD_SDIO_CH	0
#endif // __USE_SDIOCD__

#if 0
#define __WIFI__
#define WF_GPIO_RST	35
#define WF_GPIO_IRQ	34
#define WF_SDIO_CH	1
#endif // __WIFI__

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
