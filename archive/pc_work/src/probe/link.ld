ENTRY(_start)
SECTIONS
{
    . = 0x40000000;
    .text : { KEEP(*(.head)) *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON) }
    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }
}
