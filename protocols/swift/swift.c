#include "swift.h"
#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "network/network.h"

#define host_clock 10
#define T 512 //base RTT
#define h 12 //per hop scaling factor
#define min_w 1000 //min cwnd for target scaling,
#define max_w 10000 //max cwnd for target scaling
#define range 1 //max scaling range
 
int send()
{
	Timer now = GetTime();
	int credit,seq=0;
	Event e;
	while(1){
		poll_event_sync(&e);
		 if(has_data(e.packet.type))
		 {
			while(e.table.credit >= e.packet.length)
			{
			};
				e.type = Send;
				credit -= e.packet.length;
				update_table(credit,credit);
		}
		else
		{
			e.type = Send;
		}
		post_event(&e);
	}
}


int recv()
{
	int Wc;
	const int Wai = 10;
	float target_delay,max_mdf = 0.05,alfa,belta;
	int new_rtt, can_decrease = 0,num_acked;
	Event e;
	alfa = range/(1.0/min_w+1.0/max_w) ;
	belta = -alfa/max_w;
	while(1){
		poll_event_sync(&e);
		if(e.packet.type == RC_ACK)
		{
			new_rtt = GetTime() - e.packet.user_header.ts + e.packet.user_header.qlen * host_clock;
			if((GetTime()-e.table.user_slots.t_last_decrease)>=new_rtt)
			{
				can_decrease = 1;
			}
			if(alfa/e.table.user_slots.window+belta > range)
			{
				if(range > 0)
				{
					target_delay = T + e.packet.user_header.ttl*h + range;
				}
				else
				{
					target_delay = T + e.packet.user_header.ttl*h;
				}
			}
			else
			{
				if(alfa/e.table.user_slots.window+belta > 0)
				{
					target_delay = T + e.packet.user_header.ttl*h + alfa/e.table.user_slots.window+belta;
				}
				else
				{
					target_delay = T + e.packet.user_header.ttl*h;
				}				
			}
			if(new_rtt < target_delay)
			{
				if(e.table.user_slots.window>=1)
				{
					Wc = e.table.user_slots.window + Wai/e.table.user_slots.window * e.table.user_slots.num_acked;
				}
				else
				{
					Wc = e.table.user_slots.window + Wai * e.table.user_slots.num_acked;
				}
				update_table(user_slots.window,Wc);
			}
			else
			{
				if(can_decrease == 1)
				{
					if((1-belta*(new_rtt-target_delay)/new_rtt)>(1-max_mdf))
					{
						Wc = e.table.user_slots.window*(1-belta*(new_rtt-target_delay)/new_rtt);
					}
					else
					{
						Wc = e.table.user_slots.window*(1-max_mdf);
					}
				}
				update_table(user_slots.window,Wc);
			}
			if(Wc<=e.table.user_slots.window)
			{
				update_table(user_slots.t_last_decrease,GetTime());
			}
			update_table(user_slots.num_acked,e.table.user_slots.num_acked + 1);		
		}
	}
}