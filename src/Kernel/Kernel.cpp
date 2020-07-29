#include <Kernel/Arch/x86_64/Interrupts/InterruptDescriptorTable.h>
#include "BartOS.h"

#include "Kernel/Arch/x86_64/GDT.h"
#include "Kernel/Arch/x86_64/CPU.h"
#include "Kernel/Memory/Pmm.h"
#include "Kernel/Memory/Vmm.h"

#include "Multiboot2.h"

namespace BartOS
{

/*
 *  @brief the multiboot2 header struct placed in the .multiboot section.
 */
const BartOS::MyMultiboot2Header g_myMultiboot2Header __attribute__ ((section (".multiboot"))) =
{
    .m_multibootHeader =
    {
        .magic = MULTIBOOT2_HEADER_MAGIC,
        .architecture = MULTIBOOT_ARCHITECTURE_I386,
        .header_length = sizeof(BartOS::MyMultiboot2Header),
        .checksum = 0x100000000 - (0xe85250d6 + 0 + sizeof(BartOS::MyMultiboot2Header))
    },

    .m_multibootHeaderInfoReq =
    {
        .type = MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST,
        .flags = 0,
        .size = sizeof(multiboot_header_tag_information_request),
        .requests = { MULTIBOOT_TAG_TYPE_MODULE, MULTIBOOT_TAG_TYPE_MMAP, MULTIBOOT_TAG_TYPE_ELF_SECTIONS}
    },

    .m_multibootHeaderModAlign =
    {
        .type = MULTIBOOT_HEADER_TAG_MODULE_ALIGN,
        .flags = 0,
        .size = sizeof(multiboot_header_tag)
    },

    .m_multibootHeaderTagEnd =
    {
        .type = MULTIBOOT_HEADER_TAG_END,
        .flags = 0,
        .size = sizeof(multiboot_header_tag),
    }
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

extern "C" [[noreturn]] void kernel_main(uint32_t magic, const boot_info *pBootInfo)
{
    using namespace BartOS;
    if (MULTIBOOT2_BOOTLOADER_MAGIC != magic)
    {
        kprintf("Multiboot2 header doesn't match the magic number. %x != %x", MULTIBOOT2_BOOTLOADER_MAGIC, magic);
        x86_64::CPU::Hlt();
    }
    
    kprintf("BartOS by Bartlomiej Rembisz\n");
    kprintf("Kernel loaded:\n");
    kprintf("Virtual memory\t\t\t\tPhysical memory\n");
    kprintf("%p => %p\t", __kernel_virtual_start, __kernel_virtual_end);
    kprintf("%p => %p\n", __kernel_physical_start, __kernel_physical_end);

    g_pBootInfo = pBootInfo;

    x86_64::GDT::Get().Initialize();
    x86_64::IDT::IDT::Get().Initialize();

    StatusCode statusCode = MM::Pmm::Get().Initialize(GetMultiboot2Tag<multiboot_tag_mmap>(MULTIBOOT_TAG_TYPE_MMAP));
    if (STATUS_CODE_SUCCESS != statusCode)
    {
        kprintf("[PMM] Failed to initialize the physical memory manager, status code=%u - %s", statusCode, "");
        x86_64::CPU::Hlt();
    }

    kprintf("\nKernel P4 Table: %p\n", MM::Vmm::Get().m_pKernelP4Table);

    x86_64::CPU::Sti();

    while (true);
}

} // namespace BartOS
