/* The bootloader will look at this image and start execution at the symbol
   designated at the entry point. */
ENTRY(_start)
KERNEL_VIRTUAL_BASE = 0xffffffffc0000000;

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 0x00100000;

	.multiboot.data : {
       *(.multiboot.data)
   	}

	.multiboot.text : {
       *(.multiboot.text)
   	}

	. += KERNEL_VIRTUAL_BASE;
   	.text ALIGN(4096) : AT(ADDR(.text) - KERNEL_VIRTUAL_BASE) {
       *(.text)
   	}

   	.data ALIGN (4096) : AT(ADDR(.data) - KERNEL_VIRTUAL_BASE) {
       *(.data)
       *(.rodata*)
   	}

	.bss ALIGN (4096) : AT(ADDR(.bss) - KERNEL_VIRTUAL_BASE) {
		*(.bss)
	}

	KERNEL_END = .;
}


