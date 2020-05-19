#ifndef __VSNPRINTF_H__
#define __VSNPRINTF_H__

// vsnprintf format list
// %c           : Character
// %s           : String of characters
// %p           : Pointer address
// %d, %ld, %i  : Signed decimal integer
// %u, %lu      : Unsigned decimal integer
// %o           : Unsigned octal
// %x, %X       : Unsigned hexadecimal integer
// %n           : Nothing printed. The corresponding argument must be a pointer to a signed int. The number of characters written so far is stored in the pointed location.
// %%           : A % followed by another % character will write a single % to the stream
extern int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);			// vsnprintf.c
extern int _printf(const char *format, ...);// __attribute__ ((format (printf, 1, 2)));
extern int _printf_irq(const char *format, ...);
extern int _sprintf(char *buf, const char *format, ...);
extern int color_printf(char *color, const char *format, ...);

// Same as "printf/sprintf/snprintf" function.
// This function was created for a special format. ('%IP', '%b', etc...)
// With this function, the compiler does not warn you when using special format.
extern int es_printf(const char *format, ...);
extern int es_sprintf(char *buf, const char *format, ...);
extern int es_snprintf(char *buf, size_t size, const char *format, ...);

#define printf	_printf
#define sprintf	_sprintf

#ifdef __ECM_STRING__
#define _Zprintf printf
#define _Rprintf printf
#define _Gprintf printf
#define _Yprintf printf
#define _Bprintf printf
#define _Mprintf printf
#define _Cprintf printf
#define _Wprintf printf
#else
#define _Zprintf(format, args...) color_printf(TTY_COLOR_BLACK, format, ## args)
#define _Rprintf(format, args...) color_printf(TTY_COLOR_RED, format, ## args)
#define _Gprintf(format, args...) color_printf(TTY_COLOR_GREEN, format, ## args)
#define _Yprintf(format, args...) color_printf(TTY_COLOR_YELLOW, format, ## args)
#define _Bprintf(format, args...) color_printf(TTY_COLOR_BLUE, format, ## args)
#define _Mprintf(format, args...) color_printf(TTY_COLOR_MAGEN, format, ## args)
#define _Cprintf(format, args...) color_printf(TTY_COLOR_CYAN, format, ## args)
#define _Wprintf(format, args...) color_printf(TTY_COLOR_WHITE, format, ## args)
#endif

#endif /* __VSNPRINTF_H__ */
