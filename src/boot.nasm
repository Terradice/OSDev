%if 0
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
%endif

bits 32
global _start, _long_mode_init, _loop_page_tables, end_of_kernel

%define kernel_phys_offset 0xffffffffc0000000
%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)
%define CODE_SEG     0x0008
%define DATA_SEG     0x0010
;%define VIDEO_ENABLED

extern gdt_ptr  
extern gdt_ptr_lowerhalf
extern kernel_main
extern paging_init
extern page_tables

MBALIGN		equ  1<<0             ; align loaded modules on page boundaries
MEMINFO		equ  1<<1             ; provide memory map
VIDINFO		equ  1<<2
;     this is the Multiboot 'flag' field
%ifdef VIDEO_ENABLED
	FLAGS		equ  MBALIGN | MEMINFO | VIDINFO
%else
	FLAGS		equ  MBALIGN | MEMINFO
%endif
MAGIC		equ  0x1BADB002       ; 'magic number' lets bootloader find the header
CHECKSUM	equ -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	%ifdef VIDEO_ENABLED
		dd 1280
		dd 720
		dd 32
	%endif
	
_start:
	mov esp, stack_top - kernel_phys_offset
	lgdt [gdt_ptr_lowerhalf]
	call paging_init

	mov edi, ebx
	jmp 0x08:_long_mode_init

	cli 
	hlt
	.end:
	
_long_mode_init:
	bits 64
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	add rsp, kernel_phys_offset
	lgdt [gdt_ptr]
	and rsp, -16        ; Align stack on 128 byte boundary

	call reload_CS

	mov rax, cr0
	and ax, 0xFFFB          ;clear coprocessor emulation CR0.EM
	or ax, 0x2                      ;set coprocessor monitoring  CR0.MP
	mov cr0, rax
	mov rax, cr4
	or ax, 3 << 9           ;set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
	mov cr4, rax



	mov rax, kernel_main
	call rax
	cli
	hlt

reload_CS:
   ; Reload data segment registers:
   mov   ax, 0x10 ; 0x10 points at the new data selector
   mov   ds, ax
   mov   es, ax
   mov   fs, ax
   mov   gs, ax
   mov   ss, ax
   ret

section .bss
stack_bottom:
	resb 32768
stack_top:

section .kend
end_of_kernel: