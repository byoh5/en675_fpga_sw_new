#include "dev.h"

#ifdef __SENSOR__


#define	SENSOR_SONY_I2C_CA		0x34
//#define	SENSOR_SONY_I2C_CA		0x20

#if model_Sens==SENS_OV4689
	#define SENSOR_OMNI_I2C_CA		0x42
#else
	#define SENSOR_OMNI_I2C_CA		0x6C		//	GPIO1 is LOW
	//#define SENSOR_OMNI_I2C_CA		0x20		//	GPIO1 is High
#endif


void Isp_VLOCKO_init(void)
{
	VIRQO_EN_Tw(1);
	CLI_VLOCKO_Tw(1);
}

void Wait_VLOCKO(void)
{
	while(!(ISP_RIRQ_VOr&0x1));
	CLI_VLOCKO_Tw(1);
	return;
}

void Wait_VLOCKO_M(UINT Cnt)
{
	for(UINT i=0;i<Cnt;i++) Wait_VLOCKO();
}

void Isp_VLOCKO1_init(void)
{
	VIT1_IRQ_ENw(1);
	CLI_VLOCKO_IT1w(1);
}

void Wait_VLOCKO1(void)
{
	while(!(ISP_RIRQr&0x200000));
	CLI_VLOCKO_IT1w(1);
	return;
}

void ISRT0 SetSensTwi(BYTE abDevaddr, WORD awAddr, BYTE abData)
{	// Write to Sensor
#if (USE_I2C0==3) && (USE_SPI0==0)
	//if(gbSensorOff) return;

	SI2C_STA

	SI2C_WAIT(SENS_WRITE_NODEV1, I2cWrite(SENSOR_I2C_CH,abDevaddr,0,0))
	SI2C_CODE(SENS_WRITE_NOACK2, I2cWrite(SENSOR_I2C_CH,(awAddr>>8)&0xff, 0,0))
	SI2C_CODE(SENS_WRITE_NOACK2, I2cWrite(SENSOR_I2C_CH,(awAddr>>0)&0xff, 0,0))
	SI2C_CODE(SENS_WRITE_NOACK3, I2cWrite(SENSOR_I2C_CH,abData, 1,0))

	return;

	SI2C_END

#elif (model_Sens_Ctrl==1)
	#error if use TWI for sensor control, Please set "USE_SPI0 0, USE_I2C0 3, I2C0_SPEED 400000" in "peripheral.cmake"
#endif
}

BYTE ISRT0 GetSensTwi(BYTE abDevaddr, WORD awAddr)
{	// Read from Sensor
	//if(gbSensorOff) return SENS_READ_NODEV1;

	BYTE bData = 0;

	SI2C_STA

	SI2C_WAIT(SENS_READ_NODEV1, I2cWrite(SENSOR_I2C_CH,abDevaddr,0,0))
	SI2C_CODE(SENS_READ_NOACK1, I2cWrite(SENSOR_I2C_CH,(awAddr>>8)&0xff, 0,0))
	SI2C_CODE(SENS_READ_NOACK2, I2cWrite(SENSOR_I2C_CH,(awAddr>>0)&0xff, 0,1))

	SI2C_CODE(SENS_READ_NOACK3, I2cWrite(SENSOR_I2C_CH,abDevaddr|0x1,0,0))
	bData = I2cRead(SENSOR_I2C_CH,1,0);

	return bData;

	SI2C_END
}

#if model_Omni
void ISRT0 SetSens(WORD awAddr, BYTE abData)
{
  #if (model_Sens_Ctrl==1)
	SetSensTwi(SENSOR_OMNI_I2C_CA,awAddr,abData);
  #else
	#error Omni sensor "model_Sens_Ctrl is 1(TWI)" only
  #endif
}

BYTE ISRT0 GetSens(WORD awAddr)
{
  #if (model_Sens_Ctrl==1)
	return GetSensTwi(SENSOR_OMNI_I2C_CA,awAddr);
  #else
	return 0;
  #endif
}
#endif

#if model_Sony
void ISRT0 SetSensSpi_Sony(BYTE ID, BYTE Adr, BYTE Dat)
{
  #if (USE_SPI0==2) && (SPI0_BIT==24) && (USE_I2C0==0)
	BYTE SpiDat[] = {0,0,0,0};

	SpiDat[2] = ID;
	SpiDat[1] = Adr;
	SpiDat[0] = Dat;
	SpiSetCs(SENSOR_SPI_CH,0);
	SpiWrite(SENSOR_SPI_CH,SpiDat);
	SpiSetCs(SENSOR_SPI_CH,1);

  #elif (model_Sens_Ctrl==0)
	#error if use SPI for sensor control, Please set "USE_I2C0 0, USE_SPI0 2, SPI0_SPEED 1500000, SPI0_BIT 24, SPI0_LSB 1" in "peripheral.cmake"
  #endif
}

