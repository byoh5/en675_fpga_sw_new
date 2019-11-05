#include "dev.h"

#if defined(__NETWORK__)
#if defined(__FILESYSTEM__)
#if (LOAD_FS_SDCARD==1)

#include <string.h>

#include "lwip/mem.h"
#include "enx_file.h"
#include "fsfilelist.h"
#include "sdcard.h"
#include "muxer_videnc.h"
#include "videnc_info.h"

#define ENX_WEBLS_DGB_DEBUG		ENX_ON
#define ENX_WEBLS_DGB_ERROR		ENX_ON

#define tprintf(debug, fmt, args...) do { if((debug)&ENX_ON) printf("    %04d:%s: "fmt, __LINE__, __func__, ##args); } while(0);

typedef enum {
	fext_None,
	fext_MP4,
} FSFILELIST_EXT; // FSFILELIST - extension

typedef struct {
	char *flname;						// Use it if the name is longer than SAVE_FILENAME_LENS.
	char fname[SAVE_FILENAME_LENS+1];
	BYTE fattrib;
	BYTE extension;
	WORD fdate;
	WORD ftime;
	UINT fsize;
} FSFILELIST;

typedef struct {
	FSFILELIST *flItem;
	
	UINT nTotalDirCnt;
	UINT nTotalFileCnt;
	UINT nTotalItemCnt;

	UINT nDirCnt;
	UINT nFileCnt;
	UINT nMP4FileCnt;
	UINT nOutputCnt;

	UINT nSumItemname;	// DIR/File name length
	UINT nSumFilesize;	// File size length

	// Variables for re-request
	char *strPath;
	UINT nPathSize;
} FSFLData;

static FSFLData fsfld_html; // webls html only
int gWebls_page = 1;		// webls html only

static int webls_number_count(UINT number)
{
	int i;
	for (i = 0; number / 10 != 0; i++) {
		number = number / 10;
	}
	return i + 1;
}

static void webls_fsfld_free(FSFLData *pfsfld)
{
	if (pfsfld->strPath) {
		vPortFree(pfsfld->strPath);
		pfsfld->strPath = NULL;
	}

	if (pfsfld->flItem) {
		UINT i;
		for (i = 0; i < pfsfld->nTotalItemCnt; i++) {
			if (pfsfld->flItem[i].flname) {
				vPortFree(pfsfld->flItem[i].flname);
				pfsfld->flItem[i].flname = NULL;
			}
		}

		vPortFree(pfsfld->flItem);
		pfsfld->flItem = NULL;
	}

	pfsfld->nTotalDirCnt = 0;
	pfsfld->nTotalFileCnt = 0;
	pfsfld->nTotalItemCnt = 0;
	pfsfld->nDirCnt = 0;
	pfsfld->nFileCnt = 0;
	pfsfld->nMP4FileCnt = 0;
	pfsfld->nOutputCnt = 0;
	pfsfld->nSumItemname = 0;
	pfsfld->nSumFilesize = 0;
	pfsfld->nPathSize = 0;
}

