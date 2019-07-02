
#ifndef	_MOAL_SDIO_H
#define	_MOAL_SDIO_H

#ifndef BLOCK_MODE
/** Block mode */
#define BLOCK_MODE	1
#endif

#ifndef BYTE_MODE
/** Byte Mode */
#define BYTE_MODE	0
#endif

#ifndef FIXED_ADDRESS
/** Fixed address mode */
#define FIXED_ADDRESS	0
#endif


#ifdef STA_SUPPORT
/** Default firmware name */

#define DEFAULT_FW_NAME	"mrvl/sd8782_uapsta.bin"

#ifndef DEFAULT_FW_NAME
#define DEFAULT_FW_NAME ""
#endif
#endif /* STA_SUPPORT */

#ifdef UAP_SUPPORT
/** Default firmware name */

#define DEFAULT_AP_FW_NAME "mrvl/sd8782_uapsta.bin"

#ifndef DEFAULT_AP_FW_NAME
#define DEFAULT_AP_FW_NAME ""
#endif
#endif /* UAP_SUPPORT */

/** Default firmaware name */

#define DEFAULT_AP_STA_FW_NAME "mrvl/sd8782_uapsta.bin"

#ifndef DEFAULT_AP_STA_FW_NAME
#define DEFAULT_AP_STA_FW_NAME ""
#endif

/** Structure: SDIO MMC card */
struct sdio_mmc_card
{
  struct sdio_func *func;                                                       /** sdio_func structure pointer */        
  moal_handle *handle;                                                          /** moal_handle structure pointer */
};


/********************************************************
		Global Functions
********************************************************/
/** Register device function */
mlan_status woal_register_dev(moal_handle * handle);
/** Function to read register */
mlan_status woal_read_reg(moal_handle * handle, t_u32 reg, t_u32 * data);
/** Function to write data to IO memory */
mlan_status woal_write_data_sync(moal_handle * handle, mlan_buffer * pmbuf,
                                 t_u32 port, t_u32 timeout);
/** Function to write register */
mlan_status woal_write_reg(moal_handle * handle, t_u32 reg, t_u32 data);
#endif /* _MOAL_SDIO_H */
