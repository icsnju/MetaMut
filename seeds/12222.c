// Check the basic reporting/warning and the application of constraints.
// RUN: %clang_analyze_cc1 %s \
// RUN:   -analyzer-checker=core \
// RUN:   -analyzer-checker=alpha.unix.StdCLibraryFunctions \
// RUN:   -analyzer-config alpha.unix.StdCLibraryFunctions:ModelPOSIX=true \
// RUN:   -analyzer-checker=debug.StdCLibraryFunctionsTester \
// RUN:   -analyzer-checker=debug.ExprInspection \
// RUN:   -triple x86_64-unknown-linux-gnu \
// RUN:   -verify=report

// Check the bugpath related to the reports.
// RUN: %clang_analyze_cc1 %s \
// RUN:   -analyzer-checker=core \
// RUN:   -analyzer-checker=alpha.unix.StdCLibraryFunctions \
// RUN:   -analyzer-config alpha.unix.StdCLibraryFunctions:ModelPOSIX=true \
// RUN:   -analyzer-checker=debug.StdCLibraryFunctionsTester \
// RUN:   -analyzer-checker=debug.ExprInspection \
// RUN:   -triple x86_64-unknown-linux-gnu \
// RUN:   -analyzer-output=text \
// RUN:   -verify=bugpath

typedef __SIZE_TYPE__ size_t;
#define __SSIZE_TYPE__                                                         \
  __typeof__(_Generic((__SIZE_TYPE__)0,                                        \
                      unsigned long long int : (long long int)0,               \
                      unsigned long int : (long int)0,                         \
                      unsigned int : (int)0,                                   \
                      unsigned short : (short)0))
typedef __SSIZE_TYPE__ ssize_t;
typedef struct {
  int x;
} FILE;

// do not use the default values for these constants to verify that this
// definition is found
#define EOF (-2)
#define AT_FDCWD (-101)

#ifdef __cplusplus
#define restrict /*restrict*/
#endif

int isascii(int);
int islower(int);
int isalpha(int);
int isalnum(int);
int isblank(int);
int ispunct(int);
int isupper(int);
int isgraph(int);
int isprint(int);
int isdigit(int);
int isspace(int);
int isxdigit(int);
int toupper(int);
int tolower(int);
int toascii(int);

int getc(FILE *);
int fgetc(FILE *);
int getchar(void);
size_t fread(void *restrict, size_t, size_t, FILE *restrict);
size_t fwrite(const void *restrict, size_t, size_t, FILE *restrict);
ssize_t read(int, void *, size_t);
ssize_t write(int, const void *, size_t);
ssize_t getline(char **restrict, size_t *restrict, FILE *restrict);
ssize_t getdelim(char **restrict, size_t *restrict, int, FILE *restrict);
char *getenv(const char *);

typedef int off_t;
typedef unsigned int mode_t;
typedef __WCHAR_TYPE__ wchar_t;
typedef unsigned int dev_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;
typedef unsigned long socklen_t;
typedef unsigned long int pthread_t;
typedef unsigned long time_t;
typedef unsigned long clockid_t;
typedef __INT64_TYPE__ off64_t;

typedef struct {
  int a;
} DIR;
struct stat {
  int a;
};
struct timespec { int x; };
struct timeval { int x; };
struct sockaddr;
struct sockaddr_at;
struct msghdr;
struct utimbuf;
struct itimerval;
typedef union {
  int x;
} pthread_cond_t;
typedef union {
  int x;
} pthread_attr_t;
typedef union {
  int x;
} pthread_mutex_t;
typedef union {
  int x;
} pthread_mutexattr_t;

FILE *fopen(const char *restrict pathname, const char *restrict mode);
FILE *tmpfile(void);
FILE *freopen(const char *restrict pathname, const char *restrict mode,
              FILE *restrict stream);
