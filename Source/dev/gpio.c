#include "dev.h"

static _GPIO_PIN *arrGPIO[GPIO_CNT];
static _GPIO_MUX *arrGPIOMUX[GPIO_CNT];
static tIhnd arrGPIOIrq[GPIO_CNT];

void GpioInit(void)
{
	for (uint64_t i = 0; i < GPIO_CNT; i++) {
		arrGPIO[i] = (_GPIO_PIN *)(REG_BASE_GPIO + (i << 3));
		arrGPIOMUX[i] = (_GPIO_MUX *)(REG_BASE_SYS + ((256 + i) << 3));
		arrGPIOIrq[i].irqfn = NULL;
		arrGPIOIrq[i].arg = NULL;
	}
	printf("GPIO Init\n");
}

void GpioRiseEdge(UINT nCH)
{	// Direction : Gpio rising edge sel
	arrGPIO[nCH]->GPIO_IRQ_DIR = 1;
}

void GpioFallEdge(UINT nCH)
{	// Direction : Gpio falling edge sel
	arrGPIO[nCH]->GPIO_IRQ_DIR = 0;
}

void GpioBothEdge(UINT nCH)
{	// Direction : Gpio Both edge sel
	arrGPIO[nCH]->GPIO_IRQ_DIR = 2;
}

UINT GpioGetEdge(UINT nCH)
{	// Direction : Gpio direction get to input
	return arrGPIO[nCH]->GPIO_IRQ_DIR;
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
	return arrGPIO[nCH]->GPIO_OEN;
}

void GpioSetHi(UINT nCH)
{	// Output Pin Setting : HIGH
	arrGPIO[nCH]->GPIO_OUT = 1;
}

void GpioSetLo(UINT nCH)
{	// Output Pin Setting : LOW
	arrGPIO[nCH]->GPIO_OUT = 0;
}

UINT GpioGetOut(UINT nCH)
{
	return arrGPIO[nCH]->GPIO_OUT;
}

void GpioFuncPin(UINT nCH, UINT nSel)
{	// Multi Function Selection
	if (nSel > 3) {
		printf("Error GPIO%02u Function Selection(%u)\n", nCH, nSel);
	} else {
		arrGPIOMUX[nCH]->GPIO_MUX = nSel;
	}
}

void GpioFuncPinOff(UINT nCH)
{	// GPIO Selectin
	arrGPIOMUX[nCH]->GPIO_MUX = 0;
}

UINT GpioGetFuncPin(UINT nCH)
{	// Multi Function Selection
	return arrGPIOMUX[nCH]->GPIO_MUX;
}

UINT GpioGetPin(UINT nCH)
{
	return arrGPIO[nCH]->GPIO_IN;
}

void GpioIrqCallback(UINT nCH, irq_fn irqfn, void *arg)
{
	arrGPIOIrq[nCH].irqfn = irqfn;
	arrGPIOIrq[nCH].arg = arg;
}

void GpioIrqOn(UINT nCH)
{	// Direction : Gpio interrupt enable
	arrGPIO[nCH]->GPIO_IRQ_EN = 1;
}

void GpioIrqOff(UINT nCH)
{	// Direction : Gpio interrupt disable
	arrGPIO[nCH]->GPIO_IRQ_EN = 0;
}

UINT GpioGetIrqEn(UINT nCH)
{
	return arrGPIO[nCH]->GPIO_IRQ_EN;
}

void GpioIrqClear(UINT nCH)
{
	arrGPIO[nCH]->GPIO_IRQ_CLR = 1;
}

UINT GpioIsIrq(UINT nCH)
{
	return arrGPIO[nCH]->GPIO_IRQ;
}

void IrqGpio(UINT nCH)
{
	if (GpioIsIrq(nCH)) {
		_printf("GPIO%d IRQ Get\n", nCH);
		if (arrGPIOIrq[nCH].irqfn) {
			arrGPIOIrq[nCH].irqfn(arrGPIOIrq[nCH].arg);
		}
		GpioIrqClear(nCH);
	}
}
