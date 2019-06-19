#include "dev.h"

#ifdef __SENSOR__

#include "isp_sensor_tbl.h"


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
	UINT i;

	for(i=0;i<Cnt;i++)
	{
		Wait_VLOCKO();
	}
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

void SetSens_IMX291(BYTE DEV, BYTE ADDR, BYTE DAT)
{
	while(I2cWrite(SENSOR_I2C_CH,SENS_SONY_DEV,0,0));

	I2cWrite(SENSOR_I2C_CH,DEV,0,0);
	I2cWrite(SENSOR_I2C_CH,ADDR,0,0);
	I2cWrite(SENSOR_I2C_CH,DAT,1,0);
}

void SONY_SetSens(BYTE DEV_ID, UINT Adr, BYTE Dat)
{

	while(I2cWrite(SENSOR_I2C_CH,DEV_ID,0,0));

	I2cWrite(SENSOR_I2C_CH,((Adr&0xFF00)>>8), 0,0);
	I2cWrite(SENSOR_I2C_CH,((Adr&0x00FF)>>0), 0,0);
	I2cWrite(SENSOR_I2C_CH,Dat, 1,0);
}

void SetSens_OVSense(WORD ADDR, BYTE DAT)
{
	while(I2cWrite(SENSOR_I2C_CH,SENS_OV_DEV,0,0));

//	while(I2cWrite(SENSOR_I2C_CH,SENS_OV_DEV,0,0))
//	{
//			sleep_(1);
//	}
//	Wait_VLOCKO();

//	I2cWrite(SENSOR_I2C_CH,SENS_OV_DEV,0,0);


	I2cWrite(SENSOR_I2C_CH,(ADDR>>8),0,0);
	I2cWrite(SENSOR_I2C_CH,(ADDR),0,0);
	I2cWrite(SENSOR_I2C_CH,DAT,1,0);
}

#ifdef		OV2718_LVDS_4CH
void OV2718_Init(void)
{

	UINT i;

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PreSync_Config(1, 2200, 1125, 0xe, 0, 0x13, 0x8, 1928, 1088,0,1);
	Isp_PostSync_Config(1, 0, 2200, 1125, 0x3a, 0x2, 0x6, 0x4, 1928, 1088, 3);

	Isp_SDesPowerOn(FN_ON,LVDS_INTERFACE,0);
	Isp_SensorPowerOn(FN_ON,SENS_13M);
//	Isp_SensorPowerOn(FN_ON,SENS_27M);
	Isp_Lvds_Config(LVDS_12BIT, LVDS_4LANE, 0, 0);
//	Isp_Lvds_Config(LVDS_12BIT, LVDS_4LANE, 0, 1);
	Isp_SDesDelay(7, 0, 0, 0, 0);
//	Isp_SDesDelay(5, 0, 0, 0, 0);
	Isp_SDesPosition(0x1f, 0x1, 1988, 1100);
	SLVw(3);
	SYNC_COD0w(0xfff);
	SYNC_COD1w(0x000);
	SYNC_COD2w(0x000);
	SYNC_COD3_0w(0x000);
	SYNC_COD3_1w(0x800);
	SYNC_COD3_2w(0x0ab0);
	SOF_NOw(1);

	ASYNC_ONw(1);
	ASYNC_DSYNCw(1);

	SetSens_OVSense(0x3013, 0x01);
	sleep_(100000);

	for(i=0;i<ARRAY_SIZE(gwOV2718Tbl); i++)
	{
		SetSens_OVSense(gwOV2718Tbl[i][0],gwOV2718Tbl[i][1]);
//		sleep_(100);
	}

	sleep_(10000);
	SetSens_OVSense(0x315a, 0x01);
	SetSens_OVSense(0x30b6, 0x03);


}
#endif

