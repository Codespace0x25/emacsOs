ENTRY(_start)

SECTIONS {
    . = 1M;

    .multiboot :
    {
        KEEP(*(.multiboot))
    }

    .text :
    {
        *(.text*)
    }

    .bss :
    {
        *(COMMON)
        *(.bss*)
    }
}
