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
dev_target = dev_main

all : $(dev_target) $(target_example_rx) $(target_example_tx)

#user targets
example_tx_src += ${dev_src}
example_tx_src += protocols/example/example.c
example_tx_src += protocols/example/example_tx.c

$(target_example_tx) : $(example_tx_src) 
	$(dev_cc) -o $(BUILD_DIR)/$(target_example_tx) $(dev_cflags) $(example_tx_src) $(dev_lib_flags) -I$(inc_dir)


example_rx_src += ${dev_src}
example_rx_src += protocols/example/example.c
example_rx_src += protocols/example/example_rx.c

$(target_example_rx) : $(example_rx_src) 
	$(dev_cc) -o $(BUILD_DIR)/$(target_example_rx) $(dev_cflags) $(example_rx_src) $(dev_lib_flags) -I$(inc_dir)


#test target
target_test_src += ${dev_src}
target_test_src += function_test.c

$(dev_target) : $(target_test_src)
	$(dev_cc) -o $(BUILD_DIR)/$(dev_target) $(dev_cflags) $(target_test_src) $(dev_lib_flags) -I$(inc_dir)

clean : 
	cd build && rm $(target_example_tx) $(target_example_rx) $(dev_target)