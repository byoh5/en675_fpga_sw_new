#include "dev.h"
#include "rv_utils.h" // dmwrite32, dmread32
//#define TEST_DDR_SIZE DDR_SIZE
#define TEST_DDR_SIZE 64*1024*1024

#if 1
void DdrTest(void)
{
	//dma_set(DDR_BASE, DDR_SIZE, 0);
	volatile ULONG *pDDR = (volatile ULONG *)DDR_BASE;

	printf("DDR Test - Write\n");
	for(pDDR = (volatile ULONG *)DDR_BASE; pDDR < (volatile ULONG *)(DDR_BASE+TEST_DDR_SIZE); pDDR++)
	{
		if (((long)pDDR) % (long)(1024*1024) == 0) {
			printf("DDR Test - Write(0x%08X)\n", pDDR);
		}
		if ((ULONG)pDDR % 64 == 0) {
			hwflush_dcache_range((ULONG)pDDR, 64);
		}
		//dmwrite32((uint)pDDR, (uint)pDDR);
		*pDDR = (ULONG)pDDR;
	}
	printf("DDR Test - Check\n");
	ULONG ok = 0, fail = 0;
	for(pDDR = (volatile ULONG *)DDR_BASE; pDDR < (volatile ULONG *)(DDR_BASE+TEST_DDR_SIZE); pDDR++)
	{
		if ((ULONG)pDDR % 64 == 0) {
			hwflush_dcache_range((ULONG)pDDR, 64);
		}
		if (((long)pDDR) % (long)(1024*1024) == 0) {
			printf("DDR Test - Read(0x%08X)...\n", pDDR);
		}
		//long getdata = dmread32((uint)pDDR);
		ULONG getdata = *pDDR;
		if (getdata != (ULONG)pDDR) {
			//printf("DDR Test - X: 0x%08X 0x%08X\n", pDDR, getdata);
			fail++;
		} else {
			//printf("DDR Test - O: 0x%08X 0x%08X\n", pDDR, getdata);
			ok++;
		}
	}
	printf("DDR Test - Done(O:%lu X:%lu)\n", ok, fail);
}
#else
void DdrTest(void)
{
	DmaMemSet_isr(0, (BYTE *)DDR_BASE, 0, DDR_SIZE);

	volatile UINT * pDDR;
	printf("DDR Test - Write\n");
	for(pDDR = (UINT *)DDR_BASE; pDDR < (UINT *)(DDR_BASE+TEST_DDR_SIZE); pDDR++)
	{
		*pDDR = (UINT)pDDR;
		if (((UINT)pDDR) % (1024*1024) == 0) {
			printf("DDR Test - Write(0x%08X)\n", pDDR);
		}
	}
	printf("DDR Test - Check\n");
	for(pDDR = (UINT *)DDR_BASE; pDDR < (UINT *)(DDR_BASE+TEST_DDR_SIZE); pDDR++)
	{
		if (*pDDR!=(UINT)pDDR) {
			printf("DDR Test - X: 0x%08X 0x%08X\n", *pDDR, (UINT)pDDR);
		} else {
			printf("DDR Test - O: 0x%08X 0x%08X\n", *pDDR, (UINT)pDDR);
		}
	}
	printf("DDR Test - Done\n");
}
#endif

