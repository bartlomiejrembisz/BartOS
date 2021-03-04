#include "KernelHeap.h"

namespace BartOS
{

namespace MM
{

KernelHeap::KernelHeap()
{
}

// ---------------------------------------------------------------------------------------------------------

void KernelHeap::Initialize()
{
    m_slab16.Initialize(128 * KiB, kernel_init_tag());
    m_slab32.Initialize(64 * KiB, kernel_init_tag());
    m_slab64.Initialize(32 * KiB, kernel_init_tag());
    m_slab128.Initialize(16 * KiB, kernel_init_tag());
    m_slab256.Initialize(8 * KiB, kernel_init_tag());
    m_slab512.Initialize(4 * KiB, kernel_init_tag());
    m_slab1024.Initialize(2 * KiB, kernel_init_tag());
    m_slab2048.Initialize(2 * KiB, kernel_init_tag());
    m_pageDirectoryAllocator.Initialize(INITIAL_HEAP_SIZE, kernel_init_tag());
}
// ---------------------------------------------------------------------------------------------------------

void *KernelHeap::Allocate(const size_t nBytes)
{
    ASSERT(nBytes > 0);

    void *pAlloc = nullptr;
    if (nBytes <= m_slab16.m_slabSize)
        pAlloc = m_slab16.Allocate();
    else if (nBytes <= m_slab32.m_slabSize)
        pAlloc = m_slab32.Allocate();
    else if (nBytes <= m_slab64.m_slabSize)
        pAlloc = m_slab64.Allocate();
    else if (nBytes <= m_slab128.m_slabSize)
        pAlloc = m_slab128.Allocate();
    else if (nBytes <= m_slab256.m_slabSize)
        pAlloc = m_slab256.Allocate();
    else if (nBytes <= m_slab512.m_slabSize)
        pAlloc = m_slab512.Allocate();
    else if (nBytes <= m_slab1024.m_slabSize)
        pAlloc = m_slab1024.Allocate();
    else if (nBytes <= m_slab2048.m_slabSize)
        pAlloc = m_slab2048.Allocate();

    ASSERT(pAlloc);
    return pAlloc;
}

// ---------------------------------------------------------------------------------------------------------

void *KernelHeap::Allocate(const size_t nBytes, const size_t alignment)
{
    ASSERT (nBytes > 0);

    void *pAlloc = nullptr;
    if (nBytes <= (m_slab16.m_slabSize) && (IS_ALIGNED(m_slab16.m_slabSize, alignment)))
        pAlloc = m_slab16.Allocate();
    else if ((nBytes <= m_slab32.m_slabSize) && (IS_ALIGNED(m_slab16.m_slabSize, alignment)))
        pAlloc = m_slab32.Allocate();
    else if ((nBytes <= m_slab64.m_slabSize) && (IS_ALIGNED(m_slab16.m_slabSize, alignment)))
        pAlloc = m_slab64.Allocate();
    else if ((nBytes <= m_slab128.m_slabSize) && (IS_ALIGNED(m_slab16.m_slabSize, alignment)))
        pAlloc = m_slab128.Allocate();
    else if ((nBytes <= m_slab256.m_slabSize) && (IS_ALIGNED(m_slab16.m_slabSize, alignment)))
        pAlloc = m_slab256.Allocate();
    else if ((nBytes <= m_slab512.m_slabSize) && (IS_ALIGNED(m_slab16.m_slabSize, alignment)))
        pAlloc = m_slab512.Allocate();
    else if ((nBytes <= m_slab1024.m_slabSize) && (IS_ALIGNED(m_slab16.m_slabSize, alignment)))
        pAlloc = m_slab1024.Allocate();
    else if ((nBytes <= m_slab2048.m_slabSize) && (IS_ALIGNED(m_slab16.m_slabSize, alignment)))
        pAlloc = m_slab2048.Allocate();

    ASSERT(pAlloc);
    return pAlloc;
}

// ---------------------------------------------------------------------------------------------------------

void KernelHeap::Free(void *pBuffer)
{
    if (!pBuffer)
        return;

    if (m_slab16.ContainsSlab(pBuffer))
        m_slab16.Free(pBuffer);
    else if (m_slab32.ContainsSlab(pBuffer))
        m_slab32.Free(pBuffer);
    else if (m_slab64.ContainsSlab(pBuffer))
        m_slab64.Free(pBuffer);
    else if (m_slab128.ContainsSlab(pBuffer))
        m_slab128.Free(pBuffer);
    else if (m_slab256.ContainsSlab(pBuffer))
        m_slab256.Free(pBuffer);
    else if (m_slab512.ContainsSlab(pBuffer))
        m_slab512.Free(pBuffer);
    else if (m_slab1024.ContainsSlab(pBuffer))
        m_slab1024.Free(pBuffer);
    else if (m_slab2048.ContainsSlab(pBuffer))
        m_slab2048.Free(pBuffer);
}

// ---------------------------------------------------------------------------------------------------------

x86_64::PageTable *KernelHeap::AllocatePml()
{
    return reinterpret_cast<x86_64::PageTable *>(m_pageDirectoryAllocator.Allocate());
}

// ---------------------------------------------------------------------------------------------------------

void KernelHeap::FreePml(x86_64::PageTable * const pPageTable)
{
    if (!pPageTable)
        return;

    m_pageDirectoryAllocator.Free(reinterpret_cast<void *>(pPageTable));
}

} // namespace MM

} // namespace BartOS