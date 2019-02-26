//*************************************************************************************************
// Led task
//*************************************************************************************************
#include "dev.h"
#include "enx_freertos.h"

#include "ledblink.h"

void LedblinkTask(void *pvParameters)
{
	uint64_t ledcnt = 0;
	while (1) {
		ledcnt++;

		if (ledcnt % 1000 == 0) {
			static uint64_t k = 0;
			if (k++%2)	GpioSetHi(GPIO_LED0);
			else		GpioSetLo(GPIO_LED0);
		}

		if (ledcnt % 100 == 0) {
			static uint64_t k = 0;
			if (k++%2)	GpioSetHi(GPIO_LED1);
			else		GpioSetLo(GPIO_LED1);
		}

		if (ledcnt % 50 == 0) {
			static uint64_t k = 0;
			if (k++%2)	GpioSetHi(GPIO_LED2);
			else		GpioSetLo(GPIO_LED2);
		}

		vTaskDelay(1);
	}
	UNUSED(pvParameters);
}
