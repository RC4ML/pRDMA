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
		//  if(has_data(e.packet.type)){
			while(e.table.credit >= e.packet.length){};
				e.type = Send;
				credit -= e.packet.length;
				seq++;
				update_table(credit,credit);
				update_pkg(user_header.seq,seq);
		// }else{
		// 	e.type = Send;
		// }
		post_event(&e);
	}
}


int recv(){
	int Wc;
	const int Wai = 10,maxStage = 5;
	float u,u1,txRate0,txRate1,U,n=0.95;
	int diff;
	int BT = B*T;
	Event e;
	while(1){
		poll_event_sync(&e);
		if(e.packet.type == RC_ACK){
			if(e.packet.user_header.seq>e.table.user_slots.lastUpdateSeq)
			{
				u=0;
				int txBytes_diff0 = e.packet.user_header.txBytes0 - e.table.user_slots.txBytes0;
				int tx_diff0 = e.packet.user_header.ts0-e.table.user_slots.ts0;
				txRate0 = txBytes_diff0/tx_diff0;
				if(e.packet.user_header.qlen0 < e.table.user_slots.qlen0)
				{
					u1 = e.packet.user_header.qlen0/BT;
				}else
				{
					u1 = e.table.user_slots.qlen0/BT;
				}
				u1 = u1 + txRate0/B;
				if(u1>u){
					u = u1;
					diff = e.packet.user_header.ts0 - e.table.user_slots.ts0;
				}
				int txBytes_diff1 = e.packet.user_header.txBytes1 - e.table.user_slots.txBytes1;
				int tx_diff1 = e.packet.user_header.ts1-e.table.user_slots.ts1;
				txRate1 = txBytes_diff1/tx_diff1;
				if(e.packet.user_header.qlen1 < e.table.user_slots.qlen1)
				{
					u1 = e.packet.user_header.qlen1/BT;
				}else
				{
					u1 = e.table.user_slots.qlen1/BT;
				}
				u1 = u1 + txRate1/B;
				if(u1>u){
					u = u1;
					diff = e.packet.user_header.ts1 - e.table.user_slots.ts1;
				}
				if(diff>T)
				{
					diff = T;
				}
				U = (1-diff/T)*e.table.user_slots.U + diff/T*u;
				update_table(user_slots.U,U);
				if((U >= n)||(e.table.user_slots.incStage >= maxStage))
				{
					Wc = e.table.user_slots.window/(U/n)+Wai;
					update_table(user_slots.incStage,0);
				}else
				{
					Wc = e.table.user_slots.window + Wai;
					update_table(user_slots.incStage,e.table.user_slots.incStage+1);;
				}
				update_table(credit,e.table.credit+e.packet.length+Wc-e.table.user_slots.window);
				update_table(user_slots.window,Wc);
				update_table(user_slots.lastUpdateSeq,e.table.user_slots.lastUpdateSeq + SND_NXT);
			}else
			{
				update_table(credit,e.table.credit+e.packet.length);
			}
			update_table(user_slots.txBytes0,e.packet.user_header.txBytes0);
			update_table(user_slots.ts0,e.packet.user_header.ts0);
			update_table(user_slots.qlen0,e.packet.user_header.qlen0);
			update_table(user_slots.txBytes1,e.packet.user_header.txBytes1);
			update_table(user_slots.ts1,e.packet.user_header.ts1);
			update_table(user_slots.qlen1,e.packet.user_header.qlen1);			
			e.type = Done;
			post_event(&e);
		}
	}
}