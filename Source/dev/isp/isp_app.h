
//*************************************************************************************************
// isp_tbl.c
//-------------------------------------------------------------------------------------------------
extern void SetSens(WORD awAddr, BYTE abData);
extern BYTE GetSens(WORD awAddr);
extern void SetSensBurst(WORD awAddr, BYTE* apbData, BYTE abLength);

//extern void OV4689_Init(void);
//extern void IMX335_Init(void);
//extern void OS08A10_Init(void);
//extern void OV2718_Init(void);
//extern void IMX291_Init(void);
//extern void IMX274_Init(void);
//extern void IMX225_Init(void);

extern void Isp_VLOCKO_init(void);
extern void Wait_VLOCKO(void);
extern void Isp_VLOCKO1_init(void);
extern void Wait_VLOCKO1(void);
extern void Wait_VLOCKO_M(UINT Cnt);


//*************************************************************************************************
// LCD Function
//-------------------------------------------------------------------------------------------------
extern void Init_LCD_Control(void);


//*************************************************************************************************
// ISP DDR
//-------------------------------------------------------------------------------------------------
extern void Isp_Ddr_Cong(void);


//*************************************************************************************************
// isp_interface.c
//-------------------------------------------------------------------------------------------------
extern void Isp_SDesPowerOn(BOOL OnOff, BOOL IsMipi, BYTE MipiClkPhase);
extern void Isp_SDesDelay(BYTE Lck);
extern void Isp_SensorPowerOn(BOOL OnOff, UINT Clk);
extern void Isp_Parallel_Config(BOOL OnOff/*, BOOL IsSlave*/, BOOL IsUseExtClk, BOOL IsClkDdr, BYTE ClkDly/*, BOOL HSyncPol, BOOL VSyncPol, BOOL SyncMode, BOOL IsUseAsync*/, BYTE BitMode);
extern void Isp_PreClk_Config(BYTE Clk, BOOL Usefrc);
extern void Isp_PreSync_Config(BOOL IsSlave, UINT Htw, UINT Vtw, UINT HsyncOfs, UINT VsyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw, BOOL IsASync, BOOL IsNSync, BOOL HSyncPol, BOOL VSyncPol);
extern void Isp_PostSync_Config(BOOL OSyncMode, BOOL ExSyncSel, UINT Htw, UINT Vtw, UINT HsyncOfs, UINT VsyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw, BYTE OCSel);
extern void Isp_PostClk_Config(BYTE Clk);
extern void Isp_Gamma_Config(BOOL Y_OnOff, BOOL C_OnOff);
extern void Isp_Edge_Config(BOOL OnOff);
extern void Isp_Dnr3d_Config(BOOL OnOff, BYTE Clk, BYTE DnrFk, BYTE DnrTh, BYTE DnrGain);
//extern void Isp_Defect_Config(BOOL OnOff, BOOL GsumCnt, BOOL RBSumCnt, BYTE GWgt, BYTE RBWgt, BYTE DfSlope, UINT GThres, UINT RBThres, UINT DfMax, UINT DfMin, BYTE DfCsel);
//extern void Isp_Dnr2d_Config(BOOL OnOff, BOOL Dnr2dMode, BYTE Dnr2dCnt, BYTE Dnr2dDth, BYTE Dnr2dGain);
extern void Isp_Dnr2d_Config(BOOL OnOff, BYTE Icsel, BYTE Ocsel);
extern UINT Isp_Frc_Adr_Config(UINT Adr, UINT Page, UINT Hw, UINT Vw, UINT Hmargin, UINT Vmargin);
extern UINT Isp_Wdr_Adr_config(UINT Adr, UINT Page, UINT Hw, UINT Vw, UINT Hmargin, UINT Vmargin);
extern UINT Isp_Cvb_Adr_Config(UINT Adr, UINT Hw, UINT Vw);
extern UINT Isp_YC_Adr_Config(UINT Adr, UINT Ch, UINT Page, UINT Hw, UINT Vw, UINT Hmargin, UINT Vmargin);

