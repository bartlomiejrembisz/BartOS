#ifndef INTERRUPT_DESCRIPTOR_TABLE_H
#define INTERRUPT_DESCRIPTOR_TABLE_H

#include "Lib/Core/BitFields.h"
#include "Lib/Core/Singleton.h"

#include "Isrs.h"

namespace BartOS
{

namespace x86_64
{

//! IDT descriptor
class [[gnu::packed]] Descriptor
{
public:
    uint16_t        m_size;     // Length of the IDT.
    PhysAddr m_addr;  // Address of the IDT.
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! Interrupt descriptor table entry.
class [[gnu::packed]] Gate
{
public:
    //! IDT Gate Word1.
    class Word1 : public Bitmap<64>
    {
    public:
        typedef BitField<Word1, 16> LowerFunPtr;
        typedef BitField<LowerFunPtr, 16> GDTSelector;
        typedef BitField<GDTSelector, 16> Options;
        typedef BitField<Options, 16> MiddleFunPtr;
    };

    //! IDT Gate Word2
    class Word2 : public Bitmap<64>
    {
    public:
        typedef BitField<Word2, 32> RemainingFunPtr;
        typedef BitField<RemainingFunPtr, 32> Reserved;
    };

    //! Constructor
    Gate();

    //! Destructor
    ~Gate();

private:
    /*
     *  @brief  Set the gate
     *
     *  @param isr_handler pointer to the interrupt service routine.
     *  @param interruptStackTable the interrupt stack table.
     */
    void Set(void(*isr_handler)(), const uint8_t interruptStackTable);

    Word1::Type     m_word1;       //< Gate Word 1.
    Word2::Type     m_word2;       //< Gate Word 2.

    friend class InterruptDescriptorTable;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! Interrupt descriptor table.
class InterruptDescriptorTable : public Singleton<InterruptDescriptorTable>
{
public:
    //! Constructor.
    InterruptDescriptorTable();

    //! Destructor.
    ~InterruptDescriptorTable();

    //! Initialize the Global Descriptor Table.
    void Initialize();

private:
    /*
     *  @brief Set the gate.
     *
     *  @param entry the entry enum.
     */
    void SetGate(const uint16_t entry);

    Descriptor  m_descriptor;               //< The IDT descriptor.
    Gate        m_gates[NUM_OF_GATES];

    friend class Singleton<InterruptDescriptorTable>;
};

using IDT = InterruptDescriptorTable;

} // namespace x86

} // namespace BartOS

#endif // INTERRUPT_DESCRIPTOR_TABLE_H
