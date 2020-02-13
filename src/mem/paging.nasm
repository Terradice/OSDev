global paging_init, page_tables, enable_paging
%define kernel_phys_offset 0xffffffffc0000000
section .bss
align 4096
page_tables:
    .pml4:
        resb 4096
    .pdpt_low:
        resb 4096
    .pdpt_hi:
        resb 4096
    .pd:
        resb 4096
    .pt:
        resb 4096 * 16      ; 16 page tables == 32 MiB mapped
.end:

section .multiboot.text
bits 32

paging_init:

set_up_page_tables:
    xor eax, eax
    mov edi, page_tables - kernel_phys_offset
    mov ecx, (page_tables.end - page_tables) / 4
    rep stosd

    mov eax, 0x03
    mov edi, page_tables.pt - kernel_phys_offset
    mov ecx, 512 * 16

.loop0:
    stosd
    push eax
    xor eax, eax
    stosd
    pop eax
    add eax, 0x1000
    loop .loop0

    ; set up page directories
    mov eax, page_tables.pt - kernel_phys_offset
    or eax, 0x03
    mov edi, page_tables.pd - kernel_phys_offset
    mov ecx, 16

.loop1:
    stosd
    push eax
    xor eax, eax
    stosd
    pop eax
    add eax, 0x1000
    loop .loop1

    ; set up pdpt
    mov eax, page_tables.pd - kernel_phys_offset
    or eax, 0x03
    mov edi, page_tables.pdpt_low - kernel_phys_offset
    stosd
    xor eax, eax
    stosd

    mov eax, page_tables.pd - kernel_phys_offset
    or eax, 0x03
    mov edi, page_tables.pdpt_hi - kernel_phys_offset + 511*8
    stosd
    xor eax, eax
    stosd

    ; set up pml4
    mov eax, page_tables.pdpt_low - kernel_phys_offset
    or eax, 0x03
    mov edi, page_tables.pml4 - kernel_phys_offset
    stosd
    xor eax, eax
    stosd

    mov eax, page_tables.pdpt_low - kernel_phys_offset
    or eax, 0x03
    mov edi, page_tables.pml4 - kernel_phys_offset + 256*8
    stosd
    xor eax, eax
    stosd

    mov eax, page_tables.pdpt_hi - kernel_phys_offset
    or eax, 0x03
    mov edi, page_tables.pml4 - kernel_phys_offset + 511*8
    stosd
    xor eax, eax
    stosd
    
enable_paging:
    mov eax, page_tables - kernel_phys_offset
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    or eax, 1 << 7
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret