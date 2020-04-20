/* 
 * Copyright (c) 2018, Chips&Media
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "dev.h" //#include "../../en675.h"
#define EYENIX_FIX
#define BDMA_USE
//#define BDMA_USE_RTOS

#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include "vpuconfig.h"
#include "vputypes.h"

#include "../vdi_osal.h"

#ifdef EYENIX_TEST

#define EYENIX_SYSTEM_HEAP

#define ALIGN_16_SIZE   	16//8
#define ALIGN_16(x) 		(((x) + ALIGN_16_SIZE - 1) & ~(ALIGN_16_SIZE - 1))

#if 1

//system end 0x9030_0000

//#define YUV_BUFFER_BASE  		(DDR1_BASE + 0x300000)//, size (36Mbyte)
//#define YUV_SUBBUFFER_BASE  	(DDR1_BASE + 0x2700000)//, size (6Mbyte)


#define FILE_BUFFER_1CH_SIZE (9*1024*1024)
#define FILE_BUFFER_2CH_SIZE (2*1024*1024)

#define BIT_BUFFER_BASE  		(DDR1_BASE +0x600000)//, size (9Mbyte)
#define BIT_SUB_BUFFER_BASE  	(DDR1_BASE +0xF00000)//, size (2Mbyte)


#define DEC_BIT_BUFFER_BASE  		(DDR1_BASE +0x1100000)//, size (9Mbyte)
#define DEC_BIT_SUB_BUFFER_BASE  	(DDR1_BASE +0x1A00000)//, size (2Mbyte)

#define BIT_READER_BASE  		(BIT_BUFFER_BASE)
#define BIT_READER_SUB_BASE  	(BIT_SUB_BUFFER_BASE)

//#define BIT_RENDER_BASE  (YUV_BUFFER_BASE)
#define BIT_BUFFER_1CH_SIZE (9*1024*1024)
#define BIT_BUFFER_2CH_SIZE (2*1024*1024)

#ifndef EYENIX_TEST_HEAP
#define YUV_BUFFER_BASE  (DDR1_BASE +0x1100000)
#define YUV_SUBBUFFER_BASE  (DDR1_BASE +0x1a00000)
#endif

#else
#define FILE_BUFFER_BASE (DDR0_BASE +0xDC00000)//(DDR0_BASE +0xC800000)//(DDR0_BASE+0x500000) //simulater test //ksj porting for EN675 //0x5000000

#define YUV_SUBBUFFER_BASE  (DDR0_BASE +0x7800000)
#define BIT_READER_BASE  (YUV_SUBBUFFER_BASE + 0xA00000)
#define BIT_RENDER_BASE  (BIT_READER_BASE+0x500000)

#define YUV_BUFFER_BASE  (DDR0_BASE +0xE700000)//(DDR0_BASE +0xDC00000)//0x8D200000//(DDR0_BASE +0x2000000)//(DDR0_BASE+0x10000000) //simulater test //ksj porting for EN675 //0x5000000
#define BIT_BUFFER_BASE  (DDR0_BASE +0xDD00000)//(DDR0_BASE +0xD200000)//(DDR0_BASE +0x3000000)
#define BIT_SUB_BUFFER_BASE  (DDR0_BASE +0xE800000)
#define FILE_BUFFER_SIZE 800*1024 //test size //ksj porting for EN675  //1024*1024*10
#define FILE_BUFFER_1CH_SIZE (12*1024*1024)
#define FILE_BUFFER_2CH_SIZE (2*1024*1024)
#define BIT_BUFFER_1CH_SIZE (14977)
#define BIT_BUFFER_2CH_SIZE (18193)

#endif

#ifndef EYENIX_SYSTEM_HEAP
UINT MALLOC_ADDRESS = DDR1_BASE+0x2000000; //DDR0_BASE+0x700000; //= DDR0_BASE+0XFA00000; //250Mbyte offset
#endif


#ifdef CODECBIN_FROM_BIN
#include "chagall17.h"
//#include "chagall.h"
//#include "hevc_code_little.h"
//#include "chagall_debug.h"
//#include "chagall_debug2.h"
#endif

#ifdef BIT_FROM_BIN
#include "test416_240_hevc_all_i.h"
#endif


#ifndef EYENIX_SYSTEM_HEAP
UINT OFFSET_ADDRESS = 0;
#endif

#else
#define FILE_BUFFER_BASE 0x5000000
#define FILE_BUFFER_SIZE 1024*1024*10

#endif


static osal_file_t s_file[100];
static int fileindex;

int InitLog() 
{
    return 1;	
}

void DeInitLog()
{

}

void SetMaxLogLevel(int level)
{
}
int GetMaxLogLevel(void)
{
    return -1;
}

void LogMsg(int level, const char *format, ...)
{
    va_list ptr;
    char logBuf[MAX_PRINT_LENGTH] = {0};

    va_start( ptr, format );

    vsnprintf_( logBuf, MAX_PRINT_LENGTH, format, ptr ); //ksj porting for EN675

    va_end(ptr);

    dbgprintf(logBuf);

}


void osal_init_keyboard()
{

}

void osal_close_keyboard()
{

}

void * osal_memcpy(void * dst, const void * src, int count)
{
	//printf("memcpy dst 0x%x src 0x%x size %d\n", dst, src, count);
#ifdef BDMA_USE
	 //hwflush_dcache_range_all();

	 //if(count < 64 || (((int)dst)&0xF) || (((int)src)&0xF) || (count%16))
	//	 memcpy(dst, src, count);
	// else
	 {
		 //dbgprintf("BDmaMemSet_isr src 0x%x dst 0x%x size %d\n", src, dst, count);
#ifdef BDMA_USE_RTOS
		 BDmaMemCpy_rtos_flush(CPU_ID, dst, src, count);
#else
		 hwflush_dcache_range(dst, count);
		 hwflush_dcache_range(src, count);
		 BDmaMemCpy_isr(CPU_ID, dst, src, count);
		 hwflush_dcache_range(dst, count);
		 hwflush_dcache_range(src, count);
#endif
	 }
#else
	 hwflush_dcache_range(src, count);
	 hwflush_dcache_range(dst, count);
	 memcpy(dst, src, count);//lint !e670
	 hwflush_dcache_range(dst, count);
	 hwflush_dcache_range(src, count);
#endif
     return;
}

int osal_memcmp(const void* src, const void* dst, int size)
{
	int ret;
    ret= memcmp(src, dst, size);
    //hwflush_dcache_range_all();
	 hwflush_dcache_range(dst, size);
	 hwflush_dcache_range(src, size);
    return ret;
}

void * osal_memset(void *dst, int val, int count)
{
		//printf("memset 0x%x val %d size %d\n", dst, val, count);
#ifdef BDMA_USE


	// if(count < 64 || (((int)dst)&0xf) || (count%16))
	//	 memset(dst, val, count);
	// else
	 {
		 //hwflush_dcache_range_all();
#ifdef BDMA_USE_RTOS
		 BDmaMemSet_rtos_flush(CPU_ID, dst, val, count);
#else
		 hwflush_dcache_range(dst, count);
		 BDmaMemSet_isr(CPU_ID, dst, val, count);
		 hwflush_dcache_range(dst, count);
#endif
	 }
#else
	hwflush_dcache_range(dst, count);
    memset(dst, val, count);
	hwflush_dcache_range(dst, count);
#endif
}


void * osal_malloc(int size)
{
#if 0
	register ULONG hs, he;
	asm volatile("la %0, _heap_start" : "=r"(hs));
	asm volatile("la %0, _heap_end" : "=r"(he));
#endif
#ifndef EYENIX_SYSTEM_HEAP
	BYTE *address;
	static UINT bigsize=0;
	if (OFFSET_ADDRESS > 40*1024*1024) {
		dbgprintf("malloc fail 1 %x \n", OFFSET_ADDRESS);
		return NULL;
	}

	address = (BYTE *)(MALLOC_ADDRESS+OFFSET_ADDRESS);
	OFFSET_ADDRESS += ALIGN_16(size);

	if (address) {

		if(bigsize<ALIGN_16(size))
			bigsize = ALIGN_16(size);

		dbgprintf("malloc %d byte bigsize %d - address(0x%08X)\n", size, bigsize, address);
	} else {
		dbgprintf("malloc fail 2\n");
	}

	return address;
#else	
	int *addr;

	addr = malloc(size);
	if(addr == NULL)
	{
		dbgprintf("malloc size %d address %x \n", size, addr);
		while(1);
	}
	//dbgprintf("malloc size %d address %x \n", size, addr);

    //return malloc(size);
	return (void *)addr;
#endif    
}

void * osal_realloc(void* ptr, int size)
{
#ifndef EYENIX_SYSTEM_HEAP
	dbgprintf("not support realloc!\n");
	return 0;
#else
	int *addr;

	addr = realloc(ptr, size);
	if(addr == 0)
	{
		dbgprintf("realloc size %d address %x \n", size, addr);
		while(1);
	}
	//dbgprintf("realloc size %d address %x \n", size, addr);

	//return realloc(ptr, size);
	return (void *)addr;
#endif
}

void osal_free(void *p)
{
#ifndef EYENIX_SYSTEM_HEAP
	//dbgprintf("mem free! - address(0x%08X) \n", p);
	//OFFSET_ADDRESS -= ALIGN_16(size);
	//printf("address(0x%08X)\n", MALLOC_ADDRESS+OFFSET_ADDRESS);
	//if(OFFSET_ADDRESS < 0)
	//	OFFSET_ADDRESS = 0;
#else

	//dbgprintf("free address %x \n", p);
    free(p);//lint -e{424}
#endif    
}

int osal_fflush(osal_file_t fp)
{
    return 1;
}

int osal_feof(osal_file_t fp)
{
    return 0;
}

osal_file_t osal_fopen(const char * osal_file_tname, const char * mode)
{
	static fileio_buf_t *file;

	if(fileindex>=100)
	{
		dbgprintf("test file pointer end!!!!!!!!!!!!!!!!!!!!!!!!!!%d!!!!!!!!!!!\n",fileindex);
		return NULL;
	}

	if(strcmp(mode,"yuv")==0)
	{
		//dbgprintf("yuv buffer unused \n");
		//while(1);
#ifndef EYENIX_TEST_HEAP
		if(strcmp(osal_file_tname,"yuv/main")==0)
		{
			file = &s_file[fileindex++];
			file->_base = (char *)YUV_BUFFER_BASE;
			file->_bufsiz = FILE_BUFFER_1CH_SIZE;
			file->_ptr = (char *)0;
		}
		else if(strcmp(osal_file_tname,"yuv/sub")==0)
		{
			file = &s_file[fileindex++];
			file->_base = (char *)YUV_SUBBUFFER_BASE;
			file->_bufsiz = FILE_BUFFER_2CH_SIZE;
			file->_ptr = (char *)0;
		}
		else
		{
			file = &s_file[fileindex++];
			file->_base = (char *)0;//YUV_BUFFER_BASE;
			file->_bufsiz = 0;//FILE_BUFFER_SIZE;
			file->_ptr = (char *)0;
		}
#endif
	}
	else if(strcmp(mode,"bit")==0)
	{
		file = &s_file[fileindex++];
#ifdef BIT_FROM_BIN
		file->_base = (char *)test416_240_hevc_all_i;//BIT_BUFFER_BASE;
		file->_bufsiz = ALIGN_16(TEST416_240_HEVC_ALL_I_LEN);//FILE_BUFFER_SIZE;
		file->_ptr = (char *)0;
#else
		if(strcmp(osal_file_tname,"bit/main")==0)
		{
			file = &s_file[fileindex++];
			file->_base = (char *)DEC_BIT_BUFFER_BASE;
			file->_bufsiz = BIT_BUFFER_1CH_SIZE;
			file->_ptr = (char *)0;
		}
		else if(strcmp(osal_file_tname,"bit/sub")==0)
		{
			file = &s_file[fileindex++];
			file->_base = (char *)DEC_BIT_SUB_BUFFER_BASE;
			file->_bufsiz = BIT_BUFFER_2CH_SIZE;
			file->_ptr = (char *)0;
		}
		else
		{
			file->_base = (char *)DEC_BIT_BUFFER_BASE;
			file->_bufsiz = BIT_BUFFER_1CH_SIZE;
			file->_ptr = (char *)0;
		}
#endif
	}
	else if(strcmp(mode,"firm")==0)
	{
#ifdef CODECBIN_FROM_BIN
		file = &s_file[fileindex++];
		file->_base = (char *)bit_code_h;
		//dbgprintf(" !!!!!!!!!!!!!! CODECBIN_FROM_BIN %x \n", file->_base);
		file->_bufsiz = HEVC_CODE_LITTLE_LEN;
		file->_ptr = (char *)0;
#else
		file = &s_file[fileindex++];
		file->_base = (char *)0;
		file->_bufsiz = 0;
		file->_ptr = (char *)0;
#endif
	}
	else if(strcmp(mode,"reader")==0)
	{
		file->_base = (char *)BIT_READER_BASE;
		file->_bufsiz = 0;
		file->_ptr = (char *)0;
	}
	else if(strcmp(mode,"render")==0)
	{
		dbgprintf("please develop render memory\n");
		while(1);
#if 0
		file->_base = (char *)BIT_RENDER_BASE;
		file->_bufsiz = 0;
		file->_ptr = (char *)0;
#endif
	}
	else
	{
		file = &s_file[fileindex++];
		file->_base = (char *)0;
		file->_bufsiz = 0;
		file->_ptr = (char *)0;
	}
    return file;
}

#if 0
osal_file_t osal_fopen_yuv(const int index, const char * mode, int memsize)
{
	static fileio_buf_t *file;

	if(fileindex>=100)
	{
		dbgprintf("test file pointer end!!!!!!!!!!!!!!!!!!!!!!!!!!%d!!!!!!!!!!!\n",fileindex);
		return NULL;
	}

	if(strcmp(mode,"yuv")==0)
	{
		file = &s_file[fileindex++];
		file->_base = (char *)YUV_BUFFER_BASE + (memsize*index);
		file->_bufsiz = memsize;
		file->_ptr = (char *)0;
	}
	else
	{
		return 0;
	}
	if(file->_base > YUV_SUBBUFFER_BASE)
	{
		file->_base = (char *)0;
		file->_bufsiz = 0;
		file->_ptr = (char *)0;
		return 0;
	}
    return file;
}
#endif

size_t osal_fwrite(const void * p, int size, int count, osal_file_t s_file)
{
	unsigned int addr = (unsigned int)((unsigned int)s_file->_base+(unsigned int)s_file->_ptr);
    osal_memcpy((unsigned int *)addr, (unsigned int *)p, count*size);
    s_file->_ptr += count*size;

    return count*size;
}
size_t osal_fread(void *p, int size, int count, osal_file_t s_file)
{
	unsigned int addr = (unsigned int)((unsigned int)s_file->_base+(unsigned int)s_file->_ptr);
    osal_memcpy((unsigned int *)p, (unsigned int *)addr, count*size);
    s_file->_ptr += count*size;

#ifdef DEC_LOOP_TEST
    if(s_file->_ptr > 0x5C0000)
    {
    	dbgprintf("DEC_LOOP_TEST DEC Bitstream buffer area Over! reset first !\n");
    	s_file->_ptr = 0;
    }
#endif
    //if(s_file->_bufsiz < count*size)
    //	return s_file->_bufsiz;
    return count*size;
}

long osal_ftell(osal_file_t s_file)
{
    return s_file->_bufsiz;
}

int osal_fseek(osal_file_t s_file, long offset, int origin)
{

    return offset;
}
int osal_fclose(osal_file_t s_file)
{
    s_file->_base = 0;//(char *)FILE_BUFFER_BASE;
    s_file->_bufsiz = 0;//FILE_BUFFER_SIZE;
    s_file->_ptr = (char *)0;

    return 1;
}
int osal_fscanf(osal_file_t s_file, const char * _Format, ...)
{
    return 1;
}

int osal_fprintf(osal_file_t s_file, const char * _Format, ...)
{
    va_list ptr;
    char logBuf[MAX_PRINT_LENGTH] = {0};

    va_start( ptr, _Format);	

    vsnprintf_(logBuf, MAX_PRINT_LENGTH, _Format, ptr); //ksj porting for EN675

    va_end(ptr);

    dbgprintf(logBuf);

    return 1;

}

int osal_kbhit(void)
{
    return 0;
}
int osal_getch(void)
{
    return -1;
}

int osal_flush_ch(void)
{
    return -1;
}

void osal_msleep(Uint32 millisecond)
{
	printf("osal_msleep  %dms \n",millisecond);
    Uint32 countDown = millisecond;
    WaitXms(countDown);
    //while (countDown > 0) countDown--;
    //VLOG(WARN, "<%s:%d> Please implemenet osal_msleep()\n", __FUNCTION__, __LINE__);
}

osal_thread_t osal_thread_create(void(*start_routine)(void*), void*arg)
{
    return NULL;
}

Int32 osal_thread_join(osal_thread_t thread, void** retval)
{
    return -1;
}

Int32 osal_thread_timedjoin(osal_thread_t thread, void** retval, Uint32 second)
{
    return 2;   /* error */
}