static UINT webls_fsfld_init_check(char *path, FSFLData *pfsfld, UINT *reflag)
{
	FILINFO finfo;
	BYTE res;
	DIR dir;

	if (f_opendir(&dir, path) != FR_OK) {
		return ENX_FAIL;
	}

	UINT nTotalDirCnt = 0;
	UINT nTotalFileCnt = 0;
	UINT nTotalFileSize = 0; // overflow 될 것이지만, 값의 정확성보다, 비교를 위한것이므로 의미있다
	UINT selfname = 1;

	// file/dir count
	while (1) {
		res = f_readdir(&dir, &finfo);
		if ((res != FR_OK) || !finfo.fname[0]) {
			f_closedir(&dir);
			break;
		}
		if (selfname && strcmp(finfo.fname, ".") == 0) {
			selfname = 0;
			continue;
		}

		if (!(finfo.fattrib & AM_SYS && finfo.fattrib & AM_HID)) {
			if (finfo.fattrib & AM_ARC) {
				nTotalFileCnt++;
				nTotalFileSize += finfo.fsize;
			} else if (finfo.fattrib & AM_DIR) {
				nTotalDirCnt++;
			}
		}
	}

	*reflag = 0;
	if (pfsfld->strPath) {
		if (strcmp(pfsfld->strPath, path) == 0) {
			if ((pfsfld->nPathSize == nTotalFileSize) && (pfsfld->nTotalDirCnt == nTotalDirCnt) && (pfsfld->nTotalFileCnt == nTotalFileCnt)) {
				*reflag = 1;
			}
		}
	}

	if (*reflag == 0) {
		webls_fsfld_free(pfsfld);

		UINT nPathlen = strlen(path);

		pfsfld->nTotalDirCnt = nTotalDirCnt;
		pfsfld->nTotalFileCnt = nTotalFileCnt;
		pfsfld->nTotalItemCnt = nTotalDirCnt + nTotalFileCnt;

		pfsfld->nDirCnt = 0;
		pfsfld->nFileCnt = 0;
		pfsfld->nMP4FileCnt = 0;
		pfsfld->nOutputCnt = 0;

		pfsfld->nSumItemname = 0;
		pfsfld->nSumFilesize = 0;

		pfsfld->strPath = (char *)pvPortCalloc(nPathlen + 1, sizeof(char));
		if (pfsfld->strPath) {
			strlcpy(pfsfld->strPath, path, nPathlen + 1);
		}
		pfsfld->nPathSize = nTotalFileSize;

		// file의 갯수만큼 FSFILELIST구조체를 malloc 한다.
		pfsfld->flItem = pvPortCalloc(pfsfld->nTotalItemCnt, sizeof(FSFILELIST));
		if (pfsfld->flItem == NULL) {
			tprintf(ENX_WEBLS_DGB_ERROR, "malloc fail : %s, %u, %lubyte\n", path, pfsfld->nTotalItemCnt, pfsfld->nTotalItemCnt * sizeof(FSFILELIST));
			return ENX_FAIL;
		}
	}

	return ENX_OK;
}

static UINT webls_get_fileinfo(char *path, FSFLData *pfsfld)
{
	FILINFO finfo;
	BYTE res;
	DIR dir;
	UINT iI = 0;

	if (pfsfld->nTotalItemCnt == 0) {
		return ENX_FAIL;
	}

	if (f_opendir(&dir, path) != FR_OK) {
		return ENX_FAIL;
	}

	pfsfld->nSumItemname = 0;
	pfsfld->nSumFilesize = 0;
	UINT selfname = 1;

	while (1) {
		res = f_readdir(&dir, &finfo);
		if ((res != FR_OK) || !finfo.fname[0]) {
			f_closedir(&dir);
			break;
		}
		if (selfname && (strcmp(finfo.fname, ".") == 0)) {
			selfname = 0;
			continue;
		}

		if (!(finfo.fattrib & AM_SYS && finfo.fattrib & AM_HID)) {
			if (finfo.fattrib & AM_ARC || finfo.fattrib & AM_DIR) {
				if (pfsfld->nTotalItemCnt > iI) {
					char *fn = finfo.fname;
					int name_len = strlen(fn);
					int copy_len = 0;
					if (name_len > SAVE_FILENAME_LENS) {
						pfsfld->flItem[iI].flname = pvPortCalloc(name_len + 1, sizeof(char));
						if (pfsfld->flItem[iI].flname) {
							copy_len = strlcpy(pfsfld->flItem[iI].flname, fn, name_len + 1);
						} else {
							copy_len = strlcpy(pfsfld->flItem[iI].fname, fn, SAVE_FILENAME_LENS + 1);
						}
					} else {
						pfsfld->flItem[iI].flname = NULL;
						copy_len = strlcpy(pfsfld->flItem[iI].fname, fn, SAVE_FILENAME_LENS + 1);
					}

					pfsfld->flItem[iI].fattrib = finfo.fattrib;
					pfsfld->flItem[iI].extension = 0;
					pfsfld->flItem[iI].fdate = finfo.fdate;
					pfsfld->flItem[iI].ftime = finfo.ftime;
					pfsfld->flItem[iI].fsize = finfo.fsize;

					if (pfsfld->flItem[iI].fattrib & AM_ARC) {
						pfsfld->nSumFilesize += webls_number_count(pfsfld->flItem[iI].fsize);
						if (strstr(finfo.altname, ".MP4")) {
							pfsfld->flItem[iI].extension = 1;
							pfsfld->nSumItemname += (copy_len * 3);
						} else {
							pfsfld->nSumItemname += (copy_len * 2);
						}
					} else {
						pfsfld->nSumItemname += (copy_len * 2);
					}

					iI++;
				}
			}
		}
	}

	return ENX_OK;
}

