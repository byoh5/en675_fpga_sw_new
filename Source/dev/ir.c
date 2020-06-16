#include "dev.h"

#if USE_IR

ISRD static tIhnd hIRIrq;

void IrInit(UINT nAddr)
{
	IR_EN = 1;
	IR_MARGIN = ((APB_FREQ / 1000) * 1.125) / 2;
	IR_RPT_MG = 100000;
	IR_CLK = APB_FREQ / 1000;
	IR_IRQ_ADDR = nAddr;
	IR_IRQ_EN = 1;

	hIRIrq.irqfn = NULL;
	hIRIrq.arg = NULL;
}

void IrDeInit(void)
{
	IR_IRQ_EN = 0;
	IR_EN = 0;

	hIRIrq.irqfn = NULL;
	hIRIrq.arg = NULL;
}

void IrSetClk(UINT clk)
{
	IR_CLK = clk;
}

UINT IrGetClk(void)
{
	return IR_CLK;
}

void IrSetEn(ENX_SWITCH sw)
{
	IR_EN = sw;
}

UINT IrGetEn(void)
{
	return IR_EN;
}

void IrSetIrqAddr(UINT nAddr)
{
	IR_IRQ_ADDR = nAddr;
}

UINT IrGetIrqAddr(void)
{
	return IR_IRQ_ADDR;
}

void IrSetMargin(UINT nMargin)
{
	IR_MARGIN = nMargin;
}

UINT IrGetMargin(void)
{
	return IR_MARGIN;
}

void IrSetRepeatMargin(UINT nMargin)
{
	IR_RPT_MG = nMargin;
}

UINT IrGetRepeatMargin(void)
{
	return IR_RPT_MG;
}

#if EN675_SINGLE
void IrSetSync(UINT sync)
{
	IR_SYNC = sync;
}

UINT IrGetSync(void)
{
	return IR_SYNC;
}
#endif

UINT IrIsRepeat(void)
{
	return IR_RPT;
}

UINT IrGetAddr(void)
{
	return IR_ADDR;
}

UINT IrGetDat(void)
{
	return IR_DAT;
}

void IrIrqCallback(irq_fn irqfn, void *arg)
{
	hIRIrq.irqfn = irqfn;
	hIRIrq.arg = arg;
}

void IrSetIrqEn(ENX_SWITCH sw)
{
	IR_IRQ_EN = sw;
}

ENX_SWITCH IrGetIrqEn(void)
{
	return IR_IRQ_EN;
}

void IrIrqClear(void)
{
	IR_IRQ_CLR = 1;
}

UINT IrIsIrq(void)
{
	return IR_IRQ;
}

void IrqIr(void)
{
	if (IrIsIrq()) {
		if (hIRIrq.irqfn) {
			hIRIrq.irqfn(hIRIrq.arg);
		} else {
			printf("IR IRQ Get\n");
		}
		IrIrqClear();
	}
}
#else
void IrqIr(void)
{
	_Rprintf("IR IRQ Get! IR is inactive.\n");
	ENX_ASSERT(0);
}
#endif
