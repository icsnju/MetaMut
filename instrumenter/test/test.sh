#!/bin/bash

CC=$(pwd)/../output/afl-bins/afl-gcc-fast
CXX=$(pwd)/../output/afl-bins/afl-g++-fast
make clean
../scripts/server.py --bbmap-dir bbmap_dir make CC=$CC CXX=$CXX
../scripts/fixelf.py -i configs.json ./a.out

echo "===== AFL_DUMP ===="
echo NBRS: $(AFL_PRINT_NBRS=1 ./a.out)
echo BBMAP_DIR: $(AFL_PRINT_BBMAP_DIR=1 ./a.out)

echo "===== SHOWMAP ===="
../scripts/showmap.py --show-bb --bbmap-dir bbmap_dir -- make run

make clean
rm -rf a.out configs.json bbmap_dir
