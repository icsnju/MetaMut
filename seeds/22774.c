// RUN: %clang_analyze_cc1 -Wno-int-to-pointer-cast -analyzer-checker=alpha.security.taint,debug.TaintTest %s -verify

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

#define BUFSIZE 10
int Buffer[BUFSIZE];

struct XYStruct {
  int x;
  int y;
  char z;
};

void taintTracking(int x) {
  int n;
  int *addr = &Buffer[0];
  scanf("%d", &n);
  addr += n;// expected-warning + {{tainted}}
  *addr = n; // expected-warning + {{tainted}}

  double tdiv = n / 30; // expected-warning+ {{tainted}}
  char *loc_cast = (char *) n; // expected-warning +{{tainted}}
  char tinc = tdiv++; // expected-warning + {{tainted}}
  int tincdec = (char)tinc--; // expected-warning+{{tainted}}

  // Tainted ptr arithmetic/array element address.
  int tprtarithmetic1 = *(addr+1); // expected-warning + {{tainted}}

  // Dereference.
  int *ptr;
  scanf("%p", &ptr);
  int ptrDeref = *ptr; // expected-warning + {{tainted}}
  int _ptrDeref = ptrDeref + 13; // expected-warning + {{tainted}}

  // Pointer arithmetic + dereferencing.
  // FIXME: We fail to propagate the taint here because RegionStore does not
  // handle ElementRegions with symbolic indexes.
  int addrDeref = *addr; // expected-warning + {{tainted}}
  int _addrDeref = addrDeref; // expected-warning + {{tainted}}

  // Tainted struct address, casts.
  struct XYStruct *xyPtr = 0;
  scanf("%p", &xyPtr);
  void *tXYStructPtr = xyPtr; // expected-warning + {{tainted}}
  struct XYStruct *xyPtrCopy = tXYStructPtr; // expected-warning + {{tainted}}
  int ptrtx = xyPtr->x;// expected-warning + {{tainted}}
  int ptrty = xyPtr->y;// expected-warning + {{tainted}}

  // Taint on fields of a struct.
  struct XYStruct xy = {2, 3, 11};
  scanf("%d", &xy.y);
  scanf("%d", &xy.x);
  int tx = xy.x; // expected-warning + {{tainted}}
  int ty = xy.y; // FIXME: This should be tainted as well.
  char ntz = xy.z;// no warning
  // Now, scanf scans both.
  scanf("%d %d", &xy.y, &xy.x);
  int ttx = xy.x; // expected-warning + {{tainted}}
  int tty = xy.y; // expected-warning + {{tainted}}
}

void BitwiseOp(int in, char inn, int zz) {
  // Taint on bitwise operations, integer to integer cast.
  int m;
  int x = 0;
  scanf("%d", &x);
  int y = (in << (x << in)) * 5;// expected-warning + {{tainted}}
  // The next line tests integer to integer cast.
  int z = y & inn; // expected-warning + {{tainted}}
  if (y == zz) { // expected-warning + {{tainted}}
    m = z | z;// expected-warning + {{tainted}}
  }
  else
    m = inn;
  int mm = m; // expected-warning 1 {{tainted}}
}

// Test getenv.
char *getenv(const char *name);
void getenvTest(char *home) {
  home = getenv("HOME"); // expected-warning + {{tainted}}
  if (home != 0) { // expected-warning + {{tainted}}
      char d = home[0]; // expected-warning + {{tainted}}
    }
}

int fscanfTest(void) {
  FILE *fp;
  char s[80];
  int t;

  // Check if stdin is treated as tainted.
  fscanf(stdin, "%s %d", s, &t);
  // Note, here, s is not tainted, but the data s points to is tainted.
  char *ts = s;
  char tss = s[0]; // expected-warning + {{tainted}}
  int tt = t; // expected-warning + {{tainted}}
  if((fp=fopen("test", "w")) == 0) // expected-warning + {{tainted}}
    return 1;
  fprintf(fp, "%s %d", s, t); // expected-warning + {{tainted}}
  fclose(fp); // expected-warning + {{tainted}}

  // Test fscanf and fopen.
  if((fp=fopen("test","r")) == 0) // expected-warning + {{tainted}}
    return 1;
  fscanf(fp, "%s%d", s, &t); // expected-warning + {{tainted}}
  fprintf(stdout, "%s %d", s, t); // expected-warning + {{tainted}}
  return 0;
}

// Check if we propagate taint from stdin when it's used in an assignment.
void stdinTest1(void) {
  int i;
  fscanf(stdin, "%d", &i);
  int j = i; // expected-warning + {{tainted}}
}
void stdinTest2(FILE *pIn) {
  FILE *p = stdin;
  FILE *pp = p;
  int ii;

  fscanf(pp, "%d", &ii);
  int jj = ii;// expected-warning + {{tainted}}

  fscanf(p, "%d", &ii);// expected-warning + {{tainted}}
  int jj2 = ii;// expected-warning + {{tainted}}

  ii = 3;
  int jj3 = ii;// no warning

  p = pIn;
  fscanf(p, "%d", &ii);
  int jj4 = ii;// no warning
}

void stdinTest3(void) {
  FILE **ppp = &stdin;
  int iii;
  fscanf(*ppp, "%d", &iii);
  int jjj = iii;// expected-warning + {{tainted}}
}

// Test that stdin does not get invalidated by calls.
void foo(void);
void stdinTest4(void) {
  int i;
  fscanf(stdin, "%d", &i);
  foo();
  int j = i; // expected-warning + {{tainted}}
}

int getw(FILE *);
void getwTest(void) {
  int i = getw(stdin); // expected-warning + {{tainted}}
}

typedef long ssize_t;
ssize_t getline(char ** __restrict, size_t * __restrict, FILE * __restrict);
int  printf(const char * __restrict, ...);
void free(void *ptr);
void getlineTest(void) {
  FILE *fp;
  char *line = 0;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, stdin)) != -1) {
    printf("%s", line); // expected-warning + {{tainted}}
  }
  free(line); // expected-warning + {{tainted}}
}

// Test propagation functions - the ones that propagate taint from arguments to
// return value, ptr arguments.

int atoi(const char *nptr);
long atol(const char *nptr);
long long atoll(const char *nptr);

void atoiTest(void) {
  char s[80];
  scanf("%s", s);
  int d = atoi(s); // expected-warning + {{tainted}}
  int td = d; // expected-warning + {{tainted}}

  long l = atol(s); // expected-warning + {{tainted}}
  int tl = l; // expected-warning + {{tainted}}

  long long ll = atoll(s); // expected-warning + {{tainted}}
  int tll = ll; // expected-warning + {{tainted}}

}

char *pointer1;
void *pointer2;
void noCrashTest(void) {
  if (!*pointer1) {
    __builtin___memcpy_chk(pointer2, pointer1, 0, 0); // no-crash
  }
}
