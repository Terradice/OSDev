#include <stdint.h>
#include <io.h>

inline uint8_t inb(uint32_t port) {
    uint8_t ret;
    asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
    return ret;
}

inline void outb(uint32_t port, uint8_t value) {
    asm volatile("outb %%al,%%dx": :"d" (port), "a" (value));
}