#ifndef MEMORY_H
#define MEMORY_H

#include "PhysicalAddress.h"
#include "VirtualAddress.h"

namespace BartOS
{

#define KiB (1024)
#define MiB (KiB * 1024)
#define GiB (MiB * 1024)

#define PAGE_SIZE (4 * KiB)
#define PAGE_SIZE_2M (2 * MiB)
#define PAGE_SIZE_1G (1 * GiB)

#define MIN_KERNEL_SIZE (1 * GiB)

//! Paging typedefs.
using TableEntryIndex = uint16_t;
using PageTableIndex = uint8_t;

//! Page size enums.
enum PageSize
{
    PAGE_4K = PAGE_SIZE,
    PAGE_2M = PAGE_SIZE_2M,
    PAGE_1G = PAGE_SIZE_1G,
    INVALID_PAGE_SIZE
};

//! The page flags.
enum PageFlags : uint8_t
{
    PRESENT         = 0x1,
    WRITABLE        = 0x2,
    USER_ACCESSIBLE = 0x4,
    WRITE_THROUGH   = 0x8,
    DISABLE_CACHE   = 0x10,
    HUGE_PAGE       = 0x20,
    GLOBAL          = 0x40,
    NO_EXECUTE      = 0x80
};

//! The page table levels.
enum PageTableLevel
{
    TABLE_LEVEL4,
    TABLE_LEVEL3,
    TABLE_LEVEL2,
    TABLE_LEVEL1,
    PAGE_LEVEL
};

//! Kernel symbols exposed by the linker.
extern "C" uintptr_t KERNEL_VMA[];
extern "C" uintptr_t __kernel_virtual_start[];
extern "C" uintptr_t __kernel_physical_start[];
extern "C" uintptr_t __kernel_virtual_end[];
extern "C" uintptr_t __kernel_physical_end[];

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
 *  @brief Get kmalloc eternal end pointer
 * 
 *  @return the kmalloc eternal end.
 */
void *get_kmalloc_eternal_ptr();

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! PageTableLevel to VirtualAddress::Layout::Level meta.
template <PageTableLevel LEVEL>
class PageLevelToAddrLevel
{
};

// ---------------------------------------------------------------------------------------------------------

template <>
class PageLevelToAddrLevel<TABLE_LEVEL4>
{
public:
    typedef VirtualAddress::Layout::Level4 Level;
};

// ---------------------------------------------------------------------------------------------------------

template <>
class PageLevelToAddrLevel<TABLE_LEVEL3>
{
public:
    typedef VirtualAddress::Layout::Level3 Level;
};

// ---------------------------------------------------------------------------------------------------------

template <>
class PageLevelToAddrLevel<TABLE_LEVEL2>
{
public:
    typedef VirtualAddress::Layout::Level2 Level;
};

// ---------------------------------------------------------------------------------------------------------

template <>
class PageLevelToAddrLevel<TABLE_LEVEL1>
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

} // namespace BartOS

#endif // MEMORY_H