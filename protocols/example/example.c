#include "example.h"
#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "network/network.h"

int send(){
	Event e;
	while(1){
		poll_event_sync(&e);
		if(has_data(e.packet.type)){
			if(e.table.credit >= e.packet.length){
				update_table(credit,e.table.credit-e.packet.length);
				e.type = Send;
			}else{
				e.type = LoopBack;
			}
		}else{
			e.type = Send;
		}
		post_event(&e);
	}
}

int recv(){
	Event e;
	while(1){
		poll_event_sync(&e);
		if(e.packet.type == RC_ACK){
			update_table(credit, e.table.credit+e.packet.length);
		}
		e.type = Drop;
		post_event(&e);
	}
}