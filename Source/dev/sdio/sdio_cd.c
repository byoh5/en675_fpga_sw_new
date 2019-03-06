#include "dev.h"
#include "sdio_cd.h"
#include "enx_freertos.h"

#include <string.h> // memset

static SDIO_SD sdinfo;

#define ENX_SDIOCD_CMD_DEBUG
#define ENX_SDIOCD_DAT_DEBUG
#define ENX_SDIOCD_ERR_DEBUG

#define ENX_SDIOCD_DELAY vTaskDelay
#define DELAY_MS(ms) ENX_SDIOCD_DELAY(ms)

#ifdef ENX_SDIOCD_CMD_DEBUG
	#define gprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
	#define SDIO_CMD_LOG_START	{	printf("(%04d)%s : START\n", __LINE__, __func__);	}
	#define SDIO_CMD_LOG_END	{	printf("(%04d)%s : E N D\n", __LINE__, __func__);	}
#else
	#define gprintf(fmt, args...) do {} while(0);
	#define SDIO_CMD_LOG_START	{ }
	#define SDIO_CMD_LOG_END	{ }
#endif

#ifdef ENX_SDIOCD_DAT_DEBUG
	#define tprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
	#define SDIO_DAT_LOG_START	{	printf("(%04d)%s : START\n", __LINE__, __func__);	}
	#define SDIO_DAT_LOG_END	{	printf("(%04d)%s : E N D\n", __LINE__, __func__);	}
#else
	#define tprintf(fmt, args...) do {} while(0);
	#define SDIO_DAT_LOG_START	{ }
	#define SDIO_DAT_LOG_END	{ }
#endif

#ifdef ENX_SDIOCD_ERR_DEBUG
	#define eprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define eprintf(fmt, args...) do {} while(0);
#endif

//-------------------------------------------------------------------------------------------------
// CMD8
#define CMD8_VHS			0x00000100		// [11: 8] : 2.7-3.6V
#define CMD8_PATTERN		0x000000aa		// [ 7: 0] : Just "0xaa"

// ACMD41
#define	ACMD41_HCS			0x40000000		// [   30] : SDHC or SDXC Supported
#define ACMD41_XPC			0x10000000		// [   28] : SDXC Maximum Performance
#define ACMD41_S18R			0x01000000		// [   24] : Switching to 1.8V Request
#define	ACMD41_VOLT			0x00ff8000		// [23:15] : Voltage window OCR

// OCR register
#define OCR_POWUP			0x80000000		// Power up procedure end
#define OCR_CCS				0x40000000		// Card Capacity Status(0:SDSC, 1:SDHCorSDXC)
#define OCR_UHSII			0x20000000		// UHS-II Card Status(0:Non, 1:UHS-II)
#define OCR_S18A			0x01000000		// Switching to 1.8V Accepted
#define OCR_VOLT			0x00ff8000		// Voltage window

// SCR register
#define SCR_SPEC_VER_0		0	/* Implements system specification 1.0 - 1.01 */
#define SCR_SPEC_VER_1		1	/* Implements system specification 1.10 */
#define SCR_SPEC_VER_2		2	/* Implements system specification 2.00 */

#define SCR_CCC_SWITCH		0x00000400		// [  :10] : High speed switch

// Status Mask
#define RCA_RCA_MASK		0xffff0000

// Sd power up retry count
#define	SD_TRY_CNT			100

//-------------------------------------------------------------------------------------------------
// SDIO-CD Card Reg
void SdioPrintOCR(SD_OCR *ocr)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\n");
	printf("ocr->busy     [   31] : %u(0x%08X)\n", ocr->busy, ocr->busy);
	printf("ocr->CCS      [   30] : %u(0x%08X)\n", ocr->CCS, ocr->CCS);
	printf("ocr->UHSIICS  [   29] : %u(0x%08X)\n", ocr->UHSIICS, ocr->UHSIICS);
	printf("ocr->_res1    [28:25] : %u(0x%08X)\n", ocr->_res1, ocr->_res1);
	printf("ocr->VDD18    [   24] : %u(0x%08X)\n", ocr->VDD18, ocr->VDD18);
	printf("ocr->VDD35_36 [   23] : %u(0x%08X)\n", ocr->VDD35_36, ocr->VDD35_36);
	printf("ocr->VDD34_35 [   22] : %u(0x%08X)\n", ocr->VDD34_35, ocr->VDD34_35);
	printf("ocr->VDD33_34 [   21] : %u(0x%08X)\n", ocr->VDD33_34, ocr->VDD33_34);
	printf("ocr->VDD32_33 [   20] : %u(0x%08X)\n", ocr->VDD32_33, ocr->VDD32_33);
	printf("ocr->VDD31_32 [   19] : %u(0x%08X)\n", ocr->VDD31_32, ocr->VDD31_32);
	printf("ocr->VDD30_31 [   18] : %u(0x%08X)\n", ocr->VDD30_31, ocr->VDD30_31);
	printf("ocr->VDD29_30 [   17] : %u(0x%08X)\n", ocr->VDD29_30, ocr->VDD29_30);
	printf("ocr->VDD28_29 [   16] : %u(0x%08X)\n", ocr->VDD28_29, ocr->VDD28_29);
	printf("ocr->VDD27_28 [   15] : %u(0x%08X)\n", ocr->VDD27_28, ocr->VDD27_28);
	printf("ocr->_res2    [14: 8] : %u(0x%08X)\n", ocr->_res2, ocr->_res2);
	printf("ocr->VDDLOW   [    7] : %u(0x%08X)\n", ocr->VDDLOW, ocr->VDDLOW);
	printf("ocr->_res3    [ 6: 0] : %u(0x%08X)\n", ocr->_res3, ocr->_res3);
	printf("===================================================\n");

	SDIO_CMD_LOG_END;
}

void SdioPrintCID(SD_CID *cid)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\n");
	printf("cid->MID    [127:120] : %u(0x%08X)\n", cid->MID, cid->MID);
	printf("cid->OID    [119:104] : %u(0x%08X)\n", cid->OID, cid->OID);
	printf("cid->PNM1   [103: 96] : %u(0x%08X) : %c\n", cid->PNM1, cid->PNM1, cid->PNM1);
	printf("cid->PNM2     [95:88] : %u(0x%08X) : %c\n", cid->PNM2, cid->PNM2, cid->PNM2);
	printf("cid->PNM3     [87:80] : %u(0x%08X) : %c\n", cid->PNM3, cid->PNM3, cid->PNM3);
	printf("cid->PNM4     [79:72] : %u(0x%08X) : %c\n", cid->PNM4, cid->PNM4, cid->PNM4);
	printf("cid->PNM5     [71:64] : %u(0x%08X) : %c\n", cid->PNM5, cid->PNM5, cid->PNM5);
	printf("cid->PRV      [63:56] : %u(0x%08X)\n", cid->PRV, cid->PRV);
	printf("cid->PSN      [55:24] : %u(0x%08X)\n", cid->PSN0 << 8 | cid->PSN1, cid->PSN0 << 8 | cid->PSN1);
	printf("cid->_res1    [23:20] : %u(0x%08X)\n", cid->_res1, cid->_res1);
	printf("cid->MDT      [19: 8] : %u(0x%08X)\n", cid->MDT, cid->MDT);
	printf("cid->CRC      [ 7: 1] : %u(0x%08X)\n", cid->CRC, cid->CRC);
	printf("cid->_res2    [    0] : %u(0x%08X)\n", cid->_res2, cid->_res2);
	printf("===================================================\n");

	SDIO_CMD_LOG_END;
}

