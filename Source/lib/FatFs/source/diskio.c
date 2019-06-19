/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "dev.h"
#include "sdcard.h"

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv) {
#if (LOAD_FS_FLS==1)
	case DEV_FLS :
		return 0;
#endif
#if (LOAD_FS_SDCARD==1)
	case DEV_SD :
#if defined(__USE_SDIOCD__)
		if (SdioCdDet()) {
			return (STA_NODISK | STA_NOINIT);
		}
		if (SdioCdGetActive()) {
			return 0;
		}
#endif
		break;
#endif
#if (LOAD_FS_USB==1)
	case DEV_USB :
		return (STA_NODISK | STA_NOINIT);
#endif
#if (LOAD_FS_MEM==1)
	case DEV_MEM:
		return (STA_NODISK | STA_NOINIT);
#endif
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv) {
#if (LOAD_FS_FLS==1)
	case DEV_FLS :
		return 0;
#endif
#if (LOAD_FS_SDCARD==1)
	case DEV_SD :
#if defined(__USE_SDIOCD__)
		if (SdioCdDet()) {
			return (STA_NODISK | STA_NOINIT);
		}
		if (SdioCdGetActive()) {
			return 0;
		}
#endif
		break;
#endif
#if (LOAD_FS_USB==1)
	case DEV_USB :
		return (STA_NODISK | STA_NOINIT);
#endif
#if (LOAD_FS_MEM==1)
	case DEV_MEM:
		return (STA_NODISK | STA_NOINIT);
#endif
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;

	if (count == 0) {
		return res;
	}

	switch (pdrv) {
#if (LOAD_FS_FLS==1)
	case DEV_FLS :
		break;
#endif
#if (LOAD_FS_SDCARD==1)
	case DEV_SD :
#if defined(__USE_SDIOCD__)
		res = RES_OK;
//		flprintf("CNT(%2u) buff(0x%08X) addr(0x%08X)\n", count, buff, sector);
		if (SdioCdRead((BYTE *)buff, (UINT)sector, (UINT)count) == ENX_FAIL) {
			res = RES_ERROR;
		}
#endif
		break;
#endif
#if (LOAD_FS_USB==1)
	case DEV_USB :
		break;
#endif
#if (LOAD_FS_MEM==1)
	case DEV_MEM:
		break;
#endif
	}
	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;
//	int result;

	switch (pdrv) {
#if (LOAD_FS_FLS==1)
	case DEV_FLS :
		return res;
#endif
#if (LOAD_FS_SDCARD==1)
	case DEV_SD :
		res = RES_OK;
//		flprintf("CNT(%2u) buff(0x%08X) addr(0x%08X)\n", count, buff, sector);
		if (SdioCdWrite((BYTE *)buff, (UINT)sector, (UINT)count) == ENX_FAIL) {
			res = RES_ERROR;
		}
		return res;
#endif
#if (LOAD_FS_USB==1)
	case DEV_USB :
		return res;
#endif
#if (LOAD_FS_MEM==1)
	case DEV_MEM:
		return res;
#endif
	}
	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;
//	int result;

	WORD  *pW  = (WORD  *)buff;
	DWORD *pDw = (DWORD *)buff;

	switch (pdrv) {
#if (LOAD_FS_FLS==1)
	case DEV_FLS :
		switch (cmd) {
		case CTRL_SYNC:
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:
			*pDw = NO_OF_SERCTORS;
			res = RES_OK;
		case GET_SECTOR_SIZE:
			*pW = (WORD)ERASE_BLOCK_SIZE;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE:
			*pDw = (DWORD)1;
			res = RES_OK;
			break;
		case CTRL_TRIM:
			res = RES_OK;
			break;
		}
		return res;
#endif
#if (LOAD_FS_SDCARD==1)
	case DEV_SD :
#if defined(__USE_SDIOCD__)
		switch (cmd) {
		case CTRL_SYNC :
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT :
			*pDw = (DWORD)SdioCdGetSectorCnt();
			res = RES_OK;
			break;
		case GET_SECTOR_SIZE :
			*pW = (WORD)512;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE :
			*pDw = (DWORD)(SdioCdGetAUSize()+1);
			res = RES_OK;
			break;
		case CTRL_TRIM :
			res = SdioCdE((UINT)pDw[0], (UINT)pDw[1]);
			break;
		}
#endif
		break;
#endif
#if (LOAD_FS_USB==1)
	case DEV_USB :
		return res;
#endif
#if (LOAD_FS_MEM==1)
	case DEV_MEM:
		return res;
#endif
	}
	return res;
}

/*-----------------------------------------------------------------------*/
/* Time Functions                                                        */
/*-----------------------------------------------------------------------*/
DWORD get_fattime(void)
{
	struct tm tmout;
	enx_get_tmtime(gptMsgShare.TIME, &tmout, ENX_YES);
	return    ((DWORD)(tmout.tm_year - 80)	<< 25)		// (tTime->tm_year + 1900 - 1980)
			| ((DWORD)(tmout.tm_mon + 1)	<< 21)
			| ((DWORD)(tmout.tm_mday)		<< 16)
			| ((DWORD)(tmout.tm_hour)		<< 11)
			| ((DWORD)(tmout.tm_min)		<<  5)
			| ((DWORD)(tmout.tm_sec / 2)	<<  0);
}
