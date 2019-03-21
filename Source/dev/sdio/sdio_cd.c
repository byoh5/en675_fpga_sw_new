#include "dev.h"

#if defined(__USE_SDIOCD__)

#include "sdio_cd.h"
#include "enx_freertos.h"

#include <string.h> // memset

static SDIO_SD sdinfo;

//#define sdio_support_1_8v

#define ENX_SDIOCD_DELAY vTaskDelay
#define DELAY_MS(ms) ENX_SDIOCD_DELAY(ms)

#define ENX_SDIOCD_IRQ_LOCK portENTER_CRITICAL
#define ENX_SDIOCD_IRQ_UNLOCK portEXIT_CRITICAL

static void SdioCdLog_hwflush_dcache_range(uint sadr, uint size)
{
	printf("H/W Flush Addr  0x%08X->0x%08X, Byte %lubyte\n", sadr, sadr + size, size);
	uint64_t a = rdcycle();
	hwflush_dcache_range(sadr, size);
	uint64_t b = rdcycle();
	printf("H/W Flush Cycle %lu\n", b - a);
}

#define ENX_SDIOCD_FLUSH_DCACHE SdioCdLog_hwflush_dcache_range

//-------------------------------------------------------------------------------------------------
// CMD8
#define CMD8_VHS			0x00000100		// [11: 8] : 2.7-3.6V
#define CMD8_PATTERN		0x000000aa		// [ 7: 0] : Just "0xaa"

// ACMD41
#define	ACMD41_HCS			0x40000000		// [   30] : SDHC or SDXC Supported
#define ACMD41_XPC			0x10000000		// [   28] : SDXC Maximum Performance
#define ACMD41_S18R			0x01000000		// [   24] : Switching to 1.8V Request
#define	ACMD41_VOLT			0x00ff8000		// [23:15] : Voltage window OCR

// SCR register
#define SCR_SPEC_VER_0		0	/* Implements system specification 1.0 - 1.01 */
#define SCR_SPEC_VER_1		1	/* Implements system specification 1.10 */
#define SCR_SPEC_VER_2		2	/* Implements system specification 2.00 */
#define SCR_CCC_SWITCH		0x00000400		// [  :10] : High speed switch

// SD bus widths
#define SD_BUS_WIDTH_1		0
#define SD_BUS_WIDTH_4		2

// SD_SWITCH mode
#define SD_SWITCH_CHECK		0
#define SD_SWITCH_SET		1

// SD_SWITCH function groups
#define SD_SWITCH_GRP_ACCESS	0

// SD_SWITCH access modes
#define SD_SWITCH_ACCESS_DEF	0
#define SD_SWITCH_ACCESS_HS		1

// Status Mask
#define RCA_RCA_MASK		0xffff0000

// Sd power up retry count
#define	SD_TRY_CNT			100

BYTE *SDIO_DATA_BASE = NULL;

//-------------------------------------------------------------------------------------------------
// SDIO-CD Card Reg
void SdioPrintOCR(SD_OCR *ocr)
{
	ENX_LOG_START(DBG_SDIO_CD_CMD);

	printf("===================================================\n");
	hexDump("OCR", (BYTE *)ocr, sizeof(SD_OCR));
	printf("===================================================\n");
	printf("ocr->busy     [   31] : 0x%08X(%u)\n", ocr->busy, ocr->busy);
	printf("ocr->CCS      [   30] : 0x%08X(%u)\n", ocr->CCS, ocr->CCS);
	printf("ocr->UHSIICS  [   29] : 0x%08X(%u)\n", ocr->UHSIICS, ocr->UHSIICS);
	printf("ocr->_res1    [28:25] : 0x%08X(%u)\n", ocr->_res1, ocr->_res1);
	printf("ocr->VDD18    [   24] : 0x%08X(%u)\n", ocr->VDD18, ocr->VDD18);
	printf("ocr->VDD35_36 [   23] : 0x%08X(%u)\n", ocr->VDD35_36, ocr->VDD35_36);
	printf("ocr->VDD34_35 [   22] : 0x%08X(%u)\n", ocr->VDD34_35, ocr->VDD34_35);
	printf("ocr->VDD33_34 [   21] : 0x%08X(%u)\n", ocr->VDD33_34, ocr->VDD33_34);
	printf("ocr->VDD32_33 [   20] : 0x%08X(%u)\n", ocr->VDD32_33, ocr->VDD32_33);
	printf("ocr->VDD31_32 [   19] : 0x%08X(%u)\n", ocr->VDD31_32, ocr->VDD31_32);
	printf("ocr->VDD30_31 [   18] : 0x%08X(%u)\n", ocr->VDD30_31, ocr->VDD30_31);
	printf("ocr->VDD29_30 [   17] : 0x%08X(%u)\n", ocr->VDD29_30, ocr->VDD29_30);
	printf("ocr->VDD28_29 [   16] : 0x%08X(%u)\n", ocr->VDD28_29, ocr->VDD28_29);
	printf("ocr->VDD27_28 [   15] : 0x%08X(%u)\n", ocr->VDD27_28, ocr->VDD27_28);
	printf("ocr->_res2    [14: 8] : 0x%08X(%u)\n", ocr->_res2, ocr->_res2);
	printf("ocr->VDDLOW   [    7] : 0x%08X(%u)\n", ocr->VDDLOW, ocr->VDDLOW);
	printf("ocr->_res3    [ 6: 0] : 0x%08X(%u)\n", ocr->_res3, ocr->_res3);
	printf("===================================================\n");

	ENX_LOG_END(DBG_SDIO_CD_CMD);
}

void SdioPrintCID(SD_CID *cid)
{
	ENX_LOG_START(DBG_SDIO_CD_CMD);

	printf("===================================================\n");
	hexDump("CID", (BYTE *)cid, sizeof(SD_CID));
	printf("===================================================\n");
	printf("cid->MID    [127:120] : 0x%08X(%u)\n", cid->MID, cid->MID);
	printf("cid->OID    [119:104] : 0x%08X(%u)\n", cid->OID, cid->OID);
	printf("cid->PNM1   [103: 96] : 0x%08X(%u) : %c\n", cid->PNM1, cid->PNM1, cid->PNM1);
	printf("cid->PNM2     [95:88] : 0x%08X(%u) : %c\n", cid->PNM2, cid->PNM2, cid->PNM2);
	printf("cid->PNM3     [87:80] : 0x%08X(%u) : %c\n", cid->PNM3, cid->PNM3, cid->PNM3);
	printf("cid->PNM4     [79:72] : 0x%08X(%u) : %c\n", cid->PNM4, cid->PNM4, cid->PNM4);
	printf("cid->PNM5     [71:64] : 0x%08X(%u) : %c\n", cid->PNM5, cid->PNM5, cid->PNM5);
	printf("cid->PRV      [63:56] : 0x%08X(%u)\n", cid->PRV, cid->PRV);
	printf("cid->PSN      [55:24] : 0x%08X(%u)\n", cid->PSN0 << 8 | cid->PSN1, cid->PSN0 << 8 | cid->PSN1);
	printf("cid->_res1    [23:20] : 0x%08X(%u)\n", cid->_res1, cid->_res1);
	printf("cid->MDT      [19: 8] : 0x%08X(%u)\n", cid->MDT, cid->MDT);
	printf("cid->CRC      [ 7: 1] : 0x%08X(%u)\n", cid->CRC, cid->CRC);
	printf("cid->_res2    [    0] : 0x%08X(%u)\n", cid->_res2, cid->_res2);
	printf("===================================================\n");

	ENX_LOG_END(DBG_SDIO_CD_CMD);
}

