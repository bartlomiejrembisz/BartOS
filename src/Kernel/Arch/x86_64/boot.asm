section .multiboot
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0                         ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header lengthmagic number' lets bootloader find the header
    ; checksum

    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; insert optional multiboot tags here

    ; required end tag
    dw 0    ; type

    dw 0    ; flags
    dd 8    ; size
header_end:

section .bss
align 16
global stack_top
stack_bottom:
resb 16384
stack_top:

section .text
global long_mode_start
long_mode_start:
    bits 64

    ; Run global constructors
    extern _init
    call _init

    ; Enter the high-level kernel.
    extern kernel_main
    call kernel_main

    ; Run global destructors.
    extern _fini
    call _fini

    ; Infinite loop.
    cli
.l1:hlt
    jmp .l1