int fclose(FILE *stream);
int fseek(FILE *stream, long offset, int whence);
int fileno(FILE *stream);
long a64l(const char *str64);
char *l64a(long value);
int open(const char *path, int oflag, ...);
int openat(int fd, const char *path, int oflag, ...);
int access(const char *pathname, int amode);
int faccessat(int dirfd, const char *pathname, int mode, int flags);
int dup(int fildes);
int dup2(int fildes1, int filedes2);
int fdatasync(int fildes);
int fnmatch(const char *pattern, const char *string, int flags);
int fsync(int fildes);
int truncate(const char *path, off_t length);
int symlink(const char *oldpath, const char *newpath);
int symlinkat(const char *oldpath, int newdirfd, const char *newpath);
int lockf(int fd, int cmd, off_t len);
int creat(const char *pathname, mode_t mode);
unsigned int sleep(unsigned int seconds);
int dirfd(DIR *dirp);
unsigned int alarm(unsigned int seconds);
int closedir(DIR *dir);
char *strdup(const char *s);
char *strndup(const char *s, size_t n);
wchar_t *wcsdup(const wchar_t *s);
int mkstemp(char *template);
char *mkdtemp(char *template);
char *getcwd(char *buf, size_t size);
int mkdir(const char *pathname, mode_t mode);
int mkdirat(int dirfd, const char *pathname, mode_t mode);
int mknod(const char *pathname, mode_t mode, dev_t dev);
int mknodat(int dirfd, const char *pathname, mode_t mode, dev_t dev);
int chmod(const char *path, mode_t mode);
int fchmodat(int dirfd, const char *pathname, mode_t mode, int flags);
int fchmod(int fildes, mode_t mode);
int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags);
int chown(const char *path, uid_t owner, gid_t group);
int lchown(const char *path, uid_t owner, gid_t group);
int fchown(int fildes, uid_t owner, gid_t group);
int rmdir(const char *pathname);
int chdir(const char *path);
int link(const char *oldpath, const char *newpath);
int linkat(int fd1, const char *path1, int fd2, const char *path2, int flag);
int unlink(const char *pathname);
int unlinkat(int fd, const char *path, int flag);
int fstat(int fd, struct stat *statbuf);
int stat(const char *restrict path, struct stat *restrict buf);
int lstat(const char *restrict path, struct stat *restrict buf);
int fstatat(int fd, const char *restrict path, struct stat *restrict buf, int flag);
DIR *opendir(const char *name);
DIR *fdopendir(int fd);
int isatty(int fildes);
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
int close(int fildes);
long fpathconf(int fildes, int name);
long pathconf(const char *path, int name);
FILE *fdopen(int fd, const char *mode);
void rewinddir(DIR *dir);
void seekdir(DIR *dirp, long loc);
int rand_r(unsigned int *seedp);
int fileno(FILE *stream);
int fseeko(FILE *stream, off_t offset, int whence);
off_t ftello(FILE *stream);
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
void *mmap64(void *addr, size_t length, int prot, int flags, int fd, off64_t offset);
int pipe(int fildes[2]);
off_t lseek(int fildes, off_t offset, int whence);
ssize_t readlink(const char *restrict path, char *restrict buf, size_t bufsize);
ssize_t readlinkat(int fd, const char *restrict path, char *restrict buf, size_t bufsize);
int renameat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath);
char *realpath(const char *restrict file_name, char *restrict resolved_name);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int getopt(int argc, char *const argv[], const char *optstring);

// In some libc implementations, sockaddr parameter is a transparent
// union of the underlying sockaddr_ pointers instead of being a
// pointer to struct sockaddr.
// We match that with the joker Irrelevant type.
#define __SOCKADDR_ALLTYPES    \
  __SOCKADDR_ONETYPE(sockaddr) \
  __SOCKADDR_ONETYPE(sockaddr_at)
#define __SOCKADDR_ONETYPE(type) struct type *restrict __##type##__;
typedef union {
  __SOCKADDR_ALLTYPES
} __SOCKADDR_ARG __attribute__((__transparent_union__));
#undef __SOCKADDR_ONETYPE
#define __SOCKADDR_ONETYPE(type) const struct type *restrict __##type##__;
typedef union {
  __SOCKADDR_ALLTYPES
} __CONST_SOCKADDR_ARG __attribute__((__transparent_union__));
#undef __SOCKADDR_ONETYPE

