cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_C_COMPILER_WORKS 1)

enable_language(ASM_NASM)
project(TerraOS C ASM_NASM)
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf64 -o <OBJECT> <SOURCE>")
set(CMAKE_C_COMPILER x86_64-elf-gcc)
set(CMAKE_ASM_COMPILER nasm)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/linker.ld)
set(LINK_DEPENDS ${LINKER_SCRIPT})

string(TIMESTAMP BUILD_DATE '"%Y-%m-%d %H:%M"')
set(LIB_VERSION_STRING '"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}"')

file(GLOB C_SRCS src/*.c src/*/*.c)
file(GLOB S_SRCS src/*.nasm src/*/*.nasm)
add_executable(TerraOS ${C_SRCS} ${S_SRCS})
target_include_directories(TerraOS PRIVATE ${CMAKE_SOURCE_DIR}/include /usr/include)

set(CMAKE_C_FLAGS "-std=gnu99 -ffreestanding -O3 -Wall -Wextra -mcmodel=kernel -D BUILD_DATE=${BUILD_DATE} -D BUILD_VER=${LIB_VERSION_STRING}")
target_link_options(TerraOS PRIVATE "SHELL:-T${LINKER_SCRIPT} -nostdlib -lgcc -z max-page-size=0x1000")

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -D DEBUG")
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/Debug)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/Release)
	target_link_options(TerraOS PRIVATE "SHELL:-T${LINKER_SCRIPT} -nostdlib -lgcc -z max-page-size=0x1000 -s")
endif()