cmake_minimum_required (VERSION 3.16)
project(MUSS CXX C)
include(cmake/string_to_list.cmake)
include(cmake/find_llvm.cmake)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

option(DEBUG "enable debug information")

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
  file(GLOB SOURCES ${OPT_FILES})
  add_executable(${name} ${SOURCES})
  target_include_directories(${name} PRIVATE include)
  target_link_libraries(${name} PRIVATE ${LINK_LIBS})
  target_include_directories(${name} PRIVATE
    . ${LLVM_INCLUDE_DIRS})
  target_link_directories(${name} PRIVATE
    ${LLVM_LIBRARY_DIRS})
  if (DEBUG)
    target_compile_options(${name} PRIVATE -g -DBACKWARD_HAS_DW=1)
  endif ()
  if(DEFINED OPT_OPTIONS)
    target_compile_options(${name} PRIVATE ${OPT_OPTIONS})
  endif()
  if(DEFINED OPT_DEPENDS)
    target_link_libraries(${name} PRIVATE ${OPT_DEPENDS})
  endif()
endfunction()

file(GLOB SOURCES SOURCES "lib/*.cpp" "lib/*/*.cpp")
add_library(Driver STATIC main.cpp ${SOURCES})
if (DEBUG)
  target_compile_options(Driver PRIVATE -g -DBACKWARD_HAS_DW=1)
endif ()
target_include_directories(Driver PRIVATE
  . include ${LLVM_INCLUDE_DIRS})
target_link_directories(Driver PRIVATE
  ${LLVM_LIBRARY_DIRS})
target_link_libraries(Driver PRIVATE ${LINK_LIBS} dw)

add_muss_executable(muss
  FILES "mu*/*.cpp" "mu*/*.cpp" DEPENDS Driver)
add_muss_executable(mu.s
  FILES "mu.s/*.cpp" "mu.s/*.cpp" DEPENDS Driver)
add_muss_executable(mu.u
  FILES "mu.u/*.cpp" "mu.u/*.cpp" DEPENDS Driver)
