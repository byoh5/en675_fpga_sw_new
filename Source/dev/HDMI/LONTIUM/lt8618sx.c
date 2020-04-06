/******************************************************************************
  * @project: LT8618SX
  * @file: lt8618sx.c
  * @author: xhguo
  * @company: LONTIUM COPYRIGHT and CONFIDENTIAL
  * @date: 2017.07.25
******************************************************************************/

#include "dev.h"

#ifdef __HDMI_LONTIUM__

#include "lt8618sx.h"

BYTE I2CADR;

											 	//  hfp,hs,hbp,hact,htot,vfp,vs,vbp,vact, vtot
const struct video_timing video_1280x720_50P	= { 440,40,220,1280,1980,  5, 5, 20, 720, 750};
const struct video_timing video_1280x720_60P	= { 110,40,220,1280,1650,  5, 5, 20, 720, 750};

const struct video_timing video_1920x1080_50P	= { 528,44,148,1920,2640,  4, 5, 36,1080,1125};
const struct video_timing video_1920x1080_60P	= {  88,44,148,1920,2200,  4, 5, 36,1080,1125};

const struct video_timing video_1920x1080_50I	= { 528,44,148,1920,2640,  4,10, 15,1080,1125};
const struct video_timing video_1920x1080_60I	= {  88,44,148,1920,2200,  4,10, 15,1080,1125};

const struct video_timing video_2560x1440_25P	= { 880,80,440,2560,3960, 10,10, 40,1440,1500};
const struct video_timing video_2560x1440_30P	= { 220,80,440,2560,3300, 10,10, 40,1440,1500};

const struct video_timing video_3840x2160_25P	= {1056,88,296,3840,5280,  8,10, 72,2160,2250};
const struct video_timing video_3840x2160_30P	= { 176,88,296,3840,4400, 20,10, 60,2160,2250};


video_timing const* in_video_timing = &video_1920x1080_60P;

BYTE HDMI_VIC=0x00;		// Video Format Identification Code
BYTE HDMI_Y=0x00;		// 0:RGB, 1:YCbCr444, 2:YCbCr422


void HDMI_WriteI2C_Byte(BYTE RegAddr, BYTE d)
{
//	HDMI_TwiWrite(I2CADR,RegAddr,d);
	HDMI_TwiWrite(LT8618SX_ADR,RegAddr,d);
}

BYTE HDMI_ReadI2C_Byte(BYTE RegAddr)
{
	BYTE p_data;
	p_data = 0;
//	HDMI_TwiRead(I2CADR, RegAddr, &p_data);
	HDMI_TwiRead(LT8618SX_ADR, RegAddr, &p_data);

	return p_data;
}

void LT8618SX_Chip_ID(void)
{
	uint32_t dwChipID = 0x00;

	I2CADR = LT8618SX_ADR;
	HDMI_WriteI2C_Byte(0xFF,0x80);
	HDMI_WriteI2C_Byte(0xee,0x01);

	dwChipID = (HDMI_ReadI2C_Byte(0x00)<<16) | (HDMI_ReadI2C_Byte(0x01)<<8) | HDMI_ReadI2C_Byte(0x02);
//	xil_printf("LT8618SX Chip ID 0x%x\n\r",dwChipID);
	_printf("LT8618SX Chip ID 0x%x\n\r",dwChipID);
}

void LT8618SX_RST_PD_Init(void)
{
	HDMI_WriteI2C_Byte(0xff,0x80);
	HDMI_WriteI2C_Byte(0x11,0x00); //reset MIPI Rx logic.
}

void LT8618SX_TTL_Input_Analog(void)//xuxi
{
	//TTL mode
	HDMI_WriteI2C_Byte(0xff,0x81);
	HDMI_WriteI2C_Byte(0x02,0x66);
	HDMI_WriteI2C_Byte(0x0a,0x06);
	HDMI_WriteI2C_Byte(0x15,0x06);
}

