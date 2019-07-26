#include "dev.h"
#include "enx_freertos.h"
#if 1

#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)

ISRD static tIhnd tCHKSUMIrq;

WORD Checksum16(BYTE *apbDst, UINT anNum)
{
	CHKSUM_ADR = (intptr_t)apbDst;
	CHKSUM_LEN = anNum;
	CHKSUM_GO = 1;
	while (CHKSUM_GO);
	return SWAP_BYTES_IN_WORD(CHKSUM_DAT);
}

#ifdef __FREERTOS__
WORD Checksum16_rtos(BYTE *apbDst, UINT anNum)
{
	portENTER_CRITICAL();
	CHKSUM_ADR = (intptr_t)apbDst;
	CHKSUM_LEN = anNum;
	CHKSUM_GO = 1;
	while (CHKSUM_GO);
	WORD res = SWAP_BYTES_IN_WORD(CHKSUM_DAT);
	portEXIT_CRITICAL();
	return res;
}
#endif

void ChksumIrqCallback(irq_fn irqfn, void *arg)
{
	tCHKSUMIrq.irqfn = irqfn;
	tCHKSUMIrq.arg = arg;
}

void ChksumSetIrqEn(ENX_SWITCH sw)
{	// Interrupt enable/disable
	CHKSUM_IRQ_EN = sw;
}

ENX_SWITCH ChksumGetIrqEn(void)
{
	return CHKSUM_IRQ_EN;
}

void ChksumIrqClear(void)
{
	CHKSUM_IRQ_CLR = 1;
}

UINT ChksumIsIrq(void)
{
	return CHKSUM_IRQ;
}

void IrqChksum(void)
{
	if (ChksumIsIrq()) {
		_printf("Chksum IRQ Get\n");
		if (tCHKSUMIrq.irqfn) {
			tCHKSUMIrq.irqfn(tCHKSUMIrq.arg);
		}
		ChksumIrqClear();
	}
}
#else
void IrqChksum(void)
{
	_Rprintf("Chksum IRQ Get! Chksum is inactive.\n");
	ENX_ASSERT(0);
}
#endif
