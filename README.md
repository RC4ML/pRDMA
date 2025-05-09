## Introduction

The compilation environment for programs running on the RISC-V processor embedded in the SwRDMA engine. The ./protocols directory contains six sample CCA programs. Developers can implement their own CCAs by programming in the ./protocols directory.

## Install RISC-V Compilation Toolchain

Pull RISC-V GNU Toolchain Submodule

~~~bash
git submodule update --init --recursive
~~~

Install Dependency Libraries

~~~bash
sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
~~~

Configure the Build

~~~bash
cd riscv-gnu-toolchain/
./configure --prefix=/opt/riscv
make
~~~

Add these lines to your **~/.bashrc** file:

~~~bash
export RISCV="/opt/riscv"
export PATH=$PATH:$RISCV/bin
~~~

Verify Installation

~~~bash
riscv64-unknow-elf-gcc --version
~~~

## Compiling RISC-V Programs

~~~bash
mkdir build
mkdir gen
make
~~~

## Converting RISC-V Programs to MEM Files

Convert compiled files to binary(.bin) format:

~~~bash
riscv64-unknown-elf-objcopy -O binary build/example_rx build/example_rx_bin
riscv64-unknown-elf-objcopy -O binary build/example_tx build/example_tx_bin
~~~

Convert binary(.bin) files to text(.txt) format:

~~~bash
hexdump -C build/example_rx_bin > gen/inst_rx.txt
hexdump -C build/example_tx_bin > gen/inst_tx.txt
~~~

Convert text(.txt) files to MEM(.mem) format (for FPGA):

~~~bash
cd gen
python3 test.py
~~~

### After generating the MEM(.mem) file, we load it into the RISC-V instruction memory (on-chip RAM of the FPGA) and then generate the FPGA bitstream.
