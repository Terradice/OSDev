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

#ifndef __IRQ_H__
#define __IRQ_H__

#include <sys/idt.h>

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

extern void isr0(struct regs_t *);
extern void isr1(struct regs_t *);
extern void isr2(struct regs_t *);
extern void isr3(struct regs_t *);
extern void isr4(struct regs_t *);
extern void isr5(struct regs_t *);
extern void isr6(struct regs_t *);
extern void isr7(struct regs_t *);
extern void isr8(struct regs_t *);
extern void isr9(struct regs_t *);
extern void isr10(struct regs_t *);
extern void isr11(struct regs_t *);
extern void isr12(struct regs_t *);
extern void isr13(struct regs_t *);
extern void isr14(struct regs_t *);
extern void isr16(struct regs_t *);
extern void isr17(struct regs_t *);
extern void isr18(struct regs_t *);
extern void isr19(struct regs_t *);
extern void isr20(struct regs_t *);


extern void irq0(struct regs_t *);
extern void irq1(struct regs_t *);
extern void irq2(struct regs_t *);
extern void irq3(struct regs_t *);
extern void irq4(struct regs_t *);
extern void irq5(struct regs_t *);
extern void irq6(struct regs_t *);
extern void irq7(struct regs_t *);
extern void irq8(struct regs_t *);
extern void irq9(struct regs_t *);
extern void irq10(struct regs_t *);
extern void irq11(struct regs_t *);
extern void irq12(struct regs_t *);
extern void irq13(struct regs_t *);
extern void irq14(struct regs_t *);
extern void irq15(struct regs_t *);

void init_irq(void);
void interrupt_await(unsigned int num);
uint8_t interrupts[255];

#endif
