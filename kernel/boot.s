SECTION .multiboot_header
ALIGN 8
multiboot_header_start:
    dd 0xe85250d6
    dd 0
    dd multiboot_header_end - multiboot_header_start
    dd 0x17adaefa

    dw 5
    dw 0
    dd 24
    dd 0
    dd 0
    dd 32
    dd 0

    dw 0
    dw 0
    dd 8
multiboot_header_end:

SECTION .text
GLOBAL start
EXTERN kmain

start:
    cli
    mov esp, stack_top
    push ebx
    push eax
    call kmain

halt:
    cli
    hlt
    jmp halt

SECTION .bss
ALIGN 16
stack_bottom:
    resb 16384
stack_top:

