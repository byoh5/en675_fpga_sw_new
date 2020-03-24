#include "dev.h"

void Isp_SYNC_CODE(WORD awC0, WORD awC1, WORD awC2, WORD awC3_0, WORD awC3_1, WORD awC3_2, WORD awS0C3_0, WORD awS0C3_1, WORD awS0C3_2, WORD awS1C3_0, WORD awS1C3_1, WORD awS1C3_2)
{
#if (model_Sens_Intf==1) || (model_Sens_Intf==2)	// LVDS or MIPI
	SYNC_COD0w(awC0);
	SYNC_COD1w(awC1);
	SYNC_COD2w(awC2);				//	Short Packet Line Start
	SYNC_COD3_0w(awC3_0);			//	Short Packet Line End
	SYNC_COD3_1w(awC3_1);
	SYNC_COD3_2w(awC3_2);
#endif

#if (model_Sens_Intf==2)	// MIPI
	SYNC_S0COD3_0w(awS0C3_0);
	SYNC_S0COD3_1w(awS0C3_1);
	SYNC_S0COD3_2w(awS0C3_2);			//	Long Packet Line Start - Raw 10Bit or
	SYNC_S1COD3_0w(awS1C3_0);			//	Long Packet Line End
	SYNC_S1COD3_1w(awS1C3_1);
	SYNC_S1COD3_2w(awS1C3_2);
#endif
}

//	OnOff			->	0 : Off, 1 : On
//	IsMipi			->	0 : LVDS, 1 : Mipi
//	MipiClkPhase	->	Select Mipi 1/4 Clock Phase 0, 1, 2, 3
void Isp_SDesPowerOn(BOOL OnOff, BOOL IsMipi, BYTE MipiClkPhase)
{
	if(OnOff) {
//		SDES_PDw(0x00);
//		if(IsMipi)	{	RDES_CK_SELw(1);	LSYNCM_SELw(MipiClkPhase);	LVDS_RXONw(0);	}
//		else		{	RDES_CK_SELw(0);	LVDS_RXONw(1);	}
		CH_DSELw(0);
		CH_HSSELw(0);
		CH_VSSELw(0);
//		LCK_SONw(1);
	}
	else {
//		SDES_PDw(0x1f);
//		LVDS_RXONw(0);
		CH_DSELw(1);
		CH_HSSELw(1);
		CH_VSSELw(1);
//		LCK_SONw(0);
	}
}

void Isp_SDesDelay(BYTE Lck)
{
#if (model_Sens_Intf==1) || (model_Sens_Intf==2)	// MIPI
	LCK_DLYw(Lck);
#endif
}

//void Isp_SdesTimingAlign(BYTE Res)
//{
//	UINT i,j;
//	UINT XRes, YRes;
//
//	switch(Res)
//	{
//		case	RES_720_25P	:	{XRes = 3959;	YRes = 749;		}	break;
//		case	RES_720_30P	:   {XRes = 3299;	YRes = 749;		}   break;
//		case	RES_720_50P	:   {XRes = 1979;	YRes = 749;		}   break;
//		case	RES_720_60P	:   {XRes = 1649;	YRes = 749;		}   break;
//		case	RES_1080_25P:   {XRes = 2639;	YRes = 1124;	}   break;
//		case	RES_1080_30P:   {XRes = 2199;	YRes = 1124;	}   break;
//		case	RES_1080_50P:   {XRes = 2639;	YRes = 1124;	}   break;
//		case	RES_1080_60P:   {XRes = 2199;	YRes = 1124;	}   break;
////		case	RES_4M_25P	:   {XRes = 		}
////		case	RES_4M_30P	:   {XRes = 		}
////		case	RES_4K_15P	:   {XRes = 		}
//		default				:	{XRes = 2639;	YRes = 1124;	}   break;
//
//	}
//
//	for(i=0;i<8;i++)
//	{
//		LCK_DLYw(i);
//		for(
//	}
//
//}

