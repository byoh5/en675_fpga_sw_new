#include "dev.h"
#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif
#if 1

#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)

ISRD static tIhnd tCHKSUMIrq;

inline void Checksum16_isr_async(BYTE *apbDst, UINT anNum)
{
	CHKSUM_ADR = (intptr_t)apbDst;
	CHKSUM_LEN = anNum;
	CHKSUM_GO = 1;
}

inline WORD Checksum16_isr(BYTE *apbDst, UINT anNum)
{
	Checksum16_isr_async(apbDst, anNum);
	while (CHKSUM_GO);
	return ChksumGetDat();
}

#ifdef __FREERTOS__
void Checksum16_rtos_async(BYTE *apbDst, UINT anNum)
{
	portENTER_CRITICAL();
	Checksum16_isr_async(apbDst, anNum);
	portEXIT_CRITICAL();
}

WORD Checksum16_rtos(BYTE *apbDst, UINT anNum)
{
	portENTER_CRITICAL();
	WORD res = Checksum16_isr(apbDst, anNum);
	portEXIT_CRITICAL();
	return res;
}
#endif

inline WORD ChksumGetDat(void)
{
	return SWAP_BYTES_IN_WORD(CHKSUM_DAT);
}

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
		if (tCHKSUMIrq.irqfn) {
			tCHKSUMIrq.irqfn(tCHKSUMIrq.arg);
		} else {
			printf("Chksum IRQ Get\n");
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

void ChksumRegShow(ENX_YN isDetail)
{
	_Cprintf("CHKSUM Register View\n");
	printf("========================================\n");
	if (isDetail == ENX_YES) {
		_Yprintf(" 0:0x%08X\n", _cm(REG_BASE_CHKSUM, 0));
		printf("   %-20s: %u\n", "IRQ", CHKSUM_IRQ);
		printf("   %-20s: %u\n", "IRQ_EN", CHKSUM_IRQ_EN);
		printf("   %-20s: %u\n", "IRQ_CLR", CHKSUM_IRQ_CLR);
		printf("   %-20s: %u\n", "GO", CHKSUM_GO);
		_Yprintf(" 1:0x%08X\n", _cm(REG_BASE_CHKSUM, 1));
		printf("   %-20s: 0x%08X\n", "ADR", CHKSUM_ADR);
		_Yprintf(" 2:0x%08X\n", _cm(REG_BASE_CHKSUM, 2));
		printf("   %-20s: 0x%08X, %u\n", "LEN", CHKSUM_LEN, CHKSUM_LEN);
		_Yprintf(" 3:0x%08X\n", _cm(REG_BASE_CHKSUM, 3));
		printf("   %-20s: 0x%04X, %u\n", "DAT", CHKSUM_DAT, CHKSUM_DAT);
	} else {
		printf("CHKSUM  0:0x%08X  1:0x%08X  2:0x%08X  3:0x%08X\n", _cm(REG_BASE_CHKSUM, 0), _cm(REG_BASE_CHKSUM, 1), _cm(REG_BASE_CHKSUM, 2), _cm(REG_BASE_CHKSUM, 3));
	}
	printf("========================================\n");
}
