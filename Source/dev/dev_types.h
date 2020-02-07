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

//******************************************************************************
// 1. System define
//------------------------------------------------------------------------------
#define CPU_ID					read_csr(mhartid)

#define BDMA_CNT				4
#define CDMA_CNT				4
#define GPIO_CNT				72
#define SDIO_CNT				2
#define UART_CNT				9
#define SPI_CNT					9
#define I2C_CNT					9
#define TIMER_CNT				39
#define USB_CNT					1
#define ETHERNET_CNT			1
#define I2S_CNT					1
#define ADC_CNT					4

#define CPU_BREAK()				__asm __volatile("EBREAK")
#define NOP()					__asm __volatile("NOP")

// uart printf out /////////////////////////////////////////////////////////////
#define DEBUG_UART_NUM 			7
#define INVALID_UART_NUM		0xFF


// Tick irq ////////////////////////////////////////////////////////////////////
#define TIME_TICK			100							// 10ms
#define TIME_1TICK_TO_MS	(1000 / TIME_TICK)
#define TIME_1MS			((CPU_FREQ / 100) / 1000)
#define TIMECMP_NEXT_VAL	((CPU_FREQ / 100) / TIME_TICK)


//******************************************************************************
//
//------------------------------------------------------------------------------
//
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

#define ABS(x)					( (x) < 0  ? -(x) : (x) )
#define ABSDIFF(A,B)			( (A) < (B) ? (B)-(A) : (A)-(B) )

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

typedef void (*idle_fn)(uint64_t tick);

//******************************************************************************
// CPUx<->CPUx S/W IRQ Message define
//------------------------------------------------------------------------------
#define CPU0_MSG_MUTEX	SYS_MUTEX0
#define CPU0_MSG_FLAG	SYS_REG0

enum {
	eCPU0_MSG_SHELL_RX		= 0x00000001,
	eCPU0_MSG_STREAM_INFO	= 0x00000002,
};

#define CPU1_MSG_MUTEX	SYS_MUTEX1
#define CPU1_MSG_FLAG	SYS_REG1

#define CPU2_MSG_MUTEX	SYS_MUTEX2
#define CPU2_MSG_FLAG	SYS_REG2

#define CPU3_MSG_MUTEX	SYS_MUTEX3
#define CPU3_MSG_FLAG	SYS_REG3


//******************************************************************************
// IRQ define
//------------------------------------------------------------------------------
#define IRQ_SOURCE_COUNT			40

typedef struct {
	uint64_t sync_count;
	uint64_t swirq_count;
	uint64_t timeirq_count;
	uint64_t exirq_count[IRQ_SOURCE_COUNT + 1];
} irq_count;

