#ifndef __SWIFT_H__
#define __SWIFT_H__

#include "riscv/misc.h"

typedef struct{
    uint window;
	uint t_last_decrease;
	uint num_acked;
}SwiftUserSlots;

typedef struct{
	uint ts;
	uint qlen;
	uint ttl;
	uint reserve;
}SwiftUserHeader;

#define UserSlots SwiftUserSlots
#define UserHeader SwiftUserHeader

#define UserTableSize 3
#define UserPacketSize 4

#define TracedPackets ACK

int send();
int recv();

#endif