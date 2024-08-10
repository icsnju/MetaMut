// RUN: %clang_analyze_cc1 -analyzer-checker=core,alpha.unix.Stream -verify %s

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

void check_fread(void) {
  FILE *fp = tmpfile();
  fread(0, 0, 0, fp); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_fwrite(void) {
  FILE *fp = tmpfile();
  fwrite(0, 0, 0, fp); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_fseek(void) {
  FILE *fp = tmpfile();
  fseek(fp, 0, 0); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_ftell(void) {
  FILE *fp = tmpfile();
  ftell(fp); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_rewind(void) {
  FILE *fp = tmpfile();
  rewind(fp); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_fgetpos(void) {
  FILE *fp = tmpfile();
  fpos_t pos;
  fgetpos(fp, &pos); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_fsetpos(void) {
  FILE *fp = tmpfile();
  fpos_t pos;
  fsetpos(fp, &pos); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_clearerr(void) {
  FILE *fp = tmpfile();
  clearerr(fp); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_feof(void) {
  FILE *fp = tmpfile();
  feof(fp); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_ferror(void) {
  FILE *fp = tmpfile();
  ferror(fp); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void check_fileno(void) {
  FILE *fp = tmpfile();
  fileno(fp); // expected-warning {{Stream pointer might be NULL}}
  fclose(fp);
}

void f_open(void) {
  FILE *p = fopen("foo", "r");
  char buf[1024];
  fread(buf, 1, 1, p); // expected-warning {{Stream pointer might be NULL}}
  fclose(p);
}

void f_seek(void) {
  FILE *p = fopen("foo", "r");
  if (!p)
    return;
  fseek(p, 1, SEEK_SET); // no-warning
  fseek(p, 1, 3); // expected-warning {{The whence argument to fseek() should be SEEK_SET, SEEK_END, or SEEK_CUR}}
  fclose(p);
}

void f_double_close(void) {
  FILE *p = fopen("foo", "r");
  if (!p)
    return;
  fclose(p);
  fclose(p); // expected-warning {{Stream might be already closed}}
}

void f_double_close_alias(void) {
  FILE *p1 = fopen("foo", "r");
  if (!p1)
    return;
  FILE *p2 = p1;
  fclose(p1);
  fclose(p2); // expected-warning {{Stream might be already closed}}
}

void f_use_after_close(void) {
  FILE *p = fopen("foo", "r");
  if (!p)
    return;
  fclose(p);
  clearerr(p); // expected-warning {{Stream might be already closed}}
}

void f_open_after_close(void) {
  FILE *p = fopen("foo", "r");
  if (!p)
    return;
  fclose(p);
  p = fopen("foo", "r");
  if (!p)
    return;
  fclose(p);
}

void f_reopen_after_close(void) {
  FILE *p = fopen("foo", "r");
  if (!p)
    return;
  fclose(p);
  // Allow reopen after close.
  p = freopen("foo", "w", p);
  if (!p)
    return;
  fclose(p);
}

void f_leak(int c) {
  FILE *p = fopen("foo.c", "r");
  if (!p)
    return;
  if(c)
    return; // expected-warning {{Opened stream never closed. Potential resource leak}}
  fclose(p);
}

FILE *f_null_checked(void) {
  FILE *p = fopen("foo.c", "r");
  if (p)
    return p; // no-warning
  else
    return 0;
}

void pr7831(FILE *fp) {
  fclose(fp); // no-warning
}

// PR 8081 - null pointer crash when 'whence' is not an integer constant
void pr8081(FILE *stream, long offset, int whence) {
  fseek(stream, offset, whence);
}

void check_freopen_1(void) {
  FILE *f1 = freopen("foo.c", "r", (FILE *)0); // expected-warning {{Stream pointer might be NULL}}
  f1 = freopen(0, "w", (FILE *)0x123456);      // Do not report this as error.
}

void check_freopen_2(void) {
  FILE *f1 = fopen("foo.c", "r");
  if (f1) {
    FILE *f2 = freopen(0, "w", f1);
    if (f2) {
      // Check if f1 and f2 point to the same stream.
      fclose(f1);
      fclose(f2); // expected-warning {{Stream might be already closed.}}
    } else {
      // Reopen failed.
      // f1 is non-NULL but points to a possibly invalid stream.
      rewind(f1); // expected-warning {{Stream might be invalid}}
      // f2 is NULL but the previous error stops the checker.
      rewind(f2);
    }
  }
}

void check_freopen_3(void) {
  FILE *f1 = fopen("foo.c", "r");
  if (f1) {
    // Unchecked result of freopen.
    // The f1 may be invalid after this call.
    freopen(0, "w", f1);
    rewind(f1); // expected-warning {{Stream might be invalid}}
    fclose(f1);
  }
}

extern FILE *GlobalF;
extern void takeFile(FILE *);

void check_escape1(void) {
  FILE *F = tmpfile();
  if (!F)
    return;
  fwrite("1", 1, 1, F); // may fail
  GlobalF = F;
  fwrite("1", 1, 1, F); // no warning
}

void check_escape2(void) {
  FILE *F = tmpfile();
  if (!F)
    return;
  fwrite("1", 1, 1, F); // may fail
  takeFile(F);
  fwrite("1", 1, 1, F); // no warning
}

void check_escape3(void) {
  FILE *F = tmpfile();
  if (!F)
    return;
  takeFile(F);
  F = freopen(0, "w", F);
  if (!F)
    return;
  fwrite("1", 1, 1, F); // may fail
  fwrite("1", 1, 1, F); // no warning
}

void check_escape4(void) {
  FILE *F = tmpfile();
  if (!F)
    return;
  fwrite("1", 1, 1, F); // may fail

  // no escape at (non-StreamChecker-handled) system call
  // FIXME: all such calls should be handled by the checker
  fprintf(F, "0");

  fwrite("1", 1, 1, F); // expected-warning {{might be 'indeterminate'}}
  fclose(F);
}

int Test;
_Noreturn void handle_error(void);

void check_leak_noreturn_1(void) {
  FILE *F1 = tmpfile();
  if (!F1)
    return;
  if (Test == 1) {
    handle_error(); // no warning
  }
  rewind(F1);
} // expected-warning {{Opened stream never closed. Potential resource leak}}

// Check that "location uniqueing" works.
// This results in reporting only one occurence of resource leak for a stream.
void check_leak_noreturn_2(void) {
  FILE *F1 = tmpfile();
  if (!F1)
    return;
  if (Test == 1) {
    return; // no warning
  }
  rewind(F1);
} // expected-warning {{Opened stream never closed. Potential resource leak}}
// FIXME: This warning should be placed at the `return` above.
// See https://reviews.llvm.org/D83120 about details.
