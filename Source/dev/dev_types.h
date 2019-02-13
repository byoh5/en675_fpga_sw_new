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

#if !defined INT_MAX
#define INT_MAX __INT_MAX__
#endif

#if !defined UINT_MAX
#define UINT_MAX (__INT_MAX__ * 2U + 1)
#endif

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
#define DEF_OK					1
#define DEF_FAIL				0

#define TRUE					1
#define FALSE					0

#ifndef NULL
#define NULL 					((void*)0)
#endif

#define	UNUSED(x)				((void)(x))

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

//******************************************************************************
// Register define
//------------------------------------------------------------------------------
//
#define BF_1(a1) \
				a1;
#define BF_2(a1,a2)\
				a2;a1;
#define BF_3(a1,a2,a3)\
				a3;a2;a1;
#define BF_4(a1,a2,a3,a4)\
				a4;a3;a2;a1;
#define BF_5(a1,a2,a3,a4,a5)\
				a5;a4;a3;a2;a1;
#define BF_6(a1,a2,a3,a4,a5,a6)\
				a6;a5;a4;a3;a2;a1;
#define BF_7(a1,a2,a3,a4,a5,a6,a7)\
				a7;a6;a5;a4;a3;a2;a1;
#define BF_8(a1,a2,a3,a4,a5,a6,a7,a8)\
				a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_9(a1,a2,a3,a4,a5,a6,a7,a8,a9)\
				a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_10(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)\
				a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_11(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)\
				a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_12(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)\
				a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_13(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)\
				a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_14(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14)\
				a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_15(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)\
				a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_16(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16)\
				a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_17(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17)\
				a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_18(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18)\
				a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_19(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19)\
				a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_20(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20)\
				a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_21(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21)\
				a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_22(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22)\
				a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_23(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23)\
				a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_24(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24)\
				a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_25(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25)\
				a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_26(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26)\
				a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_27(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27)\
				a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_28(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28)\
				a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_29(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29)\
				a29;a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_30(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30)\
				a30;a29;a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_31(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31)\
				a31;a30;a29;a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_32(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32)\
				a32;a31;a30;a29;a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;

#define	_regs_					typedef	union{UINT a; struct{	// Register union start
#define	_rege_					};}								// Register union end

#define	_am(uon,base,addr)		((volatile uon*)(UINT*)(base+(addr)))->a	// All macro
#define	_bm(uon,base,addr,mne)	((volatile uon*)(UINT*)(base+(addr)))->mne	// Bit macro

#endif // _DEV_TYPES_H_
