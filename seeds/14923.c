// RUN: %clang_analyze_cc1 -analyzer-checker=core,security.insecureAPI.vfork,unix.Vfork -verify %s
// RUN: %clang_analyze_cc1 -analyzer-checker=core,security.insecureAPI.vfork,unix.Vfork -verify -x c++ %s

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

void foo(void);

// Ensure that child process is properly checked.
int f1(int x, int y) {
  pid_t pid = vfork(); // expected-warning{{Call to function 'vfork' is insecure}}
  if (pid != 0)
    return 0;

  switch (x) {
  case 0:
    // Ensure that modifying pid is ok.
    pid = 1; // no-warning
    // Ensure that calling allowlisted routines is ok.
    switch (y) {
    case 0:
      execl("", "", 0); // no-warning
      break;
    case 1:
      execle("", "", 0); // no-warning
      break;
    case 2:
      execlp("", "", 0); // no-warning
      break;
    case 3:
      execv("", NULL); // no-warning
      break;
    case 4:
      execve("", NULL, NULL); // no-warning
      break;
    case 5:
      execvp("", NULL); // no-warning
      break;
    case 6:
      execvpe("", NULL, NULL); // no-warning
      break;
    }
    _exit(1); // no-warning
    break;
  case 1:
    // Ensure that writing variables is prohibited.
    x = 0; // expected-warning{{This assignment is prohibited after a successful vfork}}
    break;
  case 2:
    // Ensure that calling functions is prohibited.
    foo(); // expected-warning{{This function call is prohibited after a successful vfork}}
    break;
  default:
    // Ensure that returning from function is prohibited.
    return 0; // expected-warning{{Return is prohibited after a successful vfork; call _exit() instead}}
  }

  while(1);
}

// Same as previous but without explicit pid variable.
int f2(int x) {
  pid_t pid = vfork(); // expected-warning{{Call to function 'vfork' is insecure}}

  switch (x) {
  case 0:
    // Ensure that writing pid is ok.
    pid = 1; // no-warning
    // Ensure that calling allowlisted routines is ok.
    execl("", "", 0); // no-warning
    _exit(1); // no-warning
    break;
  case 1:
    // Ensure that writing variables is prohibited.
    x = 0; // expected-warning{{This assignment is prohibited after a successful vfork}}
    break;
  case 2:
    // Ensure that calling functions is prohibited.
    foo(); // expected-warning{{This function call is prohibited after a successful vfork}}
    break;
  default:
    // Ensure that returning from function is prohibited.
    return 0; // expected-warning{{Return is prohibited after a successful vfork; call _exit() instead}}
  }

  while(1);
}

// Ensure that parent process isn't restricted.
int f3(int x) {
  if (vfork() == 0) // expected-warning{{Call to function 'vfork' is insecure}}
    _exit(1);
  x = 0; // no-warning
  foo(); // no-warning
  return 0;
} // no-warning

// Unbound pids are special so test them separately.
void f4(int x) {
  switch (x) {
  case 0:
    vfork(); // expected-warning{{Call to function 'vfork' is insecure}}
    x = 0; // expected-warning{{This assignment is prohibited after a successful vfork}}
    break;

  case 1:
    {
      char args[2];
      switch (vfork()) { // expected-warning{{Call to function 'vfork' is insecure}}
      case 0:
        args[0] = 0; // expected-warning{{This assignment is prohibited after a successful vfork}}
        exit(1);
      }
      break;
    }

  case 2:
    {
      pid_t pid;
      if ((pid = vfork()) == 0) // expected-warning{{Call to function 'vfork' is insecure}}
        while(1); // no-warning
      break;
    }
  }
  while(1);
} //no-warning


void f5(void) {
  // See "libxtables: move some code to avoid cautions in vfork man page"
  // (http://lists.netfilter.org/pipermail/netfilter-buglog/2014-October/003280.html).
  if (vfork() == 0) { // expected-warning{{Call to function 'vfork' is insecure}}
    execl("prog", "arg1", 0); // no-warning
    exit(1);  // expected-warning{{This function call is prohibited after a successful vfork}}
  }
}

