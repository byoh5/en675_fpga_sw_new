#ifndef __DEV_REGS_H__
#define __DEV_REGS_H__

//******************************************************************************
// 1. TYPE DEF
//------------------------------------------------------------------------------
#include "dev_types.h"

//******************************************************************************
// 2. Memory Size & Base Address Definition
//------------------------------------------------------------------------------
// Mem size
#define SFLS_SIZE				(16*1024*1024)	//	 16MB
#define DDR_SIZE				(64*1024*1024)	//	 64MB	-	AP Memory AD251232 , Winbond W979H2KK , Etron EM6KA32HGDA
//#define DDR_SIZE				(128*1024*1024)	//	128MB	-	AP Memory 1Gb , Fidelix 1Gb, Winbond W97AH2KK
//#define DDR_SIZE				(256*1024*1024)	//	256MB	-	AP Memory AD220032
#define SRAM_SIZE				(    256*1024)	//	256kB

// Base address
#define SFLS_BASE				0xC0000000	//	OK
#define DDR_BASE				0x80000000	//	OK
#define SRAM_BASE				0xA0000000	//	OK

// RegBase address
#define REG_BASE_ISP			0x40000000
#define REG_BASE_H265			0x40100000
#define REG_BASE_H264       	0x40200000
#define REG_BASE_DDR        	0x40300000
#define REG_BASE_SFLS       	0x40400000
#define REG_BASE_SDIO0      	0x40500000
#define REG_BASE_SDIO1      	0x40600000
#define REG_BASE_ETH        	0x40700000
#define REG_BASE_AUD        	0x40800000
#define REG_BASE_UTIL       	0x40900000
#define REG_BASE_GPIO       	0x40A00000
#define REG_BASE_SPI0       	0x40B00000
#define REG_BASE_SPI1       	0x40C00000
#define REG_BASE_SPI2       	0x40D00000
#define REG_BASE_SPI3       	0x40E00000
#define REG_BASE_SPI4       	0x40F00000
#define REG_BASE_SPI5       	0x41000000
#define REG_BASE_SPI6       	0x41100000
#define REG_BASE_SPI7       	0x41200000
#define REG_BASE_SPI8       	0x41300000
#define REG_BASE_I2C0       	0x41400000
#define REG_BASE_I2C1       	0x41500000
#define REG_BASE_I2C2       	0x41600000
#define REG_BASE_I2C3       	0x41700000
#define REG_BASE_I2C4       	0x41800000
#define REG_BASE_I2C5       	0x41900000
#define REG_BASE_I2C6       	0x41A00000
#define REG_BASE_I2C7       	0x41B00000
#define REG_BASE_I2C8       	0x41C00000
#define REG_BASE_TIMER0     	0x41D00000
#define REG_BASE_TIMER1     	0x41E00000
#define REG_BASE_TIMER2     	0x41F00000
#define REG_BASE_TIMER3     	0x42000000
#define REG_BASE_TIMER4     	0x42100000
#define REG_BASE_TIMER5     	0x42200000
#define REG_BASE_TIMER6     	0x42300000
#define REG_BASE_TIMER7     	0x42400000
#define REG_BASE_TIMER8     	0x42500000
#define REG_BASE_TIMER9     	0x42600000
#define REG_BASE_TIMER10    	0x42700000
#define REG_BASE_TIMER11    	0x42800000
#define REG_BASE_TIMER12    	0x42900000
#define REG_BASE_TIMER13    	0x42A00000
#define REG_BASE_TIMER14    	0x42B00000
#define REG_BASE_TIMER15    	0x42C00000
#define REG_BASE_TIMER16    	0x42D00000
#define REG_BASE_TIMER17    	0x42E00000
#define REG_BASE_TIMER18    	0x42F00000
#define REG_BASE_TIMER19    	0x43000000
#define REG_BASE_TIMER20    	0x43100000
#define REG_BASE_TIMER21    	0x43200000
#define REG_BASE_TIMER22    	0x43300000
#define REG_BASE_TIMER23    	0x43400000
#define REG_BASE_TIMER24    	0x43500000
#define REG_BASE_TIMER25    	0x43600000
#define REG_BASE_TIMER26    	0x43700000
#define REG_BASE_TIMER27    	0x43800000
#define REG_BASE_TIMER28    	0x43900000
#define REG_BASE_TIMER29    	0x43A00000
#define REG_BASE_TIMER30    	0x43B00000
#define REG_BASE_TIMER31    	0x43C00000
#define REG_BASE_TIMER32    	0x43D00000
#define REG_BASE_TIMER33    	0x43E00000
#define REG_BASE_TIMER34    	0x43F00000
#define REG_BASE_TIMER35    	0x44000000
#define REG_BASE_TIMER36    	0x44100000
#define REG_BASE_TIMER37    	0x44200000
#define REG_BASE_TIMER38    	0x44300000
#define REG_BASE_UART0      	0x44400000
#define REG_BASE_UART1      	0x44500000
#define REG_BASE_UART2      	0x44600000
#define REG_BASE_UART3      	0x44700000
#define REG_BASE_UART4      	0x44800000
#define REG_BASE_UART5      	0x44900000
#define REG_BASE_UART6      	0x44A00000
#define REG_BASE_UART7      	0x44B00000
#define REG_BASE_UART8      	0x44C00000
#define REG_BASE_AES			0x44D00000
#define REG_BASE_SHA			0x44E00000
#define REG_BASE_CHKSUM			0x44F00000
#define REG_BASE_SYS			0x45000000
#define REG_BASE_DMA0			0x45100000
#define REG_BASE_DMA1			0x45100020
#define REG_BASE_I2S			0x45200000
#define REG_BASE_IRQ			0x45300000
#define REG_BASE_ADC			0x45400000

//******************************************************************************
// 3. System define
//------------------------------------------------------------------------------
#define OSC_FREQ				(25*1000*1000)
#define MCK_FREQ				(50*1000*1000)

#define DMA_CNT					2
#define GPIO_CNT				72
#define SDIO_CNT				2
#define UART_CNT				9
#define SPI_CNT					9
#define I2C_CNT					9
#define TIMER_CNT				39
#define USB_CNT					1
#define ETHERNET_CNT			1
#define I2S_CNT					1
#define ADC_CNT					8

//******************************************************************************
// x. Register
//------------------------------------------------------------------------------

#include "dev_reg.h"

_regs_ BF_8(UINT GPIO_IN : 1 ,UINT GPIO_OUT : 1 ,UINT GPIO_OEN : 1 ,UINT GPIO_IRQ_DIR : 2 ,UINT GPIO_IRQ_EN : 1 ,UINT GPIO_IRQ_CLR : 1 ,UINT GPIO_IRQ : 1 , UINT _rev0 : 24 ) _rege_ _GPIO_PIN;

_regs_ BF_5(UINT _rev0 : 28, UINT BITMODE : 1 ,UINT IOMODE : 1 ,UINT MODE : 1 ,UINT EN : 1 ) _rege_ _SDIO_REG0;
_regs_ BF_4(UINT _rev0 : 17, UINT CLK_EN : 1 ,UINT CLK_SELECT : 2 ,UINT CLK_DIV : 12 ) _rege_ _SDIO_REG1;
_regs_ BF_1(UINT CMD_ARG : 32 ) _rege_ _SDIO_REG2;
_regs_ BF_10(UINT _rev0 : 1, UINT CMD_IDX : 7 ,UINT _rev1 : 14, UINT CMD_RESP_TOUT : 1 ,UINT CMD_RESP_CRCERR : 1 ,UINT _rev2 : 4, UINT CMD_RESP_BUSY_EN : 1 ,UINT CMD_RESP_TYPE : 1 ,UINT CMD_RESP_EN : 1 ,UINT CMD_EN : 1 ) _rege_ _SDIO_REG3;
_regs_ BF_4(UINT _rev0 : 18, UINT CMD_RESP_IDX : 6 ,UINT _rev1 : 1, UINT CMD_RESP_CRC : 7 ) _rege_ _SDIO_REG4;
_regs_ BF_1(UINT CMD_RESP_DAT127_96 : 32 ) _rege_ _SDIO_REG5;
_regs_ BF_1(UINT CMD_RESP_DAT95_64 : 32 ) _rege_ _SDIO_REG6;
_regs_ BF_1(UINT CMD_RESP_DAT63_32 : 32 ) _rege_ _SDIO_REG7;
_regs_ BF_1(UINT CMD_RESP_DAT31_0 : 32 ) _rege_ _SDIO_REG8;
_regs_ BF_1(UINT CMD_RESP_TLMT : 32 ) _rege_ _SDIO_REG9;
_regs_ BF_1(UINT DAT_ADR : 32 ) _rege_ _SDIO_REG10;
_regs_ BF_3(UINT _rev0 : 4, UINT DAT_BLKBYTE : 12 ,UINT DAT_BLKNUM : 16 ) _rege_ _SDIO_REG11;
_regs_ BF_1(UINT DAT_BLKADR : 32 ) _rege_ _SDIO_REG12;
_regs_ BF_13(UINT DAT_CRCERR : 1 ,UINT _rev0 : 20, UINT IO_IRQ : 1 ,UINT IO_IRQ_CLR : 1 ,UINT IO_IRQ_EN : 1 ,UINT CMD_IRQ : 1 ,UINT CMD_IRQ_CLR : 1 ,UINT CMD_IRQ_EN : 1 ,UINT DAT_IRQ : 1 ,UINT DAT_IRQ_CLR : 1 ,UINT DAT_IRQ_EN : 1 ,UINT DAT_WE : 1 ,UINT DAT_EN : 1 ) _rege_ _SDIO_REG13;
_regs_ BF_2(UINT _rev0 : 16, UINT DAT_BLKCNT : 16 ) _rege_ _SDIO_REG14;
_regs_ BF_4(UINT _rev0 : 18, UINT DAT_STOP_CMD : 6 ,UINT _rev1 : 2, UINT DAT_IORW_CMD : 6 ) _rege_ _SDIO_REG15;
_regs_ BF_8(UINT _rev0 : 2, UINT DAT_WRCMD_S : 6 ,UINT _rev1 : 2, UINT DAT_WRCMD_M : 6 ,UINT _rev2 : 2, UINT DAT_RDCMD_S : 6 ,UINT _rev3 : 2, UINT DAT_RDCMD_M : 6 ) _rege_ _SDIO_REG16;

