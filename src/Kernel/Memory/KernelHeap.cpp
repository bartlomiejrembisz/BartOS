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
    m_slab16.Initialize(4 * KiB, kmalloc_eternal_tag());
    m_slab32.Initialize(4 * KiB, kmalloc_eternal_tag());
    m_slab64.Initialize(4 * KiB, kmalloc_eternal_tag());
    m_slab128.Initialize(4 * KiB, kmalloc_eternal_tag());
    m_slab256.Initialize(4 * KiB, kmalloc_eternal_tag());
    m_slab512.Initialize(4 * KiB, kmalloc_eternal_tag());
    m_slab1024.Initialize(2 * KiB, kmalloc_eternal_tag());
    m_slab2048.Initialize(1 * KiB, kmalloc_eternal_tag());
    m_slab4096.Initialize(16 * KiB, kmalloc_eternal_tag());
}

// ---------------------------------------------------------------------------------------------------------

void *KernelHeap::Allocate(const size_t nBytes)
{
    if (nBytes <= m_slab16.m_slabSize)
        return m_slab16.Allocate();
    else if (nBytes <= m_slab32.m_slabSize)
        return m_slab32.Allocate();
    else if (nBytes <= m_slab64.m_slabSize)
        return m_slab64.Allocate();
    else if (nBytes <= m_slab128.m_slabSize)
        return m_slab128.Allocate();
    else if (nBytes <= m_slab256.m_slabSize)
        return m_slab256.Allocate();
    else if (nBytes <= m_slab512.m_slabSize)
        return m_slab512.Allocate();
    else if (nBytes <= m_slab1024.m_slabSize)
        return m_slab1024.Allocate();
    else if (nBytes <= m_slab2048.m_slabSize)
        return m_slab2048.Allocate();
    else if (nBytes <= m_slab4096.m_slabSize)
        return m_slab4096.Allocate();
}

// ---------------------------------------------------------------------------------------------------------

void KernelHeap::Free(void *pBuffer)
{
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
    else if (m_slab4096.ContainsSlab(pBuffer))
        m_slab4096.Free(pBuffer);

}

} // namespace MM

} // namespace BartOS