#ifndef __ETHLOOPBACK_H__
#define __ETHLOOPBACK_H__

extern void EthloopbackTask(void *ctx);
extern eEthphyLoopbackMode EthloopbackGetMode(void);
extern void EthloopbackSetMode(eEthphyLoopbackMode eRunMode);
extern void EthloopbackSetLoop(UINT u32Loop);

#endif // __ETHLOOPBACK_H__
