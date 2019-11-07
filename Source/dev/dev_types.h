//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:     dev_types.h
//	Description:   EN675 SW type definition
//	Author:        Team 2
//
//	Note:			
//
//	Ver   Date   Author  Changes
//	---  ------  ------  -------
//	0.1  151026  ygkim   separated from "dev.h" to share the type information with IP SW
//  0.2  190131  hjlee   support EN675
//------------------------------------------------------------------------------
#ifndef _DEV_TYPES_H_
#define	_DEV_TYPES_H_

#define ARRAY_SIZE(x)			(sizeof(x) / sizeof((x)[0]))

#include <stdint.h>							// for xxxx_t
#include <limits.h>							// for __xxxx_MAX__
#include <stdarg.h>							// for va_list

typedef _Bool					bool;		// 1 bit
typedef _Bool					BOOL;		//  "

typedef char					CHAR;		// 8 bit
typedef unsigned char			UCHAR;		//	"
typedef unsigned char			BYTE;		//	"
typedef unsigned char			uchar;		//	"
typedef signed   char			INT8S;		//	"
typedef unsigned char			INT8U;		//	"
typedef signed   char			s8;			//	"
typedef unsigned char			u8;			//	"
typedef signed   char			__s8;		//	"
typedef unsigned char			__u8;		//	"
typedef signed   char			s_int8;		//	"
typedef unsigned char			u_int8;		//	"
typedef signed   char			int8;		//	"
typedef unsigned char			uint8;		//	"
typedef volatile uint8			vuint8;		//	"

typedef short					SHORT;		// 16 bit
typedef unsigned short			USHORT;		//	"
typedef unsigned short			WORD;		//	"
typedef unsigned short			WCHAR;		//	"
typedef unsigned short			ushort;		//	"
typedef signed   short			INT16S;		//	"
typedef unsigned short			INT16U;		//	"
typedef signed   short			s16;		//	"
typedef unsigned short  		u16;		//	"
typedef signed   short			__s16;		//	"
typedef unsigned short  		__u16;		//	"
typedef signed   short			s_int16;	//	"
typedef unsigned short			u_int16;	//	"
typedef signed   short			int16;		//	"
typedef unsigned short			uint16;		//	"
typedef volatile uint16			vuint16;	//	"

typedef int						INT;		// 32 bit
typedef unsigned int			UINT;		//	"
typedef unsigned int			DWORD;		//	"
typedef unsigned int			uint;		//	"
typedef signed   int			INT32S;		//	"
typedef unsigned int			INT32U;		//	"
typedef signed   int			s32;		//	"
typedef unsigned int			u32;		//	"
typedef signed   int			__s32;		//	"
typedef unsigned int			__u32;		//	"
typedef signed   int			s_int32;	//	"
typedef unsigned int			u_int32;	//	"
typedef signed   int			int32;		//	"
typedef unsigned int			uint32;		//	"
typedef volatile uint32			vuint32;	//	"

typedef long					LONG;		// 64 bit
typedef unsigned long			ULONG;		//	"
typedef unsigned long			QWORD;		//	"
typedef unsigned long			ulong;		//	"
typedef signed   long			INT64S;		//	"
typedef unsigned long			INT64U;		//	"
typedef signed   long			s64;		//	"
typedef unsigned long			u64;		//	"
typedef signed   long			__s64;		//	"
typedef unsigned long			__u64;		//	"
typedef signed   long			s_int64;	//	"
typedef unsigned long			u_int64;	//	"
typedef signed   long			int64;		//	"
typedef unsigned long			uint64;		//	"
typedef volatile uint64			vuint64;	//	"

//******************************************************************************
//
//------------------------------------------------------------------------------
//
typedef enum {
	ENX_DISABLED = 0,
	ENX_ENABLED = 1
} ENX_ENABLE;

typedef enum {
	ENX_FALSE = 0,
	ENX_TRUE = 1
} ENX_TF;

typedef enum {
	ENX_NO = 0,
	ENX_YES = 1
} ENX_YN;

typedef enum {
	ENX_OFF = 0,
	ENX_ON = 1
} ENX_SWITCH;

typedef enum {
	ENX_LOW = 0,
	ENX_HIGH = 1,
} ENX_HL;

typedef enum {
	ENX_FAIL = 0,
	ENX_OK = 1
} ENX_OKFAIL;

typedef enum {
	SDIO_OK = 0,
	SDIO_FAIL = 1
} SDIO_OKFAIL;

#define FALSE					0
#define TRUE					1

#ifndef NULL
#define NULL 					((void*)0)
#endif

#define	UNUSED(x)				((void)(x))

#define ENX_MEM_ALIGNMENT			(64)
#define ENX_MEM_ALIGN_SIZE(size)	(((size) + ENX_MEM_ALIGNMENT - 1) & ~(ENX_MEM_ALIGNMENT - 1))
#define ENX_MEM_ALIGN_BUFFER(size)	(((size) + ENX_MEM_ALIGNMENT - 1))
#define ENX_MEM_ALIGN(addr)			(((intptr_t)(addr) + ENX_MEM_ALIGNMENT - 1) & ~(intptr_t)(ENX_MEM_ALIGNMENT - 1))

#define bitchange(bitCheckValue, bitLocation, setBit) (setBit ? (bitCheckValue | bitLocation) : (bitCheckValue & (~(bitLocation))))
#define bitcheck(bitCheckValue, bitLocation) (bitCheckValue & bitLocation)

#define num_loop(val, MAX)		{ ++(val); if((val)>=(MAX)){val = 0;} }
#define cQueue_isfull(cQueue)	(((((cQueue)->tail+1) % (cQueue)->tot_num) == (cQueue)->head) ? ENX_OK : ENX_FAIL)
#define cQueue_isempty(cQueue)	(((cQueue)->head == (cQueue)->tail) ? ENX_OK : ENX_FAIL)

#define ENX_HTONL(x) ((((x) & (unsigned int)0x000000ffUL) << 24) | \
                     (((x) & (unsigned int)0x0000ff00UL) <<  8) | \
                     (((x) & (unsigned int)0x00ff0000UL) >>  8) | \
                     (((x) & (unsigned int)0xff000000UL) >> 24))

#define ISRT0		//__attribute__((section(".ispmtext")))
#define ISRT		//__attribute__((section(".ispmtext")))
#define ISRD		//__attribute__((section(".ispmdata")))

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
/*typeof is gcc keywords */
#define container_of(ptr, type, member) ({                   \
        const typeof( ((type *)0)->member ) *__mptr = (ptr); \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define ATTR_MALIGN(x)	__attribute__((__aligned__(x)))
#define ATTR_MALIGN64	__attribute__((__aligned__(64)))
#define ATTR_MALIGN16	__attribute__((__aligned__(16)))
#define ATTR_BIGENDIAN	__attribute__((__scalar_storage_order__("big-endian")))
#define ATTR_PACKED		__attribute__((packed))

#ifndef max
#define max(a,b)				(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)				(((a) < (b)) ? (a) : (b))
#endif

#define flprintf(fmt, args...) do { printf("    %04d:%s: "fmt, __LINE__, __func__, ##args); } while(0);

#define TTY_COLOR_RESET		"\033[0m"
#define TTY_COLOR_BLACK		"\033[1;30m"
#define TTY_COLOR_RED		"\033[1;31m"
#define TTY_COLOR_GREEN		"\033[1;32m"
#define TTY_COLOR_YELLOW	"\033[1;33m"
#define TTY_COLOR_BLUE		"\033[1;34m"
#define TTY_COLOR_MAGEN		"\033[1;35m"
#define TTY_COLOR_CYAN		"\033[1;36m"
#define TTY_COLOR_WHITE		"\033[1;37m"