void SdioPrintCSD(SD_CSD *csd)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\n");
	if (csd->csd_v1.CSD_STRUCTURE == 0) { // CSD Version 1.0
		printf("csd->CSD_STRUCTURE      [127:126] : %u(0x%08X)\n", csd->csd_v1.CSD_STRUCTURE, csd->csd_v1.CSD_STRUCTURE);
	} else if (csd->csd_v1.CSD_STRUCTURE == 1) { // CSD Version 2.0
		printf("csd->CSD_STRUCTURE      [127:126] : %u(0x%08X)\n", csd->csd_v2.CSD_STRUCTURE, csd->csd_v2.CSD_STRUCTURE);
		printf("csd->_res1              [125:120] : %u(0x%08X)\n", csd->csd_v2._res1, csd->csd_v2._res1);
		printf("csd->TAAC               [119:112] : %u(0x%08X)\n", csd->csd_v2.TAAC, csd->csd_v2.TAAC);
		printf("csd->NSAC               [111:104] : %u(0x%08X)\n", csd->csd_v2.NSAC, csd->csd_v2.NSAC);
		printf("csd->TRAN_SPEED         [103: 96] : %u(0x%08X)\n", csd->csd_v2.TRAN_SPEED, csd->csd_v2.TRAN_SPEED);
		printf("csd->CCC                [ 95: 84] : %u(0x%08X)\n", csd->csd_v2.CCC, csd->csd_v2.CCC);
		printf("csd->READ_BL_LEN        [ 83: 80] : %u(0x%08X)\n", csd->csd_v2.READ_BL_LEN, csd->csd_v2.READ_BL_LEN);
		printf("csd->READ_BL_PARTIAL    [ 79: 79] : %u(0x%08X)\n", csd->csd_v2.READ_BL_PARTIAL, csd->csd_v2.READ_BL_PARTIAL);
		printf("csd->WRITE_BLK_MISALIGN [ 78: 78] : %u(0x%08X)\n", csd->csd_v2.WRITE_BLK_MISALIGN, csd->csd_v2.WRITE_BLK_MISALIGN);
		printf("csd->READ_BLK_MISALIGN  [ 77: 77] : %u(0x%08X)\n", csd->csd_v2.READ_BLK_MISALIGN, csd->csd_v2.READ_BLK_MISALIGN);
		printf("csd->DSR_IMP            [ 76: 76] : %u(0x%08X)\n", csd->csd_v2.DSR_IMP, csd->csd_v2.DSR_IMP);
		printf("csd->_res2              [ 75: 70] : %u(0x%08X)\n", csd->csd_v2._res2, csd->csd_v2._res2);
		printf("csd->C_SIZE             [ 69: 48] : %u(0x%08X)\n", csd->csd_v2.C_SIZE0 << 6 | csd->csd_v2.C_SIZE1, csd->csd_v2.C_SIZE0 << 6 | csd->csd_v2.C_SIZE1);
		printf("csd->_res3              [ 47: 47] : %u(0x%08X)\n", csd->csd_v2._res3, csd->csd_v2._res3);
		printf("csd->ERASE_BLK_EN       [ 46: 46] : %u(0x%08X)\n", csd->csd_v2.ERASE_BLK_EN, csd->csd_v2.ERASE_BLK_EN);
		printf("csd->SECTOR_SIZE        [ 45: 39] : %u(0x%08X)\n", csd->csd_v2.SECTOR_SIZE, csd->csd_v2.SECTOR_SIZE);
		printf("csd->WP_GRP_SIZE        [ 38: 32] : %u(0x%08X)\n", csd->csd_v2.WP_GRP_SIZE, csd->csd_v2.WP_GRP_SIZE);
		printf("csd->WP_GRP_ENABLE      [ 31: 31] : %u(0x%08X)\n", csd->csd_v2.WP_GRP_ENABLE, csd->csd_v2.WP_GRP_ENABLE);
		printf("csd->_res4              [ 30: 29] : %u(0x%08X)\n", csd->csd_v2._res4, csd->csd_v2._res4);
		printf("csd->R2W_FACTOR         [ 28: 26] : %u(0x%08X)\n", csd->csd_v2.R2W_FACTOR, csd->csd_v2.R2W_FACTOR);
		printf("csd->WRITE_BL_LEN       [ 25: 22] : %u(0x%08X)\n", csd->csd_v2.WRITE_BL_LEN, csd->csd_v2.WRITE_BL_LEN);
		printf("csd->WRITE_BL_PARTIAL   [ 21: 19] : %u(0x%08X)\n", csd->csd_v2.WRITE_BL_PARTIAL, csd->csd_v2.WRITE_BL_PARTIAL);
		printf("csd->_res5              [ 20: 16] : %u(0x%08X)\n", csd->csd_v2._res5, csd->csd_v2._res5);
		printf("csd->FILE_FORMAT_GRP    [ 15: 15] : %u(0x%08X)\n", csd->csd_v2.FILE_FORMAT_GRP, csd->csd_v2.FILE_FORMAT_GRP);
		printf("csd->COPY               [ 14: 14] : %u(0x%08X)\n", csd->csd_v2.COPY, csd->csd_v2.COPY);
		printf("csd->PERM_WRITE_PROTECT [ 13: 13] : %u(0x%08X)\n", csd->csd_v2.PERM_WRITE_PROTECT, csd->csd_v2.PERM_WRITE_PROTECT);
		printf("csd->TMP_WRITE_PROTECT  [ 12: 12] : %u(0x%08X)\n", csd->csd_v2.TMP_WRITE_PROTECT, csd->csd_v2.TMP_WRITE_PROTECT);
		printf("csd->FILE_FORMAT        [ 11: 10] : %u(0x%08X)\n", csd->csd_v2.FILE_FORMAT, csd->csd_v2.FILE_FORMAT);
		printf("csd->_res6              [  9:  8] : %u(0x%08X)\n", csd->csd_v2._res6, csd->csd_v2._res6);
		printf("csd->CRC                [  7:  1] : %u(0x%08X)\n", csd->csd_v2.CRC, csd->csd_v2.CRC);
		printf("csd->_res7              [  0:  0] : %u(0x%08X)\n", csd->csd_v2._res7, csd->csd_v2._res7);
	} else {
		printf("(%04d)%s : CSD Version Check Error\n", __LINE__, __func__);
	}
	printf("===================================================\n");

	SDIO_CMD_LOG_END;
}

