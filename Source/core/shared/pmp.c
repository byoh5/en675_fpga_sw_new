#include "dev.h"
#include "pmp.h"

inline static int ispow2(ULONG val)
{
    return val && !(val & (val-1));
}

void clear_pmp(void)
{
	write_csr(pmpcfg0, 0x0UL);
	write_csr(pmpaddr0, 0x0UL);
	write_csr(pmpaddr1, 0x0UL);
	write_csr(pmpaddr2, 0x0UL);
	write_csr(pmpaddr3, 0x0UL);
	write_csr(pmpaddr4, 0x0UL);
	write_csr(pmpaddr5, 0x0UL);
	write_csr(pmpaddr6, 0x0UL);
	write_csr(pmpaddr7, 0x0UL);
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

    printf("pmp_entry_set_pow2 : n=%d, addr=0x%lx, len=%d, pmpcfg_shift=%d, cfgmask=0x%016lx, pmpcfg=0x%016lx, pmpmask=0x%016lx, pmpaddr=0x%016lx\n", n, addr, len, pmpcfg_shift, cfgmask, pmpcfg, pmpmask, pmpaddr);

    /* write csrs */
    printf("pmpcfg0=0x%016lx, pmpaddr0=0x%016lx, pmpaddr1=0x%016lx, pmpaddr2=0x%016lx, pmpaddr3=0x%016lx\n", read_csr(pmpcfg0), read_csr(pmpaddr0), read_csr(pmpaddr1), read_csr(pmpaddr2), read_csr(pmpaddr3));

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

    printf("pmpcfg0=0x%016lx, pmpaddr0=0x%016lx, pmpaddr1=0x%016lx, pmpaddr2=0x%016lx, pmpaddr3=0x%016lx\n", read_csr(pmpcfg0), read_csr(pmpaddr0), read_csr(pmpaddr1), read_csr(pmpaddr2), read_csr(pmpaddr3));

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

inline ULONG set_field(ULONG reg, ULONG mask, ULONG val)
{
    return ((reg & ~mask) | ((val * (mask & ~(mask << 1))) & mask));
}

/*
 * mode_set_and_continue
 *
 * Set mstatus.mpp, sets mepc to instruction after mret and then issues mret
 * Note: the hart will continue running on the same stack
 */
void mode_set_and_continue(unsigned mode)
{
    write_csr(mstatus, set_field(read_csr(mstatus), MSTATUS_MPP, mode));
    asm volatile (
        "lla t0, 1f\n"
        "csrw mepc, t0\n"
        "mret\n"
        "1:"
        ::: "t0"
    );
}