//------------------------------------------------------------------------------
// Octet defines
#define  DEF_OCTET_NBR_BITS		8
#define  DEF_OCTET_MASK			0xff

#define  DEF_NIBBLE_NBR_BITS	4
#define  DEF_NIBBLE_MASK		0x0f

//******************************************************************************
// Register define
//------------------------------------------------------------------------------
//
#define BF_1(a)			a;
#define BF_2(a,...)		BF_1(__VA_ARGS__);a;
#define BF_3(a,...)		BF_2(__VA_ARGS__);a;
#define BF_4(a,...)		BF_3(__VA_ARGS__);a;
#define BF_5(a,...)		BF_4(__VA_ARGS__);a;
#define BF_6(a,...)		BF_5(__VA_ARGS__);a;
#define BF_7(a,...)		BF_6(__VA_ARGS__);a;
#define BF_8(a,...)		BF_7(__VA_ARGS__);a;
#define BF_9(a,...)		BF_8(__VA_ARGS__);a;
#define BF_10(a,...)	BF_9(__VA_ARGS__);a;
#define BF_11(a,...)	BF_10(__VA_ARGS__);a;
#define BF_12(a,...)	BF_11(__VA_ARGS__);a;
#define BF_13(a,...)	BF_12(__VA_ARGS__);a;
#define BF_14(a,...)	BF_13(__VA_ARGS__);a;
#define BF_15(a,...)	BF_14(__VA_ARGS__);a;
#define BF_16(a,...)	BF_15(__VA_ARGS__);a;
#define BF_17(a,...)	BF_16(__VA_ARGS__);a;
#define BF_18(a,...)	BF_17(__VA_ARGS__);a;
#define BF_19(a,...)	BF_18(__VA_ARGS__);a;
#define BF_20(a,...)	BF_19(__VA_ARGS__);a;
#define BF_21(a,...)	BF_20(__VA_ARGS__);a;
#define BF_22(a,...)	BF_21(__VA_ARGS__);a;
#define BF_23(a,...)	BF_22(__VA_ARGS__);a;
#define BF_24(a,...)	BF_23(__VA_ARGS__);a;
#define BF_25(a,...)	BF_24(__VA_ARGS__);a;
#define BF_26(a,...)	BF_25(__VA_ARGS__);a;
#define BF_27(a,...)	BF_26(__VA_ARGS__);a;
#define BF_28(a,...)	BF_27(__VA_ARGS__);a;
#define BF_29(a,...)	BF_28(__VA_ARGS__);a;
#define BF_30(a,...)	BF_29(__VA_ARGS__);a;
#define BF_31(a,...)	BF_30(__VA_ARGS__);a;
#define BF_32(a,...)	BF_31(__VA_ARGS__);a;

#define	_regs_					typedef	union{UINT a; struct{	// Register union start
#define	_rege_					};}								// Register union end

#define	_am(uon,base,addr)		((volatile uon*)(UINT*)(base+(addr)))->a	// All macro
#define	_bm(uon,base,addr,mne)	((volatile uon*)(UINT*)(base+(addr)))->mne	// Bit macro
#define _cm(base, idx)			(*(UINT*)(base+(idx<<3)))					//

#define	_wr32(a,d)				(*((volatile UINT *)(unsigned long)(a)) = (d))			//			"			- Always used for ISP Register Write
#define	_rd32(a)				(*((volatile UINT *)(unsigned long)(a)))				//			"			- Always used for ISP Register Read

#define	SetIsp(anAddr, anData)	(_wr32(REG_BASE_ISP+((UINT)(anAddr)<<2), (UINT)(anData)))	// Write to bridge(Isp,Aud)
#define	GetIsp(anAddr)			(_rd32(REG_BASE_ISP+((UINT)(anAddr)<<2))                )	// Read from bridge(Isp,Aud)

#define BitMask_01			0x1
#define BitMask_02			0x3
#define BitMask_03			0x7
#define BitMask_04			0xf
#define BitMask_05			0x1f
#define BitMask_06			0x3f
#define BitMask_07			0x7f
#define BitMask_08			0xff
#define BitMask_09			0x1ff
#define BitMask_10			0x3ff
#define BitMask_11			0x7ff
#define BitMask_12			0xfff
#define BitMask_13			0x1fff
#define BitMask_14			0x3fff
#define BitMask_15			0x7fff
#define BitMask_16			0xffff
#define BitMask_17			0x1ffff
#define BitMask_18			0x3ffff
#define BitMask_19			0x7ffff
#define BitMask_20			0xfffff
#define BitMask_21			0x1fffff
#define BitMask_22			0x3fffff
#define BitMask_23			0x7fffff
#define BitMask_24			0xffffff
#define BitMask_25			0x1ffffff
#define BitMask_26			0x3ffffff
#define BitMask_27			0x7ffffff
#define BitMask_28			0xfffffff
#define BitMask_29			0x1fffffff
#define BitMask_30			0x3fffffff
#define BitMask_31			0x7fffffff
#define BitMask_32			0xffffffff

#define ValSft_R00(val)		((val)>> 0)
#define ValSft_R01(val)		((val)>> 1)
#define ValSft_R02(val)		((val)>> 2)
#define ValSft_R03(val)		((val)>> 3)
#define ValSft_R04(val)		((val)>> 4)
#define ValSft_R05(val)		((val)>> 5)
#define ValSft_R06(val)		((val)>> 6)
#define ValSft_R07(val)		((val)>> 7)
#define ValSft_R08(val)		((val)>> 8)
#define ValSft_R09(val)		((val)>> 9)
#define ValSft_R10(val)		((val)>>10)
#define ValSft_R11(val)		((val)>>11)
#define ValSft_R12(val)		((val)>>12)
#define ValSft_R13(val)		((val)>>13)
#define ValSft_R14(val)		((val)>>14)
#define ValSft_R15(val)		((val)>>15)
#define ValSft_R16(val)		((val)>>16)
#define ValSft_R17(val)		((val)>>17)
#define ValSft_R18(val)		((val)>>18)
#define ValSft_R19(val)		((val)>>19)
#define ValSft_R20(val)		((val)>>20)
#define ValSft_R21(val)		((val)>>21)
#define ValSft_R22(val)		((val)>>22)
#define ValSft_R23(val)		((val)>>23)
#define ValSft_R24(val)		((val)>>24)
#define ValSft_R25(val)		((val)>>25)
#define ValSft_R26(val)		((val)>>26)
#define ValSft_R27(val)		((val)>>27)
#define ValSft_R28(val)		((val)>>28)
#define ValSft_R29(val)		((val)>>29)
#define ValSft_R30(val)		((val)>>30)
#define ValSft_R31(val)		((val)>>31)

