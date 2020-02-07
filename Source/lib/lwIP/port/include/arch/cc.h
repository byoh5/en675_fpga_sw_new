/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef LWIP_ARCH_CC_H
#define LWIP_ARCH_CC_H

#if 1
#include "dev.h"
#include <string.h>
#include <stdlib.h>

#include "enx_freertos.h"

#define X8_F  "02x"
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"
#define SZT_F "lu"

#ifndef BYTE_ORDER
		#define BYTE_ORDER	LITTLE_ENDIAN
#else
	#if	BYTE_ORDER!=BIG_ENDIAN
		#undef	BYTE_ORDER
		#define BYTE_ORDER	LITTLE_ENDIAN
	#endif
#endif

#ifndef LWIP_NO_STDDEF_H
#define LWIP_NO_STDDEF_H 0
#endif

#ifndef LWIP_NO_STDINT_H
#define LWIP_NO_STDINT_H 0
#endif

#ifndef LWIP_NO_INTTYPES_H
#define LWIP_NO_INTTYPES_H 0
#endif

#ifndef LWIP_TIMEVAL_PRIVATE
#define LWIP_TIMEVAL_PRIVATE 0
#endif

#ifndef LWIP_NO_LIMITS_H
#define LWIP_NO_LIMITS_H 0
#endif

#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT	__attribute__ ((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

#define SSIZE_MAX INT_MAX

//typedef uint64_t sys_prot_t;

//-------------------------------------------------------------------------------------------------
// H/W assist functions
WORD my_chksum(const void *dataptr, WORD len);
#define LWIP_CHKSUM	my_chksum
#define LWIP_RAND() ((u32_t)rand())	
//-------------------------------------------------------------------------------------------------
// malloc
#define mem_clib_free	vPortFree
#define mem_clib_malloc pvPortMalloc
#define mem_clib_calloc pvPortCalloc

//-------------------------------------------------------------------------------------------------
//
//#define LWIP_DEBUG								// Debug message print
//#define LWIP_NOASSERT							// Assert message print
#define LWIP_PROVIDE_ERRNO

#else
/* see https://sourceforge.net/p/predef/wiki/OperatingSystems/ */
#if defined __ANDROID__
#define LWIP_UNIX_ANDROID
#elif defined __linux__
#define LWIP_UNIX_LINUX
#elif defined __APPLE__
#define LWIP_UNIX_MACH
#elif defined __OpenBSD__
#define LWIP_UNIX_OPENBSD
#elif defined __CYGWIN__
#define LWIP_UNIX_CYGWIN
#endif

#define LWIP_TIMEVAL_PRIVATE 0
#include <sys/time.h>

/* different handling for unit test, normally not needed */
#ifdef LWIP_NOASSERT_ON_ERROR
#define LWIP_ERROR(message, expression, handler) do { if (!(expression)) { \
  handler;}} while(0)
#endif

#if defined(LWIP_UNIX_ANDROID) && defined(FD_SET)
typedef __kernel_fd_set fd_set;
#endif

struct sio_status_s;
typedef struct sio_status_s sio_status_t;
#define sio_fd_t sio_status_t*
#define __sio_fd_t_defined
#endif

#endif /* LWIP_ARCH_CC_H */
