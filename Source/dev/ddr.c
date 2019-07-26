#include "dev.h"


#define DDR_MR_ADR(CH) _bm(_DDR0_0,REG_BASE_DDR[CH], (0<<3),MR_ADR) // 8 Bit, 8'h0, RW
#define DDR_MR_WRDAT(CH) _bm(_DDR0_0,REG_BASE_DDR[CH], (0<<3),MR_WRDAT) // 8 Bit, 8'h0, RW
#define DDR_MR_WE(CH) _bm(_DDR0_0,REG_BASE_DDR[CH], (0<<3),MR_WE) // 1 Bit, 1'h0, RW
#define DDR_MR_REQ(CH) _bm(_DDR0_0,REG_BASE_DDR[CH], (0<<3),MR_REQ) // 1 Bit, 1'h0, RW
#define DDR_MR1_OP(CH) _bm(_DDR0_1,REG_BASE_DDR[CH], (1<<3),MR1_OP) // 8 Bit, 8'hE2, RW
#define DDR_MR2_OP(CH) _bm(_DDR0_1,REG_BASE_DDR[CH], (1<<3),MR2_OP) // 8 Bit, 8'h06, RW
#define DDR_MEM_SEL(CH) _bm(_DDR0_1,REG_BASE_DDR[CH], (1<<3),MEM_SEL) // 3 Bit, 3'h2, RW
#define DDR_PWR_CUR(CH) _bm(_DDR0_1,REG_BASE_DDR[CH], (1<<3),PWR_CUR) // 1 Bit, 1'h0, R
#define DDR_PWR_REQ(CH) _bm(_DDR0_1,REG_BASE_DDR[CH], (1<<3),PWR_REQ) // 1 Bit, 1'h0, RW
#define DDR_T_REFI(CH) _bm(_DDR0_2,REG_BASE_DDR[CH], (2<<3),T_REFI) // 16 Bit, 16'h580, RW
#define DDR_T_REFAB(CH) _bm(_DDR0_3,REG_BASE_DDR[CH], (3<<3),T_REFAB) // 7 Bit, 7'h17, RW
#define DDR_ZQCS_EN(CH) _bm(_DDR0_3,REG_BASE_DDR[CH], (3<<3),ZQCS_EN) // 1 Bit, 1'h0, RW
#define DDR_T_ZQCS(CH) _bm(_DDR0_3,REG_BASE_DDR[CH], (3<<3),T_ZQCS) // 6 Bit, 6'h0F, RW
#define DDR_ZQCS_LMT(CH) _bm(_DDR0_3,REG_BASE_DDR[CH], (3<<3),ZQCS_LMT) // 16 Bit, 16'hFFFF, RW
#define DDR_T_RCD(CH) _bm(_DDR0_4,REG_BASE_DDR[CH], (4<<3),T_RCD) // 4 Bit, 4'h2, RW
#define DDR_T_RRD(CH) _bm(_DDR0_4,REG_BASE_DDR[CH], (4<<3),T_RRD) // 4 Bit, 4'h0, RW
#define DDR_T_WTR(CH) _bm(_DDR0_4,REG_BASE_DDR[CH], (4<<3),T_WTR) // 4 Bit, 4'h4, RW
#define DDR_T_RTP(CH) _bm(_DDR0_4,REG_BASE_DDR[CH], (4<<3),T_RTP) // 4 Bit, 4'h3, RW
#define DDR_T_WR(CH) _bm(_DDR0_4,REG_BASE_DDR[CH], (4<<3),T_WR) // 4 Bit, 4'h7, RW
#define DDR_T_RP(CH) _bm(_DDR0_4,REG_BASE_DDR[CH], (4<<3),T_RP) // 4 Bit, 4'h1, RW
#define DDR_T_MRR(CH) _bm(_DDR0_4,REG_BASE_DDR[CH], (4<<3),T_MRR) // 4 Bit, 4'h0, RW
#define DDR_T_MRW(CH) _bm(_DDR0_4,REG_BASE_DDR[CH], (4<<3),T_MRW) // 4 Bit, 4'h4, RW
#define DDR_RD_EN_MASK(CH) _bm(_DDR0_5,REG_BASE_DDR[CH], (5<<3),RD_EN_MASK) // 8 Bit, 8'h20, RW
#define DDR_RD_VAL_EDGE(CH) _bm(_DDR0_5,REG_BASE_DDR[CH], (5<<3),RD_VAL_EDGE) // 1 Bit, 1'h1, RW
#define DDR_RD_VAL_LTC(CH) _bm(_DDR0_5,REG_BASE_DDR[CH], (5<<3),RD_VAL_LTC) // 4 Bit, 4'h6, RW
#define DDR_MRR_VAL_LTC(CH) _bm(_DDR0_5,REG_BASE_DDR[CH], (5<<3),MRR_VAL_LTC) // 4 Bit, 4'h6, RW
#define DDR_WR_LTC(CH) _bm(_DDR0_5,REG_BASE_DDR[CH], (5<<3),WR_LTC) // 3 Bit, 3'h1, RW
#define DDR_MR_RDDAT0(CH) _bm(_DDR0_6,REG_BASE_DDR[CH], (6<<3),MR_RDDAT0) // 32 Bit, 32'h0, R
#define DDR_MR_RDDAT1(CH) _bm(_DDR0_7,REG_BASE_DDR[CH], (7<<3),MR_RDDAT1) // 32 Bit, 32'h0, R
#define DDR_MR_RDDAT2(CH) _bm(_DDR0_8,REG_BASE_DDR[CH], (8<<3),MR_RDDAT2) // 32 Bit, 32'h0, R
#define DDR_MR_RDDAT3(CH) _bm(_DDR0_9,REG_BASE_DDR[CH], (9<<3),MR_RDDAT3) // 32 Bit, 32'h0, R
#define DDR_DLY_CKEOUT(CH) _bm(_DDR0_10,REG_BASE_DDR[CH], (10<<3),DLY_CKEOUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CSOUT(CH) _bm(_DDR0_10,REG_BASE_DDR[CH], (10<<3),DLY_CSOUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA9OUT(CH) _bm(_DDR0_10,REG_BASE_DDR[CH], (10<<3),DLY_CA9OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA8OUT(CH) _bm(_DDR0_10,REG_BASE_DDR[CH], (10<<3),DLY_CA8OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA7OUT(CH) _bm(_DDR0_11,REG_BASE_DDR[CH], (11<<3),DLY_CA7OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA6OUT(CH) _bm(_DDR0_11,REG_BASE_DDR[CH], (11<<3),DLY_CA6OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA5OUT(CH) _bm(_DDR0_11,REG_BASE_DDR[CH], (11<<3),DLY_CA5OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA4OUT(CH) _bm(_DDR0_11,REG_BASE_DDR[CH], (11<<3),DLY_CA4OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA3OUT(CH) _bm(_DDR0_11,REG_BASE_DDR[CH], (11<<3),DLY_CA3OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA2OUT(CH) _bm(_DDR0_11,REG_BASE_DDR[CH], (11<<3),DLY_CA2OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA1OUT(CH) _bm(_DDR0_11,REG_BASE_DDR[CH], (11<<3),DLY_CA1OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_CA0OUT(CH) _bm(_DDR0_11,REG_BASE_DDR[CH], (11<<3),DLY_CA0OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQS0IN(CH) _bm(_DDR0_12,REG_BASE_DDR[CH], (12<<3),DLY_DQS0IN) // 4 Bit, 4'h0, RW
#define DDR_DLY_DM0OUT(CH) _bm(_DDR0_12,REG_BASE_DDR[CH], (12<<3),DLY_DM0OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ7OUT(CH) _bm(_DDR0_13,REG_BASE_DDR[CH], (13<<3),DLY_DQ7OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ6OUT(CH) _bm(_DDR0_13,REG_BASE_DDR[CH], (13<<3),DLY_DQ6OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ5OUT(CH) _bm(_DDR0_13,REG_BASE_DDR[CH], (13<<3),DLY_DQ5OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ4OUT(CH) _bm(_DDR0_13,REG_BASE_DDR[CH], (13<<3),DLY_DQ4OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ3OUT(CH) _bm(_DDR0_13,REG_BASE_DDR[CH], (13<<3),DLY_DQ3OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ2OUT(CH) _bm(_DDR0_13,REG_BASE_DDR[CH], (13<<3),DLY_DQ2OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ1OUT(CH) _bm(_DDR0_13,REG_BASE_DDR[CH], (13<<3),DLY_DQ1OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ0OUT(CH) _bm(_DDR0_13,REG_BASE_DDR[CH], (13<<3),DLY_DQ0OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQS1IN(CH) _bm(_DDR0_14,REG_BASE_DDR[CH], (14<<3),DLY_DQS1IN) // 4 Bit, 4'h0, RW
#define DDR_DLY_DM1OUT(CH) _bm(_DDR0_14,REG_BASE_DDR[CH], (14<<3),DLY_DM1OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ15OUT(CH) _bm(_DDR0_15,REG_BASE_DDR[CH], (15<<3),DLY_DQ15OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ14OUT(CH) _bm(_DDR0_15,REG_BASE_DDR[CH], (15<<3),DLY_DQ14OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ13OUT(CH) _bm(_DDR0_15,REG_BASE_DDR[CH], (15<<3),DLY_DQ13OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ12OUT(CH) _bm(_DDR0_15,REG_BASE_DDR[CH], (15<<3),DLY_DQ12OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ11OUT(CH) _bm(_DDR0_15,REG_BASE_DDR[CH], (15<<3),DLY_DQ11OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ10OUT(CH) _bm(_DDR0_15,REG_BASE_DDR[CH], (15<<3),DLY_DQ10OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ9OUT(CH) _bm(_DDR0_15,REG_BASE_DDR[CH], (15<<3),DLY_DQ9OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ8OUT(CH) _bm(_DDR0_15,REG_BASE_DDR[CH], (15<<3),DLY_DQ8OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQS2IN(CH) _bm(_DDR0_16,REG_BASE_DDR[CH], (16<<3),DLY_DQS2IN) // 4 Bit, 4'h0, RW
#define DDR_DLY_DM2OUT(CH) _bm(_DDR0_16,REG_BASE_DDR[CH], (16<<3),DLY_DM2OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ23OUT(CH) _bm(_DDR0_17,REG_BASE_DDR[CH], (17<<3),DLY_DQ23OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ22OUT(CH) _bm(_DDR0_17,REG_BASE_DDR[CH], (17<<3),DLY_DQ22OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ21OUT(CH) _bm(_DDR0_17,REG_BASE_DDR[CH], (17<<3),DLY_DQ21OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ20OUT(CH) _bm(_DDR0_17,REG_BASE_DDR[CH], (17<<3),DLY_DQ20OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ19OUT(CH) _bm(_DDR0_17,REG_BASE_DDR[CH], (17<<3),DLY_DQ19OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ18OUT(CH) _bm(_DDR0_17,REG_BASE_DDR[CH], (17<<3),DLY_DQ18OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ17OUT(CH) _bm(_DDR0_17,REG_BASE_DDR[CH], (17<<3),DLY_DQ17OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ16OUT(CH) _bm(_DDR0_17,REG_BASE_DDR[CH], (17<<3),DLY_DQ16OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQS3IN(CH) _bm(_DDR0_18,REG_BASE_DDR[CH], (18<<3),DLY_DQS3IN) // 4 Bit, 4'h0, RW
#define DDR_DLY_DM3OUT(CH) _bm(_DDR0_18,REG_BASE_DDR[CH], (18<<3),DLY_DM3OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ31OUT(CH) _bm(_DDR0_19,REG_BASE_DDR[CH], (19<<3),DLY_DQ31OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ30OUT(CH) _bm(_DDR0_19,REG_BASE_DDR[CH], (19<<3),DLY_DQ30OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ29OUT(CH) _bm(_DDR0_19,REG_BASE_DDR[CH], (19<<3),DLY_DQ29OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ28OUT(CH) _bm(_DDR0_19,REG_BASE_DDR[CH], (19<<3),DLY_DQ28OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ27OUT(CH) _bm(_DDR0_19,REG_BASE_DDR[CH], (19<<3),DLY_DQ27OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ26OUT(CH) _bm(_DDR0_19,REG_BASE_DDR[CH], (19<<3),DLY_DQ26OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ25OUT(CH) _bm(_DDR0_19,REG_BASE_DDR[CH], (19<<3),DLY_DQ25OUT) // 4 Bit, 4'h0, RW
#define DDR_DLY_DQ24OUT(CH) _bm(_DDR0_19,REG_BASE_DDR[CH], (19<<3),DLY_DQ24OUT) // 4 Bit, 4'h0, RW