void SdioPrintCSD(SD_CSD *csd)
{
	ENX_LOG_START(DBG_SDIO_CD_CMD);

	printf("===================================================\n");
	hexDump("CSD", (BYTE *)csd, sizeof(SD_CSD));
	printf("===================================================\n");
	if (csd->csd_v1.CSD_STRUCTURE == 0) { // CSD Version 1.0
		printf("csd->CSD_STRUCTURE      [127:126] : 0x%08X(%u)\n", csd->csd_v1.CSD_STRUCTURE, csd->csd_v1.CSD_STRUCTURE);
	} else if (csd->csd_v1.CSD_STRUCTURE == 1) { // CSD Version 2.0
		printf("csd->CSD_STRUCTURE      [127:126] : 0x%08X(%u)\n", csd->csd_v2.CSD_STRUCTURE, csd->csd_v2.CSD_STRUCTURE);
		printf("csd->_res1              [125:120] : 0x%08X(%u)\n", csd->csd_v2._res1, csd->csd_v2._res1);
		printf("csd->TAAC               [119:112] : 0x%08X(%u)\n", csd->csd_v2.TAAC, csd->csd_v2.TAAC);
		printf("csd->NSAC               [111:104] : 0x%08X(%u)\n", csd->csd_v2.NSAC, csd->csd_v2.NSAC);
		printf("csd->TRAN_SPEED         [103: 96] : 0x%08X(%u)\n", csd->csd_v2.TRAN_SPEED, csd->csd_v2.TRAN_SPEED);
		printf("csd->CCC                [ 95: 84] : 0x%08X(%u)\n", csd->csd_v2.CCC, csd->csd_v2.CCC);
		printf("csd->READ_BL_LEN        [ 83: 80] : 0x%08X(%u)\n", csd->csd_v2.READ_BL_LEN, csd->csd_v2.READ_BL_LEN);
		printf("csd->READ_BL_PARTIAL    [ 79: 79] : 0x%08X(%u)\n", csd->csd_v2.READ_BL_PARTIAL, csd->csd_v2.READ_BL_PARTIAL);
		printf("csd->WRITE_BLK_MISALIGN [ 78: 78] : 0x%08X(%u)\n", csd->csd_v2.WRITE_BLK_MISALIGN, csd->csd_v2.WRITE_BLK_MISALIGN);
		printf("csd->READ_BLK_MISALIGN  [ 77: 77] : 0x%08X(%u)\n", csd->csd_v2.READ_BLK_MISALIGN, csd->csd_v2.READ_BLK_MISALIGN);
		printf("csd->DSR_IMP            [ 76: 76] : 0x%08X(%u)\n", csd->csd_v2.DSR_IMP, csd->csd_v2.DSR_IMP);
		printf("csd->_res2              [ 75: 70] : 0x%08X(%u)\n", csd->csd_v2._res2, csd->csd_v2._res2);
		printf("csd->C_SIZE             [ 69: 48] : 0x%08X(%u)\n", csd->csd_v2.C_SIZE0 << 6 | csd->csd_v2.C_SIZE1, csd->csd_v2.C_SIZE0 << 6 | csd->csd_v2.C_SIZE1);
		printf("csd->_res3              [ 47: 47] : 0x%08X(%u)\n", csd->csd_v2._res3, csd->csd_v2._res3);
		printf("csd->ERASE_BLK_EN       [ 46: 46] : 0x%08X(%u)\n", csd->csd_v2.ERASE_BLK_EN, csd->csd_v2.ERASE_BLK_EN);
		printf("csd->SECTOR_SIZE        [ 45: 39] : 0x%08X(%u)\n", csd->csd_v2.SECTOR_SIZE, csd->csd_v2.SECTOR_SIZE);
		printf("csd->WP_GRP_SIZE        [ 38: 32] : 0x%08X(%u)\n", csd->csd_v2.WP_GRP_SIZE, csd->csd_v2.WP_GRP_SIZE);
		printf("csd->WP_GRP_ENABLE      [ 31: 31] : 0x%08X(%u)\n", csd->csd_v2.WP_GRP_ENABLE, csd->csd_v2.WP_GRP_ENABLE);
		printf("csd->_res4              [ 30: 29] : 0x%08X(%u)\n", csd->csd_v2._res4, csd->csd_v2._res4);
		printf("csd->R2W_FACTOR         [ 28: 26] : 0x%08X(%u)\n", csd->csd_v2.R2W_FACTOR, csd->csd_v2.R2W_FACTOR);
		printf("csd->WRITE_BL_LEN       [ 25: 22] : 0x%08X(%u)\n", csd->csd_v2.WRITE_BL_LEN, csd->csd_v2.WRITE_BL_LEN);
		printf("csd->WRITE_BL_PARTIAL   [ 21: 19] : 0x%08X(%u)\n", csd->csd_v2.WRITE_BL_PARTIAL, csd->csd_v2.WRITE_BL_PARTIAL);
		printf("csd->_res5              [ 20: 16] : 0x%08X(%u)\n", csd->csd_v2._res5, csd->csd_v2._res5);
		printf("csd->FILE_FORMAT_GRP    [ 15: 15] : 0x%08X(%u)\n", csd->csd_v2.FILE_FORMAT_GRP, csd->csd_v2.FILE_FORMAT_GRP);
		printf("csd->COPY               [ 14: 14] : 0x%08X(%u)\n", csd->csd_v2.COPY, csd->csd_v2.COPY);
		printf("csd->PERM_WRITE_PROTECT [ 13: 13] : 0x%08X(%u)\n", csd->csd_v2.PERM_WRITE_PROTECT, csd->csd_v2.PERM_WRITE_PROTECT);
		printf("csd->TMP_WRITE_PROTECT  [ 12: 12] : 0x%08X(%u)\n", csd->csd_v2.TMP_WRITE_PROTECT, csd->csd_v2.TMP_WRITE_PROTECT);
		printf("csd->FILE_FORMAT        [ 11: 10] : 0x%08X(%u)\n", csd->csd_v2.FILE_FORMAT, csd->csd_v2.FILE_FORMAT);
		printf("csd->_res6              [  9:  8] : 0x%08X(%u)\n", csd->csd_v2._res6, csd->csd_v2._res6);
		printf("csd->CRC                [  7:  1] : 0x%08X(%u)\n", csd->csd_v2.CRC, csd->csd_v2.CRC);
		printf("csd->_res7              [  0:  0] : 0x%08X(%u)\n", csd->csd_v2._res7, csd->csd_v2._res7);
	} else {
		printf("(%04d)%s : CSD Version Check Error\n", __LINE__, __func__);
	}
	printf("===================================================\n");

	ENX_LOG_END(DBG_SDIO_CD_CMD);
}

