// RUN: %clang_analyze_cc1 -analyzer-checker=core,alpha.unix.Stream,alpha.unix.StdCLibraryFunctions,debug.ExprInspection \
// RUN:   -analyzer-config alpha.unix.StdCLibraryFunctions:ModelPOSIX=true -verify=stream,any %s

// RUN: %clang_analyze_cc1 -analyzer-checker=core,alpha.unix.Stream,debug.ExprInspection \
// RUN:   -analyzer-config alpha.unix.StdCLibraryFunctions:ModelPOSIX=true -verify=stream,any %s

// RUN: %clang_analyze_cc1 -analyzer-checker=core,alpha.unix.StdCLibraryFunctions,debug.ExprInspection \
// RUN:   -analyzer-config alpha.unix.StdCLibraryFunctions:ModelPOSIX=true -verify=stdfunc,any %s

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

extern void clang_analyzer_eval(int);

void *buf;
size_t size;
size_t n;

void test_fopen(void) {
  FILE *fp = fopen("path", "r");
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}} any-warning{{FALSE}}
  fclose(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
}

void test_tmpfile(void) {
  FILE *fp = tmpfile();
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}} any-warning{{FALSE}}
  fclose(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
}

void test_fclose(void) {
  FILE *fp = tmpfile();
  fclose(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
}

void test_freopen(void) {
  FILE *fp = tmpfile();
  fp = freopen("file", "w", fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  fclose(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
}

void test_fread(void) {
  FILE *fp = tmpfile();
  size_t ret = fread(buf, size, n, fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{The 4th argument to 'fread' is NULL but should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  clang_analyzer_eval(ret <= n); // any-warning{{TRUE}}
  clang_analyzer_eval(ret == n); // any-warning{{TRUE}} any-warning{{FALSE}}

  fclose(fp);
}

void test_fwrite(void) {
  FILE *fp = tmpfile();
  size_t ret = fwrite(buf, size, n, fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{The 4th argument to 'fwrite' is NULL but should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  clang_analyzer_eval(ret <= n); // any-warning{{TRUE}}
  clang_analyzer_eval(ret == n); // any-warning{{TRUE}} any-warning{{FALSE}}

  fclose(fp);
}

void test_fseek(void) {
  FILE *fp = tmpfile();
  fseek(fp, 0, 0); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  fclose(fp);
}

void test_ftell(void) {
  FILE *fp = tmpfile();
  ftell(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  fclose(fp);
}

void test_rewind(void) {
  FILE *fp = tmpfile();
  rewind(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  fclose(fp);
}

void test_fgetpos(void) {
  FILE *fp = tmpfile();
  fpos_t pos;
  fgetpos(fp, &pos); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  fclose(fp);
}

void test_fsetpos(void) {
  FILE *fp = tmpfile();
  fpos_t pos;
  fsetpos(fp, &pos); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  fclose(fp);
}

void test_clearerr(void) {
  FILE *fp = tmpfile();
  clearerr(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  fclose(fp);
}

void test_feof(void) {
  FILE *fp = tmpfile();
  feof(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  fclose(fp);
}

void test_ferror(void) {
  FILE *fp = tmpfile();
  ferror(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  fclose(fp);
}

void test_fileno(void) {
  FILE *fp = tmpfile();
  fileno(fp); // \
  // stream-warning{{Stream pointer might be NULL}} \
  // stdfunc-warning{{should not be NULL}}
  clang_analyzer_eval(fp != NULL); // any-warning{{TRUE}}
  fclose(fp);
}