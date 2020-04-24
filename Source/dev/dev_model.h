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

//	Sensor Setting--------------------------------------------------------------
#define model_Sens		SENS_IMX415	// Sensor selection
//#define model_Sens		SENS_OS08A10	// Sensor selection
//#define model_Sens		SENS_IMX291	// Sensor selection
//#define model_Sens		SENS_IMX323	// Sensor selection

#define model_Sens_Ctrl		1		// 0 : SPI, supported Sony Sensor Only
									// 1 : TWI, supported All Sensor

//#define model_Sens_Intf		0
#define model_Sens_Intf		2		// 0 : Parallel, supported IMX225, IMX323
									// 1 : LVDS, supported OS08A10, IMX291, OV2718,
									// 2 : MIPI, supported OV4689, IMX335, IMX274, IMX225, IMX415

#define model_Sens_Fps		15
//#define model_Sens_Fps		30		// 60 fps : supported IMX225
									// 30 fps : supported IMX335, OV4689, OV2718, IMX291, IMX323
									// 20 fps : supported IMX335
									// 15 fps : supported IMX274, OV4689, OS08A10, IMX415
									// 12 fps : =12.5fps, supported IMX335

//-------------------------------------------------------------------------------------------------
#define model_Sony			((model_Sens==SENS_IMX415)||(model_Sens==SENS_IMX274)||(model_Sens==SENS_IMX334)||(model_Sens==SENS_IMX335)||(model_Sens==SENS_IMX225)||(model_Sens==SENS_IMX291)||(model_Sens==SENS_IMX327)||(model_Sens==SENS_IMX307)||(model_Sens==SENS_IMX323))
#define model_Omni			((model_Sens==SENS_OS08A10)||(model_Sens==SENS_OV4689)||(model_Sens==SENS_OV2718))
#define model_Aptn			((model_Sens==SENS_AR0237CSH))
#define model_Pana			((model_Sens==SENS_MN34210)||(model_Sens==SENS_MN34229)||(model_Sens==SENS_MN34227)||(model_Sens==SENS_MN34420))

#define model_Mipi			((model_Sens==SENS_IMX415)||(model_Sens==SENS_OV4689)||(model_Sens==SENS_IMX335)||(model_Sens==SENS_IMX274)||(model_Sens==SENS_IMX225))
#define model_Lvds			((model_Sens==SENS_OS08A10)||(model_Sens==SENS_IMX291)||(model_Sens==SENS_OV2718))
#define model_Parallel		((model_Sens==SENS_IMX225)||(model_Sens==SENS_IMX323))

#define model_60fps			((model_Sens==SENS_IMX225))
#define model_30fps			((model_Sens==SENS_IMX335)||(model_Sens==SENS_OV4689)||(model_Sens==SENS_OV2718)||(model_Sens==SENS_IMX291)||(model_Sens==SENS_IMX323))
#define model_20fps			((model_Sens==SENS_IMX335))
#define model_15fps			((model_Sens==SENS_IMX415)||(model_Sens==SENS_IMX274)||(model_Sens==SENS_OV4689)||(model_Sens==SENS_OS08A10))
#define model_12fps			((model_Sens==SENS_IMX335))

//-------------------------------------------------------------------------------------------------
//	Display
// HDMI는 device.cmake 에서 설정함
// HDMI 제어 시 I2C 설정은 peripheral.cmake 에서 USE_I2C7 3, I2C7_SPEED 400000 로 설정

//	DDR
#define			USE_FRC				1

//	Sensor Control Setting------------------------------------------------------
// Sensor 제어 시 I2C 설정은 peripheral.cmake 에서 USE_I2C0 3, I2C0_SPEED 400000 로 설정
// Sensor 제어 시 SPI 설정은 peripheral.cmake 에서 USE_SPI0 2, SPI0_SPEED 1500000, SPI0_BIT 24, SPI0_LSB 1 로 설정


//	External Digital Input------------------------------------------------------
//#define			USE_DIG_CH2		1						//	BT1120 YC Multiplexed
//	#define			USE_WRCH2_DS1_RDCH2_PIP0		1
//#define			USE_DIG_CH3		1						//	BT1120 YC Multiplexed
//	#define			USE_WRCH3_DS2_RDCH3_PIP1		1

//	ISP Function----------------------------------------------------------------
#define			USE_DNR3D			1		// TODO KSH ◆ 4K 시 USE_DNR3D 아직 사용 안됨

