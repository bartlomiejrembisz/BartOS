set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(ARCH ${CMAKE_SYSTEM_PROCESSOR})

set(TC_PATH ${CMAKE_SOURCE_DIR}/Toolchain/local/${ARCH})
set(CROSS_COMPILE ${ARCH}-elf)

link_directories(${CMAKE_SOURCE_DIR}/Toolchain/local/${ARCH}/lib/gcc/${CROSS_COMPILE}/10.2.0)

set(CMAKE_C_COMPILER		"${TC_PATH}/bin/${CROSS_COMPILE}-gcc")
set(CMAKE_CXX_COMPILER		"${TC_PATH}/bin/${CROSS_COMPILE}-gcc")
set(CMAKE_LINKER		"${TC_PATH}/bin/${CROSS_COMPILE}-ld")
set(CMAKE_AR			"${TC_PATH}/bin/${CROSS_COMPILE}-ar")
set(CMAKE_RANLIB 		"${TC_PATH}/bin/${CROSS_COMPILE}-ar")

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -F dwarf -g")
set(GCC_FLAGS "${GCC_FLAGS} -Wall -Wextra -fno-PIC -mcmodel=large -mno-red-zone")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS} -std=gnu++17 -fno-exceptions -fno-rtti")

#include_directories(${TC_PATH}/${CROSS_COMPILE}/include)
#include_directories(${TC_PATH}/${CROSS_COMPILE}/include/c++/10.2.0)
#include_directories(${TC_PATH}/${CROSS_COMPILE}/include/c++/10.2.0/x86_64-elf)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