int socket(int domain, int type, int protocol);
int accept(int socket, __SOCKADDR_ARG address, socklen_t *restrict address_len);
int bind(int socket, __CONST_SOCKADDR_ARG address, socklen_t address_len);
int getpeername(int socket, __SOCKADDR_ARG address, socklen_t *restrict address_len);
int getsockname(int socket, __SOCKADDR_ARG address, socklen_t *restrict address_len);
int connect(int socket, __CONST_SOCKADDR_ARG address, socklen_t address_len);
ssize_t recvfrom(int socket, void *restrict buffer, size_t length, int flags, __SOCKADDR_ARG address, socklen_t *restrict address_len);
ssize_t sendto(int socket, const void *message, size_t length, int flags, __CONST_SOCKADDR_ARG dest_addr, socklen_t dest_len);
int listen(int sockfd, int backlog);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
int getsockopt(int socket, int level, int option_name, void *restrict option_value, socklen_t *restrict option_len);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
int socketpair(int domain, int type, int protocol, int sv[2]);
int shutdown(int socket, int how);
int getnameinfo(const struct sockaddr *restrict sa, socklen_t salen, char *restrict node, socklen_t nodelen, char *restrict service, socklen_t servicelen, int flags);
int utime(const char *filename, struct utimbuf *buf);
int futimens(int fd, const struct timespec times[2]);
int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags);
int utimes(const char *filename, const struct timeval times[2]);
int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
struct tm *localtime(const time_t *tp);
struct tm *localtime_r(const time_t *restrict timer, struct tm *restrict result);
char *asctime_r(const struct tm *restrict tm, char *restrict buf);
char *ctime_r(const time_t *timep, char *buf);
struct tm *gmtime_r(const time_t *restrict timer, struct tm *restrict result);
struct tm *gmtime(const time_t *tp);
int clock_gettime(clockid_t clock_id, struct timespec *tp);
int getitimer(int which, struct itimerval *curr_value);

int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg);
int pthread_attr_destroy(pthread_attr_t *attr);
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_getstacksize(const pthread_attr_t *restrict attr, size_t *restrict stacksize);
int pthread_attr_getguardsize(const pthread_attr_t *restrict attr, size_t *restrict guardsize);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

void clang_analyzer_eval(int);
void clang_analyzer_warnIfReached();

int glob;

void test_alnum_concrete(int v) {
  int ret = isalnum(256); // \
  // report-warning{{The 1st argument to 'isalnum' is 256 but should be an unsigned char value or EOF}} \
  // bugpath-warning{{The 1st argument to 'isalnum' is 256 but should be an unsigned char value or EOF}} \
  // bugpath-note{{The 1st argument to 'isalnum' is 256 but should be an unsigned char value or EOF}}
  (void)ret;
}

void test_alnum_symbolic(int x) {
  int ret = isalnum(x);
  (void)ret;

  clang_analyzer_eval(EOF <= x && x <= 255); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{Left side of '&&' is true}} \
  // bugpath-note{{'x' is <= 255}}
}

void test_alnum_symbolic2(int x) {
  if (x > 255) { // \
    // bugpath-note{{Assuming 'x' is > 255}} \
    // bugpath-note{{Taking true branch}}

    int ret = isalnum(x); // \
    // report-warning{{The 1st argument to 'isalnum' is >= 256 but should be an unsigned char value or EOF}} \
    // bugpath-warning{{The 1st argument to 'isalnum' is >= 256 but should be an unsigned char value or EOF}} \
    // bugpath-note{{The 1st argument to 'isalnum' is >= 256 but should be an unsigned char value or EOF}}

    (void)ret;
  }
}

void test_toupper_concrete(int v) {
  int ret = toupper(256); // \
  // report-warning{{The 1st argument to 'toupper' is 256 but should be an unsigned char value or EOF}} \
  // bugpath-warning{{The 1st argument to 'toupper' is 256 but should be an unsigned char value or EOF}} \
  // bugpath-note{{The 1st argument to 'toupper' is 256 but should be an unsigned char value or EOF}}
  (void)ret;
}

void test_toupper_symbolic(int x) {
  int ret = toupper(x);
  (void)ret;

  clang_analyzer_eval(EOF <= x && x <= 255); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{Left side of '&&' is true}} \
  // bugpath-note{{'x' is <= 255}}
}

void test_toupper_symbolic2(int x) {
  if (x > 255) { // \
    // bugpath-note{{Assuming 'x' is > 255}} \
    // bugpath-note{{Taking true branch}}

    int ret = toupper(x); // \
    // report-warning{{The 1st argument to 'toupper' is >= 256 but should be an unsigned char value or EOF}} \
    // bugpath-warning{{The 1st argument to 'toupper' is >= 256 but should be an unsigned char value or EOF}} \
    // bugpath-note{{The 1st argument to 'toupper' is >= 256 but should be an unsigned char value or EOF}}

    (void)ret;
  }
}

