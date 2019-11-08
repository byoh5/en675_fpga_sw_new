/* **************************************************************************
 File Name	: 	isp_imd.c
 Description:	EN675 - Motion detector
 Designer	:	Kim, Sunghoon
 Date		:	19. 7. 24
 Copyright �� Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"

#define IMD_FUNC_FPS		FPS_VDI

UINT 	gnBoxPosIMD[IMD_DET_EA];
UINT 	gnBoxOnIMD = 0;
UINT	gnBoxNumIMD = 0;
UINT	gnBoxMskNum = 0;
UINT	gnBoxMskSel = 0;
UINT	gnBoxFillIMD = 0;
UINT	gnBoxToneIMD = 0;

UINT	gnBoxMskPos[MASK_EA];
UINT	gnBoxMskPosNum = 0;
BYTE	gbBoxMskLUT[MASK_EA];
BYTE	gbBoxMskDet[MASK_EA];

int		gbMaskOnNum = 0;	// BYTE �� ó���ð� 10~20us ���� ����
int		gbIMD_Bright = 0;
int		gbIMD_Moving = 0;
int		gbIMD_Update = 0;
int		gnIMD_TextOn = 0;


void InitIMD(void)
{
	IMD_CK_SELw(2);			// 74.25MHz ���� (FPGA������ ���� ���� ������� 74.25MHz�� ����)
	IMD_CK_PDw(UP(Itl));	// TODO KSH> MD Clock Enable - ������� ���� �ô� �׻� Clock Power Down ���!!!

	IMD_HSELw(1);			// MD HLOCK ����.  ��1���� ������ ��� ISP HLOCK�� ���
	IMD_HONw(1);			// MD Clock ���� �� ���õ� HLOCK�� �������� Clock�� �����Ѵ�. (�ٸ� Source ���õ� �׻� ��1��)

//	IMD_ISELw(0);			// ISP Image Source�� ACE ����� MD �Է����� ���
							// 0 : ISP ACE
							// 1 : ISP RGB Interpolator Y
							// 2 ~ 13 : ���� �߰� (Digital �Է� Source ���..)
//	IMD_IVSELw(0);			// ISP Sync ����
//	IMD_VMODw(0);			// Only 0

	IMD_ONw(1);
	IMD_FLTw(1);

//	IMD_TESTw(0);
//	IMD_DUALw(0);

	IMD_GMAw(1);

//	IMD_SELw(0);

//	IMD_64Bw(IMD_64B_ON);

	IMD_THw(0xd<<IMD_64B_ON);

//	IMD_BUPCw(0);

//	IMD_OSDBDWw(1);
	IMD_OSDONw(0/*0x1f*/);		// TODO KSH> MD TEST OSD
//	IMD_OSDLVw(0x200);

	IMD_KMAXw(0xc0);
	IMD_KMINw(0x80);
	IMD_KGAw(0x80);
//	IMD_KTHw(0);

	IMD_VSPw(0xc/*0*/);
	IMD_VWw(IMD_VW/*+1*/);
//	IMD_HSPw(0);
	IMD_HWw(IMD_HW/*+1*/);

#if 0
	IMD_WON1w(1);
	IMD_WVSP1w(0);
	IMD_WVEP1w(0xf/*IMD_VW*/);
	IMD_WHSP1w(0);
	IMD_WHEP1w(0x1b/*IMD_HW*/);

	IMD_WON2w(1);
	IMD_WVSP2w(0xf);
	IMD_WVEP2w(0x21/*IMD_VW*/);
	IMD_WHSP2w(0x1b);
	IMD_WHEP2w(0x3b/*IMD_HW*/);

	IMD_WON3w(1);
	IMD_WVSP3w(0);
	IMD_WVEP3w(IMD_VW);
	IMD_WHSP3w(0);
	IMD_WHEP3w(IMD_HW);
#endif

	MaskIMD();
}

void ISRT SensIMD(void)
{
	const int iThresIMD = LibUtlInterp1D(giCurAgc, ADNR_AGC_MIN, ADNR_AGC_MAX, 0, 5<<IMD_64B_ON); 	// Control threshold by AGC level,  giCurAgc�� CLAMP(MINMAX) ���� �ʴ� ��� giCurAgc�� 800, ADNR_AGC_MAX�� 512�̸� 8�� ��� !!!
	IMD_THw( ((((IMD_SENS_MAX - UP(ItlSens))<<1) + 3)<<IMD_64B_ON) + iThresIMD ); 											// 3 : Offset
}

