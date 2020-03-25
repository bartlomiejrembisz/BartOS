OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_start)

SECTIONS
{
	. = 1M;

     __kernel_start = .;

     .multiboot ALIGN(4K):
     {
        *(.multiboot)
     }

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

	 __kernel_end = .;
}