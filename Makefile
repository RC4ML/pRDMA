BUILD_DIR = build

#Device code
dev_cc = /opt/riscv/bin/riscv64-unknown-elf-gcc
dev_riscv_src_dir = ./riscv
inc_dir = ./


dev_src += ${dev_riscv_src_dir}/misc.c
dev_src += ${dev_riscv_src_dir}/print.c
dev_src += ${dev_riscv_src_dir}/start.s

dev_cflags = -march=rv32i_zicsr_zifencei -mabi=ilp32 -T link.ld -O3
dev_lib_flags = -nostdlib -lgcc


target_example_tx = example_tx
target_example_rx = example_rx
target_dcqcn_tx = dcqcn_tx
target_dcqcn_rx = dcqcn_rx
target_timely_tx = timely_tx
target_timely_rx = timely_rx
target_hpcc_tx = hpcc_tx
target_hpcc_rx = hpcc_rx
target_homa_tx = homa_tx
target_homa_rx = homa_rx
target_swift_tx = swift_tx
target_swift_rx = swift_rx
dev_target = dev_main

all : $(dev_target) $(target_example_rx) $(target_example_tx) $(target_dcqcn_tx) $(target_dcqcn_rx) $(target_timely_tx) $(target_timely_rx) $(target_hpcc_tx) $(target_hpcc_rx) $(target_homa_tx) $(target_homa_rx) $(target_swift_tx) $(target_swift_rx)

#user targets
example_tx_macro = -D TX -D EXAMPLE
example_rx_macro = -D RX -D EXAMPLE
example_src += ${dev_src}
example_src += network/simulation.c
example_src += network/network.c
example_src += protocols/example/example.c
example_src += protocols/example/example_main.c

$(target_example_tx) : $(example_src) 
	$(dev_cc) $(example_tx_macro) -o $(BUILD_DIR)/$(target_example_tx) $(dev_cflags) $(example_src) $(dev_lib_flags) -I$(inc_dir)

$(target_example_rx) : $(example_src) 
	$(dev_cc) $(example_rx_macro) -o $(BUILD_DIR)/$(target_example_rx) $(dev_cflags) $(example_src) $(dev_lib_flags) -I$(inc_dir)


#user targets
dcqcn_tx_macro = -D TX -D DCQCN
dcqcn_rx_macro = -D RX -D DCQCN
dcqcn_src += ${dev_src}
dcqcn_src += network/simulation.c
dcqcn_src += network/network.c
dcqcn_src += protocols/dcqcn/dcqcn.c
dcqcn_src += protocols/dcqcn/dcqcn_main.c

$(target_dcqcn_tx) : $(dcqcn_src) 
	$(dev_cc) $(dcqcn_tx_macro) -o $(BUILD_DIR)/$(target_dcqcn_tx) $(dev_cflags) $(dcqcn_src) $(dev_lib_flags) -I$(inc_dir)

$(target_dcqcn_rx) : $(dcqcn_src) 
	$(dev_cc) $(dcqcn_rx_macro) -o $(BUILD_DIR)/$(target_dcqcn_rx) $(dev_cflags) $(dcqcn_src) $(dev_lib_flags) -I$(inc_dir)

#user targets
timely_tx_macro = -D TX -D TIMELY
timely_rx_macro = -D RX -D TIMELY
timely_src += ${dev_src}
timely_src += network/simulation.c
timely_src += network/network.c
timely_src += protocols/timely/timely.c
timely_src += protocols/timely/timely_main.c

$(target_timely_tx) : $(timely_src) 
	$(dev_cc) $(timely_tx_macro) -o $(BUILD_DIR)/$(target_timely_tx) $(dev_cflags) $(timely_src) $(dev_lib_flags) -I$(inc_dir)

$(target_timely_rx) : $(timely_src) 
	$(dev_cc) $(timely_rx_macro) -o $(BUILD_DIR)/$(target_timely_rx) $(dev_cflags) $(timely_src) $(dev_lib_flags) -I$(inc_dir)

#user targets
hpcc_tx_macro = -D TX -D HPCC
hpcc_rx_macro = -D RX -D HPCC
hpcc_src += ${dev_src}
hpcc_src += network/simulation.c
hpcc_src += network/network.c
hpcc_src += protocols/hpcc/hpcc.c
hpcc_src += protocols/hpcc/hpcc_main.c

$(target_hpcc_tx) : $(hpcc_src) 
	$(dev_cc) $(hpcc_tx_macro) -o $(BUILD_DIR)/$(target_hpcc_tx) $(dev_cflags) $(hpcc_src) $(dev_lib_flags) -I$(inc_dir)

$(target_hpcc_rx) : $(hpcc_src) 
	$(dev_cc) $(hpcc_rx_macro) -o $(BUILD_DIR)/$(target_hpcc_rx) $(dev_cflags) $(hpcc_src) $(dev_lib_flags) -I$(inc_dir)

#user targets
homa_tx_macro = -D TX -D HOMA
homa_rx_macro = -D RX -D HOMA
homa_src += ${dev_src}
homa_src += network/simulation.c
homa_src += network/network.c
homa_src += protocols/homa/homa.c
homa_src += protocols/homa/homa_main.c

$(target_homa_tx) : $(homa_src) 
	$(dev_cc) $(homa_tx_macro) -o $(BUILD_DIR)/$(target_homa_tx) $(dev_cflags) $(homa_src) $(dev_lib_flags) -I$(inc_dir)

$(target_homa_rx) : $(homa_src) 
	$(dev_cc) $(homa_rx_macro) -o $(BUILD_DIR)/$(target_homa_rx) $(dev_cflags) $(homa_src) $(dev_lib_flags) -I$(inc_dir)

#user targets
swift_tx_macro = -D TX -D SWIFT
swift_rx_macro = -D RX -D SWIFT
swift_src += ${dev_src}
swift_src += network/simulation.c
swift_src += network/network.c
swift_src += protocols/swift/swift.c
swift_src += protocols/swift/swift_main.c

$(target_swift_tx) : $(swift_src) 
	$(dev_cc) $(swift_tx_macro) -o $(BUILD_DIR)/$(target_swift_tx) $(dev_cflags) $(swift_src) $(dev_lib_flags) -I$(inc_dir)

$(target_swift_rx) : $(swift_src) 
	$(dev_cc) $(swift_rx_macro) -o $(BUILD_DIR)/$(target_swift_rx) $(dev_cflags) $(swift_src) $(dev_lib_flags) -I$(inc_dir)	

#test target
target_test_src += ${dev_src}
target_test_src += function_test.c

$(dev_target) : $(target_test_src)
	$(dev_cc) -o $(BUILD_DIR)/$(dev_target) $(dev_cflags) $(target_test_src) $(dev_lib_flags) -I$(inc_dir)

clean : 
	cd build && rm $(target_example_tx) $(target_example_rx) $(dev_target)