/*
    This file is a part of the TerraOS source code.
    Copyright (C) 2020  Terradice

    TerraOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <libc/string.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void strcat(char* dest, char* src, int pos) {
	int olen = pos || strlen(dest);
	int nlen = strlen(src);

	for(int i = olen; i < olen+nlen; i++) {
		dest[i] = src[i-olen];
	}
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 == *s2) {
        if (*s1 == '\0') {
            return 0;
        }

        ++s1;
        ++s2;
    }

    return *s1 - *s2;
}

void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t *p = (uint8_t *)src;
    uint8_t *q = (uint8_t *)dst;
    uint8_t *end = p + n;

    while (p != end) {
        *q++ = *p++;
    }

    return dst;
}

void memset(void* dest, int val, size_t len) {
  for (uint8_t* temp = dest; len--;)
    *temp++ = val;
}


void *memmove(void *dst, const void *src, size_t n) {
    uint8_t *p = (uint8_t *)src;
    uint8_t *q = (uint8_t *)dst;
    uint8_t *end = p + n;

    if (q > p && q < end) {
        p = end;
        q += n;

        while (p != src) {
            *--q = *--p;
        }
    }
    else
    {
        while (p != end)
        {
            *q++ = *p++;
        }
    }

    return dst;
}