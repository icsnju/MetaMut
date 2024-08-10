// RUN: %clang_analyze_cc1 -verify %s \
// RUN: -analyzer-checker=core \
// RUN: -analyzer-checker=alpha.unix.Stream \
// RUN: -analyzer-checker=debug.StreamTester \
// RUN: -analyzer-checker=debug.ExprInspection

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

void clang_analyzer_eval(int);
void clang_analyzer_dump(int);
void clang_analyzer_warnIfReached(void);
void StreamTesterChecker_make_feof_stream(FILE *);
void StreamTesterChecker_make_ferror_stream(FILE *);

void error_fopen(void) {
  FILE *F = fopen("file", "r");
  if (!F)
    return;
  clang_analyzer_eval(feof(F)); // expected-warning {{FALSE}}
  clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
  fclose(F);
}

void error_freopen(void) {
  FILE *F = fopen("file", "r");
  if (!F)
    return;
  F = freopen(0, "w", F);
  if (!F)
    return;
  clang_analyzer_eval(feof(F)); // expected-warning {{FALSE}}
  clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
  fclose(F);
}

void stream_error_feof(void) {
  FILE *F = fopen("file", "r");
  if (!F)
    return;
  StreamTesterChecker_make_feof_stream(F);
  clang_analyzer_eval(feof(F));   // expected-warning {{TRUE}}
  clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
  clearerr(F);
  clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
  clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
  fclose(F);
}

void stream_error_ferror(void) {
  FILE *F = fopen("file", "r");
  if (!F)
    return;
  StreamTesterChecker_make_ferror_stream(F);
  clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
  clang_analyzer_eval(ferror(F)); // expected-warning {{TRUE}}
  clearerr(F);
  clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
  clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
  fclose(F);
}

void error_fread(void) {
  FILE *F = tmpfile();
  if (!F)
    return;
  char Buf[10];
  int Ret = fread(Buf, 1, 10, F);
  if (Ret == 10) {
    clang_analyzer_eval(feof(F) || ferror(F)); // expected-warning {{FALSE}}
  } else {
    clang_analyzer_eval(feof(F) || ferror(F)); // expected-warning {{TRUE}}
    if (feof(F)) {
      clang_analyzer_warnIfReached(); // expected-warning {{REACHABLE}}
      fread(Buf, 1, 10, F);           // expected-warning {{Read function called when stream is in EOF state}}
      clang_analyzer_eval(feof(F));   // expected-warning {{TRUE}}
      clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
    }
    if (ferror(F)) {
      clang_analyzer_warnIfReached(); // expected-warning {{REACHABLE}}
      fread(Buf, 1, 10, F);           // expected-warning {{might be 'indeterminate'}}
    }
  }
  fclose(F);
  Ret = fread(Buf, 1, 10, F); // expected-warning {{Stream might be already closed}}
}

void error_fwrite(void) {
  FILE *F = tmpfile();
  if (!F)
    return;
  const char *Buf = "123456789";
  int Ret = fwrite(Buf, 1, 10, F);
  if (Ret == 10) {
    clang_analyzer_eval(feof(F) || ferror(F)); // expected-warning {{FALSE}}
  } else {
    clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
    clang_analyzer_eval(ferror(F)); // expected-warning {{TRUE}}
    fwrite(0, 1, 10, F);            // expected-warning {{might be 'indeterminate'}}
  }
  fclose(F);
  Ret = fwrite(0, 1, 10, F); // expected-warning {{Stream might be already closed}}
}

void freadwrite_zerosize(FILE *F) {
  size_t Ret;
  Ret = fwrite(0, 1, 0, F);
  clang_analyzer_dump(Ret); // expected-warning {{0 }}
  Ret = fwrite(0, 0, 1, F);
  clang_analyzer_dump(Ret); // expected-warning {{0 }}
  Ret = fread(0, 1, 0, F);
  clang_analyzer_dump(Ret); // expected-warning {{0 }}
  Ret = fread(0, 0, 1, F);
  clang_analyzer_dump(Ret); // expected-warning {{0 }}
}

void freadwrite_zerosize_eofstate(FILE *F) {
  fwrite(0, 1, 0, F);
  fwrite(0, 0, 1, F);
  fread(0, 1, 0, F); // expected-warning {{Read function called when stream is in EOF state}}
  fread(0, 0, 1, F); // expected-warning {{Read function called when stream is in EOF state}}
}

