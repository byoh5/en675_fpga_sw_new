//*************************************************************************************************
// User shell commands
//*************************************************************************************************
#include "dev.h"
#include "enx_freertos.h"

#include "shell_cmd_fs.h"

#include <stdio.h>		// for snprintf
#include <stdlib.h>		// for atoi
#include <string.h>		// for strstr
#include "md5.h"		// for md5

#if defined(__FILESYSTEM__)
#include "enx_file.h"
#if (LOAD_FS_SDCARD==1)
#include "sdcard.h"
#endif
#endif

#if defined(__FILESYSTEM__)
const char *sUsrLSCmd[]     = {"List Contents of Directory",    (char*)0};
const char *sDiskInitCmd[]  = {"Disk Init",                     (char*)0};
const char *sFormatCmd[]    = {"Disk FORMAT",                   (char*)0};
const char *sMKDIRCmd[]     = {"Make Directory",                (char*)0};
const char *sRMDIRCmd[]     = {"Remove Directory",              (char*)0};
const char *sRMCmd[]        = {"Remove File",                   (char*)0};
const char *sPWDCmd[]       = {"Print Working Directory",       (char*)0};
const char *sCDCmd[]        = {"Change Directory",              (char*)0};
const char *sFCatCmd[]      = {"File Concatenate",              (char*)0};
const char *sFCreateCmd[]   = {"File Create(test)",             (char*)0};
const char *sFCopyCmd[]     = {"File Copy(test)",               (char*)0};
const char *sFstatCmd[]     = {"File Info",                     (char*)0};
const char *sFileHash[]     = {"File Hash Check",               (char*)0};
#if (LOAD_FS_SDCARD==1)
const char *sSDCardSpeedTestCmd[] = {"SDCard Speed Test",       (char*)0};
const char *sMakevidCmd[]   = {"H.264 SD Save On/Off",          (char*)0};
#endif
const char *sFatTestCmd[]   = {"File system Test",              (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//...............................................
//
int UsrLSCmd(INT32S argc, char *argv[])
{
	char strGetcwd[256] = {0};
	f_getcwd(strGetcwd, 256);
	fat_ls(strGetcwd);
	printf("\n");
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

//...............................................
//
int UsrDiskInitCmd(INT32S argc, char *argv[])
{
	if (argc != 2) {
		printf("error : ex) init 0:   // 0: driver default init\n");
		return 0;
	}

	int dnNum = argv[1][0] - '0';
	FRESULT res = drive_init(dnNum);
	printf("%s\n", put_rc(res));
	return 0;
}

//...............................................
//
int UsrFormatCmd(INT32S argc, char *argv[])
{
	if (argc != 2) {
		printf("error : ex) format 0:   // 0: driver format\n");
		return 0;
	}

	UINT workbuf_len = 0x8000;
	BYTE *workbuf = pvPortMalloc(workbuf_len);
	FRESULT res = FR_OK;
	int dnNum = argv[1][0] - '0';
	printf("%d: Format Start\n", dnNum);
#if (LOAD_FS_FLS==1)
	if (dnNum == DEV_FLS) {
		res = f_mkfs(argv[1], FM_FAT, FAT_BLOCK_FLS, workbuf, workbuf_len);
	} else
#endif
#if (LOAD_FS_SDCARD==1)
	if (dnNum == DEV_SD) {
		res = f_mkfs(argv[1], FM_FAT32, FAT_BLOCK_SD, workbuf, workbuf_len);
	}
#endif

	if (workbuf) {
		vPortFree(workbuf);
	}

	printf("%s Format %s.\n", argv[1], put_rc(res));
	return 0;
}

//...............................................
//
int UsrMKRMDIRCmd(INT32S argc, char *argv[])
{
#if 1
	if (argc == 2) {
		FRESULT res; 
		if (strcmp(argv[0], "mkdir") == 0) {
			printf("Make %s dir, ", argv[1]);
			res = f_mkdir(argv[1]);
			printf("%s.\n", put_rc(res));
		} else if (strcmp(argv[0], "rmdir") == 0) {
			printf("Remove %s dir, ", argv[1]);
			res = f_unlink(argv[1]);
			printf("%s.\n", put_rc(res));
		} else {
			Shell_Unknown();
		}
	} else {
		Shell_Unknown();
	}
#else
	printf("[%s] Unsupported command\n", argv[0]);
#endif
	return 0;
}

//...............................................
//
int UsrRMCmd(INT32S argc, char *argv[])
{
#if 1
	if (argc == 2) {
		FRESULT res;
		if (strcmp(argv[1], "*") == 0) {
			printf("Remove all file.\n");

			FILINFO finfo;
			DIR dir;
			char strGetcwd[256] = {0};

			res = f_getcwd(strGetcwd, sizeof(strGetcwd));
			if (res == FR_OK) {
				res = f_opendir(&dir, strGetcwd);
				while (1) {
					res = f_readdir(&dir, &finfo);
					if ((res != FR_OK) || !finfo.fname[0]) {
						res = FR_OK;
						break;
					}

					if (finfo.fattrib & AM_ARC) {
						printf("Remove %s file, ", finfo.fname);
						res = f_unlink(finfo.fname);
						printf("%s.\n", put_rc(res));
					}
				}
				f_closedir(&dir);
			}
			printf("Done.\n");
		} else {
			printf("Remove %s file, ", argv[1]);
			res = f_unlink(argv[1]);
			printf("%s.\n", put_rc(res));
		}
	} else {
		Shell_Unknown();
	}
#else
	printf("[%s] Unsupported command\n", argv[0]);
#endif
	return 0;
	UNUSED(argc);
}

//...............................................
//
int UsrPWDCmd(INT32S argc, char *argv[])
{
	char strGetcwd[256] = {0};
	FRESULT res = f_getcwd(strGetcwd, 256);
	if (res != FR_OK) {
		printf("Error : PWD dir failed(%s)\n", put_rc(res));
	} else {
		printf("%s\n", strGetcwd);
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

//...............................................
//
int UsrCDCmd(INT32S argc, char *argv[])
{
	if (argc == 2) {
#if (LOAD_FS_FLS==1)
		char strFLSdir[8];
		sprintf(strFLSdir, "%d:", DEV_FLS);
		if (strstr(argv[1], strFLSdir) != 0) {
			f_chdrive(strFLSdir);
		}
#endif

#if (LOAD_FS_SDCARD==1)
		char strMMCdir[8];
		sprintf(strMMCdir, "%d:", DEV_SD);
		if (strstr(argv[1], strMMCdir) != 0) {
			f_chdrive(strMMCdir);
		}
#endif

		if (f_chdir(argv[1]) == FR_OK) {
			char strGetcwd[256] = {0};
			f_getcwd(strGetcwd, 256);
			printf("%s\n", strGetcwd);
			return 0;
		}
	}

	printf("Could not find the path.\n");
	return 0;
}

#define SAVEK 512
#define SAVE_BUFF (SAVEK*1024)

int UsrFCatCmd(INT32S argc, char *argv[])
{
#if 1
	if (argc != 4) {
		printf("info : fcat file1name file2name outfilename\n");
		printf("ex   : fcat a.txt b.txt c.txt\n");
		printf("     : c.txt result => a.txt data + b.txt data\n");
		return 0;
	}

	// sd카드에 공간부족여부 확인
	// ?
	//

	FIL infp, outfp;
	FRESULT fres;

	BYTE strBuff[SAVE_BUFF];
	memset(strBuff, 0, SAVE_BUFF);
	UINT br, bw;
	UINT time_s1, time_s2, time_e;
	float tot_time;

	fres = f_open(&outfp, argv[3], FA_WRITE | FA_CREATE_ALWAYS);
	if (fres != FR_OK) {
		printf("%s f_open fail(%s)\n", argv[3], put_rc(fres));
		return 0;
	}
	printf("%s f_open ok\n", argv[3]);

	time_s1 = xTaskGetTickCount();
	fres = f_open(&infp, argv[1], FA_READ);
	if (fres != FR_OK) {
		printf("%s f_open fail(%s)\n", argv[1], put_rc(fres));
		f_close(&outfp);
		return 0;
	}
	printf("%s f_open ok\n", argv[1]);

	printf("%s => %s copy start\n", argv[1], argv[3]);
	for (;;) {
		fres = f_read(&infp, strBuff, SAVE_BUFF, &br);
		if (fres != FR_OK) {
			printf("%s f_read fail(%s)\n", argv[1], put_rc(fres));
			f_close(&outfp);
			f_close(&infp);
			return 0;
		}
		if (br == 0) {
			f_close(&infp);
			printf("%s => %s copy ok\n", argv[1], argv[3]);
			break;
		}

		fres = f_write(&outfp, strBuff, br, &bw);
		if (fres != FR_OK) {
			printf("%s f_write fail(%s)\n", argv[3], put_rc(fres));
			f_close(&infp);
			f_close(&outfp);
			return 0;
		}
	}
	time_e = xTaskGetTickCount();
	tot_time = (time_e - time_s1) / (float)TIME_TICK;
	printf("time %4.2f\n", tot_time);

	time_s2 = xTaskGetTickCount();
	fres = f_open(&infp, argv[2], FA_READ);
	if (fres != FR_OK) {
		printf("%s f_open fail(%s)\n", argv[2], put_rc(fres));
		f_close(&outfp);
		return 0;
	}
	printf("%s f_open ok\n", argv[2]);

	printf("%s => %s copy start\n", argv[2], argv[3]);
	for (;;) {
		fres = f_read(&infp, strBuff, SAVE_BUFF, &br);
		if (fres != FR_OK) {
			printf("%s f_read fail(%s)\n", argv[2], put_rc(fres));
			f_close(&outfp);
			f_close(&infp);
			return 0;
		}
		if (br == 0) {
			f_close(&infp);
			printf("%s => %s copy ok\n", argv[2], argv[3]);
			break;
		}

		fres = f_write(&outfp, strBuff, br, &bw);
		if (fres != FR_OK) {
			printf("%s f_write fail(%s)\n", argv[3], put_rc(fres));
			f_close(&infp);
			f_close(&outfp);
			return 0;
		}
	}
	time_e = xTaskGetTickCount();
	tot_time = (time_e - time_s2) / (float)TIME_TICK;
	printf("time %4.2f\n", tot_time);

	f_close(&outfp);
	printf("%s fcat ok\n", argv[3]);
	tot_time = (time_e - time_s1) / (float)TIME_TICK;
	printf("total time %4.2f\n", tot_time);
#else
	printf("[%s] Unsupported command\n", argv[0]);
#endif
	return 0;
}

int UsrFCreateCmd(INT32S argc, char *argv[])
{
#if 1
	if (argc != 5 && argc != 6) {
		printf("info : fcreate filename unitsize(kbyte) filesize(kbyte) character (opt:loop count)\n");
		printf("ex   : fcreate test.txt 2 4 A\n");
		printf("       => test.txt file create, file size 4kbyte\n");
		printf("ex   : fcreate test 2 4 A 100\n");
		printf("       => test0, test1, test2 .. test99 file create, file size 4kbyte\n");
		printf("          file content : \"A\" string\n");
		printf("          max unit size : 64\n");
		return 0;
	}

	FIL filFile;
	FRESULT fres;

	BYTE *strBuff = NULL;
	char strNameBuff[255];
	char cFileContent = argv[4][0];
	uint32 fsize = 0, createcount, i;
	uint32 unitsize = atoi(argv[2]) * 1024;
	uint32 filesize = atoi(argv[3]) * 1024;
	UINT nFileWirteSize;

	strBuff = (BYTE *)pvPortMalloc(SAVE_BUFF);
	if (strBuff == NULL) {
		printf("malloc fila(%d)\n", SAVE_BUFF);
		return 0;
	}

	for (i = 0; i < unitsize; i++) {
		strBuff[i] = cFileContent;
	}

	if (argc == 6) {
		createcount = atoi(argv[5]);
	} else {
		createcount = 1;
	}

	uint32 avg_tot_time = 0;
	uint32 avg_bps = 0;

	for (i = 0; i < createcount; i++) {
		if (createcount == 1) {
			sprintf(strNameBuff, "%s", argv[1]);
		} else {
			sprintf(strNameBuff, "%s%d", argv[1], i);
		}
		printf("create : %s, ", strNameBuff);
		uint32 time_s = xTaskGetTickCount();
		fres = f_open(&filFile, strNameBuff, FA_WRITE | FA_CREATE_ALWAYS);
		if (fres != FR_OK) {
			printf("%s\n", put_rc(fres));
			goto fcreate_done;
		}

		for (fsize = 0; fsize < filesize; fsize += unitsize) {
			fres = f_write(&filFile, strBuff, unitsize, &nFileWirteSize);
			if (fres != FR_OK) {
				printf("%s\n", put_rc(fres));
				break;
			}
		}

		fres = f_close(&filFile);
		uint32 time_e = xTaskGetTickCount();
		uint32 tot_time = (time_e - time_s) * (1000 / TIME_TICK);
		uint32 KBytePerSec = ((fsize >> 10) * 1000) / tot_time;
		avg_tot_time += tot_time;
		avg_bps += KBytePerSec;
		printf("total time %dms, KBytePerSec %dKB/s\n", tot_time, KBytePerSec);
		vTaskDelay(1);
	}

	printf("create file count : %d\n", createcount);
	printf("average time      : %dms\n", avg_tot_time / createcount);
	printf("average KByte/sec : %dKB/s\n", avg_bps / createcount);

fcreate_done:
	if (strBuff) {
		vPortFree(strBuff);
	}
#else
	printf("[%s] Unsupported command\n", argv[0]);
#endif
	return 0;
}

int UsrFCopyCmd(INT32S argc, char *argv[])
{
#if 1
	if (argc != 3) {
		printf("err input\n");
		printf("ex : %s target_filename new_filename\n", argv[0]);
		return 0;
	}

	printf("target file name : [%s]\n", argv[1]);
	printf("new file name : [%s]\n", argv[2]);

	BYTE *strBuff = (BYTE *)pvPortCalloc(SAVE_BUFF, 1);
	if (strBuff == NULL) {
		printf("error : fcreate malloc fail\n");
		return 0;
	}

	FIL fp_r, fp_w;
	UINT nRSize, nWSize, nTotSize = 0;

	if (f_open(&fp_r, argv[1], FA_READ) != FR_OK) {
		printf("Error : f_open File system fail(%s)\n", argv[1]);
		goto done;
	}
	if (f_open(&fp_w, argv[2], FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
		printf("Error : f_open File system fail(%s)\n", argv[2]);
		goto done;
	}

	while (1) {
		if (f_eof(&fp_r)) {
			break;
		}
	
		if (f_read(&fp_r, strBuff, SAVE_BUFF, &nRSize) != FR_OK) {
			printf("Error : f_read File system fail(%s)\n", argv[1]);
			break;
		}
		if (f_write(&fp_w, strBuff, nRSize, &nWSize) != FR_OK) {
			printf("Error : f_write File system fail(%s)\n", argv[1]);
			break;
		}

		nTotSize += nWSize;
	}

	f_close(&fp_r);
	f_close(&fp_w);

	printf("file copy end(%dbyte)\n", nTotSize);

done:
	if (strBuff) {
		vPortFree(strBuff);
	}
#else
	printf("[%s] Unsupported command\n", argv[0]);
#endif
	return 0;
}

int UsrFstatCmd(INT32S argc, char *argv[])
{
	if (argc != 2) {
		printf("Error : ex) fstat filename\n");
		return 0;
	}

	FILINFO finfo;
	FRESULT res = f_stat(argv[1], &finfo);
	if (res != FR_OK) {
		printf("Get %s file status, %s.\n", argv[1], put_rc(res));
		return 0;	
	}

	//printf("FILE ALTNAME : %s\n", finfo.altname);
	printf("FILE NAME : %s\n", finfo.fname);
	printf("FILE SIZE : %d\n", finfo.fsize);
	printf("FILE Date : %d\n", finfo.fdate);
	printf("FILE Time : %d\n", finfo.ftime);
	printf("FILE Attribute : 0x%X\n", finfo.fattrib);

	return 0;
}

int UsrFileHash(INT32S argc, char *argv[])
{
#if 1
	if (argc != 2) {
		printf("err input\n");
		printf("ex : %s target_filename\n", argv[0]);
		return 0;
	}

	FIL fp;
	FRESULT res;
	UINT nSize;

	char strMD5[33] = {0};
	unsigned char hash[16];
	MD5_CTX mdContext;

	printf("target file name : [%s]\n", argv[1]);
	BYTE *bBuff = (BYTE *)pvPortCalloc(SAVE_BUFF, 1);
	if (bBuff == NULL) {
		printf("Error : malloc fail\n");
		return 0;
	}

	if ((res = f_open(&fp, argv[1], FA_READ)) != FR_OK) {
		printf("Open %s file, %s.\n", argv[1], put_rc(res));
		goto done;
	}

	MD5Init(&mdContext);

	while (1) {
		if (f_eof(&fp)) {
			break;
		}

		if ((res = f_read(&fp, bBuff, SAVE_BUFF, &nSize)) != FR_OK) {
			printf("Read %s file, %s.\n", argv[1], put_rc(res));
			break;
		}

		MD5Update(&mdContext, bBuff, nSize);
	}

	f_close(&fp);

	MD5Final(hash, &mdContext);
	Hex2Str32B(mdContext.digest, strMD5);

	printf("MD5: %s\n\n", strMD5);

done:
	if (bBuff) {
		vPortFree(bBuff);
	}
#else
	printf("[%s] Unsupported command\n", argv[0]);
#endif
	return 0;
}

#if (LOAD_FS_SDCARD==1)
int UsrSDCardSpeedTestCmd(INT32S argc, char *argv[])
{
#if 1
	const UINT arrUnitsize[] = {512, 1*1024, 2*1024, 4*1024, 8*1024, 16*1024, 32*1024, 64*1024, 128*1024, /*256*1024, 512*1024, 1024*1024*/};
	const UINT nTestcount = sizeof(arrUnitsize) / sizeof(arrUnitsize[0]);
	const char *line = "|-----------%c------------%c------------%c------------%c------------|\n";
	const char *strFilebuff = "1:/spdtest.bin";
	const UINT nTimeoutSec = 5;
	UINT nFilesize = 256 * 1024 * 1024;

	FIL fp;
	FRESULT fres;
	BYTE *arrUnit = NULL;
	UINT i, j, br, loop, time_s, time_m, time_e, tot_time, KBytePerSec, procSize, m_out;

	if (argc == 2) {
		nFilesize = atoi(argv[1]) * 1024 * 1024;
	}

	if (getSDState() != sd_IDLE) {
		printf("SDCard no idle!\n");
		goto done1;
	}

	printf("Test Filesize(%dbyte) MaxUnitSize(%dByte) Timeout(%dsec)\n", nFilesize, arrUnitsize[nTestcount-1], nTimeoutSec);
	arrUnit = (BYTE *)pvPortMalloc(arrUnitsize[nTestcount-1]);
	if (arrUnit == NULL) {
		printf("Malloc fail\n");
		goto done1;
	}

	fres = f_open(&fp, strFilebuff, FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
	if (fres != FR_OK) {
		printf("f_open: %s\n", put_rc(fres));
		goto done1;
	}

	fres = f_expand(&fp, nFilesize, 1);
	if (fres != FR_OK) {
		printf("f_expand: %s\n", put_rc(fres));
		goto done1;
	}

	char strBuf[512];

	printf(line, '-', '-', '-', '-');
	printf("| %9s | %10s | %10s | %10s | %10s |\n", "Unit size", "WriteTime", "WriteSpeed", "ReadTime", "ReadSpeed");
	printf("| %9s | %10s | %10s | %10s | %10s |\n", "", "(s)", "(KB/s)", "(s)", "(KB/s)");
	printf(line, '|', '|', '|', '|');
	for (i = 0; i < nTestcount; i++) {
		f_lseek(&fp, 0);

		snprintf(strBuf, 512, "| %7.1fKB | ", arrUnitsize[i] / 1024.0);
		printf("%s", strBuf);
		loop = nFilesize / arrUnitsize[i];
		time_s = xTaskGetTickCount();

		procSize = m_out = 0;
		for (j = 0; j < loop ; j++) {
			fres = f_write(&fp, arrUnit, arrUnitsize[i], &br);
			if (fres != FR_OK) {
				printf("f_write: %s\n", put_rc(fres));
				goto done2;
			} else if (arrUnitsize[i] != br) {
				printf("f_write: all write fail(%d/%d)\n", br, arrUnitsize[i]);
				goto done2;
			}
			procSize += br;
			time_m = xTaskGetTickCount();
			if ((time_m - time_s) >= (nTimeoutSec * TIME_TICK)) {
				m_out = 1;
				break;
			}
		}

		if (m_out == 1) {
			time_e = time_m;
		} else {
			time_e = xTaskGetTickCount();
		}
		tot_time = (time_e - time_s) * (1000 / TIME_TICK);
		KBytePerSec = ((procSize >> 10) * 1000) / tot_time;

		snprintf(strBuf, 512, "%10.2f | %10d | ", tot_time / 1000.0, KBytePerSec);
		printf("%s", strBuf);

		f_lseek(&fp, 0);

		time_s = xTaskGetTickCount();

		procSize = m_out = 0;
		for (j = 0; j < loop ; j++) {
			fres = f_read(&fp, arrUnit, arrUnitsize[i], &br);
			if (fres != FR_OK) {
				printf("f_read: %s\n", put_rc(fres));
				goto done2;
			} else if (arrUnitsize[i] != br) {
				printf("f_read: all read fail(%d/%d)\n", br, arrUnitsize[i]);
				goto done2;
			}
			procSize += br;
			time_m = xTaskGetTickCount();
			if ((time_m - time_s) >= (nTimeoutSec * 100)) {
				m_out = 1;
				break;
			}
		}

		if (m_out == 1) {
			time_e = time_m;
		} else {
			time_e = xTaskGetTickCount();
		}
		tot_time = (time_e - time_s) * (1000 / TIME_TICK);
		KBytePerSec = ((procSize >> 10) * 1000) / tot_time;

		snprintf(strBuf, 512, "%10.2f | %10d |\n", tot_time / 1000.0, KBytePerSec);
		printf("%s", strBuf);
	}

	fres = f_close(&fp);
	if (fres != FR_OK) {
		printf("f_close: %s\n", put_rc(fres));
		goto done2;
	}

	fres = f_unlink(strFilebuff);
	if (fres != FR_OK) {
		printf("f_unlink: %s\n", put_rc(fres));
		goto done2;
	}

done2:
	printf(line, '-', '-', '-', '-');
done1:

	if (arrUnit) {
		vPortFree(arrUnit);
	}
#else
	printf("[%s] Unsupported command\n", argv[0]);
#endif
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

int UsrMakevidCmd(INT32S argc, char *argv[])
{
	if (argc != 2) {
		printf("Error : ex) %s on   // Start normal recording on the SD card.\n", argv[0]);
		printf("        ex) %s off  // Stop normal recording on SD card.\n", argv[0]);
#if (FAT_SDSAVE_EVENT==1)
		printf("        ex) %s e    // Start event recording on the SD card.\n", argv[0]);
#endif
		return 0;
	}

	if (strcmp("on", argv[1]) == 0) {
		if (gtUser.bSdVidSave != ENX_ON) {
			gtUser.bSdVidSave = ENX_ON;
			printf("SD SAVE ON\n");
		} else {
			printf("Error : Already SD Save ON\n");
		}
	} else if (strcmp("off", argv[1]) == 0) {
		if (gtUser.bSdVidSave != ENX_OFF) {
			gtUser.bSdVidSave = ENX_OFF;
			printf("SD SAVE OFF\n");
		} else {
			printf("Error : Already SD Save OFF\n");
		}
	}
#if (FAT_SDSAVE_EVENT==1)
	else if (strcmp("e", argv[1]) == 0) {
		if (muxer_videnc_state(eRecEvent) == 0) {
			muxer_videnc_init();
			muxer_videnc_go(eRecEvent);
		}
	}
#endif
	else {
#if (FAT_SDSAVE_EVENT==1)
		printf("Error : option 'on' or 'off' or 'e'\n");
#else
		printf("Error : option 'on' or 'off'\n");
#endif
	}
	return 0;
}
#endif

int UseFatTest(INT32S argc, char *argv[])
{
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}
#endif
