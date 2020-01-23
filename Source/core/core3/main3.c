#include "dev.h"

#if 0
UINT gnIF_Funcs_Run = 0;
void IF_Funcs_Timer_irq(void *ctx)
{
	gnIF_Funcs_Run = 1;
}
#endif

void main_3(int cpu_id)
{
	SYS_REG0 = 0;
	while(SYS_REG0 == 0x0) {} // Wait for CPU0 to be ready.

	write_csr(mstatus, MSTATUS_FS); // Enable

	enx_externalirq_init_cpu3();

#if defined(__SENSOR__)

  #if 0	// ddr write test
	#define DDR0_BASE	0x80000000
	#define DDR1_BASE	0x90000000

	const UINT nHW = 416;
	const UINT nVW = 240;
	const UINT nDdrAddrY = DDR1_BASE + 0xAAA780;	// DDR 주소 설정
	const UINT nDdrAddrC = nDdrAddrY + (nHW * nVW);

	PSCK_SELw(2);
	PPCK_SELw(2);
	FN_CK0_SELw(2);
	PR_CK0_SELw(2);
	PSCK_PDw(1);
	PPCK_PDw(1);
	PR_CK0_PDw(1);
	FN_CK0_PDw(1);

	POS_HZw(1);
	//HSPOw(0x6);
	//VSPOw(0x4);
	HWOw(1928);
	VWOw(1088);
	HTWOw(2200-2);
	VTWOw(1125-1);
	OSYNC_MODw(1);
	//VLOCKI2_POSw(0x2);
	//HLOCKI2_POSw(0x3a);
	//EXVSYNC_SELw(0);
	//EXHSYNC_SELw(0);
	POS_HZw(0);

	Isp_Output_init();

	YC_OSELw(0x11);

	YCW_CK0_SELw(2);
	YCW_CK0_PDw(1);
	IM_YADR0w(nDdrAddrY>>4);
	IM_CADR0w(nDdrAddrC>>4);
	IM_HWI0w(nHW);
	IM_IVSEL0w(0);
	IM_ISEL0w(0xC);

	IM_GO0w(1);

	while(1);
  #else
	Isp_init();
  #endif

	//VIRQI_EN_Tw(1);
	//CLI_VLOCKI_Tw(1);		// TODO KSH> 컴파일 문제?
	extern UINT gnViIrqOn;

  #if 0
	#define TIMER_IRQ_CH	0
	TimerSetFreq(TIMER_IRQ_CH, 24, 29999, 1);
	TimerIrqCallback(TIMER_IRQ_CH, IF_Funcs_Timer_irq, 0);
	TimerSetIrqEn(TIMER_IRQ_CH, ENX_ON);
	TimerStart(TIMER_IRQ_CH);
  #endif

	INIT_STR("--------- Main Loop Start ---------");

	while (1)
	{
  #if 0
		Wait_VLOCKO();

		isp_main();

		IF_Funcs();
  #else
		if(gnViIrqOn/*ISP_RIRQ_VIr*/) {
			//CLI_VLOCKI_Tw(1);
			gnViIrqOn = 0;
			isp_main();
		}
		else {
			IF_Funcs();
		}
  #endif

//		ddr_control();	// for DDR Test, 사용하지 않음
	}
#else
	while (1) {
  #ifdef __ECM_STRING__
		Comm();
  #endif
		if (SYS_REG0 == 0xC) {
  #ifdef __USE_LED1__
			GpioSetOut(GPIO_LED1, GPIO_OUT_LOW);
  #endif
  #ifdef __USE_LED2__
			GpioSetOut(GPIO_LED2, GPIO_OUT_LOW);
  #endif
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xA;
		}
		WaitXms(1);
	}
#endif
}
