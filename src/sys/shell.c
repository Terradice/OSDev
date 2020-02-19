#include <libc/stdio.h>
#include <libc/string.h>
#include <video/vga.h>

extern void terminal_printf(const char* format, ...);

void run_command(const char* cmd) {
	terminal_putchar('\n');
	if(!strcmp(cmd, "copyright")) {
		terminal_printf("TerraOS Copyright (C) 2020 Terradice\n");
		terminal_printf("This program comes with ABSOLUTELY NO WARRANTY; for details type 'show w'\n");
		terminal_printf("This is free software, and you are welcome to redistribute it\n")
		terminal_printf("under certain conditions; type `show c' for details.");

	} else if(!strcmp(cmd, "buildinfo")) {
		terminal_printf("TerraOS Version %s built at %s", BUILD_VER, BUILD_DATE);
	} else if(!strcmp(cmd, "help")) {
		terminal_printf("Commands:\ncopyright - Show copyright info\nbuildinfo - Show current build info\n");
		terminal_printf("help - Show this output");
	} else {
		terminal_printf("Unknown command");
	}
	terminal_putchar('\n');
}