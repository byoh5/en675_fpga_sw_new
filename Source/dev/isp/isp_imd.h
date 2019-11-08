/* **************************************************************************
 File Name	: 	isp_imd.h
 Description:	EN675 - Motion detector
 Designer	:	Kim, Sunghoon
 Date		:	19. 7. 24
 Copyright �� Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#ifndef _ISP_IMD_H_
#define	_ISP_IMD_H_

#ifndef __ISP_LIB__
	#define IMD_64B_ON		0//model_8M		// IMD MAP�� 1 line �� 32*3 bit �̹Ƿ�, �ִ� 32*3*32=3072pixel ���� ����, 8M�� ��� 3840pixel�̹Ƿ� IMD_64B�� 1�� �����ؾ� �ϸ�, IMD_TH���� 2��� �����ؾ� ��		TODO KSH �� IMD_64B�� 1�϶� �����Ӿ��� ������ �����ӹ߻� ����

	#define IMD_HW_B		(5+IMD_64B_ON)
	#define IMD_VW_B		(5)				// 8M�� ��� 2160/32=67.5 �̹Ƿ� 0x600�������� 0x600+(67-1)*3=0x6C6�������� IMD MAP�� ��µǾ�� ��(LOGIC �ڵ�δ� 0x6DF���� ����)		TODO KSH �� IMD MAP�� 8M���� 0x6C6���� ��µǴ��� Ȯ���ʿ�

	#define IMD_HW			(RP(FR_HW)>>IMD_HW_B)
	#define IMD_VW			(RP(FR_VW)>>IMD_VW_B)

	#define IMD_DET_EA		50				// 2M �ػ󵵿��� �׽�Ʈ �� �ִ� 35�� �������� �����, IMD_BOX_EA*2 �̻��̾�� ��, 1M�ػ��� IMD_IMGũ��� 880 byte �̹Ƿ� Masking & Combine ó���� ���� (880/4) >= IMD_DET_EA �� 220 ���Ͽ��� ��

	#define IMD_BOX_EA		16				// 16 �ʰ� ���� �� gnBoxToneIMD �߰��ؾ� ��
	#define IMD_OUT_EA		(IMD_BOX_EA-2)	// number of motion box, IMD_BOX_EA���� mask �ּҰ��� 1���� background 1���� ������ ����

	#define IMD_SENS_MAX	10

	#define IMD2BOX_X		(4)				// IMD ODM�� BOX OSD�� ��� ȭ�鿡�� ��ġ�ϴ� ������ -16�� �Ѵ�.
	#define IMD2BOX_Y		(10)

	#define IMD_TEXT_X		(MN_MXSP+1)
	#define IMD_TEXT_Y		14
#endif

#define wRectIMD(INDEX)	((wRECT*)(gnBoxPosIMD + (((INDEX)  )<<1)))			// INDEX=1~11 -> Rect=0~10
#define bRectIMD(INDEX)	((bRECT*)(gnBoxPosIMD + (((INDEX)  )   )))			// (LIB) INDEX=1~11 -> Rect=0~10

#define bRectOri(INDEX)	((bRECT*)(pnIMD_IMG   + (((INDEX)  )   )))			// (LIB) INDEX=1~11 -> Rect=0~10

#define bRectMsk(INDEX)	((bRECT*)(gnBoxMskPos + (((INDEX)  )   )))			// (LIB) INDEX=0~3 -> Rect=0~3

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
