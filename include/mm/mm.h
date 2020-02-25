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

#ifndef __MM__H_
#define __MM__H_

#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 			0x1000
#define HUGE_PAGE_SIZE		0x200000

#define VIRTUAL_PHYS_BASE 	0xffff800000000000
#define KERNEL_PHYS_OFFSET 	0xffffffffc0000000
#define MEM_PHYS_OFFSET 	0xffff800000000000
#define MEMORY_BASE 		0x1000000

#define VMM_PRESENT 	1 << 0
#define VMM_WRITE 		1 << 1
#define VMM_USER 		1 << 2
#define VMM_WT 			1 << 3
#define VMM_NO_CACHE 	1 << 4
#define VMM_DIRTY		1 << 5
#define VMM_LARGE		1 << 7


#define write_cr(reg, val) ({ \
    asm volatile ("mov " reg ", %0" : : "r" (val)); \
})

static inline void invlpg(void* m) {
    asm volatile ( "invlpg (%0)" : : "b"(m) : "memory" );
}

struct page_table {
	uint64_t entries[512];
};

struct pt_entries {
	size_t pml4;
	size_t pdp;
	size_t pd;
	size_t pt;
};

typedef struct page_table pt_t;

struct page_table* kernel_pml4;
void init_vmm(uint64_t mmap_addr, uint64_t mmap_length);
void map_page(struct page_table* pml4, size_t phys_addr, size_t virt_addr, size_t flags);

void init_pmm(uint64_t total_memory, uint64_t mmap_addr, uint64_t mmap_length);
void * pmm_alloc(size_t count);
void pmm_free(void* ptr, size_t count);
#endif