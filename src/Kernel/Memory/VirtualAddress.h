#ifndef VIRTUAL_ADDRESS_H
#define VIRTUAL_ADDRESS_H

#include "Address.h"

#include "Libraries/Misc/BitFields.h"

namespace BartOS
{

class PhysicalAddress;

class [[gnu::packed]] VirtualAddress : public Address<VirtualAddress>
{
public:
    class Layout : public Bitmap<TypeSizeTraits<uintptr_t>::bitSize>
    {
    public:
        typedef BitField<Layout, 12> PageOffset;
        typedef BitField<PageOffset, 9> Level1;
        typedef BitField<Level1, 9> Level2;
        typedef BitField<Level2, 9> Level3;
        typedef BitField<Level3, 9> Level4;
    };

    //! Constructor
    VirtualAddress();

    /*
     *  @brief Constructor
     * 
     *  @param address the address.
     */
    explicit VirtualAddress(const uintptr_t address);

    //! Setters
    Layout::Level4::ValueType SetLevel4(const Layout::Level4::ValueType level4);
    Layout::Level3::ValueType SetLevel3(const Layout::Level3::ValueType level3);
    Layout::Level2::ValueType SetLevel2(const Layout::Level2::ValueType level2);
    Layout::Level1::ValueType SetLevel1(const Layout::Level1::ValueType level1);
    Layout::PageOffset::ValueType SetPageOffset(const Layout::PageOffset::ValueType pageOffset);

    //! Getters
    Layout::Level4::ValueType GetLevel4() const;
    Layout::Level3::ValueType GetLevel3() const;
    Layout::Level2::ValueType GetLevel2() const;
    Layout::Level1::ValueType GetLevel1() const;
    Layout::PageOffset::ValueType GetPageOffset() const;

    /*
     *  @brief Create VirtualAddress from PhysicalAddress.
     *         Offsets the address by KERNEL_VMA
     * 
     *  @param physicalAddress the physical address.
     */
    static VirtualAddress Create(const PhysicalAddress &physicalAddress);
};

} // namespace BartOS

#endif // VIRTUAL_ADDRESS_H