static void DdrParamBypass(UINT var)
{
	DDR_DLY_CKEOUT  = var;
	DDR_DLY_CSOUT   = var;
	DDR_DLY_CA9OUT  = var;
	DDR_DLY_CA8OUT  = var;
	DDR_DLY_CA7OUT  = var;
	DDR_DLY_CA6OUT  = var;
	DDR_DLY_CA5OUT  = var;
	DDR_DLY_CA4OUT  = var;
	DDR_DLY_CA3OUT  = var;
	DDR_DLY_CA2OUT  = var;
	DDR_DLY_CA1OUT  = var;
	DDR_DLY_CA0OUT  = var;
	DDR_DLY_DQS0IN  = var;
	DDR_DLY_DM0OUT  = var;
	DDR_DLY_DQ7OUT  = var;
	DDR_DLY_DQ6OUT  = var;
	DDR_DLY_DQ5OUT  = var;
	DDR_DLY_DQ4OUT  = var;
	DDR_DLY_DQ3OUT  = var;
	DDR_DLY_DQ2OUT  = var;
	DDR_DLY_DQ1OUT  = var;
	DDR_DLY_DQ0OUT  = var;
	DDR_DLY_DQS1IN  = var;
	DDR_DLY_DM1OUT  = var;
	DDR_DLY_DQ15OUT = var;
	DDR_DLY_DQ14OUT = var;
	DDR_DLY_DQ13OUT = var;
	DDR_DLY_DQ12OUT = var;
	DDR_DLY_DQ11OUT = var;
	DDR_DLY_DQ10OUT = var;
	DDR_DLY_DQ9OUT  = var;
	DDR_DLY_DQ8OUT  = var;
	DDR_DLY_DQS2IN  = var;
	DDR_DLY_DM2OUT  = var;
	DDR_DLY_DQ23OUT = var;
	DDR_DLY_DQ22OUT = var;
	DDR_DLY_DQ21OUT = var;
	DDR_DLY_DQ20OUT = var;
	DDR_DLY_DQ19OUT = var;
	DDR_DLY_DQ18OUT = var;
	DDR_DLY_DQ17OUT = var;
	DDR_DLY_DQ16OUT = var;
	DDR_DLY_DQS3IN  = var;
	DDR_DLY_DM3OUT  = var;
	DDR_DLY_DQ31OUT = var;
	DDR_DLY_DQ30OUT = var;
	DDR_DLY_DQ29OUT = var;
	DDR_DLY_DQ28OUT = var;
	DDR_DLY_DQ27OUT = var;
	DDR_DLY_DQ26OUT = var;
	DDR_DLY_DQ25OUT = var;
	DDR_DLY_DQ24OUT = var;
}

void CaOutDly(UINT var)
{
	DDR_DLY_CA9OUT  = var;
	DDR_DLY_CA8OUT  = var;
	DDR_DLY_CA7OUT  = var;
	DDR_DLY_CA6OUT  = var;
	DDR_DLY_CA5OUT  = var;
	DDR_DLY_CA4OUT  = var;
	DDR_DLY_CA3OUT  = var;
	DDR_DLY_CA2OUT  = var;
	DDR_DLY_CA1OUT  = var;
	DDR_DLY_CA0OUT  = var;
}

void DqOutDly(UINT var)
{
	DDR_DLY_DQ7OUT  = var;
	DDR_DLY_DQ6OUT  = var;
	DDR_DLY_DQ5OUT  = var;
	DDR_DLY_DQ4OUT  = var;
	DDR_DLY_DQ3OUT  = var;
	DDR_DLY_DQ2OUT  = var;
	DDR_DLY_DQ1OUT  = var;
	DDR_DLY_DQ0OUT  = var;
	DDR_DLY_DQ15OUT = var;
	DDR_DLY_DQ14OUT = var;
	DDR_DLY_DQ13OUT = var;
	DDR_DLY_DQ12OUT = var;
	DDR_DLY_DQ11OUT = var;
	DDR_DLY_DQ10OUT = var;
	DDR_DLY_DQ9OUT  = var;
	DDR_DLY_DQ8OUT  = var;
	DDR_DLY_DQ23OUT = var;
	DDR_DLY_DQ22OUT = var;
	DDR_DLY_DQ21OUT = var;
	DDR_DLY_DQ20OUT = var;
	DDR_DLY_DQ19OUT = var;
	DDR_DLY_DQ18OUT = var;
	DDR_DLY_DQ17OUT = var;
	DDR_DLY_DQ16OUT = var;
	DDR_DLY_DQ31OUT = var;
	DDR_DLY_DQ30OUT = var;
	DDR_DLY_DQ29OUT = var;
	DDR_DLY_DQ28OUT = var;
	DDR_DLY_DQ27OUT = var;
	DDR_DLY_DQ26OUT = var;
	DDR_DLY_DQ25OUT = var;
	DDR_DLY_DQ24OUT = var;
}

