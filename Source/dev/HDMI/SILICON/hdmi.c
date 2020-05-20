/* **************************************************************************
 File Name	: 	en331.c
 Description	:	PC interface through uart
 Designer		: 	Park, Jun-Young.
 Date		:	14.11.5
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

//#include "../cpu_main.h"

#include "dev.h"
//#include "enx_dev.h"

#ifdef __HDMI_SILICON__

#include "defs.h"
#include "TypeDefs.h"
//#include "Initializations.h"
//#include "SerialPort.h"
//#include "util.h"
#include "TPI.h"
#include "HDCP.h"
#include "i2c_master_sw.h"
#include "si_apiConfigure.h"
#include "Constants.h"
#include "Externals.h"

#define HDMI_INFO_SET(_VIC, VFM){	VideoModeDescription.VIC = _VIC;\
									VideoModeDescription.HDMIVideoFormat = VFM;}


SYSTEM_STATE state;
WORD R_Monitor;
WORD T_Monitor;
BYTE gbVideoChg = 0;

void VideoModeChg(BYTE *abDetFrm)
{
//	UINT nRegReadBuf=0;
	static WORD rHWIDTH, rVTOTAL;
//	static BYTE rEXD_INTERLACE;
	static BYTE ModeBuf = 0xee;
	static BYTE ModeNum = 0x6;
	static BYTE gPostClk = 0;

//	_printf("--------HDMI Function START-------",0,0);

//	_printf("ModeNum : ",ModeNum);
//	_printf("ModeBuf : ",ModeBuf);

//	ENX_I2Cr(DevAddr,0x203,&nRegReadBuf);	//ch A				//	EN674 HTWO, VTWO
//	nRegReadBuf = GetIsp();
	rHWIDTH = HTWOr;
	rVTOTAL = VTWOr;
	gPostClk = PPCK_SELr;

	//_printf_irq("HDMI:HTWO=%d,VTWO=%d\r\n", rHWIDTH, rVTOTAL);

//	ENX_I2Cr(DevAddr,0x325,&nRegReadBuf);	//ch A
//	rEXD_HTC =((nRegReadBuf>>16)&0x3fff);						//	EN674 HTWO

//	ENX_I2Cr(DevAddr,0x328,&nRegReadBuf);	//ch A
//	rEXD_INTERLACE = ((nRegReadBuf>>31)&0x1);
//	rEXD_INTERLACE	=	0;										//	Always Progressive

//	_printf("ModeBufxx : ",ModeBuf);

	if((rHWIDTH==0xF76)&&(rVTOTAL==0x2ED)&&(gPostClk==2))			{ ModeNum	=	1;	/*	_printf("Write: 1",0,0);	*/	}	//720P25
	else if((rHWIDTH==0xCE2)&&(rVTOTAL==0x2ED)&&(gPostClk==2))		{ ModeNum	=	2;  /*	_printf("Write: 2",0,0);   	*/	} 	//720P30
	else if((rHWIDTH==0x7BA)&&(rVTOTAL==0x2ED)&&(gPostClk==2))		{ ModeNum	=	3;  /*	_printf("Write: 3",0,0);   	*/	} 	//720P50
	else if((rHWIDTH==0x670)&&(rVTOTAL==0x2ED)&&(gPostClk==2))		{ ModeNum	=	4;  /*	_printf("Write: 4",0,0);   	*/	} 	//720P60
	else if((rHWIDTH==0xA4E)&&(rVTOTAL==0x464)&&(gPostClk==2))		{ ModeNum	=	5;  /*	_printf("Write: 5",0,0);   	*/	} 	//1080P25
	else if((rHWIDTH==0x896)&&(rVTOTAL==0x464)&&(gPostClk==2))		{ ModeNum	=	6;  /*	_printf("Write 1080P30");  	*/	} 	//1080P30
	else if((rHWIDTH==0xA4E)&&(rVTOTAL==0x464)&&(gPostClk==1))		{ ModeNum	=	9;  /*	_printf("Write: 9",0,0);   	*/	} 	//1080P50
	else if((rHWIDTH==0x896)&&(rVTOTAL==0x464)&&(gPostClk==1))		{ ModeNum	=	10; /*	_printf("Write: 10",0,0);   */	} 	//1080P60
	else if((rHWIDTH==0xF76)&&(gPostClk==1))						{ ModeNum	=	11; /*	_printf("Write: 11",0,0);   */	} 	//EX-SDI 1440P25
	else if((rHWIDTH==0xCE2)&&(gPostClk==1))						{ ModeNum	=	12; /*	_printf("Write: 12",0,0);   */	} 	//EX-SDI 1440P30
	else															{ ModeNum	=	10; /*	_printf("Write: Last",0,0);	*/	}

	//	0xce2, 0x5db -> 4M Total Count
