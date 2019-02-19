#include "dev.h"

void enx_exirq_source1(void)
{
	if(IRQ_ISP)
	{
		if(IRQ_ISP0){};
		if(IRQ_ISP1){};
		if(IRQ_ISP2){};
		if(IRQ_ISP3){};
		if(IRQ_ISP4){};
		if(IRQ_ISP5){};
		if(IRQ_ISP6){};
		if(IRQ_ISP7){};
		if(IRQ_ISP8){};
		if(IRQ_ISP9){};
		if(IRQ_ISP10){};
		if(IRQ_ISP11){};
		if(IRQ_ISP12){};
		if(IRQ_ISP13){};
		if(IRQ_ISP14){};
		if(IRQ_ISP15){};
		if(IRQ_ISP16){};
		if(IRQ_ISP17){};
		if(IRQ_ISP18){};
		if(IRQ_ISP19){};
		if(IRQ_ISP20){};
		if(IRQ_ISP21){};
		if(IRQ_ISP22){};
		if(IRQ_ISP23){};
		if(IRQ_ISP24){};
		if(IRQ_ISP25){};
		if(IRQ_ISP26){};
		if(IRQ_ISP27){};
		if(IRQ_ISP28){};
		if(IRQ_ISP29){};
		if(IRQ_ISP30){};
	}
}

void enx_exirq_source2(void)
{
	if(IRQ_CDC)
	{
		if(IRQ_DMA){};
		if(IRQ_H265){};
		if(IRQ_H264){};
	}
}

void enx_exirq_source3(void)
{
	if(IRQ_ETH)
	{
		if(IRQ_USB_DMA){};
		if(IRQ_USB_MC){};
		if(IRQ_I2S_RX){};
		if(IRQ_I2S_TX){};
		if(IRQ_ETH_RX){};
		if(IRQ_ETH_TX){};
	}
}
void enx_exirq_source4(void)
{
	if(IRQ_SDIO)
	{
		if(IRQ_SDIO0){};
		if(IRQ_SDIO1){};
	}
}

void enx_exirq_source5(void)
{
	if(IRQ_UTIL)
	{
		if(IRQ_AES){};
		if(IRQ_SHA){};
		if(IRQ_CHKSUM){};
	}
}

void enx_exirq_source6(void)
{
	if(IRQ_UART)
	{
		if(IRQ_UART0){IrqUart(0);}
		if(IRQ_UART1){IrqUart(1);}
		if(IRQ_UART2){IrqUart(2);}
		if(IRQ_UART3){IrqUart(3);}
		if(IRQ_UART4){IrqUart(4);}
		if(IRQ_UART5){IrqUart(5);}
		if(IRQ_UART6){IrqUart(6);}
		if(IRQ_UART7){IrqUart(7);}
		if(IRQ_UART8){IrqUart(8);}
	}
	if(IRQ_SPI)
	{
		if(IRQ_SPI0){};
		if(IRQ_SPI1){};
		if(IRQ_SPI2){};
		if(IRQ_SPI3){};
		if(IRQ_SPI4){};
		if(IRQ_SPI5){};
		if(IRQ_SPI6){};
		if(IRQ_SPI7){};
		if(IRQ_SPI8){};

	}
	if(IRQ_I2C)
	{
		if(IRQ_I2C0){};
		if(IRQ_I2C1){};
		if(IRQ_I2C2){};
		if(IRQ_I2C3){};
		if(IRQ_I2C4){};
		if(IRQ_I2C5){};
		if(IRQ_I2C6){};
		if(IRQ_I2C7){};
		if(IRQ_I2C8){};
	}
}

