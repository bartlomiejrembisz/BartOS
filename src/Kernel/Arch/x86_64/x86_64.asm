section .text

; Load the global descriptor table.
; rdi = pointer to global_descriptor
; rsi = code_off
; rdx = data_off
global load_gdt
load_gdt:
    ; push global descriptor to the GDT register.
    lgdt [rdi]

    ; push data segment offset to GDT data registers
    mov ds, rdx
    mov es, rdx
    mov gs, rdx
    mov fs, rdx
    mov ss, rdx

    ; push code segment and .gdt_flush address to stack to form an address to jump to.
    push rsi
    push .gdt_flush

    ; dereference stack and jump.
    jmp [rsp]

.gdt_flush:
    ; restore the stack to what it was before the jump.
    add rsp, 0x10

    ret

global gdt_set_code_segment
gdt_set_code_segment:
    ; push code segment and .gdt_flush address to stack to form an address to jump to.
    push rdi
    push load_gdt.gdt_flush

    ; dereference stack and jump to gdt_flush.
    jmp [rsp]

global gdt_set_data_segment
gdt_set_data_segment:
    ; push data segment offset to GDT data registers
    mov ds, rdi
    mov es, rdi
    mov gs, rdi
    mov fs, rdi
    mov ss, rdi

    ret

; Load the interrupt descriptor table.
; rdi = pointer to global_descriptor
global load_idt
load_idt:
    lidt [rdi]

    ret
