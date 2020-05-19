#include "dev.h"
#include "syscall.h"

#ifdef __FREERTOS__
#include "enx_freertos.h"
#endif

irq_count core_irq_count[4];

#define EXIRQ_POINT 1

volatile uint32* cpu_msip =   (uint32*)(CLINT_BASE + 0x0);
volatile uint64* timecmp =    (uint64*)(CLINT_BASE + 0x4000);
volatile uint64* mtime =      (uint64*)(CLINT_BASE + 0xbff8);

#define IRQ_PRIO01_BASE				0x0C000004 // RW: Source  1 priority (ISP)
#define IRQ_PRIO02_BASE				0x0C000008 // RW: Source  2 priority (VCODEC)
#if EN675_SINGLE
#define IRQ_PRIO03_BASE				0x0C00000C // RW: Source  3 priority (DMA0~3)
#define IRQ_PRIO04_BASE				0x0C000010 // RW: Source  4 priority (DMA4~7)
#define IRQ_PRIO05_BASE				0x0C000014 // RW: Source  5 priority (DMA8~11)
#define IRQ_PRIO06_BASE				0x0C000018 // RW: Source  6 priority (DMA12~15)
#else
#define IRQ_PRIO03_BASE				0x0C00000C // RW: Source  3 priority (DMA0)
#define IRQ_PRIO04_BASE				0x0C000010 // RW: Source  4 priority (DMA1)
#define IRQ_PRIO05_BASE				0x0C000014 // RW: Source  5 priority (DMA2)
#define IRQ_PRIO06_BASE				0x0C000018 // RW: Source  6 priority (DMA3)
#endif
#define IRQ_PRIO07_BASE				0x0C00001C // RW: Source  7 priority (Core0)
#define IRQ_PRIO08_BASE				0x0C000020 // RW: Source  8 priority (Core1)
#define IRQ_PRIO09_BASE				0x0C000024 // RW: Source  9 priority (ETH)
#define IRQ_PRIO10_BASE				0x0C000028 // RW: Source 10 priority (SDIO0)
#define IRQ_PRIO11_BASE				0x0C00002C // RW: Source 11 priority (SDIO1)
#define IRQ_PRIO12_BASE				0x0C000030 // RW: Source 12 priority (NPU)
#define IRQ_PRIO13_BASE				0x0C000034 // RW: Source 13 priority (I2S)
#define IRQ_PRIO14_BASE				0x0C000038 // RW: Source 14 priority (USB)
#define IRQ_PRIO15_BASE				0x0C00003C // RW: Source 15 priority (SHA)
#define IRQ_PRIO16_BASE				0x0C000040 // RW: Source 16 priority (AES)
#define IRQ_PRIO17_BASE				0x0C000044 // RW: Source 17 priority (CHKSUM)
#define IRQ_PRIO18_BASE				0x0C000048 // RW: Source 18 priority (OIC)
#define IRQ_PRIO19_BASE				0x0C00004C // RW: Source 19 priority (GPIO0~3, PWM0~3, SPI0)
#define IRQ_PRIO20_BASE				0x0C000050 // RW: Source 20 priority (GPIO4~7, PWM4~7, I2C0, UART0)
#define IRQ_PRIO21_BASE				0x0C000054 // RW: Source 21 priority (GPIO8~11, PWM8~11, SPI1)
#define IRQ_PRIO22_BASE				0x0C000058 // RW: Source 22 priority (GPIO12~15, PWM12~15, I2C1, UART1)
#define IRQ_PRIO23_BASE				0x0C00005C // RW: Source 23 priority (GPIO16~19, SPI2)
#define IRQ_PRIO24_BASE				0x0C000060 // RW: Source 24 priority (GPIO20~23, PWM16~19, I2C2, UART2)
#define IRQ_PRIO25_BASE				0x0C000064 // RW: Source 25 priority (GPIO24~27, PWM20~23, SPI3)
#define IRQ_PRIO26_BASE				0x0C000068 // RW: Source 26 priority (GPIO28~31, PWM24~27, I2C3, UART3)
#define IRQ_PRIO27_BASE				0x0C00006C // RW: Source 27 priority (GPIO32~35, PWM28~31, SPI4)
#define IRQ_PRIO28_BASE				0x0C000070 // RW: Source 28 priority (GPIO36~39, I2C4, UART4)
#define IRQ_PRIO29_BASE				0x0C000074 // RW: Source 29 priority (GPIO40~43, SPI5)
#define IRQ_PRIO30_BASE				0x0C000078 // RW: Source 30 priority (GPIO44~47, I2C5, UART5)
#define IRQ_PRIO31_BASE				0x0C00007C // RW: Source 31 priority (GPIO48~51, SPI6)
#define IRQ_PRIO32_BASE				0x0C000080 // RW: Source 32 priority (GPIO52~55, PWM32~34, I2C6, UART6)
#define IRQ_PRIO33_BASE				0x0C000084 // RW: Source 33 priority (GPIO56~59, PWM35~38, SPI7)
#define IRQ_PRIO34_BASE				0x0C000088 // RW: Source 34 priority (GPIO60~63, I2C7, UART7)
#define IRQ_PRIO35_BASE				0x0C00008C // RW: Source 35 priority (GPIO64~67, SPI8)
#define IRQ_PRIO36_BASE				0x0C000090 // RW: Source 36 priority (GPIO68~71, I2C8, UART8)
#define IRQ_PRIO37_BASE				0x0C000094 // RW: Source 37 priority (IR)
#define IRQ_PRIO38_BASE				0x0C000098 // RW: Source 38 priority (OMC)
#define IRQ_PRIO39_BASE				0x0C00009C // RW: Source 39 priority (Reserved)
#define IRQ_PRIO40_BASE				0x0C0000A0 // RW: Source 40 priority (Reserved)

#define IRQ_PENDING_BASE			0x0C001000 // R: Interrupt pending bits

#define IRQ_ENABLES_H0M0_BASE		0x0C002000 // RW: Hart 0 M-mode enables [31: 0]
#define IRQ_ENABLES_H0M1_BASE		0x0C002004 // RW: Hart 0 M-mode enables [39:32]
#define IRQ_ENABLES_H0S0_BASE		0x0C002080 // RW: Hart 0 S-mode enables [31: 0]
#define IRQ_ENABLES_H0S1_BASE		0x0C002084 // RW: Hart 0 S-mode enables [39:32]
#define IRQ_ENABLES_H1M0_BASE		0x0C002100 // RW: Hart 1 M-mode enables [31: 0]
#define IRQ_ENABLES_H1M1_BASE		0x0C002104 // RW: Hart 1 M-mode enables [39:32]
#define IRQ_ENABLES_H1S0_BASE		0x0C002180 // RW: Hart 1 S-mode enables [31: 0]
#define IRQ_ENABLES_H1S1_BASE		0x0C002184 // RW: Hart 1 S-mode enables [39:32]
#define IRQ_ENABLES_H2M0_BASE		0x0C002200 // RW: Hart 2 M-mode enables [31: 0]
#define IRQ_ENABLES_H2M1_BASE		0x0C002204 // RW: Hart 2 M-mode enables [39:32]
#define IRQ_ENABLES_H2S0_BASE		0x0C002280 // RW: Hart 2 S-mode enables [31: 0]
#define IRQ_ENABLES_H2S1_BASE		0x0C002284 // RW: Hart 2 S-mode enables [39:32]
#define IRQ_ENABLES_H3M0_BASE		0x0C002300 // RW: Hart 3 M-mode enables [31: 0]
#define IRQ_ENABLES_H3M1_BASE		0x0C002304 // RW: Hart 3 M-mode enables [39:32]
#define IRQ_ENABLES_H3S0_BASE		0x0C002380 // RW: Hart 3 S-mode enables [31: 0]
#define IRQ_ENABLES_H3S1_BASE		0x0C002384 // RW: Hart 3 S-mode enables [39:32]

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

