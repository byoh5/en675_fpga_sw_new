#include "dev.h"
#include "isp_vlc_tbl.h"
#include "isp_cvbs_tbl.h"

//	Digital Input Interface------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
static UINT g_ConfigICh0 = 0;
static UINT g_ConfigICh1 = 0;
static UINT g_ConfigICh2 = 0;
static UINT g_ConfigICh3 = 0;

//	Pin : SS_DIN0 ~ SS_DIN15, SS_HS, SS_VS, SS_CKI
UINT Isp_DigIn_CH0_Config(UINT iMode, UINT ClkDly, UINT RegLatchPos, UINT Hsp, UINT Hw, UINT Vsp, UINT Vw, BOOL Set)
{
	if(Set==IF_MODE_SET)
	{
		//	Clock Power Down
		IBT_PCK_PDw(0);
		BT_PCK_PDw(0);
		BT1120_PCK_PDw(0);
		PCLK_HF_PDw(0);

		switch(iMode)
		{
			case IF_BT1120_8BIT_SDR:
				BT1120_SMODw(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MODw(1);				//	'1' : 8Bit BT1120
				YC_MODw(0);					//	'1' : 8 bit YC Input
				BT_MODESDw(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MODw(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDRw(0);				//	'1' : BT656 DDR Input Mode
				YC_SMODw(0);				//	'1' : 16Bit YC Input Mode
				Y_MODw(0);					//	'1' : Y 8Bit Only

				BT1120_PCK_SELw(0);			//	PCLK
				IBT_PCK_SELw(1);			//	PCLK/2
				BT_PCK_SELw(4);				//	PCLK/2
				BT1120_PCK_PDw(1);
				PCLK_HF_PDw(1);
			break;

			case IF_BT1120_8BIT_DDR:
				BT1120_SMODw(1);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MODw(1);             //	'1' : 8Bit BT1120
				YC_MODw(0);                 //	'1' : 8 bit YC Input
				BT_MODESDw(0);              //	'1' : BT565, '0' : BT1120
				BT1120_16MODw(0);           //	'1' : BT1120 16Bit Mode,
				BT656_DDRw(0);              //	'1' : BT656 DDR Input Mode
				YC_SMODw(0);                //	'1' : 16Bit YC Input Mode
				Y_MODw(0);                  //	'1' : Y 8Bit Only

				BT1120_PCK_SELw(0);			//	PCLK
				IBT_PCK_SELw(0);			//	PCLK
				BT_PCK_SELw(3);				//	PCLK
				BT1120_PCK_PDw(1);
			break;

			case IF_BT1120_16BIT:
				BT1120_SMODw(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MODw(0);				//	'1' : 8Bit BT1120
				YC_MODw(0);					//	'1' : 8 bit YC Input
				BT_MODESDw(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MODw(1);			//	'1' : BT1120 16Bit Mode,
				BT656_DDRw(0);				//	'1' : BT656 DDR Input Mode
				YC_SMODw(0);				//	'1' : 16Bit YC Input Mode
				Y_MODw(0);					//	'1' : Y 8Bit Only

				BT1120_PCK_SELw(0);			//	PCLK
				IBT_PCK_SELw(0);			//	PCLK
				BT_PCK_SELw(3);				//	PCLK
			break;

			case IF_BT656_SDR:
				BT1120_SMODw(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MODw(0);				//	'1' : 8Bit BT1120
				YC_MODw(0);					//	'1' : 8 bit YC Input
				BT_MODESDw(1);				//	'1' : BT565, '0' : BT1120
				BT1120_16MODw(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDRw(0);				//	'1' : BT656 DDR Input Mode
				YC_SMODw(0);				//	'1' : 16Bit YC Input Mode
				Y_MODw(0);					//	'1' : Y 8Bit Only

				BT1120_PCK_SELw(0);			//	PCLK
				IBT_PCK_SELw(0);			//	PCLK
				BT_PCK_SELw(4);				//	PCLK/2
				PCLK_HF_PDw(1);
			break;

			case IF_BT656_DDR:
				BT1120_SMODw(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MODw(1);				//	'1' : 8Bit BT1120
				YC_MODw(0);					//	'1' : 8 bit YC Input
				BT_MODESDw(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MODw(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDRw(1);				//	'1' : BT656 DDR Input Mode
				YC_SMODw(0);				//	'1' : 16Bit YC Input Mode
				Y_MODw(0);					//	'1' : Y 8Bit Only

				BT1120_PCK_SELw(0);			//	PCLK
				IBT_PCK_SELw(0);			//	PCLK
				BT_PCK_SELw(3);				//	PCLK
				BT1120_PCK_PDw(1);
			break;

			case IF_YC_8BIT:
				BT1120_SMODw(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MODw(0);				//	'1' : 8Bit BT1120
				YC_MODw(1);					//	'1' : 8 bit YC Input
				BT_MODESDw(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MODw(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDRw(0);				//	'1' : BT656 DDR Input Mode
				YC_SMODw(0);				//	'1' : 16Bit YC Input Mode
				Y_MODw(0);					//	'1' : Y 8Bit Only
				BT_TRG_HSIw(RegLatchPos);
				BT_TRG_VSIw(RegLatchPos);

				BT1120_PCK_SELw(0);			//	PCLK
				IBT_PCK_SELw(0);			//	PCLK
				BT_PCK_SELw(4);				//	PCLK/2
				PCLK_HF_PDw(1);
			break;

			case IF_YC_16BIT:
				BT1120_SMODw(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MODw(0);				//	'1' : 8Bit BT1120
				YC_MODw(0);					//	'1' : 8 bit YC Input
				BT_MODESDw(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MODw(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDRw(0);				//	'1' : BT656 DDR Input Mode
				YC_SMODw(1);				//	'1' : 16Bit YC Input Mode
				Y_MODw(0);					//	'1' : Y 8Bit Only
				BT_TRG_HSIw(RegLatchPos);
				BT_TRG_VSIw(RegLatchPos);

				BT1120_PCK_SELw(0);			//	PCLK
				IBT_PCK_SELw(0);			//	PCLK
				BT_PCK_SELw(3);				//	PCLK
			break;

			default:						//	BT1120 16Bit Mode
				BT1120_SMODw(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MODw(0);				//	'1' : 8Bit BT1120
				YC_MODw(0);					//	'1' : 8 bit YC Input
				BT_MODESDw(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MODw(1);			//	'1' : BT1120 16Bit Mode,
				BT656_DDRw(0);				//	'1' : BT656 DDR Input Mode
				YC_SMODw(0);				//	'1' : 16Bit YC Input Mode
				Y_MODw(0);					//	'1' : Y 8Bit Only

				BT1120_PCK_SELw(0);			//	PCLK
				IBT_PCK_SELw(1);			//	PCLK/2
				BT_PCK_SELw(4);				//	PCLK/2
				BT1120_PCK_PDw(1);
			break;
		}

		//	Clock Power On
		IBT_PCK_PDw(1);
		BT_PCK_PDw(1);
		PCLK_PDw(1);

		//	Resolution
		HSPIw(Hsp);
		VSPIw(Vsp);
		HWIw(Hw);
		VWIw(Vw);

		//	Internal Sync & Data Latch
		CH_DSELw(1);
		CH_HSSELw(1);
		CH_VSSELw(1);
		TRG_DINw(1);
		TRG_HSIw(1);
		TRG_VSIw(1);
		POL_HSIw(1);
		POL_VSIw(1);
		VSYN_NAONw(1);
		BT_TRG_DINw(RegLatchPos);			//	Pixel Data Latch Position : Pos or Neg

		PCLK_DLYw(ClkDly);					//	Pixel Clock Delay
		YCBCR_MODw(1);

		BT_DIN_SELw(1);						//	Channel 0 Digital Input Path Enable
		BT_OBUF_RSTw(1);					//	inbuf buffer reset
		DDRFF_RSw(1);						//	BT Decoder buffer reset
		DDRFF_RSw(0);
	}
	else
	{
		CH_DSELw(0);
		CH_HSSELw(0);
		CH_VSSELw(0);
		POL_HSIw(0);
		POL_VSIw(0);
		VSYN_NAONw(0);
		BT_DIN_SELw(0);

		//	Clock Power Down
		IBT_PCK_PDw(0);
		BT_PCK_PDw(0);
		PCLK_PDw(0);
		BT1120_PCK_PDw(0);
	}

	g_ConfigICh0 = iMode;
	return IF_SET_OK;
}

//	Pin : SS_DIN8 ~ SS_DIN15, GPIO4(Clock),   Option : GPIO7(Hsync), GPIO8(Vsync), GPIO9(Clock Out)
UINT Isp_DigIn_CH1_Config(UINT iMode, UINT ClkDly, UINT RegLatchPos, UINT Hsp, UINT Hw, UINT Vsp, UINT Vw, BOOL Set)
{
	if(Set==IF_MODE_SET)
	{
		//	Clock Power Down
		IBT0_PCK_PDw(0);
		BT0_PCK_PDw(0);
		BT1120_PCK0_PDw(0);
		PCLK_HF_PD0w(0);

		//GPIO MUX
		DSENS_CKI_ON0w(1);					//	GPIO4 -> PCLK0 Function

		switch(iMode)
		{
			case IF_BT1120_8BIT_SDR:
				BT1120_SMOD0w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD0w(1);			//	'1' : 8Bit BT1120
				YC_MOD0w(0);				//	'1' : 8 bit YC Input
				BT_MODESD0w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR0w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD0w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD0w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK0_SELw(0);		//	PCLK0	->	GPIO4
				IBT0_PCK_SELw(1);			//	PCLK0/2
				BT0_PCK_SELw(1);			//	PCLK0/2
				BT1120_PCK0_PDw(1);
				PCLK_HF_PD0w(1);
			break;

			case IF_BT1120_8BIT_DDR:
				BT1120_SMOD0w(1);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD0w(1);			//	'1' : 8Bit BT1120
				YC_MOD0w(0);				//	'1' : 8 bit YC Input
				BT_MODESD0w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR0w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD0w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD0w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK0_SELw(0);		//	PCLK0	->	GPIO4
				IBT0_PCK_SELw(0);			//	PCLK0
				BT0_PCK_SELw(0);			//	PCLK0
				BT1120_PCK0_PDw(1);
			break;

			case IF_BT1120_16BIT:
				return	IF_NO_MATCH_ERROR;	//	Not Supported
			break;

			case IF_BT656_SDR:
				BT1120_SMOD0w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD0w(0);			//	'1' : 8Bit BT1120
				YC_MOD0w(0);				//	'1' : 8 bit YC Input
				BT_MODESD0w(1);				//	'1' : BT565, '0' : BT1120
				BT656_DDR0w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD0w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD0w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK0_SELw(0);		//	PCLK0
				IBT0_PCK_SELw(0);			//	PCLK0
				BT0_PCK_SELw(1);			//	PCLK0/2
				PCLK_HF_PD0w(1);
			break;

			case IF_BT656_DDR:
				BT1120_SMOD0w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD0w(1);			//	'1' : 8Bit BT1120
				YC_MOD0w(0);				//	'1' : 8 bit YC Input
				BT_MODESD0w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR0w(1);				//	'1' : BT656 DDR Input Mode
				YC_SMOD0w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD0w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK0_SELw(0);		//	PCLK0	->	GPIO4
				IBT0_PCK_SELw(0);			//	PCLK0
				BT0_PCK_SELw(0);			//	PCLK0
				BT1120_PCK0_PDw(1);
			break;

			case IF_YC_8BIT:
				BT1120_SMOD0w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD0w(0);			//	'1' : 8Bit BT1120
				YC_MOD0w(1);				//	'1' : 8 bit YC Input
				BT_MODESD0w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR0w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD0w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD0w(0);					//	'1' : Y 8Bit Only
				DSENS_HON0w(1);				//	GPIO7	->	HSYNC
				DSENS_VON0w(1);				//	GPIO8	->	VSYNC
				BT_TRG_HSI0w(RegLatchPos);
				BT_TRG_VSI0w(RegLatchPos);

				BT1120_PCK0_SELw(0);		//	PCLK0	->	GPIO4
				IBT0_PCK_SELw(1);			//	PCLK0/2
				BT0_PCK_SELw(1);			//	PCLK0/2
				BT1120_PCK0_PDw(1);
				PCLK_HF_PD0w(1);
			break;

			case IF_YC_16BIT:
				return	IF_NO_MATCH_ERROR;	//	Not Supported
			break;

			default:						//	BT1120 16Bit Mode
				BT1120_SMOD0w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD0w(1);			//	'1' : 8Bit BT1120
				YC_MOD0w(0);				//	'1' : 8 bit YC Input
				BT_MODESD0w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR0w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD0w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD0w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK0_SELw(0);		//	PCLK0	->	GPIO4
				IBT0_PCK_SELw(1);			//	PCLK0/2
				BT0_PCK_SELw(1);			//	PCLK0/2
				BT1120_PCK0_PDw(1);
				PCLK_HF_PD0w(1);
			break;
		}

		//	Clock Power On
		IBT0_PCK_PDw(1);
		BT0_PCK_PDw(1);
		PCLK0_PDw(1);

		HSPI0w(Hsp);
		VSPI0w(Vsp);
		HWI0w(Hw);
		VWI0w(Vw);

		POL_HSI0w(1);
		POL_VSI0w(1);
		VSYN_NAON0w(1);

		BT_TRG_DIN0w(RegLatchPos);			//	Pixel Data Latch Position : Pos or Neg
		PCLK0_DLYw(ClkDly);					//	Pixel Clock Delay
		YCBCR_MOD0w(1);

		BT_OBUF_RST0w(1);					//	inbuf buffer reset
		DDRFF_RS0w(1);						//	BT Decoder buffer reset
	}
	else
	{
		VSYN_NAON0w(0);
		IBT0_PCK_PDw(0);
		BT0_PCK_PDw(0);
		PCLK0_PDw(0);
		DSENS_CKI_ON0w(0);
		DSENS_HON0w(0);
		DSENS_VON0w(0);

	}

	g_ConfigICh1 = iMode;
	return IF_SET_OK;
}

//	Pin : SS_DIN16 ~ SS_DIN31, GPIO5(Clock),   Option : GPIO10(Hsync), GPIO11(Vsync), GPIO12(Clock Out)
UINT Isp_DigIn_CH2_Config(UINT iMode, UINT ClkDly, UINT RegLatchPos, UINT Hsp, UINT Hw, UINT Vsp, UINT Vw, BOOL Set)
{
	if(Set==IF_MODE_SET)
	{
		//	Clock Power Down
		IBT1_PCK_PDw(0);
		BT1_PCK_PDw(0);
		BT1120_PCK1_PDw(0);
		PCLK_HF_PD1w(0);

		//GPIO MUX
		DSENS_CKI_ON1w(1);					//	GPIO5 -> PCLK1 Function

		switch(iMode)
		{
			case IF_BT1120_8BIT_SDR:
				BT1120_SMOD1w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD1w(1);			//	'1' : 8Bit BT1120
				YC_MOD1w(0);				//	'1' : 8 bit YC Input
				BT_MODESD1w(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MOD1w(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDR1w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD1w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD1w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK1_SELw(0);		//	PCLK1	->	GPIO5
				IBT1_PCK_SELw(1);			//	PCLK1/2
				BT1_PCK_SELw(1);			//	PCLK1/2
				BT1120_PCK1_PDw(1);
				PCLK_HF_PD1w(1);
			break;

			case IF_BT1120_8BIT_DDR:
				BT1120_SMOD1w(1);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD1w(1);			//	'1' : 8Bit BT1120
				YC_MOD1w(0);				//	'1' : 8 bit YC Input
				BT_MODESD1w(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MOD1w(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDR1w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD1w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD1w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK1_SELw(0);		//	PCLK1	->	GPIO5
				IBT1_PCK_SELw(0);			//	PCLK1
				BT1_PCK_SELw(0);			//	PCLK1
				BT1120_PCK1_PDw(1);
			break;

			case IF_BT1120_16BIT:
				BT1120_SMOD1w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD1w(0);			//	'1' : 8Bit BT1120
				YC_MOD1w(0);				//	'1' : 8 bit YC Input
				BT_MODESD1w(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MOD1w(1);			//	'1' : BT1120 16Bit Mode,
				BT656_DDR1w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD1w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD1w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK1_SELw(0);		//	PCLK1
				IBT1_PCK_SELw(0);			//	PCLK1
				BT1_PCK_SELw(0);			//	PCLK1
			break;

			case IF_BT656_SDR:
				BT1120_SMOD1w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD1w(0);			//	'1' : 8Bit BT1120
				YC_MOD1w(0);				//	'1' : 8 bit YC Input
				BT_MODESD1w(1);				//	'1' : BT565, '0' : BT1120
				BT1120_16MOD1w(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDR1w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD1w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD1w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK1_SELw(0);		//	PCLK1
				IBT1_PCK_SELw(0);			//	PCLK1
				BT1_PCK_SELw(1);			//	PCLK1/2
				PCLK_HF_PD1w(1);
			break;

			case IF_BT656_DDR:
				BT1120_SMOD1w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD1w(1);			//	'1' : 8Bit BT1120
				YC_MOD1w(0);				//	'1' : 8 bit YC Input
				BT_MODESD1w(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MOD1w(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDR1w(1);				//	'1' : BT656 DDR Input Mode
				YC_SMOD1w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD1w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK1_SELw(0);		//	PCLK1	->	GPIO5
				IBT1_PCK_SELw(0);			//	PCLK1
				BT1_PCK_SELw(0);			//	PCLK1
				BT1120_PCK1_PDw(1);
			break;

			case IF_YC_8BIT:
				BT1120_SMOD1w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD1w(0);			//	'1' : 8Bit BT1120
				YC_MOD1w(1);				//	'1' : 8 bit YC Input
				BT_MODESD1w(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MOD1w(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDR1w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD1w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD1w(0);					//	'1' : Y 8Bit Only
				DSENS_HON1w(1);				//	GPIO7	->	HSYNC
				DSENS_VON1w(1);				//	GPIO8	->	VSYNC
				BT_TRG_HSI1w(RegLatchPos);
				BT_TRG_VSI1w(RegLatchPos);

				BT1120_PCK1_SELw(0);		//	PCLK1	->	GPIO5
				IBT1_PCK_SELw(1);			//	PCLK1/2
				BT1_PCK_SELw(1);			//	PCLK1/2
				BT1120_PCK1_PDw(1);
				PCLK_HF_PD1w(1);
			break;

			case IF_YC_16BIT:
				BT1120_SMOD1w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD1w(0);			//	'1' : 8Bit BT1120
				YC_MOD1w(0);				//	'1' : 8 bit YC Input
				BT_MODESD1w(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MOD1w(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDR1w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD1w(1);				//	'1' : 16Bit YC Input Mode
				Y_MOD1w(0);					//	'1' : Y 8Bit Only
				BT_TRG_HSI1w(RegLatchPos);
				BT_TRG_VSI1w(RegLatchPos);

				BT1120_PCK1_SELw(0);		//	PCLK1
				IBT1_PCK_SELw(0);			//	PCLK1
				BT1_PCK_SELw(0);			//	PCLK1
			break;

			default:						//	BT1120 16Bit Mode
				BT1120_SMOD1w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD1w(1);			//	'1' : 8Bit BT1120
				YC_MOD1w(0);				//	'1' : 8 bit YC Input
				BT_MODESD1w(0);				//	'1' : BT565, '0' : BT1120
				BT1120_16MOD1w(0);			//	'1' : BT1120 16Bit Mode,
				BT656_DDR1w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD1w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD1w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK1_SELw(0);		//	PCLK1	->	GPIO4
				IBT1_PCK_SELw(1);			//	PCLK1/2
				BT1_PCK_SELw(1);			//	PCLK1/2
				BT1120_PCK1_PDw(1);
				PCLK_HF_PD1w(1);
			break;
		}

		//	Clock Power On
		IBT1_PCK_PDw(1);
		BT1_PCK_PDw(1);
		PCLK1_PDw(1);

		HSPI1w(Hsp);
		VSPI1w(Vsp);
		HWI1w(Hw);
		VWI1w(Vw);

		POL_HSI1w(1);
		POL_VSI1w(1);
		VSYN_NAON1w(1);

		BT_TRG_DIN1w(RegLatchPos);			//	Pixel Data Latch Position : Pos or Neg
		PCLK1_DLYw(ClkDly);					//	Pixel Clock Delay
		YCBCR_MOD1w(1);

		BT_OBUF_RST1w(1);					//	inbuf buffer reset
		DDRFF_RS1w(1);						//	BT Decoder buffer reset
	}
	else
	{
		VSYN_NAON1w(0);
		IBT1_PCK_PDw(0);
		BT1_PCK_PDw(0);
		PCLK1_PDw(0);
		DSENS_CKI_ON1w(0);
		DSENS_HON1w(0);
		DSENS_VON1w(0);

	}

	g_ConfigICh2 = iMode;
	return IF_SET_OK;
}


//	Pin : SS_DIN24 ~ SS_DIN31, GPIO6(Clock),   Option : GPIO13(Hsync), GPIO14(Vsync), GPIO15(Clock Out)
UINT Isp_DigIn_CH3_Config(UINT iMode, UINT ClkDly, UINT RegLatchPos, UINT Hsp, UINT Hw, UINT Vsp, UINT Vw, BOOL Set)
{
	if(Set==IF_MODE_SET)
	{
		//	Clock Power Down
		IBT2_PCK_PDw(0);
		BT2_PCK_PDw(0);
		BT1120_PCK2_PDw(0);
		PCLK_HF_PD2w(0);

		//GPIO MUX
		DSENS_CKI_ON2w(1);					//	GPIO4 -> PCLK0 Function

		switch(iMode)
		{
			case IF_BT1120_8BIT_SDR:
				BT1120_SMOD2w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD2w(1);			//	'1' : 8Bit BT1120
				YC_MOD2w(0);				//	'1' : 8 bit YC Input
				BT_MODESD2w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR2w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD2w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD2w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK2_SELw(0);		//	PCLK0	->	GPIO4
				IBT2_PCK_SELw(1);			//	PCLK0/2
				BT2_PCK_SELw(1);			//	PCLK0/2
				BT1120_PCK2_PDw(1);
				PCLK_HF_PD2w(1);
			break;

			case IF_BT1120_8BIT_DDR:
				BT1120_SMOD2w(1);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD2w(1);			//	'1' : 8Bit BT1120
				YC_MOD2w(0);				//	'1' : 8 bit YC Input
				BT_MODESD2w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR2w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD2w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD2w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK2_SELw(0);		//	PCLK0	->	GPIO4
				IBT2_PCK_SELw(0);			//	PCLK0
				BT2_PCK_SELw(0);			//	PCLK0
				BT1120_PCK2_PDw(1);
			break;

			case IF_BT1120_16BIT:
				return	IF_NO_MATCH_ERROR;	//	Not Supported
			break;

			case IF_BT656_SDR:
				BT1120_SMOD2w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD2w(0);			//	'1' : 8Bit BT1120
				YC_MOD2w(0);				//	'1' : 8 bit YC Input
				BT_MODESD2w(1);				//	'1' : BT565, '0' : BT1120
				BT656_DDR2w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD2w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD2w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK2_SELw(0);		//	PCLK0
				IBT2_PCK_SELw(0);			//	PCLK0
				BT2_PCK_SELw(1);			//	PCLK0/2
				PCLK_HF_PD2w(1);
			break;

			case IF_BT656_DDR:
				BT1120_SMOD2w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD2w(1);			//	'1' : 8Bit BT1120
				YC_MOD2w(0);				//	'1' : 8 bit YC Input
				BT_MODESD2w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR2w(1);				//	'1' : BT656 DDR Input Mode
				YC_SMOD2w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD2w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK2_SELw(0);		//	PCLK0	->	GPIO4
				IBT2_PCK_SELw(0);			//	PCLK0
				BT2_PCK_SELw(0);			//	PCLK0
				BT1120_PCK2_PDw(1);
			break;

			case IF_YC_8BIT:
				BT1120_SMOD2w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD2w(0);			//	'1' : 8Bit BT1120
				YC_MOD2w(1);				//	'1' : 8 bit YC Input
				BT_MODESD2w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR2w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD2w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD2w(0);					//	'1' : Y 8Bit Only
				DSENS_HON2w(1);				//	GPIO7	->	HSYNC
				DSENS_VON2w(1);				//	GPIO8	->	VSYNC
				BT_TRG_HSI2w(RegLatchPos);
				BT_TRG_VSI2w(RegLatchPos);

				BT1120_PCK2_SELw(0);		//	PCLK0	->	GPIO4
				IBT2_PCK_SELw(1);			//	PCLK0/2
				BT2_PCK_SELw(1);			//	PCLK0/2
				BT1120_PCK2_PDw(1);
				PCLK_HF_PD2w(1);
			break;

			case IF_YC_16BIT:
				return	IF_NO_MATCH_ERROR;	//	Not Supported
			break;

			default:						//	BT1120 16Bit Mode
				BT1120_SMOD2w(0);			//	'1' : BT1120 DDR, '0' : BT1120 SDR
				BT1120_MOD2w(1);			//	'1' : 8Bit BT1120
				YC_MOD2w(0);				//	'1' : 8 bit YC Input
				BT_MODESD2w(0);				//	'1' : BT565, '0' : BT1120
				BT656_DDR2w(0);				//	'1' : BT656 DDR Input Mode
				YC_SMOD2w(0);				//	'1' : 16Bit YC Input Mode
				Y_MOD2w(0);					//	'1' : Y 8Bit Only

				BT1120_PCK2_SELw(0);		//	PCLK0	->	GPIO4
				IBT2_PCK_SELw(1);			//	PCLK0/2
				BT2_PCK_SELw(1);			//	PCLK0/2
				BT1120_PCK2_PDw(1);
				PCLK_HF_PD2w(1);
			break;
		}

		//	Clock Power On
		IBT2_PCK_PDw(1);
		BT2_PCK_PDw(1);
		PCLK2_PDw(1);

		HSPI2w(Hsp);
		VSPI2w(Vsp);
		HWI2w(Hw);
		VWI2w(Vw);

		POL_HSI2w(1);
		POL_VSI2w(1);
		VSYN_NAON2w(1);

		BT_TRG_DIN2w(RegLatchPos);			//	Pixel Data Latch Position : Pos or Neg
		PCLK2_DLYw(ClkDly);					//	Pixel Clock Delay
		YCBCR_MOD2w(1);

		BT_OBUF_RST2w(1);					//	inbuf buffer reset
		DDRFF_RS2w(1);						//	BT Decoder buffer reset
	}
	else
	{
		VSYN_NAON2w(0);
		IBT2_PCK_PDw(0);
		BT2_PCK_PDw(0);
		PCLK2_PDw(0);
		DSENS_CKI_ON2w(0);
		DSENS_HON2w(0);
		DSENS_VON2w(0);

	}

	g_ConfigICh3 = iMode;
	return IF_SET_OK;
}

//IFO_OPIN_0TO7
//IFO_OPIN_8TO15
//IFO_OPIN_16TO23
//IFO_OPIN_24TO31
//IFO_OPIN_0TO15
//IFO_OPIN_16TO31
//IFO_OPIN_0TO23


//	Internal Function
void DoCk_Set(UINT PinList, UINT Clk)
{
	if(PinList==IFO_OPIN_0TO7)			{	DO0_CK_SET(Clk);    }
	else if(PinList==IFO_OPIN_8TO15)	{	DO1_CK_SET(Clk);    }
	else if(PinList==IFO_OPIN_16TO23)	{	DO2_CK_SET(Clk);    }
	else if(PinList==IFO_OPIN_24TO31)	{	DO3_CK_SET(Clk);	}
	else if(PinList==IFO_OPIN_0TO15)	{	DO0_CK_SET(Clk);	DO1_CK_SET(Clk);	}
	else if(PinList==IFO_OPIN_16TO31)	{	DO2_CK_SET(Clk);	DO3_CK_SET(Clk);	}
	else if(PinList==IFO_OPIN_0TO23)	{	DO0_CK_SET(Clk);	DO1_CK_SET(Clk);	DO2_CK_SET(Clk);	}
}

//	Digital Output Interface-----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//
//	Output Mode Configuration
//	BT1120 16BIT(DO0 ~ DO15)  + BT1120 16BIT(DO16 ~ DO31)								->	Use DCKO0, DCKO1
//	BT1120 8 BIT(DO0 ~ DO7)   + BT1120 8 BIT(DO8  ~ DO15) + LCD MCU 16BIT(DO16 ~ DO31)	->	Use DCKO0, DCKO1, DCKO2(LCD)
//	LCD RGB 24BIT(DO0 ~ DO23) + BT1120 8 BIT(DO24 ~ DO31)								->	Use DCKO0, DCKO2(LCD)
//	LCD MCU 16BIT(DO0 ~ DO15) + BT1120 8 BIT(DO16 ~ DO23) + BT1120 8 BIT(DO24 ~ DO31)	->	Use DCKO0, DCKO1, DCKO2(LCD)
//
//	PinList	->	IFO_OPIN_0TO7		//	Use pDO0 	~ 	pDO7
//				IFO_OPIN_8TO15	    //	Use pDO8 	~ 	pDO15
//				IFO_OPIN_16TO23	    //	Use pDO16 	~ 	pDO23
//				IFO_OPIN_24TO31	    //	Use pGPIO24 ~ 	pGPIO31
//				IFO_OPIN_0TO15	    //	Use pDO0 	~ 	pDO15
//				IFO_OPIN_16TO31	    //	Use pDO16 	~ 	pDO23, pGPIO24 ~ pGPIO31
//				IFO_OPIN_0TO23	    //	Use pDO0 	~ 	pDO23
//
//	Mode	->	IFO_BT1120_16BIT
//				IFO_BT1120_8BIT_SDR
//				IFO_BT1120_8BIT_DDR
//				IFO_BT656_SDR
//				IFO_BT656_DDR
//				IFO_LCD_MCU_16BIT
//				IFO_LCD_SPI_18BIT
//				IFO_LCD_RGB_24BIT
//
//	Rsync	->	IFO_ISP_SYNC		//	ISP Post Sync
//				IFO_RD_SYNC1        //	Internal Sync Generator 1 Sync
//				IFO_RD_SYNC2        //	Internal Sync Generator 2 Sync
//				IFO_RD_SYNC3        //	Internal Sync Generator 3 Sync
//
//	Res		->	IFO_720_25P
//				IFO_720_30P
//				IFO_720_50P
//				IFO_720_60P
//				IFO_1080_25P
//				IFO_1080_30P
//				IFO_1080_50P
//				IFO_1080_60P
//				IFO_1440_25P
//				IFO_1440_30P
//
//	Path	->	IFO_DIG_RDCH1_PATH			//	DDR Read Channel 1
//				IFO_DIG_RDCH2_PATH			//	DDR Read Channel 2
//				IFO_DIG_RDCH3_PATH			//	DDR Read Channel 3
//				IFO_DIG_RDCH4_PATH			//	DDR Read Channel 4
//				IFO_DIG_CH0_DIRT_PATH		//	Digital Input Channel 0 Direct Path
//				IFO_DIG_CH1_DIRT_PATH		//	Digital Input Channel 1 Direct Path
//				IFO_DIG_CH2_DIRT_PATH		//	Digital Input Channel 2 Direct Path
//				IFO_DIG_CH3_DIRT_PATH		//	Digital Input Channel 3 Direct Path
//				IFO_DIG_RDCH1_FONT_PATH		//	DDR Read Channel 1 + Font Path
//				IFO_DIG_RDCH2_FONT_PATH		//  DDR Read Channel 2 + Font Path
//				IFO_DIG_RDCH3_FONT_PATH		//  DDR Read Channel 3 + Font Path
//				IFO_DIG_RDCH4_FONT_PATH		//  DDR Read Channel 4 + Font Path
//				IFO_DIG_CH0_DIRT_FONT_PATH	//	Digital Input Channel 0 Direct Path + Font
//				IFO_DIG_CH1_DIRT_FONT_PATH	//	Digital Input Channel 1 Direct Path + Font
//				IFO_DIG_CH2_DIRT_FONT_PATH	//	Digital Input Channel 2 Direct Path + Font
//				IFO_DIG_CH3_DIRT_FONT_PATH	//	Digital Input Channel 3 Direct Path + Font
//				IFO_ISP_PATH				//	ISP Path (Normal Operation)
//				IFO_DIG_PIP_FONT_PATH		//	PIP Only Path + Font
//				IFO_DIG_MERGE_FONT_PATH		//	Image Merge Path + Font Path
//				IFO_DIG_DZOOM_FONT_PATH		//	Digital Input DZoom Path + Font
//	Vsp	->		Vertical Start Position
//	Hsp	->		Horizontal Start Position

static UINT Use0To7		=	USE_EMPTY;
static UINT Use8To15	=	USE_EMPTY;
static UINT Use16To23	=	USE_EMPTY;
static UINT Use24To31	=	USE_EMPTY;

UINT Isp_DigOut_Config(UINT PinList, UINT Mode, UINT Rsync, UINT Res, UINT Path, UINT Hsp, UINT Vsp)
{
	UINT HTW = 0x896;
	UINT VTW = 0x464;
	UINT HW	 = 0x780;
	UINT VW	 = 0x438;
	UINT FRQ = 0;
	UINT IFRQ = 0;

	//	Output Interface Pin Enable
	switch(PinList)
	{
		case IFO_OPIN_0TO7		:	DO_OENw(DO_OENr&0xFFFFFF00);		break;
		case IFO_OPIN_8TO15		:	DO_OENw(DO_OENr&0xFFFF00FF);        break;
		case IFO_OPIN_16TO23	:	DO_OENw(DO_OENr&0xFF00FFFF);        break;
		case IFO_OPIN_24TO31	:	DO_OENw(DO_OENr&0x00FFFFFF);    	break;
		case IFO_OPIN_0TO15		:	DO_OENw(DO_OENr&0xFFFF0000);        break;
		case IFO_OPIN_16TO31	:	DO_OENw(DO_OENr&0x0000FFFF);        break;
		case IFO_OPIN_0TO23		:	DO_OENw(DO_OENr&0xFF000000);        break;
	}

	//	Select Resolution
	switch(Res)
	{
		case IFO_720_25P	:	HTW = 3960-2; VTW = 750-1;	HW = 1280;	VW = 720;	FRQ = FRQ_74M;	break;
		case IFO_720_30P	:	HTW = 3300-2; VTW = 750-1;	HW = 1280;	VW = 720;	FRQ = FRQ_74M;	break;
		case IFO_720_50P    :	HTW = 1980-2; VTW = 750-1;	HW = 1280;	VW = 720;	FRQ = FRQ_74M;	break;
		case IFO_720_60P    :	HTW = 1650-2; VTW = 750-1;	HW = 1280;	VW = 720;	FRQ = FRQ_74M;	break;
		case IFO_1080_25P   :	HTW = 2640-2; VTW =	1125-1;	HW = 1920;	VW = 1080;	FRQ = FRQ_74M;	break;
		case IFO_1080_30P   :	HTW = 2200-2; VTW =	1125-1;	HW = 1920;	VW = 1080;	FRQ = FRQ_74M;	break;
		case IFO_1080_50P   :	HTW = 2640-2; VTW =	1125-1;	HW = 1920;	VW = 1080;	FRQ = FRQ_148M;	break;
		case IFO_1080_60P   :	HTW = 2200-2; VTW =	1125-1;	HW = 1920;	VW = 1080;	FRQ = FRQ_148M;	break;
		case IFO_1440_25P   :	HTW = 3960-2; VTW =	1650-1;	HW = 1920;	VW = 1080;	FRQ = FRQ_148M;	break;
		case IFO_1440_30P   :	HTW = 3300-2; VTW =	1650-1;	HW = 1920;	VW = 1080;	FRQ = FRQ_148M;	break;
	}

	//	Select Sync Generator
	switch(Rsync)
	{
		case IFO_ISP_SYNC:	DZOOM_VSPw(Vsp);	DZOOM_HSPw(Hsp);	break;
		case IFO_RD_SYNC1:	RYC_OSYNC_MOD1w(1);	RYC_HTWO1w(HTW);	RYC_VTWO1w(VTW);	RYC_HWI1w(HW);	RYC_VWI1w(VW);		RYC_HSPI1w(Hsp);	RYC_VSPI1w(Vsp);	RYCCK1_SET(FRQ);	break;
		case IFO_RD_SYNC2:	RYC_OSYNC_MOD2w(1);	RYC_HTWO2w(HTW);	RYC_VTWO2w(VTW);	RYC_HWI2w(HW);	RYC_VWI2w(VW);		RYC_HSPI2w(Hsp);	RYC_VSPI2w(Vsp);	RYCCK2_SET(FRQ);	break;
		case IFO_RD_SYNC3:	RYC_OSYNC_MOD3w(1);	RYC_HTWO3w(HTW);	RYC_VTWO3w(VTW);	RYC_HWI3w(HW);	RYC_VWI3w(VW);		RYC_HSPI3w(Hsp);	RYC_VSPI3w(Vsp);	RYCCK3_SET(FRQ);	break;
	}

	switch(Path)
	{
		case IFO_ISP_PATH:
			ITU_SSEL0w(0);	ITU_ASEL0w(0);
			if((PinList==IFO_OPIN_16TO31)||(PinList==IFO_OPIN_24TO31))
			{
				if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))	{	DITCK1_SET(FRQ);	OTCK1_SET(FRQ);		ITCK1_SET(FRQ);		DO2_CK_SET(FRQ);	DO3_CK_SET(FRQ);	}
				else if(Mode==IFO_BT1120_8BIT_SDR)							{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	ITCK1_SET(FRQ);		DO2_CK_SET(FRQ-1);	DO3_CK_SET(FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
				else														{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	ITCK1_SET(FRQ);		DO2_CK_SET(FRQ-1);	DO3_CK_SET(FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);}
			}
			else {
				if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))	{	DITCK0_SET(FRQ);	OTCK0_SET(FRQ);		ITCK0_SET(FRQ);		DO0_CK_SET(FRQ);	DO1_CK_SET(FRQ);	}
				else if(Mode==IFO_BT1120_8BIT_SDR)							{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	ITCK0_SET(FRQ);		DO0_CK_SET(FRQ-1);	DO1_CK_SET(FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
				else														{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	ITCK0_SET(FRQ);		DO0_CK_SET(FRQ-1);	DO1_CK_SET(FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);}
			}
		break;

		case IFO_DIG_RDCH1_PATH:
			POST_OSEL1w(6);		ITU_ASEL1w(10);
			if(Rsync==IFO_ISP_SYNC)			{	ITU_SSEL1w(0);  }
			else if(Rsync==IFO_RD_SYNC1)	{	ITU_SSEL1w(5);  }
			else if(Rsync==IFO_RD_SYNC2)	{	ITU_SSEL1w(6);  }
			else 							{	ITU_SSEL1w(7);	}
			ITCK1_SET(FRQ);
			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK1_SET(FRQ);	OTCK1_SET(FRQ);		DoCk_Set(PinList, FRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
			else															{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);}
		break;

		case IFO_DIG_RDCH2_PATH:
			POST_OSEL1w(7);		ITU_ASEL1w(11);
			if(Rsync==IFO_ISP_SYNC)			{	ITU_SSEL1w(0);  }
			else if(Rsync==IFO_RD_SYNC1)	{	ITU_SSEL1w(5);  }
			else if(Rsync==IFO_RD_SYNC2)	{	ITU_SSEL1w(6);  }
			else 							{	ITU_SSEL1w(7);	}
			ITCK1_SET(FRQ);
			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK1_SET(FRQ);	OTCK1_SET(FRQ);		DoCk_Set(PinList, FRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
			else															{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);}
		break;

		case IFO_DIG_RDCH3_PATH:
			POST_OSEL1w(8);		ITU_ASEL1w(12);
			if(Rsync==IFO_ISP_SYNC)			{	ITU_SSEL1w(0);  }
			else if(Rsync==IFO_RD_SYNC1)	{	ITU_SSEL1w(5);  }
			else if(Rsync==IFO_RD_SYNC2)	{	ITU_SSEL1w(6);  }
			else 							{	ITU_SSEL1w(7);	}
			ITCK1_SET(FRQ);
			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK1_SET(FRQ);	OTCK1_SET(FRQ);		DoCk_Set(PinList, FRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
			else															{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);}
		break;

		case IFO_DIG_RDCH4_PATH:
			POST_OSEL1w(9);		ITU_ASEL1w(13);
			if(Rsync==IFO_ISP_SYNC)			{	ITU_SSEL1w(0);  }
			else if(Rsync==IFO_RD_SYNC1)	{	ITU_SSEL1w(5);  }
			else if(Rsync==IFO_RD_SYNC2)	{	ITU_SSEL1w(6);  }
			else 							{	ITU_SSEL1w(7);	}
			ITCK1_SET(FRQ);
			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK1_SET(FRQ);	OTCK1_SET(FRQ);		DoCk_Set(PinList, FRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
			else															{	DITCK1_SET(FRQ-1);	OTCK1_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);}
		break;

		case IFO_DIG_RDCH1_FONT_PATH:
			OSD_ISEL0w(6);		POST_OSEL0w(0);		ITU_ASEL0w(15);
			if(Rsync==IFO_ISP_SYNC)			{	OSD_IVSEL0w(5);	ITU_SSEL0w(0);	}
			else if(Rsync==IFO_RD_SYNC1)	{	OSD_IVSEL0w(5);	ITU_SSEL0w(5);  }
			else if(Rsync==IFO_RD_SYNC2)	{	OSD_IVSEL0w(6);	ITU_SSEL0w(6);  }
			else 							{	OSD_IVSEL0w(7);	ITU_SSEL0w(7);  }
			FN_CK0_SET(FRQ);	PR_CK0_SET(FRQ);	ITCK0_SET(FRQ);
			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK0_SET(FRQ);	OTCK0_SET(FRQ);		DoCk_Set(PinList, FRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
			else															{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);}
		break;

		case IFO_DIG_RDCH2_FONT_PATH:
			OSD_ISEL0w(7);		POST_OSEL0w(0);		ITU_ASEL0w(16);
			if(Rsync==IFO_ISP_SYNC)			{	OSD_IVSEL0w(5);	ITU_SSEL0w(0);	}
			else if(Rsync==IFO_RD_SYNC1)	{	OSD_IVSEL0w(5);	ITU_SSEL0w(5);  }
			else if(Rsync==IFO_RD_SYNC2)	{	OSD_IVSEL0w(6);	ITU_SSEL0w(6);  }
			else 							{	OSD_IVSEL0w(7);	ITU_SSEL0w(7);  }
			FN_CK0_SET(FRQ);	PR_CK0_SET(FRQ);	ITCK0_SET(FRQ);
			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK0_SET(FRQ);	OTCK0_SET(FRQ);		DoCk_Set(PinList, FRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
			else															{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);}
		break;

		case IFO_DIG_RDCH3_FONT_PATH:
			OSD_ISEL0w(8);		POST_OSEL0w(0);		ITU_ASEL0w(17);
			if(Rsync==IFO_ISP_SYNC)			{	OSD_IVSEL0w(5);	ITU_SSEL0w(0);	}
			else if(Rsync==IFO_RD_SYNC1)	{	OSD_IVSEL0w(5);	ITU_SSEL0w(5);  }
			else if(Rsync==IFO_RD_SYNC2)	{	OSD_IVSEL0w(6);	ITU_SSEL0w(6);  }
			else 							{	OSD_IVSEL0w(7);	ITU_SSEL0w(7);  }
			FN_CK0_SET(FRQ);	PR_CK0_SET(FRQ);	ITCK0_SET(FRQ);
			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK0_SET(FRQ);	OTCK0_SET(FRQ);		DoCk_Set(PinList, FRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
			else															{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);}
		break;

		case IFO_DIG_RDCH4_FONT_PATH:
			OSD_ISEL0w(9);		POST_OSEL0w(0);		ITU_ASEL0w(18);
			if(Rsync==IFO_ISP_SYNC)			{	OSD_IVSEL0w(5);	ITU_SSEL0w(0);	}
			else if(Rsync==IFO_RD_SYNC1)	{	OSD_IVSEL0w(5);	ITU_SSEL0w(5);  }
			else if(Rsync==IFO_RD_SYNC2)	{	OSD_IVSEL0w(6);	ITU_SSEL0w(6);  }
			else 							{	OSD_IVSEL0w(7);	ITU_SSEL0w(7);  }
			FN_CK0_SET(FRQ);	PR_CK0_SET(FRQ);	ITCK0_SET(FRQ);
			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK0_SET(FRQ);	OTCK0_SET(FRQ);		DoCk_Set(PinList, FRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
			else															{	DITCK0_SET(FRQ-1);	OTCK0_SET(FRQ-1);	DoCk_Set(PinList, FRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);}
		break;

		case IFO_DIG_CH0_DIRT_PATH:
			POST_OSEL1w(10);	ITU_SSEL1w(1);		ITU_ASEL1w(1);
			if((g_ConfigICh0==IF_BT1120_8BIT_SDR)||(g_ConfigICh0==IF_BT656_SDR)||(g_ConfigICh0==IF_YC_8BIT))	{	ITCK1_SET(7);	IFRQ = 7;	}
			else																								{	ITCK1_SET(3);	IFRQ = 3; 	}

			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK1_SET(IFRQ);	OTCK1_SET(IFRQ);	DoCk_Set(PinList, IFRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{
				if((g_ConfigICh0==IF_BT1120_8BIT_SDR)||(g_ConfigICh0==IF_BT656_SDR)||(g_ConfigICh0==IF_YC_8BIT))	{	DITCK1_SET(IFRQ-4);	OTCK1_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
			else															{
				if((g_ConfigICh0==IF_BT1120_8BIT_SDR)||(g_ConfigICh0==IF_BT656_SDR)||(g_ConfigICh0==IF_YC_8BIT))	{	DITCK1_SET(IFRQ-4);	OTCK1_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
		break;

		case IFO_DIG_CH1_DIRT_PATH:
			POST_OSEL1w(11);		ITU_SSEL1w(2);		ITU_ASEL1w(2);
			if((g_ConfigICh1==IF_BT1120_8BIT_SDR)||(g_ConfigICh1==IF_BT656_SDR)||(g_ConfigICh1==IF_YC_8BIT))	{	ITCK1_SET(8);	IFRQ = 8;	}
			else																								{	ITCK1_SET(4);	IFRQ = 4; 	}

			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK1_SET(IFRQ);	OTCK1_SET(IFRQ);	DoCk_Set(PinList, IFRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{
				if((g_ConfigICh1==IF_BT1120_8BIT_SDR)||(g_ConfigICh1==IF_BT656_SDR)||(g_ConfigICh1==IF_YC_8BIT))	{	DITCK1_SET(IFRQ-4);	OTCK1_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
			else															{
				if((g_ConfigICh1==IF_BT1120_8BIT_SDR)||(g_ConfigICh1==IF_BT656_SDR)||(g_ConfigICh1==IF_YC_8BIT))	{	DITCK1_SET(IFRQ-4);	OTCK1_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
		break;

		case IFO_DIG_CH2_DIRT_PATH:
			POST_OSEL1w(12);		ITU_SSEL1w(3);		ITU_ASEL1w(3);
			if((g_ConfigICh2==IF_BT1120_8BIT_SDR)||(g_ConfigICh2==IF_BT656_SDR)||(g_ConfigICh2==IF_YC_8BIT))	{	ITCK1_SET(9);	IFRQ = 9;	}
			else																								{	ITCK1_SET(5);	IFRQ = 5; 	}

			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK1_SET(IFRQ);	OTCK1_SET(IFRQ);	DoCk_Set(PinList, IFRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{
				if((g_ConfigICh2==IF_BT1120_8BIT_SDR)||(g_ConfigICh2=IF_BT656_SDR)||(g_ConfigICh2==IF_YC_8BIT))		{	DITCK1_SET(IFRQ-4);	OTCK1_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
			else															{
				if((g_ConfigICh2==IF_BT1120_8BIT_SDR)||(g_ConfigICh2==IF_BT656_SDR)||(g_ConfigICh2==IF_YC_8BIT))	{	DITCK1_SET(IFRQ-4);	OTCK1_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
		break;

		case IFO_DIG_CH3_DIRT_PATH:
			POST_OSEL1w(13);		ITU_SSEL1w(4);		ITU_ASEL1w(4);
			if((g_ConfigICh3==IF_BT1120_8BIT_SDR)||(g_ConfigICh3==IF_BT656_SDR)||(g_ConfigICh3==IF_YC_8BIT))	{	ITCK1_SET(10);	IFRQ = 10;	}
			else																								{	ITCK1_SET(6);	IFRQ = 6; 	}

			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK1_SET(IFRQ);	OTCK1_SET(IFRQ);	DoCk_Set(PinList, IFRQ);}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{
				if((g_ConfigICh3==IF_BT1120_8BIT_SDR)||(g_ConfigICh3=IF_BT656_SDR)||(g_ConfigICh3==IF_YC_8BIT))		{	DITCK1_SET(IFRQ-4);	OTCK1_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
			else															{
				if((g_ConfigICh3==IF_BT1120_8BIT_SDR)||(g_ConfigICh3==IF_BT656_SDR)||(g_ConfigICh3==IF_YC_8BIT))	{	DITCK1_SET(IFRQ-4);	OTCK1_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
		break;

		case IFO_DIG_CH0_DIRT_FONT_PATH:
			OSD_ISEL0w(1);	OSD_IVSEL0w(1);		POST_OSEL0w(0);		ITU_SSEL0w(1);	ITU_ASEL0w(5);
			if((g_ConfigICh0==IF_BT1120_8BIT_SDR)||(g_ConfigICh0==IF_BT656_SDR)||(g_ConfigICh0==IF_YC_8BIT))	{	FN_CK0_SET(7);	PR_CK0_SET(7);	ITCK0_SET(7);	IFRQ = 7;	}
			else																								{	FN_CK0_SET(3);	PR_CK0_SET(3);	ITCK0_SET(3);	IFRQ = 3; 	}

			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK0_SET(IFRQ);	OTCK0_SET(IFRQ);	DoCk_Set(PinList, IFRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{
				if((g_ConfigICh0==IF_BT1120_8BIT_SDR)||(g_ConfigICh0=IF_BT656_SDR)||(g_ConfigICh0==IF_YC_8BIT))	{	DITCK0_SET(IFRQ-4);	OTCK0_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
			else															{
				if((g_ConfigICh0==IF_BT1120_8BIT_SDR)||(g_ConfigICh0==IF_BT656_SDR)||(g_ConfigICh0==IF_YC_8BIT))	{	DITCK0_SET(IFRQ-4);	OTCK0_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
		break;

		case IFO_DIG_CH1_DIRT_FONT_PATH:
			OSD_ISEL0w(2);	OSD_IVSEL0w(2);		POST_OSEL0w(0);		ITU_SSEL0w(2);	ITU_ASEL0w(6);
			if((g_ConfigICh1==IF_BT1120_8BIT_SDR)||(g_ConfigICh1==IF_BT656_SDR)||(g_ConfigICh1==IF_YC_8BIT))	{	FN_CK0_SET(8);	PR_CK0_SET(8);	ITCK0_SET(8);	IFRQ = 8;	}
			else																								{	FN_CK0_SET(4);	PR_CK0_SET(4);	ITCK0_SET(4);	IFRQ = 4; 	}

			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK0_SET(IFRQ);	OTCK0_SET(IFRQ);	DoCk_Set(PinList, IFRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{
				if((g_ConfigICh1==IF_BT1120_8BIT_SDR)||(g_ConfigICh1=IF_BT656_SDR)||(g_ConfigICh1==IF_YC_8BIT))	{	DITCK0_SET(IFRQ-4);	OTCK0_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
			else															{
				if((g_ConfigICh1==IF_BT1120_8BIT_SDR)||(g_ConfigICh1==IF_BT656_SDR)||(g_ConfigICh1==IF_YC_8BIT))	{	DITCK0_SET(IFRQ-4);	OTCK0_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
		break;

		case IFO_DIG_CH2_DIRT_FONT_PATH:
			OSD_ISEL0w(3);	OSD_IVSEL0w(3);		POST_OSEL0w(0);		ITU_SSEL0w(3);	ITU_ASEL0w(7);
			if((g_ConfigICh2==IF_BT1120_8BIT_SDR)||(g_ConfigICh2==IF_BT656_SDR)||(g_ConfigICh2==IF_YC_8BIT))	{	FN_CK0_SET(9);	PR_CK0_SET(9);	ITCK0_SET(9);	IFRQ = 9;	}
			else																								{	FN_CK0_SET(5);	PR_CK0_SET(5);	ITCK0_SET(5);	IFRQ = 5; 	}

			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK0_SET(IFRQ);	OTCK0_SET(IFRQ);	DoCk_Set(PinList, IFRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{
				if((g_ConfigICh2==IF_BT1120_8BIT_SDR)||(g_ConfigICh2=IF_BT656_SDR)||(g_ConfigICh2==IF_YC_8BIT))	{	DITCK0_SET(IFRQ-4);	OTCK0_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
			else															{
				if((g_ConfigICh2==IF_BT1120_8BIT_SDR)||(g_ConfigICh2==IF_BT656_SDR)||(g_ConfigICh2==IF_YC_8BIT))	{	DITCK0_SET(IFRQ-4);	OTCK0_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
		break;

		case IFO_DIG_CH3_DIRT_FONT_PATH:
			OSD_ISEL0w(4);	OSD_IVSEL0w(4);		POST_OSEL0w(0);		ITU_SSEL0w(4);	ITU_ASEL0w(8);
			if((g_ConfigICh2==IF_BT1120_8BIT_SDR)||(g_ConfigICh2==IF_BT656_SDR)||(g_ConfigICh2==IF_YC_8BIT))	{	FN_CK0_SET(10);	PR_CK0_SET(10);	ITCK0_SET(10);	IFRQ = 10;	}
			else																								{	FN_CK0_SET(6);	PR_CK0_SET(6);	ITCK0_SET(6);	IFRQ = 6; 	}

			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))		{	DITCK0_SET(IFRQ);	OTCK0_SET(IFRQ);	DoCk_Set(PinList, IFRQ);	}
			else if(Mode==IFO_BT1120_8BIT_SDR)								{
				if((g_ConfigICh3==IF_BT1120_8BIT_SDR)||(g_ConfigICh3=IF_BT656_SDR)||(g_ConfigICh3==IF_YC_8BIT))	{	DITCK0_SET(IFRQ-4);	OTCK0_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
			else															{
				if((g_ConfigICh3==IF_BT1120_8BIT_SDR)||(g_ConfigICh3==IF_BT656_SDR)||(g_ConfigICh3==IF_YC_8BIT))	{	DITCK0_SET(IFRQ-4);	OTCK0_SET(IFRQ-4);	DoCk_Set(PinList, IFRQ-4);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);}
				else																								{	return	IF_NO_MATCH_ERROR;	}
			}
		break;

		case IFO_DIG_PIP_FONT_PATH:
			OSD_ISEL0w(10);		POST_OSEL0w(0);

			if(Rsync==IFO_ISP_SYNC)
			{
				OSD_IVSEL0w(4);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(0);  ITU_SSEL0w(0);}
				else 																					{	ITU_ASEL1w(0);	ITU_SSEL1w(0);}
			}
			else if(Rsync==IFO_RD_SYNC1)
			{
				OSD_IVSEL0w(5);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(20);	ITU_SSEL0w(5);}
				else 																					{	ITU_ASEL1w(20); ITU_SSEL1w(5);}
			}
			else if(Rsync==IFO_RD_SYNC2)
			{
				OSD_IVSEL0w(6);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(20);	ITU_SSEL0w(6);}
				else 																					{	ITU_ASEL1w(20); ITU_SSEL1w(6);}
			}
			else
			{
				OSD_IVSEL0w(7);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(20);	ITU_SSEL0w(7);}
				else 																					{	ITU_ASEL1w(20); ITU_SSEL1w(7);}
			}


			if(Res<IFO_1080_50P)
			{
				FN_CK0_SET(2);	PR_CK0_SET(2);	IFRQ = 2;
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITCK0_SET(2);   }
				else 																					{	ITCK1_SET(2);	}
			}
			else if((Res>=IFO_1080_50P)&&(Res<IFO_4K_30P))
			{
				FN_CK0_SET(1);	PR_CK0_SET(1);	IFRQ = 1;
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITCK0_SET(1);   }
				else 																					{	ITCK1_SET(1);	}
			}
			else
			{
				FN_CK0_SET(0);	PR_CK0_SET(0);	IFRQ = 0;
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITCK0_SET(0);   }
				else 																					{	ITCK1_SET(0);	}
			}


			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))
			{
				DoCk_Set(PinList, IFRQ);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))	{	DITCK0_SET(IFRQ);	OTCK0_SET(IFRQ);	}
				else																				{	DITCK1_SET(IFRQ);	OTCK1_SET(IFRQ);	}
			}
			else if(Mode==IFO_BT1120_8BIT_SDR)
			{
				if(IFRQ==0) {	return	IF_NO_MATCH_ERROR;	}
				else {
					DoCk_Set(PinList, IFRQ-1);
					if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))	{	DITCK0_SET(IFRQ-1);	OTCK0_SET(IFRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
					else																				{	DITCK1_SET(IFRQ-1);	OTCK1_SET(IFRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
				}
			}
			else
			{
				if(IFRQ==0) {	return	IF_NO_MATCH_ERROR;	}
				else {
					DoCk_Set(PinList, IFRQ-1);
					if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))	{	DITCK0_SET(IFRQ-1);	OTCK0_SET(IFRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);	}
					else																				{	DITCK1_SET(IFRQ-1);	OTCK1_SET(IFRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);	}
				}
			}
		break;

		case IFO_DIG_MERGE_FONT_PATH:
			OSD_ISEL0w(15);		POST_OSEL0w(0);

			if(Rsync==IFO_ISP_SYNC)
			{
				return IF_NO_MATCH_ERROR;
			}
			else if(Rsync==IFO_RD_SYNC1)
			{
				OSD_IVSEL0w(5);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(23);	ITU_SSEL0w(5);}
				else 																					{	ITU_ASEL1w(23); ITU_SSEL1w(5);}
			}
			else if(Rsync==IFO_RD_SYNC2) {
				OSD_IVSEL0w(6);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(23);	ITU_SSEL0w(6);}
				else 																					{	ITU_ASEL1w(23); ITU_SSEL1w(6);}
			}
			else {
				OSD_IVSEL0w(7);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(23);	ITU_SSEL0w(7);}
				else 																					{	ITU_ASEL1w(23); ITU_SSEL1w(7);}
			}


			if(Res<IFO_1080_50P)
			{
				FN_CK0_SET(2);	PR_CK0_SET(2);	IFRQ = 2;
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITCK0_SET(2);   }
				else 																					{	ITCK1_SET(2);	}
			}
			else if((Res>=IFO_1080_50P)&&(Res<IFO_4K_30P))
			{
				FN_CK0_SET(1);	PR_CK0_SET(1);	IFRQ = 1;
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITCK0_SET(1);   }
				else 																					{	ITCK1_SET(1);	}
			}
			else
			{
				FN_CK0_SET(0);	PR_CK0_SET(0);	IFRQ = 0;
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITCK0_SET(0);   }
				else 																					{	ITCK1_SET(0);	}
			}


			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))
			{
				DoCk_Set(PinList, IFRQ);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))	{	DITCK0_SET(IFRQ);	OTCK0_SET(IFRQ);	}
				else																				{	DITCK1_SET(IFRQ);	OTCK1_SET(IFRQ);	}
			}
			else if(Mode==IFO_BT1120_8BIT_SDR)
			{
				if(IFRQ==0) {	return	IF_NO_MATCH_ERROR;	}
				else {
					DoCk_Set(PinList, IFRQ-1);
					if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))	{	DITCK0_SET(IFRQ-1);	OTCK0_SET(IFRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
					else																				{	DITCK1_SET(IFRQ-1);	OTCK1_SET(IFRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
				}
			}
			else
			{
				if(IFRQ==0) {	return	IF_NO_MATCH_ERROR;	}
				else {
					DoCk_Set(PinList, IFRQ-1);
					if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))	{	DITCK0_SET(IFRQ-1);	OTCK0_SET(IFRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);	}
					else																				{	DITCK1_SET(IFRQ-1);	OTCK1_SET(IFRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);	}
				}
			}
		break;

		case IFO_DIG_DZOOM_FONT_PATH:
			OSD_ISEL0w(11);		POST_OSEL0w(0);

			if(Rsync==IFO_ISP_SYNC)
			{
				return	IF_NO_MATCH_ERROR;
			}
			else if(Rsync==IFO_RD_SYNC1)
			{
				OSD_IVSEL0w(5);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(22);	ITU_SSEL0w(5);}
				else 																					{	ITU_ASEL1w(22); ITU_SSEL1w(5);}
			}
			else if(Rsync==IFO_RD_SYNC2)
			{
				OSD_IVSEL0w(6);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(22);	ITU_SSEL0w(6);}
				else 																					{	ITU_ASEL1w(22); ITU_SSEL1w(6);}
			}
			else
			{
				OSD_IVSEL0w(7);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITU_ASEL0w(22);	ITU_SSEL0w(7);}
				else 																					{	ITU_ASEL1w(22); ITU_SSEL1w(7);}
			}


			if(Res<IFO_1080_50P)
			{
				FN_CK0_SET(2);	PR_CK0_SET(2);	IFRQ = 2;
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITCK0_SET(2);   }
				else 																					{	ITCK1_SET(2);	}
			}
			else if((Res>=IFO_1080_50P)&&(Res<IFO_4K_30P))
			{
				FN_CK0_SET(1);	PR_CK0_SET(1);	IFRQ = 1;
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITCK0_SET(1);   }
				else 																					{	ITCK1_SET(1);	}
			}
			else
			{
				FN_CK0_SET(0);	PR_CK0_SET(0);	IFRQ = 0;
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))		{	ITCK0_SET(0);   }
				else 																					{	ITCK1_SET(0);	}
			}


			if((Mode==IFO_BT1120_16BIT)||(Mode==IFO_BT1120_8BIT_DDR))
			{
				DoCk_Set(PinList, IFRQ);
				if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))	{	DITCK0_SET(IFRQ);	OTCK0_SET(IFRQ);	}
				else																				{	DITCK1_SET(IFRQ);	OTCK1_SET(IFRQ);	}
			}
			else if(Mode==IFO_BT1120_8BIT_SDR)
			{
				if(IFRQ==0) {	return	IF_NO_MATCH_ERROR;	}
				else {
					DoCk_Set(PinList, IFRQ-1);
					if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))	{	DITCK0_SET(IFRQ-1);	OTCK0_SET(IFRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	}
					else																				{	DITCK1_SET(IFRQ-1);	OTCK1_SET(IFRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	}
				}
			}
			else
			{
				if(IFRQ==0) {	return	IF_NO_MATCH_ERROR;	}
				else {
					DoCk_Set(PinList, IFRQ-1);
					if((PinList==IFO_OPIN_0TO15)||(PinList==IFO_OPIN_0TO7)||(PinList==IFO_OPIN_24TO31))	{	DITCK0_SET(IFRQ-1);	OTCK0_SET(IFRQ-1);	D_ITU_FIFO_ON0w(1);	D_ITU_DOFF_RS0w(1);	D_ITU_DO_BT656_ON0w(1);	}
					else																				{	DITCK1_SET(IFRQ-1);	OTCK1_SET(IFRQ-1);	D_ITU_FIFO_ON1w(1);	D_ITU_DOFF_RS1w(1);	D_ITU_DO_BT656_ON1w(1);	}
				}
			}
		break;

	}

	//	Select Output Mode
	switch(PinList)
	{
		case IFO_OPIN_0TO15	    :
			if((Use0To7==USE_EMPTY)&&(Use8To15==USE_EMPTY))		{
				if(Path<IFO_DIG_RDCH1_FONT_PATH)	{	Use0To7 = USE_OCH1_16BIT;	Use8To15 = USE_OCH1_16BIT;	D_BT20_ISEL0w(1);	}
				else								{	Use0To7 = USE_OCH0_16BIT;	Use8To15 = USE_OCH0_16BIT;	D_BT20_ISEL0w(0);	}
				DOMODEw(0);
			}
			else return IF_NO_MATCH_ERROR;
		break;

		case IFO_OPIN_16TO31	:
			if((Use16To23==USE_EMPTY)&&(Use24To31==USE_EMPTY))	{
				if(Path<IFO_DIG_RDCH1_FONT_PATH)	{	Use16To23 = USE_OCH1_16BIT;	Use24To31 = USE_OCH1_16BIT;	D_BT20_ISEL1w(1);	}
				else								{	Use16To23 = USE_OCH0_16BIT;	Use24To31 = USE_OCH0_16BIT;	D_BT20_ISEL1w(0);	}
				DOMODEw(0);
				DO_MUX_ONw(1);
			}
			else return IF_NO_MATCH_ERROR;
		break;

		case IFO_OPIN_0TO7		:
			if((Use0To7==USE_EMPTY))							{
				if(Path<IFO_DIG_RDCH1_FONT_PATH)	{	Use0To7 = USE_OCH1_8BIT;	D_BT20_ISEL0w(1);	D_ITU_OMOD0w(1);	D_ITU_CTOG_INV0w(1);	}
				else								{	Use0To7 = USE_OCH0_8BIT;	D_BT20_ISEL0w(0);	D_ITU_OMOD0w(1);	D_ITU_CTOG_INV0w(1);	}
				DOMODEw(2);
			}
			else return IF_NO_MATCH_ERROR;
		break;

		case IFO_OPIN_8TO15	    :
			if((Use8To15==USE_EMPTY))							{
				if(Path<IFO_DIG_RDCH1_FONT_PATH)	{	Use8To15 = USE_OCH1_8BIT;	D_BT20_ISEL1w(1);	D_ITU_OMOD1w(1);	D_ITU_CTOG_INV1w(1);	}
				else								{	Use8To15 = USE_OCH0_8BIT;	D_BT20_ISEL1w(0);	D_ITU_OMOD1w(1);	D_ITU_CTOG_INV1w(1);	}
				DOMODEw(2);
			}
			else return IF_NO_MATCH_ERROR;
		break;

		case IFO_OPIN_16TO23	:
			if((Use16To23==USE_EMPTY))							{
				if(Path<IFO_DIG_RDCH1_FONT_PATH)	{	Use16To23 = USE_OCH1_8BIT;	D_BT20_ISEL0w(1);	D_ITU_OMOD0w(1);	D_ITU_CTOG_INV0w(1);	}
				else								{	Use16To23 = USE_OCH0_8BIT;	D_BT20_ISEL0w(0);	D_ITU_OMOD0w(1);	D_ITU_CTOG_INV0w(1);	}
				DOMODEw(5);
			}
			else return IF_NO_MATCH_ERROR;
		break;

		case IFO_OPIN_24TO31	:
			if((Use24To31==USE_EMPTY))							{
				if(Path<IFO_DIG_RDCH1_FONT_PATH)	{	Use24To31 = USE_OCH1_8BIT;	D_BT20_ISEL1w(1);	D_ITU_OMOD1w(1);	D_ITU_CTOG_INV1w(1);	}
				else								{	Use24To31 = USE_OCH0_8BIT;	D_BT20_ISEL1w(0);	D_ITU_OMOD1w(1);	D_ITU_CTOG_INV1w(1);	}
				DOMODEw(5);
				DO_MUX_ONw(1);
			}
			else return IF_NO_MATCH_ERROR;
		break;

		default :

		break;
	}

	return IF_SET_OK;
}

//	Dcko0Clk, Dcko1Clk, Dcko2Clk	->	Select Digital Clock (EN675 Pin : DCKO0, DCKO1, DCKO2)
//										CLK_297M			//	ISP PLL	Clock (297MHz)
//										CLK_148M            //	ISP PLL	Clock (148.5MHz)
//										CLK_74M             //	ISP PLL	Clock (74.25MHz)
//										CLK_DIG_CH0         //	Digital Input Channel Pixel Clock 0	EN675 Pin : SS_CKI)
//										CLK_DIG_CH1         //	Digital Input Channel Pixel Clock 1 EN675 Pin : GPIO4)
//										CLK_DIG_CH2         //	Digital Input Channel Pixel Clock 2 EN675 Pin : GPIO5)
//										CLK_DIG_CH3         //	Digital Input Channel Pixel Clock 3 EN675 Pin : GPIO6)
//										CLK_DIG_CH0_DIV2    //	Digital Input Channel Pixel Clock 0	1/2 Divide Clock
//										CLK_DIG_CH1_DIV2    //	Digital Input Channel Pixel Clock 1 1/2 Divide Clock
//										CLK_DIG_CH2_DIV2    //	Digital Input Channel Pixel Clock 2 1/2 Divide Clock
//										CLK_DIG_CH3_DIV2    //	Digital Input Channel Pixel Clock 3 1/2 Divide Clock
//										CLK_PLL_DIV0        //	ISP PLL Adjust 0 Clock (Special)
//										CLK_PLL_DIV1        //	ISP PLL Adjust 1 Clock (Special)
//										CLK_PLL_DIV2        //	ISP PLL Adjust 2 Clock (Special)
//										CLK_DDR_RATE0       //	Digital Output Channel 0 DDR Rate Clock
//										CLK_DDR_RATE1       //	Digital Output Channel 1 DDR Rate Clock
void Isp_Dout_Clock0_Config(BOOL Dcko, BYTE DckoClk, BYTE DckoDly, BOOL ClkoInv)
{
	if(Dcko)	{	DCKO0_PDw(0);	DCKO0_DLYw(DckoDly);	DCKO0_SELw(DckoClk);	DCKO0_INVw(ClkoInv);	DCKO0_PDw(1);	DCKO0_OENw(0);	}
	else		{	DCKO0_PDw(0);	DCKO0_OENw(1);	}
}

void Isp_Dout_Clock1_Config(BOOL Dcko, BYTE DckoClk, BYTE DckoDly, BOOL ClkoInv)
{
	if(Dcko)	{	DCKO1_PDw(0);	DCKO1_DLYw(DckoDly);	DCKO1_SELw(DckoClk);	DCKO1_INVw(ClkoInv);	DCKO1_PDw(1);	DCKO1_OENw(0);	}
	else		{	DCKO1_PDw(0);	DCKO1_OENw(1);	}
}

void Isp_Dout_Clock2_Config(BOOL Dcko, BYTE DckoClk, BYTE DckoDly, BOOL ClkoInv)
{
	if(Dcko)	{	DCKO2_PDw(0);	DCKO2_DLYw(DckoDly);	DCKO2_SELw(DckoClk);	DCKO2_INVw(ClkoInv);	DCKO2_PDw(1);	DCKO2_OENw(0);	}
	else		{	DCKO2_PDw(0);	DCKO2_OENw(1);	}
}


void Isp_Dout0_Sync_Config(UINT Htw, UINT HSyncOfs, UINT VSyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw)
{
	ITU_HTWO0w(Htw-2);
	VLOCKO_IT_POS0w(VSyncOfs);
	HLOCKO_IT_POS0w(HSyncOfs);
	HSPC0w(Hsp);	VSPC0w(Vsp);
	HWC0w(Hw);		VWC0w(Vw);
}

void Isp_Dout1_Sync_Config(UINT Htw, UINT HSyncOfs, UINT VSyncOfs, UINT Hsp, UINT Vsp, UINT Hw, UINT Vw)
{
	ITU_HTWO1w(Htw-2);
	VLOCKO_IT_POS1w(VSyncOfs);
	HLOCKO_IT_POS1w(HSyncOfs);
	HSPC1w(Hsp);	VSPC1w(Vsp);
	HWC1w(Hw);		VWC1w(Vw);
}


//	DDR Digital Write Path Function---------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Path	->		WR_CH_ISP_NO_FONT				//	ISP Result Image No Font Path
//				WR_CH_DIGITAL_NO_FONT_CH0		//	Digital Input Channel 0
//				WR_CH_DIGITAL_NO_FONT_CH1		//	Digital Input Channel 1
//				WR_CH_DIGITAL_NO_FONT_CH2		//	Digital Input Channel 2
//				WR_CH_DIGITAL_NO_FONT_CH3        //	Digital Input Channel 3
//				WR_CH_JPEG_NO_FONT_DECODER      //	JPEG Decoder No Font Path
//				WR_CH_ISP_FONT                  //	ISP Result Image + Font
//				WR_CH_DIGITAL_FONT_CH0          //	Digital Input Channel 0 + Font Path
//				WR_CH_DIGITAL_FONT_CH1          //	Digital Input Channel 1 + Font Path
//				WR_CH_DIGITAL_FONT_CH2          //	Digital Input Channel 2 + Font Path
//				WR_CH_DIGITAL_FONT_CH3          //	Digital Input Channel 3 + Font Path
//				WR_CH_DOWN_SCALER0              //	Down-Scaler 0 Result Path
//				WR_CH_DOWN_SCALER1              //	Down-Scaler 1 Result Path
//				WR_CH_DOWN_SCALER2              //	Down-Scaler 2 Result Path
//				WR_CH_DOWN_SCALER3              //	Down-Scaler 3 Result Path
//				WR_CH_PIP                       //	PIP Image
//				WR_CH_DZOOM                     //	Dzoom Read Channel Path
//				WR_CH_JPEG_FONT_DECODER         //	Jpeg Decoder Path + Font
//				WR_CH_THERMAL_Y                 //	Thermal Image Y Path
//				WR_CH_THERMAL_PSEUDO            //	Thermal Pseudo Image Path
//
//BYTE VlcMode->	NO_VLCBIT					//	Don't use VLC Funtion
//				VLC_4BIT						//	4 Bit compression
//				VLC_5BIT						//	5 Bit compression
//				VLC_6BIT						//	6 Bit compression

//	Clk	->		CLK_297M						//	ISP PLL	Clock (297MHz)
//				CLK_148M               	    	//	ISP PLL	Clock (148.5MHz)
//				CLK_74M                	    	//	ISP PLL	Clock (74.25MHz)
//				CLK_DIG_CH0            	    	//	Digital Input Channel Pixel Clock 0	EN675 Pin : SS_CKI)
//				CLK_DIG_CH1            	    	//	Digital Input Channel Pixel Clock 1 EN675 Pin : GPIO4)
//				CLK_DIG_CH2            	    	//	Digital Input Channel Pixel Clock 2 EN675 Pin : GPIO5)
//				CLK_DIG_CH3            	    	//	Digital Input Channel Pixel Clock 3 EN675 Pin : GPIO6)
//				CLK_DIG_CH0_DIV2       	    	//	Digital Input Channel Pixel Clock 0	1/2 Divide Clock
//				CLK_DIG_CH1_DIV2       	    	//	Digital Input Channel Pixel Clock 1 1/2 Divide Clock
//				CLK_DIG_CH2_DIV2       	    	//	Digital Input Channel Pixel Clock 2 1/2 Divide Clock
//				CLK_DIG_CH3_DIV2       	    	//	Digital Input Channel Pixel Clock 3 1/2 Divide Clock
//				CLK_PLL_DIV0           	    	//	ISP PLL Adjust Clock (Special)
//
//	WrSync	->	WR_ISP_SYNC						//	ISP Sync
//              WR_DIG_CH0_SYNC					//	Digital Input Channel 0 Sync
//              WR_DIG_CH1_SYNC					//	Digital Input Channel 1 Sync
//              WR_DIG_CH2_SYNC					//	Digital Input Channel 2 Sync
//              WR_DIG_CH3_SYNC					//	Digital Input Channel 3 Sync
//              WR_DDR_BYPASS_WDR_SYNC			//	DDR Bypass Sync For WDR
//              WR_SYNC2						//	Internal Sync Generator 2 Sync
//              WR_DIG_THERMAL					//	Digital Thermal Input Sync

//	Normally Default Value -> Color : 1, Intl : 0
void Isp_WrCh0_Config(UINT Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE Clk, BOOL OnOff)
{
	if(OnOff==0) {	YCW_CK0_PDw(0);	return;	}

	IM_HWI0w(Hw);
	IM_CWEN0w(Color);
	IM_INTL0_ONw(Intl);

	switch(Path)
	{
		case WR_CH_ISP_NO_FONT           :		IM_ISEL0w(12);	IM_NPFONT0w(1);			break;
		case WR_CH_DIGITAL_NO_FONT_CH0   :		IM_ISEL0w(4);							break;
		case WR_CH_DIGITAL_NO_FONT_CH1   :		IM_ISEL0w(5);							break;
		case WR_CH_DIGITAL_NO_FONT_CH2   :		IM_ISEL0w(6);							break;
		case WR_CH_DIGITAL_NO_FONT_CH3   :		IM_ISEL0w(7);							break;
		case WR_CH_JPEG_NO_FONT_DECODER  :		IM_ISEL0w(16);							break;
		case WR_CH_ISP_FONT              :		IM_ISEL0w(12);	IM_NPFONT0w(0);			break;
		case WR_CH_DIGITAL_FONT_CH0      :		IM_ISEL0w(8);	IM_NPFONT0w(0);			break;
		case WR_CH_DIGITAL_FONT_CH1      :		IM_ISEL0w(9);	IM_NPFONT0w(0);			break;
		case WR_CH_DIGITAL_FONT_CH2      :		IM_ISEL0w(10);	IM_NPFONT0w(0);			break;
		case WR_CH_DIGITAL_FONT_CH3      :		IM_ISEL0w(11);	IM_NPFONT0w(0);			break;
		case WR_CH_DOWN_SCALER0          :		IM_ISEL0w(0);							break;
		case WR_CH_DOWN_SCALER1          :		IM_ISEL0w(1);							break;
		case WR_CH_DOWN_SCALER2          :		IM_ISEL0w(2);							break;
		case WR_CH_DOWN_SCALER3          :		IM_ISEL0w(3);							break;
		case WR_CH_PIP                   :		IM_ISEL0w(13);							break;
		case WR_CH_DZOOM                 :		IM_ISEL0w(14);							break;
		case WR_CH_JPEG_FONT_DECODER     :		IM_ISEL0w(15);	IM_NPFONT0w(0);			break;
		case WR_CH_THERMAL_Y             :		IM_ISEL0w(17);							break;
		case WR_CH_THERMAL_PSEUDO        :		IM_ISEL0w(18);							break;
	}

	IM_IVSEL0w(0);
	YCW_CK0_SET(Clk);
}

//	Normally Default Value -> Color : 1, Intl : 0, FrcPage : 0, Rdock : 0, RdCh : 0
void Isp_WrCh1_Config(BYTE Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE VlcMode, BYTE Clk, BOOL IsFrc, BYTE FrcPage, BYTE Rdock, BYTE RdCh, BOOL OnOff)
{
	if(OnOff==0) {	YCW_CK1_PDw(0);	return;	}

	IM_HWI1w(Hw);																	//	Horizontal Width
	IM_CWEN1w(Color);																//	Enable Color Write
	IM_2PAGE1w(FrcPage);															//	When IsFrc==1, FRC Page -> 0: 3 Page, 1: 2 Page
	IM_INTL1_ONw(Intl);																//	Interlace Input?
	IM_WFRC1_ONw(IsFrc);															//	Use FRC?
	IM_FRC1_WLOCKONw(Rdock);														//	Change Write Mode, 0 : Write First, 1 -> Read Firest
	IM_WCH1_SELw(RdCh);																//	When Read First, Select Base Read Channel

	switch(Path)
	{
		case WR_CH_ISP_NO_FONT           :		IM_ISEL1w(12);	IM_NPFONT1w(1);			break;
		case WR_CH_DIGITAL_NO_FONT_CH0   :		IM_ISEL1w(4);							break;
		case WR_CH_DIGITAL_NO_FONT_CH1   :		IM_ISEL1w(5);							break;
		case WR_CH_DIGITAL_NO_FONT_CH2   :		IM_ISEL1w(6);							break;
		case WR_CH_DIGITAL_NO_FONT_CH3   :		IM_ISEL1w(7);							break;
		case WR_CH_JPEG_NO_FONT_DECODER  :		IM_ISEL1w(16);							break;
		case WR_CH_ISP_FONT              :		IM_ISEL1w(12);	IM_NPFONT1w(0);			break;
		case WR_CH_DIGITAL_FONT_CH0      :		IM_ISEL1w(8);	IM_NPFONT1w(0);			break;
		case WR_CH_DIGITAL_FONT_CH1      :		IM_ISEL1w(9);	IM_NPFONT1w(0);			break;
		case WR_CH_DIGITAL_FONT_CH2      :		IM_ISEL1w(10);	IM_NPFONT1w(0);			break;
		case WR_CH_DIGITAL_FONT_CH3      :		IM_ISEL1w(11);	IM_NPFONT1w(0);			break;
		case WR_CH_DOWN_SCALER0          :		IM_ISEL1w(0);							break;
		case WR_CH_DOWN_SCALER1          :		IM_ISEL1w(1);							break;
		case WR_CH_DOWN_SCALER2          :		IM_ISEL1w(2);							break;
		case WR_CH_DOWN_SCALER3          :		IM_ISEL1w(3);							break;
		case WR_CH_PIP                   :		IM_ISEL1w(13);							break;
		case WR_CH_DZOOM                 :		IM_ISEL1w(14);							break;
		case WR_CH_JPEG_FONT_DECODER     :		IM_ISEL1w(15);	IM_NPFONT1w(0);			break;
		case WR_CH_THERMAL_Y             :		IM_ISEL1w(17);							break;
		case WR_CH_THERMAL_PSEUDO        :		IM_ISEL1w(18);							break;
	}

	IM_IVSEL1w(WrSync);

	if(VlcMode==NO_VLCBIT)	{	LC_WON1w(0);	}
	else{
		if(VlcMode==VLC_4BIT)		{	LC_MODEY1w(2);	LC_MODEC1w(2);	}
		else if(VlcMode==VLC_5BIT)	{	LC_MODEY1w(1);	LC_MODEC1w(1);	}
		else						{	LC_MODEY1w(0);	LC_MODEC1w(0);	}

		LC_WON1w(1);
	}

	YCW_CK1_SET(Clk);
}

//	Normally Default Value -> Color : 1, Intl : 0, FrcPage : 0, Rdock : 0, RdCh : 1
void Isp_WrCh2_Config(BYTE Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE VlcMode, BYTE Clk, BOOL IsFrc, BYTE FrcPage, BYTE Rdock, BYTE RdCh, BOOL OnOff)
{
	if(OnOff==0) {	YCW_CK2_PDw(0);	return;	}

	IM_HWI2w(Hw);																	//	Horizontal Width
	IM_CWEN2w(Color);																//	Enable Color Write
	IM_2PAGE2w(FrcPage);															//	When IsFrc==1, FRC Page -> 0: 3 Page, 1: 2 Page
	IM_INTL2_ONw(Intl);																//	Interlace Input?
	IM_WFRC2_ONw(IsFrc);															//	Use FRC?
	IM_FRC2_WLOCKONw(Rdock);														//	Change Write Mode, 0 : Write First, 1 -> Read Firest
	IM_WCH2_SELw(RdCh);																//	When Read First, Select Base Read Channel

	switch(Path)
	{
		case WR_CH_ISP_NO_FONT           :		IM_ISEL2w(12);	IM_NPFONT2w(1);			break;
		case WR_CH_DIGITAL_NO_FONT_CH0   :		IM_ISEL2w(4);							break;
		case WR_CH_DIGITAL_NO_FONT_CH1   :		IM_ISEL2w(5);							break;
		case WR_CH_DIGITAL_NO_FONT_CH2   :		IM_ISEL2w(6);							break;
		case WR_CH_DIGITAL_NO_FONT_CH3   :		IM_ISEL2w(7);							break;
		case WR_CH_JPEG_NO_FONT_DECODER  :		IM_ISEL2w(16);							break;
		case WR_CH_ISP_FONT              :		IM_ISEL2w(12);	IM_NPFONT2w(0);			break;
		case WR_CH_DIGITAL_FONT_CH0      :		IM_ISEL2w(8);	IM_NPFONT2w(0);			break;
		case WR_CH_DIGITAL_FONT_CH1      :		IM_ISEL2w(9);	IM_NPFONT2w(0);			break;
		case WR_CH_DIGITAL_FONT_CH2      :		IM_ISEL2w(10);	IM_NPFONT2w(0);			break;
		case WR_CH_DIGITAL_FONT_CH3      :		IM_ISEL2w(11);	IM_NPFONT2w(0);			break;
		case WR_CH_DOWN_SCALER0          :		IM_ISEL2w(0);							break;
		case WR_CH_DOWN_SCALER1          :		IM_ISEL2w(1);							break;
		case WR_CH_DOWN_SCALER2          :		IM_ISEL2w(2);							break;
		case WR_CH_DOWN_SCALER3          :		IM_ISEL2w(3);							break;
		case WR_CH_PIP                   :		IM_ISEL2w(13);							break;
		case WR_CH_DZOOM                 :		IM_ISEL2w(14);							break;
		case WR_CH_JPEG_FONT_DECODER     :		IM_ISEL2w(15);	IM_NPFONT2w(0);			break;
		case WR_CH_THERMAL_Y             :		IM_ISEL2w(17);							break;
		case WR_CH_THERMAL_PSEUDO        :		IM_ISEL2w(18);							break;
	}

	IM_IVSEL2w(WrSync);

	if(VlcMode==NO_VLCBIT)	{	LC_WON2w(0);	}
	else{
		if(VlcMode==VLC_4BIT)		{	LC_MODEY2w(2);	LC_MODEC2w(2);	}
		else if(VlcMode==VLC_5BIT)	{	LC_MODEY2w(1);	LC_MODEC2w(1);	}
		else						{	LC_MODEY2w(0);	LC_MODEC2w(0);	}

		LC_WON2w(1);
	}

	YCW_CK2_SET(Clk);
}

//	Normally Default Value -> Color : 1, Intl : 0, FrcPage : 0, Rdock : 0, RdCh : 2
void Isp_WrCh3_Config(BYTE Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE VlcMode, BYTE Clk, BOOL IsFrc, BYTE FrcPage, BYTE Rdock, BYTE RdCh, BOOL OnOff)
{
	if(OnOff==0) {	YCW_CK3_PDw(0);	return;	}

	IM_HWI3w(Hw);																	//	Horizontal Width
	IM_CWEN3w(Color);																//	Enable Color Write
	IM_2PAGE3w(FrcPage);															//	When IsFrc==1, FRC Page -> 0: 3 Page, 1: 2 Page
	IM_INTL3_ONw(Intl);																//	Interlace Input?
	IM_WFRC3_ONw(IsFrc);															//	Use FRC?
	IM_FRC3_WLOCKONw(Rdock);														//	Change Write Mode, 0 : Write First, 1 -> Read Firest
	IM_WCH3_SELw(RdCh);																//	When Read First, Select Base Read Channel

	switch(Path)
	{
		case WR_CH_ISP_NO_FONT           :		IM_ISEL3w(12);	IM_NPFONT3w(1);			break;
		case WR_CH_DIGITAL_NO_FONT_CH0   :		IM_ISEL3w(4);							break;
		case WR_CH_DIGITAL_NO_FONT_CH1   :		IM_ISEL3w(5);							break;
		case WR_CH_DIGITAL_NO_FONT_CH2   :		IM_ISEL3w(6);							break;
		case WR_CH_DIGITAL_NO_FONT_CH3   :		IM_ISEL3w(7);							break;
		case WR_CH_JPEG_NO_FONT_DECODER  :		IM_ISEL3w(16);							break;
		case WR_CH_ISP_FONT              :		IM_ISEL3w(12);	IM_NPFONT3w(0);			break;
		case WR_CH_DIGITAL_FONT_CH0      :		IM_ISEL3w(8);	IM_NPFONT3w(0);			break;
		case WR_CH_DIGITAL_FONT_CH1      :		IM_ISEL3w(9);	IM_NPFONT3w(0);			break;
		case WR_CH_DIGITAL_FONT_CH2      :		IM_ISEL3w(10);	IM_NPFONT3w(0);			break;
		case WR_CH_DIGITAL_FONT_CH3      :		IM_ISEL3w(11);	IM_NPFONT3w(0);			break;
		case WR_CH_DOWN_SCALER0          :		IM_ISEL3w(0);							break;
		case WR_CH_DOWN_SCALER1          :		IM_ISEL3w(1);							break;
		case WR_CH_DOWN_SCALER2          :		IM_ISEL3w(2);							break;
		case WR_CH_DOWN_SCALER3          :		IM_ISEL3w(3);							break;
		case WR_CH_PIP                   :		IM_ISEL3w(13);							break;
		case WR_CH_DZOOM                 :		IM_ISEL3w(14);							break;
		case WR_CH_JPEG_FONT_DECODER     :		IM_ISEL3w(15);	IM_NPFONT3w(0);			break;
		case WR_CH_THERMAL_Y             :		IM_ISEL3w(17);							break;
		case WR_CH_THERMAL_PSEUDO        :		IM_ISEL3w(18);							break;
	}

	IM_IVSEL3w(WrSync);

	if(VlcMode==NO_VLCBIT)	{	LC_WON3w(0);	}
	else{
		if(VlcMode==VLC_4BIT)		{	LC_MODEY3w(2);	LC_MODEC3w(2);	}
		else if(VlcMode==VLC_5BIT)	{	LC_MODEY3w(1);	LC_MODEC3w(1);	}
		else						{	LC_MODEY3w(0);	LC_MODEC3w(0);	}

		LC_WON3w(1);
	}

	YCW_CK3_SET(Clk);
}

//	Normally Default Value -> Color : 1, Intl : 0, FrcPage : 0, Rdock : 0, RdCh : 3
void Isp_WrCh4_Config(BYTE Path, SHORT Hw, BYTE WrSync, BOOL Color, BOOL Intl, BYTE VlcMode, BYTE Clk, BOOL IsFrc, BYTE FrcPage, BYTE Rdock, BYTE RdCh, BOOL OnOff)
{
	if(OnOff==0) {	YCW_CK4_PDw(0);	return;	}

	IM_HWI4w(Hw);																	//	Horizontal Width
	IM_CWEN4w(Color);																//	Enable Color Write
	IM_2PAGE4w(FrcPage);															//	When IsFrc==1, FRC Page -> 0: 3 Page, 1: 2 Page
	IM_INTL4_ONw(Intl);																//	Interlace Input?
	IM_WFRC4_ONw(IsFrc);															//	Use FRC?
	IM_FRC4_WLOCKONw(Rdock);														//	Change Write Mode, 0 : Write First, 1 -> Read Firest
	IM_WCH4_SELw(RdCh);																//	When Read First, Select Base Read Channel

	switch(Path)
	{
		case WR_CH_ISP_NO_FONT           :		IM_ISEL4w(12);	IM_NPFONT4w(1);			break;
		case WR_CH_DIGITAL_NO_FONT_CH0   :		IM_ISEL4w(4);							break;
		case WR_CH_DIGITAL_NO_FONT_CH1   :		IM_ISEL4w(5);							break;
		case WR_CH_DIGITAL_NO_FONT_CH2   :		IM_ISEL4w(6);							break;
		case WR_CH_DIGITAL_NO_FONT_CH3   :		IM_ISEL4w(7);							break;
		case WR_CH_JPEG_NO_FONT_DECODER  :		IM_ISEL4w(16);							break;
		case WR_CH_ISP_FONT              :		IM_ISEL4w(12);	IM_NPFONT4w(0);			break;
		case WR_CH_DIGITAL_FONT_CH0      :		IM_ISEL4w(8);	IM_NPFONT4w(0);			break;
		case WR_CH_DIGITAL_FONT_CH1      :		IM_ISEL4w(9);	IM_NPFONT4w(0);			break;
		case WR_CH_DIGITAL_FONT_CH2      :		IM_ISEL4w(10);	IM_NPFONT4w(0);			break;
		case WR_CH_DIGITAL_FONT_CH3      :		IM_ISEL4w(11);	IM_NPFONT4w(0);			break;
		case WR_CH_DOWN_SCALER0          :		IM_ISEL4w(0);							break;
		case WR_CH_DOWN_SCALER1          :		IM_ISEL4w(1);							break;
		case WR_CH_DOWN_SCALER2          :		IM_ISEL4w(2);							break;
		case WR_CH_DOWN_SCALER3          :		IM_ISEL4w(3);							break;
		case WR_CH_PIP                   :		IM_ISEL4w(13);							break;
		case WR_CH_DZOOM                 :		IM_ISEL4w(14);							break;
		case WR_CH_JPEG_FONT_DECODER     :		IM_ISEL4w(15);	IM_NPFONT4w(0);			break;
		case WR_CH_THERMAL_Y             :		IM_ISEL4w(17);							break;
		case WR_CH_THERMAL_PSEUDO        :		IM_ISEL4w(18);							break;
	}

	IM_IVSEL4w(WrSync);

	if(VlcMode==NO_VLCBIT)	{	LC_WON4w(0);	}
	else{
		if(VlcMode==VLC_4BIT)		{	LC_MODEY4w(2);	LC_MODEC4w(2);	}
		else if(VlcMode==VLC_5BIT)	{	LC_MODEY4w(1);	LC_MODEC4w(1);	}
		else						{	LC_MODEY4w(0);	LC_MODEC4w(0);	}

		LC_WON4w(1);
	}

	YCW_CK4_SET(Clk);
}

//	Mode	->	0: Write 1 Image, 1 : continuous Write Image
//	WrEn	->	Mode==0 -> 0 : No Operation, 1: Write 1 Image
//				Mode==1 -> 0 : No Operation, 1: Write Image continuously
void Isp_WrCh0_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn)
{
	if(WrEn)
	{
		if(Mode==0)		IM_GO0w(WrEn);						//	Write 1 Image Mode
		else			IM_CGO0w(WrEn);						//	Write Continuous Image Mode
	}
	else	{	IM_GO0w(0);		IM_CGO0w(0);	}

	YCW_CK0_PDw(ClkEn);
}

void Isp_WrCh1_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn)
{
	if(WrEn)
	{
		if(Mode==0)		IM_GO1w(WrEn);						//	Write 1 Image Mode
		else			IM_CGO1w(WrEn);						//	Write Continuous Image Mode
	}
	else	{	IM_GO1w(0);		IM_CGO1w(0);	}

	YCW_CK1_PDw(ClkEn);
}

void Isp_WrCh2_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn)
{
	if(WrEn)
	{
		if(Mode==0)		IM_GO2w(WrEn);						//	Write 1 Image Mode
		else			IM_CGO2w(WrEn);						//	Write Continuous Image Mode
	}
	else	{	IM_GO2w(0);		IM_CGO2w(0);	}

	YCW_CK2_PDw(ClkEn);
}

void Isp_WrCh3_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn)
{
	if(WrEn)
	{
		if(Mode==0)		IM_GO3w(WrEn);						//	Write 1 Image Mode
		else			IM_CGO3w(WrEn);						//	Write Continuous Image Mode
	}
	else	{	IM_GO3w(0);		IM_CGO3w(0);	}

	YCW_CK3_PDw(ClkEn);
}

void Isp_WrCh4_Wr(BYTE Mode, BOOL WrEn, BOOL ClkEn)
{
	if(WrEn)
	{
		if(Mode==0)		IM_GO4w(WrEn);						//	Write 1 Image Mode
		else			IM_CGO4w(WrEn);						//	Write Continuous Image Mode
	}
	else	{	IM_GO4w(0);		IM_CGO4w(0);	}

	YCW_CK4_PDw(ClkEn);
}

//	Write Channel0 Manual Write Address Setting -> Channel 0 is Only Manual Address
void Isp_WrCh0_Adr(UINT YAdr, UINT CAdr)
{
	IM_YADR0w(YAdr);
	IM_CADR0w(CAdr);
}

//	Write Channel1 Manual Write Address Setting
void Isp_WrCh1_MAdr(UINT YAdr, UINT CAdr)
{
	IM_YADR1_P0w(YAdr);
	IM_CADR1_P0w(CAdr);
}

//	Write Channel1 Frc  Address Setting
void Isp_WrCh1_FrcAdr(UINT YAdr0, UINT CAdr0, UINT YAdr1, UINT CAdr1, UINT YAdr2, UINT CAdr2)
{
	IM_YADR1_P0w(YAdr0);
	IM_CADR1_P0w(CAdr0);
	IM_YADR1_P1w(YAdr1);
	IM_CADR1_P1w(CAdr1);
	IM_YADR1_P2w(YAdr2);
	IM_CADR1_P2w(CAdr2);
}

//	Write Channel2 Manual Write Address Setting
void Isp_WrCh2_MAdr(UINT YAdr, UINT CAdr)
{
	IM_YADR2_P0w(YAdr);
	IM_CADR2_P0w(CAdr);
}

//	Write Channel2 Frc  Address Setting
void Isp_WrCh2_FrcAdr(UINT YAdr0, UINT CAdr0, UINT YAdr1, UINT CAdr1, UINT YAdr2, UINT CAdr2)
{
	IM_YADR2_P0w(YAdr0);
	IM_CADR2_P0w(CAdr0);
	IM_YADR2_P1w(YAdr1);
	IM_CADR2_P1w(CAdr1);
	IM_YADR2_P2w(YAdr2);
	IM_CADR2_P2w(CAdr2);
}

//	Write Channel3 Manual Write Address Setting
void Isp_WrCh3_MAdr(UINT YAdr, UINT CAdr)
{
	IM_YADR3_P0w(YAdr);
	IM_CADR3_P0w(CAdr);
}

//	Write Channel3 Frc  Address Setting
void Isp_WrCh3_FrcAdr(UINT YAdr0, UINT CAdr0, UINT YAdr1, UINT CAdr1, UINT YAdr2, UINT CAdr2)
{
	IM_YADR3_P0w(YAdr0);
	IM_CADR3_P0w(CAdr0);
	IM_YADR3_P1w(YAdr1);
	IM_CADR3_P1w(CAdr1);
	IM_YADR3_P2w(YAdr2);
	IM_CADR3_P2w(CAdr2);
}

//	Write Channel4 Manual Write Address Setting
void Isp_WrCh4_MAdr(UINT YAdr, UINT CAdr)
{
	IM_YADR4_P0w(YAdr);
	IM_CADR4_P0w(CAdr);
}

//	Write Channel3 Frc  Address Setting
void Isp_WrCh4_FrcAdr(UINT YAdr0, UINT CAdr0, UINT YAdr1, UINT CAdr1, UINT YAdr2, UINT CAdr2)
{
	IM_YADR4_P0w(YAdr0);
	IM_CADR4_P0w(CAdr0);
	IM_YADR4_P1w(YAdr1);
	IM_CADR4_P1w(CAdr1);
	IM_YADR4_P2w(YAdr2);
	IM_CADR4_P2w(CAdr2);
}

//	Internal Generation Sync Setting---------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//	SyncMode	->	SYNC_STANDALONE				//	Independent Sync
//					SYNC_RELATION				//	Dependent Sync
//
//	Clk		->		CLK_297M					//	ISP PLL 297M
//                  CLK_148M               		//	ISP PLL 148.5M
//                  CLK_74M                		//	ISP PLL 74.25M
//                  CLK_DIG_CH0            		//	Digital Input Channel 0 Bypass Clock	PCLK
//                  CLK_DIG_CH1            		//	Digital Input Channel 1 Bypass Clock	PCLK0
//                  CLK_DIG_CH2            		//	Digital Input Channel 2 Bypass Clock	PCLK1
//                  CLK_DIG_CH3            		//	Digital Input Channel 3 Bypass Clock	PCLK2
//                  CLK_DIG_CH0_DIV2       		//	Digital Input Channel 0 1/2 Divide Clock
//                  CLK_DIG_CH1_DIV2       		//	Digital Input Channel 1 1/2 Divide Clock
//                  CLK_DIG_CH2_DIV2       		//	Digital Input Channel 2 1/2 Divide Clock
//                  CLK_DIG_CH3_DIV2       		//	Digital Input Channel 3 1/2 Divide Clock
//                  CLK_PLL_DIV0           		//	ISP Adjust Clock (Special)
//                  CLK_PLL_DIV1           		//	ISP Adjust Clock (Special)
//                  CLK_PLL_DIV2           		//	ISP Adjust Clock (Special)
//
//	Htwi, Vtwi	->	Sync Total Count
//	Hw, Vw		->	Active Width
//	OnOff		->	Sync Generator On/Off 0: Off, 1: On

void Isp_ISyncGen_Config1(BYTE SyncMode, BYTE Clk, UINT Htwi, UINT Vtwi, UINT Hw, UINT Vw, BOOL OnOff)
{
	RYC_OSYNC_MOD1w(SyncMode);
	RYC_HTWO1w(Htwi-2);
	RYC_VTWO1w(Vtwi-1);
	RYC_HWI1w(Hw);
	RYC_VWI1w(Vw);

	if(OnOff)	{		RYCCK1_SET(Clk);	}
	else		{		RYCCK1_PDw(0);		}
}

void Isp_ISyncGen_Config2(BYTE SyncMode, BYTE Clk, UINT Htwi, UINT Vtwi, UINT Hw, UINT Vw, BOOL OnOff)
{
	RYC_OSYNC_MOD2w(SyncMode);
	RYC_HTWO2w(Htwi-2);
	RYC_VTWO2w(Vtwi-1);
	RYC_HWI2w(Hw);
	RYC_VWI2w(Vw);

	if(OnOff)	{		RYCCK2_SET(Clk);	}
	else		{		RYCCK2_PDw(0);		}
}

void Isp_ISyncGen_Config3(BYTE SyncMode, BYTE Clk, UINT Htwi, UINT Vtwi, UINT Hw, UINT Vw, BOOL OnOff)
{
	RYC_OSYNC_MOD3w(SyncMode);
	RYC_HTWO3w(Htwi-2);
	RYC_VTWO3w(Vtwi-1);
	RYC_HWI3w(Hw);
	RYC_VWI3w(Vw);

	if(OnOff)	{		RYCCK3_SET(Clk);	}
	else		{		RYCCK3_PDw(0);		}
}

//	DDR Digital Read Path Function---------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//	OnOff	->	0 : Off,	1 : On
//
//	Color	->	WR_COLOR						1
//				WR_NO_COLOR						0
//
//	Path	->	RD_CH_PIP0						0		//	PIP Channel 0
//              RD_CH_PIP1		                1		//	PIP Channel 1
//              RD_CH_PIP2		                2		//	PIP Channel 2
//              RD_CH_PIP3                      3		//	PIP Channel 3
//				RD_CH_ISP_SYNC					4
//				RD_CH_ISYNC1					5
//				RD_CH_ISYNC2					6
//				RD_CH_ISYNC3					7
//              RD_CH_IMG_MERGE                 8		//	Image Merge Channel
//
//	Clk		->	CLK_297M						0 		//	ISP PLL 297M
//              CLK_148M               			1 		//	ISP PLL 148.5M
//              CLK_74M                			2       //	ISP PLL 74.25M
//              CLK_DIG_CH0            			3       //	Digital Input Channel 0 Bypass Clock	PCLK	(EM675 PIN : SS_CKI)
//              CLK_DIG_CH1            			4       //	Digital Input Channel 1 Bypass Clock	PCLK0	(EM675 PIN : GPIO4)
//              CLK_DIG_CH2            			5       //	Digital Input Channel 2 Bypass Clock	PCLK1   (EM675 PIN : GPIO5)
//              CLK_DIG_CH3            			6       //	Digital Input Channel 3 Bypass Clock	PCLK2   (EM675 PIN : GPIO6)
//              CLK_DIG_CH0_DIV2       			7       //	Digital Input Channel 0 1/2 Divide Clock
//              CLK_DIG_CH1_DIV2       			8       //	Digital Input Channel 1 1/2 Divide Clock
//              CLK_DIG_CH2_DIV2       			9       //	Digital Input Channel 2 1/2 Divide Clock
//              CLK_DIG_CH3_DIV2       			10      //	Digital Input Channel 3 1/2 Divide Clock
//              CLK_PLL_DIV0           			11      //	ISP Adjust Clock (Special)
//
//	RdSync	->	RD_ISP_SYNC						0		//	ISP Post Sync
//              RD_DIG_CH0_SYNC		            1       //	Digital Input Channel 0 Sync
//              RD_DIG_CH1_SYNC		            2       //	Digital Input Channel 1 Sync
//              RD_DIG_CH2_SYNC		            3       //	Digital Input Channel 2 Sync
//              RD_DIG_CH3_SYNC		            4       //	Digital Input Channel 3 Sync
//              RD_SYNC1			            5       //	Internal Generation Sync 1
//              RD_SYNC2			            6       //	Internal Generation Sync 2
//              RD_SYNC3			            7       //	Internal Generation Sync 3
//
//	BYTE VlcMode	->	NO_VLCBIT				3
//						VLC_4BIT				2
//						VLC_5BIT				1
//						VLC_6BIT				0
//
//	WrCh	->	Use Default Value
//
//	Normally Default Value -> Color : 1
void Isp_RdCh0_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BOOL OnOff)
{
	IM_RHWI0w(Hw);										//	Read Horizontal Width
	IM_CLRREN0w(Color);									//	Read Color ?

	IM_RVSEL0w(RdSync);
	IM_RISEL0w(Path);

	if(OnOff)	{	IM_RON0w(1);	YCR_CK0_SET(Clk);	}
	else		{	IM_RON0w(0);	YCR_CK0_PDw(0);		}
}

//	Normally Default Value -> Color : 1, Rdock : 0, WrCh : 0
void Isp_RdCh1_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BYTE VlcMode, BOOL IsFrc, BYTE Rdock, BYTE WrCh, BOOL OnOff)
{
	IM_RHWI1w(Hw);										//	Read Horizontal Width
	IM_CLRREN1w(Color);									//	Read Color ?
	IM_RFRC1_ONw(IsFrc);								//	Use Frc ?
	IM_FRC1_RLOCKONw(Rdock);							//	Read Address Base FRC ?
	IM_RCH1_SELw(WrCh);									//	Default Value

	IM_RVSEL1w(RdSync);
	IM_RISEL1w(Path);

	if(VlcMode==NO_VLCBIT)	{	LC_RON1w(0);	}
	else					{	LC_RON1w(1);	}

	if(OnOff)	{	IM_RON1w(1);	YCR_CK1_SET(Clk);	}
	else		{	IM_RON1w(0);	YCR_CK1_PDw(0);		}
}

//	Normally Default Value -> Color : 1, Rdock : 0, WrCh : 1
void Isp_RdCh2_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BYTE VlcMode, BOOL IsFrc, BYTE Rdock, BYTE WrCh, BOOL OnOff)
{
	IM_RHWI2w(Hw);										//	Read Horizontal Width
	IM_CLRREN2w(Color);									//	Read Color ?
	IM_RFRC2_ONw(IsFrc);								//	Use Frc ?
	IM_FRC2_RLOCKONw(Rdock);							//	Read Address Base FRC ?
	IM_RCH2_SELw(WrCh);									//	Default Value

	IM_RVSEL2w(RdSync);
	IM_RISEL2w(Path);

	if(VlcMode==NO_VLCBIT)	{	LC_RON2w(0);	}
	else					{	LC_RON2w(1);	}

	if(OnOff)	{	IM_RON2w(1);	YCR_CK2_SET(Clk);	}
	else		{	IM_RON2w(0);	YCR_CK2_PDw(0);		}
}

//	Normally Default Value -> Color : 1, Rdock : 0, WrCh : 2
void Isp_RdCh3_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BYTE VlcMode, BOOL IsFrc, BYTE Rdock, BYTE WrCh, BOOL OnOff)
{
	IM_RHWI3w(Hw);										//	Read Horizontal Width
	IM_CLRREN3w(Color);									//	Read Color ?
	IM_RFRC3_ONw(IsFrc);								//	Use Frc ?
	IM_FRC3_RLOCKONw(Rdock);							//	Read Address Base FRC ?
	IM_RCH3_SELw(WrCh);									//	Default Value

	IM_RVSEL3w(RdSync);
	IM_RISEL3w(Path);

	if(VlcMode==NO_VLCBIT)	{	LC_RON3w(0);	}
	else					{	LC_RON3w(1);	}

	if(OnOff)	{	IM_RON3w(1);	YCR_CK3_SET(Clk);	}
	else		{	IM_RON3w(0);	YCR_CK3_PDw(0);		}
}

//	Normally Default Value -> Color : 1, Rdock : 0, WrCh : 3
void Isp_RdCh4_Config(BYTE Path, SHORT Hw, BYTE Color, BYTE Clk, BYTE RdSync, BYTE VlcMode, BOOL IsFrc, BYTE Rdock, BYTE WrCh, BOOL OnOff)
{
	IM_RHWI4w(Hw);										//	Read Horizontal Width
	IM_CLRREN4w(Color);									//	Read Color ?
	IM_RFRC4_ONw(IsFrc);								//	Use Frc ?
	IM_FRC4_RLOCKONw(Rdock);							//	Read Address Base FRC ?
	IM_RCH4_SELw(WrCh);									//	Default Value

	IM_RVSEL4w(RdSync);
	IM_RISEL4w(Path);

	if(VlcMode==NO_VLCBIT)	{	LC_RON4w(0);	}
	else					{	LC_RON4w(1);	}

	if(OnOff)	{	IM_RON4w(1);	YCR_CK4_SET(Clk);	}
	else		{	IM_RON4w(0);	YCR_CK4_PDw(0);		}
}

//	Read Channel0 Manual Read Address Setting	-> Read Channel 0 is only using manual address
void Isp_RdCh0_MAdr(UINT YAdr, UINT CAdr)
{
	IM_YADR0w(YAdr);
	IM_CADR0w(CAdr);
}

//	Read Channel1 Manual Read Address Setting
void Isp_RdCh1_MAdr(UINT YAdr, UINT CAdr)
{
	IM_YADR1_P1w(YAdr);
	IM_CADR1_P1w(CAdr);
}

//	Read Channel2 Manual Read Address Setting
void Isp_RdCh2_MAdr(UINT YAdr, UINT CAdr)
{
	IM_YADR2_P1w(YAdr);
	IM_CADR2_P1w(CAdr);
}

//	Read Channel3 Manual Read Address Setting
void Isp_RdCh3_MAdr(UINT YAdr, UINT CAdr)
{
	IM_YADR3_P1w(YAdr);
	IM_CADR3_P1w(CAdr);
}

//	Read Channel4 Manual Read Address Setting
void Isp_RdCh4_MAdr(UINT YAdr, UINT CAdr)
{
	IM_YADR4_P1w(YAdr);
	IM_CADR4_P1w(CAdr);
}


//	Digital Down Scaler Path Function---------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------
void Isp_DS_Step_Config(UINT DsCoeff)
{
	CODEC_DS_STEPw(DsCoeff);
}

//	DSRatioX, DSRatioY -> 64 (1 : 1), 128( 1: 0.5), ........
//	LPF 		-> 0: Weak, 1: Normal, 2: Strong
//	Clk			->	CLK_297M
//					CLK_148M
//					CLK_74M
//					CLK_DIG_CH0
//					CLK_DIG_CH1
//					CLK_DIG_CH2
//					CLK_DIG_CH3
//					CLK_DIG_CH0_DIV2
//					CLK_DIG_CH1_DIV2
//					CLK_DIG_CH2_DIV2
//					CLK_DIG_CH3_DIV2
//					CLK_PLL_DIV0
//	DsHwi		->	Input real horizontal width
//	DsVwi		->	Input real vertical width
//	Hlpf,Vlpf	->	LPF_LV1 : Weak
//              	LPF_LV2 : Normal
//              	LPF_LV3 : Strong
//	Hoff,Voff	->	0
//	OnOff		->	FN_ON, FN_OFF
void Isp_DS0_Config(BYTE Path, BYTE Clk, SHORT DSRatioX, SHORT DSRatioY, SHORT DsHwi, SHORT DsVwi, BYTE Hlpf, BYTE Vlpf, SHORT Hoff, SHORT Voff, BOOL OnOff)
{

	//	Claculate H/V Width
	DS0_DKXw(DSRatioX);
	DS0_DKYw(DSRatioY);
	CDS0_HWw((((CODEC_DS_STEPr<<10)/DSRatioX)*DsHwi)>>10);
	CDS0_VWw((((CODEC_DS_STEPr<<10)/DSRatioY)*DsVwi)>>10);

	//	Select Lpf Strength
	DS0_HLPFw(Hlpf);
	DS0_VLPFw(Vlpf);

	//	Down-Sclae Start Offset
	DS0_HOFFw(Hoff);
	DS0_VOFFw(Voff);

	//	Select Path
	switch(Path)
	{
		case DS_ISP_FONT_PATH		    :	CDS0_VISELw(0);	break;
		case DS_ISP_NO_FONT_PATH		:	CDS0_VISELw(0);	break;
		case DS_DIGITAL_CH0_PATH		:	CDS0_VISELw(1);	break;
		case DS_DIGITAL_CH1_PATH		:	CDS0_VISELw(2);	break;
		case DS_DIGITAL_CH2_PATH		:	CDS0_VISELw(3);	break;
		case DS_DIGITAL_CH3_PATH		:	CDS0_VISELw(4);	break;
		case DS_DIGITAL_CH0_FONT_PATH   :	CDS0_VISELw(1);	break;
		case DS_DIGITAL_CH1_FONT_PATH   :	CDS0_VISELw(2);	break;
		case DS_DIGITAL_CH2_FONT_PATH   :	CDS0_VISELw(3);	break;
		case DS_DIGITAL_CH3_FONT_PATH   :	CDS0_VISELw(4);	break;
		case DS_COLOR_BAR_PATH		    :					break;			//	Special Path
		case DS_DDR_RD_CH1_PATH		    :					break;          //	Special Path
		case DS_DDR_RD_CH2_PATH		    :					break;          //	Special Path
		case DS_DDR_RD_CH3_PATH		    :					break;          //	Special Path
		case DS_DDR_RD_CH4_PATH		    :					break;          //	Special Path
		case DS_DDR_RD_DZOOM_PATH	    :					break;          //	Special Path
		case DS_JPEG_DEC_FONT_PATH	    :					break;          //	Special Path
		case DS_JPEG_DEC_RD_PATH		:					break;          //	Special Path
		case DS_THERNAL_Y_PATH		    :	CDS0_VISELw(9);	break;
	}

	CDS0_ISELw(Path);
	//	Down-Scaler On/Off
	if(OnOff)		{	CDS0_PCK_SET(Clk);	}
	else			{	CDS0_PCK_PDw(0);	}

	DS0_ONw(OnOff);
}

void Isp_DS1_Config(BYTE Path, BYTE Clk, SHORT DSRatioX, SHORT DSRatioY, SHORT DsHwi, SHORT DsVwi, BYTE Hlpf, BYTE Vlpf, SHORT Hoff, SHORT Voff, BOOL OnOff)
{

	//	Claculate H/V Width
	DS1_DKXw(DSRatioX);
	DS1_DKYw(DSRatioY);
	CDS1_HWw((((CODEC_DS_STEPr<<10)/DSRatioX)*DsHwi)>>10);
	CDS1_VWw((((CODEC_DS_STEPr<<10)/DSRatioY)*DsVwi)>>10);

	//	Select Lpf Strength
	DS1_HLPFw(Hlpf);
	DS1_VLPFw(Vlpf);

	//	Down-Sclae Start Offset
	DS1_HOFFw(Hoff);
	DS1_VOFFw(Voff);

	//	Select Path
	switch(Path)
	{
		case DS_ISP_FONT_PATH		    :	CDS1_VISELw(0);		break;
		case DS_ISP_NO_FONT_PATH		:	CDS1_VISELw(0);		break;
		case DS_DIGITAL_CH0_PATH		:	CDS1_VISELw(1);		break;
		case DS_DIGITAL_CH1_PATH		:	CDS1_VISELw(2);		break;
		case DS_DIGITAL_CH2_PATH		:	CDS1_VISELw(3);		break;
		case DS_DIGITAL_CH3_PATH		:	CDS1_VISELw(4);		break;
		case DS_DIGITAL_CH0_FONT_PATH   :	CDS1_VISELw(1);		break;
		case DS_DIGITAL_CH1_FONT_PATH   :	CDS1_VISELw(2);		break;
		case DS_DIGITAL_CH2_FONT_PATH   :	CDS1_VISELw(3);		break;
		case DS_DIGITAL_CH3_FONT_PATH   :	CDS1_VISELw(4);		break;
		case DS_COLOR_BAR_PATH		    :						break;			//	Special Path
		case DS_DDR_RD_CH1_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_CH2_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_CH3_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_CH4_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_DZOOM_PATH	    :						break;          //	Special Path
		case DS_JPEG_DEC_FONT_PATH	    :						break;          //	Special Path
		case DS_JPEG_DEC_RD_PATH		:						break;          //	Special Path
		case DS_THERNAL_Y_PATH		    :	CDS1_VISELw(9);		break;
	}

	CDS1_ISELw(Path);

	//	Down-Scaler On/Off
	if(OnOff)		{	CDS1_PCK_SET(Clk);	}
	else			{	CDS1_PCK_PDw(0);	}

	DS1_ONw(OnOff);
}

void Isp_DS2_Config(BYTE Path, BYTE Clk, SHORT DSRatioX, SHORT DSRatioY, SHORT DsHwi, SHORT DsVwi, BYTE Hlpf, BYTE Vlpf, SHORT Hoff, SHORT Voff, BOOL OnOff)
{

	//	Claculate H/V Width
	DS2_DKXw(DSRatioX);
	DS2_DKYw(DSRatioY);
	CDS2_HWw((((CODEC_DS_STEPr<<10)/DSRatioX)*DsHwi)>>10);
	CDS2_VWw((((CODEC_DS_STEPr<<10)/DSRatioY)*DsVwi)>>10);

	//	Select Lpf Strength
	DS2_HLPFw(Hlpf);
	DS2_VLPFw(Vlpf);

	//	Down-Sclae Start Offset
	DS2_HOFFw(Hoff);
	DS2_VOFFw(Voff);

	//	Select Path
	switch(Path)
	{
		case DS_ISP_FONT_PATH		    :	CDS2_VISELw(0);		break;
		case DS_ISP_NO_FONT_PATH		:	CDS2_VISELw(0);		break;
		case DS_DIGITAL_CH0_PATH		:	CDS2_VISELw(1);		break;
		case DS_DIGITAL_CH1_PATH		:	CDS2_VISELw(2);		break;
		case DS_DIGITAL_CH2_PATH		:	CDS2_VISELw(3);		break;
		case DS_DIGITAL_CH3_PATH		:	CDS2_VISELw(4);		break;
		case DS_DIGITAL_CH0_FONT_PATH   :	CDS2_VISELw(1);		break;
		case DS_DIGITAL_CH1_FONT_PATH   :	CDS2_VISELw(2);		break;
		case DS_DIGITAL_CH2_FONT_PATH   :	CDS2_VISELw(3);		break;
		case DS_DIGITAL_CH3_FONT_PATH   :	CDS2_VISELw(4);		break;
		case DS_COLOR_BAR_PATH		    :						break;			//	Special Path
		case DS_DDR_RD_CH1_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_CH2_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_CH3_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_CH4_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_DZOOM_PATH	    :						break;          //	Special Path
		case DS_JPEG_DEC_FONT_PATH	    :						break;          //	Special Path
		case DS_JPEG_DEC_RD_PATH		:						break;          //	Special Path
		case DS_THERNAL_Y_PATH		    :	CDS2_VISELw(9);		break;
	}

	CDS2_ISELw(Path);

	//	Down-Scaler On/Off
	if(OnOff)		{	CDS2_PCK_SET(Clk);	}
	else			{	CDS2_PCK_PDw(0);	}

	DS2_ONw(OnOff);

}

void Isp_DS3_Config(BYTE Path, BYTE Clk, SHORT DSRatioX, SHORT DSRatioY, SHORT DsHwi, SHORT DsVwi, BYTE Hlpf, BYTE Vlpf, SHORT Hoff, SHORT Voff, BOOL OnOff)
{

	//	Claculate H/V Width
	DS3_DKXw(DSRatioX);
	DS3_DKYw(DSRatioY);
	CDS3_HWw((((CODEC_DS_STEPr<<10)/DSRatioX)*DsHwi)>>10);
	CDS3_VWw((((CODEC_DS_STEPr<<10)/DSRatioY)*DsVwi)>>10);

	//	Select Lpf Strength
	DS3_HLPFw(Hlpf);
	DS3_VLPFw(Vlpf);

	//	Down-Sclae Start Offset
	DS3_HOFFw(Hoff);
	DS3_VOFFw(Voff);

	//	Select Path
	switch(Path)
	{
		case DS_ISP_FONT_PATH		    :	CDS3_VISELw(0);		break;
		case DS_ISP_NO_FONT_PATH		:	CDS3_VISELw(0);		break;
		case DS_DIGITAL_CH0_PATH		:	CDS3_VISELw(1);		break;
		case DS_DIGITAL_CH1_PATH		:	CDS3_VISELw(2);		break;
		case DS_DIGITAL_CH2_PATH		:	CDS3_VISELw(3);		break;
		case DS_DIGITAL_CH3_PATH		:	CDS3_VISELw(4);		break;
		case DS_DIGITAL_CH0_FONT_PATH   :	CDS3_VISELw(1);		break;
		case DS_DIGITAL_CH1_FONT_PATH   :	CDS3_VISELw(2);		break;
		case DS_DIGITAL_CH2_FONT_PATH   :	CDS3_VISELw(3);		break;
		case DS_DIGITAL_CH3_FONT_PATH   :	CDS3_VISELw(4);		break;
		case DS_COLOR_BAR_PATH		    :						break;			//	Special Path
		case DS_DDR_RD_CH1_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_CH2_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_CH3_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_CH4_PATH		    :						break;          //	Special Path
		case DS_DDR_RD_DZOOM_PATH	    :						break;          //	Special Path
		case DS_JPEG_DEC_FONT_PATH	    :						break;          //	Special Path
		case DS_JPEG_DEC_RD_PATH		:						break;          //	Special Path
		case DS_THERNAL_Y_PATH		    :	CDS3_VISELw(9);		break;
	}

	CDS3_ISELw(Path);

	//	Down-Scaler On/Off
	if(OnOff)		{	CDS3_PCK_SET(Clk);	}
	else			{	CDS3_PCK_PDw(0);	}

	DS3_ONw(OnOff);
}

//	EdgeOnOff	->	0 : Off, 1: On
//	MGain		->	Edge Enhancement Main Gain (ROI Center Area)
//	Slice		->	Y Slice Y Threshold (Low Value)
//	Clip		->	Y Clip Y Threshold (High Value)
//	ROIOnOff	->	Edge Enhancement ROI Function On/Off	0 : Off, 1: On
//	ROIHsp		->	ROI Area Horizontal Start Position
//	ROIHed		->	ROI Area Horizontal End Position
//	ROIVsp		->	ROI Area Vertical Start Position
//	ROIVed		->	ROI Area Vertical End Position
//	ROIGain		->	ROI Side Area Gain (Top, Botton, Right, Left Area)
//	ROIOsdOnOff	->	ROI Area OSD On/Off
void Isp_DS0_Edge_Enhance_Config(BOOL EdgeOnOff, BYTE MGain, BYTE Slice, BYTE Clip, BYTE ROIOnOff, SHORT ROIHsp, SHORT ROIHed, SHORT ROIVsp, SHORT ROIVed, BYTE ROIGain, BOOL ROIOsdOnOff)
{
	DS0_APTGAw(MGain);
	DS0_APTSLw(Slice);
	DS0_APTCLw(Clip);

	DS0_APTHSPw(ROIHsp);
	DS0_APTHEDw(ROIHed);
	DS0_APTVSPw(ROIVsp);
	DS0_APTVEDw(ROIVed);

	DS0_APTGA_Rw(ROIGain);
	DS0_RAPT_OSD_ONw(ROIOsdOnOff);
	DS0_RAPT_ONw(ROIOnOff);
	DS0_APTONw(EdgeOnOff);
}

void Isp_DS1_Edge_Enhance_Config(BOOL EdgeOnOff, BYTE MGain, BYTE Slice, BYTE Clip, BYTE ROIOnOff, SHORT ROIHsp, SHORT ROIHed, SHORT ROIVsp, SHORT ROIVed, BYTE ROIGain, BOOL ROIOsdOnOff)
{
	DS1_APTGAw(MGain);
	DS1_APTSLw(Slice);
	DS1_APTCLw(Clip);

	DS1_APTHSPw(ROIHsp);
	DS1_APTHEDw(ROIHed);
	DS1_APTVSPw(ROIVsp);
	DS1_APTVEDw(ROIVed);

	DS1_APTGA_Rw(ROIGain);
	DS1_RAPT_OSD_ONw(ROIOsdOnOff);
	DS1_RAPT_ONw(ROIOnOff);
	DS1_APTONw(EdgeOnOff);
}

void Isp_DS2_Edge_Enhance_Config(BOOL EdgeOnOff, BYTE MGain, BYTE Slice, BYTE Clip, BYTE ROIOnOff, SHORT ROIHsp, SHORT ROIHed, SHORT ROIVsp, SHORT ROIVed, BYTE ROIGain, BOOL ROIOsdOnOff)
{
	DS2_APTGAw(MGain);
	DS2_APTSLw(Slice);
	DS2_APTCLw(Clip);

	DS2_APTHSPw(ROIHsp);
	DS2_APTHEDw(ROIHed);
	DS2_APTVSPw(ROIVsp);
	DS2_APTVEDw(ROIVed);

	DS2_APTGA_Rw(ROIGain);
	DS2_RAPT_OSD_ONw(ROIOsdOnOff);
	DS2_RAPT_ONw(ROIOnOff);
	DS2_APTONw(EdgeOnOff);
}

void Isp_DS3_Edge_Enhance_Config(BOOL EdgeOnOff, BYTE MGain, BYTE Slice, BYTE Clip, BYTE ROIOnOff, SHORT ROIHsp, SHORT ROIHed, SHORT ROIVsp, SHORT ROIVed, BYTE ROIGain, BOOL ROIOsdOnOff)
{
	DS3_APTGAw(MGain);
	DS3_APTSLw(Slice);
	DS3_APTCLw(Clip);

	DS3_APTHSPw(ROIHsp);
	DS3_APTHEDw(ROIHed);
	DS3_APTVSPw(ROIVsp);
	DS3_APTVEDw(ROIVed);

	DS3_APTGA_Rw(ROIGain);
	DS3_RAPT_OSD_ONw(ROIOsdOnOff);
	DS3_RAPT_ONw(ROIOnOff);
	DS3_APTONw(EdgeOnOff);
}

//	Digital Zoom Function---------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//	OnOff			->	0 : Off, 1 : On
//	DzSync			->	DZ_ISP_SYNC		//	ISP Sync (For Image Blending + Dzoom)
//	                    DZ_ISYNC_GEN1	//	Internal Sync Generator 1 Sync
//	                    DZ_ISYNC_GEN2	//	Internal Sync Generator 2 Sync
//	                    DZ_ISYNC_GEN3	//	Internal Sync Generator 3 Sync
//	                    DZ_PIP_CH0		//	PIP Channel 0 Sync
//	                    DZ_PIP_CH1		//	PIP Channel 1 Sync
//	                    DZ_PIP_CH2		//	PIP Channel 2 Sync
//	                    DZ_PIP_CH3		//	PIP Channel 3 Sync
//
//	IsAutoAlign		->	DZ_MANUAL_ALGN : Manual Alignment(Left Up Start Position), DZ_AUTO_ALGN : auto Alignment (Center Align)
//						When (IsAutoAlign==0), Control HOffset & VOffset
//	IsDkxAuto		->	DZ_DKX_MANUAL : Control DKX & DKY separately, DZ_DKX_AUTO : Control DKY (=DKX)
//	DzoomCh			->	DZ_RD_CH0		//	DDR Read Channel 0
//	                    DZ_RD_CH1		//	DDR Read Channel 1
//	                    DZ_RD_CH2		//	DDR Read Channel 2
//	                    DZ_RD_CH3		//	DDR Read Channel 3
//	                    DZ_RD_CH4		//	DDR Read Channel 4
//	DzVwo			->	Read image vertical width : Equate to read channel vertical width
//	Dkx, Dky		->	Dzoom Ratio : 0x1000 - X1, 0x800 - X2, 0x400 - X4, ......
//	DzRdHsp			->	Read Dzoom Image Horizontal Start Position	(When using ISP Sync, It is Valid -> Other case Don't care)
//	DzRdVsp			->	Read Dzoom Image Vertical Start Position	(When using ISP Sync, It is Valid -> Other case Don't care)
//	HOffset			->	Dzoom Horizontal Align Offset (When IsAutoAlign==0, Don't care)
//	VOffset			->	Dzoom Vertical Align Offset	  (When IsAutoAlign==0, Don't care)

void Isp_DZoom_Config(BOOL OnOff, BYTE Clk, BYTE DzSync, BOOL IsAutoAlign, BOOL IsDkxAuto, BYTE DzoomCh, UINT DzVwo, UINT Dkx, UINT Dky, UINT DzRdHsp, UINT DzRdVsp, UINT HOffset, UINT VOffset)
{
	if(IsAutoAlign)					{	DZ_DAONw(IsAutoAlign);														}
	else							{	DZ_DAONw(IsAutoAlign);		DZ_HSP_POSw(HOffset);	DZ_VSP_POSw(VOffset);	}

	if(DzSync==DZ_ISP_SYNC)			{	DZOOM_VSPw(DzRdVsp);	DZOOM_HSPw(DzRdHsp);			}
	else if(DzSync==DZ_ISYNC_GEN1)	{															}
	else if(DzSync==DZ_ISYNC_GEN2)	{															}
	else							{															}

	DZ_VWOw(DzVwo);													//	Dzoom Image Vertical Width
	DZ_CH_SELw(DzoomCh);											//	Select Dzoom Channel
	DZ_DKXY_ONw(IsDkxAuto);											//	DKX,DKY Control Way
	DZ_BUF_ASELw(DzSync);											//	Select Sync
	DZ_ZHLOCK_SELw(DzSync);
	DZ_VSP_KYw(Dky);
	DZ_HSP_KXw(Dkx);

	if(DzoomCh==DZ_RD_CH0)			IM_RYCB_MOD0w(1);
	else if(DzoomCh==DZ_RD_CH1)		IM_RYCB_MOD1w(1);
	else if(DzoomCh==DZ_RD_CH2)		IM_RYCB_MOD2w(1);
	else if(DzoomCh==DZ_RD_CH3)		IM_RYCB_MOD3w(1);
	else							IM_RYCB_MOD4w(1);

	if(OnOff)				{	DZPCK_SET(Clk);	}					//	Dzoom Clock OnOff
	else					{	DZPCK_PDw(0);	}

	DZ_DZOOM_ONw(OnOff);											//	Dzoom OnOff
}

void Isp_DZoom_Ratio(UINT Dkx, UINT Dky)
{
	DZ_VSP_KYw(Dky);
	DZ_HSP_KXw(Dkx);
}

//	PIP Function------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

//	PipOnOff	->	Total PIP On/Off
//	Clk			->	PIP_ISP_297M				//	 ISP PLL 297M
//					PIP_ISP_148M				//	 ISP PLL 148.5M
//					PIP_ISP_74M					//	 ISP PLL 74.25M
//					PIP_DIG_CH0					//	 Digital Input Channel 0 Bypass Clock	PCLK	(EN675 PIN : SS_CKI)
//					PIP_DIG_CH1					//	 Digital Input Channel 1 Bypass Clock	PCLK0	(EN675 PIN : GPIO4)
//					PIP_DIG_CH2					//	 Digital Input Channel 2 Bypass Clock	PCLK1   (EN675 PIN : GPIO5)
//					PIP_DIG_CH3					//	 Digital Input Channel 3 Bypass Clock	PCLK2   (EN675 PIN : GPIO6)
//					PIP_DIG_CH0_DIV2			//	 Digital Input Channel 0 1/2 Divide Clock
//					PIP_DIG_CH1_DIV2			//	 Digital Input Channel 1 1/2 Divide Clock
//					PIP_DIG_CH2_DIV2			//	 Digital Input Channel 2 1/2 Divide Clock
//					PIP_DIG_CH3_DIV2			//	 Digital Input Channel 3 1/2 Divide Clock
//					PIP_PCK_DIV0                //	 ISP Adjust Clock 0 (Special)
//					PIP_PCK_DIV2                //	 ISP Adjust Clock 2 (Special)
//	MainImgCh	->	Select Background
//					PIP_MAIN_ISP				//	ISP Image
//					PIP_MAIN_DIG_CH0            //	Digital Input Channel 0
//					PIP_MAIN_DIG_CH1            //  Digital Input Channel 1
//					PIP_MAIN_DIG_CH2            //  Digital Input Channel 2
//					PIP_MAIN_DIG_CH3            //  Digital Input Channel 3
//					PIP_MAIN_DDR_RD_CH0         //	DDR Read Channel 0
//					PIP_MAIN_DDR_RD_CH1         //  DDR Read Channel 1
//					PIP_MAIN_DDR_RD_CH2         //  DDR Read Channel 2
//					PIP_MAIN_DDR_RD_CH3         //  DDR Read Channel 3
//					PIP_MAIN_DDR_RD_CH4         //  DDR Read Channel 4
//	Sync		->	When (MainImgCh == PIP_MAIN_ISP ,PIP_MAIN_DIG_CH0, PIP_MAIN_DIG_CH1, PIP_MAIN_DIG_CH2, PIP_MAIN_DIG_CH3), Don't Care
//					PIP_ISP_SYNC				//	ISP Sync
//					PIP_DIG_CH0_SYNC			//	Internal Sync Generator 1 Sync
//					PIP_DIG_CH1_SYNC			//	Internal Sync Generator 2 Sync
//					PIP_DIG_CH2_SYNC			//	Internal Sync Generator 3 Sync
//		            PIP_DIG_CH3_SYNC			//	PIP Channel 0 Sync
//		            PIP_ISYNC_GEN1				//	PIP Channel 1 Sync
//		            PIP_ISYNC_GEN2				//	PIP Channel 2 Sync
//		            PIP_ISYNC_GEN3				//	PIP Channel 3 Sync
//	IsBaseImgMask	->	PIP_MMASK				//	Main Image Mask (Balck Color)
//						PIP_NO_MMASK			//	Main Image Bypass

void Isp_Pip_Clk_Config(BOOL PipOnOff, BYTE Clk, BYTE MainImgCh, BYTE Sync, BOOL IsBaseImgMask)
{
	PIP_MAIN_ISELw(MainImgCh);
	PIP_IVSELw(Sync);
	PIP_MMASKw(IsBaseImgMask);

	if(PipOnOff)		{	PIP_PCK_SET(Clk);	FONT0_PIP_ONw(1);	}
	else				{	FONT0_PIP_ONw(0);	PIP0_ONw(0);	PIP1_ONw(0);	PIP2_ONw(0);	PIP3_ONw(0);	PIP_PPCK_PDw(0);
							PIP0_BOX_ONw(0);	PIP1_BOX_ONw(0);	PIP2_BOX_ONw(0);	PIP3_BOX_ONw(0);
						}
}

//	PipRdCh			->	PIP_DDR_RD_CH0				//	DDR Read Channel 0
//	                    PIP_DDR_RD_CH1				//	DDR Read Channel 1
//	                    PIP_DDR_RD_CH2				//	DDR Read Channel 2
//	                    PIP_DDR_RD_CH3				//	DDR Read Channel 3
//	                    PIP_DDR_RD_CH4				//	DDR Read Channel 4
//	                    PIP_DDR_RD_DZOOM			//	DDR Read Dzoom Channel
//
//	PipHsp,PipVsp	->	PIP H/V Start Position
//	PipHw, PipVw	->	PIP H/V Width
//	IsLine			->	0 : No Outline, 1: Use Outline
//	LineY			->	Line Y	Value
//	LineCb          ->	Line Cb Value
//	LineCr          ->	Line Cr Value
//	Mix				->	PIP_MIX_100					//	100% PIP Image
//                        PIP_MIX_75                  //	75% PIP Image + 25% Background Image
//                        PIP_MIX_50                  //	50% PIP Image + 50% Background Image
//                        PIP_MIX_25                  //	25% PIP Image + 75% Background Image

void Isp_Pip_Ch0_Config(BOOL OnOff, BYTE PipRdCh, UINT PipHsp, UINT PipVsp, UINT PipHw, UINT PipVw, BOOL IsLine, UINT LineColor, BYTE Mix, UINT DdrLtc)
{
	PIP0_ISELw(PipRdCh);
	PIP0_HSPw(PipHsp);
	PIP0_VSPw(PipVsp);
	PIP0_HWw(PipHw);
	PIP0_VWw(PipVw);
	PIP0_BOX_ONw(IsLine);
	PIP0_BOX_Yw((LineColor>>16)&0xFF);
	PIP0_BOX_CBw((LineColor>>8)&0xFF);
	PIP0_BOX_CRw((LineColor)&0xFF);
	PIP0_MIX_SELw(Mix);
	PIP0_DDR_LTCw(DdrLtc);
	PIP0_ONw(OnOff);
}

void Isp_Pip_Ch1_Config(BOOL OnOff, BYTE PipRdCh, UINT PipHsp, UINT PipVsp, UINT PipHw, UINT PipVw, BOOL IsLine, UINT LineColor, BYTE Mix, UINT DdrLtc)
{
	PIP1_ISELw(PipRdCh);
	PIP1_HSPw(PipHsp);
	PIP1_VSPw(PipVsp);
	PIP1_HWw(PipHw);
	PIP1_VWw(PipVw);
	PIP1_BOX_ONw(IsLine);
	PIP1_BOX_Yw((LineColor>>16)&0xFF);
	PIP1_BOX_CBw((LineColor>>8)&0xFF);
	PIP1_BOX_CRw((LineColor)&0xFF);
	PIP1_MIX_SELw(Mix);
	PIP1_DDR_LTCw(DdrLtc);
	PIP1_ONw(OnOff);
}

void Isp_Pip_Ch2_Config(BOOL OnOff, BYTE PipRdCh, UINT PipHsp, UINT PipVsp, UINT PipHw, UINT PipVw, BOOL IsLine, UINT LineColor, BYTE Mix, UINT DdrLtc)
{
	PIP2_ISELw(PipRdCh);
	PIP2_HSPw(PipHsp);
	PIP2_VSPw(PipVsp);
	PIP2_HWw(PipHw);
	PIP2_VWw(PipVw);
	PIP2_BOX_ONw(IsLine);
	PIP2_BOX_Yw((LineColor>>16)&0xFF);
	PIP2_BOX_CBw((LineColor>>8)&0xFF);
	PIP2_BOX_CRw((LineColor)&0xFF);
	PIP2_MIX_SELw(Mix);
	PIP2_DDR_LTCw(DdrLtc);
	PIP2_ONw(OnOff);
}

void Isp_Pip_Ch3_Config(BOOL OnOff, BYTE PipRdCh, UINT PipHsp, UINT PipVsp, UINT PipHw, UINT PipVw, BOOL IsLine, UINT LineColor, BYTE Mix, UINT DdrLtc)
{
	PIP3_ISELw(PipRdCh);
	PIP3_HSPw(PipHsp);
	PIP3_VSPw(PipVsp);
	PIP3_HWw(PipHw);
	PIP3_VWw(PipVw);
	PIP3_BOX_ONw(IsLine);
	PIP3_BOX_Yw((LineColor>>16)&0xFF);
	PIP3_BOX_CBw((LineColor>>8)&0xFF);
	PIP3_BOX_CRw((LineColor)&0xFF);
	PIP3_MIX_SELw(Mix);
	PIP3_DDR_LTCw(DdrLtc);
	PIP3_ONw(OnOff);
}

//	Image Merge--------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//	OnOff		->	FN_ON, FN_OFF
//	Clk			->	Image Merge Operation Clock
//					ISP_CLK_PLL			: ISP PLL Clock (297 MHz)
//					ISP_CLK_148M		: ISP PLL / 2 (148.5 MHz)
//					ISP_CLK_74M			: ISP PLL / 4 (74.5 MHz)
//					ISP_CLK_PLL_DIV0	: ISP PLL Adjust Clock
//	ChCnt		->	Merge Channel Value
//					MRG_IMG2 : 2 Channel
//					MRG_IMG3 : 3 Channel
//					MRG_IMG4 : 4 Channel
//	RdSync		->	Select Read Sync
//					RD_ISP_SYNC
//					RD_DIG_CH0_SYNC
//					RD_DIG_CH1_SYNC
//					RD_DIG_CH2_SYNC
//					RD_DIG_CH3_SYNC
//					RD_SYNC1
//					RD_SYNC2
//					RD_SYNC3
//	HtwSync		->	Read sync horizontal total count
//	Htw			->	Merge image total horizontal pixel width
//	Hw			->	Each image horizontal pixel width
//
//	return value->	IF_SET_OK						0x77
//                  IF_NO_MATCH_ERROR				0x18

UINT Isp_Img_HMerge(BOOL OnOff, BYTE Clk, BYTE ChCnt, BYTE RdSync, UINT HtwSync, UINT Htw, UINT Hw, UINT Hsp, UINT Vsp, UINT Vw)
{
	if(Htw!=Hw*ChCnt)	{	return	IF_NO_MATCH_ERROR;	}

	MRG_HSP0w(Hsp);
	MRG_HW0w(Hw+16);

	MRG_HSP1w(Hsp+Hw);
	MRG_HW1w(Hw+16);

	MRG_HSP2w(Hsp+(Hw*2));
	MRG_HW2w(Hw+16);

	MRG_HSP3w(Hsp+(Hw*3));
	MRG_HW3w(Hw+16);

	MRG_VSPw(Vsp);
	MRG_VWw(Vw);
	MRG_CNTw(ChCnt);
	IMG_IVSELw(RdSync);
	MRG_HWw(Htw);

	RI1_MR_ONw(1);
	if(ChCnt==2)		{	RI2_MR_ONw(1);									}
	else if(ChCnt==3)	{	RI2_MR_ONw(1);	RI3_MR_ONw(1);					}
	else				{	RI2_MR_ONw(1);	RI3_MR_ONw(1);	RI4_MR_ONw(1);	}

	//	DDR Latency
	DDR_LTC_MRG0w(HtwSync-Hw);
	DDR_LTC_MRG1w(4);
	DDR_LTC_MRG2w(Hw);
	DDR_LTC_MRG3w(Hw*2);

	if(OnOff)	{	if(Clk==ISP_CLK_PLL_DIV0)	{	IMGM_CK_SET(3);	}	else	{	IMGM_CK_SET(Clk);	}	}
	else		{	IMGM_CK_PDw(0);		RI1_MR_ONw(0);	RI2_MR_ONw(0);	RI3_MR_ONw(0);	RI4_MR_ONw(0);		}

	return	IF_SET_OK;
}

//	Encoder Output-----------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//	OnOff		->	FN_ON, FN_OFF
//	IsNTSC		->	NTSC, PAL
//	CvbsFreq	->	FREQ_27M, FREQ_36M
//	SourceFrq	->	ISP_74M
//					ISP_148M
//	SourcePath	->	DS_ISP_FONT_PATH
//					DS_ISP_NO_FONT_PATH
//					DS_DIGITAL_CH0_PATH
//					DS_DIGITAL_CH1_PATH
//					DS_DIGITAL_CH2_PATH
//					DS_DIGITAL_CH3_PATH
//					DS_DIGITAL_CH0_FONT_PATH
//					DS_DIGITAL_CH1_FONT_PATH
//					DS_DIGITAL_CH2_FONT_PATH
//					DS_DIGITAL_CH3_FONT_PATH
//					DS_COLOR_BAR_PATH
//
//	VlcMode		->	NO_VLCBIT
//					VLC_4BIT
//					VLC_5BIT
//					VLC_6BIT

void Isp_Cvbs_Config(BOOL OnOff, BOOL IsNtsc, BYTE CvbsFreq, BYTE SourceFrq, BYTE SourcePath, BYTE VlcMode, UINT Hsp, UINT Vsp)
{
#if model_TgtBd == 2	// CVBS Æ÷ÇÔ ¾ÈµÊ
	OnOff = UP_OFF;
#endif

	UINT i, SourceSiz=0;
	const UINT *Source;

	ECK_PDw(0);		CDS4_PCK_PDw(0);	HWE_AUTOw(0);
	DS4_KXY_ONw(0);	DS4_HLPFw(2);	DS4_VLPFw(2);
	DS4_DKY_STw(0);	DS4_HOFFw(0);	DS4_VOFFw(0);

	DDR_RENC_LTCw(300);

	HSPEw(Hsp);
	VSPEw(Vsp);

	if(IsNtsc==NTSC)
	{
		if(CvbsFreq==FREQ_27M)	{																							//	27MHz CVBS
			Source = CVBS_27M_NTSC;		SourceSiz = ARRAY_SIZE(CVBS_27M_NTSC);
			HTWEw(0x359);		VTWEw(0x20c);		HWEw(0x2da);		VWEw(0xf8);		ECK_SELw(4);

			DS4_DKXw(0xaa);	DS4_DKYw(0x8c);	CDS4_HWw(0x300);	CDS4_VWw(0x1f2);
			if(SourceFrq==ISP_74M)	{	ENC_AL_RMODw(1);	ENC_AL_WMODw(1);	EFLD_INVw(0);	FLDE_INV2w(1);	}
			else					{	ENC_AL_RMODw(0);	ENC_AL_WMODw(0);	EFLD_INVw(0);	FLDE_INV2w(0);	}

			DS3_KXY_ONw(0);

		}
		else					{																					}		//	36MHz CVBS
	}
	else
	{
		if(CvbsFreq==FREQ_27M)	{					}
		else					{					}
	}

	for(i=0;i<SourceSiz; i++)	{	SetIsp(CVBS_ISP_ADR+i,Source[i]);	}

	//	VLC Configuration
	if(VlcMode==NO_VLCBIT)			{	LC_WENC_ONw(0);		LC_RENC_ONw(0);	}
	else{
		if(VlcMode==VLC_4BIT)		{	LC_ENC_BMODEw(2);	}
		else if(VlcMode==VLC_5BIT)	{	LC_ENC_BMODEw(1);	}
		else						{	LC_ENC_BMODEw(0);	}

		LC_WENC_ONw(1);
		LC_RENC_ONw(1);
	}

	//	Select Source Path & Clock
	switch(SourcePath)
	{
		 case DS_ISP_FONT_PATH         :	CDS4_VISELw(0);	CDS4_ISELw(0);
		 									if(PPCK_SELr==4)		CDS4_PCK_SELw(7);
		 									else if(PPCK_SELr==5)	CDS4_PCK_SELw(11);
		 									else					CDS4_PCK_SELw(PPCK_SELr);																					break;
		 case DS_ISP_NO_FONT_PATH      :    CDS4_VISELw(0);	CDS4_ISELw(1);
		 									if(PPCK_SELr==4)		CDS4_PCK_SELw(7);
		 									else if(PPCK_SELr==5)	CDS4_PCK_SELw(11);
		 									else					CDS4_PCK_SELw(PPCK_SELr);																					break;
		 case DS_DIGITAL_CH0_PATH      :    CDS4_VISELw(1);	CDS4_ISELw(2);	if(BT_PCK_SELr==3)	CDS4_PCK_SELw(3);	else	CDS4_PCK_SELw(7);	break;
		 case DS_DIGITAL_CH1_PATH      :    CDS4_VISELw(2);	CDS4_ISELw(3);	if(BT0_PCK_SELr==0)	CDS4_PCK_SELw(4);	else	CDS4_PCK_SELw(8);	break;
		 case DS_DIGITAL_CH2_PATH      :    CDS4_VISELw(3);	CDS4_ISELw(4);	if(BT1_PCK_SELr==0)	CDS4_PCK_SELw(5);	else	CDS4_PCK_SELw(9);	break;
		 case DS_DIGITAL_CH3_PATH      :    CDS4_VISELw(4);	CDS4_ISELw(5);	if(BT2_PCK_SELr==0)	CDS4_PCK_SELw(6);	else	CDS4_PCK_SELw(10);	break;
		 case DS_DIGITAL_CH0_FONT_PATH :    CDS4_VISELw(1);	CDS4_ISELw(6);	if(BT_PCK_SELr==3)	CDS4_PCK_SELw(3);	else	CDS4_PCK_SELw(7);	break;
		 case DS_DIGITAL_CH1_FONT_PATH :    CDS4_VISELw(2);	CDS4_ISELw(7);	if(BT0_PCK_SELr==0)	CDS4_PCK_SELw(4);	else	CDS4_PCK_SELw(8);	break;
		 case DS_DIGITAL_CH2_FONT_PATH :    CDS4_VISELw(3);	CDS4_ISELw(8);	if(BT1_PCK_SELr==0)	CDS4_PCK_SELw(5);	else	CDS4_PCK_SELw(9);	break;
		 case DS_DIGITAL_CH3_FONT_PATH :    CDS4_VISELw(4);	CDS4_ISELw(9);	if(BT2_PCK_SELr==0)	CDS4_PCK_SELw(6);	else	CDS4_PCK_SELw(10);	break;
		 case DS_COLOR_BAR_PATH        :    CDS4_VISELw(0);	CDS4_ISELw(10);
		 									if(PPCK_SELr==4)		CDS4_PCK_SELw(7);
		 									else if(PPCK_SELr==5)	CDS4_PCK_SELw(11);
		 									else					CDS4_PCK_SELw(PPCK_SELr);																					break;
	}


	if(OnOff==FN_ON)	{	DS4_ONw(1);		CDS4_PCK_PDw(1);	ECK_PDw(1);		ENC_ONw(1); }
	else				{	DS4_ONw(0);		CDS4_PCK_PDw(0);	ECK_PDw(0);		ENC_ONw(0);	}
}

void Isp_Cvbs_Adr(UINT CvbsAdr0, UINT CvbsAdr1, UINT CvbsAdr2)
{
	ENC_ADR0w(CvbsAdr0);
	ENC_ADR1w(CvbsAdr1);
	ENC_ADR2w(CvbsAdr2);
}

//	VLC Configuration Function------------------------------------------------------------------------------
void Isp_Vlc_Config(BYTE VlcMode)
{
	UINT i, SourceSiz=0;
	const UINT *Source;

	if(VlcMode==VLC_4BIT)		{	Source = VLC4BIT; SourceSiz = ARRAY_SIZE(VLC4BIT);	}
	else if(VlcMode==VLC_5BIT)	{	Source = VLC5BIT; SourceSiz = ARRAY_SIZE(VLC5BIT);	}
	else						{	Source = VLC6BIT; SourceSiz = ARRAY_SIZE(VLC6BIT);	}

//	printf("!!!!!!! VlcMode:%d\r\n", SourceSiz/*VlcMode*//*ARRAY_SIZE(Source)*/);

	for(i=0;i<SourceSiz; i++)
	{
		SetIsp(VLC_C_ADR+i,Source[i]);			//	Color VLC Table
		SetIsp(VLC_Y_ADR+i,Source[i]);			//	Y VLC Table
	}
}

