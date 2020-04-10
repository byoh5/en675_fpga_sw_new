#ifndef __ISP_CLK_H__
#define __ISP_CLK_H__

//	Clock Define
#define		ISP_74M			2
#define		ISP_148M		1
#define		ISP_297M		0

//	MIPI Clock----------------------------------------------------- isp_interface.c
#define MIPI_297M			0
#define MIPI_148M			1
#define MIPI_74M			2
#define MIPI_EXT			3

#define M_297M				MIPI_297M
#define M_148M				MIPI_148M
#define M_74M				MIPI_74M
#define M_EXT				MIPI_EXT

//	Sensor Clock--------------------------------------------------- isp_interface.c
#define	SENS_148M			0
#define	SENS_74M			1
#define	SENS_37M			2
#define	SENS_18M			3
#define	SENS_27M			4
#define	SENS_13M			5
#define	SENS_6M				6

//	Isp Pre/Post Clock---------------------------------------------	isp_interface.c
#define	ISP_CLK_PLL			0
#define	ISP_CLK_148M		1
#define	ISP_CLK_74M			2
#define	ISP_CLK_PCLK		3
#define	ISP_CLK_PCLK_DIV2	4
#define	ISP_CLK_PLL_DIV0	5

#define C_PLL	ISP_CLK_PLL
#define C_148M	ISP_CLK_148M
#define C_74M	ISP_CLK_74M
#define C_PCK	ISP_CLK_PCLK
#define C_PCKD2	ISP_CLK_PCLK_DIV2
#define C_PLLD0	ISP_CLK_PLL_DIV0

//	Down-Scale Clock-----------------------------------------------	isp_interface.c
#define	DS_ISP_297M                 1
#define	DS_ISP_148M                 1
#define	DS_ISP_74M					2
#define	DS_DIG_CH0                  3
#define	DS_DIG_CH1                  4
#define	DS_DIG_CH2                  5
#define	DS_DIG_CH3                  6
#define	DS_DIG_CH0_DIV2             7
#define	DS_DIG_CH1_DIV2             8
#define	DS_DIG_CH2_DIV2             9
#define	DS_DIG_CH3_DIV2             10
#define	DS_PCK_DIV0                 11

//	Internal Clock------------------------------------------------	isp_interface.c
#define	CLK_297M						0
#define	CLK_148M               			1
#define	CLK_74M                			2
#define	CLK_DIG_CH0            			3
#define	CLK_DIG_CH1            			4
#define	CLK_DIG_CH2            			5
#define	CLK_DIG_CH3            			6
#define	CLK_DIG_CH0_DIV2       			7
#define	CLK_DIG_CH1_DIV2       			8
#define	CLK_DIG_CH2_DIV2       			9
#define	CLK_DIG_CH3_DIV2       			10
#define	CLK_PLL_DIV0           			11
#define	CLK_PLL_DIV1           			12
#define	CLK_PLL_DIV2           			13
#define	CLK_DDR_RATE0          			14
#define	CLK_DDR_RATE1          			15



#define	CLK_NORMAL						0
#define	CLK_INVERSE						1

//	Frequency
#define	FRQ_297M						0
#define	FRQ_148M						1
#define	FRQ_74M							2

//	Pixel Clock
#define		IBT_PCK_SET(Sel)		IBT_PCK_PDw(0);		IBT_PCK_SELw(Sel);	IBT_PCK_PDw(1)
#define		IBT0_PCK_SET(Sel)		IBT0_PCK_PDw(0);	IBT0_PCK_SELw(Sel);	IBT0_PCK_PDw(1)
#define		IBT1_PCK_SET(Sel)		IBT1_PCK_PDw(0);	IBT1_PCK_SELw(Sel);	IBT1_PCK_PDw(1)
#define		IBT2_PCK_SET(Sel)		IBT2_PCK_PDw(0);	IBT2_PCK_SELw(Sel);	IBT2_PCK_PDw(1)

#define		BT_PCK_SET(Sel)			BT_PCK_PDw(0);		BT_PCK_SELw(Sel);	BT_PCK_PDw(1)
#define		BT0_PCK_SET(Sel)		BT0_PCK_PDw(0);		BT0_PCK_SELw(Sel);	BT0_PCK_PDw(1)
#define		BT1_PCK_SET(Sel)		BT1_PCK_PDw(0);		BT1_PCK_SELw(Sel);	BT1_PCK_PDw(1)
#define		BT2_PCK_SET(Sel)		BT2_PCK_PDw(0);		BT2_PCK_SELw(Sel);	BT2_PCK_PDw(1)

//	Sync Gen
#define		RYCCK1_SET(Sel)		RYCCK1_PDw(0);	RYCCK1_SELw(Sel);	RYCCK1_PDw(1)
#define		RYCCK2_SET(Sel)		RYCCK2_PDw(0);	RYCCK2_SELw(Sel);	RYCCK2_PDw(1)
#define		RYCCK3_SET(Sel)		RYCCK3_PDw(0);	RYCCK3_SELw(Sel);	RYCCK3_PDw(1)

