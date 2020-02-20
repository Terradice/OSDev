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
#include <sys/panic.h>
#include <libc/string.h>
#include <multiboot.h>
#include <stdint.h>
#include <stddef.h>

#define DIV_ROUND_UP(x, d) (x + (d - 1)) / d
static uint64_t *bitmap = (uint64_t*)MEMORY_BASE;
static int bitmap_entries;

uint8_t getAbsoluteBitState(uint64_t* map, uint64_t bit) {
  size_t off = bit / 64;
  size_t mask = (1 << (bit % 64));
  return (map[off] & mask) == mask;
}

void setAbsoluteBitState(uint64_t* map, uint64_t bit) {
  size_t off = bit / 64;
  size_t mask = (1 << (bit % 64));

  map[off] |= mask;
}

void unsetAbsoluteBitState(uint64_t* map, uint64_t bit) {
  size_t off = bit / 64;
  size_t mask = (1 << (bit % 64));

  map[off] &= ~mask;
}

void init_pmm(uint64_t total_memory, uint64_t mmap_addr, uint64_t mmap_length) {
    bitmap_entries = total_memory / PAGE_SIZE;
    memset(bitmap, 0xFF, bitmap_entries / 8);
    uint64_t avail_mem = 0;

    multiboot_memory_map_t *mmap;
    for (mmap = (multiboot_memory_map_t *) mmap_addr;
        (unsigned long) mmap < mmap_addr + mmap_length;
        mmap = (multiboot_memory_map_t *)((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
        // qemu_printf (" size = 0x%x, base_addr = 0x%x%x,"
        //         " length = 0x%x%x, type = 0x%x\n",
        //         (unsigned) mmap->size,
        //         (unsigned) (mmap->addr >> 32),
        //         (unsigned) (mmap->addr & 0xffffffff),
        //         (unsigned) (mmap->len >> 32),
        //         (unsigned) (mmap->len & 0xffffffff),
        //         (unsigned) mmap->type);

        if(mmap->type == 1) {
            avail_mem += mmap->len;
            pmm_free(mmap->addr+MEMORY_BASE, mmap->len/PAGE_SIZE);
        }
    }

    qemu_printf("Available memory: %iGB\n", avail_mem >> 30);

    int base = DIV_ROUND_UP(MEMORY_BASE, PAGE_SIZE);
    int end = DIV_ROUND_UP(MEMORY_BASE+bitmap_entries/8, PAGE_SIZE);

    for(int i = base; i < end; i++) {
        setAbsoluteBitState(bitmap, i);
    }
}

void * pmm_alloc(size_t count) {
    uint64_t first = 0;
    uint64_t found = 0; 
    for(int i = 0; i < bitmap_entries*64; i++) {
        if(!getAbsoluteBitState(bitmap, i)) {
            if(!found) {
                first = i;
            };
            found++;
            if(found == count) {
                goto alloc;
            }
        } else {
            first = 0;
            found = 0;
            continue;
        }
    }

    panic("OUT OF MEMORY");

    return NULL;

    alloc:;

    for (uint64_t i = first; i < count; i++) {
        setAbsoluteBitState(bitmap, i);
    }

    // qemu_printf("Allocated %i pages at 0x%x\n", count, first*PAGE_SIZE+MEMORY_BASE);

    return (void*)(first * PAGE_SIZE + MEMORY_BASE);
}


void pmm_free(void * addr, size_t count) {
    int index = DIV_ROUND_UP((uint64_t)addr-MEMORY_BASE, PAGE_SIZE);
    for(int i = index; i < count; i++) {
        // qemu_printf("Index: %i\n", i);
        unsetAbsoluteBitState(bitmap, i);
    }
}