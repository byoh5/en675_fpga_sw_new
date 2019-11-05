#include "dev.h"
#include "pmp.h"

inline static int ispow2(ULONG val)
{
	return val && !(val & (val-1));
}

void setup_pmp(void)
{
	// Set up a PMP to permit access to all of memory.
	// Ignore the illegal-instruction trap if PMPs aren't supported.
	ULONG pmpc = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
	asm volatile ("la t0, 1f\n\t"
					"csrrw t0, mtvec, t0\n\t"
					"csrw pmpaddr0, %1\n\t"
					"csrw pmpcfg0, %0\n\t"
					".align 2\n\t"
					"1: csrw mtvec, t0"
					: : "r" (pmpc), "r" (-1UL) : "t0");
}

static int pmp_entry_set_pow2(unsigned n, BYTE prot, ULONG addr, ULONG len)
{
	if (__riscv_xlen != 64) return -1;		// only for RV64

	/* calculate PMP config register and offset */
	int pmpcfg_shift = (n & 7) << 3;

	/* encode config */
	prot |= (len == 4) ? PMP_NA4 : PMP_NAPOT;
	ULONG pmpcfg = ((ULONG)prot) << pmpcfg_shift;
	ULONG cfgmask = ~(0xfful << pmpcfg_shift);

	/* encode address */
	ULONG pmpmask = (len>>PMP_SHIFT)-1;
	ULONG pmpaddr = ((addr >> PMP_SHIFT) & ~pmpmask) | (pmpmask >> 1);

	/* write csrs */
	pmpcfg |= (read_csr(pmpcfg0) & cfgmask) | pmpcfg;		// only for pmp entry 0~7

	switch(n) {
	case 0:	write_csr(pmpaddr0, pmpaddr); break;
	case 1:	write_csr(pmpaddr1, pmpaddr); break;
	case 2:	write_csr(pmpaddr2, pmpaddr); break;
	case 3:	write_csr(pmpaddr3, pmpaddr); break;
	case 4:	write_csr(pmpaddr4, pmpaddr); break;
	case 5:	write_csr(pmpaddr5, pmpaddr); break;
	case 6:	write_csr(pmpaddr6, pmpaddr); break;
	case 7:	write_csr(pmpaddr7, pmpaddr); break;
	default: break;
	}

	write_csr(pmpcfg0, pmpcfg);

	return 0;
}

int pmp_entry_set(unsigned n, BYTE prot, ULONG addr, ULONG len)
{
	/* check parameters */
	if (n >= PMPADDR_COUNT || (len < 4) || (!ispow2(len)) || (addr%len)) {
		printf("pmp_entry_set error : n=%d, addr=0x%lx, len=0x%lx\n", n, addr, len);
		return -1;
	}

	return pmp_entry_set_pow2(n, prot, addr, len);
}
