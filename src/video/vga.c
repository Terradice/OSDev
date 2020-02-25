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

#include <video/vga.h>
#include <libc/string.h>
#include <sys/io.h>
#include <sys/panic.h>

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)  {
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)  {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t* offset_collumn;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void update_cursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(){
    for(size_t i = 0; i < VGA_HEIGHT; i++){
        for (size_t m = 0; m < VGA_WIDTH; m++){
            terminal_buffer[i * VGA_WIDTH + m] = terminal_buffer[(i + 1) * VGA_WIDTH + m];
        }
    }
}

void terminal_putchar(char c) {
	switch(c) {
		case '\n': {
			offset_collumn[terminal_row] = terminal_column;
			terminal_row++;
			if(terminal_row == VGA_HEIGHT) {
				terminal_scroll();
				terminal_row--;
			};
			terminal_column = 0;
			break;
		}

		case '\b': {
			if(terminal_column == 0) {
				if(terminal_row == 0) {
					break;
				}
				terminal_row--;
				terminal_column = offset_collumn[terminal_row];
			} else {
				terminal_column--;
			}

			terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
			break;
		}

		default: {
			terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
			if (++terminal_column == VGA_WIDTH) {
				terminal_putchar('\n');
				// terminal_row++;
				// terminal_column = offset_collumn[terminal_row];
				// if (terminal_row == VGA_HEIGHT)
					// terminal_row = 0;
			}
			break;
		}
	}
	update_cursor(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

