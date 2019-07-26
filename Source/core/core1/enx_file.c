//*************************************************************************************************
// File system (FatFs)
//*************************************************************************************************
#include "dev.h"

#if defined(__FILESYSTEM__)

#include "enx_freertos.h"
#include "enx_file.h"
#include "sdcard.h"

#include <stdlib.h>	// for atoi

FATFS gtFatFs[FF_VOLUMES]; // File system object

//*************************************************************************************************
// File system
//-------------------------------------------------------------------------------------------------
#if 1
const char *put_rc(FRESULT rc)
{
	static const char *str[] = {
		"Succeeded\0",															// 0 FR_OK
		"A hard error occurred in the low level disk I/O layer\0",				// 1 FR_DISK_ERR
		"Assertion failed\0",													// 2 FR_INT_ERR
		"The physical drive cannot work\0",										// 3 FR_NOT_READY
		"Could not find the file\0",											// 4 FR_NO_FILE
		"Could not find the path\0",											// 5 FR_NO_PATH
		"The path name format is invalid\0",									// 6 FR_INVALID_NAME
		"Access denied due to prohibited access or directory full\0",			// 7 FR_DENIED
		"Access denied due to prohibited access\0",								// 8 FR_EXIST
		"The file/directory object is invalid\0",								// 9 FR_INVALID_OBJECT
		"The physical drive is write protected\0",								// 10 FR_WRITE_PROTECTED
		"The logical drive number is invalid\0",								// 11 FR_INVALID_DRIVE
		"The volume has no work area\0",										// 12 FR_NOT_ENABLED
		"There is no valid FAT volume\0",										// 13 FR_NO_FILESYSTEM
		"The f_mkfs() aborted due to any problem\0",							// 14 FR_MKFS_ABORTED
		"Could not get a grant to access the volume within defined period\0",	// 15 FR_TIMEOUT
		"The operation is rejected according to the file sharing policy\0",		// 16 FR_LOCKED
		"LFN working buffer could not be allocated\0",							// 17 FR_NOT_ENOUGH_CORE
		"Number of open files > FF_FS_LOCK\0",									// 18 FR_TOO_MANY_OPEN_FILES
		"Given parameter is invalid\0",											// 19 FR_INVALID_PARAMETER
		"Error Message Error\0"													// res error
	};
	return str[min(rc, FR_INVALID_PARAMETER)];
}

FRESULT drive_init(DriveNum dnNum)
{
	FRESULT res = FR_OK;
	char strPath[16] = {0};

	switch (dnNum) {
#if (LOAD_FS_FLS==1)
		case DEV_FLS:
			sprintf(strPath, "%d:/%s", DEV_FLS, FLS_DIR_WEB_PATH);
			res = fat_mkdir(dnNum, strPath);
			sprintf(strPath, "%d:/%s", DEV_FLS, FLS_DIR_LOG_PATH);
			res = fat_mkdir(dnNum, strPath);
			break;
#endif
#if (LOAD_FS_SDCARD==1)
		case DEV_SD:
			sprintf(strPath, "%d:/%s", DEV_SD, SD_DIR_NOR);
			res = fat_mkdir(dnNum, strPath);
			sprintf(strPath, "%d:/%s", DEV_SD, SD_DIR_EVT);
			res = fat_mkdir(dnNum, strPath);
			sprintf(strPath, "%d:/%s", DEV_SD, SD_DIR_SNAP);
			res = fat_mkdir(dnNum, strPath);
			break;
#endif
		default:
			break;
	}

	return res;
}

FRESULT drive_in(DriveNum dnNum)
{
	FRESULT res;
	DIR dir;
	TCHAR buf[25];

	sprintf(buf, "%d:", dnNum);
	res = f_mount(&gtFatFs[dnNum], (const TCHAR*)buf, 1);
	if (res == FR_OK) {
		res = f_opendir(&dir, buf);
		printf("%d:/ mount res : %s\n", dnNum, put_rc(res));
		if (res == FR_OK) {
			f_closedir(&dir);
		} else {
			printf("%d:/ dir check fail : %s\n", dnNum, put_rc(res));
			drive_out(dnNum);
		}
	} else {
		printf("%d:/ mount fail : %s\n", dnNum, put_rc(res));
	}

	return res;
}

