#include "dev.h"
#include "syscall.h"

#include "enx_freertos.h"

volatile uint64* mtime =      (uint64*)(CLINT_BASE + 0xbff8);
volatile uint64* timecmp =    (uint64*)(CLINT_BASE + 0x4000);

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

//static volatile unsigned int *iPanding = (unsigned int *)IRQ_PENDING_BASE;

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

#define IRQ_ISP_PRINTF(...)	//printf(__VA_ARGS__)
volatile UINT gnVoIrqCnt = 0;

void enx_exirq_source1(void)
{
	if (IRQ_ISP) {
		IRQ_ISP_PRINTF("IRQ_ISP:");
		if (IRQ_ISP0){CLI_VLOCKIw(1);}
		if (IRQ_ISP1){CLI_VLOCKWw(1);}
		if (IRQ_ISP2){CLI_VLOCKOw(1); gnVoIrqCnt++;
			//if(!(gnVoIrqCnt%(FPS_VDO*5))) _printf("VLOCKO_IRQ %d!!!\n", gnVoIrqCnt/FPS_VDO);	// TODO ◆ KSH VLOCKO IRQ test
			//else if(!(gnVoIrqCnt%FPS_VDO)) _printf_irq("VLOCKO_IRQ %d\n", gnVoIrqCnt/FPS_VDO);
			IRQ_ISP_PRINTF("VLOCKO_IRQ\n");
		}
		if (IRQ_ISP3){CLI_JPGw(1);}
		if (IRQ_ISP4){CLI_UIRQ0w(1);}
		if (IRQ_ISP5){CLI_UIRQ1w(1);}
		if (IRQ_ISP6){CLI_VLOCKW2w(1);}
		if (IRQ_ISP7){IP_CLIUIRQw(1);}
		if (IRQ_ISP8){}
		if (IRQ_ISP9){CLI_VLOCKI0w(1);}
		if (IRQ_ISP10){CLI_VLOCKI1w(1);}
		if (IRQ_ISP11){CLI_VLOCKI2w(1);}
		if (IRQ_ISP12){CLI_IMW0w(1);}
		if (IRQ_ISP13){CLI_IMW1w(1);}
		if (IRQ_ISP14){CLI_IMW2w(1);}
		if (IRQ_ISP15){CLI_IMW3w(1);}
		if (IRQ_ISP16){CLI_IMW4w(1);}
		if (IRQ_ISP17){CLI_IMW0_DSw(1);}
		if (IRQ_ISP18){CLI_IMW1_DSw(1);}
		if (IRQ_ISP19){CLI_IMW2_DSw(1);}
		if (IRQ_ISP20){CLI_IMW3_DSw(1);}
		if (IRQ_ISP21){CLI_VLOCKO_IT1w(1);}
		if (IRQ_ISP22){CLI_VLOCKO_IT2w(2);}
		if (IRQ_ISP23){CLI_VLOCKO_IT2w(3);}
		if (IRQ_ISP24){}
		if (IRQ_ISP25){}
		if (IRQ_ISP26){}
		if (IRQ_ISP27){}
		if (IRQ_ISP28){}
		if (IRQ_ISP29){}
		if (IRQ_ISP30){}
	} else {
		IRQ_ISP_PRINTF("NO IRQ_ISP");
	}

	IRQ_ISP_PRINTF("\n");
}

/*
_regs_ BF_11(
UINT CDC : 1 ,
UINT _rev0 : 22,
UINT CDMA3 : 1 ,				0x100	8
UINT CDMA2 : 1 ,				0x 80	7
UINT CDMA1 : 1 ,				0x 40	6
UINT CDMA0 : 1 ,				0x 20	5
UINT BDMA3 : 1 ,				0x 10	4
UINT BDMA2 : 1 ,				0x  8	3
UINT BDMA1 : 1 ,				0x  4	2
UINT BDMA0 : 1 ,				0x  2	1
UINT H265 : 1 ) _rege_ _IRQ_1; 	0x  1	0
#define IRQ_CDC _bm(_IRQ_1,REG_BASE_IRQ, (1<<3),CDC) // 1 Bit, 1'h0, R
*/

