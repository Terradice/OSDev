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
#include <sys/idt.h>

extern void qemu_printf(const char* format, ...);
extern void terminal_printf(const char* format, ...);

void print_frame(struct regs_t *frame) {
	qemu_printf("int_no: 0x%x, err: 0x%x\n\tr15: 0x%x, r14: 0x%x, r13: 0x%x, r12: 0x%x, r11: 0x%x, r10: 0x%x, r9: 0x%x, r8: 0x%x\n\trdi: 0x%x, rsi: 0x%x, rbp: 0x%x, rdx: 0x%x, rcx: 0x%x, rbx: 0x%x, rax: 0x%x\n\trip: 0x%x, cs: 0x%x, rflags: 0x%x, rsp: 0x%x, ss: 0x%x\n",
		frame->int_no, frame->err, frame->r15, frame->r14, frame->r13, frame->r12, frame->r11, frame->r10, frame->r9, frame->r8, frame->rdi, frame->rsi, frame->rbp, frame->rdx, frame->rcx, frame->rbx, frame->rax, frame->rip, frame->cs, frame->rflags, frame->rsp, frame->ss);
}


void exception_handler(struct regs_t *frame) {
	switch(frame->int_no) {
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

	// asm volatile ("cli");
	qemu_printf("Exception %i\n", frame->int_no);
	print_frame(frame);
	outb(0x20, 0x20);
	// asm volatile ("hlt");
}