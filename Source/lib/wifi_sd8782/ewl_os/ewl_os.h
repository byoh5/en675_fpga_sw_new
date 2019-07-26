#ifndef __EWL_OS_H__
#define __EWL_OS_H__

/*
 * library functions needed by ewl_lib
 *
 */

#include "dev.h"

//#include <types.h>
#include "enx_freertos.h" // for TimerHandle_t
#include <stdarg.h> /* for va_list  */

void* 
ewl_os_malloc(int bytes);

void*
ewl_os_realloc(void* oldmem, size_t bytes);

void 
ewl_os_free(void *p);

void *
ewl_os_memset(void *s, int c, size_t n);


void *
ewl_os_memcpy(void *dst, const void *src, size_t n);

int 
ewl_os_strncmp(const char * cs,const char * ct,size_t count);

size_t 
ewl_os_strlen(const char * s);

char * 
ewl_os_strcpy(char * dest,const char *src);

char * 
ewl_os_strncpy(char * dest,const char *src,size_t count);

void *
ewl_os_memmove(void *dest0, void const *source0, size_t length);

int 
ewl_os_strcmp(const char * cs,const char * ct);

int 
ewl_os_memcmp(const void *s1, const void *s2, size_t n);

char * 
ewl_os_strrchr(const char * s, int c);

char * 
ewl_os_strstr(const char * s1,const char * s2);

int 
ewl_os_snprintf(char* str, size_t n, const char* format, ...);

u32 
ewl_os_timer_get_ms(void);

void 
ewl_os_timer_cancel_timeout(TimerHandle_t id);

TimerHandle_t
ewl_os_timer_sched_timeout_cb(u32 ms, u8 type, void (*cb)(void *ctx), void* ctx);

void
ewl_os_disable_int(void);

void
ewl_os_restore_int(void);

void
ewl_os_wait_us(u32 us);

int 
ewl_os_printf(const char *fmt, ...);

int 
ewl_os_vprint(const char *fmt, va_list args);

void
ewl_os_hexdump(char *prompt, u8 * buf, int len);

int 
ewl_os_atoi(char *s);

#endif /* __EWL_OS_H__ */