//	ClkSel	->	Select Sensor Operation Clock
//	SENS_148M	0 : 148.5 MHz
//	SENS_74M	1 : 74.25 MHz
//	SENS_37M	2 : 37.125 MHz
//	SENS_18M	3 : 18.5625 MHz
//	SENS_27M	4 : 27 MHz	(CLK Input Clock)
//	SENS_13M	5 : 13.5 MHz (CLK Input Clock / 2)
void Isp_SensorPowerOn(BOOL OnOff, UINT Clk)
{
	const BYTE ClkSel = (Clk==148) ? SENS_148M :
						(Clk==74) ? SENS_74M :
						(Clk==37) ? SENS_37M :
						(Clk==18) ? SENS_18M :
						(Clk==27) ? SENS_27M :
						(Clk==13) ? SENS_13M :
						/*(Clk==6) ?*/ SENS_6M ;

	PCKO_CK_PDw(0);
	PCKO_PDw(0);
	PCKO_SELw(ClkSel);
	PCKO_CK_PDw(1);
	PCKO_PDw(1);
	PCKO_OENw(0);

	if(OnOff)		PCKO_OENw(0);
	else			PCKO_OENw(1);
}

//	BOOL OnOff			->	0 : Parallel Interface Off, 1 : Parallel Interface On,
//	BOOL IsSlave		->	ISP_MASTER (0)
//							ISP_SLAVE (1)
//	BOOL IsUseExtClk	->	PARA_EXTER_CLK (0)
//							PARA_INTER_CLK (1)
//	BOOL IsClkDdr		->	PARA_CLK_SDR (0)
//							PARA_CLK_DDR (1)
//	BOOL HSyncPol		->	SYNC_FALL (0)
//							SYNC_RISE (1)
//	BOOL SyncMode		->	SAME_EDGE (0)
//							DIFF_EDGE (1)
//	BOOL IsUseAsync		->	NO_USE_AUTOSYNC (0)
//						->	USE_AUTOSYNC 	(1)
void Isp_Parallel_Config(BOOL OnOff/*, BOOL IsSlave*/, BOOL IsUseExtClk, BOOL IsClkDdr, BYTE ClkDly/*, BOOL HSyncPol, BOOL VSyncPol, BOOL SyncMode, BOOL IsUseAsync*/, BYTE BitMode)
{

	PCLK_PDw(OnOff);

	if(OnOff)	{	CH_DSELw(1);	CH_HSSELw(1);	CH_VSSELw(1);								}
	else		{	CH_DSELw(0);	CH_HSSELw(0);	CH_VSSELw(0);	IBT_PCK_PDw(0);	BT_PCK_PDw(0);	IDDR_ONw(0);	ISDR_OPw(0);	return;	}

//	if(IsSlave)		SLVw(3);
//	else			SLVw(0);

	IBT_PCK_SET(0);

	if(IsUseExtClk==PARA_EXTER_CLK)	{	BT_PCK_SET(3);	}			//	External Sensor Pixel Clock
	else							{	BT_PCK_SET(2);	}			//	Internal 74.25 MHz

	if(IsClkDdr==PARA_CLK_SDR)		{	IDDR_ONw(1);	ISDR_OPw(1);	}
	else							{	IDDR_ONw(1);	ISDR_OPw(0);	}

	DDRFF_RSw(1);
	DDRFF_RSw(0);

//	POL_HSIw(HSyncPol);
//	POL_VSIw(VSyncPol);

//	VSYN_NAONw(SyncMode);
//	ASYNC_ONw(IsUseAsync);

	PDI_MODw(BitMode);

	PCLK_DLYw(ClkDly);
}

