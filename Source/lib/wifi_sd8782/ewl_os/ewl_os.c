/* ------------------------ OR32 includes -------------------------------- */
#include "dev.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "enx_freertos.h"
#include "ewl_os.h"

#ifndef MAX_PRINTF_LEN
#define MAX_PRINTF_LEN 160
#endif

#if 1
#define ENTER_CRITICAL()  CRITICAL_BEGIN

#define EXIT_CRITICAL()   CRITICAL_END
#else
#define ENTER_CRITICAL() 

#define EXIT_CRITICAL()   

#endif

/*
    is defined in sdio.c
*/
//void disable_int(void);

//void restore_int(void);

/*
 * library functions needed by ewl_lib
 *
 */

void* 
ewl_os_malloc(int bytes)
{
	void *new = pvPortMalloc(bytes);
	if (new == NULL) {
		printf("%s fail(size:%d)\r\n", __func__, bytes);
	}
	return new;
}

void*
ewl_os_realloc(void* oldmem, size_t bytes)
{
    void *new = pvPortRealloc(oldmem, bytes);
	if (new == NULL) {
		printf("%s fail(size:%d)\r\n", __func__, bytes);
	}
    return new;
}

void*
ewl_os_calloc(size_t n_elements, size_t elem_size)
{
    void *new = pvPortCalloc(n_elements, elem_size);
	if (new == NULL) {
		printf("%s fail(size:%d)\r\n", __func__, n_elements*elem_size);
	}
    return new;
}

void 
ewl_os_free(void *p)
{
    vPortFree(p);
}

void *
ewl_os_memset(void *s, int c, size_t n)
{
    return memset(s, c, n);
}

void *
ewl_os_memcpy(void *dst, const void *src, size_t n)
{
    return memcpy(dst, src, n);
}

int 
ewl_os_strncmp(const char * cs,const char * ct,size_t count)
{
    return strncmp(cs,ct,count);
}

char * 
ewl_os_strcpy(char * dest,const char *src)
{
    return strcpy(dest,src);
}

int 
ewl_os_strcmp(const char * cs,const char * ct)
{
    return strcmp(cs,ct);
}

size_t 
ewl_os_strlen(const char * s)
{
    return strlen(s);
}

char * 
ewl_os_strncpy(char * dest,const char *src,size_t count)
{
    return strncpy(dest,src,count);
}

void *
ewl_os_memmove(void *dest0, void const *source0, size_t length)
{
#if 0
    return memmove(dest0,source0,length);
#else
	size_t i;
	void *temp_src;
	void *temp_dest = dest0;
	temp_src = (void *)ewl_os_malloc(length);
	for(i=0;i<length;i++)
	{
		*((char*)temp_src+i) = *((char*)source0+i);
	}
	for(i=0;i<length;i++)
	{
		*((char*)temp_dest+i) = *((char*)temp_src+i);
	}
	ewl_os_free(temp_src);
	return dest0;
#endif
}

int 
ewl_os_memcmp(const void *s1, const void *s2, size_t n)
{
    return memcmp(s1, s2, n);
}

int
ewl_os_strnicmp(char *s1, char *s2, size_t n)
{
#if 0
    return strnicmp(s1,s2,n);
#else
     size_t i;
     int res = 0;
     for(i = 0; s1[i] != '\0'; i++) {
          if(s1[i] >= 65 && s1[i] <= 90)
               continue;
          else if(s1[i] >= 97 && s1[i] <= 122)
               s1[i] -= 32;
     }
 
     for(i = 0; s2[i] != '\0'; i++) {
          if(s2[i] >= 65 && s2[i] <= 90)
               continue;
          else if(s2[i] >= 97 && s2[i] <= 122)
               s2[i] -= 32;
     }
 
     for(i = 0; i < n; i++) {
          if((s1[i] == '\0') && (s2[i]) == '\0')
               res = 0;
          else if((s1[i] != '\0') && (s2[i]) == '\0')
               res = 1;
          else if((s1[i] == '\0') && (s2[i]) != '\0')
               res = -1;
  
          if((int)s1[i] > (int)s2[i])
               res = 1;
          else if((int)s1[i] < (int)s2[i])
               res = -1;
     }
     return res;
#endif
}