void error_fread_fwrite_zerosize(void) {
  FILE *F = fopen("file", "r");
  if (!F)
    return;

  freadwrite_zerosize(F);
  clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
  clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}

  StreamTesterChecker_make_ferror_stream(F);
  freadwrite_zerosize(F);
  clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
  clang_analyzer_eval(ferror(F)); // expected-warning {{TRUE}}

  StreamTesterChecker_make_feof_stream(F);
  freadwrite_zerosize_eofstate(F);
  clang_analyzer_eval(feof(F));   // expected-warning {{TRUE}}
  clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}

  fclose(F);
}

void error_fseek(void) {
  FILE *F = fopen("file", "r");
  if (!F)
    return;
  int rc = fseek(F, 1, SEEK_SET);
  if (rc) {
    int IsFEof = feof(F), IsFError = ferror(F);
    // Get feof or ferror or no error.
    clang_analyzer_eval(IsFEof || IsFError);
    // expected-warning@-1 {{FALSE}}
    // expected-warning@-2 {{TRUE}}
    clang_analyzer_eval(IsFEof && IsFError); // expected-warning {{FALSE}}
    // Error flags should not change.
    if (IsFEof)
      clang_analyzer_eval(feof(F)); // expected-warning {{TRUE}}
    else
      clang_analyzer_eval(feof(F)); // expected-warning {{FALSE}}
    if (IsFError)
      clang_analyzer_eval(ferror(F)); // expected-warning {{TRUE}}
    else
      clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
  } else {
    clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
    clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
    // Error flags should not change.
    clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
    clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
  }
  fclose(F);
}

void error_fseek_0(void) {
  FILE *F = fopen("file", "r");
  if (!F)
    return;
  int rc = fseek(F, 0, SEEK_SET);
  if (rc) {
    int IsFEof = feof(F), IsFError = ferror(F);
    // Get ferror or no error, but not feof.
    clang_analyzer_eval(IsFError);
    // expected-warning@-1 {{FALSE}}
    // expected-warning@-2 {{TRUE}}
    clang_analyzer_eval(IsFEof);
    // expected-warning@-1 {{FALSE}}
    // Error flags should not change.
    clang_analyzer_eval(feof(F)); // expected-warning {{FALSE}}
    if (IsFError)
      clang_analyzer_eval(ferror(F)); // expected-warning {{TRUE}}
    else
      clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
  } else {
    clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
    clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
    // Error flags should not change.
    clang_analyzer_eval(feof(F));   // expected-warning {{FALSE}}
    clang_analyzer_eval(ferror(F)); // expected-warning {{FALSE}}
  }
  fclose(F);
}

void error_indeterminate(void) {
  FILE *F = fopen("file", "r+");
  if (!F)
    return;
  const char *Buf = "123456789";
  int rc = fseek(F, 0, SEEK_SET);
  if (rc) {
    if (feof(F)) {
      fwrite(Buf, 1, 10, F); // no warning
    } else if (ferror(F)) {
      fwrite(Buf, 1, 10, F); // expected-warning {{might be 'indeterminate'}}
    } else {
      fwrite(Buf, 1, 10, F); // expected-warning {{might be 'indeterminate'}}
    }
  }
  fclose(F);
}

void error_indeterminate_clearerr(void) {
  FILE *F = fopen("file", "r+");
  if (!F)
    return;
  const char *Buf = "123456789";
  int rc = fseek(F, 0, SEEK_SET);
  if (rc) {
    if (feof(F)) {
      clearerr(F);
      fwrite(Buf, 1, 10, F); // no warning
    } else if (ferror(F)) {
      clearerr(F);
      fwrite(Buf, 1, 10, F); // expected-warning {{might be 'indeterminate'}}
    } else {
      clearerr(F);
      fwrite(Buf, 1, 10, F); // expected-warning {{might be 'indeterminate'}}
    }
  }
  fclose(F);
}

void error_indeterminate_feof1(void) {
  FILE *F = fopen("file", "r+");
  if (!F)
    return;
  char Buf[10];
  if (fread(Buf, 1, 10, F) < 10) {
    if (feof(F)) {
      // error is feof, should be non-indeterminate
      fwrite("1", 1, 1, F); // no warning
    }
  }
  fclose(F);
}

void error_indeterminate_feof2(void) {
  FILE *F = fopen("file", "r+");
  if (!F)
    return;
  char Buf[10];
  if (fread(Buf, 1, 10, F) < 10) {
    if (ferror(F) == 0) {
      // error is feof, should be non-indeterminate
      fwrite("1", 1, 1, F); // no warning
    }
  }
  fclose(F);
}
