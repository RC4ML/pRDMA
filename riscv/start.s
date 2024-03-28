.section .text.custom_start
.globl _start
_start:
    la sp, _stack_top    # 设置sp
	call _print_init;
    call main
	