#include "dev.h"
#include "syscall.h"

#define IRQ_PRIO1_BASE				0x0C000004 // RW: Source 1 priority (ISP)
#define IRQ_PRIO2_BASE				0x0C000008 // RW: Source 2 priority (DMA, H.264, H.265)
#define IRQ_PRIO3_BASE				0x0C00000C // RW: Source 3 priority (USB, I2S, ETH)
#define IRQ_PRIO4_BASE				0x0C000010 // RW: Source 4 priority (SDIO)
#define IRQ_PRIO5_BASE				0x0C000014 // RW: Source 5 priority (AES, SHA, CHKSUM)
#define IRQ_PRIO6_BASE				0x0C000018 // RW: Source 6 priority (UART, SPI, I2C)
#define IRQ_PRIO7_BASE				0x0C00001C // RW: Source 7 priority (GPIO)
#define IRQ_PRIO8_BASE				0x0C000020 // RW: Source 8 priority (TIMER)

#define IRQ_PENDING_BASE			0x0C001000 // R: Interrupt pending bits

#define IRQ_ENABLES1_BASE			0x0C002000 // RW: Hart 0 M-mode enables
#define IRQ_ENABLES2_BASE			0x0C002080 // RW: Hart 0 S-mode enables
#define IRQ_ENABLES3_BASE			0x0C002100 // RW: Hart 1 M-mode enables
#define IRQ_ENABLES4_BASE			0x0C002180 // RW: Hart 1 S-mode enables
#define IRQ_ENABLES5_BASE			0x0C002200 // RW: Hart 2 M-mode enables
#define IRQ_ENABLES6_BASE			0x0C002280 // RW: Hart 2 S-mode enables
#define IRQ_ENABLES7_BASE			0x0C002300 // RW: Hart 3 M-mode enables
#define IRQ_ENABLES8_BASE			0x0C002380 // RW: Hart 3 S-mode enables

#define IRQ_THRESHOLD1_BASE			0x0C200000 // RW: Hart 0 M-mode priority threshold
#define IRQ_THRESHOLD2_BASE			0x0C201000 // RW: Hart 0 S-mode priority threshold
#define IRQ_THRESHOLD3_BASE			0x0C202000 // RW: Hart 1 M-mode priority threshold
#define IRQ_THRESHOLD4_BASE			0x0C203000 // RW: Hart 1 S-mode priority threshold
#define IRQ_THRESHOLD5_BASE			0x0C204000 // RW: Hart 2 M-mode priority threshold
#define IRQ_THRESHOLD6_BASE			0x0C205000 // RW: Hart 2 S-mode priority threshold
#define IRQ_THRESHOLD7_BASE			0x0C206000 // RW: Hart 3 M-mode priority threshold
#define IRQ_THRESHOLD8_BASE			0x0C207000 // RW: Hart 3 S-mode priority threshold

#define IRQ_CLAIM_COMPLIET1_BASE	0x0C200004 // RW: Hart 0 M-mode (R)claim/(W)complete
#define IRQ_CLAIM_COMPLIET2_BASE	0x0C201004 // RW: Hart 0 S-mode (R)claim/(W)complete
#define IRQ_CLAIM_COMPLIET3_BASE	0x0C202004 // RW: Hart 1 M-mode (R)claim/(W)complete
#define IRQ_CLAIM_COMPLIET4_BASE	0x0C203004 // RW: Hart 1 S-mode (R)claim/(W)complete
#define IRQ_CLAIM_COMPLIET5_BASE	0x0C204004 // RW: Hart 2 M-mode (R)claim/(W)complete
#define IRQ_CLAIM_COMPLIET6_BASE	0x0C205004 // RW: Hart 2 S-mode (R)claim/(W)complete
#define IRQ_CLAIM_COMPLIET7_BASE	0x0C206004 // RW: Hart 3 M-mode (R)claim/(W)complete
#define IRQ_CLAIM_COMPLIET8_BASE	0x0C207004 // RW: Hart 3 S-mode (R)claim/(W)complete

static volatile unsigned int *iPrioBase[8] = {
		(unsigned int *)IRQ_PRIO1_BASE,
		(unsigned int *)IRQ_PRIO2_BASE,
		(unsigned int *)IRQ_PRIO3_BASE,
		(unsigned int *)IRQ_PRIO4_BASE,
		(unsigned int *)IRQ_PRIO5_BASE,
		(unsigned int *)IRQ_PRIO6_BASE,
		(unsigned int *)IRQ_PRIO7_BASE,
		(unsigned int *)IRQ_PRIO8_BASE,
};

