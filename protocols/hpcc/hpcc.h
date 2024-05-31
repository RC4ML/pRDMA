#ifndef __HPCC_H__
#define __HPCC_H__

#include "riscv/misc.h"

typedef struct{
    uint window;
}HpccUserSlots;

typedef struct{
	uint seq;
	uint qlen;
	uint txBytes;
	uint ts;
	uint B;
	uint reserve0;
	uint reserve1;
	uint reserve2;
}HpccUserHeader;

#define UserSlots HpccUserSlots
#define UserHeader HpccUserHeader

#define UserTableSize 1
#define UserPacketSize 8

#define TracedPackets ACK

int send();
int recv();

#endif