#include "dcqcn.h"
#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "network/network.h"

#define BC_expires 1024*1024*10
#define T55us 55*1000/4
#define Cycle_64B (1000*16)<<8  //cycle/cacheline

int send(){
	Timer now = GetTime();
	int credit;
	Event e;
	while(1){
		poll_event_sync(&e);
		if(e.packet.type == CE){
			if(GetTime() - now >= 50*MicroSecond){
				e.packet.type = CNP;
				e.type = Send;
				now = GetTime();
			}else{
				e.type = Done;
			}
		}else if(has_data(e.packet.type)){
			int time_diff = (GetTime() - e.table.user_slots.timer)<<8;
			while(time_diff < (e.table.user_slots.divede_rate<<e.packet.len_log)){
				time_diff = (GetTime() - e.table.user_slots.timer)<<8;
			};
			e.type = Send;
			// credit -= e.packet.length;
			// update_table(credit,credit);
			update_table(user_slots.timer,GetTime());			
		}else{
			e.type = Send;
		}
		post_event(&e);
	}
}

int recv(){
	int Rt,Rc;   //MB/s
	const int Rai = 5;
	int a=1 << 16, g=(1<<16)/256;
	int last_time;
	int byte_count=0;
	int T = 0;
	int BC = 0;
	Event e;
	while(1){
		poll_event_async(&e);
		if(poll_event_async(&e) == 1){
			if(e.packet.type == CNP){
				Rt = e.table.user_slots.rate;
				Rc = fxp_mult(Rt, ((1<<16)-a/2));
				a = fxp_mult(a, ((1<<16)-((1<<16)-g)))+g;
				byte_count = 0;
				T = 0;
				BC = 0;
				last_time = GetTime();
				update_table(user_slots.rate, Rc);
				e.type = Done;
			}else if(has_data(e.packet.type)){
				byte_count += e.packet.length;
				if(byte_count >= BC_expires){
					BC++;
					byte_count = 0;
					if(T<5 && BC<5){
						Rc = (e.table.user_slots.rate + Rt)/2;
					}else{
						Rt += Rai;
						Rc = (e.table.user_slots.rate + Rt)/2;
					}
				}
				if(write_req(e.packet.type)){
					e.packet.type = ACK;
					e.type = GenTxEvent;
				}else{
					e.type = Done;
				}
				update_table(user_slots.rate, Rc);
			}
			post_event(&e);
		}else{
			if(GetTime()-last_time >= T55us){
				a = fxp_mult(a, ((1<<16)-g));
				last_time = GetTime();
				T++;
				if(T<5 && BC<5){
					Rc = (e.table.user_slots.rate + Rt)/2;
				}else{
					Rt = Rt + Rai;
					Rc = (e.table.user_slots.rate + Rt)/2;
				}
				update_table(user_slots.rate, Rc);
			}
		}
	}
}