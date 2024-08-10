// RUN: %clang_cc1 -analyze -analyzer-checker=core,unix.Malloc -verify %s

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

typedef void* gpointer;
typedef const void* gconstpointer;
typedef unsigned long gsize;
typedef unsigned int guint;

gpointer g_malloc(gsize n_bytes);
gpointer g_malloc0(gsize n_bytes);
gpointer g_realloc(gpointer mem, gsize n_bytes);
gpointer g_try_malloc(gsize n_bytes);
gpointer g_try_malloc0(gsize n_bytes);
gpointer g_try_realloc(gpointer mem, gsize n_bytes);
gpointer g_malloc_n(gsize n_blocks, gsize n_block_bytes);
gpointer g_malloc0_n(gsize n_blocks, gsize n_block_bytes);
gpointer g_realloc_n(gpointer mem, gsize n_blocks, gsize n_block_bytes);
gpointer g_try_malloc_n(gsize n_blocks, gsize n_block_bytes);
gpointer g_try_malloc0_n(gsize n_blocks, gsize n_block_bytes);
gpointer g_try_realloc_n(gpointer mem, gsize n_blocks, gsize n_block_bytes);
void g_free(gpointer mem);
gpointer g_memdup(gconstpointer mem, guint byte_size);
gpointer g_strconcat(gconstpointer string1, ...);

static const gsize n_bytes = 1024;

void f1(void) {
  gpointer g1 = g_malloc(n_bytes);
  gpointer g2 = g_malloc0(n_bytes);
  g1 = g_realloc(g1, n_bytes * 2);
  gpointer g3 = g_try_malloc(n_bytes);
  gpointer g4 = g_try_malloc0(n_bytes);
  g3 = g_try_realloc(g3, n_bytes * 2);
  gpointer g5 = g_malloc_n(n_bytes, sizeof(char));
  gpointer g6 = g_malloc0_n(n_bytes, sizeof(char));
  g5 = g_realloc_n(g5, n_bytes * 2, sizeof(char));
  gpointer g7 = g_try_malloc_n(n_bytes, sizeof(char));
  gpointer g8 = g_try_malloc0_n(n_bytes, sizeof(char));
  g7 = g_try_realloc_n(g7, n_bytes * 2, sizeof(char));

  g_free(g1);
  g_free(g2);
  g_free(g2); // expected-warning{{Attempt to free released memory}}
}

void f2(void) {
  gpointer g1 = g_malloc(n_bytes);
  gpointer g2 = g_malloc0(n_bytes);
  g1 = g_realloc(g1, n_bytes * 2);
  gpointer g3 = g_try_malloc(n_bytes);
  gpointer g4 = g_try_malloc0(n_bytes);
  g3 = g_try_realloc(g3, n_bytes * 2);
  gpointer g5 = g_malloc_n(n_bytes, sizeof(char));
  gpointer g6 = g_malloc0_n(n_bytes, sizeof(char));
  g5 = g_realloc_n(g5, n_bytes * 2, sizeof(char));
  gpointer g7 = g_try_malloc_n(n_bytes, sizeof(char));
  gpointer g8 = g_try_malloc0_n(n_bytes, sizeof(char));
  g7 = g_try_realloc_n(g7, n_bytes * 2, sizeof(char));

  g_free(g1);
  g_free(g2);
  g_free(g3);
  g3 = g_memdup(g3, n_bytes); // expected-warning{{Use of memory after it is freed}}
}

