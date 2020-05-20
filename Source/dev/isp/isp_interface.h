#ifndef __ISP_INTERFACE_H__
#define __ISP_INTERFACE_H__


//#define	ON				1
//#define	OFF				0


//ISP Master & Slave------------------------------------------------------------------
#define	ISP_MASTER		0
#define	ISP_SLAVE		1

//Parallel input Bit------------------------------------------------------------------
#define	BIT10			0
#define	BIT12TOBit12	1
#define	BIT8			2

//	Parallel Interface----------------------------------------------------------------
#define	PARA_EXTER_CLK	0
#define	PARA_INTER_CLK	1

#define	PARA_CLK_SDR	0
#define	PARA_CLK_DDR	1

#define	SYNC_FALL		0
#define	SYNC_RISE       1

#define	SAME_EDGE 		0
#define	DIFF_EDGE 		1

#define	NO_USE_AUTOSYNC 0
#define	USE_AUTOSYNC 	1

//	MIPI------------------------------------------------------------------------
#define	MIPI_10BIT		10
#define	MIPI_12BIT		12

#define	MIPI_1LANE		0
#define	MIPI_2LANE		1
#define	MIPI_4LANE		2

#define	NO_USE_CHECK	0
#define	USE_ECC_CHECK	1
#define	USE_WCL_CHECK	1
#define	USE_WCF_CHECK	1
#define	USE_WCFE_CHECK	1

//	Resolution-------------------------------------------------------------------------
#define	RES_720_25P			0
#define	RES_720_30P			1
#define	RES_720_50P			2
#define	RES_720_60P			3
#define	RES_1080_25P		4
#define	RES_1080_30P		5
#define	RES_1080_50P		6
#define	RES_1080_60P		7
#define	RES_4M_25P			8
#define	RES_4M_30P			9
#define	RES_4K_15P			10

//	Defect & 2D DNR--------------------------------------------------------------------
#define	DF_SUM_4		0
#define	DF_SUM_6		1

#define	DF_WGT_CASEA	0
#define	DF_WGT_CASEB	1
#define	DF_WGT_CASEC	2

#define	DF_GTHRES		0xce7
#define	DF_RBTHRES		0xd39

#define	DF_MAX			0x1fff
#define	DF_MIN			0x1000

#define	DF_SLOPE_NOR	8

#define	DNR2D_CNT8		8
#define	DNR2D_CNT9		9
#define	DNR2D_CNT10		10
#define	DNR2D_CNT11		11
#define	DNR2D_CNT12		12					//	Max 12

#define	DNR2D_DTH_MOD	0
#define	DNR2D_SUM_MOD	1

#endif /* __ISP_INTERFACE_H__ */
