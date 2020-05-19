#include "dev.h"

#if 0
UINT gnIF_Funcs_Run = 0;
void IF_Funcs_Timer_irq(void *ctx)
{
	gnIF_Funcs_Run = 1;
}
#endif

void Init_Vsys(void) // System
{
	SYS_REG0 = 0xf;
	while(SYS_REG0 == 0xf) {} // Wait for CPU0 to be ready.

	enx_externalirq_init_cpu3();

	AtoBIrqCallback(CPUtoISPcallback, NULL);
	AtoBSetIrqEn(ENX_ON);
}

void main_3(int cpu_id)
{
	Init_Vsys(); INIT_STR("Init_Vsys...");	// CPU3 System initial

//	FORCE_ABT_SOFFw(1); // I2S-BCK pin muxer issus

#if defined(__SENSOR__)

	Init_Visp(); INIT_STR("Init_Visp...");	// ISP initial
	Init_Vcap(); INIT_STR("Init_Vcap...");	// Video path set
	Init_Vout(); INIT_STR("Init_Vout...");	// Digital/Analog Output set
	Init_Virq(); INIT_STR("Init_Virq...");	// Video interrupt enable

	INIT_STR("--------- Main Loop Start ---------");

	while (1)
	{
		Visp();
		Vcap();
		//Venc();
		//Vdec();
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