void SdioPrintSSR(SD_SSR *ssr)
{
	ENX_LOG_START(DBG_SDIO_CD_CMD);

	printf("===================================================\n");
	hexDump("SSR", (BYTE *)ssr, sizeof(SD_SSR));
	printf("===================================================\n");
	printf("ssr->DAT_BUS_WIDTH          [511:510] : 0x%08X(%u)\n", ssr->DAT_BUS_WIDTH, ssr->DAT_BUS_WIDTH);
	printf("ssr->SECURED_MODE           [509:509] : 0x%08X(%u)\n", ssr->SECURED_MODE, ssr->SECURED_MODE);
	printf("ssr->RSF                    [508:502] : 0x%08X(%u)\n", ssr->RSF, ssr->RSF);
	printf("ssr->_res1                  [501:496] : 0x%08X(%u)\n", ssr->_res1, ssr->_res1);
	printf("ssr->SD_CARD_TYPE           [495:480] : 0x%08X(%u)\n", ssr->SD_CARD_TYPE, ssr->SD_CARD_TYPE);
	printf("ssr->SIZE_OF_PROTECTED_AREA [479:448] : 0x%08X(%u)\n", ssr->SIZE_OF_PROTECTED_AREA, ssr->SIZE_OF_PROTECTED_AREA);
	printf("ssr->SPEED_CLASS            [447:440] : 0x%08X(%u)\n", ssr->SPEED_CLASS, ssr->SPEED_CLASS);
	printf("ssr->PERFORMANCE_MOVE       [439:432] : 0x%08X(%u)\n", ssr->PERFORMANCE_MOVE, ssr->PERFORMANCE_MOVE);
	printf("ssr->AU_SIZE                [431:428] : 0x%08X(%u)\n", ssr->AU_SIZE, ssr->AU_SIZE);
	printf("ssr->_res2                  [427:424] : 0x%08X(%u)\n", ssr->_res2, ssr->_res2);
	printf("ssr->ERASE_SIZE             [423:408] : 0x%08X(%u)\n", ssr->ERASE_SIZE, ssr->ERASE_SIZE);
	printf("ssr->ERASE_TIMEOUT          [407:402] : 0x%08X(%u)\n", ssr->ERASE_TIMEOUT, ssr->ERASE_TIMEOUT);
	printf("ssr->ERASE_OFFSET           [401:400] : 0x%08X(%u)\n", ssr->ERASE_OFFSET, ssr->ERASE_OFFSET);
	printf("ssr->UHS_SPEED_GRADE        [399:396] : 0x%08X(%u)\n", ssr->UHS_SPEED_GRADE, ssr->UHS_SPEED_GRADE);
	printf("ssr->UHS_AU_SIZE            [395:392] : 0x%08X(%u)\n", ssr->UHS_AU_SIZE, ssr->UHS_AU_SIZE);
	printf("ssr->_res3                  [391:384] : 0x%08X(%u)\n", ssr->_res3, ssr->_res3);
	printf("ssr->_res4                  [383:352] : 0x%08X(%u)\n", ssr->_res4, ssr->_res4);
	printf("ssr->_res5                  [351:320] : 0x%08X(%u)\n", ssr->_res5, ssr->_res5);
	printf("ssr->_res6                  [319:288] : 0x%08X(%u)\n", ssr->_res6, ssr->_res6);
	printf("ssr->_res7                  [287:256] : 0x%08X(%u)\n", ssr->_res7, ssr->_res7);
	printf("ssr->_res8                  [255:224] : 0x%08X(%u)\n", ssr->_res8, ssr->_res8);
	printf("ssr->_res9                  [223:192] : 0x%08X(%u)\n", ssr->_res9, ssr->_res9);
	printf("ssr->_res10                 [191:160] : 0x%08X(%u)\n", ssr->_res10, ssr->_res10);
	printf("ssr->_res11                 [159:128] : 0x%08X(%u)\n", ssr->_res11, ssr->_res11);
	printf("ssr->_res12                 [127: 96] : 0x%08X(%u)\n", ssr->_res12, ssr->_res12);
	printf("ssr->_res13                 [ 95: 64] : 0x%08X(%u)\n", ssr->_res13, ssr->_res13);
	printf("ssr->_res14                 [ 63: 32] : 0x%08X(%u)\n", ssr->_res14, ssr->_res14);
	printf("ssr->_res15                 [ 31:  0] : 0x%08X(%u)\n", ssr->_res15, ssr->_res15);
	printf("===================================================\n");

	ENX_LOG_END(DBG_SDIO_CD_CMD);
}

void SdioPrintSCR(SD_SCRreg *scr)
{
	ENX_LOG_START(DBG_SDIO_CD_CMD);

	printf("===================================================\n");
	hexDump("SCR", (BYTE *)scr, sizeof(SD_SCRreg));
	printf("===================================================\n");
	printf("scr->SCR_STRUCTURE         [63:60] : 0x%08X(%u)\n", scr->SCR_STRUCTURE, scr->SCR_STRUCTURE);
	printf("scr->SD_SPEC               [59:56] : 0x%08X(%u)\n", scr->SD_SPEC, scr->SD_SPEC);
	printf("scr->DATA_STAT_AFTER_ERASE [55:55] : 0x%08X(%u)\n", scr->DATA_STAT_AFTER_ERASE, scr->DATA_STAT_AFTER_ERASE);
	printf("scr->SD_SECURITY           [54:52] : 0x%08X(%u)\n", scr->SD_SECURITY, scr->SD_SECURITY);
	printf("scr->SD_BUS_WIDTHS         [51:48] : 0x%08X(%u)\n", scr->SD_BUS_WIDTHS, scr->SD_BUS_WIDTHS);
	printf("scr->SD_SPEC3              [47:47] : 0x%08X(%u)\n", scr->SD_SPEC3, scr->SD_SPEC3);
	printf("scr->EX_SECURITY           [46:43] : 0x%08X(%u)\n", scr->EX_SECURITY, scr->EX_SECURITY);
	printf("scr->_res1                 [42:34] : 0x%08X(%u)\n", scr->_res1, scr->_res1);
	printf("scr->CMD_SUPPORT           [33:32] : 0x%08X(%u)\n", scr->CMD_SUPPORT, scr->CMD_SUPPORT);
	printf("scr->_res2                 [31: 0] : 0x%08X(%u)\n", scr->_res2, scr->_res2);
	printf("===================================================\n");

	ENX_LOG_END(DBG_SDIO_CD_CMD);
}