const volatile ULONG REG_BASE_DDR[] = { REG_BASE_DDR0, REG_BASE_DDR1 };


#define TEST_DDR0_SIZE DDR0_SIZE
//#define TEST_DDR0_SIZE 1*1024

char DdrMapTest(BYTE bCH)
{
	const ULONG DDR_BASE = (bCH==1) ? DDR1_BASE : DDR0_BASE;

	volatile ULONG *pDDR = (volatile ULONG *)DDR_BASE;
	//printf("DDR MAP Test - Write\n");
	for(pDDR = (volatile ULONG *)DDR_BASE; pDDR < (volatile ULONG *)(DDR_BASE+TEST_DDR0_SIZE); pDDR++)
	{
		if ((ULONG)pDDR % 64 == 0) hwflush_dcache_range((ULONG)pDDR, 0x40);
		//*pDDR = (ULONG)pDDR;
		*pDDR = (ULONG)0x55555555aaaaaaaa;
	}
	ULONG ok = 0, fail = 0;
	for(pDDR = (volatile ULONG *)DDR_BASE; pDDR < (volatile ULONG *)(DDR_BASE+TEST_DDR0_SIZE); pDDR++)
	{
		if ((ULONG)pDDR % 64 == 0) hwflush_dcache_range((ULONG)pDDR, 0x40);
		ULONG getdata = *pDDR;
		//if (getdata != (ULONG)pDDR)
		if (getdata != (ULONG)0x55555555aaaaaaaa)
		{
                printf("DDR Test - X: %08lx 0x%08lx\n", pDDR, getdata);
                fail++;
		}
		else
		{
			ok++;
		}
     }
    printf("DDR Test - Done(O:%lu X:%lu)\n", ok, fail);
	return ((fail)? 1 : 0);
}