void LT8618SX_TTL_Input_Digtal(void) //dsren
{
	//TTL mode
	#if (Video_Input_Mode==Input_RGB888)
	{
//		xil_printf("Video_Input_Mode=Input_RGB888\n\r");
		_printf("Video_Input_Mode=Input_RGB888\n\r");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0x45,0x70); //RGB channel swap
		HDMI_WriteI2C_Byte(0x4f,0x40); //0x00;  0x40: dclk
		HDMI_WriteI2C_Byte(0x50,0x00);
	}
	#elif (Video_Input_Mode==Input_YCbCr444)
	{
//		xil_printf("Video_Input_Mode=Input_YCbCr444\n\r");
		_printf("Video_Input_Mode=Input_YCbCr444\n\r");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0x45,0x70); //RGB channel swap
		HDMI_WriteI2C_Byte(0x4f,0x40); //0x00;  0x40: dclk
		HDMI_WriteI2C_Byte(0x50,0x00);
	}
	#elif (Video_Input_Mode==Input_YCbCr422_16BIT)
	{
//		xil_printf("Video_Input_Mode=Input_YCbCr422_16BIT\n\r");
		_printf("Video_Input_Mode=Input_YCbCr422_16BIT\n\r");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0x45,0x00); //RGB channel swap
		HDMI_WriteI2C_Byte(0x4f,0x40); //0x00;  0x40: dclk
		HDMI_WriteI2C_Byte(0x50,0x00);
	}
	#elif (Video_Input_Mode==Input_BT1120_16BIT)
	{
//		xil_printf("Video_Input_Mode=Input_BT1120_16BIT\n\r");
		_printf("Video_Input_Mode=Input_BT1120_16BIT\n\r");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0x45,0x70); //RGB channel swap
		HDMI_WriteI2C_Byte(0x4f,0x40); //0x00;  0x40: dclk
		HDMI_WriteI2C_Byte(0x50,0x00);
		HDMI_WriteI2C_Byte(0x48,0x0c); //Embedded sync mode input enable.
	}
	#elif (Video_Input_Mode==Input_BT656_8BIT)
	{
//		xil_printf("Video_Input_Mode=Input_BT656_8BIT\n\r");
		_printf("Video_Input_Mode=Input_BT656_8BIT\n\r");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0x45,0x00); //RGB channel swap
		HDMI_WriteI2C_Byte(0x4f,0x00); //0x00;  0x40: dclk
		HDMI_WriteI2C_Byte(0x50,0x00);
		HDMI_WriteI2C_Byte(0x48,0x48); //48Embedded sync mode input enable.
	}
	#endif
}