static volatile unsigned int *iPanding = (unsigned int *)IRQ_PENDING_BASE;

static volatile unsigned int *iEnables[8] = {
		(unsigned int *)IRQ_ENABLES1_BASE,
		(unsigned int *)IRQ_ENABLES2_BASE,
		(unsigned int *)IRQ_ENABLES3_BASE,
		(unsigned int *)IRQ_ENABLES4_BASE,
		(unsigned int *)IRQ_ENABLES5_BASE,
		(unsigned int *)IRQ_ENABLES6_BASE,
		(unsigned int *)IRQ_ENABLES7_BASE,
		(unsigned int *)IRQ_ENABLES8_BASE,
};

static volatile unsigned int *iThreshold[8] = {
		(unsigned int *)IRQ_THRESHOLD1_BASE,
		(unsigned int *)IRQ_THRESHOLD2_BASE,
		(unsigned int *)IRQ_THRESHOLD3_BASE,
		(unsigned int *)IRQ_THRESHOLD4_BASE,
		(unsigned int *)IRQ_THRESHOLD5_BASE,
		(unsigned int *)IRQ_THRESHOLD6_BASE,
		(unsigned int *)IRQ_THRESHOLD7_BASE,
		(unsigned int *)IRQ_THRESHOLD8_BASE,
};

static volatile unsigned int *iClaimCompliet[8] = {
		(unsigned int *)IRQ_CLAIM_COMPLIET1_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET2_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET3_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET4_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET5_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET6_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET7_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET8_BASE,
};

void enx_exirq_source1(void)
{
	if (IRQ_ISP) {
		printf("IRQ_ISP\n");
		if (IRQ_ISP0){};
		if (IRQ_ISP1){};
		if (IRQ_ISP2){};
		if (IRQ_ISP3){};
		if (IRQ_ISP4){};
		if (IRQ_ISP5){};
		if (IRQ_ISP6){};
		if (IRQ_ISP7){};
		if (IRQ_ISP8){};
		if (IRQ_ISP9){};
		if (IRQ_ISP10){};
		if (IRQ_ISP11){};
		if (IRQ_ISP12){};
		if (IRQ_ISP13){};
		if (IRQ_ISP14){};
		if (IRQ_ISP15){};
		if (IRQ_ISP16){};
		if (IRQ_ISP17){};
		if (IRQ_ISP18){};
		if (IRQ_ISP19){};
		if (IRQ_ISP20){};
		if (IRQ_ISP21){};
		if (IRQ_ISP22){};
		if (IRQ_ISP23){};
		if (IRQ_ISP24){};
		if (IRQ_ISP25){};
		if (IRQ_ISP26){};
		if (IRQ_ISP27){};
		if (IRQ_ISP28){};
		if (IRQ_ISP29){};
		if (IRQ_ISP30){};
	}
}

void enx_exirq_source2(void)
{
	if (IRQ_CDC) {
		if (IRQ_DMA){IrqDma();};
		if (IRQ_H265){printf("IRQ_H265\n");};
		if (IRQ_H264){printf("IRQ_H264\n");};
	}
}

void enx_exirq_source3(void)
{
	if (IRQ_ETH) {
		if (IRQ_USB_DMA){printf("IRQ_USB_DMA\n");};
		if (IRQ_USB_MC){printf("IRQ_USB_MC\n");};
		if (IRQ_I2S_RX){printf("IRQ_I2S_RX\n");};
		if (IRQ_I2S_TX){printf("IRQ_I2S_TX\n");};
		if (IRQ_ETH_RX){printf("IRQ_ETH_RX\n");};
		if (IRQ_ETH_TX){printf("IRQ_ETH_TX\n");};
	}
}

void enx_exirq_source4(void)
{
	if (IRQ_SDIO) {
		if (IRQ_SDIO0){IrqSdio(0);};
		if (IRQ_SDIO1){IrqSdio(1);};
	}
}

void enx_exirq_source5(void)
{
	if (IRQ_UTIL) {
		if (IRQ_AES){printf("IRQ_AES\n");};
		if (IRQ_SHA){printf("IRQ_SHA\n");};
		if (IRQ_CHKSUM){printf("IRQ_CHKSUM\n");};
	}
}

