ENTRY(ISR_reset)

PROVIDE(_estack = ORIGIN(ram) + LENGTH(ram));

SECTIONS {
	.isr_vectors : {
		KEEP(*(.isr_vectors))
		. = ALIGN(4);
	} >rom

	.text : {
		*(.text*)

		. = ALIGN(4);
		*(.rodata*)

		KEEP(*(.init))
		KEEP(*(.fini))

		. = ALIGN(4);
	} >rom

	.ARM.exidx : {
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} >rom

	. = ALIGN(4);
	_etext = .;

	.noinit (NOLOAD) : {
		*(.noinit*)
		. = ALIGN(4);
	} >ram

	.data : {
		_data = .;

		*(.data*)
		. = ALIGN(4);
		*(.rxdata*)

		. = ALIGN(4);
		_edata = .;
	} >ram AT >rom
	_data_loadaddr = LOADADDR(.data);

	.bss : {
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} >ram

	.heap (NOLOAD) : {
		_heap = .;
		. = ALIGN(4);
		_eheap = .;
	} >ram
}
