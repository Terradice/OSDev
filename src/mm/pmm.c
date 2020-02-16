#include <mm/mm.h>
#include <stdint.h>
#include <stddef.h>

#define MEMORY_BASE 0x1000000
#define BITMAP_BASE (MEMORY_BASE / PAGE_SIZE)
#define test_bit(base, offset) ({ \
    int ret; \
    asm volatile ( \
        "bt %1, %2;" \
        : "=@ccc" (ret) \
        : "r" (base), "r" (offset) \
        : "memory" \
    ); \
    ret; \
})

#define set_bit(base, offset) ({ \
    int ret; \
    asm volatile ( \
        "bts %1, %2;" \
        : "=@ccc" (ret) \
        : "r" (base), "r" (offset) \
        : "memory" \
    ); \
    ret; \
})

#define reset_bit(base, offset) ({ \
    int ret; \
    asm volatile ( \
        "btr %1, %2;" \
        : "=@ccc" (ret) \
        : "r" (base), "r" (offset) \
        : "memory" \
    ); \
    ret; \
})

static volatile uint32_t *mem_bitmap;
static size_t bitmap_entries = 32;

__attribute__((always_inline)) static inline int read_bitmap(size_t i) {
    i -= BITMAP_BASE;

    return test_bit(mem_bitmap, i);
}

__attribute__((always_inline)) static inline void set_bitmap(size_t i, size_t count) {
    i -= BITMAP_BASE;

    size_t f = i + count;
    for (size_t j = i; j < f; j++)
        set_bit(mem_bitmap, j);
}

__attribute__((always_inline)) static inline void unset_bitmap(size_t i, size_t count) {
    i -= BITMAP_BASE;

    size_t f = i + count;
    for (size_t j = i; j < f; j++)
        reset_bit(mem_bitmap, j);
}

void init_pmm(uint64_t total_memory) {
	// memory_total = total_memory;
}

void * pmm_alloc(size_t count) {
	size_t to_find = count;
	size_t i;

	for(i = BITMAP_BASE; i < BITMAP_BASE+bitmap_entries;) {
		if(!read_bitmap(i++)) {
			if(!--to_find) {
				goto done;
			}
		} else {
			to_find = count;
		}
	}

    return;

	done:;
		size_t start = i - count;
		set_bitmap(start, count);
		return (void *)(start * PAGE_SIZE);
}

void pmm_free(void* ptr, size_t count) {
	size_t start = (size_t)ptr / PAGE_SIZE;

	unset_bitmap(start, count);
}