//	Jpeg Test-------------------------------------------------------------------
//#define			JPG_ENCODING			1
//#define			JPG_DECODING			1
//	#define			JPG_DECODING_DSCALE		1

//	CVBS------------------------------------------------------------------------
//	#define			CVBS_VLC_6BIT		1
//	#define			CVBS_VLC_5BIT		1
//	#define			CVBS_VLC_4BIT		1

//	LCD Type--------------------------------------------------------------------
//#define				LCD_INTERFACE			1
//	#define				FR_TFT035_MCU_PARALLEL_18BIT_MODE		1
//	#define				FR_TFT035_MCU_PARALLEL_16BIT_MODE		1
//	#define				FR_TFT035_RGB_16BIT_MODE		1
//	#define				FR_TFT035_SPI_4LANE_18BIT_MODE		1


// Board Evaluation Setting for ISP --------------------------------------------------------------

#define model_TgtBd			2		// Target board
									// 0 : KU Bd, 1 : KU+VU Bd, 2 : VU Bd(CPU0:RTOS, CPU1:ISP+CODEC), 3 : customer

//#define model_Lcd			2		// 0 : None
									// 1 : TFT035
									// 2 : TFT028
									// 3 : TFT023

#define model_Save			0		// 0 : save Parameter value to External EEPROM by I2C(TWI)
									//*1 : save Parameter value to internal Flash (default)
									// 2 : None save

#define model_Key			2		//*0 : KEY input by ADC
									// 1 : KEY input by GPIO
									// 2 : no physical key (UART Key only)

#define model_Lens			0		// Lens type (+MENU)
									//*0 : normal, 1 : Motorized AF

#define model_MAF_Lens		0		// Motorized AF Lens model
									//*0 : YT2812, 1 : YT3013, 2 : YT3017, 3 : YT30022, 4 : YT3021, 5~9 : Cstom Lens (Needed Tuning)

#define model_Iris			0		// IRIS control by
									// 0: (no use), 1: KIWI driver(DC), *2: MPWM(DC), 3: Step motor(pIris) 4: AF drvIC (PID)

#define model_Tdn			2		// 0 : TDN control by 1 port 2 state (H, L)
									// 1 : TDN control by 1 port 3 state (H, L, Hi-z)
									//*2 : TDN control by 2 ports

#define model_Cds			0		//*0 : CDS input by ADC
									// 1 : CDS input by GPIO

#define model_Led			2		// 0 : IR-LED control by LPWM
									//*1 : IR-LED control by GPIO
									// 2 : no IR-LED

#define model_MotionOut		0		//*0 : GPIO Motion Alarm Disable
									// 1 : GPIO Motion Alarm Enable


// Special Functions ------------------------------------------------------

#define model_ParID		0x12345678	// Enable parameter ID for control with ECM
#define model_HexID		0x1234		// Hex file ID of parameter for download with ECM


// Options --------------------------------------------------------------
#define model_CharMax		1		// 0 : Font char max ea - 256 (BYTE only)
									//*1 : Font char max ea - ISP_FONT_CHAR_EA (BYTE & WORD)

#define model_I2cSlave		0		//*0 : I2C operate to Master mode (default)


#define model_WDR_ROI		0		// 0 : no use (+MENU)
									// 1 : use (+MENU)


// --------------------------------------------------------------
#define model_Iris_DC		(/*(model_Iris==0)||*/(model_Iris==1)||(model_Iris==2))
#define model_Iris_P		((model_Iris==3)||(model_Iris==4))

#define model_1M			((model_Sens>= 0)&&(model_Sens<10))
#define model_2M			((model_Sens>=10)&&(model_Sens<20))	// EN781 no used
#define model_2M30p			((model_Sens>=20)&&(model_Sens<30))	// EN781 2M only
#define model_4M			((model_Sens>=40)&&(model_Sens<50))
#define model_8M			((model_Sens>=80)&&(model_Sens<90))


// --------------------------------------------------------------
#if (model_Sens_Intf!=0)&&(model_Sens_Intf!=1)&&(model_Sens_Intf!=2)
	#error "model_Sens_Intf" incorrect setting
#elif (model_Sens_Intf==0) && (model_Parallel==0)
	#error "model_Sens_Intf 0" is not supported.
#elif (model_Sens_Intf==1) && (model_Lvds==0)
	#error "model_Sens_Intf 1" is not supported.
#elif (model_Sens_Intf==2) && (model_Mipi==0)
	#error "model_Sens_Intf 2" is not supported.
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