//*************************************************************************************************
// digital_interface.c
//-------------------------------------------------------------------------------------------------
extern UINT Isp_DigIn_CH0_Config(UINT iMode, UINT ClkDly, UINT RegLatchPos, UINT Hsp, UINT Hw, UINT Vsp, UINT Vw, BOOL Set);
extern UINT Isp_DigIn_CH1_Config(UINT iMode, UINT ClkDly, UINT RegLatchPos, UINT Hsp, UINT Hw, UINT Vsp, UINT Vw, BOOL Set);
extern UINT Isp_DigIn_CH2_Config(UINT iMode, UINT ClkDly, UINT RegLatchPos, UINT Hsp, UINT Hw, UINT Vsp, UINT Vw, BOOL Set);
extern UINT Isp_DigIn_CH3_Config(UINT iMode, UINT ClkDly, UINT RegLatchPos, UINT Hsp, UINT Hw, UINT Vsp, UINT Vw, BOOL Set);
extern UINT Isp_DigOut_Config(UINT PinList, UINT Mode, UINT Rsync, UINT Res, UINT Path, UINT Hsp, UINT Vsp);
extern void Isp_Dout_Clock0_Config(BOOL Dcko, BYTE DckoClk, BYTE Dly, BOOL ClkoInv);
extern void Isp_Dout_Clock1_Config(BOOL Dcko, BYTE DckoClk, BYTE Dly, BOOL ClkoInv);
extern void Isp_Dout_Clock2_Config(BOOL Dcko, BYTE DckoClk, BYTE Dly, BOOL ClkoInv);
extern void Isp_Dout0_Sync_Config(UINT Htw, UINT HSyncOfs, UINT VSyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw);
extern void Isp_Dout1_Sync_Config(UINT Htw, UINT HSyncOfs, UINT VSyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw);
extern void Isp_WrCh0_Config(UINT Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE Clk, BOOL OnOff);
extern void Isp_WrCh1_Config(BYTE Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE VlcMode, BYTE Clk, BOOL IsFrc, BYTE FrcPage, BYTE Rdock, BYTE RdCh, BOOL OnOff);
extern void Isp_WrCh2_Config(BYTE Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE VlcMode, BYTE Clk, BOOL IsFrc, BYTE FrcPage, BYTE Rdock, BYTE RdCh, BOOL OnOff);
extern void Isp_WrCh3_Config(BYTE Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE VlcMode, BYTE Clk, BOOL IsFrc, BYTE FrcPage, BYTE Rdock, BYTE RdCh, BOOL OnOff);
extern void Isp_WrCh4_Config(BYTE Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE VlcMode, BYTE Clk, BOOL IsFrc, BYTE FrcPage, BYTE Rdock, BYTE RdCh, BOOL OnOff);
extern void Isp_WrCh0_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn);
extern void Isp_WrCh1_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn);
extern void Isp_WrCh2_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn);
extern void Isp_WrCh3_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn);
extern void Isp_WrCh4_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn);
extern void Isp_WrCh0_Adr(UINT YAdr, UINT CAdr);
extern void Isp_WrCh1_MAdr(UINT YAdr, UINT CAdr);
extern void Isp_WrCh1_FrcAdr(UINT YAdr0, UINT CAdr0, UINT YAdr1, UINT CAdr1, UINT YAdr2, UINT CAdr2);
extern void Isp_WrCh2_MAdr(UINT YAdr, UINT CAdr);
extern void Isp_WrCh2_FrcAdr(UINT YAdr0, UINT CAdr0, UINT YAdr1, UINT CAdr1, UINT YAdr2, UINT CAdr2);
extern void Isp_WrCh3_MAdr(UINT YAdr, UINT CAdr);
extern void Isp_WrCh3_FrcAdr(UINT YAdr0, UINT CAdr0, UINT YAdr1, UINT CAdr1, UINT YAdr2, UINT CAdr2);
extern void Isp_WrCh4_MAdr(UINT YAdr, UINT CAdr);
extern void Isp_WrCh4_FrcAdr(UINT YAdr0, UINT CAdr0, UINT YAdr1, UINT CAdr1, UINT YAdr2, UINT CAdr2);
extern void Isp_ISyncGen_Config1(BYTE SyncMode, BYTE Clk, UINT Htwi, UINT Vtwi, UINT Hw, UINT Vw, BOOL OnOff);
extern void Isp_ISyncGen_Config2(BYTE SyncMode, BYTE Clk, UINT Htwi, UINT Vtwi, UINT Hw, UINT Vw, BOOL OnOff);
extern void Isp_ISyncGen_Config3(BYTE SyncMode, BYTE Clk, UINT Htwi, UINT Vtwi, UINT Hw, UINT Vw, BOOL OnOff);
extern void Isp_RdCh0_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BOOL OnOff);
extern void Isp_RdCh1_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BYTE VlcMode, BOOL IsFrc, BYTE Rdock, BYTE RdCh, BOOL OnOff);
extern void Isp_RdCh2_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BYTE VlcMode, BOOL IsFrc, BYTE Rdock, BYTE RdCh, BOOL OnOff);
extern void Isp_RdCh3_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BYTE VlcMode, BOOL IsFrc, BYTE Rdock, BYTE RdCh, BOOL OnOff);
extern void Isp_RdCh4_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BYTE VlcMode, BOOL IsFrc, BYTE Rdock, BYTE RdCh, BOOL OnOff);
extern void Isp_RdCh0_MAdr(UINT YAdr, UINT CAdr);
extern void Isp_RdCh1_MAdr(UINT YAdr, UINT CAdr);
extern void Isp_RdCh2_MAdr(UINT YAdr, UINT CAdr);
extern void Isp_RdCh3_MAdr(UINT YAdr, UINT CAdr);
extern void Isp_RdCh4_MAdr(UINT YAdr, UINT CAdr);
extern void Isp_DS_Step_Config(UINT DsCoeff);
extern void Isp_DS0_Config(BYTE Path, BYTE Clk, SHORT DSRatioX, SHORT DSRatioY, SHORT DsHwi, SHORT DsVwi, BYTE Hlpf, BYTE Vlpf, SHORT Hoff, SHORT Voff, BOOL OnOff);
extern void Isp_DS1_Config(BYTE Path, BYTE Clk, SHORT DSRatioX, SHORT DSRatioY, SHORT DsHwi, SHORT DsVwi, BYTE Hlpf, BYTE Vlpf, SHORT Hoff, SHORT Voff, BOOL OnOff);
extern void Isp_DS2_Config(BYTE Path, BYTE Clk, SHORT DSRatioX, SHORT DSRatioY, SHORT DsHwi, SHORT DsVwi, BYTE Hlpf, BYTE Vlpf, SHORT Hoff, SHORT Voff, BOOL OnOff);
extern void Isp_DS3_Config(BYTE Path, BYTE Clk, SHORT DSRatioX, SHORT DSRatioY, SHORT DsHwi, SHORT DsVwi, BYTE Hlpf, BYTE Vlpf, SHORT Hoff, SHORT Voff, BOOL OnOff);
extern void Isp_DS0_Edge_Enhance_Config(BOOL EdgeOnOff, BYTE MGain, BYTE Slice, BYTE Clip, BYTE ROIOnOff, SHORT ROIHsp, SHORT ROIHed, SHORT ROIVsp, SHORT ROIVed, BYTE ROIGain, BOOL ROIOsdOnOff);
extern void Isp_DS1_Edge_Enhance_Config(BOOL EdgeOnOff, BYTE MGain, BYTE Slice, BYTE Clip, BYTE ROIOnOff, SHORT ROIHsp, SHORT ROIHed, SHORT ROIVsp, SHORT ROIVed, BYTE ROIGain, BOOL ROIOsdOnOff);
extern void Isp_DS2_Edge_Enhance_Config(BOOL EdgeOnOff, BYTE MGain, BYTE Slice, BYTE Clip, BYTE ROIOnOff, SHORT ROIHsp, SHORT ROIHed, SHORT ROIVsp, SHORT ROIVed, BYTE ROIGain, BOOL ROIOsdOnOff);
extern void Isp_DS3_Edge_Enhance_Config(BOOL EdgeOnOff, BYTE MGain, BYTE Slice, BYTE Clip, BYTE ROIOnOff, SHORT ROIHsp, SHORT ROIHed, SHORT ROIVsp, SHORT ROIVed, BYTE ROIGain, BOOL ROIOsdOnOff);
extern void Isp_DZoom_Config(BOOL OnOff, BYTE Clk, BYTE DzSync, BOOL IsAutoAlign, BOOL IsDkxAuto, BYTE DzoomCh, UINT DzVwo, UINT Dkx, UINT Dky, UINT DzRdHsp, UINT DzRdVsp, UINT HOffset, UINT VOffset);
extern void Isp_DZoom_Ratio(UINT Dkx, UINT Dky);
extern void Isp_Pip_Clk_Config(BOOL PipOnOff, BYTE Clk, BYTE MainImgCh, BYTE Sync, BOOL IsBaseImgMask);
extern void Isp_Pip_Ch0_Config(BOOL OnOff, BYTE PipRdCh, UINT PipHsp, UINT PipVsp, UINT PipHw, UINT PipVw, BOOL IsLine, UINT LineColor, BYTE Mix, UINT DdrLtc);
extern void Isp_Pip_Ch1_Config(BOOL OnOff, BYTE PipRdCh, UINT PipHsp, UINT PipVsp, UINT PipHw, UINT PipVw, BOOL IsLine, UINT LineColor, BYTE Mix, UINT DdrLtc);
extern void Isp_Pip_Ch2_Config(BOOL OnOff, BYTE PipRdCh, UINT PipHsp, UINT PipVsp, UINT PipHw, UINT PipVw, BOOL IsLine, UINT LineColor, BYTE Mix, UINT DdrLtc);
extern void Isp_Pip_Ch3_Config(BOOL OnOff, BYTE PipRdCh, UINT PipHsp, UINT PipVsp, UINT PipHw, UINT PipVw, BOOL IsLine, UINT LineColor, BYTE Mix, UINT DdrLtc);
extern UINT Isp_Img_HMerge(BOOL OnOff, BYTE Clk, BYTE ChCnt, BYTE RdSync, UINT HtwSync, UINT Htw, UINT Hw, UINT Hsp, UINT Vsp, UINT Vw);
extern void Isp_Cvbs_Config(BOOL OnOff, BOOL IsNtsc, BYTE CvbsFreq, BYTE SourceFrq, BYTE SorcePath, BYTE VlcMode, UINT Hsp, UINT Vsp);
extern void Isp_Cvbs_Adr(UINT CvbsAdr0, UINT CvbsAdr1, UINT CvbsAdr2);
extern void Isp_Vlc_Config(BYTE VlcMode);


