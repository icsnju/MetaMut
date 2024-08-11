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
  set(multiValueArgs FILES INCLUDES DEPENDS OPTIONS)
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

function(add_muss_library name)
  set(singleValueArgs "")
  set(multiValueArgs FILES INCLUDES DEPENDS OPTIONS)
  cmake_parse_arguments(OPT ""
    "${singleValueArgs}" "${multiValueArgs}" ${ARGV})
  file(GLOB SOURCES ${OPT_FILES})
  add_library(${name} STATIC ${SOURCES})
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

add_muss_library(Driver FILES main.cpp "lib/*.cpp" "lib/*/*.cpp")
add_muss_library(mu.s.lib FILES "mu.s/*.cpp" "mu.s/*.cpp")
add_muss_library(mu.u.lib FILES "mu.u/*.cpp" "mu.u/*.cpp")
add_muss_library(mu.u2.lib FILES "mu.u.v2/*.cpp" "mu.u.v2/*.cpp")

file(WRITE ${CMAKE_BINARY_DIR}/dummy.cpp "")
add_muss_executable(muss
  FILES ${CMAKE_BINARY_DIR}/dummy.cpp
  DEPENDS Driver mu.s.lib mu.u.lib mu.u2.lib)
add_muss_executable(mu.s
  FILES ${CMAKE_BINARY_DIR}/dummy.cpp
  DEPENDS Driver mu.s.lib)
add_muss_executable(mu.u
  FILES ${CMAKE_BINARY_DIR}/dummy.cpp
  DEPENDS Driver mu.u.lib)
add_muss_executable(mu.u2
  FILES ${CMAKE_BINARY_DIR}/dummy.cpp
  DEPENDS Driver mu.u2.lib)
