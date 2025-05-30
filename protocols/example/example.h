#ifndef __EXAMPLE_CC_H__
#define __EXAMPLE_CC_H__

#include "riscv/misc.h"

#define UserTableSize 0
#define UserPacketSize 0

#define TracedPackets (WRITE_REQ_WITH_DATA | READ_RESP_WITH_DATA | ACK)

int recv();
int send();

#endif