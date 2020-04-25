#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

/*  How many bytes from the start of the file we search for the header. */
#define MULTIBOOT_SEARCH                        32768
#define MULTIBOOT_HEADER_ALIGN                  8

/*  The magic field should contain this. */
#define MULTIBOOT2_HEADER_MAGIC                 0xe85250d6

/*  This should be in %eax. */
#define MULTIBOOT2_BOOTLOADER_MAGIC             0x36d76289

/*  Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN                     0x00001000

/*  Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN                    0x00000008

/*  Flags set in the ’flags’ member of the multiboot header. */

#define MULTIBOOT_TAG_ALIGN                  8
#define MULTIBOOT_TAG_TYPE_END               0
#define MULTIBOOT_TAG_TYPE_CMDLINE           1
#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME  2
#define MULTIBOOT_TAG_TYPE_MODULE            3
#define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO     4
#define MULTIBOOT_TAG_TYPE_BOOTDEV           5
#define MULTIBOOT_TAG_TYPE_MMAP              6
#define MULTIBOOT_TAG_TYPE_VBE               7
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER       8
#define MULTIBOOT_TAG_TYPE_ELF_SECTIONS      9
#define MULTIBOOT_TAG_TYPE_APM               10
#define MULTIBOOT_TAG_TYPE_EFI32             11
#define MULTIBOOT_TAG_TYPE_EFI64             12
#define MULTIBOOT_TAG_TYPE_SMBIOS            13
#define MULTIBOOT_TAG_TYPE_ACPI_OLD          14
#define MULTIBOOT_TAG_TYPE_ACPI_NEW          15
#define MULTIBOOT_TAG_TYPE_NETWORK           16
#define MULTIBOOT_TAG_TYPE_EFI_MMAP          17
#define MULTIBOOT_TAG_TYPE_EFI_BS            18
#define MULTIBOOT_TAG_TYPE_EFI32_IH          19
#define MULTIBOOT_TAG_TYPE_EFI64_IH          20
#define MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR    21

#define MULTIBOOT_HEADER_TAG_END  0
#define MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST  1
#define MULTIBOOT_HEADER_TAG_ADDRESS  2
#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS  3
#define MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS  4
#define MULTIBOOT_HEADER_TAG_FRAMEBUFFER  5
#define MULTIBOOT_HEADER_TAG_MODULE_ALIGN  6
#define MULTIBOOT_HEADER_TAG_EFI_BS        7
#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI32  8
#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64  9
#define MULTIBOOT_HEADER_TAG_RELOCATABLE  10

#define MULTIBOOT_ARCHITECTURE_I386  0
#define MULTIBOOT_ARCHITECTURE_MIPS32  4
#define MULTIBOOT_HEADER_TAG_OPTIONAL 1

#define MULTIBOOT_LOAD_PREFERENCE_NONE 0
#define MULTIBOOT_LOAD_PREFERENCE_LOW 1
#define MULTIBOOT_LOAD_PREFERENCE_HIGH 2

#define MULTIBOOT_CONSOLE_FLAGS_CONSOLE_REQUIRED 1
#define MULTIBOOT_CONSOLE_FLAGS_EGA_TEXT_SUPPORTED 2

namespace BartOS
{
typedef unsigned char           multiboot_uint8_t;
typedef unsigned short          multiboot_uint16_t;
typedef unsigned int            multiboot_uint32_t;
typedef unsigned long long      multiboot_uint64_t;

struct multiboot_header
{
    /*  Must be MULTIBOOT_MAGIC - see above. */
    multiboot_uint32_t magic;

    /*  ISA */
    multiboot_uint32_t architecture;

    /*  Total header length. */
    multiboot_uint32_t header_length;

    /*  The above fields plus this one must equal 0 mod 2^32. */
    multiboot_uint32_t checksum;
};

struct [[gnu::packed]] multiboot_header_tag
{
    multiboot_uint16_t type;
    multiboot_uint16_t flags;
    multiboot_uint32_t size;
};

struct [[gnu::packed]] multiboot_header_tag_information_request
{
    multiboot_uint16_t type;
    multiboot_uint16_t flags;
    multiboot_uint32_t size;
    multiboot_uint32_t requests[3];
};

struct [[gnu::packed]] multiboot_mmap_entry
{
    multiboot_uint64_t addr;
    multiboot_uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE		        1
#define MULTIBOOT_MEMORY_RESERVED		        2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
    multiboot_uint32_t type;
    multiboot_uint32_t zero;
};

struct [[gnu::packed]] multiboot_tag
{
    multiboot_uint32_t type;
    multiboot_uint32_t size;
};

struct [[gnu::packed]] multiboot_tag_mmap : public multiboot_tag
{
    multiboot_uint32_t entry_size;
    multiboot_uint32_t entry_version;
    struct multiboot_mmap_entry entries;
};

struct [[gnu::packed]] multiboot_tag_elf_sections : public multiboot_tag
{
    multiboot_uint32_t num;
    multiboot_uint32_t entsize;
    multiboot_uint32_t shndx;
    char sections;
};

struct [[gnu::packed]] multiboot_tag_module : public multiboot_tag
{
    multiboot_uint32_t mod_start;
    multiboot_uint32_t mod_end;
    char cmdline;
};

struct [[gnu::packed]] boot_info
{
    multiboot_uint32_t total_size;
    multiboot_uint32_t reserved;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief The multiboot2 header.
 */
struct [[gnu::packed]] MyMultiboot2Header
{
    multiboot_header                                        m_multibootHeader;              ///< The multiboot2 header.
    alignas(8) multiboot_header_tag_information_request     m_multibootHeaderInfoReq;       ///< The info request header tag.
    alignas(8) multiboot_header_tag                         m_multibootHeaderModAlign;      ///< The module alignment header tag.

    alignas(8) multiboot_header_tag                         m_multibootHeaderTagEnd;        ///< The header tag terminator. Must be last.
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

extern const BartOS::boot_info *g_pBootInfo;

// ---------------------------------------------------------------------------------------------------------

template <typename TAG>
const TAG *GetMultiboot2Tag(const multiboot_uint32_t type);

} // namespace BartOS

#endif // MULTIBOOT2_H