bits 32
KERNEL_VIRTUAL_BASE equ 0xFFFFFFFF80000000

section .bss
align 16
global stack_top
stack_bottom:
resb 16384
stack_top:

section .data
align 4096
global p4_table
p4_table:
    dq p3_table - KERNEL_VIRTUAL_BASE + 0b11
    times (510) dq 0
    dq p3_table - KERNEL_VIRTUAL_BASE + 0b11

align 4096
p3_table:
    dq p2_table - KERNEL_VIRTUAL_BASE + 0b11
    times (509) dq 0
    dq p2_table - KERNEL_VIRTUAL_BASE + 0b11
    times (1) dq 0

align 4096
p2_table:
    dq 0x0000000000000000 + 0x83
    dq 0x0000000000200000 + 0x83
    times (509) dq 0
    dq p1_temp_map_table - KERNEL_VIRTUAL_BASE + 0b11

; Last 2MB of the kernel address space are used for temporary mappings
; This table is used for that mapping.
align 4096
global p1_temp_map_table
p1_temp_map_table:
    times (512) dq 0

section .rodata
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
.size:
    dw $ - gdt64 - 1
.addr:
    dq gdt64 - KERNEL_VIRTUAL_BASE

section .text
global _start
_start:
    cli
    cld

    ; push multiboot2 magic and boot_info respectively.
    mov edi, eax
    mov esi, ebx

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

    ; unmap identity mapping
    mov rax, 0
    mov [p4_table], rax
    mov [p3_table], rax
    ; reload cr3 to flush entire tlb
    mov rax, p4_table - KERNEL_VIRTUAL_BASE
    mov cr3, rax

    ; reload the gdt with virtual address.
    mov rax, gdt64.addr
    mov rbx, KERNEL_VIRTUAL_BASE
    add rax, rbx
    mov [gdt64.addr], rax
    lgdt [gdt64.pointer]

    ; adjust multiboot info address.
    add rsi, rbx

    push rdi
    push rsi

    ; Run global constructors
    extern _init
    call _init

    pop rsi
    pop rdi
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
