#include "riscv/misc.h"
#include "network/network.h"

int global_x;
int global_y = 0;
int global_z = 100;

void main() {
	int regValue = 1;
	WriteCSR(0x90,2);

	regValue = SwapCSR(0x90,regValue);

	int v = ReadCSR(0x90);
	print("regValue:%d\n",regValue);
	print("csr:%d\n",v);

	print("123");
	print("456");
	print("789");
	print("\n");
	int value = -100;
	print("String:%s, Dec:%d, Hex:%X, hex:%x default:%%\n","str", value, value, value);

	char buffer[128];
    sprint(buffer, "Decimal:%d, hex:%x, HEX:%X\n", 0xF0000000, 0x80000000, 42);
	print(buffer);
	print("%d\n", __builtin_offsetof(Event,table.credit));
	update_table(credit, 1);
	update_table(rate, 2);
	update_table(timer, 3);

	print("cycle:%d\n",GetTime());
	print("cycle:%d\n",GetTime());
	print("cycle:%d\n",GetTime());

	void* ret = alloc(100);
	global_x = 10;
	global_y = 20;
	global_z = 30;
	trap("end of execution");

    while(1);
}