void SdioPrintSFS(SD_SFS *sfs)
{	// 4.3.10.4 Switch Function Status / Table4-13: Status Data Structure
	ENX_LOG_START(DBG_SDIO_CD_CMD);

	printf("===================================================\n");
	hexDump("SFS", (BYTE *)sfs, sizeof(SD_SFS));
	printf("===================================================\n");
	printf("sfs->MAX_CUR_PWR  [511:496] : 0x%08X(%u)\n", sfs->MAX_CUR_PWR, sfs->MAX_CUR_PWR); // Maximum Current/Power Consumption
	printf("sfs->SUP_FGROUP6  [495:480] : 0x%08X(%u)\n", sfs->SUP_FGROUP6, sfs->SUP_FGROUP6); // Support Bits of Functions in Function Group 6
	printf("sfs->SUP_FGROUP5  [479:464] : 0x%08X(%u)\n", sfs->SUP_FGROUP5, sfs->SUP_FGROUP5); // Support Bits of Functions in Function Group 5
	printf("sfs->SUP_FGROUP4  [463:448] : 0x%08X(%u)\n", sfs->SUP_FGROUP4, sfs->SUP_FGROUP4); // Support Bits of Functions in Function Group 4
	printf("sfs->SUP_FGROUP3  [447:432] : 0x%08X(%u)\n", sfs->SUP_FGROUP3, sfs->SUP_FGROUP3); // Support Bits of Functions in Function Group 3
	printf("sfs->SUP_FGROUP2  [431:416] : 0x%08X(%u)\n", sfs->SUP_FGROUP2, sfs->SUP_FGROUP2); // Support Bits of Functions in Function Group 2
	printf("sfs->SUP_FGROUP1  [415:400] : 0x%08X(%u)\n", sfs->SUP_FGROUP1, sfs->SUP_FGROUP1); // Support Bits of Functions in Function Group 1
	printf("sfs->FUN_FGROUP6  [399:396] : 0x%08X(%u)\n", sfs->FUN_FGROUP6, sfs->FUN_FGROUP6); // Function Selection of Function Group 6
	printf("sfs->FUN_FGROUP5  [395:392] : 0x%08X(%u)\n", sfs->FUN_FGROUP5, sfs->FUN_FGROUP5); // Function Selection of Function Group 5
	printf("sfs->FUN_FGROUP4  [391:388] : 0x%08X(%u)\n", sfs->FUN_FGROUP4, sfs->FUN_FGROUP4); // Function Selection of Function Group 4
	printf("sfs->FUN_FGROUP3  [387:384] : 0x%08X(%u)\n", sfs->FUN_FGROUP3, sfs->FUN_FGROUP3); // Function Selection of Function Group 3
	printf("sfs->FUN_FGROUP2  [383:380] : 0x%08X(%u)\n", sfs->FUN_FGROUP2, sfs->FUN_FGROUP2); // Function Selection of Function Group 2
	printf("sfs->FUN_FGROUP1  [379:376] : 0x%08X(%u)\n", sfs->FUN_FGROUP1, sfs->FUN_FGROUP1); // Function Selection of Function Group 1
	printf("sfs->DATA_STR_VER [375:368] : 0x%08X(%u)\n", sfs->DATA_STR_VER, sfs->DATA_STR_VER); // Data Structure Version
	printf("sfs->RES_FGROUP6  [367:352] : 0x%08X(%u)\n", sfs->RES_FGROUP6, sfs->RES_FGROUP6); // Busy Status of Functions in Group 6
	printf("sfs->RES_FGROUP5  [351:336] : 0x%08X(%u)\n", sfs->RES_FGROUP5, sfs->RES_FGROUP5); // Busy Status of Functions in Group 5
	printf("sfs->RES_FGROUP4  [335:320] : 0x%08X(%u)\n", sfs->RES_FGROUP4, sfs->RES_FGROUP4); // Busy Status of Functions in Group 4
	printf("sfs->RES_FGROUP3  [319:304] : 0x%08X(%u)\n", sfs->RES_FGROUP3, sfs->RES_FGROUP3); // Busy Status of Functions in Group 3
	printf("sfs->RES_FGROUP2  [303:288] : 0x%08X(%u)\n", sfs->RES_FGROUP2, sfs->RES_FGROUP2); // Busy Status of Functions in Group 2
	printf("sfs->RES_FGROUP1  [287:272] : 0x%08X(%u)\n", sfs->RES_FGROUP1, sfs->RES_FGROUP1); // Busy Status of Functions in Group 1
	printf("sfs->_res0        [271:256] : 0x%08X(%u)\n", sfs->_res0, sfs->_res0);
	printf("sfs->_res1        [255:224] : 0x%08X(%u)\n", sfs->_res1, sfs->_res1);
	printf("sfs->_res2        [223:192] : 0x%08X(%u)\n", sfs->_res2, sfs->_res2);
	printf("sfs->_res3        [191:160] : 0x%08X(%u)\n", sfs->_res3, sfs->_res3);
	printf("sfs->_res4        [159:128] : 0x%08X(%u)\n", sfs->_res4, sfs->_res4);
	printf("sfs->_res5        [127: 96] : 0x%08X(%u)\n", sfs->_res5, sfs->_res5);
	printf("sfs->_res6        [ 95: 64] : 0x%08X(%u)\n", sfs->_res6, sfs->_res6);
	printf("sfs->_res7        [ 63: 32] : 0x%08X(%u)\n", sfs->_res7, sfs->_res7);
	printf("sfs->_res8        [ 31:  0] : 0x%08X(%u)\n", sfs->_res8, sfs->_res8);
	printf("===================================================\n");

	ENX_LOG_END(DBG_SDIO_CD_CMD);
}

//-------------------------------------------------------------------------------------------------
// SDIO-CD Command

static UINT SdioCdAppCmd(UINT nRCA)
{	// CMD55(R1) : Application-Specific Command
	UINT nResp = 0;
	return SdioCmd(sdinfo.nCH, SDCMD_APP_CMD, nRCA, &nResp, ecrtR1);
}

static UINT SdioCdReset(void)
{	// CMD0(R1), 8(R7) : Power on(Reset)
	UINT nResp = 0, nRes, nArg;

	SdioCmd(sdinfo.nCH, SDCMD_GO_IDLE_STATE, 0x00000000, NULL, ecrtR1);
	DELAY_MS(1);

	SdioCmd(sdinfo.nCH, SDCMD_GO_IDLE_STATE, 0x00000000, NULL, ecrtR1);
	DELAY_MS(1);

	nArg = CMD8_VHS | CMD8_PATTERN;
	nRes = SdioCmd(sdinfo.nCH, SDCMD_SEND_IF_COND, nArg, &nResp, ecrtR7);

	if ((nRes == DEF_OK) && (nResp & nArg)) {
		ENX_LOG_END(DBG_SDIO_CD_CMD);
		return DEF_OK;
	} else {
		ENX_DEBUGF(DBG_SDIO_CD_ERR, "End(FAIL) : RESP(0x%08X)\n", nResp);
		ENX_LOG_END(DBG_SDIO_CD_CMD);
		return DEF_FAIL;
	}
}

