#include "dev.h"

void enx_peri_init(void)
{
	UartInit(7, UART7_SPEED);

	BDmaInit();
	CDmaInit();

	printf("Start EN675 DDR Init(1GBx2)\n");
	if (DdrInit(0, 4, 0) == ENX_FAIL) {
		if (DdrInit(0, 4, 1) == ENX_FAIL) {
			_Rprintf("DDR0 Init Fail!\n");
			while(1);
		}
	}
	hwflush_dcache_range_all();
	BDmaMemSet_isr(0, (BYTE *)DDR0_BASE, 0x00, DDR0_SIZE);
	hwflush_dcache_range_all();

	printf("%s\n", TTY_COLOR_RESET);

	SflsGetinfo();
	SflsInit();
}

void enx_sys_init(void)
{
	write_csr(mstatus, MSTATUS_FS); // Enable
	//for (int i = 0; i < 32; i++) {
	//	init_fp_reg(i);
	//}
	//write_csr(fcsr, 0);

	UINT arrMark[5];
	arrMark[0] = SYS_MARK0;
	arrMark[1] = SYS_MARK1;
	arrMark[2] = SYS_MARK2;
	arrMark[3] = SYS_MARK3;
	arrMark[4] = SYS_MARK4;
	char *strName = (char *)arrMark;
	printf("%s", TTY_COLOR_CYAN);
	for (int i = 0; i < sizeof(arrMark); i++) {
		printf("%c", strName[i]);
	}
	printf("%04X-%02X-%02X %02X:%02X:%02X%s\n", SYS_RTL_YEAR, SYS_RTL_MONTH, SYS_RTL_DAY, SYS_RTL_HOUR, SYS_RTL_MINUTE, SYS_RTL_SECOND, TTY_COLOR_RESET);
}

void main_bootloader(int cpu_id)
{
	if (cpu_id == 0) {
		*mtime = 0; // timer init

		enx_peri_init();
		enx_sys_init();

		//enx_externalirq_init();
		//enx_timerirq_init();

		SYS_REG0 = 0xA; // CPU0 Ready!

		printf("Init Device - RTL-191016-1408\n");

		while(1);
	} else {
		while(1);
	}
}