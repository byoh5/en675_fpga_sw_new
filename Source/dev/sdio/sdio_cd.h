#ifndef _SDIO_CD_H_
#define	_SDIO_CD_H_

#if defined(__USE_SDIOCD__)
/* Standard SD commands (4.0)               type | argument               | response */

/* Class 0 - Basic Commands */
#define SDCMD_GO_IDLE_STATE            0 /* bc   |                             | -   */
#define SDCMD_ALL_SEND_CID             2 /* bcr  |                             | R2  */
#define SDCMD_SEND_RELATIVE_ADDR       3 /* bcr  |                             | R6  */
#define SDCMD_SET_DSR                  4 /* bc   | [31:16] DSR                 | -   */
#define SDCMD_SELECT_CARD              7 /* ac   | [31:16] RCA                 | R1b */
#define SDCMD_SEND_IF_COND             8 /* bcr  | [11: 8] VHS [ 7: 0] Check Pattern | R7  */
#define SDCMD_SEND_CSD                 9 /* ac   | [31:16] RCA                 | R2  */
#define SDCMD_SEND_CID                10 /* ac   | [31:16] RCA                 | R2  */
#define SDCMD_VOLTAGE_SWITCH          11 /* ac   | [31: 0] 0                   | R1  */
#define SDCMD_STOP_TRANSMISSION       12 /* ac   |                             | R1b */
#define SDCMD_SEND_STATUS             13 /* ac   | [31:16] RCA                 | R1  */
#define SDCMD_GO_INACTIVE_STATE       15 /* ac   | [31:16] RCA                 | -   */

/* Class 2 - Block-Oriented Read Commands */
#define SDCMD_SET_BLOCKLEN            16 /* ac   | [31: 0] Block Length        | R1  */
#define SDCMD_READ_SINGLE_BLOCK       17 /* adtc | [31: 0] Data Address        | R1  */
#define SDCMD_READ_MULTIPLE_BLOCK     18 /* adtc | [31: 0] Data Address        | R1  */
#define SDCMD_SEND_TUNING_BLOCK       19 /* adtc | [31: 0] 0                   | R1  */
#define SDCMD_SPEED_CLASS_CONTROL     20 /* ac   | [31:28] SCC [27: 0] 0       | R1  */
#define SDCMD_SET_BLOCK_COUNT         23 /* ac   | [31: 0] Block Count         | R1  */

/* Class 4 - Block-Oriented Write Commands */
#define SDCMD_WRITE_BLOCK             24 /* adtc | [31: 0] Data Address        | R1  */
#define SDCMD_WRITE_MULTIPLE_BLOCK    25 /* adtc | [31: 0] Data Address        | R1  */
#define SDCMD_PROGRAM_CSD             27 /* adtc |                             | R1  */

/* Class 5 - Erase Commands */
#define SDCMD_ERASE_WR_BLK_START      32 /* ac   | [31: 0] Data Address        | R1  */
#define SDCMD_ERASE_WR_BLK_END        33 /* ac   | [31: 0] Data Address        | R1  */
#define SDCMD_ERASE                   38 /* ac   |                             | R1b */

/* Class 6 - Block-Oriented Write Protection Commands */
#define SDCMD_SET_WRITE_PROT          28 /* ac   | [31: 0] Data Address        | R1b */
#define SDCMD_CLR_WRITE_PROT          29 /* ac   | [31: 0] Data Address        | R1b */
#define SDCMD_SEND_WRITE_PROT         30 /* adtc | [31: 0] WPData Address      | R1  */

/* Class 7 - Lock Card */
#define SDCMD_LOCK_UNLOCK             42 /* adtc | [31: 0] 0                   | R1  */

/* Class 8 - Application-Specific Commands */
#define SDCMD_APP_CMD                 55 /* ac   | [31:16] RCA                 | R1  */
#define SDCMD_GEN_CMD                 56 /* adtc | [ 0] RD/WR                  | R1  */

