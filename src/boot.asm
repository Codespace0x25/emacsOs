[bits 32]
section .multiboot2
align 8

multiboot2_header:
    dd 0xE85250D6             ; magic
    dd 0                      ; architecture (i386 = 0)
    dd multiboot2_header_end - multiboot2_header ; header length
    dd 0x100000000 - (0xE85250D6 + 0 + (multiboot2_header_end - multiboot2_header)) ; checksum

    ; Framebuffer tag (type 5, size 20)
    ;;align 8
    ;;dw 5        ; type
    ;;dw 0        ; flags (must be 0)
    ;;dd 20       ; size
    ;;dd 1024     ; width
    ;;dd 768      ; height
    ;;dd 32       ; depth (bits per pixel)

    ; End tag
    align 8
    dw 0
    dw 0
    dd 8

multiboot2_header_end:

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .text
global _start

extern kernel_main

_start:
    mov esp, stack_top
    ;;push ebx		     ; ;;ebx contains pointer to Multiboot2 info
    call kernel_main      ; void kernel_main(uint32_t mb_info_ptr)

    cli
.hang:
    hlt
    jmp .hang
