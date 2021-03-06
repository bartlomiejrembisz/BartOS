#ifndef SLAB_ALLOCATOR_H
#define SLAB_ALLOCATOR_H

#include "Kernel/BartOS.h"

#include "Libraries/libc/string.h"

#include "frg/list.hpp"

namespace BartOS
{

namespace MM
{

struct kmalloc_eternal_tag {};

class KernelHeap;

//! A slab allocator object.
template<size_t SLAB_SIZE>
class SlabAllocator
{
public:
    static constexpr size_t m_slabSize = SLAB_SIZE;

    /*
     *  @brief Constructor
     * 
     *  @param totalSize the total size of the slab pool.
     */
    SlabAllocator(const size_t totalSize);

    /*
     *  @brief Allocate a slab.
     * 
     *  @return pointer to the slab.
     */
    void *Allocate();

    /*
     *  @brief Initialize the slab allocator.
     * 
     *  @param pBuffer address of slab to free.
     */
    void Free(void *pBuffer);

    /*
     *  @brief Does this allocator contains the slab.
     * 
     *  @param pBuffer address of slab.
     * 
     *  @return whether the cache contains the slab.
     */
    bool ContainsSlab(void *pBuffer);

    /*
     *  @brief Get the number of free slabs left.
     * 
     *  @return the total free slabs left.
     */
    size_t GetFreeSlabsLeft();

    /*
     *  @brief Get the total number of slabs.
     * 
     *  @return the total number of slabs.
     */
    size_t GetTotalSlabsAvailable();

private:
    /*
     *  @brief Constructor
     */
    SlabAllocator();

    /*
     *  @brief Constructor
     *  Only used by KernelHeap to initially construct the Slab Allocator.
     * 
     *  @param totalSize the total size of the slab pool.
     */
    SlabAllocator(const size_t totalSize, kmalloc_eternal_tag);
    
    /*
     *  @brief Initialize the slab allocator
     *  Only used by KernelHeap to initially construct the Slab Allocator.
     * 
     *  @param totalSize the total size of the slab pool.
     */
    void Initialize(const size_t totalSize, kmalloc_eternal_tag);

    struct SlabEntry
    {
        static constexpr uint32_t SLAB_MAGIC_NUMBER = 0xDEADBEEF;

        //! Constructor.
        SlabEntry(SlabEntry *pNext) :
            m_pNext(pNext),
            m_magicNumber(SLAB_MAGIC_NUMBER)
        {
        }

        SlabEntry   *m_pNext;
        uint32_t    m_magicNumber;
    };

    class SlabCache
    {
    public:
        /*
         *  @brief Initialize the slab allocator.
         */
        SlabCache();

        /*
         *  @brief Initialize the slab allocator.
         * 
         *  @param pSlabPool pointer to the slab pool.
         *  @param totalSize total size for allocation.
         */
        SlabCache(void *pSlabPool, const size_t totalSize);

        /*
         *  @brief Initialize the slab allocator
         * 
         *  @param pSlabPool pointer to the slab pool.
         *  @param totalSize the total size of the slab pool.
         */
        void Initialize(void *pSlabPool, const size_t totalSize);

        /*
         *  @brief Allocate a slab.
         * 
         *  @return pointer to the slab.
         */
        void *Allocate();

        /*
         *  @brief Initialize the slab allocator.
         * 
         *  @param pBuffer address of slab to free.
         */
        void Free(void *pBuffer);

        /*
         *  @brief Does this cache contain the slab.
         * 
         *  @param pBuffer address of slab.
         * 
         *  @return whether the cache contains the slab.
         */
        bool ContainsSlab(void *pBuffer);

        frg::default_list_hook<SlabCache> m_slabCacheHook;      ///< frg intrusive list interface.

        SlabEntry   *m_pNextFreeSlab;                           ///< Pointer to the next slab.
        size_t      m_nSlabsLeft;                               ///< Number of slabs left.
        size_t      m_totalSlabsAllocated;                      ///< Total slabs allocated.
        size_t      m_totalSlabsFreed;                          ///< Total slabs freed.

        uint8_t     *m_pSlabPool;                               ///< The base of the slab pool.
        uint8_t     *m_pPoolEnd;                                ///< The end of the slab pool.
    };
    
    //! The free list typedef.
    using SlabCacheList = 
    	frg::intrusive_list<
	    	SlabCache,
    		frg::locate_member<
			    SlabCache,
			    frg::default_list_hook<SlabCache>,
			    &SlabCache::m_slabCacheHook
		    >
	    >;

    SlabCache               m_mainSlabCache;        ///< The main slab cache.
    SlabCacheList           m_slabCacheList;        ///< The slab cache list for additional slab caches.
    size_t                  m_totalSlabsAvailable;  ///< The total number of slabs available.
    size_t                  m_totalSize;            ///< The total size of the slab cache.

    static_assert(sizeof(SlabEntry) <= SLAB_SIZE, "Slab size must be bigger than SLAB_SIZE");
    static_assert(IsPowerOfTwo<SLAB_SIZE>::value);

    friend class KernelHeap;
};

} // namespace MM

} // namespace BartOS

#endif // KERNEL_HEAP_H