void DqG0OutDly(BYTE bCH, UINT var)
{
	DDR_DLY_DQ7OUT(bCH)  = var;
	DDR_DLY_DQ6OUT(bCH)  = var;
	DDR_DLY_DQ5OUT(bCH)  = var;
	DDR_DLY_DQ4OUT(bCH)  = var;
	DDR_DLY_DQ3OUT(bCH)  = var;
	DDR_DLY_DQ2OUT(bCH)  = var;
	DDR_DLY_DQ1OUT(bCH)  = var;
	DDR_DLY_DQ0OUT(bCH)  = var;
}
void DqG1OutDly(BYTE bCH, UINT var)
{
	DDR_DLY_DQ15OUT(bCH) = var;
	DDR_DLY_DQ14OUT(bCH) = var;
	DDR_DLY_DQ13OUT(bCH) = var;
	DDR_DLY_DQ12OUT(bCH) = var;
	DDR_DLY_DQ11OUT(bCH) = var;
	DDR_DLY_DQ10OUT(bCH) = var;
	DDR_DLY_DQ9OUT(bCH)  = var;
	DDR_DLY_DQ8OUT(bCH)  = var;
}
void DqG2OutDly(BYTE bCH, UINT var)
{
	DDR_DLY_DQ23OUT(bCH) = var;
	DDR_DLY_DQ22OUT(bCH) = var;
	DDR_DLY_DQ21OUT(bCH) = var;
	DDR_DLY_DQ20OUT(bCH) = var;
	DDR_DLY_DQ19OUT(bCH) = var;
	DDR_DLY_DQ18OUT(bCH) = var;
	DDR_DLY_DQ17OUT(bCH) = var;
	DDR_DLY_DQ16OUT(bCH) = var;
}
void DqG3OutDly(BYTE bCH, UINT var)
{
	DDR_DLY_DQ31OUT(bCH) = var;
	DDR_DLY_DQ30OUT(bCH) = var;
	DDR_DLY_DQ29OUT(bCH) = var;
	DDR_DLY_DQ28OUT(bCH) = var;
	DDR_DLY_DQ27OUT(bCH) = var;
	DDR_DLY_DQ26OUT(bCH) = var;
	DDR_DLY_DQ25OUT(bCH) = var;
	DDR_DLY_DQ24OUT(bCH) = var;
}
void DqOutDly(BYTE bCH, UINT var)
{
	DqG0OutDly(bCH,var);
	DqG1OutDly(bCH,var);
	DqG2OutDly(bCH,var);
	DqG3OutDly(bCH,var);
}
void DmOutDly(BYTE bCH, UINT var)
{
	DDR_DLY_DM0OUT(bCH)  = var;
	DDR_DLY_DM1OUT(bCH)  = var;
	DDR_DLY_DM2OUT(bCH)  = var;
	DDR_DLY_DM3OUT(bCH)  = var;
}
void CaOutDly(BYTE bCH, int var)
{
	DDR_DLY_CA9OUT(bCH)  = var;
	DDR_DLY_CA8OUT(bCH)  = var;
	DDR_DLY_CA7OUT(bCH)  = var;
	DDR_DLY_CA6OUT(bCH)  = var;
	DDR_DLY_CA5OUT(bCH)  = var;
	DDR_DLY_CA4OUT(bCH)  = var;
	DDR_DLY_CA3OUT(bCH)  = var;
	DDR_DLY_CA2OUT(bCH)  = var;
	DDR_DLY_CA1OUT(bCH)  = var;
	DDR_DLY_CA0OUT(bCH)  = var;
}