#define ValSft_L00(val)		((val)<< 0)
#define ValSft_L01(val)		((val)<< 1)
#define ValSft_L02(val)		((val)<< 2)
#define ValSft_L03(val)		((val)<< 3)
#define ValSft_L04(val)		((val)<< 4)
#define ValSft_L05(val)		((val)<< 5)
#define ValSft_L06(val)		((val)<< 6)
#define ValSft_L07(val)		((val)<< 7)
#define ValSft_L08(val)		((val)<< 8)
#define ValSft_L09(val)		((val)<< 9)
#define ValSft_L10(val)		((val)<<10)
#define ValSft_L11(val)		((val)<<11)
#define ValSft_L12(val)		((val)<<12)
#define ValSft_L13(val)		((val)<<13)
#define ValSft_L14(val)		((val)<<14)
#define ValSft_L15(val)		((val)<<15)
#define ValSft_L16(val)		((val)<<16)
#define ValSft_L17(val)		((val)<<17)
#define ValSft_L18(val)		((val)<<18)
#define ValSft_L19(val)		((val)<<19)
#define ValSft_L20(val)		((val)<<20)
#define ValSft_L21(val)		((val)<<21)
#define ValSft_L22(val)		((val)<<22)
#define ValSft_L23(val)		((val)<<23)
#define ValSft_L24(val)		((val)<<24)
#define ValSft_L25(val)		((val)<<25)
#define ValSft_L26(val)		((val)<<26)
#define ValSft_L27(val)		((val)<<27)
#define ValSft_L28(val)		((val)<<28)
#define ValSft_L29(val)		((val)<<29)
#define ValSft_L30(val)		((val)<<30)
#define ValSft_L31(val)		((val)<<31)

#define _REG_BASE_			REG_BASE_ISP

//******************************************************************************
// IRQ define
//------------------------------------------------------------------------------
typedef enum {
	eigiISP = 0,
	eigiDMA = 1,
	eigiH264 = 1,
	eigiH265 = 1,
	eigiUSB = 2,
	eigiI2S = 2,
	eigiETH = 2,
	eigiSDIO = 3,
	eigiASE = 4,
	eigiSHA = 4,
	eigiCHKSUM = 4,
	eigiBUS = 4,
	eigiUART = 5,
	eigiSPI = 5,
	eigiI2C = 5,
	eigiGPIO = 6,
	eigiTIMER = 7
} eIRQ_GROUP_INDEX;

typedef void (*irq_fn)(void *arg);

typedef struct {
	irq_fn irqfn;
	void *arg;
} tIhnd;

//******************************************************************************
// GPIO define
//------------------------------------------------------------------------------
typedef enum {
	GPIO_EDGE_FALL = 0,
	GPIO_EDGE_RISE = 1,
	GPIO_EDGE_BOTHE = 2
} GPIO_EDGE;

typedef enum {
	GPIO_DIR_OUT = 0,
	GPIO_DIR_IN = 1
} GPIO_DIR;

typedef enum {
	GPIO_OUT_LOW = 0,
	GPIO_OUT_HI = 1
} GPIO_OUT;

//******************************************************************************
// SFLS define
//------------------------------------------------------------------------------
#if 1
#define SFLS_SINGLE 0
#define SFLS_DUAL 1
#define SFLS_QUAD 2
#else
typedef enum {
	SFLS_SINGLE = 0,
	SFLS_DUAL = 1,
	SFLS_QUAD = 2,
} SFLS_IOMODE;
#endif

typedef enum {
	SFLS_E_ALL = 0,
	SFLS_E_4K = 1,
	SFLS_E_32K = 2,
	SFLS_E_64K = 3
} SFLS_ERASE;

typedef void (*sfls_func)(void);
typedef void (*sfls_erase_func)(UINT addr);

typedef struct {
	BYTE ior_cmd;	// read cmd
	BYTE ior_adr;	// read adr
	BYTE ior_dat;	// read dat
	BYTE iow_cmd;	// write cmd
	BYTE iow_adr;	// write adr
	BYTE iow_dat;	// write dat
	BYTE iowe_cmd;	// write enable
	BYTE iors_cmd;	// read status cmd
	BYTE iors_dat;	// read status dat

	BYTE cmd_read;
	BYTE cmd_page_program;
	BYTE cmd_write_enable;
	BYTE cmd_read_status;
	BYTE cmd_enter_qpi;
	BYTE cmd_sector_erase;
	BYTE cmd_32k_erase;
	BYTE cmd_64k_erase;
	BYTE cmd_chip_erase;

	BYTE gap;
	BYTE rdltc;

	sfls_func func_init;
	sfls_func func_write_enable;
	sfls_erase_func func_secter_erase;
	sfls_erase_func func_32k_erase;
	sfls_erase_func func_64k_erase;
	sfls_erase_func func_chip_erase;
} SFLScontrol;

//******************************************************************************
// I2S define
//------------------------------------------------------------------------------
typedef enum {
	eI2sCodecPCM = 0,
	eI2sCodecG711a = 2,
	eI2sCodecG711u = 3
} I2S_CODEC;

typedef enum {
	eI2sWidth8b = 0,
	eI2sWidth16b = 1,
	eI2sWidth24b = 2,
	eI2sWidth32b = 3,
} I2S_DATA_WIDTH;

typedef enum {
	eI2sReq128B,
	eI2sReq256B,
	eI2sReq512B,
	eI2sReq1024B
} I2S_REQ_B;

typedef enum {
	eI2sLoop128KB,
	eI2sLoop256KB,
	eI2sLoop512KB,
	eI2sLoop1024KB
} I2S_LOOP_KB;

typedef enum {
	eI2sWRDW8bit,
	eI2sWRDW16bit,
	eI2sWRDW24bit,
	eI2sWRDW32bit
} I2S_I2StoMEM_BIT; // WRDW

typedef enum {
	eI2sRXDW8bit,
	eI2sRXDW16bit,
	eI2sRXDW24bit,
	eI2sRXDW32bit
} I2S_RX_BIT; // RXDW

typedef enum {
	eI2sRDDW8bit,
	eI2sRDDW16bit,
	eI2sRDDW24bit,
	eI2sRDDW32bit
} I2S_MEMtoI2S_BIT; // RDDW

typedef enum {
	eI2sTXDW8bit,
	eI2sTXDW16bit,
	eI2sTXDW24bit,
	eI2sTXDW32bit
} I2S_TX_BIT; // TXDW


//******************************************************************************
// SDIO define
//------------------------------------------------------------------------------
typedef enum {
	ecrtR1,
	ecrtR1b,
	ecrtR2,
	ecrtR3,
	ecrtR4,
	ecrtR5,
	ecrtR6,
	ecrtR7
} eCmdRespType;

typedef enum {
	SDIO_DIO_SINGLE_READ = 0,
	SDIO_DIO_SINGLE_WRITE = 1,
	SDIO_DIO_MULTI_READ = 2,
	SDIO_DIO_MULTI_WRITE = 3,
	SDIO_DIO_IO_READ = 4,
	SDIO_DIO_IO_WRITE = 5,
} eSDIO_DIO_TYPE;

typedef enum {
	SDIO_1BIT_MODE = 0,
	SDIO_4BIT_MODE = 1
} eSDIO_BIT_MODE;

typedef enum {
	SDIO_CD_MODE = 0,
	SDIO_IO_MODE = 1
} eSDIO_IO_MODE;

typedef enum {
	SDIO_BM_BYTE= 0,
	SDIO_BM_BLOCK = 1
} eSDIO_CMD53_BM;

typedef enum {
	SDIO_OP_FIXED = 0,
	SDIO_OP_INCREMENT = 1
} eSDIO_CMD53_OP;

typedef void (*user_delay_fn)(uint64 ms);

//******************************************************************************
// FileSystem define
//------------------------------------------------------------------------------
/* Definitions of physical drive number for each drive */
typedef enum {
	DEV_FLS,				// Serial Flash Memory
	DEV_SD,					// microSD Card
	DEV_USB,				// USB MSD
	DEV_MEM,				// DDR Memory Disk
} DriveNum;

