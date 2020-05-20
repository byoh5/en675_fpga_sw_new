//*************************************************************************************************
// Software version info & fixed define
//-------------------------------------------------------------------------------------------------

	#define 	model_ver			0x6500		// Eyenix base code version (Fixed)

//	#define 	SENS_MN34210			1		// Pana 1.3M 60P(also MN34219), LVDS Mode - TBD
	#define 	SENS_IMX225				4		// Sony	1.3M 60P(also IMX224 ), LVDS Mode - TBD

//	#define 	SENS_MN34229			11		// Pana 2M 60p, LVDS Mode
//	#define 	SENS_MN34420			12		// Pana 2M 60p, LVDS Mode
//	#define 	SENS_IMX307				13		// Sony	2M 60p, LVDS Mode
//	#define 	SENS_IMX327				14		// Sony	2M 60p, LVDS Mode
	#define 	SENS_IMX291				15		// Sony	2M 60p, LVDS Mode
//	#define 	SENS_AR0237CSH 			18 		// Aptina 2M 60p, HiSpi Mode (PLCC Type)

//	#define 	SENS_MN34227			21		// Pana	2M 30p(also MN34220), LVDS Mode
	#define 	SENS_OV2718				22		// Omni	2M 30p, Parallel Mode
//	#define 	SENS_IMX122 			23		// Sony 2M 30p(also IMX222/IMX322), Parallel Mode - TBD
	#define 	SENS_IMX323				24		// Sony	2M 30p, Parallel Mode
//	#define 	SENS_AR0237CSP			25		// Aptina 2M 30p, Parallel Mode(PLCC Type) - TBD

	#define		SENS_IMX335				40		// Sony 4M 30p
//	#define		SENS_OS05A10			45		// Omni 4M 30p
	#define		SENS_OV4689				46		// Omni 4M 30p

//	#define		SENS_IMX334				80		// Sony 8M 30p
	#define		SENS_IMX274				81		// Sony 8M 30p
	#define		SENS_IMX415				82		// Sony 8M 30p
	#define		SENS_OS08A10			85		// Omni 8M 30p


#ifndef __SENS_LIST__

//-------------------------------------------------------------------------------------------------
#define model_Sony			((model_Sens==SENS_IMX415)||(model_Sens==SENS_IMX274)||(model_Sens==SENS_IMX334)||(model_Sens==SENS_IMX335)||(model_Sens==SENS_IMX225)||(model_Sens==SENS_IMX291)||(model_Sens==SENS_IMX327)||(model_Sens==SENS_IMX307)||(model_Sens==SENS_IMX323))
#define model_Omni			((model_Sens==SENS_OS08A10)||(model_Sens==SENS_OV4689)||(model_Sens==SENS_OV2718))
#define model_Aptn			((model_Sens==SENS_AR0237CSH))
#define model_Pana			((model_Sens==SENS_MN34210)||(model_Sens==SENS_MN34229)||(model_Sens==SENS_MN34227)||(model_Sens==SENS_MN34420))

#define model_Mipi			((model_Sens==SENS_IMX415)||(model_Sens==SENS_OV4689)||(model_Sens==SENS_IMX335)||(model_Sens==SENS_IMX274)||(model_Sens==SENS_IMX225))
#define model_Parallel		((model_Sens==SENS_IMX225)||(model_Sens==SENS_IMX323))

#define model_60fps			((model_Sens==SENS_IMX225))
#define model_30fps			((model_Sens==SENS_IMX335)||(model_Sens==SENS_OV4689)||(model_Sens==SENS_OV2718)||(model_Sens==SENS_IMX291)||(model_Sens==SENS_IMX323))
#define model_20fps			((model_Sens==SENS_IMX335))
#define model_15fps			((model_Sens==SENS_IMX415)||(model_Sens==SENS_IMX274)||(model_Sens==SENS_OV4689)||(model_Sens==SENS_OS08A10))
#define model_12fps			((model_Sens==SENS_IMX335))

// Board Evaluation Setting for ISP ---------------------------------------------------------------

#define model_CharMax		1		// 0 : Font char max ea - 256 (BYTE only)
									//*1 : Font char max ea - ISP_FONT_CHAR_EA (BYTE & WORD)

#define model_I2cSlave		0		//*0 : I2C operate to Master mode (default)


#define model_WDR_ROI		0		// 0 : no use (+MENU)
									// 1 : use (+MENU)

// Special Functions ------------------------------------------------------

#define model_ParID		0x12345678	// Enable parameter ID for control with ECM
#define model_HexID		0x1234		// Hex file ID of parameter for download with ECM


// --------------------------------------------------------------
#define model_Iris_DC		(/*(model_Iris==0)||*/(model_Iris==1)||(model_Iris==2))
#define model_Iris_P		((model_Iris==3)||(model_Iris==4))

#define model_1M			((model_Sens>= 0)&&(model_Sens<10))
#define model_2M			((model_Sens>=10)&&(model_Sens<20))	// EN781 no used
#define model_2M30p			((model_Sens>=20)&&(model_Sens<30))	// EN781 2M only
#define model_4M			((model_Sens>=40)&&(model_Sens<50))
#define model_8M			((model_Sens>=80)&&(model_Sens<90))


// --------------------------------------------------------------
#if (model_Sens_Intf!=0)&&(model_Sens_Intf!=1)
	#error "model_Sens_Intf" incorrect setting
#elif (model_Sens_Intf==0) && (model_Parallel==0)
	#error "model_Sens_Intf 0" is not supported.
#elif (model_Sens_Intf==1) && (model_Mipi==0)
	#error "model_Sens_Intf 1" is not supported.
#endif

#if (model_Sens_Fps!=60)&&(model_Sens_Fps!=30)&&(model_Sens_Fps!=20)&&(model_Sens_Fps!=15)&&(model_Sens_Fps!=12)
	#error "model_Sens_Fps" incorrect setting
#elif (model_Sens_Fps==60) && (model_60fps==0)
	#error "model_Sens_Fps 60" is not supported.
#elif (model_Sens_Fps==30) && (model_30fps==0)
	#error "model_Sens_Fps 30" is not supported.
#elif (model_Sens_Fps==20) && (model_20fps==0)
	#error "model_Sens_Fps 20" is not supported.
#elif (model_Sens_Fps==15) && (model_15fps==0)
	#error "model_Sens_Fps 15" is not supported.
#elif (model_Sens_Fps==12) && (model_12fps==0)
	#error "model_Sens_Fps 12" is not supported.
#endif


#endif