void LT8618SX_BT_Set(BYTE VideoFmt)
{
	#if (Video_Input_Mode==Input_BT1120_16BIT)
	{
		switch(VideoFmt){
			case 1 : in_video_timing = &video_1280x720_50P;		HDMI_VIC = 0;	break;		//720P25
			case 2 : in_video_timing = &video_1280x720_60P;		HDMI_VIC = 0;	break;		//720P30
			case 3 : in_video_timing = &video_1280x720_50P;		HDMI_VIC = 19;	break;		//720P50
			case 4 : in_video_timing = &video_1280x720_60P;		HDMI_VIC = 4;	break;		//720P60

			case 5 : in_video_timing = &video_1920x1080_50P;	HDMI_VIC = 33;	break;		//1080P25
			case 6 : in_video_timing = &video_1920x1080_60P;	HDMI_VIC = 34;	break;		//1080P30
			case 7 : in_video_timing = &video_1920x1080_50I;	HDMI_VIC = 20;	break;		//1080i50
			case 8 : in_video_timing = &video_1920x1080_60I;	HDMI_VIC = 5;	break;		//1080i60
			case 9 : in_video_timing = &video_1920x1080_50P;	HDMI_VIC = 31;	break;		//1080P50
			case 10: in_video_timing = &video_1920x1080_60P;	HDMI_VIC = 16;	break;		//1080P60

			case 11: in_video_timing = &video_2560x1440_25P;	HDMI_VIC = 0;	break;		//1440P25
			case 12: in_video_timing = &video_2560x1440_30P;	HDMI_VIC = 0;	break;		//1440P30
			case 15: in_video_timing = &video_2560x1440_25P;	HDMI_VIC = 0;	break;		//1440P12.5
			case 16: in_video_timing = &video_2560x1440_30P;	HDMI_VIC = 0;	break;		//1440P15

			case 13: in_video_timing = &video_3840x2160_25P;	HDMI_VIC = 0;	break;		//2160P25
			case 14: in_video_timing = &video_3840x2160_30P;	HDMI_VIC = 0;	break;		//2160P30

			default: in_video_timing = &video_1920x1080_60P;	HDMI_VIC = 16;	break;
		}

		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0x20,(in_video_timing->hact>>8));
		HDMI_WriteI2C_Byte(0x21,in_video_timing->hact);
		HDMI_WriteI2C_Byte(0x22,(in_video_timing->hfp>>8));
		HDMI_WriteI2C_Byte(0x23,in_video_timing->hfp);
		HDMI_WriteI2C_Byte(0x24,(in_video_timing->hs>>8));
		HDMI_WriteI2C_Byte(0x25,in_video_timing->hs);
		HDMI_WriteI2C_Byte(0x26,0x00);
		HDMI_WriteI2C_Byte(0x27,0x00);

		HDMI_WriteI2C_Byte(0x2c,(in_video_timing->htotal>>8));
		HDMI_WriteI2C_Byte(0x2d,in_video_timing->htotal);
		HDMI_WriteI2C_Byte(0x2e,(in_video_timing->hact>>8));
		HDMI_WriteI2C_Byte(0x2f,in_video_timing->hact);
		HDMI_WriteI2C_Byte(0x30,(in_video_timing->hfp>>8));
		HDMI_WriteI2C_Byte(0x31,in_video_timing->hfp);
		HDMI_WriteI2C_Byte(0x32,(in_video_timing->hbp>>8));
		HDMI_WriteI2C_Byte(0x33,in_video_timing->hbp);
		HDMI_WriteI2C_Byte(0x34,(in_video_timing->hs>>8));
		HDMI_WriteI2C_Byte(0x35,in_video_timing->hs);

		HDMI_WriteI2C_Byte(0x36,(in_video_timing->vact>>8));
		HDMI_WriteI2C_Byte(0x37,in_video_timing->vact);
		HDMI_WriteI2C_Byte(0x38,(in_video_timing->vfp>>8));
		HDMI_WriteI2C_Byte(0x39,in_video_timing->vfp);
		HDMI_WriteI2C_Byte(0x3a,(in_video_timing->vbp>>8));
		HDMI_WriteI2C_Byte(0x3b,in_video_timing->vbp);
		HDMI_WriteI2C_Byte(0x3c,(in_video_timing->vs>>8));
		HDMI_WriteI2C_Byte(0x3d,in_video_timing->vs);

		// interlace or progressive
		if((VideoFmt==7)|(VideoFmt==8)){
			HDMI_WriteI2C_Byte(0x47,0x47);
			HDMI_WriteI2C_Byte(0x48,0x8c);
		}
		else {
			HDMI_WriteI2C_Byte(0x47,0x07);
			HDMI_WriteI2C_Byte(0x48,0x0c);
		}
	}
	#endif
}