void enx_exirq_source7(void)
{
	if(IRQ_GPIO_G2)
	{
		if(IRQ_GPIO71){};
		if(IRQ_GPIO70){};
		if(IRQ_GPIO69){};
		if(IRQ_GPIO68){};
		if(IRQ_GPIO67){};
		if(IRQ_GPIO66){};
		if(IRQ_GPIO65){};
		if(IRQ_GPIO64){};
		if(IRQ_GPIO63){};
		if(IRQ_GPIO62){};
		if(IRQ_GPIO61){};
		if(IRQ_GPIO60){};
		if(IRQ_GPIO59){};
		if(IRQ_GPIO58){};
		if(IRQ_GPIO57){};
		if(IRQ_GPIO56){};
		if(IRQ_GPIO55){};
		if(IRQ_GPIO54){};
		if(IRQ_GPIO53){};
		if(IRQ_GPIO52){};
		if(IRQ_GPIO51){};
		if(IRQ_GPIO50){};
		if(IRQ_GPIO49){};
		if(IRQ_GPIO48){};
	}
	if(IRQ_GPIO_G1)
	{
		if(IRQ_GPIO47){};
		if(IRQ_GPIO46){};
		if(IRQ_GPIO45){};
		if(IRQ_GPIO44){};
		if(IRQ_GPIO43){};
		if(IRQ_GPIO42){};
		if(IRQ_GPIO41){};
		if(IRQ_GPIO40){};
		if(IRQ_GPIO39){};
		if(IRQ_GPIO38){};
		if(IRQ_GPIO37){};
		if(IRQ_GPIO36){};
		if(IRQ_GPIO35){};
		if(IRQ_GPIO34){};
		if(IRQ_GPIO33){};
		if(IRQ_GPIO32){};
		if(IRQ_GPIO31){};
		if(IRQ_GPIO30){};
		if(IRQ_GPIO29){};
		if(IRQ_GPIO28){};
		if(IRQ_GPIO27){};
		if(IRQ_GPIO26){};
		if(IRQ_GPIO25){};
		if(IRQ_GPIO24){};
	}
	if(IRQ_GPIO_G0)
	{
		if(IRQ_GPIO23){};
		if(IRQ_GPIO22){};
		if(IRQ_GPIO21){};
		if(IRQ_GPIO20){};
		if(IRQ_GPIO19){};
		if(IRQ_GPIO18){};
		if(IRQ_GPIO17){};
		if(IRQ_GPIO16){};
		if(IRQ_GPIO15){};
		if(IRQ_GPIO14){};
		if(IRQ_GPIO13){};
		if(IRQ_GPIO12){};
		if(IRQ_GPIO11){};
		if(IRQ_GPIO10){};
		if(IRQ_GPIO9){};
		if(IRQ_GPIO8){};
		if(IRQ_GPIO7){};
		if(IRQ_GPIO6){};
		if(IRQ_GPIO5){};
		if(IRQ_GPIO4){};
		if(IRQ_GPIO3){};
		if(IRQ_GPIO2){};
		if(IRQ_GPIO1){};
		if(IRQ_GPIO0){};
	}
}

void enx_exirq_source8(void)
{
	if(IRQ_TIMER_G1)
	{
		if(IRQ_TIMER38){};
		if(IRQ_TIMER37){};
		if(IRQ_TIMER36){};
		if(IRQ_TIMER35){};
		if(IRQ_TIMER34){};
		if(IRQ_TIMER33){};
		if(IRQ_TIMER32){};
		if(IRQ_TIMER31){};
		if(IRQ_TIMER30){};
		if(IRQ_TIMER29){};
		if(IRQ_TIMER28){};
		if(IRQ_TIMER27){};
		if(IRQ_TIMER26){};
		if(IRQ_TIMER25){};
		if(IRQ_TIMER24){};
		if(IRQ_TIMER23){};
		if(IRQ_TIMER22){};
		if(IRQ_TIMER21){};
		if(IRQ_TIMER20){};
	}
	if(IRQ_TIMER_G0)
	{
		if(IRQ_TIMER19){};
		if(IRQ_TIMER18){};
		if(IRQ_TIMER17){};
		if(IRQ_TIMER16){};
		if(IRQ_TIMER15){};
		if(IRQ_TIMER14){};
		if(IRQ_TIMER13){};
		if(IRQ_TIMER12){};
		if(IRQ_TIMER11){};
		if(IRQ_TIMER10){};
		if(IRQ_TIMER9){};
		if(IRQ_TIMER8){};
		if(IRQ_TIMER7){};
		if(IRQ_TIMER6){};
		if(IRQ_TIMER5){};
		if(IRQ_TIMER4){};
		if(IRQ_TIMER3){};
		if(IRQ_TIMER2){};
		if(IRQ_TIMER1){};
		if(IRQ_TIMER0){};
	}
}

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

