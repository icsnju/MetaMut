cmake_minimum_required (VERSION 3.16)
project(MUSS CXX C)
include(cmake/string_to_list.cmake)
include(cmake/find_llvm.cmake)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(LINK_LIBS
  clangTooling
  clangToolingASTDiff
  clangToolingRefactoring
  clangToolingCore
  clangASTMatchers
  clangFrontend
  clangDriver
  clangSerialization
  clangTransformer
  clangRewrite
  clangFormat
  clangToolingInclusions
  clangParse
  clangSema
  clangAnalysis
  clangEdit
  clangAST
  clangLex
  clangBasic
  clang
  LLVM)

file(GLOB_RECURSE SOURCES "lib/*.cpp")
add_executable(muss ${SOURCES} main.cpp)
target_include_directories(muss PRIVATE include)
target_link_libraries(muss PRIVATE ${LINK_LIBS})
target_include_directories(muss PRIVATE
  . ${LLVM_INCLUDE_DIRS})
target_link_directories(muss PRIVATE
  ${LLVM_LIBRARY_DIRS})
target_compile_options(muss PRIVATE -ggdb3 -w -fmax-errors=3)