void LT8618SX_PLL(void) //zhangzhichun
{
	/*
	//4K 300MHz Pixel clk
	HDMI_WriteI2C_Byte(0xff,0x81);
	HDMI_WriteI2C_Byte(0x23,0x40);
	HDMI_WriteI2C_Byte(0x24,0x64); //icp set
	HDMI_WriteI2C_Byte(0x25,0x0b);
	HDMI_WriteI2C_Byte(0x2c,0xbc);
	HDMI_WriteI2C_Byte(0x2d,0x88);
	HDMI_WriteI2C_Byte(0x2e,0x01);
	HDMI_WriteI2C_Byte(0x2f,0x00);
	HDMI_WriteI2C_Byte(0x26,0x55);
	HDMI_WriteI2C_Byte(0x27,0x60);
	HDMI_WriteI2C_Byte(0x28,0x88);
	*/

	#if (Video_Input_Mode==Input_RGB888||Video_Input_Mode==Input_YCbCr444||Video_Input_Mode==Input_YCbCr422_16BIT||Video_Input_Mode==Input_BT1120_16BIT)
	{
//		xil_printf("Tx_PLL==Input_RGB_SDR\n\r");
		_printf("Tx_PLL==Input_RGB_SDR\n\r");
		HDMI_WriteI2C_Byte(0xff,0x81);
		HDMI_WriteI2C_Byte(0x23,0x40);
		HDMI_WriteI2C_Byte(0x24,0x64); //icp set
		HDMI_WriteI2C_Byte(0x25,0x05);
		HDMI_WriteI2C_Byte(0x2c,0x9e);
		HDMI_WriteI2C_Byte(0x2d,0x88);
		HDMI_WriteI2C_Byte(0x2e,0x01);
		HDMI_WriteI2C_Byte(0x2f,0x00);
		HDMI_WriteI2C_Byte(0x26,0x55);
		HDMI_WriteI2C_Byte(0x27,0x60);
		HDMI_WriteI2C_Byte(0x28,0x88);
	}
	#elif (Video_Input_Mode==Input_BT656_8BIT||Video_Input_Mode==Input_BT656_10BIT||Video_Input_Mode==Input_BT656_12BIT)
	{
//		xil_printf("Tx_PLL==Input_BT656\n\r");
		_printf("Tx_PLL==Input_BT656\n\r");
		HDMI_WriteI2C_Byte(0xff,0x81);
		HDMI_WriteI2C_Byte(0x23,0x40);
		HDMI_WriteI2C_Byte(0x24,0x64); //icp set
		HDMI_WriteI2C_Byte(0x25,0x05);
		HDMI_WriteI2C_Byte(0x2c,0x9e);
		HDMI_WriteI2C_Byte(0x2d,0x89); //BT656
		HDMI_WriteI2C_Byte(0x2e,0x01);
		HDMI_WriteI2C_Byte(0x2f,0x00);
		HDMI_WriteI2C_Byte(0x26,0x55);
		HDMI_WriteI2C_Byte(0x27,0x60);
		HDMI_WriteI2C_Byte(0x28,0xa9);//BT656
	}
	#endif
}

void LT8618SX_HDMI_TX_Phy(void)
{
	HDMI_WriteI2C_Byte(0xff,0x81);
	HDMI_WriteI2C_Byte(0x30,0xea);
	HDMI_WriteI2C_Byte(0x31,0x44);
	HDMI_WriteI2C_Byte(0x32,0x4a);
	HDMI_WriteI2C_Byte(0x33,0x0b);
	HDMI_WriteI2C_Byte(0x34,0x00);
	HDMI_WriteI2C_Byte(0x35,0x00);
	HDMI_WriteI2C_Byte(0x36,0x00);
	HDMI_WriteI2C_Byte(0x37,0x44);
	HDMI_WriteI2C_Byte(0x3f,0x0f);
	HDMI_WriteI2C_Byte(0x40,0xa0);
	HDMI_WriteI2C_Byte(0x41,0xa0);
	HDMI_WriteI2C_Byte(0x42,0xa0);
	HDMI_WriteI2C_Byte(0x43,0xa0);
	HDMI_WriteI2C_Byte(0x44,0x0a);
}

