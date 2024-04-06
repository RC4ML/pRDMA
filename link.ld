ENTRY(_start)

/* 0-8K		Inst BRAM */
_code_size = 0x2000;/*4K=0x1000*/

/* 8-24K		Data BRAM */
_text_size = 0x1000;
_stack_size = 0x1000;
_heap_size = 0x2000;

/* 24k-~		HBM */
_print_size = 0x1000;



MEMORY
{
    ROM (rx) : ORIGIN	= 0x80000000, LENGTH = _code_size
    RAM_TEXT (rw) : ORIGIN	= ORIGIN(ROM) + _code_size, 		LENGTH = _text_size
    RAM_STACK (rw) : ORIGIN	= ORIGIN(RAM_TEXT) + _text_size,	LENGTH = _stack_size
    RAM_HEAP (rw) : ORIGIN	= ORIGIN(RAM_STACK) + _stack_size,	LENGTH = _heap_size
    RAM_PRINT (rw) : ORIGIN	= ORIGIN(RAM_HEAP) + _heap_size,	LENGTH = _print_size
}


SECTIONS
{
	.text : ALIGN(16){
		*(.text.custom_start)
		*(.text)
	} > ROM

    .data : ALIGN(16){
        *(.data)
		*(.sdata)
    } > RAM_TEXT

    .bss : ALIGN(16){
        *(.bss)
        *(.sbss)
    } > RAM_TEXT

	. = ORIGIN(RAM_HEAP);
	_heap_start = .;
	_heap_end = . + _heap_size;
	
	. = ORIGIN(RAM_STACK);
	_stack_top = . + _stack_size;

	. = ORIGIN(RAM_PRINT);
	_print_start = .;
	_print_end = . + _print_size;

	. = ALIGN(8); # 8 Bytes
    _end = .;
	
}