#ifdef		IMX291
void IMX291_Init(void)
{
	UINT i=0;

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PreSync_Config(1, 2200, 1125, 0xa, 0xc, 0x15, 6, 1928, 1088,0,0);
	Isp_PostSync_Config(1, 0, 2200, 1125, 0x3a, 0x2, 0x6, 0x4, 1928, 1088, 1);

	Isp_SensorPowerOn(1,SENS_37M);			//	Sensor Clock
	Isp_SDesPowerOn(FN_ON, LVDS_INTERFACE, 0);
	Isp_Lvds_Config(LVDS_12BIT, LVDS_4LANE, 0, 1);
//	Isp_SDesDelay(4,4,4,4,4);
	Isp_SDesDelay(2,0,0,0,0);
//	Isp_SDesDelay(6,0,1,4,1);			//	2019.05.02 Crazy
	Isp_SDesPosition(0x1f, 0x12, 1988, 1100);
	SLVw(0);

	SYNC_COD0w(0xfff);
	SYNC_COD1w(0x000);
	SYNC_COD2w(0x000);
	SYNC_COD3_0w(0x000);
	SYNC_COD3_1w(0x800);
	SYNC_COD3_2w(0x0ab0);
	SOF_NOw(1);
	RDES_PNSELw(1);
	OMOD_BITw(0);

	POL_HSIw(0);
	POL_VSIw(0);


	#ifdef		IMX291_I2C

		SetSens_IMX291(SENS_SONY_ID2_TWI,0x00,0x1);		 sleep_(10000);
		SetSens_IMX291(SENS_SONY_ID2_TWI,0x02,0x1);		 sleep_(10000);

		for(i=0;i<ARRAY_SIZE(gwTblIMX291); i++)
		{

			if (i<28)			SetSens_IMX291(SENS_SONY_ID2_TWI, gwTblIMX291[i][0], gwTblIMX291[i][1]);
			else if (i<40)		SetSens_IMX291(SENS_SONY_ID3_TWI, gwTblIMX291[i][0], gwTblIMX291[i][1]);
			else if	(i<48)		SetSens_IMX291(SENS_SONY_ID4_TWI, gwTblIMX291[i][0], gwTblIMX291[i][1]);
			else if	(i<60)		SetSens_IMX291(SENS_SONY_ID5_TWI, gwTblIMX291[i][0], gwTblIMX291[i][1]);
			else if (i<61)		SetSens_IMX291(SENS_SONY_ID6_TWI, gwTblIMX291[i][0], gwTblIMX291[i][1]);
			sleep_(1000);
		}

		SetSens_IMX291(SENS_SONY_ID2_TWI, 0x9c, 0x22);					// Clamp sensitivity adjust	// 150317 LH

		SetSens_IMX291(SENS_SONY_ID2_TWI, 0x0, 0x0);	sleep_(3000);	// standby cancel
		SetSens_IMX291(SENS_SONY_ID2_TWI, 0x2, 0x0);	sleep_(2000);	// xmaster

	// SensClk
		#ifdef		IMX291_30FPS
		SetSens_IMX291(SENS_SONY_ID2_TWI, 0x09, 0x02);	//FRSEL
		SetSens_IMX291(SENS_SONY_ID2_TWI, 0x1c, 0x30);	//HMAX
		SetSens_IMX291(SENS_SONY_ID2_TWI, 0x1d, 0x11);	//HMAX
		#elif		IMX291_60FPS
		SetSens_IMX291(SENS_SONY_ID2_TWI, 0x09, 0x01);	//FRSEL
		SetSens_IMX291(SENS_SONY_ID2_TWI, 0x1c, 0x98);	//HMAX
		SetSens_IMX291(SENS_SONY_ID2_TWI, 0x1d, 0x08);	//HMAX
		#endif
	#elif		IMX291_SPI


	#endif

	//Sensor V Reverse
	SetSens_IMX291(SENS_SONY_ID2_TWI, 0x07, 0x41);
}
#endif

