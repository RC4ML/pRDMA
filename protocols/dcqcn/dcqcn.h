#ifndef __DCQCN_H__
#define __DCQCN_H__

#include "riscv/misc.h"


typedef struct{
    uint rate;
    uint timer;
	uint divede_rate;
    uint byte_count;
    uint BC;
    uint T;
    uint last_time;
    uint rt;
    uint a;
}DcqcnUserSlots;

typedef struct{
	uint ecn;
}DcqcnUserHeader;

#define UserSlots DcqcnUserSlots
#define UserHeader DcqcnUserHeader

#define UserTableSize 9
#define UserPacketSize 1


#define TracedPackets (WRITE_REQ_WITH_DATA | READ_RESP_WITH_DATA)

int send();
int recv();

#endif