#include "dev.h"

#if 0
UINT gnIF_Funcs_Run = 0;
void IF_Funcs_Timer_irq(void *ctx)
{
	gnIF_Funcs_Run = 1;
}
#endif

static void CPUtoISPcallback(void *ctx)
{
	//while (CPU_TO_ISP_MSG_MUTEX);

	extern void IspMsgFnc(UINT anMsg);
	IspMsgFnc(SHREG_CMD);

	printf("CPU to ISP!(%X)\n", SHREG_CMD);

	SHREG_CMD = 0;
	CPU_TO_ISP_MSG_MUTEX = 0;

	BtoAIrqCall();
}

void main_3(int cpu_id)
{
	SYS_REG0 = 0xf;
	while(SYS_REG0 == 0xf) {} // Wait for CPU0 to be ready.

	enx_externalirq_init_cpu3();

	AtoBIrqCallback(CPUtoISPcallback, NULL);
	AtoBSetIrqEn(ENX_ON);

#if defined(__SENSOR__)

	Isp_init();

  #if 0//model_TgtBd == 1
	VIRQI_EN_Tw(1);
	//CLI_VLOCKI_Tw(1);		// TODO KSH> 컴파일 문제?
  #else
	extern UINT gnViIrqOn;
  #endif

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
	#if 0//model_TgtBd == 1
		if(ISP_RIRQ_VIr) { CLI_VLOCKI_Tw(1); isp_main(); }
	#else
		if(gnViIrqOn) { gnViIrqOn = 0; isp_main(); }
	#endif
		else { IF_Funcs(); }
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
