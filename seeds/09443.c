// RUN: %clang_analyze_cc1 \
// RUN:  -analyzer-checker=alpha.security.cert.env.InvalidPtr\
// RUN:  -analyzer-output=text -verify -Wno-unused %s

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
char *getenv(const char *name);
char *setlocale(int category, const char *locale);
char *strerror(int errnum);

typedef struct {
  char * field;
} lconv;
lconv *localeconv(void);

typedef struct {
} tm;
char *asctime(const tm *timeptr);

int strcmp(const char*, const char*);
extern void foo(char *e);
extern char* bar(void);


void getenv_test1(void) {
  char *p;

  p = getenv("VAR");
  *p; // no-warning

  p = getenv("VAR2");
  *p; // no-warning, getenv result was assigned to the same pointer
}

void getenv_test2(void) {
  char *p, *p2;

  p = getenv("VAR");
  // expected-note@-1{{previous function call was here}}
  *p; // no-warning

  p2 = getenv("VAR2");
  // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}

  *p;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void getenv_test3(void) {
  char *p, *p2, *p3;

  p = getenv("VAR");
  *p; // no-warning

  p = getenv("VAR2");
  // expected-note@-1{{previous function call was here}}
  p2 = getenv("VAR2");
  // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}

  p3 = getenv("VAR3");

  *p;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void getenv_test4(void) {
  char *p, *p2, *p3;

  p = getenv("VAR");
  // expected-note@-1{{previous function call was here}}
  p2 = getenv("VAR2");
  // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}
  p3 = getenv("VAR3");

  *p;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void getenv_test5(void) {
  char *p, *p2, *p3;

  p = getenv("VAR");
  p2 = getenv("VAR2");
  // expected-note@-1{{previous function call was here}}
  p3 = getenv("VAR3");
  // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}

  *p2;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void getenv_test6(void) {
  char *p, *p2;
  p = getenv("VAR");
  *p; // no-warning

  p = getenv("VAR2");
  // expected-note@-1{{previous function call was here}}
  *p; // no-warning

  p2 = getenv("VAR3");
  // expected-note@-1{{previous function call was here}}
  // expected-note@-2{{'getenv' call may invalidate the result of the previous 'getenv'}}

  *p;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}

  *p2; // no-warning

  p = getenv("VAR4");
  // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}

  *p; // no-warning
  *p2;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void getenv_test7(void) {
  char *p, *p2;
  p = getenv("VAR");
  // expected-note@-1{{previous function call was here}}
  *p; // no-warning

  p2 = getenv("VAR2");
  // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}

  foo(p);
  // expected-warning@-1{{use of invalidated pointer 'p' in a function call}}
  // expected-note@-2{{use of invalidated pointer 'p' in a function call}}
}

void getenv_test8(void) {
  static const char *array[] = {
     0,
     0,
     "/var/tmp",
     "/usr/tmp",
     "/tmp",
     "."
  };

  if( !array[0] )
  // expected-note@-1{{Taking true branch}}
    array[0] = getenv("TEMPDIR");
    // expected-note@-1{{previous function call was here}}

  if( !array[1] )
  // expected-note@-1{{Taking true branch}}
    array[1] = getenv("TMPDIR");
  // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}

  *array[0];
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void getenv_test9(void) {
  char *p, *p2;
  p = getenv("something");
  p = bar();
  p2 = getenv("something");
  *p; // no-warning: p does not point to getenv anymore
}

void getenv_test10(void) {
  strcmp(getenv("VAR1"), getenv("VAR2"));
  // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}
  // expected-note@-2{{previous function call was here}}
  // expected-warning@-3{{use of invalidated pointer 'getenv("VAR1")' in a function call}}
  // expected-note@-4{{use of invalidated pointer 'getenv("VAR1")' in a function call}}
}

void dereference_pointer(char* a) {
  *a;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void getenv_test11(void) {
  char *p = getenv("VAR");
  // expected-note@-1{{previous function call was here}}

  char *pp = getenv("VAR2");
  // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}

  dereference_pointer(p);
  // expected-note@-1{{Calling 'dereference_pointer'}}
}

void getenv_test12(int flag1, int flag2) {
  char *p = getenv("VAR");
  // expected-note@-1{{previous function call was here}}

  if (flag1) {
    // expected-note@-1{{Assuming 'flag1' is not equal to 0}}
    // expected-note@-2{{Taking true branch}}
    char *pp = getenv("VAR2");
    // expected-note@-1{{'getenv' call may invalidate the result of the previous 'getenv'}}
  }

  if (flag2) {
    // expected-note@-1{{Assuming 'flag2' is not equal to 0}}
    // expected-note@-2{{Taking true branch}}
    *p;
    // expected-warning@-1{{dereferencing an invalid pointer}}
    // expected-note@-2{{dereferencing an invalid pointer}}
  }
}

void setlocale_test1(void) {
  char *p, *p2;
  p = setlocale(0, "VAR");
  *p; // no-warning

  p = setlocale(0, "VAR2");
  // expected-note@-1{{previous function call was here}}
  *p; // no-warning

  p2 = setlocale(0, "VAR3");
  // expected-note@-1{{'setlocale' call may invalidate the result of the previous 'setlocale'}}

  *p;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void setlocale_test2(int flag) {
  char *p, *p2;
  p = setlocale(0, "VAR");
  *p; // no-warning

  p = setlocale(0, "VAR2");
  // expected-note@-1{{previous function call was here}}
  *p; // no-warning

  if (flag) {
    // expected-note@-1{{Assuming 'flag' is not equal to 0}}
    // expected-note@-2{{Taking true branch}}
    p2 = setlocale(0, "VAR3");
    // expected-note@-1{{'setlocale' call may invalidate the result of the previous 'setlocale'}}
  }

  *p;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void strerror_test1(void) {
  char *p, *p2;

  p = strerror(0);
  *p; // no-warning

  p = strerror(1);
  // expected-note@-1{{previous function call was here}}
  *p; // no-warning

  p2 = strerror(2);
  // expected-note@-1{{'strerror' call may invalidate the result of the previous 'strerror'}}

  *p;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void strerror_test2(int errno) {
  char *p, *p2;

  p = strerror(0);
  *p; // no-warning

  p = strerror(1);
  // expected-note@-1{{previous function call was here}}
  *p; // no-warning

  if (0 == 1) {
    // expected-note@-1{{0 is not equal to 1}}
    // expected-note@-2{{Taking false branch}}
    p2 = strerror(2);
  }

  *p; // no-warning

  if (errno) {
    // expected-note@-1{{Assuming 'errno' is not equal to 0}}
    // expected-note@-2{{Taking true branch}}
    p2 = strerror(errno);
    // expected-note@-1{{'strerror' call may invalidate the result of the previous 'strerror'}}
  }

  *p;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void asctime_test(void) {
  const tm *t;
  const tm *tt;

  char* p = asctime(t);
  // expected-note@-1{{previous function call was here}}
  char* pp = asctime(tt);
  // expected-note@-1{{'asctime' call may invalidate the result of the previous 'asctime'}}

  *p;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void localeconv_test1(void) {
  lconv *lc1 = localeconv();
  // expected-note@-1{{previous function call was here}}
  lconv *lc2 = localeconv();
  // expected-note@-1{{'localeconv' call may invalidate the result of the previous 'localeconv'}}

  *lc1;
  // expected-warning@-1{{dereferencing an invalid pointer}}
  // expected-note@-2{{dereferencing an invalid pointer}}
}

void localeconv_test2(void) {
  // TODO: false negative
  lconv *lc1 = localeconv();
  lconv *lc2 = localeconv();
  lc1->field;
}
