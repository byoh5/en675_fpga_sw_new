#include "dev.h"

#ifdef __SENSOR__


void Isp_irq_init(void)
{
	//	For VLOCKO Interrupt
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PostSync_Config(1, 0, 2200, 1125, 0x3a, 0x2, 0x6, 0x4, 1928, 1088, 1);	// VLOCK
	Isp_VLOCKO_init();						//	Post Sync
}

void Isp_SensorRst(void)
{
	Wait_VLOCKO();
	Wait_VLOCKO();

	//	Sensor Init
#if model_Sens==SENS_OV2718
	PCKO_SELw(3);		//	18.5 MHz
	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	GpioSetOut(SENSOR_RESET_GPIO_CH,0);
	Wait_VLOCKO();
#else
	GpioSetOut(SENSOR_RESET_GPIO_CH,0);
#endif

	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	GpioSetOut(SENSOR_RESET_GPIO_CH,1);
	Wait_VLOCKO();
}

void Isp_Sensor_init(void)
{
	//----------------------------------------------------------------------------------------
	Isp_SensorRst();

	IspSDesPowerOn();
	IspSensorPowerOn();

	IspSDesConfig();
	IspSDesDelay();
	IspSDesPosition();

	IspSYNC_CODE();

	void printf_SensorSetting(void); printf_SensorSetting();
	InitSensRun();

#if (model_Sens==SENS_IMX291)
	PRE_GROFSw(0x3c4);
	PRE_GBOFSw(0x3c4);
	PRE_ROFSw(0x3c4);
	PRE_BOFSw(0x3c4);
#endif

	_printf("ISP Sync & Sensor configuration...\r\n");
}

void Isp_PrePost_init(void)
{
	IspPreClkConfig();
	IspPostClkConfig();
	IspPreSyncConfig();
	IspPostSyncConfig();

#if (model_Sens==SENS_OV2718)
	ASYNC_DSYNCw(1);
#endif

#if (model_Sens==SENS_IMX291) || (model_Sens==SENS_OV2718)
	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	SYNC_UPw(1);
#endif

	_printf("ISP Clk/Res configuration...\r\n");
}

void Isp_Output_init(void)
{
	//	Digital Output Configuration----------------------------------------------------------
	//IspDout0SyncConfig();
	//IspDout1SyncConfig();

	//	Digital Output Configuration
	Isp_DigOut_Config(IFO_OPIN_0TO15,  IFO_BT1120_16BIT, IFO_ISP_SYNC, IFO_1080_30P, IFO_ISP_PATH, 0, 0);		//	For HDMI (PinList, Dout Mode, Read Sync, Resolution, Output Source Path, Vsp, Hsp)

	Isp_DigOut_Config(IFO_OPIN_16TO31, IFO_BT1120_16BIT, IFO_ISP_SYNC, IFO_1080_30P, IFO_ISP_PATH, 0, 0);		//	For HD-SDI (PinList, Dout Mode, Read Sync, Resolution, Output Source Path, Vsp, Hsp)
//	Isp_DigOut_Config(IFO_OPIN_24TO31, IFO_BT1120_8BIT_SDR, IFO_ISP_SYNC, IFO_1080_30P, IFO_ISP_PATH, 0, 0);		//	For HD-SDI (PinList, Dout Mode, Read Sync, Resolution, Output Source Path, Vsp, Hsp)


	Isp_Dout_Clock1_Config(FN_ON, CLK_74M, 0, CLK_INVERSE);														//	DCKO1 Pin Clock Out	->	FPGA HDSDI	(Clken, ClkFrq, ClkDelay, ClkInv)
//	Isp_Dout_Clock1_Config(FN_ON, CLK_148M, 0, CLK_INVERSE);													//	DCKO1 Pin Clock Out	->	FPGA HDSDI	(Clken, ClkFrq, ClkDelay, ClkInv)
	Isp_Dout_Clock0_Config(FN_ON, CLK_74M, 0, CLK_INVERSE);														//	DCKO0 Pin Clock Out	->	FPGA HDMI

#ifdef	CVBS_OUTPUT
	Isp_Cvbs_Config(FN_ON, NTSC, FREQ_27M, ISP_74M, DS_ISP_FONT_PATH, NO_VLCBIT, 0x7a, 0xf);
#endif

	_printf("Output configuration...\r\n");
}

