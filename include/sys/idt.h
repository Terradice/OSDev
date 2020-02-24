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

#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>
#include <stddef.h>

struct regs_t {
    uint64_t ds;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8,
             rdi, rsi, rbp, rdx, rcx, rbx, rax;
    uint64_t int_no, err;
    uint64_t rip, cs, rflags;
    uint64_t rsp, ss;
};

void init_idt(void);
extern void __idt_default_handler();
int register_interrupt_handler(size_t vec, void *handler, uint8_t ist, uint8_t type);

#endif