#ifndef __MM__H_
#define __MM__H_

#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define KERNEL_PHYS_OFFSET 0xffffffffc0000000
#define MEM_PHYS_OFFSET 0xffff800000000000

void init_pmm(uint64_t total_memory, uint64_t mmap_addr, uint64_t mmap_length);
void * pmm_alloc(size_t count);
void pmm_free(void* ptr, size_t count);
#endif