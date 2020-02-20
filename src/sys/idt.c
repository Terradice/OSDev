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
#include <sys/exceptions.h>
#include <sys/io.h>

struct idt_entry_t {
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t zero;
};

struct idt_ptr_t {
    uint16_t size;
    /* Start address */
    uint64_t address;
} __attribute((packed));

static struct idt_entry_t idt[256];
extern void *int_thunks[];
extern void irq_handler(struct system_frame *r);

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

    register_interrupt_handler(0x0, exc_div0_handler, 0, 0x8e);
    register_interrupt_handler(0x1, exc_debug_handler, 0, 0x8e);
    register_interrupt_handler(0x2, exc_nmi_handler, 0, 0x8e);
    register_interrupt_handler(0x3, exc_breakpoint_handler, 0, 0x8e);
    register_interrupt_handler(0x4, exc_overflow_handler, 0, 0x8e);
    register_interrupt_handler(0x5, exc_bound_range_handler, 0, 0x8e);
    register_interrupt_handler(0x6, exc_inv_opcode_handler, 0, 0x8e);
    register_interrupt_handler(0x7, exc_no_dev_handler, 0, 0x8e);
    register_interrupt_handler(0x8, exc_double_fault_handler, 1, 0x8e);
    register_interrupt_handler(0xa, exc_inv_tss_handler, 0, 0x8e);
    register_interrupt_handler(0xb, exc_no_segment_handler, 0, 0x8e);
    register_interrupt_handler(0xc, exc_ss_fault_handler, 0, 0x8e);
    register_interrupt_handler(0xd, exc_gpf_handler, 0, 0x8e);
    register_interrupt_handler(0xe, exc_page_fault_handler, 0, 0x8e);
    register_interrupt_handler(0x10, exc_x87_fp_handler, 0, 0x8e);
    register_interrupt_handler(0x11, exc_alignment_check_handler, 0, 0x8e);
    register_interrupt_handler(0x12, exc_machine_check_handler, 0, 0x8e);
    register_interrupt_handler(0x13, exc_simd_fp_handler, 0, 0x8e);
    register_interrupt_handler(0x14, exc_virt_handler, 0, 0x8e);

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