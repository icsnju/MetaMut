// RUN: %clang_cc1 -verify -std=c99 -Wunknown-pragmas -pedantic %s -fsyntax-only
// RUN: %clang_cc1 -verify -std=c99 -Wunknown-pragmas -pedantic %s -fsyntax-only -DGCC
// RUN: %clang_cc1 -verify -std=c99 -Wunknown-pragmas -pedantic %s -fsyntax-only -DCLANG
// RUN: %clang_cc1 -verify -std=c99 -Wunknown-pragmas -pedantic %s -fsyntax-only -fms-extensions -DMS

#if defined(CLANG)
#pragma clang system_header
// expected-no-diagnostics
#elif defined(GCC)
#pragma GCC system_header
// expected-no-diagnostics
#elif defined(MS)
#pragma system_header
// expected-no-diagnostics
#else
// expected-warning@+1{{unknown pragma ignored}}
#pragma system_header

// expected-note@+4{{previous definition is here}}
// expected-warning@+4{{redefinition of typedef 'x' is a C11 feature}}
#endif

typedef int x;
typedef int x;

// RUN: %clang_cc1 -E %s | FileCheck %s
// PR9861: Verify that line markers are not messed up in -E mode.
// CHECK: # 1 "{{.*}}pragma_sysheader.h" 1
// CHECK-NEXT: # 12 "{{.*}}pragma_sysheader.h"
// CHECK: typedef int x;
// CHECK: typedef int x;
// CHECK-NEXT: # 7 "{{.*}}pragma_sysheader.c" 2