static UINT webls_get_this_page_length(FSFLData *pfsfld, int lspage)
{
	UINT i;
	UINT idx_start = (lspage - 1) * LSHTML_FILES_PER_PAGE;
	UINT idx_end = idx_start + LSHTML_FILES_PER_PAGE;
	idx_end = idx_end < pfsfld->nTotalItemCnt ? idx_end : pfsfld->nTotalItemCnt;

	pfsfld->nDirCnt = 0;
	pfsfld->nFileCnt = 0;
	pfsfld->nMP4FileCnt = 0;
	pfsfld->nOutputCnt = 0;

	pfsfld->nSumItemname = 0;
	pfsfld->nSumFilesize = 0;

	for (i = idx_start; i < idx_end; i++) {
		char *fn = pfsfld->flItem[i].flname != NULL ? pfsfld->flItem[i].flname : pfsfld->flItem[i].fname;
		int name_len = strlen(fn);
		if (pfsfld->flItem[i].fattrib & AM_ARC) {
			if (pfsfld->flItem[i].extension == 1) {
				pfsfld->nMP4FileCnt++;
				pfsfld->nSumItemname += (name_len * 3);
			} else {
				pfsfld->nFileCnt++;
				pfsfld->nSumItemname += (name_len * 2);
			}
			pfsfld->nSumFilesize += webls_number_count(pfsfld->flItem[i].fsize);
		} else if (pfsfld->flItem[i].fattrib & AM_DIR) {
			pfsfld->nDirCnt++;
			pfsfld->nSumItemname += (name_len * 2);
		}
	}
	pfsfld->nOutputCnt = pfsfld->nFileCnt + pfsfld->nDirCnt;

	return ENX_OK;
}

static int webls_compar(const void *a, const void *b)
{
	FSFILELIST *hfla = (FSFILELIST *)a;
	FSFILELIST *hflb = (FSFILELIST *)b;

	if (hfla->fattrib != hflb->fattrib) {
		if (hfla->fattrib & AM_DIR) 		return 0;
		else								return 1;
	}

	char *aname;
	if (hfla->flname) {
		aname = hfla->flname;
	} else {
		aname = hfla->fname;
	}

	char *bname;
	if (hflb->flname) {
		bname = hflb->flname;
	} else {
		bname = hflb->fname;
	}

	// The topmost (..)
	if (hfla->fattrib & AM_DIR) {
		if (strcmp(aname, "..") == 0)		return 0;
		else if (strcmp(bname, "..") == 0)	return 1;
	}

#ifdef LSHTML_COMPAR_DATATIME
	UINT dta = ((UINT)hfla->fdate) << 16 | hfla->ftime;
	UINT dtb = ((UINT)hflb->fdate) << 16 | hflb->ftime;
//	(dta <= dtb) : new ... old
//	(dta >  dtb) : old ... new
	return dta <= dtb;
#else
//	strcmp(aname, bname) : A B C ... X Y Z
//	strcmp(bname, aname) : Z Y X ... C B A
	return strcmp(aname, bname);
#endif
}

// ls.html Header
#define LSHTMLhead			"<!DOCTYPE html><html><head><title></title><link rel=\"stylesheet\" type=\"text/css\" href=\"/bootstrap.min.css\"><link rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\"><style>td {text-align: center;vertical-align: middle;}</style><script type=\"text/javascript\">function play(path){var popOption = \"width=660, height=400, resizable=no\";PV = window.open(\"\",\"Player\",popOption);var frameString= \"<!DOCTYPE html><html><head><title>Player</title></head><body><video width=\\\"640\\\" height=\\\"360\\\" controls autoplay><source src=\"+path+\" type=\\\"video/mp4\\\">Your browser does not support HTML5 video.</video></body></html>\";PV.document.open();PV.document.write(frameString);PV.document.close();}</script></head><body><div class=\"sub-title\"><h3>File List</h3>When you access this page, recording stops.<br \\>You can resume recording in the menu SDcard &gt; SD Setup.</div><hr>"
#define LSHTMLhead_page1	"<table style=\"width: 600px;\"><tr><td style=\"text-align: center;\"><script type=\"text/javascript\">"
#define LSHTMLhead_pageinfo "page=%d;maxp=%d;"
#define LSHTMLhead_page2	"for(i=1;i<=maxp;i++){if(i==page){document.write(\"[\"+i+\"] \");}else{document.write(\"<a href=\\\"ls.html?page=\"+i+\"\\\">\"+i+\"</a> \");}}</script></tr></td></table><br \\>"
#define LSHTMLhead_table	"<table class=\"table table-bordered\" style=\"width: 600px;\"><colgroup><col style=\"width: 60px;\"><col style=\"width: 200px;\"><col style=\"width: 100px;\"><col style=\"width: 180px;\"><col style=\"width: 60px;\"></colgroup><tr><td>Type</td><td>Name</td><td>Size</td><td>Datetime</td><td>Play</td></tr>"

