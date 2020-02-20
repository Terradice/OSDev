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

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

void exc_div0_handler(void);
void exc_debug_handler(void);
void exc_nmi_handler(void);
void exc_breakpoint_handler(void);
void exc_overflow_handler(void);
void exc_bound_range_handler(void);
void exc_inv_opcode_handler(void);
void exc_no_dev_handler(void);
void exc_double_fault_handler(void);
void exc_inv_tss_handler(void);
void exc_no_segment_handler(void);
void exc_ss_fault_handler(void);
void exc_gpf_handler(void);
void exc_page_fault_handler(void);
void exc_x87_fp_handler(void);
void exc_alignment_check_handler(void);
void exc_machine_check_handler(void);
void exc_simd_fp_handler(void);
void exc_virt_handler(void);
void exc_security_handler(void);

#endif