void SdioPrintSSR(SD_SSR *ssr)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\n");
	printf("ssr->DAT_BUS_WIDTH          [511:510] : %u(0x%08X)\n", ssr->DAT_BUS_WIDTH, ssr->DAT_BUS_WIDTH);
	printf("ssr->SECURED_MODE           [509:509] : %u(0x%08X)\n", ssr->SECURED_MODE, ssr->SECURED_MODE);
	printf("ssr->RSF                    [508:502] : %u(0x%08X)\n", ssr->RSF, ssr->RSF);
	printf("ssr->_res1                  [501:496] : %u(0x%08X)\n", ssr->_res1, ssr->_res1);
	printf("ssr->SD_CARD_TYPE           [495:480] : %u(0x%08X)\n", ssr->SD_CARD_TYPE, ssr->SD_CARD_TYPE);
	printf("ssr->SIZE_OF_PROTECTED_AREA [479:448] : %u(0x%08X)\n", ssr->SIZE_OF_PROTECTED_AREA, ssr->SIZE_OF_PROTECTED_AREA);
	printf("ssr->SPEED_CLASS            [447:440] : %u(0x%08X)\n", ssr->SPEED_CLASS, ssr->SPEED_CLASS);
	printf("ssr->PERFORMANCE_MOVE       [439:432] : %u(0x%08X)\n", ssr->PERFORMANCE_MOVE, ssr->PERFORMANCE_MOVE);
	printf("ssr->AU_SIZE                [431:428] : %u(0x%08X)\n", ssr->AU_SIZE, ssr->AU_SIZE);
	printf("ssr->_res2                  [427:424] : %u(0x%08X)\n", ssr->_res2, ssr->_res2);
	printf("ssr->ERASE_SIZE             [423:408] : %u(0x%08X)\n", ssr->ERASE_SIZE, ssr->ERASE_SIZE);
	printf("ssr->ERASE_TIMEOUT          [407:402] : %u(0x%08X)\n", ssr->ERASE_TIMEOUT, ssr->ERASE_TIMEOUT);
	printf("ssr->ERASE_OFFSET           [401:400] : %u(0x%08X)\n", ssr->ERASE_OFFSET, ssr->ERASE_OFFSET);
	printf("ssr->UHS_SPEED_GRADE        [399:396] : %u(0x%08X)\n", ssr->UHS_SPEED_GRADE, ssr->UHS_SPEED_GRADE);
	printf("ssr->UHS_AU_SIZE            [395:392] : %u(0x%08X)\n", ssr->UHS_AU_SIZE, ssr->UHS_AU_SIZE);
	printf("ssr->_res3                  [391:384] : %u(0x%08X)\n", ssr->_res3, ssr->_res3);
	printf("ssr->_res4                  [383:352] : %u(0x%08X)\n", ssr->_res4, ssr->_res4);
	printf("ssr->_res5                  [351:320] : %u(0x%08X)\n", ssr->_res5, ssr->_res5);
	printf("ssr->_res6                  [319:288] : %u(0x%08X)\n", ssr->_res6, ssr->_res6);
	printf("ssr->_res7                  [287:256] : %u(0x%08X)\n", ssr->_res7, ssr->_res7);
	printf("ssr->_res8                  [255:224] : %u(0x%08X)\n", ssr->_res8, ssr->_res8);
	printf("ssr->_res9                  [223:192] : %u(0x%08X)\n", ssr->_res9, ssr->_res9);
	printf("ssr->_res10                 [191:160] : %u(0x%08X)\n", ssr->_res10, ssr->_res10);
	printf("ssr->_res11                 [159:128] : %u(0x%08X)\n", ssr->_res11, ssr->_res11);
	printf("ssr->_res12                 [127: 96] : %u(0x%08X)\n", ssr->_res12, ssr->_res12);
	printf("ssr->_res13                 [ 95: 64] : %u(0x%08X)\n", ssr->_res13, ssr->_res13);
	printf("ssr->_res14                 [ 63: 32] : %u(0x%08X)\n", ssr->_res14, ssr->_res14);
	printf("ssr->_res15                 [ 31:  0] : %u(0x%08X)\n", ssr->_res15, ssr->_res15);
	printf("===================================================\n");

	SDIO_CMD_LOG_END;
}

void SdioPrintSCR(SD_SCRreg *scr)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\n");
	printf("scr->SCR_STRUCTURE         [63:60] : %u(0x%08X)\n", scr->SCR_STRUCTURE, scr->SCR_STRUCTURE);
	printf("scr->SD_SPEC               [59:56] : %u(0x%08X)\n", scr->SD_SPEC, scr->SD_SPEC);
	printf("scr->DATA_STAT_AFTER_ERASE [55:55] : %u(0x%08X)\n", scr->DATA_STAT_AFTER_ERASE, scr->DATA_STAT_AFTER_ERASE);
	printf("scr->SD_SECURITY           [54:52] : %u(0x%08X)\n", scr->SD_SECURITY, scr->SD_SECURITY);
	printf("scr->SD_BUS_WIDTHS         [51:48] : %u(0x%08X)\n", scr->SD_BUS_WIDTHS, scr->SD_BUS_WIDTHS);
	printf("scr->SD_SPEC3              [47:47] : %u(0x%08X)\n", scr->SD_SPEC3, scr->SD_SPEC3);
	printf("scr->EX_SECURITY           [46:43] : %u(0x%08X)\n", scr->EX_SECURITY, scr->EX_SECURITY);
	printf("scr->_res1                 [42:34] : %u(0x%08X)\n", scr->_res1, scr->_res1);
	printf("scr->CMD_SUPPORT           [33:32] : %u(0x%08X)\n", scr->CMD_SUPPORT, scr->CMD_SUPPORT);
	printf("scr->_res2                 [31: 0] : %u(0x%08X)\n", scr->_res2, scr->_res2);
	printf("===================================================\n");

	SDIO_CMD_LOG_END;
}