#if defined(OV4689_MIPI_15FPS) || defined(OV4689_MIPI_30FPS)
void OV4689_Init(void)
{
	UINT i=0;

	Isp_SDesPowerOn(FN_ON,MIPI_INTERFACE,2);					//	OnOff, IsMipi, MipiClkPhase

	#ifdef		OV4689_MIPI_15FPS
	Isp_SensorPowerOn(FN_ON, SENS_13M);				//	13.5MHz
	#else
	Isp_SensorPowerOn(FN_ON, SENS_27M);				//	27MHz
	#endif

	Isp_Mipi_Config(MIPI_10BIT, MIPI_4LANE, 0, 1, USE_ECC_CHECK, USE_WCL_CHECK, NO_USE_CHECK, NO_USE_CHECK, 3);
	Isp_SDesPosition(0xb5, 0xc, 2584, 1458);			//	4M + Margin
//	Isp_SDesDelay(2, 0, 0, 0, 0);
	Isp_SDesDelay(4, 0, 0, 0, 0);	//	20190503 : main_asic_crazy_ISP_R5_HDMI_DDR_Auto_Align


	SYNC_COD0w(0);
	SYNC_COD1w(1);
	SYNC_COD2w(0x2);				//	Short Packet Line Start
	SYNC_COD3_0w(0x3);				//	Short Packet Line End
	SYNC_COD3_1w(0xb8);
	SYNC_COD3_2w(0xd20);
	SYNC_S0COD3_0w(0);
	SYNC_S0COD3_1w(0x2000);
	SYNC_S0COD3_2w(0x2b);			//	Long Packet Line Start
	SYNC_S1COD3_0w(0x3);			//	Long Packet Line End
	SYNC_S1COD3_1w(0);
//	SYNC_S1COD3_2w(3);
	SYNC_S1COD3_2w(0xd20);

	MIPI_HLOCK_POSw(8);				//	Very Important!!! For Image Phase
	MIPI_LAT_AENw(0);


//	VLOCKO_IT_POS0w(0x44e);
//	ASYNC_ONw(1);
//	VSYN_NAONw(1);
	POL_VSIw(1);

	#ifdef		OV4689_MIPI_15FPS
	for(i=0;i<ARRAY_SIZE(gTblMIPI4689_15FPS); i++)
	{
		while(I2cWrite(SENSOR_I2C_CH,0x42,0,0));
		I2cWrite(SENSOR_I2C_CH,((gTblMIPI4689_15FPS[i][0]&0xFF00)>>8), 0,0);
		I2cWrite(SENSOR_I2C_CH,((gTblMIPI4689_15FPS[i][0]&0x00FF)>>0), 0,0);
		I2cWrite(SENSOR_I2C_CH,gTblMIPI4689_15FPS[i][1], 1,0);
		sleep_(500);
	}
	#elif		OV4689_MIPI_30FPS
	for(i=0;i<ARRAY_SIZE(gTblMIPI4689_30FPS); i++)
	{
		while(I2cWrite(SENSOR_I2C_CH,0x42,0,0));
		I2cWrite(SENSOR_I2C_CH,((gTblMIPI4689_30FPS[i][0]&0xFF00)>>8), 0,0);
		I2cWrite(SENSOR_I2C_CH,((gTblMIPI4689_30FPS[i][0]&0x00FF)>>0), 0,0);
		I2cWrite(SENSOR_I2C_CH,gTblMIPI4689_30FPS[i][1], 1,0);
		sleep_(500);
	}
	#endif

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PreSync_Config(1, 3300, 1500, 0xb0, 0xc, 0x8, 4, 2568, 1448,1,1);
	Isp_PostSync_Config(1, 0, 3300, 1500, 0x3a, 0x2, 0x6, 0x4, 2568, 1448, 1);

	//	For Test
//	TPAT_SELw(8);

//	sleep_(1000);

}
#endif

#ifdef		IMX335_MIPI_4LANE