void ISRT0 SetSens(WORD awAddr, BYTE abData)
{
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

#if model_Sens_Ctrl == 1
	SetSensTwi(SENSOR_SONY_I2C_CA,awAddr,abData);
#else
	const BYTE bSpiIDt = (awAddr>>8)&0xff;
	const BYTE bSpiID = (SENS_SONY_ID2_TWI <= bSpiIDt && bSpiIDt <= SENS_SONY_ID6_TWI) ? (bSpiIDt - SENS_SONY_ID2_TWI) + SENS_SONY_ID2 : bSpiIDt;
	SetSensSpi_Sony(bSpiID, awAddr&0xff, abData);
#endif
}

BYTE ISRT0 GetSens(WORD awAddr)
{
#if model_Sens_Ctrl == 1
	return GetSensTwi(SENSOR_SONY_I2C_CA,awAddr);
#else
	return 0;
#endif
}
#endif

#if 0
#if model_Sens==SENS_OV2718
void OV2718_Init(void)
{
	IspSDesPowerOn();
	IspSensorPowerOn(SENS_13M);

	IspSDesConfig(0, 1);
	Isp_SDesDelay(7, 0, 0, 0, 0);
	IspSDesPosition();

	Isp_SYNC_CODE(0xfff, 0x000, 0x000, 0x000, 0x800, 0xab0, 0,0,0,0,0,0);

	InitSensRun();

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	IspPreSyncConfig(1, 0xe, 0, 0x13, 0x8, 1, 1, 0, 0);
	IspPostSyncConfig(3);

	ASYNC_DSYNCw(1);
}
#endif

#if model_Sens==SENS_IMX291
void IMX291_Init(void)
{
	IspSDesPowerOn();
#if	(model_Sens_Fps==60)
	IspSensorPowerOn(SENS_74M);			//	Sensor Clock
#else
	IspSensorPowerOn(SENS_37M);			//	Sensor Clock
#endif

	IspSDesConfig(1, 1);
	Isp_SDesDelay(2,0,0,0,0);
	IspSDesPosition();

	Isp_SYNC_CODE(0xfff, 0x000, 0x000, 0x000, 0x800, 0xab0, 0,0,0,0,0,0);

	InitSensRun();

#if	(model_Sens_Fps==60)
	Isp_PreClk_Config(ISP_CLK_148M);		//	Isp Pre Module Clock
#else
	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
#endif
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	IspPreSyncConfig(0, 0xa, 0xc, 0x15, 6, 0, 0, 0, 0);
	IspPostSyncConfig(1);
}
#endif

#if model_Sens==SENS_OS08A10
void OS08A10_Init(void)
{
	IspSDesPowerOn();
	IspSensorPowerOn(SENS_27M);				//	27MHz

	IspSDesConfig(1, 0);
	Isp_SDesDelay(2, 0, 0, 0, 0);
	IspSDesPosition();

	Isp_SYNC_CODE(0xfff, 0x000, 0x000, 0x000, 0x200, 0x280, 0,0,0,0,0,0);

	InitSensRun();

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	IspPreSyncConfig(1, 0xa, 0xc, 0x15, 6, 0, 0, 0, 1);
	IspPostSyncConfig(1);
}
#endif

#if model_Sens==SENS_IMX225
void IMX225_Init(void)
{
	IspSDesPowerOn(2);						// MipiClkPhase
	IspSensorPowerOn(SENS_27M);					//	Sensor Clock

	IspSDesConfig(MIPI_10BIT, 7, NO_USE_CHECK);
	Isp_SDesDelay(2, 0, 0, 0, 0);			//	20190503 : main_asic_61_ISP_R5.bit
	IspSDesPosition();

	Isp_SYNC_CODE(0, 1, 0x2, 0x3, 0xb8, 0xf30, 0, 0x2000, 0x2b, 0x3, 0, 3);	// 0x2b : Long Packet Line Start - Raw 10Bit

	InitSensRun();

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	IspPreSyncConfig(1, 0xb0, 0xc, 0x8, 4, 0, 1, 0, 1);
	IspPostSyncConfig(2);
}
#endif

#if	model_Sens==SENS_IMX335

void IMX335_Init(void)
{
	IspSDesPowerOn(2);						// MipiClkPhase
	IspSensorPowerOn(SENS_13M);					//	Sensor Clock

	IspSDesConfig(MIPI_12BIT, 7, NO_USE_CHECK);
	Isp_SDesDelay(2, 0, 0, 0, 0);			//	20190503 : main_asic_61_ISP_R5.bit
	IspSDesPosition();

	Isp_SYNC_CODE(0, 1, 0x2, 0x3, 0xb8, 0xf30, 0, 0x2000, 0x2c, 0x3, 0, 3);	// 0x2c : Long Packet Line Start - Raw 12Bit

	InitSensRun();

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	IspPreSyncConfig(1, 0xb0, 0xc, 0x8, 4, 1, 1, 0, 1);
	IspPostSyncConfig(2);
}

