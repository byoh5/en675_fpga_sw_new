#include "dev.h"

#if USE_IR

#include "enx_freertos.h"
#include "ir_recv.h"

static TaskHandle_t ir_recv_notity;

static void ir_recv_irq(void *ctx)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(ir_recv_notity, &xHigherPriorityTaskWoken);
	if (xHigherPriorityTaskWoken) {
		gbXsrTaskSwitchNeeded = 1;
	}
}

void ir_recv_task(void *ctx)
{
	printf("%s start\n", __func__);

	ir_recv_notity = xTaskGetCurrentTaskHandle();

	IrInit(IR_IRQ_ADDR);
	IrIrqCallback(ir_recv_irq, NULL);
	IrSetIrqEn(ENX_ON);

	printf("%s init ok\n", __func__);

	while (1) {
		if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
			UINT getIRDart = IrGetDat();
			printf("IR Input [");
			switch (getIRDart) {
			case 0x45:     printf("1");   break;
			case 0x46:     printf("2");   break;
			case 0x47:     printf("3");   break;
			case 0x44:     printf("4");   break;
			case 0x40:     printf("5");   break;
			case 0x43:     printf("6");   break;
			case 0x07:     printf("7");   break;
			case 0x15:     printf("8");   break;
			case 0x09:     printf("9");   break;
			case 0x19:     printf("0");   break;
			case 0x16:     printf("*");   break;
			case 0x0D:     printf("#");   break;
			case 0x1C:     printf("OK");  break;
			case 0x18:     printf("^");   break;
			case 0x08:     printf("<");   break;
			case 0x5A:     printf(">");   break;
			case 0x52:     printf("v");   break;
			default:       printf("Err(%02X)", getIRDart);  break;
			}
			printf("] %s\n", IR_RPT == 0 ? "normal" : "repeat");
		}
	}
}
#endif