void Isp_Function_init(void)
{
	//	Isp Function Configuration------------------------------------------------------------
	InitAe();
	IspAgcSet();

	//DF_DETHSPw(RP(DF_HSP));
	//DF_DETHEPw(RP(FR_HW) + RP(DF_HSP) - 1);
	//DF_DETVSPw(RP(DF_VSP));
	//DF_DETVEPw(RP(FR_VW) + RP(DF_VSP) - 1);

	InitContrast();

	InitAwb();

	InitIMD();

	BOX_HSP0w(RP(BOX_HSP));
	BOX_VSP0w(RP(BOX_VSP));
	PrivacyBox();						// Box Privacy

	HlMask();							// High light mask

	Isp_Gamma_Config(1,1);				//	Y/C Gamma On
	Isp_Edge_Config(1);					//	Edge Enhancement On

	//CHLPF_ONw(1);
	//CHLPF_SELw(1);

	FontSetColor(ISP_FONT_GREEN, ISP_FONT_WHITE, ISP_FONT_GRAY, ISP_FONT_YELLOW);
	//FontSetSize(BIG_FONT,BIG_FONT_AREA,BORDER_THICK);

	//FontCharInit(gnFontChar0, ARRAY_SIZE(gnFontChar0));		//	Write Font Data
	//FontClrAll(NO_ALPHA,MN_WHITE);


	FORCE_ABT_SOFFw(1);

	//	Dnr3d On
#ifdef	USE_DNR3D
	Isp_Dnr3d_Config(FN_ON, 0x80, 0x40, 0x20);
#endif
	//Isp_Dnr2d_Config(FN_ON, DNR2D_SUM_MOD, DNR2D_CNT8, 0x38, 0x30);
	Isp_Dnr2d_Config(FN_ON, SP(Dnr2dICSel), SP(Dnr2dOCSel));
	//Isp_Defect_Config(FN_ON, DF_SUM_6, DF_SUM_4, DF_WGT_CASEB, DF_WGT_CASEB, DF_SLOPE_NOR, DF_GTHRES, DF_RBTHRES, DF_MAX, DF_MIN, 3);


	_printf("ISP Function configuration...\r\n");
}

void Isp_DDR_init(void)
{
	//	DDR Init---------------------------------------
	Isp_Ddr_Cong();

	//FIFO_ROSELw(1);
	//WR_MSELw(1);
	RD_MODw(0);
	AXI_IDSw(0);


//	bus_init();											//	Bus Monitor Init

#ifdef	USE_FRC
	DDR_RDNR_LTCw(0x3a0);
	DDR_RWDR_LTCw(0x500);
  #if	model_4M
	DDR_RFRC_LTCw(0x620);
  #elif	model_8M
	DDR_RFRC_LTCw(0xf80/*0xe00*/);
  #else
	DDR_RFRC_LTCw(0x200);
  #endif

	DDR_RENC_LTCw(0x300);
	DDR_RYC_LTCw(0x500);

//	RD_LTC7w(0x20);
//	RD_LTC8w(0x20);
//	RD_LTC9w(0x20);
//	RD_LTC10w(0x20);
	RD_LTC11w(0xf0);
	RD_LTC12w(0xf0);

	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	SD_MODw(0);			// DDR OFF

	RD_LTC7w(4);
	RD_LTC8w(4);
	RD_LTC9w(4);
	RD_LTC10w(4);
	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	Wait_VLOCKO();
	CPU_FRC_ENw(1);		// DDR OFF
	BUS_RD_RSTw(1);
#endif
}

void Isp_Digital_input_init(void)
{
	//	External Digital Input Configuration----------------------------------------
	Isp_DS_Step_Config(0x40);														//	Down-Scale Ratio Setting
	Isp_Pip_Clk_Config(1, CLK_74M, PIP_MAIN_ISP, PIP_ISP_SYNC, PIP_NO_MMASK);		//	PIP Main Setting
	Isp_Vlc_Config(VLC_6BIT);

#ifdef	USE_DIG_CH2		//	Digital Input Channel 2 (BT1120 YCMuxed 8Bit)
	_printf("Digital Channel2 Input  ---------> \r\n");
	Isp_DigIn_CH2_Config(IF_BT1120_8BIT_SDR, 0, IF_NEGEDGE, 4, 1920, 0, 1080, IF_MODE_SET);							//	Digital Channel 2 Setting		(Input Mode, Input Clock Delay, Clock Latch Position, Hsp, Hw, Vsp, Vw, On/Off)
  #ifdef	USE_WRCH2_DS1_RDCH2_PIP0
	Isp_DS1_Config(DS_DIGITAL_CH2_PATH, CLK_DIG_CH2_DIV2, 0x80, 0x80, 1920, 1080, LPF_LV3, LPF_LV3, 0, 0, FN_ON);	//	Down-Scale 1 Setting
	Isp_DS1_Edge_Enhance_Config(FN_ON, 0x40, 0x10, 0xe0, DS_APT_ROI_OFF, 0, 0, 0, 0, 0, DS_ROI_OSD_OFF);			//	Down-Scale Aperture Setting
//	Isp_WrCh2_FrcAdr(0x82b4000, 0x82f4000, 0x8314000, 0x8354000, 0x8374000, 0x83b4000);								//	DDR Write Address Setting
	Isp_WrCh2_Config(WR_CH_DOWN_SCALER1, 960, WR_DIG_CH2_SYNC, WR_COLOR, NO_INTERLACE, NO_VLCBIT, CLK_DIG_CH2_DIV2, USE_FRC, 0, DDR_WR_FIRST, DDR_RDCH2, IF_MODE_SET);	//	DDR Write Channel Setting -> Down-Scale Image
//	Isp_WrCh2_Config(WR_CH_DOWN_SCALER1, 960, WR_DIG_CH2_SYNC, WR_COLOR, NO_INTERLACE, VLC_6BIT, CLK_DIG_CH2_DIV2, USE_FRC, 0, DDR_WR_FIRST, DDR_RDCH2, IF_MODE_SET);	//	DDR Write Channel Setting -> Down-Scale Image

	Isp_WrCh2_Wr(WR_WRITE_CONTINU_MODE, FN_ON, FN_ON);																		//	Start Write...
	Isp_Pip_Ch0_Config(FN_ON, PIP_DDR_RD_CH2, 0x42, 0x226, 960, 540, PIP_LINE_ON, LINE_GREEN, PIP_MIX_100, 0x600);

	Isp_RdCh2_Config(RD_CH_PIP0, 960, WR_COLOR, CLK_74M, RD_ISP_SYNC, NO_VLCBIT, USE_FRC, DDR_WR_FIRST, DDR_WRCH2, IF_MODE_SET);		//	DDR Read Channel Settingg
//	Isp_RdCh2_Config(RD_CH_PIP0, 960, WR_COLOR, CLK_74M, RD_ISP_SYNC, VLC_6BIT, USE_FRC, DDR_WR_FIRST, DDR_WRCH2, IF_MODE_SET);		//	DDR Read Channel Settingg
  #endif
#endif

#ifdef	USE_DIG_CH3		//	Digital Input Channel 3 (BT1120 YCMuxed 8Bit)
	_printf("Digital Channel3 Input---------> \r\n");
	Isp_DigIn_CH3_Config(IF_BT1120_8BIT_SDR, 4, IF_POSEDGE, 4, 1920, 0, 1080, IF_MODE_SET);							//	Digital Channel 2 Setting	(Input Mode, Input Clock Delay, Clock Latch Position, Hsp, Hw, Vsp, Vw, On/Off)
  #ifdef	USE_WRCH3_DS2_RDCH3_PIP1
	Isp_DS2_Config(DS_DIGITAL_CH3_PATH, CLK_DIG_CH3_DIV2, 0x80, 0x80, 1920, 1080, LPF_LV3, LPF_LV3, 0, 0, FN_ON);	//	Down-Scale 2 Setting
	Isp_DS2_Edge_Enhance_Config(FN_ON, 0x40, 0x10, 0xe0, DS_APT_ROI_OFF, 0, 0, 0, 0, 0, DS_ROI_OSD_OFF);			//	Down-Scale Aperture Setting
//	Isp_WrCh3_FrcAdr(0x8194000, 0x81d4000, 0x81f4000, 0x8234000, 0x8254000, 0x8294000);								//	DDR Write Address Setting
	Isp_WrCh3_Config(WR_CH_DOWN_SCALER2, 960, WR_DIG_CH3_SYNC, WR_COLOR, NO_INTERLACE, NO_VLCBIT, CLK_DIG_CH3_DIV2, USE_FRC, 0, DDR_WR_FIRST, DDR_RDCH3, IF_MODE_SET);	//	DDR Write Channel Setting -> Down-Scale 2 Image
//	Isp_WrCh3_Config(WR_CH_DOWN_SCALER2, 960, WR_DIG_CH3_SYNC, WR_COLOR, NO_INTERLACE, VLC_6BIT, CLK_DIG_CH3_DIV2, USE_FRC, 0, DDR_WR_FIRST, DDR_RDCH3, IF_MODE_SET);	//	DDR Write Channel Setting -> Down-Scale 2 Image

	Isp_WrCh3_Wr(WR_WRITE_CONTINU_MODE, FN_ON, FN_ON);																		//	Start Write...
	Isp_Pip_Ch1_Config(FN_ON, PIP_DDR_RD_CH3, 0x3fe, 0x226, 960, 540, PIP_LINE_ON, LINE_BLUE, PIP_MIX_100, 0x700);

	Isp_RdCh3_Config(RD_CH_PIP1, 960, WR_COLOR, CLK_74M, RD_ISP_SYNC, NO_VLCBIT, USE_FRC, DDR_WR_FIRST, DDR_WRCH3, IF_MODE_SET);			//	DDR Read Channel Setting
//	Isp_RdCh3_Config(RD_CH_PIP1, 960, WR_COLOR, CLK_74M, RD_ISP_SYNC, VLC_6BIT, USE_FRC, DDR_WR_FIRST, DDR_WRCH3, IF_MODE_SET);			//	DDR Read Channel Setting
  #endif
#endif

	_printf("Digital Input configuration...\r\n");
}

void OutMode(void)
{
#if 0
	static BYTE bOutFps = 0xee;
	static BYTE bSysFreq = 0xee;

	if(bOutFps!=UP(OutFps) || bSysFreq!=UP(SysFreq)) {
#endif
		extern UINT gnVDI_CHG;
		gnVDI_CHG = 2;

		FreqAdjust();				// FPS_VDO & FPS_VDI 및 gnAeVtw & gnAeHtw 설정

		Isp_PrePost_init();			// ISP Pre & Post의 Clk과 Res 설정

		IspDout0SyncConfig();
		IspDout1SyncConfig();
		//Isp_Output_init();			// Output 설정

#if 0
		bOutFps = UP(OutFps);
		bSysFreq = UP(SysFreq);
	}
#endif
}

void Isp_init(void)
{	// The execution order of the functions is important !!!

	InitDataSet();				// Data setting for initialization
	AppLoadPar();				// load parameter from memory
	InitMenu();					// If hold down a specific key, reset the user parameters.

	Isp_irq_init();				// Enable External Interrupts & Wait_VLOCKO() 사용을 위한 임시 설정

	InMode();

	Isp_Sensor_init();			// ISP Sync 설정 및 Sensor Initial

	FONT_INIT();				// SetFontChg() 보다 먼저 실행되어야 함
	InitUsrParChgAll();			// SensFlip(), SensMirror() 실행을 위해 Isp_Sensor_init()이 먼저 설정되어야 함
	ChangeMenuSize();

	OutMode();

	Isp_Output_init();			// Output 설정

	Isp_Function_init();		// OSD Font 및 ISP 기능 설정

	Isp_DDR_init();				// ISP 에서 사용하는 DDR 설정

	Isp_Digital_input_init();	// Digital Input configuration

	YCW_DCK2_PDw(0);			// Clock enable ->  isp_clk.h의 YCW_DCK2_SET(2) 함수로 대치 가능.
	YCW_DCK2_SELw(2);			// TODO KSH ◆ WDR 사용 시 꼭 필요한지 확인 필요
	YCW_DCK2_PDw(1);

	//extern void ParFncTest(void); ParFncTest();		// TODO KSH ◆ ParFncTest()

	VIRQO_ENw(1);				// VLOCKO IRQ Routine Test, enx_exirq_source1() 함수에서 CLI_VLOCKOw(1) 실행됨
	VIRQI_ENw(1);				// Sensor FPS 출력용
}

void Hdmi_Check(void)
{
#ifdef __HDMI__
	BYTE DetFmt0[2] = {0,0};

	Hdmi(DetFmt0);
	WriteByteTPI(0x63,0x40);
//	_printf("HDMI Detect format..\r\n");
#endif
}

void isp_DispLogo(void)
{
	const BYTE bLogo[][14] = {{142,143,146,147,150,151,168,169,172,173,176,177,180,181},
							  {144,145,148,149,152,153,170,171,174,175,178,179,182,183}};

	FontStr(0,0,(const char*)bLogo[0],14);
	FontStr(1,0,(const char*)bLogo[1],14);
}

void isp_DispTime(void)
{
	static UINT VO_CNT = 0;
	VO_CNT++;

	static ULONG isp_main_first_time = 0;
	if(isp_main_first_time==0) isp_main_first_time = BenchTimeStart();
	const UINT nCur_ms = BenchTimeStop(isp_main_first_time)/1000;

	static ULONG isp_main_start_time = 0;
	const UINT isp_main_call_time = BenchTimeStop(isp_main_start_time);
	isp_main_start_time = BenchTimeStart();
	if(isp_main_start_time) FontDecEx(3, 4, NO_ALPHA, MN_YELLOW, isp_main_call_time, 6, 1);
	FontStr(3,0,"VLO",3);
	FontStr(3,10,"us",2);

	extern UINT gnVDI_4FPS;
	const UINT nFPS_Float = gnVDI_4FPS&3;
	FontStr(4,0,"VDI",3);
	FontDecEx(4, 4, NO_ALPHA, MN_YELLOW, gnVDI_4FPS>>2, 3, 1);
	FontCharEx(4,7, NO_ALPHA, MN_YELLOW, '.');
	FontStrEx(4,8, NO_ALPHA, MN_YELLOW, (nFPS_Float==0) ? "00": (nFPS_Float==1) ? "25" : (nFPS_Float==2) ? "50" : "75", 2);
	FontStr(4,10,"FPS",3);

	#define DISP_TIME_Y		17
	#define DISP_TIME_X		0

	//FontStr(DISP_TIME_Y, DISP_TIME_X, "TIME",4);

	extern UINT gnVoIrqCnt;
	FontStrEx(DISP_TIME_Y+1, DISP_TIME_X,    NO_ALPHA, MN_GREEN,  "IRQ",3);
	FontDecEx(DISP_TIME_Y+1, DISP_TIME_X+3,  NO_ALPHA, MN_YELLOW, (gnVoIrqCnt*1000)/FPS_VDO, 10, 1);
	FontStr(DISP_TIME_Y+1, DISP_TIME_X+13, "ms",2);

	FontStrEx(DISP_TIME_Y+2, DISP_TIME_X,    NO_ALPHA, MN_GREEN,  "CPU",3);
	FontDecEx(DISP_TIME_Y+2, DISP_TIME_X+3,  NO_ALPHA, MN_YELLOW, nCur_ms, 10, 1);
	FontStr(DISP_TIME_Y+2, DISP_TIME_X+13, "ms",2);

	FontStrEx(DISP_TIME_Y+3, DISP_TIME_X,    NO_ALPHA, MN_GREEN,  "VLO",3);
	FontDecEx(DISP_TIME_Y+3, DISP_TIME_X+3,  NO_ALPHA, MN_YELLOW, (VO_CNT*1000)/FPS_VDO, 10, 1);
	FontStr(DISP_TIME_Y+3, DISP_TIME_X+13, "ms",2);
}