static volatile unsigned int *iPrioBase[IRQ_SOURCE_COUNT + 1] = {
		(unsigned int *)NULL,
		(unsigned int *)IRQ_PRIO01_BASE,
		(unsigned int *)IRQ_PRIO02_BASE,
		(unsigned int *)IRQ_PRIO03_BASE,
		(unsigned int *)IRQ_PRIO04_BASE,
		(unsigned int *)IRQ_PRIO05_BASE,
		(unsigned int *)IRQ_PRIO06_BASE,
		(unsigned int *)IRQ_PRIO07_BASE,
		(unsigned int *)IRQ_PRIO08_BASE,
		(unsigned int *)IRQ_PRIO09_BASE,
		(unsigned int *)IRQ_PRIO10_BASE,
		(unsigned int *)IRQ_PRIO11_BASE,
		(unsigned int *)IRQ_PRIO12_BASE,
		(unsigned int *)IRQ_PRIO13_BASE,
		(unsigned int *)IRQ_PRIO14_BASE,
		(unsigned int *)IRQ_PRIO15_BASE,
		(unsigned int *)IRQ_PRIO16_BASE,
		(unsigned int *)IRQ_PRIO17_BASE,
		(unsigned int *)IRQ_PRIO18_BASE,
		(unsigned int *)IRQ_PRIO19_BASE,
		(unsigned int *)IRQ_PRIO20_BASE,
		(unsigned int *)IRQ_PRIO21_BASE,
		(unsigned int *)IRQ_PRIO22_BASE,
		(unsigned int *)IRQ_PRIO23_BASE,
		(unsigned int *)IRQ_PRIO24_BASE,
		(unsigned int *)IRQ_PRIO25_BASE,
		(unsigned int *)IRQ_PRIO26_BASE,
		(unsigned int *)IRQ_PRIO27_BASE,
		(unsigned int *)IRQ_PRIO28_BASE,
		(unsigned int *)IRQ_PRIO29_BASE,
		(unsigned int *)IRQ_PRIO30_BASE,
		(unsigned int *)IRQ_PRIO31_BASE,
		(unsigned int *)IRQ_PRIO32_BASE,
		(unsigned int *)IRQ_PRIO33_BASE,
		(unsigned int *)IRQ_PRIO34_BASE,
		(unsigned int *)IRQ_PRIO35_BASE,
		(unsigned int *)IRQ_PRIO36_BASE,
		(unsigned int *)IRQ_PRIO37_BASE,
		(unsigned int *)IRQ_PRIO38_BASE,
		(unsigned int *)IRQ_PRIO39_BASE,
		(unsigned int *)IRQ_PRIO40_BASE,
};

//static volatile unsigned int *iPanding = (unsigned int *)IRQ_PENDING_BASE;