// ls.html Footer
#define LSHTMLfoot			"</table></body></html>"

// ls.html Body Dir type (89 + (name_length * 2))byte
#define LSHTMLbodyD			"<tr><td>D</td><td><a href=\"%s/"SD_DIR_LS_HTML"\">%s</a></td><td>-</td><td>%04u/%02u/%02u %02u:%02u:%02u</td><td></td></tr>"
#define LSHTMLbodyD_size	(89+9)

// ls.html Body File type (80 + (name_length * 2) + size_length)byte
#define LSHTMLbodyA1		"<tr><td>A</td><td><a href=\"%s\">%s</a></td><td>%u</td><td>%04u/%02u/%02u %02u:%02u:%02u</td><td></td></tr>"
#define LSHTMLbodyA1_size	(80+9)

// ls.html Body File type (131 + (name_length * 3) + size_length)byte
#define LSHTMLbodyA2		"<tr><td>A</td><td><a href=\"%s\">%s</a></td><td>%u</td><td>%04u/%02u/%02u %02u:%02u:%02u</td><td><a href=\"javascript:play('%s')\">&#x25B6;</a></td></tr>"
#define LSHTMLbodyA2_size	(80+17+34)

// ls.html SDcard is empty
#define LSHTMLbodyNoSD		"<tr><td colspan=\"5\">SDcard is empty!</td></tr>"
#define LSHTMLbodyNoSD_size	46

