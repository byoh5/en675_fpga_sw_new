/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

// API Command Lengths
//=======================

// These are here because they are referenced in av_config.c
// That needs to change.

#define VIDEO_SETUP_CMD_LEN     	(9)
#define VIDEO_SYNC_SETUP_CMD_LEN	(20)

#define AUDIO_SETUP_CMD_LEN 		(7)
#define I2S_MAPPING_CMD_LEN     	(4)
#define I2S_INPUT_CFG_CMD_LEN   	(1)
#define I2S_STREAM_HDR_CMD_LEN  	(5)

#define AVI_INFOFRAME_CMD_LEN		(14)
#define AUDIO_INFOFRAME_CMD_LEN		(15)

#define EHDMI_MODE_CMD_LEN			(1)
#define HDCP_OVERRIDE_MODE_CMD_LEN	(1)

#define PRODUCT_CODE_CMD_LEN		(1)
#define TPI_FW_VERSION_CMD_LEN		((byte)sizeof(TPI_FW_VERSION))
#define CTRL_CMD_VERSION_CMD_LEN	(1)

//void SerComm(void);