static volatile unsigned int *iEnables[16] = {
		(unsigned int *)IRQ_ENABLES_H0M0_BASE,
		(unsigned int *)IRQ_ENABLES_H0M1_BASE,
		(unsigned int *)IRQ_ENABLES_H0S0_BASE,
		(unsigned int *)IRQ_ENABLES_H0S1_BASE,
		(unsigned int *)IRQ_ENABLES_H1M0_BASE,
		(unsigned int *)IRQ_ENABLES_H1M1_BASE,
		(unsigned int *)IRQ_ENABLES_H1S0_BASE,
		(unsigned int *)IRQ_ENABLES_H1S1_BASE,
		(unsigned int *)IRQ_ENABLES_H2M0_BASE,
		(unsigned int *)IRQ_ENABLES_H2M1_BASE,
		(unsigned int *)IRQ_ENABLES_H2S0_BASE,
		(unsigned int *)IRQ_ENABLES_H2S1_BASE,
		(unsigned int *)IRQ_ENABLES_H3M0_BASE,
		(unsigned int *)IRQ_ENABLES_H3M1_BASE,
		(unsigned int *)IRQ_ENABLES_H3S0_BASE,
		(unsigned int *)IRQ_ENABLES_H3S1_BASE,

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

////////////////////////////////////////////////////////////////////////////////
// External Interrupt Handle
////////////////////////////////////////////////////////////////////////////////

void enx_exirq_view(void)
{
	printf("IRQ_01_T(0x%08X) IRQ_02_T(0x%08X) IRQ_03_T(0x%08X) IRQ_04_T(0x%08X)\n", IRQ_01_T, IRQ_02_T, IRQ_03_T, IRQ_04_T);
	printf("IRQ_05_T(0x%08X) IRQ_06_T(0x%08X) IRQ_07_T(0x%08X) IRQ_08_T(0x%08X)\n", IRQ_05_T, IRQ_06_T, IRQ_07_T, IRQ_08_T);
	printf("IRQ_09_T(0x%08X) IRQ_10_T(0x%08X) IRQ_11_T(0x%08X) IRQ_12_T(0x%08X)\n", IRQ_09_T, IRQ_10_T, IRQ_11_T, IRQ_12_T);
	printf("IRQ_13_T(0x%08X) IRQ_14_T(0x%08X) IRQ_15_T(0x%08X) IRQ_16_T(0x%08X)\n", IRQ_13_T, IRQ_14_T, IRQ_15_T, IRQ_16_T);
	printf("IRQ_17_T(0x%08X) IRQ_18_T(0x%08X) IRQ_19_T(0x%08X) IRQ_20_T(0x%08X)\n", IRQ_17_T, IRQ_18_T, IRQ_19_T, IRQ_20_T);
	printf("IRQ_21_T(0x%08X) IRQ_22_T(0x%08X) IRQ_23_T(0x%08X) IRQ_24_T(0x%08X)\n", IRQ_21_T, IRQ_22_T, IRQ_23_T, IRQ_24_T);
	printf("IRQ_25_T(0x%08X) IRQ_26_T(0x%08X) IRQ_27_T(0x%08X) IRQ_28_T(0x%08X)\n", IRQ_25_T, IRQ_26_T, IRQ_27_T, IRQ_28_T);
	printf("IRQ_29_T(0x%08X) IRQ_30_T(0x%08X) IRQ_31_T(0x%08X) IRQ_32_T(0x%08X)\n", IRQ_29_T, IRQ_30_T, IRQ_31_T, IRQ_32_T);
	printf("IRQ_33_T(0x%08X) IRQ_34_T(0x%08X) IRQ_35_T(0x%08X) IRQ_36_T(0x%08X)\n", IRQ_33_T, IRQ_34_T, IRQ_35_T, IRQ_36_T);
	printf("IRQ_37_T(0x%08X) IRQ_38_T(0x%08X) IRQ_39_T(0x%08X) IRQ_40_T(0x%08X)\n", IRQ_37_T, IRQ_38_T, IRQ_39_T, IRQ_40_T);
}

#define IRQ_ISP_PRINTF(...)	//printf(__VA_ARGS__)
UINT gnVoIrqCnt = 0;
UINT gnViIrqCnt = 0;
//UINT gnVoIrqOn = 0;
UINT gnViIrqOn = 0;
UINT gnVDI_4FPS = 0;
UINT gnVDI_CHG = 2;

#include "isp_jpeg.h"

void enx_exirq_source1(void)
{
	if (IRQ_G_ISP) {
		IRQ_ISP_PRINTF("IRQ_ISP:");
		if (IRQ_ISP0){CLI_VLOCKIw(1); gnViIrqOn = 1;

			static ULONG pre_time = 0;
			const ULONG cur_time = rdcycle();
			const ULONG dep = (pre_time < cur_time) ? cur_time - pre_time : (0xFFFFFFFFFFFFFFFF - pre_time) + cur_time + 1;

			if(gnVDI_CHG || dep > (4008*(CPU_FREQ/1000))) {	// 4000 = 4sec, 8 = 8ms : 60fps일 경우 최악 조건에서 IRQ 처리 함수 진입 시간이 1회 1~31ms, 2회 17~47ms 이 될 수 있으며 판정 불가능, 판정이 가능한 최대 조건은 1회 9~23 ms, 2회 25~39 이므로 24(=16+8)로 판단해야 함
				pre_time = cur_time;
				if(gnVDI_CHG)	gnVDI_CHG--;
				else			gnVDI_4FPS = gnViIrqCnt;
				gnViIrqCnt = 0;
			}
			else gnViIrqCnt++;

			//printf("VI:%d\r\n", gnViIrqCnt);
		}
		if (IRQ_ISP1){CLI_VLOCKWw(1);}
		if (IRQ_ISP2){CLI_VLOCKOw(1);// gnVoIrqOn = 1;
			gnVoIrqCnt++;
			//if(!(gnVoIrqCnt%(FPS_VDO*5))) _printf("VLOCKO_IRQ %d!!!\n", gnVoIrqCnt/FPS_VDO);	// TODO KSH ◆ VLOCKO IRQ test
			//else if(!(gnVoIrqCnt%FPS_VDO)) _printf_irq("VLOCKO_IRQ %d\n", gnVoIrqCnt/FPS_VDO);
			IRQ_ISP_PRINTF("VLOCKO_IRQ\n");
			//printf("2");

			gptMsgShare.VIDEO_TICK++;
			//if (jpeg_enc.enable == 1) {
#if 0
			if (SYS_REG6 == 0) {
				while(SYS_MUTEX10);
				enx_jpeg_enc_start(30, 0, 0);
				SYS_MUTEX10 = 0;
			}
#endif
		}
		if (IRQ_ISP3) {
			//printf("J");

			// JPEG enc mode
			// jpeg_irq_handler(NULL);

			// JPEG dec mode
			printf("JPEG dec done?");

			CLI_JPGw(1);
		}
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
		enx_exirq_view();
	}

	IRQ_ISP_PRINTF("\n");
}

void enx_exirq_source2(void)
{
	if (IRQ_VCODEC) {
		printf("IRQ_VCODEC\n");
	} else {
		printf("NO IRQ_VCODEC\n");
		enx_exirq_view();
	}
}

void enx_exirq_source3(void)
{
	uint32 get = IRQ_03_T;
	_IRQ_2 *irq = (void *)&get;
	if (irq->G_DMA0) {
		if (irq->BDMA0){IrqBDma(0);}
		if (irq->CDMA0){IrqCDma(0);}
#if EN675_SINGLE
		if (irq->BDMA1){IrqBDma(1);}
		if (irq->CDMA1){IrqCDma(1);}
		if (irq->BDMA2){IrqBDma(2);}
		if (irq->CDMA2){IrqCDma(2);}
		if (irq->BDMA3){IrqBDma(3);}
		if (irq->CDMA3){IrqCDma(3);}
#endif
	} else {
		printf("NO IRQ_G_DMA0\n");
		enx_exirq_view();
	}
}

void enx_exirq_source4(void)
{
	uint32 get = IRQ_04_T;
	_IRQ_3 *irq = (void *)&get;
	if (irq->G_DMA1) {
#if EN675_SINGLE
		if (irq->BDMA4){IrqBDma(4);}
		if (irq->CDMA4){IrqCDma(4);}
		if (irq->BDMA5){IrqBDma(5);}
		if (irq->CDMA5){IrqCDma(5);}
		if (irq->BDMA6){IrqBDma(6);}
		if (irq->CDMA6){IrqCDma(6);}
		if (irq->BDMA7){IrqBDma(7);}
		if (irq->CDMA7){IrqCDma(7);}
#else
		if (irq->BDMA1){IrqBDma(1);}
		if (irq->CDMA1){IrqCDma(1);}
#endif
	} else {
		printf("NO IRQ_G_DMA1\n");
		enx_exirq_view();
	}
}

void enx_exirq_source5(void)
{
	uint32 get = IRQ_05_T;
	_IRQ_4 *irq = (void *)&get;
	if (irq->G_DMA2) {
#if EN675_SINGLE
		if (irq->BDMA8){IrqBDma(8);}
		if (irq->CDMA8){IrqCDma(8);}
		if (irq->BDMA9){IrqBDma(9);}
		if (irq->CDMA9){IrqCDma(9);}
		if (irq->BDMA10){IrqBDma(10);}
		if (irq->CDMA10){IrqCDma(10);}
		if (irq->BDMA11){IrqBDma(11);}
		if (irq->CDMA11){IrqCDma(11);}
#else
		if (irq->BDMA2){IrqBDma(2);}
		if (irq->CDMA2){IrqCDma(2);}
#endif
	} else {
		printf("NO IRQ_G_DMA2\n");
		enx_exirq_view();
	}
}

void enx_exirq_source6(void)
{
	uint32 get = IRQ_06_T;
	_IRQ_5 *irq = (void *)&get;
	if (irq->G_DMA3) {
#if EN675_SINGLE
		if (irq->BDMA12){IrqBDma(12);}
		if (irq->CDMA12){IrqCDma(12);}
		if (irq->BDMA13){IrqBDma(13);}
		if (irq->CDMA13){IrqCDma(13);}
		if (irq->BDMA14){IrqBDma(14);}
		if (irq->CDMA14){IrqCDma(14);}
		if (irq->BDMA15){IrqBDma(15);}
		if (irq->CDMA15){IrqCDma(15);}
#else
		if (irq->BDMA3){IrqBDma(3);}
		if (irq->CDMA3){IrqCDma(3);}
#endif
	} else {
		printf("NO IRQ_G_DMA3\n");
		enx_exirq_view();
	}
}

void enx_exirq_source7(void)
{
	if (IRQ_ATOB) {
		IrqAtoB();
	} else {
		printf("NO IRQ_ATOB\n");
		enx_exirq_view();
	}
}

void enx_exirq_source8(void)
{
	if (IRQ_BTOA) {
		IrqBtoA();
	} else {
		printf("NO IRQ_BTOA\n");
		enx_exirq_view();
	}
}

void enx_exirq_source9(void)
{
	if (IRQ_G_ETH) {
		if (IRQ_ETH_TX) {IrqEthTx();}
		if (IRQ_ETH_RX) {IrqEthRx();}
	} else {
		printf("NO IRQ_G_ETH\n");
		enx_exirq_view();
	}
}

void enx_exirq_source10(void)
{
	if (IRQ_SDIO0) {
		IrqSdio(0);
	} else {
		printf("NO IRQ_SDIO0\n");
		enx_exirq_view();
	}
}

void enx_exirq_source11(void)
{
	if (IRQ_SDIO1) {
		IrqSdio(1);
	} else {
		printf("NO IRQ_SDIO1\n");
		enx_exirq_view();
	}
}

void enx_exirq_source12(void)
{
	if (IRQ_NPU) {
		printf("IRQ_NPU\n");
	} else {
		printf("NO IRQ_NPU\n");
		enx_exirq_view();
	}
}

void enx_exirq_source13(void)
{
//	_Cprintf("1");
	if (IRQ_G_I2S) {
		if (IRQ_I2S_RX){//_Cprintf("R");
			IrqI2sRx();}
		if (IRQ_I2S_TX){//_Cprintf("T");
			IrqI2sTx();}
	} else {
		printf("NO IRQ_G_I2S\n");
		enx_exirq_view();
	}
}

void enx_exirq_source14(void)
{
	if (IRQ_G_USB) {
		if (IRQ_USB_DMA){printf("IRQ_USB_DMA\n");}
		if (IRQ_USB_MC){printf("IRQ_USB_MC\n");}
	} else {
		printf("NO IRQ_G_USB\n");
		enx_exirq_view();
	}
}

void enx_exirq_source15(void)
{
//	printf("15 ChksumIsIrq(%d/%d) ShaIsIrq(%d/%d) AesIsIrq(%d/%d)\n", IRQ_CHKSUM, ChksumIsIrq(), IRQ_SHA, ShaIsIrq(), IRQ_AES, AesIsIrq());
	if (IRQ_CHKSUM) {
		IrqChksum();
	} else {
		printf("NO IRQ_CHKSUM\n");
		enx_exirq_view();
	}
}

void enx_exirq_source16(void)
{
//	printf("16 ChksumIsIrq(%d/%d) ShaIsIrq(%d/%d) AesIsIrq(%d/%d)\n", IRQ_CHKSUM, ChksumIsIrq(), IRQ_SHA, ShaIsIrq(), IRQ_AES, AesIsIrq());
#if EN675_SINGLE
	uint32 get = IRQ_16_T;
	_IRQ_15 *irq = (void *)&get;
	if (irq->SHA_ONESHOT) {
		IrqShaOneshot();
	} else if (irq->SHA_CHOP) {
		IrqShaChop();
	} else {
#else
	if (IRQ_SHA) {
		IrqSha();
	} else {
#endif
		printf("NO IRQ_SHA\n");
		enx_exirq_view();
	}
}

void enx_exirq_source17(void)
{
//	printf("17 ChksumIsIrq(%d/%d) ShaIsIrq(%d/%d) AesIsIrq(%d/%d)\n", IRQ_CHKSUM, ChksumIsIrq(), IRQ_SHA, ShaIsIrq(), IRQ_AES, AesIsIrq());
	if (IRQ_AES) {
		IrqAes();
	} else {
		printf("NO IRQ_AES\n");
		enx_exirq_view();
	}
}

void enx_exirq_source18(void)
{
	if (IRQ_OIC) {
		IrqOic();
	} else {
		printf("NO IRQ_OIC\n");
		enx_exirq_view();
	}
}

void enx_exirq_source19(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_19_T;
	_IRQ_18 *irq = (void *)&get;
	if (irq->G_SOURCE19) {
		if (irq->SPI0){IrqSpi(0);}
		if (irq->TIMER3){IrqTimer(3);}
		if (irq->TIMER2){IrqTimer(2);}
		if (irq->TIMER1){IrqTimer(1);}
		if (irq->TIMER0){IrqTimer(0);}
		if (irq->GPIO3){IrqGpio(3);}
		if (irq->GPIO2){IrqGpio(2);}
		if (irq->GPIO1){IrqGpio(1);}
		if (irq->GPIO0){IrqGpio(0);}
	} else {
		printf("NO IRQ_G_SOURCE19\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE19) {
		if (IRQ_SPI0){IrqSpi(0);}
		if (IRQ_TIMER3){IrqTimer(3);}
		if (IRQ_TIMER2){IrqTimer(2);}
		if (IRQ_TIMER1){IrqTimer(1);}
		if (IRQ_TIMER0){IrqTimer(0);}
		if (IRQ_GPIO3){IrqGpio(3);}
		if (IRQ_GPIO2){IrqGpio(2);}
		if (IRQ_GPIO1){IrqGpio(1);}
		if (IRQ_GPIO0){IrqGpio(0);}
	} else {
		printf("NO IRQ_G_SOURCE19\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source20(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_20_T;
	_IRQ_19 *irq = (void *)&get;
	if (irq->G_SOURCE20) {
		if (irq->I2C0){IrqI2c(0);}
		if (irq->UART0){IrqUart(0);}
		if (irq->TIMER7){IrqTimer(7);}
		if (irq->TIMER6){IrqTimer(6);}
		if (irq->TIMER5){IrqTimer(5);}
		if (irq->TIMER4){IrqTimer(4);}
		if (irq->GPIO7){IrqGpio(7);}
		if (irq->GPIO6){IrqGpio(6);}
		if (irq->GPIO5){IrqGpio(5);}
		if (irq->GPIO4){IrqGpio(4);}
	} else {
		printf("NO IRQ_G_SOURCE20\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE20) {
		if (IRQ_I2C0){IrqI2c(0);}
		if (IRQ_UART0){IrqUart(0);}
		if (IRQ_TIMER7){IrqTimer(7);}
		if (IRQ_TIMER6){IrqTimer(6);}
		if (IRQ_TIMER5){IrqTimer(5);}
		if (IRQ_TIMER4){IrqTimer(4);}
		if (IRQ_GPIO7){IrqGpio(7);}
		if (IRQ_GPIO6){IrqGpio(6);}
		if (IRQ_GPIO5){IrqGpio(5);}
		if (IRQ_GPIO4){IrqGpio(4);}
	} else {
		printf("NO IRQ_G_SOURCE20\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source21(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_21_T;
	_IRQ_20 *irq = (void *)&get;
	if (irq->G_SOURCE21) {
		if (irq->SPI1){IrqSpi(1);}
		if (irq->TIMER11){IrqTimer(11);}
		if (irq->TIMER10){IrqTimer(10);}
		if (irq->TIMER9){IrqTimer(9);}
		if (irq->TIMER8){IrqTimer(8);}
		if (irq->GPIO11){IrqGpio(11);}
		if (irq->GPIO10){IrqGpio(10);}
		if (irq->GPIO9){IrqGpio(9);}
		if (irq->GPIO8){IrqGpio(8);}
	} else {
		printf("NO IRQ_G_SOURCE21\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE21) {
		if (IRQ_SPI1){IrqSpi(1);}
		if (IRQ_TIMER11){IrqTimer(11);}
		if (IRQ_TIMER10){IrqTimer(10);}
		if (IRQ_TIMER9){IrqTimer(9);}
		if (IRQ_TIMER8){IrqTimer(8);}
		if (IRQ_GPIO11){IrqGpio(11);}
		if (IRQ_GPIO10){IrqGpio(10);}
		if (IRQ_GPIO9){IrqGpio(9);}
		if (IRQ_GPIO8){IrqGpio(8);}
	} else {
		printf("NO IRQ_G_SOURCE21\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source22(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_22_T;
	_IRQ_21 *irq = (void *)&get;
	if (irq->G_SOURCE22) {
		if (irq->I2C1){IrqI2c(1);}
		if (irq->UART1){IrqUart(1);}
		if (irq->TIMER15){IrqTimer(15);}
		if (irq->TIMER14){IrqTimer(14);}
		if (irq->TIMER13){IrqTimer(13);}
		if (irq->TIMER12){IrqTimer(12);}
		if (irq->GPIO15){IrqGpio(15);}
		if (irq->GPIO14){IrqGpio(14);}
		if (irq->GPIO13){IrqGpio(13);}
		if (irq->GPIO12){IrqGpio(12);}
	} else {
		printf("NO IRQ_G_SOURCE22\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE22) {
		if (IRQ_I2C1){IrqI2c(1);}
		if (IRQ_UART1){IrqUart(1);}
		if (IRQ_TIMER15){IrqTimer(15);}
		if (IRQ_TIMER14){IrqTimer(14);}
		if (IRQ_TIMER13){IrqTimer(13);}
		if (IRQ_TIMER12){IrqTimer(12);}
		if (IRQ_GPIO15){IrqGpio(15);}
		if (IRQ_GPIO14){IrqGpio(14);}
		if (IRQ_GPIO13){IrqGpio(13);}
		if (IRQ_GPIO12){IrqGpio(12);}
	} else {
		printf("NO IRQ_G_SOURCE22\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source23(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_23_T;
	_IRQ_22 *irq = (void *)&get;
	if (irq->G_SOURCE23) {
		if (irq->SPI2){IrqSpi(2);}
		if (irq->GPIO19){IrqGpio(19);}
		if (irq->GPIO18){IrqGpio(18);}
		if (irq->GPIO17){IrqGpio(17);}
		if (irq->GPIO16){IrqGpio(16);}
	} else {
		printf("NO IRQ_G_SOURCE23\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE23) {
		if (IRQ_SPI2){IrqSpi(2);}
		if (IRQ_GPIO19){IrqGpio(19);}
		if (IRQ_GPIO18){IrqGpio(18);}
		if (IRQ_GPIO17){IrqGpio(17);}
		if (IRQ_GPIO16){IrqGpio(16);}
	} else {
		printf("NO IRQ_G_SOURCE23\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source24(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_24_T;
	_IRQ_23 *irq = (void *)&get;
	if (irq->G_SOURCE24) {
		if (irq->I2C2){IrqI2c(2);}
		if (irq->UART2){IrqUart(2);}
		if (irq->TIMER19){IrqTimer(19);}
		if (irq->TIMER18){IrqTimer(18);}
		if (irq->TIMER17){IrqTimer(17);}
		if (irq->TIMER16){IrqTimer(16);}
		if (irq->GPIO23){IrqGpio(23);}
		if (irq->GPIO22){IrqGpio(22);}
		if (irq->GPIO21){IrqGpio(21);}
		if (irq->GPIO20){IrqGpio(20);}
	} else {
		printf("NO IRQ_G_SOURCE24\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE24) {
		if (IRQ_I2C2){IrqI2c(2);}
		if (IRQ_UART2){IrqUart(2);}
		if (IRQ_TIMER19){IrqTimer(19);}
		if (IRQ_TIMER18){IrqTimer(18);}
		if (IRQ_TIMER17){IrqTimer(17);}
		if (IRQ_TIMER16){IrqTimer(16);}
		if (IRQ_GPIO23){IrqGpio(23);}
		if (IRQ_GPIO22){IrqGpio(22);}
		if (IRQ_GPIO21){IrqGpio(21);}
		if (IRQ_GPIO20){IrqGpio(20);}
	} else {
		printf("NO IRQ_G_SOURCE24\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source25(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_25_T;
	_IRQ_24 *irq = (void *)&get;
	if (irq->G_SOURCE25) {
		if (irq->SPI3){IrqSpi(3);}
		if (irq->TIMER23){IrqTimer(23);}
		if (irq->TIMER22){IrqTimer(22);}
		if (irq->TIMER21){IrqTimer(21);}
		if (irq->TIMER20){IrqTimer(20);}
		if (irq->GPIO27){IrqGpio(27);}
		if (irq->GPIO26){IrqGpio(26);}
		if (irq->GPIO25){IrqGpio(25);}
		if (irq->GPIO24){IrqGpio(24);}
	} else {
		printf("NO IRQ_G_SOURCE25\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE25) {
		if (IRQ_SPI3){IrqSpi(3);}
		if (IRQ_TIMER23){IrqTimer(23);}
		if (IRQ_TIMER22){IrqTimer(22);}
		if (IRQ_TIMER21){IrqTimer(21);}
		if (IRQ_TIMER20){IrqTimer(20);}
		if (IRQ_GPIO27){IrqGpio(27);}
		if (IRQ_GPIO26){IrqGpio(26);}
		if (IRQ_GPIO25){IrqGpio(25);}
		if (IRQ_GPIO24){IrqGpio(24);}
	} else {
		printf("NO IRQ_G_SOURCE25\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source26(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_26_T;
	_IRQ_25 *irq = (void *)&get;
	if (irq->G_SOURCE26) {
		if (irq->I2C3){IrqI2c(3);}
		if (irq->UART3){IrqUart(3);}
		if (irq->TIMER27){IrqTimer(27);}
		if (irq->TIMER26){IrqTimer(26);}
		if (irq->TIMER25){IrqTimer(25);}
		if (irq->TIMER24){IrqTimer(24);}
		if (irq->GPIO31){IrqGpio(31);}
		if (irq->GPIO30){IrqGpio(30);}
		if (irq->GPIO29){IrqGpio(29);}
		if (irq->GPIO28){IrqGpio(28);}
	} else {
		printf("NO IRQ_G_SOURCE26\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE26) {
		if (IRQ_I2C3){IrqI2c(3);}
		if (IRQ_UART3){IrqUart(3);}
		if (IRQ_TIMER27){IrqTimer(27);}
		if (IRQ_TIMER26){IrqTimer(26);}
		if (IRQ_TIMER25){IrqTimer(25);}
		if (IRQ_TIMER24){IrqTimer(24);}
		if (IRQ_GPIO31){IrqGpio(31);}
		if (IRQ_GPIO30){IrqGpio(30);}
		if (IRQ_GPIO29){IrqGpio(29);}
		if (IRQ_GPIO28){IrqGpio(28);}
	} else {
		printf("NO IRQ_G_SOURCE26\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source27(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_27_T;
	_IRQ_26 *irq = (void *)&get;
	if (irq->G_SOURCE27) {
		if (irq->SPI4){IrqSpi(4);}
		if (irq->TIMER31){IrqTimer(31);}
		if (irq->TIMER30){IrqTimer(30);}
		if (irq->TIMER29){IrqTimer(29);}
		if (irq->TIMER28){IrqTimer(28);}
		if (irq->GPIO35){IrqGpio(35);}
		if (irq->GPIO34){IrqGpio(34);}
		if (irq->GPIO33){IrqGpio(33);}
		if (irq->GPIO32){IrqGpio(32);}
	} else {
		printf("NO IRQ_G_SOURCE27\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE27) {
		if (IRQ_SPI4){IrqSpi(4);}
		if (IRQ_TIMER31){IrqTimer(31);}
		if (IRQ_TIMER30){IrqTimer(30);}
		if (IRQ_TIMER29){IrqTimer(29);}
		if (IRQ_TIMER28){IrqTimer(28);}
		if (IRQ_GPIO35){IrqGpio(35);}
		if (IRQ_GPIO34){IrqGpio(34);}
		if (IRQ_GPIO33){IrqGpio(33);}
		if (IRQ_GPIO32){IrqGpio(32);}
	} else {
		printf("NO IRQ_G_SOURCE27\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source28(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_28_T;
	_IRQ_27 *irq = (void *)&get;
	if (irq->G_SOURCE28) {
		if (irq->I2C4){IrqI2c(4);}
		if (irq->UART4){IrqUart(4);}
		if (irq->GPIO39){IrqGpio(39);}
		if (irq->GPIO38){IrqGpio(38);}
		if (irq->GPIO37){IrqGpio(37);}
		if (irq->GPIO36){IrqGpio(36);}
	} else {
		printf("NO IRQ_G_SOURCE28\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE28) {
		if (IRQ_I2C4){IrqI2c(4);}
		if (IRQ_UART4){IrqUart(4);}
		if (IRQ_GPIO39){IrqGpio(39);}
		if (IRQ_GPIO38){IrqGpio(38);}
		if (IRQ_GPIO37){IrqGpio(37);}
		if (IRQ_GPIO36){IrqGpio(36);}
	} else {
		printf("NO IRQ_G_SOURCE28\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source29(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_29_T;
	_IRQ_28 *irq = (void *)&get;
	if (irq->G_SOURCE29) {
		if (irq->SPI5){IrqSpi(5);}
		if (irq->GPIO43){IrqGpio(43);}
		if (irq->GPIO42){IrqGpio(42);}
		if (irq->GPIO41){IrqGpio(41);}
		if (irq->GPIO40){IrqGpio(40);}
	} else {
		printf("NO IRQ_G_SOURCE29\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE29) {
		if (IRQ_SPI5){IrqSpi(5);}
		if (IRQ_GPIO43){IrqGpio(43);}
		if (IRQ_GPIO42){IrqGpio(42);}
		if (IRQ_GPIO41){IrqGpio(41);}
		if (IRQ_GPIO40){IrqGpio(40);}
	} else {
		printf("NO IRQ_G_SOURCE29\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source30(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_30_T;
	_IRQ_29 *irq = (void *)&get;
	if (irq->G_SOURCE30) {
		if (irq->I2C5){IrqI2c(5);}
		if (irq->UART5){IrqUart(5);}
		if (irq->GPIO47){IrqGpio(47);}
		if (irq->GPIO46){IrqGpio(46);}
		if (irq->GPIO45){IrqGpio(45);}
		if (irq->GPIO44){IrqGpio(44);}
	} else {
		printf("NO IRQ_G_SOURCE30\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE30) {
		if (IRQ_I2C5){IrqI2c(5);}
		if (IRQ_UART5){IrqUart(5);}
		if (IRQ_GPIO47){IrqGpio(47);}
		if (IRQ_GPIO46){IrqGpio(46);}
		if (IRQ_GPIO45){IrqGpio(45);}
		if (IRQ_GPIO44){IrqGpio(44);}
	} else {
		printf("NO IRQ_G_SOURCE30\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source31(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_31_T;
	_IRQ_30 *irq = (void *)&get;
	if (irq->G_SOURCE31) {
		if (irq->SPI6){IrqSpi(6);}
		if (irq->GPIO51){IrqGpio(51);}
		if (irq->GPIO50){IrqGpio(50);}
		if (irq->GPIO49){IrqGpio(49);}
		if (irq->GPIO48){IrqGpio(48);}
	} else {
		printf("NO IRQ_G_SOURCE31\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE31) {
		if (IRQ_SPI6){IrqSpi(6);}
		if (IRQ_GPIO51){IrqGpio(51);}
		if (IRQ_GPIO50){IrqGpio(50);}
		if (IRQ_GPIO49){IrqGpio(49);}
		if (IRQ_GPIO48){IrqGpio(48);}
	} else {
		printf("NO IRQ_G_SOURCE31\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source32(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_32_T;
	_IRQ_31 *irq = (void *)&get;
	if (irq->G_SOURCE32) {
		if (irq->I2C6){IrqI2c(6);}
		if (irq->UART6){IrqUart(6);}
		if (irq->TIMER34){IrqTimer(34);}
		if (irq->TIMER33){IrqTimer(33);}
		if (irq->TIMER32){IrqTimer(32);}
		if (irq->GPIO55){IrqGpio(55);}
		if (irq->GPIO54){IrqGpio(54);}
		if (irq->GPIO53){IrqGpio(53);}
		if (irq->GPIO52){IrqGpio(52);}
	} else {
		printf("NO IRQ_G_SOURCE32\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE32) {
		if (IRQ_I2C6){IrqI2c(6);}
		if (IRQ_UART6){IrqUart(6);}
		if (IRQ_TIMER34){IrqTimer(34);}
		if (IRQ_TIMER33){IrqTimer(33);}
		if (IRQ_TIMER32){IrqTimer(32);}
		if (IRQ_GPIO55){IrqGpio(55);}
		if (IRQ_GPIO54){IrqGpio(54);}
		if (IRQ_GPIO53){IrqGpio(53);}
		if (IRQ_GPIO52){IrqGpio(52);}
	} else {
		printf("NO IRQ_G_SOURCE32\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source33(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_33_T;
	_IRQ_32 *irq = (void *)&get;
	if (irq->G_SOURCE33) {
		if (irq->SPI7){IrqSpi(7);}
		if (irq->TIMER38){IrqTimer(38);}
		if (irq->TIMER37){IrqTimer(37);}
		if (irq->TIMER36){IrqTimer(36);}
		if (irq->TIMER35){IrqTimer(35);}
		if (irq->GPIO59){IrqGpio(59);}
		if (irq->GPIO58){IrqGpio(58);}
		if (irq->GPIO57){IrqGpio(57);}
		if (irq->GPIO56){IrqGpio(56);}
	} else {
		printf("NO G_SOURCE33\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE33) {
		if (IRQ_SPI7){IrqSpi(7);}
		if (IRQ_TIMER38){IrqTimer(38);}
		if (IRQ_TIMER37){IrqTimer(37);}
		if (IRQ_TIMER36){IrqTimer(36);}
		if (IRQ_TIMER35){IrqTimer(35);}
		if (IRQ_GPIO59){IrqGpio(59);}
		if (IRQ_GPIO58){IrqGpio(58);}
		if (IRQ_GPIO57){IrqGpio(57);}
		if (IRQ_GPIO56){IrqGpio(56);}
	} else {
		printf("NO IRQ_G_SOURCE33\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source34(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_34_T;
	_IRQ_33 *irq = (void *)&get;
	if (irq->G_SOURCE34) {
		if (irq->I2C7){IrqI2c(7);}
		if (irq->UART7){IrqUart(7);}
		if (irq->GPIO63){IrqGpio(63);}
		if (irq->GPIO62){IrqGpio(62);}
		if (irq->GPIO61){IrqGpio(61);}
		if (irq->GPIO60){IrqGpio(60);}
	} else {
		printf("NO G_SOURCE34\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE34) {
		if (IRQ_I2C7){IrqI2c(7);}
		if (IRQ_UART7){IrqUart(7);}
		if (IRQ_GPIO63){IrqGpio(63);}
		if (IRQ_GPIO62){IrqGpio(62);}
		if (IRQ_GPIO61){IrqGpio(61);}
		if (IRQ_GPIO60){IrqGpio(60);}
	} else {
		printf("NO IRQ_G_SOURCE34\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source35(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_35_T;
	_IRQ_34 *irq = (void *)&get;
	if (irq->G_SOURCE35) {
		if (irq->SPI8){IrqSpi(8);}
		if (irq->GPIO67){IrqGpio(67);}
		if (irq->GPIO66){IrqGpio(66);}
		if (irq->GPIO65){IrqGpio(65);}
		if (irq->GPIO64){IrqGpio(64);}
	} else {
		printf("NO G_SOURCE35\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE35) {
		if (IRQ_SPI8){IrqSpi(8);}
		if (IRQ_GPIO67){IrqGpio(67);}
		if (IRQ_GPIO66){IrqGpio(66);}
		if (IRQ_GPIO65){IrqGpio(65);}
		if (IRQ_GPIO64){IrqGpio(64);}
	} else {
		printf("NO IRQ_G_SOURCE35\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source36(void)
{
#if EXIRQ_POINT
	uint32 get = IRQ_36_T;
	_IRQ_35 *irq = (void *)&get;
	if (irq->G_SOURCE36) {
		if (irq->I2C8){IrqI2c(8);}
		if (irq->UART8){IrqUart(8);}
		if (irq->GPIO71){IrqGpio(71);}
		if (irq->GPIO70){IrqGpio(70);}
		if (irq->GPIO69){IrqGpio(69);}
		if (irq->GPIO68){IrqGpio(68);}
	} else {
		printf("NO G_SOURCE36\n");
		enx_exirq_view();
	}
#else
	if (IRQ_G_SOURCE36) {
		if (IRQ_I2C8){IrqI2c(8);}
		if (IRQ_UART8){IrqUart(8);}
		if (IRQ_GPIO71){IrqGpio(71);}
		if (IRQ_GPIO70){IrqGpio(70);}
		if (IRQ_GPIO69){IrqGpio(69);}
		if (IRQ_GPIO68){IrqGpio(68);}
	} else {
		printf("NO IRQ_G_SOURCE36\n");
		enx_exirq_view();
	}
#endif
}

void enx_exirq_source37(void)
{
#if 1
	if (IRQ_IR) {
		IrqIr();
	} else {
		printf("NO G_SOURCE37\n");
		enx_exirq_view();
	}
#else
	printf("%s\n", __func__);
	enx_exirq_view();
#endif
}

void enx_exirq_source38(void)
{
#if 0
	uint32 get = IRQ_38_T;
	_IRQ_37 *irq = (void *)&get;
	if (irq->G_SOURCE38) {
		if (irq->OMC_COOLDONWN) {printf("IRQ_OMC_COOLDONWN\n");}
		if (irq->OMC_HIGHTEMP) {printf("IRQ_OMC_HIGHTEMP\n");}
		if (irq->OMC_OVERTEMP) {printf("IRQ_OMC_OVERTEMP\n");}
		if (irq->OMC_ASP) {printf("IRQ_OMC_ASP\n");}
	} else {
		printf("NO G_SOURCE37\n");
		enx_exirq_view();
	}
#else
	printf("%s\n", __func__);
	enx_exirq_view();
#endif
}

void enx_exirq_source39(void)
{
	printf("%s\n", __func__);
	enx_exirq_view();
}

void enx_exirq_source40(void)
{
	printf("%s\n", __func__);
	enx_exirq_view();
}

////////////////////////////////////////////////////////////////////////////////
// Software Interrupt Handle
////////////////////////////////////////////////////////////////////////////////
#include "shell.h"
#include "enx_stream.h"

void enx_swirq_hart0(void)
{
	while (CPU0_MSG_MUTEX);			// lock
	UINT msg_flag = CPU0_MSG_FLAG;	// flag copy
	CPU0_MSG_FLAG = 0;				// clear
	CPU0_MSG_MUTEX = 0;				// unlock

	if (msg_flag & eCPU0_MSG_SHELL_RX) {
		msg_flag &= ~eCPU0_MSG_SHELL_RX;
#ifdef __ECM_STRING__
		IsrShell();
#endif
	}

	if (msg_flag & eCPU0_MSG_STREAM_INFO) {
		msg_flag &= ~eCPU0_MSG_STREAM_INFO;
#if defined(__NETWORK__)
		IsrStreamdata();
#endif
	}

	if (msg_flag) {
		printf("SWIRQ_CPU0: flag remains(0x%08X)\n", msg_flag);
	}
}

void enx_swirq_hart1(void)
{
	printf("SWIRQ CPU1\n");

	while (CPU1_MSG_MUTEX);			// lock
	UINT msg_flag = CPU1_MSG_FLAG;	// flag copy
	CPU1_MSG_FLAG = 0;				// clear
	CPU1_MSG_MUTEX = 0;				// unlock

	if (msg_flag) {
		printf("SWIRQ_CPU1: flag remains(0x%08X)\n", msg_flag);
	}
}

void enx_swirq_hart2(void)
{
	printf("SWIRQ CPU2\n");

	while (CPU2_MSG_MUTEX);			// lock
	UINT msg_flag = CPU2_MSG_FLAG;	// flag copy
	CPU2_MSG_FLAG = 0;				// clear
	CPU2_MSG_MUTEX = 0;				// unlock

	if (msg_flag) {
		printf("SWIRQ_CPU2: flag remains(0x%08X)\n", msg_flag);
	}
}

void enx_swirq_hart3(void)
{
	printf("SWIRQ CPU3\n");

	while (CPU3_MSG_MUTEX);			// lock
	UINT msg_flag = CPU3_MSG_FLAG;	// flag copy
	CPU3_MSG_FLAG = 0;				// clear
	CPU3_MSG_MUTEX = 0;				// unlock

	if (msg_flag) {
		printf("SWIRQ_CPU3: flag remains(0x%08X)\n", msg_flag);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Interrupt status
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
// Trap Handle
////////////////////////////////////////////////////////////////////////////////
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

	uint64 cpu_id = CPU_ID;
	printf("cpuid       : %lu\n", cpu_id);
	printf("uptime      : %lus\n", gptMsgShare.UPTIME);
#if (ENX_RTSP_use==1)
	printf("RTP error step: %d\n", rtp_step);
#endif

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

////////////////////////////////////////////////////////////////////////////////
// Interrupt Handle Array
////////////////////////////////////////////////////////////////////////////////
void (*exirq_source[IRQ_SOURCE_COUNT+1])(void) = {NULL,
		enx_exirq_source1, enx_exirq_source2, enx_exirq_source3, enx_exirq_source4,
		enx_exirq_source5, enx_exirq_source6, enx_exirq_source7, enx_exirq_source8,
		enx_exirq_source9, enx_exirq_source10, enx_exirq_source11, enx_exirq_source12,
		enx_exirq_source13, enx_exirq_source14, enx_exirq_source15, enx_exirq_source16,
		enx_exirq_source17, enx_exirq_source18, enx_exirq_source19, enx_exirq_source20,
		enx_exirq_source21, enx_exirq_source22, enx_exirq_source23, enx_exirq_source24,
		enx_exirq_source25, enx_exirq_source26, enx_exirq_source27, enx_exirq_source28,
		enx_exirq_source29, enx_exirq_source30, enx_exirq_source31, enx_exirq_source32,
		enx_exirq_source33, enx_exirq_source34, enx_exirq_source35, enx_exirq_source36,
		enx_exirq_source37, enx_exirq_source38, enx_exirq_source39, enx_exirq_source40,
};

void (*swirq_source[4])(void) = {
		enx_swirq_hart0, enx_swirq_hart1, enx_swirq_hart2, enx_swirq_hart3,
};

////////////////////////////////////////////////////////////////////////////////
// Timer Interrupt Handle
////////////////////////////////////////////////////////////////////////////////
void enx_timerirq_next(void)
{
    if (mtime && timecmp) {
    	uint64 cpu_id = CPU_ID;
    	//timecmp[cpu_id] = *mtime + TIMECMP_NEXT_VAL; // 1ms
    	timecmp[cpu_id] = timecmp[cpu_id] + TIMECMP_NEXT_VAL; // 1ms
    	if (cpu_id == 0) {
			static uint64 gbTickCnt = TIME_TICK - 1;
			if (gbTickCnt == 0) {
				gbTickCnt = TIME_TICK - 1;
				gptMsgShare.TIME++;
				gptMsgShare.UPTIME++;
//				printf("[%u]\n", gptMsgShare.UPTIME);
			} else {
				gbTickCnt--;
			}
    	}
    }
}

////////////////////////////////////////////////////////////////////////////////
// Interrupt
////////////////////////////////////////////////////////////////////////////////
#ifdef __FREERTOS__
void trap_from_machine_mode_freertos_sync(uintptr_t mcause, uintptr_t mepc, uint64 cpuid, uintptr_t regs[32])
{
	core_irq_count[cpuid].sync_count++;
	switch(mcause) {
	case CAUSE_USER_ECALL:
	case CAUSE_SUPERVISOR_ECALL:
	case CAUSE_MACHINE_ECALL:
		if (regs[14] == ECALL_YIELD_CMD) {
//			_Rprintf("OS switchcontext\n");
			vTaskSwitchContext();
		} else{
//			_Rprintf("do_syscall\n");
			regs[7] = do_syscall(regs[7], regs[8], regs[9], regs[10], regs[11], regs[12], regs[14]);
		}
		break;

	default:
		printf("Task name: [%s]\n", pcTaskGetName(NULL));
		bad_trap(mcause, mepc, regs);
		break;
	}
}

#if 0
void trap_from_machine_mode_freertos_handle_interrupt(int32_t mcause)
{
#if 1
	uint64 cpuidx = CPU_ID * 2;

	gbXsrTaskSwitchNeeded = 0;
//		_Cprintf(".");
	for (int i = 0; i < 2; i++) {
		volatile unsigned int *pClaim = iClaimCompliet[cpuidx+i];
		if (pClaim <= 0xC200000 && pClaim > 0xC207004) {
			printf("IRQ Error! 0x%08X\n", (UINT)(intptr_t)pClaim);
		}
		volatile unsigned int source = *iClaimCompliet[cpuidx+i]; // Get Claim IRQ
		if (source == 0 || source > IRQ_SOURCE_COUNT) {
			continue;
		}
#if 0 // Test용 log
		else if (IRQ_ETH_RX == 0 && IRQ_CDC == 0 && IRQ_I2S_TX == 0 && IRQ_I2S_RX == 0 && SDIO1_IO_IRQ == 0 && IRQ_TIMER8 == 0) {
			printf("CPU%u-OS-IRQ%d (%d/%c)\n", cpuid, source, (cpuidx+i), (cpuidx+i)%2==0 ? 'M':'S');
		}
#endif
		core_irq_count[CPU_ID].exirq_count[source]++;
		exirq_source[source]();
		*iClaimCompliet[cpuidx+i] = source;	// Set Complete IRQ
	}
	portYIELD_FROM_ISR(gbXsrTaskSwitchNeeded);
#endif
}
#endif

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
	case IRQ_M_SOFT:
//		_Rprintf(".");
		core_irq_count[cpuid].swirq_count++;
		swirq_source[cpuid]();	// Execution
		cpu_msip[cpuid] = 0;	// Clear
		break;

	case IRQ_M_TIMER:
//		_Rprintf(".");
		core_irq_count[cpuid].timeirq_count++;
		vPortSysTickHandler();
//		_Cprintf("z");
		break;

	case IRQ_M_EXT:
		gbXsrTaskSwitchNeeded = 0;
//		_Cprintf(".");
		for (int i = 0; i < 2; i++) {
#if 0
			volatile unsigned int *pClaim = iClaimCompliet[cpuidx+i];
			if (((UINT)(intptr_t)pClaim) <= 0xC200000 && ((UINT)(intptr_t)pClaim) > 0xC207004) {
				printf("IRQ Error! 0x%08X\n", (UINT)(intptr_t)pClaim);
			}
#endif
			volatile unsigned int source = *iClaimCompliet[cpuidx+i]; // Get Claim IRQ
			if (source == 0 || source > IRQ_SOURCE_COUNT) {
				continue;
			}
#if 0 // Test용 log
			else if (IRQ_ETH_RX == 0 && IRQ_CDC == 0 && IRQ_I2S_TX == 0 && IRQ_I2S_RX == 0 && SDIO1_IO_IRQ == 0 && IRQ_TIMER8 == 0) {
				printf("CPU%u-OS-IRQ%d (%d/%c)\n", cpuid, source, (cpuidx+i), (cpuidx+i)%2==0 ? 'M':'S');
			}
#endif
			core_irq_count[cpuid].exirq_count[source]++;
			exirq_source[source]();
			*iClaimCompliet[cpuidx+i] = source;	// Set Complete IRQ
		}
		portYIELD_FROM_ISR(gbXsrTaskSwitchNeeded);
		break;

	default:
		printf("IRQ.1 0x%lX\n", mcause);
		break;
	}
}
#endif

uintptr_t trap_from_machine_mode_sync(uintptr_t mcause, uintptr_t mepc, uint64 cpuid, uintptr_t regs[32])
{
	core_irq_count[cpuid].sync_count++;
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
	mcause = (mcause << 1) >> 1;
	switch (mcause) {
	case IRQ_M_SOFT:
		core_irq_count[cpuid].swirq_count++;
		swirq_source[cpuid]();	// Execution
		cpu_msip[cpuid] = 0;	// Clear
		break;

	case IRQ_M_TIMER:
		core_irq_count[cpuid].timeirq_count++;
		enx_timerirq_next();
		break;

	case IRQ_M_EXT:
		for (int i = 0; i < 2; i++) {
			volatile unsigned int *pClaim = iClaimCompliet[cpuidx+i];
			if (((UINT)(intptr_t)pClaim) <= 0xC200000 && ((UINT)(intptr_t)pClaim) > 0xC207004) {
				printf("IRQ Error! 0x%08X\n", (UINT)(intptr_t)pClaim);
			}
			volatile unsigned int source = *iClaimCompliet[cpuidx+i]; // Get Claim IRQ
			if (source == 0 || source > IRQ_SOURCE_COUNT) {
				continue;
			}
#if 1 // Test용 log
			else if (source != 1 && source != 4 && source != 9) {
				printf("CPU%u-FW-IRQ%d (%d/%c)\n", cpuid, source, (cpuidx+i), (cpuidx+i)%2==0 ? 'M':'S');
			}
#endif
			core_irq_count[cpuid].exirq_count[source]++;
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

////////////////////////////////////////////////////////////////////////////////
// Interrupt Controller
////////////////////////////////////////////////////////////////////////////////
UINT enx_externalirq_is_enable(UINT cpuid, eIRQ_GROUP_INDEX perlIdx, uint64 type)
{
	uint32 enableidx = perlIdx % 32;
	uint32 enabletype = (type << 1) + (perlIdx / 32);
	cpuid = cpuid * 4;
	return (*iEnables[cpuid+enabletype] >> enableidx) & 0x1;
}

void enx_externalirq_perl(eIRQ_GROUP_INDEX perlIdx, uint64 onoff, uint64 type)
{
	if (type > 1) {
		printf("Error type(%lu) (M-mode:0 S-mode:1)\n", type);
		return;
	}

	if (onoff) {
		*iPrioBase[perlIdx] = 1; // 1 ~ IRQ_SOURCE_COUNT
	} else {
		*iPrioBase[perlIdx] = 0;
	}

	uint64 cpuid = CPU_ID * 4;
	uint32 enableidx = perlIdx % 32;
	uint32 enabletype = (type << 1) + (perlIdx / 32);
	if (onoff) {
		*iEnables[cpuid+enabletype] |= 1 << (enableidx + 0);
	} else {
		*iEnables[cpuid+enabletype] &= ~(1 << (enableidx + 0));
	}
	*iThreshold[cpuid+type] = 0;
}

/* Used to program the machine timer compare register. */
uint64_t ullNextTime = 0ULL;
const uint64_t *pullNextTime = &ullNextTime;
const size_t uxTimerIncrementsForOneTick = ( size_t ) ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ); /* Assumes increment won't go over 32-bits. */
volatile uint64_t * const pullMachineTimerCompareRegister = ( volatile uint64_t * const ) ( CLINT_BASE + 0x4000 );

void enx_timerirq_init(void)
{
	/* reuse existing routine */
#if 0
	uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;
	volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) ( CLINT_BASE + 0xBFFC );
	volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) ( CLINT_BASE + 0xBFF8 );

	do
	{
		ulCurrentTimeHigh = *pulTimeHigh;
		ulCurrentTimeLow = *pulTimeLow;
	} while( ulCurrentTimeHigh != *pulTimeHigh );

	ullNextTime = ( uint64_t ) ulCurrentTimeHigh;
	ullNextTime <<= 32ULL;
	ullNextTime |= ( uint64_t ) ulCurrentTimeLow;
	ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
	*pullMachineTimerCompareRegister = ullNextTime;

	/* Prepare the time to use after the next tick interrupt. */
	ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
#else
	uint64 cpu_id = CPU_ID;
	timecmp[cpu_id] = *mtime + TIMECMP_NEXT_VAL;						// Next timer Interrupt
#endif
	set_csr(mie, MIP_MTIP);												// Enable the Machine-Timer bit in MIE
}

void enx_externalirq_all_disable(void)
{
	for (uint32 i = 0; i < ARRAY_SIZE(iEnables); i++) {
		*iEnables[i] = 0;
	}
}

static void enx_externalirq_disable(void)
{
	uint64 cpuid = CPU_ID * 4;
	for (uint64 i = 0; i < 4; i++) {
		*iEnables[i + cpuid] = 0;
	}
}

void enx_externalirq_init_cpu0(void)
{
	enx_externalirq_disable();

	set_csr(mie, MIP_MEIP|MIP_MSIP);									// Enable External Interrupts, Software Interrupt
//	set_csr(mie, MIP_MEIP);												// Enable External Interrupts
	set_csr(mstatus, MSTATUS_MIE);										// Machine Interrupt Enable

//	set_csr(mstatus, MSTATUS_MPIE);										// Machine Previous Interrupt Enabler
//	set_csr(mstatus, MSTATUS_MPP);										// Machine Previous Privilege Mode

//	enx_externalirq_perl(eigiISP, ENX_ON, 0);							// Enable ISP Interrupts
//	enx_externalirq_perl(eigiVCODEC, ENX_ON, 0);						// Enable VCodec Interrupts
#if EN675_SINGLE
	enx_externalirq_perl(eigiDMA0_3, ENX_ON, 0);						// Enable DMA0~3 Interrupts
//	enx_externalirq_perl(eigiDMA4_7, ENX_ON, 0);						// Enable DMA4~7 Interrupts
//	enx_externalirq_perl(eigiDMA8_11, ENX_ON, 0);						// Enable DMA8~11 Interrupts
//	enx_externalirq_perl(eigiDMA12_15, ENX_ON, 0);						// Enable DMA12~15 Interrupts
#else
	enx_externalirq_perl(eigiDMA0, ENX_ON, 0);							// Enable DMA0 Interrupts
//	enx_externalirq_perl(eigiDMA1, ENX_ON, 0);							// Enable DMA1 Interrupts
//	enx_externalirq_perl(eigiDMA2, ENX_ON, 0);							// Enable DMA2 Interrupts
//	enx_externalirq_perl(eigiDMA3, ENX_ON, 0);							// Enable DMA3 Interrupts
#endif
	enx_externalirq_perl(eigiETH, ENX_ON, 0);							// Enable ETH_TX Interrupts
	enx_externalirq_perl(eigiSDIO0, ENX_ON, 0);							// Enable SDIO0 Interrupts
	enx_externalirq_perl(eigiSDIO1, ENX_ON, 0);							// Enable SDIO1 Interrupts
	enx_externalirq_perl(eigiNPU, ENX_ON, 0);							// Enable NPU Interrupts
	enx_externalirq_perl(eigiI2S, ENX_ON, 0);							// Enable I2S Interrupts
	enx_externalirq_perl(eigiUSB, ENX_ON, 0);							// Enable USB Interrupts
	enx_externalirq_perl(eigiCHKSUM, ENX_ON, 0);						// Enable CHKSUM Interrupts
	enx_externalirq_perl(eigiAES, ENX_ON, 0);							// Enable AES Interrupts
	enx_externalirq_perl(eigiSHA, ENX_ON, 0);							// Enable SHA Interrupts
	enx_externalirq_perl(eigiOIC, ENX_ON, 0);							// Enable OIC Interrupts
	enx_externalirq_perl(eigiBTOA, ENX_ON, 0);							// Enable BTOA Interrupts
	enx_externalirq_perl(eigiGPIO0_3_PWM0_3_SPI0, ENX_ON, 0);			// Enable GPIO0~3, PWM0~3, SPI0 Interrupts
	enx_externalirq_perl(eigiGPIO4_7_PWM4_7_I2C0_UART0, ENX_ON, 0);		// Enable GPIO4~7, PWM4~7, I2C0, UART0 Interrupts
	enx_externalirq_perl(eigiGPIO8_11_PWM8_11_SPI1, ENX_ON, 0);			// Enable GPIO8~11, PWM8~11, SPI1 Interrupts
	enx_externalirq_perl(eigiGPIO12_15_PWM12_15_I2C1_UART1, ENX_ON, 0);	// Enable GPIO12~15, PWM12~15, I2C1, UART1 Interrupts
	enx_externalirq_perl(eigiGPIO16_19_SPI2, ENX_ON, 0);				// Enable GPIO16~19, SPI2 Interrupts
	enx_externalirq_perl(eigiGPIO20_23_PWM16_19_I2C2_UART2, ENX_ON, 0);	// Enable GPIO20~23, PWM16~19, I2C2, UART2 Interrupts
	enx_externalirq_perl(eigiGPIO24_27_PWM20_23_SPI3, ENX_ON, 0);		// Enable GPIO24~27, PWM20~23, SPI3 Interrupts
	enx_externalirq_perl(eigiGPIO28_31_PWM24_27_I2C3_UART3, ENX_ON, 0);	// Enable GPIO28~31, PWM24~27, I2C3, UART3 Interrupts
	enx_externalirq_perl(eigiGPIO32_35_PWM28_31_SPI4, ENX_ON, 0);		// Enable GPIO32~35, PWM28~31, SPI4 Interrupts
	enx_externalirq_perl(eigiGPIO36_39_I2C4_UART4, ENX_ON, 0);			// Enable GPIO36~39, I2C4, UART4 Interrupts
	enx_externalirq_perl(eigiGPIO40_43_SPI5, ENX_ON, 0);				// Enable GPIO40~43, SPI5 Interrupts
	enx_externalirq_perl(eigiGPIO44_47_I2C5_UART5, ENX_ON, 0);			// Enable GPIO44~47, I2C5, UART5 Interrupts
	enx_externalirq_perl(eigiGPIO48_51_SPI6, ENX_ON, 0);				// Enable GPIO48~51, SPI6 Interrupts
	enx_externalirq_perl(eigiGPIO52_55_PWM32_34_I2C6_UART6, ENX_ON, 0);	// Enable GPIO52~55, PWM32~34, I2C6, UART6 Interrupts
	enx_externalirq_perl(eigiGPIO56_59_PWM35_38_SPI7, ENX_ON, 0);		// Enable GPIO56~59, PWM35~38, SPI7 Interrupts
	enx_externalirq_perl(eigiGPIO60_63_I2C7_UART7, ENX_ON, 0);			// Enable GPIO60~63, I2C7, UART7 Interrupts
	enx_externalirq_perl(eigiGPIO64_67_SPI8, ENX_ON, 0);				// Enable GPIO64~67, SPI8 Interrupts
	enx_externalirq_perl(eigiGPIO68_71_I2C8_UART8, ENX_ON, 0);			// Enable GPIO68~71, I2C8, UART8 Interrupts
	enx_externalirq_perl(eigiIR, ENX_ON, 0);							// Enable IR Interrupts
	enx_externalirq_perl(eigiOMC, ENX_ON, 0);							// Enable OMC Interrupts
	enx_externalirq_perl(eigiReserved39, ENX_ON, 0);					// Enable Reserved39 Interrupts
	enx_externalirq_perl(eigiReserved40, ENX_ON, 0);					// Enable Reserved40 Interrupts
}

void enx_externalirq_init_cpu1(void)
{
	enx_externalirq_disable();

	set_csr(mie, MIP_MEIP|MIP_MSIP);									// Enable External Interrupts, Software Interrupt
//	set_csr(mie, MIP_MEIP);												// Enable External Interrupts
	set_csr(mstatus, MSTATUS_MIE);										// Machine Interrupt Enable

#if EN675_SINGLE
	enx_externalirq_perl(eigiDMA4_7, ENX_ON, 0);						// Enable DMA4~7 Interrupts
#else
	enx_externalirq_perl(eigiDMA1, ENX_ON, 0);							// Enable DMA1 Interrupts
#endif
}

void enx_externalirq_init_cpu2(void)
{
	enx_externalirq_disable();

	set_csr(mie, MIP_MEIP|MIP_MSIP);									// Enable External Interrupts, Software Interrupt
//	set_csr(mie, MIP_MEIP);												// Enable External Interrupts
	set_csr(mstatus, MSTATUS_MIE);										// Machine Interrupt Enable

#if EN675_SINGLE
	enx_externalirq_perl(eigiDMA8_11, ENX_ON, 0);						// Enable DMA8~11 Interrupts
#else
	enx_externalirq_perl(eigiDMA2, ENX_ON, 0);							// Enable DMA2 Interrupts
#endif
}

void enx_externalirq_init_cpu3(void)
{
	enx_externalirq_disable();

	set_csr(mie, MIP_MEIP|MIP_MSIP);									// Enable External Interrupts, Software Interrupt
//	set_csr(mie, MIP_MEIP);												// Enable External Interrupts
	set_csr(mstatus, MSTATUS_MIE);										// Machine Interrupt Enable

	enx_externalirq_perl(eigiISP, ENX_ON, 0);							// Enable ISP Interrupts
	enx_externalirq_perl(eigiVCODEC, ENX_ON, 0);						// Enable Codec Interrupts
#if EN675_SINGLE
	enx_externalirq_perl(eigiDMA12_15, ENX_ON, 0);						// Enable DMA12~15 Interrupts
#else
	enx_externalirq_perl(eigiDMA3, ENX_ON, 0);							// Enable DMA3 Interrupts
#endif
	enx_externalirq_perl(eigiGPIO60_63_I2C7_UART7, ENX_OFF, 0);			// Enable GPIO60~63, I2C7, UART7 Interrupts
	enx_externalirq_perl(eigiATOB, ENX_ON, 0);							// Enable ATOB Interrupts
//	enx_externalirq_perl(eigiGPIO, ENX_ON, 0);							// Enable GPIO Interrupts
//	enx_externalirq_perl(eigiTIMER, ENX_ON, 0);							// Enable TIMER Interrupts
}

void enx_wake_cpu(int cpu_id)
{
	cpu_msip[cpu_id] = 1;
}

void enx_wake_message_to_cpu0(UINT flag)
{
	while (CPU0_MSG_MUTEX);		// lock
	CPU0_MSG_FLAG |= flag;		// set flag
	CPU0_MSG_MUTEX = 0;			// unlock
	enx_wake_cpu(0);			// call cpu
}

void enx_wake_message_to_cpu1(UINT flag)
{
	while (CPU1_MSG_MUTEX);		// lock
	CPU1_MSG_FLAG |= flag;		// set flag
	CPU1_MSG_MUTEX = 0;			// unlock
	enx_wake_cpu(1);			// call cpu
}

void enx_wake_message_to_cpu2(UINT flag)
{
	while (CPU2_MSG_MUTEX);		// lock
	CPU2_MSG_FLAG |= flag;		// set flag
	CPU2_MSG_MUTEX = 0;			// unlock
	enx_wake_cpu(2);			// call cpu
}

void enx_wake_message_to_cpu3(UINT flag)
{
	while (CPU3_MSG_MUTEX);		// lock
	CPU3_MSG_FLAG |= flag;		// set flag
	CPU3_MSG_MUTEX = 0;			// unlock
	enx_wake_cpu(3);			// call cpu
}

uint32_t enx_get_wake_cpu(int cpu_id)
{
	return cpu_msip[cpu_id];
}