//*************************************************************************************************
// ISP Library
//-------------------------------------------------------------------------------------------------
extern void InitSensRun(void);
extern void SensFlip(BYTE aOn);
extern void SensMirror(BYTE aOn);
extern void AE_SHTw(UINT val);
extern void AE_DSSw(UINT val);
extern void AE_AGCw(UINT val);
extern void AE_FREQw(UINT val, UINT val1);
extern void AE_ETCw(UINT val);
extern void SensUpdate(void);
extern int LibUtlInterp1D(int aiXCur, int aiXPos1, int aiXPos2, int aiYPos1, int aiYPos2);
extern int LibUtlInterp1D_CLAMP(int aiXCur, int aiXPos1, int aiXPos2, int aiYPos1, int aiYPos2);
extern BYTE LibUtlKeyPass(const BYTE* abpPassKeys, BYTE abKeySize, BYTE* abpPassCnt);


//*************************************************************************************************
// User Parameter
//-------------------------------------------------------------------------------------------------
extern void InitDataSet(void);
extern void InitUsrParChgAll(void);
extern void UsrDataReset(void);
extern void SetByte(BYTE *apAddr, const BYTE abLen, UINT anData);
extern UINT GetByte(BYTE *apAddr, const BYTE abLen);
extern void UsrParValid(const UINT anValid);
extern UINT UsrParBit(const UINT anIdx);
extern void UsrParChg(const UINT anStrIdx);
extern void UsrParChgEndIdx(const UINT anEndIdx);
extern void UsrParCpy(BYTE *dest, BYTE *src);
extern BYTE UsrParSiz(const UINT anIdx);
extern void UsrParReset(void);
extern void UsrParStyle(const int abStyle, const int abInit, const int abValChg);
extern void UsrParSave(UINT anSaveOn);
extern void AppSavePar(void);
extern void AppLoadPar(void);


