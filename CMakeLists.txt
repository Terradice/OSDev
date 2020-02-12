cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_C_COMPILER_WORKS 1)

enable_language(ASM_NASM)
project(TerraOS C ASM_NASM)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf64 -o <OBJECT> <SOURCE>")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_C_FLAGS "-std=gnu99 -ffreestanding -O2 -Wall -Wextra -mcmodel=kernel")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -lgcc")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(TerraOS src/kernel.c src/boot.asm
    src/drivers/exceptions.c src/drivers/idt.c
    src/drivers/io.c src/drivers/irq.c src/drivers/isr.asm
    src/mem/gdt.asm src/mem/paging.asm)

set_target_properties(TerraOS PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/src/linker.ld)
target_include_directories(TerraOS PRIVATE ${CMAKE_SOURCE_DIR}/include /usr/include)
