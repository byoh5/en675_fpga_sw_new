/*
 * wpa_supplicant/hostapd / OS specific functions
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
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

#ifndef OS_H
#define OS_H

typedef long os_time_t;


struct os_time {
	os_time_t sec;
	os_time_t usec;
};

/**
 * os_get_time - Get current time (sec, usec)
 * @t: Pointer to buffer for the time
 * Returns: 0 on success, -1 on failure
 */
int os_get_time(struct os_time *t);


/* Helper macros for handling struct os_time */

#define os_time_before(a, b) \
	((a)->sec < (b)->sec || \
	 ((a)->sec == (b)->sec && (a)->usec < (b)->usec))

#define os_time_sub(a, b, res) do { \
	(res)->sec = (a)->sec - (b)->sec; \
	(res)->usec = (a)->usec - (b)->usec; \
	if ((res)->usec < 0) { \
		(res)->sec--; \
		(res)->usec += 1000000; \
	} \
} while (0)

/**
 * os_get_random - Get cryptographically strong pseudo random data
 * @buf: Buffer for pseudo random data
 * @len: Length of the buffer
 * Returns: 0 on success, -1 on failure
 */
int os_get_random(unsigned char *buf, size_t len);

/**
 * os_random - Get pseudo random value (not necessarily very strong)
 * Returns: Pseudo random value
 */
unsigned long os_random(void);


/**
 * os_zalloc - Allocate and zero memory
 * @size: Number of bytes to allocate
 * Returns: Pointer to allocated and zeroed memory or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer with os_free().
 */
void * os_zalloc(size_t size);

/**
 * os_strdup - Duplicate a string
 * @s: Source string
 * Returns: Allocated buffer with the string copied into it or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer with os_free().
 */
char * os_strdup(const char *s);

#ifndef os_malloc
#define os_malloc(s) ewl_os_malloc((s))
#endif

#ifndef os_realloc
#define os_realloc(p, s) ewl_os_realloc((p), (s))
#endif

#ifndef os_free
#define os_free(p) ewl_os_free((p))
#endif

#ifndef os_memcpy
#define os_memcpy(d, s, n) ewl_os_memcpy((d), (s), (n))
#endif

#ifndef os_memmove
#define os_memmove(d, s, n) ewl_os_memmove((d), (s), (n))
#endif

#ifndef os_memset
#define os_memset(s, c, n) ewl_os_memset(s, c, n)
#endif

#ifndef os_memcmp
#define os_memcmp(s1, s2, n) ewl_os_memcmp((s1), (s2), (n))
#endif

#ifndef os_strlen
#define os_strlen(s) ewl_os_strlen(s)
#endif

#ifndef os_strchr
#define os_strchr(s, c) ewl_os_strrchr((s), (c))
#endif

#ifndef os_strcmp
#define os_strcmp(s1, s2) ewl_os_strcmp((s1), (s2))
#endif

#ifndef os_strncmp
#define os_strncmp(s1, s2, n) ewl_os_strncmp((s1), (s2), (n))
#endif

#ifndef os_strstr
#define os_strstr(h, n) ewl_os_strstr((h), (n))
#endif

#ifndef os_strncpy
#define os_strncpy(d, s, n) ewl_os_strncpy((d), (s), (n))
#endif

#ifndef os_strrchr
#define os_strrchr(s, c) ewl_os_strrchr((s), (c))
#endif

#ifndef os_snprintf
#define os_snprintf ewl_os_snprintf
#endif

#ifndef os_atoi
#define os_atoi ewl_os_atoi
#endif





#endif /* OS_H */