_regs_ BF_16(UINT CLK_DIV : 12 ,UINT _rev0 : 6, UINT TX_TYPE : 1 ,UINT STOP_BIT : 1 ,UINT PARITY_EN : 1 ,UINT PARITY_TYPE : 1 ,UINT TX_IRQ : 1 ,UINT TX_IRQ_EN : 1 ,UINT TX_IRQ_CLR : 1 ,UINT TX_EMPTY : 1 ,UINT TX_FULL : 1 ,UINT RX_IRQ : 1 ,UINT RX_IRQ_EN : 1 ,UINT RX_IRQ_CLR : 1 ,UINT RX_EMPTY : 1 ,UINT RX_FULL : 1 ) _rege_ _UART_REG0;
_regs_ BF_2(UINT _rev0 : 24, UINT RX_DAT : 8 ) _rege_ _UART_REG1;
_regs_ BF_2(UINT _rev0 : 24, UINT TX_DAT : 8 ) _rege_ _UART_REG2;
_regs_ BF_2(UINT _rev0 : 12, UINT RX_LMT : 20 ) _rege_ _UART_REG3;

_regs_ BF_9(UINT RX_DAT : 8 ,UINT TX_DAT : 8 ,UINT _rev0 : 10, UINT ACT : 1 ,UINT MODE : 1 ,UINT BIT_MODE : 1 ,UINT IRQ : 1 ,UINT IRQ_EN : 1 ,UINT IRQ_CLR : 1 ) _rege_ _I2C_REG0;
_regs_ BF_2(UINT CLK_DIV : 16 , UINT _rev0 : 16 ) _rege_ _I2C_REG1;
_regs_ BF_7(UINT _rev0 : 26, UINT MST_COL : 1 ,UINT MST_ACK : 1 ,UINT MST_REPEAT : 1 ,UINT MST_LAST : 1 ,UINT MST_RW : 1 ,UINT MST_GO : 1 ) _rege_ _I2C_REG2;
_regs_ BF_8(UINT _rev0 : 19, UINT I2C_SDA : 1 ,UINT I2C_SCL : 1 ,UINT SLV_ACK_IN : 1 ,UINT SLV_GO : 1 ,UINT SLV_RW : 1 ,UINT SLV_ACK_OUT : 1 ,UINT SLV_ADR : 7 ) _rege_ _I2C_REG3;

_regs_ BF_9(UINT _rev0 : 4, UINT JOB_PTR : 8 ,UINT DONE_PTR : 6 ,UINT IRQ : 1 ,UINT IRQ_EN : 1 ,UINT IRQ_CLR : 1 ,UINT VALUE : 8 ,UINT MODE : 2 ,UINT GO : 1 ) _rege_ _DMA_REG0;
_regs_ BF_1(UINT SRC : 32 ) _rege_ _DMA_REG1;
_regs_ BF_1(UINT DST : 32 ) _rege_ _DMA_REG2;
_regs_ BF_1(UINT LEN : 32 ) _rege_ _DMA_REG3;

_regs_ BF_10(UINT EN : 1 ,UINT PWM_EN : 1 ,UINT CK_EN : 1 ,UINT IRQ_EN : 1 ,UINT CNT_CLR : 1 ,UINT IRQ_CLR : 1 ,UINT IRQ : 1 ,UINT _rev0 : 1, UINT DIV : 8 ,UINT LMT : 16 ) _rege_ _TIMER_REG0;
_regs_ BF_2(UINT TRIG : 16 ,UINT CNT : 16 ) _rege_ _TIMER_REG1;

