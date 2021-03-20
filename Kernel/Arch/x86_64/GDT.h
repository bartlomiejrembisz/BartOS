#ifndef GDT_H
#define GDT_H

#include "BartOS.h"

#include "Lib/Core/BitFields.h"
#include "Lib/Core/Singleton.h"

namespace BartOS
{

namespace x86_64
{

class GlobalDescriptorTable : public Singleton<GlobalDescriptorTable>
{
public:
    using SelectorOffset = uint8_t;

    static constexpr uint8_t GDT_ENTRY_SIZE = 8;
    
    //! Selector enum
    enum Selector
    {
        NULL_SEL,
        KERNEL_CODE,
        KERNEL_DATA,
        USER_CODE,
        USER_DATA,
        GDT_ENTRY_COUNT
    };

    //! Initialize the Global Descriptor Table.
    void Initialize();

    /*
     *  @brief Set the code segment.
     *
     *  @param selector the GDT selector.
     */
    void SetCodeSegment(const Selector selector);

    /*
     *  @brief Set the data segment.
     *
     *  @param selector the GDT selector.
     */
    void SetDataSegment(const Selector selector);

    /*
     *  @brief Get the selector offset 
     * 
     *  @param selector the GDT selector.
     * 
     *  @return the selector offset into the GDT.
     */
    static SelectorOffset GetSelectorOffset(const Selector selector);

private:
    //! GDT Descriptor.
    struct [[gnu::packed]] Descriptor
    {
        uint16_t        m_size;
        PhysAddr m_addr;  // Address of the GDT
    };

    //! GDT Entry.
    class Entry : public Bitmap<64>
    {
    public:
        typedef BitField<Entry, 16> Limit;
        typedef BitField<Limit, 24> Base;
        typedef BitField<Base, 8> AccessByte;
        typedef BitField<AccessByte, 4> LimitRemainder;
        typedef BitField<LimitRemainder, 4> Flags;
        typedef BitField<Flags, 8> BaseRemainder;

        static Entry Create(const Base::ValueType base, const uint32_t limit, const AccessByte::ValueType accessByte,
            const Flags::ValueType flags);
    };

    //! Constructor.
    GlobalDescriptorTable();

    //! Destructor.
    ~GlobalDescriptorTable();

    Descriptor  m_descriptor;                       //< The GDT descriptor.
    Entry       m_entries[GDT_ENTRY_COUNT];         //< The list of GDT entries.

    friend Singleton<GlobalDescriptorTable>;
};

using GDT = GlobalDescriptorTable;

} // namespace x86

} // namespace BartOS

#endif // GDT_H
