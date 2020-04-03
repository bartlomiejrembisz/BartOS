cmake_minimum_required(VERSION 3.10)

project(BartOS)

# Setup C
set(CMAKE_C_COMPILER /home/bartek/opt/cross/bin/x86_64-elf-gcc)

# Setup using asm
enable_language(ASM)
set(CMAKE_ASM_COMPILER /usr/bin/nasm)
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <SOURCE> <FLAGS> -o <OBJECT>")

set(CMAKE_SOURCE_DIR ./src)

# Setup compilation flags
set(CMAKE_ASM_FLAGS "-fpic -ffreestanding $(DIRECTIVES) -lgcc -g -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_ASM_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_ASM_FLAGS} -std=c++17 -fno-exceptions -fno-rtti")
set(CMAKE_EXE_LINKER_FLAGS "-ffreestanding -nostdlib -T linker.ld")
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/*.cpp ${CMAKE_SOURCE_DIR}./*.c)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}./*.h)

include_directories(include/)
include_directories(src/)

add_executable(BartOS.bin ${SOURCE_FILES})