void enx_exirq_source6(void)
{
	if (IRQ_UART) {
		if (IRQ_UART0){IrqUart(0);};
		if (IRQ_UART1){IrqUart(1);};
		if (IRQ_UART2){IrqUart(2);};
		if (IRQ_UART3){IrqUart(3);};
		if (IRQ_UART4){IrqUart(4);};
		if (IRQ_UART5){IrqUart(5);};
		if (IRQ_UART6){IrqUart(6);};
		if (IRQ_UART7){IrqUart(7);};
		if (IRQ_UART8){IrqUart(8);};
	}
	if (IRQ_SPI) {
		if (IRQ_SPI0){IrqSpi(0);};
		if (IRQ_SPI1){IrqSpi(1);};
		if (IRQ_SPI2){IrqSpi(2);};
		if (IRQ_SPI3){IrqSpi(3);};
		if (IRQ_SPI4){IrqSpi(4);};
		if (IRQ_SPI5){IrqSpi(5);};
		if (IRQ_SPI6){IrqSpi(6);};
		if (IRQ_SPI7){IrqSpi(7);};
		if (IRQ_SPI8){IrqSpi(8);};
	}
	if (IRQ_I2C) {
		if (IRQ_I2C0){IrqI2c(0);};
		if (IRQ_I2C1){IrqI2c(1);};
		if (IRQ_I2C2){IrqI2c(2);};
		if (IRQ_I2C3){IrqI2c(3);};
		if (IRQ_I2C4){IrqI2c(4);};
		if (IRQ_I2C5){IrqI2c(5);};
		if (IRQ_I2C6){IrqI2c(6);};
		if (IRQ_I2C7){IrqI2c(7);};
		if (IRQ_I2C8){IrqI2c(8);};
	}
}

void enx_exirq_source7(void)
{
	if (IRQ_GPIO_G2) {
		if (IRQ_GPIO71){IrqGpio(71);}
		if (IRQ_GPIO70){IrqGpio(70);}
		if (IRQ_GPIO69){IrqGpio(69);}
		if (IRQ_GPIO68){IrqGpio(68);}
		if (IRQ_GPIO67){IrqGpio(67);}
		if (IRQ_GPIO66){IrqGpio(66);}
		if (IRQ_GPIO65){IrqGpio(65);}
		if (IRQ_GPIO64){IrqGpio(64);}
		if (IRQ_GPIO63){IrqGpio(63);}
		if (IRQ_GPIO62){IrqGpio(62);}
		if (IRQ_GPIO61){IrqGpio(61);}
		if (IRQ_GPIO60){IrqGpio(60);}
		if (IRQ_GPIO59){IrqGpio(59);}
		if (IRQ_GPIO58){IrqGpio(58);}
		if (IRQ_GPIO57){IrqGpio(57);}
		if (IRQ_GPIO56){IrqGpio(56);}
		if (IRQ_GPIO55){IrqGpio(55);}
		if (IRQ_GPIO54){IrqGpio(54);}
		if (IRQ_GPIO53){IrqGpio(53);}
		if (IRQ_GPIO52){IrqGpio(52);}
		if (IRQ_GPIO51){IrqGpio(51);}
		if (IRQ_GPIO50){IrqGpio(50);}
		if (IRQ_GPIO49){IrqGpio(49);}
		if (IRQ_GPIO48){IrqGpio(48);}
	}
	if (IRQ_GPIO_G1) {
		if (IRQ_GPIO47){IrqGpio(47);}
		if (IRQ_GPIO46){IrqGpio(46);}
		if (IRQ_GPIO45){IrqGpio(45);}
		if (IRQ_GPIO44){IrqGpio(44);}
		if (IRQ_GPIO43){IrqGpio(43);}
		if (IRQ_GPIO42){IrqGpio(42);}
		if (IRQ_GPIO41){IrqGpio(41);}
		if (IRQ_GPIO40){IrqGpio(40);}
		if (IRQ_GPIO39){IrqGpio(39);}
		if (IRQ_GPIO38){IrqGpio(38);}
		if (IRQ_GPIO37){IrqGpio(37);}
		if (IRQ_GPIO36){IrqGpio(36);}
		if (IRQ_GPIO35){IrqGpio(35);}
		if (IRQ_GPIO34){IrqGpio(34);}
		if (IRQ_GPIO33){IrqGpio(33);}
		if (IRQ_GPIO32){IrqGpio(32);}
		if (IRQ_GPIO31){IrqGpio(31);}
		if (IRQ_GPIO30){IrqGpio(30);}
		if (IRQ_GPIO29){IrqGpio(29);}
		if (IRQ_GPIO28){IrqGpio(28);}
		if (IRQ_GPIO27){IrqGpio(27);}
		if (IRQ_GPIO26){IrqGpio(26);}
		if (IRQ_GPIO25){IrqGpio(25);}
		if (IRQ_GPIO24){IrqGpio(24);}
	}
	if (IRQ_GPIO_G0) {
		if (IRQ_GPIO23){IrqGpio(23);}
		if (IRQ_GPIO22){IrqGpio(22);}
		if (IRQ_GPIO21){IrqGpio(21);}
		if (IRQ_GPIO20){IrqGpio(20);}
		if (IRQ_GPIO19){IrqGpio(19);}
		if (IRQ_GPIO18){IrqGpio(18);}
		if (IRQ_GPIO17){IrqGpio(17);}
		if (IRQ_GPIO16){IrqGpio(16);}
		if (IRQ_GPIO15){IrqGpio(15);}
		if (IRQ_GPIO14){IrqGpio(14);}
		if (IRQ_GPIO13){IrqGpio(13);}
		if (IRQ_GPIO12){IrqGpio(12);}
		if (IRQ_GPIO11){IrqGpio(11);}
		if (IRQ_GPIO10){IrqGpio(10);}
		if (IRQ_GPIO9){IrqGpio(9);}
		if (IRQ_GPIO8){IrqGpio(8);}
		if (IRQ_GPIO7){IrqGpio(7);}
		if (IRQ_GPIO6){IrqGpio(6);}
		if (IRQ_GPIO5){IrqGpio(5);}
		if (IRQ_GPIO4){IrqGpio(4);}
		if (IRQ_GPIO3){IrqGpio(3);}
		if (IRQ_GPIO2){IrqGpio(2);}
		if (IRQ_GPIO1){IrqGpio(1);}
		if (IRQ_GPIO0){IrqGpio(0);}
	}
}

