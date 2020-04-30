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
struct page_table* kernel_pml4 = (struct page_table*)VIRTUAL_PHYS_BASE;
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

void map_page(struct page_table* pml4, size_t phys_addr, size_t virt_addr, size_t flags) {
    struct pt_entries offs = virtual_to_entries((void*)virt_addr);

    // qemu_printf("offs: pml4: %i pdp: %i pd: %i pt: %i\n", offs.pml4, offs.pdp, offs.pd, offs.pt);
    // struct page_table* pdp = get_or_alloc_ent(pml4, offs.pml4, flags);
    // qemu_printf("PDP allocated\n");
    // struct page_table* pd = get_or_alloc_ent(pdp, offs.pdp, flags);
    // qemu_printf("PD allocated\n");
    // if(pd[offs.pd] & 0x1) {

    // }

    // if(pd[offs.pd] & 0x1) {
    //     pt = (uint64_t*)((pd[offs.pd] & 0xfffffffffffff000) + VIRTUAL_PHYS_BASE);
    // } else {
    //     pt = (uint64_t*)((size_t)pmm_alloc(1) + VIRTUAL_PHYS_BASE);
    //     pd[offs.pd] = (uint64_t)((size_t)pt - VIRTUAL_PHYS_BASE) | 0b111;
    // }
    // struct page_table* pt = get_or_alloc_ent(pd, offs.pd, flags);
    // qemu_printf("PT allocated\n");
    // pt->entries[offs.pt] = (uint64_t)(phys_addr | flags);

    uint64_t *pdp, *pd, *pt;
    if(pml4->entries[offs.pml4] & 0x1) {
        pdp = (uint64_t*)((pml4->entries[offs.pml4] & 0xfffffffffffff000) + VIRTUAL_PHYS_BASE);
    } else {
        pdp = (uint64_t*)((size_t)pmm_alloc(1) + VIRTUAL_PHYS_BASE);

        if ((size_t)pdp == VIRTUAL_PHYS_BASE)
            qemu_printf("Failed PDP\n");

        pml4->entries[offs.pml4] = (uint64_t)((size_t)pdp - VIRTUAL_PHYS_BASE) | 0b111;
    }

    if(pdp[offs.pdp] & 0x1) {
        qemu_printf("PD present\n");
        pd = (uint64_t *)((pdp[offs.pdp] & 0xfffffffffffff000) + VIRTUAL_PHYS_BASE);
    } else {
        qemu_printf("PD not present\n");
        pd = (uint64_t*)((size_t)pmm_alloc(1) + VIRTUAL_PHYS_BASE);

        if ((size_t)pd == VIRTUAL_PHYS_BASE)
            qemu_printf("Failed PD\n");

        pdp[offs.pdp] = (uint64_t)((size_t)pd - VIRTUAL_PHYS_BASE) | 0b111;
    }

    qemu_printf("pd: 0x%x offs.pd: 0x%x\n", pd, offs.pd);
    if(pd[offs.pd] & 0x1) {
        pt = (uint64_t*)((pd[offs.pd] & 0xfffffffffffff000) + VIRTUAL_PHYS_BASE);
    } else {
        pt = (uint64_t*)((size_t)pmm_alloc(1) + VIRTUAL_PHYS_BASE);

        if ((size_t)pt == VIRTUAL_PHYS_BASE)
            qemu_printf("Failed PD\n");

        pd[offs.pd] = (uint64_t)((size_t)pt - VIRTUAL_PHYS_BASE) | 0b111;
    }

    pt[offs.pt] = (uint64_t)(phys_addr | flags);
    invlpg((void*)virt_addr);

}

void init_vmm(uint64_t mmap_addr, uint64_t mmap_length) {
	kernel_pml4 = (struct page_table*)pmm_alloc(1);
	memset((void*)(kernel_pml4), 0x0, PAGE_SIZE);

	multiboot_memory_map_t *mmap;
    for (mmap = (multiboot_memory_map_t *) mmap_addr;
        (unsigned long) mmap < mmap_addr + mmap_length;
        mmap = (multiboot_memory_map_t *)((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
        if(mmap->type == 1) {
    	   map_huge_pages(kernel_pml4, (void*)mmap->addr+VIRTUAL_PHYS_BASE, (void*)mmap->addr, mmap->len/PAGE_SIZE, VMM_PRESENT);
        }
    }

    write_cr("3", (size_t)kernel_pml4);
}