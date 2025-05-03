section .text
global GDT_Load

; GDT_Load(struct GDTPtr* ptr)
GDT_Load:
    lgdt [eax]          ; load GDT pointer from eax
    mov ax, 0x10        ; data segment selector (offset 2 * 8)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ; far jump to code selector (offset 1 * 8)
.flush:
    ret