/* Class 10 - Switch Function Commands */
#define SDCMD_SWITCH_FUNC              6 /* adtc | [31] mode [15:12] G4 [11: 8] G3 [ 7: 4] G2 [ 3: 0] G1 | R1  */

/* Application Specific Commands */
#define SDCMD_SET_BUS_WIDTH            6 /* ac   | [ 1: 0] Bus Width           | R1  */
#define SDCMD_SD_STATUS               13 /* adtc |                             | R1  */
#define SDCMD_SEND_NUM_WR_BLOCKS      22 /* adtc |                             | R1  */
#define SDCMD_SET_WR_BLK_ERASE_COUNT  23 /* ac   | [22: 0] Number of Blocks    | R1  */
#define SDCMD_SD_SEND_OP_COND         41 /* ac   | [30] HCS [28] XPC [24] S18R [23: 0] Vdd Voltage Window | R3  */
#define SDCMD_SET_CLR_CARD_DETECT     42 /* ac   | [ 0] Set_CD                 | R1  */
#define SDCMD_SEND_SCR                51 /* adtc |                             | R1  */

enum {
	SD_SPEED_CLASS0,
	SD_SPEED_CLASS2,
	SD_SPEED_CLASS4,
	SD_SPEED_CLASS6,
	SD_SPEED_CLASS10,
	SD_SPEED_REV
};

typedef ATTR_BIGENDIAN ATTR_MALIGN16 struct {
	uint32 DAT_BUS_WIDTH				:2;
	uint32 SECURED_MODE 				:1;
	uint32 RSF							:7;
	uint32 _res1						:6;
	uint32 SD_CARD_TYPE 				:16;
	uint32 SIZE_OF_PROTECTED_AREA;
	uint32 SPEED_CLASS					:8;
	uint32 PERFORMANCE_MOVE 			:8;
	uint32 AU_SIZE						:4;
	uint32 _res2						:4;
	uint32 ERASE_SIZE					:16;
	uint32 ERASE_TIMEOUT				:6;
	uint32 ERASE_OFFSET 				:2;
	uint32 UHS_SPEED_GRADE				:4;
	uint32 UHS_AU_SIZE					:4;
	uint32 _res3						:8;
	uint32 _res4;
	uint32 _res5;
	uint32 _res6;
	uint32 _res7;
	uint32 _res8;
	uint32 _res9;
	uint32 _res10;
	uint32 _res11;
	uint32 _res12;
	uint32 _res13;
	uint32 _res14;
	uint32 _res15;
//	uint32 _res112[112];
} ATTR_PACKED SD_SSR;

typedef ATTR_BIGENDIAN ATTR_MALIGN16 struct {
	uint32 SCR_STRUCTURE 				:4;
	uint32 SD_SPEC						:4;
	uint32 DATA_STAT_AFTER_ERASE		:1;
	uint32 SD_SECURITY					:3;
	uint32 SD_BUS_WIDTHS				:4;
	uint32 SD_SPEC3						:1;
	uint32 EX_SECURITY					:4;
	uint32 _res1						:9;
	uint32 CMD_SUPPORT					:2;
	uint32 _res2;
//	uint32 _res112[112+14];
} ATTR_PACKED SD_SCRreg;

typedef ATTR_BIGENDIAN ATTR_MALIGN16 struct {
	uint32 MAX_CUR_PWR					:16;
	uint32 SUP_FGROUP6					:16;
	uint32 SUP_FGROUP5					:16;
	uint32 SUP_FGROUP4					:16;
	uint32 SUP_FGROUP3					:16;
	uint32 SUP_FGROUP2					:16;
	uint32 SUP_FGROUP1					:16;
	uint32 FUN_FGROUP6					:4;
	uint32 FUN_FGROUP5					:4;
	uint32 FUN_FGROUP4					:4;
	uint32 FUN_FGROUP3					:4;
	uint32 FUN_FGROUP2					:4;
	uint32 FUN_FGROUP1					:4;
	uint32 DATA_STR_VER					:8;
	uint32 RES_FGROUP6					:16;
	uint32 RES_FGROUP5					:16;
	uint32 RES_FGROUP4					:16;
	uint32 RES_FGROUP3					:16;
	uint32 RES_FGROUP2					:16;
	uint32 RES_FGROUP1					:16;
	uint32 _res0						:16;
	uint32 _res1;
	uint32 _res2;
	uint32 _res3;
	uint32 _res4;
	uint32 _res5;
	uint32 _res6;
	uint32 _res7;
	uint32 _res8;
//	uint32 _res112[112];
} ATTR_PACKED SD_SFS;

