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

#define num_loop(val, MAX)		{ ++(val); if((val)>=(MAX)){val = 0;} }
#define cQueue_isfull(cQueue)	(((((cQueue)->tail+1) % (cQueue)->tot_num) == (cQueue)->head) ? ENX_OK : ENX_FAIL)
#define cQueue_isempty(cQueue)	(((cQueue)->head == (cQueue)->tail) ? ENX_OK : ENX_FAIL)

#define ENX_HTONL(x) ((((x) & (unsigned int)0x000000ffUL) << 24) | \
                     (((x) & (unsigned int)0x0000ff00UL) <<  8) | \
                     (((x) & (unsigned int)0x00ff0000UL) >>  8) | \
                     (((x) & (unsigned int)0xff000000UL) >> 24))

#define ISRT0		//__attribute__((section(".ispmtext")))
#define ISRT		//__attribute__((section(".ispmtext")))
#define ISRD		__attribute__((section(".ispmdata")))

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
	SDIO_DIO_MULTI_WRITE = 3
} eSDIO_DIO_TYPE;

typedef enum {
	SDIO_1BIT_MODE = 0,
	SDIO_4BIT_MODE = 1
} eSDIO_BIT_MODE;

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
	eSTREAMMSG_H265I = 1,
	eSTREAMMSG_H265P,
	eSTREAMMSG_H264I,
	eSTREAMMSG_H264P,
	eSTREAMMSG_JPEG,
	eSTREAMMSG_AUDIO,
	eSTREAMMSG_TXTS
} AVType;

typedef enum {
	e_res3840x2160,		// 16:9		UHD		8,294,400
	e_res2560x2048,		// 5:4		QSXGA	5,242,880
	e_res2560x1600,		// 16:10	WQVGA	4,096,000
	e_res2560x1440,		// 16:9		WQHD	3,686,400
	e_res2048x1536,		// 4:3		QXGA	3,145,728
	e_res1920x1200,		// 8:5		WUXGA	2,304,000
	e_res1920x1080,		// 16:9		FHD		2,073,600
	e_res1280x720,		// 16:9		HD		  921,600
	e_res800x600,		// 4:3		SVGA	  480,000
	e_res704x480,		// 3:2		-		  337,920
	e_res704x400,		// 16:9		-		  281,600
	e_res640x480,		// 4:3		VGA		  307,200
	e_res640x360,		// 16:9		-		  230,400
	e_res320x240,		// 4:3		QVGA	   76,800
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
// Key define
//------------------------------------------------------------------------------
#define FPS_VDO			30		// TODO KSH set FPS_VDO

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

#define UPi(N)				N##_IDX									// Index of User Parameter
#define UPs(N)				N##_SIZ									// Size of User Parameter

#define UPw(N,D)			SetByte(gbUsrParTbl+UPi(N), UPs(N), D)	// Write User Parameter
#define UPr(N)				GetByte(gbUsrParTbl+UPi(N), UPs(N))		// Read User Parameter

#define UPo(N)				gbUsrParTbl[UPi(N)], UPs(N)				// Use User Parameter in MENU
#define UPx(N)				N, sizeof(N)							// Use Default Type in MENU


#define gbCamTitle(INDEX)	gbUsrParTbl[UPi(UpCamTitle0)+((INDEX))]

#define UPinv(N)			(UPi(N)+UPs(N)-1)


//------------------------------------------------------------------------------
#define PAR_LIST1(N,...)	UPi(N),
#define PAR_LIST2(N,...)	PAR_LIST1(N,__VA_ARGS__) N##_1_IDX,
#define PAR_LIST3(N,...)	PAR_LIST2(N,__VA_ARGS__) N##_2_IDX,
#define PAR_LIST4(N,...)	PAR_LIST3(N,__VA_ARGS__) N##_3_IDX,

#define _UP_(S,N,...)		PAR_LIST##S(N,__VA_ARGS__)
#define _UPa_(S,N,...)		N##_UP_END }; enum { N##_UP_START=((N##_UP_END+3)&~3)-1, PAR_LIST##S(N,__VA_ARGS__)
#include "isp_par_tbl.h"
enum {
	UP_START=0,
	USR_PAR_LIST
	UP_END
};

#undef _UP_
#define _UP_(S,N,...)		enum { UPs(N) = S };
#undef _UPa_
#define _UPa_	_UP_
#include "isp_par_tbl.h"
USR_PAR_LIST

#define USR_PAR_EA			(UP_END+1)

extern BYTE gbUsrParSaveChk;
extern BYTE gbUsrParReadChk;
extern BYTE gbUsrParTbl[USR_PAR_EA];
extern BYTE gbUsrParTblSaved[USR_PAR_EA];

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


#define	PAR00			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 0))
#define	PAR01			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 1))
#define	PAR02			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 2))
#define	PAR03			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 3))
#define	PAR04			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 4))
#define	PAR05			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 5))
#define	PAR06			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 6))
#define	PAR07			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 7))
#define	PAR08			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 8))
#define	PAR09			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+ 9))
#define	PAR0A			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+10))
#define	PAR0B			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+11))
#define	PAR0C			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+12))
#define	PAR0D			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+13))
#define	PAR0E			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+14))
#define	PAR0F			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+15))
#define	PAR10			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+16))
#define	PAR11			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+17))
#define	PAR12			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+18))
#define	PAR13			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+19))
#define	PAR14			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+20))
#define	PAR15			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+21))
#define	PAR16			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+22))
#define	PAR17			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+23))
#define	PAR18			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+24))
#define	PAR19			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+25))
#define	PAR1A			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+26))
#define	PAR1B			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+27))
#define	PAR1C			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+28))
#define	PAR1D			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+29))
#define	PAR1E			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+30))
#define	PAR1F			(*(((UINT*)(gbUsrParTbl+UPi(UpPAR00)))+31))


//******************************************************************************
//
//------------------------------------------------------------------------------





#endif // _DEV_TYPES_H_
