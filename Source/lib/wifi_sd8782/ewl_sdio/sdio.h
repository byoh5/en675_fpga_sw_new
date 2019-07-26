#ifndef MMC_SDIO_H
#define MMC_SDIO_H




#include "sdio_func.h"
//Extern
//function prototye
//--------------------------------------------------------------------------------------
extern int sdio_init(struct sdio_func **pfunc);
extern unsigned char sdio_readb(struct sdio_func *func, unsigned int addr, int *err_ret);
extern void sdio_writeb(struct sdio_func *func, u8 b, unsigned int addr, int *err_ret);
extern void sdio_claim_host(struct sdio_func *func);
extern void sdio_release_host(struct sdio_func *func);
extern int sdio_enable_func(struct sdio_func *func);
extern int sdio_disable_func(struct sdio_func *func);
extern int sdio_set_block_size(struct sdio_func *func, u32 blksz);
extern int sdio_claim_irq(struct sdio_func *func, sdio_irq_handler_t *handler);
extern void sdio_register_driver(struct sdio_driver *drv);
extern int sdio_release_irq(struct sdio_func *func);
extern int sdio_writesb(struct sdio_func *func, unsigned int addr, void *src,
        int count);
extern int sdio_write_pbuf(struct sdio_func *func, unsigned int addr, void *p,
        int count);        
     
int sdio_readsb(struct sdio_func *func, void *dst, unsigned int addr,
        int count);
        
extern int sdio_read_pbuf(struct sdio_func *func, void *p, unsigned int addr,
        int count);     
        
extern void sd_irq_handler(void *dummy);           
#endif /* MMC_SDIO_H */