typedef ATTR_BIGENDIAN ATTR_MALIGN16 struct {
	uint8 Tuning[64];
//	uint32 _res112[112];
} ATTR_PACKED SD_Tuning;

typedef ATTR_MALIGN16 struct { BF_28(
	uint32 OUT_OF_RANGE 				:1,
	uint32 ADDRESS_ERROR 				:1,
	uint32 BLOCK_LEN_ERROR 				:1,
	uint32 ERASE_SEQ_ERROR				:1,
	uint32 ERASE_PARAM					:1,
	uint32 WP_VIOLATION					:1,
	uint32 CARD_IS_LOCKED				:1,
	uint32 LOCK_UNLOCK_FAILED 			:1,
	uint32 COM_CRC_ERROR				:1,
	uint32 ILLEGAL_COMMAND				:1,
	uint32 CARD_ECC_FAILED 				:1,
	uint32 CC_ERROR						:1,
	uint32 ERROR						:1,
	uint32 _res1 						:1,
	uint32 _res2 						:1,
	uint32 CSD_OVERWRITE	 			:1,
	uint32 WP_ERASE_SKIP				:1,
	uint32 CARD_ECC_DISABLED			:1,
	uint32 ERASE_RESET					:1,
	uint32 CURRENT_STATE				:4,
	uint32 READY_FOR_DATA				:1,
	uint32 _res3						:2,
	uint32 APP_CMD						:1,
	uint32 _res4						:1,
	uint32 AKE_SEQ_ERROR				:1,
	uint32 _res5						:1,
	uint32 _res6						:1,
	uint32 _res7						:1
)} ATTR_PACKED SD_SC;

typedef ATTR_MALIGN16 struct { BF_5(
	uint32 CSD_STRUCTURE				:2,
	uint32 _res1						:6,
	uint32 TAAC							:8,
	uint32 NSAC							:8,
	uint32 TRAN_SPEED					:8
)BF_8(
	uint32 CCC							:12,
	uint32 READ_BL_LEN					:4,
	uint32 READ_BL_PARTIAL				:1,
	uint32 WRITE_BLK_MISALIGN			:1,
	uint32 READ_BLK_MISALIGN			:1,
	uint32 DSR_IMP						:1,
	uint32 _res2						:2,
	uint32 C_SIZE0						:10
)BF_9(
	uint32 C_SIZE1						:2,
	uint32 VDD_R_CURR_MIN				:3,
	uint32 VDD_R_CURR_MAX				:3,
	uint32 VDD_W_CURR_MIN				:3,
	uint32 VDD_W_CURR_MAX				:3,
	uint32 C_SIZE_MULT					:3,
	uint32 ERASE_BLK_EN					:1,
	uint32 SECTOR_SIZE					:7,
	uint32 WP_GRP_SIZE					:7
)BF_14(
	uint32 WP_GRP_ENABLE				:1,
	uint32 _res3						:2,
	uint32 R2W_FACTOR					:3,
	uint32 WRITE_BL_LEN					:4,
	uint32 WRITE_BL_PARTIAL				:1,
	uint32 _res4						:5,
	uint32 FILE_FORMAT_GRP				:1,
	uint32 COPY							:1,
	uint32 PERM_WRITE_PROTECT			:1,
	uint32 TMP_WRITE_PROTECT 			:1,
	uint32 FILE_FORMAT 					:2,
	uint32 _res5 						:2,
	uint32 CRC 							:7,
	uint32 _res6						:1
)} ATTR_PACKED SD_CSD_v1;

