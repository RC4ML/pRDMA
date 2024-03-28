BUILD_DIR = build

#Device code
dev_cc = /opt/riscv/bin/riscv64-unknown-elf-gcc
dev_src_dir = ./riscv
dev_target = dev_main

dev_src += ${dev_src_dir}/misc.c
dev_src += ${dev_src_dir}/print.c
dev_src += ${dev_src_dir}/main.c
dev_src += ${dev_src_dir}/start.s


dev_cflags = -march=rv32i_zicsr_zifencei -mabi=ilp32 -T link.ld 
dev_lib_flags = -nostdlib -lgcc
$(dev_target) : $(dev_src)
	$(dev_cc) -o $(BUILD_DIR)/$(dev_target) $(dev_cflags) $(dev_src) $(dev_lib_flags)
