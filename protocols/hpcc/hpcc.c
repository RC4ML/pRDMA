#include "hpcc.h"
#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "network/network.h"

#define SND_NXT 10
#define T 512 //base RTT
#define B 3000

int send(){
	Timer now = GetTime();
	int credit,seq=0;
	Event e;
	while(1){
		poll_event_sync(&e);
		 if(has_data(e.packet.type)){
			while(e.table.credit >= e.packet.length){};
				e.type = Send;
				credit -= e.packet.length;
				seq++;
				update_table(credit,credit);
				update_pkg(user_header.seq,seq);
		}else{
			e.type = Send;
		}
		post_event(&e);
	}
}


int recv(){
	int Wc;
	const int Wai = 10,maxStage = 5;
	float u,txRate,U,n=0.95;
	int diff;
	Event e;
	while(1){
		poll_event_sync(&e);
		if(e.packet.type == RC_ACK){
			if(e.packet.user_header.seq>e.table.user_slots.lastUpdateSeq){
				u=0;
				txRate = (e.packet.user_header.txBytes - e.table.user_slots.txBytes)/(e.packet.user_header.ts-e.table.user_slots.ts);
				if(e.packet.user_header.qlen < e.table.user_slots.qlen){
					u = e.packet.user_header.qlen/(B*T)+txRate/B;
				}else{
					u = e.table.user_slots.qlen/(B*T)+txRate/B;
				}
				diff = e.packet.user_header.ts - e.table.user_slots.ts;
				if(diff>T){
					diff = T;
				}
				U = (1-diff/T)*e.table.user_slots.U + diff/T*u;
				update_table(user_slots.U,U);
				if((U >= n)||(e.table.user_slots.incStage >= maxStage)){
					Wc = e.table.user_slots.window/(U/n)+Wai;
					update_table(user_slots.incStage,0);
				}else{
					Wc = e.table.user_slots.window + Wai;
					update_table(user_slots.incStage,e.table.user_slots.incStage+1);;
				}
				update_table(credit,e.table.credit+e.packet.length+Wc-e.table.user_slots.window);
				update_table(user_slots.window,Wc);
				update_table(user_slots.lastUpdateSeq,e.table.user_slots.lastUpdateSeq + SND_NXT);
			}else{
				update_table(credit,e.table.credit+e.packet.length);
			}
			update_table(user_slots.txBytes,e.packet.user_header.txBytes);
			update_table(user_slots.ts,e.packet.user_header.ts);
			update_table(user_slots.qlen,e.packet.user_header.qlen);
			e.type = Done;
			post_event(&e);
		}
	}
}