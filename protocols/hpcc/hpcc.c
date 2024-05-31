#include "hpcc.h"
#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "network/network.h"

#define SND_NXT 10
#define T 512 //base RTT

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
	int lastUpdateSeq=0;
	float u,txRate,U,n=0.95;
	int txBytes=0, qlen=0, ts=0, B=0,diff,incStage=0;
	Event e;
	while(1){
		poll_event_sync(&e);
		if(e.packet.type == RC_ACK){
			if(e.packet.user_header.seq>lastUpdateSeq){
				u=0;
				txRate = (e.packet.user_header.txBytes - txBytes)/(e.packet.user_header.ts-ts);
				if(e.packet.user_header.qlen < qlen){
					u = e.packet.user_header.qlen/(B*T)+txRate/e.packet.user_header.B;
				}else{
					u = qlen/(B*T)+txRate/e.packet.user_header.B;
				}
				diff = e.packet.user_header.ts - ts;
				if(diff>T){
					diff = T;
				}
				U = (1-diff/T)*U + diff/T*u;
				if((U >= n)||(incStage >= maxStage)){
					Wc = e.table.user_slots.window/(U/n)+Wai;
					incStage = 0;
				}else{
					Wc = e.table.user_slots.window + Wai;
					incStage++;
				}
				update_table(credit,e.table.credit+e.packet.length+Wc-e.table.user_slots.window);
				update_table(user_slots.window,Wc);
				lastUpdateSeq = lastUpdateSeq + SND_NXT;
			}else{
				update_table(credit,e.table.credit+e.packet.length);
			}
			e.type = Done;
			post_event(&e);
		}
	}
}