//	Clk	->	Select Isp Pre Processor Clock
//	ISP_CLK_PLL			0   //	ISP PLL Output
//	ISP_CLK_148M		1   //	ISP PLL / 2 (148.5 MHz)
//	ISP_CLK_74M			2   //	ISP PLL / 2 (74.5 MHz)
//	ISP_CLK_PCLK		3   //	SS_CKI Input
//	ISP_CLK_PCLK_DIV2	4   //	SS_CKI Input 1/2 Divided Clock
//	ISP_CLK_PLL_DIV0	5	//	ISP PLL Output Adjust CLock 0
void Isp_PreClk_Config(BYTE Clk, BOOL Usefrc)
{
	BT_PCK_PDw(0);	FPCK_PDw(0);

	if(Clk==5)		BT_PCK_SELw(7);
	else			BT_PCK_SELw(Clk);

	FPCK_SELw(Clk);

	BT_PCK_PDw(1);	FPCK_PDw(1);

	if(Usefrc)	{
		YCW_DCK2_PDw(0);
		if(Clk==ISP_CLK_PCLK_DIV2)		{ YCW_DCK2_SELw(7); }
		else if(Clk==ISP_CLK_PLL_DIV0)	{ YCW_DCK2_SELw(12); }
		else							{ YCW_DCK2_SELw(Clk); }
		YCW_DCK2_PDw(1);	// WDR 사용 시 필요
	}
	else YCW_DCK2_PDw(0);
}


//	IsSlave+	->	0 : Master Mode, 1 : Slave Mode
//	Htw			->	Horizontal Total Counter : Real Number
//	Vtw			->	Vertical Total Counter : Real Number
//	HsyncOfs*	->	Horizontal Sync Offset, Hsp 인자로 Image 위치가 맞지 않을때 Sync의 위치를 이동하여 image 위치를 맞추는데 사용한다.
//	VsyncOfs*	->	Vertical Sync Offset, Vsp 인자로 Image 위치가 맞지 않을때 Sync의 위치를 이동하여 image 위치를 맞추는데 사용한다.
//	Hsp*		->	Post module horizontal image start position
//	Vsp*		->	Post module vertical image start position
//	Hw			->	Horizontal Active Width
//	Vw			->	Vertical Active Width
//	IsASync+	->	omni sensor와 같이 Active 이외의 구간에서 sync가 나오지 않는 sensor에서 1로 설정
//	IsNSync+	->	입력 sync의 H/V 위상이 일치하지 않는 상황에서 1로 설정
//	HSyncPol	->	SYNC_FALL (0), SYNC_RISE (1)
//	VSyncPol	->	SYNC_FALL (0), SYNC_RISE (1)
void Isp_PreSync_Config(BOOL IsSlave, UINT Htw, UINT Vtw, UINT HsyncOfs, UINT VsyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw, BOOL IsASync, BOOL IsNSync, BOOL HSyncPol, BOOL VSyncPol)
{
	PRS_HZw(1);

	HSPIw(Hsp);		VSPIw(Vsp);		HWIw(Hw);		VWIw(Vw);

	if(IsSlave)		{												SLVw(3);	}
	else			{		HTWIw(Htw-2);		VTWIw(Vtw-1);		SLVw(0);	}


	VLOCKI_POSw(VsyncOfs);
	HLOCKI_POSw(HsyncOfs);

	ASYNC_ONw(IsASync);
	VSYN_NAONw(IsNSync);
	PRS_HZw(0);

	POL_HSIw(HSyncPol);
	POL_VSIw(VSyncPol);
}

void Isp_AutoSync_Config()
{

}