void MaskIMD(void)
{
	int i,j;
	BYTE	bBoxMskLUT[MASK_EA];

	gbMaskOnNum = 0;

	for(i=0; i<MASK_EA; i++)
	{
		const UINT nMskOn = (gbMnImdCfg(i)->bAction == UP_ON);
		bRectMsk(i)->sy = MIN(gbMnImdCfg(i)->bPosY                       , IMD_VW)&0x7f;
		bRectMsk(i)->ey = MIN(gbMnImdCfg(i)->bPosY + gbMnImdCfg(i)->bSizY, IMD_VW)&0x7f;
		bRectMsk(i)->sx = MIN(gbMnImdCfg(i)->bPosX                       , IMD_HW)&0x7f;
		bRectMsk(i)->ex = MIN(gbMnImdCfg(i)->bPosX + gbMnImdCfg(i)->bSizX, IMD_HW)&0x7f;

		SetIsp(IMDAREA_BASE+i, nMskOn<<31 | bRectMsk(i)->sy<<24 | bRectMsk(i)->ey<<16 | bRectMsk(i)->sx<<8 | bRectMsk(i)->ex);

		gbMaskOnNum += nMskOn;

		if(nMskOn) {
			bBoxMskLUT[i] = i;

			for(j=0; j<i; j++)
			{
				if(bBoxMskLUT[j] >= MASK_EA) continue;

//				if(*(gnBoxMskPos+i) == *(gnBoxMskPos+j))     		// i == j
//				{
//					bBoxMskLUT[i] = j;
//					break;
//				}
				else if(bRectMsk(j)->sx <= bRectMsk(i)->sx && bRectMsk(i)->ex <= bRectMsk(j)->ex &&	// i < j
						bRectMsk(j)->sy <= bRectMsk(i)->sy && bRectMsk(i)->ey <= bRectMsk(j)->ey )
				{
					bBoxMskLUT[i] = MASK_EA;
					break;
				}
				else if(bRectMsk(i)->sx <= bRectMsk(j)->sx && bRectMsk(j)->ex <= bRectMsk(i)->ex &&	// i > j
						bRectMsk(i)->sy <= bRectMsk(j)->sy && bRectMsk(j)->ey <= bRectMsk(i)->ey )
				{
					bBoxMskLUT[j] = MASK_EA;
				}
			}
		}
		else {
			bBoxMskLUT[i] = MASK_EA;	// bBoxMskLUT[i]�� MASK_EA�̸� invalid
		}
	}

	gnBoxMskPosNum = 0;
	for(i=0; i<MASK_EA; i++)
	{
//		if(bBoxMskLUT[i] >= MASK_EA) continue;

		if(bBoxMskLUT[i] == i) {
			*bRectMsk(gnBoxMskPosNum) = *bRectMsk(i);
			//bBoxMskLUT[i] = gnBoxMskPosNum;
			gbBoxMskLUT[gnBoxMskPosNum] = i;
			gnBoxMskPosNum++;
		}
//		else {
//			bBoxMskLUT[i] = bBoxMskLUT[bBoxMskLUT[i]];
//		}
	}

#if IMD_TEST
	//printf("%d, %d, %d, %d\r\n", bBoxMskLUT[0], bBoxMskLUT[1], bBoxMskLUT[2], bBoxMskLUT[3]);
	for(i=0; i<gnBoxMskPosNum; i++)
	{
		printf("[%d] : %d, %d, %d, %d\r\n", gbBoxMskLUT[i], bRectMsk(i)->sx, bRectMsk(i)->ex, bRectMsk(i)->sy, bRectMsk(i)->ey);
	}
#endif
}