#endif

#if model_Sens==SENS_IMX274
void IMX274_Init()
{
	IspSDesPowerOn(0);						// MipiClkPhase
	IspSensorPowerOn(SENS_13M);					//	Sensor Clock	-	15p

	IspSDesConfig(MIPI_12BIT, 7, NO_USE_CHECK);
	Isp_SDesDelay(6, 0, 0, 0, 0);
	IspSDesPosition();

	Isp_SYNC_CODE(0, 1, 0x2, 0x3, 0xb8, 0xf30, 0, 0x2000, 0x2c, 0x3, 0, 3);	// 0x2c : Long Packet Line Start - Raw 12Bit

	InitSensRun();

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	IspPreSyncConfig(1, 0xa, 0xc, 0x15, 6, 0, 0, 0, 1);
	IspPostSyncConfig(1);
}
#endif

#if model_Sens==SENS_OV4689
void OV4689_Init(void)
{
	IspSDesPowerOn(2);						// MipiClkPhase
#if	(model_Sens_Fps==15)
	IspSensorPowerOn(SENS_13M);		// 13.5MHz
#elif (model_Sens_Fps==30)
	IspSensorPowerOn(SENS_27M);		// 27MHz
#endif

	IspSDesConfig(MIPI_10BIT, 8, USE_WCL_CHECK);
	Isp_SDesDelay(4, 0, 0, 0, 0);	//	20190503 : main_asic_crazy_ISP_R5_HDMI_DDR_Auto_Align
	IspSDesPosition();

	Isp_SYNC_CODE(0, 1, 0x2, 0x3, 0xb8, 0xd20, 0, 0x2000, 0x2b, 0x3, 0, 0xd20);

	InitSensRun();

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	IspPreSyncConfig(1, 0xb0, 0xc, 0x8, 4, 1, 1, 0, 1);
	IspPostSyncConfig(1);
}
#endif
#endif

//	LCD Function

void LCD_INSDAT_Write(UINT INSDAT0, UINT INSDAT1,UINT INSDAT2,UINT INSDAT3, UINT IStep)
{
	SetIsp(0x34c, INSDAT0);
	SetIsp(0x34d, INSDAT1);
	SetIsp(0x34e, INSDAT2);
	SetIsp(0x34f, INSDAT3);
	LCD0_INST_STEPw(IStep);

	SetIsp(0x034a, 0x80000000);					//	Write LCD Command

	WaitXus(400);

}

