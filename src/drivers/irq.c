#include <stdbool.h>
#include <drivers/idt.h>
#include <drivers/irq.h>
#include <io.h>

void irq_handler(struct system_frame *r) {
    interrupts[r->int_no-32] = true;
    // asm volatile("xchg %bx, %bx");
    outb(0x20, 0x20);
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



void interrupt_await(unsigned int num) {
    while(!interrupts[num-32]) { asm("hlt"); }
    interrupts[num-32] = false;
}