typedef ATTR_MALIGN16 struct { BF_5(
	uint32 CSD_STRUCTURE				:2,
	uint32 _res1						:6,
	uint32 TAAC							:8,
	uint32 NSAC							:8,
	uint32 TRAN_SPEED					:8
)BF_8(
	uint32 CCC							:12,
	uint32 READ_BL_LEN					:4,
	uint32 READ_BL_PARTIAL				:1,
	uint32 WRITE_BLK_MISALIGN			:1,
	uint32 READ_BLK_MISALIGN			:1,
	uint32 DSR_IMP						:1,
	uint32 _res2						:6,
	uint32 C_SIZE0						:6
)BF_5(
	uint32 C_SIZE1						:16,
	uint32 _res3						:1,
	uint32 ERASE_BLK_EN					:1,
	uint32 SECTOR_SIZE					:7,
	uint32 WP_GRP_SIZE					:7
)BF_14(
	uint32 WP_GRP_ENABLE				:1,
	uint32 _res4						:2,
	uint32 R2W_FACTOR					:3,
	uint32 WRITE_BL_LEN					:4,
	uint32 WRITE_BL_PARTIAL				:1,
	uint32 _res5						:5,
	uint32 FILE_FORMAT_GRP				:1,
	uint32 COPY							:1,
	uint32 PERM_WRITE_PROTECT			:1,
	uint32 TMP_WRITE_PROTECT 			:1,
	uint32 FILE_FORMAT 					:2,
	uint32 _res6 						:2,
	uint32 CRC 							:7,
	uint32 _res7						:1
)} ATTR_PACKED SD_CSD_v2;

typedef ATTR_MALIGN16 union {
	UINT a[4];
	SD_CSD_v1 csd_v1;
	SD_CSD_v2 csd_v2;
} SD_CSD;

typedef ATTR_MALIGN16 struct { BF_17(
	uint32 busy 						:1,
	uint32 CCS 							:1,
	uint32 UHSIICS 						:1,
	uint32 _res1						:4,
	uint32 VDD18						:1,
	uint32 VDD35_36						:1,
	uint32 VDD34_35						:1,
	uint32 VDD33_34 					:1,
	uint32 VDD32_33						:1,
	uint32 VDD31_32						:1,
	uint32 VDD30_31 					:1,
	uint32 VDD29_30						:1,
	uint32 VDD28_29						:1,
	uint32 VDD27_28 					:1,
	uint32 _res2 						:7,
	uint32 VDDLOW	 					:1,
	uint32 _res3						:7
)} ATTR_PACKED SD_OCR;

typedef ATTR_MALIGN16 struct { BF_3(
	uint32 MID 							:8,
	uint32 OID							:16,
	uint32 PNM1							:8
)BF_4(
	uint32 PNM2							:8,
	uint32 PNM3							:8,
	uint32 PNM4							:8,
	uint32 PNM5							:8
)BF_2(
	uint32 PRV 							:8,
	uint32 PSN0							:24
)BF_5(
	uint32 PSN1							:8,
	uint32 _res1 						:4,
	uint32 MDT 							:12,
	uint32 CRC 							:7,
	uint32 _res2 						:1
)} ATTR_PACKED SD_CID;

typedef ATTR_MALIGN16 struct {
	UINT nCH;
	UINT nErrorCode;
	UINT nActive;
	UINT nVoltageMode;
	UINT nSetClock;

	UINT rca;
	SD_OCR ocr;
	SD_CID cid;
	SD_CSD csd;
	SD_SC sc;

	SD_SCRreg scr; // 8byte
	ULONG t1[7];
	SD_SSR ssr; // 64byte
	SD_SFS sfs; // 64byte
	SD_Tuning stuning; // 64byte
	//ULONG t2[8];
} SDIO_SD;

#endif // __USE_SD__
#endif // _SDIO_CD_H_
