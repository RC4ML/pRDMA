#include "timely.h"
#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "network/network.h"


#define Cycle_64B 14400 << 8  //cycle/cacheline (1000ns/(4.444444ns/cycle) *64B)
#define minRTT 512 //fix
#define T_low 600 //fix
#define T_high 2000 //fix

int send(){
	Timer now = GetTime();
	int credit;
	Event e;
	while(1){
		poll_event_sync(&e);
		if(likely(has_data(e.packet.type))){
			int time_diff = (GetTime() - e.table.user_slots.timer)<<8;
			while(time_diff < (e.table.user_slots.divede_rate<<e.packet.len_log)){
				time_diff = (GetTime() - e.table.user_slots.timer)<<8;
			};
			e.type = Send;
			// credit -= e.packet.length;
			// update_table(credit,credit);
			update_pkg(user_header.timestamp,GetTime());
			update_table(user_slots.timer,GetTime());			
		}else{
			e.type = Send;
		}
		post_event(&e);
	}
}

int recv(){
	int Rc;
	const int Rai = 5;
	int a=1<<16 *7/8, b=1<<16*4/5;
	int prev_rtt = 1000, new_rtt, new_rtt_diff,rtt_diff;
	int gradient;
	int tmp,tmp1;
	Event e;
	while(1){
		poll_event_sync(&e);
		if(e.packet.type == RC_ACK){
			new_rtt = GetTime() - e.packet.user_header.timestamp;
			new_rtt_diff = new_rtt - prev_rtt;
			prev_rtt =  new_rtt;
			rtt_diff = fxp_mult(rtt_diff, ((1<<16)-a)) + fxp_mult(new_rtt_diff, a);
			gradient = (rtt_diff<<16)/512;
			if(new_rtt < T_low){
				Rc = e.table.user_slots.rate + Rai;
			}else if(new_rtt > T_high){
				tmp = 1<<16 - ((T_high<<16)/new_rtt);
				tmp1 = 1<<16 - tmp;
				Rc = fxp_mult(e.table.user_slots.rate, tmp1);
			}else if(gradient <= 0){
				Rc = e.table.user_slots.rate + 5*Rai;
			}else{
				tmp = fxp_mult(b, gradient);
				Rc = fxp_mult(e.table.user_slots.rate, ((1<<16) - tmp));
			}
			e.type = Done;
			update_table(user_slots.rate, Rc);
		}
		post_event(&e);
	}
}