void test_tolower_concrete(int v) {
  int ret = tolower(256); // \
  // report-warning{{The 1st argument to 'tolower' is 256 but should be an unsigned char value or EOF}} \
  // bugpath-warning{{The 1st argument to 'tolower' is 256 but should be an unsigned char value or EOF}} \
  // bugpath-note{{The 1st argument to 'tolower' is 256 but should be an unsigned char value or EOF}}
  (void)ret;
}

void test_tolower_symbolic(int x) {
  int ret = tolower(x);
  (void)ret;

  clang_analyzer_eval(EOF <= x && x <= 255); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{Left side of '&&' is true}} \
  // bugpath-note{{'x' is <= 255}}
}

void test_tolower_symbolic2(int x) {
  if (x > 255) { // \
    // bugpath-note{{Assuming 'x' is > 255}} \
    // bugpath-note{{Taking true branch}}

    int ret = tolower(x); // \
    // report-warning{{The 1st argument to 'tolower' is >= 256 but should be an unsigned char value or EOF}} \
    // bugpath-warning{{The 1st argument to 'tolower' is >= 256 but should be an unsigned char value or EOF}} \
    // bugpath-note{{The 1st argument to 'tolower' is >= 256 but should be an unsigned char value or EOF}}

    (void)ret;
  }
}

void test_toascii_concrete(int v) {
  int ret = toascii(256); // \
  // report-warning{{The 1st argument to 'toascii' is 256 but should be an unsigned char value or EOF}} \
  // bugpath-warning{{The 1st argument to 'toascii' is 256 but should be an unsigned char value or EOF}} \
  // bugpath-note{{The 1st argument to 'toascii' is 256 but should be an unsigned char value or EOF}}
  (void)ret;
}

void test_toascii_symbolic(int x) {
  int ret = toascii(x);
  (void)ret;

  clang_analyzer_eval(EOF <= x && x <= 255); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{Left side of '&&' is true}} \
  // bugpath-note{{'x' is <= 255}}
}

void test_toascii_symbolic2(int x) {
  if (x > 255) { // \
    // bugpath-note{{Assuming 'x' is > 255}} \
    // bugpath-note{{Taking true branch}}

    int ret = toascii(x); // \
    // report-warning{{The 1st argument to 'toascii' is >= 256 but should be an unsigned char value or EOF}} \
    // bugpath-warning{{The 1st argument to 'toascii' is >= 256 but should be an unsigned char value or EOF}} \
    // bugpath-note{{The 1st argument to 'toascii' is >= 256 but should be an unsigned char value or EOF}}

    (void)ret;
  }
}

void test_notnull_concrete(FILE *fp) {
  fread(0, sizeof(int), 10, fp); // \
  // report-warning{{The 1st argument to 'fread' is NULL but should not be NULL}} \
  // bugpath-warning{{The 1st argument to 'fread' is NULL but should not be NULL}} \
  // bugpath-note{{The 1st argument to 'fread' is NULL but should not be NULL}}
}
void test_notnull_symbolic(FILE *fp, int *buf) {
  fread(buf, sizeof(int), 10, fp);
  clang_analyzer_eval(buf != 0); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{'buf' is not equal to null}}
}
void test_notnull_symbolic2(FILE *fp, int *buf) {
  if (!buf)                          // bugpath-note{{Assuming 'buf' is null}} \
            // bugpath-note{{Taking true branch}}
    fread(buf, sizeof(int), 10, fp); // \
    // report-warning{{The 1st argument to 'fread' is NULL but should not be NULL}} \
    // bugpath-warning{{The 1st argument to 'fread' is NULL but should not be NULL}} \
    // bugpath-note{{The 1st argument to 'fread' is NULL but should not be NULL}}
}

int __not_null_buffer(void *, int, int);

void test_notnull_buffer_1(void *buf) {
  __not_null_buffer(buf, 0, 1);
  clang_analyzer_eval(buf != 0); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // report-warning{{FALSE}} \
  // bugpath-warning{{FALSE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{FALSE}} \
  // bugpath-note{{Assuming 'buf' is equal to null}} \
  // bugpath-note{{Assuming 'buf' is not equal to null}}
}

