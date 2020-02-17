#include <mm/mm.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t *bitmap;
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

extern void qemu_printf(const char* format, ...);

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

void init_pmm(uint64_t total_memory) {
	bitmap_entries = total_memory / PAGE_SIZE;
    memset(bitmap, 0, bitmap_entries);
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
            qemu_printf("address: 0x%x size: %i\n", first, found);
        } else {
            first = 0;
            found = 0;
            continue;
        }
    }

    return NULL;

    alloc:;

    for (uint64_t i = first; i < count+1; i++) {
        setAbsoluteBitState(bitmap, i);
    }

    qemu_printf("Allocated %i pages at 0x%x\n", count, first);
    qemu_printf("Bitmap: 0x%x\n", getAbsoluteBitState(bitmap, first));

    return (void*)(first * PAGE_SIZE);
}


void pmm_free(void * addr, size_t count) {

}