void IMX335_Init(void)
{
	UINT i=0;

//	Isp_SensorPowerOn(1,4);					//	Sensor Clock
	Isp_SensorPowerOn(1,SENS_13M);					//	Sensor Clock
	Isp_SDesPowerOn(FN_ON,MIPI_INTERFACE,2);
	Isp_Mipi_Config(MIPI_12BIT, MIPI_4LANE, 0, 1, USE_ECC_CHECK, NO_USE_CHECK, NO_USE_CHECK, NO_USE_CHECK, 3);
	Isp_SDesPosition(0xb5, 0xc, 2584, 1458);
	Isp_SDesDelay(2, 0, 0, 0, 0);			//	20190503 : main_asic_61_ISP_R5.bit
	SLVw(0);

	MIPI_HLOCK_POSw(7);				//	Very Important!!!

	SYNC_COD0w(0);
	SYNC_COD1w(1);
	SYNC_COD2w(0x2);				//	Short Packet Line Start
	SYNC_COD3_0w(0x3);				//	Short Packet Line End
	SYNC_COD3_1w(0xb8);
	SYNC_COD3_2w(0xf30);
	SYNC_S0COD3_0w(0);
	SYNC_S0COD3_1w(0x2000);
	SYNC_S0COD3_2w(0x2c);			//	Long Packet Line Start - Raw 12Bit
	SYNC_S1COD3_0w(0x3);			//	Long Packet Line End
	SYNC_S1COD3_1w(0);
	SYNC_S1COD3_2w(3);
	RDES_RPOSw(0xb5);


	MIPI_LAT_AENw(0);
//	ASYNC_ONw(1);
//	VSYN_NAONw(1);
	POL_VSIw(1);

	SONY_SetSens(SENS_SONY_DEV, 0x3000, 0x1);	sleep_(1000000);		// standby
	SONY_SetSens(SENS_SONY_DEV, 0x3002, 0x1);	sleep_(1000000);		// xmaster stop
	SONY_SetSens(SENS_SONY_DEV, 0x3001, 0x01);					// REG Hold
	SONY_SetSens(SENS_SONY_DEV, 0x3003, 0x01);					// SW rese

	for(i=0;i<ARRAY_SIZE(gTblMIPI_IMX335); i++)
	{

		SONY_SetSens(SENS_SONY_DEV,gTblMIPI_IMX335[i][0],gTblMIPI_IMX335[i][1]);

		sleep_(1000);
	}

	SONY_SetSens(SENS_SONY_DEV, 0x3003, 0x00);						// SW reset OFF
	SONY_SetSens(SENS_SONY_DEV, 0x3001, 0x00);						// REG Hold OFF
	SONY_SetSens(SENS_SONY_DEV, 0x3000, 0x0);	sleep_(3000000);		// standby cancel
	SONY_SetSens(SENS_SONY_DEV, 0x3002, 0x0);	sleep_(2000000);		// xmaster


	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PreSync_Config(1, 2200, 1125, 0xb0, 0xc, 0x8, 4, 2568, 1448,1,1);
	Isp_PostSync_Config(1, 0, 3300, 1500, 0x3a, 0x2, 0x6, 0x4, 2568, 1448, 2);

}

#endif

