/* **************************************************************************
 File Name	: 	isp_imd.h
 Description:	EN675 - Motion detector
 Designer	:	Kim, Sunghoon
 Date		:	19. 7. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#ifndef _ISP_IMD_H_
#define	_ISP_IMD_H_

#ifndef __ISP_LIB__
	#define IMD_64B_ON		0//model_8M		// IMD MAP의 1 line 당 32*3 bit 이므로, 최대 32*3*32=3072pixel 까지 가능, 8M의 경우 3840pixel이므로 IMD_64B를 1로 설정해야 하며, IMD_TH값을 2배로 적용해야 함		TODO KSH ◆ IMD_64B가 1일때 움직임없는 곳에서 움직임발생 문제

	#define IMD_HW_B		(5+IMD_64B_ON)
	#define IMD_VW_B		(5)				// 8M의 경우 2160/32=67.5 이므로 0x600번지부터 0x600+(67-1)*3=0x6C6번지까지 IMD MAP이 출력되어야 함(LOGIC 코드로는 0x6DF까지 가능)		TODO KSH ◆ IMD MAP이 8M에서 0x6C6까지 출력되는지 확인필요

	#define IMD_HW			(RP(FR_HW)>>IMD_HW_B)
	#define IMD_VW			(RP(FR_VW)>>IMD_VW_B)

	#define IMD_DET_EA		50				// 2M 해상도에서 테스트 시 최대 35개 정도까지 검출됨, IMD_BOX_EA*2 이상이어야 함, 1M해상도의 IMD_IMG크기는 880 byte 이므로 Masking & Combine 처리를 위해 (880/4) >= IMD_DET_EA 즉 220 이하여야 함

	#define IMD_BOX_EA		16				// 16 초과 설정 시 gnBoxToneIMD 추가해야 함
	#define IMD_OUT_EA		(IMD_BOX_EA-2)	// number of motion box, IMD_BOX_EA에서 mask 최소개수 1개와 background 1개를 제외한 개수

	#define IMD_SENS_MAX	10

	#define IMD2BOX_X		(4)				// IMD ODM과 BOX OSD가 출력 화면에서 일치하는 값에서 -16을 한다.
	#define IMD2BOX_Y		(10)

	#define IMD_TEXT_X		(MN_MXSP+1)
	#define IMD_TEXT_Y		14
#endif

#define wRectIMD(INDEX)	((wRECT*)(gnBoxPosIMD + (((INDEX)  )<<1)))
#define bRectIMD(INDEX)	((bRECT*)(gnBoxPosIMD + (((INDEX)  )   )))

#define bRectOri(INDEX)	((bRECT*)(pnIMD_IMG   + (((INDEX)  )   )))

#define bRectMsk(INDEX)	((bRECT*)(gnBoxMskPos + (((INDEX)  )   )))

extern UINT gnBoxPosIMD[];
extern UINT gnBoxOnIMD;
extern UINT gnBoxNumIMD;
extern UINT gnBoxMskNum;
extern UINT gnBoxMskSel;
extern UINT gnBoxFillIMD;
extern UINT gnBoxToneIMD;

extern void CamMovTh(const int);
extern void BrtChgTh(const int, const int, const int, const int);
extern void MaskIMD(void);
extern void InitIMD(void);
extern void LibIMD(const int aIMD_HW, const int aIMD_VW, const int aIMD_DET_EA, const int aIMD_FPS);
extern void IMD(void);


#define IMD_TEST	0	// TODO KSH + IMD Grp for test (LIB)
#if IMD_TEST
	#define	GrpIMD2(ID,VAL,...)		{ __VA_ARGS__  gnTxGrp[ID].m_UINT = VAL; }
	#define	GrpIMD(ID,VAL,...)		//{ __VA_ARGS__  gnTxGrp[ID].m_UINT = VAL; }
	#define TxGrpIMD()				UartTxGrp()
#else
	#define	GrpIMD2(ID,VAL,...)
	#define	GrpIMD(ID,VAL,...)
	#define TxGrpIMD()
#endif




#endif//_ISP_IMD_H_