//*************************************************************************************************
// MENU
//-------------------------------------------------------------------------------------------------
extern void OsdCamTitle(void);
extern void ChangeMenuSize(void);
extern void InitMenu(void);
extern void Menu(void);
extern void menu_redraw(int iChg, int iGrayOnly);
extern void menu_val_chack(void);


//*************************************************************************************************
// isp main
//-------------------------------------------------------------------------------------------------
extern void CPUtoISPcallback(void *ctx);
extern void OutMode(void);

extern void Init_Visp(void);
extern void Init_Vcap(void);
extern void Init_Vout(void);
extern void Init_Virq(void);

extern void Visp(void);
extern void Vcap(void);

#define INIT_DELAY(T)	WaitXms(((1000/(model_Sens_Fps))+1)*T)/*Wait_VLOCKO_M(T)*/		// Uint of T is 1/FPS sec

//*************************************************************************************************
// TDN
//-------------------------------------------------------------------------------------------------
enum {
	TDN_HLD = 0,
	TDN_DAY,
	TDN_NIT
};

//------------------------------------------------------------------------------
// extern
extern UINT gnTdnChkOut;		// Day & BW status
extern void TDN(void);


//*************************************************************************************************
// CONTRAST
//-------------------------------------------------------------------------------------------------
#define UpDEFOG_TH_LOW	0x100		// Defog Maximum Strength for menu LOW (0x0 ~ 0xffff)
#define UpDEFOG_TH_MID	0x200		// Defog Maximum Strength for menu MIDDLE (LOW ~ 0xffff)
#define UpDEFOG_TH_HIGH	0x500		// Defog Maximum Strength for menu HIGH (MIDDLE ~ 0xffff)