void enx_exirq_source8(void)
{
	if (IRQ_TIMER_G1) {
		if (IRQ_TIMER38){IrqTimer(38);};
		if (IRQ_TIMER37){IrqTimer(37);};
		if (IRQ_TIMER36){IrqTimer(36);};
		if (IRQ_TIMER35){IrqTimer(35);};
		if (IRQ_TIMER34){IrqTimer(34);};
		if (IRQ_TIMER33){IrqTimer(33);};
		if (IRQ_TIMER32){IrqTimer(32);};
		if (IRQ_TIMER31){IrqTimer(31);};
		if (IRQ_TIMER30){IrqTimer(30);};
		if (IRQ_TIMER29){IrqTimer(29);};
		if (IRQ_TIMER28){IrqTimer(28);};
		if (IRQ_TIMER27){IrqTimer(27);};
		if (IRQ_TIMER26){IrqTimer(26);};
		if (IRQ_TIMER25){IrqTimer(25);};
		if (IRQ_TIMER24){IrqTimer(24);};
		if (IRQ_TIMER23){IrqTimer(23);};
		if (IRQ_TIMER22){IrqTimer(22);};
		if (IRQ_TIMER21){IrqTimer(21);};
		if (IRQ_TIMER20){IrqTimer(20);};
	}
	if (IRQ_TIMER_G0) {
		if (IRQ_TIMER19){IrqTimer(19);};
		if (IRQ_TIMER18){IrqTimer(18);};
		if (IRQ_TIMER17){IrqTimer(17);};
		if (IRQ_TIMER16){IrqTimer(16);};
		if (IRQ_TIMER15){IrqTimer(15);};
		if (IRQ_TIMER14){IrqTimer(14);};
		if (IRQ_TIMER13){IrqTimer(13);};
		if (IRQ_TIMER12){IrqTimer(12);};
		if (IRQ_TIMER11){IrqTimer(11);};
		if (IRQ_TIMER10){IrqTimer(10);};
		if (IRQ_TIMER9){IrqTimer(9);};
		if (IRQ_TIMER8){IrqTimer(8);};
		if (IRQ_TIMER7){IrqTimer(7);};
		if (IRQ_TIMER6){IrqTimer(6);};
		if (IRQ_TIMER5){IrqTimer(5);};
		if (IRQ_TIMER4){IrqTimer(4);};
		if (IRQ_TIMER3){IrqTimer(3);};
		if (IRQ_TIMER2){IrqTimer(2);};
		if (IRQ_TIMER1){IrqTimer(1);};
		if (IRQ_TIMER0){IrqTimer(0);};
	}
}

