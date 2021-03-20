#ifndef PAGING_H
#define PAGING_H

#include "Arch/x86_64/Paging/PageTable.h"

namespace BartOS
{

namespace MM
{

//! Pml type traits.
template<PageMapLevel PML>
class PmlPageSize
{
};

// ---------------------------------------------------------------------------------------------------------

template<>
class PmlPageSize<PML4>
{
public:
    static constexpr PageSize value = INVALID_PAGE_SIZE;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class PmlPageSize<PML3>
{
public:
    static constexpr PageSize value = PAGE_1G;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class PmlPageSize<PML2>
{
public:
    static constexpr PageSize value = PAGE_2M;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class PmlPageSize<PML1>
{
public:
    static constexpr PageSize value = PAGE_4K;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

} // namespace MM

} // namespace BartOS

#endif // PAGING_H