#ifndef __ISP_DIGITAL_INTERFACE_H__
#define __ISP_DIGITAL_INTERFACE_H__

//	Isp Path DDR Mode
#define DDR_ISP_2PAGE					0
#define DDR_ISP_3PAGE					1
#define DDR_ISP_BYP_DNR_WDR				2
#define DDR_ISP_BYPASS					3
#define DDR_ISP_5PAGE					4

//	Input Interface
#define	IF_BT1120_8BIT_SDR				0
#define	IF_BT1120_8BIT_DDR				1
#define	IF_BT1120_16BIT					2
#define	IF_BT656_SDR					3
#define	IF_BT656_DDR					4
#define	IF_YC_8BIT						5
#define	IF_YC_16BIT						6


//	Mode Set & Reset
#define	IF_MODE_RELEASE					0
#define	IF_MODE_SET						1

//	On & Off
#define	FN_OFF							0
#define	FN_ON							1

//	Data Latch Position
#define	IF_NEGEDGE						0
#define	IF_POSEDGE						1


//	Error
#define	IF_SET_OK						0x77
#define	IF_NO_MATCH_ERROR				0x18


//	Output Pin List
#define	IFO_OPIN_0TO7					0x1		//	Use pDO0 	~ 	pDO7
#define	IFO_OPIN_8TO15					0x2		//	Use pDO8 	~ 	pDO15
#define	IFO_OPIN_16TO23					0x4		//	Use pDO16 	~ 	pDO23
#define	IFO_OPIN_24TO31					0x8		//	Use pGPIO24 ~ 	pGPIO31
#define	IFO_OPIN_0TO15					0x10	//	Use pDO0 	~ 	pDO15
#define	IFO_OPIN_16TO31					0x20	//	Use pDO16 	~ 	pDO23, pGPIO24 ~ pGPIO31
#define	IFO_OPIN_0TO23					0x40	//	Use pDO0 	~ 	pDO23

//	Output Protocol
#define	IFO_BT1120_16BIT				0
#define	IFO_BT1120_8BIT_SDR				1
#define	IFO_BT1120_8BIT_DDR				2
#define	IFO_BT656_SDR					3
//#define	IFO_BT656_DDR					4
#define	IFO_LCD_MCU_16BIT				5
#define	IFO_LCD_SPI_18BIT				6
#define	IFO_LCD_RGB_24BIT				7

//	Output Sync Seletion
#define	IFO_ISP_SYNC					0
#define	IFO_RD_SYNC1					1
#define	IFO_RD_SYNC2					2
#define	IFO_RD_SYNC3					3

//	Output Resolution
#define	IFO_720_25P						0
#define	IFO_720_30P						1
#define	IFO_720_50P 					2
#define	IFO_720_60P 					3
#define	IFO_1080_25P					4
#define	IFO_1080_30P					5
#define	IFO_1080_50P					6
#define	IFO_1080_60P					7
#define	IFO_1440_25P					8
#define	IFO_1440_30P					9
#define	IFO_4K_15P						10
#define	IFO_4K_30P						11

//	Output Path For Digital Output
#define	IFO_DIG_RDCH1_PATH				1
#define	IFO_DIG_RDCH2_PATH				2
#define	IFO_DIG_RDCH3_PATH				3
#define	IFO_DIG_RDCH4_PATH				4
#define	IFO_DIG_CH0_DIRT_PATH			5
#define	IFO_DIG_CH1_DIRT_PATH			6
#define	IFO_DIG_CH2_DIRT_PATH			7
#define	IFO_DIG_CH3_DIRT_PATH			8
#define	IFO_DIG_RDCH1_FONT_PATH			9
#define	IFO_DIG_RDCH2_FONT_PATH			10
#define	IFO_DIG_RDCH3_FONT_PATH			11
#define	IFO_DIG_RDCH4_FONT_PATH			12
#define	IFO_DIG_CH0_DIRT_FONT_PATH		13
#define	IFO_DIG_CH1_DIRT_FONT_PATH		14
#define	IFO_DIG_CH2_DIRT_FONT_PATH		15
#define	IFO_DIG_CH3_DIRT_FONT_PATH		16
#define	IFO_ISP_PATH					17
#define	IFO_DIG_PIP_FONT_PATH			18
#define	IFO_DIG_MERGE_FONT_PATH			19
#define	IFO_DIG_DZOOM_FONT_PATH			20

