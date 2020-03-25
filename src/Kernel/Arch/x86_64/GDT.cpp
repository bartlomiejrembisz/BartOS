#include "GDT.h"


namespace BartOS
{

namespace x86
{

namespace
{

//! AccessByte flags.
using AccessFlag = uint8_t;
// Present bit.
static const AccessFlag PRESENT_BIT     = 0b10000000;

// Privl flag.
static const AccessFlag USER_PRIV       = 0b01100000;   // Privl = 3 is user privilege (lowest)
static const AccessFlag KERNEL_PRIV     = 0b00000000;   // Privl = 0 is kernel privilege (highest)

// Descriptor type.
static const AccessFlag CODE_DATA_TYPE  = 0b00010000;   // Descriptor type = 1 for code and data.
static const AccessFlag TASK_SEG_TYPE   = 0b00000000;   // Descriptor type = 0 for task state segment.

// Executable bit.
static const AccessFlag EXE_SEG_BIT     = 0b00001000;   // Executable bit = 1 for executable segment.
static const AccessFlag DATA_SEG_BIT    = 0b00000000;   // Executable bit = 0 for data segment.

// Direction bit.
static const AccessFlag SEG_GROW_UP     = 0b00000100;   // Direction bit = 1 offset > limit (grows up).
static const AccessFlag SEG_GROW_DOWN   = 0b00000000;   // Direciton bit = 0 offset < limit (grows down)

// Readwrite bit. Whether is writable/readable.
static const AccessFlag READWRITE_BIT   = 0b00000010;   // EXE_SEG_BIT = readable, DATA_SET_BIT = writable.
static const AccessFlag ACCESSED_BIT    = 0b00000001;   // Whether the segment is being accessed

//! Flag bits for the Flags entry bits.
using FlagBit = uint8_t;
static const FlagBit GRANULARITY = 0b1000;
static const FlagBit SIZE        = 0b0100;
static const FlagBit L           = 0b0010;

} // namespace

GlobalDescriptorTable::GlobalDescriptorTable() : 
    m_descriptor { .m_size = sizeof(Entry) * GDT_ENTRY_COUNT, (uint64_t)&m_entries[0] }
{
}

// ---------------------------------------------------------------------------------------------------------

GlobalDescriptorTable::~GlobalDescriptorTable()
{
}

// ---------------------------------------------------------------------------------------------------------

void GlobalDescriptorTable::Initialize()
{
    m_entries[NULL_SEL] = Entry::Create(0, 0, 0, 0);

    m_entries[KERNEL_CODE] = Entry::Create(0, 0xFFFFFFFF, PRESENT_BIT | KERNEL_PRIV |
        CODE_DATA_TYPE | EXE_SEG_BIT | READWRITE_BIT, GRANULARITY | SIZE);

    m_entries[KERNEL_DATA] = Entry::Create(0, 0xFFFFFFFF, PRESENT_BIT | KERNEL_PRIV |
        CODE_DATA_TYPE | DATA_SEG_BIT | READWRITE_BIT, GRANULARITY | SIZE);

    m_entries[USER_CODE] = Entry::Create(0, 0xFFFFFFFF, PRESENT_BIT | USER_PRIV |
        CODE_DATA_TYPE | EXE_SEG_BIT | READWRITE_BIT, GRANULARITY | SIZE);

    m_entries[USER_DATA] = Entry::Create(0, 0xFFFFFFFF, PRESENT_BIT | USER_PRIV |
        CODE_DATA_TYPE | DATA_SEG_BIT | READWRITE_BIT, GRANULARITY | SIZE);
}

// ---------------------------------------------------------------------------------------------------------

GlobalDescriptorTable::SelectorOffset GlobalDescriptorTable::GetSelectorOffset(const Selector selector)
{
    return GDT_ENTRY_SIZE * selector;
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

auto GlobalDescriptorTable::Entry::Create(const Base::ValueType base, const uint32_t limit, const AccessByte::ValueType accessByte,
            const Flags::ValueType flags) -> Entry
{
    // Disgusting split bitfields not yet supported by BitFields.h

    Entry gdtEntry;

    gdtEntry.Set<Limit>(limit & 0xFFFF);
    gdtEntry.Set<LimitRemainder>((limit >> 16) & 0xF);
    gdtEntry.Set<Base>(base & 0xFFFFFF);
    gdtEntry.Set<BaseRemainder>((base >> 24) & 0xFF);
    gdtEntry.Set<AccessByte>(accessByte);
    gdtEntry.Set<Flags>(flags);

    return gdtEntry;
}

} // namespace x86

} // namespace BartOS
