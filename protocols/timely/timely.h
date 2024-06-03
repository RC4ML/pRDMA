#ifndef __TIMELY_H__
#define __TIMELY_H__

#include "riscv/misc.h"

typedef struct{
    uint rate;
    uint timer;	
	uint divede_rate;
	uint prev_rtt;
	uint rtt_diff;
}TimelyUserSlots;

typedef struct{
	uint timestamp;
}TimelyUserHeader;

#define UserSlots TimelyUserSlots
#define UserHeader TimelyUserHeader

#define UserTableSize 5
#define UserPacketSize 1

#define TracedPackets ACK

int send();
int recv();

#endif