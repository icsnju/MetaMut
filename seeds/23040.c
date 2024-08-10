// RUN: %clang_cc1 -fsyntax-only -Wno-unused-value -fno-diagnostics-show-line-numbers %s 2>&1 | FileCheck %s
// REQUIRES: asserts

void foo(void) {

  ""; // 
// CHECK: {{^  "<A7><C3>"; // <F8>}}
// CHECK: {{^   \^~~~~~~}}

  /*  */ const char *d = "";

// CHECK: {{^  /\* <FE><AB> \*/ const char \*d = "<A5>";}}
// CHECK: {{^                                  \^~~~}}

  "xx鿿d";
// CHECK: {{^  "xx鿿<BF>d";}}
// CHECK: {{^             \^~~~}}

  "xxbcd";
// CHECK: {{^  "xx<E9><BF>bcd";}}
// CHECK: {{^     \^~~~~~~~}}

  "xxabcd";
// CHECK: {{^  "xx<E9>abcd";}}
// CHECK: {{^     \^~~~}}

  "xxd";
// CHECK: {{^  "xx<E9><BF><E9><BF>d";}}
// CHECK: {{^     \^~~~~~~~~~~~~~~}}

  "xxxxxxxxxxxxxxxxxxxxxxxxx";
// CHECK: {{^  "xx<E9><BF>xxxxxxxxxxxxxxxxxxxxx<E9><BF>xx";}}
// CHECK: {{^     \^~~~~~~~                     ~~~~~~~~}}

  "?kSg7,	2,Du*,p&(K:'1jOŰ<:";

  "xxxxxxxxxxxxx";
}
// CHECK-NOT:Assertion