void (*exirq_source2_item[9])(UINT) = {
		NULL, IrqBDma, IrqBDma, IrqBDma, IrqBDma, IrqCDma, IrqCDma, IrqCDma, IrqCDma // H.265 IRQ 발생시 null 참조로 시스템 die
};
UINT exirq_source2_argv[9] = {0, 0, 1, 2, 3, 0, 1, 2, 3};

void enx_exirq_source2(void)
{
#if 1
	UINT getirq = _am(_IRQ_1, REG_BASE_IRQ, (1<<3));
	if (getirq) {
		for (int i = 0; i < 9; i++) {
			if((getirq >> i) & 0x1) {
				exirq_source2_item[i](exirq_source2_argv[i]);
			}
		}
	} else {
		printf("0x%08X\n", getirq);
	}
#else
	if (IRQ_CDC) {
		if (IRQ_BDMA0){IrqBDma(0);}
		if (IRQ_BDMA1){IrqBDma(1);}
		if (IRQ_BDMA2){IrqBDma(2);}
		if (IRQ_BDMA3){IrqBDma(3);}
		if (IRQ_CDMA0){IrqCDma(0);}
		if (IRQ_CDMA1){IrqCDma(1);}
		if (IRQ_CDMA2){IrqCDma(2);}
		if (IRQ_CDMA3){IrqCDma(3);}
		if (IRQ_H265){printf("IRQ_H265\n");}
	} else {
		printf("%u - %u/%u/%u/%u/%u/%u/%u/%u\n", IRQ_CDC, IRQ_BDMA0, IRQ_BDMA1, IRQ_BDMA2, IRQ_BDMA3, IRQ_CDMA0, IRQ_CDMA1, IRQ_CDMA2, IRQ_CDMA3);
	}
#endif
}

void enx_exirq_source3(void)
{
	if (IRQ_ETH) {
		if (IRQ_USB_DMA){printf("IRQ_USB_DMA\n");}
		if (IRQ_USB_MC){printf("IRQ_USB_MC\n");}
		if (IRQ_I2S_RX){IrqI2sRx();}
		if (IRQ_I2S_TX){IrqI2sTx();}
		if (IRQ_ETH_RX){IrqEthRx();}
		if (IRQ_ETH_TX){IrqEthTx();}
	} else {
		printf("NO IRQ_ETH\n");
	}
}

void enx_exirq_source4(void)
{
	if (IRQ_SDIO) {
		if (IRQ_SDIO0){IrqSdio(0);}
		if (IRQ_SDIO1){IrqSdio(1);}
	} else {
		printf("NO IRQ_SDIO\n");
	}
}

void enx_exirq_source5(void)
{
	if (IRQ_UTIL) {
		if (IRQ_AES){IrqAes();}
		if (IRQ_SHA){IrqSha();}
		if (IRQ_CHKSUM){IrqChksum();}
		if (IRQ_BUS){IrqBus();}
	} else {
		printf("NO IRQ_UTIL\n");
	}
}

void enx_exirq_source6(void)
{
	int k = 0;
	if (IRQ_UART) {
		if (IRQ_UART0){IrqUart(0);}
		if (IRQ_UART1){IrqUart(1);}
		if (IRQ_UART2){IrqUart(2);}
		if (IRQ_UART3){IrqUart(3);}
		if (IRQ_UART4){IrqUart(4);}
		if (IRQ_UART5){IrqUart(5);}
		if (IRQ_UART6){IrqUart(6);}
		if (IRQ_UART7){IrqUart(7);}
		if (IRQ_UART8){IrqUart(8);}
	} else {
		k++;
	}
	if (IRQ_SPI) {
		if (IRQ_SPI0){IrqSpi(0);}
		if (IRQ_SPI1){IrqSpi(1);}
		if (IRQ_SPI2){IrqSpi(2);}
		if (IRQ_SPI3){IrqSpi(3);}
		if (IRQ_SPI4){IrqSpi(4);}
		if (IRQ_SPI5){IrqSpi(5);}
		if (IRQ_SPI6){IrqSpi(6);}
		if (IRQ_SPI7){IrqSpi(7);}
		if (IRQ_SPI8){IrqSpi(8);}
	} else {
		k++;
	}
	if (IRQ_I2C) {
		if (IRQ_I2C0){IrqI2c(0);}
		if (IRQ_I2C1){IrqI2c(1);}
		if (IRQ_I2C2){IrqI2c(2);}
		if (IRQ_I2C3){IrqI2c(3);}
		if (IRQ_I2C4){IrqI2c(4);}
		if (IRQ_I2C5){IrqI2c(5);}
		if (IRQ_I2C6){IrqI2c(6);}
		if (IRQ_I2C7){IrqI2c(7);}
		if (IRQ_I2C8){IrqI2c(8);}
	} else {
		k++;
	}
	if (k == 3) {
		printf("NO IRQ_UART/SPI/I2C\n");
	}
}