#define UpACETBL1_LOW	0x10		// ACE Dark Area Strength for menu LOW (0x0 ~ 0xff)
#define UpACETBL1_MID	0x30		// ACE Dark Area Strength for menu MIDDLE (LOW ~ 0xff)
#define UpACETBL1_HIGH	0x50		// ACE Dark Area Strength for menu HIGH (MIDDLE ~ 0xff)

#define UpACETBL2_LOW	0x08		// ACE Bright Area Strength for menu LOW (0x0 ~ 0xff)
#define UpACETBL2_MID	0x18		// ACE Bright Area Strength for menu MIDDLE (LOW ~ 0xff)
#define UpACETBL2_HIGH	0x20		// ACE Bright Area Strength for menu HIGH (MIDDLE ~ 0xff)

#define UpACEWDR1_TH	64/*0x20*/		// ACE Dark Area Strength of WDR (0x0 ~ 0xff)
#define UpACEWDR2_TH	32/*0x20*/		// ACE Bright Area Strength of WDR (0x0 ~ 0xff)

#define DEFOG_X_OFF 	30

//------------------------------------------------------------------------------
// extern
extern BYTE gbHistoM[];

extern const UINT		gnAe1Size;
extern unsigned int 	gnHistoM[];
//extern unsigned int 	gnDefogV;
//extern unsigned int 	gnDwdrV;

extern unsigned char 	gbContrastMode;		// DEFOG OFF/ON
extern unsigned char 	gbDefogMode;        // "	 MODE MANUAL/AUTO
extern unsigned char 	gbDefogLevel;       // "	 STRENGTH LOW/MID/HIGH

extern unsigned int 	gnHistoV[];

extern unsigned int 	nDefogTbl[];


extern void InitContrast(void);
extern void Gamma(void);
extern void AceDefog(void);
extern void LibDefog(unsigned int * anDefogThres, unsigned int * anDefogWeight, unsigned int *anDefogTbl, const BYTE abDssOn);

extern void InitHisto(void);
extern void Histo(void);


//*************************************************************************************************
// 10. AWB
//-------------------------------------------------------------------------------------------------
#define UpAWB_TCNT_LMT		0x1000		  // White pixel cnt limit

#define UpAWB_AGC_MIN		0x120																	// Chroma AGC position for maximum weight
#define AWB_AGC_MAX 		0x1e0																	// Chroma AGC position for minimum weight
#define AWB_AGC_WGT_MAX 	0x100																	// Chroma maximum weight (100% = 0x100)
#define UpAWB_AGC_WGT_MIN	0x40																	// Chroma minimum weight

#define UpAWB_SPEED_MTRX	0xf8										// Matrix tracking speed (Low is high speed, max=0xff)
#define AWB_SPEED_GAIN		UP(AWB_SPEED_MTRX)							// RGB gain tracking Speed (Low is high speed)

#define UpAWB_YSLICE		4/*0x40*/									// White Y slice
#define UpAWB_YCLIP 		0x200										// White Y clip

#define AWB_GAIN_DFLT		0x100										// RGB Gain default
#define UpAWB_GAIN_TIMES	3																		// RGB Gain limit (X times)

#define AWB_GAIN_MIN		(AWB_GAIN_DFLT/UP(AWB_GAIN_TIMES))									// RGB Gain min
#define AWB_GAIN_MAX		(AWB_GAIN_DFLT*UP(AWB_GAIN_TIMES))									// RGB Gain max


//-------------------------------------------------------------------------------------------------
// Extern
extern void Awb(void);
extern void InitAwb(void);
extern void HueChroma(void);

//extern UINT	gnAwbErr;		// AWB tracking finish state


//*************************************************************************************************
// 11. DEFECT
//-------------------------------------------------------------------------------------------------
extern void DefectAuto(void);


//*************************************************************************************************
// 14. ETC (ISP applications)
//-------------------------------------------------------------------------------------------------
extern int udiv4x(const UINT anSum8bit, const UINT anCnt, const UINT anCntZeroVal);

//------------------------------------------------------------------------------
// ADNR
extern int giIspAgc[3];
extern BYTE gbIspAgcSta;
extern BYTE gbIspAgcEnd;
extern void IspAgcSet(void);
extern void IspAgcCurSet(void);

#define ADNR_AGC_MIN		giIspAgc[0]		// Min AGC arg.
#define ADNR_AGC_MID		giIspAgc[1]		// Mid AGC arg.
#define ADNR_AGC_MAX		giIspAgc[2]		// Max AGC arg.

#define ADNR_AGC_STA		giIspAgc[gbIspAgcSta]
#define ADNR_AGC_END		giIspAgc[gbIspAgcEnd]

#define Up3D_DNR_GAIN_LOW		0x80
#define Up3D_DNR_GAIN_MIDDLE	0x80
#define Up3D_DNR_GAIN_HIGH		0x80

extern void Adnr_2D_Gain(const UINT anGain);
extern void Adnr(void);

//------------------------------------------------------------------------------
// Sharpness
extern void Sharpness(void);

//------------------------------------------------------------------------------
// Color surpression

//#define UpCES_LCLP		0x14
//#define UpCES_WLCLP		0x0A

//				UpCSUP_TH	UpCSUP_WTH
// SENS_MN34210		0x74	0x78
// SENS_IMX225		0x78	0x78

// SENS_MN34229		0x78	0x78
// SENS_MN34420		0x74	0x74
// SENS_IMX307		0x74	0x74
// SENS_IMX327		0x74	0x74
// SENS_IMX291		0x70	0x70
// SENS_AR0237CSH	0x74	0x74

// SENS_IMX290		0x74	0x74
// SENS_MN34422		0x00	0x00

// SENS_MN34227		0x78	0x78
// SENS_OV2718		0x74	0x74
// SENS_IMX323		0x74	0x74

// SENS_IMX335		0x72	0x72
// SENS_OS05A10		0x72	0x72

// SENS_IMX334		0x72	0x72
// SENS_IMX274		0x72	0x72
// SENS_OS08A10		0x72	0x72

extern void CSup(void);

//------------------------------------------------------------------------------
// Highlight Mask
extern void HlMask(void);

//------------------------------------------------------------------------------
// Mirro / Flip
extern void Flip(void);

//------------------------------------------------------------------------------
// Digital Zoom
#define DZ_HW_MR	0//((model_8M) ? 16 : 8)	// DZOOM HW Margin, 8M의 경우 RP(PO_HW)+8 이 16의 배수가 안됨

#define DZ_HW	RP(FR_HW)//(RP(PO_HW)+DZ_HW_MR)
#define DZ_VW	RP(FR_VW)//RP(PO_VW)

extern void Dzoom_init(void);
extern void DZoom(void);

//------------------------------------------------------------------------------
// Color space
extern void ColorSpace(void);