typedef struct {
	unsigned int _res:29;
	unsigned int priority:3;
} IRQ_Priority;

typedef struct {
	unsigned int _res1:23;
	unsigned int pending8:1;
	unsigned int pending7:1;
	unsigned int pending6:1;
	unsigned int pending5:1;
	unsigned int pending4:1;
	unsigned int pending3:1;
	unsigned int pending2:1;
	unsigned int pending1:1;
	unsigned int _res2:1;
} IRQ_Pending;

typedef struct {
	unsigned int _res1:23;
	unsigned int enables1:1;
	unsigned int enables0:7;
	unsigned int _res2:1;
} IRQ_Enable;

typedef struct {
	unsigned long threshold:3;
	unsigned long _res:29;
	unsigned long claim_complete:32;
} IRQ_Thres_Claim;

volatile unsigned int *iPrioBase[8] = {
		(unsigned int *)IRQ_PRIO1_BASE,
		(unsigned int *)IRQ_PRIO2_BASE,
		(unsigned int *)IRQ_PRIO3_BASE,
		(unsigned int *)IRQ_PRIO4_BASE,
		(unsigned int *)IRQ_PRIO5_BASE,
		(unsigned int *)IRQ_PRIO6_BASE,
		(unsigned int *)IRQ_PRIO7_BASE,
		(unsigned int *)IRQ_PRIO8_BASE,
};
volatile unsigned int *iPanding = (unsigned int *)IRQ_PENDING_BASE;
volatile unsigned int *iEnables[8] = {
		(unsigned int *)IRQ_ENABLES1_BASE,
		(unsigned int *)IRQ_ENABLES2_BASE,
		(unsigned int *)IRQ_ENABLES3_BASE,
		(unsigned int *)IRQ_ENABLES4_BASE,
		(unsigned int *)IRQ_ENABLES5_BASE,
		(unsigned int *)IRQ_ENABLES6_BASE,
		(unsigned int *)IRQ_ENABLES7_BASE,
		(unsigned int *)IRQ_ENABLES8_BASE,
};
volatile unsigned int *iThreshold[8] = {
		(unsigned int *)IRQ_THRESHOLD1_BASE,
		(unsigned int *)IRQ_THRESHOLD2_BASE,
		(unsigned int *)IRQ_THRESHOLD3_BASE,
		(unsigned int *)IRQ_THRESHOLD4_BASE,
		(unsigned int *)IRQ_THRESHOLD5_BASE,
		(unsigned int *)IRQ_THRESHOLD6_BASE,
		(unsigned int *)IRQ_THRESHOLD7_BASE,
		(unsigned int *)IRQ_THRESHOLD8_BASE,
};
volatile unsigned int *iClaimCompliet[8] = {
		(unsigned int *)IRQ_CLAIM_COMPLIET1_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET2_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET3_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET4_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET5_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET6_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET7_BASE,
		(unsigned int *)IRQ_CLAIM_COMPLIET8_BASE,
};

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

