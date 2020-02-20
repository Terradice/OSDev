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

#ifndef __PIT_H__
#define __PIT_H__

#define PIT_REG_COUNTER0 0x40
#define PIT_REG_COUNTER1 0x41
#define PIT_REG_COUNTER2 0x42
#define PIT_REG_COMMAND  0x43

#define PIT_OCW_MASK_BINCOUNT 1 //00000001
#define PIT_OCW_MASK_MODE 0xE //00001110
#define PIT_OCW_MASK_RL 0x30 //00110000
#define PIT_OCW_MASK_COUNTER 0xC0 //11000000

#define PIT_OCW_BINCOUNT_BINARY 0 //0
#define PIT_OCW_BINCOUNT_BCD 1 //1

#define PIT_OCW_MODE_TERMINALCOUNT 0 //0000
#define PIT_OCW_MODE_ONESHOT 0x2 //0010
#define PIT_OCW_MODE_RATEGEN 0x4 //0100
#define PIT_OCW_MODE_SQUAREWAVEGEN 0x6 //0110
#define PIT_OCW_MODE_SOFTWARETRIG 0x8 //1000
#define PIT_OCW_MODE_HARDWARETRIG 0xA //1010

#define PIT_OCW_RL_LATCH 0 //000000
#define PIT_OCW_RL_LSBONLY 0x10 //010000
#define PIT_OCW_RL_MSBONLY 0x20 //100000
#define PIT_OCW_RL_DATA 0x30 //110000

#define PIT_OCW_COUNTER_0 0 //00000000
#define PIT_OCW_COUNTER_1 0x40 //01000000
#define PIT_OCW_COUNTER_2 0x80 //10000000

void pit_start_counter (uint32_t freq, uint8_t counter, uint8_t mode);

#endif