//******************************************************************************
// Video/Audio Index define
//------------------------------------------------------------------------------
typedef enum {
	e_vsVSource1,
	e_vsVSource2,
	e_vsVSource3,
	e_vsVSource4,
	e_vsEndorUnknown
} eVideoSource;
#define VIDEO_SOURCE_CNT e_vsEndorUnknown

typedef enum {
	e_vcVEncoder1,
	e_vcVEncoder2,
	e_vcVEncoder3,
	e_vcEndorUnknown
} eVideoChannel;
#define VIDEO_CHANNEL_CNT e_vcEndorUnknown

typedef enum {
	eSTREAMMSG_H265I = 1,
	eSTREAMMSG_H265P,
	eSTREAMMSG_H264I,
	eSTREAMMSG_H264P,
	eSTREAMMSG_JPEG,
	eSTREAMMSG_AUDIO,
	eSTREAMMSG_TXTS
} AVType;

typedef enum {
	e_vcodecH265,
	e_vcodecH264,
	e_vcodecJPEG
} VideoCodec;

typedef enum {
	e_res3840x2160,		// 16:9		UHD		8,294,400, 0
	e_res2560x2048,		// 5:4		QSXGA	5,242,880, 1
	e_res2560x1600,		// 16:10	WQVGA	4,096,000, 2
	e_res2560x1440,		// 16:9		WQHD	3,686,400, 3
	e_res2048x1536,		// 4:3		QXGA	3,145,728, 4
	e_res1920x1200,		// 8:5		WUXGA	2,304,000, 5
	e_res1920x1080,		// 16:9		FHD		2,073,600, 6
	e_res1280x720,		// 16:9		HD		  921,600, 7
	e_res800x600,		// 4:3		SVGA	  480,000, 8
	e_res704x480,		// 3:2		-		  337,920, 9
	e_res704x400,		// 16:9		-		  281,600, 10
	e_res640x480,		// 4:3		VGA		  307,200, 11
	e_res640x360,		// 16:9		-		  230,400, 12
	e_res320x240,		// 4:3		QVGA	   76,800, 13
	e_resEndorUnknown
} VideoResolution;

typedef enum {
	e_br50m = 0,
	e_br45m,
	e_br40m,
	e_br35m,
	e_br30m,
	e_br25m,
	e_br20m,
	e_br19m,
	e_br18m,
	e_br17m,
	e_br16m,
	e_br15m,
	e_br14m,
	e_br13m,
	e_br12m,
	e_br11m,
	e_br10m,
	e_br9m,
	e_br8m,
	e_br7m,
	e_br6m,
	e_br5m,
	e_br4m,
	e_br3m,
	e_br2m,
	e_br1m,
	e_br512k,
	e_br256k,
	e_br128k,
	e_brEndorUnknown,
} VideoBitRateIdx;

typedef enum {
	e_brmQBR = 0,	// Video Bitrate: Q Fixed
	e_brmVBR = 1, 	// Video Bitrate: Variable
	e_brmCBR = 2,	// Video Bitrate: Constant
	e_brmCVBR = 3,	// Video Bitrate: Constant Variable
	e_brmEndorUnknown = 4,
} VideoBitRateMode;

typedef enum {
	e_pmBaseline = 0,
	e_pmMainCB = 1,
	e_pmEndorUnknown = 2,
} H264ProfileMode;

//******************************************************************************
// Wi-Fi define
//------------------------------------------------------------------------------
enum {
	WLIF_UAP_START,
	WLIF_STA_START,
	WLIF_UAP_ACTIVE,
	WLIF_STA_ACTIVE,
	WLIF_STA_CONNECT_WAIT,
	WLIF_CONNECTED,
	WLIF_DIS_CONNECTED,
	WLIF_SCAN,
	WLIF_UAP_BSS_ON,
	WLIF_UAP_BSS_OFF,
	WLIF_UAP_BSS_ERROR,
	WLIF_UAP_DISCONN,
	WLIF_INIT_FAIL = 0x1000,
	WLIF_STA_NO_AP = 0x1001,
	WLIF_STA_WPA_AUTH_FAIL = 0x1002,
	WLIF_NONE = 0xFFFF,
};

typedef struct {
	UINT netif;			//	netif pointer
	UINT type;			//	0 - sta, 1 -uap
	UINT event;			//	0 connected, 1 disconnected, 2 scan
	BYTE mac_addr[8];	//	wifi mac address(6byte).
} wlif_priv_t;

//******************************************************************************
// Key define
//------------------------------------------------------------------------------
#define FPS_VDO		model_Sens_Fps

#define	UART_HOLD_CNT	(2<<(FPS_VDO>30))					// Holding time
#define	UART_KEY_RPTF	(FPS_VDO>>2)						// First Repeat time
#define	UART_KEY_RPTT	(FPS_VDO>>3)						// Repeat time
#define	UART_KEY_RPTT2	(FPS_VDO>>4)						// Repeat time(Fast)	// 2016331 - WHL

#define PUSH_DELAY_NOT	1									// No Holding time
#define PUSH_DELAY_MIN	(FPS_VDO)							// Min Holding time
#define PUSH_DELAY_NOR	(FPS_VDO+1)							// Max Holding time


#define KEY_VAL_U	1
#define KEY_VAL_D	2
#define KEY_VAL_R	4
#define KEY_VAL_L	8
#define KEY_VAL_C	16
#define KEY_VAL_E	32
#define KEY_VAL_S	64

#define	KEY			gbMKey				// for MENU REPEAT
#define	KEY_U		((gbMKey   )&1)		//			"
#define	KEY_D		((gbMKey>>1)&1)		//			"
#define	KEY_R		((gbMKey>>2)&1)		//			"
#define	KEY_L		((gbMKey>>3)&1)		//			"
#define	KEY_C		((gbMKey>>4)&1)		//			"
#define	KEY_E		((gbMKey>>5)&1)		//			"

#define	pKEY		gbPKey				// for MENU PUSHING
#define	pKEY_U		((gbPKey   )&1)		//			"
#define	pKEY_D		((gbPKey>>1)&1)		//			"
#define	pKEY_R		((gbPKey>>2)&1)		//			"
#define	pKEY_L		((gbPKey>>3)&1)		//			"
#define	pKEY_C		((gbPKey>>4)&1)		//			"
#define	pKEY_E		((gbPKey>>5)&1)		//			"


// Key bit field
typedef	union{
	UINT	nKey;
	struct {
		BYTE bRpt;		// Addr 3, nKey[ 7: 0]	- Repeat Result (It is used in the OSD MENU)
		BYTE bCnt;		// Addr 2, nKey[15: 8]	- Repeat Counter
		BYTE bOn;		// Addr 1, nKey[23:16]	- Key On/Off
		BYTE bSft;		// Addr 0, nKey[31:24]	- Shift buffer
	}v;	// Value
	struct {
		BYTE Ur   : 1;	// Addr 3, nKey[0]		- bRpt - for MENU REPEAT
		BYTE Dr   : 1;	// Addr 3, nKey[1]		-	"
		BYTE Rr   : 1;	// Addr 3, nKey[2]		-	"
		BYTE Lr   : 1;	// Addr 3, nKey[3]		-	"
		BYTE Cr   : 1;	// Addr 3, nKey[4]		-	"
		BYTE Er   : 1;	// Addr 3, nKey[5]		-	"
		BYTE res1 : 2;	// Addr 3, nKey[7:6]	-	"
		BYTE bCnt : 8;	// Addr 2, nKey[15:8]	- bCnt
		BYTE U    : 1;	// Addr 1, nKey[16]		- bOn - for MENU PUSHING
		BYTE D    : 1;	// Addr 1, nKey[17]		-	"
		BYTE R    : 1;	// Addr 1, nKey[18]		-	"
		BYTE L    : 1;	// Addr 1, nKey[19]		-	"
		BYTE C    : 1;	// Addr 1, nKey[20]		-	"
		BYTE E    : 1;	// Addr 1, nKey[21]		-	"
		BYTE res0 : 2;	// Addr 1, nKey[23:22]	-	"
		BYTE bSft : 8;	// Addr 0, nKey[31:24]	- bSft
	}b;	// Bit
} TKey;


