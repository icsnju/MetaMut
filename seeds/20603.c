// RUN: %clang_analyze_cc1 -triple hexagon-unknown-linux -analyzer-checker=core,valist.Unterminated,valist.CopyToSelf -analyzer-output=text -verify %s
// RUN: %clang_analyze_cc1 -triple x86_64-pc-linux-gnu -analyzer-checker=core,valist.Unterminated,valist.CopyToSelf -analyzer-output=text -verify %s

// Like the compiler, the static analyzer treats some functions differently if
// they come from a system header -- for example, it is assumed that system
// functions do not arbitrarily free() their parameters, and that some bugs
// found in system headers cannot be fixed by the user and should be
// suppressed.

#pragma clang system_header

#ifdef __cplusplus
#define restrict /*restrict*/
#endif

typedef __builtin_va_list va_list;

#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap)          __builtin_va_end(ap)
#define va_arg(ap, type)    __builtin_va_arg(ap, type)
#define va_copy(dst, src)   __builtin_va_copy(dst, src)

int vprintf (const char *restrict format, va_list arg);

int vsprintf (char *restrict s, const char *restrict format, va_list arg);

int some_library_function(int n, va_list arg);

// No warning from system header.
inline void __impl_detail(int fst, ...) {
  va_list va;
  (void)va_arg(va, int);
}

void f1(int fst, ...) {
  va_list va;
  va_start(va, fst); // expected-note{{Initialized va_list}}
  return; // expected-warning{{Initialized va_list 'va' is leaked}}
  // expected-note@-1{{Initialized va_list 'va' is leaked}}
}

void f2(int fst, ...) {
  va_list va;
  va_start(va, fst); // expected-note{{Initialized va_list}}
  va_end(va); // expected-note{{Ended va_list}}
  va_start(va, fst); // expected-note{{Initialized va_list}}
} // expected-warning{{Initialized va_list 'va' is leaked}}
  // expected-note@-1{{Initialized va_list 'va' is leaked}}

void f3(int fst, ...) {
  va_list va, va2;
  va_start(va, fst);
  va_copy(va2, va); // expected-note{{Initialized va_list}}
  va_end(va); // expected-warning{{Initialized va_list 'va2' is leaked}}
  // expected-note@-1{{Initialized va_list 'va2' is leaked}}
}

void f4(va_list *fst, ...) {
  va_start(*fst, fst); // expected-note{{Initialized va_list}}
  return; // expected-warning{{Initialized va_list is leaked}}
  // expected-note@-1{{Initialized va_list is leaked}}
}

void f5(va_list fst, ...) {
  va_start(fst, fst); // expected-note{{Initialized va_list}}
} // expected-warning{{Initialized va_list}}
  // expected-note@-1{{Initialized va_list}}

void f6(va_list *fst, ...) {
  va_start(*fst, fst); // expected-note{{Initialized va_list}}
  (void)va_arg(*fst, int);
  //FIXME: this should NOT cause a warning
  va_end(*fst); // expected-warning{{Initialized va_list is leaked}}
  // expected-note@-1{{Initialized va_list is leaked}}
}

void f7(int *fst, ...) {
  va_list x;
  va_list *y = &x;
  va_start(*y,fst); // expected-note{{Initialized va_list}}
} // expected-warning{{Initialized va_list 'x' is leaked}}
  // expected-note@-1{{Initialized va_list 'x' is leaked}}

void f8(int *fst, ...) {
  va_list x;
  va_list *y = &x;
  va_start(*y,fst);
  va_end(x);
} // no-warning 

void reinit(int *fst, ...) {
  va_list va;
  va_start(va, fst); // expected-note{{Initialized va_list}}
                     // expected-note@-1{{Initialized va_list}}
  va_start(va, fst); // expected-warning{{Initialized va_list 'va' is initialized again}}
  // expected-note@-1{{Initialized va_list 'va' is initialized again}}
} // expected-warning{{Initialized va_list 'va' is leaked}}
  // expected-note@-1{{Initialized va_list 'va' is leaked}}

void reinitOk(int *fst, ...) {
  va_list va;
  va_start(va, fst);
  va_end(va);
  va_start(va, fst);
  va_end(va);
} // no-warning

void copyself(int fst, ...) {
  va_list va;
  va_start(va, fst); // expected-note{{Initialized va_list}}
  va_copy(va, va); // expected-warning{{va_list 'va' is copied onto itself}}
                   // expected-note@-1{{va_list 'va' is copied onto itself}}
  va_end(va);
}

void copyselfUninit(int fst, ...) {
  va_list va;
  va_copy(va, va); // expected-warning{{va_list 'va' is copied onto itself}}
  // expected-note@-1{{va_list 'va' is copied onto itself}}
}

void copyOverwrite(int fst, ...) {
  va_list va, va2;
  va_start(va, fst); // expected-note{{Initialized va_list}}
  va_copy(va, va2); // expected-warning{{Initialized va_list 'va' is overwritten by an uninitialized one}}
  // expected-note@-1{{Initialized va_list 'va' is overwritten by an uninitialized one}}
}

//This only generates a warning for the valist.Uninitialized checker
void copyUnint(int fst, ...) {
  va_list va, va2;
  va_copy(va, va2);
} // no-warning

void recopy(int fst, ...) {
  va_list va, va2;
  va_start(va, fst);
  va_copy(va2, va); // expected-note{{Initialized va_list}}
  va_copy(va2, va); // expected-warning{{Initialized va_list 'va2' is initialized again}}
  // expected-note@-1{{Initialized va_list 'va2' is initialized again}}
  va_end(va);
  va_end(va2);
}

void doublemsg(int fst, ...) {
  va_list va, va2;
  va_start(va, fst), va_start(va2, fst); // expected-warning{{Initialized va_list 'va' is leaked}}
  // expected-warning@-1{{Initialized va_list 'va2' is leaked}}
  // expected-note@-2{{Initialized va_list}}
  // expected-note@-3{{Initialized va_list}}
  // expected-note@-4{{Initialized va_list}}
  // expected-note@-5{{Initialized va_list 'va' is leaked}} 
} 

void in_array(int fst, ...) {
  va_list va_array[8];
  va_start(va_array[3], fst); // expected-note{{Initialized va_list}}
} // expected-warning{{Initialized va_list 'va_array[3]' is leaked}}
  // expected-note@-1{{Initialized va_list 'va_array[3]' is leaked}}

struct containing_a_valist {
  va_list vafield;
  int foobar;
};

void in_struct(int fst, ...) {
  struct containing_a_valist s;
  va_start(s.vafield, fst); // expected-note{{Initialized va_list}}
} // expected-warning{{Initialized va_list 's.vafield' is leaked}}
  // expected-note@-1{{Initialized va_list 's.vafield' is leaked}}

void casting(int fst, ...) {
  char mem[sizeof(va_list)];
  va_start(*(va_list *) mem, fst); // expected-note{{Initialized va_list}}
} // expected-warning{{Initialized va_list 'mem[0]' is leaked}}
  // expected-note@-1{{Initialized va_list 'mem[0]' is leaked}}


void castingOk(int fst, ...) {
  char mem[sizeof(va_list)];
  va_start(*(va_list *) mem, fst);
  va_end(*(va_list *) mem);
} // no-warning