void LCD_FR_TFT035_6_Init(void)
{
	//	Xilinx Board


	//	LCD Reset
//	GpioFuncPin(7,0);				//	GPIO Function
//	GpioSetDir(7,0);
//	GpioSetOut(7,1);
//	WaitXus(10);
//	GpioSetOut(7,0);
//	WaitXus(10);
//	GpioSetOut(7,1);

	SYS_GPIO7_MUX	=	0;
	GPIO_PIN7_OEN	=	0;
	GPIO_PIN7_OUT	=	1;
	WaitXus(1000);
	GPIO_PIN7_OUT	=	0;
	WaitXus(1000);
	GPIO_PIN7_OUT	=	1;


	//	Read Channel Setting
	LCD0_VSELw(0xa);				//	Read Sync 2
	LCD0_DSELw(0x4);				//	Read Channel 4

	//	LCD Mode Setting

	//	Down -Scale
	CDS3_PCK_SET(2);
	CDS3_ISELw(1);
	CDS3_VISELw(0);
	CDS3_HWw(480);
	CDS3_VWw(320);
	DS3_DKXw(0x100);				//	1920 -> 480
	DS3_DKYw(0xd9);					//	1080 -> 320
	DS3_ONw(1);

	//	Write Channel
	YCW_CK4_SET(2);				//	PCLK - BT1120 16Bit Input
	IM_IVSEL4w(0);					//	BT Channel Main
	IM_ISEL4w(3);					//	Down-Scale 0
	IM_HWI4w(480);
	IM_WFRC4_ONw(1);
	IM_CGO4w(1);

	//	Read Channel
	IM_RVSEL4w(6);
	IM_RISEL4w(6);
	IM_RFRC4_ONw(1);
	IM_RHWI4w(480);
	DDR_RYC_LTC2w(0x100);
	IM_RON4w(1);
	YCR_CK4_SET(13);



	#ifdef		FR_TFT035_MCU_PARALLEL_16BIT_MODE		//	320 X 480
	DOMODEw(4);						//
	LCD0_PMODw(7);					//	MCU18Bit RGB 6-6-6 Interface
	PCK_EMAX2w(8);					//	PCK_DIV2
	PCK_EHVL2w(4);
	PCK_EDIV_PD2w(1);

	PCK_EMAX3w(8);					//	PCK_DIV3
	PCK_EHVL3w(4);
	PCK_EDIV_PD3w(1);

	RYC_OSYNC_MOD2w(1);
	RYC_HWI2w(0x1e0);
	RYC_VWI2w(0x140);
	RYC_HTWO2w(0x208);
	RYC_VTWO2w(0x1e0);
	RYC_HSPI2w(8);
	RYC_VSPI2w(8);

//	IM_FRC4_RLOCKONw(1);							//	For Low Latency Output
//	IM_FRC4_WLOCKONw(1);							//	For Low Latency Output
	IM_RCINV4w(0);

	//	Pin Mux
	VSO_OMODw(5);					//	LCD CSX
	HSO_OMODw(5);					//	LCD DCX
	DENO_OMODw(6);					//	Stuck '0'
	HVSO_CK_PDw(0);
	HVSO_CK_SELw(14);				//	PCK_DIV3
	HVSO_CK_PDw(1);

	//	Output Clock
	DO0_CK_PDw(0);
	DO1_CK_PDw(0);
	DO2_CK_PDw(0);
	DO3_CK_PDw(0);
	DO0_CK_SELw(14);				//	PCK_DIV3
	DO1_CK_SELw(14);
	DO2_CK_SELw(2);
	DO3_CK_SELw(2);
	DO0_CK_PDw(1);
	DO1_CK_PDw(1);
	DO2_CK_PDw(1);
	DO3_CK_PDw(1);
	DCKO2_PDw(0);
	DCKO2_SELw(13);					//	For Xilinx Board, PCK_EDIV3
	DCKO2_PDw(1);
	DCKO2_INVw(1);

	LCD0_INST_CONw(0);

	#elif		FR_TFT035_RGB_16BIT_MODE
	DOMODEw(4);						//
	LCD0_PMODw(2);					//	RGB 16Bit  RGB 5-6-5 Interface	1CK -> 1CK
	PCK_EMAX2w(0x8);				//	PCK_DIV2
	PCK_EHVL2w(0x4);
	PCK_EDIV_PD2w(1);

	PCK_EMAX3w(0x8);				//	PCK_DIV3
	PCK_EHVL3w(0x4);
	PCK_EDIV_PD3w(1);

	RYC_OSYNC_MOD2w(1);
	RYC_HWI2w(0x1e0);
	RYC_VWI2w(0x140);
	RYC_HTWO2w(0x208);
	RYC_VTWO2w(0x1e0);
	RYC_HSPI2w(6);
	RYC_VSPI2w(6);
	RYC_SHSO_FP2w(0x10);
	RYC_SHSO_RP2w(0x20);
	RYC_SVSO_FP2w(0x0);
	RYC_SVSO_RP2w(0x6);
	RYC_DENO_VSPI2w(0x8);
	RYC_DENO_VEDI2w(0x148);
	RYC_DENO_HRP2w(0x30);
	RYC_DENO_HFP2w(0x210);

	//	Pin Mux
	VSO_OMODw(3);					//	LCD CSX
	HSO_OMODw(3);					//	LCD DCX
	DENO_OMODw(3);					//	Stuck '0'
	HVSO_CK_PDw(0);
	HVSO_CK_SELw(14);				//	PCK_DIV3
	HVSO_CK_PDw(1);

	//	Output Clock
	DO0_CK_PDw(0);
	DO1_CK_PDw(0);
	DO2_CK_PDw(0);
	DO3_CK_PDw(0);
	DO0_CK_SELw(14);				//	PCK_DIV3
	DO1_CK_SELw(14);
	DO2_CK_SELw(2);
	DO3_CK_SELw(2);
	DO0_CK_PDw(1);
	DO1_CK_PDw(1);
	DO2_CK_PDw(1);
	DO3_CK_PDw(1);
	DCKO2_PDw(0);
	DCKO2_SELw(13);					//	For Xilinx Board, PCK_EDIV3
	DCKO2_PDw(1);
	DCKO2_INVw(1);

	LCD0_INST_CONw(0);

	#elif		FR_TFT035_SPI_4LANE_18BIT_MODE
	DOMODEw(0);						//
	LCD0_PMODw(0xb);				//	SPI 4 Wire 18Bit
	PCK_EMAX2w(0xd7);				//	PCK_DIV2
	PCK_EHVL2w(0x6b);
	PCK_EDIV_PD2w(1);

	PCK_EMAX3w(0x8);				//	PCK_DIV3		about 30 ns
	PCK_EHVL3w(0x4);
	PCK_EDIV_PD3w(1);

	RYC_OSYNC_MOD2w(1);
	RYC_HWI2w(0x1e0);
	RYC_VWI2w(0x140);
	RYC_HTWO2w(0x208);
	RYC_VTWO2w(0x1e0);
	RYC_HSPI2w(8);
	RYC_VSPI2w(8);

	IM_FRC2_RLOCKONw(1);							//	For Low Latency Output
	IM_FRC2_WLOCKONw(1);							//	For Low Latency Output
	IM_RCINV2w(0);								//

	//	Pin Mux
	VSO_OMODw(5);					//	LCD CSX
	HSO_OMODw(5);					//	LCD DCX
	DENO_OMODw(5);					//	LCD SDA
	HVSO_CK_PDw(0);
	HVSO_CK_SELw(14);				//	PCK_DIV3
	HVSO_CK_PDw(1);

	LCD0FF_RSw(1);								//	Fifo Reset
	LCD0_ICw(1);								//	Instrunction Mode On
	LCD0_CSXw(0);
	LCD0_DCXw(0);
	LCD0_DCX_SELw(1);

	//	Output Clock
	DO0_CK_PDw(0);
	DO1_CK_PDw(0);
	DO2_CK_PDw(0);
	DO3_CK_PDw(0);
	DO0_CK_SELw(14);				//	PCK_DIV3
	DO1_CK_SELw(14);
	DO2_CK_SELw(2);
	DO3_CK_SELw(2);
	DO0_CK_PDw(1);
	DO1_CK_PDw(1);
	DO2_CK_PDw(1);
	DO3_CK_PDw(1);
	DCKO2_PDw(0);
	DCKO2_SELw(13);					//	For Xilinx Board, PCK_EDIV3
	DCKO2_PDw(1);
	DCKO2_INVw(1);

	LCD0_INST_CONw(0);

	#endif

	//	Sync Gen Clock
	RYCCK2_PDw(0);
	RYCCK2_SELw(13);				//	PCK_DIV2
	RYCCK2_PDw(1);

	//	LCD IP Clock
	LCD_CK_PDw(0);
	LCD_CK_SELw(11);				//	PCK_DIV2
	LCD_CK_PDw(1);

	WaitXus(20);						//	LCD Fifo Clear Delay

	LCDO_CK_PDw(0);
	LCDO_CK_SELw(12);				//	PCK_DIV3
	LCDO_CK_PDw(1);

	//	LCD Output Clock & DCX, CSX Port Enable
	VSO_OENw(0);
	HSO_OENw(0);
	DENO_OENw(0);
	DO_OENw(0);

	DCKO2_OENw(0);


	#ifdef		FR_TFT035_RGB_16BIT_MODE
//	LCD0FF_RSw(1);								//	Fifo Reset
//	LCD0_ICw(1);								//	Instrunction Mode On
//	LCD0_CSXw(0);
//	LCD0_DCXw(0);
////	LCD0_DCX_SELw(1);
//
//	LCD_INSDAT_Write(0xE0000309,0x08160A3F,0x784C090A,0x08161A0F,16);
//	LCD_INSDAT_Write(0xE1001619,0x030F0532,0x4546040E,0x0D35370F,16);
//	LCD_INSDAT_Write(0xC0171500,0,0,0,3);
//	LCD_INSDAT_Write(0xC1410000,0,0,0,2);
//	LCD_INSDAT_Write(0xC5001280,0,0,0,4);
//	//	Memory Address Set	->	HV Exchange !!
//	LCD_INSDAT_Write(0x2A000001,0xDF000000,0,0,5);			//	SC, EC	->  480
//	LCD_INSDAT_Write(0x2B000001,0x3F000000,0,0,5);			//	SP, EP	->  320
////	LCD_INSDAT_Write(0x36480000,0,0,0,2);
//	LCD_INSDAT_Write(0x36280000,0,0,0,2);
//	LCD_INSDAT_Write(0x3A550000,0,0,0,2);
//	LCD_INSDAT_Write(0xB0000000,0,0,0,2);
//	LCD_INSDAT_Write(0xB1A00000,0,0,0,2);
//	LCD_INSDAT_Write(0xB4020000,0,0,0,2);
//	LCD_INSDAT_Write(0xB6020200,0,0,0,3);
//	LCD_INSDAT_Write(0xE9000000,0,0,0,2);
//	LCD_INSDAT_Write(0xF7A9512C,0x82000000,0,0,5);
//	LCD_INSDAT_Write(0x11000000,0,0,0,1);
//	WaitXus(24);
//	LCD_INSDAT_Write(0x29000000,0,0,0,1);
//	WaitXus(4);
//	LCD0_INSTPARA0w(0x2c);

	LCD0FF_RSw(1);								//	Fifo Reset
	LCD0_ICw(0);								//	Instrunction Mode On
	LCD0_CSXw(0);
	LCD0_DCXw(0);

	#endif


	#ifdef		FR_TFT035_SPI_4LANE_18BIT_MODE
	LCD0_SPI_LSBw(0);							//	MSB First
	LCD0FF_RSw(1);								//	Fifo Reset
	LCD0_ICw(1);								//	Instrunction Mode On
	LCD0_CSXw(0);
	LCD0_DCXw(0);
	LCD0_DCX_SELw(1);

	DCKO2_OENw(0);

	LCD_INSDAT_Write(0xE0000309,0x08160A3F,0x784C090A,0x08161A0F,16);
	LCD_INSDAT_Write(0xE1001619,0x030F0532,0x4546040E,0x0D35370F,16);
	LCD_INSDAT_Write(0xC0171500,0,0,0,3);
	LCD_INSDAT_Write(0xC1410000,0,0,0,2);
	LCD_INSDAT_Write(0xC5001200,0,0,0,4);
	//	Memory Address Set	->	HV Exchange !!
	LCD_INSDAT_Write(0x2A000001,0xDF000000,0,0,5);			//	SC, EC	->  480
	LCD_INSDAT_Write(0x2B000001,0x3F000000,0,0,5);			//	SP, EP	->  320
//	LCD_INSDAT_Write(0x36480000,0,0,0,2);					//	320X480
	LCD_INSDAT_Write(0x36280000,0,0,0,2);					//	480X320	->	MV = 1 !
	LCD_INSDAT_Write(0x3A660000,0,0,0,2);
	LCD_INSDAT_Write(0xB0000000,0,0,0,2);
	LCD_INSDAT_Write(0xB1A00000,0,0,0,2);
	LCD_INSDAT_Write(0xB4020000,0,0,0,2);
	LCD_INSDAT_Write(0xB6020200,0,0,0,3);
	LCD_INSDAT_Write(0xE9000000,0,0,0,2);
	LCD_INSDAT_Write(0xF7A9512C,0x82000000,0,0,5);
	LCD_INSDAT_Write(0x11000000,0,0,0,1);
	WaitXus(2400);
	LCD_INSDAT_Write(0x29000000,0,0,0,1);
	WaitXus(400);
	LCD0_INSTPARA0w(0x2c);

	#endif

	#ifdef		FR_TFT035_MCU_PARALLEL_16BIT_MODE
	LCD0FF_RSw(1);								//	Fifo Reset
	LCD0_ICw(1);								//	Instrunction Mode On
	LCD0_CSXw(0);
	LCD0_DCXw(0);
//	LCD0_DCX_SELw(1);

	LCD_INSDAT_Write(0xE0000309,0x08160A3F,0x784C090A,0x08161A0F,16);
	LCD_INSDAT_Write(0xE1001619,0x030F0532,0x4546040E,0x0D35370F,16);
	LCD_INSDAT_Write(0xC0171500,0,0,0,3);
	LCD_INSDAT_Write(0xC1410000,0,0,0,2);
	LCD_INSDAT_Write(0xC5001280,0,0,0,4);
	//	Memory Address Set	->	HV Exchange !!
	LCD_INSDAT_Write(0x2A000001,0xDF000000,0,0,5);			//	SC, EC	->  480
	LCD_INSDAT_Write(0x2B000001,0x3F000000,0,0,5);			//	SP, EP	->  320
//	LCD_INSDAT_Write(0x36480000,0,0,0,2);
	LCD_INSDAT_Write(0x36280000,0,0,0,2);
	LCD_INSDAT_Write(0x3A550000,0,0,0,2);
	LCD_INSDAT_Write(0xB0000000,0,0,0,2);
	LCD_INSDAT_Write(0xB1A00000,0,0,0,2);
	LCD_INSDAT_Write(0xB4020000,0,0,0,2);
	LCD_INSDAT_Write(0xB6020200,0,0,0,3);
	LCD_INSDAT_Write(0xE9000000,0,0,0,2);
	LCD_INSDAT_Write(0xF7A9512C,0x82000000,0,0,5);
	LCD_INSDAT_Write(0x11000000,0,0,0,1);
	WaitXus(2400);
	LCD_INSDAT_Write(0x29000000,0,0,0,1);
	WaitXus(400);
	LCD0_INSTPARA0w(0x2c);

	#endif

	WaitXus(1000);
	LCD0_ICw(0);								//	Instrunction Mode On
	LCD0_WIMGw(1);											//	Image Pumping Mode On

}


