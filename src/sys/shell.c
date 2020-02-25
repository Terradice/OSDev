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

#include <libc/stdio.h>
#include <libc/string.h>
#include <video/vga.h>
#include <sys/shell.h>
#include <sys/panic.h>
#include <sys/pit.h>
#include <sys/irq.h>
#include <stdint.h>

extern void terminal_printf(const char* format, ...);
extern void play_freq(uint32_t freq);
extern void nosound();

void sleep(size_t time) {
	for(size_t i = 0; i < time; i++) {
		pit_start_counter(1, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);
	}
}

void playsong() {
	// uint32_t song[256] = {293, 293, 440, 392, 369, 349, 293, 349, 392};
	// for(int i = 0; i < 9; i++) {
	// 	play_freq(song[i]);
	// 	sleep(10);
	// 	nosound();
	// }

	int pause = 20;

	play_freq(293); //D4
	sleep(pause/3);
	nosound();
	sleep(pause/3);

	play_freq(293); //D4
	sleep(pause/3);
	nosound();
	sleep(pause/3);

	play_freq(587); //D5
	sleep(pause);

	play_freq(440); //A4
	sleep(pause*2);

	play_freq(415); //G#4
	sleep(pause);

	play_freq(392); //G4
	sleep(pause);

	play_freq(349); //F4
	sleep(pause*2);

	play_freq(293); //D4
	sleep(pause);

	play_freq(349); //F4
	sleep(pause);

	play_freq(392); //G4
	sleep(pause);
	nosound();
}

void run_command(const char* cmd) {
	terminal_putchar('\n');
	if(!strcmp(cmd, "copyright")) {
		terminal_printf("TerraOS Copyright (C) 2020 Terradice\n");
		terminal_printf("This program comes with ABSOLUTELY NO WARRANTY; for details type 'show w'\n");
		terminal_printf("This is free software, and you are welcome to redistribute it\n");
		terminal_printf("under certain conditions; type `show c' for details.");

	} else if(!strcmp(cmd, "buildinfo")) {
		terminal_printf("TerraOS Version %s built at %s", BUILD_VER, BUILD_DATE);
	} else if(!strcmp(cmd, "help")) {
		terminal_printf("Commands:\ncopyright - Show copyright info\nbuildinfo - Show current build info\n");
		terminal_printf("help - Show this output");
	} else if(!strcmp(cmd, "panic")) {
		panic("Shell panic");
	} else if(!strcmp(cmd, "playsound")) {
		playsong();
		// volatile uint32_t i = 0;
		// while(i < 0x100000) {i++; ;}
	} else {
		terminal_printf("Unknown command");
	}
	terminal_putchar('\n');
}