#ifdef		IMX225_MIPI_4LANE_60FPS
void IMX225_Init(void)
{
	UINT i=0;

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PreSync_Config(1, 1650, 750, 0xa, 0xc, 0x15, 6, 1288, 728,0,1);
	Isp_PostSync_Config(1, 0, 1650, 750, 0x3a, 0x2, 0x6, 0x4, 1288, 728, 1);

	Isp_SensorPowerOn(FN_ON,SENS_27M);					//	Sensor Clock
	Isp_SDesPowerOn(FN_ON, MIPI_INTERFACE, 2);
	Isp_Mipi_Config(MIPI_10BIT, MIPI_4LANE, 0, 1, USE_ECC_CHECK, NO_USE_CHECK, NO_USE_CHECK, NO_USE_CHECK, 3);

	Isp_SDesPosition(0xb5, 0xc, 1308, 736);
	Isp_SDesDelay(2, 0, 0, 0, 0);			//	20190503 : main_asic_61_ISP_R5.bit
	SLVw(0);

	MIPI_HLOCK_POSw(7);				//	Very Important!!!

	SYNC_COD0w(0);
	SYNC_COD1w(1);
	SYNC_COD2w(0x2);				//	Short Packet Line Start
	SYNC_COD3_0w(0x3);				//	Short Packet Line End
	SYNC_COD3_1w(0xb8);
	SYNC_COD3_2w(0xf30);
	SYNC_S0COD3_0w(0);
	SYNC_S0COD3_1w(0x2000);
	SYNC_S0COD3_2w(0x2b);			//	Long Packet Line Start - Raw 10Bit
	SYNC_S1COD3_0w(0x3);			//	Long Packet Line End
	SYNC_S1COD3_1w(0);
	SYNC_S1COD3_2w(3);
	RDES_RPOSw(0xb5);


	MIPI_LAT_AENw(0);
//	VSYN_NAONw(1);
	POL_VSIw(1);

	SONY_SetSens(SENS_SONY_DEV, 0x3000, 0x1);	sleep_(1000000);		// standby
	SONY_SetSens(SENS_SONY_DEV, 0x3002, 0x1);	sleep_(1000000);		// xmaster stop
	SONY_SetSens(SENS_SONY_DEV, 0x3001, 0x01);					// REG Hold
	SONY_SetSens(SENS_SONY_DEV, 0x3003, 0x01);					// SW rese

	for(i=0;i<ARRAY_SIZE(gwTblIMX225); i++)
	{

		SONY_SetSens(SENS_SONY_DEV,gwTblIMX225[i][0],gwTblIMX225[i][1]);

		sleep_(1000);
	}

	SONY_SetSens(SENS_SONY_DEV, 0x3000, 0x0);	sleep_(3000000);		// standby cancel
	SONY_SetSens(SENS_SONY_DEV, 0x3002, 0x0);	sleep_(2000000);		// xmaster


	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PreSync_Config(1, 1650, 750, 0xb0, 0xc, 0x8, 4, 1288, 728,0,1);
	Isp_PostSync_Config(1, 0, 1650, 750, 0x3a, 0x2, 0x6, 0x4, 1288, 728, 2);

}

#endif

#ifdef		IMX225_PARALLEL

void SONY_SetSensSpi(BYTE ID, BYTE Adr, BYTE Dat)
{
	BYTE SpiDat[3] = {0,0,0};

	SpiDat[2] = ID;
	SpiDat[1] = Adr;
	SpiDat[0] = Dat;
	SpiCsLo(SENSOR_SPI_CH);
	SpiWrite(SENSOR_SPI_CH,SpiDat);
	SpiCsHi(SENSOR_SPI_CH);

}

void IMX225_Init(void)
{

	UINT i=0;

	Isp_SDesPowerOn(FN_OFF,0,0);			//	LVDS/MIPI Off
	Isp_SensorPowerOn(FN_ON,SENS_27M);		//	Sensor Clock

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PreSync_Config(1, 1650, 750, 0xa, 0xc, 0x15, 6, 1288, 728,0,1);
	Isp_PostSync_Config(1, 0, 1650, 750, 0x3a, 0x2, 0x6, 0x4, 1288, 728, 1);

//	Isp_Parallel_Config(FN_ON, ISP_SLAVE, PARA_EXTER_CLK, PARA_CLK_SDR, 0, SYNC_RISE, SYNC_RISE, DIFF_EDGE, NO_USE_AUTOSYNC);
	Isp_Parallel_Config(FN_ON, ISP_SLAVE, PARA_EXTER_CLK, PARA_CLK_SDR, 0, SYNC_FALL, SYNC_RISE, DIFF_EDGE, NO_USE_AUTOSYNC);

	SLVw(3);								//	Parallel Slave Mode
	IDDR_ONw(1);
	ISDR_OPw(1);

	SONY_SetSensSpi(0x02, 0x00, 0x01);
	SONY_SetSensSpi(0x02, 0x02, 0x01);
	SONY_SetSensSpi(0x02, 0x01, 0x01);
	SONY_SetSensSpi(0x02, 0x03, 0x01);

	sleep_(1000000);

	for(i=1;i<30;i++)	{	SONY_SetSensSpi(0x02, gwTblIMX225_0[i][0], gwTblIMX225_0[i][1]);	}		//	ID2
	for(i=0;i<9;i++)	{	SONY_SetSensSpi(0x03, gwTblIMX225_1[i][0], gwTblIMX225_1[i][1]);	}		//	ID3
	for(i=0;i<12;i++)	{	SONY_SetSensSpi(0x04, gwTblIMX225_2[i][0], gwTblIMX225_2[i][1]);	}		//	ID4

	sleep_(1000000);

	SONY_SetSensSpi(0x02, 0x00, 0x00);
	SONY_SetSensSpi(0x02, 0x02, 0x00);

}
#endif