void f3(void) {
  gpointer g1 = g_malloc(n_bytes);
  gpointer g2 = g_malloc0(n_bytes);
  g1 = g_realloc(g1, n_bytes * 2);
  gpointer g3 = g_try_malloc(n_bytes);
  gpointer g4 = g_try_malloc0(n_bytes);
  g3 = g_try_realloc(g3, n_bytes * 2); // expected-warning{{Potential leak of memory pointed to by 'g4'}}
  gpointer g5 = g_malloc_n(n_bytes, sizeof(char));
  gpointer g6 = g_malloc0_n(n_bytes, sizeof(char));
  g5 = g_realloc_n(g5, n_bytes * 2, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g6'}}
  gpointer g7 = g_try_malloc_n(n_bytes, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g5'}}
  gpointer g8 = g_try_malloc0_n(n_bytes, sizeof(char));
  g7 = g_try_realloc_n(g7, n_bytes * 2, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g8'}}

  g_free(g1); // expected-warning{{Potential leak of memory pointed to by 'g7'}}
  g_free(g2);
  g_free(g3);
}

void f4(void) {
  gpointer g1 = g_malloc(n_bytes);
  gpointer g2 = g_malloc0(n_bytes);
  g1 = g_realloc(g1, n_bytes * 2);
  gpointer g3 = g_try_malloc(n_bytes);
  gpointer g4 = g_try_malloc0(n_bytes);
  g3 = g_try_realloc(g3, n_bytes * 2);
  gpointer g5 = g_malloc_n(n_bytes, sizeof(char));
  gpointer g6 = g_malloc0_n(n_bytes, sizeof(char));
  g5 = g_realloc_n(g5, n_bytes * 2, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g6'}}
  gpointer g7 = g_try_malloc_n(n_bytes, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g5'}}
  gpointer g8 = g_try_malloc0_n(n_bytes, sizeof(char));
  g7 = g_try_realloc_n(g7, n_bytes * 2, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g8'}}

  g_free(g1); // expected-warning{{Potential leak of memory pointed to by 'g7'}}
  g_free(g2);
  g_free(g3);
  g_free(g4);
}

void f5(void) {
  gpointer g1 = g_malloc(n_bytes);
  gpointer g2 = g_malloc0(n_bytes);
  g1 = g_realloc(g1, n_bytes * 2);
  gpointer g3 = g_try_malloc(n_bytes);
  gpointer g4 = g_try_malloc0(n_bytes);
  g3 = g_try_realloc(g3, n_bytes * 2);
  gpointer g5 = g_malloc_n(n_bytes, sizeof(char));
  gpointer g6 = g_malloc0_n(n_bytes, sizeof(char));
  g5 = g_realloc_n(g5, n_bytes * 2, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g6'}}
  gpointer g7 = g_try_malloc_n(n_bytes, sizeof(char));
  gpointer g8 = g_try_malloc0_n(n_bytes, sizeof(char));
  g7 = g_try_realloc_n(g7, n_bytes * 2, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g8'}}

  g_free(g1); // expected-warning{{Potential leak of memory pointed to by 'g7'}}
  g_free(g2);
  g_free(g3);
  g_free(g4);
  g_free(g5);
}

void f6(void) {
  gpointer g1 = g_malloc(n_bytes);
  gpointer g2 = g_malloc0(n_bytes);
  g1 = g_realloc(g1, n_bytes * 2);
  gpointer g3 = g_try_malloc(n_bytes);
  gpointer g4 = g_try_malloc0(n_bytes);
  g3 = g_try_realloc(g3, n_bytes * 2);
  gpointer g5 = g_malloc_n(n_bytes, sizeof(char));
  gpointer g6 = g_malloc0_n(n_bytes, sizeof(char));
  g5 = g_realloc_n(g5, n_bytes * 2, sizeof(char));
  gpointer g7 = g_try_malloc_n(n_bytes, sizeof(char));
  gpointer g8 = g_try_malloc0_n(n_bytes, sizeof(char));
  g7 = g_try_realloc_n(g7, n_bytes * 2, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g8'}}

  g_free(g1); // expected-warning{{Potential leak of memory pointed to by 'g7'}}
  g_free(g2);
  g_free(g3);
  g_free(g4);
  g_free(g5);
  g_free(g6);
}

void f7(void) {
  gpointer g1 = g_malloc(n_bytes);
  gpointer g2 = g_malloc0(n_bytes);
  g1 = g_realloc(g1, n_bytes * 2);
  gpointer g3 = g_try_malloc(n_bytes);
  gpointer g4 = g_try_malloc0(n_bytes);
  g3 = g_try_realloc(g3, n_bytes * 2);
  gpointer g5 = g_malloc_n(n_bytes, sizeof(char));
  gpointer g6 = g_malloc0_n(n_bytes, sizeof(char));
  g5 = g_realloc_n(g5, n_bytes * 2, sizeof(char));
  gpointer g7 = g_try_malloc_n(n_bytes, sizeof(char));
  gpointer g8 = g_try_malloc0_n(n_bytes, sizeof(char));
  g7 = g_try_realloc_n(g7, n_bytes * 2, sizeof(char)); // expected-warning{{Potential leak of memory pointed to by 'g8'}}

  g_free(g1);
  g_free(g2);
  g_free(g3);
  g_free(g4);
  g_free(g5);
  g_free(g6);
  g_free(g7);
}

void f8(void) {
  typedef struct {
    gpointer str;
  } test_struct;

  test_struct *s1 = (test_struct *)g_malloc0(sizeof(test_struct));
  test_struct *s2 = (test_struct *)g_memdup(s1, sizeof(test_struct));
  gpointer str = g_strconcat("text", s1->str, s2->str, NULL); // no-warning
  g_free(str);
  g_free(s2);
  g_free(s1);
}