#if 0
void isp_LedCtrl(void)
{
	static UINT LED_FLG = 0;

	//	LED Display when it operated
	if(LED_FLG<10) { GPIO_PIN55_OUT = 1; /*FontStrEx(0,gnFontXw-5,NO_ALPHA,MN_GREEN,"EN675",5);*/ } // 10 Frames On		// TODO KSH> GPIO_IR_LED 에서 GPIO 55 사용
	else		   { GPIO_PIN55_OUT = 0; /*FontClr(0,gnFontXw-5,5);*/                         } // 10 Frames Off		// "

	LED_FLG = (LED_FLG<20) ? (LED_FLG+1) : 0;
}
#endif

void IF_Funcs(void)
{
	const ULONG IspIfTimeSta = rdcycle();
	static ULONG IF_Funcs_Time = 0;

	if(ISP_RIRQ_VOr) {			// VLOCKO 에 동기화하여 실행
		CLI_VLOCKO_Tw(1);
		isp_DispTime();
		isp_DispLogo();
		//isp_LedCtrl();
		//DebugDisp2(1, Dec, "UPTEST0", 25, 0, gbUsrParTbl[UPi(LckDly)], 4)
		//DebugDisp2(1, Dec, "UPTEST1", 26, 0, UpList->LckDly, 4)
		//DebugDisp2(1, Dec, "UPTEST2", 27, 0, UpListEx->Sensor.LckDly, 4)
		//DebugDisp2(1, Dec, "UPTEST0", 25, 0, gbUsrParTbl[UPi(CamTitle2)], 4)
		//DebugDisp2(1, Dec, "UPTEST1", 26, 0, UpList->CamTitle2, 4)
		//DebugDisp2(1, Dec, "UPTEST2", 27, 0, UpListEx->MENU_SYSTEM.CamTitle2, 4)
		//DebugDisp2(1, Dec, "UPTEST0", 25, 0, gbUsrParTbl[UPi(Sensor_FPS)], 4)
		//DebugDisp2(1, Dec, "UPTEST1", 26, 0, UpList->Sensor_FPS, 4)
		//DebugDisp2(1, Dec, "UPTEST2", 27, 0, UpListEx->ISP_BINARY_INFO.Sensor_FPS, 4)
		//DebugDisp2(1, Dec, "UPTEST0", 25, 0, gbUsrParTbl[UP_END], 4)
		//DebugDisp2(1, Dec, "UPTEST1", 26, 0, UpList->End, 4)
		//DebugDisp2(1, Dec, "UPTEST2", 27, 0, UpListEx->End, 4)
		//DebugDisp2(1, Dec, "UPTEST0", 25, 0, gbMnPvcCfg(0)->bAction, 4)
		//DebugDisp2(1, Dec, "UPTEST1", 26, 0, UpList->PVC0.bAction, 4)
		//DebugDisp2(1, Dec, "UPTEST2", 27, 0, UpListEx->PVC0.bAction, 4)
		DebugDisp2(1, Dec, "UPTEST0", 25, 0, gbMnImdCfg(3)->bSizY, 4)
		DebugDisp2(1, Dec, "UPTEST1", 26, 0, UpList->IMD3.bSizY, 4)
		DebugDisp2(1, Dec, "UPTEST2", 27, 0, UpListEx->IMD3.bSizY, 4)

#if 0	// ddr write test
		const int iX = B30_HSPr;
		const int iY = B30_VSPr;
		//const UINT nAddrY = (FRC_ADR0r<<4) + ((((RP(PO_HW) * iY) + iX)*10)>>3);
		const UINT nAddrY = (IM_YADR0r<<4) + (RP(PO_HW) * iY) + iX;
		const UINT nAddrC = (IM_CADR0r<<4) + ((RP(PO_HW)>>1) * iY) + (iX>>1);

		hwflush_dcache_range(nAddrC, 64);
		hwflush_dcache_range(nAddrY, 64);

		const BYTE bDdrY = *((volatile BYTE *)(unsigned long)(nAddrY));
		const BYTE bDdrC = *((volatile BYTE *)(unsigned long)(nAddrC));

		DebugDisp2(1, Hex, "DDR YC ", 13, 0, bDdrY/*(bDdrY<<8) | bDdrC*/, 4)
		DebugDisp2(1, Hex, "ADDR Y ", 14, 0, nAddrY, 8)
		DebugDisp2(1, Dec, "B30_HSP", 15, 0, iX, 4)
		DebugDisp2(1, Dec, "B30_VSP", 16, 0, iY, 4)
#endif
	}
	else if(ABSDIFF(IF_Funcs_Time,IspIfTimeSta) < (CPU_FREQ/IF_FUNC_FPS)) return;


//	FontStr(4,0,"ADC1",4);
//	FontDec(4,5,AdcGet(1), 5, 1);

//	extern UINT gnNO_IRQ_UART_SPI_I2C;
//	FontStr(5,0,"NO IRQ",6);
//	FontDec(5,7,gnNO_IRQ_UART_SPI_I2C, 6, 1);

//	FontDecEx(4+0,0, NO_ALPHA, MN_YELLOW, AdcGet(0), 5, 1);
//	FontDecEx(4+1,0, NO_ALPHA, MN_YELLOW, AdcGet(1), 5, 1);
//	FontDecEx(4+2,0, NO_ALPHA, MN_YELLOW, AdcGet(2), 5, 1);
//	FontDecEx(4+3,0, NO_ALPHA, MN_YELLOW, AdcGet(3), 5, 1);
//	FontDecEx(4+4,0, NO_ALPHA, MN_YELLOW, AdcGet(4), 5, 1);
//	FontDecEx(4+5,0, NO_ALPHA, MN_YELLOW, AdcGet(5), 5, 1);
//	FontDecEx(4+6,0, NO_ALPHA, MN_YELLOW, AdcGet(6), 5, 1);
//	FontDecEx(4+7,0, NO_ALPHA, MN_YELLOW, AdcGet(7), 5, 1);

//	static UINT nPAR00 = 0;
//	if(nPAR00 != PAR00) {
//		nPAR00 = PAR00;
//		extern void UartClk(UINT nCH, UINT Speed_Hz, int ofs);
//		UartClk(7, UART7_SPEED, (nPAR00 > 100) ? 100 - (int)nPAR00 : (int)nPAR00);
//	}

	Comm(); 								// Communication Interface

	KeyScan();								// Key input detection

//	static int iUD = 0;
//	static int iLR = 0;
//	if(KEY_U) iUD++;
//	if(KEY_D) iUD--;
//	if(KEY_R) iLR++;
//	if(KEY_L) iLR--;
//	FontDec(6,5, NO_ALPHA, MN_WHITE, iUD, 5, 1);
//	FontDec(7,5, NO_ALPHA, MN_WHITE, iLR, 5, 1);


	Menu(); 								// Menu

	AppSavePar();
	AppLoadPar();

	if(!gbMnDebugBypass) {

//		Focus;								// Auto Focus or Focus Assist

//		TDN();								// Day & Night

		Gamma();							// Gamma control

//		BoxLast();							// Last Box control
	}

	Hdmi_Check();	// VLOCKO 에 동기화될 필요 없이 가끔 한번씩 실행하면 됨

	IF_Funcs_Time = rdcycle();

	FontBenchTime(1/*gbMnDebugFnc==1*/, 23, 0, "ISP_I", IspIfTimeSta, 6);
}

void isp_main(void)
{
	const ULONG IspMainTimeSta = BenchTimeStart();

	if(!gbMnDebugBypass) {

		Ae();								// Auto exposure

		Af();								// Auto exposure

		AceDefog(); 						// ACE & Defog

		Awb();								// Auto white balance

		Adnr();								// DNR 2D/3D

		Sharpness();						// Sharpness (aperture)

		CSup(); 							// Color surpression

		IMD();								// Motion detector

		DefectAuto();						// Defect correction

		BoxLast();							// Last Box control

		UartTxGrpRun();

		AeDev();
	}

	FontBenchTime(1/*gbMnDebugFnc==1*/, 22, 0, "ISP_M", IspMainTimeSta, 6);
}

#endif
