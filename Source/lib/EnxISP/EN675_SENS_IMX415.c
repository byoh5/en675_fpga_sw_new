/* **************************************************************************
 File Name	:	EN675_SENS_IMX415.c
 Description:	EN675 - IMX415 device driver
 Designer	:	Sung, Min-Je
 Date		:	19. 6. 11
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#define __SENS_LIST__
#include "dev_model.h"				// EN675 - __SENS_LIST__
#define model_Sens	SENS_IMX415

#include "isp_lib_main.h"

//-------------------------------------------------------------------------------------------------

	// Sensor Master Mode,	MIPI 4ch, 12bit,	INCK : 27MHz
	const WORD gwTblIMX415[][2] = 	{
									//0x30**
									{0x3008,    0x5D},
									{0x300A,    0x42},

                                    // Crop
									{0x301C,    0x00},

                                    // Binning
                                    {0x3020,    0x00},
                                    {0x3021,    0x00},
                                    {0x3022,    0x00},

									{0x3024,    0xCA},     // VMAX
									{0x3025,    0x08},     //  "

									{0x3028,    0x98},     // HMAX
									{0x3029,    0x08},     //  "

                                    {0x3031,    0x01},
									{0x3033,    0x05},
                                    // Crop
									{0x3040,    0x00},     // PIX_HST
									{0x3041,    0x00},     //   "
									{0x3042,    0x18},		// PIX_HWIDTH
									{0x3043,    0x0F},     //   "
									{0x3044,    0x18},     // PIX_VST
									{0x3045,    0x0F},     //   "
									{0x3046,    0x20},     // PIX_VWIDTH
									{0x3047,    0x11},     //   "

									{0x3050,    0x08},		//	SHR0[7:0]
									{0x30C1,    0x00},

                                    // Binning
									{0x30D9,    0x06},
									{0x30DA,    0x02},

									{0x3116,    0x23},
									{0x3118,    0xC6},
									{0x311A,    0xE7},
									{0x311E,    0x23},
									{0x32D4,    0x21},
									{0x32EC,    0xA1},
									{0x3452,    0x7F},
									{0x3453,    0x03},
									{0x358A,    0x04},
									{0x35A1,    0x02},
									{0x36BC,    0x0C},
									{0x36CC,    0x53},
									{0x36CD,    0x00},
									{0x36CE,    0x3C},
									{0x36D0,    0x8C},
									{0x36D1,    0x00},
									{0x36D2,    0x71},
									{0x36D4,    0x3C},
									{0x36D6,    0x53},
                                    {0x36D7,    0x00},
									{0x36D8,    0x71},
									{0x36DA,    0x8C},
									{0x36DB,    0x00},
									// Binning
                                    {0x3701,    0x03},


									{0x3724,    0x02},
									{0x3726,    0x02},
									{0x3732,    0x02},

									{0x3734,    0x03},
									{0x3736,    0x03},
									{0x3742,    0x03},

									{0x3862,    0xE0},

									{0x38CC,    0x30},
									{0x38CD,    0x2F},

									{0x395C,    0x0C},

									{0x3A42,    0xD1},

									{0x3A4C,    0x77},

									{0x3AE0,    0x02},

									{0x3AEC,    0x0C},

									{0x3B00,    0x2E},

									{0x3B06,    0x29},

									{0x3B98,    0x25},
									{0x3B99,    0x21},

									{0x3B9B,    0x13},
									{0x3B9C,    0x13},
									{0x3B9D,    0x13},
									{0x3B9E,    0x13},

									{0x3BA1,    0x00},
									{0x3BA2,    0x06},
									{0x3BA3,    0x0B},
									{0x3BA4,    0x10},
									{0x3BA5,    0x14},
                                    {0x3BA6,    0x18},
                                    {0x3BA7,    0x1A},
                                    {0x3BA8,    0x1A},
                                    {0x3BA9,    0x1A},

                                    {0x3BAC,    0xED},
                                    {0x3BAD,    0x01},
									{0x3BAE,    0xF6},
									{0x3BAF,    0x02},

									{0x3BB0,    0xA2},
									{0x3BB1,    0x03},
									{0x3BB2,    0xE0},
									{0x3BB3,    0x03},
									{0x3BB4,    0xE0},
									{0x3BB5,    0x03},
									{0x3BB6,    0xE0},
									{0x3BB7,    0x03},
									{0x3BB8,    0xE0},
									{0x3BBA,    0xE0},
                                    {0x3BBC,    0xDA},
									{0x3BBE,    0x88},
									{0x3BC0,    0x44},
									{0x3BC2,    0x7B},
									{0x3BC4,    0xA2},
									{0x3BC8,    0xBD},
                                    {0x3BCA,    0xBD},

									{0x4004,    0xC0},
									{0x4005,    0x06},

									{0x400C,    0x00},
									{0x4018,    0x7F},

									{0x401A,    0x37},
									{0x401C,    0x37},

                                    {0x401E,    0xF7},
                                    {0x401F,    0x00},
                                    {0x4020,    0x3F},
                                    {0x4022,    0x6F},
                                    {0x4024,    0x3F},
                                    {0x4026,    0x5F},
                                    {0x4028,    0x2F},
                                    {0x4074,    0x01}
									};

void InitSensRun(void)
{
	SetSens(0x3000, 0x1);		WaitXus(1000);
	SetSens(0x3002, 0x1);		WaitXus(1000);
	SetSens(0x3001, 0x1);

	for(UINT i=0;i<ARRAY_SIZE(gwTblIMX415); i++)
	{
		SetSens(gwTblIMX415[i][0],gwTblIMX415[i][1]);
		//WaitXus(200);
	}

	SetSens(0x3001, 0x0);
	SetSens(0x3000, 0x0);		WaitXus(20000);
	SetSens(0x3002, 0x0);		WaitXus(20000);

	SetSens(0x30e2, 0x0);		WaitXus(20000);
}