void LT8618SX_HDMI_TX_Digital(void)
{
  	//AVI
	HDMI_WriteI2C_Byte(0xff,0x84);
	HDMI_WriteI2C_Byte(0x43,0x6f-HDMI_VIC-((HDMI_Y<<5)+0x10)-0x28);  //AVI_PB0
	HDMI_WriteI2C_Byte(0x44,(HDMI_Y<<5)+0x10);	//AVI_PB1
	HDMI_WriteI2C_Byte(0x45,0x28);				//AVI_PB2, 16:9
	HDMI_WriteI2C_Byte(0x47,HDMI_VIC);			//AVI_PB4
}

void LT8618SX_CSC(void)
{
	#if (Video_Output_Mode == Output_RGB888)
	{
//		xil_printf("Video_Output_Mode=Output_RGB888\n\r");
		_printf("Video_Output_Mode=Output_RGB888\n\r");
		HDMI_Y=0;
		HDMI_WriteI2C_Byte(0xff,0x82);
		if(Video_Input_Mode==Input_YCbCr444)
		{
			HDMI_WriteI2C_Byte(0xb9,0x08); //YCbCr to RGB
		}
	  	else if(Video_Input_Mode==Input_YCbCr422_16BIT||
			 Video_Input_Mode==Input_BT1120_16BIT||
			 Video_Input_Mode==Input_BT1120_20BIT||
			 Video_Input_Mode==Input_BT1120_24BIT||
			 Video_Input_Mode==Input_BT656_8BIT ||
			 Video_Input_Mode==Input_BT656_10BIT||
			 Video_Input_Mode==Input_BT656_12BIT)
		{
			HDMI_WriteI2C_Byte(0xb9,0x18); //YCbCr to RGB,YCbCr 422 convert to YCbCr 444
		}
		else
		  HDMI_WriteI2C_Byte(0xb9,0x00); //No csc
	}
	#elif (Video_Output_Mode == Output_YCbCr444)
	{
//		xil_printf("Video_Output_Mode=Output_YCbCr444\n\r");
		_printf("Video_Output_Mode=Output_YCbCr444\n\r");
		HDMI_Y=1;
	}
	#elif (Video_Output_Mode == Output_YCbCr422_16BIT)
	{
//		xil_printf("Video_Output_Mode=Output_YCbCr422_16BIT\n\r");
		_printf("Video_Output_Mode=Output_YCbCr422_16BIT\n\r");
		HDMI_Y=2;
	}
	#endif
}

void LT8618SX_Audio_Init(void)
{
	#if (Audio_Input_Mode==I2S_2CH)
	{
//		xil_printf("Audio input I2S 2ch\n\r");
		_printf("Audio input I2S 2ch\n\r");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0xd6,0x8c);
		HDMI_WriteI2C_Byte(0xd7,0x04); //sync polarity

		HDMI_WriteI2C_Byte(0xff,0x84);
		HDMI_WriteI2C_Byte(0x06,0x08);
		HDMI_WriteI2C_Byte(0x07,0x10);

		//add
		HDMI_WriteI2C_Byte(0x16,0x01);
		HDMI_WriteI2C_Byte(0x34,0xd4); //CTS_N
	}
	#elif (Audio_Input_Mode==SPDIF)
	{
//		xil_printf("Audio input SPDIF\n\r");
		_printf("Audio input SPDIF\n\r");
		HDMI_WriteI2C_Byte(0xff,0x82);
		HDMI_WriteI2C_Byte(0xd6,0x8c);
		HDMI_WriteI2C_Byte(0xd7,0x04); //sync polarity

		HDMI_WriteI2C_Byte(0xff,0x84);
		HDMI_WriteI2C_Byte(0x06,0x0c);
		HDMI_WriteI2C_Byte(0x07,0x10);

		HDMI_WriteI2C_Byte(0x34,0xd4); //CTS_N
	}
	#endif
}

