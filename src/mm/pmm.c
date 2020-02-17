#include <mm/mm.h>
#include <sys/panic.h>
#include <multiboot.h>
#include <stdint.h>
#include <stddef.h>

#define MEMORY_BASE 0x1000000
#define DIV_ROUND_UP(x, d) (x + (d - 1)) / d
extern uint64_t KERNEL_END;
static uint64_t *bitmap = (uint64_t*)&KERNEL_END;
static int bitmap_entries;

void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes) {
  for (uint32_t i = 0; i < nbytes; i++) {
    *(dest + i) = *(source + i);
  }
}

void memset(void* dest, int val, size_t len) {
  for (uint8_t* temp = dest; len--;)
    *temp++ = val;
}


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

extern void qemu_printf(const char* format, ...);

void init_pmm(uint64_t total_memory, uint64_t mmap_addr, uint64_t mmap_length) {
    bitmap_entries = total_memory / PAGE_SIZE;

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

        int index = DIV_ROUND_UP(mmap->addr, PAGE_SIZE) / 8;
        int len = DIV_ROUND_UP(mmap->len, PAGE_SIZE);
        if(mmap->type == 1) {
            for(int i = index; i < index+len; i++) {
                unsetAbsoluteBitState(bitmap, i);
            }
        } else {
            for(int i = index; i < index+len; i++) {
                setAbsoluteBitState(bitmap, i);
            } 
        }
    }

    qemu_printf("Total bitmap entries: %i\n", bitmap_entries);
    // for(int i = 0; i < bitmap_entries; i++) {
    //     if(i*PAGE_SIZE*8 < MEMORY_BASE) {
    //         setAbsoluteBitState(bitmap, i);
    //     } else {
    //         unsetAbsoluteBitState(bitmap, i);
    //     }
    // }
}

void * pmm_alloc(size_t count) {
    uint64_t first = 0;
    uint64_t found = 0; 
    for(int i = 0; i < bitmap_entries; i++) {
        if(!getAbsoluteBitState(bitmap, i)) {
            if(!found) {
                first = i;
            };
            found++;
            if(found == count) {
                goto alloc;
            }
        } else {
            // qemu_printf("address: 0x%x size: %i\n", first, found);
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

    qemu_printf("Allocated %i pages at 0x%x\n", count, first);

    return (void*)(first * PAGE_SIZE);
}


void pmm_free(void * addr, size_t count) {
    int index = (int)addr / PAGE_SIZE;
    // qemu_printf("Index: %i\n", index);
    for(int i = index; i < count; i++) 
        unsetAbsoluteBitState(bitmap, i);
    // qemu_printf("Bitmap: 0x%x\n", getAbsoluteBitState(bitmap, index));
}