void test_notnull_buffer_2(void *buf) {
  __not_null_buffer(buf, 1, 0);
  clang_analyzer_eval(buf != 0); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // report-warning{{FALSE}} \
  // bugpath-warning{{FALSE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{FALSE}} \
  // bugpath-note{{Assuming 'buf' is equal to null}} \
  // bugpath-note{{Assuming 'buf' is not equal to null}}
}

void test_notnull_buffer_3(void *buf) {
  __not_null_buffer(buf, 1, 1);
  clang_analyzer_eval(buf != 0); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{'buf' is not equal to null}}
}

void test_no_node_after_bug(FILE *fp, size_t size, size_t n, void *buf) {
  if (fp) // \
  // bugpath-note{{Assuming 'fp' is null}} \
  // bugpath-note{{Taking false branch}}
    return;
  size_t ret = fread(buf, size, n, fp); // \
  // report-warning{{The 4th argument to 'fread' is NULL but should not be NULL}} \
  // bugpath-warning{{The 4th argument to 'fread' is NULL but should not be NULL}} \
  // bugpath-note{{The 4th argument to 'fread' is NULL but should not be NULL}}
  clang_analyzer_warnIfReached(); // not reachable
}

// This is one test case for the ARR38-C SEI-CERT rule.
void ARR38_C_F(FILE *file) {
  enum { BUFFER_SIZE = 1024 };
  wchar_t wbuf[BUFFER_SIZE]; // bugpath-note{{'wbuf' initialized here}}

  const size_t size = sizeof(*wbuf);   // bugpath-note{{'size' initialized to}}
  const size_t nitems = sizeof(wbuf);  // bugpath-note{{'nitems' initialized to}}

  // The 3rd parameter should be the number of elements to read, not
  // the size in bytes.
  fread(wbuf, size, nitems, file); // \
  // report-warning{{The 1st argument to 'fread' is a buffer with size 4096 but should be a buffer with size equal to or greater than the value of the 2nd argument (which is 4) times the 3rd argument (which is 4096)}} \
  // bugpath-warning{{The 1st argument to 'fread' is a buffer with size 4096 but should be a buffer with size equal to or greater than the value of the 2nd argument (which is 4) times the 3rd argument (which is 4096)}} \
  // bugpath-note{{The 1st argument to 'fread' is a buffer with size 4096 but should be a buffer with size equal to or greater than the value of the 2nd argument (which is 4) times the 3rd argument (which is 4096)}}
}

int __two_constrained_args(int, int);
void test_constraints_on_multiple_args(int x, int y) {
  // State split should not happen here. I.e. x == 1 should not be evaluated
  // FALSE.
  __two_constrained_args(x, y);
  //NOTE! Because of the second `clang_analyzer_eval` call we have two bug
  clang_analyzer_eval(x == 1); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}}
  clang_analyzer_eval(y == 1); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}}
}

int __arg_constrained_twice(int);
void test_multiple_constraints_on_same_arg(int x) {
  __arg_constrained_twice(x);
  clang_analyzer_eval(x < 1 || x > 2); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{Assuming 'x' is < 1}} \
  // bugpath-note{{Left side of '||' is true}}
}

int __variadic(void *stream, const char *format, ...);
void test_arg_constraint_on_variadic_fun(void) {
  __variadic(0, "%d%d", 1, 2); // \
  // report-warning{{The 1st argument to '__variadic' is NULL but should not be NULL}} \
  // bugpath-warning{{The 1st argument to '__variadic' is NULL but should not be NULL}} \
  // bugpath-note{{The 1st argument to '__variadic' is NULL but should not be NULL}}
}

int __buf_size_arg_constraint(const void *, size_t);
void test_buf_size_concrete(void) {
  char buf[3];                       // bugpath-note{{'buf' initialized here}}
  __buf_size_arg_constraint(buf, 4); // \
  // report-warning{{The 1st argument to '__buf_size_arg_constraint' is a buffer with size 3 but should be a buffer with size equal to or greater than the value of the 2nd argument (which is 4)}} \
  // bugpath-warning{{The 1st argument to '__buf_size_arg_constraint' is a buffer with size 3 but should be a buffer with size equal to or greater than the value of the 2nd argument (which is 4)}} \
  // bugpath-note{{The 1st argument to '__buf_size_arg_constraint' is a buffer with size 3 but should be a buffer with size equal to or greater than the value of the 2nd argument (which is 4)}}
}
void test_buf_size_symbolic(int s) {
  char buf[3];
  __buf_size_arg_constraint(buf, s);
  clang_analyzer_eval(s <= 3); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{'s' is <= 3}}
}
void test_buf_size_symbolic_and_offset(int s) {
  char buf[3];
  __buf_size_arg_constraint(buf + 1, s);
  clang_analyzer_eval(s <= 2); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}} \
  // bugpath-note{{'s' is <= 2}}
}