void trap_from_machine_mode(void)
{
	unsigned long mcause = read_csr(mcause);
	if ((mcause&0x8000000000000000) != 0x0) {
		mcause -= 0x8000000000000000;
		switch(mcause) {
			case 7:
			    if (mtime && timecmp) {
			    	//*HLS()->timecmp = *mtime + (1000000) ; // / configTICK_RATE_HZ);
			    	//*timecmp = *mtime + (1000000);
			        //*timecmp1 = *mtime + (1000000);
			    }
				break;
			case 11:
				for (int i = 0; i < 8; i++) {
					volatile unsigned int source = *iClaimCompliet[i];
					if (source != 0) {
						_printf("CPU%d(%c) - IRQ%d\n", i/2, i%2==0 ? 'M':'S', i);
					}
					switch (source) {
						case 1:
							enx_exirq_source1();
							break;
						case 2:
							enx_exirq_source2();
							break;
						case 3:
							enx_exirq_source3();
							break;
						case 4:
							enx_exirq_source4();
							break;
						case 5:
							enx_exirq_source5();
							break;
						case 6:
							enx_exirq_source6();
							break;
						case 7:
							enx_exirq_source7();
							break;
						case 8:
							enx_exirq_source8();
							break;
					}
					*iClaimCompliet[i] = source;
				}
				break;
			default:
				_printf("IRQ.1 0x%lX\n", mcause);
				break;
		}
	}
	else {
		_printf("IRQ.0 0x%lX\n", mcause);
		switch(mcause) {
			case 0:		_printf("Instruction address misaligned\n");	break;
			case 1:		_printf("Instruction access fault\n");			break;
			case 2:		_printf("Illegal instruction\n");				break;
			case 3:		_printf("Breakpoint\n");						break;
			case 4:		_printf("Load address misaligned\n");			break;
			case 5:		_printf("Load access fault\n");					break;
			case 6:		_printf("Store/AMO address misaligned\n");		break;
			case 7:		_printf("Store/AMO access fault\n");			break;
			case 8:		_printf("Environment call from U-mode\n");		break;
			case 9:		_printf("Environment call from S-mode\n");		break;
			//case 10:	_printf("Reserved\n");							break;
			case 11:	_printf("Environment call from M-mode\n");		break;
			case 12:	_printf("Instruction page fault\n");			break;
			case 13:	_printf("Load page fault\n");					break;
			//case 14:	_printf("Reserved\n");							break;
			case 15:	_printf("Store/AMO page fault\n");				break;
			default:	_printf("Reserved\n");							break;
		}
	}
}

// Group1
#define IRQ_USE_ISP 1

// Group2
#define IRQ_USE_DMA 2
#define IRQ_USE_H264 2
#define IRQ_USE_H265 2

// Group3
#define IRQ_USE_USB 3
#define IRQ_USE_I2S 3
#define IRQ_USE_ETH 3

// Group4
#define IRQ_USE_SDIO 4

// Group5
#define IRQ_USE_AES 5
#define IRQ_USE_SHA 5
#define IRQ_USE_CHKSUM 5

// Group6
#define IRQ_USE_UART 6
#define IRQ_USE_SPI 6
#define IRQ_USE_I2C 6

// Group7
#define IRQ_USE_GPIO 7

// Group8
#define IRQ_USE_TIMER 8

