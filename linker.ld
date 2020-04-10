OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_start)

KERNEL_PHYSICAL_ADDRESS = 0x0100000;
KERNEL_VIRTUAL_BASE = 0xFFFFFFFFC0000000;
KERNEL_VIRTUAL_ADDRESS = KERNEL_VIRTUAL_BASE + KERNEL_PHYSICAL_ADDRESS;

SECTIONS
{
    . = KERNEL_VIRTUAL_ADDRESS;
    __kernel_virtual_start = .;
    __kernel_physical_start = KERNEL_PHYSICAL_ADDRESS;

    /* Multiboot header. */
    .multiboot ALIGN(4K): AT(ADDR(.multiboot) - KERNEL_VIRTUAL_BASE)
    {
        *(.multiboot)
    }

    /* Kernel */
    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRTUAL_BASE)
    {
        *(.text)
    }

    /* Read-only data. */
    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRTUAL_BASE)
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRTUAL_BASE)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRTUAL_BASE)
    {
        *(COMMON)
        *(.bss)
    }

    __kernel_physical_end = . - KERNEL_VIRTUAL_BASE;
    __kernel_virtual_end = .;
}