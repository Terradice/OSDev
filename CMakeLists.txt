cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_C_COMPILER_WORKS 1)

enable_language(ASM_NASM)
project(TerraOS C ASM_NASM)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf64 -o <OBJECT> <SOURCE>")
set(CMAKE_C_COMPILER x86_64-elf-gcc)
set(CMAKE_ASM_COMPILER nasm)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_C_FLAGS "-std=gnu99 -ffreestanding -O3 -Wall -Wextra -mcmodel=kernel")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/linker.ld)
set(LINK_DEPENDS ${LINKER_SCRIPT})

file(GLOB C_SRCS src/*.c src/*/*.c)
file(GLOB S_SRCS src/*.nasm src/*/*.nasm)

add_executable(TerraOS ${C_SRCS} ${S_SRCS})

target_include_directories(TerraOS PRIVATE ${CMAKE_SOURCE_DIR}/include /usr/include)
target_link_options(TerraOS PRIVATE "SHELL:-T${LINKER_SCRIPT} -nostdlib -lgcc -z max-page-size=0x1000 -s")
