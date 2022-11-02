ENTRY(_start)

SECTIONS {
    /* Start of kernel */
    . = 0x80200000;

    _image_start = .;

    .text : {
        *(.text.entry)
        *(.text)
        *(.text*)
    }

    .rodata : {
        *(.rodata)
        *(.rodata*)
    }

    . = ALIGN(1 << 16);
    .data : {
        *(.data.pagesize4)
        *(.data.pagesize)
        *(.data)
        *(.data*)
    }

    . = ALIGN(8);
    .bss : {
        _sbss = .;
        *(.bss)
        *(.bss*)

        . = ALIGN(16);
        _ebss = .;

        _init_stack_begin = .;
        . += 4096;
        . = ALIGN(16);
        _init_stack_end = .;
    }

    . = ALIGN(1 << 12);
    _image_end = .;
}
