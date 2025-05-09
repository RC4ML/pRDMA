#ifndef __HPCC_H__
#define __HPCC_H__

#include "riscv/misc.h"

typedef struct{
    uint window;
	uint seq;
	uint qlen0;
	uint txBytes0;
	uint ts0;	
	uint qlen1;
	uint txBytes1;
	uint ts1;	
	uint lastUpdateSeq;
	uint incStage;
	uint U;
}HpccUserSlots;

typedef struct{
	uint seq;
	uint qlen0;
	uint txBytes0;
	uint ts0;
	uint B;
	uint qlen1;
	uint txBytes1;
	uint ts1;	
}HpccUserHeader;

#define UserSlots HpccUserSlots
#define UserHeader HpccUserHeader

#define UserTableSize 11
#define UserPacketSize 8

#define TracedPackets ACK

int send();
int recv();

#endif