char ddr_cal_test(BYTE bCH)
{
	volatile char result = 0;
	DDR_MR_ADR(bCH) = 32;
	DDR_MR_WE(bCH)  = 0;
	DDR_MR_REQ(bCH) = 1;
	while(DDR_MR_REQ(bCH));
	if(DDR_MR_RDDAT0(bCH)!=0x00000000) result++;
	if(DDR_MR_RDDAT1(bCH)!=0x00000000) result++;
	if(DDR_MR_RDDAT2(bCH)!=0xffffffff) result++;
	if(DDR_MR_RDDAT3(bCH)!=0xffffffff) result++;
	//if(result>0) printf("%08x %08x %08x %08x ", DDR_MR_RDDAT0(bCH),DDR_MR_RDDAT1(bCH),DDR_MR_RDDAT2(bCH),DDR_MR_RDDAT3(bCH));
	DDR_MR_ADR(bCH) = 40;
	DDR_MR_WE(bCH)  = 0;
	DDR_MR_REQ(bCH) = 1;
	while(DDR_MR_REQ(bCH));
	if(DDR_MR_RDDAT0(bCH)!=0xffffffff) result++;
	if(DDR_MR_RDDAT1(bCH)!=0x00000000) result++;
	if(DDR_MR_RDDAT2(bCH)!=0xffffffff) result++;
	if(DDR_MR_RDDAT3(bCH)!=0x00000000) result++;
	//if(result>0) printf("%08x %08x %08x %08x\n", DDR_MR_RDDAT0(bCH),DDR_MR_RDDAT1(bCH),DDR_MR_RDDAT2(bCH),DDR_MR_RDDAT3(bCH));
	DDR_MR_ADR(bCH) = 32;
	DDR_MR_WE(bCH)  = 0;
	DDR_MR_REQ(bCH) = 1;
	while(DDR_MR_REQ(bCH));
	if(DDR_MR_RDDAT0(bCH)!=0x00000000) result++;
	if(DDR_MR_RDDAT1(bCH)!=0x00000000) result++;
	if(DDR_MR_RDDAT2(bCH)!=0xffffffff) result++;
	if(DDR_MR_RDDAT3(bCH)!=0xffffffff) result++;
	//if(result>0) printf("%08x %08x %08x %08x ", DDR_MR_RDDAT0(bCH),DDR_MR_RDDAT1(bCH),DDR_MR_RDDAT2(bCH),DDR_MR_RDDAT3(bCH));
	DDR_MR_ADR(bCH) = 40;
	DDR_MR_WE(bCH)  = 0;
	DDR_MR_REQ(bCH) = 1;
	while(DDR_MR_REQ(bCH));
	if(DDR_MR_RDDAT0(bCH)!=0xffffffff) result++;
	if(DDR_MR_RDDAT1(bCH)!=0x00000000) result++;
	if(DDR_MR_RDDAT2(bCH)!=0xffffffff) result++;
	if(DDR_MR_RDDAT3(bCH)!=0x00000000) result++;
	//if(result>0) printf("%08x %08x %08x %08x\n", DDR_MR_RDDAT0(bCH),DDR_MR_RDDAT1(bCH),DDR_MR_RDDAT2(bCH),DDR_MR_RDDAT3(bCH));
	return result;
}
char ddr_rw_test(BYTE bCH)
{
	const volatile ULONG DDR_BASE = (bCH==1) ? DDR1_BASE : DDR0_BASE;

	volatile char result = 0;
	volatile UINT *ADDR0_BASE = (UINT *)(DDR_BASE);
	hwflush_dcache_range(DDR_BASE, 0x40);
	ADDR0_BASE[0] = 0xffffffff;
	ADDR0_BASE[1] = 0;
	ADDR0_BASE[2] = 0xffffffff;
	ADDR0_BASE[3] = 0;
	ADDR0_BASE[4] = 0xffffffff;
	ADDR0_BASE[5] = 0xffffffff;
	ADDR0_BASE[6] = 0;
	ADDR0_BASE[7] = 0;
	ADDR0_BASE[8] = 0xaaaaaaaa;
	ADDR0_BASE[9] = 0x55555555;
	ADDR0_BASE[10] = 0xaaaaaaaa;
	ADDR0_BASE[11] = 0x55555555;
	ADDR0_BASE[12] = 0x55555555;
	ADDR0_BASE[13] = 0xaaaaaaaa;
	ADDR0_BASE[14] = 0x55555555;
	ADDR0_BASE[15] = 0xaaaaaaaa;
	hwflush_dcache_range(DDR_BASE, 0x40);
	if(ADDR0_BASE[0]!=0xffffffff) result++;
	if(ADDR0_BASE[1]!=0x00000000) result++;
	if(ADDR0_BASE[2]!=0xffffffff) result++;
	if(ADDR0_BASE[3]!=0x00000000) result++;
	if(ADDR0_BASE[4]!=0xffffffff) result++;
	if(ADDR0_BASE[5]!=0xffffffff) result++;
	if(ADDR0_BASE[6]!=0x00000000) result++;
	if(ADDR0_BASE[7]!=0x00000000) result++;
	if(ADDR0_BASE[8]!=0xaaaaaaaa) result++;
	if(ADDR0_BASE[9]!=0x55555555) result++;
	if(ADDR0_BASE[10]!=0xaaaaaaaa) result++;
	if(ADDR0_BASE[11]!=0x55555555) result++;
	if(ADDR0_BASE[12]!=0x55555555) result++;
	if(ADDR0_BASE[13]!=0xaaaaaaaa) result++;
	if(ADDR0_BASE[14]!=0x55555555) result++;
	if(ADDR0_BASE[15]!=0xaaaaaaaa) result++;
	//printf("%08x %08x %08x %08x %08x %08x %08x %08x\n", ADDR0_BASE[0], ADDR0_BASE[1], ADDR0_BASE[2], ADDR0_BASE[3], ADDR0_BASE[4], ADDR0_BASE[5], ADDR0_BASE[6], ADDR0_BASE[7]);
	return result;
}

