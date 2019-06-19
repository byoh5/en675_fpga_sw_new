#ifndef __DEV_REG_EX_H__
#define __DEV_REG_EX_H__

//******************************************************************************
// x. Register
//------------------------------------------------------------------------------
#include "dev_reg.h"

_regs_ BF_8(UINT GPIO_IN : 1 ,UINT GPIO_OUT : 1 ,UINT GPIO_OEN : 1 ,UINT GPIO_IRQ_DIR : 2 ,UINT GPIO_IRQ_EN : 1 ,UINT GPIO_IRQ_CLR : 1 ,UINT GPIO_IRQ : 1 , UINT _rev0 : 24 ) _rege_ _GPIO_PIN;
_regs_ BF_2(UINT _rev0 : 30, UINT GPIO_MUX : 2 ) _rege_ _GPIO_MUX;

_regs_ BF_3(UINT DSTMAC_EN : 1 ,UINT _rev0 : 15, UINT DSTMAC_ADR_47_32 : 16 ) _rege_ _ETH_DSTMAC1;
_regs_ BF_1(UINT DSTMAC_ADR_31_0 : 32 ) _rege_ _ETH_DSTMAC2;

_regs_ BF_9(UINT CMD_RXEDGE : 1 ,UINT CMD_TXEDGE : 1 ,UINT DAT_RXEDGE : 1 ,UINT DAT_TXEDGE : 1 ,UINT _rev0 : 24, UINT BITMODE : 1 ,UINT IOMODE : 1 ,UINT MODE : 1 ,UINT EN : 1 ) _rege_ _SDIO_REG0;
_regs_ BF_4(UINT _rev0 : 17, UINT CLK_EN : 1 ,UINT CLK_SELECT : 2 ,UINT CLK_DIV : 12 ) _rege_ _SDIO_REG1;
_regs_ BF_1(UINT CMD_ARG : 32 ) _rege_ _SDIO_REG2;
_regs_ BF_10(UINT _rev0 : 2, UINT CMD_IDX : 6 ,UINT _rev1 : 14, UINT CMD_RESP_TOUT : 1 ,UINT CMD_RESP_CRCERR : 1 ,UINT _rev2 : 4, UINT CMD_RESP_BUSY_EN : 1 ,UINT CMD_RESP_TYPE : 1 ,UINT CMD_RESP_EN : 1 ,UINT CMD_EN : 1 ) _rege_ _SDIO_REG3;
_regs_ BF_4(UINT _rev0 : 18, UINT CMD_RESP_IDX : 6 ,UINT _rev1 : 1, UINT CMD_RESP_CRC : 7 ) _rege_ _SDIO_REG4;
_regs_ BF_1(UINT CMD_RESP_DAT127_96 : 32 ) _rege_ _SDIO_REG5;
_regs_ BF_1(UINT CMD_RESP_DAT95_64 : 32 ) _rege_ _SDIO_REG6;
_regs_ BF_1(UINT CMD_RESP_DAT63_32 : 32 ) _rege_ _SDIO_REG7;
_regs_ BF_1(UINT CMD_RESP_DAT31_0 : 32 ) _rege_ _SDIO_REG8;
_regs_ BF_1(UINT CMD_RESP_TLMT : 32 ) _rege_ _SDIO_REG9;
_regs_ BF_1(UINT DAT_ADR : 32 ) _rege_ _SDIO_REG10;
_regs_ BF_3(UINT _rev0 : 4, UINT DAT_BLKBYTE : 12 ,UINT DAT_BLKNUM : 16 ) _rege_ _SDIO_REG11;
_regs_ BF_1(UINT DAT_BLKADR : 32 ) _rege_ _SDIO_REG12;
_regs_ BF_15(UINT DAT_CRCERR : 1 ,UINT DAT_BUSY : 1 ,UINT _rev0 : 18, UINT IO_IRQ : 1 ,UINT IO_IRQ_CLR : 1 ,UINT IO_IRQ_EN : 1 ,UINT CMD_IRQ : 1 ,UINT CMD_IRQ_CLR : 1 ,UINT CMD_IRQ_EN : 1 ,UINT DAT_IRQ : 1 ,UINT DAT_IRQ_CLR : 1 ,UINT DAT_IRQ_EN : 1 ,UINT CMD53_OP : 1 ,UINT DAT_WE : 1 ,UINT DAT_EN : 1 ) _rege_ _SDIO_REG13;
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

_regs_ BF_12(UINT _rev0 : 1, UINT DONE_VAL : 1 ,UINT _rev1 : 2, UINT DONE_PTR : 6 ,UINT _rev2 : 2, UINT JOB_PTR : 6 ,UINT IRQ : 1 ,UINT IRQ_EN : 1 ,UINT IRQ_CLR : 1 ,UINT VALUE : 8 ,UINT MODE : 2 ,UINT GO : 1 ) _rege_ _DMA_REG0;
_regs_ BF_1(UINT SRC : 32 ) _rege_ _DMA_REG1;
_regs_ BF_1(UINT DST : 32 ) _rege_ _DMA_REG2;
_regs_ BF_1(UINT LEN : 32 ) _rege_ _DMA_REG3;

_regs_ BF_10(UINT EN : 1 ,UINT PWM_EN : 1 ,UINT CK_EN : 1 ,UINT IRQ_EN : 1 ,UINT CNT_CLR : 1 ,UINT IRQ_CLR : 1 ,UINT IRQ : 1 ,UINT _rev0 : 1, UINT DIV : 8 ,UINT LMT : 16 ) _rege_ _TIMER_REG0;
_regs_ BF_2(UINT TRIG : 16 ,UINT CNT : 16 ) _rege_ _TIMER_REG1;

_regs_ BF_1(UINT RX_DAT : 32 ) _rege_ _SPI_REG0;
_regs_ BF_1(UINT TX_DAT : 32 ) _rege_ _SPI_REG1;
_regs_ BF_15(UINT EN : 1 ,UINT _rev0 : 7, UINT CLK_DIV : 8 ,UINT _rev1 : 2, UINT BIT_MODE : 1 ,UINT CS_OEN : 1 ,UINT CS_OUT : 1 ,UINT IRQ_EN : 1 ,UINT IRQ_CLR : 1 ,UINT IRQ : 1 ,UINT ONE_BITMODE : 1 ,UINT CLK_MODE : 2 ,UINT WS : 2 ,UINT RW : 2 ,UINT GO : 1 ) _rege_ _SPI_REG2;

_regs_ BF_5(UINT EN : 1 ,UINT MAG : 1 ,UINT MUL : 3 ,UINT _rev0 : 11, UINT DAT : 16 ) _rege_ _ADC_REG;



#endif