// temp code => change lib
size_t strspn(const char *string, const char *control)
{
    const unsigned char *str = (const unsigned char *)string;
    const unsigned char *ctrl = (const unsigned char *)control;

    unsigned char map[32];
    int count;

    // Clear out bit map
    for (count = 0; count < 32; count++) map[count] = 0;

    // Set bits in control map
    while (*ctrl)
    {
        map[*ctrl >> 3] |= (1 << (*ctrl & 7));
        ctrl++;
    }

    // 1st char NOT in control map stops search
    if (*str)
    {
        count = 0;
        while (map[*str >> 3] & (1 << (*str & 7)))
        {
            count++;
            str++;
        }

        return count;
    }

    return 0;
}

char * 
ewl_os_strrchr(const char * s, int c)
{
    return strchr(s,c);
}

char * 
ewl_os_strstr(const char * s1,const char * s2)
{
    return strstr(s1,s2);
}

u32
ewl_os_timer_get_ms(void)
{
    return TimeGetMs();
}

void 
ewl_os_timer_cancel_timeout(TimerHandle_t id)
{
#if 1
	if (id) {
		xTimerDelete(id, 0);
	}
#else
    timer_cancel_timeout(id);
#endif
}

TimerHandle_t
ewl_os_timer_sched_timeout_cb(u32 ms, u8 type, void (*cb)(void *ctx), void* ctx)
{
#if 1
	TimerHandle_t th = xTimerCreate("ewlosT", ms, type, ( void * )ctx, cb);
	if (th) {
		BaseType_t xReturn = xTimerStart(th, 0);
		if (xReturn == pdPASS) {
			return th;
		} else {
			printf("%s(%d): error.\n", __func__, __LINE__);
			return NULL;
		}
	} else {
		printf("%s(%d): error.\n", __func__, __LINE__);
		return NULL;
	}
#else
    return timer_sched_timeout_cb(ms,type,cb,ctx);
#endif
}

void ewl_os_disable_int(void)
{
	portENTER_CRITICAL();
}

void ewl_os_restore_int(void)
{
	portEXIT_CRITICAL();
}

void
ewl_os_wait_us(u32 us)
{
    WaitXus(us);
}

int 
ewl_os_printf(const char *fmt, ...)
{
	char str[MAX_PRINTF_LEN] = {0};
	va_list ap;
	va_start(ap, fmt);
	int len = vsnprintf(str, MAX_PRINTF_LEN, fmt, ap);
	va_end(ap);
	printf(str);

	return len;
}

void
ewl_os_hexdump(char *prompt, u8 * buf, int len)
{
	int i, pos = 0;
	char strbuff[MAX_PRINTF_LEN] = {0};
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)buf;

	// Output description if given.
	if (prompt != NULL)
		ewl_os_printf("%s:\r\n", prompt);

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
			{
				strbuff[pos++] = ' ';
				strbuff[pos++] = ' ';
				ewl_os_strncpy(&strbuff[pos], (char *)buff, 16);
				ewl_os_printf("%s\r\n", strbuff);
				pos = 0;
			}

			// Output the offset.
			ewl_os_snprintf(&strbuff[pos], MAX_PRINTF_LEN - pos, "  %04x ", i);
			pos += 7;
		}

		// Now the hex code for the specific character.
		ewl_os_snprintf(&strbuff[pos], MAX_PRINTF_LEN - pos, " %02x", pc[i]);
		pos += 3;

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];
		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		i++;
	}

	// And print the final ASCII bit.
	strbuff[pos++] = ' ';
	strbuff[pos++] = ' ';
	ewl_os_strncpy(&strbuff[pos], (char *)buff, 16);
	pos += 16;
	ewl_os_printf("%s\r\n", strbuff);
}


int 
ewl_os_vprint(const char *fmt, va_list args)
{
	char str[MAX_PRINTF_LEN] = {0};
	int len = vsnprintf(str, MAX_PRINTF_LEN, fmt, args);
	printf(str);

	return len;
}

int 
ewl_os_snprintf(char* str, size_t n, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int len = vsnprintf(str, n, format, args);
	va_end(args);

	return len;
}

int 
ewl_os_atoi(char *s)
{
	return atoi(s);
}