void DdrGetId(BYTE bCH)
{
	DDR_MR_ADR(bCH) = 5;
	DDR_MR_WE(bCH)  = 0;
	DDR_MR_REQ(bCH) = 1;
	while(DDR_MR_REQ(bCH));
	printf("MR5(%d):%08x\n", bCH,DDR_MR_RDDAT0(bCH));
	DDR_MR_ADR(bCH) = 8;
	DDR_MR_WE(bCH)  = 0;
	DDR_MR_REQ(bCH) = 1;
	while(DDR_MR_REQ(bCH));
	printf("MR8(%d):%08x\n", bCH,DDR_MR_RDDAT0(bCH));
}

void DdrOn(BYTE bCH)
{
	if(!DDR_PWR_CUR(bCH))
	{
		DDR_PWR_REQ(bCH) = 1;
		while(DDR_PWR_REQ(bCH));
	}
}
void DdrOff(BYTE bCH)
{
	if(DDR_PWR_CUR(bCH))
	{
		DDR_PWR_REQ(bCH) = 1;
		while(DDR_PWR_REQ(bCH));
	}
}
void DdrOffOn(BYTE bCH)
{
	DdrOff(bCH);
	DdrOn(bCH);
}

void DdrInit(BYTE bCH, BYTE Sel)
{
	volatile int i;
	volatile char min=15, max=0;

	DdrOff(bCH);

	DDR_MEM_SEL(bCH) = Sel;// (0:128Mb S2/S4, 1:256Mb S2/S4, 2:512Mb S2/S4, 3:1Gb S2, 4:1Gb S4, 5: 2Gb S2, 6:2Gb S4, 7:4Gb S2/S4)
	DDR_RD_EN_MASK(bCH)  = 0x08;
	DDR_RD_VAL_EDGE(bCH) = 1;
	DDR_RD_VAL_LTC(bCH)	= 6;
	DDR_MRR_VAL_LTC(bCH)	= 6;
	DDR_WR_LTC(bCH) 		= 0;

	DdrOn(bCH);

	for(i=0;i<16;i++)
	{
		DDR_DLY_CSOUT(bCH) = i;
		CaOutDly(bCH,i);
		volatile char temp = ddr_cal_test(bCH);
		if(temp!=0) printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
		DdrOffOn(bCH);
	}
	if(min==15){ printf("CA(%d) FAIL\n",bCH); while(1);}
	CaOutDly(bCH,(max+min+1)>>1);
	DDR_DLY_CSOUT(bCH) = (max+min+1)>>1;
	printf("DDR_DLY_CA*OUT(%d) : %d  ",bCH,DDR_DLY_CA9OUT(bCH));
	printf("DDR_DLY_CSOUT(%d) : %d\n",bCH,DDR_DLY_CSOUT(bCH));
	DdrOffOn(bCH);
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR_DLY_DQS0IN(bCH) = i;
		volatile char temp = ddr_cal_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR_DLY_DQS0IN(%d) FAIL\n",bCH); while(1);}
	DDR_DLY_DQS0IN(bCH) = min+2;
	printf("DDR_DLY_DQS0IN(%d) : %d\n",bCH,DDR_DLY_DQS0IN(bCH));

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR_DLY_DQS1IN(bCH) = i;
		volatile char temp = ddr_cal_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR_DLY_DQS1IN(%d) FAIL\n",bCH); while(1);}
	DDR_DLY_DQS1IN(bCH) = min+2;
	printf("DDR_DLY_DQS1IN(%d) : %d\n",bCH,DDR_DLY_DQS1IN(bCH));

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR_DLY_DQS2IN(bCH) = i;
		volatile char temp = ddr_cal_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR_DLY_DQS2IN(%d) FAIL\n",bCH); while(1);}
	DDR_DLY_DQS2IN(bCH) = min+2;
	printf("DDR_DLY_DQS2IN(%d) : %d\n",bCH,DDR_DLY_DQS2IN(bCH));

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR_DLY_DQS3IN(bCH) = i;
		volatile char temp = ddr_cal_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR_DLY_DQS3IN(%d) FAIL\n",bCH); while(1);}
	DDR_DLY_DQS3IN(bCH) = min+2;
	DDR_RD_VAL_LTC(bCH)	= 7;

	printf("DDR_DLY_DQS3IN(%d) : %d\n",bCH,DDR_DLY_DQS3IN(bCH));

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DmOutDly(bCH,i);
		DqOutDly(bCH,i);
		volatile char temp = ddr_rw_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR_DMDQ_OUT(%d) FAIL\n",bCH); while(1);}
	DmOutDly(bCH,min+2);
	DqOutDly(bCH,min+2);
	printf("DDR_DMDQ_OUT(%d) : %d\n",bCH,min+2);

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR_DLY_DM0OUT(bCH) = i;
		volatile char temp = ddr_rw_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DDR_DLY_DM0OUT(bCH) = min+2;
	printf("DDR_DLY_DM0OUT(%d) : %d\n",bCH,DDR_DLY_DM0OUT(bCH));

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR_DLY_DM1OUT(bCH) = i;
		volatile char temp = ddr_rw_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DDR_DLY_DM1OUT(bCH) = min+2;
	printf("DDR_DLY_DM1OUT(%d) : %d\n",bCH,DDR_DLY_DM1OUT(bCH));
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR_DLY_DM2OUT(bCH) = i;
		volatile char temp = ddr_rw_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DDR_DLY_DM2OUT(bCH) = min+2;
	printf("DDR_DLY_DM2OUT(%d) : %d\n",bCH,DDR_DLY_DM2OUT(bCH));
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR_DLY_DM3OUT(bCH) = i;
		volatile char temp = ddr_rw_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DDR_DLY_DM3OUT(bCH) = min+2;
	printf("DDR_DLY_DM3OUT(%d) : %d\n",bCH,DDR_DLY_DM3OUT(bCH));

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DqG0OutDly(bCH,i);
		volatile char temp = ddr_rw_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DqG0OutDly(bCH,min+2);
	printf("DDR_DQG0_OUT(%d) : %d\n",bCH,min+2);
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DqG1OutDly(bCH,i);
		volatile char temp = ddr_rw_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DqG1OutDly(bCH,min+2);
	printf("DDR_DQG1_OUT(%d) : %d\n",bCH,min+2);
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DqG2OutDly(bCH,i);
		volatile char temp = ddr_rw_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DqG2OutDly(bCH,min+2);
	printf("DDR_DQG2_OUT(%d) : %d\n",bCH,min+2);



	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DqG3OutDly(bCH,i);
		volatile char temp = ddr_rw_test(bCH);
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DqG3OutDly(bCH,min+2);
	printf("DDR_DQG3_OUT(%d) : %d\n",bCH,min+2);

//	DdrMapTest(bCH);
}
