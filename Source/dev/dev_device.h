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

#define model_TgtBd 0

#define EN675_SINGLE 1

#define OSC_FREQ	27000000
#define APB_FREQ	37125000
#define AXI_FREQ	74250000
#define CPU_FREQ	74250000

#if 1
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
#define __SFLS_W25Q64JVIQ__
#define __SFLS_WINBOND__
#define __SFLS_8MB__
#endif
#if 0
#define __SFLS_W25Q256JVQ__
#define __SFLS_WINBOND__
#define __SFLS_32MB__
#endif
#if 0
#define __SFLS_MX25L12835F__
#define __SFLS_MACRONIX__
#define __SFLS_16MB__
#endif
#if 1
#define __SFLS_MX25U12832F__
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
#if 0
#define __SFLS_IS25LP128__
#define __SFLS_ISSI__
#define __SFLS_16MB__
#endif
#if 0
#define __SFLS_IS25LP256__
#define __SFLS_ISSI__
#define __SFLS_32MB__
#endif

#if 1
#define __FREERTOS__
#endif // __FREERTOS__

#if 0
#define __ECM_STRING__
#endif // __ECM_STRING__

#if 0
#define __USE_ISP__
#endif // __USE_ISP__

#define USE_ISP_FRC		0

#if 0
#define __SENSOR__
#define SENSOR_RESET_GPIO_CH	1
#define SENSOR_I2C_CH	0
#define SENSOR_SPI_CH	0
#endif // __SENSOR__

#define model_Sens		SENS_IMX415
#define model_Sens_Ctrl	1
#define model_Sens_Intf	1
#define model_Sens_Fps	15

#if 0
#define model_Lcd		1
#endif // __USE_LCD__

#define model_Save		2
#define model_Key		0

#define model_Lens		0
#define model_MAF_Lens	0
#define model_Iris		0

#define model_Tdn		2
#define model_Tdn_Ext	0

#define model_Led		0

#define model_MotionOut	0


#if 0 || 0
#define HDMI_I2C_CH	7
#endif

#if 0
#define __HDMI_SILICON__
#define HDMI_I2C_CA	0x90
#endif // __HDMI_SILICON__

#if 0
#define __HDMI_LONTIUM__
//#define   LT8618SX_ADR			0x76		//CI2CA HIGH : 0x76
#define   LT8618SX_ADR			0x72		//CI2CA LOW : 0x72
#endif // __HDMI_LONTIUM__

#if 0
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

#if 0
#define __AUDIO__
#define AUDIO_GPIO_RST	20
#define AUDIO_GPIO_IRQ	21
#define AUDIO_I2C_CH	2
#if 0
#define __AUDIO_TLV320AIC3206__
#define AUDIO_TLV320AIC3206_I2C_CA 0x30
#endif
#endif // __AUDIO__

#if 0
#define __USE_IRIS_PWM__
#define TIMER_PWM_IRIS	38
#endif // __USE_IRIS_PWM__

#if 0
#define __USE_IR_LED_LPWM__
#endif // __USE_IR_LED_LPWM__

#if 0
#define __USE_IR_LED_GPIO__
#define GPIO_IR_LED	55
#endif // __USE_IR_LED_GPIO__

#if 1
#define __USE_LED0__
#define GPIO_LED0	55
#endif // __USE_LED0__

#if 1
#define __USE_LED1__
#define GPIO_LED1	7
#endif // __USE_LED1__

#if 0
#define __USE_LED2__
#define GPIO_LED2	8
#endif // __USE_LED2__

#if 0
#define __USE_SDIOCD__
#define SD_GPIO_RST	33
#define SD_GPIO_IRQ	32
#define SD_SDIO_CH	0
#endif // __USE_SDIOCD__

#if 0
#define __WIFI__
#define WF_GPIO_RST	35
#define WF_GPIO_IRQ	34
#define WF_SDIO_CH	0
#if 0
#define __WIFI_SD8782__
#endif
#endif // __WIFI__

#if 0
#define __FILESYSTEM__
#if 0
#define LOAD_FS_FLS 1
#else
#define LOAD_FS_FLS 0
#endif // LOAD_FS_FLS
#if 0
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

#if 0
#define __IR__
#define IR_IRQOCC_ADDR	0
#endif // __IR__

#endif // __DEV_DEVICE_H__