_regs_ BF_1(UINT RX_DAT : 32 ) _rege_ _SPI_REG0;
_regs_ BF_1(UINT TX_DAT : 32 ) _rege_ _SPI_REG1;
_regs_ BF_15(UINT EN : 1 ,UINT _rev0 : 7, UINT CLK_DIV : 8 ,UINT _rev1 : 2, UINT BIT_MODE : 1 ,UINT CS_OEN : 1 ,UINT CS_OUT : 1 ,UINT IRQ_EN : 1 ,UINT IRQ_CLR : 1 ,UINT IRQ : 1 ,UINT ONE_BITMODE : 1 ,UINT CLK_MODE : 2 ,UINT WS : 2 ,UINT RW : 2 ,UINT GO : 1 ) _rege_ _SPI_REG2;

_regs_ BF_4(UINT CLK_LMT : 16 ,UINT _rev0 : 14, UINT CKEN : 1 ,UINT EN : 1 ) _rege_ _ADC_REG0;
_regs_ BF_5(UINT CHEN : 1 ,UINT MAG : 1 ,UINT MUL : 3 ,UINT _rev0 : 11, UINT DAT : 16 ) _rege_ _ADC_REG1;

//******************************************************************************
// x. Function
//------------------------------------------------------------------------------

extern void DdrTest(void); // ddr.c
extern void DdrInit(void); // ddr.c

extern void GpioInit(void);
extern void GpioRiseEdge(UINT nCH);
extern void GpioFallEdge(UINT nCH);
extern void GpioBothEdge(UINT nCH);
extern void GpioInDir(UINT nCH);
extern void GpioOutDir(UINT nCH);
extern UINT GpioGetDir(UINT nCH);
extern void GpioSetHi(UINT nCH);
extern void GpioSetLo(UINT nCH);
extern void GpioFuncPin(UINT nCH);
extern void GpioFuncPinOff(UINT nCH);
extern UINT GpioGetFuncPin(UINT nCH);
extern UINT GpioGetPin(UINT nCH);
extern void GpioIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void GpioIrqOn(UINT nCH);
extern void GpioIrqOff(UINT nCH);
extern void GpioIrqClear(UINT nCH);
extern UINT GpioIsIrq(UINT nCH);
extern void IrqGpio(UINT nCH);

extern void UartInit(UINT nCH, UINT Speed_Hz);
extern void UartDeinit(UINT nCH);
extern void UartTx(UINT nCH, char data);
extern UINT UartTxIsEmpty(UINT nCH);
extern UINT UartTxIsFull(UINT nCH);
extern void UartTxIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void UartTxIrqOn(UINT nCH);
extern void UartTxIrqOff(UINT nCH);
extern void UartTxIrqClear(UINT nCH);
extern UINT UartTxIsIrq(UINT nCH);
extern UINT UartRx(UINT nCH);
extern UINT UartRxGetByte(UINT nCH);
extern UINT UartRxIsEmpty(UINT nCH);
extern UINT UartRxIsFull(UINT nCH);
extern void UartRxIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void UartRxIrqOn(UINT nCH);
extern void UartRxIrqOff(UINT nCH);
extern void UartRxIrqClear(UINT nCH);
extern UINT UartRxIsIrq(UINT nCH);
extern void IrqUart(UINT nCH);

extern void I2cInit(UINT nCH, UINT Speed_Hz);
extern void I2cDeInit(UINT nCH);
extern UINT I2cWrite(UINT nCH, BYTE dat, BYTE last, BYTE repeat);
extern UINT I2cRead(UINT nCH, BYTE last, BYTE repeat);
extern UINT I2cCheck(UINT nCH, BYTE addr);
extern void I2cChCheck(UINT nCH);
extern void I2cIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void I2cIrqOn(UINT nCH);
extern void I2cIrqOff(UINT nCH);
extern void I2cIrqClear(UINT nCH);
extern UINT I2cIsIrq(UINT nCH);
extern void IrqI2c(UINT nCH);

extern void SpiInit(UINT nCH, UINT Speed_Hz, UINT WordSize, UINT BitDirection);
extern void SpiDeinit(UINT nCH);
extern void SpiCsLo(UINT nCH);
extern void SpiCsHi(UINT nCH);
extern void SpiWrite(UINT nCH, BYTE *dat);
extern void SpiRead(UINT nCH, BYTE *dat);
extern void SpiRW(UINT nCH, BYTE *WrDat, BYTE *RdDat);
extern void SpiIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void SpiIrqOn(UINT nCH);
extern void SpiIrqOff(UINT nCH);
extern void SpiIrqClear(UINT nCH);
extern UINT SpiIsIrq(UINT nCH);
extern void IrqSpi(UINT nCH);

