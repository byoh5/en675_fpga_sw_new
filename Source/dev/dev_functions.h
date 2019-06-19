#ifndef __DEV_FUNCTIONS_H__
#define __DEV_FUNCTIONS_H__

//******************************************************************************
// x. Function
//------------------------------------------------------------------------------

// irq.c ///////////////////////////////////////////////////////////////////////
extern void enx_timerirq_next(void);
extern void enx_timerirq_init(void);
extern void enx_externalirq_init(void);

// Device driver ///////////////////////////////////////////////////////////////

extern void DdrTest(void); // ddr.c
extern void DdrInit(BYTE bCH, BYTE Sel); // ddr.c

extern void SflsInit(void);
extern UINT SflsGetinfo(void);
extern void SflsWriteEnable(void);
extern void SflsSectErase(UINT addr, ENX_YN sync);
extern void Sfls32KErase(UINT addr, ENX_YN sync);
extern void Sfls64KErase(UINT addr, ENX_YN sync);
extern void SflsChipErase(ENX_YN sync);
extern UINT SflsGetUsrreq(void);
extern void SflsWriteReg(BYTE wrdata, BYTE len, BYTE iomode);
extern UINT SflsReadReg(BYTE cmd, BYTE iomode, BYTE gap);
extern SFLScontrol *SflsGetControl(void);
extern void SflsRegShow(ENX_YN isDetail);

extern void GpioInit(void);
extern void GpioSetEdge(UINT nCH, GPIO_EDGE edge);
extern UINT GpioGetEdge(UINT nCH);
extern void GpioSetDir(UINT nCH, GPIO_DIR dir);
extern GPIO_DIR GpioGetDir(UINT nCH);
extern void GpioSetOut(UINT nCH, GPIO_OUT out);
extern GPIO_OUT GpioGetOut(UINT nCH);
extern void GpioFuncPin(UINT nCH, UINT nSel);
extern void GpioFuncPinOff(UINT nCH);
extern UINT GpioGetFuncPin(UINT nCH);
extern UINT GpioGetPin(UINT nCH);
extern void GpioIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void GpioSetIrqEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH GpioGetIrqEn(UINT nCH);
extern void GpioIrqClear(UINT nCH);
extern UINT GpioIsIrq(UINT nCH);
extern void IrqGpio(UINT nCH);

extern void UartInit(UINT nCH, UINT Speed_Hz);
extern void UartDeinit(UINT nCH);
extern void UartTx(UINT nCH, char data);
extern void UartTxSetByte(UINT nCH, char data);
extern UINT UartTxIsEmpty(UINT nCH);
extern UINT UartTxIsFull(UINT nCH);
extern void UartTxIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void UartTxSetIrqEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH UartTxGetIrqEn(UINT nCH);
extern void UartTxIrqClear(UINT nCH);
extern UINT UartTxIsIrq(UINT nCH);
extern UINT UartRx(UINT nCH);
extern UINT UartRxGetByte(UINT nCH);
extern UINT UartRxIsEmpty(UINT nCH);
extern UINT UartRxIsFull(UINT nCH);
extern void UartRxIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void UartRxSetIrqEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH UartRxGetIrqEn(UINT nCH);
extern void UartRxIrqClear(UINT nCH);
extern UINT UartRxIsIrq(UINT nCH);
extern void IrqUart(UINT nCH);

extern void I2cInit(UINT nCH, UINT Speed_Hz);
extern void I2cDeInit(UINT nCH);
extern UINT I2cWrite(UINT nCH, BYTE dat, BYTE last, BYTE repeat);
extern UINT I2cRead(UINT nCH, BYTE last, BYTE repeat);
extern ENX_OKFAIL I2cCheck(UINT nCH, BYTE addr);
extern void I2cChCheck(UINT nCH);
extern void I2cIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void I2cSetIrqEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH I2cGetIrqEn(UINT nCH);
extern void I2cIrqClear(UINT nCH);
extern UINT I2cIsIrq(UINT nCH);
extern void IrqI2c(UINT nCH);

