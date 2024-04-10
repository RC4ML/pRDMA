#include "riscv/misc.h"
#include "network/network.h"
#include "example.h"


int main(){
	#if defined TX
        send();
    #elif defined RX
        recv();
    #endif

}