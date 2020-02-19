#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

void itoa (char *buf, int base, int d);
void vprintf(void (*write)(char), const char* format, va_list arg);
void printf(void (*write)(char), const char* format, ...);

#endif