typedef enum {
	eigiISP = 1,
	eigiVCODEC = 2,
	eigiDMA0 = 3,
	eigiDMA1 = 4,
	eigiDMA2 = 5,
	eigiDMA3 = 6,
	eigiATOB = 7,
	eigiBTOA = 8,
	eigiETH = 9,
	eigiSDIO0 = 10,
	eigiSDIO1 = 11,
	eigiNPU = 12,
	eigiI2S = 13,
	eigiUSB = 14,
	eigiCHKSUM = 15,
	eigiSHA = 16,
	eigiASE = 17,
	eigiOIC = 18,
	eigiGPIO0_3_PWM0_3_SPI0 = 19,
	eigiGPIO4_7_PWM4_7_I2C0_UART0 = 20,
	eigiGPIO8_11_PWM8_11_SPI1 = 21,
	eigiGPIO12_15_PWM12_15_I2C1_UART1 = 22,
	eigiGPIO16_19_SPI2 = 23,
	eigiGPIO20_23_PWM16_19_I2C2_UART2 = 24,
	eigiGPIO24_27_PWM20_23_SPI3 = 25,
	eigiGPIO28_31_PWM24_27_I2C3_UART3 = 26,
	eigiGPIO32_35_PWM28_31_SPI4 = 27,
	eigiGPIO36_39_I2C4_UART4 = 28,
	eigiGPIO40_43_SPI5 = 29,
	eigiGPIO44_47_I2C5_UART5 = 30,
	eigiGPIO48_51_SPI6 = 31,
	eigiGPIO52_55_PWM32_34_I2C6_UART6 = 32,
	eigiGPIO56_59_PWM35_38_SPI7 = 33,
	eigiGPIO60_63_I2C7_UART7 = 34,
	eigiGPIO64_67_SPI8 = 35,
	eigiGPIO68_71_I2C8_UART8 = 36,
	eigiIR = 37,
	eigiOMC = 38,
	eigiReserved39 = 39,
	eigiReserved40 = 40,
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
// UART define
//------------------------------------------------------------------------------
typedef enum {
	UART_PARITY_NONE = 0,
	UART_PARITY_EVEN = 2,
	UART_PARITY_ODD = 3
} UART_PARITY_MODE;

//******************************************************************************
// I2C define
//------------------------------------------------------------------------------
typedef enum {
	I2C_Master = 0,			// Master
	I2C_Slave = 1,			// Slave
} I2C_MODE;

typedef enum {
	I2C_MSBfirst = 0,		// MSB first
	I2C_LSBfirst = 1,		// LSB first
} I2C_BITMODE;

//******************************************************************************
// SPI define
//------------------------------------------------------------------------------
typedef enum {
	SPI_WS_8BIT = 0,
	SPI_WS_16BIT = 1,
	SPI_WS_24BIT = 2,
	SPI_WS_32BIT = 3,
} SPI_WORD_SIZE;

typedef enum {
	SPI_CS_OUT_LOW = 0,		// Output - Low
	SPI_CS_OUT_HIGH = 1,	// Output - High
	SPI_CS_HIGH_Z = 2,		// High-z
} SPI_CSMODE;

typedef enum {
	SPI_MSBfirst = 0,		// MSB first
	SPI_LSBfirst = 1,		// LSB first
} SPI_BITMODE;

typedef enum {
	SPI_BYTE_MODE = 0,		// Byte mode
	SPI_BIT_MODE = 1,		// Bit mode
} SPI_ONEBITMODE;

typedef enum {
	SPI_CLKDIR_LOW_POSI = 0,	// Low + positive edge clock
	SPI_CLKDIR_LOW_NEGA = 1,	// Low + negative edge clock
	SPI_CLKDIR_HIGH_POSI = 2,	// High + positive edge clock
	SPI_CLKDIR_HIGH_NEGA = 3,	// High + negative edge clock
} SPI_CLKDIR_MODE;

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

typedef struct {
	char SFDPSignature[4];			// 00:03
	BYTE SFDPMinorNum;				// 04
	BYTE SFDPMajorNum;				// 05
	BYTE NumberParameterHeaders;	// 06
	BYTE _res00;					// 07
	BYTE IDNum;						// 08
	BYTE ParameterTableMinorNum;	// 09
	BYTE ParameterTableMajorNum;	// 0A
	BYTE ParameterTableLength;		// 0B
	UINT ParameterTablePointer:24;	// 0C:0E
	UINT _res01:8;					// 0F
} SFLSsfdp_header;

typedef struct {
	UINT BlockSectorEreaseSize:2;	// 00		00:reserved, 01:4KB-erase, 10:reserved, 11:64KB-erase
	UINT WriteGranularity:1;		//			0:no, 1:yes
	UINT WEIR_WVSR:1;				//			00:N/A, 01:use 50h opcode, 11:use 60h opcode
	UINT WEOS_WVSR:1;				//
	UINT _res04:3;					//
	UINT FourKBEraseOpcode:8;		// 01		xx:4KB Erase Support(opcode), FF:not supported
	UINT Sup112FastRead:1;			// 02		0:not supported, 1:supported
	UINT AddrByte:2;				//			00:3byte, 01:3-4byte(e.g. defaults to 3byte mode, enters 4byte mode on command) 10:4byte, 11:reserved
	UINT SupDTRClocking:1;			//			0:not supported, 1:supported
	UINT Sup122FastRead:1;			//			0:not supported, 1:supported
	UINT Sup144FastRead:1;			//			0:not supported, 1:supported
	UINT Sup114FastRead:1;			//			0:not supported, 1:supported
	UINT _res05:1;					//
	UINT _res06:8;					// 03
	UINT FlashMemoryDensity;		// 04:07
	UINT DummyClock144FR:5;			// 08
	UINT Modebit144FR:3;			//
	UINT Op144FR:8;					// 09
	UINT DummyClock114FR:5;			// 0A
	UINT Modebit114FR:3;			//
	UINT Op114FR:8;					// 0B
	UINT DummyClock112FR:5;			// 0C
	UINT Modebit112FR:3;			//
	UINT Op112FR:8;					// 0D
	UINT DummyClock122FR:5;			// 0E
	UINT Modebit122FR:3;			//
	UINT Op122FR:8;					// 0F
	UINT Sup222FastRead:1;			// 10		0:not supported, 1:supported
	UINT _res07:3;					//
	UINT Sup444FastRead:1;			//			0:not supported, 1:supported
	UINT _res08:3;					//
	UINT _res09:24;					// 11:13
	UINT _res10:16;					// 14:15
	UINT DummyClock222FR:5;			// 16
	UINT Modebit222FR:3;			//
	UINT Op222FR:8;					// 17
	UINT _res11:16;					// 18:19
	UINT DummyClock444FR:5;			// 1A
	UINT Modebit444FR:3;			//
	UINT Op444FR:8;					// 1B
	BYTE SectorType1Size;			// 1C
	BYTE SectorType1Op;				// 1D
	BYTE SectorType2Size;			// 1E
	BYTE SectorType2Op;				// 1F
	BYTE SectorType3Size;			// 20
	BYTE SectorType3Op;				// 21
	BYTE SectorType4Size;			// 22
	BYTE SectorType4Op;				// 23
	UINT _res12[3];					// 24:2F
	UINT _res13[4];					// 31:3F
	UINT _res14[4];					// 41:4F
	UINT _res15[4];					// 51:5F
	UINT _Res16[40];				// 60:FF
} SFLSsfdp;

typedef void (*sfls_func)(void);
typedef void (*sfls_erase_func)(UINT addr);
typedef BYTE (*sfls_read_status)(void);
typedef void (*sfls_write_status)(BYTE status);
typedef void (*sfls_read_sfdp_func)(BYTE *sfdp_bin);
typedef UINT (*sfls_check_func)(void);

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

	BYTE is_qpi;

	BYTE cmd_read;
	BYTE cmd_page_program;
	BYTE cmd_write_enable;
	BYTE cmd_read_status;

	BYTE gap;
	BYTE rdltc;

	UINT size;

	sfls_func func_init_1;
	sfls_func func_init_2;
	sfls_func func_write_enable;
	sfls_erase_func func_secter_erase;
	sfls_erase_func func_32k_erase;
	sfls_erase_func func_64k_erase;
	sfls_erase_func func_chip_erase;
	sfls_read_status func_read_status;
	sfls_write_status func_write_status;
	sfls_read_sfdp_func func_read_sfdp;
	sfls_func func_enter_4b;
	sfls_func func_exit_4b;
	sfls_check_func func_is_4b;
	sfls_func func_enter_qpi;
	sfls_func func_exit_qpi;
	sfls_check_func func_is_qpi;
	sfls_func func_enter_qe;
	sfls_func func_exit_qe;
	sfls_check_func func_is_qe;
	sfls_func func_enter_protection;
	sfls_func func_exit_protection;
	sfls_check_func func_is_protection;
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
// Checksum/SHA/AES define
//------------------------------------------------------------------------------
typedef enum {
	eSHA224 = 0,	// SHA_MODE=0
	eSHA256 = 1,	// SHA_MODE=1
} SHAmode;

typedef enum {				//				bit		type	mode
	eAES128_ECB_DEC = 0,	// AES_MODE=0	128		ecb		dec
	eAES128_ECB_ENC = 1,	// AES_MODE=1	128		ecb		enc
	eAES128_CBC_DEC = 2,	// AES_MODE=2	128		cbc		dec
	eAES128_CBC_ENC = 3,	// AES_MODE=3	128		cbc		enc
	eAES256_ECB_DEC = 4,	// AES_MODE=4	256		ecb		dec
	eAES256_ECB_ENC = 5,	// AES_MODE=5	256		ecb		enc
	eAES256_CBC_DEC = 6,	// AES_MODE=6	256		cbc		dec
	eAES256_CBC_ENC = 7,	// AES_MODE=7	256		cbc		enc
} AESmode;


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
// Ethernet define
//------------------------------------------------------------------------------
typedef enum {
	ENIF_NO_USE,
	ENIF_AUTO_NEGOTIATION,
	ENIF_MAN_10M_HALF,
	ENIF_MAN_10M_FULL,
	ENIF_MAN_100M_HALF,
	ENIF_MAN_100M_FULL,
	ENIF_MAN_1000M_FULL,
} ETHERNETIF_AUTONEGO;

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
#define IF_FUNC_FPS		30//25

#define	UART_HOLD_CNT	(2<<(IF_FUNC_FPS>30))					// Holding time
#define	UART_KEY_RPTF	(IF_FUNC_FPS>>2)						// First Repeat time
#define	UART_KEY_RPTT	(IF_FUNC_FPS>>3)						// Repeat time
#define	UART_KEY_RPTT2	(IF_FUNC_FPS>>4)						// Repeat time(Fast)	// 2016331 - WHL

#define PUSH_DELAY_NOT	1									// No Holding time
#define PUSH_DELAY_MIN	(IF_FUNC_FPS)							// Min Holding time
#define PUSH_DELAY_NOR	(IF_FUNC_FPS+1)							// Max Holding time


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
// BOX OSD
//------------------------------------------------------------------------------
#define ISP_BOX_EA			32				// (fix) number of Box
#define PVC_EA				16//32			// number of privacy box (max 32ea, CAUTION. share with IMD box ea)
#define MASK_EA				4				// number of IMD area box

typedef struct _tagPRIVACY{				// 5 byte 크기, 모두 BYTE형이므로 aligned 필요 없음
	BYTE	bAction;
	BYTE	bPosX;
	BYTE	bPosY;
	BYTE	bSizX;
	BYTE	bSizY;
} _PRIVACY;

typedef struct _IMD_RECT_W {
	WORD 	ey;
	WORD 	sy;
	WORD 	ex;
	WORD 	sx;
} wRECT;

typedef struct _IMD_RECT_B {
	BYTE 	ey;
	BYTE 	sy;
	BYTE 	ex;
	BYTE 	sx;
} bRECT;


//******************************************************************************
// User Parameter define
//------------------------------------------------------------------------------
#include "isp_user_parameter.h"

#define UPt(N)				N##_TYP									// Type of User Parameter

#define _UP_TYPEDEF_1(N,...)	typedef BYTE UPt(N);
#define _UP_TYPEDEF_2(N,...)	typedef WORD UPt(N);
#define _UP_TYPEDEF_4(N,...)	typedef UINT UPt(N);

#define	UP_TITLE_ALIGNED	2	// !!! User Parameter에서 UINT 사용 시 4로 설정해야 함 !!!
#define UP_TITLE(N)
#define UP_SET(S,N,...)		_UP_TYPEDEF_##S(N,...)
USR_PAR_LIST


#ifdef __ISP_LIB__
	#define UP(N)		(*gLib##N)

	#define PAR_BASE	(gLib##UpPAR00)

	#ifdef UP_LIB_LINK
		//#define UP_LIB1(N)	BYTE *gLib##N = 0;
		//#define UP_LIB2(N)	WORD *gLib##N = 0;
		//#define UP_LIB4(N)	UINT *gLib##N = 0;
		#define UP_LIB(N)	UPt(N) *gLib##N = 0;//UP_LIB##S(N)
	#else
		//#define UP_LIB1(N)	extern BYTE *gLib##N;
		//#define UP_LIB2(N)	extern WORD *gLib##N;
		//#define UP_LIB4(N)	extern UINT *gLib##N;
		#define UP_LIB(N)	extern UPt(N) *gLib##N;//UP_LIB##S(N)
	#endif

	UP_LIB_LIST
#else

#define UpListEx			((UP_LIST_EX*)gbUsrParTbl)
#define UpList				((UP_LIST*)gbUsrParTbl)
#define UP(N)				UpList->N
//#define UPd(N)				((UP_LIST*)gbUsrParTblSaved)->N

#define UPi(N)				N##_IDX									// Index of User Parameter
#define UPs(N)				N##_SIZ									// Size of User Parameter


//#define UPw(N,D)			UP(N) = (D)//SetByte(gbUsrParTbl+UPi(N), UPs(N), D)	// Write User Parameter
//#define UPr(N)				UP(N)//GetByte(gbUsrParTbl+UPi(N), UPs(N))		// Read User Parameter

//#define UPo(N)				UP(N)//, sizeof(UP(N))//gbUsrParTbl[UPi(N)], UPs(N)				// Use User Parameter in MENU
//#define UPx(N)				N//, sizeof(N)							// Use Default Type in MENU


#define gbCamTitle(INDEX)	gbUsrParTbl[UPi(CamTitle0)+((INDEX))]

#define UPinv(N)			(UPi(N)+UPs(N)-1)


//------------------------------------------------------------------------------
#define PAR_IDX1(N,...)		UPi(N),
#define PAR_IDX2(N,...)		PAR_IDX1(N,__VA_ARGS__) N##_1_IDX,
#define PAR_IDX3(N,...)		PAR_IDX2(N,__VA_ARGS__) N##_2_IDX,
#define PAR_IDX4(N,...)		PAR_IDX3(N,__VA_ARGS__) N##_3_IDX,

#define _UP_IDX_1(N,...)	                                                         PAR_IDX1(N,__VA_ARGS__)
#define _UP_IDX_2(N,...)	N##_UP_END }; enum { N##_UP_START=((N##_UP_END+1)&~1)-1, PAR_IDX2(N,__VA_ARGS__)
#define _UP_IDX_4(N,...)	N##_UP_END }; enum { N##_UP_START=((N##_UP_END+3)&~3)-1, PAR_IDX4(N,__VA_ARGS__)

#undef UP_TITLE
#define UP_TITLE(N)			N##_UP_END }; enum { N##_UP_START=((N##_UP_END+(UP_TITLE_ALIGNED-1))&~(UP_TITLE_ALIGNED-1))-1,
#undef UP_SET
#define UP_SET(S,N,...)		_UP_IDX_##S(N,...)
enum {
	UP_START=0,
	USR_PAR_LIST
	UP_END
};

//---------------------------------------------------------
#define _UP_TYPE_1(N,...)	BYTE N;
#define _UP_TYPE_2(N,...)	WORD N;
#define _UP_TYPE_4(N,...)	UINT N;

#undef UP_TITLE
#define UP_TITLE(...)		__attribute__((__aligned__(UP_TITLE_ALIGNED)))
#undef UP_SET
#define UP_SET(S,N,...)		_UP_TYPE_##S(N,...)//UPt(N) N;
typedef struct {
	BYTE Start;
	USR_PAR_LIST
	BYTE End;
	_PRIVACY	PVC0;
	_PRIVACY	PVC1;
	_PRIVACY	PVC2;
	_PRIVACY	PVC3;
	_PRIVACY	PVC4;
	_PRIVACY	PVC5;
	_PRIVACY	PVC6;
	_PRIVACY	PVC7;
	_PRIVACY	PVC8;
	_PRIVACY	PVC9;
	_PRIVACY	PVC10;
	_PRIVACY	PVC11;
	_PRIVACY	PVC12;
	_PRIVACY	PVC13;
	_PRIVACY	PVC14;
	_PRIVACY	PVC15;
	_PRIVACY	IMD0;
	_PRIVACY	IMD1;
	_PRIVACY	IMD2;
	_PRIVACY	IMD3;
} UP_LIST;

//---------------------------------------------------------
#undef UP_TITLE
#define UP_TITLE(N)		}; struct __UP_##N {
struct __UP_START { BYTE Start;
USR_PAR_LIST
}; struct __UP_END { BYTE End; };

#undef UP_SET
#define UP_SET(...)
#undef UP_TITLE
#define UP_TITLE(N)		typedef struct __UP_##N UP_##N;
USR_PAR_LIST

#undef UP_TITLE
#define UP_TITLE(N)		__attribute__((__aligned__(UP_TITLE_ALIGNED))) UP_##N	N;
typedef struct {
	BYTE Start;
	USR_PAR_LIST		// !!! ECM의 UP_END 위치때문에 마지막 구조체는 모두 BYTE이어야 함 !!!
	BYTE End;
	_PRIVACY	PVC0;
	_PRIVACY	PVC1;
	_PRIVACY	PVC2;
	_PRIVACY	PVC3;
	_PRIVACY	PVC4;
	_PRIVACY	PVC5;
	_PRIVACY	PVC6;
	_PRIVACY	PVC7;
	_PRIVACY	PVC8;
	_PRIVACY	PVC9;
	_PRIVACY	PVC10;
	_PRIVACY	PVC11;
	_PRIVACY	PVC12;
	_PRIVACY	PVC13;
	_PRIVACY	PVC14;
	_PRIVACY	PVC15;
	_PRIVACY	IMD0;
	_PRIVACY	IMD1;
	_PRIVACY	IMD2;
	_PRIVACY	IMD3;
} UP_LIST_EX;

//---------------------------------------------------------
#undef UP_TITLE
#define UP_TITLE(N)
#undef UP_SET
#define UP_SET(S,N,...)		enum { UPs(N) = S };
USR_PAR_LIST

#define MpPvcCfgIdx			(UP_END+1)
#define MpItlWinCfgIdx		(MpPvcCfgIdx + (PVC_EA*sizeof(_PRIVACY)))
#define USR_PAR_EA			(MpItlWinCfgIdx + (MASK_EA*sizeof(_PRIVACY)))


extern BYTE gbUsrParChgOn;
extern BYTE gbUsrParSaveChk;
extern BYTE gbUsrParReadChk;
extern BYTE gbUsrParTbl[USR_PAR_EA];
extern BYTE gbUsrParTblSaved[USR_PAR_EA];
extern BYTE gbStylePreviousOn;

#define PAR_BASE	((UINT*)(gbUsrParTbl+UPi(UpPAR00)))

//#define UP_LIB1(N) { extern BYTE *gLib##N; gLib##N = &UP(N); }
//#define UP_LIB2(N) { extern WORD *gLib##N; gLib##N = &UP(N); }
//#define UP_LIB4(N) { extern UINT *gLib##N; gLib##N = &UP(N); }
#define UP_LIB(N)	{ extern UPt(N) *gLib##N; gLib##N = &UP(N); }//UP_LIB##S(N)

#endif


#define USE_UP_PAR		0

#if USE_UP_PAR == 0
	extern UINT	gnRxPar[32];
	#undef PAR_BASE
	#define PAR_BASE	gnRxPar
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

#define DispDec(Y, X, VAL, LEN)						FontDecEx(Y, X, NO_ALPHA, MN_WHITE, VAL, LEN, 1)						// TODO KSH> Font Fnc 나중에 제거
#define DispDatHex(STR, Y, X, VAL)					FontStrHex( UP_ON, Y, X, MN_GREEN, STR, MN_WHITE, VAL, 8)
#define DispDatDec(STR, Y, X, VAL)					FontStrDec( UP_ON, Y, X, MN_GREEN, STR, MN_WHITE, VAL, MN_FONT_MAX_PATH, 1)

#define DispDat0Hex(ON, STR, Y, X, VAL)				FontStrHex( ON, Y, X, MN_GREEN, STR, MN_WHITE, VAL, 8)
#define DispDat0Dec(ON, STR, Y, X, VAL)				FontStrDec( ON, Y, X, MN_GREEN, STR, MN_WHITE, VAL, MN_FONT_MAX_PATH, 1)
#define DispDat0									DispDat0Hex
#define DebugDisp(ON, TYPE, STR, Y, X, VAL, ...)	DispDat0##TYPE( ON, STR, Y, X, VAL );									// Debug display by Debug mode

#define DispDat2Hex(ON, STR, Y, X, VAL, LEN)		FontStrHex( ON, Y, X, MN_GREEN, STR, MN_WHITE, VAL, LEN)
#define DispDat2Dec(ON, STR, Y, X, VAL, LEN)		FontStrDec( ON, Y, X, MN_GREEN, STR, MN_WHITE, VAL, LEN, 1)
#define DebugDisp2(ON, TYPE, STR, Y, X, VAL, LEN)	DispDat2##TYPE( ON, STR, Y, X, VAL, LEN );								// Debug display by Debug mode

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
#ifdef __ISP_LIB__
	#define SENS_SEL(...)
	#define RP_SEL(...)
#else
	//#define ENUM(N,V)		enum { N = V };

	//#define _SP1_(N,V)		enum { SP(N) = V };
	#define SENS_DEF(N,V,S)	enum { N##__SENS_##S = V };
	#define SENS_SEL(N,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10,V11,V12,V13,V14,V15,V16,V17,V18,V19,V20,S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15,S16,S17,S18,S19,S20)	\
		SENS_DEF(N,V1,S1)	SENS_DEF(N,V2,S2)	SENS_DEF(N,V3,S3)	SENS_DEF(N,V4,S4)	SENS_DEF(N,V5,S5)	SENS_DEF(N,V6,S6)	SENS_DEF(N,V7,S7)	SENS_DEF(N,V8,S8)	\
		SENS_DEF(N,V9,S9)	SENS_DEF(N,V10,S10)	SENS_DEF(N,V11,S11)	SENS_DEF(N,V12,S12)	SENS_DEF(N,V13,S13)	SENS_DEF(N,V14,S14)	SENS_DEF(N,V15,S15)	SENS_DEF(N,V16,S16)	\
		SENS_DEF(N,V17,S17)	SENS_DEF(N,V18,S18)	SENS_DEF(N,V19,S19)	SENS_DEF(N,V20,S20)

	//#define _RP1_(N,V)		enum { RP(N) = V };
	#define RP_DEF(N,V,R)	enum { N##__##R = V };
	#define RP_SEL(N,V1,V2,V3,V4,V5,R1,R2,R3,R4,R5) \
		enum { N##__##R1=V1 }; enum { N##__##R2=V2 }; enum { N##__##R3=V3 }; enum { N##__##R4=V4 }; enum { N##__##R5=V5 };
#endif

#include "isp_sensor_parameter.h"

extern UINT FPS_VDI;
extern UINT FPS_VDO;

extern UINT gnAeVtw;
extern UINT gnAeHtw;


//******************************************************************************
//
//------------------------------------------------------------------------------
//typedef double float32;
typedef float float32;

#define _itof(I)		((float32)(I))
#define _fmul(F1,F2)	((F1)*(F2))
#define _fdiv(F1,F2)	((F1)/(F2))
#define _fadd(F1,F2)	((F1)+(F2))
#define _ftoi(F)		((int)(F))
//#define TOFLOAT32(f)	f				// TODO KSH> AWB 초기화 코드 최적화 가능?

//******************************************************************************
//
//------------------------------------------------------------------------------




#endif // _DEV_TYPES_H_