void SdioPrintSFS(SD_SFS *sfs)
{	// 4.3.10.4 Switch Function Status / Table4-13: Status Data Structure
	SDIO_CMD_LOG_START;

	printf("===================================================\n");
	hexDump("SFS", (BYTE *)sfs, sizeof(SD_SFS));
	printf("===================================================\n");
	printf("sfs->MAX_CUR_PWR  [511:496] : %u(0x%04X)\n", sfs->MAX_CUR_PWR, sfs->MAX_CUR_PWR); // Maximum Current/Power Consumption
	printf("sfs->SUP_FGROUP6  [495:480] : %u(0x%04X)\n", sfs->SUP_FGROUP6, sfs->SUP_FGROUP6); // Support Bits of Functions in Function Group 6
	printf("sfs->SUP_FGROUP5  [479:464] : %u(0x%04X)\n", sfs->SUP_FGROUP5, sfs->SUP_FGROUP5); // Support Bits of Functions in Function Group 5
	printf("sfs->SUP_FGROUP4  [463:448] : %u(0x%04X)\n", sfs->SUP_FGROUP4, sfs->SUP_FGROUP4); // Support Bits of Functions in Function Group 4
	printf("sfs->SUP_FGROUP3  [447:432] : %u(0x%04X)\n", sfs->SUP_FGROUP3, sfs->SUP_FGROUP3); // Support Bits of Functions in Function Group 3
	printf("sfs->SUP_FGROUP2  [431:416] : %u(0x%04X)\n", sfs->SUP_FGROUP2, sfs->SUP_FGROUP2); // Support Bits of Functions in Function Group 2
	printf("sfs->SUP_FGROUP1  [415:400] : %u(0x%04X)\n", sfs->SUP_FGROUP1, sfs->SUP_FGROUP1); // Support Bits of Functions in Function Group 1
	printf("sfs->FUN_FGROUP6  [399:396] : %u(0x%01X)\n", sfs->FUN_FGROUP6, sfs->FUN_FGROUP6); // Function Selection of Function Group 6
	printf("sfs->FUN_FGROUP5  [395:392] : %u(0x%01X)\n", sfs->FUN_FGROUP5, sfs->FUN_FGROUP5); // Function Selection of Function Group 5
	printf("sfs->FUN_FGROUP4  [391:388] : %u(0x%01X)\n", sfs->FUN_FGROUP4, sfs->FUN_FGROUP4); // Function Selection of Function Group 4
	printf("sfs->FUN_FGROUP3  [387:384] : %u(0x%01X)\n", sfs->FUN_FGROUP3, sfs->FUN_FGROUP3); // Function Selection of Function Group 3
	printf("sfs->FUN_FGROUP2  [383:380] : %u(0x%01X)\n", sfs->FUN_FGROUP2, sfs->FUN_FGROUP2); // Function Selection of Function Group 2
	printf("sfs->FUN_FGROUP1  [379:376] : %u(0x%01X)\n", sfs->FUN_FGROUP1, sfs->FUN_FGROUP1); // Function Selection of Function Group 1
	printf("sfs->DATA_STR_VER [375:368] : %u(0x%02X)\n", sfs->DATA_STR_VER, sfs->DATA_STR_VER); // Data Structure Version
	printf("sfs->RES_FGROUP6  [367:352] : %u(0x%04X)\n", sfs->RES_FGROUP6, sfs->RES_FGROUP6); // Busy Status of Functions in Group 6
	printf("sfs->RES_FGROUP5  [351:336] : %u(0x%04X)\n", sfs->RES_FGROUP5, sfs->RES_FGROUP5); // Busy Status of Functions in Group 5
	printf("sfs->RES_FGROUP4  [335:320] : %u(0x%04X)\n", sfs->RES_FGROUP4, sfs->RES_FGROUP4); // Busy Status of Functions in Group 4
	printf("sfs->RES_FGROUP3  [319:304] : %u(0x%04X)\n", sfs->RES_FGROUP3, sfs->RES_FGROUP3); // Busy Status of Functions in Group 3
	printf("sfs->RES_FGROUP2  [303:288] : %u(0x%04X)\n", sfs->RES_FGROUP2, sfs->RES_FGROUP2); // Busy Status of Functions in Group 2
	printf("sfs->RES_FGROUP1  [287:272] : %u(0x%04X)\n", sfs->RES_FGROUP1, sfs->RES_FGROUP1); // Busy Status of Functions in Group 1
	printf("sfs->_res0        [271:256] : %u(0x%08X)\n", sfs->_res0, sfs->_res0);
	printf("sfs->_res1        [255:224] : %u(0x%08X)\n", sfs->_res1, sfs->_res1);
	printf("sfs->_res2        [223:192] : %u(0x%08X)\n", sfs->_res2, sfs->_res2);
	printf("sfs->_res3        [191:160] : %u(0x%08X)\n", sfs->_res3, sfs->_res3);
	printf("sfs->_res4        [159:128] : %u(0x%08X)\n", sfs->_res4, sfs->_res4);
	printf("sfs->_res5        [127: 96] : %u(0x%08X)\n", sfs->_res5, sfs->_res5);
	printf("sfs->_res6        [ 95: 64] : %u(0x%08X)\n", sfs->_res6, sfs->_res6);
	printf("sfs->_res7        [ 63: 32] : %u(0x%08X)\n", sfs->_res7, sfs->_res7);
	printf("sfs->_res8        [ 31:  0] : %u(0x%08X)\n", sfs->_res8, sfs->_res8);
	printf("===================================================\n");

	SDIO_CMD_LOG_END;
}

//-------------------------------------------------------------------------------------------------
// SDIO-CD Command

static UINT SdioCdReset(void)
{	// CMD0, 8 : Power on(Reset)
	UINT nResp, nRes;

	nRes = SdioCmd(sdinfo.nCH, 0, 0x00000000, 0, 0, 0);
	gprintf("[ 0] res(%d)\n", nRes);
	DELAY_MS(1);

	nRes = SdioCmd(sdinfo.nCH, 0, 0x00000000, 0, 0, 0);
	gprintf("[ 0] res(%d)\n", nRes);
	DELAY_MS(1);

	nRes = SdioCmd(sdinfo.nCH, 8, CMD8_VHS|CMD8_PATTERN, 1, 0, 0); // Send IF Condition
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR7);
	gprintf("[ 8] res(%d) RESP(0x%08X)\n", nRes, nResp);

	if (nResp & 0x300) {
		SDIO_CMD_LOG_END;
		return DEF_OK;
	} else {
		eprintf("End(FAIL) : RESP(0x%08X)\n", nResp);
		SDIO_CMD_LOG_END;
		return DEF_FAIL;
	}
}

