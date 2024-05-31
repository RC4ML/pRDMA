#ifndef __HOMA_H__
#define __HOMA_H__

#include "riscv/misc.h"

typedef struct{
	uint prio;
	uint msg_length;
}HomaUserSlots;

typedef struct{
	uint grant;
	uint prio;
}HomaUserHeader;

#define UserSlots HomaUserSlots
#define UserHeader HomaUserHeader

#define UserTableSize 2
#define UserPacketSize 2

#define TracedPackets ACK

int send();
int recv();

#endif