#ifndef __DCQCN_H__
#define __DCQCN_H__

#include "riscv/misc.h"

typedef struct{
	uint slot1;
	uint slot2;
}ExampleUserSlots;

typedef struct{
	uint field1;
	uint field2;
	uint field3;
}ExampleUserHeader;

#define UserSlots ExampleUserSlots
#define UserHeader ExampleUserHeader

#define UserMetaSize 5

#endif