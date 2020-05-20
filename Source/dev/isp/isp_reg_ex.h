/* **************************************************************************
 File Name  :   isp_reg_ex.h
 Description:   EN675 - ISP Register Macro
 Designer   :   Kim, Sunghoon
 Date       :   19. 7. 3
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#ifndef __ISP_REG_EX_H__
#define __ISP_REG_EX_H__


#define _REG_BASE_			REG_BASE_ISP

//******************************************************************************
// ISP BASE Setting
//------------------------------------------------------------------------------
#define	_rd32(a)			((a - _REG_BASE_)>>2)

#define BitMask_01			0xffffffff
#define BitMask_02			0xffffffff
#define BitMask_03			0xffffffff
#define BitMask_04			0xffffffff
#define BitMask_05			0xffffffff
#define BitMask_06			0xffffffff
#define BitMask_07			0xffffffff
#define BitMask_08			0xffffffff
#define BitMask_09			0xffffffff
#define BitMask_10			0xffffffff
#define BitMask_11			0xffffffff
#define BitMask_12			0xffffffff
#define BitMask_13			0xffffffff
#define BitMask_14			0xffffffff
#define BitMask_15			0xffffffff
#define BitMask_16			0xffffffff
#define BitMask_17			0xffffffff
#define BitMask_18			0xffffffff
#define BitMask_19			0xffffffff
#define BitMask_20			0xffffffff
#define BitMask_21			0xffffffff
#define BitMask_22			0xffffffff
#define BitMask_23			0xffffffff
#define BitMask_24			0xffffffff
#define BitMask_25			0xffffffff
#define BitMask_26			0xffffffff
#define BitMask_27			0xffffffff
#define BitMask_28			0xffffffff
#define BitMask_29			0xffffffff
#define BitMask_30			0xffffffff
#define BitMask_31			0xffffffff
#define BitMask_32			0xffffffff

#define ValSft_R00(val)		(val)
#define ValSft_R01(val)		(val)
#define ValSft_R02(val)		(val)
#define ValSft_R03(val)		(val)
#define ValSft_R04(val)		(val)
#define ValSft_R05(val)		(val)
#define ValSft_R06(val)		(val)
#define ValSft_R07(val)		(val)
#define ValSft_R08(val)		(val)
#define ValSft_R09(val)		(val)
#define ValSft_R10(val)		(val)
#define ValSft_R11(val)		(val)
#define ValSft_R12(val)		(val)
#define ValSft_R13(val)		(val)
#define ValSft_R14(val)		(val)
#define ValSft_R15(val)		(val)
#define ValSft_R16(val)		(val)
#define ValSft_R17(val)		(val)
#define ValSft_R18(val)		(val)
#define ValSft_R19(val)		(val)
#define ValSft_R20(val)		(val)
#define ValSft_R21(val)		(val)
#define ValSft_R22(val)		(val)
#define ValSft_R23(val)		(val)
#define ValSft_R24(val)		(val)
#define ValSft_R25(val)		(val)
#define ValSft_R26(val)		(val)
#define ValSft_R27(val)		(val)
#define ValSft_R28(val)		(val)
#define ValSft_R29(val)		(val)
#define ValSft_R30(val)		(val)
#define ValSft_R31(val)		(val)

#include "isp_reg.h"


// TODO KSH ◆ ISP BASE Setting
enum { YGAMMA_BASE	= YGMY_00r		};
enum { CGAMMA_BASE	= CGMY_00r		};

enum { AWBL_CM_BASE	= AWBL_CM00_HSPr};

enum { DNR_Q_BASE	= DNR_Q00r		};

enum { BOXTON_BASE	= BTONE31r		};
enum { BOXFIL_BASE	= BFL_ON31r		};
enum { BOXPOS_BASE	= B0_VSPr		};
enum { BOXPLT_BASE	= BOX0_PLTr		};
enum { BOXFMT_BASE	= BITBOX0r		};
enum { BBOXFMT_BASE	= BIT_CB0r		};

enum { IMDAREA_BASE	= IMD_WON1r		};

enum { TMG_BASE		= TMG_TBL00r	};


//******************************************************************************
// ISP Register Macro Setting
//------------------------------------------------------------------------------
#undef _rd32

#undef BitMask_01
#undef BitMask_02
#undef BitMask_03
#undef BitMask_04
#undef BitMask_05
#undef BitMask_06
#undef BitMask_07
#undef BitMask_08
#undef BitMask_09
#undef BitMask_10
#undef BitMask_11
#undef BitMask_12
#undef BitMask_13
#undef BitMask_14
#undef BitMask_15
#undef BitMask_16
#undef BitMask_17
#undef BitMask_18
#undef BitMask_19
#undef BitMask_20
#undef BitMask_21
#undef BitMask_22
#undef BitMask_23
#undef BitMask_24
#undef BitMask_25
#undef BitMask_26
#undef BitMask_27
#undef BitMask_28
#undef BitMask_29
#undef BitMask_30
#undef BitMask_31
#undef BitMask_32

#undef ValSft_R00
#undef ValSft_R01
#undef ValSft_R02
#undef ValSft_R03
#undef ValSft_R04
#undef ValSft_R05
#undef ValSft_R06
#undef ValSft_R07
#undef ValSft_R08
#undef ValSft_R09
#undef ValSft_R10
#undef ValSft_R11
#undef ValSft_R12
#undef ValSft_R13
#undef ValSft_R14
#undef ValSft_R15
#undef ValSft_R16
#undef ValSft_R17
#undef ValSft_R18
#undef ValSft_R19
#undef ValSft_R20
#undef ValSft_R21
#undef ValSft_R22
#undef ValSft_R23
#undef ValSft_R24
#undef ValSft_R25
#undef ValSft_R26
#undef ValSft_R27
#undef ValSft_R28
#undef ValSft_R29
#undef ValSft_R30
#undef ValSft_R31


#define	_wr32(a,d)				(*((volatile UINT *)(unsigned long)(a)) = (d))			//			"			- Always used for ISP Register Write
#define	_rd32(a)				(*((volatile UINT *)(unsigned long)(a)))				//			"			- Always used for ISP Register Read

#define	SetIsp(anAddr, anData)	(_wr32(REG_BASE_ISP+((UINT)(anAddr)<<2), (UINT)(anData)))	// Write to bridge(Isp,Aud)
#define	GetIsp(anAddr)			(_rd32(REG_BASE_ISP+((UINT)(anAddr)<<2))                )	// Read from bridge(Isp,Aud)

#define BitMask_01			0x1
#define BitMask_02			0x3
#define BitMask_03			0x7
#define BitMask_04			0xf
#define BitMask_05			0x1f
#define BitMask_06			0x3f
#define BitMask_07			0x7f
#define BitMask_08			0xff
#define BitMask_09			0x1ff
#define BitMask_10			0x3ff
#define BitMask_11			0x7ff
#define BitMask_12			0xfff
#define BitMask_13			0x1fff
#define BitMask_14			0x3fff
#define BitMask_15			0x7fff
#define BitMask_16			0xffff
#define BitMask_17			0x1ffff
#define BitMask_18			0x3ffff
#define BitMask_19			0x7ffff
#define BitMask_20			0xfffff
#define BitMask_21			0x1fffff
#define BitMask_22			0x3fffff
#define BitMask_23			0x7fffff
#define BitMask_24			0xffffff
#define BitMask_25			0x1ffffff
#define BitMask_26			0x3ffffff
#define BitMask_27			0x7ffffff
#define BitMask_28			0xfffffff
#define BitMask_29			0x1fffffff
#define BitMask_30			0x3fffffff
#define BitMask_31			0x7fffffff
#define BitMask_32			0xffffffff

#define ValSft_R00(val)		((val)>> 0)
#define ValSft_R01(val)		((val)>> 1)
#define ValSft_R02(val)		((val)>> 2)
#define ValSft_R03(val)		((val)>> 3)
#define ValSft_R04(val)		((val)>> 4)
#define ValSft_R05(val)		((val)>> 5)
#define ValSft_R06(val)		((val)>> 6)
#define ValSft_R07(val)		((val)>> 7)
#define ValSft_R08(val)		((val)>> 8)
#define ValSft_R09(val)		((val)>> 9)
#define ValSft_R10(val)		((val)>>10)
#define ValSft_R11(val)		((val)>>11)
#define ValSft_R12(val)		((val)>>12)
#define ValSft_R13(val)		((val)>>13)
#define ValSft_R14(val)		((val)>>14)
#define ValSft_R15(val)		((val)>>15)
#define ValSft_R16(val)		((val)>>16)
#define ValSft_R17(val)		((val)>>17)
#define ValSft_R18(val)		((val)>>18)
#define ValSft_R19(val)		((val)>>19)
#define ValSft_R20(val)		((val)>>20)
#define ValSft_R21(val)		((val)>>21)
#define ValSft_R22(val)		((val)>>22)
#define ValSft_R23(val)		((val)>>23)
#define ValSft_R24(val)		((val)>>24)
#define ValSft_R25(val)		((val)>>25)
#define ValSft_R26(val)		((val)>>26)
#define ValSft_R27(val)		((val)>>27)
#define ValSft_R28(val)		((val)>>28)
#define ValSft_R29(val)		((val)>>29)
#define ValSft_R30(val)		((val)>>30)
#define ValSft_R31(val)		((val)>>31)

#define ValSft_L00(val)		((val)<< 0)
#define ValSft_L01(val)		((val)<< 1)
#define ValSft_L02(val)		((val)<< 2)
#define ValSft_L03(val)		((val)<< 3)
#define ValSft_L04(val)		((val)<< 4)
#define ValSft_L05(val)		((val)<< 5)
#define ValSft_L06(val)		((val)<< 6)
#define ValSft_L07(val)		((val)<< 7)
#define ValSft_L08(val)		((val)<< 8)
#define ValSft_L09(val)		((val)<< 9)
#define ValSft_L10(val)		((val)<<10)
#define ValSft_L11(val)		((val)<<11)
#define ValSft_L12(val)		((val)<<12)
#define ValSft_L13(val)		((val)<<13)
#define ValSft_L14(val)		((val)<<14)
#define ValSft_L15(val)		((val)<<15)
#define ValSft_L16(val)		((val)<<16)
#define ValSft_L17(val)		((val)<<17)
#define ValSft_L18(val)		((val)<<18)
#define ValSft_L19(val)		((val)<<19)
#define ValSft_L20(val)		((val)<<20)
#define ValSft_L21(val)		((val)<<21)
#define ValSft_L22(val)		((val)<<22)
#define ValSft_L23(val)		((val)<<23)
#define ValSft_L24(val)		((val)<<24)
#define ValSft_L25(val)		((val)<<25)
#define ValSft_L26(val)		((val)<<26)
#define ValSft_L27(val)		((val)<<27)
#define ValSft_L28(val)		((val)<<28)
#define ValSft_L29(val)		((val)<<29)
#define ValSft_L30(val)		((val)<<30)
#define ValSft_L31(val)		((val)<<31)

#include "isp_reg.h"


#endif//__ISP_REG_EX_H__
