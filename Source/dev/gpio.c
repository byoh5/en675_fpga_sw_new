#include "dev.h"

static _GPIO_PIN *arrGPIO[GPIO_CNT];

void GpioInit(void)
{
	for (uint64_t i = 0; i < GPIO_CNT; i++) {
		arrGPIO[i] = (_GPIO_PIN *)(REG_BASE_GPIO + (i << 2));
	}
}

void GpioEi(UINT nCH)
{	// Direction : Gpio interrupt enable
	arrGPIO[nCH]->GPIO_IRQ_EN = 1;
}

void GpioDi(UINT nCH)
{	// Direction : Gpio interrupt disable
	arrGPIO[nCH]->GPIO_IRQ_EN = 0;
}

void GpioRiseEdge(UINT nCH)
{	// Direction : Gpio rising edge sel
	arrGPIO[nCH]->GPIO_IRQ_DIR = 1;
}

void GpioFallEdge(UINT nCH)
{	// Direction : Gpio falling edge sel
	arrGPIO[nCH]->GPIO_IRQ_DIR = 0;
}

void GpioInDir(UINT nCH)
{	// Direction : Gpio direction set to output
	arrGPIO[nCH]->GPIO_OEN = 1;
}

void GpioOutDir(UINT nCH)
{	// Direction : Gpio direction set to input
	arrGPIO[nCH]->GPIO_OEN = 0;
}

UINT GpioGetDir(UINT nCH)
{	// Direction : Gpio direction get to input
	return arrGPIO[nCH]->GPIO_OUT;
}

void GpioSetHi(UINT nCH)
{	// Output Pin Setting : HIGH
	arrGPIO[nCH]->GPIO_OUT = 1;
}

void GpioSetLo(UINT nCH)
{	// Output Pin Setting : LOW
	arrGPIO[nCH]->GPIO_OUT = 0;
}

void GpioFuncPin(UINT nCH)
{	// Multi Function Selection
	_printf("%s Not implemented.(ch:%d)\n", __func__, nCH);
}

void GpioFuncPinOff(UINT nCH)
{	// GPIO Selectin
	_printf("%s Not implemented.(ch:%d)\n", __func__, nCH);
}

UINT GpioGetFuncPin(UINT nCH)
{	// Multi Function Selection
	_printf("%s Not implemented.(ch:%d)\n", __func__, nCH);
	return 0;
}

UINT GpioGetPin(UINT nCH)
{
	return arrGPIO[nCH]->GPIO_IN;
}

void IsrGpio(void *ctx)
{
	for (int i = 0; i < GPIO_CNT; i++) {
		if (arrGPIO[i]->GPIO_IRQ) {
			arrGPIO[i]->GPIO_IRQ_CLR = 1;
			switch (i) {
				default:
					_printf("GPIO Get[%d]\n", i);
					break;
			}
		}
	}
	UNUSED(ctx);
}