#if		JPG_ENCODING||JPG_DECODING
void JPG_Init(void)
{
	SYNC_UPw(1);									//	Sync_up
	JPG_HSELw(0);									//	Select ISP Post HLOCK
	JPG_HONw(1);

	JPG_ZRL_CUTw(0);
	JPG_RST_IVLw(0);
	JPG_QP0w(5);
	JPG_QP1w(1);
	JPG_YDC_QTBLw(0xa);
	JPG_CDC_QTBLw(0x11);
	JPG_MAXw(0x7d000);
	JPG_HWw(0x780);
	JPG_VWw(0x438);

	JPG_VOFSw(0);
	JVLOCK_POSw(6);
	JPG_DC_FIXw(1);
	JPG_YQT_GAINw(0x20);
	JPG_CQT_GAINw(0x20);
	JPG_QP_MODEw(0);
	JPG_C_QUALITYw(0);
	JPG_C_DCT_CUT_ENw(0);
	JPG_C_DCT_CUT_ONw(0);
	JPG_C_DCT_CUT_NUMw(0);
	JPG_ONw(1);

	#ifdef		JPG_ENCODING
	JPGCK_PDw(0);
	JPGCK_SELw(2);									//	ISP Post Clock 74.25MHz
	JPGCK_PDw(1);
	#endif

	JPG_MEM_PDw(0);
	JPG_MODEw(0);									//	JPEG Encoding Mode
	JPG_MEM_PDw(1);

	if(SD_MODr==3)
	{
		HLOCKI2_POSw(0x8e);
		HLOCKO_IT_POS0w(0x63);
	}
}

