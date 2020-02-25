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

%macro pusham 0
    cld
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro


%macro popam 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

extern exception_handler
%macro except_handler_err_code 1
    push qword [rsp+5*8]
    push qword [rsp+5*8]
    push qword [rsp+5*8]
    push qword [rsp+5*8]
    push qword [rsp+5*8]
    pusham
    mov rdi, %1
    mov rsi, rsp
    mov rdx, qword[rsp+20*8]

    mov rax, [rsp+16*8]
    cmp rax, 0x08
    je .nozerorbp
    xor rbp, rbp
  .nozerorbp:
    call exception_handler
    popam
    iretq
%endmacro

%macro except_handler 1
    pusham
    mov rdi, %1
    mov rsi, rsp
    xor rdx, rdx
    ; Only reset rbp to limit trace if coming from userland
    mov rax, [rsp+16*8]
    cmp rax, 0x08
    je .nozerorbp
    xor rbp, rbp
  .nozerorbp:
    call exception_handler
    popam
    iretq
%endmacro

section .text
%macro ISR_NOERR 1
    global isr%1
    isr%1:
        push byte 0
        push byte %1
        jmp isr_common
%endmacro

%macro ISR_ERR 1
    global isr%1
    isr%1:
        push byte %1
        jmp isr_common
%endmacro

%macro gen_irq_handler 1
    global irq%1
    irq%1:
        push byte 0
        push byte %1+32
        jmp isr_common
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR 8
ISR_NOERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_ERR 30
ISR_NOERR 31

%assign i 0
%rep  16
gen_irq_handler i
%assign i i + 1
%endrep

extern irq_handler
irq_common_stub:
	pusham
	mov rdi, rsp   ; RDI=1st parameter

	cld
	mov rax, irq_handler
	call rax

	popam
	add rsp, 16
	iretq

isr_common:
    pusham

    xor rax, rax
    mov ax, ds
    push rax

    mov ax, 0
    mov ds, ax
    mov es, ax

    mov rdi, rsp
    call irq_handler

    pop rax
    mov ds, ax
    mov es, ax

    popam
    add rsp, 16
    iretq

global __idt_default_handler
__idt_default_handler:
	pusham

	popam
	iretq

