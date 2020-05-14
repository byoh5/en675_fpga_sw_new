#include "dev.h"

UINT FPS_VDI = model_Sens_Fps;
UINT FPS_VDO = model_Sens_Fps;

#ifndef __SENSOR__

void ISRT0 SetSens(WORD awAddr, BYTE abData) { }
void ISRT0 SetSensBurst(WORD awAddr, BYTE* apbData, BYTE abLength) { }

#else

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
	//CLI_VLOCKO_Tw(1);		// TODO KSH> 컴파일 문제?
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

void ISRT0 SetSensTwiBurst(BYTE abDevaddr, WORD awAddr, BYTE* apbData, BYTE abLength) // Prevent for optimization
{
#if (USE_I2C0==3) && (USE_SPI0==0)
	//if(gbSensorOff) return;
	UINT i=0;

	SI2C_STA

	SI2C_WAIT(SENS_WRITE_NODEV1, I2cWrite(SENSOR_I2C_CH,abDevaddr,0,0))
	SI2C_CODE(SENS_WRITE_NOACK2, I2cWrite(SENSOR_I2C_CH,(awAddr>>8)&0xff, 0,0))
	SI2C_CODE(SENS_WRITE_NOACK2, I2cWrite(SENSOR_I2C_CH,(awAddr>>0)&0xff, 0,0))

	for(i=0; i<abLength; i++) {
		SI2C_CODE(SENS_WRITE_NOACK4, I2cWrite(SENSOR_I2C_CH, apbData[i], ((i+1)==abLength),0))
	}

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
  #if (USE_SPI0==2) && (USE_I2C0==0) && (SPI0_BIT==24)
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

void SetSensSpiBurst_Sony(BYTE abID, BYTE abAddr, BYTE* abData, BYTE abLength)
{
  #if (USE_SPI0==2) && (USE_I2C0==0) && (SPI0_BIT==24)
	UINT i=0;

	SpiSetWs(SENSOR_SPI_CH, 8);

	SpiSetCs(SENSOR_SPI_CH, 0);

	SpiWrite(SENSOR_SPI_CH, &abID);
	SpiWrite(SENSOR_SPI_CH, &abAddr);

	for(i=0; i<abLength; i++) SpiWrite(SENSOR_SPI_CH, abData+i);

	SpiSetCs(SENSOR_SPI_CH, 1);

	SpiSetWs(SENSOR_SPI_CH, SPI0_BIT);

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

void ISRT0 SetSensBurst(WORD awAddr, BYTE* apbData, BYTE abLength) // Prevent for optimization
{
#if model_Sens_Ctrl == 1
	SetSensTwiBurst(SENSOR_SONY_I2C_CA, awAddr, apbData, abLength);
#else
	const BYTE bSpiIDt = (awAddr>>8)&0xff;
	const BYTE bSpiID = (SENS_SONY_ID2_TWI <= bSpiIDt && bSpiIDt <= SENS_SONY_ID6_TWI) ? (bSpiIDt - SENS_SONY_ID2_TWI) + SENS_SONY_ID2 : bSpiIDt;
	SetSensSpiBurst_Sony(bSpiID, awAddr&0xff, apbData, abLength);
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
//	DownScaler settings
#define DS_ENABLE(CH, En)		DS##CH##_ONw(En)
#define CDS_HW(CH, Res)			CDS##CH##_HWw(Res)
#define CDS_VW(CH, Res)			CDS##CH##_VWw(Res)
#define DS_DKX(CH, Val)			DS##CH##_DKXw(Val)
#define DS_DKY(CH, Val)			DS##CH##_DKYw(Val)
#define DS_HLPF(CH, Val)		DS##CH##_HLPFw(Val)
#define DS_VLPF(CH, Val)		DS##CH##_VLPFw(Val)

#define CDS_PCK_PD(CH, Val)		CDS##CH##_PCK_PDw(Val)
#define CDS_PCK_SEL(CH, Sel)	CDS##CH##_PCK_SELw(Sel)
#define CDS_ISEL(CH, Sel)		CDS##CH##_ISELw(Sel)
#define CDS_VISEL(CH, Sel)		CDS##CH##_VISELw(Sel)

void DownScalerSet(BYTE bCh, BYTE bEn, BYTE bSrc, UINT nIHRes, UINT nIVRes, UINT nOHRes, UINT nOVRes, BYTE bLPF)
{
	if(bEn) {

		switch {
			case 0 : {
				DS_ENABLE(0, 0); break;
			}
		}
		CDS_PCK_PD(bCh, 0); // DS Clock Power Down

		// Why??? Need to check..
		CDS_PCK_SEL(bCh, 2);
		CDS_ISEL(1);
		CDS_VISEL(0);

		CDS_HW(bCh, nOHRes);
		CDS_VW(bCh, nOVRes);
		DS_DKX(bCh, (nIHRes/nOHRes)*64);
		DS_DKY(bCh, (nIVRes/nOVRes)*64);

		DS_HLPF(bCh,bLPF);
		DS_VLPF(bCh,bLPF);
		CDS_PCK_PD(bCh,1); // DS Clock Power On
		DS_ENABLE(bCh, 1);
	}
	else {
		switch {
			case 0 : DS_ENABLE(0, 0); break;
			case 1 : DS_ENABLE(1, 0); break;
			case 2 : DS_ENABLE(2, 0); break;
			case 3 : DS_ENABLE(3, 0); break;
			case 4 : DS_ENABLE(4, 0); break;
		}
	}
}
#endif

//	LCD Function
//#define LCD_DEBUG
#define model_Lcd			1		// 0 : None
									// 1 : TFT035
									// 2 : TFT028
									// 3 : TFT023

void LCD_INSDAT_Write(UINT INSDAT0, UINT INSDAT1,UINT INSDAT2,UINT INSDAT3, UINT IStep)
{
	SetIsp(0x34c, INSDAT0);
	SetIsp(0x34d, INSDAT1);
	SetIsp(0x34e, INSDAT2);
	SetIsp(0x34f, INSDAT3);
	LCD0_INST_STEPw(IStep);

#ifdef LCD_DEBUG
	printf("\r\nTest LCD controls..\r\n");
	printf("GetIsp(0x34c) : 0x%08x\r\n", GetIsp(0x34c));
	printf("GetIsp(0x34d) : 0x%08x\r\n", GetIsp(0x34d));
	printf("GetIsp(0x34e) : 0x%08x\r\n", GetIsp(0x34e));
	printf("GetIsp(0x34f) : 0x%08x\r\n", GetIsp(0x34f));
#endif

	SetIsp(0x034a, 0x80000000);					//	Write LCD Command

	while(LCD_CMD_BUSYr);

	//sleep_(2000);
	WaitXus(2000);
}

void LCD_DeviceInit(void)
{
	//	LCD Reset
	SYS_GPIO7_MUX	=	0;
	GPIO_PIN7_OEN	=	0;
	GPIO_PIN7_OUT	=	1;
	WaitXus(5000);
	GPIO_PIN7_OUT	=	0;
	WaitXus(5000);
	GPIO_PIN7_OUT	=	1;

#if model_Lcd==1
	LCD_INSDAT_Write(0xE0000309,0x08160A3F,0x784C090A,0x08161A0F,16);
	LCD_INSDAT_Write(0xE1001619,0x030F0532,0x4546040E,0x0D35370F,16);
	LCD_INSDAT_Write(0xC0171500,0,0,0,3);
	LCD_INSDAT_Write(0xC1410000,0,0,0,2);
	LCD_INSDAT_Write(0xC5001280,0,0,0,4);
	//	Memory Address Set	->	HV Exchange !!
	LCD_INSDAT_Write(0x2A000001,0xDF000000,0,0,5);			//	SC, EC	->  480
	LCD_INSDAT_Write(0x2B000001,0x3F000000,0,0,5);			//	SP, EP	->  320
	LCD_INSDAT_Write(0x36280000,0,0,0,2);
	LCD_INSDAT_Write(0x3A550000,0,0,0,2);
	LCD_INSDAT_Write(0xB0000000,0,0,0,2);
	LCD_INSDAT_Write(0xB1A00000,0,0,0,2);
	LCD_INSDAT_Write(0xB4020000,0,0,0,2);
	LCD_INSDAT_Write(0xB6020200,0,0,0,3);
	LCD_INSDAT_Write(0xE9000000,0,0,0,2);
	LCD_INSDAT_Write(0xF7A9512C,0x82000000,0,0,5);
	LCD_INSDAT_Write(0x11000000,0,0,0,1);
	WaitXus(200000);
	LCD_INSDAT_Write(0x29000000,0,0,0,1);

#elif model_Lcd==2

	LCD_INSDAT_Write(0x28000000,0,0,0,1); //display OFF
	LCD_INSDAT_Write(0x11000000,0,0,0,2); //exit SLEEP mode
	LCD_INSDAT_Write(0xCB392c00,0x34020000,0,0,6); //Power Control A
	LCD_INSDAT_Write(0xCF008130,0,0,0,4); //Power Control B

	LCD_INSDAT_Write(0xE8850179,0,0,0,4); //Driver timing control A
	LCD_INSDAT_Write(0xEA000000,0,0,0,3); //Driver timing control B
	LCD_INSDAT_Write(0xED640312,0x81000000,0,0,5); //Power On sequence control

	LCD_INSDAT_Write(0xF7200000,0,0,0,2); //Pump ratio control
	LCD_INSDAT_Write(0xC0260400,0,0,0,3); //power control 1
	LCD_INSDAT_Write(0xC1110000,0,0,0,2); //power control 2
	LCD_INSDAT_Write(0xC5353E00,0,0,0,3); //VCOM control 1
	LCD_INSDAT_Write(0xC7BE0000,0,0,0,2); //VCOM control 2

	LCD_INSDAT_Write(0x36680000,0,0,0,2); //memory access control = BGR

	LCD_INSDAT_Write(0xB1001000,0,0,0,3); //frame rate control
	LCD_INSDAT_Write(0xB60AA200,0,0,0,3); //display function control

	LCD_INSDAT_Write(0x3A550000,0,0,0,2); //pixel format = 16 bit per pixel
	LCD_INSDAT_Write(0xF2020000,0,0,0,2); //3G Gamma control

	LCD_INSDAT_Write(0x26010000,0,0,0,2); //Gamma curve 3
	LCD_INSDAT_Write(0x2A000001,0x3F000000,0,0,5); //column address set
	LCD_INSDAT_Write(0x2B000000,0xEF000000,0,0,5); //page address set
	//LCD_INSDAT_Write(0x2A000000,0xEF000000,0,0,5); //column address set
	//LCD_INSDAT_Write(0x2B000001,0x3F000000,0,0,5); //page address set
	LCD_INSDAT_Write(0xB4020000,0,0,0,2);
	//LCD_INSDAT_Write(0xB4030000,0,0,0,2); // Display inversion control test (0x000000XX, XX = DINV[1:0])

	WaitXus(200000);
	LCD_INSDAT_Write(0x29000000,0,0,0,1); //display ON

#endif

	WaitXus(200000);
	LCD0_INSTPARA0w(0x2c);

	WaitXus(500000);
	LCD0_ICw(0);								//	Instruction Mode On
	LCD0_WIMGw(1);								//	Image Pumping Mode On
}

void Init_LCD_Control(void)
{
#ifdef LCD_DEBUG
	printf("Init LCD controller..\r\n");
#endif

	//	Down -Scale
	CDS3_PCK_SET(2);
	CDS3_ISELw(1);
	CDS3_VISELw(0);
#if model_Lcd==1
	CDS3_HWw(480);					//	1920 -> 480
	CDS3_VWw(320);					//	1080 -> 320
	DS3_DKXw(0x100);
	DS3_DKYw(0xd9);

#elif model_Lcd==2
	CDS3_HWw(320);					//	1920 -> 320
	CDS3_VWw(240);					//	1080 -> 240
	DS3_DKXw(384);
	DS3_DKYw(288);
#endif
	DS3_HLPFw(2);
	DS3_VLPFw(2);
	DS3_ONw(1);

	//	Write Channel
	YCW_CK1_SET(2);				//	PCLK - BT1120 16Bit Input
	IM_IVSEL1w(0);					//	BT Channel Main
	IM_ISEL1w(3);					//	Down-Scale 0
#if model_Lcd==1
	IM_HWI1w(480);
#elif model_Lcd==2
	IM_HWI1w(320);
#endif

	IM_WFRC1_ONw(1);
	IM_CGO1w(1);

	//	Read Channel
	IM_RVSEL1w(6);
	IM_RISEL1w(6);
	IM_RFRC1_ONw(1);
#if model_Lcd==1
	IM_RHWI1w(480);
#elif model_Lcd==2
	IM_RHWI1w(320);
#endif
	DDR_RYC_LTC2w(0x100);
	IM_RON1w(1);
	YCR_CK1_SET(13);

	DOMODEw(4);						//
	LCD0_PMODw(7);					//	MCU18Bit RGB 6-6-6 Interface
	PCK_EMAX2w(8);					//	PCK_DIV2
	PCK_EHVL2w(4);
	PCK_EDIV_PD2w(1);

	PCK_EMAX3w(8);					//	PCK_DIV3
	PCK_EHVL3w(4);
	PCK_EDIV_PD3w(1);

	RYC_OSYNC_MOD2w(1);
#if model_Lcd==1
	RYC_HWI2w(0x1e0);
	RYC_VWI2w(0x140);
	RYC_HTWO2w(0x208);
	RYC_VTWO2w(0x1e0);
#elif model_Lcd==2
	RYC_HWI2w(320);
	RYC_VWI2w(240);
	RYC_HTWO2w(346);
	RYC_VTWO2w(360);
#endif
	RYC_HSPI2w(8);
	RYC_VSPI2w(8);

	IM_RCINV1w(0);

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
	DO0_CK_SELw(14);				//	PCK_DIV3
	DO1_CK_SELw(14);
	DO0_CK_PDw(1);
	DO1_CK_PDw(1);

	DCKO2_PDw(0);
	DCKO2_INVw(1);
	DCKO2_SELw(13);					//	For Xilinx Board, PCK_EDIV3
	DCKO2_PDw(1);

	//	Sync Gen Clock
	RYCCK2_PDw(0);
	RYCCK2_SELw(13);				//	PCK_DIV2
	RYCCK2_PDw(1);

	//	LCD Output Clock & DCX, CSX Port Enable
	VSO_OENw(0);
	HSO_OENw(0);
	DENO_OENw(0);
	DO_OENw(0);

	DCKO2_OENw(0);


	//---LCD Mode Setting-------
	//	Read Channel Setting
	LCD0_VSELw(0xa);				//	Read Sync 2
	LCD0_DSELw(0x1);				//	Read Channel 1

	LCD0_INST_CONw(0);

	//	LCD IP Clock
	LCD_CK_PDw(0);
	LCD_CK_SELw(11);				//	PCK_DIV2
	LCD_CK_PDw(1);

	WaitXus(100);						//	LCD Fifo Clear Delay

	LCDO_CK_PDw(0);
	LCDO_CK_SELw(12);				//	PCK_DIV3
	LCDO_CK_PDw(1);

	LCD0FF_RSw(1);								//	FIFO Reset
	LCD0_ICw(1);								//	Instruction Mode On
	LCD0_CSXw(0);
	LCD0_DCXw(0);
//	LCD0_DCX_SELw(1);

	LCD_DeviceInit();

	D_ITU_YCCH0w(0);							//	DO31 ~ DO16 -> Output Channel 0
}

#if		JPG_ENCODING||JPG_DECODING
void JPG_Init(void)
{
	SYNC_UPw(1);									//	Sync_up

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
	BYTE *heap_end;
	asm volatile("la %0, _heap_end" : "=r"(heap_end));

	const UINT Isp_Wdr_Adr = (((intptr_t)heap_end)/*DDR1_BASE*/>>4);
	const UINT Isp_Frc_Adr = Isp_Wdr_Adr_config(Isp_Wdr_Adr, 1, RP(PO_HW), RP(PO_VW), DZ_HW_MR<<1, 0);	// DZOOM 시 DZ_HW_MR+a를 해야 하단 자주색 깜빡임 사라짐
	const UINT Isp_Cvb_Adr = Isp_Frc_Adr_Config(Isp_Frc_Adr, 2, RP(PO_HW), RP(PO_VW), DZ_HW_MR<<1, 0);	//	"
	const UINT Isp_YC_Adr1 = Isp_Cvb_Adr_Config(Isp_Cvb_Adr, 720/*960*/, 576);

	const UINT Isp_YC_Adr2 = Isp_YC_Adr_Config(Isp_YC_Adr1, 1, 3, RP(FR_HW), RP(FR_VW), DZ_HW_MR, 8);
	const UINT Isp_YC_Adr3 = Isp_YC_Adr_Config(Isp_YC_Adr2, 2, 3, RP(FR_HW), RP(FR_VW), 0, 0);
//	const UINT Isp_YC_Adr4 = Isp_YC_Adr_Config(Isp_YC_Adr3, 3, 3, RP(PO_HW), RP(PO_VW), 0, 0);
//	const UINT Isp_YC_AdrE = Isp_YC_Adr_Config(Isp_YC_Adr4, 4, 3, RP(PO_HW), RP(PO_VW), 0, 0);
}

#endif
