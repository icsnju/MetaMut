

#ifndef TARGETED_TOP_H
#define TARGETED_TOP_H


extern int NestedVar1;

enum {
  VALUE = 3
};

extern int TopVar;

typedef struct {
  int x;
  int y;

  int z;
  int w;
} Vector;

static inline int vector_get_x(Vector v) {
  int x = v.x;
  return x;
}

#endif

// This tests that we will correctly error out on the deserialized decl.

// RUN: export LSAN_OPTIONS=detect_leaks=0
// RUN: c-index-test -write-pch %t.h.pch %S/targeted-top.h
// RUN: env CINDEXTEST_FAILONERROR=1 not c-index-test -cursor-at=%S/targeted-nested1.h:2:16 %s -include %t.h \
// RUN:    -Xclang -error-on-deserialized-decl=NestedVar1
// RUN: env CINDEXTEST_FAILONERROR=1 not c-index-test -cursor-at=%S/targeted-nested1.h:2:16 %s -include %t.h \
// RUN:    -Xclang -error-on-deserialized-decl=NestedVar1 2>&1 \
// RUN:  | FileCheck %s

// CHECK: error: 'NestedVar1' was deserialized