void DmOutDly(UINT var)
{
	DDR_DLY_DM0OUT  = var;
	DDR_DLY_DM1OUT  = var;
	DDR_DLY_DM2OUT  = var;
	DDR_DLY_DM3OUT  = var;
}

void DdrTestOff(void)
{
	if (DDR_PWR_CUR) {
		DDR_PWR_REQ = 1;
		while (DDR_PWR_REQ);
	}
}
void DdrTestOn(int var, int RD_mask, int RD_edge, int RD_ltc, int WR_ltc)
{
	printf("DDR Test - RD(MASK:%2x, EDGE:%d, LTC:%2x) WR(LTC:%x) Param(%x) - ", RD_mask, RD_edge, RD_ltc, WR_ltc, var);

//	_regs_ BF_5(UINT RD_EN_MASK : 8 ,UINT _rev0 : 16, UINT RD_VAL_EDGE : 1 ,UINT RD_VAL_LTC : 4 ,UINT WR_LTC : 3 ) _rege_ _DDR_5;
	DDR_RD_EN_MASK  = 0x20; // 0 ~ 0xff
	DDR_RD_VAL_EDGE = 1; // 0, 1
	DDR_RD_VAL_LTC	= 6; // 0 ~ 15
	DDR_WR_LTC 		= 1; // 0 ~ 7
	DdrParamBypass(var); // 0 ~ 15

	DDR_PWR_REQ = (DDR_PWR_CUR)? 0 : 1;
	while(DDR_PWR_REQ);
}




char ddr_cal_test()
{
	char result = 0;
	DDR_MR_ADR = 32;
	DDR_MR_WE  = 0;
	DDR_MR_REQ = 1;
	while(DDR_MR_REQ);
	if(DDR_MR_RDDAT0!=0x00000000) result++;
	if(DDR_MR_RDDAT1!=0x00000000) result++;
	if(DDR_MR_RDDAT2!=0xffffffff) result++;
	if(DDR_MR_RDDAT3!=0xffffffff) result++;
	//if(result>0) _printf("%08x %08x %08x %08x ", DDR_MR_RDDAT0,DDR_MR_RDDAT1,DDR_MR_RDDAT2,DDR_MR_RDDAT3);
	DDR_MR_ADR = 40;
	DDR_MR_WE  = 0;
	DDR_MR_REQ = 1;
	while(DDR_MR_REQ);
	if(DDR_MR_RDDAT0!=0xffffffff) result++;
	if(DDR_MR_RDDAT1!=0x00000000) result++;
	if(DDR_MR_RDDAT2!=0xffffffff) result++;
	if(DDR_MR_RDDAT3!=0x00000000) result++;
	//if(result>0) _printf("%08x %08x %08x %08x\n", DDR_MR_RDDAT0,DDR_MR_RDDAT1,DDR_MR_RDDAT2,DDR_MR_RDDAT3);
	DDR_MR_ADR = 32;
	DDR_MR_WE  = 0;
	DDR_MR_REQ = 1;
	while(DDR_MR_REQ);
	if(DDR_MR_RDDAT0!=0x00000000) result++;
	if(DDR_MR_RDDAT1!=0x00000000) result++;
	if(DDR_MR_RDDAT2!=0xffffffff) result++;
	if(DDR_MR_RDDAT3!=0xffffffff) result++;
	//if(result>0) _printf("%08x %08x %08x %08x ", DDR_MR_RDDAT0,DDR_MR_RDDAT1,DDR_MR_RDDAT2,DDR_MR_RDDAT3);
	DDR_MR_ADR = 40;
	DDR_MR_WE  = 0;
	DDR_MR_REQ = 1;
	while(DDR_MR_REQ);
	if(DDR_MR_RDDAT0!=0xffffffff) result++;
	if(DDR_MR_RDDAT1!=0x00000000) result++;
	if(DDR_MR_RDDAT2!=0xffffffff) result++;
	if(DDR_MR_RDDAT3!=0x00000000) result++;
	//if(result>0) _printf("%08x %08x %08x %08x\n", DDR_MR_RDDAT0,DDR_MR_RDDAT1,DDR_MR_RDDAT2,DDR_MR_RDDAT3);
	return result;
}

