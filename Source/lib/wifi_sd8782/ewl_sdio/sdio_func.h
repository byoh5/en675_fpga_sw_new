
#ifndef __SDIO__FUNC__
#define __SDIO__FUNC__

#include "dev_types.h"

#define sdio_get_drvdata(f)     (f->priv_data)
#define sdio_set_drvdata(f,d)   (f->priv_data) = d

/**
 * SDIO_DEVICE - macro used to describe a specific SDIO device
 * @vend: the 16 bit manufacturer code
 * @dev: the 16 bit function id
 *
 * This macro is used to create a struct sdio_device_id that matches a
 * specific device. The class field will be set to SDIO_ANY_ID.
 */
#define SDIO_DEVICE(vend,dev) \
        .class = SDIO_ANY_ID, \
        .vendor = (vend), .device = (dev)

#define MARVELL_VENDOR_ID  0x02df

/* SDIO */
#define SDIO_ANY_ID (~0)
 
struct sdio_device_id {
	u8    class;                  /* Standard interface or SDIO_ANY_ID  */
	u16   vendor;                 /* Vendor or SDIO_ANY_ID              */
	u16   device;                 /* Device ID or SDIO_ANY_ID           */
};

/*
* SDIO function devices
*/
struct sdio_func {
  unsigned int            num;            /* function number */
  unsigned char           class;          /* standard interface class */
  unsigned short          vendor;         /* vendor id */
  unsigned short          device;         /* device id */
  unsigned short		  blksz;		  /* sdio blk size*/
  void                    *priv_data;     /* saved pointer for struct sdio_mmc_card */
};

/*
 * SDIO function device driver
 */
struct sdio_driver 
{     
    struct sdio_driver *next;
    char *name;
    struct sdio_device_id *id_table;
    
    int (*probe)(struct sdio_func*, const struct sdio_device_id *);
    void (*remove)(struct sdio_func *);
};

typedef void (sdio_irq_handler_t)(struct sdio_func *func);
#endif //__SDIO__FUNC__