char *enx_webls_get_html(char *path, UINT *allocsize)
{
	char strbuffer[512];
	UINT i, nRes, reflag = 0;

	// 해당 위치에서 file의 갯수를 확인한다.
	nRes = webls_fsfld_init_check(path, &fsfld_html, &reflag);
	if (nRes == ENX_FAIL) {
		tprintf(ENX_WEBLS_DGB_ERROR, "dir open fail : %s\n", path);
		gWebls_page = 1; // page reset
		webls_fsfld_free(&fsfld_html);
		return NULL;
	}

	if (reflag == 0) {
		// 구조체에 모든 정보를 넣는다.
		nRes = webls_get_fileinfo(path, &fsfld_html);
		if (nRes == ENX_FAIL) {
			tprintf(ENX_WEBLS_DGB_ERROR, "get fileinfo fail : %s\n", path);
			gWebls_page = 1; // page reset
			webls_fsfld_free(&fsfld_html);
			return NULL;
		}

		// 정렬기준은 webls_compar함수의 기준으로 한다.
		qsort((char *)fsfld_html.flItem, fsfld_html.nTotalItemCnt, sizeof(FSFILELIST), webls_compar);
	}

	// 해당 page에 해당하는 file name의 문자열길이의 total length와 file size의 문자열길이의 total length을 구한다.
	webls_get_this_page_length(&fsfld_html, gWebls_page);

	// page 개수를 구해온다.
	int page_num = (fsfld_html.nTotalItemCnt + (LSHTML_FILES_PER_PAGE - 1)) / LSHTML_FILES_PER_PAGE;
	int page_len = sprintf(strbuffer, LSHTMLhead_pageinfo, gWebls_page, page_num);

	// html 형태로 만들어준다. html의 총 길이는 contents_len. contents_len이 realdata_len와 동일해야 한다.
	UINT contents_len = (sizeof(LSHTMLhead) - 1) + (sizeof(LSHTMLhead_page1) - 1) + (sizeof(LSHTMLhead_page2) - 1) + (sizeof(LSHTMLhead_table) - 1) + (sizeof(LSHTMLfoot) - 1);
	contents_len += (LSHTMLbodyA1_size * fsfld_html.nFileCnt);
	contents_len += (LSHTMLbodyA2_size * fsfld_html.nMP4FileCnt);
	contents_len += (LSHTMLbodyD_size * fsfld_html.nDirCnt);
	contents_len += fsfld_html.nSumItemname + fsfld_html.nSumFilesize + page_len;
	char *data = (char *)mem_calloc(contents_len, sizeof(char));
	if (data == NULL) {
		tprintf(ENX_WEBLS_DGB_ERROR, "malloc fail : %s, %lubyte\n", path, contents_len * sizeof(char));
		gWebls_page = 1; // page reset
		webls_fsfld_free(&fsfld_html);
		return NULL;
	}
	UINT realdata_len = 0, oneitem_len = 0;
	char *pdata = data;

	// Add HTML Header
	realdata_len = strlcpy(pdata, LSHTMLhead, contents_len);
	pdata += realdata_len;

	pdata = enx_strcat(pdata, LSHTMLhead_page1);
	realdata_len += (sizeof(LSHTMLhead_page1) - 1);

	oneitem_len = page_len;
	pdata = enx_strcat(pdata, strbuffer);
	realdata_len += oneitem_len;

	pdata = enx_strcat(pdata, LSHTMLhead_page2);
	realdata_len += (sizeof(LSHTMLhead_page2) - 1);

	pdata = enx_strcat(pdata, LSHTMLhead_table);
	realdata_len += (sizeof(LSHTMLhead_table) - 1);

	// Add HTML Body
	UINT idx_start = (gWebls_page - 1) * LSHTML_FILES_PER_PAGE;
	UINT idx_end = idx_start + LSHTML_FILES_PER_PAGE;
	idx_end = idx_end < fsfld_html.nTotalItemCnt ? idx_end : fsfld_html.nTotalItemCnt;
	for (i = idx_start; i < idx_end; i++) {
		char *file_name = fsfld_html.flItem[i].flname != NULL ? fsfld_html.flItem[i].flname : fsfld_html.flItem[i].fname;
		if (fsfld_html.flItem[i].fattrib & AM_DIR) {
			oneitem_len = sprintf(pdata, LSHTMLbodyD, file_name, file_name, 
				(fsfld_html.flItem[i].fdate >> 9) + 1980, (fsfld_html.flItem[i].fdate >> 5) & 0xf, (fsfld_html.flItem[i].fdate & 0x1f),
				(fsfld_html.flItem[i].ftime >> 11), (fsfld_html.flItem[i].ftime >> 5) & 0x3f, (fsfld_html.flItem[i].ftime & 0x1f) * 2);
			pdata += oneitem_len;
			realdata_len += oneitem_len;
		} else if (fsfld_html.flItem[i].fattrib & AM_ARC) {
			if (fsfld_html.flItem[i].extension == 1) {
				oneitem_len = sprintf(pdata, LSHTMLbodyA2, file_name, file_name, fsfld_html.flItem[i].fsize, 
					(fsfld_html.flItem[i].fdate >> 9) + 1980, (fsfld_html.flItem[i].fdate >> 5) & 0xf, (fsfld_html.flItem[i].fdate & 0x1f),
					(fsfld_html.flItem[i].ftime >> 11), (fsfld_html.flItem[i].ftime >> 5) & 0x3f, (fsfld_html.flItem[i].ftime & 0x1f) * 2,
					file_name);
			} else {
				oneitem_len = sprintf(pdata, LSHTMLbodyA1, file_name, file_name, fsfld_html.flItem[i].fsize, 
					(fsfld_html.flItem[i].fdate >> 9) + 1980, (fsfld_html.flItem[i].fdate >> 5) & 0xf, (fsfld_html.flItem[i].fdate & 0x1f),
					(fsfld_html.flItem[i].ftime >> 11), (fsfld_html.flItem[i].ftime >> 5) & 0x3f, (fsfld_html.flItem[i].ftime & 0x1f) * 2);
			}
			pdata += oneitem_len;
			realdata_len += oneitem_len;
		}
	}

	// Add HTML Footer
	pdata = enx_strcat(pdata, LSHTMLfoot);
	realdata_len += (sizeof(LSHTMLfoot) - 1);

	*allocsize = realdata_len;

	// Page reset
	gWebls_page = 1;

	if (contents_len != realdata_len) {
		tprintf(ENX_WEBLS_DGB_ERROR, "contents_len(%u) != realdata_len(%u)\n", contents_len, realdata_len);
	}

	// data는 fs_close_custom함수에서 free한다.
	return data;
}