#ifdef		OV08A10_LVDS_15P
void OS08A10_Init(void)
{
	UINT i;

	Isp_SDesPowerOn(FN_ON,LVDS_INTERFACE,0);					//	OnOff, IsMipi, MipiClkPhase
	Isp_SensorPowerOn(FN_ON, SENS_27M);				//	27MHz

	Isp_Lvds_Config(LVDS_10BIT, LVDS_4LANE, 0, 1);
	Isp_SDesPosition(0xb5, 0xc, 3856, 2176);
	Isp_SDesDelay(2, 0, 0, 0, 0);
	SLVw(0);
	SYNC_COD0w(0xfff);
	SYNC_COD1w(0x000);
	SYNC_COD2w(0x000);
	SYNC_COD3_0w(0x000);
//	SYNC_COD3_1w(0x800);
//	SYNC_COD3_2w(0x0ab0);
	SYNC_COD3_1w(0x200);
	SYNC_COD3_2w(0x280);


	ASYNC_ONw(1);
//	VSYN_NAONw(1);
	POL_VSIw(1);

	for(i=0;i<ARRAY_SIZE(gTblMIPIOS08A10_15FPS); i++)
	{
		SetSens_OVSense(gTblMIPIOS08A10_15FPS[i][0],gTblMIPIOS08A10_15FPS[i][1]);
//		sleep_(100);
	}


	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PreSync_Config(1, 4400, 2250, 0xa, 0xc, 0x15, 6, 3840, 2168,0,0);
	Isp_PostSync_Config(1, 0, 4400, 2250, 0x3a, 0x2, 0x6, 0x4, 3840, 2168, 1);
}
#endif