FRESULT drive_out(DriveNum dnNum)
{
	FRESULT res;
	TCHAR buf[25];

	sprintf(buf, "%d:", dnNum);
	res = f_mount(NULL, (const TCHAR *)buf, 0);
	printf("%d:/ unmount res : %s\n", dnNum, put_rc(res));

	return res;
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
FRESULT fat_mkdir(DriveNum dnNum, char *strPath)
{
	BYTE *workbuf = NULL;

	UINT workbuf_len = 0x1000;
	DIR dir;
	FRESULT res = f_opendir(&dir, strPath);
	switch (res) {
		case FR_OK:	// dir Already
			f_closedir(&dir);
			break;
		case FR_NO_FILESYSTEM:
			printf("No File System\n");
			workbuf = pvPortMalloc(workbuf_len);
#if (LOAD_FS_FLS==1)
			if (dnNum == DEV_FLS) {
				char strDrive[4] = {0};
				sprintf(strDrive, "%d:", dnNum);
				res = f_mkfs(strDrive, 1, FAT_BLOCK_FLS, workbuf, workbuf_len);
				if (res != FR_OK) {
					break;
				}
			}
#if (LOAD_FS_SDCARD==1)
			else
#endif
#endif
#if (LOAD_FS_SDCARD==1)
			if (dnNum == DEV_SD) {
#if 0
				flprintf("Disable f_mkfs\n");
#else
#if 1
				char strDrive[4] = {0};
				sprintf(strDrive, "%d:", dnNum);
				res = f_mkfs(strDrive, FM_FAT32, FAT_BLOCK_SD, workbuf, workbuf_len);
				if (res != FR_OK) {
					printf("Error: %s\n", put_rc(res));
					SdioCdClockDown();
					res = f_mkfs(strDrive, FM_FAT32, FAT_BLOCK_SD, workbuf, workbuf_len);
					if (res != FR_OK) {
						break;
					}
				}
#else
				res = fat_sdmkfs(DEV_SD, FAT_BLOCK_SD);
				if (res != FR_OK) {
					printf("Error: %s\n", put_rc(res));
					Sdio0ClockDiv(eSDIO_CLK_25MHz);
					res = fat_sdmkfs(DEV_SD, FAT_BLOCK_SD);
					if (res != FR_OK) {
						break;
					}
				}
#endif
#endif
			}
#endif
		case FR_NO_PATH:
#if 0
			flprintf("Disable f_mkfs\n");
#else
			res = f_mkdir(strPath);
			if (res == FR_OK) {
				printf("Make %s : ", strPath);
			}
#endif
		default:
			printf("%s\n", put_rc(res));
			break;
	}

	if (workbuf) {
		vPortFree(workbuf);
	}
	
	return res;
}

BYTE fat_get_freesize(DriveNum dnNum)
{
	BYTE bFreesize = 0;
#if 0
	flprintf("Disable f_getfree\n");
#else
	FRESULT res;
	TCHAR buf[4] = {0};
	FATFS *fs = NULL;
	UINT fre_clust, tot_sect;
	sprintf(buf, "%d:", dnNum);
	res = f_getfree((const TCHAR *)buf, &fre_clust, &fs);
	if (res != FR_OK) {
		printf("Error(%s:%d) : %s\n", __func__, __LINE__, put_rc(res));
		return -1;
	}
	tot_sect = fs->n_fatent - 2;	//  * fs->csize
	bFreesize = (BYTE)((fre_clust*100.0)/tot_sect);
#ifdef ENX_DEBUG
	ULONG nVer;
	ULONG nPow = 0;
	nVer = fs->ssize * fs->csize;
	for (nPow = 0; nPow < 10; nPow++) {
		if ((nVer >> nPow) == 1024) {
			break;
		}
	}
	if (nPow == 10) {
		printf("Err : Not case\n");
	}
	printf("Disk Size (free size is %uKB, total size %uKB) ", (fre_clust << nPow), (tot_sect << nPow));
#endif
#endif
	printf("%d:/ Free %d%%\n", dnNum, bFreesize);
	return bFreesize;
}

FRESULT fat_ls(char *position)
{
	FRESULT res;
	FILINFO finfo;
	DIR dir;
	UINT s1, s2;
	DWORD p1;
	p1 = s1 = s2 = 0;

	res = f_opendir(&dir, position);
	if (res == FR_OK) {
		while (1) {
			res = f_readdir(&dir, &finfo);
			if ((res != FR_OK) || !finfo.fname[0]) {
				break;
			}

			if (finfo.fattrib & AM_DIR) {
				s2++;
			} else {
				s1++;
				p1 += finfo.fsize;
			}

			printf("\n%c%c%c%c%c %u/%02u/%02u %02u:%02u:%02u %9d  [%s]",
					(finfo.fattrib & AM_DIR) ? 'D' : '-',
					(finfo.fattrib & AM_RDO) ? 'R' : '-',
					(finfo.fattrib & AM_HID) ? 'H' : '-',
					(finfo.fattrib & AM_SYS) ? 'S' : '-',
					(finfo.fattrib & AM_ARC) ? 'A' : '-',
					(finfo.fdate >> 9) + 1980, (finfo.fdate >> 5) & 15, (finfo.fdate & 31),
					(finfo.ftime >> 11), (finfo.ftime >> 5) & 63, (finfo.ftime & 31) * 2,
					finfo.fsize, finfo.fname);
		}
		f_closedir(&dir);
	}
	printf("\n%6d File(s), %u bytes total\n%6d Dir(s)\n", s1, p1, s2);
	if (position[0] == '0') {
		fat_get_freesize(DEV_FLS);
	} else if (position[0] == '1') {
		fat_get_freesize(DEV_SD);
	}
	return res;
}

#if (LOAD_FS_SDCARD==1)
/**
	format sd card.
	we think that drive name is always '1:'
	buf if you change drvie name of SD. you should modify 'VolToPart'
	
	@param  dnNdum
			: should be '1' 
	@param au
			: sector*n
			for sd card, sector size should be 512,
			n, 1, 2, .. 128.

*/
FRESULT fat_sdmkfs(DriveNum dnNum, int au)
{
	FRESULT res = FR_OK;
#if 0
	flprintf("Disable f_mkfs\n");
#else
	char drive[4] = {0};

	sprintf(drive, "%d:", dnNum);

	res = f_mount(NULL, drive, 0);				// 1. unmount
	res = f_mount(&gtFatFs[dnNum], drive, 1);	// 2. mount

	UINT workbuf_len = 0x8000;
	BYTE *workbuf = pvPortMalloc(workbuf_len);
	if (workbuf == NULL) {
		printf("malloc fail\n");
		return FR_NOT_ENABLED;
	}

	res = f_mkfs(drive, 0, au, workbuf, workbuf_len);	// 3. format
	printf("f_mkfs: %s\n",put_rc(res));

	if (workbuf) {
		vPortFree(workbuf);
	}

	res = f_mount(NULL, drive, 0);				// 4. recheck, unmount
	res = f_mount(&gtFatFs[dnNum], drive, 1);	// 5. recheck, mount => state!
	printf("mount %s\n",put_rc(res));
#endif
	return res;
}
#endif

#endif
#endif