void JPG_Encoding(UINT jpg_adr)
{
	JPG_ADRw(jpg_adr);

	JPG_IVSELw(0);									//	Encoding Path
	JPG_ISELw(8);									//	ISP Path

	JPG_GOw(1);

}

void JPG_Decoding_Display_Set(void)
{
	JPG_DEC_TONw(1);								//	Digital R/W Channel 3 -> Decoder Read Enable

	RYC_OSYNC_MOD1w(1);
	RYC_HTWO1w(0x896);
	RYC_VTWO1w(0x464);
#ifdef	JPG_DECODING_DSCALE
	RYC_VWI1w(1080);
	RYC_HWI1w(1920);
	RYC_HSPI1w(0x10);
	RYC_VSPI1w(0x10);

	RYC_OSYNC_MOD2w(1);
	RYC_HTWO2w(0x896);
	RYC_VTWO2w(0x464);
	RYC_VWI2w(0x2d0);
	RYC_HWI2w(0x500);
	RYC_HSPI2w(0x180);
	RYC_VSPI2w(0xb4);
#else
	RYC_VWI1w(0x438);
	RYC_HWI1w(0x780);
	RYC_HSPI1w(0x10);
	RYC_VSPI1w(0x10);
#endif

	IM_RVSEL3w(5);

#ifdef	JPG_DECODING_DSCALE
	ITU_SSEL0w(0x6);								//	Decoding Image Write Channel 2 -> Read Channel 2 -> Output
	ITU_ASEL0w(0xc);								//	Read Channel 2
	OSD_ISEL0w(0x7);
	OSD_IVSEL0w(0x6);
	//	Down Scale Setting
	CDS0_VISELw(5);
	CDS0_ISELw(0x1b);
	CDS0_HONw(1);
	CDS0_HSELw(0x5);
	DS0_KXY_ONw(0);
	DS0_HLPFw(2);
	DS0_VLPFw(2);
	DS0_DKXw(0x60);
	DS0_DKYw(0x60);
	CDS0_HWw(1280);
	CDS0_VWw(720);
	CDS0_PCK_PDw(0);
	CDS0_PCK_SELw(2);								//	74.25MHz
	CDS0_PCK_PDw(1);
	DS0_ONw(1);
	//	Write Channel Setting
	IM_IVSEL2w(6);
	IM_ISEL2w(0);
	IM_HWI2w(1280);

	IM_WFRC2_ONw(1);
	YCW_CK2_PDw(0);
	YCW_CK2_SELw(2);
	YCW_CK2_PDw(1);
	//	Read Channel Setting
	YCR2_HONw(1);
	YCR2_HLOCKI_SELw(6);
	IM_RFRC2_ONw(1);
	IM_RON2w(1);
	IM_CLRREN2w(1);
	IM_RVSEL2w(6);
	IM_RISEL2w(6);
	IM_RHWI2w(1280);
	DDR_RYC_LTC2w(0x402);
	YCR_CK2_PDw(0);
	YCRCK2_SELw(2);
	YCR_CK2_PDw(1);

	IM_CGO2w(1);
#else
	ITU_SSEL0w(0x5);								//	Decoding Image Direct Output
	ITU_ASEL0w(0x13);
	OSD_ISEL0w(0xc);
	OSD_IVSEL0w(0x5);
#endif

	JPG_MEM_PDw(0);
	JPG_MODEw(1);
	JPG_MEM_PDw(1);

	RYCCK1_PDw(0);
	RYCCK2_PDw(0);
	YCR_CK3_PDw(0);
	RYCCK1_SELw(2);									//	74.25MHz
	RYCCK2_SELw(2);									//	74.25MHz
	YCRCK3_SELw(2);									//	74.25MHz
	RYCCK1_PDw(1);
	RYCCK2_PDw(1);
	YCR_CK3_PDw(1);

	JDEC_CK_PDw(0);
	JDEC_CK_SELw(1);								//	JPEG Decoder Clock 74.25 MHz
	JDEC_CK_PDw(1);

	YCR3_HLOCKI_SELw(5);							//	Select RYC Channel 1 HLOCK Sync
	YCR3_HONw(1);
	JPG_IVSELw(5);									//	Decoding Sync
	JPG_ISELw(0x19);

	DDR_RYC_LTC1w(0x3f2);

//	IM_RHWI3w(0x780);
}