//	_printf("ModeBuf1 : ",ModeBuf);

//	ModeNum = ((rHWIDTH==0xF76 && rVTOTAL==0x2ED)) ? 1 :							//720P25
//			  ((rHWIDTH==0xCE2 && rVTOTAL==0x2ED)) ? 2 :							//720P30
//			  ((rHWIDTH==0x7BA && rVTOTAL==0x2ED)) ? 3 :							//720P50
//			  ((rHWIDTH==0x670 && rVTOTAL==0x2ED)) ? 4 :							//720P60
//			  ((rHWIDTH==0xA4E && rVTOTAL==0x464)) ? 5 :							//1080P25
//			  ((rHWIDTH==0x896 && rVTOTAL==0x464)) ? 6 :							//1080P30
//			  ((rHWIDTH==0xA4E && ((rVTOTAL>=0x232)&&(rVTOTAL<=0x233)))) ? 7 :	    //1080i50
//			  ((rHWIDTH==0x896 && ((rVTOTAL>=0x232)&&(rVTOTAL<=0x233)))) ? 8 :		//1080i60
//			  ((rHWIDTH==0xA4E && rVTOTAL==0x464))	? 9 :			                //1080P50
//			  ((rHWIDTH==0x896 && rVTOTAL==0x464))	? 10:			                //1080P60
//			  (rEXD_HTC==0xF76)		  				? 11:			                //EX-SDI 1440P25
//			  (rEXD_HTC==0xCE2)						? 12:10;		                //EX-SDI 1440P30

