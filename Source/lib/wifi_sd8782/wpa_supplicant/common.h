/*
 * wpa_supplicant/hostapd / common helper functions, etc.
 * Copyright (c) 2002-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef COMMON_H
#define COMMON_H

#define WPA_DEBUG_MSG_ON 0

#include "os.h"
#ifndef IFNAMSIZ
#define IFNAMSIZ        16
#endif


#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321
#define __BYTE_ORDER __BIG_ENDIAN

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define le_to_host16(n) (n)
#define host_to_le16(n) (n)
#define be_to_host16(n) bswap_16(n)
#define host_to_be16(n) bswap_16(n)
#define le_to_host32(n) (n)
#define be_to_host32(n) bswap_32(n)
#define host_to_be32(n) bswap_32(n)
#define le_to_host64(n) (n)
#define host_to_le64(n) (n)
#define be_to_host64(n) bswap_64(n)
#define host_to_be64(n) bswap_64(n)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define le_to_host16(n) bswap_16(n)
#define host_to_le16(n) bswap_16(n)
#define be_to_host16(n) (n)
#define host_to_be16(n) (n)
#define le_to_host32(n) bswap_32(n)
#define be_to_host32(n) (n)
#define host_to_be32(n) (n)
#define le_to_host64(n) bswap_64(n)
#define host_to_le64(n) bswap_64(n)
#define be_to_host64(n) (n)
#define host_to_be64(n) (n)
#ifndef WORDS_BIGENDIAN
#define WORDS_BIGENDIAN
#endif
#else
#error Could not determine CPU byte order
#endif


/* Macros for handling unaligned 16-bit variables */
#define WPA_GET_BE16(a) ((u16) (((a)[0] << 8) | (a)[1]))
#define WPA_PUT_BE16(a, val)			\
	do {					\
		(a)[0] = ((u16) (val)) >> 8;	\
		(a)[1] = ((u16) (val)) & 0xff;	\
	} while (0)

#define WPA_GET_LE16(a) ((u16) (((a)[1] << 8) | (a)[0]))
#define WPA_PUT_LE16(a, val)			\
	do {					\
		(a)[1] = ((u16) (val)) >> 8;	\
		(a)[0] = ((u16) (val)) & 0xff;	\
	} while (0)

#define WPA_GET_BE24(a) ((((u32) (a)[0]) << 16) | (((u32) (a)[1]) << 8) | \
			 ((u32) (a)[2]))
#define WPA_PUT_BE24(a, val)				\
	do {						\
		(a)[0] = (u8) (((u32) (val)) >> 16);	\
		(a)[1] = (u8) (((u32) (val)) >> 8);	\
		(a)[2] = (u8) (((u32) (val)) & 0xff);	\
	} while (0)

#define WPA_GET_BE32(a) ((((u32) (a)[0]) << 24) | (((u32) (a)[1]) << 16) | \
			 (((u32) (a)[2]) << 8) | ((u32) (a)[3]))
#define WPA_PUT_BE32(a, val)				\
	do {						\
		(a)[0] = (u8) (((u32) (val)) >> 24);	\
		(a)[1] = (u8) (((u32) (val)) >> 16);	\
		(a)[2] = (u8) (((u32) (val)) >> 8);	\
		(a)[3] = (u8) (((u32) (val)) & 0xff);	\
	} while (0)

#define WPA_PUT_BE64(a, val)				\
	do {						\
		(a)[0] = (u8) (((u64) (val)) >> 56);	\
		(a)[1] = (u8) (((u64) (val)) >> 48);	\
		(a)[2] = (u8) (((u64) (val)) >> 40);	\
		(a)[3] = (u8) (((u64) (val)) >> 32);	\
		(a)[4] = (u8) (((u64) (val)) >> 24);	\
		(a)[5] = (u8) (((u64) (val)) >> 16);	\
		(a)[6] = (u8) (((u64) (val)) >> 8);	\
		(a)[7] = (u8) (((u64) (val)) & 0xff);	\
	} while (0)


#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#define WPA_ASSERT(a) do { } while (0)

#define hostapd_get_rand os_get_random
int hwaddr_aton(const char *txt, u8 *addr);
int hexstr2bin(const char *hex, u8 *buf, size_t len);
void inc_byte_array(u8 *counter, size_t len);
void wpa_get_ntp_timestamp(u8 *buf);
int wpa_snprintf_hex(char *buf, size_t buf_size, const u8 *data, size_t len);

#define PRINTF_FORMAT(a,b) 	__attribute__ ((format (printf, (a), (b))))
#define STRUCT_PACKED 		__attribute__ ((packed))


int hexstr2bin(const char *hex, u8 *buf, size_t len);

/* Debugging function - conditional printf and hex dump. Driver wrappers can
 * use these for debugging purposes. */

enum { MSG_MSGDUMP, MSG_DEBUG, MSG_INFO, MSG_WARNING, MSG_ERROR };



#define wpa_debug_print_timestamp() do { } while (0)

void wpa_hexdump(int level, const char *title, const u8 *buf, size_t len);
void wpa_hexdump_key(int level, const char *title, const u8 *buf, size_t len);
#define wpa_hexdump_ascii(l,t,b,le) do { } while (0)
#define wpa_hexdump_ascii_key(l,t,b,le) do { } while (0)
#define wpa_debug_open_file() do { } while (0)
#define wpa_debug_close_file() do { } while (0)

#if (WPA_DEBUG_MSG_ON == 1)
#define wpa_msg(a, b, pformat, args...) do {printf(pformat "\n", ## args); } while (0)
#define wpa_printf(level, pformat, args...) do {printf(pformat "\n", ## args); } while (0)
#else
#define wpa_msg(a, b, pformat, args...) do { } while (0)
#define wpa_printf(level, pformat, args...) do { } while (0)
#endif

#endif /* COMMON_H */
