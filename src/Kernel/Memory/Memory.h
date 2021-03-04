#ifndef MEMORY_H
#define MEMORY_H

#include "PhysicalAddress.h"
#include "VirtualAddress.h"

#include <new>

namespace BartOS
{

#define KiB (1024)
#define MiB (KiB * 1024)
#define GiB (MiB * 1024)

#define PAGE_SIZE (4 * KiB)
#define PAGE_SIZE_2M (2 * MiB)
#define PAGE_SIZE_1G (1 * GiB)

#define INITIAL_HEAP_SIZE (16 * KiB)
#define INITIAL_STACK_SIZE (16 * KiB)

#define ADDRESS_SPACE_TOP (0xFFFFFFFFFFFFFFFF)
#define TEMP_MAP_ADDR (0xFFFFFFFFFFE00000)

//! Paging typedefs.
using TableEntryIndex = uint16_t;
using PageTableIndex = uint8_t;

//! Page size enums.
enum PageSize
{
    PAGE_4K             = PAGE_SIZE,
    PAGE_2M             = PAGE_SIZE_2M,
    PAGE_1G             = PAGE_SIZE_1G,
    INVALID_PAGE_SIZE   = 0
};

//! The page flags.
enum PageFlags : uint16_t
{
    NO_FLAGS            = 0 << 0,
    PRESENT             = 1 << 0,
    WRITABLE            = 1 << 1,
    USER_ACCESSIBLE     = 1 << 2,
    WRITE_THROUGH       = 1 << 3,
    CACHE_DISABLED      = 1 << 4,
    HUGE_PAGE           = 1 << 5,
    GLOBAL              = 1 << 6,
    NO_EXECUTE          = 1 << 7,
    ALLOCATE_ON_DEMAND  = 1 << 8,
    SWAPPED_OUT         = 1 << 9
};

//! The page table levels.
enum PageMapLevel
{
    PML4,
    PML3,
    PML2,
    PML1,
    PAGE_LEVEL,
    NUMBER_OF_LEVELS
};

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

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

class Allocator
{
public:
    void *allocate(const size_t size);
    void free(void *ptr);
    void deallocate(void *ptr, unsigned long size);
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Initialize kmalloc eternal
 */
void init_kmalloc_eternal();

/*
 *  @brief kmalloc eternal
 * 
 *  @param size the size to allocate.
 * 
 *  @return the memory address of the allocation.
 */
void *kmalloc_eternal(const size_t size);

/*
 *  @brief kmalloc eternal aligned
 *
 *  @param size the size to allocate.
 *  @param alignment the alignment
 *
 *  @return the memory address of the allocation.
 */
void *kmalloc_eternal_aligned(const size_t size, size_t alignment);

/*
 *  @brief set kmalloc eternal end pointer
 * 
 *  @return the kmalloc eternal end.
 */
void set_kmalloc_eternal_ptr(void *pKmallocEternal);

/*
 *  @brief Get kmalloc eternal begin pointer
 * 
 *  @return the kmalloc eternal begin.
 */
void *get_kmalloc_eternal_begin();

/*
 *  @brief Get kmalloc eternal end pointer
 * 
 *  @return the kmalloc eternal end.
 */
void *get_kmalloc_eternal_ptr();

/*
 *  @brief Get kmalloc eternal end pointer
 * 
 *  @return the kmalloc eternal end.
 */
void disable_kmalloc_eternal();

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! PageMapLevel to VirtualAddress::Layout::Level meta.
template <PageMapLevel LEVEL>
class PageLevelToAddrLevel
{
};

// ---------------------------------------------------------------------------------------------------------

template <>
class PageLevelToAddrLevel<PML4>
{
public:
    typedef VirtualAddress::Layout::Level4 Level;
};

// ---------------------------------------------------------------------------------------------------------

template <>
class PageLevelToAddrLevel<PML3>
{
public:
    typedef VirtualAddress::Layout::Level3 Level;
};

// ---------------------------------------------------------------------------------------------------------

template <>
class PageLevelToAddrLevel<PML2>
{
public:
    typedef VirtualAddress::Layout::Level2 Level;
};

// ---------------------------------------------------------------------------------------------------------

template <>
class PageLevelToAddrLevel<PML1>
{
public:
    typedef VirtualAddress::Layout::Level1 Level;
};

// ---------------------------------------------------------------------------------------------------------

template <>
class PageLevelToAddrLevel<PAGE_LEVEL>
{
public:
    typedef VirtualAddress::Layout::PageOffset Level;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! Get the lower page level.
template <PageMapLevel LEVEL>
class LowerPageLevel
{
public:
    static_assert(NUMBER_OF_LEVELS > LEVEL);
    static constexpr PageMapLevel m_level = static_cast<PageMapLevel>(LEVEL + 1);
};

} // namespace BartOS

//! New and delete can't be in a namespace.
void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *ptr);
void operator delete[](void *ptr);
void operator delete(void *ptr, size_t size);
void operator delete[](void *ptr, size_t size);

void *kmalloc(const size_t size);
void *kmalloc(const size_t size, const size_t alignment);
void kfree(void * const ptr);

#endif // MEMORY_H