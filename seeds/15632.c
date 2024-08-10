// RUN: %clang_analyze_cc1 -analyzer-output=text -verify %s \
// RUN:   -analyzer-checker=core \
// RUN:   -analyzer-checker=unix.Malloc \
// RUN:   -analyzer-checker=debug.ExprInspection \
// RUN:   -analyzer-config core.CallAndMessage:ArgPointeeInitializedness=true

void clang_analyzer_warnIfReached(void);

// Passing uninitialized const data to function
// Like the compiler, the static analyzer treats some functions differently if
// they come from a system header -- for example, it is assumed that system
// functions do not arbitrarily free() their parameters, and that some bugs
// found in system headers cannot be fixed by the user and should be
// suppressed.
#pragma clang system_header

#ifdef __cplusplus
#define restrict /*restrict*/
#endif

typedef __typeof(sizeof(int)) size_t;
typedef long long __int64_t;
typedef __int64_t __darwin_off_t;
typedef __darwin_off_t fpos_t;

typedef struct _FILE FILE;
#define SEEK_SET 0 /* Seek from beginning of file. */
#define SEEK_CUR 1 /* Seek from current position. */
#define SEEK_END 2 /* Seek from end of file. */

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
// Include a variant of standard streams that occur in the pre-processed file.
extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

int scanf(const char *restrict format, ...);
int fscanf(FILE *restrict, const char *restrict, ...);
int printf(const char *restrict format, ...);
int fprintf(FILE *restrict, const char *restrict, ...);
int getchar(void);

void setbuf(FILE *restrict, char *restrict);
int setvbuf(FILE *restrict, char *restrict, int, size_t);

FILE *funopen(const void *,
              int (*)(void *, char *, int),
              int (*)(void *, const char *, int),
              fpos_t (*)(void *, fpos_t, int),
              int (*)(void *));

FILE *fopen(const char *restrict path, const char *restrict mode);
FILE *tmpfile(void);
FILE *freopen(const char *restrict pathname, const char *restrict mode, FILE *restrict stream);
int fclose(FILE *fp);
size_t fread(void *restrict, size_t, size_t, FILE *restrict);
size_t fwrite(const void *restrict, size_t, size_t, FILE *restrict);
int fputc(int ch, FILE *stream);
int fseek(FILE *__stream, long int __off, int __whence);
long int ftell(FILE *__stream);
void rewind(FILE *__stream);
int fgetpos(FILE *restrict stream, fpos_t *restrict pos);
int fsetpos(FILE *stream, const fpos_t *pos);
void clearerr(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
int fileno(FILE *stream);

size_t strlen(const char *);

char *strcpy(char *restrict, const char *restrict);
char *strncpy(char *dst, const char *src, size_t n);
char *strsep(char **stringp, const char *delim);
void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *s, int c, size_t n);

typedef unsigned long __darwin_pthread_key_t;
typedef __darwin_pthread_key_t pthread_key_t;
int pthread_setspecific(pthread_key_t, const void *);

int sqlite3_bind_text_my(int, const char*, int n, void(*)(void*));

typedef void (*freeCallback) (void*);
typedef struct {
  int i;
  freeCallback fc;
} StWithCallback;

int dealocateMemWhenDoneByVal(void*, StWithCallback);
int dealocateMemWhenDoneByRef(StWithCallback*, const void*);

typedef struct CGContext *CGContextRef;
CGContextRef CGBitmapContextCreate(void *data/*, size_t width, size_t height,
                                   size_t bitsPerComponent, size_t bytesPerRow,
                                   CGColorSpaceRef space,
                                   CGBitmapInfo bitmapInfo*/);
void *CGBitmapContextGetData(CGContextRef context);

// Include xpc.
typedef struct _xpc_connection_s * xpc_connection_t;
typedef void (*xpc_finalizer_t)(void *value);
void xpc_connection_set_context(xpc_connection_t connection, void *context);
void xpc_connection_set_finalizer_f(xpc_connection_t connection, xpc_finalizer_t finalizer);
void xpc_connection_resume(xpc_connection_t connection);

//The following are fake system header functions for generic testing.
void fakeSystemHeaderCallInt(int *);
void fakeSystemHeaderCallIntPtr(int **);

// Some data strauctures may hold onto the pointer and free it later.
void fake_insque(void *, void *);
typedef struct fake_rb_tree { void *opaque[8]; } fake_rb_tree_t;
void fake_rb_tree_init(fake_rb_tree_t *, const void *);
void *fake_rb_tree_insert_node(fake_rb_tree_t *, void *);

