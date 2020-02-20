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

#include <mm/mm.h>
#include <libc/string.h>
#include <multiboot.h>

#define DIV_ROUND_UP(x, d) (x + (d - 1)) / d

struct page_table* kernel_pml4;
size_t address_mask = ~(0xFFF | (1ull << 63));

extern void qemu_printf(const char* format, ...);

struct pt_entries virtual_to_entries(void* virt) {
	struct pt_entries ent = {
       	.pml4 = ((size_t)virt >> 39) & 0x1ff,
        .pdp = ((size_t)virt >> 30) & 0x1ff,
        .pd = ((size_t)virt >> 21) & 0x1ff,
        .pt = ((size_t)virt >> 12) & 0x1ff,
	};

	return ent;
}

struct page_table* get_or_alloc_ent(struct page_table* tab, size_t off, int flags) {
	uint64_t ent_addr = tab->entries[off] & address_mask;
	if(!ent_addr) {
		ent_addr = tab->entries[off] = (uint64_t)pmm_alloc(1);
		if(!ent_addr) return NULL;

		tab->entries[off] |= flags | VMM_PRESENT;
		memset((void*)(ent_addr + VIRTUAL_PHYS_BASE), 0, PAGE_SIZE);
	}

	return (struct page_table*)(ent_addr + VIRTUAL_PHYS_BASE);
}

void map_huge_pages(struct page_table* pml4, void* virt, void* phys, size_t count, int flags) {
	while(count--) {
		struct pt_entries offs = virtual_to_entries(virt);
		struct page_table* pml4_virt = (struct page_table*)((uint64_t)pml4+VIRTUAL_PHYS_BASE);
		struct page_table* pdp_virt = get_or_alloc_ent(pml4_virt, offs.pml4, flags);
		struct page_table* pd_virt = get_or_alloc_ent(pdp_virt, offs.pdp, flags);
		pd_virt->entries[offs.pd] = (uint64_t)phys | flags | VMM_LARGE;
		virt = (void*)((uintptr_t)virt+HUGE_PAGE_SIZE);
		phys = (void*)((uintptr_t)phys+HUGE_PAGE_SIZE);
	}
}

struct page_table* new_address_space() {
	struct page_table* new_pml4 = (struct page_table*)pmm_alloc(1);

	memset((void*)(new_pml4+VIRTUAL_PHYS_BASE), 0x0, PAGE_SIZE/8);
    // map_huge_pages(new, (void*)0xFFFFFFFF80000000, NULL, 64, 3);
    // map_huge_pages(new, (void*)0xFFFF800000000000, NULL, 512 * 4, 3);

	return new_pml4;
}

void init_vmm() {
	kernel_pml4 = new_address_space();
}

// void init_vmm(uint64_t mmap_addr, uint64_t mmap_length) {
// 	// kernel_directory = (struct page_directory*)pmm_alloc(DIV_ROUND_UP(sizeof(struct page_directory), PAGE_SIZE));
// 	// pml4 = (struct page_table*)pmm_alloc(DIV_ROUND_UP(sizeof(struct page_table), PAGE_SIZE));

// 	multiboot_memory_map_t *mmap;
//     for (mmap = (multiboot_memory_map_t *) mmap_addr;
//         (unsigned long) mmap < mmap_addr + mmap_length;
//         mmap = (multiboot_memory_map_t *)((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
//         if(mmap->type == 1) {
//         	for(int i = mmap->addr+MEMORY_BASE; i < mmap->len; i+= PAGE_SIZE * 1024) {
// 				qemu_printf("0x%x\n", i);
// 	        	// get_page(i, kernel_directory);
//         	}
//         }
//     }

// 	// kernel_directory = (struct page_directory*)pmm_alloc(DIV_ROUND_UP(sizeof(struct page_directory), PAGE_SIZE));

// 	// uint32_t i = 0;
// 	// for(i = 0; i < 0xFFFFFFFF;) {
// 	// 	get_page(i, kernel_directory);
// 	// 	i += PAGE_SIZE * 1024;
// 	// }

// 	qemu_printf("Finished VMM mapping\n");

// 	switch_page_directory(pml4);
// 	qemu_printf("Finished VMM initialization\n");
// }