#include "dev.h"

static volatile _GPIO_PIN * const arrGPIO[GPIO_CNT]    = {(_GPIO_PIN *)(REG_BASE_GPIO+(0<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(1<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(2<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(3<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(4<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(5<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(6<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(7<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(8<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(9<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(10<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(11<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(12<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(13<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(14<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(15<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(16<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(17<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(18<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(19<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(20<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(21<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(22<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(23<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(24<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(25<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(26<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(27<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(28<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(29<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(30<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(31<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(32<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(33<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(34<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(35<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(36<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(37<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(38<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(39<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(40<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(41<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(42<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(43<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(44<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(45<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(46<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(47<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(48<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(49<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(50<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(51<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(52<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(53<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(54<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(55<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(56<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(57<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(58<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(59<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(60<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(61<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(62<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(63<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(64<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(65<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(66<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(67<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(68<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(69<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(70<<3)), (_GPIO_PIN *)(REG_BASE_GPIO+(71<<3))};
static volatile _GPIO_MUX * const arrGPIOMUX[GPIO_CNT] = {(_GPIO_MUX *)(REG_BASE_SYS+((256+0)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+1)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+2)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+3)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+4)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+5)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+6)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+7)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+8)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+9)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+10)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+11)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+12)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+13)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+14)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+15)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+16)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+17)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+18)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+19)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+20)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+21)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+22)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+23)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+24)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+25)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+26)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+27)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+28)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+29)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+30)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+31)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+32)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+33)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+34)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+35)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+36)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+37)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+38)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+39)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+40)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+41)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+42)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+43)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+44)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+45)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+46)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+47)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+48)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+49)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+50)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+51)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+52)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+53)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+54)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+55)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+56)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+57)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+58)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+59)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+60)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+61)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+62)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+63)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+64)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+65)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+66)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+67)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+68)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+69)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+70)<<3)), (_GPIO_MUX *)(REG_BASE_SYS+((256+71)<<3))};
ISRD static tIhnd arrGPIOIrq[GPIO_CNT];

void GpioInit(void)
{
	for (uint64_t i = 0; i < GPIO_CNT; i++) {
		arrGPIOIrq[i].irqfn = NULL;
		arrGPIOIrq[i].arg = NULL;
	}
	ENX_DEBUGF(DBG_GPIO_LOG, "GPIO Init\n");
}

void GpioSetEdge(UINT nCH, GPIO_EDGE edge)
{	// Direction : Gpio rising/falling/both edge sel
	arrGPIO[nCH]->GPIO_IRQ_DIR = edge;
}

GPIO_EDGE GpioGetEdge(UINT nCH)
{	// Direction : Gpio direction get to input
	return arrGPIO[nCH]->GPIO_IRQ_DIR;
}

void GpioSetDir(UINT nCH, GPIO_DIR dir)
{	// Direction : Gpio direction set to input/output, 0:output, 1:input
	arrGPIO[nCH]->GPIO_OEN = dir;
}

GPIO_DIR GpioGetDir(UINT nCH)
{	// Direction : Gpio direction get to input
	return arrGPIO[nCH]->GPIO_OEN;
}

void GpioSetOut(UINT nCH, GPIO_OUT out)
{	// Output Pin Setting : high / low
	arrGPIO[nCH]->GPIO_OUT = out;
}

GPIO_OUT GpioGetOut(UINT nCH)
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

void GpioSetIrqEn(UINT nCH, ENX_SWITCH sw)
{	// Direction : Gpio interrupt enable/disable
	arrGPIO[nCH]->GPIO_IRQ_EN = sw;
}

ENX_SWITCH GpioGetIrqEn(UINT nCH)
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
		ENX_DEBUGF(DBG_GPIO_LOG, "GPIO%d IRQ Get\n", nCH);
		if (arrGPIOIrq[nCH].irqfn) {
			arrGPIOIrq[nCH].irqfn(arrGPIOIrq[nCH].arg);
		}
		GpioIrqClear(nCH);
	}
}
