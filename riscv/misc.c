#include "misc.h"
#include "encoding.h"

static char* cur_heap_end = &_heap_start;

void* alloc(int size){
	if(cur_heap_end+size > &_heap_end){
		return (void*)-1;
	}else{
		cur_heap_end += size;
		return (void*)(cur_heap_end-size);
	}
}

void trap(char* cause){
	WriteCSR(CSR_TRAP,1);
	dump_csr();
	print("Exception:%s\n",cause);
	__asm__ volatile("WFI");
}