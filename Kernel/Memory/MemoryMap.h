#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include "PhysAddr.h"
#include "VirtAddr.h"

namespace BartOS
{

//! Kernel symbols exposed by the linker.
extern "C" Address_t KERNEL_VMA[];
extern "C" Address_t __kernel_virtual_start[];
extern "C" Address_t __kernel_physical_start[];
extern "C" Address_t __kernel_virtual_end[];
extern "C" Address_t __kernel_physical_end[];

//! Kernel sections exposed by the linker.
extern "C" Address_t __multiboot_start[];
extern "C" Address_t __multiboot_end[];
extern "C" Address_t __text_start[];
extern "C" Address_t __text_end[];
extern "C" Address_t __rodata_start[];
extern "C" Address_t __rodata_end[];
extern "C" Address_t __data_start[];
extern "C" Address_t __data_end[];
extern "C" Address_t __bss_start[];
extern "C" Address_t __bss_end[];

#define ADDRESS_SPACE_TOP (0xFFFFFFFFFFFFFFFF)
#define TEMP_MAP_ADDR (0xFFFFFFFFFFE00000)

}

#endif // MEMORY_MAP_H