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

#include <stdbool.h>
#include <sys/idt.h>
#include <sys/irq.h>
#include <sys/io.h>

extern void qemu_printf(const char* format, ...);
extern void print_frame(struct regs_t *frame);
int (*interrupt_handlers[256])(struct regs_t *r);

void irq_handler(struct regs_t *r) {
    #ifdef DEBUG
        if(r->int_no == IRQ0) {

        } else {
            // print_frame(r);
        }
    #endif

    if(interrupt_handlers[r->int_no]) {
        interrupt_handlers[r->int_no](r);
    }
    if (r->int_no >= 40) {
        outb(0xA0, 0x20);
    }
}

void register_irq_handler(uint8_t num, int (*handler)(struct regs_t *r)) {
    interrupt_handlers[num] = handler;
}

void irq_remap(void) {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}


void init_irq(void) {
    irq_remap();
    register_interrupt_handler(IRQ0, irq0, 0, 0x8e);
    register_interrupt_handler(IRQ1, irq1, 0, 0x8e);
    register_interrupt_handler(IRQ2, irq2, 0, 0x8e);
    register_interrupt_handler(IRQ3, irq3, 0, 0x8e);
    register_interrupt_handler(IRQ4, irq4, 0, 0x8e);
    register_interrupt_handler(IRQ5, irq5, 0, 0x8e);
    register_interrupt_handler(IRQ6, irq6, 0, 0x8e);
    register_interrupt_handler(IRQ7, irq7, 0, 0x8e);
    register_interrupt_handler(IRQ8, irq8, 0, 0x8e);
    register_interrupt_handler(IRQ9, irq9, 0, 0x8e);
    register_interrupt_handler(IRQ10, irq10, 0, 0x8e);
    register_interrupt_handler(IRQ11, irq11, 0, 0x8e);
    register_interrupt_handler(IRQ12, irq12, 0, 0x8e);
    register_interrupt_handler(IRQ13, irq13, 0, 0x8e);
    register_interrupt_handler(IRQ14, irq14, 0, 0x8e);
    register_interrupt_handler(IRQ15, irq15, 0, 0x8e);
    asm ("sti");
}