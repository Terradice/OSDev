#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

uint8_t inb(uint32_t port);
void outb(uint32_t port, uint8_t value);

#endif