//	Output Interface
#define		ITCK0_SET(Sel)		ITCK0_PDw(0);	ITCK0_SELw(Sel);	ITCK0_PDw(1)
#define		ITCK1_SET(Sel)		ITCK1_PDw(0);	ITCK1_SELw(Sel);	ITCK1_PDw(1)
#define		DITCK0_SET(Sel)		DITCK0_PDw(0);	DITCK0_SELw(Sel);	DITCK0_PDw(1)
#define		DITCK1_SET(Sel)		DITCK1_PDw(0);	DITCK1_SELw(Sel);	DITCK1_PDw(1)
#define		OTCK0_SET(Sel)		OTCK0_PDw(0);	OTCK0_SELw(Sel);	OTCK0_PDw(1)
#define		OTCK1_SET(Sel)		OTCK1_PDw(0);	OTCK1_SELw(Sel);	OTCK1_PDw(1)
#define		DO0_CK_SET(Sel)		DO0_CK_PDw(0);	DO0_CK_SELw(Sel);	DO0_CK_PDw(1)
#define		DO1_CK_SET(Sel)		DO1_CK_PDw(0);	DO1_CK_SELw(Sel);	DO1_CK_PDw(1)
#define		DO2_CK_SET(Sel)		DO2_CK_PDw(0);	DO2_CK_SELw(Sel);	DO2_CK_PDw(1)
#define		DO3_CK_SET(Sel)		DO3_CK_PDw(0);	DO3_CK_SELw(Sel);	DO3_CK_PDw(1)

//	Font & Box
#define		FN_CK0_SET(Sel)		FN_CK0_PDw(0);	FN_CK0_SELw(Sel);	FN_CK0_PDw(1)
#define		PR_CK0_SET(Sel)		PR_CK0_PDw(0);	PR_CK0_SELw(Sel);	PR_CK0_PDw(1)

//	Write Channel
#define		YCW_CK0_SET(Sel)	YCW_CK0_PDw(0);	YCW_CK0_SELw(Sel);	YCW_CK0_PDw(1)
#define		YCW_CK1_SET(Sel)	YCW_CK1_PDw(0);	YCW_CK1_SELw(Sel);	YCW_CK1_PDw(1)
#define		YCW_CK2_SET(Sel)	YCW_CK2_PDw(0);	YCW_CK2_SELw(Sel);	YCW_CK2_PDw(1)
#define		YCW_CK3_SET(Sel)	YCW_CK3_PDw(0);	YCW_CK3_SELw(Sel);	YCW_CK3_PDw(1)
#define		YCW_CK4_SET(Sel)	YCW_CK4_PDw(0);	YCW_CK4_SELw(Sel);	YCW_CK4_PDw(1)

//	Read Channel
#define		YCR_CK0_SET(Sel)	YCR_CK0_PDw(0);	YCRCK0_SELw(Sel);	YCR_CK0_PDw(1)
#define		YCR_CK1_SET(Sel)	YCR_CK1_PDw(0);	YCRCK1_SELw(Sel);	YCR_CK1_PDw(1)
#define		YCR_CK2_SET(Sel)	YCR_CK2_PDw(0);	YCRCK2_SELw(Sel);	YCR_CK2_PDw(1)
#define		YCR_CK3_SET(Sel)	YCR_CK3_PDw(0);	YCRCK3_SELw(Sel);	YCR_CK3_PDw(1)
#define		YCR_CK4_SET(Sel)	YCR_CK4_PDw(0);	YCRCK4_SELw(Sel);	YCR_CK4_PDw(1)

//	Down-Scale
#define		CDS0_PCK_SET(Sel)	CDS0_PCK_PDw(0);	CDS0_PCK_SELw(Sel);	CDS0_PCK_PDw(1)
#define		CDS1_PCK_SET(Sel)	CDS1_PCK_PDw(0);	CDS1_PCK_SELw(Sel);	CDS1_PCK_PDw(1)
#define		CDS2_PCK_SET(Sel)	CDS2_PCK_PDw(0);	CDS2_PCK_SELw(Sel);	CDS2_PCK_PDw(1)
#define		CDS3_PCK_SET(Sel)	CDS3_PCK_PDw(0);	CDS3_PCK_SELw(Sel);	CDS3_PCK_PDw(1)

//	Dzoom Clock
#define		DZPCK_SET(Sel)		DZPCK_PDw(0);	DZPCK_SELw(Sel);	DZPCK_PDw(1)

//	PIP Clock
#define		PIP_PCK_SET(Sel)	PIP_PPCK_PDw(0);	PIP_PPCK_SELw(Sel);		PIP_PPCK_PDw(1)

//	Image Merge Clock
#define		IMGM_CK_SET(Sel)	IMGM_CK_PDw(0);		IMGM_CK_SELw(Sel);		IMGM_CK_PDw(1)

#endif /* __ISP_CLK_H__ */
