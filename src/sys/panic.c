#include <sys/panic.h>

extern void qemu_printf(const char* format, ...);
void panic(const char* msg) {
	asm volatile ("cli");
	qemu_printf("[KERNEL PANIC] %s\n", msg);
	asm volatile ("hlt");
}