
.code32

.global load_gdt
load_gdt:
    mov 0xc(%esp), %edx
    mov 0x4(%esp), %eax
    lgdt (%eax)

    mov %edx, %ds   # Reload all the data descriptors with Data selector (2nd arg)
    mov %edx, %es
    mov %edx, %gs
    mov %edx, %fs
    mov %edx, %ss

    pushl 0x8(%esp)     # Create FAR pointer on stack using Code selector (2nd argument).
    pushl $_gdt_flush    # Offset of FAR JMP will be _gdt_flush label below

    ljmp *(%esp)
    
_gdt_flush:
    add $8, %esp    # Restore stack (remove 2 DWORD values we put on stack to
                    #   create FAR pointer)
    ret
