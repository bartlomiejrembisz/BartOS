#ifndef VIRTUAL_ADDRESS_H
#define VIRTUAL_ADDRESS_H

#include "Address.h"

#include "Lib/Core/BitFields.h"

namespace BartOS
{

class PhysAddr;

class [[gnu::packed]] VirtAddr : public Address<VirtAddr>
{
public:
    class Layout : public Bitmap<TypeSizeTraits<Address_t>::bitSize>
    {
    public:
        typedef BitField<Layout, 12> PageOffset;
        typedef BitField<PageOffset, 9> Level1;
        typedef BitField<Level1, 9> Level2;
        typedef BitField<Level2, 9> Level3;
        typedef BitField<Level3, 9> Level4;
    };

    //! Constructor
    VirtAddr();

    /*
     *  @brief Constructor
     * 
     *  @param address the address.
     */
    VirtAddr(const Address_t address);

    /*
     *  @brief Constructor
     *
     *  @param pObject the object pointer.
     */
    template <typename T>
    VirtAddr(const T *pObject);

    //! Setters
    Layout::Level4::ValueType SetLevel4(const Layout::Level4::ValueType level4);
    Layout::Level3::ValueType SetLevel3(const Layout::Level3::ValueType level3);
    Layout::Level2::ValueType SetLevel2(const Layout::Level2::ValueType level2);
    Layout::Level1::ValueType SetLevel1(const Layout::Level1::ValueType level1);
    Layout::PageOffset::ValueType SetPageOffset(const Layout::PageOffset::ValueType pageOffset);
    
    template<typename T>
    void SetLevel(const typename T::ValueType value);

    //! Getters
    Layout::Level4::ValueType GetLevel4() const;
    Layout::Level3::ValueType GetLevel3() const;
    Layout::Level2::ValueType GetLevel2() const;
    Layout::Level1::ValueType GetLevel1() const;
    Layout::PageOffset::ValueType GetPageOffset() const;

    template<typename T>
    typename T::ValueType GetLevel() const;

    /*
     *  @brief Create VirtAddr from PhysAddr.
     *         Offsets the address by KERNEL_VMA
     * 
     *  @param physAddr the physical address.
     */
    static VirtAddr FromPhys(const PhysAddr &physAddr);
};

// ---------------------------------------------------------------------------------------------------------

template <typename T>
VirtAddr::VirtAddr(const T *pObject) :
    Address(reinterpret_cast<Address_t >(pObject))
{
}

// ---------------------------------------------------------------------------------------------------------

template<typename T>
void VirtAddr::SetLevel(const typename T::ValueType value)
{
    T::Set(m_addr, value);
}

// ---------------------------------------------------------------------------------------------------------

template<typename T>
typename T::ValueType VirtAddr::GetLevel() const
{
    return static_cast<typename T::ValueType>(T::Get(m_addr));
}

} // namespace BartOS

#endif // VIRTUAL_ADDRESS_H
