cmake_minimum_required(VERSION 3.16)
project(BartOS C CXX ASM_NASM)

if (CMAKE_C_COMPILER_FORCED)
	set(CMAKE_C_COMPILER_WORKS TRUE)
	return()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/Toolchain/CMakeLists.txt)

set(BUILD_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Build)

add_subdirectory(Kernel)

