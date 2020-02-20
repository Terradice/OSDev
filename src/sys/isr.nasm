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

global exc_div0_handler
global exc_debug_handler
global exc_nmi_handler
global exc_breakpoint_handler
global exc_overflow_handler
global exc_bound_range_handler
global exc_inv_opcode_handler
global exc_no_dev_handler
global exc_double_fault_handler
global exc_inv_tss_handler
global exc_no_segment_handler
global exc_ss_fault_handler
global exc_gpf_handler
global exc_page_fault_handler
global exc_x87_fp_handler
global exc_alignment_check_handler
global exc_machine_check_handler
global exc_simd_fp_handler
global exc_virt_handler
global exc_security_handler
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

section .text
exc_div0_handler:
    except_handler 0x0
exc_debug_handler:
    except_handler 0x1
exc_nmi_handler:
    except_handler 0x2
exc_breakpoint_handler:
    except_handler 0x3
exc_overflow_handler:
    except_handler 0x4
exc_bound_range_handler:
    except_handler 0x5
exc_inv_opcode_handler:
    except_handler 0x6
exc_no_dev_handler:
    except_handler 0x7
exc_double_fault_handler:
    except_handler_err_code 0x8
exc_inv_tss_handler:
    except_handler_err_code 0xa
exc_no_segment_handler:
    except_handler_err_code 0xb
exc_ss_fault_handler:
    except_handler_err_code 0xc
exc_gpf_handler:
    except_handler_err_code 0xd
exc_page_fault_handler:
    except_handler_err_code 0xe
exc_x87_fp_handler:
    except_handler 0x10
exc_alignment_check_handler:
    except_handler_err_code 0x11
exc_machine_check_handler:
    except_handler 0x12
exc_simd_fp_handler:
    except_handler 0x13
exc_virt_handler:
    except_handler 0x14
exc_security_handler:
    except_handler_err_code 0x1e

irq0:
	push byte 0
	push byte 32
	jmp irq_common_stub

irq1:
	push byte 0
	push byte 33
	jmp irq_common_stub

irq2:
	push byte 0
	push byte 34
	jmp irq_common_stub

irq3:
	push byte 0
	push byte 35
	jmp irq_common_stub

irq4:
	push byte 0
	push byte 36
	jmp irq_common_stub

irq5:
	push byte 0
	push byte 37
	jmp irq_common_stub

irq6:
	push byte 0
	push byte 38
	jmp irq_common_stub

irq7:
	push byte 0
	push byte 39
	jmp irq_common_stub


irq8:
	push byte 0
	push byte 40
	jmp irq_common_stub

irq9:
	push byte 0
	push byte 41
	jmp irq_common_stub

irq10:
	push byte 0
	push byte 42
	jmp irq_common_stub

irq11:
	push byte 0
	push byte 43
	jmp irq_common_stub

irq12:
	push byte 0
	push byte 44
	jmp irq_common_stub

irq13:
	push byte 0
	push byte 45
	jmp irq_common_stub

irq14:
	push byte 0
	push byte 46
	jmp irq_common_stub

irq15:
	push byte 0
	push byte 47
	jmp irq_common_stub

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

global __idt_default_handler
__idt_default_handler:
	pusham

	popam
	iretq

