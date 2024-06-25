cmake_minimum_required (VERSION 3.16)
project(afl-cc CXX C)

include(cmake/string_to_list.cmake)
include(cmake/find_llvm.cmake)
include(cmake/proj.cmake)

set(NEEDED_LLVM_VARS
  LLVM_PACKAGE_VERSION
  LLVM_VERSION_MAJOR
  LLVM_VERSION_MINOR
  LLVM_VERSION_PATCH
  LLVM_DEFINITIONS
  LLVM_ENABLE_ASSERTIONS
  LLVM_ENABLE_EH
  LLVM_ENABLE_RTTI
  LLVM_INCLUDE_DIRS
  LLVM_LIBRARY_DIRS
  LLVM_TOOLS_BINARY_DIR
  LLVM_ENABLE_VISIBILITY_INLINES_HIDDEN
  TARGET_TRIPLE
)

# gcc plugin
# sudo apt install gcc-11-plugin-dev
execute_process(
  COMMAND ${CMAKE_C_COMPILER} -print-file-name=plugin
  OUTPUT_VARIABLE GCC_PLUGIN_DIR)
string(STRIP ${GCC_PLUGIN_DIR} GCC_PLUGIN_DIR)


set(AFL_PATH ${CMAKE_BINARY_DIR}/afl-bins)
set(AFL_DEFINES
  -DAFL_PATH=\"${AFL_PATH}\"
  -DBIN_PATH=\"/usr/local/bin\"
  -DLLVM_BINDIR=\"${LLVM_TOOLS_BINARY_DIR}\"
  -DVERSION=\"++4.07a\"
  -DLLVM_LIBDIR=\"${LLVM_LIBRARY_DIRS}\"
  -DLLVM_VERSION=\"${LLVM_PACKAGE_VERSION}\"
  -DAFL_CLANG_FLTO=\"-flto=full\"
  -DAFL_REAL_LD=\"${LLVM_TOOLS_BINARY_DIR}/ld.lld\"
  -DAFL_CLANG_LDPATH=\"1\"
  -DAFL_CLANG_FUSELD=\"1\"
  -DCLANG_BIN=\"${LLVM_TOOLS_BINARY_DIR}/clang\"
  -DCLANGPP_BIN=\"${LLVM_TOOLS_BINARY_DIR}/clang++\"
  -DUSE_BINDIR=1
  -DLLVM_MINOR=${LLVM_VERSION_MINOR}
  -DLLVM_MAJOR=${LLVM_VERSION_MAJOR}
)

add_proj_program(afl-cc-bin
  FILES src/afl-cc.c src/afl-common.c
  INCLUDES include ${LLVM_INCLUDE_DIRS}
  OPTIONS -ggdb3 ${AFL_DEFINES})

add_proj_program(afl-ld-lto
  FILES src/afl-ld-lto.c
  INCLUDES include ${LLVM_INCLUDE_DIRS}
  OPTIONS -ggdb3 ${AFL_DEFINES})

add_proj_library(afl-compiler-rt
  INCLUDES include
  FILES instrumentation/afl-compiler-rt.o.c
  OPTIONS -O3)

add_proj_library(afl-gcc-pass
  INCLUDES include
  FILES instrumentation/afl-gcc-pass.so.cc
  TYPE SHARED
  INCLUDES ${GCC_PLUGIN_DIR}/include
  OPTIONS -O3 -fno-rtti)

set(OFILES
  ${AFL_PATH}/dynamic_list.txt
  ${AFL_PATH}/afl-gcc-fast
  ${AFL_PATH}/afl-g++-fast
  ${AFL_PATH}/afl-ld-lto
  ${AFL_PATH}/afl-compiler-rt.o
  ${AFL_PATH}/afl-gcc-pass.so
)
add_custom_command(
  OUTPUT ${OFILES}
  COMMAND mkdir -p ${AFL_PATH}
  COMMAND ln -s $<TARGET_FILE:afl-cc-bin> ${AFL_PATH}/afl-gcc-fast
  COMMAND ln -s $<TARGET_FILE:afl-cc-bin> ${AFL_PATH}/afl-g++-fast
  COMMAND ln -s $<TARGET_FILE:afl-ld-lto> ${AFL_PATH}/afl-ld-lto
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_list.txt ${AFL_PATH}
  COMMAND ln -s $<TARGET_FILE:afl-compiler-rt> ${AFL_PATH}/afl-compiler-rt.o
  COMMAND ln -s $<TARGET_FILE:afl-gcc-pass> ${AFL_PATH}/afl-gcc-pass.so
  )

add_custom_target(afl-bins ALL DEPENDS 
  ${OFILES}
  ${AFL_PATH}/server.py
  ${AFL_PATH}/fixelf.py
)

configure_file(scripts/server.py ${AFL_PATH}/server.py)
configure_file(scripts/fixelf.py ${AFL_PATH}/fixelf.py)