#ifdef		IMX274_4K_MIPI_15P
void IMX274_Init()
{
	UINT i=0;

//	Isp_SensorPowerOn(1,SENS_27M);					//	Sensor Clock	-	15p
	Isp_SensorPowerOn(1,SENS_13M);					//	Sensor Clock	-	15p
//	Isp_SensorPowerOn(FN_ON,SENS_6M);					//	Sensor Clock	-	7.5p
	Isp_SDesPowerOn(FN_ON,MIPI_INTERFACE,0);
	Isp_Mipi_Config(MIPI_12BIT, MIPI_4LANE, 0, 1, USE_ECC_CHECK, NO_USE_CHECK, NO_USE_CHECK, NO_USE_CHECK, 3);
//	Isp_Mipi_Config(MIPI_10BIT, MIPI_4LANE, 0, 1, USE_ECC_CHECK, NO_USE_CHECK, NO_USE_CHECK, NO_USE_CHECK, 3);
	Isp_SDesPosition(0xb5, 0xc, 3860, 2180);
//	Isp_SDesDelay(2, 0, 0, 0, 0);			//	20190503 : main_asic_61_ISP_R5.bit
	Isp_SDesDelay(6, 0, 0, 0, 0);			//	20190503 : main_asic_61_ISP_R5.bit
	SLVw(0);

	MIPI_HLOCK_POSw(7);				//	Very Important!!!

	SYNC_COD0w(0);
	SYNC_COD1w(1);
	SYNC_COD2w(0x2);				//	Short Packet Line Start
	SYNC_COD3_0w(0x3);				//	Short Packet Line End
	SYNC_COD3_1w(0xb8);
	SYNC_COD3_2w(0xf30);
	SYNC_S0COD3_0w(0);
	SYNC_S0COD3_1w(0x2000);
	SYNC_S0COD3_2w(0x2c);			//	Long Packet Line Start - Raw 12Bit
	SYNC_S1COD3_0w(0x3);			//	Long Packet Line End
	SYNC_S1COD3_1w(0);
	SYNC_S1COD3_2w(3);
	RDES_RPOSw(0xb5);
	MIPI_LAT_AENw(1);				//	Why???????
	POL_VSIw(1);

	SONY_SetSens(SENS_SONY_DEV, 0x3000, 0x12);

	for(i=0;i<ARRAY_SIZE(gwTblIMX274); i++)
	{

		SONY_SetSens(SENS_SONY_DEV,gwTblIMX274[i][0],gwTblIMX274[i][1]);

		sleep_(1000);
	}

	sleep_(700000);				//	wait 10ms

	SONY_SetSens(SENS_SONY_DEV, 0x3000, 0x00);
	SONY_SetSens(SENS_SONY_DEV, 0x303E, 0x02);

	sleep_(500000);				//	wait 7ms

	//Setting 3
	SONY_SetSens(SENS_SONY_DEV, 0x30F4, 0x00);
	SONY_SetSens(SENS_SONY_DEV, 0x3018, 0xA2);

	Isp_PreClk_Config(ISP_CLK_74M);			//	Isp Pre Module Clock
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PreSync_Config(1, 4400, 2250, 0xa, 0xc, 0x15, 6, 3840, 2168,0,0);
	Isp_PostSync_Config(1, 0, 4400, 2250, 0x3a, 0x2, 0x6, 0x4, 3840, 2168, 1);

}

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

	sleep_(2000);

}

void LCD_FR_TFT035_6_Init(void)
{
	//	Xilinx Board


	//	LCD Reset
//	GpioFuncPin(7,0);				//	GPIO Function
//	GpioSetDir(7,0);
//	GpioSetOut(7,1);
//	sleep_(50);
//	GpioSetOut(7,0);
//	sleep_(50);
//	GpioSetOut(7,1);

	SYS_GPIO7_MUX	=	0;
	GPIO_PIN7_OEN	=	0;
	GPIO_PIN7_OUT	=	1;
	sleep_(5000);
	GPIO_PIN7_OUT	=	0;
	sleep_(5000);
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

	sleep_(100);						//	LCD Fifo Clear Delay

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
//	sleep_(120);
//	LCD_INSDAT_Write(0x29000000,0,0,0,1);
//	sleep_(20);
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
	sleep_(12000);
	LCD_INSDAT_Write(0x29000000,0,0,0,1);
	sleep_(2000);
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
	sleep_(12000);
	LCD_INSDAT_Write(0x29000000,0,0,0,1);
	sleep_(2000);
	LCD0_INSTPARA0w(0x2c);

	#endif

	sleep_(5000);
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
	WDR_ADR_LEw(DDR_BASE_ISP+0x000000);
	FRC_ADR0w(DDR_BASE_ISP+0x028000);
	FRC_ADR1w(DDR_BASE_ISP+0x050000);
	FRC_ADR2w(DDR_BASE_ISP+0x078000);
	FRC_ADR3w(DDR_BASE_ISP+0x0a0000);
	FRC_ADR4w(DDR_BASE_ISP+0x0c8000);
	ENC_ADR0w(DDR_BASE_ISP+0x0f0000);
	ENC_ADR1w(DDR_BASE_ISP+0x101000);
	ENC_ADR2w(DDR_BASE_ISP+0x112000);
	ENC_ADR3w(DDR_BASE_ISP+0x123000);
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
}

#endif