//******************************************************************************
// User Parameter define
//------------------------------------------------------------------------------
#ifndef __ISP_LIB__

#define UpList				((UP_LIST*)gbUsrParTbl)
#define UP(N)				UpList->N

#define UPi(N)				N##_IDX									// Index of User Parameter
#define UPs(N)				N##_SIZ									// Size of User Parameter

//#define UPw(N,D)			UP(N) = (D)//SetByte(gbUsrParTbl+UPi(N), UPs(N), D)	// Write User Parameter
//#define UPr(N)				UP(N)//GetByte(gbUsrParTbl+UPi(N), UPs(N))		// Read User Parameter

//#define UPo(N)				UP(N)//, sizeof(UP(N))//gbUsrParTbl[UPi(N)], UPs(N)				// Use User Parameter in MENU
//#define UPx(N)				N//, sizeof(N)							// Use Default Type in MENU


#define gbCamTitle(INDEX)	gbUsrParTbl[UPi(UpCamTitle0)+((INDEX))]

#define UPinv(N)			(UPi(N)+UPs(N)-1)


//------------------------------------------------------------------------------
#define PAR_IDX1(N,...)		UPi(N),
#define PAR_IDX2(N,...)		PAR_IDX1(N,__VA_ARGS__) N##_1_IDX,
#define PAR_IDX3(N,...)		PAR_IDX2(N,__VA_ARGS__) N##_2_IDX,
#define PAR_IDX4(N,...)		PAR_IDX3(N,__VA_ARGS__) N##_3_IDX,

#define _UP_IDX_1(N,...)	                                                         PAR_IDX1(N,__VA_ARGS__)
#define _UP_IDX_2(N,...)	N##_UP_END }; enum { N##_UP_START=((N##_UP_END+1)&~1)-1, PAR_IDX2(N,__VA_ARGS__)
#define _UP_IDX_4(N,...)	N##_UP_END }; enum { N##_UP_START=((N##_UP_END+3)&~3)-1, PAR_IDX4(N,__VA_ARGS__)

#define _UP_(S,N,...)		_UP_IDX_##S(N,...)
#include "isp_par_tbl.h"
enum {
	UP_START=0,
	USR_PAR_LIST
	UP_END
};


#define _UP_TYPE_1(N,...)	BYTE N;
#define _UP_TYPE_2(N,...)	WORD N;
#define _UP_TYPE_4(N,...)	UINT N;

#undef _UP_
#define _UP_(S,N,...)		_UP_TYPE_##S(N,...)
#include "isp_par_tbl.h"
typedef struct {
	BYTE UpStart;
	USR_PAR_LIST
	BYTE UpEnd;
} UP_LIST;


#undef _UP_
#define _UP_(S,N,...)		enum { UPs(N) = S };
#include "isp_par_tbl.h"
USR_PAR_LIST


#define USR_PAR_EA			(UP_END+1)

extern BYTE gbUsrParChgOn;
extern BYTE gbUsrParSaveChk;
extern BYTE gbUsrParReadChk;
extern BYTE gbUsrParTbl[USR_PAR_EA];
extern BYTE gbUsrParTblSaved[USR_PAR_EA];

#define PAR_BASE	((UINT*)(gbUsrParTbl+UPi(UpPAR00)))

#define UP_LIB1(N) { extern BYTE *gLib##N; gLib##N = &UP(N); }
#define UP_LIB2(N) { extern WORD *gLib##N; gLib##N = &UP(N); }
#define UP_LIB4(N) { extern UINT *gLib##N; gLib##N = &UP(N); }
#define UP_LIB(S,N)	UP_LIB##S(N)

#else
	#define UP(N)		(*gLib##N)

	#define PAR_BASE	(gLib##UpPAR00)

	#ifdef UP_LIB_LINK
		#define UP_LIB1(N)	BYTE *gLib##N = 0;
		#define UP_LIB2(N)	WORD *gLib##N = 0;
		#define UP_LIB4(N)	UINT *gLib##N = 0;
		#define UP_LIB(S,N)	UP_LIB##S(N)
	#else
		#define UP_LIB1(N)	extern BYTE *gLib##N;
		#define UP_LIB2(N)	extern WORD *gLib##N;
		#define UP_LIB4(N)	extern UINT *gLib##N;
		#define UP_LIB(S,N)	UP_LIB##S(N)
	#endif

	#include "isp_par_tbl.h"
	UP_LIB_LIST
#endif
//******************************************************************************
// User Data define
//------------------------------------------------------------------------------
#define USR_DATA_EA_AWBPRST	16
#define USR_DATA_EA_IRIS	8
#define USR_DATA_EA_SAF		2
#define USR_DATA_EA_AE		sizeof(TAeStat)	// 36 BYTE
#define USR_DATA_EA			64							// Byte total  (max 4K-4)

enum {
	UD_START 	= 0,									// Head

	UdAwbPrst	,
	UdIris		= UdAwbPrst+USR_DATA_EA_AWBPRST,
	UdSaf		= UdIris + USR_DATA_EA_IRIS,

	UD_END_REAL = UdSaf + USR_DATA_EA_SAF
};

#define UD_END 			(USR_DATA_EA-1)					// Tail
#define UD(I)			gbUsrDataTbl[I]

extern BYTE gbUsrDataSaveChk;
extern BYTE gbUsrDataReadChk;
extern BYTE gbUsrDataTbl[USR_DATA_EA];


//******************************************************************************
// Font define
//------------------------------------------------------------------------------
#if model_CharMax == 1
	#define EXCH		WORD
	#define EXCH_ADD	1
#else
	#define EXCH		char
	#define EXCH_ADD	0
#endif


//******************************************************************************
// Debug Graph & Parameter channel
//------------------------------------------------------------------------------
union uFLOAT{
	float	m_float;
	UINT	m_UINT;
};

#define GRP_NUMBER		8

extern union uFLOAT gnTxGrp[GRP_NUMBER];

#define	GRP0			(gnTxGrp[0].m_UINT)
#define	GRP1			(gnTxGrp[1].m_UINT)
#define	GRP2			(gnTxGrp[2].m_UINT)
#define	GRP3			(gnTxGrp[3].m_UINT)
#define	GRP4			(gnTxGrp[4].m_UINT)
#define	GRP5			(gnTxGrp[5].m_UINT)
#define	GRP6			(gnTxGrp[6].m_UINT)
#define	GRP7			(gnTxGrp[7].m_UINT)