void enx_exirq_source7(void)
{
	int k = 0;
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
	} else {
		k++;
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
	} else {
		k++;
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
	} else {
		k++;
	}
	if (k == 3) {
		printf("NO IRQ_GPIO_G0/1/2\n");
	}
}

void enx_exirq_source8(void)
{
	int k = 0;
	if (IRQ_TIMER_G1) {
		if (IRQ_TIMER38){IrqTimer(38);}
		if (IRQ_TIMER37){IrqTimer(37);}
		if (IRQ_TIMER36){IrqTimer(36);}
		if (IRQ_TIMER35){IrqTimer(35);}
		if (IRQ_TIMER34){IrqTimer(34);}
		if (IRQ_TIMER33){IrqTimer(33);}
		if (IRQ_TIMER32){IrqTimer(32);}
		if (IRQ_TIMER31){IrqTimer(31);}
		if (IRQ_TIMER30){IrqTimer(30);}
		if (IRQ_TIMER29){IrqTimer(29);}
		if (IRQ_TIMER28){IrqTimer(28);}
		if (IRQ_TIMER27){IrqTimer(27);}
		if (IRQ_TIMER26){IrqTimer(26);}
		if (IRQ_TIMER25){IrqTimer(25);}
		if (IRQ_TIMER24){IrqTimer(24);}
		if (IRQ_TIMER23){IrqTimer(23);}
		if (IRQ_TIMER22){IrqTimer(22);}
		if (IRQ_TIMER21){IrqTimer(21);}
		if (IRQ_TIMER20){IrqTimer(20);}
	} else {
		k++;
	}
	if (IRQ_TIMER_G0) {
		if (IRQ_TIMER19){IrqTimer(19);}
		if (IRQ_TIMER18){IrqTimer(18);}
		if (IRQ_TIMER17){IrqTimer(17);}
		if (IRQ_TIMER16){IrqTimer(16);}
		if (IRQ_TIMER15){IrqTimer(15);}
		if (IRQ_TIMER14){IrqTimer(14);}
		if (IRQ_TIMER13){IrqTimer(13);}
		if (IRQ_TIMER12){IrqTimer(12);}
		if (IRQ_TIMER11){IrqTimer(11);}
		if (IRQ_TIMER10){IrqTimer(10);}
		if (IRQ_TIMER9){IrqTimer(9);}
		if (IRQ_TIMER8){IrqTimer(8);}
		if (IRQ_TIMER7){IrqTimer(7);}
		if (IRQ_TIMER6){IrqTimer(6);}
		if (IRQ_TIMER5){IrqTimer(5);}
		if (IRQ_TIMER4){IrqTimer(4);}
		if (IRQ_TIMER3){IrqTimer(3);}
		if (IRQ_TIMER2){IrqTimer(2);}
		if (IRQ_TIMER1){IrqTimer(1);}
		if (IRQ_TIMER0){IrqTimer(0);}
	} else {
		k++;
	}
	if (k == 2) {
		printf("NO IRQ_TIMER_G0/1\n");
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

	printf("mstatus : %lX\n", mstatus);
	printf("mie     : %lX\n", mie);
	printf("mip     : %lX\n", mip);
	printf("mideleg : %lX\n", mideleg);
	printf("mcause  : %lX\n", mcause);

	printf("IRQ Priority\n\t");
	for (int i = 0; i < 8; i++) {
		printf("[0x%08X,%8bb]", iPrioBase[i], *iPrioBase[i]);
		if (i==3) {
			printf("\n\t");
		}
	}
	printf("\n");
	printf("IRQ Pending\n\t[0x%08X,%8bb]\n", iPanding, *iPanding);
	printf("IRQ Enables\n\t");
	for (int i = 0; i < 8; i++) {
		printf("[0x%08X,%8bb]", iEnables[i], *iEnables[i]);
		if (i==3) {
			printf("\n\t");
		}
	}
	printf("\n");
	printf("IRQ Threshold\n\t");
	for (int i = 0; i < 8; i++) {
		printf("[0x%08X,%8bb]", iThreshold[i], *iThreshold[i]);
		if (i==3) {
			printf("\n\t");
		}
	}
	printf("\n");
	printf("IRQ ClaimCompliet\n\t");
	for (int i = 0; i < 8; i++) {
		UINT source = *iClaimCompliet[i];
		printf("[0x%08X,%8bb]", iClaimCompliet[i], source);
		*iClaimCompliet[i] = source;
		if (i==3) {
			printf("\n\t");
		}
	}
	printf("\n");

	printf("======================================================\n");
}
#endif
extern volatile int rtp_step;
extern void network_ethif_check_buffer(void);
static void __attribute__((noreturn)) bad_trap(uintptr_t mcause, uintptr_t mepc, uintptr_t regs[32])
{
	mepc -= 4;
	switch (mcause) {
	case CAUSE_MISALIGNED_FETCH:	printf("Instruction address misaligned\n");	break;
	case CAUSE_FETCH_ACCESS:		printf("Instruction access fault\n");			break;
	case CAUSE_ILLEGAL_INSTRUCTION:	printf("Illegal instruction\n");				break;
	case CAUSE_BREAKPOINT:			printf("Breakpoint\n");						break;
	case CAUSE_MISALIGNED_LOAD:		printf("Load address misaligned\n");			break;
	case CAUSE_LOAD_ACCESS:			printf("Load access fault\n");					break;
	case CAUSE_MISALIGNED_STORE:	printf("Store/AMO address misaligned\n");		break;
	case CAUSE_STORE_ACCESS:		printf("Store/AMO access fault\n");			break;
	//case 10:						printf("Reserved\n");							break;
	case CAUSE_FETCH_PAGE_FAULT:	printf("Instruction page fault\n");			break;
	case CAUSE_LOAD_PAGE_FAULT:		printf("Load page fault\n");					break;
	//case 14:						printf("Reserved\n");							break;
	case CAUSE_STORE_PAGE_FAULT:	printf("Store/AMO page fault\n");				break;
	default:						printf("Reserved(%d)\n", mcause);				break;
	}

	printf("uptime      : %lus\n", gptMsgShare.UPTIME);
	printf("RTP error step: %d\n", rtp_step);

	static const char* regnames[] = {
		"ra", "t0", "t1", "t2",
		"s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
		"a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
		"s8", "s9", "sA", "sB", "t3", "t4", "t5", "t6"
	};

	for (int i = 0; i < 28; i += 4) {
		for(int j = 0; j < 4; j++)
			printf("%s %16lx%c", regnames[i+j], regs[i+1+j], j < 3 ? ' ' : '\n');
	}
	printf("machine mode: unhandlable trap %lu @ mepc(0x%08lX) mstatus(0x%08lX)\n", read_csr(mcause), mepc, regs[29]);

//	printk("pc %lx va %lx insn       %x sr %lx\n", tf->epc, tf->badvaddr,
//			(uint32_t)tf->insn, tf->status);
//	network_ethif_check_buffer();
	while (1);
}

void enx_timerirq_next(void)
{
    if (mtime && timecmp) {
    	uint64 cpu_id = read_csr(mhartid);
    	//timecmp[cpu_id] = *mtime + TIMECMP_NEXT_VAL; // 1ms
    	timecmp[cpu_id] = timecmp[cpu_id] + TIMECMP_NEXT_VAL; // 1ms
    	if (cpu_id == 0) {
			static uint64 gbTickCnt = TIME_TICK - 1;
			if (gbTickCnt == 0) {
				gbTickCnt = TIME_TICK - 1;
				gptMsgShare.TIME++;
				gptMsgShare.UPTIME++;
			} else {
				gbTickCnt--;
			}
    	}
    }
}

void (*exirq_source[9])(void) = {
	NULL, enx_exirq_source1, enx_exirq_source2, enx_exirq_source3, enx_exirq_source4, enx_exirq_source5, enx_exirq_source6, enx_exirq_source7, enx_exirq_source8
};

#ifdef __FREERTOS__
#if 1
void trap_from_machine_mode_freertos_sync(uintptr_t mcause, uintptr_t mepc, uint64 cpuid, uintptr_t regs[32])
{
	switch(mcause) {
	case CAUSE_USER_ECALL:
	case CAUSE_SUPERVISOR_ECALL:
	case CAUSE_MACHINE_ECALL:
		if (regs[28] == ECALL_YIELD_CMD) {
			vTaskSwitchContext();
		} else {
			regs[7] = do_syscall(regs[7], regs[8], regs[9], regs[10], regs[11], regs[12], regs[14]);
		}
		break;

	default:
		printf("Task name: [%s]\n", pcTaskGetName(NULL));
		bad_trap(mcause, mepc, regs);
		break;
	}
}

void trap_from_machine_mode_freertos_async(uintptr_t mcause, uintptr_t mepc, uint64 cpuid, uintptr_t regs[32])
{
	uint64 cpuidx = cpuid * 2;
#if 1
    //	800372a2:	55fd                	li	a1,-1
    //	800372a4:	8185                	srli	a1,a1,0x1
    //	800372a6:	8de9                	and	a1,a1,a0
	mcause = (mcause << 1) >> 1;
#else
	//	800372a2:	55fd                	li	a1,-1
	//	800372a4:	15fe                	slli	a1,a1,0x3f
	//	800372a6:	95aa                	add	a1,a1,a0
	mcause -= 0x8000000000000000;
#endif
	switch (mcause) {
	case IRQ_M_TIMER:
		vPortSysTickHandler();
		break;

	case IRQ_M_EXT:
		gbXsrTaskSwitchNeeded = 0;
		for (int i = 0; i < 2; i++) {
			volatile unsigned int source = *iClaimCompliet[cpuidx+i]; // Get Claim IRQ
			if (source == 0 || source > 8) {
				continue;
			}
#if 0 // Test용 log
			else if (IRQ_ETH_RX == 0 && IRQ_CDC == 0 && IRQ_I2S_TX == 0 && IRQ_I2S_RX == 0 && SDIO1_IO_IRQ == 0 && IRQ_TIMER8 == 0) {
				printf("CPU%u-OS-IRQ%d (%d/%c)\n", cpuid, source, (cpuidx+i), (cpuidx+i)%2==0 ? 'M':'S');
			}
#endif
#if 1
			exirq_source[source]();
#else
			switch (source) {
			case 1:	enx_exirq_source1();	break;
			case 2:	enx_exirq_source2();	break;
			case 3:	enx_exirq_source3();	break;
			case 4:	enx_exirq_source4();	break;
			case 5:	enx_exirq_source5();	break;
			case 6:	enx_exirq_source6();	break;
			case 7:	enx_exirq_source7();	break;
			case 8:	enx_exirq_source8();	break;
			default: printf("Err irq\n");	break;
			}
#endif
			*iClaimCompliet[cpuidx+i] = source;	// Set Complete IRQ
		}
		portYIELD_FROM_ISR(gbXsrTaskSwitchNeeded);
		break;

	default:
		printf("IRQ.1 0x%lX\n", mcause);
		break;
	}
}
#else
void trap_from_machine_mode_freertos(uintptr_t mcause, uintptr_t mepc, uint64 cpuid, uintptr_t regs[32])
{
	uint64 cpuidx = cpuid * 2;
	if ((mcause & 0x8000000000000000) != 0x0) {
		mcause -= 0x8000000000000000;
		switch (mcause) {
		case IRQ_M_TIMER:
			vPortSysTickHandler();
			break;

		case IRQ_M_EXT:
			gbXsrTaskSwitchNeeded = 0;
			for (int i = 0; i < 2; i++) {
				volatile unsigned int source = *iClaimCompliet[cpuidx+i]; // Get Claim IRQ
				if (source == 0 || source > 8) {
					continue;
				} else if (IRQ_ETH_RX == 0 && IRQ_CDC == 0 && IRQ_I2S_TX == 0 && IRQ_I2S_RX == 0 && SDIO1_IO_IRQ == 0 && IRQ_TIMER8 == 0) {
					printf("CPU%u-OS-IRQ%d (%d/%c)\n", cpuid, source, (cpuidx+i), (cpuidx+i)%2==0 ? 'M':'S');
				}
#if 1
				exirq_source[source]();
#else
				switch (source) {
				case 1:	enx_exirq_source1();	break;
				case 2:	enx_exirq_source2();	break;
				case 3:	enx_exirq_source3();	break;
				case 4:	enx_exirq_source4();	break;
				case 5:	enx_exirq_source5();	break;
				case 6:	enx_exirq_source6();	break;
				case 7:	enx_exirq_source7();	break;
				case 8:	enx_exirq_source8();	break;
				default: printf("Err irq\n");	break;
				}
#endif
				*iClaimCompliet[cpuidx+i] = source;	// Set Complete IRQ
			}
			portYIELD_FROM_ISR(gbXsrTaskSwitchNeeded);
			break;

		default:
			printf("IRQ.1 0x%lX\n", mcause);
			break;
		}
	} else {
		switch(mcause) {
		case CAUSE_USER_ECALL:
		case CAUSE_SUPERVISOR_ECALL:
		case CAUSE_MACHINE_ECALL:
			if (regs[28] == ECALL_YIELD_CMD) {
				vTaskSwitchContext();
			} else {
				regs[7] = do_syscall(regs[7], regs[8], regs[9], regs[10], regs[11], regs[12], regs[14]);
			}
			break;

		default:
			printf("Task name: [%s]\n", pcTaskGetName(NULL));
			bad_trap(mcause, mepc, regs);
			break;
		}
	}
}
#endif
#endif

#if 1
uintptr_t trap_from_machine_mode_sync(uintptr_t mcause, uintptr_t mepc, uint64 cpuid, uintptr_t regs[32])
{
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

uintptr_t trap_from_machine_mode_async(uintptr_t mcause, uintptr_t mepc, uint64 cpuid, uintptr_t regs[32])
{
	uint64 cpuidx = cpuid * 2;
	mcause -= 0x8000000000000000;
	switch (mcause) {
	case IRQ_M_TIMER:
		enx_timerirq_next();
		break;

	case IRQ_M_EXT:
		for (int i = 0; i < 2; i++) {
			volatile unsigned int source = *iClaimCompliet[cpuidx+i]; // Get Claim IRQ
			if (source == 0 || source > 8) {
				continue;
			}
#if 1 // Test용 log
			else if (source != 1 && source != 6) {
				printf("CPU%u-FW-IRQ%d (%d/%c)\n", cpuid, source, (cpuidx+i), (cpuidx+i)%2==0 ? 'M':'S');
			}
#endif
			exirq_source[source]();
			*iClaimCompliet[cpuidx+i] = source; // Set Complete IRQ
		}
		break;

	default:
		printf("IRQ.1 0x%lX\n", mcause);
		break;
	}
	return mepc;
}
#else
uintptr_t trap_from_machine_mode(uintptr_t mcause, uintptr_t mepc, uint64 cpuid, uintptr_t regs[32])
{
	uint64 cpuidx = cpuid * 2;
	if ((mcause & 0x8000000000000000) != 0x0) {
		mcause -= 0x8000000000000000;
		switch (mcause) {
		case IRQ_M_TIMER:
			enx_timerirq_next();
			break;

		case IRQ_M_EXT:
			for (int i = 0; i < 2; i++) {
				volatile unsigned int source = *iClaimCompliet[cpuidx+i]; // Get Claim IRQ
				if (source == 0 || source > 8) {
					continue;
				} else if (source != 1 && source != 6) {
					printf("CPU%u-FW-IRQ%d (%d/%c)\n", cpuid, source, (cpuidx+i), (cpuidx+i)%2==0 ? 'M':'S');
				}
#if 1
				exirq_source[source]();
#else
				switch (source) {
				case 1:	enx_exirq_source1();	break;
				case 2:	enx_exirq_source2();	break;
				case 3:	enx_exirq_source3();	break;
				case 4:	enx_exirq_source4();	break;
				case 5:	enx_exirq_source5();	break;
				case 6:	enx_exirq_source6();	break;
				case 7:	enx_exirq_source7();	break;
				case 8:	enx_exirq_source8();	break;
				default: printf("Err irq\n");	break;
				}
#endif
				*iClaimCompliet[cpuidx+i] = source; // Set Complete IRQ
			}
			break;

		default:
			printf("IRQ.1 0x%lX\n", mcause);
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
#endif

static void enx_externalirq_perl(eIRQ_GROUP_INDEX perlIdx, uint64 onoff, uint64 type)
{
	if (type > 1) {
		printf("Error type(%lu) (M-mode:0 S-mode:1)\n", type);
		return;
	}

	if (onoff) {
		*iPrioBase[perlIdx] = 1; // 1 ~ 7
	} else {
		*iPrioBase[perlIdx] = 0;
	}

	uint64 cpuid = read_csr(mhartid) * 2;
	*iEnables[cpuid+type] |= 1 << (perlIdx + 1);
	*iThreshold[cpuid+type] = 0;
}

void enx_timerirq_init(void)
{
	/* reuse existing routine */
	uint64 cpu_id = read_csr(mhartid);
	timecmp[cpu_id] = *mtime + TIMECMP_NEXT_VAL;		// Next timer Interrupt

	set_csr(mie, MIP_MTIP);								// Enable the Machine-Timer bit in MIE
}

void enx_externalirq_init(void)
{
	set_csr(mie, MIP_MEIP);								// Enable External Interrupts
	set_csr(mstatus, MSTATUS_MIE);						// Machine Interrupt Enable

//	set_csr(mstatus, MSTATUS_MPIE);						// Machine Previous Interrupt Enabler
//	set_csr(mstatus, MSTATUS_MPP);						// Machine Previous Privilege Mode

	enx_externalirq_perl(eigiISP, ENX_ON, 0);			// Enable ISP Interrupts
	enx_externalirq_perl(eigiDMA, ENX_ON, 0);			// Enable DMA Interrupts
	enx_externalirq_perl(eigiH264, ENX_ON, 0);			// Enable H264 Interrupts
	enx_externalirq_perl(eigiH265, ENX_ON, 0);			// Enable H265 Interrupts
	enx_externalirq_perl(eigiUSB, ENX_ON, 0);			// Enable USB Interrupts
	enx_externalirq_perl(eigiI2S, ENX_ON, 0);			// Enable I2S Interrupts
	enx_externalirq_perl(eigiETH, ENX_ON, 0);			// Enable ETH Interrupts
	enx_externalirq_perl(eigiSDIO, ENX_ON, 0);			// Enable SDIO Interrupts
	enx_externalirq_perl(eigiASE, ENX_ON, 0);			// Enable AES Interrupts
	enx_externalirq_perl(eigiSHA, ENX_ON, 0);			// Enable SHA Interrupts
	enx_externalirq_perl(eigiCHKSUM, ENX_ON, 0);		// Enable CHKSUM Interrupts
	enx_externalirq_perl(eigiBUS, ENX_ON, 0);			// Enable BUS Interrupts
	enx_externalirq_perl(eigiUART, ENX_ON, 0);			// Enable UART Interrupts
	enx_externalirq_perl(eigiSPI, ENX_ON, 0);			// Enable SPI Interrupts
	enx_externalirq_perl(eigiI2C, ENX_ON, 0);			// Enable I2C Interrupts
	enx_externalirq_perl(eigiGPIO, ENX_ON, 0);			// Enable GPIO Interrupts
	enx_externalirq_perl(eigiTIMER, ENX_ON, 0);			// Enable TIMER Interrupts
}