void enx_externalirq_init(void)
{
	uint64_t cpuid = read_csr(mhartid);

	// Enable External Interrupts
	write_csr(mie, MIP_MEIP);

	// Enable Global Interrupts
	unsigned long mstatus = read_csr(mstatus);
	_printf("MSTATUS SET1: 0x%X\n", mstatus);
	mstatus = INSERT_FIELD(mstatus, MSTATUS_MIE, 1); // Machine Interrupt Enable
	mstatus = INSERT_FIELD(mstatus, MSTATUS_MPIE, 0); // Machine Previous Interrupt Enabler
	mstatus = INSERT_FIELD(mstatus, MSTATUS_MPP, 0); // Machine Previous Privilege Mode
	_printf("MSTATUS SET2: 0x%X\n", mstatus);
	write_csr(mstatus, mstatus);

#if 1
	iPrioBase[0] = 0;
	iPrioBase[1] = 0;
	iPrioBase[2] = 0;
	iPrioBase[3] = 0;
	iPrioBase[4] = 0;
	iPrioBase[5] = 1;
	iPrioBase[6] = 0;
	iPrioBase[7] = 0;

	iEnables[0] = 0x40;
	iEnables[1] = 0;
	iEnables[2] = 0;
	iEnables[3] = 0;
	iEnables[4] = 0;
	iEnables[5] = 0;
	iEnables[6] = 0;
	iEnables[7] = 0;

	iThreshold[0] = 0;
	iThreshold[1] = 0;
	iThreshold[2] = 0;
	iThreshold[3] = 0;
	iThreshold[4] = 0;
	iThreshold[5] = 0;
	iThreshold[6] = 0;
	iThreshold[7] = 0;


#else
	// source
	volatile unsigned int * pAdr_prio1 = (unsigned int *)0x0C000004; // ISP
	volatile unsigned int * pAdr_prio2 = (unsigned int *)0x0C000008; // DMA, H.264, H.265
	volatile unsigned int * pAdr_prio3 = (unsigned int *)0x0C00000C; // USB, I2S, ETH
	volatile unsigned int * pAdr_prio4 = (unsigned int *)0x0C000010; // SDIO
	volatile unsigned int * pAdr_prio5 = (unsigned int *)0x0C000014; // AES, SHA, CHKSUM
	volatile unsigned int * pAdr_prio6 = (unsigned int *)0x0C000018; // UART, SPI, I2C
	volatile unsigned int * pAdr_prio7 = (unsigned int *)0x0C00001C; // GPIO
	volatile unsigned int * pAdr_prio8 = (unsigned int *)0x0C000020; // TIMER

	// Target
	volatile unsigned int * pAdr_ie1   = (unsigned int *)0x0C002000; // CPU1 - Machine
	volatile unsigned int * pAdr_ie2   = (unsigned int *)0x0C002080; // CPU1 - Supervisor
	volatile unsigned int * pAdr_ie3   = (unsigned int *)0x0C002100; // CPU2 - Machine
	volatile unsigned int * pAdr_ie4   = (unsigned int *)0x0C002180; // CPU2 - Supervisor
	volatile unsigned int * pAdr_ie5   = (unsigned int *)0x0C002200; // CPU3 - Machine
	volatile unsigned int * pAdr_ie6   = (unsigned int *)0x0C002280; // CPU3 - Supervisor
	volatile unsigned int * pAdr_ie7   = (unsigned int *)0x0C002300; // CPU4 - Machine
	volatile unsigned int * pAdr_ie8   = (unsigned int *)0x0C002380; // CPU4 - Supervisor

	volatile unsigned int * pAdr_thre1 = (unsigned int *)0x0C200000; // CPU1 - Machine
	volatile unsigned int * pAdr_thre2 = (unsigned int *)0x0C201000; // CPU1 - Supervisor
	volatile unsigned int * pAdr_thre3 = (unsigned int *)0x0C202000; // CPU2 - Machine
	volatile unsigned int * pAdr_thre4 = (unsigned int *)0x0C203000; // CPU2 - Supervisor
	volatile unsigned int * pAdr_thre5 = (unsigned int *)0x0C204000; // CPU3 - Machine
	volatile unsigned int * pAdr_thre6 = (unsigned int *)0x0C205000; // CPU3 - Supervisor
	volatile unsigned int * pAdr_thre7 = (unsigned int *)0x0C206000; // CPU4 - Machine
	volatile unsigned int * pAdr_thre8 = (unsigned int *)0x0C207000; // CPU4 - Supervisor

	volatile unsigned int * pAdr_mthre = (unsigned int *)0xc200000;
	volatile unsigned int * pAdr_sthre = (unsigned int *)0xc201000;
	volatile unsigned int * pAdr_pend  = (unsigned int *)0xc001000;

	*pAdr_prio1 = 0;
	*pAdr_prio2 = 0;
	*pAdr_prio3 = 0;
	*pAdr_prio4 = 0;
	*pAdr_prio5 = 0;
	*pAdr_prio6 = 1;
	*pAdr_prio7 = 0;
	*pAdr_prio8 = 0;

	*pAdr_ie1 = 0x40;
	*pAdr_ie2 = 0;
	*pAdr_ie3 = 0;
	*pAdr_ie4 = 0;
	*pAdr_ie5 = 0;
	*pAdr_ie6 = 0;
	*pAdr_ie7 = 0;
	*pAdr_ie8 = 0;

	*pAdr_mthre = 0x0;
#endif
}