#define	GRP0F			(gnTxGrp[0].m_float)
#define	GRP1F			(gnTxGrp[1].m_float)
#define	GRP2F			(gnTxGrp[2].m_float)
#define	GRP3F			(gnTxGrp[3].m_float)
#define	GRP4F			(gnTxGrp[4].m_float)
#define	GRP5F			(gnTxGrp[5].m_float)
#define	GRP6F			(gnTxGrp[6].m_float)
#define	GRP7F			(gnTxGrp[7].m_float)


#define	PAR00			(*(PAR_BASE+ 0))
#define	PAR01			(*(PAR_BASE+ 1))
#define	PAR02			(*(PAR_BASE+ 2))
#define	PAR03			(*(PAR_BASE+ 3))
#define	PAR04			(*(PAR_BASE+ 4))
#define	PAR05			(*(PAR_BASE+ 5))
#define	PAR06			(*(PAR_BASE+ 6))
#define	PAR07			(*(PAR_BASE+ 7))
#define	PAR08			(*(PAR_BASE+ 8))
#define	PAR09			(*(PAR_BASE+ 9))
#define	PAR0A			(*(PAR_BASE+10))
#define	PAR0B			(*(PAR_BASE+11))
#define	PAR0C			(*(PAR_BASE+12))
#define	PAR0D			(*(PAR_BASE+13))
#define	PAR0E			(*(PAR_BASE+14))
#define	PAR0F			(*(PAR_BASE+15))
#define	PAR10			(*(PAR_BASE+16))
#define	PAR11			(*(PAR_BASE+17))
#define	PAR12			(*(PAR_BASE+18))
#define	PAR13			(*(PAR_BASE+19))
#define	PAR14			(*(PAR_BASE+20))
#define	PAR15			(*(PAR_BASE+21))
#define	PAR16			(*(PAR_BASE+22))
#define	PAR17			(*(PAR_BASE+23))
#define	PAR18			(*(PAR_BASE+24))
#define	PAR19			(*(PAR_BASE+25))
#define	PAR1A			(*(PAR_BASE+26))
#define	PAR1B			(*(PAR_BASE+27))
#define	PAR1C			(*(PAR_BASE+28))
#define	PAR1D			(*(PAR_BASE+29))
#define	PAR1E			(*(PAR_BASE+30))
#define	PAR1F			(*(PAR_BASE+31))


//******************************************************************************
// I2C Debug
//------------------------------------------------------------------------------
#define ENABLE_I2C_WAIT		0
#define ENABLE_SI2C_WAIT	0

#if ENABLE_I2C_ERR == 1
	#define I2C_STA				BYTE bErr = 0;
	#define I2C_END				I2c_Err: UartTxStrHexNoIRQ("I2C ERR :", bErr, 2);  I2c_mode(0); return bErr;
	#define I2C_CODE(ERR,CODE)	if(CODE) { bErr = ERR; goto I2c_Err; }
#else
	#define I2C_STA
	#define I2C_END
	#define I2C_CODE(ERR,CODE)	CODE;
#endif

#if ENABLE_SI2C_ERR == 1
	#define SI2C_STA			BYTE bErr = 0;
	#define SI2C_END			I2c_Err: TxStrNoIRQ("SI2C ERR :", bErr, 2);  return bErr;
	#define SI2C_CODE(ERR,CODE)	if(CODE) { bErr = ERR; goto I2c_Err; }
#else
	#define SI2C_STA
	#define SI2C_END
	#define SI2C_CODE(ERR,CODE)	CODE;
#endif

#if ENABLE_I2C_WAIT == 1
	#define I2C_WAIT(ERR,CODE)	while(CODE);
#else
	#define I2C_WAIT(ERR,CODE)	I2C_CODE(ERR,CODE)
#endif

#if ENABLE_SI2C_WAIT == 1
	#define SI2C_WAIT(ERR,CODE)	while(CODE);
#else
	#define SI2C_WAIT(ERR,CODE)	SI2C_CODE(ERR,CODE)
#endif


enum {
	EEP_NO_ERROR,

	EEP_WRITE_NODEV,
	EEP_WRITE_MASTER,
	EEP_WRITE_NOACK1,
	EEP_WRITE_NOACK2,
	EEP_WRITE_NOACK3,
	EEP_WRITE_NOACK4,

	EEP_READ_NODEV,
	EEP_READ_MASTER,
	EEP_READ_NOACK1,
	EEP_READ_NOACK2,
	EEP_READ_NOACK3,
};

enum {
	SENS_WRITE_NODEV1 = 80,
	SENS_WRITE_NOACK1,
	SENS_WRITE_NOACK2,
	SENS_WRITE_NOACK3,
	SENS_WRITE_NOACK4,

	SENS_READ_NODEV1 = 90,
	SENS_READ_NOACK1,
	SENS_READ_NOACK2,
	SENS_READ_NOACK3,
	SENS_READ_NOACK4,
};


//******************************************************************************
// Sensor define
//------------------------------------------------------------------------------
#ifndef __ISP_LIB__
	#define SENS_DEF(N,V,S)	enum { N##___SENS_##S = V };

	#define SENS_SEL(N,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10,V11,V12,V13,V14,V15,V16,V17,V18,V19,V20,S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15,S16,S17,S18,S19,S20)	\
		SENS_DEF(N,V1,S1)	SENS_DEF(N,V2,S2)	SENS_DEF(N,V3,S3)	SENS_DEF(N,V4,S4)	SENS_DEF(N,V5,S5)	SENS_DEF(N,V6,S6)	SENS_DEF(N,V7,S7)	SENS_DEF(N,V8,S8)	\
		SENS_DEF(N,V9,S9)	SENS_DEF(N,V10,S10)	SENS_DEF(N,V11,S11)	SENS_DEF(N,V12,S12)	SENS_DEF(N,V13,S13)	SENS_DEF(N,V14,S14)	SENS_DEF(N,V15,S15)	SENS_DEF(N,V16,S16)	\
		SENS_DEF(N,V17,S17)	SENS_DEF(N,V18,S18)	SENS_DEF(N,V19,S19)	SENS_DEF(N,V20,S20)
#else
	#define SENS_SEL(...)
#endif
//------------------------------------------------------------------------------
// Frame info
#if model_1M
	#define FR_HW		1280			// Digital Output formatter horizontal active width
	#define FR_VW		720				// Digital Output formatter vertical active width
	#define PO_HW		1288            // HWI(pre module active image horizontal width) & HWO(Post module horizontal image active width)
	#define PO_VW		728             // VWI(pre module active image vertical width) & VWO(Post module vertical image active width)
	#define LV_HW		1308			// Lvds/Mipi internal buffer에서 image를 read하는 horizontal width를 설정한다. 이 값은 margin을 포함하여 실제 horizontal active pixel보다 크게 설정한다.
	#define LV_VW		736				// Lvds/Mipi internal buffer에서 image를 read하는 vertical width를 설정한다. 이 값은 margin을 포함하여 실제 vertical active pixel보다 크게 설정한다.
	#define FR_HTW60	1650			// Total  H width
	#define FR_VTW60	750				// Total  V width
	#define FR_HTW50	1980			// Total  H width
	#define FR_VTW50	750				// Total  V width