extern void SpiInit(UINT nCH, UINT Speed_Hz, BYTE nBIT, BYTE isLSB);
extern void SpiDeinit(UINT nCH);
extern void SpiSetCs(UINT nCH, ENX_HL hl);
extern ENX_HL SpiGetCs(UINT nCH);
extern void SpiWrite(UINT nCH, BYTE *dat);
extern void SpiRead(UINT nCH, BYTE *dat);
extern void SpiRW(UINT nCH, BYTE *WrDat, BYTE *RdDat);
extern void SpiIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void SpiSetIrqEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH SpiGetIrqEn(UINT nCH);
extern void SpiIrqClear(UINT nCH);
extern UINT SpiIsIrq(UINT nCH);
extern void IrqSpi(UINT nCH);

extern void BDmaInit(void);
extern void BDmaMemCpy_isr(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern UINT BDmaMemCpy_isr_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void BDmaMemCpy_rtos(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern UINT BDmaMemCpy_rtos_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void BDmaMemSet_isr(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum);
extern UINT BDmaMemSet_isr_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum);
extern void BDmaMemSet_rtos(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum);
extern void BDmaIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void BDmaSetIrqEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH BDmaGetIrqEn(UINT nCH);
extern void BDmaIrqClear(UINT nCH);
extern UINT BDmaIsIrq(UINT nCH);
extern void IrqBDma(UINT nCH);

extern void CDmaInit(void);
extern void CDmaMemCpy_isr(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern UINT CDmaMemCpy_isr_async(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void CDmaMemCpy_rtos(UINT nCH, BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void CDmaMemSet_isr(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum);
extern UINT CDmaMemSet_isr_async(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum);
extern void CDmaMemSet_rtos(UINT nCH, BYTE *apbDst, BYTE abVal, UINT anNum);
extern void CDmaIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void CDmaSetIrqEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH CDmaGetIrqEn(UINT nCH);
extern void CDmaIrqClear(UINT nCH);
extern UINT CDmaIsIrq(UINT nCH);
extern void IrqCDma(UINT nCH);

extern void TimerInit(UINT nCH);
extern void TimerDeInit(UINT nCH);
extern void TimerSetFreq(UINT nCH, UINT nDiv, UINT nLmt, UINT nTrig);
extern void TimerStart(UINT nCH);
extern void TimerStop(UINT nCH);
extern void TimerSetEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH TimerGetEn(UINT nCH);
extern void TimerSetCken(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH TimerGetCken(UINT nCH);
extern void TimerSetPWMEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH TimerGetPWMEn(UINT nCH);
extern void TimerSetDiv(UINT nCH, UINT nDiv);
extern UINT TimerGetDiv(UINT nCH);
extern void TimerSetLmt(UINT nCH, UINT nLmt);
extern UINT TimerGetLmt(UINT nCH);
extern void TimerSetTrig(UINT nCH, UINT nTrig);
extern UINT TimerGetTrig(UINT nCH);
extern UINT TimerGetCount(UINT nCH);
extern void TimerIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void TimerSetIrqEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH TimerGetIrqEn(UINT nCH);
extern void TimerIrqClear(UINT nCH);
extern UINT TimerIsIrq(UINT nCH);
extern void IrqTimer(UINT nCH);

extern void MdioInit(UINT Speed_Hz);
extern void MdioRead(BYTE PhyAdr, BYTE RegAdr, WORD *RdDat);
extern void MdioWrite(BYTE PhyAdr, BYTE RegAdr, WORD WrDat);
extern void MdioSetClklmt(UINT Clklmt);
extern UINT MdioGetClklmt(void);
extern void MdioSetClk(UINT Speed_Hz);
extern UINT MdioGetClk(void);

extern void EthInit(void);
extern void EthRxFilterMacAdr(BYTE *addr);
extern UINT EthRxFilterInsert(BYTE *addr);
extern UINT EthRxFilterDelete(BYTE *addr);
extern void EthRxFilterList(void);
extern void EthRxSetBuffer(BYTE *addr, UINT rx_lmt);
extern void EthTxPacket(BYTE *addr, UINT Len);
extern void EthRxTxClockDly(BYTE u8TXe, BYTE u8TXd, BYTE u8RXe, BYTE u8RXd);
extern void EthRxTxInit(UINT type, UINT speed, UINT duplex);
extern void EthSetRxFilterEn(ENX_SWITCH sw);
extern ENX_SWITCH EthGetRxFilterEn(void);
extern void EthSetRxEn(ENX_SWITCH sw);
extern ENX_SWITCH EthGetRxEn(void);
extern void EthTxIrqCallback(irq_fn irqfn, void *arg);
extern void EthRxIrqCallback(irq_fn irqfn, void *arg);
extern void EthSetTxIrqEn(ENX_SWITCH sw);
extern void EthSetRxIrqEn(ENX_SWITCH sw);
extern ENX_SWITCH EthGetTxIrqEn(void);
extern ENX_SWITCH EthGetRxIrqEn(void);
extern void EthTxIrqClear(void);
extern void EthRxIrqClear(void);
extern UINT EthIsTxIrq(void);
extern UINT EthIsRxIrq(void);
extern void IrqEthTx(void);
extern void IrqEthRx(void);
extern void EthMacAddrCheck(BYTE *addr);

extern void I2sInit(void);
extern void I2sMstInit(UINT freq, UINT byte);
extern void I2sSlvInit(void);
extern void I2sTxCfg(UINT tx_mode, UINT tx_cd, UINT tx_dw, UINT rd_byte, UINT rd_dw, UINT rd_len, UINT tx_lr);
extern void I2sSetTxMode(UINT tx_mode);
extern UINT I2sGetTxMode(void);
extern void I2sSetTxCodec(UINT tx_cd);
extern UINT I2sGetTxCodec(void);
extern void I2sSetTxDw(UINT tx_dw);
extern UINT I2sGetTxDw(void);
extern void I2sSetRdByte(UINT rd_byte);
extern UINT I2sGetRdByte(void);
extern void I2sSetRdDw(UINT rd_dw);
extern UINT I2sGetRdDw(void);
extern void I2sSetRdLen(UINT rd_len);
extern UINT I2sGetRdLen(void);
extern void I2sSetTxLr(UINT tx_lr);
extern UINT I2sGetTxLr(void);
extern void I2sTxBaseAddr(BYTE *addr);
extern UINT I2sTxPos(void);
extern void I2sSetTxEn(ENX_SWITCH sw);
extern ENX_SWITCH I2sGetTxEn(void);
extern void I2sTxIrqCallback(irq_fn irqfn, void *arg);
extern void I2sRxIrqCallback(irq_fn irqfn, void *arg);
extern void I2sSetTxIrqEn(ENX_SWITCH sw);
extern void I2sSetRxIrqEn(ENX_SWITCH sw);
extern ENX_SWITCH I2sGetTxIrqEn(void);
extern ENX_SWITCH I2sGetRxIrqEn(void);
extern void I2sTxIrqClear(void);
extern void I2sRxIrqClear(void);
extern UINT I2sTxIsIrq(void);
extern UINT I2sRxIsIrq(void);
extern void IrqI2sTx(void);
extern void IrqI2sRx(void);

extern void SdioInit(UINT nCH, UINT Speed_Hz);
extern UINT SdioCmd(UINT nCH, BYTE Cmd, UINT Arg, UINT *nResp, eCmdRespType cmdType);
extern void SdioSetDelayfn(UINT nCH, user_delay_fn user_delay);
extern void SdioGetResp(UINT nCH, UINT *nResp, eCmdRespType cmdType);
extern void SdioSetClockDef(UINT nCH);
extern void SdioSetClock(UINT nCH, UINT Speed_Hz);
extern void SdioSetClockDiv(UINT nCH, UINT nClkDiv);
extern UINT SdioGetClockDiv(UINT nCH);
extern void SdioSetClockEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH SdioGetClockEn(UINT nCH);
extern void SdioClockDivPrint(UINT nCH, char *strBuffer);
extern void SdioSetBitMode(UINT nCH, eSDIO_BIT_MODE bitmode);
extern eSDIO_BIT_MODE SdioGetBitMode(UINT nCH);
extern void SdioSetDataBlockByte(UINT nCH, UINT BlkByte);
extern UINT SdioGetDataBlockByte(UINT nCH);
extern UINT SdioIsDataEn(UINT nCH);
extern ENX_OKFAIL SdioDataIO(UINT nCH, eSDIO_DIO_TYPE iotype, ULONG MemDst, UINT BlkAdr, UINT BlkCnt);
extern void SdioSetDataCmd(UINT nCH, eSDIO_DIO_TYPE iotype, UINT nCmd);
extern UINT SdioGetDataCmd(UINT nCH, eSDIO_DIO_TYPE iotype);
extern void SdioIoIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void SdioCmdIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void SdioDatIrqCallback(UINT nCH, irq_fn irqfn, void *arg);
extern void SdioSetIoIrqEn(UINT nCH, ENX_SWITCH sw);
extern void SdioSetCmdIrqEn(UINT nCH, ENX_SWITCH sw);
extern void SdioSetDatIrqEn(UINT nCH, ENX_SWITCH sw);
extern ENX_SWITCH SdioGetIoIrqEn(UINT nCH);
extern ENX_SWITCH SdioGetCmdIrqEn(UINT nCH);
extern ENX_SWITCH SdioGetDatIrqEn(UINT nCH);
extern void SdioIoIrqClear(UINT nCH);
extern void SdioCmdIrqClear(UINT nCH);
extern void SdioDatIrqClear(UINT nCH);
extern UINT SdioIoIsIrq(UINT nCH);
extern UINT SdioCmdIsIrq(UINT nCH);
extern UINT SdioDatIsIrq(UINT nCH);
extern void IrqSdio(UINT nCH);
extern void SdioRegShow(UINT nCH, UINT isDetail);

extern void AdcInit(UINT Speed_Hz);
extern void AdcOn(UINT nCH);
extern void AdcOff(UINT nCH);
extern BYTE AdcIsEnable(UINT nCH);
extern WORD AdcGet(UINT nCH);

extern WORD Checksum16(BYTE *apbDst, UINT anNum);
extern void ChksumIrqCallback(irq_fn irqfn, void *arg);
extern void ChksumSetIrqEn(ENX_SWITCH sw);
extern ENX_SWITCH ChksumGetIrqEn(void);
extern void ChksumIrqClear(void);
extern UINT ChksumIsIrq(void);
extern void IrqChksum(void);

extern void AesInit(void);
extern void Aes128Enc(BYTE *apbDst, BYTE *apbSrc, UINT len);
extern void Aes128Dec(BYTE *apbDst, BYTE *apbSrc, UINT len);
extern void Aes256Enc(BYTE *apbDst, BYTE *apbSrc, UINT len);
extern void Aes256Dec(BYTE *apbDst, BYTE *apbSrc, UINT len);
extern void AesIrqCallback(irq_fn irqfn, void *arg);
extern void AesSetIrqEn(ENX_SWITCH sw);
extern ENX_SWITCH AesGetIrqEn(void);
extern void AesIrqClear(void);
extern UINT AesIsIrq(void);
extern void IrqAes(void);

extern void Sha224(BYTE *apbSrc, UINT len);
extern void Sha256(BYTE *apbSrc, UINT len);
extern void ShaIrqCallback(irq_fn irqfn, void *arg);
extern void ShaSetIrqEn(ENX_SWITCH sw);
extern ENX_SWITCH ShaGetIrqEn(void);
extern void ShaIrqClear(void);
extern UINT ShaIsIrq(void);
extern void IrqSha(void);

extern void WdtInit(ULONG milsec);
extern void WdtReset(void);

extern void BusInit(void);
extern void BusSetIrqPeriod(UINT msec);
extern UINT BusGetIrqPeriod(void);
extern void BusIrqCallback(irq_fn irqfn, void *arg);
extern void BusSetIrqEn(ENX_SWITCH sw);
extern ENX_SWITCH BusGetIrqEn(void);
extern void BusIrqClear(void);
extern UINT BusIsIrq(void);
extern void IrqBus(void);



#endif
