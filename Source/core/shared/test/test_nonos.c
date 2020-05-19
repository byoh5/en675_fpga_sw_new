#include "dev.h"
#include "test_header.h"
#include "test_nonos.h"

void testloop1(void)
{
	if (SYS_REG5 & 0x10) {
		test_dma_init(256*1024, -1, 256*1024-64*1024, 'B', CPU_ID, eDMAtest_DDRtoDDR);
		test_dma_start();
		SYS_REG5 = 0;
	}
}

void testloop2(void)
{
#if (USE_ETH==1)
	if (SYS_REG6 & 0x10) {
		test_eth_init();
		test_eth_start();
		SYS_REG6 = 0;
	}
#endif
}

void testloop3(void)
{

}