static UINT SdioCdInitialization(void)
{	// ACMD41(R3)
	UINT *pResp;
	UINT nRes, nArg, nTryCnt = 0;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	nArg = ACMD41_HCS | ACMD41_XPC | ACMD41_VOLT;
#ifdef sdio_support_1_8v
	nArg |= ACMD41_S18R;
#endif
	pResp = (UINT *)&sdinfo.ocr;

	for (UINT nTryCnt = SD_TRY_CNT; nTryCnt; nTryCnt--) {
		if (SdioCdDet()) {
			ENX_DEBUGF(DBG_SDIO_CD_ERR, "End(FAIL) : SDcard no detect\n");
			nRes = DEF_FAIL;
			break;
		}

		SdioCdAppCmd(0);
		nRes = SdioCmd(sdinfo.nCH, SDCMD_SD_SEND_OP_COND, nArg, pResp, ecrtR3);

		if (nRes == DEF_OK) {
			if (sdinfo.ocr.busy == 1) {
				nRes = DEF_OK;
				break;
			} else {
				nRes = DEF_FAIL;
			}
		}

		DELAY_MS(10); // Polling less than 10ms interval
	}

#if (DBG_SDIO_CD_CMD & ENX_DBG_STATE)
	SdioPrintOCR(&sdinfo.ocr);
#endif

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return DEF_OK;
}

