#include "network.h"

void config_regs(){
	WriteCSR(CSR_TRACED_PACKETS, TracedPackets);
	WriteCSR(CSR_USER_TABLE_SIZE, UserTableSize);
	WriteCSR(CSR_USER_PACKET_SIZE, UserPacketSize);
	CompileTimeAssert(UserPacketSize==0 ||\
		UserPacketSize == 1 ||\
		UserPacketSize == 2 ||\
		UserPacketSize == 4 ||\
		UserPacketSize == 8 \
		);
}