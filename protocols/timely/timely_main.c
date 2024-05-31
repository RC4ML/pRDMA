#include "riscv/misc.h"
#include "network/network.h"
#include "timely.h"


int main(){
	config_regs();
	#if defined TX
        send();
    #elif defined RX
        recv();
    #endif

}