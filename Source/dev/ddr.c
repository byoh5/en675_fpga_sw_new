#include "dev.h"
#include "rv_utils.h" // dmwrite32, dmread32
//#define TEST_DDR_SIZE DDR_SIZE
#define TEST_DDR_SIZE 1*64

#if 1
void DdrTest(void)
{
	//dma_set(DDR_BASE, DDR_SIZE, 0);
	volatile UINT *pDDR = (volatile UINT *)DDR_BASE;

	printf("DDR Test - Write\n");
	for(pDDR = (volatile UINT *)DDR_BASE; pDDR < (volatile UINT *)(DDR_BASE+TEST_DDR_SIZE); pDDR++)
	{
		//if (((long)pDDR) % (long)(1024*1024) == 0) {
			printf("DDR Test - Write(0x%08X)\n", pDDR);
		//}
		dmwrite32((uint)pDDR, (uint)pDDR);
	}
	printf("DDR Test - Check\n");
	for(pDDR = (volatile UINT *)DDR_BASE; pDDR < (volatile UINT *)(DDR_BASE+TEST_DDR_SIZE); pDDR++)
	{
		long getdata = dmread32((uint)pDDR);
		if (getdata != (long)pDDR) {
			printf("DDR Test - X: 0x%08X 0x%08X\n", pDDR, getdata);
		} else {
			printf("DDR Test - O: 0x%08X 0x%08X\n", pDDR, getdata);
		}
	}
	printf("DDR Test - Done\n");
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

void DdrInit(void)
{
	DdrTestOff();

	DDR_RD_EN_MASK  = 0x20;
	DDR_RD_VAL_EDGE = 1;
	DDR_RD_VAL_LTC	= 6;
	DDR_WR_LTC 		= 1;
	DdrParamBypass(5);

	DDR_PWR_REQ = (DDR_PWR_CUR)? 0 : 1;
	while(DDR_PWR_REQ);
}

void DdrTestProcess(void)
{
	volatile UINT *pDDR = (volatile UINT *)DDR_BASE;

	//hwflush_dcache_range(DDR_BASE, DDR_BASE + 16*1024);
	int ok = 0, fail = 0;

	//printf("DDR Test - Write\n");
	for (pDDR = (volatile UINT *)DDR_BASE; pDDR < (volatile UINT *)(DDR_BASE+TEST_DDR_SIZE); pDDR++) {
		dmwrite32((uint)pDDR, (uint)pDDR);
	}
	//printf("DDR Test - Check\n");
	for (pDDR = (volatile UINT *)DDR_BASE; pDDR < (volatile UINT *)(DDR_BASE+TEST_DDR_SIZE); pDDR++) {
		long getdata = dmread32((uint)pDDR);
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

void DdrTestOff(void)
{
	if (DDR_PWR_CUR) {
		DDR_PWR_REQ = 1;
	}
	while (DDR_PWR_REQ);
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

