// RUN: %clang_analyze_cc1 -analyzer-checker=unix.Malloc -analyzer-inline-max-stack-depth=5 -verify %s

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

void *malloc(size_t);
void *valloc(size_t);
void free(void *);
void *realloc(void *ptr, size_t size);
void *reallocf(void *ptr, size_t size);
void *calloc(size_t nmemb, size_t size);

void exit(int) __attribute__ ((__noreturn__));
void *memcpy(void * restrict s1, const void * restrict s2, size_t n);
size_t strlen(const char *);

static void my_malloc1(void **d, size_t size) {
  *d = malloc(size);
}

static void *my_malloc2(int elevel, size_t size) {
  void     *data;
  data = malloc(size);
  if (data == 0)
    exit(0);
  return data;
}

static void my_free1(void *p) {
  free(p);
}

static void test1(void) {
  void *data = 0;
  my_malloc1(&data, 4);
} // expected-warning {{Potential leak of memory pointed to by 'data'}}

static void test11(void) {
  void *data = 0;
  my_malloc1(&data, 4);
  my_free1(data);
}

static void testUniqueingByallocationSiteInTopLevelFunction(void) {
  void *data = my_malloc2(1, 4);
  data = 0;
  int x = 5;// expected-warning {{Potential leak of memory pointed to by 'data'}}
  data = my_malloc2(1, 4);
} // expected-warning {{Potential leak of memory pointed to by 'data'}}

static void test3(void) {
  void *data = my_malloc2(1, 4);
  free(data);
  data = my_malloc2(1, 4);
  free(data);
}

int test4(void) {
  int *data = (int*)my_malloc2(1, 4);
  my_free1(data);
  data = (int *)my_malloc2(1, 4);
  my_free1(data);
  return *data; // expected-warning {{Use of memory after it is freed}}
}

void test6(void) {
  int *data = (int *)my_malloc2(1, 4);
  my_free1((int*)data);
  my_free1((int*)data); // expected-warning{{Use of memory after it is freed}}
}

// TODO: We should warn here.
void test5(void) {
  int *data;
  my_free1((int*)data);
}

static char *reshape(char *in) {
    return 0;
}

void testThatRemoveDeadBindingsRunBeforeEachCall(void) {
    char *v = malloc(12);
    v = reshape(v);
    v = reshape(v);// expected-warning {{Potential leak of memory pointed to by 'v'}}
}

// Test that we keep processing after 'return;'
void fooWithEmptyReturn(int x) {
  if (x)
    return;
  x++;
  return;
}

int uafAndCallsFooWithEmptyReturn(void) {
  int *x = (int*)malloc(12);
  free(x);
  fooWithEmptyReturn(12);
  return *x; // expected-warning {{Use of memory after it is freed}}
}


// If we inline any of the malloc-family functions, the checker shouldn't also
// try to do additional modeling.
char *strndup(const char *str, size_t n) {
  if (!str)
    return 0;
  
  // DO NOT FIX. This is to test that we are actually using the inlined
  // behavior!
  if (n < 5)
    return 0;
  
  size_t length = strlen(str);
  if (length < n)
    n = length;
  
  char *result = malloc(n + 1);
  memcpy(result, str, n);
  result[n] = '\0';
  return result;
}

void useStrndup(size_t n) {
  if (n == 0) {
    (void)strndup(0, 20); // no-warning
    return;
  } else if (n < 5) {
    (void)strndup("hi there", n); // no-warning
    return;
  } else {
    (void)strndup("hi there", n);
    return; // expected-warning{{leak}}
  }
}