char *enx_webls_get_html_sdcheck(UINT *allocsize)
{
	if (getSDState() != sd_OFF) {
		return NULL;
	}

	// 이것을 html 형태로 만들어준다. html의 총 길이는 contents_len. contents_len이 realdata_len와 동일해야 한다.
	UINT contents_len = (sizeof(LSHTMLhead) - 1) + (sizeof(LSHTMLfoot) - 1) + (sizeof(LSHTMLbodyNoSD) - 1);
	char *data = (char *)mem_calloc(contents_len, sizeof(char));
	if (data == NULL) {
		tprintf(ENX_WEBLS_DGB_ERROR, "malloc fail : %lubyte\n", contents_len * sizeof(char));
		return NULL;
	}
	char *pdata = data;

	// Add HTML Header
	UINT realdata_len = strlcpy(pdata, LSHTMLhead, contents_len);
	pdata += realdata_len;

	// Add HTML Body(SDcard is empty)
	pdata = enx_strcat(pdata, LSHTMLbodyNoSD);
	realdata_len += (sizeof(LSHTMLbodyNoSD) - 1);

	// Add HTML Footer
	pdata = enx_strcat(pdata, LSHTMLfoot);
	realdata_len += (sizeof(LSHTMLfoot) - 1);

	*allocsize = realdata_len;

	if (contents_len != realdata_len) {
		tprintf(ENX_WEBLS_DGB_ERROR, "contents_len(%u) != realdata_len(%u)\n", contents_len, realdata_len);
	}

	// data는 fs_close_custom함수에서 free한다.
	return data;
}

// ls.xml Header
#define LSXMLhead			"<?xml version=\"1.0\" encoding=\"utf-8\" ?><EYENIX>"

// ls.xml Footer
#define LSXMLfoot			"</EYENIX>"

// ls.xml Body Dir type (56 + name_length)byte
#define LSXMLbodyD			"<F><T>D</T><N>%s</N><S>-</S><D>%04u/%02u/%02u %02u:%02u:%02u</D></F>"
#define LSXMLbodyD_size		(56)

// ls.xml Body File type (55 + name_length + size_length)byte
#define LSXMLbodyA			"<F><T>A</T><N>%s</N><S>%u</S><D>%04u/%02u/%02u %02u:%02u:%02u</D></F>"
#define LSXMLbodyA_size		(55)

// ls.xml SDcard is empty
#define LSXMLbodyNoSD		"<E>SDcard is empty!</E>"
#define LSXMLbodyNoSD_size	(23)

