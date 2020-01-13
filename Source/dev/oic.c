#include "dev.h"

#if 0
ISRD static tIhnd hOICIrq;

void OICInit(void)
{

}

void OICIrqCallback(irq_fn irqfn, void *arg)
{
	hOICIrq.irqfn = irqfn;
	hOICIrq.arg = arg;
}

void OICSetIrqEn(ENX_SWITCH sw)
{
	OIC_IRQ_EN = sw;
}

ENX_SWITCH OICGetIrqEn(void)
{
	return OIC_IRQ_EN;
}

void OICIrqClear(void)
{
	OIC_IRQ_CLR = 1;
}

UINT OICIsIrq(void)
{
	return OIC_IRQ;
}

void IrqOIC(void)
{
	if (OICIsIrq()) {
		if (hOICIrq.irqfn) {
			hOICIrq.irqfn(hOICIrq.arg);
		} else {
			printf("OIC IRQ Get\n");
		}
		OICIrqClear();
	}
}
#else
void IrqOic(void)
{
	_Rprintf("OIC IRQ Get! OIC is inactive.\n");
	ENX_ASSERT(0);
}
#endif