osal_mutex_t osal_mutex_create(void)
{
    return NULL;
}

void osal_mutex_destroy(osal_mutex_t mutex)
{
}

BOOL osal_mutex_lock(osal_mutex_t mutex)
{
	dbgprintf("osal_mutex_lock !!!!!!!!!!!\n");
    return FALSE;
}

BOOL osal_mutex_unlock(osal_mutex_t mutex)
{
	dbgprintf("osal_mutex_unlock !!!!!!!!!!!\n");
    return FALSE;
}

osal_sem_t osal_sem_init(Uint32 count)
{
    VLOG(WARN, "<%s:%d> NEED TO IMPLEMENT %s\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return NULL;
}

void osal_sem_destroy(osal_sem_t sem)
{
    VLOG(WARN, "<%s:%d> NEED TO IMPLEMENT %s\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

void osal_sem_wait(osal_sem_t sem)
{
    VLOG(WARN, "<%s:%d> NEED TO IMPLEMENT %s\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

void osal_sem_post(osal_sem_t sem)
{
    VLOG(WARN, "<%s:%d> NEED TO IMPLEMENT %s\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

int _gettimeofday( struct timeval *tv, void *tzvp )
{
    Uint64 t = 0;//__your_system_time_function_here__();  // get uptime in nanoseconds
    tv->tv_sec = t / 1000000000;  // convert to seconds
    tv->tv_usec = ( t % 1000000000 ) / 1000;  // get remaining microseconds

    VLOG(WARN, "<%s:%d> NEED TO IMPLEMENT %s\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return 0;
}

Uint64 osal_gettime(void)
{
    VLOG(WARN, "<%s:%d> NEED TO IMPLEMENT %s\n", __FUNCTION__, __LINE__, __FUNCTION__);
    return 0ULL;
}


#ifdef LIB_C_STUB
/*
* newlib_stubs.c 
* the bellow code is just to build ref-code. customers will removed the bellow code bacuase they need a library which is related to the system library such as newlibc
*/
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>

#ifndef STDOUT_USART
#define STDOUT_USART 0
#endif

#ifndef STDERR_USART
#define STDERR_USART 0
#endif

#ifndef STDIN_USART
#define STDIN_USART 0
#endif

#undef errno
extern int errno;

/*
environ
A pointer to a list of environment variables and their values. 
For a minimal environment, this empty list is adequate:
*/
char *__env[1] = { 0 };
char **environ = __env;

int _write(int file, char *ptr, int len);

void _exit(int status) {
    _write(1, "exit", 4);
    while (1) {
        ;
    }
}

int _close(int file) {
    return -1;
}
/*
execve
Transfer control to a new process. Minimal implementation (for a system without processes):
*/
int _execve(char *name, char **argv, char **env) {
    errno = ENOMEM;
    return -1;
}
/*
fork
Create a new process. Minimal implementation (for a system without processes):
*/

int _fork() {
    errno = EAGAIN;
    return -1;
}
/*
fstat
Status of an open file. For consistency with other minimal implementations in these examples,
all files are regarded as character special devices.
The `sys/stat.h' header file required is distributed in the `include' subdirectory for this C library.
*/
int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/*
getpid
Process-ID; this is sometimes used to generate strings unlikely to conflict with other processes. Minimal implementation, for a system without processes:
*/

int _getpid() {
    return 1;
}

/*
isatty
Query whether output stream is a terminal. For consistency with the other minimal implementations,
*/
int _isatty(int file) {
    switch (file){
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        //errno = ENOTTY;
        errno = EBADF;
        return 0;
    }
}

/*
kill
Send a signal. Minimal implementation:
*/
int _kill(int pid, int sig) {
    errno = EINVAL;
    return (-1);
}

/*
link
Establish a new name for an existing file. Minimal implementation:
*/

int _link(char *old, char *new) {
    errno = EMLINK;
    return -1;
}

/*
lseek
Set position in a file. Minimal implementation:
*/
int _lseek(int file, int ptr, int dir) {
    return 0;
}

/*
sbrk
Increase program data space.
Malloc and related functions depend on this
*/
caddr_t _sbrk(int incr) {

    // extern char _ebss; // Defined by the linker
    char _ebss;
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_ebss;
    }
    prev_heap_end = heap_end;

    //char * stack = (char*) __get_MSP();
    char * stack = 0;
    if (heap_end + incr >  stack)//lint !e413
    {
        _write (STDERR_FILENO, "Heap and stack collision\n", 25);
        errno = ENOMEM;
        return  (caddr_t) -1;
        //abort ();
    }

    heap_end += incr;
    return (caddr_t) prev_heap_end;

}

/*
read
Read a character to a file. `libc' subroutines will use this system routine for input from all files, including stdin
Returns -1 on error or blocks until the number of characters have been read.
*/

int _read(int file, char *ptr, int len) {
    int n;
    int num = 0;
    switch (file) {
    case STDIN_FILENO:
        for (n = 0; n < len; n++) {
            char c =0;
#if   STDIN_USART == 1
            while ((USART1->SR & USART_FLAG_RXNE) == (Uint16)RESET) {}
            c = (char)(USART1->DR & (Uint16)0x01FF);
#elif STDIN_USART == 2
            while ((USART2->SR & USART_FLAG_RXNE) == (Uint16) RESET) {}
            c = (char) (USART2->DR & (Uint16) 0x01FF);
#elif STDIN_USART == 3
            while ((USART3->SR & USART_FLAG_RXNE) == (Uint16)RESET) {}
            c = (char)(USART3->DR & (Uint16)0x01FF);
#endif
            *ptr++ = c;
            num++;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return num;
}

/*
stat
Status of a file (by name). Minimal implementation:
int    _EXFUN(stat,( const char *__path, struct stat *__sbuf ));
*/

int _stat(const char *filepath, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/*
times
Timing information for current process. Minimal implementation:
*/

clock_t _times(struct tms *buf) {
    return -1;
}

/*
unlink
Remove a file's directory entry. Minimal implementation:
*/
int _unlink(char *name) {
    errno = ENOENT;
    return -1;
}

/*
wait
Wait for a child process. Minimal implementation:
*/
int _wait(int *status) {
    errno = ECHILD;
    return -1;
}

/*
write
Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
Returns -1 on error or number of bytes sent
*/
int _write(int file, char *ptr, int len) {
    int n;
    switch (file) {
    case STDOUT_FILENO: /*stdout*/
        for (n = 0; n < len; n++) {
#if STDOUT_USART == 1
            while ((USART1->SR & USART_FLAG_TC) == (Uint16)RESET) {}
            USART1->DR = (*ptr++ & (Uint16)0x01FF);
#elif  STDOUT_USART == 2
            while ((USART2->SR & USART_FLAG_TC) == (Uint16) RESET) {
            }
            USART2->DR = (*ptr++ & (Uint16) 0x01FF);
#elif  STDOUT_USART == 3
            while ((USART3->SR & USART_FLAG_TC) == (Uint16)RESET) {}
            USART3->DR = (*ptr++ & (Uint16)0x01FF);
#endif
        }
        break;
    case STDERR_FILENO: /* stderr */
        for (n = 0; n < len; n++) {
#if STDERR_USART == 1
            while ((USART1->SR & USART_FLAG_TC) == (Uint16)RESET) {}
            USART1->DR = (*ptr++ & (Uint16)0x01FF);
#elif  STDERR_USART == 2
            while ((USART2->SR & USART_FLAG_TC) == (Uint16) RESET) {
            }
            USART2->DR = (*ptr++ & (Uint16) 0x01FF);
#elif  STDERR_USART == 3
            while ((USART3->SR & USART_FLAG_TC) == (Uint16)RESET) {}
            USART3->DR = (*ptr++ & (Uint16)0x01FF);
#endif
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}

#endif
//#endif

