#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "riscv/recursive_macro.h"
#include "protocols/example/example.h"


typedef enum{
	reserve0 = 0x0,
    RC_WRITE_FIRST = 0x06,
    RC_WRITE_MIDDLE = 0x07,
    RC_WRITE_LAST = 0x08,
    RC_WRITE_LAST_WITH_IMD = 0x09,
    RC_WRITE_ONLY = 0x0A,
    RC_WRITE_ONLY_WIT_IMD = 0x0B,
    RC_READ_REQUEST = 0x0C,
    RC_READ_RESP_FIRST = 0x0D,
    RC_READ_RESP_MIDDLE = 0x0E,
    RC_READ_RESP_LAST = 0x0F,
    RC_READ_RESP_ONLY = 0x10,
    RC_ACK = 0x11,  
    CNP = 0x81,
    CE = 0x82,
    reserve = 0xFF,
}PkgType;

typedef enum {
	GenTxEvent,
	Send,
	LoopBack,
	Drop,
}EventType;

typedef struct{
	uint credit;
	uint rate;
	Timer timer;
	#ifdef UserSlots
		UserSlots user_slots;
	#endif
}CCTable;

typedef struct {
	int qpn;
	int type;
	int length;
	#ifdef UserHeader
		UserHeader user_header;
	#endif
}PacketMeta;

typedef struct{
	CCTable table;
	PacketMeta packet;
	EventType type;
}Event;

#define BaseMetaSize 6  /*CCTable:credit, rate, timer; PacketMeta: qpn, type, length*/

CompileTimeAssert(\
	(BaseMetaSize+UserMetaSize)*sizeof(uint) == \
	(sizeof(CCTable)+sizeof(PacketMeta))\
	);

CompileTimeAssert(UserMetaSize<REPEAT_MAX);

static uint event_count = 0;
static inline void post_event(Event* e){
	WriteCSR(CSR_EVENT_TYPE, e->type);
	event_count++;
	WriteCSR(CSR_CORE_EVENT_COUNT, event_count);
};

CompileTimeAssert(BaseMetaSize<REPEAT_MAX);

inline void _read_from_csr(Event* e){
	uint* p = (uint*)e;
	#define ExpBase(i, _) p[i]=ReadCSR(CSR_CCTABLE_START+i);
	EVAL(REPEAT(BaseMetaSize, ExpBase, ~));
	#define ExpUser(i, _) p[BaseMetaSize+i]=ReadCSR(CSR_CCTABLE_START+BaseMetaSize+i);
	EVAL(REPEAT(UserMetaSize, ExpUser, ~));
}

#define update_table(MEMBER, value) \
	WriteCSR(\
	(__builtin_offsetof(CCTable, MEMBER)/sizeof(int))+CSR_CCTABLE_START,\
	value\
	)

static inline int poll_event_async(Event* e){
	uint res = 0;
	if(SwapCSR(CSR_HAS_EVENT,res)){
		_read_from_csr(e);
	}
	return res;
};

static inline void poll_event_sync(Event* e){
	uint res = 0;
	while(!SwapCSR(CSR_HAS_EVENT,res)){
	}
	_read_from_csr(e);
};

static inline int has_data(PkgType type){
	return (RC_WRITE_FIRST<=type)&&(type<=RC_READ_RESP_ONLY);
};

#endif