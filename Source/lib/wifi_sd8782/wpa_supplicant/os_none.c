/*
 * wpa_supplicant/hostapd / Empty OS specific functions
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
 *
 * This file can be used as a starting point when adding a new OS target. The
 * functions here do not really work as-is since they are just empty or only
 * return an error value. os_internal.c can be used as another starting point
 * or reference since it has example implementation of many of these functions.
 */

#include "includes.h"

#include "os.h"

int isaac_rand(unsigned long **rand); /* isaac.c */

/*
    1 tick is 10ms in Non-OS
*/
#if 0
void os_sleep(os_time_t sec, os_time_t usec)
{
    u32 xTick;
    
    xTick = sec*100 + usec/10000;
    timer_delay(xTick*10); /*blocking waiting*/
}
#endif

/*
    get time in Non-OS
*/
int os_get_time(struct os_time *t)
{
    u32     ms;
    ms = ewl_os_timer_get_ms();
    t->sec  = ms/1000;
    t->usec = (ms%1000)*1000;
    return 0;
}

#if 0
int os_mktime(int year, int month, int day, int hour, int min, int sec,
          os_time_t *t)
{
    return -1;
}
#endif

int os_get_random(unsigned char *buf, size_t len)
{
#ifdef ISSAC
    unsigned long *plrand;
     
    isaac_rand(&plrand);
    
    memcpy(buf,(char*)plrand,len);
#endif
    return 0;
	UNUSED(buf);
	UNUSED(len);
}

void * os_zalloc(size_t size)
{
    void *result = ewl_os_malloc(size);
    ewl_os_memset(result,0,size);
    return result;
}

char * os_strdup(const char *s)
{
    char *result = (char*)ewl_os_malloc(strlen(s) + 1);
    
    if (result == (char*)0)
        return (char*)0;
    
    ewl_os_strcpy(result, s);
    return result;
}