void DdrInitTest(void)
{
	char min = 15, max = 0;

	for (UINT i = 0; i < 16; i++) {
		DDR_DLY_CSOUT = i;
		char temp = ddr_cal_test();
		if (temp != 0) {
			printf("_");
		} else {
			printf("*");
			if (min > i) {
				min = i;
			}
			if (max < i) {
				max = i;
			}
		}
	}
	DDR_DLY_CA9OUT  = (max+min+1)>>1;
	DDR_DLY_CA8OUT  = (max+min+1)>>1;
	DDR_DLY_CA7OUT  = (max+min+1)>>1;
	DDR_DLY_CA6OUT  = (max+min+1)>>1;
	DDR_DLY_CA5OUT  = (max+min+1)>>1;
	DDR_DLY_CA4OUT  = (max+min+1)>>1;
	DDR_DLY_CA3OUT  = (max+min+1)>>1;
	DDR_DLY_CA2OUT  = (max+min+1)>>1;
	DDR_DLY_CA1OUT  = (max+min+1)>>1;
	DDR_DLY_CA0OUT  = (max+min+1)>>1;
	DDR_DLY_CSOUT   = (max+min+1)>>1;
#if 1
	printf("DDR_DLY_CAOUT  : %u %u %u %u %u %u %u %u %u %u %u\n",
			DDR_DLY_CA9OUT, DDR_DLY_CA8OUT, DDR_DLY_CA7OUT, DDR_DLY_CA6OUT, DDR_DLY_CA5OUT,
			DDR_DLY_CA4OUT, DDR_DLY_CA3OUT, DDR_DLY_CA2OUT, DDR_DLY_CA1OUT, DDR_DLY_CA0OUT, DDR_DLY_CSOUT);
#else
	printf("DDR_DLY_CA9OUT : %d\n", DDR_DLY_CA9OUT);
	printf("DDR_DLY_CA8OUT : %d\n", DDR_DLY_CA8OUT);
	printf("DDR_DLY_CA7OUT : %d\n", DDR_DLY_CA7OUT);
	printf("DDR_DLY_CA6OUT : %d\n", DDR_DLY_CA6OUT);
	printf("DDR_DLY_CA5OUT : %d\n", DDR_DLY_CA5OUT);
	printf("DDR_DLY_CA4OUT : %d\n", DDR_DLY_CA4OUT);
	printf("DDR_DLY_CA3OUT : %d\n", DDR_DLY_CA3OUT);
	printf("DDR_DLY_CA2OUT : %d\n", DDR_DLY_CA2OUT);
	printf("DDR_DLY_CA1OUT : %d\n", DDR_DLY_CA1OUT);
	printf("DDR_DLY_CA0OUT : %d\n", DDR_DLY_CA0OUT);
	printf("DDR_DLY_CSOUT : %d\n", DDR_DLY_CSOUT);
#endif
	min = 15;
	max = 0;
	for (UINT i = 0; i < 16; i++) {
		DDR_DLY_DQS0IN = i;
		char temp = ddr_cal_test();
		if (temp != 0) {
			printf("_");
		} else {
			printf("*");
			if (min > i) {
				min = i;
			}
			if (max < i) {
				max = i;
			}
		}
	}
	DDR_DLY_DQS0IN = min + 1;
	printf("DDR_DLY_DQS0IN : %d\n", DDR_DLY_DQS0IN);

	min = 15;
	max = 0;
	for (UINT i = 0; i < 16; i++) {
		DDR_DLY_DQS1IN = i;
		char temp = ddr_cal_test();
		if (temp != 0) {
			printf("_");
		} else {
			printf("*");
			if (min > i) {
				min = i;
			}
			if (max < i) {
				max = i;
			}
		}
	}
	DDR_DLY_DQS1IN = min + 1;
	printf("DDR_DLY_DQS1IN : %d\n", DDR_DLY_DQS1IN);

	min = 15;
	max = 0;
	for (UINT i = 0; i < 16; i++) {
		DDR_DLY_DQS2IN = i;
		char temp = ddr_cal_test();
		if (temp != 0) {
			printf("_");
		} else {
			printf("*");
			if (min > i) {
				min = i;
			}
			if (max < i) {
				max = i;
			}
		}
	}
	DDR_DLY_DQS2IN = min + 1;
	printf("DDR_DLY_DQS2IN : %d\n", DDR_DLY_DQS2IN);

	min = 15;
	max = 0;
	for (UINT i = 0; i < 16; i++) {
		DDR_DLY_DQS3IN = i;
		char temp = ddr_cal_test();
		if (temp != 0) {
			printf("_");
		} else {
			printf("*");
			if (min > i) {
				min = i;
			}
			if (max < i) {
				max = i;
			}
		}
	}
	DDR_DLY_DQS3IN = min + 1;
	printf("DDR_DLY_DQS3IN : %d\n", DDR_DLY_DQS3IN);
}