#if 0
void IrqStatus(void)
{
	unsigned long mstatus = read_csr(mstatus);
	unsigned long mie = read_csr(mie);
	unsigned long mip = read_csr(mip);
	unsigned long mideleg = read_csr(mideleg);
	unsigned long mcause = read_csr(mcause);

	_printf("mstatus : %lX\n", mstatus);
	_printf("mie     : %lX\n", mie);
	_printf("mip     : %lX\n", mip);
	_printf("mideleg : %lX\n", mideleg);
	_printf("mcause  : %lX\n", mcause);

	_printf("IRQ Priority\n\t");
	for (int i = 0; i < 8; i++) {
		_printf("[0x%08X,%8bb]", iPrioBase[i], *iPrioBase[i]);
		if (i==3) {
			_printf("\n\t");
		}
	}
	_printf("\n");
	_printf("IRQ Pending\n\t[0x%08X,%8bb]\n", iPanding, *iPanding);
	_printf("IRQ Enables\n\t");
	for (int i = 0; i < 8; i++) {
		_printf("[0x%08X,%8bb]", iEnables[i], *iEnables[i]);
		if (i==3) {
			_printf("\n\t");
		}
	}
	_printf("\n");
	_printf("IRQ Threshold\n\t");
	for (int i = 0; i < 8; i++) {
		_printf("[0x%08X,%8bb]", iThreshold[i], *iThreshold[i]);
		if (i==3) {
			_printf("\n\t");
		}
	}
	_printf("\n");
	_printf("IRQ ClaimCompliet\n\t");
	for (int i = 0; i < 8; i++) {
		UINT source = *iClaimCompliet[i];
		_printf("[0x%08X,%8bb]", iClaimCompliet[i], source);
		*iClaimCompliet[i] = source;
		if (i==3) {
			_printf("\n\t");
		}
	}
	_printf("\n");

	_printf("======================================================\n");
}
#endif

static void __attribute__((noreturn)) bad_trap(uintptr_t mcause, uintptr_t mepc, uintptr_t regs[32])
{
	switch (mcause) {
	case CAUSE_MISALIGNED_FETCH:	_printf("Instruction address misaligned\n");	break;
	case CAUSE_FETCH_ACCESS:		_printf("Instruction access fault\n");			break;
	case CAUSE_ILLEGAL_INSTRUCTION:	_printf("Illegal instruction\n");				break;
	case CAUSE_BREAKPOINT:			_printf("Breakpoint\n");						break;
	case CAUSE_MISALIGNED_LOAD:		_printf("Load address misaligned\n");			break;
	case CAUSE_LOAD_ACCESS:			_printf("Load access fault\n");					break;
	case CAUSE_MISALIGNED_STORE:	_printf("Store/AMO address misaligned\n");		break;
	case CAUSE_STORE_ACCESS:		_printf("Store/AMO access fault\n");			break;
	//case 10:						_printf("Reserved\n");							break;
	case CAUSE_FETCH_PAGE_FAULT:	_printf("Instruction page fault\n");			break;
	case CAUSE_LOAD_PAGE_FAULT:		_printf("Load page fault\n");					break;
	//case 14:						_printf("Reserved\n");							break;
	case CAUSE_STORE_PAGE_FAULT:	_printf("Store/AMO page fault\n");				break;
	default:						_printf("Reserved(%d)\n", mcause);				break;
	}
	_printf("machine mode: unhandlable trap %d @ %p\n", read_csr(mcause), mepc);
	while (1);
}