#elif model_2M || model_2M30p
	#define FR_HW		1920			// Digital Output formatter horizontal active width
	#define FR_VW		1080			// Digital Output formatter vertical active width
	#define PO_HW		1928            // HWI(pre module active image horizontal width) & HWO(Post module horizontal image active width)
	#define PO_VW		1088            // VWI(pre module active image vertical width) & VWO(Post module vertical image active width)
	#define LV_HW		1988			// Lvds/Mipi internal buffer에서 image를 read하는 horizontal width를 설정한다. 이 값은 margin을 포함하여 실제 horizontal active pixel보다 크게 설정한다.
	#define LV_VW		1100			// Lvds/Mipi internal buffer에서 image를 read하는 vertical width를 설정한다. 이 값은 margin을 포함하여 실제 vertical active pixel보다 크게 설정한다.
	#define FR_HTW60	2200			// Total  H width
	#define FR_VTW60	1125			// Total  V width
	#define FR_HTW50	2640			// Total  H width
	#define FR_VTW50	1125			// Total  V width
#elif model_4M
	#define FR_HW		2560			// Digital Output formatter horizontal active width
	#define FR_VW		1440			// Digital Output formatter vertical active width
	#define PO_HW		2568            // HWI(pre module active image horizontal width) & HWO(Post module horizontal image active width)
	#define PO_VW		1448            // VWI(pre module active image vertical width) & VWO(Post module vertical image active width)
	#define LV_HW		2584			// Lvds/Mipi internal buffer에서 image를 read하는 horizontal width를 설정한다. 이 값은 margin을 포함하여 실제 horizontal active pixel보다 크게 설정한다.
	#define LV_VW		1458			// Lvds/Mipi internal buffer에서 image를 read하는 vertical width를 설정한다. 이 값은 margin을 포함하여 실제 vertical active pixel보다 크게 설정한다.
	#define FR_HTW60	3300			// Total  H width
	#define FR_VTW60	1500			// Total  V width
	#define FR_HTW50	3960			// Total  H width
	#define FR_VTW50	1500			// Total  V width
#elif model_8M
	#define FR_HW		3840			// Digital Output formatter horizontal active width
	#define FR_VW		2160			// Digital Output formatter vertical active width
	#define PO_HW		3848            // HWI(pre module active image horizontal width) & HWO(Post module horizontal image active width)
	#define PO_VW		2168            // VWI(pre module active image vertical width) & VWO(Post module vertical image active width)
	#define LV_HW		3860			// Lvds/Mipi internal buffer에서 image를 read하는 horizontal width를 설정한다. 이 값은 margin을 포함하여 실제 horizontal active pixel보다 크게 설정한다.
	#define LV_VW		2180			// Lvds/Mipi internal buffer에서 image를 read하는 vertical width를 설정한다. 이 값은 margin을 포함하여 실제 vertical active pixel보다 크게 설정한다.
	#define FR_HTW60	4400			// Total  H width
	#define FR_VTW60	2250			// Total  V width
	#define FR_HTW50	5280			// Total  H width
	#define FR_VTW50	2250			// Total  V width
#endif

//------------------------------------------------------------------------------
// Link
#if model_Sens ==			SENS_IMX291
	#define Sens(a) a##___##SENS_IMX291
#elif model_Sens ==			SENS_IMX327
	#define Sens(a) a##___##SENS_IMX327
#elif model_Sens ==			SENS_SPC1
	#define Sens(a) a##___##SENS_SPC1
#elif model_Sens ==			SENS_IMX335
	#define Sens(a) a##___##SENS_IMX335
#elif model_Sens ==			SENS_OS05A10
	#define Sens(a) a##___##SENS_OS05A10
#elif model_Sens ==			SENS_IMX334
	#define Sens(a) a##___##SENS_IMX334
#elif model_Sens ==			SENS_IMX274
	#define Sens(a) a##___##SENS_IMX274
#elif model_Sens ==			SENS_IMX415
	#define Sens(a) a##___##SENS_IMX415
#elif model_Sens ==			SENS_OS08A10
	#define Sens(a) a##___##SENS_OS08A10
#elif model_Sens ==			SENS_IMX225
	#define Sens(a) a##___##SENS_IMX225
#elif model_Sens ==			SENS_OV2718
	#define Sens(a) a##___##SENS_OV2718
#elif model_Sens ==			SENS_OV4689
	#define Sens(a) a##___##SENS_OV4689

#else
	//#define Sens(a) a##___##NOSENS
	#define Sens(a) a
#endif

//------------------------------------------------------------------------------
// Sensor Setting
#define _SV_(NAME,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10,V11,V12,V13,V14,V15,V16,V17,V18,V19,V20) SENS_SEL(NAME,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10,V11,V12,V13,V14,V15,V16,V17,V18,V19,V20,\
\
					OV2718, IMX291,OS08A10, IMX225, IMX335, IMX274, OV4689, IMX415, none09, none10, none11, none12, none13, none14, none15, none16, none17, none18, none19, none20)
