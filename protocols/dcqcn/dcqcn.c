#include "dcqcn.h"
#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "network/network.h"

#define BC_expires 100

int dcqcn_send(){
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
				e.type = Drop;
			}
		}else if(has_data(e.packet.type)){
			credit = e.table.rate*(GetTime() - e.table.timer) + e.table.credit;
			if(credit >= e.packet.length){
				e.type = Send;
				credit -= e.packet.length;
				update_table(credit,credit);
				update_table(timer,GetTime());
			}else{
				e.type = LoopBack;
			}
		}else{
			e.type = Send;
		}
		post_event(&e);
	}
}

int dcqcn_recv(){
	int Rt,Rc;
	const int Rai = 1;
	float a=1, g=0.5;
	int last_time;
	int byte_count=0;
	int T = 0;
	int BC = 0;
	Event e;
	while(1){
		poll_event_async(&e);
		if(poll_event_async(&e) == 1){
			if(e.packet.type == CNP){
				Rt = e.table.rate;
				Rc = Rt*(1-a/2);
				a = (1-g)*a+g;
				byte_count = 0;
				T = 0;
				BC = 0;
				last_time = GetTime();
				update_table(rate, Rc);
			}else if(has_data(e.packet.type)){
				byte_count += e.packet.length;
				if(byte_count >= BC_expires){
					BC++;
					byte_count = 0;
					if(T<5 && BC<5){
						Rc = (e.table.rate + Rt)/2;
					}else{
						Rt += Rai;
						Rc = (e.table.rate + Rt)/2;
					}
				}
				update_table(rate, Rc);
			}
			e.type = Drop;
			post_event(&e);
		}else{
			if(GetTime()-last_time >= 55){
				a = (1-g)*a;
				last_time = GetTime();
				T++;
				if(T<5 && BC<5){
					Rc = (e.table.rate + Rt)/2;
				}else{
					Rt = Rt + Rai;
					Rc = (e.table.rate + Rt)/2;
				}
				update_table(rate, Rc);
			}
		}
	}
}