//	Option
#define	IFO_DZOOM						1		//	Read Channel Only

//	Use Pin Mode
#define	USE_EMPTY						0
#define	USE_OCH0_16BIT					1
#define	USE_OCH1_16BIT					2
#define	USE_OCH0_8BIT					3
#define	USE_OCH1_8BIT					4


//	Internal Sync Generator Setting------------------------------------------------------------------------

#define	SYNC_STANDALONE					1
#define	SYNC_RELATION					0

//	Digital DDR Sdram R/W Channel------------------------------------------------------------------------
//	DDR Write Channel Sync
#define	WR_ISP_SYNC						0
#define	WR_DIG_CH0_SYNC					1
#define	WR_DIG_CH1_SYNC					2
#define	WR_DIG_CH2_SYNC					3
#define	WR_DIG_CH3_SYNC					4
#define	WR_DDR_BYPASS_WDR_SYNC			5
#define	WR_SYNC2						6
#define	WR_DIG_THERMAL					7

//	Internal Path  For Write Channel
#define	WR_CH_ISP_NO_FONT				0
#define	WR_CH_DIGITAL_NO_FONT_CH0		1
#define	WR_CH_DIGITAL_NO_FONT_CH1		2
#define	WR_CH_DIGITAL_NO_FONT_CH2		3
#define	WR_CH_DIGITAL_NO_FONT_CH3       4
#define	WR_CH_JPEG_NO_FONT_DECODER      5
#define	WR_CH_ISP_FONT                  6
#define	WR_CH_DIGITAL_FONT_CH0          7
#define	WR_CH_DIGITAL_FONT_CH1          8
#define	WR_CH_DIGITAL_FONT_CH2          9
#define	WR_CH_DIGITAL_FONT_CH3          10
#define	WR_CH_DOWN_SCALER0              11
#define	WR_CH_DOWN_SCALER1              12
#define	WR_CH_DOWN_SCALER2              13
#define	WR_CH_DOWN_SCALER3              14
#define	WR_CH_PIP                       15
#define	WR_CH_DZOOM                     16
#define	WR_CH_JPEG_FONT_DECODER         17
#define	WR_CH_THERMAL_Y                 18
#define	WR_CH_THERMAL_PSEUDO            19

#define	RD_CH_PIP0						0
#define	RD_CH_PIP1						1
#define	RD_CH_PIP2						2
#define	RD_CH_PIP3                      3
#define	RD_CH_ISP_SYNC					4
#define	RD_CH_ISYNC1					5
#define	RD_CH_ISYNC2					6
#define	RD_CH_ISYNC3					7
#define	RD_CH_IMG_MERGE                 8


#define	DDR_WRCH1						0
#define	DDR_WRCH2						1
#define	DDR_WRCH3						2
#define	DDR_WRCH4						3

#define	DDR_RDCH1						0
#define	DDR_RDCH2						1
#define	DDR_RDCH3						2
#define	DDR_RDCH4						3

//	RdLock
#define	DDR_WR_FIRST					0
#define	DDR_RD_FIRST					1

//	Color R/W Enable
#define	WR_COLOR						1
#define	WR_NO_COLOR						0

//	Interlace
#define	NO_INTERLACE					0
#define	INTERLACE						1

//	Frc Page
#define	FRC_3PAGE						0
#define	FRC_2PAGE						1

//	DDR Read Channel Sync
#define	RD_ISP_SYNC						0
#define	RD_DIG_CH0_SYNC					1
#define	RD_DIG_CH1_SYNC					2
#define	RD_DIG_CH2_SYNC					3
#define	RD_DIG_CH3_SYNC					4
#define	RD_SYNC1						5
#define	RD_SYNC2						6
#define	RD_SYNC3						7

//	Write Channel DDR Address
#define Isp_WrCh0_MYAdr(Adr)				IM_YADR0w(Adr)
#define Isp_WrCh0_MCAdr(Adr)				IM_CADR0w(Adr)

