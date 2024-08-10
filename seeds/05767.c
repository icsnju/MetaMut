// RUN: %clang_analyze_cc1 -triple x86_64-pc-linux-gnu -analyzer-checker=core,valist.Uninitialized,valist.CopyToSelf -analyzer-output=text -verify %s

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

// This is called in call_inlined_uses_arg(),
// and the warning is generated during the analysis of call_inlined_uses_arg().
void inlined_uses_arg(va_list arg) {
  (void)va_arg(arg, int); // expected-warning{{va_arg() is called on an uninitialized va_list}}
  // expected-note@-1{{va_arg() is called on an uninitialized va_list}}
}

void call_inlined_uses_arg(int fst, ...) {
  va_list va;
  inlined_uses_arg(va); // expected-note{{Calling 'inlined_uses_arg'}}
}

void f6(va_list *fst, ...) {
  va_start(*fst, fst);
  (void)va_arg(*fst, int);
  va_end(*fst);
}

int va_list_get_int(va_list *va) {
  return va_arg(*va, int); // no-warning
}

struct MyVaList {
  va_list l;
};
int va_list_get_int2(struct MyVaList *va) {
  return va_arg(va->l, int); // no-warning
}

void call_vprintf_bad(int isstring, ...) {
  va_list va;
  vprintf(isstring ? "%s" : "%d", va); // expected-warning{{Function 'vprintf' is called with an uninitialized va_list argument}}
  // expected-note@-1{{Function 'vprintf' is called with an uninitialized va_list argument}}
  // expected-note@-2{{Assuming 'isstring' is 0}}
  // expected-note@-3{{'?' condition is false}}
}

void call_vsprintf_bad(char *buffer, ...) {
  va_list va;
  va_start(va, buffer); // expected-note{{Initialized va_list}}
  va_end(va); // expected-note{{Ended va_list}}
  vsprintf(buffer, "%s %d %d %lf %03d", va); // expected-warning{{Function 'vsprintf' is called with an uninitialized va_list argument}}
  // expected-note@-1{{Function 'vsprintf' is called with an uninitialized va_list argument}}
}

