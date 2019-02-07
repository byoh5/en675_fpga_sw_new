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
extern int _printf(const char *format, ...);

#endif /* __VSNPRINTF_H__ */
