#ifndef _SDIO_CD_H_
#define	_SDIO_CD_H_

enum {
	SD_SPEED_CLASS0,
	SD_SPEED_CLASS2,
	SD_SPEED_CLASS4,
	SD_SPEED_CLASS6,
	SD_SPEED_CLASS10,
	SD_SPEED_REV
};

typedef struct { BF_5(
	uint32 DAT_BUS_WIDTH				:2,
	uint32 SECURED_MODE 				:1,
	uint32 RSF							:7,
	uint32 _res1						:6,
	uint32 SD_CARD_TYPE 				:16
)
	uint32 SIZE_OF_PROTECTED_AREA;
BF_5(
	uint32 SPEED_CLASS					:8,
	uint32 PERFORMANCE_MOVE 			:8,
	uint32 AU_SIZE						:4,
	uint32 _res2						:4,
	uint32 ERASE_SIZE0					:8
)BF_6(
	uint32 ERASE_SIZE1					:8,
	uint32 ERASE_TIMEOUT				:6,
	uint32 ERASE_OFFSET 				:2,
	uint32 UHS_SPEED_GRADE				:4,
	uint32 UHS_AU_SIZE					:4,
	uint32 _res3						:8
)
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
}__attribute__ ((packed)) SD_SSR;

typedef struct { BF_28(
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
)}__attribute__ ((packed)) SD_SC;

typedef struct { BF_5(
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
)}__attribute__ ((packed)) SD_CSD_v1;

typedef struct { BF_5(
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
)}__attribute__ ((packed)) SD_CSD_v2;

typedef union {
	UINT a[4];
	SD_CSD_v1 csd_v1;
	SD_CSD_v2 csd_v2;
} SD_CSD;

typedef struct { BF_17(
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
)}__attribute__ ((packed)) SD_OCR;

typedef struct { BF_3(
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
)}__attribute__ ((packed)) SD_CID;

typedef struct { BF_9(
	uint32 SCR_STRUCTURE 				:4,
	uint32 SD_SPEC						:4,
	uint32 DATA_STAT_AFTER_ERASE		:1,
	uint32 SD_SECURITY					:3,
	uint32 SD_BUS_WIDTHS				:4,
	uint32 SD_SPEC3						:1,
	uint32 EX_SECURITY					:4,
	uint32 _res1						:9,
	uint32 CMD_SUPPORT					:2
)
	uint32 _res2;
}__attribute__ ((packed)) SD_SCRreg;


typedef struct {
	UINT nCH;
	UINT nErrorCode;
	UINT nActive;
	UINT nVoltageMode;
	UINT nSetClock;

	SD_OCR ocr;
	SD_CID cid;
	SD_CSD csd;
	UINT rca;
	SD_SCRreg scr;
	UINT t1[6];
	SD_SSR ssr;
	SD_SC sc;
} SDIO_SD;

#endif // _SDIO_CD_H_
