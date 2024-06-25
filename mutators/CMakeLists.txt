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

function(add_muss_executable name)
  set(singleValueArgs "")
  set(multiValueArgs FILES INCLUDES INCLUDES_BEFORE DEPENDS OPTIONS)
  cmake_parse_arguments(OPT ""
    "${singleValueArgs}" "${multiValueArgs}" ${ARGV})
  file(GLOB SOURCES ${OPT_FILES} "lib/*.cpp" "lib/*/*.cpp")
  add_executable(${name} ${SOURCES})
  target_include_directories(${name} PRIVATE include)
  target_link_libraries(${name} PRIVATE ${LINK_LIBS})
  target_include_directories(${name} PRIVATE
    . ${LLVM_INCLUDE_DIRS})
  target_link_directories(${name} PRIVATE
    ${LLVM_LIBRARY_DIRS})
  if(DEFINED OPT_OPTIONS)
    target_compile_options(${name} PRIVATE ${OPT_OPTIONS})
  endif()
endfunction()

add_muss_executable(muss
  FILES main.cpp "mu*/*.cpp" "mu*/*.cpp")
add_muss_executable(mu.s
  FILES main.cpp "mu.s/*.cpp" "mu.s/*.cpp")
add_muss_executable(mu.u
  FILES main.cpp "mu.u/*.cpp" "mu.u/*.cpp")