#define	Isp_WrCh1_MYAdr(Adr)				IM_YADR1w(Adr)
#define	Isp_WrCh1_MCAdr(Adr)				IM_CADR1w(Adr)
#define	Isp_WrCh1_YAdr(Adr0,Adr1,Adr2)		IM_YADR1_P0w(Adr0);	IM_YADR1_P1w(Adr1);	IM_YADR1_P2w(Adr2)
#define	Isp_WrCh1_CAdr(Adr0,Adr1,Adr2)		IM_CADR1_P0w(Adr0);	IM_CADR1_P1w(Adr1);	IM_CADR1_P2w(Adr2)

#define	Isp_WrCh2_MYAdr(Adr)				IM_YADR2w(Adr)
#define	Isp_WrCh2_MCAdr(Adr)				IM_CADR2w(Adr)
#define	Isp_WrCh2_YAdr(Adr0,Adr1,Adr2)		IM_YADR2_P0w(Adr0);	IM_YADR2_P1w(Adr1);	IM_YADR2_P2w(Adr2)
#define	Isp_WrCh2_CAdr(Adr0,Adr1,Adr2)		IM_CADR2_P0w(Adr0);	IM_CADR2_P1w(Adr1);	IM_CADR2_P2w(Adr2)

#define	Isp_WrCh3_MYAdr(Adr)				IM_YADR3w(Adr)
#define	Isp_WrCh3_MCAdr(Adr)				IM_CADR3w(Adr)
#define	Isp_WrCh3_YAdr(Adr0,Adr1,Adr2)		IM_YADR3_P0w(Adr0);	IM_YADR3_P1w(Adr1);	IM_YADR3_P2w(Adr2)
#define	Isp_WrCh3_CAdr(Adr0,Adr1,Adr2)		IM_CADR3_P0w(Adr0);	IM_CADR3_P1w(Adr1);	IM_CADR3_P2w(Adr2)

#define	Isp_WrCh4_MYAdr(Adr)				IM_YADR4w(Adr)
#define	Isp_WrCh4_MCAdr(Adr)				IM_CADR4w(Adr)
#define	Isp_WrCh4_YAdr(Adr0,Adr1,Adr2)		IM_YADR4_P0w(Adr0);	IM_YADR4_P1w(Adr1);	IM_YADR4_P2w(Adr2)
#define	Isp_WrCh4_CAdr(Adr0,Adr1,Adr2)		IM_CADR4_P0w(Adr0);	IM_CADR4_P1w(Adr1);	IM_CADR4_P2w(Adr2)

//	Write Mode
#define	WR_WRITE_ONE_MODE		0
#define	WR_WRITE_CONTINU_MODE	1

//	Read Channel DDR Address






//	Dowm Scaler Input Channel------------------------------------------------------------------------
//	Down Scaler On/Off
#define	DS_ON						1
#define	DS_OFF						0
//	Down Scaler Input Path
#define	DS_ISP_FONT_PATH			0
#define	DS_ISP_NO_FONT_PATH			1
#define	DS_DIGITAL_CH0_PATH			2
#define	DS_DIGITAL_CH1_PATH			3
#define	DS_DIGITAL_CH2_PATH			4
#define	DS_DIGITAL_CH3_PATH			5
#define	DS_DIGITAL_CH0_FONT_PATH	6
#define	DS_DIGITAL_CH1_FONT_PATH	7
#define	DS_DIGITAL_CH2_FONT_PATH	8
#define	DS_DIGITAL_CH3_FONT_PATH	9
#define	DS_COLOR_BAR_PATH			10
#define	DS_DDR_RD_CH1_PATH			11		//	Special Path
#define	DS_DDR_RD_CH2_PATH			12      //	Special Path
#define	DS_DDR_RD_CH3_PATH			13      //	Special Path
#define	DS_DDR_RD_CH4_PATH			14      //	Special Path
#define	DS_DDR_RD_DZOOM_PATH		15      //	Special Path
#define DS_JPEG_DEC_FONT_PATH		16      //	Special Path
#define	DS_JPEG_DEC_RD_PATH			17      //	Special Path
#define	DS_THERNAL_Y_PATH			18		//	Special Path

//	Down Scaler Lpf Coefficient
#define	LPF_LV1						0
#define	LPF_LV2						1
#define	LPF_LV3						2

#define	DS_APT_ROI_OFF				0
#define	DS_APT_ROI_ON				1

#define	DS_ROI_OSD_OFF				0
#define	DS_ROI_OSD_ON				1

