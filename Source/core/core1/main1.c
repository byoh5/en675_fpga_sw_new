#include "dev.h"
#include "test_nonos.h"

#if 1//defined(EN675_HEVC_TEST)
BYTE test_isp_codec_flag = 0;
BYTE test_isp_codec_done = 0;
BYTE test_isp_update_flag = 0;
BYTE test_isp_done_flag = 0;

UINT BIT_STREAM_LOCK = 0;
UINT BIT_STREAM_ADDR = 0;
UINT BIT_STREAM_SIZE = 0;
UINT BIT_STREAM_TYPE = 0;
UINT BIT_STREAM_FRNUM = 0;
UINT BIT_STREAM_RECNUM = 0;
ULONG CodecENCTimeStart = 0;
ULONG CodecENConlyTimeStart = 0;
#endif

void main_1(int cpu_id)
{
	SYS_REG0 = 0xf;
	while(SYS_REG0 == 0xf) {} // Wait for CPU0 to be ready.

#ifdef EN675_HEVC_TEST
	WaitXms(10000);
#endif
	enx_externalirq_init_cpu1();

#if model_TgtBd == 2	// CPU 2core
	enx_externalirq_perl(eigiISP, ENX_ON, 0);							// Enable ISP Interrupts
	//enx_externalirq_perl(eigiVCODEC, ENX_ON, 0);						// Enable Codec Interrupts
	enx_externalirq_perl(eigiATOB, ENX_ON, 0);							// Enable ATOB Interrupts

	AtoBIrqCallback(CPUtoISPcallback, NULL);
	AtoBSetIrqEn(ENX_ON);

	Init_Visp(); INIT_STR("Init_Visp...");	// ISP initial
	Init_Vcap(); INIT_STR("Init_Vcap...");	// Video path set
	Init_Vout(); INIT_STR("Init_Vout...");	// Digital/Analog Output set
	Init_Virq(); INIT_STR("Init_Virq...");	// Video interrupt enable

	INIT_STR("--------- Main Loop Start ---------");

#ifdef EN675_HEVC_TEST
	while(test_isp_done_flag == 0)
	{
		Visp();
		Vcap();
	}

	//codec init
	int codec_terminate = 0;
	Codec_multi_instance_init();
#endif
	while (1)
	{
		Visp();
		Vcap();

#ifdef EN675_HEVC_TEST
                Vcodec();
#endif
	}
#else
	while (1) {
#if TEST_CPU1
		testloop1();
#else
		if (SYS_REG0 == 0xA) {
#ifdef __USE_LED1__
			GpioSetOut(GPIO_LED1, GPIO_OUT_HI);
#endif
			//_printf("%d:%lu\r\n", cpu_id, *mtime);
			WaitXms(100);
			SYS_REG0 = 0xB;
		}
#endif
	}
#endif
}
