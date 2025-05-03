[bits 32]
global keyboard_handler_stub
extern keyboard_handler

keyboard_handler_stub:
    cli            ; (optional — for safety)
    pusha          ; Save all registers
    call keyboard_handler
    popa
    sti            ; (re-enable interrupts)
    iretd          ; Return from interrupt (32-bit)