//	Digital Dzoom-------------------------------------------------------------------------------------
#define	DZ_RD_CH0					0
#define	DZ_RD_CH1					1
#define	DZ_RD_CH2					2
#define	DZ_RD_CH3					3
#define	DZ_RD_CH4					4

#define	DZ_ISP_SYNC					0
#define	DZ_ISYNC_GEN1				1
#define	DZ_ISYNC_GEN2				2
#define	DZ_ISYNC_GEN3				3
#define	DZ_PIP_CH0					4
#define	DZ_PIP_CH1					5
#define	DZ_PIP_CH2					6
#define	DZ_PIP_CH3					7

#define	DZ_MANUAL_ALGN				0
#define	DZ_AUTO_ALGN				1

#define	DZ_DKX_MANUAL				0
#define	DZ_DKX_AUTO					1

//	PIP Channel----------------------------------------------------------------------------------------
//	Main Background Image
#define	PIP_MAIN_ISP				0
#define	PIP_MAIN_DIG_CH0            1
#define	PIP_MAIN_DIG_CH1            2
#define	PIP_MAIN_DIG_CH2            3
#define	PIP_MAIN_DIG_CH3            4
#define	PIP_MAIN_DDR_RD_CH0         5
#define	PIP_MAIN_DDR_RD_CH1         6
#define	PIP_MAIN_DDR_RD_CH2         7
#define	PIP_MAIN_DDR_RD_CH3         8
#define	PIP_MAIN_DDR_RD_CH4         9

//	PIP Image Source Channel
#define	PIP_DDR_RD_CH0				0
#define	PIP_DDR_RD_CH1				1
#define	PIP_DDR_RD_CH2				2
#define	PIP_DDR_RD_CH3				3
#define	PIP_DDR_RD_CH4				4
#define	PIP_DDR_RD_DZOOM			5

//	PIP Read Sync
#define	PIP_ISP_SYNC				0
#define	PIP_DIG_CH0_SYNC			1
#define	PIP_DIG_CH1_SYNC			2
#define	PIP_DIG_CH2_SYNC			3
#define	PIP_DIG_CH3_SYNC			4
#define	PIP_ISYNC_GEN1				5
#define	PIP_ISYNC_GEN2				6
#define	PIP_ISYNC_GEN3				7

//	PIP Clock
#define	PIP_ISP_297M				0
#define	PIP_ISP_148M				1
#define	PIP_ISP_74M					2
#define	PIP_DIG_CH0					3
#define	PIP_DIG_CH1					4
#define	PIP_DIG_CH2					5
#define	PIP_DIG_CH3					6
#define	PIP_DIG_CH0_DIV2			7
#define	PIP_DIG_CH1_DIV2			8
#define	PIP_DIG_CH2_DIV2			9
#define	PIP_DIG_CH3_DIV2			10
#define	PIP_PCK_DIV0                11
#define	PIP_PCK_DIV2                12

//	PIP Line Osd
#define	PIP_LINE_OFF				0
#define	PIP_LINE_ON					1

//	PIP Blending Ratio
#define	PIP_MIX_100					0
#define	PIP_MIX_75                  1
#define	PIP_MIX_50                  2
#define	PIP_MIX_25                  3

#define	PIP_NO_MMASK				0
#define	PIP_MMASK					1

//	Color
#define	LINE_BLACK					0x108080
#define	LINE_WHITE					0xF08080
#define	LINE_YELLOW					0xA82C87
#define	LINE_RED					0x336CD4
#define	LINE_GRAY					0xB48080
#define	LINE_CYAN					0x91932C
#define	LINE_BLUE					0x1BD478
#define	LINE_MAGENTA				0x3EC0CC
#define	LINE_GREEN					0x853F33

//	Imge Merge
#define	MRG_IMG2					2
#define	MRG_IMG3					3
#define	MRG_IMG4					4

//	VLC
#define	VLC_C_ADR					0x350
#define	VLC_Y_ADR					0x358

#define	NO_VLCBIT					3
#define	VLC_4BIT					2
#define	VLC_5BIT					1
#define	VLC_6BIT					0


//	CVBS-------------------------------------------------------------------------------

#define	CVBS_ISP_ADR	0x2C0

#define	NTSC			0
#define	PAL				1

#define	FREQ_27M		0
#define	FREQ_36M		1

#endif /* __ISP_DIGITAL_INTERFACE_H__ */