_SV_(LvdsPNSel,		     0,      1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 : Negative Start, 1 : Positive Start,  입력 data의 latch start point를 설정
_SV_(LvdsSofNo,		     1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 : usually Panasonic sensor, 1 : others		TODO KSH OS08A10의 SOF_NO 설정 확인 필요

_SV_(LckDly,		     7,      2,      2,      2,      2,      6,      4,      4,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 7,  Lvds/Mipi input clock delay adjust
_SV_(LdiDly0,		     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 7,  LVDS channel 0 delay adjust
_SV_(LdiDly1,		     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 7,  LVDS channel 1 delay adjust
_SV_(LdiDly2,		     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 7,  LVDS channel 2 delay adjust
_SV_(LdiDly3,		     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 7,  LVDS channel 3 delay adjust

_SV_(MipiClkPhase,	     0,      0,      0,      2,      2,      0,      2,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Select Mipi 1/4 Clock Phase 0, 1, 2, 3, image align에 영향을 주어 영상을 보면서 이 값을 조절해야 함
_SV_(MipiBit,		     0,      0,      0,     10,     12,     12,     10,     12,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(MipiHSyncOfs,	     0,      0,      0,      7,      7,      7,      8,      7,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Very Important!!! For Image Phase
_SV_(MipiUseWcl,	     0,      0,      0,      0,      0,      0,      1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// use long packet word counter check function

_SV_(IsSlave,		     1,      0,      1,      1,      1,      1,      1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 : Master Mode, 1 : Slave Mode,  Isp master/slave mode를 설정
_SV_(IsASync,		     1,      0,      0,      0,      1,      0,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Auto sync generation, omni sensor와 같이 Active 이외의 구간에서 sync가 나오지 않는 sensor에서 1로 설정
_SV_(IsNSync,		     1,      0,      0,      1,      1,      0,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 입력 sync의 H/V 위상이 일치하지 않는 상황에서 1로 설정
_SV_(PreHSyncOfs,	   0xe,    0xa,    0xa,   0xb0,   0xb0,    0xa,   0xb0,   0xa0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Pre module Horizontal Sync Offset, Hsp 인자로 Image 위치가 맞지 않을때 Sync의 위치를 이동하여 image 위치를 맞추는데 사용
_SV_(PreVSyncOfs,	     0,    0xc,    0xc,    0xc,    0xc,    0xc,    0xc,    0xc,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Pre module Vertical Sync Offset, Vsp 인자로 Image 위치가 맞지 않을때 Sync의 위치를 이동하여 image 위치를 맞추는데 사용
_SV_(PreHsp,		  0x13,   0x15,   0x15,    0x8,    0x8,   0x15,    0x8,   0x15,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Pre module Horizontal image start position
_SV_(PreVsp,		   0x8,    0x6,    0x6,    0x4,    0x4,    0x6,    0x4,    0x2,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// Pre module Vertical image start position
_SV_(PreHSyncPol,	     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 : Falling position, 1 : Rising position, Pre module Horizontal sync input polarity
_SV_(PreVSyncPol,	     0,      0,      1,      1,      1,      1,      1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 : Falling position, 1 : Rising position, Pre module Vertical sync input polarity

_SV_(OCSel,			     3,      1,      1,      2,      2,      1,      1,      3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 0 ~ 3,  Post module color phase selection, RGB interpolator의 위상을 조절, 출력단 VLOCKO_IT_POS0w(OutVSyncOfs) & HLOCKO_IT_POS0w(OutHSyncOfs) 설정에 따라 OCSel값을 잘못 설정할 수 있으니 주의!!!

_SV_(SyncCode0,		 0xfff,  0xfff,  0xfff,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncCode1,		     0,      0,      0,      1,      1,      1,      1,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncCode2,		     0,      0,      0,      2,      2,      2,      2,      2,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncCode3_0,	     0,      0,      0,      3,      3,      3,      3,      3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncCode3_1,	 0x800,  0x800,  0x200,   0xb8,   0xb8,   0xb8,   0xb8,   0xb8,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncCode3_2,	 0xab0,  0xab0,  0x280,  0xf30,  0xf30,  0xf30,  0xd20, 0x16a4,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncS0Code3_0,	     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncS0Code3_1,	     0,      0,      0, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncS0Code3_2,	     0,      0,      0,   0x2b,   0x2c,   0x2c,   0x2b,   0x2c,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncS1Code3_0,	     0,      0,      0,      3,      3,      3,      3,      3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncS1Code3_1,	     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)
_SV_(SyncS1Code3_2,	     0,      0,      0,      3,      3,      3,  0xd20,      3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)

#if	(model_Sens_Fps==60)
_SV_(SensorClk,		     0,     74,      0,     27,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
_SV_(PreClk,		     0, C_148M,      0,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#elif (model_Sens_Fps==30)
_SV_(SensorClk,		    13,     37,      0,      0,     13,      0,     27,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
_SV_(PreClk,		 C_74M,  C_74M,      0,      0,  C_74M,      0,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#elif (model_Sens_Fps==20)
_SV_(SensorClk,		     0,      0,      0,      0,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
_SV_(PreClk,		     0,      0,      0,      0,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#elif (model_Sens_Fps==15)
_SV_(SensorClk,		     0,      0,     27,      0,      0,     13,     13,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
_SV_(PreClk,		     0,      0,  C_74M,      0,      0,  C_74M,  C_74M,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#elif (model_Sens_Fps==12)
_SV_(SensorClk,		     0,      0,      0,      0,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// 148, 74, 37, 18, 27, 13, 6 MHz
_SV_(PreClk,		     0,      0,      0,      0,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0
#endif

_SV_(PostClk,		 C_74M,  C_74M,  C_74M,  C_74M,  C_74M,  C_74M,  C_74M,  C_74M,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0)	// C_PLL, C_148M, C_74M, C_PCK, C_PCKD2, C_PLLD0

#if	model_1M
	#define OutHSyncOfs		0x5c
	#define OutVSyncOfs		0x0
#elif model_2M || model_2M30p
	#define OutHSyncOfs		0x5a
	#define OutVSyncOfs		0x446
#elif model_4M
	#define OutHSyncOfs		0x5c
	#define OutVSyncOfs		0x5ba
#elif model_8M
	#define OutHSyncOfs		0x5a
	#define OutVSyncOfs		0x0
#endif


//------------------------------------------------------------------------------
// ISP Function define
#define IspSensorPowerOn()		Isp_SensorPowerOn(FN_ON,Sens(SensorClk))

#if (model_Sens_Intf==0)	// Parallel
	#define IspSDesPowerOn()	Isp_SDesPowerOn(FN_OFF,0,0)		// LVDS/MIPI Off
	#define IspSDesConfig()		Isp_Parallel_Config(FN_ON/*, ISP_SLAVE*/, PARA_EXTER_CLK, PARA_CLK_SDR, 0/*, SYNC_FALL, SYNC_RISE, DIFF_EDGE, NO_USE_AUTOSYNC*/)
#elif (model_Sens_Intf==1)	// LVDS
	#define IspSDesPowerOn()	Isp_SDesPowerOn(FN_ON, LVDS_INTERFACE, 0)
	#define IspSDesConfig()		Isp_Lvds_Config(LVDS_12BIT, LVDS_4LANE, 0, UP(UpLvdsPNSel), Sens(LvdsSofNo))
#elif (model_Sens_Intf==2)	// MIPI
	#define IspSDesPowerOn()	Isp_SDesPowerOn(FN_ON, MIPI_INTERFACE, UP(UpMipiClkPhase))
	#define IspSDesConfig()		Isp_Mipi_Config(Sens(MipiBit), MIPI_4LANE, 0, 1, UP(UpMipiHSyncOfs), USE_ECC_CHECK, Sens(MipiUseWcl), NO_USE_CHECK, NO_USE_CHECK, 3)
#endif

#define IspSDesDelay()			Isp_SDesDelay(UP(UpLckDly), UP(UpLdiDly0), UP(UpLdiDly1), UP(UpLdiDly2), UP(UpLdiDly3))

#if model_2M || model_2M30p
	#define IspSDesPosition()	Isp_SDesPosition(0x1f, 0x1, LV_HW, LV_VW)
#else
	#define IspSDesPosition()	Isp_SDesPosition(0xb5, 0xc, LV_HW, LV_VW)
#endif

#define IspSYNC_CODE()			Isp_SYNC_CODE(Sens(SyncCode0), Sens(SyncCode1), Sens(SyncCode2), Sens(SyncCode3_0), Sens(SyncCode3_1), Sens(SyncCode3_2),\
											  Sens(SyncS0Code3_0), Sens(SyncS0Code3_1), Sens(SyncS0Code3_2), Sens(SyncS1Code3_0), Sens(SyncS1Code3_1), Sens(SyncS1Code3_2))

#define IspPreClkConfig()		Isp_PreClk_Config(Sens(PreClk))
#define IspPostClkConfig()		Isp_PostClk_Config(Sens(PostClk))

#define IspPreSyncConfig()		Isp_PreSync_Config(Sens(IsSlave), FR_HTW60, FR_VTW60, UP(UpPreHSyncOfs), UP(UpPreVSyncOfs), UP(UpPreHsp), UP(UpPreVsp), PO_HW, PO_VW, Sens(IsASync), Sens(IsNSync), UP(UpPreHSyncPol), UP(UpPreVSyncPol))

#define IspPostSyncConfig()		Isp_PostSync_Config(1, 0, FR_HTW60, FR_VTW60, 0x3a, 0x2, 0x6, 0x4, PO_HW, PO_VW, UP(UpOCSel))

#define IspDout0SyncConfig()	Isp_Dout0_Sync_Config(FR_HTW60, UP(UpOutHSyncOfs), UP(UpOutVSyncOfs), 0, 0x2a, FR_HW, FR_VW)
#define IspDout1SyncConfig()	Isp_Dout1_Sync_Config(FR_HTW60, UP(UpOutHSyncOfs), UP(UpOutVSyncOfs), 0, 0x2a, FR_HW, FR_VW)

//------------------------------------------------------------------------------
#define InitSensRun			Sens(InitSensRun)


//******************************************************************************
//
//------------------------------------------------------------------------------






#endif // _DEV_TYPES_H_