char *enx_webls_get_xml(char *path, UINT *allocsize)
{
	FSFLData fsfld;
	UINT i, nRes, reflag = 0;

	memset((void *)&fsfld, 0, sizeof(FSFLData));

	// 해당 위치에서 file의 갯수를 확인한다.
	nRes = webls_fsfld_init_check(path, &fsfld, &reflag);
	if (nRes == ENX_FAIL) {
		tprintf(ENX_WEBLS_DGB_ERROR, "dir open fail : %s\n", path);
		webls_fsfld_free(&fsfld);
		return NULL;
	}

	// 구조체에 모든 정보를 넣는다.
	nRes = webls_get_fileinfo(path, &fsfld);
	if (nRes == ENX_FAIL) {
		tprintf(ENX_WEBLS_DGB_ERROR, "get fileinfo fail : %s\n", path);
		webls_fsfld_free(&fsfld);
		return NULL;
	}

	// 이것을 xml 형태로 만들어준다. xml의 총 길이는 contents_len. contents_len가 realdatalen와 동일해야 한다.
	UINT contents_len = (sizeof(LSXMLhead) - 1) + (sizeof(LSXMLfoot) - 1);
	contents_len += (LSXMLbodyA_size * fsfld.nTotalFileCnt);
	contents_len += (LSXMLbodyD_size * fsfld.nTotalDirCnt);
	contents_len += fsfld.nSumItemname + fsfld.nSumFilesize;
	char *data = (char *)mem_calloc(contents_len, sizeof(char));
	if (data == NULL) {
		tprintf(ENX_WEBLS_DGB_ERROR, "malloc fail : %s, %lubyte\n", path, contents_len * sizeof(char));
		webls_fsfld_free(&fsfld);
		return NULL;
	}
	UINT realdata_len = 0, oneitem_len = 0;
	char *pdata = data;

	// 정렬기준은 webls_compar함수의 기준으로 한다.
	qsort((char *)fsfld.flItem, fsfld.nTotalItemCnt, sizeof(FSFILELIST), webls_compar);

	// Add XML Header
	realdata_len = strlcpy(pdata, LSXMLhead, contents_len);
	pdata += realdata_len;

	// Add XML Body
	for (i = 0; i < fsfld.nTotalItemCnt; i++) {
		char *file_name = fsfld.flItem[i].flname != NULL ? fsfld.flItem[i].flname : fsfld.flItem[i].fname;
		if (fsfld.flItem[i].fattrib & AM_DIR) {
			oneitem_len = sprintf(pdata, LSXMLbodyD, file_name, 
				(fsfld.flItem[i].fdate >> 9) + 1980, (fsfld.flItem[i].fdate >> 5) & 0xf, (fsfld.flItem[i].fdate & 0x1f),
				(fsfld.flItem[i].ftime >> 11), (fsfld.flItem[i].ftime >> 5) & 0x3f, (fsfld.flItem[i].ftime & 0x1f) * 2);
			pdata += oneitem_len;
			realdata_len += oneitem_len;
		} else if (fsfld.flItem[i].fattrib & AM_ARC) {
			oneitem_len = sprintf(pdata, LSXMLbodyA, file_name, fsfld.flItem[i].fsize, 
				(fsfld.flItem[i].fdate >> 9) + 1980, (fsfld.flItem[i].fdate >> 5) & 0xf, (fsfld.flItem[i].fdate & 0x1f),
				(fsfld.flItem[i].ftime >> 11), (fsfld.flItem[i].ftime >> 5) & 0x3f, (fsfld.flItem[i].ftime & 0x1f) * 2);
			pdata += oneitem_len;
			realdata_len += oneitem_len;
		}
	}

	// Add XML Footer
	pdata = enx_strcat(pdata, LSXMLfoot);
	realdata_len += (sizeof(LSXMLfoot) - 1);

	*allocsize = realdata_len;

	// Memory free(FSFILELIST)
	webls_fsfld_free(&fsfld);

	if (contents_len != realdata_len) {
		tprintf(ENX_WEBLS_DGB_ERROR, "contents_len(%u) != realdata_len(%u)\n", contents_len, realdata_len);
	}

	// data는 fs_close_custom함수에서 free한다.
	return data;
}

char *enx_webls_get_xml_sdcheck(UINT *allocsize)
{
	if (getSDState() != sd_OFF) {
		return NULL;
	}

	// 이것을 html 형태로 만들어준다. html의 총 길이는 contents_len. contents_len이 realdata_len와 동일해야 한다.
	UINT contents_len = (sizeof(LSXMLhead) - 1) + (sizeof(LSXMLfoot) - 1) + (sizeof(LSXMLbodyNoSD) - 1);
	char *data = (char *)mem_calloc(contents_len, sizeof(char));
	if (data == NULL) {
		tprintf(ENX_WEBLS_DGB_ERROR, "malloc fail : %lubyte\n", contents_len * sizeof(char));
		return NULL;
	}
	char *pdata = data;

	// Add HTML Header
	UINT realdata_len = strlcpy(pdata, LSXMLhead, contents_len);
	pdata += realdata_len;

	// Add HTML Body(SDcard is empty)
	pdata = enx_strcat(pdata, LSXMLbodyNoSD);
	realdata_len += (sizeof(LSXMLbodyNoSD) - 1);

	// Add HTML Footer
	pdata = enx_strcat(pdata, LSXMLfoot);
	realdata_len += (sizeof(LSXMLfoot) - 1);

	*allocsize = realdata_len;

	if (contents_len != realdata_len) {
		tprintf(ENX_WEBLS_DGB_ERROR, "contents_len(%u) != realdata_len(%u)\n", contents_len, realdata_len);
	}

	// data는 fs_close_custom함수에서 free한다.
	return data;
}
#endif
#endif
#endif