//	_printf("ModeNum : ",ModeNum);
//	_printf("ModeBuf : ",ModeBuf);
//	_printf("  ",0,0);

	if(ModeNum != ModeBuf){

		gbVideoChg = TRUE;

		switch(ModeNum){
			case 0 : HDMI_INFO_SET(63, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("      ",0,0);			*/	break;
			case 1 : HDMI_INFO_SET(19, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("720P25",0,0);			*/	break;	//720P25
			case 2 : HDMI_INFO_SET( 4, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("720P30",0,0);			*/	break;	//720P30
			case 3 : HDMI_INFO_SET(19, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("720P50",0,0);			*/	break;	//720P50
			case 4 : HDMI_INFO_SET( 4, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("720P60",0,0);			*/	break;	//720P60
			case 5 : HDMI_INFO_SET(33, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("1080P25",0,0);			*/	break;	//1080P25
			case 6 : HDMI_INFO_SET(34, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("1080P30");				*/	break;	//1080P30
			case 7 : HDMI_INFO_SET(20, VMD_HDMIFORMAT_3D);			/* _printf("1080i50",0,0);			*/	break;	//1080i50
			case 8 : HDMI_INFO_SET( 5, VMD_HDMIFORMAT_3D);			/* _printf("1080i60",0,0);			*/	break;	//1080i60
			case 9 : HDMI_INFO_SET(31, VMD_HDMIFORMAT_3D);			/* _printf("1080P50",0,0);			*/	break;	//1080P50
			case 10: HDMI_INFO_SET(16, VMD_HDMIFORMAT_3D);			/* _printf("1080P60");				*/	break;	//1080P60
			case 11: HDMI_INFO_SET(63, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("EX-SDI 1440P25",0,0);	*/	break;	//EX-SDI 1440P25
			case 12: HDMI_INFO_SET(63, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("EX-SDI 1440P30",0,0);	*/	break;	//EX-SDI 1440P30
			case 13: HDMI_INFO_SET(60, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("EX-SDI 2160P25",0,0);	*/	break;	//EX-SDI 2160P25
			case 14: HDMI_INFO_SET(60, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("EX-SDI 2160P30",0,0);	*/	break;	//EX-SDI 2160P30
			default: HDMI_INFO_SET(63, VMD_HDMIFORMAT_HDMI_VIC);	/* _printf("LAST SETTING",0,0);		*/	break;
		}

		//FIX
		VideoModeDescription.AspectRatio = VMD_ASPECT_RATIO_16x9;
		VideoModeDescription.ColorSpace = VMD_COLOR_SPACE_YCBCR422;
		VideoModeDescription.ColorDepth = VMD_COLOR_DEPTH_8BIT;
		VideoModeDescription.HDCPAuthenticated = VMD_HDCP_NOT_AUTHENTICATED;
		VideoModeDescription.ThreeDStructure = 0;

//		_printf("InitVideo: V: ", (int) VideoModeDescription.VIC);
//		_printf(" A: ", (int) VideoModeDescription.AspectRatio);
//		_printf(" CS: ", (int) VideoModeDescription.ColorSpace);
//		_printf(" CD: ", (int) VideoModeDescription.ColorDepth);
//		_printf(" HA: ", (int) VideoModeDescription.HDCPAuthenticated);
//		_printf(" HF: ", (int) VideoModeDescription.HDMIVideoFormat);
//		_printf(" 3D: ", (int) VideoModeDescription.ThreeDStructure);
	}

	ModeBuf = ModeNum;
}

void InitHdmi(void)
{
	_printf("-----HDMI Init Start-----\r\n",0,0);
	_printf("Modify Eyenix\r\n",0,0);

	TPI_Init();

	state = SS_RX_NOT_READY;
	gbVideoChg = TRUE;

	R_Monitor = R_INIT_PERIOD;
	T_Monitor = T_MONITORING_PERIOD;	// TPI (TX) polling timer interval

	_printf("-----HDMI Init End-----\r\n",0,0);
}

void Hdmi(BYTE *abDetFrm)
{
	if ((state > SS_FATAL_ERROR) && !R_Monitor)
	{
		state = SS_RX_NEW_VIDEO_MODE;

		//Get Video Information
		VideoModeChg(abDetFrm);

		R_Monitor = R_MONITORING_PERIOD;
		T_Monitor = 0;
	}
	else
	{
		R_Monitor--;
	}


	if ((state > SS_RX_NOT_READY) && !T_Monitor)
    {
    	if(state == SS_RX_NEW_VIDEO_MODE)
    	{
//    		gbVideoChg = TRUE;
	        TPI_Poll();

			state = SS_TX_READY;
        	T_Monitor = T_MONITORING_PERIOD;
    	}
		else if(state == SS_TX_READY)
		{
			TPI_Poll();
			T_Monitor = T_MONITORING_PERIOD;
		}
    }
    else
	{
        T_Monitor--;
	}
}

//	HDMI I2C	------------------------------------------------------
void HDMI_TwiWrite(BYTE abDevaddr, BYTE abAddr, BYTE abData)
{

	while(I2cWrite(HDMI_I2C_CH,abDevaddr,0,0));// Avg. 47 us (I2C_CLK = 290KHz)

	I2cWrite(HDMI_I2C_CH,abAddr   , 0,0);
	I2cWrite(HDMI_I2C_CH,abData   , 1,0);
	return;

}

void HDMI_TwiWriteBlock(BYTE abDevaddr, BYTE abAddr, BYTE* apbBuf, UINT anBufEa)
{
	while(I2cWrite(HDMI_I2C_CH,abDevaddr,0,0));

	I2cWrite(HDMI_I2C_CH,(abAddr&0xff), 0,0);

	UINT i;
	for(i=1; i<anBufEa; i++)
	{
		I2cWrite(HDMI_I2C_CH,*apbBuf, 0,0);
		apbBuf++;
	}

	I2cWrite(HDMI_I2C_CH,*apbBuf, 1,0);

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

void HDMI_TwiReadBlock(BYTE abDevaddr, BYTE abAadr, BYTE* abData, UINT anDataEa)
{
	while(I2cWrite(HDMI_I2C_CH,abDevaddr,0,0));

	I2cWrite(HDMI_I2C_CH,(abAadr&0xff), 0,1);
	I2cWrite(HDMI_I2C_CH,abDevaddr | 0x1, 0,0);

	UINT i;
	for(i=1; i<anDataEa; i++)
	{
		*abData = I2cRead(HDMI_I2C_CH,0,0);
		abData++;
	}
	*abData = I2cRead(HDMI_I2C_CH,1,0);

	return;
}

void HDMI_TwiReadSegmentBlock(BYTE abDevaddr, BYTE segment, BYTE abAddr, BYTE* apbBuf, UINT anBufEa)
{	// abDevaddr : Base of DEVICE ADDRESS
	// apbWaddr : Word Address
	// abLen : length of Word Address (1 or 2)
	// apbBuf : read buffer
	// anBufEa : length of buffer

	while(I2cWrite(HDMI_I2C_CH,EDID_SEG_ADDR,0,0));

	I2cWrite(HDMI_I2C_CH,(segment&0xff), 0,0);
	I2cWrite(HDMI_I2C_CH,(abDevaddr&0xff), 0,0);
	I2cWrite(HDMI_I2C_CH,(abAddr&0xff), 0,1);
	I2cWrite(HDMI_I2C_CH,abDevaddr | 0x1, 0,0);

	UINT i;
	for(i=1; i<anBufEa; i++)
	{
		*apbBuf = I2cRead(HDMI_I2C_CH,0,0);
		apbBuf++;
	}
	*apbBuf = I2cRead(HDMI_I2C_CH,1,0);

	return;
}

#endif

