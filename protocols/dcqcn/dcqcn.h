#ifndef __DCQCN_H__
#define __DCQCN_H__

#include "riscv/misc.h"

typedef struct{
	uint slot1;
	uint slot2;
}DcqcnUserSlots;

typedef struct{
	uint field1;
	uint field2;
	uint field3;
	uint field4;
}DcqcnUserHeader;

#define UserSlots DcqcnUserSlots
#define UserHeader DcqcnUserHeader

#define UserTableSize 2
#define UserPacketSize 4

#define TracedPackets ACK

int send();
int recv();

#endif