void LT8618SX_IRQ_Init(void)
{
	HDMI_WriteI2C_Byte(0xff,0x82);
	//HDMI_WriteI2C_Byte(0x10,0x00); //Output low level active;
	HDMI_WriteI2C_Byte(0x58,0x02); //Det HPD
	//HDMI_WriteI2C_Byte(0x00,0xff); //mask0
	//HDMI_WriteI2C_Byte(0x01,0xff); //mask1
	//HDMI_WriteI2C_Byte(0x02,0xff); //mask2
	HDMI_WriteI2C_Byte(0x03,0x3f); //mask3  //Tx_det
	//HDMI_WriteI2C_Byte(0x04,0xff); //clear0
	//HDMI_WriteI2C_Byte(0x05,0xff); //clear1
	//HDMI_WriteI2C_Byte(0x06,0xff); //clear2
	HDMI_WriteI2C_Byte(0x07,0xff); //clear3
	HDMI_WriteI2C_Byte(0x07,0x3f); //clear3
}

void LT8618SX_Reset(void)
{
	HDMI_WriteI2C_Byte(0xff,0x80);
	HDMI_WriteI2C_Byte(0xee,0x01);

	HDMI_WriteI2C_Byte(0x10,0x00);
	HDMI_WriteI2C_Byte(0x11,0x00);
	HDMI_WriteI2C_Byte(0x12,0x00);
	HDMI_WriteI2C_Byte(0x13,0x00);
	HDMI_WriteI2C_Byte(0x14,0x00);
	HDMI_WriteI2C_Byte(0x15,0x00);
	HDMI_WriteI2C_Byte(0x16,0x00);
	HDMI_WriteI2C_Byte(0x18,0x00);

	HDMI_WriteI2C_Byte(0x10,0xfe);
	HDMI_WriteI2C_Byte(0x11,0xf8);
	HDMI_WriteI2C_Byte(0x12,0xff);
	HDMI_WriteI2C_Byte(0x13,0xf9);
	HDMI_WriteI2C_Byte(0x14,0xff);
	HDMI_WriteI2C_Byte(0x15,0xf9);
	HDMI_WriteI2C_Byte(0x16,0xf3);
	HDMI_WriteI2C_Byte(0x18,0xfc);
}

void LT8618SX_Init(BYTE VideoFmt)
{
	I2CADR = LT8618SX_ADR;
	HDMI_WriteI2C_Byte(0xFF,0x80);
	HDMI_WriteI2C_Byte(0xee,0x01);

	LT8618SX_Reset();
	LT8618SX_Chip_ID();

	LT8618SX_RST_PD_Init();

	LT8618SX_TTL_Input_Analog();
	LT8618SX_TTL_Input_Digtal();
//	usleep(1000000);				//Wait 1000ms
	WaitXms(1000);

	LT8618SX_PLL();
	LT8618SX_Audio_Init();
	LT8618SX_CSC();
	LT8618SX_HDMI_TX_Phy();

	LT8618SX_BT_Set(VideoFmt);
	LT8618SX_HDMI_TX_Digital();

//	xil_printf("LT8618SX Initial End...\n\r");
	_printf("LT8618SX Initial End...\n\r");
}

void HDMI_TwiWrite(BYTE abDevaddr, BYTE abAddr, BYTE abData)
{

	while(I2cWrite(HDMI_I2C_CH,abDevaddr,0,0));// Avg. 47 us (I2C_CLK = 290KHz)

	I2cWrite(HDMI_I2C_CH,abAddr   , 0,0);
	I2cWrite(HDMI_I2C_CH,abData   , 1,0);
	return;

}

void HDMI_TwiRead(BYTE abDevaddr, BYTE abAddr, BYTE* apbBuf)
{
	BYTE bData;

	while(I2cWrite(HDMI_I2C_CH,abDevaddr,0,0));

	I2cWrite(HDMI_I2C_CH,abAddr, 0,1);
	I2cWrite(HDMI_I2C_CH,abDevaddr | 0x1, 0,0);

	bData = I2cRead(HDMI_I2C_CH,1,0);
	apbBuf[0] = bData;

	return;
}

#endif

