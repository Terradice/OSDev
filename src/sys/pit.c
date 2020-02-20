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

#include <sys/io.h>
#include <sys/pit.h>
#include <stdint.h>

static inline void __pit_send_cmd(uint8_t cmd) {
	outb(PIT_REG_COMMAND, cmd);
}

static inline void __pit_send_data(uint16_t data, uint8_t counter) {
	uint8_t	port = (counter==PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 :
		((counter==PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);

	outb (port, (uint8_t)data);
}

void pit_start_counter (uint32_t freq, uint8_t counter, uint8_t mode) {
	if (freq==0)
		return;
	// printf("Starting counter %d with frequency %dHz\n", counter/0x40, freq);
	uint16_t divisor = (uint16_t)( 1193181 / (uint16_t)freq);

	// send operational command words
	uint8_t ocw = 0;
	ocw = (ocw & ~PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
	ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | counter;
	__pit_send_cmd (ocw);

	// set frequency rate
	__pit_send_data (divisor & 0xff, 0);
	__pit_send_data ((divisor >> 8) & 0xff, 0);
}
