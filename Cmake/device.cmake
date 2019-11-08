﻿SET ( OSC_FREQ 25000000 ) # 25 MHz or 27 MHz
SET ( MCK_FREQ 50000000 ) # 50 MHz or 54 MHz
SET ( CPU_FREQ 75000000 ) # 75 MHz or 81 MHz

SET ( USE_SFLS_EN25QH128A 1 )
SET ( USE_SFLS_EN25QH256 1 )
SET ( USE_SFLS_W25Q64 1 )
SET ( USE_SFLS_MX25L12835F 1 )
SET ( USE_SFLS_N25Q128A 1 )
SET ( USE_SFLS_GD25Q128C 1 )
SET ( USE_SFLS_IS25LP064 1 )
SET ( USE_SFLS_IS25LP128 1 )

SET ( USE_FREERTOS 1 )

SET ( USE_ECM_STRING 0 )

SET ( USE_SENSOR 0 )
SET ( SENSOR_RESET_GPIO_CH 1 )
SET ( SENSOR_I2C_CH 0 )
SET ( SENSOR_SPI_CH 0 )

SET ( USE_HDMI 0 )
SET ( HDMI_I2C_CH 7 )

SET ( USE_RTC 1 )
SET ( USE_RTC_PCF8563 1 )
SET ( USE_RTC_S35390A 0 )
SET ( RTC_I2C_CH 7 )

SET ( USE_EEPROM 1 )
SET ( EEPROM_I2C_CH 7 )

SET ( USE_NETWORK 1 )
SET ( USE_ETHERNET 1 )
SET ( ETHPHY_RST_GPIO_CH 54 )
SET ( ETHPHY_IRQ_GPIO_CH 53 )
SET ( USE_ETHPHY_MDIO_ADR 1 )
SET ( USE_ETHPHY_KSZ8081MNX 1 )
SET ( USE_ETHPHY_KSZ8081RNB 0 )
SET ( USE_ETHPHY_KSZ9031RNX 0 )

SET ( USE_AUDIO 0 )
SET ( AUDIO_RST_GPIO_CH 20 )
SET ( AUDIO_IRQ_GPIO_CH 21 )
SET ( AUDIO_I2C_CH 2 )
SET ( USE_AUDIO_TLV320AIC3206 1 )

SET ( USE_IRIS_PWM 0 )
SET ( IRIS_PWM_TIMER_CH 38 )

SET ( USE_IR_LED_LPWM 0 )

SET ( USE_IR_LED_GPIO 0 )
SET ( IR_LED_GPIO_CH 55 )

SET ( USE_LED0 1 )
SET ( LED0_GPIO_CH 55 )
SET ( USE_LED1 1 )
SET ( LED1_GPIO_CH 7 )
SET ( USE_LED2 1 )
SET ( LED2_GPIO_CH 8 )

SET ( USE_SD 1 )
SET ( SD_SDIO_CH 0 )
SET ( USE_SD_RST 33 )
SET ( USE_SD_IRQ 32 )

SET ( USE_WF 0 )
SET ( WF_SDIO_CH 1 )
SET ( USE_WF_RST 35 )
SET ( USE_WF_IRQ 34 )
SET ( USE_WF_SD8782 0 )

SET ( USE_FIESYSTEM 1 )
SET ( USE_FS_FLS 0 )
SET ( USE_FS_SD 1)
SET ( USE_FS_USB 0 )
SET ( USE_FS_MEM 0 )