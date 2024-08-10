#!/bin/bash

CC=$(pwd)/../output/afl-bins/afl-gcc-fast
CXX=$(pwd)/../output/afl-bins/afl-g++-fast

if ! [ -f $CC ] || ! [ -f $CXX ]; then
  echo -e "NOTE: $CC or $CXX not found"
  echo -e "\033[1;31mNOTE: Please compile instrumenter firstly\033[1;0m"
  exit 1
fi

make clean
../scripts/server.py --bbmap-dir bbmap_dir make CC=$CC CXX=$CXX

echo "===== AFL_DUMP ===="
echo NBRS: $(AFL_PRINT_NBRS=1 ./a.out)
echo BBMAP_DIR: $(AFL_PRINT_BBMAP_DIR=1 ./a.out)

echo "===== SHOWMAP ===="
../scripts/showmap.py --show-bb --bbmap-dir bbmap_dir -- make run

make clean
rm -rf a.out configs.json bbmap_dir