uintptr_t trap_from_machine_mode(uintptr_t mcause, uintptr_t mepc, uintptr_t regs[32])
{
	if ((mcause & 0x8000000000000000) != 0x0) {
		mcause -= 0x8000000000000000;
		switch (mcause) {
		case IRQ_M_TIMER:
		    if (mtime && timecmp) {
		    	int cpu_id = read_csr(mhartid);
		    	//timecmp[cpu_id] = *mtime + TIMECMP_NEXT_VAL; // 1ms
		    	timecmp[cpu_id] = timecmp[cpu_id] + TIMECMP_NEXT_VAL; // 1ms
		    	static uint64_t gbTickCnt = TIME_TICK - 1;
				if (gbTickCnt == 0) {
					gbTickCnt = TIME_TICK - 1;
					gptMsgShare.TIME++;
					gptMsgShare.UPTIME++;
				} else {
					gbTickCnt--;
				}
		    }
			break;

		case IRQ_M_EXT:
			for (int i = 0; i < 8; i++) {
				volatile unsigned int source = *iClaimCompliet[i]; // Get Claim IRQ
				if (source != 0) {
					_printf("CPU%d(%c) - IRQ%d\n", i/2, i%2==0 ? 'M':'S', i);
				}
				switch (source) {
				case 1:	enx_exirq_source1();	break;
				case 2:	enx_exirq_source2();	break;
				case 3:	enx_exirq_source3();	break;
				case 4:	enx_exirq_source4();	break;
				case 5:	enx_exirq_source5();	break;
				case 6:	enx_exirq_source6();	break;
				case 7:	enx_exirq_source7();	break;
				case 8:	enx_exirq_source8();	break;
				}
				*iClaimCompliet[i] = source; // Set Complete IRQ
			}
			break;

		default:
			_printf("IRQ.1 0x%lX\n", mcause);
			break;
		}
		return mepc;
	} else {
		switch(mcause) {
		case CAUSE_USER_ECALL:
		case CAUSE_SUPERVISOR_ECALL:
		case CAUSE_MACHINE_ECALL:
			regs[10] = do_syscall(regs[10], regs[11], regs[12], regs[13], regs[14], regs[15], regs[17]);
			break;

		default:
			bad_trap(mcause, mepc, regs);
			break;
		}
		return mepc + 4;
	}
}

void enx_externalirq_perl(eIRQ_GROUP_INDEX perlIdx, uint64_t onoff, uint64_t type)
{
	if (type > 1) {
		_printf("Error type(%lu) (M-mode:0 S-mode:1)\n", type);
		return;
	}

	if (onoff) {
		*iPrioBase[perlIdx] = 1;
	} else {
		*iPrioBase[perlIdx] = 0;
	}

	uint64_t cpuid = read_csr(mhartid);
	*iEnables[cpuid+type] |= 1 << (perlIdx + 1);
	*iThreshold[cpuid+type] = 0;
}

void enx_timerirq_init(void)
{
    /* reuse existing routine */
	int cpu_id = read_csr(mhartid);
	*mtime = 0;											// timer init
	timecmp[cpu_id] = *mtime + TIMECMP_NEXT_VAL;		// Next timer Interrupt

    set_csr(mie, MIP_MTIP);								// Enable the Machine-Timer bit in MIE
}

void enx_externalirq_init(void)
{
	set_csr(mie, MIP_MEIP);								// Enable External Interrupts
	set_csr(mstatus, MSTATUS_MIE);						// Machine Interrupt Enable
//	set_csr(mstatus, MSTATUS_MPIE);						// Machine Previous Interrupt Enabler
//	set_csr(mstatus, MSTATUS_MPP);						// Machine Previous Privilege Mode

	enx_externalirq_perl(eigiISP, DEF_ON, 0);			// Enable ISP Interrupts
	enx_externalirq_perl(eigiDMA, DEF_ON, 0);			// Enable DMA Interrupts
	enx_externalirq_perl(eigiH264, DEF_ON, 0);			// Enable H264 Interrupts
	enx_externalirq_perl(eigiH265, DEF_ON, 0);			// Enable H265 Interrupts
	enx_externalirq_perl(eigiUSB, DEF_ON, 0);			// Enable USB Interrupts
	enx_externalirq_perl(eigiI2S, DEF_ON, 0);			// Enable I2S Interrupts
	enx_externalirq_perl(eigiETH, DEF_ON, 0);			// Enable ETH Interrupts
	enx_externalirq_perl(eigiSDIO, DEF_ON, 0);			// Enable SDIO Interrupts
	enx_externalirq_perl(eigiASE, DEF_ON, 0);			// Enable AES Interrupts
	enx_externalirq_perl(eigiSHA, DEF_ON, 0);			// Enable SHA Interrupts
	enx_externalirq_perl(eigiCHKSUM, DEF_ON, 0);		// Enable CHKSUM Interrupts
	enx_externalirq_perl(eigiUART, DEF_ON, 0);			// Enable UART Interrupts
	enx_externalirq_perl(eigiSPI, DEF_ON, 0);			// Enable SPI Interrupts
	enx_externalirq_perl(eigiI2C, DEF_ON, 0);			// Enable I2C Interrupts
	enx_externalirq_perl(eigiGPIO, DEF_ON, 0);			// Enable GPIO Interrupts
	enx_externalirq_perl(eigiTIMER, DEF_ON, 0);			// Enable TIMER Interrupts
}
