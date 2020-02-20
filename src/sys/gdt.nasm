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

bits 64
global gdt_ptr, gdt_ptr_lowerhalf
%define kernel_phys_offset 0xffffffffc0000000
section .multiboot.data

align 16
gdt_ptr_lowerhalf:
    dw gdt_ptr.gdt_end - gdt_ptr.gdt_start - 1  ; GDT size
    dd gdt_ptr.gdt_start                        ; GDT start
    
align 16
gdt_ptr:
    dw .gdt_end - .gdt_start - 1                ; GDT size
    dq .gdt_start + kernel_phys_offset          ; GDT start

align 16
.gdt_start:
    .null_descriptor:
        dq 0x0000000000000000       


    .kernel_code_64:
        dq 0x00209A0000000000  

    .kernel_data:
        dq 0x0000920000000000           
.gdt_end: