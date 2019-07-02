
//	isp_tbl.c
extern void SetSens(WORD awAddr, BYTE abData);
extern BYTE GetSens(WORD awAddr);

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

//	LCD Function
extern void LCD_FR_TFT035_6_Init(void);

// ISP DDR
extern void Isp_Ddr_Cong(void);


//	isp_interface.c
extern void Isp_SYNC_CODE(WORD awC0, WORD awC1, WORD awC2, WORD awC3_0, WORD awC3_1, WORD awC3_2, WORD awS0C3_0, WORD awS0C3_1, WORD awS0C3_2, WORD awS1C3_0, WORD awS1C3_1, WORD awS1C3_2);
extern void Isp_SDesPowerOn(BOOL OnOff, BOOL IsMipi, BYTE MipiClkPhase);
extern void Isp_SDesDelay(BYTE Lck, BYTE Ldi0, BYTE Ldi1, BYTE Ldi2, BYTE Ldi3);
extern void Isp_SDesPosition(UINT LvdsHRpos, UINT LvdsVRpos, UINT LvdsHw, UINT LvdsVw);
extern void Isp_Lvds_Config(BYTE LvdsBit, BYTE LvdsLane, BOOL IsLsbFirst, BOOL PNSel, BOOL SofNo);
extern void Isp_Mipi_Config(BYTE MipiBit, BYTE MipiLane, BOOL IsLsbFirst, BOOL PNSel, WORD MipiHsyncOfs, BOOL UseEcc, BOOL UseWcl, BOOL UseWcf, BOOL UseWcfe, BYTE MipiImgPhase);
extern void Isp_SensorPowerOn(BOOL OnOff, UINT Clk);
extern void Isp_Parallel_Config(BOOL OnOff/*, BOOL IsSlave*/, BOOL IsUseExtClk, BOOL IsClkDdr, BYTE ClkDly/*, BOOL HSyncPol, BOOL VSyncPol, BOOL SyncMode, BOOL IsUseAsync*/);
extern void Isp_PreClk_Config(BYTE Clk);
extern void Isp_PreSync_Config(BOOL IsSlave, UINT Htw, UINT Vtw, UINT HsyncOfs, UINT VsyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw, BOOL IsASync, BOOL IsNSync, BOOL HSyncPol, BOOL VSyncPol);
extern void Isp_PostSync_Config(BOOL OSyncMode, BOOL ExSyncSel, UINT Htw, UINT Vtw, UINT HsyncOfs, UINT VsyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw, BYTE OCSel);
extern void Isp_PostClk_Config(BYTE Clk);
extern void Isp_Gamma_Config(BOOL Y_OnOff, BOOL C_OnOff);
extern void Isp_Edge_Config(BOOL OnOff);
extern void Isp_Dnr3d_Config(BOOL OnOff, BYTE DnrFk, BYTE DnrTh, BYTE DnrGain);
extern void Isp_Defect_Config(BOOL OnOff, BOOL GsumCnt, BOOL RBSumCnt, BYTE GWgt, BYTE RBWgt, BYTE DfSlope, UINT GThres, UINT RBThres, UINT DfMax, UINT DfMin, BYTE DfCsel);
extern void Isp_Dnr2d_Config(BOOL OnOff, BOOL Dnr2dMode, BYTE Dnr2dCnt, BYTE Dnr2dDth, BYTE Dnr2dGain);
extern void Isp_Frc_Adr_Config(UINT Adr0, UINT Adr1, UINT Adr2, UINT Adr3, UINT Adr4);

//	digital_interface.c
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
extern void Isp_Ddr_Init(BYTE IspFrcMode);
extern void Isp_WrCh0_Config(UINT Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE VlcMode, BYTE Clk, BOOL OnOff);
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
extern void Isp_RdCh0_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BYTE VlcMode, BOOL OnOff);
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


// ISP Library
extern void InitSensRun(void);
extern int LibUtlInterp1D(int aiXCur, int aiXPos1, int aiXPos2, int aiYPos1, int aiYPos2);
extern BYTE LibUtlKeyPass(const BYTE* abpPassKeys, BYTE abKeySize, BYTE* abpPassCnt);


// User Parameter
extern void InitDataSet(void);
extern void UsrDataReset(void);
extern void SetByte(BYTE *apAddr, const BYTE abLen, UINT anData);
extern UINT GetByte(BYTE *apAddr, const BYTE abLen);
extern void UsrParValid(const UINT anValid);
extern void UsrParChg(const UINT anStrIdx);
extern void UsrParChgEndIdx(const UINT anEndIdx);
extern void UsrParChgAll(void);
extern void UsrParReset(void);
extern void UsrParStyle(const BYTE abStyle);
extern void UsrParSave(UINT anSaveOn);
extern void AppSavePar(void);
extern void AppLoadPar(void);


// MENU
extern void OsdCamTitle(void);
extern void ChangeMenuSize(void);
extern void InitMenu(void);
extern void Menu(void);
extern void menu_redraw(int iChg, int iGrayOnly);

// isp main
extern void Isp_irq_init(void);
extern void Isp_Sensor_init(void);
extern void Isp_Output_init(void);
extern void Isp_Function_init(void);
extern void Isp_DDR_init(void);
extern void Isp_Digital_input_init(void);
extern void Isp_init(void);
extern void Hdmi_Check(void);
extern void IF_Funcs(void);
extern void isp_main(void);




