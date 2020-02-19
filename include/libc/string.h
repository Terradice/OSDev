#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

size_t strlen(const char* str);
void strcat(char* dest, char* src, int pos);
int strcmp(const char *s1, const char *s2);

#endif