static UINT SdioCdInitialization(void)
{	// ACMD41(R3)
	UINT *pResp;
	UINT nResp, nTryCnt = 0;
	BOOL bRes;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.ocr;

	do {
		if (SdioCdDet()) {
			eprintf("End(FAIL) : SDcard no detect\n");
			return DEF_FAIL;
		}
		if (++nTryCnt > SD_TRY_CNT) {
			eprintf("End(FAIL) : RESP(0x%08X)\n", *pResp);
			return DEF_FAIL;
		}

		DELAY_MS(100); // Polling less than 100ms interval
		bRes = SdioCmd(sdinfo.nCH, 55, 0x00000000, 1, 0, 0);
		SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
		gprintf("[55] res(%d) RESP(0x%08X)\n", bRes, nResp);
		bRes = SdioCmd(sdinfo.nCH, 41, ACMD41_HCS|ACMD41_XPC|ACMD41_VOLT
#ifdef sdio_support_1_8v
			|ACMD41_S18R
#endif
			, 1, 0, 0);	//	ACMD41
		SdioGetResp(sdinfo.nCH, pResp, ecrtR3);
		gprintf("[41] res(%d) RESP(0x%08X)\n", bRes, *pResp);
	} while (sdinfo.ocr.busy == 0);

#ifdef ENX_SDIOCD_CMD_DEBUG
	SdioPrintOCR(&sdinfo.ocr);
#endif

	SDIO_CMD_LOG_END;

	return DEF_OK;
}