void ISRT BoxIMD(void)
{
	int i,j;

	if(MENU_TITLE_IS(OFF) && UP(ItlAlarm)==UP_ON)
	{
		if(gbIMD_Bright)		 { gnIMD_TextOn = IMD_FUNC_FPS; DispStr(" BRIGHT CHANGE !!!", IMD_TEXT_Y, IMD_TEXT_X, 18); }
		else if(gbIMD_Moving)	 { gnIMD_TextOn = IMD_FUNC_FPS; DispStr(" CAMERA MOVING !!!", IMD_TEXT_Y, IMD_TEXT_X, 18); }
		else if(gnBoxNumIMD)	 { gnIMD_TextOn = IMD_FUNC_FPS;

			for(j=MASK_EA,i=0; i<gnBoxMskPosNum; i++) {
				if(gbBoxMskDet[i]) {
					if(j == MASK_EA) j = gbBoxMskLUT[i];	// ���� window���� �������� ����� ���
					else break;								// ���� window���� �������� ����� ���
				}
			}

			if(i==gnBoxMskPosNum) {							// ���� window���� �������� ����� ���
				DispStr("WINDOW  MOTION !!!", IMD_TEXT_Y, IMD_TEXT_X, 18);
				SETFONTID(IMD_TEXT_Y, IMD_TEXT_X+6, '0'+j)
			}
			else {											// ���� window���� �������� ����� ���
				DispStr("WINDOWS MOTION !!!", IMD_TEXT_Y, IMD_TEXT_X, 18);
			}
		}
		else if(gnIMD_TextOn==1) { gnIMD_TextOn = 0; DispClr(IMD_TEXT_Y, IMD_TEXT_X, 18); }
		else if(gnIMD_TextOn)    { gnIMD_TextOn--; }
	}
	else {
		if(gnIMD_TextOn)		 { gnIMD_TextOn = 0; DispClr(IMD_TEXT_Y, IMD_TEXT_X, 18); }
	}

	//---------------------------------------------------------------------------------------------
	extern BYTE gbMnImdDetWin;
	extern BYTE gbMnImdZone;
	//const int iItlDettone = (gbMnImdDetWin && (UP(ItlDettone)==3)) ? 2 : UP(ItlDettone);	// DET TONE�� 3�� �� DET WINDOW �޴� ��� �� DET TONE�� 2�� ����
	const int iItlDettone = (gbMnImdDetWin) ? 4 : UP(ItlDettone);							// DET WINDOW �޴� ��� �� DET TONE�� 4�� ����

	const int iDetNum = (gbIMD_Update) ? 0 : (iItlDettone==3) ? (gnBoxNumIMD > IMD_BOX_EA) ? IMD_BOX_EA : gnBoxNumIMD :
											 (((int)gnBoxNumIMD+gbMaskOnNum) > (IMD_OUT_EA+1)) ? (IMD_OUT_EA+1) - gbMaskOnNum : (int)gnBoxNumIMD;
	GrpIMD(7,iDetNum)		// OSD ��� ����

	// Set IMD Box OSD
	gnBoxOnIMD = 0;
	gnBoxNumIMD = 0;	// iDetNum ���� ���� �� �ʱ�ȭ
	gnBoxMskNum = 0;
	gnBoxMskSel = 0;
	gnBoxFillIMD = 0;
	gnBoxToneIMD = 0;

	if(UP(ItlMaskOsd) == UP_ON)
	{
		// Set Motion Box
		for(i=iDetNum-1; i>=0; i--) {	// �߿�!!! : bRectIMD�� wRectIMD�� ���� �� �ڿ���(iDetNum) ���� bRECT ����ü�� �ݴ� ���� sx,ex,sy,ey�� �Ҵ�
			wRectIMD(i)->sx = (((WORD)bRectIMD(i)->sx)<<IMD_HW_B) + IMD2BOX_X;
			wRectIMD(i)->ex = (((WORD)bRectIMD(i)->ex)<<IMD_HW_B) + IMD2BOX_X;
			wRectIMD(i)->sy = (((WORD)bRectIMD(i)->sy)<<IMD_VW_B) + IMD2BOX_Y;
			wRectIMD(i)->ey = (((WORD)bRectIMD(i)->ey)<<IMD_VW_B) + IMD2BOX_Y;

			if(wRectIMD(i)->ex > (RP(FR_HW)-IMD2BOX_X)) wRectIMD(i)->ex = RP(FR_HW)-IMD2BOX_X;

			gnBoxOnIMD |= (1<<i);

			if((UP(ItlRectFill) == UP_ON) && (!gbMnImdDetWin)) {
				gnBoxFillIMD |= (1<<i);
				gnBoxToneIMD |= (BOX_TONE_25<<(i<<1));
			}
		}

		if(iItlDettone == 3) {
			gnBoxNumIMD = iDetNum;
		}
		else {
			i = iDetNum;

			// Set Area Box
			for(j=0; j<MASK_EA; j++) {
				if(gbMnImdCfg(j)->bAction)
				{
					wRectIMD(i)->sx = ((gbMnImdCfg(j)->bPosX)<<IMD_HW_B) + IMD2BOX_X;
					wRectIMD(i)->ex = ((gbMnImdCfg(j)->bPosX + gbMnImdCfg(j)->bSizX)<<IMD_HW_B) + IMD2BOX_X;
					wRectIMD(i)->sy = ((gbMnImdCfg(j)->bPosY)<<IMD_VW_B) + IMD2BOX_Y;
					wRectIMD(i)->ey = ((gbMnImdCfg(j)->bPosY + gbMnImdCfg(j)->bSizY)<<IMD_VW_B) + IMD2BOX_Y;

					if(wRectIMD(i)->ex > (RP(FR_HW)-IMD2BOX_X)) wRectIMD(i)->ex = RP(FR_HW)-IMD2BOX_X;

					gnBoxOnIMD |= (1<<i);

					//----------------------------------------------------------------------------------------------------
					gnBoxMskNum++;							// gnBoxMskNum : OSD BOX�� ����� Mask�� �� ����, BoxLast()���� ���

					if(gbMnImdDetWin && gbMnImdZone==j) {
						gnBoxMskSel = gnBoxMskNum;			// gnBoxMskSel : 1 �̻��̸� DET WINDOW �޴����� window ���� ��, ���� gnBoxMskSel=4 �̸� window zone 3�� ����, BoxLast()���� ���
						gnBoxFillIMD |= (1<<i);
						gnBoxToneIMD |= (((UP(ItlDettone)>=3) ? 2 : UP(ItlDettone))<<(i<<1));
					}

					/*if(gbMnImdDetWin) {
						gnBoxToneIMD |= (((UP(ItlDettone)==3) ? 2 : UP(ItlDettone))<<(i<<1));
					}*/

					if(iItlDettone<4) {
						gnBoxFillIMD |= (1<<i);
						//gnBoxToneIMD |= (((gnBoxMskSel==gnBoxMskNum) ? iItlDettone : BOX_TONE_0)<<(i<<1));
						gnBoxToneIMD |= (BOX_TONE_0<<(i<<1));
					}
					i++;
				}
			}

			// Set Background Box
			wRectIMD(i)->sx = 0;
			wRectIMD(i)->ex = RP(FR_HW);
			wRectIMD(i)->sy = 0;
			wRectIMD(i)->ey = RP(FR_VW);
			gnBoxOnIMD |= (1<<i);
			if(iItlDettone<4) {
				gnBoxFillIMD |= (1<<i);
				gnBoxToneIMD |= (iItlDettone<<(i<<1));
			}
			i++;

			//gnBoxOnIMD |= 0xf800;			// 15:BG, 14:Mask 3, 13:Mask 2, 12:Mask 1, 11:Mask 0, 10~0:Motion
			gnBoxNumIMD = i;
		}
	}
}

void ISRT IMD(void)
{
	if(UP(Itl)) {
		const ULONG TimeStaIMD = BenchTimeStart();

		SensIMD();
		LibIMD(IMD_HW, IMD_VW, IMD_DET_EA, IMD_FUNC_FPS);
		BoxIMD();

		FontBenchTime(gbMnDebugFnc==6, 14, 0, "IMD  ", TimeStaIMD, 6);

		TxGrpIMD();
	}
	else {
		gnBoxOnIMD = 0;
		gnBoxNumIMD = 0;
		gnBoxMskNum = 0;
		gnBoxMskSel = 0;
		gnBoxFillIMD = 0;
		gnBoxToneIMD = 0;

		if(gnIMD_TextOn) { gnIMD_TextOn = 0; DispClr(IMD_TEXT_Y, IMD_TEXT_X, 18); }
	}
}


/* EOF */

