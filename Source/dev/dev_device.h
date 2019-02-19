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
#endif
#if 0
#define __RTC_S35390A__
#endif
#endif

#endif // __DEV_DEVICE_H__
