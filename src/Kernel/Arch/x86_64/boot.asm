bits 32
KERNEL_VIRTUAL_BASE equ 0xFFFFFFFFC0000000
LAST_PAGE_OFFSET equ (511 * 8)

section .multiboot
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0                         ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header lengthmagic number' lets bootloader find the header

    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

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

section .data
align 4096
p4_table:
    dq p3_table - KERNEL_VIRTUAL_BASE + 0b11
    times (510) dq 0;
    dq p3_table - KERNEL_VIRTUAL_BASE + 0b11

align 4096
p3_table:
    dq p2_table - KERNEL_VIRTUAL_BASE + 0b11
    times (510) dq 0;
    dq p2_table - KERNEL_VIRTUAL_BASE + 0b11

align 4096
p2_table:
    dq 0x0000000000000000 + 0x83
    dq 0x0000000000200000 + 0x83
    times (510) dq 0

section .rodata
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64 - KERNEL_VIRTUAL_BASE

section .text
global _start
_start:
    cli
    cld

    ; load the 64-bit GDT
    lgdt [gdt64.pointer - KERNEL_VIRTUAL_BASE]

    ; Reset EFLAGS.
    push $0
    popf

; enable paging
    ; load P4 to cr3 register
    mov eax, p4_table - KERNEL_VIRTUAL_BASE
    mov cr3, eax

    ; enable PAE-flag in cr4
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set the long mode bit in the MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x00000101
    wrmsr

    ; enable paging in the cr0 register
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    push gdt64.code
    push long_mode_entry - KERNEL_VIRTUAL_BASE
    retf

bits 64
long_mode_entry:
    mov rax, long_mode_start
    jmp rax

long_mode_start:
    ; Set up the stack for C, stack grows downwards.
    mov rsp, stack_top

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
