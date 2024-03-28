#ifndef __MISC_H__
#define __MISC_H__


extern char _print_init_str[];

extern char _heap_start;
extern char _heap_end;
extern char _print_start;
extern char _print_end;


typedef unsigned int uint;

#define ReadCSR(csr, result)\
	__asm__ volatile("csrr %0, %1" : "=r"(result) : "i"(csr) : "memory");

#define WriteCSR(csr, value)\
	__asm__ volatile ("csrw %0, %1" :: "i"(csr), "r"(value) : "memory")

void* alloc(int size);
void print(const char* format, ...);
void sprint(char* buffer, const char* format, ...);
void dump_csr();
void trap(char* cause);

#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define ct_assert(e) enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }


#endif