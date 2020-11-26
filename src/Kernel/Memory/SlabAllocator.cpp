#include "SlabAllocator.h"

#include "Vmm.h"

#include <new>

namespace BartOS
{

namespace MM
{

template<size_t SLAB_SIZE>
SlabAllocator<SLAB_SIZE>::SlabAllocator(const size_t totalSize)
{
    
    m_mainSlabCache.Initialize(kmalloc(totalSize), totalSize);
    m_totalSlabsAvailable = m_mainSlabCache.m_nSlabsLeft;
    m_totalSize = totalSize;

    m_slabCacheList.push_back(&m_mainSlabCache);
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
SlabAllocator<SLAB_SIZE>::SlabAllocator() :
    m_totalSlabsAvailable(0),
    m_totalSize(0)
{
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
SlabAllocator<SLAB_SIZE>::SlabAllocator(const size_t totalSize, kmalloc_eternal_tag)
{
    Initialize(totalSize, kmalloc_eternal_tag());
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
void SlabAllocator<SLAB_SIZE>::Initialize(const size_t totalSize, kmalloc_eternal_tag)
{
    m_mainSlabCache.Initialize(kmalloc_eternal_aligned(totalSize, SLAB_SIZE), totalSize);
    m_totalSlabsAvailable = m_mainSlabCache.m_nSlabsLeft;
    m_totalSize = totalSize;

    m_slabCacheList.push_back(&m_mainSlabCache);
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
void *SlabAllocator<SLAB_SIZE>::Allocate()
{
    const size_t percentSlabsLeft = (GetTotalSlabsAvailable() - GetFreeSlabsLeft()) / GetTotalSlabsAvailable();
    if (percentSlabsLeft < 10)
    {
        //! TODO: Allocate new Slab Cache.        
    }
    
    void *pAlloc = nullptr;
    for (SlabCache *pSlabCache : m_slabCacheList)
    {
        if (0 < pSlabCache->m_nSlabsLeft)
        {
            pAlloc = pSlabCache->Allocate();
            break;
        }
    }

    return pAlloc;
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
void SlabAllocator<SLAB_SIZE>::Free(void *pBuffer)
{
    for (SlabCache *pSlabCache : m_slabCacheList)
    {
        if (pSlabCache->ContainsSlab(pBuffer))
        {
            pSlabCache->Free(pBuffer);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
bool SlabAllocator<SLAB_SIZE>::ContainsSlab(void *pBuffer)
{
    for (SlabCache *pSlabCache : m_slabCacheList)
    {
        if (pSlabCache->ContainsSlab(pBuffer))
            return true;
    }

    return false;
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
size_t SlabAllocator<SLAB_SIZE>::GetFreeSlabsLeft()
{
    size_t nSlabsLeft = 0;

    for (SlabCache *pSlabCache : m_slabCacheList)
        nSlabsLeft += pSlabCache->m_nSlabsLeft;

    return nSlabsLeft;
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
size_t SlabAllocator<SLAB_SIZE>::GetTotalSlabsAvailable()
{
    return m_totalSlabsAvailable;
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
SlabAllocator<SLAB_SIZE>::SlabCache::SlabCache() :
    m_pNextFreeSlab(nullptr),
    m_nSlabsLeft(0),
    m_totalSlabsAllocated(0),
    m_totalSlabsFreed(0),
    m_pSlabPool(nullptr),
    m_pPoolEnd(nullptr)
{
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
SlabAllocator<SLAB_SIZE>::SlabCache::SlabCache(void *pSlabPool, const size_t totalSize)
{
    Initialize(pSlabPool, totalSize);
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
void SlabAllocator<SLAB_SIZE>::SlabCache::Initialize(void *pSlabPool, const size_t totalSize)
{
    m_pNextFreeSlab = reinterpret_cast<SlabEntry *>(pSlabPool);
    m_nSlabsLeft = 0;
    m_totalSlabsAllocated = 0;
    m_totalSlabsFreed = 0;
    m_pSlabPool = reinterpret_cast<uint8_t *>(pSlabPool);
    m_pPoolEnd = m_pSlabPool + totalSize;

    memset((void *) m_pSlabPool, 0, totalSize);

    m_pNextFreeSlab = reinterpret_cast<SlabEntry *>(m_pSlabPool);

    m_nSlabsLeft = totalSize / SLAB_SIZE;
    uint8_t *pSlabEntry = m_pSlabPool;
    
    for (size_t i = 0; i < (m_nSlabsLeft - 1); ++i)
    {
        uint8_t * const pNextSlab = pSlabEntry + SLAB_SIZE;
        new (reinterpret_cast<SlabEntry *>(pSlabEntry)) SlabEntry(reinterpret_cast<SlabEntry *>(pNextSlab));
        pSlabEntry = pNextSlab;
    }

    //! Set the last slab in the free list to point to next.
    new (reinterpret_cast<SlabEntry *>(pSlabEntry)) SlabEntry(nullptr);
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
void *SlabAllocator<SLAB_SIZE>::SlabCache::Allocate()
{
    SlabEntry *pSlabEntry = m_pNextFreeSlab;
    ASSERT(SlabEntry::SLAB_MAGIC_NUMBER == pSlabEntry->m_magicNumber);

    m_pNextFreeSlab = pSlabEntry->m_pNext;
    --m_nSlabsLeft;
    ++m_totalSlabsAllocated;

    return reinterpret_cast<void *>(pSlabEntry);
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
void SlabAllocator<SLAB_SIZE>::SlabCache::Free(void *pBuffer)
{
    SlabEntry *pSlabEntry = reinterpret_cast<SlabEntry *>(pBuffer);
    
    ASSERT(SlabEntry::SLAB_MAGIC_NUMBER != pSlabEntry->m_magicNumber);

    pSlabEntry->m_pNext = m_pNextFreeSlab;
    m_pNextFreeSlab = pSlabEntry;
    ++m_nSlabsLeft;
    ++m_totalSlabsFreed;
}

// ---------------------------------------------------------------------------------------------------------

template<size_t SLAB_SIZE>
bool SlabAllocator<SLAB_SIZE>::SlabCache::ContainsSlab(void *pBuffer)
{
    const uint8_t *pSlab = reinterpret_cast<uint8_t *>(pBuffer);

    return (pSlab >= m_pSlabPool) && (pSlab < m_pPoolEnd);
}

template class SlabAllocator<16>;
template class SlabAllocator<32>;
template class SlabAllocator<64>;
template class SlabAllocator<128>;
template class SlabAllocator<256>;
template class SlabAllocator<512>;
template class SlabAllocator<1024>;
template class SlabAllocator<2048>;
template class SlabAllocator<4096>;

} // namespace MM

} // namespace BartOS
