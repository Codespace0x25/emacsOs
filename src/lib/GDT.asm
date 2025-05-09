[BITS 32]
global GDT_Load
; void GDT_Load(struct GDTPtr* ptr)
GDT_Load:
    mov eax, [esp + 4]   ; in 32-bit cdecl, first argument is at [esp + 4]
    lgdt [eax]

    ; Reload segment registers with data selector (0x10)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to flush the instruction pipeline and load CS
    jmp 0x08:flush

flush:
    ret
