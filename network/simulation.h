#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "riscv/recursive_macro.h"
#include "network.h"


#if defined EXAMPLE
	#include "protocols/example/example.h"
#elif defined DCQCN
	#include "protocols/dcqcn/dcqcn.h"
#else
	#include "protocols/example/example.h"
#endif


void init_event(CCTable* table, PacketMeta* packet);
void process_event(CCTable* table);


#endif