#include "dev.h"

#ifdef __SENSOR__


#if 0
void Isp_irq_init(void)
{
	//	For VLOCKO Interrupt
	Isp_PostClk_Config(ISP_CLK_74M);		//	Isp Post Module Clock
	Isp_PostSync_Config(1, 0, 2200, 1125, 0x3a, 0x2, 0x6, 0x4, 1928, 1088, 1);	// VLOCK
	Isp_VLOCKO_init();						//	Post Sync
}
#endif

void Isp_SensorRst(void)
{
	INIT_DELAY(2);

	//	Sensor Init
#if model_Sens==SENS_OV2718
	PCKO_SELw(3);		//	18.5 MHz
	INIT_DELAY(3);
	GpioSetOut(SENSOR_RESET_GPIO_CH,0);
	INIT_DELAY(1);
#else
	GpioSetOut(SENSOR_RESET_GPIO_CH,0);
#endif

	INIT_DELAY(4);
	GpioSetOut(SENSOR_RESET_GPIO_CH,1);
	INIT_DELAY(1);
}

void Isp_Sensor_init(void)
{
	//----------------------------------------------------------------------------------------
	Isp_SensorRst();

	IspSDesPowerOn();
	IspSensorPowerOn();

	IspSDesConfig();
	IspSDesDelay();

	IspSYNC_CODE();

	INIT_STR_SENSOR
	InitSensRun();

#if (model_Sens==SENS_IMX291)
	#define SENSOR_BLACK_LEVEL	0xf0	// 포화영역 경계부분 노이즈 감소
#elif (model_Sens==SENS_IMX323)
	#define SENSOR_BLACK_LEVEL	0x3c	// 암부영역 컬러노이즈(자주색) 감소, 미적용 시 WDR Short 채널에 자주색 발생
#else
	#define SENSOR_BLACK_LEVEL	0x0
#endif

	#define ISP_OFFSET	(1024-((SENSOR_BLACK_LEVEL)>>2))	// ※EN673은 >>2 하지 않음

	PRE_GROFSw(ISP_OFFSET);
	PRE_GBOFSw(ISP_OFFSET);
	PRE_ROFSw(ISP_OFFSET);
	PRE_BOFSw(ISP_OFFSET);

	INIT_STR("ISP Sync & Sensor configuration...");
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
	INIT_DELAY(3);
	SYNC_UPw(1);
#endif

	INIT_STR("ISP Clk/Res configuration...");
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

	INIT_STR("Output configuration...");
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

	INIT_STR("ISP Function configuration...");
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
	#if model_1M
		#define R_LTC	0x260
	#elif model_4M
		#define R_LTC	0xa40
	#elif model_8M
		#define R_LTC	0xf80
	#else// model_2M & model_2M30p
		#define R_LTC	0x500
	#endif

	DDR_RDNR_LTCw(R_LTC);
	DDR_RWDR_LTCw(R_LTC);
	DDR_RFRC_LTCw(R_LTC);
	DDR_RYC_LTCw(R_LTC);
	DDR_RENC_LTCw(0x300);	// ENC 는 고정

	//INIT_DELAY(6);
	SD_MODw(0);			// DDR OFF

	INIT_DELAY(1/*4*/);
	CPU_FRC_ENw(1);		// DDR OFF,  SD_MODw(0) 이후 1 VLOCK Delay 후 설정해야 함!!!
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
	INIT_STR("Digital Channel2 Input  --------->");
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
	INIT_STR("Digital Channel3 Input--------->");
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

	INIT_STR("Digital Input configuration...");
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

#if 0
void isp_init_test(void)
{
	#define DDR0_BASE	0x80000000
	#define DDR1_BASE	0x90000000

	const UINT nHW = RP(PO_HW)/*416*/;
	const UINT nVW = RP(PO_VW)/*240*/;
	const UINT nHTW = RP(FR_HTW60)-2/*416+150*/;
	const UINT nVTW = RP(FR_VTW60)-1/*240+16*/;

	const UINT nWdrAddr = DDR1_BASE;
	const UINT nFrc0Addr = nWdrAddr  + ((nHW*nVW*10)>>3);
	const UINT nFrc1Addr = nFrc0Addr + ((nHW*nVW*10)>>3);
	const UINT nDdrAddrY = nFrc1Addr + ((nHW*nVW*10)>>3);
	const UINT nDdrAddrC = nDdrAddrY + (nHW*nVW);

	// Isp_PreClk_Config(SP(PreClk), USE_FRC);
	BT_PCK_PDw(0);
	FPCK_PDw(0);
	BT_PCK_SELw(ISP_CLK_74M);
	FPCK_SELw(ISP_CLK_74M);
	BT_PCK_PDw(1);
	FPCK_PDw(1);

	YCW_DCK2_PDw(0);
	YCW_DCK2_SELw(ISP_CLK_74M);
	YCW_DCK2_PDw(1);	// WDR 사용 시 필요

	// Isp_PreSync_Config(0, nHTW+2, nVTW+1, SP(PreHSyncOfs), SP(PreVSyncOfs), SP(PreHsp), SP(PreVsp), nHW, nVW, SP(IsASync), SP(IsNSync), SP(PreHSyncPol), SP(PreVSyncPol));
	PRS_HZw(1);

	HSPIw(0/*SP(PreHsp)*/);
	VSPIw(2/*SP(PreVsp)*/);	// WDR 사용 시 2 이상 설정
	HWIw(nHW);
	VWIw(nVW);

	HTWIw(nHTW);
	VTWIw(nVTW);
	SLVw(0);

	//VLOCKI_POSw(SP(PreVSyncOfs));
	//HLOCKI_POSw(SP(PreHSyncOfs));

	//ASYNC_ONw(SP(IsASync));
	//VSYN_NAONw(SP(IsNSync));
	PRS_HZw(0);

	//POL_HSIw(SP(PreHSyncPol));
	//POL_VSIw(SP(PreVSyncPol));


	// Isp_PostClk_Config(ISP_CLK_74M);
	PSCK_SELw(2);
	PPCK_SELw(2);
	FN_CK0_SELw(2);
	PR_CK0_SELw(2);
	PSCK_PDw(1);
	PPCK_PDw(1);
	PR_CK0_PDw(1);
	FN_CK0_PDw(1);

	// Isp_PostSync_Config(1, 0, nHTW+2, nVTW+1, 0x3a, 0x2, 0x6, 0x4, nHW, nVW, SP(OCSel))
	POS_HZw(1);
	HVWIO_SYNw(0);
	HWO_1D_ONw(0);
	//HSPOw(0x6);
	//VSPOw(0x4);
	HWOw(nHW);
	VWOw(nVW);
	HTWOw(nHTW);
	VTWOw(nVTW);
	OSYNC_MODw(1);
	//VLOCKI2_POSw(0x2);
	//HLOCKI2_POSw(0x3a);
	//EXVSYNC_SELw(0);
	//EXHSYNC_SELw(0);
	POS_HZw(0);

	Isp_Output_init();

	//YC_OSELw(0x11);
	SYNC_BYSw(1);
	INSELw(0x6);


  #if 1	// ddr write test
	YCW_CK0_SELw(2);
	YCW_CK0_PDw(1);
	IM_YADR0w(nDdrAddrY>>4);
	IM_CADR0w(nDdrAddrC>>4);
	IM_HWI0w(nHW);
	IM_IVSEL0w(0);
	IM_ISEL0w(0xC);
	IM_GO0w(1);
  #endif

	// Isp_Dnr3d_Config(FN_ON, 0x80, 0x40, 0x20);
	DNR3D_FKw(0x80);
	DNR3D_THw(0x40);
	DNR3D_GAw(0x20);

	DNR3D_RCH_ENw(1);
	YCR_CK4_PDw(0);
	YCRCK4_SELw(ISP_CLK_74M);
	YCR_CK4_PDw(1);
	DNR3D_ONw(1);

	// WDR 설정
	WDR_CSELw(1);
	WDR_LGAINw(0x100);
	WDR_STEPw(4);

	ACE_DTHw(0x7);
	ACE_TH1w(0);
	ACE_TH2w(0);
	ACE_GMGNw(0);
	HEQ_WGw(1);

	WDR_GAINw(0x2008);
	WDR_SGAINw(0x200);
	//WDR_SGAIN2w(0);

	/*WDR_LCLIPRw(0x2a7);
	WDR_LCLIPGw(0x3ff);
	WDR_LCLIPBw(0x221);
	WDR_SCLIPRw(0x2a7);
	WDR_SCLIPGw(0x3ff);
	WDR_SCLIPBw(0x221);*/

	// WDR GAMMA 설정 생략
	// TMG_TBL 설정 생략


	// Isp_DDR_init();
	WDR_ADR_LEw(nWdrAddr>>4);
	FRC_ADR0w(nFrc0Addr>>4);
	FRC_ADR1w(nFrc1Addr>>4);

	RD_MODw(0);
	AXI_IDSw(0);

	DDR_RDNR_LTCw(0x500);
	DDR_RWDR_LTCw(0x500);
	DDR_RFRC_LTCw(0x500);
	DDR_RYC_LTCw(0x500);
	DDR_RENC_LTCw(0x300);	// ENC 는 고정

	SD_MODw(0);			// DDR OFF

  #if 1
	//UartTxStr("★Wait...");
	//INIT_DELAY(1);
	//for(volatile int i=0;i<CPU_FREQ;i++) { __asm("C.NOP"); }	// 1 cycle = 15 clock = 15/CPU_FREQ sec,  컴파일 옵션 : -O0
	VIRQO_EN_Tw(1);
	for(volatile int i=0;i<2;i++) {
		while(!(ISP_RIRQ_VOr&0x1));
		CLI_VLOCKO_Tw(1);
		IM_GO0w(1);
	}
	//UartTxStr("★Run !!!");
  #endif

	CPU_FRC_ENw(1);		// DDR OFF,  SD_MODw(0) 이후 1 VLOCK Delay 후 설정해야 함!!!
	BUS_RD_RSTw(1);


	//INIT_DELAY(10);

	DOL_LBUFS0_ONw(1);
	WDR_ONw(1);
	TMG_ONw(1);

	while (1) {
		while(!(ISP_RIRQ_VOr&0x1));
		CLI_VLOCKO_Tw(1);
		IM_GO0w(1);
		Comm();
	}
}
#endif

void Isp_init(void)
{	// The execution order of the functions is important !!!
	InitDataSet();				// Data setting for initialization
	AppLoadPar();				// load parameter from memory
	InitMenu();					// If hold down a specific key, reset the user parameters.

//	Isp_irq_init();				// Enable External Interrupts & Wait_VLOCKO() 사용을 위한 임시 설정

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

	Isp_VLOCKO_init();			// IF_Funcs() 에서 VLOCKO 에 동기화하여 코드 실행 시 필요(테스트 용), OutMode() -> Isp_PrePost_init() 에서 Post Clk & Sync 설정 후 실행되어야 함, Isp_irq_init() 사용 시 필요 없음

	VIRQO_ENw(1);				// VLOCKO IRQ Routine Test, enx_exirq_source1() 함수에서 CLI_VLOCKOw(1) 실행됨
	VIRQI_ENw(1);				// Sensor FPS 출력용
}

void Hdmi_Check(void)
{
#ifdef __HDMI__
	BYTE DetFmt0[2] = {0,0};

	Hdmi(DetFmt0);
	WriteByteTPI(0x63,0x40);
//	INIT_STR("HDMI Detect format...");
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