//	OSyncMode	->	0 : External Sync(Slave), 1 : Standalone Sync(Master)
//	ExSyncSel	->	Select External Sync,  0 : Pre Module Sync,  1: External Input Sync (EN675 Pin VSO, HSO Input)
//	Htw			->	Horizontal Total Counter : Real Number
//	Vtw			->	Vertical Total Counter : Real Number
//	HsyncOfs	->	Horizontal Sync Offset, Hsp 인자로 image 위치가 맞지 않을때 sync의 위치를 이동하여 image 위치를 맞추는데 사용
//	VsyncOfs	->	Vertical Sync Offset, Vsp 인자로 image 위치가 맞지 않을때 sync의 위치를 이동하여 image 위치를 맞추는데 사용
//	Hsp			->	Post module horizontal image start position
//	Vsp			->	Post module vertical image start position
//	Hw			->	Horizontal Active Width
//	Vw			->	Vertical Active Width
//	OCSel*		->	Post module color phase selection
void Isp_PostSync_Config(BOOL OSyncMode, BOOL ExSyncSel, UINT Htw, UINT Vtw, UINT HsyncOfs, UINT VsyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw, BYTE OCSel)
{
	POS_HZw(1);

	HSPOw(Hsp);		VSPOw(Vsp);		HWOw(Hw);		VWOw(Vw);

	if(OSyncMode)	{	HTWOw(Htw-2);		VTWOw(Vtw-1);		OSYNC_MODw(1);	}
	else			{											OSYNC_MODw(0);	}

	VLOCKI2_POSw(VsyncOfs);
	HLOCKI2_POSw(HsyncOfs);
	EXVSYNC_SELw(ExSyncSel);
	EXHSYNC_SELw(ExSyncSel);

	//OCSELw(OCSel);	// Flip() 함수에서 실행됨

	POS_HZw(0);
}

//	ISP_CLK_PLL			0   //	ISP PLL Output
//	ISP_CLK_148M		1   //	ISP PLL / 2 (148.5 MHz)
//	ISP_CLK_74M			2   //	ISP PLL / 2 (74.5 MHz)
//	ISP_CLK_PCLK		3   //	SS_CKI Input
//	ISP_CLK_PCLK_DIV2	4   //	SS_CKI Input 1/2 Divided Clock
//	ISP_CLK_PLL_DIV0	5	//	ISP PLL Output Adjust CLock 0
void Isp_PostClk_Config(BYTE Clk)
{
	PSCK_PDw(0);	PPCK_PDw(0);	PR_CK0_PDw(0);	FN_CK0_PDw(0);

	PSCK_SELw(Clk);
	PPCK_SELw(Clk);
	FN_CK0_SELw(Clk);
	PR_CK0_SELw(Clk);

	PSCK_PDw(1);	PPCK_PDw(1);	PR_CK0_PDw(1);	FN_CK0_PDw(1);
}


void Isp_Gamma_Config(BOOL Y_OnOff, BOOL C_OnOff)
{
	//	Table Setting

	//	On
	YGMA_ONw(Y_OnOff);
	CGMA_ONw(C_OnOff);
}

void Isp_Edge_Config(BOOL OnOff)
{

//	APT_TH3w(0x40);
//	APT_TH2w(0x0c);
//	APT_TH1w(0x08);
//
//	APT_GAIN2_NEGw(0x08);
//	APT_GAIN1_NEGw(0x08);
//	APT_GAIN2_POSw(0x08);
//	APT_GAIN1_POSw(0x08);
//	APT_SLIC3w(0x10);
//	APT_CLIP3w(0x40);
//	APT_GAIN3w(0x80);
//	APT_LPFKw(0xf0);
//	APT_LPFGAw(0x80);
//	APT_LPFTHw(0x08);

	APT_ONw(OnOff);
	APT_GAIN_ONw(OnOff);

	APT_TH3w(0x80);
	APT_CLIP3w(0x80);
}

//	OnOff	->	FN_ON, FN_OFF
//	DnrFk	->	3D dnr 동작 시 feedback되는 image의 강도를 조절한다.
//	DnrTh	->	현재 이미지와 전 이미지의 difference가 이 threshold 값보다 작은 영역에서 3d dnr이 동작하게 된다.
//	DnrGain	->	Gain보다 Dnrth의 값이 3D dnr 동작에 더 큰 영향을 준다. 보통 0x20으로 사용한다.
void Isp_Dnr3d_Config(BOOL OnOff, BYTE DnrFk, BYTE DnrTh, BYTE DnrGain)
{
	DNR3D_FKw(DnrFk);
	DNR3D_THw(DnrTh);
	DNR3D_GAw(DnrGain);

	if(OnOff)	{	DNR3D_RCH_ENw(1);
					if(FPCK_SELr==4)		{	YCR_CK4_SET(7);         }
					else if(FPCK_SELr==5)	{	YCR_CK4_SET(11);        }
					else					{	YCR_CK4_SET(FPCK_SELr);	}
					DNR3D_ONw(1);
				}
	else		{	DNR3D_RCH_ENw(0);	DNR3D_ONw(0);	YCR_CK4_PDw(0);	}

}

