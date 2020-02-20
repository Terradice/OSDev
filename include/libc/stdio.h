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

#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

void itoa (char *buf, int base, int d);
void vprintf(void (*write)(char), const char* format, va_list arg);
void printf(void (*write)(char), const char* format, ...);

#endif