//------------------------------------------------------------------------------
// PrivacyBox
extern void PrivacyBox(void);

//------------------------------------------------------------------------------
// Last Box apply
//extern UINT gnBoxPosPVC[PVC_EA*2];
//extern UINT gnBoxPltPVC/*[PVC_EA]*/;
//extern UINT gnBoxTonePVC[2];
//extern UINT gnBoxFillPVC;
//extern UINT gnBoxOnPVC;
//extern UINT gnBoxFmtPVC;	// 2015112 - WHL : Add Bit Box Mode

#define gbMnPvcCfg(INDEX)		((_PRIVACY*)(gbUsrParTbl + MpPvcCfgIdx +((INDEX))*sizeof(_PRIVACY)))

#define gbMnImdCfg(INDEX)		((_PRIVACY*)(gbUsrParTbl + MpItlWinCfgIdx +((INDEX))*sizeof(_PRIVACY)))

//------------------------------------------------------------------------------
// Box Privacy
#define BOX_TONE_100	0
#define BOX_TONE_50		1
#define BOX_TONE_25		2
#define BOX_TONE_0		3

#define BOX_ON(ID)	BOSD_ON0w(BOSD_ONr | (1<<ID))
#define BOX_OFF(ID)	BOSD_ON0w(BOSD_ONr & (~(1<<ID)))

#define BOX_COLOR(ID,Y,CB,CR)	{\
	BOX##ID##_PLTw( (((Y)&0xFF)<<16) | (((CB)&0xFF)<<8) | ((CR)&0xFF) );\
}

#define BOX_ATT0(ID,FILL,TONE,Y,CB,CR) {\
	BFL_ON##ID##w(FILL);\
	BTONE##ID##w(TONE);\
	BOX_COLOR(ID,Y,CB,CR)\
}
#define BOX_ATT(ID,FILL,TONE,Y,CB,CR)	BOX_ATT0(ID,FILL,TONE,Y,CB,CR)

#define BOX0(ID,SX,SY,EX,EY)	{\
	B##ID##_HSPw(SX);\
	B##ID##_HEDw(EX);\
	B##ID##_VSPw(SY);\
	B##ID##_VEDw(EY);\
}
#define BOX(ID,SX,SY,EX,EY)	BOX0(ID,SX,SY,EX,EY)

#define LINEH0(ID,X,Y,S){\
	B##ID##_HSPw(X);\
	B##ID##_HEDw(S);\
	B##ID##_VSPw(Y);\
	B##ID##_VEDw(Y);\
}
#define LINEH(ID,X,Y,S)		LINEH0(ID,X,Y,S)

#define LINEV0(ID,X,Y,S){\
	B##ID##_HSPw(Y);\
	B##ID##_HEDw(Y+1);\
	B##ID##_VSPw(X);\
	B##ID##_VEDw(S);\
}
#define LINEV(ID,X,Y,S)		LINEV0(ID,X,Y,S)

#define BOX_MVPXL_MIN		((RP(FR_HW)/255)+1)				//Move Pixel Min Value by 1 Step
#define BOX_MVPXL_MAX		(RP(FR_VW))					//Move Pixel Max Value by 1 Step

#define UpBOX_MV_PXL		0x20		//0x8 ~ 0x438 Box Move Pixel(2M)

#define BOX_XMAX			(RP(FR_HW)/UpBOX_MV_PXL-1)		//Menu Box Position Control X Max Value
#define BOX_YMAX			(RP(FR_VW)/UpBOX_MV_PXL)		//Menu Box Position Control Y Max Value

#define UpBOX_COLOR_STEP	0x14		//0x1 ~ 0xFF	Color Step Divide

extern void BoxLast(void);


//*************************************************************************************************
// Debug & Test
//-------------------------------------------------------------------------------------------------
#define FontBenchTime(ON,Y,X,STR,TIME_START,LEN)	/*{ if(ON) {*/\
		const UINT _##TIME_START##_ = BenchTimeStop(TIME_START);\
		FontStrDecUnit(ON, Y, X, MN_GREEN, STR, MN_WHITE, /*BenchTimeStop(TIME_START)*/_##TIME_START##_, LEN, "us")/*;} }*/




/* EOF */