#if 0
//	OnOff				->	FN_ON, FN_OFF
//	GsumCnt, RBSumCnt	->	DF_SUM_4 : 4 pixel
//							DF_SUM_6 : 6 pixel
//	GWgt, RBWgt			->	DF_WGT_CASEA : Based pixel count method.
//	                        DF_WGT_CASEB : Based weight method				:	GWgt  -> Use this method normally. RBWgt -> Use this method normally.
//	                        DF_WGT_CASEC : Based multiplex weight method
//	DfSlope				->	DF_SLOPE_NOR : Use this value
//	GThres				->	DF_RBTHRES : Use this value normally
//	RBThres				->	DF_GTHRES : Use this value normally
void Isp_Defect_Config(BOOL OnOff, BOOL GsumCnt, BOOL RBSumCnt, BYTE GWgt, BYTE RBWgt, BYTE DfSlope, UINT GThres, UINT RBThres, UINT DfMax, UINT DfMin, BYTE DfCsel)
{
	DF_GSUM_SELw(GsumCnt);
	DF_RBSUM_SELw(RBSumCnt);
	DF_GWGT_SELw(GWgt);
	DF_RBWGT_SELw(RBWgt);
	DF_SLOPEw(DfSlope);
	DF_SUM_GTHw(GThres);
	DF_SUM_RBTHw(RBThres);
	DF_MINw(DfMin);
	DF_MAXw(DfMax);
	DFS_ICSELw(DfCsel);
	DF_OFSw(0);

	DF_COR_ONw(OnOff);
}
#endif

#if 1
void Isp_Dnr2d_Config(BOOL OnOff, BYTE Icsel, BYTE Ocsel)
{
	DNR_CSELw(Icsel);
	DNR_RCSELw(Ocsel);
	DNR2D_ONw(OnOff);
}
#else
//	OnOff		->	FN_ON, FN_OFF
//	Dnr2dMode	->	DNR2D_DTH_MOD (0) : Dnr2dDth 에 설정된 값보다 주변 difference가 작은 pixel에 대하여 reference로 사용하여 2d를 적용한다.
//					DNR2D_SUM_MOD (1) : Dnr2dCnt 에 설정된 pixel 개수 만큼 reference로 사용하여 2d dnr을 적용한다.
//	Dnr2dCnt	->	Dnr reference pixel count
//					DNR2D_CNT8 : Default 8 pixel
//	Dnr2dDth	->	'center pixel - near pixel' difference threshold
//	Dnr2dGain	->	2d dnr gain
void Isp_Dnr2d_Config(BOOL OnOff, BOOL Dnr2dMode, BYTE Dnr2dCnt, BYTE Dnr2dDth, BYTE Dnr2dGain)
{
	DNR2D_CNTw(Dnr2dCnt);
	DNR2D_DTHw(Dnr2dDth);
	DNR2D_GAINw(Dnr2dGain);
	DNR2D_MODw(Dnr2dMode);

	DNR2D_ONw(OnOff);
}
#endif

//	SD_MODw : 0 -> FRC 2 Page (Adr2, Adr3, Adr4 Don't care)
//			  1 -> FRC 3 Page (Adr3, Adr4 Don't care)
//			  4 -> FRC 5 Page
void Isp_Frc_Adr_Config(UINT Adr0, UINT Adr1, UINT Adr2, UINT Adr3, UINT Adr4)
{
	FRC_ADR0w(DDR_BASE_ISP+Adr0);
	FRC_ADR1w(DDR_BASE_ISP+Adr1);
	FRC_ADR2w(DDR_BASE_ISP+Adr2);
	FRC_ADR3w(DDR_BASE_ISP+Adr3);
	FRC_ADR4w(DDR_BASE_ISP+Adr4);
}




