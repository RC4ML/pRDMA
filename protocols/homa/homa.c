#include "homa.h"
#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "network/network.h"

#define INIT 7

int send(){
	Timer now = GetTime();
	int credit;
	Event e;
	while(1){
		poll_event_sync(&e);
		if(has_data(e.packet.type)){
			if(likely(e.table.credit >= e.packet.length)){
				update_table(credit,e.table.credit-e.packet.length);
				e.type = Send;
			}else{
				e.type = LoopBack;
			}
			if(e.packet.type == RC_WRITE_FIRST || e.packet.type == RC_WRITE_ONLY || e.packet.type == RC_WRITE_ONLY_WIT_IMD || e.packet.type == RC_READ_RESP_FIRST || e.packet.type == RC_READ_RESP_ONLY){
				update_pkg(user_header.prio,INIT);
			}
		}else{
			e.type = Send;
		}
		post_event(&e);
	}
}

int recv(){
	int Rt,Rc;
	const int Rai = 1;
	float a=1, g=0.5;
	int last_time;
	int byte_count=0;
	int T = 0;
	int BC = 0;
	Event e;
	while(1){
		poll_event_sync(&e);
		if(e.packet.type == GRANT){
			update_table(credit, e.table.credit+e.packet.user_header.grant);
			update_table(user_slots.prio,e.packet.user_header.prio);
			e.type = Done;
			post_event(&e);
		}else if(e.packet.type == RC_WRITE_FIRST || e.packet.type == RC_WRITE_ONLY || e.packet.type == RC_WRITE_ONLY_WIT_IMD || e.packet.type == RC_READ_RESP_FIRST || e.packet.type == RC_READ_RESP_ONLY){
			e.type = GRANT;
			update_pkg(user_header.grant, e.table.user_slots.msg_length);
			update_pkg(user_header.prio, e.packet.user_header.prio++);
			post_event(&e);
		}
	}
}