int __buf_size_arg_constraint_mul(const void *, size_t, size_t);
void test_buf_size_concrete_with_multiplication(void) {
  short buf[3];                                         // bugpath-note{{'buf' initialized here}}
  __buf_size_arg_constraint_mul(buf, 4, sizeof(short)); // \
  // report-warning{{The 1st argument to '__buf_size_arg_constraint_mul' is a buffer with size 6 but should be a buffer with size equal to or greater than the value of the 2nd argument (which is 4) times the 3rd argument (which is 2)}} \
  // bugpath-warning{{The 1st argument to '__buf_size_arg_constraint_mul' is a buffer with size 6 but should be a buffer with size equal to or greater than the value of the 2nd argument (which is 4) times the 3rd argument (which is 2)}} \
  // bugpath-note{{The 1st argument to '__buf_size_arg_constraint_mul' is a buffer with size 6 but should be a buffer with size equal to or greater than the value of the 2nd argument (which is 4) times the 3rd argument (which is 2)}}
}
void test_buf_size_symbolic_with_multiplication(size_t s) {
  short buf[3];
  __buf_size_arg_constraint_mul(buf, s, sizeof(short));
  clang_analyzer_eval(s * sizeof(short) <= 6); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}}
}
void test_buf_size_symbolic_and_offset_with_multiplication(size_t s) {
  short buf[3];
  __buf_size_arg_constraint_mul(buf + 1, s, sizeof(short));
  clang_analyzer_eval(s * sizeof(short) <= 4); // \
  // report-warning{{TRUE}} \
  // bugpath-warning{{TRUE}} \
  // bugpath-note{{TRUE}}
}

// The minimum buffer size for this function is set to 10.
int __buf_size_arg_constraint_concrete(const void *);
void test_min_buf_size(void) {
  char buf[9];// bugpath-note{{'buf' initialized here}}
  __buf_size_arg_constraint_concrete(buf); // \
  // report-warning{{The 1st argument to '__buf_size_arg_constraint_concrete' is a buffer with size 9 but should be a buffer with size equal to or greater than 10}} \
  // bugpath-warning{{The 1st argument to '__buf_size_arg_constraint_concrete' is a buffer with size 9 but should be a buffer with size equal to or greater than 10}} \
  // bugpath-note{{The 1st argument to '__buf_size_arg_constraint_concrete' is a buffer with size 9 but should be a buffer with size equal to or greater than 10}}
}

void test_file_fd_at_functions() {
  (void)linkat(-22, "from", AT_FDCWD, "to", 0); // \
  // report-warning{{The 1st argument to 'linkat' is -22 but should be a valid file descriptor or AT_FDCWD}} \
  // bugpath-warning{{The 1st argument to 'linkat' is -22 but should be a valid file descriptor or AT_FDCWD}} \
  // bugpath-note{{The 1st argument to 'linkat' is -22 but should be a valid file descriptor or AT_FDCWD}}

  // no warning for these functions if the AT_FDCWD value is used
  (void)openat(AT_FDCWD, "path", 0);
  (void)linkat(AT_FDCWD, "from", AT_FDCWD, "to", 0);
  (void)faccessat(AT_FDCWD, "path", 0, 0);
  (void)symlinkat("oldpath", AT_FDCWD, "newpath");
  (void)mkdirat(AT_FDCWD, "path", 0);
  (void)mknodat(AT_FDCWD, "path", 0, 0);
  (void)fchmodat(AT_FDCWD, "path", 0, 0);
  (void)fchownat(AT_FDCWD, "path", 0, 0, 0);
  (void)linkat(AT_FDCWD, "oldpath", AT_FDCWD, "newpath", 0);
  (void)unlinkat(AT_FDCWD, "newpath", 0);
  struct stat St;
  (void)fstatat(AT_FDCWD, "newpath", &St, 0);
  char Buf[10];
  (void)readlinkat(AT_FDCWD, "newpath", Buf, 10);
  (void)renameat(AT_FDCWD, "oldpath", AT_FDCWD, "newpath");
}
