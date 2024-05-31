#ifndef __MISC_H__
#define __MISC_H__


extern char _heap_start;
extern char _heap_end;
extern char _print_start;
extern char _print_end;


typedef unsigned int uint;

typedef uint Timer;

#define MicroSecond 1000/4

#define GetTime() ReadCSR(CSR_MTIME)


#define fxp_mult(a, b)     ((unsigned int)(((a * b) >> 16)))

#define ReadCSR(csr)({ uint __tmp;\
	__asm__ volatile("csrr %0, %1" : "=r"(__tmp) : "i"(csr) : "memory");\
	__tmp;})

#define WriteCSR(csr, value)\
	__asm__ volatile ("csrw %0, %1" :: "i"(csr), "r"(value) : "memory")

#define WriteCSRI(csr, zimm)\
	__asm__ volatile ("csrwi %0, %1" :: "i"(csr), "i"(zimm) : "memory")	

#define SwapCSR(csr, value)({\
	__asm__ volatile("csrrw %0, %1, %2" : "=r"(value) : "i"(csr), "r"(value): );\
	value;})

#define SwapCSRI(csr, value)({ uint __tmp;\
	__asm__ volatile("csrrwi %0, %1, %2" : "=r"(__tmp) : "i"(csr), "i"(value): "memory");\
	__tmp;})	

#define IncCSR(csr, value) WriteCSR(csr, ReadCSR(csr)+value);

#define likely(x) __builtin_expect(!!(x), 1) //gcc内置函数, 帮助编译器分支优化
#define unlikely(x) __builtin_expect(!!(x), 0)

void* alloc(int size);
void print(const char* format, ...);
void sprint(char* buffer, const char* format, ...);
void dump_csr();
void _trap(char* cause);

#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define CompileTimeAssert(e) enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }

#define trap(cause)\
	WriteCSR(CSR_TRAP,1);\
	dump_csr();\
	print("Trap %s:%d %s\n",__FILE__, __LINE__, cause);\
	__asm__ volatile("WFI");

#define Assert(e) \
	if(!(e)){trap("Assert fail:" #e );}

#endif