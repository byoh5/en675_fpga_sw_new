
//	Font
#define			USE_FONT			1
#define			USE_LOG				1

//	Display
// HDMI는 device.cmake 에서 설정함

//	DDR
#define			USE_FRC				1
	#define			DDR_TIMING		1
#define			DDR_ORIGINAL		1
//#define			DDR_CONTROL			1

//	Sync
#define			ISP_VLOCKO			1

//	Sensor Setting Interface	-----------------------------------------------
//#define			SENSOR_SPI				1
#define			SENSOR_I2C				1

//	1.3M	-------------------------------------------------------------------
//#define			IMX225_MIPI_4LANE_60FPS		1
//#define			IMX225_PARALLEL				1


//	2M	-----------------------------------------------------------------------
#define			IMX291			1
	#define		IMX291_30FPS	1
//	#define		IMX291_60FPS	1
	#define		IMX291_I2C		1
//	#define		IMX291_SPI		1

//#define		OV2718_LVDS_4CH			1

//	4M	-----------------------------------------------------------------------
//#define			OV4689_MIPI_30FPS		1
//#define			OV4689_MIPI_15FPS		1

//#define			IMX335_MIPI_4LANE			1
////		#define		IMX335_4M_30P			1
////		#define		IMX335_5M_20P			1
//		#define		IMX335_5M_12_5P			1

//	4K	-----------------------------------------------------------------------
//#define			IMX274_4K_MIPI_15P		1
//#define			OV08A10_LVDS_15P		1



//	External Digital Input------------------------------------------------------
//#define			USE_DIG_CH2		1						//	BT1120 YC Multiplexed
//	#define			USE_WRCH2_DS1_RDCH2_PIP0		1
//#define			USE_DIG_CH3		1						//	BT1120 YC Multiplexed
//	#define			USE_WRCH3_DS2_RDCH3_PIP1		1

//	ISP Function----------------------------------------------------------------
//#define				USE_DNR3D			1


//	Jpeg Test-------------------------------------------------------------------
//#define			JPG_ENCODING			1
//#define			JPG_DECODING			1
//	#define			JPG_DECODING_DSCALE		1

//	CVBS------------------------------------------------------------------------
//#define			CVBS_OUTPUT			1
//	#define			CVBS_VLC_6BIT		1
//	#define			CVBS_VLC_5BIT		1
//	#define			CVBS_VLC_4BIT		1

//	LCD Type--------------------------------------------------------------------
//#define				LCD_INTERFACE			1
//	#define				FR_TFT035_MCU_PARALLEL_18BIT_MODE		1
//	#define				FR_TFT035_MCU_PARALLEL_16BIT_MODE		1
//	#define				FR_TFT035_RGB_16BIT_MODE		1
//	#define				FR_TFT035_SPI_4LANE_18BIT_MODE		1


//	I2C Address-----------------------------------------------------------------
#define	SENS_SONY_DEV		0x34
//#define	SENS_SONY_DEV		0x20
#define SENS_OV_DEV			0x6C			//	GPIO1 is LOW
//#define SENS_OV2718_DEV		0x20			//	GPIO1 is High

// for SPI
#define SENS_SONY_ID2		0x2
#define SENS_SONY_ID3		0x3
#define SENS_SONY_ID4		0x4
#define SENS_SONY_ID5		0x5
#define SENS_SONY_ID6		0x6

// for TWI
#define SENS_SONY_ID2_TWI	0x30
#define SENS_SONY_ID3_TWI	0x31
#define SENS_SONY_ID4_TWI	0x32
#define SENS_SONY_ID5_TWI	0x33
#define SENS_SONY_ID6_TWI	0x34

//	HDMI
#define HDMI_ADDR 			0x7A


//*************************************************************************************************
// Software version info & fixed define
//-------------------------------------------------------------------------------------------------

	#define 	model_ver			0x6500		// Eyenix base code version (Fixed)

	#define 	SENS_IMX291				24		// Sony	2M 60p, LVDS Mode


// Board Evaluation Setting for ISP --------------------------------------------------------------
#define model_Sens		SENS_IMX291	// Sensor selection


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

#define model_Led			1		// 0 : IR-LED control by LPWM
									//*1 : IR-LED control by GPIO

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





