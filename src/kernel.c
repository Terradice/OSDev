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
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

#include <sys/idt.h>
#include <sys/irq.h>
#include <libc/stdio.h>
#include <video/vga.h>
#include <sys/shell.h>
#include <mm/mm.h>
#include <sys/io.h>
#include <multiboot.h>

#define breakpoint() \
	asm volatile("xchg %bx, %bx");

const uint8_t lower_ascii_codes[256] = {
    0x00,  'ESC',  '1',  '2',     /* 0x00 */
     '3',  '4',  '5',  '6',     /* 0x04 */
     '7',  '8',  '9',  '0',     /* 0x08 */
     '-',  '=',   '\b', '\t',     /* 0x0C */
     'q',  'w',  'e',  'r',     /* 0x10 */
     't',  'y',  'u',  'i',     /* 0x14 */
     'o',  'p',  '[',  ']',     /* 0x18 */
    '\n', 0x00,  'a',  's',     /* 0x1C */
     'd',  'f',  'g',  'h',     /* 0x20 */
     'j',  'k',  'l',  ';',     /* 0x24 */
    '\'',  '`', 0x00, '\\',     /* 0x28 */
     'z',  'x',  'c',  'v',     /* 0x2C */
     'b',  'n',  'm',  ',',     /* 0x30 */
     '.',  '/', 0x00,  '*',     /* 0x34 */
    0x00,  ' ', 0x00, 0x00,     /* 0x38 */
    0x00, 0x00, 0x00, 0x00,     /* 0x3C */
    0x00, 0x00, 0x00, 0x00,     /* 0x40 */
    0x00, 0x00, 0x00,  '7',     /* 0x44 */
     '8',  '9',  '-',  '4',     /* 0x48 */
     '5',  '6',  '+',  '1',     /* 0x4C */
     '2',  '3',  '0',  '.',     /* 0x50 */
    0x00, 0x00, 0x00, 0x00,     /* 0x54 */
    0x00, 0x00, 0x00, 0x00      /* 0x58 */
};

#define WIDTH 1280
#define HEIGHT 720

void qemu_putchar(char c) {
	outb(0x3F8, c);
}

void terminal_printf(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vprintf(terminal_putchar, format, arg);
	va_end(arg);
}

void qemu_printf(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vprintf(qemu_putchar, format, arg);
	va_end(arg);
}

extern uint64_t KERNEL_END;
extern uint64_t KERNEL_VIRTUAL_BASE;

multiboot_info_t mib;

uint32_t make_vesa_color(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t red = ((uint32_t)r) << mib.framebuffer_red_field_position;
    uint32_t green = ((uint32_t)g) << mib.framebuffer_green_field_position;
    uint32_t blue = ((uint32_t)b) << mib.framebuffer_blue_field_position;
    return red | green | blue;
}

void draw_pixel_at(int x, int y, uint32_t color) {
    uint32_t *row = ((unsigned char *)mib.framebuffer_addr) + (y * mib.framebuffer_pitch);
    row[x] = color;
}

void play_freq(uint32_t freq) {
	if(freq == 0) return;
	uint32_t div;
	uint8_t tmp;

	div = 1193180 / freq;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t) (div));
	outb(0x42, (uint8_t) (div >> 8));

	tmp = inb(0x61);
	if(tmp != (tmp | 3)) {
		outb(0x61, tmp | 3);
	}
}

void nosound() {
	uint8_t tmp = inb(0x61) & 0xFC;

	outb(0x61, tmp);
}

void kernel_main(multiboot_info_t* mb)  {
	mib = *mb;
	init_idt();
	init_irq();

	uint64_t all_mem = (mb->mem_lower+mb->mem_upper)*1024;
	init_pmm(all_mem, mb->mmap_addr, mb->mmap_length);
	// init_vmm();

	outb(0x64, 0xFF);

	/* Initialize terminal interface */
	terminal_initialize();
 	
 	#ifdef DEBUG
		terminal_printf("Debug mode enabled\n");
	#endif

 	#ifdef BUILD_VER
		terminal_printf("TerraOS Version %s built at %s\n\n", BUILD_VER, BUILD_DATE);
	#endif


	#ifdef DEBUG
		qemu_printf("Total memory: 0x%x\n", all_mem);
		qemu_printf("Kernel virtual base: 0x%x\n", KERNEL_VIRTUAL_BASE);
		qemu_printf("Kernel end: 0x%x\n", KERNEL_END);
		qemu_printf("Framebuffer address: 0x%x\n", mib.framebuffer_addr);
		qemu_printf("Framebuffer width: %i\n", mib.framebuffer_width);
		qemu_printf("Framebuffer height: %i\n", mib.framebuffer_height);
		qemu_printf("Framebuffer depth: %i\n", mib.framebuffer_pitch);

		void * addrone = pmm_alloc(8);
		terminal_printf("Allocating 1st address: 0x%x\n", addrone);
		terminal_printf("Freeing 1st address\n");
		pmm_free(addrone, 8);

		void * addrtwo = pmm_alloc(8);
		terminal_printf("Allocating 2nd address: 0x%x\n", addrtwo);

		void * addrthree = pmm_alloc(8);
		terminal_printf("Allocating 3rd address: 0x%x\n", addrthree);

		terminal_printf("Freeing 2nd address\n");
		pmm_free(addrtwo, 8);

		terminal_printf("Freeing 3rd address\n");
		pmm_free(addrthree, 8);

		terminal_printf("\n");
	#endif

	// *addr = "Hello World";
	// qemu_printf("Value: %s\n\n", *addr);
	// qemu_printf("Freeing address...\n");
	// pmm_free(addr, 8);
	// qemu_printf("Value after free: %s\n", *addr);
	// qemu_printf("Address after free: 0x%x\n\n", addr);
	// int *framebuffer = (int *) mb->framebuffer_addr;
	// uint32_t *row = ((unsigned char *)framebuffer) + (y * mb->framebuffer_pitch);
 //    row[x] = 0x7800;
	// for(int i = 0; i < mb->framebuffer_height*mb->framebuffer_width; i++) {
	// 	framebuffer[i] = 0x7800;
	// }

	// draw_pixel_at(0, 0, make_vesa_color(255, 255, 0));
	char * buff = pmm_alloc(1);
	int index = 0;
	terminal_printf("user@TerraOS# ");
	while(1) {
		interrupt_await(IRQ1);	
		uint8_t data = inb(0x60);
		// uint8_t command = inb(0x64);
		if(data & 0x80) {
			//Key Released
		} else {
			//Key Pressed
			qemu_printf("Key Pressed: 0x%x\n", data);
			if(data == 0x1C) {
				run_command(buff);
				terminal_printf("user@TerraOS# ");
				while(index > 0) buff[index--] = 0x0;
			} else if(data == '\b') {
				index--;
				buff[index] = 0x0;
				terminal_putchar('\b');
			} else if(lower_ascii_codes[data] == 0x00) {

			} else {
				buff[index] = lower_ascii_codes[data];
				index++;
				terminal_putchar(lower_ascii_codes[data]);
			}

			// if(data == 0x1) {
			// 	breakpoint();
			// } else {     
				// terminal_printf("Key pressed: 0x%x\n", data);
			// 	// terminal_printf("%s", lower_ascii_codes[data]);
			// 	terminal_putchar(lower_ascii_codes[data]);
			// }
		}
	}

	// while(1) asm ("hlt");
}