extern void DmaInit(void);
extern void DmaMemCpy_isr(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void DmaMemCpy_isr_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void DmaMemSet_isr(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum);
extern void DmaMemSet_isr_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum);
extern void DmaIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void DmaIrqOn(UINT nCH);
extern void DmaIrqOff(UINT nCH);
extern void DmaIrqClear(UINT nCH);
extern UINT DmaIsIrq(UINT nCH);
extern void IrqDma(void);

extern void TimerInit(UINT nCH);
extern void TimerDeInit(UINT nCH);
extern void TimerSetFreq(UINT nCH, UINT nDiv, UINT nLmt, UINT nTrig);
extern void TimerStart(UINT nCH);
extern void TimerStop(UINT nCH);
extern void TimerPWMEnable(UINT nCH);
extern void TimerPWMDisable(UINT nCH);
extern void TimerIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void TimerIrqOn(UINT nCH);
extern void TimerIrqOff(UINT nCH);
extern void TimerIrqClear(UINT nCH);
extern UINT TimerIsIrq(UINT nCH);
extern void IrqTimer(UINT nCH);

extern void MdioInit(UINT Speed_Hz);
extern void MdioRead(BYTE PhyAdr, BYTE RegAdr, WORD *RdDat);
extern void MdioWrite(BYTE PhyAdr, BYTE RegAdr, WORD WrDat);

extern void EthInit(void);

extern void SdioInit(UINT nCH, UINT Speed_Hz);
extern UINT SdioCmd(UINT nCH, BYTE Cmd, UINT Arg, BYTE RespEn, BYTE RespType, BYTE RespBusyEn);
extern void SdioSetDelayfn(UINT nCH, user_delay_fn user_delay);
extern void SdioGetResp(UINT nCH, UINT *nResp, eCmdRespType cmdType);
extern void SdioSetClockDef(UINT nCH);
extern void SdioSetClock(UINT nCH, UINT Speed_Hz);
extern void SdioSetClockDiv(UINT nCH, UINT nClkDiv);
extern UINT SdioGetClockDiv(UINT nCH);
extern void SdioClockEnable(UINT nCH);
extern void SdioClockDisable(UINT nCH);
extern void SdioClockDivPrint(UINT nCH, char *strBuffer);
extern void SdioIrqCallback_Io(UINT nCH, irq_fn irqfn, void *arg);
extern void SdioIrqCallback_Cmd(UINT nCH, irq_fn irqfn, void *arg);
extern void SdioIrqCallback_Dat(UINT nCH, irq_fn irqfn, void *arg);
extern void SdioIrqOn_Io(UINT nCH);
extern void SdioIrqOn_Cmd(UINT nCH);
extern void SdioIrqOn_Dat(UINT nCH);
extern void SdioIrqOff_Io(UINT nCH);
extern void SdioIrqOff_Cmd(UINT nCH);
extern void SdioIrqOff_Dat(UINT nCH);
extern void SdioIrqClear_Io(UINT nCH);
extern void SdioIrqClear_Cmd(UINT nCH);
extern void SdioIrqClear_Dat(UINT nCH);
extern UINT SdioIsIrq_Io(UINT nCH);
extern UINT SdioIsIrq_Cmd(UINT nCH);
extern UINT SdioIsIrq_Dat(UINT nCH);
extern void IrqSdio(UINT nCH);

extern void SdioCdInit(UINT nCH);
extern UINT SdioCdDet(void);
extern UINT SdioCdInitProcess(void);
extern void SdioCdClockDown(void);

extern void AdcInit(UINT Speed_Hz);
extern void AdcOn(UINT nCH);
extern void AdcOff(UINT nCH);
extern WORD AdcGet(UINT nCH);

extern WORD Checksum16(BYTE *apbDst, UINT anNum);

extern void AesInit(void);
extern void Aes128Enc(BYTE *apbDst, BYTE *apbSrc, UINT len);
extern void Aes128Dec(BYTE *apbDst, BYTE *apbSrc, UINT len);
extern void Aes256Enc(BYTE *apbDst, BYTE *apbSrc, UINT len);
extern void Aes256Dec(BYTE *apbDst, BYTE *apbSrc, UINT len);

extern void Sha224(BYTE *apbSrc, UINT len);
extern void Sha256(BYTE *apbSrc, UINT len);


#endif
