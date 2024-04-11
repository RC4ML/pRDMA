#include "simulation.h"
#include "riscv/recursive_macro.h"

void init_event(CCTable* table, PacketMeta* packet){
	_write_to_csr_table(table);
	_write_to_csr_packet(packet);

	IncCSR(CSR_RECEIVED_EVENT_COUNT, 1);

	Assert(ReadCSR(CSR_HAS_EVENT) == 0);
	WriteCSR(CSR_HAS_EVENT, 1);
}

void process_event(CCTable* table){
	_read_from_csr_table(table);/*Update CCTable from CSRs*/

	/*Generate next event using same packet*/
	IncCSR(CSR_RECEIVED_EVENT_COUNT, 1);

	Assert(ReadCSR(CSR_HAS_EVENT) == 0);
	WriteCSR(CSR_HAS_EVENT, 1);
}