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
#elif defined TIMELY
	#include "protocols/timely/timely.h"
#elif defined HPCC
	#include "protocols/hpcc/hpcc.h"
#elif defined HOMA
	#include "protocols/homa/homa.h"		
#elif defined SWIFT
	#include "protocols/swift/swift.h"					
#else
	#include "protocols/example/example.h"
#endif


void init_event(CCTable* table, PacketMeta* packet);
void process_event(CCTable* table);


#endif