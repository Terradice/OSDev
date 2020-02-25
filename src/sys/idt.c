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

#include <stdint.h>
#include <stddef.h>
#include <sys/idt.h>
#include <sys/io.h>
#include <sys/irq.h>

struct idt_entry_t {
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t zero;
} __attribute((packed));

struct idt_ptr_t {
    uint16_t size;
    /* Start address */
    uint64_t address;
} __attribute((packed));

static struct idt_entry_t idt[256];

int register_interrupt_handler(size_t vec, void *handler, uint8_t ist, uint8_t type) {
    uint64_t p = (uint64_t)handler;

    idt[vec].offset_lo = (uint16_t)p;
    idt[vec].selector = 0x08;
    idt[vec].ist = ist;
    idt[vec].type_attr = type;
    idt[vec].offset_mid = (uint16_t)(p >> 16);
    idt[vec].offset_hi = (uint32_t)(p >> 32);
    idt[vec].zero = 0;

    return 0;
}


void init_idt(void) {
    for(uint16_t i = 0; i < 256; i++) {
		register_interrupt_handler(i, __idt_default_handler, 0, 0x8e);
	}

    register_interrupt_handler(ISR0, isr0, 0, 0x8e);
    register_interrupt_handler(ISR1, isr1, 0, 0x8e);
    register_interrupt_handler(ISR2, isr2, 0, 0x8e);
    register_interrupt_handler(ISR3, isr3, 0, 0x8e);
    register_interrupt_handler(ISR4, isr4, 0, 0x8e);
    register_interrupt_handler(ISR5, isr5, 0, 0x8e);
    register_interrupt_handler(ISR6, isr6, 0, 0x8e);
    register_interrupt_handler(ISR7, isr7, 0, 0x8e);
    register_interrupt_handler(ISR8, isr8, 0, 0x8e);
    register_interrupt_handler(ISR9, isr9, 0, 0x8e);
    register_interrupt_handler(ISR10, isr10, 0, 0x8e);
    register_interrupt_handler(ISR11, isr11, 0, 0x8e);
    register_interrupt_handler(ISR12, isr12, 0, 0x8e);
    register_interrupt_handler(ISR13, isr13, 0, 0x8e);
    register_interrupt_handler(ISR14, isr14, 0, 0x8e);
    register_interrupt_handler(ISR15, isr15, 0, 0x8e);
    register_interrupt_handler(ISR16, isr16, 0, 0x8e);
    register_interrupt_handler(ISR17, isr17, 0, 0x8e);
    register_interrupt_handler(ISR18, isr18, 0, 0x8e);
    register_interrupt_handler(ISR19, isr19, 0, 0x8e);
    register_interrupt_handler(ISR20, isr20, 0, 0x8e);
    register_interrupt_handler(ISR21, isr21, 0, 0x8e);
    register_interrupt_handler(ISR22, isr22, 0, 0x8e);
    register_interrupt_handler(ISR23, isr23, 0, 0x8e);
    register_interrupt_handler(ISR24, isr24, 0, 0x8e);
    register_interrupt_handler(ISR25, isr25, 0, 0x8e);
    register_interrupt_handler(ISR26, isr26, 0, 0x8e);
    register_interrupt_handler(ISR27, isr27, 0, 0x8e);
    register_interrupt_handler(ISR28, isr28, 0, 0x8e);
    register_interrupt_handler(ISR29, isr29, 0, 0x8e);
    register_interrupt_handler(ISR30, isr30, 0, 0x8e);
    register_interrupt_handler(ISR31, isr31, 0, 0x8e);

    struct idt_ptr_t idt_ptr = {
        sizeof(idt) - 1,
        (uint64_t)idt
    };

    asm volatile (
        "lidt %0"
        :
        : "m" (idt_ptr)
    );
}