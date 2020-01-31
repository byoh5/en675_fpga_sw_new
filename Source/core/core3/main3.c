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

  #if 0
	void isp_init_test(void); isp_init_test();
  #else
	Isp_init();
	////YC_OSELw(0x11);
	//PCKO_PDw(0);	// Sensor가 Master로 동작하고 있는 경우 SLVw(0)하기 전에 Sensor Clock Off 해야 함
	//SLVw(0);
	//SYNC_BYSw(1);
	//INSELw(0x6);
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