static UINT SdioCdSwitchVoltage(void)
{	// CMD11(R1) : Switch to 1.8V
	UINT nResp = 0, nRes;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	nRes = SdioCmd(sdinfo.nCH, SDCMD_VOLTAGE_SWITCH, 0x00000000, &nResp, ecrtR1);

	// Switch 1.8V
	if (nRes == DEF_OK) {
//		PAD_SDIO0_CMD_PU = 1;		// CMD pull-up disable
//		PAD_SDIO0_DAT_PU = 1;		// DAT pull-up disable
		SdioClockDisable(sdinfo.nCH);	// CLK disable
		DELAY_MS(5);				// 5ms delay(min.)

		ENX_DEBUGF(DBG_SDIO_CD_CMD, "Set Default speed mode\n");
		SdioSetClockDiv(sdinfo.nCH, 25*1000*1000);

//		PAD_SDIO0_MSEL = 0x3F;		// 1.8v
		SdioClockEnable(sdinfo.nCH);// CLK enable
//		PAD_SDIO0_CMD_PU = 0;		// CMD pull-up enable
//		PAD_SDIO0_DAT_PU = 0;		// DAT pull-up enable

		sdinfo.nVoltageMode = 1;
		ENX_DEBUGF(DBG_SDIO_CD_ERR, "3.3v -> 1.8v ok\n");
	} else {
		sdinfo.nVoltageMode = 0;
		ENX_DEBUGF(DBG_SDIO_CD_ERR, "3.3v -> 1.8v fail\n");
	}

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioCdGetCID(int nType)
{	// CMD2, 10(R2): Type:0(CMD2) Type:1(CMD10)
	UINT *pResp;
	UINT nRes = DEF_FAIL;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	pResp = (UINT *)&sdinfo.cid;

	if (nType == 0) {
		nRes = SdioCmd(sdinfo.nCH, SDCMD_ALL_SEND_CID, 0x00000000, pResp, ecrtR2);
	} else if (nType == 1) {
		nRes = SdioCmd(sdinfo.nCH, SDCMD_SEND_CID, sdinfo.rca, pResp, ecrtR2);
	}

#if (DBG_SDIO_CD_CMD & ENX_DBG_STATE)
	SdioPrintCID(&sdinfo.cid);
#endif

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioCdGetRCA(void)
{	// CMD3(R6)
	UINT nResp = 0, nRes;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	nRes = SdioCmd(sdinfo.nCH, SDCMD_SEND_RELATIVE_ADDR, 0x00000000, &nResp, ecrtR6);

	sdinfo.rca = nResp & RCA_RCA_MASK;

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioCdGetCSD(void)
{	// CMD9(R2)
	UINT *pResp;
	UINT nRes;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	pResp = (UINT *)&sdinfo.csd;

	nRes = SdioCmd(sdinfo.nCH, SDCMD_SEND_CSD, sdinfo.rca, pResp, ecrtR2);

#if (DBG_SDIO_CD_CMD & ENX_DBG_STATE)
	SdioPrintCSD(&sdinfo.csd);
#endif

	if ((nRes == DEF_OK) && (sdinfo.csd.csd_v2.PERM_WRITE_PROTECT == 1)) {
		ENX_DEBUGF(DBG_SDIO_CD_ERR, "PERM_WRITE_PROTECT enable!\n");
		nRes = DEF_FAIL;
	}

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioCdBusWidthChange(void)
{	// CMD7(R1b), CMD42(R1), ACMD6(R1) : Bus Width 1bit -> 4bit
	UINT nResp = 0, nRes, nArg;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	// Select a card
	nRes = SdioCmd(sdinfo.nCH, SDCMD_SELECT_CARD, sdinfo.rca, &nResp, ecrtR1b);
#if 0
	// Unlock
	nRes = SdioCmd(sdinfo.nCH, SDCMD_LOCK_UNLOCK, 0x00000000, &nResp, ecrtR1b);
#endif
#if 1
	// ACMD6 : 1bit -> 4bit
	SdioCdAppCmd(sdinfo.rca);
	nArg = SD_BUS_WIDTH_4;
	nRes = SdioCmd(sdinfo.nCH, SDCMD_SET_BUS_WIDTH, nArg, &nResp, ecrtR1);

	if (nRes == DEF_OK) {
		SdioSet4BitMode(sdinfo.nCH);
	}
#endif
	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioCdRegDataRead(UINT nCmd, UINT nArg, UINT *nResp, eCmdRespType cmdType, void *pSdioReg, UINT nSize)
{
	UINT nRes, nTemp;
	UINT *getData = (UINT *)SDIO_DATA_BASE;

	nTemp = SdioGetDataBlockByte(sdinfo.nCH);
	SdioSetDataBlockByte(sdinfo.nCH, nSize);		// Data Block Byte를 설정
	SdioSetCmdDataS(sdinfo.nCH, esditREAD, nCmd);	// Data Read command를 설정

	ENX_SDIOCD_IRQ_LOCK();
	ENX_SDIOCD_FLUSH_DCACHE((ULONG)getData, nSize);
	nRes = SdioDataIO(sdinfo.nCH, esditREAD, (ULONG)getData, nArg, 1);
	ENX_SDIOCD_FLUSH_DCACHE((ULONG)getData, nSize);
	memcpy(pSdioReg, getData, nSize);
	ENX_SDIOCD_IRQ_UNLOCK();

	SdioSetDataBlockByte(sdinfo.nCH, nTemp);	// Data Block Byte를 원상복귀
	SdioSetCmdDataS(sdinfo.nCH, esditREAD, SDCMD_READ_SINGLE_BLOCK); // Data Read command를 17으로 설정(기본 Read 명령으로 설정)

	SdioGetResp(sdinfo.nCH, nResp, cmdType);
	ENX_DEBUGF(DBG_SDIO_CD_CMD, "[%2d] res(%c) RESP0(0x%08X)\n", nCmd, nRes == DEF_OK ? 'O' : 'X', nResp[0]);
	if (cmdType == ecrtR2) {
		ENX_DEBUGF(DBG_SDIO_CD_CMD, "[%2d] res(%c) RESP1(0x%08X)\n", nCmd, nRes == DEF_OK ? 'O' : 'X', nResp[1]);
		ENX_DEBUGF(DBG_SDIO_CD_CMD, "[%2d] res(%c) RESP2(0x%08X)\n", nCmd, nRes == DEF_OK ? 'O' : 'X', nResp[2]);
		ENX_DEBUGF(DBG_SDIO_CD_CMD, "[%2d] res(%c) RESP3(0x%08X)\n", nCmd, nRes == DEF_OK ? 'O' : 'X', nResp[3]);
	}

	return nRes;
}

static UINT SdioCdGetSSR(void)
{	// ACMD13(R1) : Data bus response / 64byte
	UINT nResp = 0, nRes;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	SdioCdAppCmd(sdinfo.rca);
	nRes = SdioCdRegDataRead(SDCMD_SD_STATUS, 0x00000000, &nResp, ecrtR1, &sdinfo.ssr, sizeof(SD_SSR));

#if (DBG_SDIO_CD_CMD & ENX_DBG_STATE)
	if (nRes == DEF_OK) {
		SdioPrintSSR(&sdinfo.ssr);
	}
#endif

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioCdGetSCR(void)
{	// ACMD51(R1) : Data bus response / 8byte
	UINT nRes = DEF_FAIL;
	UINT nResp, nTemp = 0;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	SdioCdAppCmd(sdinfo.rca);
	nRes = SdioCdRegDataRead(SDCMD_SEND_SCR, 0x00000000, &nResp, ecrtR1, &sdinfo.scr, sizeof(SD_SCRreg));

#if (DBG_SDIO_CD_CMD & ENX_DBG_STATE)
	if (nRes == DEF_OK) {
		SdioPrintSCR(&sdinfo.scr);
	}
#endif

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioClockSwitch(UINT mode, UINT group, BYTE value)
{	// CMD6 : Check & Switch Clock (sub)
	UINT nResp = 0, nRes, nArg = 0;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	nArg = mode << 31 | 0x00FFFFFF;
	nArg &= ~(0xFF << (group * 4));
	nArg |= value << (group * 4);
	ENX_DEBUGF(DBG_SDIO_CD_CMD, "nArg(mode:%u) : 0x%08X\n", mode, nArg);

	nRes = SdioCdRegDataRead(SDCMD_SWITCH_FUNC, nArg, &nResp, ecrtR1, &sdinfo.sfs, sizeof(SD_SFS));

#if (DBG_SDIO_CD_CMD & ENX_DBG_STATE)
	if (nRes == DEF_OK) {
		SdioPrintSFS(&sdinfo.sfs);
	}
#endif

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioCdSetClock(void)
{	// CMD6 : Check & Switch Clock (main)
	UINT nRes = DEF_FAIL;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	if (sdinfo.ssr.SPEED_CLASS >= SD_SPEED_CLASS10) {
		if (sdinfo.scr.SD_SPEC < SCR_SPEC_VER_1) {
			ENX_DEBUGF(DBG_SDIO_CD_ERR, "Spec ver 1.0 or 1.01\n");
			goto if_speed;
		}

		if (sdinfo.csd.csd_v1.CSD_STRUCTURE == 0) {
			if (!(sdinfo.csd.csd_v1.CCC & SCR_CCC_SWITCH)) {
				ENX_DEBUGF(DBG_SDIO_CD_ERR, "Card lacks mandatory switch function, performance might suffer.\n");
				goto if_speed;
			}
		} else if (sdinfo.csd.csd_v2.CSD_STRUCTURE == 1) {
			if (!(sdinfo.csd.csd_v2.CCC & SCR_CCC_SWITCH)) {
				ENX_DEBUGF(DBG_SDIO_CD_ERR, "Card lacks mandatory switch function, performance might suffer.\n");
				goto if_speed;
			}
		}

		if (SdioClockSwitch(0, 0, 1) == DEF_FAIL) {
			ENX_DEBUGF(DBG_SDIO_CD_ERR, "Problem reading switch capabilities, performance might suffer.\n");
			goto if_speed;
		}

		DELAY_MS(50);

		if (!(sdinfo.sfs.SUP_FGROUP1 & 0x2)) {
			ENX_DEBUGF(DBG_SDIO_CD_ERR, "\n");
			goto if_speed;
		}

		if (SdioClockSwitch(1, 0, 1) == DEF_FAIL) {
			ENX_DEBUGF(DBG_SDIO_CD_ERR, "Clock switching failed.\n");
			goto if_speed;
		}

		if ((sdinfo.sfs.FUN_FGROUP1 & 0xF) != 1) {
			ENX_DEBUGF(DBG_SDIO_CD_ERR, "Problem switching card into high-speed mode!\n");
		} else {
			ENX_DEBUGF(DBG_SDIO_CD_CMD, "Set High speed mode\n");
//			SdioSetClockDiv(sdinfo.nCH, 0);
//			SdioSetClockDiv(sdinfo.nCH, 1);
			SdioSetClockDiv(sdinfo.nCH, 6);
		}

		nRes = DEF_OK;
	} else {
if_speed:
		ENX_DEBUGF(DBG_SDIO_CD_CMD, "Set Default speed mode\n");
//		SdioSetClockDiv(sdinfo.nCH, 1);
		SdioSetClockDiv(sdinfo.nCH, 10);
		nRes = DEF_OK;
	}

	sdinfo.nSetClock = SdioGetClockDiv(sdinfo.nCH);

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioCdTuningCommand(void)
{	// CMD19(R1) : 4.2.4.5 Tuning Command
	UINT nResp = 0, nRes, nTemp = 0, nTry = 0;
	const BYTE checkData[64] = {0xFF,0x0F,0xFF,0x00,0xFF,0xCC,0xC3,0xCC,0xC3,0x3C,0xCC,0xFF,0xFE,0xFF,0xFE,0xEF,
								0xFF,0xDF,0xFF,0xDD,0xFF,0xFB,0xFF,0xFB,0xBF,0xFF,0x7F,0xFF,0x77,0xF7,0xBD,0xEF,
								0xFF,0xF0,0xFF,0xF0,0x0F,0xFC,0xCC,0x3C,0xCC,0x33,0xCC,0xCF,0xFF,0xEF,0xFF,0xEE,
								0xFF,0xFD,0xFF,0xFD,0xDF,0xFF,0xBF,0xFF,0xBB,0xFF,0xF7,0xFF,0xF7,0x7F,0x7B,0xDE};

	ENX_LOG_START(DBG_SDIO_CD_CMD);

sd_tuning:
	nRes = SdioCdRegDataRead(SDCMD_SEND_TUNING_BLOCK, 0x00000000, &nResp, ecrtR1, &sdinfo.stuning, sizeof(SD_Tuning));

	if (nRes == DEF_OK) {
		for (UINT i = 0; i < sizeof(SD_Tuning); i++) {
			if (checkData[i] != sdinfo.stuning.Tuning[i]) {
				hexDump("Tuning Block Pattern Mismatch", sdinfo.stuning.Tuning, sizeof(SD_Tuning));
				SdioCdClockDown(); // 주파수 낮추고 재시도(old)
				// 틀어진 bit를 찾아 해당 delay sel을 수정하는 형태로 가야 함.(new/개발필요)
				if (nTry == 0) {
					nTry++;
					goto sd_tuning;
				} else {
					nRes = DEF_FAIL;
				}
				break;
			}
		}
	}

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

static UINT SdioCdSetBlockLength(void)
{	// CMD16(R1) : Define the block length
	UINT nResp = 0, nRes, nArg = 0;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	nArg = SdioGetDataBlockByte(sdinfo.nCH);
	nRes = SdioCmd(sdinfo.nCH, SDCMD_SET_BLOCKLEN, nArg, &nResp, ecrtR1);

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

////////////////////////////////////////////////////////////////////////////////
// SDIO-CD Process
static uint64_t sdio_data_address = 1;

UINT SdioCdInitProcess(void)
{
	ENX_LOG_START(DBG_SDIO_CD_CMD);

#if (DBG_SDIO_CD_CMD & ENX_DBG_STATE)
	printf("%20s SP-TEST [0x%08X]\n", __func__, sp);
	printf("SDIO-SD reg [0x%08X] nCH\n", (UINT *)&sdinfo.nCH);
	printf("SDIO-SD reg [0x%08X] nErrorCode\n", (UINT *)&sdinfo.nErrorCode);
	printf("SDIO-SD reg [0x%08X] nActive\n", (UINT *)&sdinfo.nActive);
	printf("SDIO-SD reg [0x%08X] nVoltageMode\n", (UINT *)&sdinfo.nVoltageMode);
	printf("SDIO-SD reg [0x%08X] nSetClock\n", (UINT *)&sdinfo.nSetClock);
	printf("SDIO-SD reg [0x%08X] rca\n", (UINT *)&sdinfo.rca);
	printf("SDIO-SD reg [0x%08X] ocr(%ubyte)\n", (UINT *)&sdinfo.ocr, sizeof(sdinfo.ocr));
	printf("SDIO-SD reg [0x%08X] cid(%ubyte)\n", (UINT *)&sdinfo.cid, sizeof(sdinfo.cid));
	printf("SDIO-SD reg [0x%08X] csd(%ubyte)\n", (UINT *)&sdinfo.csd, sizeof(sdinfo.csd));
	printf("SDIO-SD reg [0x%08X] sc(%ubyte)\n", (UINT *)&sdinfo.sc, sizeof(sdinfo.sc));
	printf("SDIO-SD reg [0x%08X] scr(%ubyte)\n", (UINT *)&sdinfo.scr, sizeof(sdinfo.scr));
	printf("SDIO-SD reg [0x%08X] ssr(%ubyte)\n", (UINT *)&sdinfo.ssr, sizeof(sdinfo.ssr));
	printf("SDIO-SD reg [0x%08X] sfs(%ubyte)\n", (UINT *)&sdinfo.sfs, sizeof(sdinfo.sfs));
	printf("SDIO-SD reg [0x%08X] stuning(%ubyte)\n", (UINT *)&sdinfo.stuning, sizeof(sdinfo.stuning));
#endif

	UINT nCH = sdinfo.nCH;
	memset(&sdinfo, 0, sizeof(sdinfo));
	sdinfo.nCH = nCH;
#if 1
	BYTE *pSDIO_DATA_BASE;
	if (SDIO_DATA_BASE == NULL) {
		SDIO_DATA_BASE = pvPortMalloc(1024);
		sdio_data_address = (uint64_t)SDIO_DATA_BASE;
		for (int k = 0; k < 1024; k++) {
			SDIO_DATA_BASE[k] = 0xA5;
		}
	}
	pSDIO_DATA_BASE = SDIO_DATA_BASE;
	printf("SDIO_DATA_BASE: 0x%08X\n", SDIO_DATA_BASE);
#endif
	SdioSetClockDef(sdinfo.nCH);
	char strClockPrint[40] = {0};
#if (DBG_SDIO_CD_CMD & ENX_DBG_STATE)
	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Init Start(%s)\n", sdinfo.nCH, strClockPrint);
#endif

	// CMD 0, 8
	if (SdioCdReset() == DEF_FAIL) {
		sdinfo.nErrorCode = 1;
		goto done_fail;
	}

	// ACMD 41
	if (SdioCdInitialization() == DEF_FAIL) {
		sdinfo.nErrorCode = 2;
		goto done_fail;
	}

	sdinfo.nVoltageMode = 0;
	if (sdinfo.ocr.VDD18 && sdinfo.ocr.CCS) {
		// CMD 11
		if (SdioCdSwitchVoltage() == DEF_FAIL) {
			sdinfo.nErrorCode = 3;
		} else {
			sdinfo.nVoltageMode = 1;
		}
	}

	// CMD 2
	if (SdioCdGetCID(0) == DEF_FAIL) {
		sdinfo.nErrorCode = 4;
		goto done_fail;
	}

	// CMD 3
	if (SdioCdGetRCA() == DEF_FAIL) {
		sdinfo.nErrorCode = 5;
		goto done_fail;
	}

	// CMD 9
	if (SdioCdGetCSD() == DEF_FAIL) {
		sdinfo.nErrorCode = 6;
		goto done_fail;
	}

	// CMD 10
	if (SdioCdGetCID(1) == DEF_FAIL) {
		sdinfo.nErrorCode = 7;
		goto done_fail;
	}

	// CMD 7, 42, ACMD 6
	if (SdioCdBusWidthChange() == DEF_FAIL) {
		sdinfo.nErrorCode = 8;
		goto done_fail;
	}

	// ACMD 13
	if (SdioCdGetSSR() == DEF_FAIL) {
		sdinfo.nErrorCode = 9;
		goto done_fail;
	}

	// ACMD 51
	if (SdioCdGetSCR() == DEF_FAIL) {
		sdinfo.nErrorCode = 10;
		goto done_fail;
	}
	if (sdio_data_address != (uint64_t)SDIO_DATA_BASE) {
		printf("Error!!\n");
		printf("SDIO data address 0x%08X -> 0x%08X\n", sdio_data_address, SDIO_DATA_BASE);
		ENX_SDIOCD_FLUSH_DCACHE((ULONG)pSDIO_DATA_BASE, 1024);
		hexDump("pSDIO_DATA_BASE", pSDIO_DATA_BASE, 1024);
		ENX_SDIOCD_FLUSH_DCACHE((ULONG)SDIO_DATA_BASE, 1024);
		hexDump("SDIO_DATA_BASE", SDIO_DATA_BASE, 1024);
	}

	// CMD 6
	if (SdioCdSetClock() == DEF_FAIL) {
		sdinfo.nErrorCode = 11;
		goto done_fail;
	}

	// CMD 19
	if(sdinfo.nVoltageMode == 1) {	// UHS-1 1.8v
		if (SdioCdTuningCommand() == DEF_FAIL) {
			sdinfo.nErrorCode = 12;
			goto done_fail;
		}
	}

	// CMD 16
	if (SdioCdSetBlockLength() == DEF_FAIL) {
		sdinfo.nErrorCode = 13;
		goto done_fail;
	}

	sdinfo.nActive = 1;

	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Init OK(%s)\n", sdinfo.nCH, strClockPrint);

	ENX_LOG_END(DBG_SDIO_CD_CMD);
	return DEF_OK;
done_fail:
	SdioClockDivPrint(sdinfo.nCH, strClockPrint);
	printf("SDIO%u(MicroSD) Init Fail(err:%u)(%s)\n", sdinfo.nCH, sdinfo.nErrorCode, strClockPrint);

	ENX_LOG_END(DBG_SDIO_CD_CMD);
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
	ENX_LOG_START(DBG_SDIO_CD_CMD);

	UINT nSector = 0;
	if (sdinfo.csd.csd_v1.CSD_STRUCTURE == 0) { // CSD 1.0
		nSector = ((((sdinfo.csd.csd_v1.C_SIZE0 << 10) | (sdinfo.csd.csd_v1.C_SIZE1)) + 1) * (0x1 << (sdinfo.csd.csd_v1.C_SIZE_MULT + 2)) * (0x1 << (sdinfo.csd.csd_v1.READ_BL_LEN))) >> 9;
		printf("CSD1.0 : C_SIZE(%u)\n", sdinfo.csd.csd_v1.C_SIZE0 << 10 | sdinfo.csd.csd_v1.C_SIZE1);
		printf("         C_SIZE_MULT(%u)\n", sdinfo.csd.csd_v1.C_SIZE_MULT);
		printf("         BLOCK_LEN(%u)\n", sdinfo.csd.csd_v1.READ_BL_LEN);
		printf("         SECTOR(%u)\n", nSector);
	} else if (sdinfo.csd.csd_v1.CSD_STRUCTURE == 1) { // CSD 2.0
		nSector = (((sdinfo.csd.csd_v2.C_SIZE0 << 6) | (sdinfo.csd.csd_v2.C_SIZE1)) + 1) << 10;
		printf("CSD2.0 : C_SIZE(%u)\n", sdinfo.csd.csd_v2.C_SIZE0 << 6 | sdinfo.csd.csd_v2.C_SIZE1);
		printf("         SECTOR(%u)\n", nSector);
	} else {
		ENX_DEBUGF(DBG_SDIO_CD_ERR, "CSD Version error(%d)\n", sdinfo.csd.csd_v1.CSD_STRUCTURE);
	}

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nSector;
}

UINT SdioCdGetSize(void)
{
	UINT nSector = 0;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	if (sdinfo.csd.csd_v1.CSD_STRUCTURE == 0) {
		SD_CSD_v1 *csd = &(sdinfo.csd.csd_v1);
		nSector = (((csd->C_SIZE0 << 10 | csd->C_SIZE1) + 1) * (0x1 << (csd->C_SIZE_MULT + 2)) * (0x1 << (csd->READ_BL_LEN))) >> 9;
	} else if (sdinfo.csd.csd_v1.CSD_STRUCTURE == 1) {
		SD_CSD_v2 *csd = &(sdinfo.csd.csd_v2);
		nSector = ((csd->C_SIZE0 << 6 | csd->C_SIZE1) + 1) << 10;
	} else {
		ENX_DEBUGF(DBG_SDIO_CD_ERR, "CSD Version error(%d)\n", sdinfo.csd.csd_v1.CSD_STRUCTURE);
	}

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nSector >> 11;
}

UINT SdioCdE(UINT start_sctor, UINT end_sctor)
{	// CMD32(R1), 33(R1), 38(R1b) : Erase
	UINT nResp = 0, nRes, nBlocSp, nBlocEp;

	ENX_LOG_START(DBG_SDIO_CD_CMD);

	if (sdinfo.ocr.CCS) {
		nBlocSp = start_sctor;
		nBlocEp = end_sctor;
	} else {
		nBlocSp = start_sctor << 9;
		nBlocEp = end_sctor << 9;
	}

	// CMD32 : Erase start address
	nRes = SdioCmd(sdinfo.nCH, SDCMD_ERASE_WR_BLK_START, nBlocSp, &nResp, ecrtR1);

	// CMD33 : Erase end address
	nRes = SdioCmd(sdinfo.nCH, SDCMD_ERASE_WR_BLK_END, nBlocEp, &nResp, ecrtR1);

	// CMD38 : Erase command
	nRes = SdioCmd(sdinfo.nCH, SDCMD_ERASE, 0x00000000, &nResp, ecrtR1b);

	ENX_LOG_END(DBG_SDIO_CD_CMD);

	return nRes;
}

UINT SdioCdRead(const BYTE *buff, UINT sector, UINT count)
{
	UINT nResp = 0, nRes, nAddr = 0, nGetByte = 0;
	UINT *getData = (UINT *)buff;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

	while (SdioIsDataEn(sdinfo.nCH) == 1) {
		ENX_SDIOCD_DELAY(0);
	}

	nGetByte = SdioGetDataBlockByte(sdinfo.nCH) * count;

	ENX_SDIOCD_IRQ_LOCK();
	ENX_SDIOCD_FLUSH_DCACHE((ULONG)getData, nGetByte);
	printf("READ(%c)-start, Addr(0x%08X) Size(%u)\n", count == 1 ? 'S' : 'M', getData, nGetByte);
	nRes = SdioDataIO(sdinfo.nCH, esditREAD, (ULONG)getData, nAddr, count);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	ENX_SDIOCD_FLUSH_DCACHE((ULONG)getData, nGetByte);
	ENX_SDIOCD_IRQ_UNLOCK();

	hexDump("READ-done", getData, nGetByte);
	ENX_DEBUGF(DBG_SDIO_CD_DAT, "[%2u] res(%d) RESP(0x%08X)\n",
			count == 1 ? SdioGetCmdDataS(sdinfo.nCH, esditREAD) : SdioGetCmdDataM(sdinfo.nCH, esditREAD), nRes, nResp);

	return nRes;
}

UINT SdioCdWrite(const BYTE *buff, UINT sector, UINT count)
{
	UINT nResp = 0, nRes, nAddr = 0, nGetByte = 0;
	UINT *getData = (UINT *)buff;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

	while (SdioIsDataEn(sdinfo.nCH) == 1) {
		ENX_SDIOCD_DELAY(0);
	}

	nGetByte = SdioGetDataBlockByte(sdinfo.nCH) * count;

	ENX_SDIOCD_IRQ_LOCK();
	ENX_SDIOCD_FLUSH_DCACHE((ULONG)getData, nGetByte);
	hexDump("WRITE-start", getData, nGetByte);
	nRes = SdioDataIO(sdinfo.nCH, esditWRITE, (ULONG)getData, nAddr, count);
	SdioGetResp(sdinfo.nCH, &nResp, ecrtR1);
	ENX_SDIOCD_IRQ_UNLOCK();

	printf("WRITE(%c)-done, Addr(0x%08X) Size(%u)\n", count == 1 ? 'S' : 'M', getData, nGetByte);
	ENX_DEBUGF(DBG_SDIO_CD_DAT, "[%2u] res(%d) RESP(0x%08X) \n",
			count == 1 ? SdioGetCmdDataS(sdinfo.nCH, esditWRITE) : SdioGetCmdDataM(sdinfo.nCH, esditWRITE), nRes, nResp);

	return nRes;
}
#endif
