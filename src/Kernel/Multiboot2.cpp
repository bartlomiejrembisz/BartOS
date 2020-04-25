#include "Multiboot2.h"
#include "BartOS.h"

namespace BartOS
{

const BartOS::boot_info *g_pBootInfo = nullptr;

// ---------------------------------------------------------------------------------------------------------

template <typename TAG>
const TAG *GetMultiboot2Tag(const multiboot_uint32_t type)
{
    if (!g_pBootInfo)
        return nullptr;

    const uintptr_t tagStartAddr = reinterpret_cast<uintptr_t>(g_pBootInfo) + sizeof(boot_info);
    const uintptr_t tagEndAddr = tagStartAddr + g_pBootInfo->total_size;

    uintptr_t tagAddr = tagStartAddr;
    while (tagAddr < tagEndAddr)
    {
        const multiboot_tag *pMultibootTag = reinterpret_cast<const multiboot_tag *>(tagAddr);
        if (MULTIBOOT_HEADER_TAG_END == pMultibootTag->type)
            return nullptr;

        if (type == pMultibootTag->type)
            return reinterpret_cast<const TAG *>(pMultibootTag);

        tagAddr += ((pMultibootTag->size + 7) & ~7);
    }

    return nullptr;
}

template const multiboot_tag_mmap *GetMultiboot2Tag<multiboot_tag_mmap>(const multiboot_uint32_t type);
template const multiboot_tag_elf_sections *GetMultiboot2Tag<multiboot_tag_elf_sections>(const multiboot_uint32_t type);

} // namespace BartOS