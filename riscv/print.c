#include <stdarg.h>

#include "misc.h"
#include "encoding.h"

static char* cur_print_end = &_print_start;

void _inc_string_num(uint num){
	uint tmp;
	ReadCSR(CSR_PRINT_STRING_NUM, tmp);
	WriteCSR(CSR_PRINT_STRING_NUM, tmp+num);
}

/*Length would include the zero in the string tail*/ 
void _inc_string_length(int length){
	uint tmp;
	ReadCSR(CSR_PRINT_STRING_LEN, tmp);
	WriteCSR(CSR_PRINT_STRING_LEN, tmp+length);
	cur_print_end += length;
}

void _reset_print_status(){
	WriteCSR(CSR_PRINT_STRING_LEN, 0);
	WriteCSR(CSR_PRINT_STRING_NUM, 0);
}

void _print_init(){
	_reset_print_status();
	WriteCSR(CSR_PRINT_ADDR, &_print_start);
}

void _write_char(char byte) {
	*cur_print_end = byte;
	_inc_string_length(1);
}

void itoa(unsigned int value, char *buffer, int base, int uppercase) {
    char *p = buffer;
    if (value == 0) {
        *p++ = '0';
        *p = '\0';
        return;
    }

    char *start = p;

    while (value > 0) {
        int digit = value % base;
        if (digit < 10) {
            *p++ = '0' + digit;
        } else {
            if (uppercase) {
                *p++ = 'A' + digit - 10;
            } else {
                *p++ = 'a' + digit - 10;
            }
        }
        value /= base;
    }
    *p = '\0';

    // Reverse the string
    char *q = p - 1;
    while (start < q) {
        char tmp = *start;
        *start = *q;
        *q = tmp;
        start++;
        q--;
    }
}

void print(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    char *str = va_arg(args, char *);
                    while (*str) {
                        _write_char(*str++);
                    }
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    char buffer[32];
                    itoa(num, buffer, 10, 0);
                    char *str = buffer;
                    while (*str) {
                        _write_char(*str++);
                    }
                    break;
                }
                case 'x':
                case 'X': {
                    unsigned int num = va_arg(args, unsigned int);
                    char buffer[32];
                    itoa(num, buffer, 16, (*format == 'X'));
                    char *str = buffer;
                    while (*str) {
                        _write_char(*str++);
                    }
                    break;
                }
                default:
                    _write_char('%');
                    _write_char(*format);
            }
        } else {
            _write_char(*format);
        }
        format++;
    }

    va_end(args);
	_inc_string_num(1);
}

void sprint(char* buffer, const char* format, ...){
    va_list args;
    va_start(args, format);

    char* buf_ptr = buffer;
    const char* fmt_ptr = format;

    while (*fmt_ptr) {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            switch (*fmt_ptr) {
                case 's': {
                    const char* str = va_arg(args, const char*);
                    while (*str) {
                        *buf_ptr++ = *str++;
                    }
                    break;
                }
                case 'd': {
                    char num_str[12];
                    int num = va_arg(args, int);
                    itoa(num, num_str, 10, 0);
                    const char* tmp = num_str;
                    while (*tmp) {
                        *buf_ptr++ = *tmp++;
                    }
                    break;
                }
                case 'x': {
                    char num_str[12];
                    unsigned int num = va_arg(args, unsigned int);
                    itoa(num, num_str, 16, 0);
                    const char* tmp = num_str;
                    while (*tmp) {
                        *buf_ptr++ = *tmp++;
                    }
                    break;
                }
                case 'X': {
                    char num_str[12];
                    unsigned int num = va_arg(args, unsigned int);
                    itoa(num, num_str, 16, 1);
                    const char* tmp = num_str;
                    while (*tmp) {
                        *buf_ptr++ = *tmp++;
                    }
                    break;
                }
                default:
                    *buf_ptr++ = '%';
                    if (*fmt_ptr) {
                        *buf_ptr++ = *fmt_ptr;
                    } else {
                        fmt_ptr--; // 回退一个字符
                    }
                    break;
            }
        } else {
            *buf_ptr++ = *fmt_ptr;
        }
        fmt_ptr++;
    }

    *buf_ptr = '\0';
    va_end(args);
}


#define DUMP_START 0x50
uint dump_values[0x100-DUMP_START];

#define DUMP_CSR(csr)\
	ReadCSR(csr,dump_values[csr-DUMP_START])

#define PRINT_CSR(csr, name)\
	print("%s:%X\n",name,dump_values[csr-DUMP_START]);


void dump_csr(){
	DUMP_CSR(CSR_PRINT_ADDR);
	DUMP_CSR(CSR_PRINT_STRING_NUM);
	DUMP_CSR(CSR_PRINT_STRING_LEN);
	DUMP_CSR(CSR_TRAP);

	PRINT_CSR(CSR_PRINT_ADDR, "print_addr");
	PRINT_CSR(CSR_PRINT_STRING_NUM, "print_string_num");
	PRINT_CSR(CSR_PRINT_STRING_LEN, "print_string_len");
	PRINT_CSR(CSR_TRAP, "trap");
}