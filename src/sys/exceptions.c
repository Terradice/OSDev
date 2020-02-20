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

#include <stddef.h>
#include <sys/exceptions.h>
#include <sys/idt.h>

extern void qemu_printf(const char* format, ...);
extern void terminal_printf(const char* format, ...);

void print_frame(struct system_frame *frame) {
	qemu_printf("CPU registers: RIP: 0x%x\n\tRSP: 0x%x, RAX: 0x%x, RBX: 0x%x, RCX: 0x%x, RDX: 0x%x\n\tRSI: 0x%x, RDI: 0x%x, RBP: 0x%x\n\tR8: 0x%x, R9: 0x%x, R10: 0x%x, R11: 0x%x\n\tR12: 0x%x, R13: 0x%x, R14: 0x%x\n\tR15:\n",
		frame->rip, frame->userrsp, frame->rax, frame->rbx, frame->rcx, frame->rdx, frame->rsi, frame->rdi, frame->userrsp, frame->rbp, frame->r8, frame->r9, frame->r10, frame->r11, frame->r12, frame->r13, frame->r14, frame->r15);
}

void exception_handler(int exception, struct system_frame *frame, size_t error_code) {
	switch(error_code) {
		case 0x0: {
			// qemu_printf("Division by zero!\n");
			//Divide by zero
			break;
		}

		case 0x1: {
			//Debug
			break;
		}

		case 0x2: {
			//NMI
			break;
		}

		case 0x3: {
			//Breakpoint
			break;
		}

		case 0x4: {
			//Overflow
			break;
		}

		case 0x5: {
			//Bound Range
			break;
		}

		case 0x6: {
			//Invalid Opcode
			break;
		}

		case 0x7: {
			//No Dev
			break;
		}

		case 0x8: {
			//Double fault
			break;
		}

		case 0xa: {
			//Invalid TSS
			break;
		}

		case 0xb: {
			//No Segment
			break;
		}

		case 0xc: {
			//SS Fault
			break;
		}

		case 0xd: {
			//General Protection Fault
			break;
		}

		case 0xe: {
			qemu_printf("Page fault\n");
			//Page Fault
			break;
		}

		case 0x10: {
			//X87 FP
		}

		case 0x11: {
			//Alignment Check
			break;
		}

		case 0x12: {
			//Machine Check
			break;
		}

		case 0x13: {
			//SIMD FP
			break;
		}

		case 0x14: {
			//Virt
			break;
		}

		case 0x1e: {
			//Security
			break;
		}

		default: {
			//Unknown
		}
	}

	asm volatile ("cli");
	qemu_printf("Exception %i\n", error_code);
	print_frame(frame);
	asm volatile ("hlt");
}