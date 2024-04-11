#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "riscv/misc.h"
#include "riscv/encoding.h"
#include "riscv/recursive_macro.h"

#if defined EXAMPLE
	#include "protocols/example/example.h"
#elif defined DCQCN
	#include "protocols/dcqcn/dcqcn.h"
#else
	#include "protocols/example/example.h"
#endif

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
	GenTxEvent,/*RX CC core use*/
	Send,/*TX CC core use*/
	LoopBack,
	Done,
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

#define BaseTableSize 3  /*CCTable:credit, rate, timer*/
#define BasePacketSize 3 /*PacketMeta: qpn, type, length*/

CompileTimeAssert(\
	(BaseTableSize+BasePacketSize)*sizeof(uint) == \
	(sizeof(CCTable)+sizeof(PacketMeta))\
	);

CompileTimeAssert((BaseTableSize+UserTableSize)*sizeof(uint) == sizeof(CCTable));
CompileTimeAssert((BasePacketSize+UserPacketSize)*sizeof(uint) == sizeof(CCTable));

CompileTimeAssert(BaseTableSize<REPEAT_MAX);
CompileTimeAssert(BasePacketSize<REPEAT_MAX);
CompileTimeAssert(UserTableSize<REPEAT_MAX);
CompileTimeAssert(UserPacketSize<REPEAT_MAX);

static uint event_count = 0;
static inline void post_event(Event* e){
	WriteCSR(CSR_EVENT_TYPE, e->type);
	event_count++;
	WriteCSR(CSR_PROCESSED_EVENT_COUNT, event_count);
};

static inline void _write_to_csr_table(CCTable* table){
	uint* p = (uint*)table;
	#define ExpBase(i, _) WriteCSR(CSR_CCTABLE_START+i, p[i]);
	EVAL(REPEAT(BaseTableSize, ExpBase, ~));
	#define ExpUser(i, _) WriteCSR(CSR_CCTABLE_START+BaseTableSize+i, p[BaseTableSize+i]);
	EVAL(REPEAT(UserTableSize, ExpUser, ~));
	#undef ExpBase
	#undef ExpUser
}

static inline void _write_to_csr_packet(PacketMeta* packet){
	uint* p = (uint*)packet;
	#define ExpBase(i, _) WriteCSR(CSR_CCTABLE_START+sizeof(CCTable)+i, p[i]);
	EVAL(REPEAT(BasePacketSize, ExpBase, ~));
	#define ExpUser(i, _) WriteCSR(CSR_CCTABLE_START+sizeof(CCTable)+i, p[BasePacketSize+i]);
	EVAL(REPEAT(UserPacketSize, ExpUser, ~));
	#undef ExpBase
	#undef ExpUser
}

static inline void _read_from_csr_table(CCTable* table){
	uint* p = (uint*)table;
	#define ExpBase(i, _) p[i]=ReadCSR(CSR_CCTABLE_START+i);
	EVAL(REPEAT(BaseTableSize, ExpBase, ~));
	#define ExpUser(i, _) p[BaseTableSize+i]=ReadCSR(CSR_CCTABLE_START+BaseTableSize+i);
	EVAL(REPEAT(UserTableSize, ExpUser, ~));
	#undef ExpBase
	#undef ExpUser
}

static inline void _read_from_csr_packet(PacketMeta* packet){
	uint* p = (uint*)packet;
	#define ExpBase(i, _) p[i]=ReadCSR(CSR_CCTABLE_START+sizeof(CCTable)+i);
	EVAL(REPEAT(BasePacketSize, ExpBase, ~));
	#define ExpUser(i, _) p[BasePacketSize+i]=ReadCSR(CSR_CCTABLE_START+sizeof(CCTable)+BasePacketSize+i);
	EVAL(REPEAT(UserPacketSize, ExpUser, ~));
	#undef ExpBase
	#undef ExpUser
}

static inline void _read_from_csr(Event* e){
	_read_from_csr_table(&(e->table));
	_read_from_csr_packet(&(e->packet));
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