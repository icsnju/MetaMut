#!/bin/bash

set -v
set -e

# configurations
: "${CC_DWNDIR:=$(pwd)/compilers}"
: "${CC_OBJDIR:=$(pwd)/objects}"
: "${JOBS:=4}"

# other variables
PROJ_HOME=$(pwd)
CFORGE_OBJDIR=${PROJ_HOME}/output
INSTRUM_PATH=$PROJ_HOME/instrumenter/output/afl-bins
LLVM_VERSION=$(llvm-config --version | grep -oE '([0-9]+)\.[0-9]+\.[0-9]+' | awk -F. '{print $1}')

mkdir -p $CC_DWNDIR
mkdir -p $CC_OBJDIR
mkdir -p $CFORGE_OBJDIR

# compile afl cc wrappers
cd $PROJ_HOME/instrumenter
mkdir -p output; cd output
cmake .. -DCMAKE_C_COMPILER=gcc-11 -DCMAKE_CXX_COMPILER=g++-11
make -j4

# download gcc source
if ! [ -d $CC_DWNDIR/gcc-project-trunk ]; then
  cd $CC_DWNDIR
  echo "[*] downloading gcc's source code"
  git clone --depth=1 git://gcc.gnu.org/git/gcc.git gcc-project-trunk
fi

# gcc-project-trunk.obj
GCCTK_BIN=$CC_OBJDIR/gcc-project-trunk.obj/install/bin/gcc
if ! [ -x $GCCTK_BIN ]; then
  cd $CC_OBJDIR
  mkdir -p gcc-project-trunk.obj
  cd gcc-project-trunk.obj
  echo "[*] compiling gcc without instrumenter"
  $CC_DWNDIR/gcc-project-trunk/configure \
    --prefix=$(pwd)/install \
    --enable-languages="c,c++" \
    --host=x86_64-linux --target=x86_64-linux \
    --disable-bootstrap --disable-multilib
  make -j $JOBS
  make install
fi
GCCTK_VER=$($GCCTK_BIN --version | grep -oE '([0-9]+)\.[0-9]+\.[0-9]+' | head -n1 | awk -F. '{print $1}')

# gcc-project-trunk.instrum
INSTRUM_GCCTK_BIN=$CC_OBJDIR/gcc-project-trunk.instrum/install/bin/gcc
if ! [ -x $INSTRUM_GCCTK_BIN ]; then
  cd $CC_OBJDIR
  mkdir -p gcc-project-trunk.instrum
  cd gcc-project-trunk.instrum
  echo "[*] compiling gcc with instrumenter"
  $CC_DWNDIR/gcc-project-trunk/configure \
    --prefix=$(pwd)/install \
    --enable-languages="c,c++" \
    --host=x86_64-linux --target=x86_64-linux \
    --disable-bootstrap --disable-multilib \
    CC=$INSTRUM_PATH/afl-gcc-fast \
    CXX=$INSTRUM_PATH/afl-g++-fast \
    LD=$INSTRUM_PATH/afl-ld-lto
  python3 $INSTRUM_PATH/server.py -o configs.json -- make -j $JOBS
  cp ../gcc-project-trunk.obj/gcc/liblto_plugin.so gcc/liblto_plugin.so
  python3 $INSTRUM_PATH/server.py -o configs.json -- make -j $JOBS
  python3 $INSTRUM_PATH/server.py -- make install

  NBRS=$(cat basic-block-maps/* | wc -l)
  sed -i "s/\(instrum_gcc_nbrs = \)[0-9]\+/\1$NBRS/" $PROJ_HOME/fuzzer/configs.py
fi

# download llvm source
if ! [ -d $CC_DWNDIR/llvm-project-trunk ]; then
  cd $CC_DWNDIR
  echo "[*] downloading llvm's source code"
  git clone --depth=1 git@github.com:llvm/llvm-project llvm-project-trunk
fi

# llvm-project-trunk.obj
CLANGTK_BIN=$CC_OBJDIR/llvm-project-trunk.obj/bin/clang
if ! [ -x $CLANGTK_BIN ]; then
  cd $CC_OBJDIR
  mkdir -p llvm-project-trunk.obj
  cd llvm-project-trunk.obj
  echo "[*] compiling llvm+clang without instrumenter"
  cmake $CC_DWNDIR/llvm-project-trunk/llvm \
    -DLLVM_TOOL_CLANG_BUILD=ON \
    -DLLVM_ENABLE_PROJECTS="clang;polly" \
    -DCMAKE_BUILD_TYPE="Release" \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    # -DBUILD_SHARED_LIBS=ON \
    # -DLLVM_BUILD_LLVM_DYLIB=ON
  make clang -j $JOBS
fi
CLANGTK_VER=$($CLANGTK_BIN --version | grep -oE '([0-9]+)\.[0-9]+\.[0-9]+' | head -n1 | awk -F. '{print $1}')

# llvm-project-trunk.instrum
INSTRUM_CLANGTK_BIN=$CC_OBJDIR/llvm-project-trunk.instrum/bin/clang
if ! [ -x $INSTRUM_CLANGTK_BIN ]; then
  cd $CC_OBJDIR
  mkdir -p llvm-project-trunk.instrum
  cd llvm-project-trunk.instrum
  echo "[*] compiling llvm+clang with instrumenter"
  cmake $CC_DWNDIR/llvm-project-trunk/llvm \
    -DLLVM_TOOL_CLANG_BUILD=ON \
    -DLLVM_ENABLE_PROJECTS="clang;polly" \
    -DCMAKE_BUILD_TYPE="Release" \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DCMAKE_C_COMPILER=$INSTRUM_PATH/afl-gcc-fast \
    -DCMAKE_CXX_COMPILER=$INSTRUM_PATH/afl-g++-fast \
    # -DBUILD_SHARED_LIBS=ON \
    # -DLLVM_BUILD_LLVM_DYLIB=ON
  python3 $INSTRUM_PATH/server.py -o configs.json -- make clang -j $JOBS
  NBRS=$(cat basic-block-maps/* | wc -l)
  sed -i "s/\(instrum_clang_nbrs = \)[0-9]\+/\1$NBRS/" $PROJ_HOME/fuzzer/configs.py
fi
