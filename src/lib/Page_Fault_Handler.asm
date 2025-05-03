; page_fault_handler_stub.asm
[global page_fault_handler_stub]
[extern page_fault_handler]

page_fault_handler_stub:
    cli
    pushad
    call page_fault_handler
    popad
    iretd
