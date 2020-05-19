#ifndef __ETHLOOPBACK_H__
#define __ETHLOOPBACK_H__

extern void EthloopbackTask(void *ctx);
extern void EthloopbackSetSpeed(UINT speed);
extern void EthloopbackSetDuplex(UINT duplex);
extern eEthphyLoopbackMode EthloopbackGetMode(void);
extern void EthloopbackSetMode(eEthphyLoopbackMode eRunMode);
extern void EthloopbackSetLoop(UINT u32Loop);
extern void EthloopbackSetOption(BYTE u8Option);

#endif // __ETHLOOPBACK_H__
