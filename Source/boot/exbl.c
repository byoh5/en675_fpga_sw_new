#include <stdint.h>

#include "uart.h"
#include "print.h"
#include "boot.h"

#include "mtrap.h"
#include "bits.h"
#include "asm.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define SIZE_OF_DATA 1024*16
int g_test_data = 0;

char g_key=0x0f;


#define SLEEP_CNT 100000

#define BASE_ADDR 0x80003000

void sleep_(int cnt)
{

	for(int i=0;i<cnt;i++){
		__asm("C.NOP");
	}

}

volatile uint64_t* mtime;

void main_(void)
{
  _printf("exception\n");
  while(1);
}

#if 1
void timerTask(void* pvParameters)
{
	while(1) {
		_printf("%s:%d\r\n", __func__, __LINE__);
		vTaskDelay(100);
	}
	vTaskDelete(NULL);
}
#endif

void main_0(int cpu_id)
{
 _init_bss_section();

 Uart1_Init();
 Uart1_int_init();

 _printf("%s - HELLO EYENIX!!!!!!(%d)(%x)\n", __func__, cpu_id, &cpu_id); // ADD some

 mtime = (void*)(0x2000000 + 0xbff8);

 vTaskCreate("timer", timerTask, NULL, 1024, 5);

 vTaskStartScheduler();

 g_test_data = 0;
 while(1){
	 if(g_key == 0x0f){
		 g_key = 0x0;
		 g_test_data++;

		 _printf("0:\t\tTIME: %u\r\n", *mtime);

		 g_key = 0x1;
	 }
 	 sleep_(SLEEP_CNT);
 }
}

void main_1(int cpu_id)
{
	 Uart1_Init();

	 _printf("%s - HELLO EYENIX!!!!!!(%d)(%x)\n", __func__, cpu_id, &cpu_id); // ADD some

	 while(1){
		 if((g_key & 0x01) && !(g_key & 0x02)) {
			 g_key &= 0xfe;
			 //_printf("1:%08x\r\n",g_test_data);
			 g_key |= (0x01|0x02);
		 }
		 sleep_(SLEEP_CNT);
	 }
}

void main_2(int cpu_id)
{
	 Uart1_Init();

	 _printf("%s - HELLO EYENIX!!!!!!(%d)(%x)\n", __func__, cpu_id, &cpu_id); // ADD some

	 while(1){
		 if((g_key & 0x01) && !(g_key & 0x04)) {
			 g_key &= 0xfe;
			 //_printf("2:%08x\r\n",g_test_data);
			 g_key |= (0x01|0x04);
		 }
		 sleep_(SLEEP_CNT+500);
	 }
}

void main_3(int cpu_id)
{
	 Uart1_Init();

	 _printf("%s - HELLO EYENIX!!!!!!(%d)(%x)\n", __func__, cpu_id, &cpu_id); // ADD some

	 while(1){
		 if((g_key & 0x01) && !(g_key & 0x08)) {
			 g_key &= 0xfe;
			 //_printf("3:%08x\r\n",g_test_data);
			 g_key |= (0x01|0x08);
		 }
		 sleep_(SLEEP_CNT+1000);
	 }
}


