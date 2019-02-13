#include "dev.h"

extern void dma_set(UINT dst, UINT len, BYTE value);

void DdrTest(void)
{
	dma_set(DDR_BASE, DDR_SIZE, 0);

	volatile UINT * pDDR;
	_printf("DDR Test - Write\n");
	for(pDDR = (UINT *)DDR_BASE; pDDR < (UINT *)(DDR_BASE+DDR_SIZE); pDDR++)
	{
		*pDDR = (UINT)pDDR;
	}
	_printf("DDR Test - Check\n");
	for(pDDR = (UINT *)DDR_BASE; pDDR < (UINT *)(DDR_BASE+DDR_SIZE); pDDR++)
	{
		if(*pDDR!=(UINT)pDDR)
		{
			_printf("DDR Test - X: 0x%08X 0x%08X\n", *pDDR, (UINT)pDDR);
			//_printf("DDR Test - Error\n");
			//while(1);
		} else {
			_printf("DDR Test - O: 0x%08X 0x%08X\n", *pDDR, (UINT)pDDR);
		}
	}
	_printf("DDR Test - Done\n");
}

static void DdrParamBypass(void)
{
	DDR_DLY_CKEOUT  = 3;
	DDR_DLY_CSOUT   = 3;
	DDR_DLY_CA9OUT  = 3;
	DDR_DLY_CA8OUT  = 3;
	DDR_DLY_CA7OUT  = 3;
	DDR_DLY_CA6OUT  = 3;
	DDR_DLY_CA5OUT  = 3;
	DDR_DLY_CA4OUT  = 3;
	DDR_DLY_CA3OUT  = 3;
	DDR_DLY_CA2OUT  = 3;
	DDR_DLY_CA1OUT  = 3;
	DDR_DLY_CA0OUT  = 3;
	DDR_DLY_DQS0IN  = 3;
	DDR_DLY_DM0OUT  = 3;
	DDR_DLY_DQ7OUT  = 3;
	DDR_DLY_DQ6OUT  = 3;
	DDR_DLY_DQ5OUT  = 3;
	DDR_DLY_DQ4OUT  = 3;
	DDR_DLY_DQ3OUT  = 3;
	DDR_DLY_DQ2OUT  = 3;
	DDR_DLY_DQ1OUT  = 3;
	DDR_DLY_DQ0OUT  = 3;
	DDR_DLY_DQS1IN  = 3;
	DDR_DLY_DM1OUT  = 3;
	DDR_DLY_DQ15OUT = 3;
	DDR_DLY_DQ14OUT = 3;
	DDR_DLY_DQ13OUT = 3;
	DDR_DLY_DQ12OUT = 3;
	DDR_DLY_DQ11OUT = 3;
	DDR_DLY_DQ10OUT = 3;
	DDR_DLY_DQ9OUT  = 3;
	DDR_DLY_DQ8OUT  = 3;
	DDR_DLY_DQS2IN  = 3;
	DDR_DLY_DM2OUT  = 3;
	DDR_DLY_DQ23OUT = 3;
	DDR_DLY_DQ22OUT = 3;
	DDR_DLY_DQ21OUT = 3;
	DDR_DLY_DQ20OUT = 3;
	DDR_DLY_DQ19OUT = 3;
	DDR_DLY_DQ18OUT = 3;
	DDR_DLY_DQ17OUT = 3;
	DDR_DLY_DQ16OUT = 3;
	DDR_DLY_DQS3IN  = 3;
	DDR_DLY_DM3OUT  = 3;
	DDR_DLY_DQ31OUT = 3;
	DDR_DLY_DQ30OUT = 3;
	DDR_DLY_DQ29OUT = 3;
	DDR_DLY_DQ28OUT = 3;
	DDR_DLY_DQ27OUT = 3;
	DDR_DLY_DQ26OUT = 3;
	DDR_DLY_DQ25OUT = 3;
	DDR_DLY_DQ24OUT = 3;
}

void DdrInit(void)
{
	DDR_RD_EN_MASK  = 0x20;
	DDR_RD_VAL_EDGE = 1;
	DDR_RD_VAL_LTC	= 6;
	DDR_WR_LTC 		= 1;
	DdrParamBypass();

	DDR_PWR_REQ = (DDR_PWR_CUR)? 0 : 1;
	while(DDR_PWR_REQ);
}