void DdrInit(void)
{
#if 0
	for (int i = 6; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			for (int k = 0; k <= 7; k++) {
				for (int l = 0; l <= 1; l++) {
					DdrTestOff();

					printf("TEST: RD_VAL_EDGE(%d) RD_VAL_LTC(%d) WR_LTC(%d) ParamBypass(%d)\n", l, i, k, j);
					DDR_RD_EN_MASK  = 0x20;
					DDR_RD_VAL_EDGE = l;
					DDR_RD_VAL_LTC	= i;
					DDR_WR_LTC 		= k;
					DdrParamBypass(j);
					DDR_PWR_REQ     = 1;
					while (DDR_PWR_REQ);
					DdrInitTest();
				}
			}
		}
	}
#else
	DdrTestOff();

	//DDR_MEM_SEL = 0; //  16MByte(128Mb) S2/S4
	//DDR_MEM_SEL = 1; //  32MByte(256Mb) S2/S4
	//DDR_MEM_SEL = 2; //  64MByte(512Mb) S2/S4
	//DDR_MEM_SEL = 3; // 128MByte(1Gb) S2
	//DDR_MEM_SEL = 4; // 128MByte(1Gb) S4
	//DDR_MEM_SEL = 5; // 256MByte(2Gb) S2
	//DDR_MEM_SEL = 6; // 256MByte(2Gb) S4
	//DDR_MEM_SEL = 7; // 512MByte(4Gb) S2/S4

	DDR_RD_EN_MASK  = 0x20;
	DDR_RD_VAL_EDGE = 0;
	DDR_RD_VAL_LTC	= 6;
	DDR_WR_LTC 		= 1;
	DdrParamBypass(5);
	DDR_PWR_REQ     = 1;
	while (DDR_PWR_REQ);
	DdrInitTest();
	DDR_RD_VAL_LTC	= 7;
#endif

	printf("Select DDR_MEM_SEL: %u\n", DDR_MEM_SEL);
}

void DdrTestProcess(void)
{
	volatile UINT *pDDR = (volatile UINT *)DDR_BASE;

	//hwflush_dcache_range(DDR_BASE, DDR_BASE + 16*1024);
	int ok = 0, fail = 0;

	//printf("DDR Test - Write\n");
	for (pDDR = (volatile UINT *)DDR_BASE; pDDR < (volatile UINT *)(DDR_BASE+TEST_DDR_SIZE); pDDR++) {
		//dmwrite32((ulong)pDDR, (ulong)pDDR);
		*pDDR = pDDR;
	}
	//printf("DDR Test - Check\n");
	for (pDDR = (volatile UINT *)DDR_BASE; pDDR < (volatile UINT *)(DDR_BASE+TEST_DDR_SIZE); pDDR++) {
		//long getdata = dmread32((ulong)pDDR);
		long getdata = *pDDR;
		if (getdata != (long)pDDR) {
			//printf("DDR Test - X: 0x%08X 0x%08lX\n", pDDR, getdata);
			fail++;
		} else {
			//printf("DDR Test - O: 0x%08X 0x%08lX\n", pDDR, getdata);
			ok++;
		}
	}
	printf("Done(O:%d/X:%d)\n", ok, fail);
	if (ok > 0) {
		printf("Stop!\n");
		while(1);
	}
}