void JPG_Decoding(UINT jpg_adr)
{
	JPG_ADRw(jpg_adr);
//	JDEC_CINVw(0);

	JPG_DEC_GOw(1);									//	Decoding Start

}

#endif


void Isp_Ddr_Cong(void)
{
	#define ISP_DDR_10BIT_SIZE	(((PO_HW*PO_VW)*10)>>(3+4))
	#define ISP_DDR_ENC_SIZE	(((720/*960*/*576)<<4)>>(3+4))	// 960도 지원되나 외부 Clk 필요

	#define DDR_BASE_ISP_WDR	(DDR_BASE_ISP+0x000000)
	#define DDR_BASE_ISP_FRC0	(DDR_BASE_ISP_WDR +ISP_DDR_10BIT_SIZE)
	#define DDR_BASE_ISP_FRC1	(DDR_BASE_ISP_FRC0+ISP_DDR_10BIT_SIZE)
	#define DDR_BASE_ISP_FRC2	(DDR_BASE_ISP_FRC1+ISP_DDR_10BIT_SIZE)
	#define DDR_BASE_ISP_FRC3	(DDR_BASE_ISP_FRC2+ISP_DDR_10BIT_SIZE)
	#define DDR_BASE_ISP_FRC4	(DDR_BASE_ISP_FRC3+ISP_DDR_10BIT_SIZE)

	#define DDR_BASE_ISP_ENC0	(DDR_BASE_ISP_FRC4+ISP_DDR_10BIT_SIZE)
	#define DDR_BASE_ISP_ENC1	(DDR_BASE_ISP_ENC0+ISP_DDR_ENC_SIZE)
	#define DDR_BASE_ISP_ENC2	(DDR_BASE_ISP_ENC1+ISP_DDR_ENC_SIZE)
	#define DDR_BASE_ISP_ENC3	(DDR_BASE_ISP_ENC2+ISP_DDR_ENC_SIZE)

	WDR_ADR_LEw(DDR_BASE_ISP_WDR);
	FRC_ADR0w(DDR_BASE_ISP_FRC0);
	FRC_ADR1w(DDR_BASE_ISP_FRC1);
	FRC_ADR2w(DDR_BASE_ISP_FRC2);
	FRC_ADR3w(DDR_BASE_ISP_FRC3);
	FRC_ADR4w(DDR_BASE_ISP_FRC4);

	ENC_ADR0w(DDR_BASE_ISP_ENC0);
	ENC_ADR1w(DDR_BASE_ISP_ENC1);
	ENC_ADR2w(DDR_BASE_ISP_ENC2);
	ENC_ADR3w(DDR_BASE_ISP_ENC3);

#if 1
	IM_YADR0w(DDR_BASE_ISP+0x134000);
	IM_CADR0w(DDR_BASE_ISP+0x174000);
	IM_YADR1_P0w(DDR_BASE_ISP+0x194000);
	IM_CADR1_P0w(DDR_BASE_ISP+0x1b4000);
	IM_YADR1_P1w(DDR_BASE_ISP+0x1c4000);
	IM_CADR1_P1w(DDR_BASE_ISP+0x1e4000);
	IM_YADR1_P2w(DDR_BASE_ISP+0x1f4000);
	IM_CADR1_P2w(DDR_BASE_ISP+0x214000);
	IM_YADR2_P0w(DDR_BASE_ISP+0x224000);
	IM_CADR2_P0w(DDR_BASE_ISP+0x244000);
	IM_YADR2_P1w(DDR_BASE_ISP+0x254000);
	IM_CADR2_P1w(DDR_BASE_ISP+0x274000);
	IM_YADR2_P2w(DDR_BASE_ISP+0x284000);
	IM_CADR2_P2w(DDR_BASE_ISP+0x2a4000);
	IM_YADR3_P0w(DDR_BASE_ISP+0x2b4000);
	IM_CADR3_P0w(DDR_BASE_ISP+0x2d4000);
	IM_YADR3_P1w(DDR_BASE_ISP+0x2e4000);
	IM_CADR3_P1w(DDR_BASE_ISP+0x304000);
	IM_YADR3_P2w(DDR_BASE_ISP+0x314000);
	IM_CADR3_P2w(DDR_BASE_ISP+0x334000);
	IM_YADR4_P0w(DDR_BASE_ISP+0x344000);
	IM_CADR4_P0w(DDR_BASE_ISP+0x364000);
	IM_YADR4_P1w(DDR_BASE_ISP+0x374000);
	IM_CADR4_P1w(DDR_BASE_ISP+0x394000);
	IM_YADR4_P2w(DDR_BASE_ISP+0x3a4000);
	IM_CADR4_P2w(DDR_BASE_ISP+0x3c4000);

	IM_YADR0_DSw(DDR_BASE_ISP+0x000000);
	IM_CADR0_DSw(DDR_BASE_ISP+0x000000);
	IM_YADR1_DSw(DDR_BASE_ISP+0x000000);
	IM_CADR1_DSw(DDR_BASE_ISP+0x000000);
#endif
}

#endif