static UINT SdioCdSwitchVoltage(void)
{	// CMD11(R1) : Switch to 1.8V
	UINT bRes = DEF_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	bRes = SdioCmd(sdinfo.nCH, 11, 0x00000000, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[11] res(%d) RESP0(0x%08X)\n", bRes, nResp);

	// Switch 1.8V
	if (bRes == DEF_OK) {
//		PAD_SDIO0_CMD_PU = 1;		// CMD pull-up disable
//		PAD_SDIO0_DAT_PU = 1;		// DAT pull-up disable
		SdioClockDisable(sdinfo.nCH);	// CLK disable
		DELAY_MS(5);				// 5ms delay(min.)

		gprintf("Set Default speed mode\n");
		SdioSetClockDiv(sdinfo.nCH, 25*1000*1000);

//		PAD_SDIO0_MSEL = 0x3F;		// 1.8v
		SdioClockEnable(sdinfo.nCH);// CLK enable
//		PAD_SDIO0_CMD_PU = 0;		// CMD pull-up enable
//		PAD_SDIO0_DAT_PU = 0;		// DAT pull-up enable

		sdinfo.nVoltageMode = 1;
		eprintf("3.3v -> 1.8v ok\n");
	} else {
		sdinfo.nVoltageMode = 0;
		eprintf("3.3v -> 1.8v fail\r\n");
	}

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioCdGetCID(int nType)
{	// CMD2, 10(R2): Type:0(CMD2) Type:1(CMD10)
	UINT bRes = DEF_FAIL;
	UINT *pResp;
	UINT cmdType = 0;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.cid;

	if (nType == 0) {
		cmdType = 2;
		bRes = SdioCmd(sdinfo.nCH, cmdType, 0x00000000, 1, 1, 0);
	} else if (nType == 1) {
		cmdType = 10;
		bRes = SdioCmd(sdinfo.nCH, cmdType, sdinfo.rca, 1, 1, 0);
	}
	SdioGetResp(sdinfo.nCH, pResp, ecrtR2);
	gprintf("[%2d] res(%d) RESP0(0x%08X)\n", cmdType, bRes, pResp[0]);
	gprintf("[%2d] res(%d) RESP1(0x%08X)\n", cmdType, bRes, pResp[1]);
	gprintf("[%2d] res(%d) RESP2(0x%08X)\n", cmdType, bRes, pResp[2]);
	gprintf("[%2d] res(%d) RESP3(0x%08X)\n", cmdType, bRes, pResp[3]);

#ifdef ENX_SDIOCD_CMD_DEBUG
	SdioPrintCID(&sdinfo.cid);
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioCdGetRCA(void)
{	// CMD3(R6)
	UINT bRes = DEF_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	bRes = SdioCmd(sdinfo.nCH, 3, 0x00000000, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR6);
	gprintf("[ 3] res(%d) RESP(0x%08X)\n", bRes, nResp);

	sdinfo.rca = nResp & RCA_RCA_MASK;

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioCdGetCSD(void)
{	// CMD9(R2)
	UINT bRes = DEF_FAIL;
	UINT *pResp;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.csd;

	bRes = SdioCmd(sdinfo.nCH, 9, sdinfo.rca, 1, 1, 0);
	SdioGetResp(sdinfo.nCH, pResp, ecrtR2);
	gprintf("[ 9] res(%d) RESP0(0x%08X)\n", bRes, pResp[0]);
	gprintf("[ 9] res(%d) RESP1(0x%08X)\n", bRes, pResp[1]);
	gprintf("[ 9] res(%d) RESP2(0x%08X)\n", bRes, pResp[2]);
	gprintf("[ 9] res(%d) RESP3(0x%08X)\n", bRes, pResp[3]);

#ifdef ENX_SDIOCD_CMD_DEBUG
	SdioPrintCSD(&sdinfo.csd);
#endif

	if (sdinfo.csd.csd_v2.PERM_WRITE_PROTECT == 1) {
		eprintf("PERM_WRITE_PROTECT enable!\n");
		bRes = DEF_FAIL;
	}

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioCdBusWidthChange(void)
{	// CMD7(R1b), CMD42(R1), ACMD6(R1) : Bus Width 1bit -> 4bit
	UINT bRes = DEF_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	// Select a card
	bRes = SdioCmd(sdinfo.nCH, 7, sdinfo.rca, 1, 0, 1);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1b);
	gprintf("[ 7] res(%d) RESP(0x%08X)\n", bRes, nResp);
#if 0
	// Unlock
	bRes = SdioCmd(sdinfo.nCH, 42, 0x00000000, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1b);
	gprintf("[42] res(%d) RESP(0x%08X)\n", bRes, nResp);
#endif
	// ACMD6 : 1bit -> 4bit
	bRes = SdioCmd(sdinfo.nCH, 55, sdinfo.rca, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[55] res(%d) RESP(0x%08X)\n", bRes, nResp);

	bRes = SdioCmd(sdinfo.nCH, 6, 0x00000002, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[ 6] res(%d) RESP(0x%08X)\n", bRes, nResp);

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioCdGetSSR(void)
{	// ACMD13(R1) : Data bus response / 64byte
	UINT bRes = DEF_FAIL;
	UINT nResp, nTemp = 0;

	SDIO_CMD_LOG_START;

	bRes = SdioCmd(sdinfo.nCH, 55, sdinfo.rca, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[55] res(%d) RESP(0x%08X)\n", bRes, nResp);

	nTemp = SdioGetDataBlockByte(sdinfo.nCH);
	SdioSetDataBlockByte(sdinfo.nCH, sizeof(SD_SSR));	// Data Block Byte를 64으로 설정
	SdioSetCmdDataReadS(sdinfo.nCH, 13);				// Data Read command를 13으로 설정

	UINT *getData = (UINT *)&sdinfo.ssr;
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+sizeof(SD_SSR));
	bRes = SdioDataIO(sdinfo.nCH, esditREAD, (ULONG)getData, 0, 1);
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+sizeof(SD_SSR));
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[13] res(%d) RESP(0x%08X)\n", bRes, nResp);

	SdioSetDataBlockByte(sdinfo.nCH, nTemp);	// Data Block Byte를 원상복귀
	SdioSetCmdDataReadS(sdinfo.nCH, 17);		// Data Read command를 17으로 설정(기본 Read 명령으로 설정)

#ifdef ENX_SDIOCD_CMD_DEBUG
	if (bRes == DEF_OK) {
		SdioPrintSSR(&sdinfo.ssr);
	}
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioCdGetSCR(void)
{	// ACMD51(R1) : Data bus response / 8byte
	UINT bRes = DEF_FAIL;
	UINT nResp, nTemp = 0;

	SDIO_CMD_LOG_START;
	printf("ACMD51 status address: 0x%08X, size: %u\n", &sdinfo.scr, sizeof(SD_SCRreg));

	bRes = SdioCmd(sdinfo.nCH, 55, sdinfo.rca, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[55] res(%d) RESP(0x%08X)\n", bRes, nResp);

	nTemp = SdioGetDataBlockByte(sdinfo.nCH);
	SdioSetDataBlockByte(sdinfo.nCH, sizeof(SD_SCRreg));	// Data Block Byte를 8으로 설정
	SdioSetCmdDataReadS(sdinfo.nCH, 51);					// Data Read command를 51으로 설정

	UINT *getData = (UINT *)&sdinfo.scr;
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+sizeof(SD_SCRreg));
	bRes = SdioDataIO(sdinfo.nCH, esditREAD, (ULONG)getData, 0, 1);
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+sizeof(SD_SCRreg));
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[51] res(%d) RESP(0x%08X)\n", bRes, nResp);

	SdioSetDataBlockByte(sdinfo.nCH, nTemp);	// Data Block Byte를 원상복귀
	SdioSetCmdDataReadS(sdinfo.nCH, 17);		// Data Read command를 17으로 설정(기본 Read 명령으로 설정)

#ifdef ENX_SDIOCD_CMD_DEBUG
	if (bRes == DEF_OK) {
		SdioPrintSCR(&sdinfo.scr);
	}
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioClockSwitch(UINT mode, UINT group, BYTE value)
{	// CMD6 : Check & Switch Clock (sub)
	UINT bRes = DEF_FAIL;
	UINT nResp, nTemp = 0, nArg = 0;

	SDIO_CMD_LOG_START;

	nArg = mode << 31 | 0x00FFFFFF;
	nArg &= ~(0xFF << (group * 4));
	nArg |= value << (group * 4);

	gprintf("nArg(mode:%u) : 0x%08X\n", mode, nArg);

	nTemp = SdioGetDataBlockByte(sdinfo.nCH);
	SdioSetDataBlockByte(sdinfo.nCH, sizeof(SD_SFS));	// Data Block Byte를 64으로 설정
	SdioSetCmdDataReadS(sdinfo.nCH, 6);					// Data Read command를 6으로 설정

	UINT *getData = (UINT *)&sdinfo.sfs;
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+sizeof(SD_SFS));
	bRes = SdioDataIO(sdinfo.nCH, esditREAD, (ULONG)getData, nArg, 1);
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+sizeof(SD_SFS));
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[ 6] res(%d) RESP(0x%08X)\n", bRes, nResp);

	SdioSetDataBlockByte(sdinfo.nCH, nTemp);	// Data Block Byte를 원상복귀
	SdioSetCmdDataReadS(sdinfo.nCH, 17);		// Data Read command를 17으로 설정(기본 Read 명령으로 설정)

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioCdSetClock(void)
{	// CMD6 : Check & Switch Clock (main)
	UINT bRes = DEF_FAIL;

	SDIO_CMD_LOG_START;
	printf("CMD6 status address: 0x%08X, size: %u\n", &sdinfo.sfs, sizeof(SD_SFS));

	if (sdinfo.ssr.SPEED_CLASS >= SD_SPEED_CLASS10) {
		if (sdinfo.scr.SD_SPEC < SCR_SPEC_VER_1) {
			gprintf("spec ver 1.0 or 1.01\n");
			goto if_speed;
		}

		if (!(sdinfo.csd.csd_v2.CCC & SCR_CCC_SWITCH)) {
			gprintf("card lacks mandatory switch function, performance might suffer.\n");
			goto if_speed;
		}

		if (SdioClockSwitch(0, 0, 1) == DEF_FAIL) {
			gprintf("problem reading switch capabilities, performance might suffer.\n");
			goto if_speed;
		}

		DELAY_MS(50);

#ifdef ENX_SDIOCD_CMD_DEBUG
		SdioPrintSFS(&sdinfo.sfs);
#endif

		if (!(sdinfo.sfs.SUP_FGROUP1 & 0x2)) {
			eprintf("\n");
			goto if_speed;
		}

		if (SdioClockSwitch(1, 0, 1) == DEF_FAIL) {
			eprintf("\n");
			goto if_speed;
		}

		SdioPrintSFS(&sdinfo.sfs);

		if ((sdinfo.sfs.FUN_FGROUP1 & 0xF) != 1) {
			eprintf("Problem switching card into high-speed mode!\n");
		} else {
			gprintf("Set High speed mode\n");
			SdioSetClockDiv(sdinfo.nCH, 0);
//			SdioSetClockDiv(sdinfo.nCH, 1);
//			SdioSetClockDiv(sdinfo.nCH, 2);
		}

		bRes = DEF_OK;
	} else {
if_speed:
		gprintf("Set Default speed mode\n");
		SdioSetClockDiv(sdinfo.nCH, 1);
		bRes = DEF_OK;
	}

	sdinfo.nSetClock = SdioGetClockDiv(sdinfo.nCH);

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioCdTuningCommand(void)
{	// CMD19(R1) : 4.2.4.5 Tuning Command
	UINT bRes = DEF_FAIL;
	UINT nResp = 0, nTemp = 0, nTry = 0;
	BYTE checkData[64] = {	0xFF,0x0F,0xFF,0x00,0xFF,0xCC,0xC3,0xCC,0xC3,0x3C,0xCC,0xFF,0xFE,0xFF,0xFE,0xEF,
							0xFF,0xDF,0xFF,0xDD,0xFF,0xFB,0xFF,0xFB,0xBF,0xFF,0x7F,0xFF,0x77,0xF7,0xBD,0xEF,
							0xFF,0xF0,0xFF,0xF0,0x0F,0xFC,0xCC,0x3C,0xCC,0x33,0xCC,0xCF,0xFF,0xEF,0xFF,0xEE,
							0xFF,0xFD,0xFF,0xFD,0xDF,0xFF,0xBF,0xFF,0xBB,0xFF,0xF7,0xFF,0xF7,0x7F,0x7B,0xDE};

	SDIO_CMD_LOG_START;

	nTemp = SdioGetDataBlockByte(sdinfo.nCH);
	SdioSetDataBlockByte(sdinfo.nCH, sizeof(SD_Tuning));			// Data Block Byte를 64으로 설정
	SdioSetCmdDataReadS(sdinfo.nCH, 19);				// Data Read command를 19으로 설정

	UINT *getData = (UINT *)&sdinfo.stuning;
sd_tuning:
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+sizeof(SD_Tuning));
	bRes = SdioDataIO(sdinfo.nCH, esditREAD, (ULONG)getData, 0, 1);
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+sizeof(SD_Tuning));
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[19] res(%d) RESP(0x%08X)\n", bRes, nResp);

	if (bRes == DEF_OK) {
		for (UINT i = 0; i < sizeof(SD_Tuning); i++) {
			if (checkData[i] != getData[i]) {
				eprintf("Tuning Block Pattern Mismatch\n");
				hexDump("Tuning Block Pattern", getData, sizeof(SD_Tuning));
				SdioCdClockDown(); // 주파수 낮추고 재시도
				if (nTry == 0) {
					nTry++;
					goto sd_tuning;
				} else {
					bRes = DEF_FAIL;
				}
				break;
			}
		}
	}

	SdioSetDataBlockByte(sdinfo.nCH, nTemp);	// Data Block Byte를 원상복귀
	SdioSetCmdDataReadS(sdinfo.nCH, 17);		// Data Read command를 17으로 설정(기본 Read 명령으로 설정)

	SDIO_CMD_LOG_END;

	return bRes;
}

static UINT SdioCdSetBlockLength(void)
{	// CMD16(R1) : Define the block length
	UINT bRes = DEF_FAIL;
	UINT nResp = 0, nTemp = 0;

	SDIO_CMD_LOG_START;

	nTemp = SdioGetDataBlockByte(sdinfo.nCH);
	bRes = SdioCmd(sdinfo.nCH, 16, nTemp, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	gprintf("[16] res(%d) RESP(0x%08X)\n", bRes, nResp);

	SDIO_CMD_LOG_END;

	return bRes;
}

////////////////////////////////////////////////////////////////////////////////
// SDIO-CD Process
UINT SdioCdInitProcess(void)
{
	SDIO_CMD_LOG_START;

	UINT nCH = sdinfo.nCH;
	memset(&sdinfo, 0, sizeof(sdinfo));
	sdinfo.nCH = nCH;

	SdioSetClockDef(sdinfo.nCH);
	char strClockPrint[40] = {0};
#ifdef ENX_SDIOCD_CMD_DEBUG
	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Init Start(%s)\n", sdinfo.nCH, strClockPrint);
#endif

	SdioSetDataBlockByte(sdinfo.nCH, 512);
	UINT nTemp;
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// CMD 0, 8
	if (SdioCdReset() == DEF_FAIL) {
		sdinfo.nErrorCode = 1;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// ACMD 41
	if (SdioCdInitialization() == DEF_FAIL) {
		sdinfo.nErrorCode = 2;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	sdinfo.nVoltageMode = 0;
	if (sdinfo.ocr.VDD18 && sdinfo.ocr.CCS) {
		// CMD 11
		if (SdioCdSwitchVoltage() == DEF_FAIL) {
			sdinfo.nErrorCode = 3;
		} else {
			sdinfo.nVoltageMode = 1;
		}
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// CMD 2
	if (SdioCdGetCID(0) == DEF_FAIL) {
		sdinfo.nErrorCode = 4;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// CMD 3
	if (SdioCdGetRCA() == DEF_FAIL) {
		sdinfo.nErrorCode = 5;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// CMD 9
	if (SdioCdGetCSD() == DEF_FAIL) {
		sdinfo.nErrorCode = 6;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// CMD 10
	if (SdioCdGetCID(1) == DEF_FAIL) {
		sdinfo.nErrorCode = 7;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// CMD 7, 42, ACMD 6
	if (SdioCdBusWidthChange() == DEF_FAIL) {
		sdinfo.nErrorCode = 8;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// ACMD 13
	if (SdioCdGetSSR() == DEF_FAIL) {
		sdinfo.nErrorCode = 9;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// ACMD 51
	if (SdioCdGetSCR() == DEF_FAIL) {
		sdinfo.nErrorCode = 10;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// CMD 6
	if (SdioCdSetClock() == DEF_FAIL) {
		sdinfo.nErrorCode = 11;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// CMD 19
	if(sdinfo.nVoltageMode == 1) {	// UHS-1 1.8v
		if (SdioCdTuningCommand() == DEF_FAIL) {
			sdinfo.nErrorCode = 12;
			goto done_fail;
		}
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	// CMD 16
	if (SdioCdSetBlockLength() == DEF_FAIL) {
		sdinfo.nErrorCode = 13;
		goto done_fail;
	}
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);flprintf("nTemp Size (%d)\n", nTemp);
	sdinfo.nActive = 1;

	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Init OK(%s)\n", sdinfo.nCH, strClockPrint);

	SDIO_CMD_LOG_END;
	return DEF_OK;
done_fail:
	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Init Fail(err:%u)(%s)\n", sdinfo.nCH, sdinfo.nErrorCode, strClockPrint);

	SDIO_CMD_LOG_END;
	return DEF_FAIL;
}

void SdioCdClockDown(void)
{
	UINT Clkdiv = SdioGetClockDiv(sdinfo.nCH);
	SdioSetClockDiv(sdinfo.nCH, Clkdiv + 1);

	char strClockPrint[40] = {0};
	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Change Clock(%s)\n", sdinfo.nCH, strClockPrint);
}

void SdioCdClockRestore(void)
{
	SdioSetClockDiv(sdinfo.nCH, sdinfo.nSetClock);

	char strClockPrint[40] = {0};
	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Change Clock(%s)\n", sdinfo.nCH, strClockPrint);
}

UINT SdioCdDet(void)
{
	return GpioGetPin(SD_GPIO_IRQ);
}

// 기본적인 CH, IRQ, 최초주파수, Pull-up 등을 설정한다.
void SdioCdInit(UINT nCH)
{
	memset(&sdinfo, 0, sizeof(sdinfo));
	sdinfo.nCH = nCH;
	sdinfo.nErrorCode = 0;

	GpioSetLo(SD_GPIO_RST); // LO:ON HI:OFF
	SdioSetDelayfn(sdinfo.nCH, (user_delay_fn)ENX_SDIOCD_DELAY);

//	GpioInDir(SD_GPIO_IRQ);
//	GpioRiseEdge(SD_GPIO_IRQ);
//	GpioIrqCallback(SD_GPIO_IRQ, SdioCdDetectIrq, NULL);
//	GpioIrqOn(SD_GPIO_IRQ);
}

////////////////////////////////////////////////////////////////////////////////
// SDIO-CD Port
UINT SdioCdGetActive(void)
{
	return sdinfo.nActive;
}

UINT SdioCdGetErrCode(void)
{
	return sdinfo.nErrorCode;
}

UINT SdioCdGetAUSize(void)
{
	return sdinfo.ssr.AU_SIZE;
}

void SdioCdGetName(char *buf)
{
	buf[0] = (char)sdinfo.cid.PNM1;
	buf[1] = (char)sdinfo.cid.PNM2;
	buf[2] = (char)sdinfo.cid.PNM3;
	buf[3] = (char)sdinfo.cid.PNM4;
	buf[4] = (char)sdinfo.cid.PNM5;
}

UINT SdioCdGetSectorCnt(void)
{
	SDIO_CMD_LOG_START;

	UINT nSector = 0;

	if (sdinfo.csd.csd_v1.CSD_STRUCTURE == 1) { // CSD 2.0
		nSector = (((sdinfo.csd.csd_v2.C_SIZE0 << 6) | (sdinfo.csd.csd_v2.C_SIZE1)) + 1) << 10;
		printf("CSD2.0 : C_SIZE(%u)\n", sdinfo.csd.csd_v2.C_SIZE0 << 6 | sdinfo.csd.csd_v2.C_SIZE1);
		printf("         SECTOR(%u)\n", nSector);
	} else if (sdinfo.csd.csd_v1.CSD_STRUCTURE == 0) { // CSD 1.0
		nSector = ((((sdinfo.csd.csd_v1.C_SIZE0 << 10) | (sdinfo.csd.csd_v1.C_SIZE1)) + 1) * (0x1 << (sdinfo.csd.csd_v1.C_SIZE_MULT + 2)) * (0x1 << (sdinfo.csd.csd_v1.READ_BL_LEN))) >> 9;
		printf("CSD1.0 : C_SIZE(%u)\n", sdinfo.csd.csd_v1.C_SIZE0 << 10 | sdinfo.csd.csd_v1.C_SIZE1);
		printf("         C_SIZE_MULT(%u)\n", sdinfo.csd.csd_v1.C_SIZE_MULT);
		printf("         BLOCK_LEN(%u)\n", sdinfo.csd.csd_v1.READ_BL_LEN);
		printf("         SECTOR(%u)\n", nSector);
	} else {
		printf("CSD Version error(%d)\n", sdinfo.csd.csd_v1.CSD_STRUCTURE);
	}

	SDIO_CMD_LOG_END;

	return nSector;
}

UINT SdioCdGetSize(void)
{
	UINT nSector;
	if (sdinfo.csd.csd_v1.CSD_STRUCTURE == 0) {
		SD_CSD_v1 *csd = &(sdinfo.csd.csd_v1);
		nSector = (((csd->C_SIZE0 << 10 | csd->C_SIZE1) + 1) * (0x1 << (csd->C_SIZE_MULT + 2)) * (0x1 << (csd->READ_BL_LEN))) >> 9;
	} else {
		SD_CSD_v2 *csd = &(sdinfo.csd.csd_v2);
		nSector = ((csd->C_SIZE0 << 6 | csd->C_SIZE1) + 1) << 10;
	}
	return nSector >> 11;
}

UINT SdioCdE(UINT start_sctor, UINT end_sctor)
{	// CMD32(R1), 33(R1), 38(R1b) : Erase
	UINT bRes = DEF_FAIL;
	UINT nResp = 0;
	UINT nBlocSp, nBlocEp;

	SDIO_CMD_LOG_START;

	if (sdinfo.ocr.CCS) {
		nBlocSp = start_sctor;
		nBlocEp = end_sctor;
	} else {
		nBlocSp = start_sctor << 9;
		nBlocEp = end_sctor << 9;
	}

	// CMD32 : Erase start address
	bRes = SdioCmd(sdinfo.nCH, 32, nBlocSp, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	tprintf("[32] res(%d) RESP(0x%08X)\n", bRes, nResp);

	// CMD33 : Erase end address
	bRes = SdioCmd(sdinfo.nCH, 33, nBlocEp, 1, 0, 0);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	tprintf("[33] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	// CMD38 : Erase command
	bRes = SdioCmd(sdinfo.nCH, 38, 0, 1, 0, 1);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1b);
	tprintf("[38] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO_CMD_LOG_END;

	return bRes;
}

UINT SdioCdReadS(const BYTE *buff, UINT sector)
{
	UINT bRes = DEF_FAIL;
	UINT nResp = 0, nAddr = 0, nTemp = 0;
	UINT *_buff = (UINT *)buff;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

	while (SdioIsDataEn(sdinfo.nCH) == 1) {
		ENX_SDIOCD_DELAY(0);
	}

	SdioSetDataBlockByte(sdinfo.nCH, 512);		// 추후 삭제 예정
	nTemp = SdioGetDataBlockByte(sdinfo.nCH);	// 추후 삭졔 예정

	UINT *getData = _buff;
//	printf("hwflush 0x%08X->0x%08X\n", (ULONG)getData, ((ULONG)getData)+nTemp);
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+nTemp);
//	hexDump("READ-start", getData, nTemp);
	bRes = SdioDataIO(sdinfo.nCH, esditREAD, (ULONG)getData, nAddr, 1);
	hwflush_dcache_range((ULONG)getData, ((ULONG)getData)+nTemp);
	hexDump("READ-done", getData, nTemp);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	tprintf("[17] res(%d) RESP(0x%08X)\n", bRes, nResp);

	return bRes;
}

UINT SdioCdReadM(const BYTE *buff, UINT sector, UINT count)
{
	UINT bRes = DEF_FAIL;
	return bRes;
}

UINT SdioCdWriteS(const BYTE *buff, UINT sector)
{
	UINT bRes = DEF_FAIL;
	return bRes;
}

UINT SdioCdWriteM(const BYTE *buff, UINT sector, UINT count)
{
	UINT bRes = DEF_FAIL;
	return bRes;
}