typedef struct __SomeStruct {
  char * p;
} SomeStruct;
void fakeSystemHeaderCall(SomeStruct *);

typedef int pid_t;
pid_t fork(void);
pid_t vfork(void);
int execl(const char *path, const char *arg, ...);
int execle(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execv(const char *path, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);

void exit(int status) __attribute__ ((__noreturn__));
void _exit(int status) __attribute__ ((__noreturn__));
void _Exit(int status) __attribute__ ((__noreturn__));

#define UINT32_MAX        4294967295U
#define INT64_MIN        (-INT64_MAX-1)
#define __DBL_MAX__ 1.7976931348623157e+308
#define DBL_MAX __DBL_MAX__
#ifndef NULL
#define __DARWIN_NULL 0
#define NULL __DARWIN_NULL
#endif
#define EOF (-1)

#define offsetof(t, d) __builtin_offsetof(t, d)

typedef __typeof(sizeof(int)) size_t;
void *malloc(size_t);
void *valloc(size_t);
void free(void *);


void doStuff3(const int y){}
void doStuff2(int g){}
void doStuff_pointerToConstInt(const int *u){};
void doStuff_arrayOfConstInt(const int a[]){};

void doStuff_constPointerToConstInt              (int const * const u){};
void doStuff_constPointerToConstPointerToConstInt(int const * const * const u){};
void doStuff_pointerToConstPointerToConstInt(int const * const * u){};
void doStuff_pointerToPointerToConstInt       (int const **u){};
void doStuff_constStaticSizedArray(const int a[static 10]) {}
void doStuff_variadic(const int *u, ...){};

void f_1(void) {
  int t;               // expected-note {{'t' declared without an initial value}}
  int* tp = &t;        // expected-note {{'tp' initialized here}}
  doStuff_pointerToConstInt(tp);  // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                       // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}

void f_1_1(void) {
  int t;                 // expected-note {{'t' declared without an initial value}}
  int *tp1 = &t;         // expected-note {{'tp1' initialized here}}
  int *tp2 = tp1;        // expected-note {{'tp2' initialized to the value of 'tp1'}}
  doStuff_pointerToConstInt(tp2);  // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                       // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}


int *f_2_sub(int *p) {
  return p; // expected-note {{Returning pointer (loaded from 'p')}}
}

void f_2(void) {
  int t;                // expected-note {{'t' declared without an initial value}}
  int *p = f_2_sub(&t); // expected-note {{Passing value via 1st parameter 'p'}}
                        // expected-note@-1{{Calling 'f_2_sub'}}
                        // expected-note@-2{{Returning from 'f_2_sub'}}
                        // expected-note@-3{{'p' initialized here}}
  int *tp = p;          // expected-note {{'tp' initialized to the value of 'p'}}
  doStuff_pointerToConstInt(tp); // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                      // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}

int z;
void f_3(void) {
      doStuff_pointerToConstInt(&z);  // no warning
}

void f_4(void) {
      int x=5;
      doStuff_pointerToConstInt(&x);  // no warning
}

void f_5(void) {
  int ta[5];           // expected-note {{'ta' initialized here}}
  int *tp = ta;        // expected-note {{'tp' initialized here}}
  doStuff_pointerToConstInt(tp);  // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                       // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}

void f_5_1(void) {
  int ta[5];        // expected-note {{'ta' initialized here}}
  doStuff_pointerToConstInt(ta);  // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                       // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}

void f_6(void) {
  int ta[5] = {1,2,3,4,5};
  int* tp = ta;
  doStuff_pointerToConstInt(tp); // no-warning
}

void f_6_1(void) {
  int ta[5] = {1,2,3,4,5};
  doStuff_pointerToConstInt(ta); // no-warning
}

void f_7(void) {
      int z;        // expected-note {{'z' declared without an initial value}}
      int y=z;      // expected-warning {{Assigned value is garbage or undefined}}
                    // expected-note@-1 {{Assigned value is garbage or undefined}}
      doStuff3(y);
}

void f_8(void) {
      int g;       // expected-note {{'g' declared without an initial value}}
      doStuff2(g); // expected-warning {{1st function call argument is an uninitialized value}}
                   // expected-note@-1 {{1st function call argument is an uninitialized value}}
}

void f_9(void) {
  int a[6];                        // expected-note {{'a' initialized here}}
  int const *ptau = a;             // expected-note {{'ptau' initialized here}}
  doStuff_arrayOfConstInt(ptau);    // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                                   // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}

void f_10(void) {
  int  a[6];                     // expected-note {{'a' initialized here}}
  doStuff_arrayOfConstInt(a);    // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                                 // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}

void f_11(void) {
  int t[10];                    //expected-note {{'t' initialized here}}
  doStuff_constStaticSizedArray(t);  // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                                // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}

void f_12(void) {
  int t[10] = {0,1,2,3,4,5,6,7,8,9};
  doStuff_constStaticSizedArray(t);  // no-warning

}

// https://bugs.llvm.org/show_bug.cgi?id=35419
void f11_0(void) {
  int x; // expected-note {{'x' declared without an initial value}}
  x++; // expected-warning {{The expression is an uninitialized value. The computed value will also be garbage}}
       // expected-note@-1 {{The expression is an uninitialized value. The computed value will also be garbage}}
  clang_analyzer_warnIfReached(); // no-warning
}
void f11_1(void) {
  int x; // expected-note {{'x' declared without an initial value}}
  ++x; // expected-warning {{The expression is an uninitialized value. The computed value will also be garbage}}
       // expected-note@-1 {{The expression is an uninitialized value. The computed value will also be garbage}}
  clang_analyzer_warnIfReached(); // no-warning
}
void f11_2(void) {
  int x; // expected-note {{'x' declared without an initial value}}
  x--; // expected-warning {{The expression is an uninitialized value. The computed value will also be garbage}}
       // expected-note@-1 {{The expression is an uninitialized value. The computed value will also be garbage}}
  clang_analyzer_warnIfReached(); // no-warning
}
void f11_3(void) {
  int x; // expected-note {{'x' declared without an initial value}}
  --x; // expected-warning {{The expression is an uninitialized value. The computed value will also be garbage}}
       // expected-note@-1 {{The expression is an uninitialized value. The computed value will also be garbage}}
  clang_analyzer_warnIfReached(); // no-warning
}

int f_malloc_1(void) {
  int *ptr;

  ptr = (int *)malloc(sizeof(int)); // expected-note {{Value assigned to 'ptr'}}

  doStuff_pointerToConstInt(ptr); // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                       // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
  free(ptr);
  return 0;
}

int f_malloc_2(void) {
  int *ptr;

  ptr = (int *)malloc(sizeof(int));
  *ptr = 25;

  doStuff_pointerToConstInt(ptr); // no warning
  free(ptr);
  return 0;
}

// uninit pointer, uninit val
void f_variadic_unp_unv(void) {
  int t; // expected-note {{'t' declared without an initial value}}
  int v;
  int* tp = &t;           // expected-note {{'tp' initialized here}}
  doStuff_variadic(tp,v);  // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                          // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}
// uninit pointer, init val
void f_variadic_unp_inv(void) {
  int t; // expected-note {{'t' declared without an initial value}}
  int v = 3;
  int* tp = &t;           // expected-note {{'tp' initialized here}}
  doStuff_variadic(tp,v);  // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                          // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}

// init pointer, uninit val
void f_variadic_inp_unv(void) {
  int t=5;
  int v;                  // expected-note {{'v' declared without an initial value}}
  int* tp = &t;
  doStuff_variadic(tp,v);// expected-warning {{2nd function call argument is an uninitialized value}}
                          // expected-note@-1 {{2nd function call argument is an uninitialized value}}
}

// init pointer, init val
void f_variadic_inp_inv(void) {
  int t=5;
  int v = 3;
  int* tp = &t;
  doStuff_variadic(tp,v); // no-warning
}

// init pointer, init pointer
void f_variadic_inp_inp(void) {
  int t=5;
  int u=3;
  int *vp = &u ;
  int *tp = &t;
  doStuff_variadic(tp,vp); // no-warning
}

//uninit pointer, init pointer
void f_variadic_unp_inp(void) {
  int t; // expected-note {{'t' declared without an initial value}}
  int u=3;
  int *vp = &u ;
  int *tp = &t;             // expected-note {{'tp' initialized here}}
  doStuff_variadic(tp,vp); // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                            // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}

//init pointer, uninit pointer
void f_variadic_inp_unp(void) {
  int t=5;
  int u;
  int *vp = &u ;
  int *tp = &t;
  doStuff_variadic(tp,vp); // no-warning
}

//uninit pointer, uninit pointer
void f_variadic_unp_unp(void) {
  int t; // expected-note {{'t' declared without an initial value}}
  int u;
  int *vp = &u ;
  int *tp = &t;             // expected-note {{'tp' initialized here}}
  doStuff_variadic(tp,vp); // expected-warning {{1st function call argument is a pointer to uninitialized value}}
                            // expected-note@-1 {{1st function call argument is a pointer to uninitialized value}}
}
