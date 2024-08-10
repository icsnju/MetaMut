// RUN: %clang_analyze_cc1 -analyzer-checker=core,unix.Malloc -analyzer-max-loop 4 -verify %s
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

static int another_function(int *y) {
  if (*y > 0)
    return *y;
  return 0;
}

static void function_which_doesnt_give_up(int **x) {
   *x = 0;
}

static void function_which_gives_up(int *x) {
  for (int i = 0; i < 5; ++i)
    (*x)++;
}

static void function_which_gives_up_nested(int *x) {
  function_which_gives_up(x);
  for (int i = 0; i < 5; ++i)
    (*x)++;
}

static void function_which_doesnt_give_up_nested(int *x, int *y) {
  *y = another_function(x);
  function_which_gives_up(x);
}

void coverage1(int *x) {
  function_which_gives_up(x);
  char *m = (char*)malloc(12);
} // expected-warning {{Potential leak of memory pointed to by 'm'}}

void coverage2(int *x) {
  if (x) {
    function_which_gives_up(x);
    char *m = (char*)malloc(12);
  }
} // expected-warning {{Potential leak of memory pointed to by 'm'}}

void coverage3(int *x) {
  x++;
  function_which_gives_up(x);
  char *m = (char*)malloc(12);
} // expected-warning {{Potential leak of memory pointed to by 'm'}}

void coverage4(int *x) {
  *x += another_function(x);
  function_which_gives_up(x);
  char *m = (char*)malloc(12);
} // expected-warning {{Potential leak of memory pointed to by 'm'}}

void coverage5(int *x) {
  for (int i = 0; i<7; ++i)
    function_which_gives_up(x);
  // The root function gives up here.
  char *m = (char*)malloc(12); // no-warning
}

void coverage6(int *x) {
  for (int i = 0; i<3; ++i) {
    function_which_gives_up(x);
  }
  char *m = (char*)malloc(12);
} // expected-warning {{Potential leak of memory pointed to by 'm'}}

int coverage7_inline(int *i) {
  function_which_doesnt_give_up(&i);
  return *i; // expected-warning {{Dereference}}
}

void coverage8(int *x) {
  int y;
  function_which_doesnt_give_up_nested(x, &y);
  y = (*x)/y;  // expected-warning {{Division by zero}}
  char *m = (char*)malloc(12);
} // expected-warning {{Potential leak of memory pointed to by 'm'}}

void function_which_gives_up_settonull(int **x) {
  *x = 0;
  int y = 0;
  for (int i = 0; i < 5; ++i)
    y++;
}

void coverage9(int *x) {
  int y = 5;
  function_which_gives_up_settonull(&x);
  y = (*x);  // no warning
}

static void empty_function(void){
}
int use_empty_function(int x) {
    x = 0;
    empty_function();
    return 5/x; //expected-warning {{Division by zero}}
}
