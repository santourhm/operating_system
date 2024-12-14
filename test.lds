/*
 * ld directives the for kernel
 */

FORCE_COMMON_ALLOCATION
SECTIONS {
	/* The kernel starts at 0x1000 */
	. = 0x1000;
	.text : {
		*(.text.init.test) *(.text.init) *(.text)
		_etext = .;
	}

	.data : {
		*(.fini)
		*(.anno)
		*(.rodata)
		*(.rodata.str1.1)
		*(.rodata.str1.4)
		*(.rodata.str1.32)
		*(__ex_table)
		*(.rodata.cst8)
		*(.data)
		_edata  =  .;
	}

	.bss : {
		__bss_start = .;
		*(.bss)
		*(COMMON)
		__bss_end = .;
	}

	_end = .;
}
